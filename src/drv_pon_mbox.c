/*****************************************************************************
 *
 * Copyright (c) 2020 - 2025 MaxLinear, Inc.
 * Copyright (c) 2017 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/
/*
 * This driver drives the PON IP mailbox. It has a NetLink interface on which
 * it receives messages from user space and the driver puts them into the
 * mailbox so they can be received by the firmware, the driver can also receive
 * messages from the firmware and send them back to the user space application.
 * In addition it also loads the firmware into the PON IP and has some debug
 * functions to read and write register of the PON IP host interface.
 */

/**
 * \file drv_pon_mbox.c
 *
 */

#define DRV_NAME "pon_mbox"

/* define prefix for pr functions */
#define pr_fmt(fmt) DRV_NAME ": " fmt

#include <linux/bits.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/firmware.h>
#include <linux/hashtable.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/iopoll.h>
#include <linux/kernel.h>
#include <linux/math64.h>
#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of_device.h>
#include <linux/of.h>
#include <linux/pinctrl/consumer.h>
#include <linux/platform_device.h>
#include <linux/random.h>
#include <linux/regmap.h>
#include <linux/reset.h>
#include <linux/spi/spi.h>
#include <linux/sys_soc.h>
#include <linux/time64.h>
#include <linux/uidgid.h>
#include <linux/version.h>
#include <linux/wait.h>

#include <net/netlink.h>
#include <net/genetlink.h>
#include <net/sock.h>

#include <pon/pon_ip_msg.h>
#include <pon/pon_mbox_ikm.h>

/* Define lookup table to find SoC revision. */
#define PRX_REV_B(x, d) \
	{ .family = "PRX", .revision = "B"#x, .data = (void *)d }
#define URX_REV_B(x, d) \
	{ .family = "LGM", .revision = "B"#x, .data = (void *)d }
#define URX_REV_C(x, d) \
	{ .family = "LGM", .revision = "C"#x, .data = (void *)d }

static const struct soc_device_attribute pon_soc_rev[] = {
	PRX_REV_B(0, PONFW_HW_VERSION_VERSION_FLM_B0_FLM_B1),
	PRX_REV_B(1, PONFW_HW_VERSION_VERSION_FLM_B0_FLM_B1),
	PRX_REV_B(2, PONFW_HW_VERSION_VERSION_FLM_B2_LGM_A),
	PRX_REV_B(3, PONFW_HW_VERSION_VERSION_FLM_B3),
	URX_REV_B(0, PONFW_HW_VERSION_VERSION_LGM_B0),
	URX_REV_B(1, PONFW_HW_VERSION_VERSION_LGM_B1),
	URX_REV_C(0, PONFW_HW_VERSION_VERSION_LGM_C0),
	{ /* sentinel */ }
};

static unsigned int get_soc_rev(void)
{
	const struct soc_device_attribute *soc_info;

	soc_info = soc_device_match(pon_soc_rev);
	if (soc_info)
		return (unsigned int)(uintptr_t)soc_info->data;

	/* Return a dummy/default value, if nothing was found. */
	return 0;
}

#ifdef DEBUG_MBOX_RESPONSE_TIME
#include <linux/jiffies.h>
#endif

#if !defined(SKIP_TEP) && !defined(CONFIG_X86_INTEL_LGM) && \
	!defined(CONFIG_SOC_LGM)
#define CONFIG_PON_FW_DOWNLOAD 1
#include <secure_services.h>
#endif

#include <asm/byteorder.h>

#include <drv_pon_mbox_counters.h>
#include <drv_pon_mbox_counters_update.h>
#include <drv_pon_mbox_counters_autoupdate.h>
#include <drv_pon_mbox_serdes.h>

#include "drv_pon_mbox.h"

/** \addtogroup PON_MBOX_REFERENCE
 *  @{
 */

/** what string support, version string */
static const char pon_mbox_drv_whatversion[] __used =
	"@(#)MaxLinear PON Mailbox driver, version "
	__stringify(PACKAGE_VERSION);

#define CHIPTOP_IFMUX_CFG				0x120
#define SECTOP_PON_PPM					0x148
#define URX_CHIPTOP_PON_CR				0x194

/** List of default firmware file names */
/** FW supporting G-PON on type A */
#define PON_MBOX_FW_GPON_NAME_A "prx_gpon_fw_a.bin"
/** FW supporting G-PON on type B */
#define PON_MBOX_FW_GPON_NAME_B "prx_gpon_fw_b.bin"
/** FW supporting G-PON on URX800 type A */
#define PON_MBOX_FW_GPON_NAME_URX_A "urx_gpon_fw_a.bin"
/** FW supporting G-PON on URX800 type B */
#define PON_MBOX_FW_GPON_NAME_URX_B "urx_gpon_fw_b.bin"
/** FW supporting G-PON on URX800 type C */
#define PON_MBOX_FW_GPON_NAME_URX_C "urx_gpon_fw_c.bin"
/** FW supporting G-PON on TOPAZ type U */
#define PON_MBOX_FW_GPON_NAME_TPZ "tpz_gpon_fw.bin"
/** FW supporting XG-SPON, XG-PON and NG-PON2 on type A */
#define PON_MBOX_FW_XPON_NAME_A "prx_xpon_fw_a.bin"
/** FW supporting XG-SPON, XG-PON and NG-PON2 on type B */
#define PON_MBOX_FW_XPON_NAME_B "prx_xpon_fw_b.bin"
/** FW supporting XGS-PON, XG-PON and NG-PON2 on URX800 type A */
#define PON_MBOX_FW_XPON_NAME_URX_A "urx_xpon_fw_a.bin"
/** FW supporting XGS-PON, XG-PON and NG-PON2 on URX800 type B */
#define PON_MBOX_FW_XPON_NAME_URX_B "urx_xpon_fw_b.bin"
/** FW supporting XGS-PON, XG-PON and NG-PON2 on URX800 type C */
#define PON_MBOX_FW_XPON_NAME_URX_C "urx_xpon_fw_c.bin"
/** FW supporting XGS-PON, XG-PON and NG-PON2 on TOPAZ type U */
#define PON_MBOX_FW_XPON_NAME_TPZ "tpz_xpon_fw.bin"

/** Maximum number of concurrent alarms.
 *  If number of reported alarms is bigger than 32 it means that system is in
 *  an unusable state.
 */
#define MAX_ALARMS_NUMBER 32

/** Maximal PON FW download retries in case of an error */
#define PON_FW_DOWNLOAD_MAX_RETRIES 2

/** SerDes bit error polling interval
 * The SerDes bit error counter must be polled within a time of 400 ms to
 * avoid a counter overrun at a bit error rate of 10e-3.
 * For higher bit error rates the interval must be reduced.
 * If only lower bit error rates shall be measured, the interval time can be
 * increased to save power.
 */
#define SERDES_BIT_ERROR_COUNTER_POLLING_INTERVAL 400

/** Header used for SPI communication */
#ifdef __LITTLE_ENDIAN_BITFIELD
struct pon_mbox_spih {
	u32 len : 15;
	u32 autoinc : 1;
	u32 write : 1;
	u32 read : 1;
	u32 addr : 14;
} __packed;
#elif defined(__BIG_ENDIAN_BITFIELD)
struct pon_mbox_spih {
	u32 addr : 14;
	u32 read : 1;
	u32 write : 1;
	u32 autoinc : 1;
	u32 len : 15;
} __packed;
#endif

/** SPI header union */
union pon_mbox_spi_header {
	struct pon_mbox_spih field;
	u32 full;
} __packed;

/** Flag definitions used in conjunction with PON Host register write
 *  functions. Used to flag the action to be performed and trigger
 *  byte order conversion where required.
 */
/** WRITING FW binary data during FW download */
#define PON_WR_FLAG_FWDL 0
/** Regular write access  */
#define PON_WR_FLAG_REGULAR 1

/** global buffer for network byte order conversion */
static u32 buf_byte_order_conv[256];

#ifdef DEBUG_MBOX_RESPONSE_TIME
/** For calculation of time between IRQ Wake and thread handling */
static unsigned long debug_irq_jiffies_start;
#endif

#define CHECK_FOR_NETLINK_RESPONSE_ERR(pon, err) \
	do { \
		if (err) \
			dev_err((pon)->dev, \
				"%s: cannot send netlink response: err: %i\n", \
				__func__, err); \
	} while (0)

#define CNT_ENCODE_U64(COUNTER, DEST, PAD) \
	do { \
		err = nla_put_u64_64bit(skb, PON_MBOX_A_CNT_##COUNTER, \
				pon_output->DEST, PON_MBOX_A_CNT_##PAD); \
		if (err) \
			goto cancel; \
	} while (0)

#define CNT_ENCODE_U32(PARAM, DEST) \
	do { \
		err = nla_put_u32(skb, PON_MBOX_A_CNT_##PARAM, \
				pon_output->DEST); \
		if (err) \
			goto cancel; \
	} while (0)

static unsigned long counters_update_timer = PON_COUNTERS_DEFAULT_UPDATE_TIME;
module_param(counters_update_timer, ulong, 0644);
MODULE_PARM_DESC(counters_update_timer,
		 "time in seconds between counter updates, 0 - deactivate");

#if (KERNEL_VERSION(6, 2, 0) > LINUX_VERSION_CODE)
/* in 6.2 "genl_split_ops" was added, fallback to "genl_ops" for older kernel */
#define genl_split_ops genl_ops
#endif

/*
 * Reference to the internal driver data structure.
 * TODO: Make it possible to use multiple PON IP instances in one system. The
 * NetLink message has to get extended for that and we have to find the
 * correct device when we get a NetLink message.
 */
struct pon_mbox *pon_mbox_dev;

/* Register class to make driver/devices visible in sysfs */
static struct class *pon_mbox_class;

/**
 * Next NetLink sequence number to be used for events received from the
 * firmware.
 */
static u32 pon_mbox_seq;

/**
 * PON mailbox snd_portid and snd_seq stored to send ACK back to ponmbox after
 * fake event had been called.
 */
static u32 fake_event_msg_snd_portid;
static u32 fake_event_msg_snd_seq;

static int (*pon_mbox_mode_change_callback_func)(enum pon_mode);

static const struct nla_policy pon_mbox_genl_policy[PON_MBOX_A_MAX + 1] = {
	[PON_MBOX_A_READ_WRITE] = { .type = NLA_U8 },
	[PON_MBOX_A_COMMAND] = { .type = NLA_U16 },
	[PON_MBOX_A_ACK] = { .type = NLA_U8 },
	[PON_MBOX_A_DATA] = { .type = NLA_BINARY },
	[PON_MBOX_A_REG] = { .type = NLA_U8 },
	[PON_MBOX_A_REG_VAL] = { .type = NLA_U32 },
	[PON_MBOX_A_FLAGS] = { .type = NLA_U32 },
	[PON_MBOX_A_MODE] = { .type = NLA_U8 },
};

static const struct nla_policy counters_genl_policy[PON_MBOX_A_MAX + 1] = {
	[PON_MBOX_D_ALLOC_IDX] = { .type = NLA_U8 },
	[PON_MBOX_D_GEM_IDX] = { .type = NLA_U8 },
	[PON_MBOX_D_DSWLCH_ID] = { .type = NLA_U8 },
};

static const struct nla_policy link_request_genl_policy[PON_MBOX_A_MAX + 1] = {
	[PON_MBOX_LR_ACTION] = { .type = NLA_U8 },
	[PON_MBOX_LR_MACSA] = { .type = NLA_U64 },
};

static const struct nla_policy pin_config_genl_policy[PON_MBOX_A_MAX + 1] = {
	[PON_MBOX_A_PIN_ID] = { .type = NLA_U8 },
	[PON_MBOX_A_PIN_STATUS] = { .type = NLA_U8 },
};

static const struct nla_policy uart_config_genl_policy[PON_MBOX_A_MAX + 1] = {
	[PON_MBOX_UART_MODE] = { .type = NLA_U8 },
};

static const struct nla_policy user_mngmt_genlt_policy[PON_MBOX_A_MAX + 1] = {
	[PON_MBOX_A_USER_MNGMT_REVOKE_GRANT] = { .type = NLA_U8 },
	[PON_MBOX_A_USER_MNGMT_CMD_GROUP] = { .type = NLA_U16 },
	[PON_MBOX_A_USER_MNGMT_USER] = { .type = NLA_U32 },
};

static const struct nla_policy empty_read_policy[PON_MBOX_A_MAX + 1] = {};

static const struct nla_policy
	serdes_config_genl_policy[PON_MBOX_SRDS_MAX + 1] = {
		[PON_MBOX_SRDS_TX_EQ_MAIN] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_TX_EQ_POST] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_TX_EQ_PRE] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_VBOOST_EN] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_VBOOST_LVL] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_IBOOST_LVL] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_RX_ADAPT_AFE_EN] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_RX_ADAPT_DFE_EN] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_RX_ADAPT_CONT] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_RX_ADAPT_EN] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_RX_EQ_ATT_LVL] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_RX_EQ_ADAPT_MODE] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_RX_EQ_ADAPT_SEL] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_RX_EQ_CTLE_BOOST] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_RX_VCO_TEMP_COMP_EN] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_RX_VCO_STEP_CTRL] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_RX_VCO_FRQBAND] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_RX_MISC] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_RX_DELTA_IQ] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_RX_MARGIN_IQ] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_EQ_CTLE_POLE] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_EQ_DFE_TAP1] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_EQ_DFE_BYPASS] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_EQ_VGA1_GAIN] = { .type = NLA_U8 },
		[PON_MBOX_SRDS_EQ_VGA2_GAIN] = { .type = NLA_U8 },
	};

static const char *serdes_config_params[PON_MBOX_SRDS_MAX + 1] = {
	[PON_MBOX_SRDS_TX_EQ_MAIN] = "tx_eq_main",
	[PON_MBOX_SRDS_TX_EQ_POST] = "tx_eq_post",
	[PON_MBOX_SRDS_TX_EQ_PRE] = "tx_eq_pre",
	[PON_MBOX_SRDS_VBOOST_EN] = "vboost_en",
	[PON_MBOX_SRDS_VBOOST_LVL] = "vboost_lvl",
	[PON_MBOX_SRDS_IBOOST_LVL] = "iboost_lvl",
	[PON_MBOX_SRDS_RX_ADAPT_AFE_EN] = "rx_adapt_afe_en",
	[PON_MBOX_SRDS_RX_ADAPT_DFE_EN] = "rx_adapt_dfe_en",
	[PON_MBOX_SRDS_RX_ADAPT_CONT] = "rx_adapt_cont",
	[PON_MBOX_SRDS_RX_ADAPT_EN] = "rx_adapt_en",
	[PON_MBOX_SRDS_RX_EQ_ATT_LVL] = "rx_eq_att_lvl",
	[PON_MBOX_SRDS_RX_EQ_ADAPT_MODE] = "rx_eq_adapt_mode",
	[PON_MBOX_SRDS_RX_EQ_ADAPT_SEL] = "rx_eq_adapt_sel",
	[PON_MBOX_SRDS_RX_EQ_CTLE_BOOST] = "rx_eq_ctle_boost",
	[PON_MBOX_SRDS_RX_VCO_TEMP_COMP_EN] = "rx_vco_temp_comp_en",
	[PON_MBOX_SRDS_RX_VCO_STEP_CTRL] = "rx_vco_step_ctrl",
	[PON_MBOX_SRDS_RX_VCO_FRQBAND] = "rx_vco_frqband",
	[PON_MBOX_SRDS_RX_MISC] = "rx_misc",
	[PON_MBOX_SRDS_RX_DELTA_IQ] = "rx_delta_iq",
	[PON_MBOX_SRDS_RX_MARGIN_IQ] = "rx_margin_iq",
	[PON_MBOX_SRDS_EQ_CTLE_POLE] = "rx_eq_ctle_pole",
	[PON_MBOX_SRDS_EQ_DFE_TAP1] = "rx_eq_dfe_tap1",
	[PON_MBOX_SRDS_EQ_DFE_BYPASS] = "rx_eq_dfe_bypass",
	[PON_MBOX_SRDS_EQ_VGA1_GAIN] = "rx_eq_vga1_gain",
	[PON_MBOX_SRDS_EQ_VGA2_GAIN] = "rx_eq_vga2_gain",
};

static const struct nla_policy lt_config_genl_policy[PON_MBOX_A_MAX + 1] = {
	[PON_MBOX_LT_MODE] = { .type = NLA_U8 },
	[PON_MBOX_LT_POWER_SAVE] = { .type = NLA_U8 },
};

static const struct nla_policy iop_config_genl_policy[PON_MBOX_A_MAX + 1] = {
	[PON_MBOX_IOP_MSK] = { .type = NLA_U32 },
};

static const struct nla_policy biterror_cnt_policy[PON_MBOX_A_MAX + 1] = {
	[PON_MBOX_A_BITERR_CNT] = { .type = NLA_U64 },
	[PON_MBOX_A_BITERR_TIME] = { .type = NLA_U32 },
};

static const struct nla_policy pon_mode_genl_policy[PON_MBOX_A_MAX + 1] = {
	[PON_MBOX_A_PON_MODE] = { .type = NLA_U8 },
};

static const struct nla_policy
	cnt_twdm_wlchid_genl_policy[PON_MBOX_CNT_TWDM_WLCHID_MAX + 1] = {
		[PON_MBOX_CNT_TWDM_WLCHID_DS] = { .type = NLA_U8 },
		[PON_MBOX_CNT_TWDM_WLCHID_US] = { .type = NLA_U8 },
	};

/** NetLink Multicast group to send events to. */
static struct genl_multicast_group pon_mbox_genl_groups[] = {
	{
		.name = "msg",
	},
};

/** User permission list for each command group */
static struct pon_mbox_user_perm pon_mbox_perm_list[] = {
	[PON_MBOX_PERM_SYNCE] = {
		.uid_list[0 ... PON_MBOX_MAX_CMD_USERS - 1] = INVALID_UID
	},
	[PON_MBOX_PERM_BC_KEY] = {
		.uid_list[0 ... PON_MBOX_MAX_CMD_USERS - 1] = INVALID_UID
	},
};

static int pon_mbox_send_nl_response(struct pon_mbox *pon, u32 snd_portid,
				     u32 snd_seq, struct net *net,
				     struct pon_msg_header *header, char *msg,
				     u32 flags);

typedef int (pon_mbox_nl_encode_fn)(struct sk_buff *skb,
				    void *msg,
				    unsigned int size);

static int pon_mbox_send_nl_response_encoded(struct pon_mbox *pon,
					     u32 snd_portid,
					     u32 snd_seq, struct net *net,
					     struct pon_msg_header *header,
					     char *msg,
					     u32 flags,
					     pon_mbox_nl_encode_fn *encode_fn);

static int pon_mbox_genl_reg_write(struct sk_buff *skb, struct genl_info *info);
static int pon_mbox_genl_reg_read(struct sk_buff *skb, struct genl_info *info);
static int pon_mbox_genl_lt_config(struct sk_buff *skb, struct genl_info *info);
static int pon_mbox_genl_iop_config(struct sk_buff *skb,
				    struct genl_info *info);
static int pon_mbox_genl_serdes_config(struct sk_buff *skb,
				       struct genl_info *info);
static int pon_mbox_genl_serdes_config_read(struct sk_buff *skb,
					    struct genl_info *info);
static int pon_mbox_genl_pin_config(struct sk_buff *skb,
				    struct genl_info *info);
static int pon_mbox_genl_uart_config(struct sk_buff *skb,
				     struct genl_info *info);
static int pon_mbox_user_mngmt(struct sk_buff *skb, struct genl_info *info);

static int pon_mbox_genl_biterr_start(struct sk_buff *skb,
				      struct genl_info *info);
static int pon_mbox_genl_biterr_stop(struct sk_buff *skb,
				     struct genl_info *info);
static int pon_mbox_genl_biterr_read(struct sk_buff *skb,
				     struct genl_info *info);
static int pon_mbox_genl_mode_read(struct sk_buff *skb,
				   struct genl_info *info);
static int pon_mbox_genl_dp_config(struct sk_buff *skb,
				   struct genl_info *info);
static int pon_mbox_genl_cnt_twdm_wlchid_set(struct sk_buff *skb,
					     struct genl_info *info);

/** Read a 32 bit value from a PON IP HOST register */
static int pon_mbox_read32(struct pon_mbox *pon, int reg, u32 *buf)
{
	int ret;

	ret = pon->read(pon, reg, buf, sizeof(*buf));
	if (ret)
		dev_err(pon->dev, "problem reading register (0x%x): %i\n",
			reg, ret);
	return ret;
}

/** Write a 32 bit value to a PON IP HOST register */
static int pon_mbox_write32(struct pon_mbox *pon, int reg, u32 buf)
{
	int ret;

	ret = pon->write(pon, reg, &buf, sizeof(buf), PON_WR_FLAG_REGULAR);
	if (ret)
		dev_err(pon->dev, "problem writing register (0x%x): %i\n",
			reg, ret);
	return ret;
}

/** Write the current interrupt mask to the hardware. */
static int pon_mbox_set_irq_mask(struct pon_mbox *pon)
{
	return pon_mbox_write32(pon, PON_MBOX_IEN, pon->irq_mask);
}

/** Return free space in the mailbox in bytes in 32 bit words */
static int pon_mbox_tx_free_space(struct pon_mbox *pon)
{
	u32 len1;
	int ret;
	int free_elements;

	ret = pon_mbox_read32(pon, PON_MBOX_LEN1, &len1);
	if (ret)
		return ret;

	/* free_elements in 32 bit blocks */
	free_elements = (len1 & PON_MBOX_LEN1_XLEN_MASK) >>
		PON_MBOX_LEN1_XLEN_SHIFT;
	return free_elements * 4;
}

/** This function resets 'pending' array element,
 *  as free to use in new sequence.
 */
static void pon_mbox_release_pending(struct pon_mbox_pending *pending)
{
	if (!pending)
		return;

	pending->snd_portid = 0;
	pending->snd_seq = 0;
	pending->net = NULL;
	pending->in_use = false;
	pending->timestamp = 0;
}

/**
 * This function calls 'complete' if we are dealing with messages requested
 * from kernel (postpones cleanup), or simply resets pending to empty state.
 * It sets 'status', which needed to inform in-kernel messaging origin
 * if the operation succeeded.
 */
static void pon_mbox_release_pending_or_complete(
					struct pon_mbox_pending *pending,
					int status)
{
	if (!pending)
		return;

	if (pending->origin == PON_MBOX_MSG_ORIGIN_KERNEL) {
		pending->ikm.status = status;
		complete(&pending->ikm.response_received);
	} else {
		pon_mbox_release_pending(pending);
	}
}

static void pon_mbox_release_pending_all(struct pon_mbox *pon, int status)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(pon->pending); i++) {
		if (!pon->pending[i].in_use)
			continue;
		pon_mbox_release_pending_or_complete(&pon->pending[i], status);
	}
}

/**
 * Writes a message to the firmware mailbox.
 * This functions checks if the mailbox is ready to receive messages, when a
 * new firmware is loaded or the mailbox is being reset, no new messages are
 * allowed.
 * It also checks if there is enough space in the mailbox to add this new
 * message. If there is not enough space it adds the calling thread to a
 * wait queue which gets woken up by the irq handler when there is enough
 * space again. In case of an answer to an event the correct sequence number
 * will be used to make it possible to associate it to the event message.
 */
static ssize_t pon_mbox_write_msg(struct pon_mbox *pon, const char *buf,
				  size_t len, u8 read_write, u16 command,
				  u8 ack, u32 snd_portid, u32 snd_seq,
				  struct net *net,
				  enum pon_mbox_msg_origin origin,
				  struct pon_mbox_pending **pnd_out,
				  void *result_data, int result_size)
{
	int free_elements;
	int ret;
	struct pon_msg_header header;
	u32 seq;
	u32 src;
	struct pon_mbox_pending *pending = NULL;
	int i;
	int oldest_idx = 0;
	u64 timestamp = U64_MAX;

	if (len % PON_MBOX_BYTES_PER_WORD != 0 || len > PON_MBOX_MAX_SIZE)
		return -EINVAL;

	while (!pon->in_reset) {
		/* wait for mailbox if not in reset now */
		mutex_lock(&pon->irq_lock);
		if (!pon->tx_ready) {
			mutex_unlock(&pon->irq_lock);

			/* do not wait while holding the locks */
			ret = wait_event_interruptible_timeout(
					pon->tx_waitqueue,
					pon->tx_ready, msecs_to_jiffies(1000));
			if (ret == -ERESTARTSYS) {
				dev_err(pon->dev, "write was interrupted\n");
				return ret;
			}
			if (ret == 0) {
				dev_err(pon->dev, "write timeout occurred\n");
				return -ETIMEDOUT;
			}
			continue;
		}

		/* this is from host (1) when it is a cmd (0) */
		src = (ack == PONFW_CMD);

		if (src == PONFW_HOST) {
			/* searching for free 'pending' structure */
			for (i = pon->next_seq;
			     i < pon->next_seq + ARRAY_SIZE(pon->pending);
			     i++) {
				seq = i % ARRAY_SIZE(pon->pending);
				pending = &pon->pending[seq];
				if (pending->in_use &&
				    pending->timestamp < timestamp) {
					timestamp = pending->timestamp;
					oldest_idx = seq;
				}
				if (!pending->in_use)
					break;
			}

			if (!pending) {
				ret = -ENOMEM;
				goto out;
			}

			/*
			 * search finished, if not empty, we didn't find free
			 * pending structure
			 */
			if (pending->in_use) {
				/* Only free msgs older than 3 seconds */
				if ((ktime_get_raw_ns() - timestamp) <
					(3LL * NSEC_PER_SEC)) {
					/* Oldest entry received less than 3s
					 * ago - still have chance to be
					 * answered.
					 */
					dev_err(pon->dev, "no free structure found - can't write message\n");
					ret = -EBUSY;
					goto out;
				}
				dev_err(pon->dev,
					"no free structure found - can't write message, freeing oldest entry\n");
				pon_mbox_release_pending_or_complete(
						&pon->pending[oldest_idx],
						-ENOMEM);
				if (pon->pending[oldest_idx].in_use) {
					/* If entry still in use, caller thread
					 * have to cleanup.
					 */
					ret = -EBUSY;
					goto out;
				}
				pending = &pon->pending[oldest_idx];
			}

			/* fill write request origins */
			pending->in_use = true;
			pending->snd_portid = snd_portid;
			pending->snd_seq = snd_seq;
			pending->net = net;
			pending->origin = origin;
			init_completion(&pending->ikm.response_received);
			pending->ikm.result_data = result_data;
			pending->ikm.result_size = result_size;
			pending->ikm.status = 0;
			pending->timestamp = ktime_get_raw_ns();

			if (pnd_out)
				*pnd_out = pending;

			pon->next_seq = (seq + 1) % ARRAY_SIZE(pon->pending);
		} else {
			/*
			 * We store the sequence number for the message header
			 * in the last 3 bits of the NetLink sequence number.
			 * Extract that sequence number, we do not care about
			 * conflicts as the firmware normally does not care
			 * about the sequence number at all.
			 */
			seq = snd_seq % PON_MBOX_PENDING_MAX;
		}

		free_elements = pon_mbox_tx_free_space(pon);
		if (free_elements < 0) {
			ret = free_elements;
			goto out;
		}
		/* check if there is enough space in the mailbox, and write
		 * data in that case
		 */
		if ((free_elements * PON_MBOX_BYTES_PER_WORD) >= len)
			break;

		/* TODO: check old value */
		pon->limit1 &= 0xfe00ffff;
		pon->limit1 |= ((len / PON_MBOX_BYTES_PER_WORD) << 16)
				& 0x01ff0000;
		ret = pon_mbox_write32(pon, PON_MBOX_LIMIT1, pon->limit1);
		if (ret)
			goto out;

		pon->tx_ready = false;

		/* activate irqs for empty tx buffer */
		pon->irq_mask |= PON_MBOX_STAT_X1_RDY;
		ret = pon_mbox_set_irq_mask(pon);
		if (ret)
			goto out;

		mutex_unlock(&pon->irq_lock);
	}

	if (pon->in_reset) {
		dev_err(pon->dev, "abort write because mailbox is in reset\n");
		return -ENODEV;
	}

	header.rw = read_write;
	header.cmd = command;
	header.ack = ack;
	header.src = src;
	header.seq = seq;
	header.len = len  / PON_MBOX_BYTES_PER_WORD;

	dev_dbg(pon->dev,
		"transmit msg: rw: %i, cmd: 0x%x, ack: %i, src: %i, seq: %i, len: %zu\n",
		read_write, command, ack, src, seq, len);

	if (len > 0)
		print_hex_dump_debug("data: ", DUMP_PREFIX_OFFSET, 16, 1, buf,
				     len, false);

	ret = pon->write(pon, PON_MBOX_DATA1, &header, sizeof(header),
			 PON_WR_FLAG_REGULAR);
	if (ret) {
		dev_err(pon->dev, "problem writing register (0x%x): %i\n",
			PON_MBOX_DATA1, ret);
		goto out;
	}

	if (buf && len) {
		ret = pon->write(pon, PON_MBOX_DATA1, buf, len,
				 PON_WR_FLAG_REGULAR);
		if (ret) {
			dev_err(pon->dev,
				"cannot write to data1 register: ret: %i\n",
				ret);
			goto out;
		}
	}

	/* tell the hardware that the new message was written */
	ret = pon_mbox_write32(pon, PON_MBOX_CMD,
			       PON_MBOX_CMD_X1 | PON_MBOX_CMD_EOM);

out:
	mutex_unlock(&pon->irq_lock);
	return ret;
}

/**
 * Generates fake event. This is useful in tests where event from firmware is
 * expected, but firmware can't generate it.
 * This should not be used in production code.
 */
static int pon_mbox_genl_generate_fake_event(struct pon_fake_event *event,
					     size_t msg_len)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct pon_msg_header header = {0};

	if (msg_len < sizeof(*event))
		return -EINVAL;

	if (!event || msg_len < PON_FEVT_LEN_GET(*event))
		return -EINVAL;

	header.rw = PONFW_WRITE;
	header.cmd = PON_FEVT_CMD_GET(*event);
	header.ack = PONFW_CMD;
	header.src = PONFW_PONIP;
	header.len = PON_FEVT_LEN_GET(*event) / PON_MBOX_BYTES_PER_WORD;

	/* Send netlink message with fake event flag set */
	return pon_mbox_send_nl_response(pon, 0, 0, &init_net,
					 &header, (char *)(event + 1),
					 PON_MBOX_FLAG_FAKE_EVENT);
}

ssize_t pon_mbox_send(unsigned int cmd_id, unsigned int rw,
		      const void *input, size_t input_size,
		      void *output, size_t output_size)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct pon_mbox_pending *pending = NULL;
	ssize_t len;
	int ret;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	/* send message */
	len = pon_mbox_write_msg(pon, input, input_size, rw, cmd_id, PONFW_CMD,
				 0, 0, NULL, PON_MBOX_MSG_ORIGIN_KERNEL,
				 &pending, output, output_size);

	if (len < 0)
		return len;

	if (!pending) {
		dev_err(pon->dev, "cannot send to mbox, 'pending' is NULL\n");
		return -EBUSY;
	}

	/* wait for completion */
	if (!wait_for_completion_timeout(
				&pending->ikm.response_received,
				msecs_to_jiffies(2000))) {
		dev_dbg(pon->dev, "timeout while waiting for response in IKM");
		mutex_lock(&pon->irq_lock);
		pon_mbox_release_pending(pending);
		mutex_unlock(&pon->irq_lock);
		return -ETIMEDOUT;
	}

	/* fill output and output_size */
	ret = pending->ikm.status;

	/* free 'pending' and exit */
	mutex_lock(&pon->irq_lock);
	pon_mbox_release_pending(pending);
	mutex_unlock(&pon->irq_lock);
	return ret;
}
EXPORT_SYMBOL(pon_mbox_send);

ssize_t pon_mbox_send_ack(unsigned int cmd_id, unsigned int rw, u32 seq)
{
	struct pon_mbox *pon = pon_mbox_dev;
	ssize_t len;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	/* send message */
	len = pon_mbox_write_msg(pon, NULL, 0, rw, cmd_id, PONFW_ACK,
				 0, seq, NULL, PON_MBOX_MSG_ORIGIN_KERNEL,
				 NULL, NULL, 0);
	if (len < 0)
		return len;

	return 0;
}
EXPORT_SYMBOL(pon_mbox_send_ack);

/**
 * This function acts as a "blacklist" checker of functions that cannot be
 * called from the user space. It returns TRUE if the function is outside of
 * user space access, otherwise FALSE.
 */
static bool pon_mbox_userspace_cmd_check(u16 command)
{
	switch (command) {
	/* Add user space only command ID here */
	case PONFW_RAND_NUM_CMD_ID:
		return true;
	}
	return false;
}

static bool pon_mbox_permission_check(struct sk_buff *skb, u16 cmd)
{
	kuid_t uid = sock_i_uid(skb->sk);
	uint16_t cmd_group = 0;
	int i;

	/* Skip check if root */
	if (uid_eq(uid, GLOBAL_ROOT_UID))
		return true;

	switch (cmd) {
	/* List of commands to be user manageable */
	case PONFW_SYNCE_CONFIG_CMD_ID:
	case PONFW_SYNCE_STATUS_CMD_ID:
		cmd_group = PON_MBOX_PERM_SYNCE;
		break;
	case PONFW_XGTC_BROADCAST_KEY_TABLE_CMD_ID:
		cmd_group = PON_MBOX_PERM_BC_KEY;
		break;
	/* Skip check for commands not on the list above */
	default:
		return true;
	}

	for (i = 0; i < PON_MBOX_MAX_CMD_USERS; i++) {
		if (uid_eq(uid, pon_mbox_perm_list[cmd_group].uid_list[i]))
			return true;
	}

	/* User is not in the permitted list */
	return false;
}

/**
 * This function received the NetLink messages which are used to send messages
 * to the firmware. It extracts the attributes from the message and then calls
 * \ref pon_mbox_write_msg
 */
static int pon_mbox_genl_send(struct sk_buff *skb, struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	struct pon_msg_header buf_header = {0}, header = {0};
	struct pon_msg_header *buf_tmp = NULL;
	void *buf = NULL;
	int buf_len = 0;
	int ret;
	u8 read_write;
	u16 command;
	u8 ack;
	u32 flags = 0;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	if (!attrs[PON_MBOX_A_COMMAND] || !attrs[PON_MBOX_A_READ_WRITE] ||
	    !attrs[PON_MBOX_A_ACK])
		return -EINVAL;

	command = nla_get_u16(attrs[PON_MBOX_A_COMMAND]);
	/* Verify if message is allowed to be sent to FW from user space */
	if (pon_mbox_userspace_cmd_check(command)) {
		pr_warn("Command outside of user space access!\n");
		return -EPERM;
	}

	if (!pon_mbox_permission_check(skb, command)) {
		pr_warn("Permission denied!\n");
		return -EPERM;
	}

	if (attrs[PON_MBOX_A_DATA]) {
		buf = nla_data(attrs[PON_MBOX_A_DATA]);
		buf_len = nla_len(attrs[PON_MBOX_A_DATA]);
	}

	read_write = nla_get_u8(attrs[PON_MBOX_A_READ_WRITE]);
	ack = nla_get_u8(attrs[PON_MBOX_A_ACK]);

	if (attrs[PON_MBOX_A_FLAGS])
		flags = nla_get_u32(attrs[PON_MBOX_A_FLAGS]);

	/* This condition is satisfied when pond ACK, after fake event
	 * had been called, is received. 'flags' attribute is now set to fake
	 * event.
	 */
	if (flags & PON_MBOX_FLAG_FAKE_EVENT) {
		/* pond message header */
		buf_header.rw = read_write;
		buf_header.cmd = command;
		buf_header.ack = ack;
		buf_header.len = buf_len / PON_MBOX_BYTES_PER_WORD;

		/* ponmbox message header */
		header.rw = 0x1;
		header.cmd = 0x999;
		/* Attribute used to distinguish message to be sent as unicast
		 * to ponmbox.
		 */
		header.src = PONFW_HOST;
		header.ack = 0x1;
		header.len = (buf_len + sizeof(buf_header)) /
					PON_MBOX_BYTES_PER_WORD;

		/* Content of the ponmbox return message:
		 * ||     header     ||         payload        ||
		 * ::                ::			       ::
		 * || ponmbox header || pond header | pond buf ||
		 * ::		     ::			       ::
		 * ||     header     ||  buf_header |    buf   ||
		 */
		buf_tmp = kmalloc(buf_len + sizeof(buf_header), GFP_KERNEL);
		if (!buf_tmp)
			return -ENOMEM;

		memcpy(buf_tmp, &buf_header, sizeof(buf_header));
		memcpy(buf_tmp + 1, buf, buf_len);

		/* Ponmbox return message sent with stored
		 * fake_event_msg_snd_portid and fake_event_msg_snd_seq
		 * via unicast set in header.src.
		 */
		ret = pon_mbox_send_nl_response(pon, fake_event_msg_snd_portid,
						fake_event_msg_snd_seq,
						&init_net, &header,
						(char *)buf_tmp, 0);

		kfree(buf_tmp);

		return ret;
	}

	/* This condition is satisfied when ponmbox send message with 0x999
	 * command (fake event). 'flags' attribute is not yet set.
	 */
	if (command == 0x999) {
		/* We need to store snd_portid and snd_seq of ponmbox command
		 * to send ACK back to ponmbox when ACK from pond, after event
		 * had been called, is received.
		 */
		fake_event_msg_snd_portid = info->snd_portid;
		fake_event_msg_snd_seq = info->snd_seq;

		dev_dbg(pon->dev, "generating fake event\n");

		return pon_mbox_genl_generate_fake_event(buf, buf_len);
	}

	return pon_mbox_write_msg(pon, buf, buf_len, read_write, command, ack,
				  info->snd_portid,
				  info->snd_seq,
				  genl_info_net(info),
				  PON_MBOX_MSG_ORIGIN_NETLINK, NULL, NULL, 0);
}

/* Encode gem port counters in netlink message as a nested attribute */
static int gem_port_counters_nl_encode(struct sk_buff *skb,
				       void *buffer,
				       unsigned int size)
{
	struct pon_mbox_gem_port_counters *pon_output = buffer;
	struct nlattr *cnt = nla_nest_start(skb, PON_MBOX_A_CNT);
	int err;

	if (!cnt)
		return 1;

	err = nla_put_u32(skb, PON_MBOX_A_CNT_GEM_PORT_GEM_PORT_ID,
			  pon_output->gem_port_id);
	if (err)
		goto cancel;

	CNT_ENCODE_U64(GEM_PORT_KEY_ERRORS, key_errors, GEM_PORT_PAD);
	CNT_ENCODE_U64(GEM_PORT_RX_BYTES, rx_bytes, GEM_PORT_PAD);
	CNT_ENCODE_U64(GEM_PORT_RX_FRAGMENTS, rx_fragments, GEM_PORT_PAD);
	CNT_ENCODE_U64(GEM_PORT_RX_FRAMES, rx_frames, GEM_PORT_PAD);
	CNT_ENCODE_U64(GEM_PORT_TX_BYTES, tx_bytes, GEM_PORT_PAD);
	CNT_ENCODE_U64(GEM_PORT_TX_FRAGMENTS, tx_fragments, GEM_PORT_PAD);
	CNT_ENCODE_U64(GEM_PORT_TX_FRAMES, tx_frames, GEM_PORT_PAD);

	nla_nest_end(skb, cnt);
	return 0;
cancel:
	nla_nest_cancel(skb, cnt);
	return err;
}

/** This function is used to read data about GEM port counters. */
static int pon_mbox_genl_gem_port_counters(struct sk_buff *skb,
					   struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	struct pon_mbox_gem_port_counters pon_output = {0};
	struct pon_msg_header header = {0};
	u8 gem_port_idx;
	u8 twdm_dswlch_id;
	int err;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	if (attrs[PON_MBOX_D_GEM_IDX])
		gem_port_idx = nla_get_u8(attrs[PON_MBOX_D_GEM_IDX]);
	else
		gem_port_idx = 255;

	if (attrs[PON_MBOX_D_DSWLCH_ID])
		twdm_dswlch_id = nla_get_u8(attrs[PON_MBOX_D_DSWLCH_ID]);
	else
		twdm_dswlch_id = PON_MBOX_D_DSWLCH_ID_CURR;

	header.rw = PONFW_READ;
	header.cmd = PONFW_GEM_PORT_COUNTERS_CMD_ID;

	err = pon_mbox_gem_port_counters_update(twdm_dswlch_id, gem_port_idx,
						&pon_output, pon);
	if (err == -PON_STATUS_FW_DUP_ERR)
		header.ack = PONFW_NACK_DUP;
	else if (err < 0)
		header.ack = PONFW_NACK;
	else
		header.ack = PONFW_ACK;

	header.src = PONFW_HOST;
	header.len = (err >= 0) ? (sizeof(struct pon_mbox_gem_port_counters) /
				   PON_MBOX_BYTES_PER_WORD) : 0;

	err = pon_mbox_send_nl_response_encoded(pon, info->snd_portid,
						info->snd_seq,
						genl_info_net(info), &header,
						(void *)&pon_output, 0,
						gem_port_counters_nl_encode);
	CHECK_FOR_NETLINK_RESPONSE_ERR(pon, err);
	return err;
}

/** This function is used to read data about accumulated GEM port counters. */
static int pon_mbox_genl_gem_all_counters(struct sk_buff *skb,
					  struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	struct pon_mbox_gem_port_counters tmp, pon_output = {0};
	struct pon_msg_header header = {0};
	u8 twdm_dswlch_id;
	u8 used_gem_ports[GEM_PORTS_MAX];
	unsigned int used_gem_ports_count;
	int err, i;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	if (attrs[PON_MBOX_D_DSWLCH_ID])
		twdm_dswlch_id = nla_get_u8(attrs[PON_MBOX_D_DSWLCH_ID]);
	else
		twdm_dswlch_id = PON_MBOX_D_DSWLCH_ID_CURR;

	if (twdm_dswlch_id != PON_MBOX_D_DSWLCH_ID_CURR &&
	    twdm_dswlch_id >= TWDM_DS_MAX)
		return -EINVAL;

	header.rw = PONFW_READ;
	header.cmd = PONFW_GEM_PORT_COUNTERS_CMD_ID;

	used_gem_ports_count = pon_mbox_used_gem_ports_get(used_gem_ports,
							   GEM_PORTS_MAX,
							   pon->cnt_state);

	for (i = 0; i < used_gem_ports_count; i++) {
		err = pon_mbox_gem_port_counters_update(twdm_dswlch_id,
							used_gem_ports[i],
							&tmp, pon);
		if (err)
			dev_err(pon->dev, "gem_port_counters_update failed: err: %i\n",
				err);
	}

	pon_mbox_gem_all_counters_update(twdm_dswlch_id, &pon_output, pon);

	header.ack = PONFW_ACK;
	header.src = PONFW_HOST;
	header.len = (sizeof(struct pon_mbox_gem_port_counters) /
		     PON_MBOX_BYTES_PER_WORD);

	err = pon_mbox_send_nl_response_encoded(pon, info->snd_portid,
						info->snd_seq,
						genl_info_net(info), &header,
						(void *)&pon_output, 0,
						gem_port_counters_nl_encode);
	CHECK_FOR_NETLINK_RESPONSE_ERR(pon, err);
	return err;
}

/* Encode alloc counters in netlink message as a nested attribute */
static int alloc_counters_nl_encode(struct sk_buff *skb, void *buffer,
				    unsigned int size)
{
	struct pon_alloc_counters *pon_output = buffer;
	struct nlattr *cnt = nla_nest_start(skb, PON_MBOX_A_CNT);
	int err;

	if (!cnt)
		return 1;

	CNT_ENCODE_U64(ALLOC_IDLE, idle, ALLOC_PAD);
	CNT_ENCODE_U64(ALLOC_ALLOCATIONS, allocations, ALLOC_PAD);
	CNT_ENCODE_U64(ALLOC_US_BW, us_bw, ALLOC_PAD);

	nla_nest_end(skb, cnt);
	return 0;
cancel:
	nla_nest_cancel(skb, cnt);
	return err;
}

/** This function is used to read data about Alloc ID counters. */
static int pon_mbox_genl_alloc_counters(struct sk_buff *skb,
					struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	struct pon_alloc_counters pon_output = {0};
	struct pon_msg_header header = {0};
	u8 alloc_idx;
	int err;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	if (attrs[PON_MBOX_D_ALLOC_IDX])
		alloc_idx = nla_get_u8(attrs[PON_MBOX_D_ALLOC_IDX]);
	else
		alloc_idx = 255;

	header.rw = PONFW_READ;
	header.cmd = PONFW_ALLOC_ID_COUNTERS_CMD_ID;

	err = pon_mbox_alloc_counters_update(alloc_idx, &pon_output, pon);
	if (err == -PON_STATUS_FW_DUP_ERR)
		header.ack = PONFW_NACK_DUP;
	else if (err < 0)
		header.ack = PONFW_NACK;
	else
		header.ack = PONFW_ACK;

	header.src = PONFW_HOST;
	header.len = (err >= 0) ? (sizeof(struct pon_alloc_counters) /
				   PON_MBOX_BYTES_PER_WORD) : 0;

	err = pon_mbox_send_nl_response_encoded(pon, info->snd_portid,
						info->snd_seq,
						genl_info_net(info), &header,
						(void *)&pon_output, 0,
						alloc_counters_nl_encode);
	CHECK_FOR_NETLINK_RESPONSE_ERR(pon, err);
	return err;
}

/* Encode alloc lost counters in netlink message as a nested attribute */
static int alloc_lost_counters_nl_encode(struct sk_buff *skb,
					 void *buffer,
					 unsigned int size)
{
	struct pon_alloc_discard_counters *pon_output = buffer;
	struct nlattr *cnt = NULL;
	struct nlattr *attr = NULL;
	int err;
	unsigned int i;

	cnt = nla_nest_start(skb, PON_MBOX_A_CNT);
	if (!cnt)
		return 1;

	attr = nla_nest_start(skb, PON_MBOX_A_CNT_ALLOC_DISCARD_DISCS);
	if (!attr) {
		err = 1;
		goto cancel;
	}
	for (i = 0; i < ARRAY_SIZE(pon_output->disc); ++i) {
		CNT_ENCODE_U64(ALLOC_DISCARD_ITEM, disc[i],
			       ALLOC_DISCARD_ITEM_PAD);
	}
	nla_nest_end(skb, attr);

	attr = nla_nest_start(skb, PON_MBOX_A_CNT_ALLOC_DISCARD_RULES);
	if (!attr) {
		err = 1;
		goto cancel;
	}
	for (i = 0; i < ARRAY_SIZE(pon_output->rule); ++i) {
		CNT_ENCODE_U64(ALLOC_DISCARD_ITEM, rule[i],
			       ALLOC_DISCARD_ITEM_PAD);
	}
	nla_nest_end(skb, attr);

	nla_nest_end(skb, cnt);
	return 0;
cancel:
	if (attr)
		nla_nest_cancel(skb, attr);
	nla_nest_cancel(skb, cnt);
	return err;
}

/** This function is used to read data about Alloc Discard counters. */
static int pon_mbox_genl_alloc_lost_counters(struct sk_buff *skb,
					     struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct pon_alloc_discard_counters pon_output = {0};
	struct pon_msg_header header = {0};
	int err;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	header.rw = PONFW_READ;
	header.cmd = PONFW_ALLOC_LOST_COUNTERS_CMD_ID;

	err = pon_mbox_alloc_lost_counters_update(&pon_output, pon);
	if (err == -PON_STATUS_FW_DUP_ERR)
		header.ack = PONFW_NACK_DUP;
	else if (err < 0)
		header.ack = PONFW_NACK;
	else
		header.ack = PONFW_ACK;

	header.src = PONFW_HOST;
	header.len = (err >= 0) ? (sizeof(struct pon_alloc_discard_counters) /
				   PON_MBOX_BYTES_PER_WORD) : 0;

	err = pon_mbox_send_nl_response_encoded(pon, info->snd_portid,
						info->snd_seq,
						genl_info_net(info), &header,
						(void *)&pon_output, 0,
						alloc_lost_counters_nl_encode);
	CHECK_FOR_NETLINK_RESPONSE_ERR(pon, err);
	return err;
}

/* Encode gtc counters in netlink message as a nested attribute */
static int gtc_counters_nl_encode(struct sk_buff *skb,
				  void *buffer,
				  unsigned int size)
{
	struct pon_mbox_gtc_counters *pon_output = buffer;
	struct nlattr *cnt = nla_nest_start(skb, PON_MBOX_A_CNT);
	int err;

	if (!cnt)
		return 1;

	CNT_ENCODE_U64(GTC_BIP_ERRORS, bip_errors, GTC_PAD);
	CNT_ENCODE_U64(GTC_DISC_GEM_FRAMES, disc_gem_frames, GTC_PAD);
	CNT_ENCODE_U64(GTC_GEM_HEC_ERRORS_CORR, gem_hec_errors_corr, GTC_PAD);
	CNT_ENCODE_U64(GTC_GEM_HEC_ERRORS_UNCORR, gem_hec_errors_uncorr,
		       GTC_PAD);
	CNT_ENCODE_U64(GTC_BWMAP_HEC_ERRORS_CORR, bwmap_hec_errors_corr,
		       GTC_PAD);
	CNT_ENCODE_U64(GTC_BYTES_CORR, bytes_corr, GTC_PAD);
	CNT_ENCODE_U64(GTC_FEC_CODEWORDS_CORR, fec_codewords_corr, GTC_PAD);
	CNT_ENCODE_U64(GTC_FEC_COREWORDS_UNCORR, fec_codewords_uncorr, GTC_PAD);
	CNT_ENCODE_U64(GTC_TOTAL_FRAMES, total_frames, GTC_PAD);
	CNT_ENCODE_U64(GTC_FEC_SEC, fec_sec, GTC_PAD);
	CNT_ENCODE_U64(GTC_GEM_IDLE, gem_idle, GTC_PAD);
	CNT_ENCODE_U64(GTC_LODS_EVENTS, lods_events, GTC_PAD);
	CNT_ENCODE_U64(GTC_DG_TIME, dg_time, GTC_PAD);
	CNT_ENCODE_U64(GTC_PLOAM_CRC_ERRORS, ploam_crc_errors, GTC_PAD);

	nla_nest_end(skb, cnt);
	return 0;
cancel:
	nla_nest_cancel(skb, cnt);
	return err;
}

/** This function is used to read data about GTC counters. */
static int pon_mbox_genl_gtc_counters(struct sk_buff *skb,
				      struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	struct pon_mbox_gtc_counters pon_output = {0};
	struct pon_msg_header header = {0};
	u8 twdm_dswlch_id;
	int err;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	if (attrs[PON_MBOX_D_DSWLCH_ID])
		twdm_dswlch_id = nla_get_u8(attrs[PON_MBOX_D_DSWLCH_ID]);
	else
		twdm_dswlch_id = PON_MBOX_D_DSWLCH_ID_CURR;

	header.rw = PONFW_READ;
	header.cmd = PONFW_GTC_COUNTERS_CMD_ID;

	err = pon_mbox_gtc_counters_update(twdm_dswlch_id, &pon_output, pon);
	if (err == -PON_STATUS_FW_DUP_ERR)
		header.ack = PONFW_NACK_DUP;
	else if (err < 0)
		header.ack = PONFW_NACK;
	else
		header.ack = PONFW_ACK;

	header.src = PONFW_HOST;
	header.len = (err >= 0) ? (sizeof(struct pon_mbox_gtc_counters) /
				   PON_MBOX_BYTES_PER_WORD) : 0;

	err = pon_mbox_send_nl_response_encoded(pon, info->snd_portid,
						info->snd_seq,
						genl_info_net(info), &header,
						(void *)&pon_output, 0,
						gtc_counters_nl_encode);
	CHECK_FOR_NETLINK_RESPONSE_ERR(pon, err);
	return err;
}

/* Encode gtc counters in netlink message as a nested attribute */
static int xgtc_counters_nl_encode(struct sk_buff *skb, void *buffer,
				   unsigned int size)
{
	struct pon_mbox_xgtc_counters *pon_output = buffer;
	struct nlattr *cnt = nla_nest_start(skb, PON_MBOX_A_CNT);
	int err;

	if (!cnt)
		return 1;

	CNT_ENCODE_U64(XGTC_FS_HEC_ERR_CORR, fs_hec_err_corr, XGTC_PAD);
	CNT_ENCODE_U64(XGTC_FS_HEC_ERR_UNCORR, fs_hec_err_uncorr, XGTC_PAD);
	CNT_ENCODE_U64(XGTC_LOST_WORDS, lost_words, XGTC_PAD);
	CNT_ENCODE_U64(XGTC_PLOAM_MIC_ERR, ploam_mic_err, XGTC_PAD);
	CNT_ENCODE_U64(XGTC_PSBD_HEC_ERR_CORR, psbd_hec_err_corr, XGTC_PAD);
	CNT_ENCODE_U64(XGTC_PSBD_HEC_ERR_UNCORR, psbd_hec_err_uncorr, XGTC_PAD);

	nla_nest_end(skb, cnt);
	return 0;
cancel:
	nla_nest_cancel(skb, cnt);
	return err;
}

/** This function is used to read data about XGTC counters. */
static int pon_mbox_genl_xgtc_counters(struct sk_buff *skb,
				       struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	struct pon_mbox_xgtc_counters pon_output = {0};
	struct pon_msg_header header = {0};
	u8 twdm_dswlch_id;
	int err;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	if (attrs[PON_MBOX_D_DSWLCH_ID])
		twdm_dswlch_id = nla_get_u8(attrs[PON_MBOX_D_DSWLCH_ID]);
	else
		twdm_dswlch_id = PON_MBOX_D_DSWLCH_ID_CURR;

	header.rw = PONFW_READ;
	header.cmd = PONFW_XGTC_COUNTERS_CMD_ID;

	err = pon_mbox_xgtc_counters_update(twdm_dswlch_id, &pon_output, pon);
	if (err == -PON_STATUS_FW_DUP_ERR)
		header.ack = PONFW_NACK_DUP;
	else if (err < 0)
		header.ack = PONFW_NACK;
	else
		header.ack = PONFW_ACK;

	header.src = PONFW_HOST;
	header.len = (err >= 0) ? (sizeof(struct pon_mbox_xgtc_counters) /
				   PON_MBOX_BYTES_PER_WORD) : 0;

	err = pon_mbox_send_nl_response_encoded(pon, info->snd_portid,
						info->snd_seq,
						genl_info_net(info), &header,
						(void *)&pon_output, 0,
						xgtc_counters_nl_encode);
	CHECK_FOR_NETLINK_RESPONSE_ERR(pon, err);
	return err;
}

/* Encode ethernet counters in netlink message as a nested attribute */
static int eth_counters_nl_encode(struct sk_buff *skb,
				  void *buffer,
				  unsigned int size)
{
	struct pon_eth_counters *pon_output = buffer;
	struct nlattr *cnt = nla_nest_start(skb, PON_MBOX_A_CNT);
	int err;

	if (!cnt)
		return 1;

	CNT_ENCODE_U64(ETH_BYTES, bytes, ETH_PAD);
	CNT_ENCODE_U64(ETH_FRAMES_LT_64, frames_lt_64, ETH_PAD);
	CNT_ENCODE_U64(ETH_FRAMES_64, frames_64, ETH_PAD);
	CNT_ENCODE_U64(ETH_FRAMES_65_127, frames_65_127, ETH_PAD);
	CNT_ENCODE_U64(ETH_FRAMES_128_255, frames_128_255, ETH_PAD);
	CNT_ENCODE_U64(ETH_FRAMES_256_511, frames_256_511, ETH_PAD);
	CNT_ENCODE_U64(ETH_FRAMES_512_1023, frames_512_1023, ETH_PAD);
	CNT_ENCODE_U64(ETH_FRAMES_1024_1518, frames_1024_1518, ETH_PAD);
	CNT_ENCODE_U64(ETH_FRAMES_GT_1518, frames_gt_1518, ETH_PAD);
	CNT_ENCODE_U64(ETH_FRAMES_FCS_ERR, frames_fcs_err, ETH_PAD);
	CNT_ENCODE_U64(ETH_BYTES_FCS_ERR, bytes_fcs_err, ETH_PAD);
	CNT_ENCODE_U64(ETH_FRAMES_TOO_LONG, frames_too_long, ETH_PAD);

	nla_nest_end(skb, cnt);
	return 0;
cancel:
	nla_nest_cancel(skb, cnt);
	return err;
}

/** This function is used to read data from Ethernet receive counters. */
static int pon_mbox_genl_rx_eth_counters(struct sk_buff *skb,
					 struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	struct pon_eth_counters pon_output = {0};
	struct pon_msg_header header = {0};
	u8 gem_port_idx;
	int err;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	if (attrs[PON_MBOX_D_GEM_IDX])
		gem_port_idx = nla_get_u8(attrs[PON_MBOX_D_GEM_IDX]);
	else
		gem_port_idx = 255;

	header.rw = PONFW_READ;
	header.cmd = PONFW_RX_ETH_COUNTERS_CMD_ID;

	err = pon_mbox_rx_eth_counters_update(gem_port_idx, &pon_output, pon);
	if (err == -PON_STATUS_FW_DUP_ERR)
		header.ack = PONFW_NACK_DUP;
	else if (err < 0)
		header.ack = PONFW_NACK;
	else
		header.ack = PONFW_ACK;

	header.src = PONFW_HOST;
	header.len = (err >= 0) ? (sizeof(struct pon_eth_counters) /
				   PON_MBOX_BYTES_PER_WORD) : 0;

	err = pon_mbox_send_nl_response_encoded(pon, info->snd_portid,
						info->snd_seq,
						genl_info_net(info), &header,
						(void *)&pon_output, 0,
						eth_counters_nl_encode);
	CHECK_FOR_NETLINK_RESPONSE_ERR(pon, err);
	return err;
}

/** This function is used to read data from Ethernet transmit counters. */
static int pon_mbox_genl_tx_eth_counters(struct sk_buff *skb,
					 struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	struct pon_eth_counters pon_output = {0};
	struct pon_msg_header header = {0};
	u8 gem_port_idx;
	int err;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	if (attrs[PON_MBOX_D_GEM_IDX])
		gem_port_idx = nla_get_u8(attrs[PON_MBOX_D_GEM_IDX]);
	else
		gem_port_idx = 255;

	header.rw = PONFW_READ;
	header.cmd = PONFW_TX_ETH_COUNTERS_CMD_ID;

	err = pon_mbox_tx_eth_counters_update(gem_port_idx, &pon_output, pon);
	if (err == -PON_STATUS_FW_DUP_ERR)
		header.ack = PONFW_NACK_DUP;
	else if (err < 0)
		header.ack = PONFW_NACK;
	else
		header.ack = PONFW_ACK;

	header.src = PONFW_HOST;
	header.len = (err >= 0) ? (sizeof(struct pon_eth_counters) /
				   PON_MBOX_BYTES_PER_WORD) : 0;

	err = pon_mbox_send_nl_response_encoded(pon, info->snd_portid,
						info->snd_seq,
						genl_info_net(info), &header,
						(void *)&pon_output, 0,
						eth_counters_nl_encode);
	CHECK_FOR_NETLINK_RESPONSE_ERR(pon, err);
	return err;
}

/* Encode gtc counters in netlink message as a nested attribute */
static int twdm_lods_counters_nl_encode(struct sk_buff *skb,
					void *buffer,
					unsigned int size)
{
	struct pon_mbox_twdm_lods_counters *pon_output = buffer;
	struct nlattr *cnt = nla_nest_start(skb, PON_MBOX_A_CNT);
	int err;

	if (!cnt)
		return 1;

	CNT_ENCODE_U64(TWDM_LODS_EVENTS_ALL, events_all, TWDM_LODS_PAD);
	CNT_ENCODE_U64(TWDM_LODS_REACTIVATION_OPER, react_oper, TWDM_LODS_PAD);
	CNT_ENCODE_U64(TWDM_LODS_REACTIVATION_PROT, react_prot, TWDM_LODS_PAD);
	CNT_ENCODE_U64(TWDM_LODS_REACTIVATION_DISC, react_disc, TWDM_LODS_PAD);
	CNT_ENCODE_U64(TWDM_LODS_RESTORED_OPER, rest_oper, TWDM_LODS_PAD);
	CNT_ENCODE_U64(TWDM_LODS_RESTORED_PROT, rest_prot, TWDM_LODS_PAD);
	CNT_ENCODE_U64(TWDM_LODS_RESTORED_DISK, rest_disc, TWDM_LODS_PAD);

	nla_nest_end(skb, cnt);
	return 0;
cancel:
	nla_nest_cancel(skb, cnt);
	return err;
}

/** This function is used to read data about LODS counters. */
static int pon_mbox_genl_twdm_lods_counters(struct sk_buff *skb,
					    struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	struct pon_mbox_twdm_lods_counters pon_output = {0};
	struct pon_msg_header header = {0};
	u8 dswlch_id;
	int err;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	if (attrs[PON_MBOX_D_DSWLCH_ID])
		dswlch_id = nla_get_u8(attrs[PON_MBOX_D_DSWLCH_ID]);
	else
		dswlch_id = PON_MBOX_D_DSWLCH_ID_CURR;

	header.rw = PONFW_READ;
	header.cmd = PONFW_TWDM_LODS_COUNTERS_CMD_ID;
	err = pon_mbox_twdm_lods_counters_update(dswlch_id, &pon_output, pon);
	if (err == -PON_STATUS_FW_DUP_ERR)
		header.ack = PONFW_NACK_DUP;
	else if (err < 0)
		header.ack = PONFW_NACK;
	else
		header.ack = PONFW_ACK;

	header.src = PONFW_HOST;
	header.len = (err >= 0) ? (sizeof(struct pon_mbox_twdm_lods_counters) /
				   PON_MBOX_BYTES_PER_WORD) : 0;

	err = pon_mbox_send_nl_response_encoded(pon, info->snd_portid,
						info->snd_seq,
						genl_info_net(info), &header,
						(void *)&pon_output, 0,
						twdm_lods_counters_nl_encode);
	CHECK_FOR_NETLINK_RESPONSE_ERR(pon, err);
	return err;
}

/* Encode gtc counters in netlink message as a nested attribute */
static int twdm_optic_pl_counters_nl_encode(struct sk_buff *skb,
					    void *buffer,
					    unsigned int size)
{
	struct pon_mbox_twdm_optic_pl_counters *pon_output = buffer;
	struct nlattr *cnt = nla_nest_start(skb, PON_MBOX_A_CNT);
	int err;

	if (!cnt)
		return 1;

	CNT_ENCODE_U64(TWDM_OPTIC_PL_REJECTED, rejected, TWDM_OPTIC_PL_PAD);
	CNT_ENCODE_U64(TWDM_OPTIC_PL_INCOMPLETE,
		       incomplete, TWDM_OPTIC_PL_PAD);
	CNT_ENCODE_U64(TWDM_OPTIC_PL_COMPLETE, complete, TWDM_OPTIC_PL_PAD);

	nla_nest_end(skb, cnt);
	return 0;
cancel:
	nla_nest_cancel(skb, cnt);
	return err;
}

/* Encode xgtc counters in netlink message as a nested attribute */
static int xgtc_ploam_ds_counters_nl_encode(struct sk_buff *skb,
					    void *buffer,
					    unsigned int size)
{
	struct pon_mbox_xgtc_ploam_ds_counters *pon_output = buffer;
	struct nlattr *cnt = nla_nest_start(skb, PON_MBOX_A_CNT);
	int err;

	if (!cnt)
		return 1;

	/* (X)GTC common */
	CNT_ENCODE_U64(TC_PLOAM_DS_ASS_ONU, ass_onu, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_RNG_TIME, rng_time, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_DEACT_ONU, deact_onu, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_DIS_SER, dis_ser, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_ASS_ALLOC, ass_alloc, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_SLP_ALLOW, slp_allow, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_UNKNOWN, unknown, TC_PLOAM_DS_PAD);

	/* XGTC only */
	CNT_ENCODE_U64(TC_PLOAM_DS_BST_PROFILE, bst_profile, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_REQ_REG, req_reg, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_KEY_CTRL, key_ctrl, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_CALIB_REQ, calib_req, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_ADJ_TX_WL, adj_tx_wl, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_TUNE_CTRL, tune_ctrl, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_SYS_PROFILE, sys_profile, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_CH_PROFILE, ch_profile, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_PROT_CONTROL, prot_control, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_CHG_PW_LVL, chg_pw_lvl, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_PW_CONS, pw_cons, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_RATE_CTRL, rate_ctrl, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_REBOOT_ONU, reboot_onu, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_ADJ_TX_WL_FAIL, adj_tx_wl_fail,
		       TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_TUNE_REQ, tune_req, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_TUNE_COMPL, tune_compl, TC_PLOAM_DS_PAD);

	nla_nest_end(skb, cnt);
	return 0;
cancel:
	nla_nest_cancel(skb, cnt);
	return err;
}

/* Encode gtc counters in netlink message as a nested attribute */
static int gtc_ploam_ds_counters_nl_encode(struct sk_buff *skb,
					   void *buffer,
					   unsigned int size)
{
	struct pon_mbox_gtc_ploam_ds_counters *pon_output = buffer;
	struct nlattr *cnt = nla_nest_start(skb, PON_MBOX_A_CNT);
	int err;

	if (!cnt)
		return 1;

	/* GTC only */
	CNT_ENCODE_U64(TC_PLOAM_DS_US_OVERHEAD, us_ovh, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_ENC_PORT_ID, enc_gem_pid, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_REQ_PW, req_pw, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_NO_MESSAGE, no_msg, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_POPUP, popup, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_REQ_KEY, req_key, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_CONFIG_PORT_ID, cfg_port, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_PEE, phy_ee, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_PST, pst, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_BER_INTERVAL, ber, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_KEY_SWITCHING, key_swtime, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_EXT_BURST, ext_burst, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_PON_ID, pon_id, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_SWIFT_POPUP, swift_popup, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_RANGING_ADJ, rng_adj, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_CHG_PW_LVL, chg_pl, TC_PLOAM_DS_PAD);

	/* (X)GTC common */
	CNT_ENCODE_U64(TC_PLOAM_DS_ASS_ONU, ass_onu, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_RNG_TIME, rng_time, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_DEACT_ONU, deact_onu, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_DIS_SER, dis_ser, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_ASS_ALLOC, ass_alloc, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_SLP_ALLOW, slp_allow, TC_PLOAM_DS_PAD);
	CNT_ENCODE_U64(TC_PLOAM_DS_UNKNOWN, unknown, TC_PLOAM_DS_PAD);

	nla_nest_end(skb, cnt);
	return 0;
cancel:
	nla_nest_cancel(skb, cnt);
	return err;
}

/* Encode xgtc counters in netlink message as a nested attribute */
static int xgtc_ploam_us_counters_nl_encode(struct sk_buff *skb,
					    void *buffer,
					    unsigned int size)
{
	struct pon_mbox_xgtc_ploam_us_counters *pon_output = buffer;
	struct nlattr *cnt = nla_nest_start(skb, PON_MBOX_A_CNT);
	int err;

	if (!cnt)
		return 1;

	CNT_ENCODE_U64(TC_PLOAM_US_SER_ONU, ser_onu, TC_PLOAM_US_PAD);
	CNT_ENCODE_U64(TC_PLOAM_US_REG, reg, TC_PLOAM_US_PAD);
	CNT_ENCODE_U64(TC_PLOAM_US_KEY_REP, key_rep, TC_PLOAM_US_PAD);
	CNT_ENCODE_U64(TC_PLOAM_US_ACK, ack, TC_PLOAM_US_PAD);
	CNT_ENCODE_U64(TC_PLOAM_US_SLP_REQ, slp_req, TC_PLOAM_US_PAD);
	CNT_ENCODE_U64(TC_PLOAM_US_TUN_RES, tun_res, TC_PLOAM_US_PAD);
	CNT_ENCODE_U64(TC_PLOAM_US_PW_CONS, pw_cons, TC_PLOAM_US_PAD);
	CNT_ENCODE_U64(TC_PLOAM_US_RATE_RESP, rate_resp, TC_PLOAM_US_PAD);
	CNT_ENCODE_U64(TC_PLOAM_US_CPL_ERR, cpl_err, TC_PLOAM_US_PAD);
	CNT_ENCODE_U64(TC_PLOAM_US_TUN_RES_AN, tun_res_an, TC_PLOAM_US_PAD);
	CNT_ENCODE_U64(TC_PLOAM_US_TUN_RES_CRB, tun_res_crb, TC_PLOAM_US_PAD);

	nla_nest_end(skb, cnt);
	return 0;
cancel:
	nla_nest_cancel(skb, cnt);
	return err;
}

/* Encode gtc counters in netlink message as a nested attribute */
static int gtc_ploam_us_counters_nl_encode(struct sk_buff *skb,
					   void *buffer,
					   unsigned int size)
{
	struct pon_mbox_gtc_ploam_us_counters *pon_output = buffer;
	struct nlattr *cnt = nla_nest_start(skb, PON_MBOX_A_CNT);
	int err;

	if (!cnt)
		return 1;

	CNT_ENCODE_U64(TC_PLOAM_US_SER_ONU, ser_onu, TC_PLOAM_US_PAD);
	CNT_ENCODE_U64(TC_PLOAM_US_PASSWORD, password, TC_PLOAM_US_PAD);
	CNT_ENCODE_U64(TC_PLOAM_US_DYG_GASP, dyg_gasp, TC_PLOAM_US_PAD);
	CNT_ENCODE_U64(TC_PLOAM_US_NO_MSG, no_msg, TC_PLOAM_US_PAD);
	CNT_ENCODE_U64(TC_PLOAM_US_ENC_KEY, enc_key, TC_PLOAM_US_PAD);
	CNT_ENCODE_U64(TC_PLOAM_US_PHY_EE, phy_ee, TC_PLOAM_US_PAD);
	CNT_ENCODE_U64(TC_PLOAM_US_PST_MSG, pst_msg, TC_PLOAM_US_PAD);
	CNT_ENCODE_U64(TC_PLOAM_US_REM_ERR, rem_err, TC_PLOAM_US_PAD);
	CNT_ENCODE_U64(TC_PLOAM_US_ACK, ack, TC_PLOAM_US_PAD);
	CNT_ENCODE_U64(TC_PLOAM_US_SLP_REQ, slp_req, TC_PLOAM_US_PAD);

	nla_nest_end(skb, cnt);
	return 0;
cancel:
	nla_nest_cancel(skb, cnt);
	return err;
}

/** This function is used to read data about LODS counters. */
static int pon_mbox_genl_twdm_optic_pl_counters(struct sk_buff *skb,
						struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	struct pon_mbox_twdm_optic_pl_counters pon_output = {0};
	struct pon_msg_header header = {0};
	u8 dswlch_id;
	int err;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	if (attrs[PON_MBOX_D_DSWLCH_ID])
		dswlch_id = nla_get_u8(attrs[PON_MBOX_D_DSWLCH_ID]);
	else
		dswlch_id = PON_MBOX_D_DSWLCH_ID_CURR;

	header.rw = PONFW_READ;
	header.cmd = PONFW_TWDM_ONU_OPTIC_PL_COUNTERS_CMD_ID;
	err = pon_mbox_twdm_optic_pl_counters_update(dswlch_id,
						     &pon_output, pon);
	if (err == -PON_STATUS_FW_DUP_ERR)
		header.ack = PONFW_NACK_DUP;
	else if (err < 0)
		header.ack = PONFW_NACK;
	else
		header.ack = PONFW_ACK;

	header.src = PONFW_HOST;
	header.len = (err >= 0) ?
		(sizeof(struct pon_mbox_twdm_optic_pl_counters)
			/ PON_MBOX_BYTES_PER_WORD) : 0;

	err = pon_mbox_send_nl_response_encoded(
		pon, info->snd_portid, info->snd_seq, genl_info_net(info),
		&header, (void *)&pon_output, 0,
		twdm_optic_pl_counters_nl_encode);
	CHECK_FOR_NETLINK_RESPONSE_ERR(pon, err);

	return err;
}

/* Encode gtc counters in netlink message as a nested attribute */
static int twdm_tc_counters_nl_encode(struct sk_buff *skb, void *buffer,
				      unsigned int size)
{
	struct pon_mbox_twdm_tc_counters *pon_output = buffer;
	struct nlattr *cnt = NULL;
	struct nlattr *attr = NULL;
	int err;
	unsigned int i;

	cnt = nla_nest_start(skb, PON_MBOX_A_CNT);
	if (!cnt)
		return 1;

	attr = nla_nest_start(skb, PON_MBOX_A_CNT_TWDM_TC);
	if (!attr) {
		err = 1;
		goto cancel;
	}
	for (i = 0; i < ARRAY_SIZE(pon_output->tc_); ++i)
		CNT_ENCODE_U64(TWDM_TC_ITEM, tc_[i], TWDM_TC_ITEM_PAD);
	nla_nest_end(skb, attr);

	nla_nest_end(skb, cnt);
	return 0;
cancel:
	nla_nest_cancel(skb, cnt);
	return err;
}

/** This function is used to read data about TWDM Tuning Control counters. */
static int pon_mbox_genl_twdm_tc_counters(struct sk_buff *skb,
					  struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	struct pon_mbox_twdm_tc_counters pon_output = {0};
	struct pon_msg_header header = {0};
	u8 dswlch_id;
	int err;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	if (attrs[PON_MBOX_D_DSWLCH_ID])
		dswlch_id = nla_get_u8(attrs[PON_MBOX_D_DSWLCH_ID]);
	else
		dswlch_id = PON_MBOX_D_DSWLCH_ID_CURR;

	header.rw = PONFW_READ;
	header.cmd = PONFW_TWDM_TC_COUNTERS_CMD_ID;
	err = pon_mbox_twdm_tc_counters_update(dswlch_id,
					       &pon_output, pon);
	if (err == -PON_STATUS_FW_DUP_ERR)
		header.ack = PONFW_NACK_DUP;
	else if (err < 0)
		header.ack = PONFW_NACK;
	else
		header.ack = PONFW_ACK;

	header.src = PONFW_HOST;
	header.len = (err >= 0) ?
		(sizeof(struct pon_mbox_twdm_tc_counters)
			/ PON_MBOX_BYTES_PER_WORD) : 0;

	err = pon_mbox_send_nl_response_encoded(
		pon, info->snd_portid, info->snd_seq, genl_info_net(info),
		&header, (void *)&pon_output, 0, twdm_tc_counters_nl_encode);
	CHECK_FOR_NETLINK_RESPONSE_ERR(pon, err);

	return err;
}

/** This function is used to read data about XGTC PLOAM DS counters. */
static int pon_mbox_genl_xgtc_ploam_ds_counters(struct sk_buff *skb,
						struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	struct pon_mbox_xgtc_ploam_ds_counters pon_output = {0};
	struct pon_msg_header header = {0};
	u8 dswlch_id;
	int err;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	if (attrs[PON_MBOX_D_DSWLCH_ID])
		dswlch_id = nla_get_u8(attrs[PON_MBOX_D_DSWLCH_ID]);
	else
		dswlch_id = PON_MBOX_D_DSWLCH_ID_CURR;

	header.rw = PONFW_READ;
	header.cmd = PONFW_XGTC_PLOAM_DS_COUNTERS_CMD_ID;
	err = pon_mbox_xgtc_ploam_ds_counters_update(dswlch_id,
						     &pon_output, pon);
	if (err == -PON_STATUS_FW_DUP_ERR)
		header.ack = PONFW_NACK_DUP;
	else if (err < 0)
		header.ack = PONFW_NACK;
	else
		header.ack = PONFW_ACK;

	header.src = PONFW_HOST;
	header.len = (err >= 0) ?
		(sizeof(struct pon_mbox_xgtc_ploam_ds_counters)
			/ PON_MBOX_BYTES_PER_WORD) : 0;

	err = pon_mbox_send_nl_response_encoded(
		pon, info->snd_portid, info->snd_seq, genl_info_net(info),
		&header, (void *)&pon_output, 0,
		xgtc_ploam_ds_counters_nl_encode);
	CHECK_FOR_NETLINK_RESPONSE_ERR(pon, err);

	return err;
}

/** This function is used to read data about XGTC PLOAM DS counters. */
static int pon_mbox_genl_gtc_ploam_ds_counters(struct sk_buff *skb,
					       struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	struct pon_mbox_gtc_ploam_ds_counters pon_output = {0};
	struct pon_msg_header header = {0};
	u8 dswlch_id;
	int err;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	if (attrs[PON_MBOX_D_DSWLCH_ID])
		dswlch_id = nla_get_u8(attrs[PON_MBOX_D_DSWLCH_ID]);
	else
		dswlch_id = PON_MBOX_D_DSWLCH_ID_CURR;

	header.rw = PONFW_READ;
	header.cmd = PONFW_GTC_PLOAM_DS_COUNTERS_CMD_ID;
	err = pon_mbox_gtc_ploam_ds_counters_update(dswlch_id,
						    &pon_output, pon);
	if (err == -PON_STATUS_FW_DUP_ERR)
		header.ack = PONFW_NACK_DUP;
	else if (err < 0)
		header.ack = PONFW_NACK;
	else
		header.ack = PONFW_ACK;

	header.src = PONFW_HOST;
	header.len = (err >= 0) ?
		(sizeof(struct pon_mbox_gtc_ploam_ds_counters)
			/ PON_MBOX_BYTES_PER_WORD) : 0;

	err = pon_mbox_send_nl_response_encoded(
		pon, info->snd_portid, info->snd_seq, genl_info_net(info),
		&header, (void *)&pon_output, 0,
		gtc_ploam_ds_counters_nl_encode);
	CHECK_FOR_NETLINK_RESPONSE_ERR(pon, err);

	return err;
}

/** This function is used to read data about (X)GTC PLOAM DS counters. */
static int pon_mbox_genl_tc_ploam_ds_counters(struct sk_buff *skb,
					      struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	int ret;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	/* XGTC counters are supported by
	 * XGS-PON, XG-PON, NG-PON2
	 */
	if (pon->mode == PON_MODE_987_XGPON ||
	    pon->mode == PON_MODE_9807_XGSPON ||
	    pon->mode == PON_MODE_989_NGPON2_2G5 ||
	    pon->mode == PON_MODE_989_NGPON2_10G)
		ret = pon_mbox_genl_xgtc_ploam_ds_counters(skb, info);
	else
		ret = pon_mbox_genl_gtc_ploam_ds_counters(skb, info);

	return ret;
}

/** This function is used to read data about XGTC PLOAM US counters. */
static int pon_mbox_genl_xgtc_ploam_us_counters(struct sk_buff *skb,
						struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	struct pon_mbox_xgtc_ploam_us_counters pon_output = {0};
	struct pon_msg_header header = {0};
	u8 dswlch_id;
	int err;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	if (attrs[PON_MBOX_D_DSWLCH_ID])
		dswlch_id = nla_get_u8(attrs[PON_MBOX_D_DSWLCH_ID]);
	else
		dswlch_id = PON_MBOX_D_DSWLCH_ID_CURR;

	header.rw = PONFW_READ;
	header.cmd = PONFW_XGTC_PLOAM_US_COUNTERS_CMD_ID;
	err = pon_mbox_xgtc_ploam_us_counters_update(dswlch_id,
						     &pon_output, pon);
	if (err == -PON_STATUS_FW_DUP_ERR)
		header.ack = PONFW_NACK_DUP;
	else if (err < 0)
		header.ack = PONFW_NACK;
	else
		header.ack = PONFW_ACK;

	header.src = PONFW_HOST;
	header.len = (err >= 0) ?
		(sizeof(struct pon_mbox_xgtc_ploam_us_counters)
			/ PON_MBOX_BYTES_PER_WORD) : 0;

	err = pon_mbox_send_nl_response_encoded(
		pon, info->snd_portid, info->snd_seq, genl_info_net(info),
		&header, (void *)&pon_output, 0,
		xgtc_ploam_us_counters_nl_encode);
	CHECK_FOR_NETLINK_RESPONSE_ERR(pon, err);

	return err;
}

/** This function is used to read data about XGTC PLOAM US counters. */
static int pon_mbox_genl_gtc_ploam_us_counters(struct sk_buff *skb,
					       struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	struct pon_mbox_gtc_ploam_us_counters pon_output = {0};
	struct pon_msg_header header = {0};
	u8 dswlch_id;
	int err;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	if (attrs[PON_MBOX_D_DSWLCH_ID])
		dswlch_id = nla_get_u8(attrs[PON_MBOX_D_DSWLCH_ID]);
	else
		dswlch_id = PON_MBOX_D_DSWLCH_ID_CURR;

	header.rw = PONFW_READ;
	header.cmd = PONFW_GTC_PLOAM_US_COUNTERS_CMD_ID;
	err = pon_mbox_gtc_ploam_us_counters_update(dswlch_id,
						    &pon_output, pon);
	if (err == -PON_STATUS_FW_DUP_ERR)
		header.ack = PONFW_NACK_DUP;
	else if (err < 0)
		header.ack = PONFW_NACK;
	else
		header.ack = PONFW_ACK;

	header.src = PONFW_HOST;
	header.len = (err >= 0) ?
		(sizeof(struct pon_mbox_gtc_ploam_us_counters)
			/ PON_MBOX_BYTES_PER_WORD) : 0;

	err = pon_mbox_send_nl_response_encoded(
		pon, info->snd_portid, info->snd_seq, genl_info_net(info),
		&header, (void *)&pon_output, 0,
		gtc_ploam_us_counters_nl_encode);
	CHECK_FOR_NETLINK_RESPONSE_ERR(pon, err);

	return err;
}

/** This function is used to read data about (X)GTC PLOAM US counters. */
static int pon_mbox_genl_tc_ploam_us_counters(struct sk_buff *skb,
					      struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	int ret;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	/* XGTC counters are supported by
	 * XGS-PON, XG-PON, NG-PON2
	 */
	if (pon->mode == PON_MODE_987_XGPON ||
	    pon->mode == PON_MODE_9807_XGSPON ||
	    pon->mode == PON_MODE_989_NGPON2_2G5 ||
	    pon->mode == PON_MODE_989_NGPON2_10G)
		ret = pon_mbox_genl_xgtc_ploam_us_counters(skb, info);
	else
		ret = pon_mbox_genl_gtc_ploam_us_counters(skb, info);

	return ret;
}

/**
 * This function informs Firmware that there is something wrong with its TX
 * by setting TXERR bit - which should trigger Firmware to reset TX FIFO.
 * All messages in the mailbox will be lost.
 * input:
 *	pon - pointer to pon_mbox context structure
 *	timeout - number of tries to read answer from FW.
 *		  One try takes around 1-2[ms]
 * returns 0 if operation succeeded, and non 0 in case of any error
 */
static int pon_mbox_fw_tx_fifo_reset(struct pon_mbox *pon, int timeout)
{
	int ret;
	u32 cmd;
	int tries = 0;

	/* clear the tx buffer and accept and discard all messages from user */
	pon->in_reset = true;
	pon->tx_ready = true;
	wake_up_interruptible_all(&pon->tx_waitqueue);

	dev_dbg(pon->dev, "Set TXERR to reset TX FIFO\n");
	/* tell the firmware to reset the mailbox */
	ret = pon_mbox_write32(pon, PON_MBOX_CMD, PON_MBOX_STAT_TXERR);
	if (ret) {
		dev_err(pon->dev,
			"Firmware mailbox signal TX error command failed!\n");
		goto out;
	}

	/* wait till the firmware tells us that the reset was complete */
	do {
		usleep_range(1000, 2000);
		ret = pon_mbox_read32(pon, PON_MBOX_CMD, &cmd);
		if (ret) {
			dev_err(pon->dev, "Firmware mailbox read failed!\n");
			goto out;
		}
		tries++;
	} while ((cmd & PON_MBOX_STAT_TXERR) && (tries < timeout));

	if (tries >= timeout) {
		dev_err(pon->dev, "TXERR after reset not cleared after %i ms\n",
			timeout);
		ret = -EAGAIN;
	} else {
		dev_info(pon->dev, "Mailbox TX FIFO reset done\n");
	}
out:
	pon_mbox_release_pending_all(pon, -EIO);
	pon->in_reset = false;

	return ret;
}

/**
 * This function will reset the Mailbox, but not the PON_IP,
 * This should be called in case there is a problem
 * in the communication, but the SIP core does not need a reset.
 */
static int pon_mbox_genl_reset(struct sk_buff *skb, struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	int ret;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	dev_dbg(pon->dev,
		"Userspace has requested to reset the firmware mailbox\n");

	mutex_lock(&pon->irq_lock);

	ret = pon_mbox_fw_tx_fifo_reset(pon, 100);
	if (ret) {
		pr_err("firmware mailbox reset failed\n");
		mutex_unlock(&pon->irq_lock);
		return ret;
	}

	mutex_unlock(&pon->irq_lock);
	return 0;
}

/* Selects the PON FW file based on the HW version and the selected mode. */
static const char *pon_mbox_get_fw_filename(struct pon_mbox *pon,
					    enum pon_mode mode)
{
	bool gpon;

	switch (mode) {
	case PON_MODE_984_GPON:
	case PON_MODE_AON:
		gpon = true;
		break;
	case PON_MODE_987_XGPON:
	case PON_MODE_9807_XGSPON:
	case PON_MODE_989_NGPON2_2G5:
	case PON_MODE_989_NGPON2_10G:
		gpon = false;
		break;
	default:
		return NULL;
	}

	if (pon->hw_ver >= PON_MBOX_HW_VER_TPZ_EMU_TYPE) {
		/* Change to TPZ specific FW when it comes,
		   until then use URX-B FW for initial test phase */
		if (gpon)
			return PON_MBOX_FW_GPON_NAME_URX_B;
		else
			return PON_MBOX_FW_XPON_NAME_URX_B;
	} else if (pon->hw_ver >= PON_MBOX_HW_VER_URX_C_TYPE) {
		if (gpon)
			return PON_MBOX_FW_GPON_NAME_URX_C;
		else
			return PON_MBOX_FW_XPON_NAME_URX_C;
	} else if (pon->hw_ver >= PON_MBOX_HW_VER_URX_B_TYPE) {
		if (gpon)
			return PON_MBOX_FW_GPON_NAME_URX_B;
		else
			return PON_MBOX_FW_XPON_NAME_URX_B;
	} else if (pon->hw_ver >= PON_MBOX_HW_VER_URX_A_TYPE) {
		if (gpon)
			return PON_MBOX_FW_GPON_NAME_URX_A;
		else
			return PON_MBOX_FW_XPON_NAME_URX_A;
	} else if (pon->hw_ver >= PON_MBOX_HW_VER_B_TYPE) {
		if (gpon)
			return PON_MBOX_FW_GPON_NAME_B;
		else
			return PON_MBOX_FW_XPON_NAME_B;
	} else {
		if (gpon)
			return PON_MBOX_FW_GPON_NAME_A;
		else
			return PON_MBOX_FW_XPON_NAME_A;
	}
}

/** Unlocks or locks PON firmware download calling TEP */
static int pon_mbox_fw_dl_lock(bool lock)
{
#if !defined(SKIP_TEP) && !defined(CONFIG_X86_INTEL_LGM) && \
	!defined(CONFIG_SOC_LGM)
	if (lock)
		return sse_secure_pon_fw_dl_close();
	else
		return sse_secure_pon_fw_dl_open();
#endif
	return 0;
}

/**
 * Performs a reset of the PON_IP. The PON_IP will ask for a firmware after
 * this reset. The Netlink messages could contain an optional filename of a
 * firmware file in the data attribute. If it is not given, the previous
 * firmware file name will be used.
 */
static int pon_mbox_genl_reset_full(struct pon_mbox *pon)
{
	int ret;

#if !defined(SKIP_TEP) && \
	(defined(CONFIG_X86_INTEL_LGM) || defined(CONFIG_SOC_LGM))
	if (pon->urx_fw_dl_locked) {
		dev_err(pon->dev,
			"PON FW download is locked - reset not possible\n");
		return -EPERM;
	}
#endif

	/* clear the tx puffer and accept and discard all messages from user */
	pon->in_reset = true;
	pon->tx_ready = true;
	wake_up_interruptible_all(&pon->tx_waitqueue);

	/*
	 * We have to unlock the PON IP FW download before the reset,
	 * and can lock it again after the reset was done. This is sufficient
	 * to do the PON FW download in a secured system.
	 */
	ret = pon_mbox_fw_dl_lock(false);
	if (ret)
		dev_err(pon->dev,
			"secure pon FW download unlock failed: %i, ignore it\n",
			ret);

	ret = pon_mbox_write32(pon, PON_MBOX_CFG, PON_MBOX_CFG_RST);
	if (ret) {
		ret = pon_mbox_fw_dl_lock(true);
		if (ret)
			dev_err(pon->dev,
				"secure pon FW download lock failed: %i, ignore it\n",
				ret);
		return ret;
	}

	/*
	 * We have to wait some time till the PON IP boot ROM started,
	 * otherwise the PON FW can not be loaded reliable.
	 */
	msleep(50);
	ret = pon_mbox_fw_dl_lock(true);
	if (ret)
		dev_err(pon->dev,
			"secure pon FW download lock failed: %i, ignore it\n",
			ret);

	ret = pon_mbox_set_irq_mask(pon);
	if (ret)
		return ret;

	pon_mbox_cnt_reset(pon->cnt_state);

	return 0;
}

/**
 * Performs a reset of the PON_IP. The PON_IP will ask for a firmware after
 * this reset. The Netlink messages could contain an optional filename of a
 * firmware file in the data attribute. If it is not given, the previous
 * firmware file name will be used.
 */
static int pon_mbox_genl_reset_full_nl(struct sk_buff *skb,
				       struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	int ret;
	enum pon_mode mode;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	dev_dbg(pon->dev, "Userspace requested a firmware reset\n");

	if (attrs[PON_MBOX_A_MODE]) {
		mode = nla_get_u8(attrs[PON_MBOX_A_MODE]);
		if (pon_mbox_mode_change_callback_func) {
			ret = pon_mbox_mode_change_callback_func(mode);
			if (ret)
				return ret;
		}
		pon->mode = mode;
	}

	if (pon->mode != PON_MODE_AON) {
		if (pon->soc_spec->serdes_basic_init &&
		    !pon->serdes_cfg.srds_binit_done) {
			/* serdes basic init can only be executed once,
			 * otherwise ponmbox reset would fail
			 */
			ret = pon->soc_spec->serdes_basic_init(pon);
			if (ret)
				return ret;
			pon->serdes_cfg.srds_binit_done = true;
		}

		pon->serdes_cfg.srds_init_done = false;
		pon->lt_cfg.valid = false;
		pon->serdes_cfg.valid = false;
		pon->iop_cfg.valid = false;

		if (pon->soc_spec->ref_clk_sel) {
			ret = pon->soc_spec->ref_clk_sel(pon);
			if (ret)
				return ret;
		}
	}

	mutex_lock(&pon->irq_lock);
	ret = pon_mbox_genl_reset_full(pon);
	mutex_unlock(&pon->irq_lock);

	return ret;
}

static int pon_mbox_genl_link_disable(struct sk_buff *skb,
				      struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	int ret;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	dev_err(pon->dev, "Userspace requested disabling the HW\n");

	mutex_lock(&pon->irq_lock);

	/* clear the tx puffer and accept and discard all messages from user */
	pon->in_reset = true;
	pon->tx_ready = false;
	wake_up_interruptible_all(&pon->tx_waitqueue);

	ret = pon_mbox_write32(pon, PON_MBOX_CFG, 0);
	if (ret)
		goto err;

err:
	mutex_unlock(&pon->irq_lock);
	return ret;
}

static int is_netlink_capable(const struct genl_split_ops *ops,
			      struct sk_buff *skb,
			      struct genl_info *info)
{
	return netlink_capable(skb, CAP_NET_ADMIN) ? 0 : -EPERM;
}

/** List of the supported NetLink messages. */
static struct genl_split_ops pon_mbox_genl_ops[] = {
	{
		.cmd = PON_MBOX_C_MSG,
		.policy = pon_mbox_genl_policy,
		.doit = pon_mbox_genl_send,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_RESET,
		.policy = pon_mbox_genl_policy,
		.doit = pon_mbox_genl_reset,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_REG_READ,
		.policy = pon_mbox_genl_policy,
		.doit = pon_mbox_genl_reg_read,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_REG_WRITE,
		.policy = pon_mbox_genl_policy,
		.doit = pon_mbox_genl_reg_write,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_RESET_FULL,
		.policy = pon_mbox_genl_policy,
		.doit = pon_mbox_genl_reset_full_nl,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_LINK_DISABLE,
		.policy = pon_mbox_genl_policy,
		.doit = pon_mbox_genl_link_disable,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_ALLOC_ID_COUNTERS,
		.policy = counters_genl_policy,
		.doit = pon_mbox_genl_alloc_counters,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_ALLOC_LOST_COUNTERS,
		.policy = counters_genl_policy,
		.doit = pon_mbox_genl_alloc_lost_counters,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_GEM_PORT_COUNTERS,
		.policy = counters_genl_policy,
		.doit = pon_mbox_genl_gem_port_counters,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_GEM_ALL_COUNTERS,
		.policy = counters_genl_policy,
		.doit = pon_mbox_genl_gem_all_counters,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_GTC_COUNTERS,
		.policy = counters_genl_policy,
		.doit = pon_mbox_genl_gtc_counters,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_XGTC_COUNTERS,
		.policy = counters_genl_policy,
		.doit = pon_mbox_genl_xgtc_counters,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_ETH_RX_COUNTERS,
		.policy = counters_genl_policy,
		.doit = pon_mbox_genl_rx_eth_counters,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_ETH_TX_COUNTERS,
		.policy = counters_genl_policy,
		.doit = pon_mbox_genl_tx_eth_counters,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_LT_CONFIG,
		.policy = lt_config_genl_policy,
		.doit = pon_mbox_genl_lt_config,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_SRDS_CONFIG,
		.policy = serdes_config_genl_policy,
		.doit = pon_mbox_genl_serdes_config,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_SRDS_CONFIG_READ,
		.policy = empty_read_policy,
		.doit = pon_mbox_genl_serdes_config_read,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_IOP_CONFIG,
		.policy = iop_config_genl_policy,
		.doit = pon_mbox_genl_iop_config,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_PIN_CONFIG,
		.policy = pin_config_genl_policy,
		.doit = pon_mbox_genl_pin_config,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_UART_CONFIG,
		.policy = uart_config_genl_policy,
		.doit = pon_mbox_genl_uart_config,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_USER_MNGMT,
		.policy = user_mngmt_genlt_policy,
		.doit = pon_mbox_user_mngmt,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_BITERR_START,
		.policy = biterror_cnt_policy,
		.doit = pon_mbox_genl_biterr_start,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_BITERR_STOP,
		.policy = biterror_cnt_policy,
		.doit = pon_mbox_genl_biterr_stop,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_BITERR_READ,
		.policy = biterror_cnt_policy,
		.doit = pon_mbox_genl_biterr_read,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_MODE_READ,
		.policy = pon_mode_genl_policy,
		.doit = pon_mbox_genl_mode_read,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_DP_CONFIG,
		.policy = empty_read_policy,
		.doit = pon_mbox_genl_dp_config,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_CNT_TWDM_WLCHID,
		.policy = cnt_twdm_wlchid_genl_policy,
		.doit = pon_mbox_genl_cnt_twdm_wlchid_set,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_TWDM_LODS_COUNTERS,
		.policy = counters_genl_policy,
		.doit = pon_mbox_genl_twdm_lods_counters,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_TWDM_OPTIC_PL_COUNTERS,
		.policy = counters_genl_policy,
		.doit = pon_mbox_genl_twdm_optic_pl_counters,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_TWDM_TC_COUNTERS,
		.policy = counters_genl_policy,
		.doit = pon_mbox_genl_twdm_tc_counters,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_TC_PLOAM_DS_COUNTERS,
		.policy = counters_genl_policy,
		.doit = pon_mbox_genl_tc_ploam_ds_counters,
		.flags = GENL_CMD_CAP_DO,
	},
	{
		.cmd = PON_MBOX_C_TC_PLOAM_US_COUNTERS,
		.policy = counters_genl_policy,
		.doit = pon_mbox_genl_tc_ploam_us_counters,
		.flags = GENL_CMD_CAP_DO,
	},
};

static struct genl_family pon_mbox_genl_family = {
	.mcgrps = pon_mbox_genl_groups,
	.n_mcgrps = ARRAY_SIZE(pon_mbox_genl_groups),
#if (KERNEL_VERSION(6, 2, 0) > LINUX_VERSION_CODE)
	.ops = pon_mbox_genl_ops,
	.n_ops = ARRAY_SIZE(pon_mbox_genl_ops),
#else
	.split_ops = pon_mbox_genl_ops,
	.n_split_ops = ARRAY_SIZE(pon_mbox_genl_ops),
#endif
	.hdrsize = 0,
	.name = PON_MBOX_FAMILY,
	.version = 1,
	.maxattr = PON_MBOX_ATTR_MAX,
	.parallel_ops = 1,
	.pre_doit = is_netlink_capable,
};

/**
 * This function reads one arbitrary PON IP Host register.
 * This is called by a NetLink message.
 * This is used for debug purposes only and should not be used in production.
 */
static int pon_mbox_genl_reg_read(struct sk_buff *skb, struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	int ret;
	void *msg_head;
	u32 buf;
	u8 reg;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	if (!attrs[PON_MBOX_A_REG])
		return -EINVAL;

	reg = nla_get_u8(attrs[PON_MBOX_A_REG]);
	if (reg >= 0x20)
		return -EIO;

	mutex_lock(&pon->irq_lock);

	ret = pon_mbox_read32(pon, reg, &buf);
	if (ret) {
		mutex_unlock(&pon->irq_lock);
		return ret;
	}

	mutex_unlock(&pon->irq_lock);

	skb = genlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
	if (!skb) {
		dev_err(pon->dev, "cannot allocate netlink msg\n");
		return -ENOMEM;
	}

	msg_head = genlmsg_put_reply(skb, info, &pon_mbox_genl_family, 0,
				     PON_MBOX_C_REG_READ);
	if (!msg_head) {
		dev_err(pon->dev, "cannot create netlink msg\n");
		nlmsg_free(skb);
		return -ENOMEM;
	}

	ret = nla_put_u32(skb, PON_MBOX_A_REG_VAL, buf);
	if (ret) {
		dev_err(pon->dev, "cannot add data to netlink msg: %i\n", ret);
		genlmsg_cancel(skb, msg_head);
		nlmsg_free(skb);
		return ret;
	}

	genlmsg_end(skb, msg_head);

	ret = genlmsg_reply(skb, info);
	if (ret)
		dev_err(pon->dev, "cannot send reg read netlink msg: %i\n",
			ret);

	return ret;
}

/**
 * This function writes one arbitrary PON IP HOST register.
 * It is called through NetLink.
 * This is used for debug purposes only and should not be used in production.
 */
static int pon_mbox_genl_reg_write(struct sk_buff *skb, struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	int ret;
	void *msg_head;
	u32 buf = 0;
	u8 reg;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	if (!attrs[PON_MBOX_A_REG] || !attrs[PON_MBOX_A_REG_VAL])
		return -EINVAL;

	reg = nla_get_u8(attrs[PON_MBOX_A_REG]);
	buf = nla_get_u32(attrs[PON_MBOX_A_REG_VAL]);
	if (reg >= 0x20)
		return -EIO;

	mutex_lock(&pon->irq_lock);

	ret = pon_mbox_write32(pon, reg, buf);
	if (ret) {
		dev_err(pon->dev, "problem writing register (0x%x): %i\n",
			reg, ret);
		mutex_unlock(&pon->irq_lock);
		return ret;
	}

	mutex_unlock(&pon->irq_lock);

	skb = genlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
	if (!skb) {
		dev_err(pon->dev, "cannot allocate netlink msg\n");
		return -ENOMEM;
	}

	msg_head = genlmsg_put_reply(skb, info, &pon_mbox_genl_family, 0,
				     PON_MBOX_C_REG_WRITE);
	if (!msg_head) {
		dev_err(pon->dev, "cannot create netlink msg\n");
		nlmsg_free(skb);
		return -ENOMEM;
	}

	genlmsg_end(skb, msg_head);

	ret = genlmsg_reply(skb, info);
	if (ret)
		dev_err(pon->dev, "cannot send reg write netlink msg: %i\n",
			ret);

	return ret;
}

static int pon_mbox_genl_lt_config(struct sk_buff *skb, struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	int ret;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	if (attrs[PON_MBOX_LT_POWER_SAVE]) {
		pon->lt_cfg.loop_ps_en =
			(bool)nla_get_u8(attrs[PON_MBOX_LT_POWER_SAVE]);
	} else {
		dev_err(pon->dev, "loop timing power save missing\n");
		return -EINVAL;
	}

	pon->lt_cfg.valid = true;
	dev_dbg(pon->dev, "loop timing config received (%d)\n",
		pon->lt_cfg.loop_ps_en);

	if (pon->serdes_cfg.valid &&
	    pon->iop_cfg.valid &&
	    !pon->serdes_cfg.srds_init_done) {
		if (pon->soc_spec->pll5_init) {
			ret = pon->soc_spec->pll5_init(pon);
			if (ret)
				return ret;
		}

		ret = pon->soc_spec->serdes_init(pon);
		if (ret)
			return -ENXIO;

		pon->serdes_cfg.srds_init_done = true;
	}

	return 0;
}

static int pon_mbox_genl_serdes_config(struct sk_buff *skb,
				       struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	int i, ret;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	if (!pon->soc_spec->serdes_init || pon->mode == PON_MODE_AON)
		return 0;

	/* Index 0 is intentionally unused; SerDes config parameters start from
	 * index 1
	 */
	for (i = 1; i <= PON_MBOX_SRDS_MAX; i++) {
		if (attrs[i]) {
			switch (serdes_config_genl_policy[i].type) {
			case NLA_U32:
				pon->serdes_cfg.param[i] =
					nla_get_u32(attrs[i]);
			break;
			case NLA_U16:
				pon->serdes_cfg.param[i] =
					nla_get_u16(attrs[i]);
			break;
			case NLA_U8:
				pon->serdes_cfg.param[i] =
					nla_get_u8(attrs[i]);
			break;
			}
		} else {
			dev_err(pon->dev,
				"SerDes config parameter %s missing\n",
				serdes_config_params[i]);
			return -EINVAL;
		}

		dev_dbg(pon->dev, "SerDes config parameter %s = %d\n",
			serdes_config_params[i], pon->serdes_cfg.param[i]);
	}

	pon->serdes_cfg.valid = true;

	if (pon->lt_cfg.valid &&
	    pon->iop_cfg.valid &&
	    !pon->serdes_cfg.srds_init_done) {
		if (pon->soc_spec->pll5_init) {
			ret = pon->soc_spec->pll5_init(pon);
			if (ret)
				return ret;
		}

		ret = pon->soc_spec->serdes_init(pon);
		if (ret)
			return -ENXIO;

		pon->serdes_cfg.srds_init_done = true;
	}

	return 0;
}

static int pon_mbox_genl_iop_config(struct sk_buff *skb, struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	u32 iop_mask;
	int ret;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	if (attrs[PON_MBOX_IOP_MSK]) {
		iop_mask = nla_get_u32(attrs[PON_MBOX_IOP_MSK]);
		pon->iop_cfg.msk = *((struct ponfw_onu_interop_config *)(
					&iop_mask));
		pon->iop_cfg.valid = true;
		dev_dbg(pon->dev, "iop config received (0x%x)\n",
			iop_mask);
	} else {
		dev_err(pon->dev, "iop config missing\n");
		return -EINVAL;
	}

	if (pon->serdes_cfg.valid &&
	    pon->lt_cfg.valid &&
	    !pon->serdes_cfg.srds_init_done) {
		if (pon->soc_spec->pll5_init) {
			ret = pon->soc_spec->pll5_init(pon);
			if (ret)
				return ret;
		}

		ret = pon->soc_spec->serdes_init(pon);
		if (ret)
			return -ENXIO;

		pon->serdes_cfg.srds_init_done = true;
	}

	return 0;
}

/* Encode SerDes parameters in netlink message as a nested attribute */
static int serdes_config_nl_encode(struct sk_buff *skb,
				   void *buffer,
				   unsigned int size)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr *srds = nla_nest_start(skb, PON_MBOX_A_SRDS_READ);
	int i, ret = 0;

	if (!srds)
		return 1;

	for (i = 1; i <= PON_MBOX_SRDS_MAX; i++) {
		switch (serdes_config_genl_policy[i].type) {
		case NLA_U32:
			ret = nla_put_u32(skb, i, pon->serdes_cfg.param[i]);
		break;
		case NLA_U16:
			ret = nla_put_u16(skb, i, pon->serdes_cfg.param[i]);
		break;
		case NLA_U8:
			ret = nla_put_u8(skb, i, pon->serdes_cfg.param[i]);
		break;
		}

		if (ret) {
			dev_err(pon->dev,
				"cannot add data to netlink msg: %i\n", ret);
				nla_nest_cancel(skb, srds);
			return ret;
		}
	}

	nla_nest_end(skb, srds);
	return 0;
}

static int pon_mbox_genl_serdes_config_read(struct sk_buff *skb,
					    struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct pon_msg_header header = {0};
	int err;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	header.rw = PONFW_READ;
	header.cmd = 0xFF; /* Dummy */
	header.ack = PONFW_ACK;
	header.src = PONFW_HOST;
	header.len = PON_MBOX_SRDS_MAX / PON_MBOX_BYTES_PER_WORD;

	err = pon_mbox_send_nl_response_encoded(pon, info->snd_portid,
						info->snd_seq,
						genl_info_net(info), &header,
						NULL, 0,
						serdes_config_nl_encode);
	CHECK_FOR_NETLINK_RESPONSE_ERR(pon, err);
	return err;
}

static struct pinctrl *pinctrl_array[3];

static int pin_config_set(enum pon_mbox_gpio_pin_id pinsel,
			  enum pon_mbox_gpio_pin_status mode)
{
	struct pinctrl *pinctrl;
	struct pinctrl_state *s;
	static const char * const states[] = {
		[PON_MBOX_GPIO_PIN_STATUS_DEFAULT] = "default",
		[PON_MBOX_GPIO_PIN_STATUS_DISABLE] = "disable",
		[PON_MBOX_GPIO_PIN_STATUS_ENABLE] = "enable"
	};
	const char *state;
	int err;

	if (pinsel >= ARRAY_SIZE(pinctrl_array) || mode >= ARRAY_SIZE(states))
		return -EINVAL;

	pinctrl = pinctrl_array[pinsel];
	state = states[mode];

	if (!pinctrl || !state)
		return -ENODEV;

	s = pinctrl_lookup_state(pinctrl, state);
	if (IS_ERR(s))
		return PTR_ERR(s);

	err = pinctrl_select_state(pinctrl, s);
	if (err < 0)
		return err;

	return 0;
}

static int pon_mbox_genl_pin_config(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr **attrs = info->attrs;
	u8 pin_id, pin_status;
	void *msg_head;
	int ret;

	if (!attrs[PON_MBOX_A_PIN_ID] || !attrs[PON_MBOX_A_PIN_STATUS]) {
		pr_err("need both attributes: PIN_ID and PIN_STATUS\n");
		return -EINVAL;
	}

	pin_id = nla_get_u8(attrs[PON_MBOX_A_PIN_ID]);
	pin_status = nla_get_u8(attrs[PON_MBOX_A_PIN_STATUS]);

	/* pin_id and pin_status are checked in pin_config_set below */
	ret = pin_config_set(pin_id, pin_status);

	if (ret)
		pr_err("Unable to set pin: %u, status: %u, return: %d", pin_id,
		       pin_status, ret);

	skb = genlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
	if (!skb) {
		pr_err("cannot allocate netlink msg\n");
		return -ENOMEM;
	}

	msg_head = genlmsg_put_reply(skb, info, &pon_mbox_genl_family, 0,
				     PON_MBOX_C_MSG);

	if (!msg_head) {
		pr_err("cannot create netlink msg\n");
		nlmsg_free(skb);
		return -ENOMEM;
	}

	ret = nla_put_u8(skb, PON_MBOX_A_ACK,
			 ret == 0 ? PONFW_ACK : PONFW_NACK);

	if (ret) {
		pr_err("cannot add data to netlink msg: %i\n", ret);
		genlmsg_cancel(skb, msg_head);
		nlmsg_free(skb);
		return ret;
	}

	genlmsg_end(skb, msg_head);

	ret = genlmsg_reply(skb, info);

	if (ret)
		pr_err("cannot send pin config netlink msg: %i\n", ret);

	return 0;
}

static int pon_uart_config_set(struct pon_mbox *pon,
			       enum pon_mbox_uart_mode mode)
{
#if !defined(CONFIG_X86_INTEL_LGM) && !defined(CONFIG_SOC_LGM)
	unsigned int val;

	if (!pon->chiptop) {
		dev_err(pon->dev, "chiptop-syscon not available\n");
		return -EFAULT;
	}

	switch (mode) {
	case PON_MBOX_UART_OFF:
		/* UART0_SEL=0 and UTXD0_OEN=1 */
		val = BIT(21);
		break;
	case PON_MBOX_UART_ASC0:
		/* UART0_SEL=0 and UTXD0_OEN=0 */
		val = 0;
		break;
	case PON_MBOX_UART_ASC1:
		/* UART0_SEL=4 and UTXD0_OEN=0 */
		val = 4 << 12;
		break;
	default:
		return -EINVAL;
	}

	pon->uart_mode = mode;
	return regmap_update_bits(pon->chiptop, CHIPTOP_IFMUX_CFG,
				  GENMASK(14, 12) | BIT(21), val);
#else
	return -EPERM;
#endif
}

static int pon_mbox_genl_uart_config(struct sk_buff *skb,
				     struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	u8 uart_mode, cmd = PON_MBOX_C_MSG, ack = PONFW_ACK;
	void *msg_head;
	int ret = 0;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	if (attrs[PON_MBOX_UART_MODE]) {
		/* no range check here, is done in side the function */
		uart_mode = nla_get_u8(attrs[PON_MBOX_UART_MODE]);
		ret = pon_uart_config_set(pon, uart_mode);
		if (ret) {
			dev_err(pon->dev, "Failed to switch uart to %u, err=%d\n",
				uart_mode, ret);
			ack = PONFW_NACK;
		}
	}

	skb = genlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
	if (!skb) {
		dev_err(pon->dev, "cannot allocate netlink msg\n");
		return -ENOMEM;
	}

	msg_head = genlmsg_put_reply(skb, info, &pon_mbox_genl_family, 0, cmd);
	if (!msg_head) {
		dev_err(pon->dev, "cannot create netlink msg\n");
		nlmsg_free(skb);
		return -ENOMEM;
	}

	ret = nla_put_u8(skb, PON_MBOX_A_ACK, ack);

	if (!attrs[PON_MBOX_UART_MODE] && !ret) {
		/* no attribute, this is the get, reply with value
		 * use existing PON_MBOX_A_MODE
		 */
		ret = nla_put_u8(skb, PON_MBOX_A_MODE, pon->uart_mode);
	}

	if (ret) {
		dev_err(pon->dev, "cannot add data to netlink msg: %i\n", ret);
		genlmsg_cancel(skb, msg_head);
		nlmsg_free(skb);
		return ret;
	}

	genlmsg_end(skb, msg_head);

	ret = genlmsg_reply(skb, info);
	if (ret)
		dev_err(pon->dev, "cannot send uart config netlink msg: %i\n",
			ret);

	return 0;
}

static int pon_mbox_bit_error_counter_get(u32 *param)
{
	struct pon_mbox *pon = pon_mbox_dev;
	int ret_fw;
	struct ponfw_debug_bit_error_counter counter = { 0 };
	u32 bit_errors;

	if (!param)
		return -EFAULT;

	ret_fw = pon_mbox_send(PONFW_DEBUG_BIT_ERROR_COUNTER_CMD_ID,
			       PONFW_READ, NULL, 0, &counter,
			       sizeof(counter));
	if (ret_fw < 0) {
		dev_err(pon->dev, "getting bit error counter failed: %i\n",
			ret_fw);
		return ret_fw;
	}

	if (counter.berr_ofl == 0)
		/* If the overflow bit is not set, report the value directly
		 * from the firmware message:
		 */
		bit_errors = counter.berr_cnt;
	else if (counter.berr_cnt < 0x7FFF)
		/* If the overflow bit is set but the range is not exceeded,
		 * multiply by 2^7.
		 */
		bit_errors = counter.berr_cnt << 7;
	else
		bit_errors = UINT_MAX;

	*param = bit_errors;
	return 0;
}

static int pon_mbox_bit_error_reply(struct sk_buff *skb, struct genl_info *info,
				    u8 ack, u64 *cnt, u32 *time,
				    u8 *counter_status)
{
	struct pon_mbox *pon = pon_mbox_dev;
	void *msg_head;
	int ret = 0;

	skb = genlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
	if (!skb) {
		dev_err(pon->dev, "cannot allocate netlink msg\n");
		return -ENOMEM;
	}

	msg_head = genlmsg_put_reply(skb, info, &pon_mbox_genl_family,
				     0, PON_MBOX_C_MSG);
	if (!msg_head) {
		dev_err(pon->dev, "cannot create netlink msg\n");
		nlmsg_free(skb);
		return -ENOMEM;
	}

	ret = nla_put_u8(skb, PON_MBOX_A_ACK, ack);
	if (!ret && cnt)
		ret = nla_put_u64_64bit(skb, PON_MBOX_A_BITERR_CNT,
					*cnt,
					PON_MBOX_A_BITERR_PAD);
	if (!ret && time)
		ret = nla_put_u32(skb, PON_MBOX_A_BITERR_TIME, *time);
	if (!ret && counter_status)
		ret = nla_put_u8(skb, PON_MBOX_A_BITERR_STATUS,
				 *counter_status);

	if (ret) {
		dev_err(pon->dev, "cannot add data to netlink msg: %i\n", ret);
		genlmsg_cancel(skb, msg_head);
		nlmsg_free(skb);
		return ret;
	}

	genlmsg_end(skb, msg_head);

	ret = genlmsg_reply(skb, info);
	if (ret)
		dev_err(pon->dev, "cannot send biterr netlink msg: %i\n",
			ret);

	return ret;
}

static int pon_mbox_biterr_thread(void *param)
{
	struct pon_mbox *pon = param;
	u32 bit_error = 0;
	int ret;

	while (!kthread_should_stop()) {
		ret = pon_mbox_bit_error_counter_get(&bit_error);
		if (!ret) {
			mutex_lock(&pon->biterror_lock);
			if (bit_error == UINT_MAX)
				pon->biterror_counter = ~0ULL;
			else
				pon->biterror_counter += bit_error;
			pon->biterror_update_time = ktime_get_ns();
			mutex_unlock(&pon->biterror_lock);
			if (pon->biterror_counter == ~0ULL) {
				/* Overflow, exit the thread */
				pon->biterror_count_thread = NULL;
				return 0;
			}
		}

		msleep(SERDES_BIT_ERROR_COUNTER_POLLING_INTERVAL);
	}
	return 0;
}

static int pon_mbox_genl_biterr_start(struct sk_buff *skb,
				      struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	u8 ack = PONFW_ACK;
	u32 bit_error = 0;
	u8 counter_status = 0;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	/* initial read to clear the counter */
	(void)pon_mbox_bit_error_counter_get(&bit_error);

	mutex_lock(&pon->biterror_lock);
	pon->biterror_counter = 0;
	pon->biterror_start_time = ktime_get_ns();
	pon->biterror_update_time = pon->biterror_start_time;
	mutex_unlock(&pon->biterror_lock);

	if (!pon->biterror_count_thread) {
		pon->biterror_count_thread = kthread_run(pon_mbox_biterr_thread,
							 pon, "pon_biterr");
		if (IS_ERR(pon->biterror_count_thread)) {
			pon->biterror_count_thread = NULL;
			ack = PONFW_CMDERR;
		}
		counter_status = 1;
	}
	return pon_mbox_bit_error_reply(skb, info, ack, NULL, NULL,
					&counter_status);
}

static int pon_mbox_genl_biterr_stop(struct sk_buff *skb,
				     struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	u8 ack = PONFW_ACK;
	u64 biterror_counter;
	u32 timediff;
	u8 counter_status;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	if (pon->biterror_count_thread) {
		kthread_stop(pon->biterror_count_thread);

		pon->biterror_count_thread = NULL;
		counter_status = 0;

		mutex_lock(&pon->biterror_lock);
		biterror_counter = pon->biterror_counter;
		/* time difference in ms */
		timediff = div_u64(pon->biterror_update_time -
				   pon->biterror_start_time,
				   1000000);
		mutex_unlock(&pon->biterror_lock);
	} else {
		/* If counter is already stopped, return empty status */
		biterror_counter = 0;
		timediff = 0;
		counter_status = 0;
	}

	return pon_mbox_bit_error_reply(skb, info, ack,
					&biterror_counter, &timediff,
					&counter_status);
}

static int pon_mbox_genl_biterr_read(struct sk_buff *skb,
				     struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	u8 ack = PONFW_ACK;
	u64 biterror_counter;
	u32 timediff;
	u8 counter_status = 0;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	mutex_lock(&pon->biterror_lock);
	biterror_counter = pon->biterror_counter;
	/* time difference in ms */
	timediff = div_u64(pon->biterror_update_time - pon->biterror_start_time,
			   1000000);
	mutex_unlock(&pon->biterror_lock);

	counter_status = pon->biterror_count_thread ? 1 : 0;

	return pon_mbox_bit_error_reply(skb, info, ack,
					&biterror_counter, &timediff,
					&counter_status);
}

static inline void pon_mbox_get_random_bytes(void *buf, size_t nbytes)
{
#if defined(SKIP_TEP) || defined(CONFIG_X86_INTEL_LGM) || \
	defined(CONFIG_SOC_LGM)
	/* get random bytes from linux */
	get_random_bytes(buf, nbytes);
#else
	int ret;

	ret = sse_get_random_bytes(buf, nbytes);
	if (ret) {
		pr_err("failed to get random from TEP, use Linux: %i\n", ret);
		get_random_bytes(buf, nbytes);
	}
#endif
}

static void handle_ponip_event_rand_num(void *module, const void *msg,
					size_t msg_len, u8 seq)
{
	struct pon_mbox *pon = module;
	struct ponfw_rand_num rand = {0};
	ssize_t ret_fw = 0;

	pon_mbox_get_random_bytes(&rand, sizeof(rand));

	ret_fw = pon_mbox_write_msg(pon, (const char *)&rand, sizeof(rand),
				    PONFW_READ, PONFW_RAND_NUM_CMD_ID,
				    PONFW_ACK, 0, seq, NULL,
				    PON_MBOX_MSG_ORIGIN_KERNEL, NULL, NULL, 0);

	if (ret_fw < 0) {
		dev_err(pon->dev, "Sending random numbers to FW failed: %zi\n",
			ret_fw);
	}
}

static int pon_mbox_fatal_err_handle(struct pon_mbox *pon,
				     const char *alarm_name)
{
	int ret;

	dev_err(pon->dev, "Received %s alarm, trigger WAN subsystem reset\n",
		alarm_name);

	if (pon->mode == PON_MODE_AON)
		return 0;

	pon->serdes_cfg.srds_init_done = false;

	ret = reset_control_assert(pon->reset_wanss);
	if (ret)
		return ret;
	/* wait to make sure reset is gonna be executed */
	usleep_range(200, 400);
	ret = reset_control_deassert(pon->reset_wanss);
	if (ret)
		return ret;

	ret = reset_control_assert(pon->reset_serdes);
	if (ret)
		return ret;

	if (!pon->soc_spec->serdes_init) {
		usleep_range(50, 100);
		ret = reset_control_deassert(pon->reset_serdes);
		if (ret)
			return ret;
	}

	if (pon->soc_spec->serdes_basic_init) {
		ret = pon->soc_spec->serdes_basic_init(pon);
		if (ret)
			return ret;
	}

	if (pon->soc_spec->ref_clk_sel) {
		ret = pon->soc_spec->ref_clk_sel(pon);
		if (ret)
			return ret;
	}

	mutex_lock(&pon->irq_lock);
	ret = pon_mbox_genl_reset_full(pon);
	mutex_unlock(&pon->irq_lock);

	return ret;
}

static void pon_mbox_handle_alarm_report(void *module, const void *msg,
					 size_t msg_len, u8 seq)
{
	struct pon_mbox *pon = module;
	const struct ponfw_report_alarm *alarm = msg;
	struct device *dev = pon->dev;
	int ret;

	if (msg_len != sizeof(*alarm)) {
		dev_err(dev, "Alarm set with unexpected size: %zu\n",
			msg_len);
		return;
	}

	switch (alarm->alarm_id) {
	case PON_ALARM_ID_STATIC_PCE:
		ret = pon_mbox_fatal_err_handle(pon,
						"persistent crossbar access error");
		if (ret) {
			dev_err(pon->dev, "Error during fatal err handling: %d\n",
				ret);
			return;
		}
		break;
	case PON_ALARM_ID_STATIC_PSE:
		if (pon->serdes_error) {
			dev_err(pon->dev, "persistent SerDes error received, ignoring it\n");
			break;
		}
		pon->serdes_error = true;
		ret = pon_mbox_fatal_err_handle(pon, "persistent SerDes error");
		if (ret) {
			dev_err(pon->dev, "Error during fatal err handling: %d\n",
				ret);
			return;
		}
		break;
	case PON_ALARM_ID_EDGE_FWVERR:
		dev_err(pon->dev, "Firmware version error, the loaded firmware does not fit to the PON IP hardware version.\n");
		break;
	default:
		/* Ignore other alarms */
		break;
	}
}

static void pon_mbox_handle_alarm_clear(void *module, const void *msg,
					size_t msg_len, u8 seq)
{
	struct pon_mbox *pon = module;
	const struct ponfw_clear_alarm *alarm = msg;
	struct device *dev = pon->dev;

	if (msg_len != sizeof(*alarm)) {
		dev_err(dev, "Alarm clear with unexpected size: %zu\n",
			msg_len);
		return;
	}
}

static void pon_mbox_handle_ploam_state(void *module, const void *msg,
					size_t msg_len, u8 seq)
{
	struct pon_mbox *pon = module;
	const struct ponfw_ploam_state *ploam = msg;
	struct device *dev = pon->dev;

	if (msg_len != sizeof(*ploam)) {
		dev_err(dev, "PLOAM state event with unexpected size: %zu\n",
			msg_len);
		return;
	}

	pon_mbox_dev->ploam_state = ploam->ploam_act;
	dev_dbg(dev, "PLOAM state changed to %u\n",
		pon_mbox_dev->ploam_state);

	/* check if this is the first PLOAM state change from the init state */
	switch (pon->mode) {
	case PON_MODE_984_GPON:
		if (ploam->ploam_act == 50)
			pon->serdes_error = false;
		break;
	case PON_MODE_987_XGPON:
	case PON_MODE_9807_XGSPON:
	case PON_MODE_989_NGPON2_2G5:
	case PON_MODE_989_NGPON2_10G:
		if (ploam->ploam_act == 51 || ploam->ploam_act == 52)
			pon->serdes_error = false;
		break;
	default:
		dev_err(pon->dev, "unsupported pon_mode selected: %i\n",
			pon->mode);
	}

	/* Loop timing is done in FW only. */
}

struct pon_event_handler_entry {
	u32 cmd_id;
	void (*handle_event)(void *module, const void *msg, size_t msg_len,
			     u8 seq);
	/* Pointer to the module that handles the event */
	void *module;
	/* Node for the hash list */
	struct hlist_node node;
};

/* Number of bits for the hash table */
#define PON_MBOX_HT_BITS 4
/* Hash table for event handlers */
static DEFINE_HASHTABLE(pon_event_handler_hash_table, PON_MBOX_HT_BITS);
/* Lock for synchronizing access to the hash table */
static DECLARE_RWSEM(pon_event_handler_lock);

void pon_mbox_register_event_handler(u32 cmd_id,
				     void (*handle_event)(void *module,
							  const void *msg,
							  size_t msg_len,
							  u8 seq),
				     void *module)
{
	struct pon_event_handler_entry *entry;

	if (!handle_event || !module)
		return;

	entry = kcalloc(1, sizeof(*entry), GFP_KERNEL);
	if (!entry)
		return;

	entry->cmd_id = cmd_id;
	entry->handle_event = handle_event;
	entry->module = module;

	pr_debug("Registered event handler for cmd_id: 0x%04x (hash idx %d)\n",
		 cmd_id, hash_32(cmd_id, PON_MBOX_HT_BITS));

	down_write(&pon_event_handler_lock);
	hash_add(pon_event_handler_hash_table, &entry->node, cmd_id);
	up_write(&pon_event_handler_lock);
}
EXPORT_SYMBOL(pon_mbox_register_event_handler);

void pon_mbox_unregister_event_handler_module(void *module)
{
	struct pon_event_handler_entry *entry;
	struct hlist_node *tmp;
	int i;

	if (!module)
		return;

	down_write(&pon_event_handler_lock);
	hash_for_each_safe(pon_event_handler_hash_table, i, tmp, entry, node) {
		if (entry->module == module) {
			pr_debug("Unregistered event handler for cmd_id: 0x%04x\n",
				 entry->cmd_id);
			hash_del(&entry->node);
			kfree(entry);
		}
	}
	up_write(&pon_event_handler_lock);
}
EXPORT_SYMBOL(pon_mbox_unregister_event_handler_module);

static void pon_mbox_unregister_all_event_handlers(void)
{
	struct pon_event_handler_entry *entry;
	struct hlist_node *tmp;
	int i;

	down_write(&pon_event_handler_lock);
	hash_for_each_safe(pon_event_handler_hash_table, i, tmp, entry, node) {
		pr_debug("Unregistered event handler for cmd_id: 0x%04x\n",
			 entry->cmd_id);
		hash_del(&entry->node);
		kfree(entry);
	}
	up_write(&pon_event_handler_lock);
}

/* Check if the command ID is valid by looking it up in the hash table */
static int cmd_id_is_in_table(u32 cmd_id)
{
	struct pon_event_handler_entry *entry;

	down_read(&pon_event_handler_lock);
	hash_for_each_possible(pon_event_handler_hash_table, entry, node,
			       cmd_id) {
		if (entry->cmd_id == cmd_id) {
			up_read(&pon_event_handler_lock);
			return 1; /* Command ID is valid */
		}
	}
	up_read(&pon_event_handler_lock);
	return 0;  /* Command ID is not valid */
}

/* Structure containing the FW event information for the Linux work queue. */
struct pon_mbox_event {
	struct work_struct work;
	struct pon_mbox *pon;
	struct pon_msg_header header;
	size_t msg_len;
#ifdef DEBUG_MBOX_RESPONSE_TIME
	unsigned long debug_jiffies_start;
#endif
	char msg[];
};

/**
 * @brief Handles an event received from the PON FW by looking up the command ID
 * in the hash table and calling the corresponding handler if found.
 *
 * This function is called when an event is received from the PON FW. It looks
 * up the command ID in the hash table and calls all handlers if found.
 *
 * @param event Pointer to the structure containing event details.
 */
static void pon_mbox_handle_event_by_hashtable(struct pon_mbox_event *event)
{
	struct pon_event_handler_entry *entry;
	struct pon_msg_header *header = &event->header;
	int cmd_id = header->cmd;
	void *msg = event->msg;
	size_t msg_len = event->msg_len;

	down_read(&pon_event_handler_lock);
	/* Call all handlers registered for this command ID */
	hash_for_each_possible(pon_event_handler_hash_table, entry, node,
			       cmd_id) {
		if (entry->cmd_id == cmd_id) {
			pr_debug("%s: cmd_id=%u, seq=%u %p %pBb\n", __func__,
				 cmd_id, header->seq, entry->module,
				 entry->handle_event);
			entry->handle_event(entry->module, msg, msg_len,
					    header->seq);
		}
	}
	up_read(&pon_event_handler_lock);
}

static void pon_mbox_handle_event(struct work_struct *work)
{
	struct pon_mbox_event *event =
		container_of(work, struct pon_mbox_event, work);
#ifdef DEBUG_MBOX_RESPONSE_TIME
	unsigned int timer;

	timer = jiffies_to_usecs(jiffies - event->debug_jiffies_start);
	if (time_is_before_jiffies(event->debug_jiffies_start +
					msecs_to_jiffies(100)))
		dev_err(event->pon->dev,
			"Event (0x%x) started more than 100ms later: %d us\n",
			event->header.cmd, timer);
	event->debug_jiffies_start = jiffies;
#endif

	pon_mbox_handle_event_by_hashtable(event);

#ifdef DEBUG_MBOX_RESPONSE_TIME
	timer = jiffies_to_usecs(jiffies - event->debug_jiffies_start);
	if (time_is_before_jiffies(event->debug_jiffies_start +
				   msecs_to_jiffies(100)))
		dev_err(event->pon->dev,
			"Event (0x%x) took more than 100ms to process: %d us\n",
			event->header.cmd, timer);
#endif
	kfree(event);
}

/*
 * This adds a events which was received from the PON FW to a system work
 * queue so it can be handled asynchronously.
 * We cannot handle it in all cases directly here because this function is
 * executed in the PON mbox driver irq lock and it is not possible to send
 * a new message inside of this lock. This also has the improvement that the
 * handling of these events does not block the handling of new messages.
 */
static void pon_mbox_queue_event(struct pon_mbox *pon,
				 struct pon_msg_header *header, char *msg,
				 size_t msg_len)
{
	struct pon_mbox_event *event;
	bool ret;

	/* If the command ID is not registered, we return early. */
	if (!cmd_id_is_in_table(header->cmd))
		return;

	event = kzalloc(sizeof(*event) + msg_len, GFP_KERNEL);
	if (!event)
		return;

	event->pon = pon;
	event->header = *header;
	event->msg_len = msg_len;
#ifdef DEBUG_MBOX_RESPONSE_TIME
	event->debug_jiffies_start = jiffies;
#endif
	memcpy(event->msg, msg, msg_len);
	INIT_WORK(&event->work, pon_mbox_handle_event);

	ret = queue_work(system_long_wq, &event->work);
	if (!ret)
		dev_err(pon->dev, "Can not queue work\n");
}

/**
 * Send response to Netlink, both unicast and multicast. In case of multicast
 * message, 'net' variable is ignored.
 * Variable 'msg' is freed after use.
 * Pointer 'header' have to point to valid structure.
 * Attribute 'flags' distinguishes whether event to be called is fake or not.
 * Pointer 'encode' Is a function that should analyze the 'msg' and put the
 *                attributes into netlink message. If this is NULL, then it
 *                means that 'msg' will be put into netlink message using nl_put
 */
static int pon_mbox_send_nl_response_encoded(struct pon_mbox *pon,
					     u32 snd_portid,
					     u32 snd_seq, struct net *net,
					     struct pon_msg_header *header,
					     char *msg, u32 flags,
					     pon_mbox_nl_encode_fn *encode)
{
	struct sk_buff *skb;
	void *msg_head;
	int ret = 0;
	int plen = header->len * PON_MBOX_BYTES_PER_WORD;

	/* prepare Netlink response */
	skb = genlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
	if (!skb) {
		dev_err(pon->dev, "cannot allocate netlink msg\n");
		return -ENOMEM;
	}

	msg_head = genlmsg_put(skb, snd_portid, snd_seq, &pon_mbox_genl_family,
			       0, PON_MBOX_C_MSG);
	if (!msg_head) {
		dev_err(pon->dev, "cannot create netlink msg\n");
		nlmsg_free(skb);
		return -ENOMEM;
	}

	/* add message only when there is one */
	if (plen > 0)
		/* Encode payload in netlink message. */
		ret = encode(skb, msg, plen);

	if (!ret)
		ret = nla_put_u8(skb, PON_MBOX_A_READ_WRITE, header->rw);
	if (!ret)
		ret = nla_put_u16(skb, PON_MBOX_A_COMMAND, header->cmd);
	if (!ret)
		ret = nla_put_u8(skb, PON_MBOX_A_ACK, header->ack);
	if (flags && !ret)
		ret = nla_put_u32(skb, PON_MBOX_A_FLAGS, flags);

	if (ret) {
		dev_err(pon->dev, "cannot add data to netlink msg: %i\n", ret);
		genlmsg_cancel(skb, msg_head);
		nlmsg_free(skb);
		return ret;
	}

	genlmsg_end(skb, msg_head);

	/* send Netlink response */
	if (header->src == PONFW_HOST) {
		ret = genlmsg_unicast(net, skb, snd_portid);
		/* ignore if userspace is gone */
		if (ret == -ESRCH)
			ret = 0;
		else if (ret)
			dev_err(pon->dev, "cannot send FW unicast netlink msg to %u: %i\n",
				snd_portid, ret);
	} else {
		ret = genlmsg_multicast(&pon_mbox_genl_family, skb, 0, 0,
					GFP_KERNEL);
		/* ignore if userspace is gone */
		if (ret == -ESRCH)
			ret = 0;
		else if (ret)
			dev_err_ratelimited(
				pon->dev,
				"cannot send FW multicast netlink msg: %i\n",
				ret);
	}

	return ret;
}

static int pon_mbox_user_mngmt(struct sk_buff *skb, struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	kuid_t user = INVALID_UID, invalid_uid = INVALID_UID;
	u16 cmd_group = 0xFF;
	u32 uid_val = 0;
	u8 revoke_grant = 0, ack = PONFW_NACK;
	int i, ret = 0;
	void *msg_head;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	/* Command accessible only for root user */
	if (!uid_eq(sock_i_uid(skb->sk), GLOBAL_ROOT_UID)) {
		pr_err("Permission denied!\n");
		return -EPERM;
	}

	if (attrs[PON_MBOX_A_USER_MNGMT_CMD_GROUP])
		cmd_group = nla_get_u16(attrs[PON_MBOX_A_USER_MNGMT_CMD_GROUP]);

	if (cmd_group > PON_MBOX_PERM_MAX) {
		pr_err("Invalid command group\n");
		return -EINVAL;
	}

	if (attrs[PON_MBOX_A_USER_MNGMT_REVOKE_GRANT])
		revoke_grant =
			nla_get_u8(attrs[PON_MBOX_A_USER_MNGMT_REVOKE_GRANT]);

	if (attrs[PON_MBOX_A_USER_MNGMT_USER])
		uid_val = nla_get_u32(attrs[PON_MBOX_A_USER_MNGMT_USER]);

	user = KUIDT_INIT(uid_val);

	/* 0: revoke access -- 1: grant access */
	if (!revoke_grant) {
		for (i = 0; i < PON_MBOX_MAX_CMD_USERS; i++) {
			if (uid_eq(user,
				   pon_mbox_perm_list[cmd_group].uid_list[i])) {
				pon_mbox_perm_list[cmd_group].uid_list[i] =
					INVALID_UID;
				ack = PONFW_ACK;
				break;
			}
		}
		/* If user not found, only warn and return Ack */
		if (ack != PONFW_ACK)
			pr_warn("User not found on permission list!\n");
		ack = PONFW_ACK;
	} else {
		for (i = 0; i < PON_MBOX_MAX_CMD_USERS; i++) {
			if (uid_eq(pon_mbox_perm_list[cmd_group].uid_list[i],
				   invalid_uid)) {
				pon_mbox_perm_list[cmd_group].uid_list[i] =
					KUIDT_INIT(uid_val);
				ack = PONFW_ACK;
				break;
			}
		}
		if (ack != PONFW_ACK)
			pr_err("User list reached maximum user limit!\n");
	}

	/* Generating netlink response */
	skb = genlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
	if (!skb) {
		pr_err("cannot allocate netlink msg\n");
		return -ENOMEM;
	}

	msg_head = genlmsg_put_reply(skb, info, &pon_mbox_genl_family, 0,
				     PON_MBOX_C_MSG);
	if (!msg_head) {
		pr_err("cannot create netlink msg\n");
		nlmsg_free(skb);
		return -ENOMEM;
	}

	ret = nla_put_u8(skb, PON_MBOX_A_ACK, ack);
	if (ret) {
		pr_err("cannot add data to netlink msg: %i\n", ret);
		genlmsg_cancel(skb, msg_head);
		nlmsg_free(skb);
		return ret;
	}

	genlmsg_end(skb, msg_head);
	ret = genlmsg_reply(skb, info);
	if (ret)
		pr_err("cannot send user mngmt netlink msg: %i\n", ret);

	return ret;
}

/**
 * This function receives the NetLink message which is used to read PON mode.
 */
static int pon_mbox_genl_mode_read(struct sk_buff *skb, struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	u8 cmd = PON_MBOX_C_MSG, ack = PONFW_ACK;
	void *msg_head;
	int ret = 0;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	skb = genlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
	if (!skb) {
		dev_err(pon->dev, "cannot allocate netlink msg\n");
		return -ENOMEM;
	}

	msg_head = genlmsg_put_reply(skb, info, &pon_mbox_genl_family, 0, cmd);
	if (!msg_head) {
		dev_err(pon->dev, "cannot create netlink msg\n");
		nlmsg_free(skb);
		return -ENOMEM;
	}

	ret = nla_put_u8(skb, PON_MBOX_A_ACK, ack);

	if (!ret)
		ret = nla_put_u8(skb, PON_MBOX_A_PON_MODE, pon->mode);

	if (ret) {
		dev_err(pon->dev, "cannot add data to netlink msg: %i\n", ret);
		genlmsg_cancel(skb, msg_head);
		nlmsg_free(skb);
		return ret;
	}

	genlmsg_end(skb, msg_head);

	ret = genlmsg_reply(skb, info);
	if (ret)
		dev_err(pon->dev, "cannot send mode read netlink msg: %i\n",
			ret);

	return ret;
}

static int dp_config_nl_encode(struct sk_buff *skb, void *buffer,
			       unsigned int size)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr *dpcfg = nla_nest_start(skb, PON_MBOX_A_DP_CONFIG);
	int ret = 0;

	if (!dpcfg)
		return 1;

	ret = nla_put_u8(skb, PON_MBOX_A_DP_CONFIG_WITH_RX_FCS,
			 pon->dp_flags.with_rx_fcs);
	if (!ret)
		ret = nla_put_u8(skb, PON_MBOX_A_DP_CONFIG_WITH_TX_FCS,
				 pon->dp_flags.with_tx_fcs);
	if (!ret)
		ret = nla_put_u8(skb, PON_MBOX_A_DP_CONFIG_WITHOUT_TIMESTAMP,
				 pon->dp_flags.without_timestamp);

	if (ret)
		dev_err(pon->dev, "cannot add data to netlink msg: %i\n", ret);

	nla_nest_end(skb, dpcfg);
	return ret;
}

static int pon_mbox_genl_dp_config(struct sk_buff *skb, struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct pon_msg_header header = {
		.rw = PONFW_READ,
		.cmd = 0xFF, /* Dummy */
		.ack = PONFW_ACK,
		.src = PONFW_HOST,
		.len = PON_MBOX_DPCFG_MAX,
	};
	int err;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}

	err = pon_mbox_send_nl_response_encoded(pon, info->snd_portid,
						info->snd_seq,
						genl_info_net(info), &header,
						NULL, 0,
						dp_config_nl_encode);
	CHECK_FOR_NETLINK_RESPONSE_ERR(pon, err);
	return err;
}

static int pon_mbox_genl_cnt_twdm_wlchid_set(struct sk_buff *skb,
					     struct genl_info *info)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct nlattr **attrs = info->attrs;
	u8 ds_wlch_id, us_wlch_id;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return -ENODEV;
	}
	if (!pon->cnt_autoupdate || !pon->cnt_state) {
		pr_err("counter handling not configured\n");
		return -ENODEV;
	}

	if (attrs[PON_MBOX_CNT_TWDM_WLCHID_DS]) {
		ds_wlch_id = nla_get_u8(attrs[PON_MBOX_CNT_TWDM_WLCHID_DS]);
	} else {
		dev_err(pon->dev, "DS Wave Len Channel ID missing\n");
		return -EINVAL;
	}
	if (attrs[PON_MBOX_CNT_TWDM_WLCHID_US])
		us_wlch_id = nla_get_u8(attrs[PON_MBOX_CNT_TWDM_WLCHID_US]);
	else
		us_wlch_id = ds_wlch_id;

	if (ds_wlch_id >= TWDM_DS_MAX || us_wlch_id >= TWDM_US_MAX)
		return -EINVAL;

	pon_mbox_cnt_autoupdate_wl_switch(pon->cnt_autoupdate,
					  ds_wlch_id, us_wlch_id);

	return 0;
}

static int default_encode(struct sk_buff *skb, void *msg, unsigned int size)
{
	return nla_put(skb, PON_MBOX_A_DATA, size, msg);
}

static int pon_mbox_send_nl_response(struct pon_mbox *pon,
				     u32 snd_portid,
				     u32 snd_seq, struct net *net,
				     struct pon_msg_header *header,
				     char *msg, u32 flags)
{
	return pon_mbox_send_nl_response_encoded(pon, snd_portid, snd_seq, net,
						 header, msg, flags,
						 default_encode);
}

static int pon_mbox_read_payload(struct pon_mbox *pon, char *buf, size_t size)
{
	int ret;

	ret = pon->read(pon, PON_MBOX_DATA1, buf, size);
	if (ret) {
		dev_err(pon->dev, "cannot read from data1 register: ret: %i\n",
			ret);
		return ret;
	}

	print_hex_dump_debug("data: ", DUMP_PREFIX_OFFSET, 16, 1, buf, size,
			     false);
	return 0;
}

/**
 * This function reads one message from the mailbox. This gets called by the
 * interrupt handler. Before reading this functions checks if there are some
 * messages and some data in the mailbox and then reads out the header only. If
 * the source was the HOST and it is a ACK or NACK to a previous message it will
 * be associated to that NetLink message and send back to that process. If this
 * is an event the sequence number will be stored together with the new NetLink
 * ID to send a Ack for this message later. If this message has a payload it
 * will also be read from the mailbox and added to the NetLink message. An
 * answer to a previous NetLink message is only send to the process that send
 * the original request, an event is send to the multicast queue. If a message
 * was read we have to acknowledge it otherwise the mailbox handling will run
 * into problems.
 */
static int pon_mbox_read_msg(struct pon_mbox *pon)
{
	u32 len1;
	struct pon_msg_header header;
	int rlen;
	int rmsg;
	int plen;
	int ret;
	const int reset_timeout = 1000;
	char *msg = NULL;
	u32 snd_portid, snd_seq;
	struct net *net = NULL;
	enum pon_mbox_msg_origin origin = PON_MBOX_MSG_ORIGIN_NETLINK;
	struct pon_mbox_pending *pending = NULL;

	ret = pon_mbox_read32(pon, PON_MBOX_LEN1, &len1);
	if (ret)
		return ret;
	rlen = (len1 & PON_MBOX_LEN1_RLEN_MASK);
	if (pon->hw_ver >= PON_MBOX_HW_VER_B_TYPE)
		rmsg = (len1 & PON_MBOX_LEN1_RMSG_MASK_B) >>
			PON_MBOX_LEN1_RMSG_SHIFT_B;
	else
		rmsg = (len1 & PON_MBOX_LEN1_RMSG_MASK_A) >>
			PON_MBOX_LEN1_RMSG_SHIFT_A;

	/*
	 * If the message counter is greater than 0 and the available message
	 * length is 0, just ack the message to drop it.
	 */
	if (rmsg && !rlen) {
		dev_err(pon->dev,
			"message with length 0 found, just ack and drop it, len: 0x%x, msg: 0x%x (len1: 0x%x)\n",
			rlen, rmsg, len1);
		pon_mbox_fw_tx_fifo_reset(pon, reset_timeout);
		return -EINVAL;
	}

	/* check if there is a message available and it is at least
	 * 32 bits long
	 */
	if (!rmsg || !rlen) {
		dev_err(pon->dev,
			"no message found, but irq was raised, len: 0x%x, msg: 0x%x (len1: 0x%x)\n",
			rlen, rmsg, len1);
		pon_mbox_fw_tx_fifo_reset(pon, reset_timeout);
		return -EINVAL;
	}

	/* read header from HW */
	ret = pon->read(pon, PON_MBOX_DATA1, &header, sizeof(header));
	if (ret) {
		dev_err(pon->dev, "problem reading register (0x%x): %i\n",
			PON_MBOX_DATA1, ret);
		pon_mbox_fw_tx_fifo_reset(pon, reset_timeout);
		return ret;
	}
	plen = header.len * PON_MBOX_BYTES_PER_WORD;

	if (header.seq >= PON_MBOX_PENDING_MAX) {
		dev_err(pon->dev, "sequence number too big: %i\n", header.seq);
		pon_mbox_fw_tx_fifo_reset(pon, reset_timeout);
		return -EINVAL;
	}

	/* if this is from host, it is an answer for an earlier request. */
	/* get matching 'pending' structure if operation is not an event */
	if (header.src == PONFW_HOST) {
		pending = &pon->pending[header.seq];
		if (!pending->in_use) {
			dev_err(pon->dev,
				"cannot find request for, seq: %i, cmd: 0x%x, drop message\n",
				header.seq, header.cmd);
			pon_mbox_fw_tx_fifo_reset(pon, reset_timeout);
			return -EINVAL;
		}

		if (pending->origin == PON_MBOX_MSG_ORIGIN_NETLINK &&
		    !pending->net) {
			dev_err(pon->dev, "no pending entry for seq: %i\n",
				header.seq);
			pon_mbox_release_pending_or_complete(pending, -ENOMEM);
			pon_mbox_fw_tx_fifo_reset(pon, reset_timeout);
			return -EINVAL;
		}

		snd_portid = pending->snd_portid;
		snd_seq = pending->snd_seq;
		origin = pending->origin;
		net = pending->net;

	} else {
		snd_portid = 0;
		/* store the mail box sequence number in the last 3 bits */
		pon_mbox_seq += PON_MBOX_PENDING_MAX;
		snd_seq = pon_mbox_seq + header.seq;
	}

	/* check if the message would fit into the available data */
	if (plen >= (rlen * PON_MBOX_BYTES_PER_WORD)) {
		dev_err(pon->dev,
			"received message is bigger than available space\n");
		pon_mbox_release_pending_or_complete(pending, -ENOMEM);

		pon_mbox_fw_tx_fifo_reset(pon, reset_timeout);
		return -ENOMEM;
	}

	dev_dbg(pon->dev,
		"receive msg: rw: %i, cmd: 0x%x, ack: %i, src: %i, seq: %i, len: %i,\n",
		header.rw, header.cmd, header.ack, header.src, header.seq,
		plen);

	if (plen > 0) {
		if (origin == PON_MBOX_MSG_ORIGIN_KERNEL) {
			struct pon_mbox_ikm *ikm = &pending->ikm;

			if (!ikm->result_data || ikm->result_size < plen) {
				pon_mbox_release_pending_or_complete(pending,
								     -EINVAL);
				dev_err(pon->dev,
					"Attempting to reset the PON firmware mailbox\n");
				pon_mbox_fw_tx_fifo_reset(pon, reset_timeout);
				return -EINVAL;
			}

			ret = pon_mbox_read_payload(pon, ikm->result_data,
						    plen);
			if (ret) {
				pon_mbox_release_pending_or_complete(pending,
								     ret);
				dev_err(pon->dev,
					"Attempting to reset the PON firmware mailbox\n");
				pon_mbox_fw_tx_fifo_reset(pon, reset_timeout);
				return ret;
			}
		} else {
			msg = kmalloc(plen, GFP_KERNEL);
			if (!msg) {
				pon_mbox_release_pending_or_complete(pending,
								     -ENOMEM);
				dev_err(pon->dev,
					"Attempting to reset the PON firmware mailbox\n");
				pon_mbox_fw_tx_fifo_reset(pon, reset_timeout);
				return -ENOMEM;
			}

			ret = pon_mbox_read_payload(pon, msg, plen);
			if (ret) {
				kfree(msg);
				pon_mbox_release_pending_or_complete(pending,
								     ret);
				dev_err(pon->dev,
					"Attempting to reset the PON firmware mailbox\n");
				pon_mbox_fw_tx_fifo_reset(pon, reset_timeout);
				return ret;
			}
		}
	}

	if (header.src == PONFW_PONIP)
		pon_mbox_queue_event(pon, &header, msg, plen);

	if (header.src == PONFW_PONIP ||
	    origin == PON_MBOX_MSG_ORIGIN_NETLINK) {
		/* Only send NL response if cmd is available for userspace */
		if (!pon_mbox_userspace_cmd_check(header.cmd))
			ret = pon_mbox_send_nl_response(pon, snd_portid,
							snd_seq, net, &header,
							msg, 0);
		kfree(msg);

		pon_mbox_release_pending(pending);

		if (ret) {
			CHECK_FOR_NETLINK_RESPONSE_ERR(pon, ret);
			pon_mbox_write32(pon, PON_MBOX_CMD,
					 PON_MBOX_CMD_R1 | PON_MBOX_CMD_EOM);
			return ret;
		}
	} else {
		/*
		 * In case the FW returned an error return an error message to
		 * the calling process.
		 */
		switch (header.ack) {
		case PONFW_CMD:
		case PONFW_ACK:
			/* good case */
			break;
		case PONFW_NACK:
			plen = -PON_STATUS_FW_NACK;
			break;
		case PONFW_NACK_DUP:
			plen = -PON_STATUS_FW_DUP_ERR;
			break;
		case PONFW_NACK_DBG:
			plen = -PON_STATUS_FW_DBG;
			break;
		case PONFW_NACK_STATE:
			plen = -PON_STATUS_FW_STATE;
			break;
		case PONFW_CMDERR:
		default:
			plen = -EIO;
			break;
		}

		/* wake-up completion */
		/* do cleanup after task which was waiting finished */
		pon_mbox_release_pending_or_complete(pending, plen);
	}

	/* ack that the message was read */
	return pon_mbox_write32(pon,
				PON_MBOX_CMD,
				PON_MBOX_CMD_R1 | PON_MBOX_CMD_EOM);
}

/**
 * This function informs the user space about some event in the mailbox driver.
 * This is mostly the case when a reset was requested by the firmware or when
 * the firmware initialization is finished, this is not used to forward
 * messages from the firmware to the user space.
 */
static int pon_mbox_send_nl_message(struct pon_mbox *pon, u8 cmd)
{
	int ret;
	void *msg_head;
	struct sk_buff *skb;

	skb = genlmsg_new(GENLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!skb) {
		dev_err(pon->dev, "cannot allocate netlink msg\n");
		return -ENOMEM;
	}

	msg_head = genlmsg_put(skb, 0, 0, &pon_mbox_genl_family, 0, cmd);
	if (!msg_head) {
		dev_err(pon->dev, "cannot create netlink msg\n");
		nlmsg_free(skb);
		return -ENOMEM;
	}

	genlmsg_end(skb, msg_head);

	ret = genlmsg_multicast(&pon_mbox_genl_family, skb, 0, 0, GFP_KERNEL);

	if (ret == -ESRCH) {
		/* if genl_has_listeners fails we assume there are listeners */
		if (genl_has_listeners(&pon_mbox_genl_family, &init_net, 0))
			dev_err(pon->dev, "cannot send driver multicast netlink msg: %i\n",
				ret);
	} else if (ret) {
		dev_err(pon->dev, "cannot send driver multicast netlink msg: %i\n",
			ret);
	}
	return ret;
}

/**
 * This function gets called when the firmware triggered an error. This is the
 * case when we send an unknown message for example. In this case the pon
 * SW -> FW mailbox is reset, the firmware still runs further. All pending
 * writes to the mailbox will return an error. After the mailbox is reset
 * again we signal that to the FW and it will operate normal again, but
 * some messages could have be lost.
 *
 * See SAS section 12.7.7 "Mailbox Reset in Case of Errors" for details.
 */
static int pon_mbox_fw_err(struct pon_mbox *pon)
{
	int ret;

	dev_err(pon->dev, "Firmware reported an error => mailbox reset\n");

	pon_mbox_send_nl_message(pon, PON_MBOX_C_RESET);

	/* clear the tx buffer and accept and discard all messages from user */
	pon->in_reset = true;
	pon->tx_ready = true;
	wake_up_interruptible_all(&pon->tx_waitqueue);
	schedule();

	/* Clear inbox/TX FIFO (SW->FW) */
	ret = pon_mbox_write32(pon,
			       PON_MBOX_CMD,
			       PON_MBOX_CMD_X1 | PON_MBOX_CMD_RST);
	if (ret)
		return ret;

	pon_mbox_release_pending_all(pon, -EIO);
	pon->in_reset = false;
	return ret;
}

static int pon_mbox_download_firmware(struct pon_mbox *pon, u32 boot_stat);

/**
 * This IRQ handler gets called every time an IRQ is raised by the PON IP.
 * We lock the IRQ handling here to make sure nothing else changes the IRQ
 * mask while this code is running. First all the error IRQs are handled and
 * then the normal TX and RX IRQs are handled. In the end we have to make sure
 * that all handled IRQs are getting acknowledged to the hardware.
 * Most of the actual functionality is done in other functions which are called
 * by the IRQ handler.
 */
static irqreturn_t pon_mbox_irq_thread(int irq, void *cookie)
{
	struct pon_mbox *pon = cookie;
	u32 irq_status;
	int ret;
	int i;
	u32 irq_ack = 0;
#ifdef DEBUG_MBOX_RESPONSE_TIME
	unsigned int timer;

	timer = jiffies_to_usecs(jiffies - debug_irq_jiffies_start);
	if (time_is_before_jiffies(debug_irq_jiffies_start +
					msecs_to_jiffies(100)))
		dev_err(pon->dev, "IRQ handling took more than 100ms start threaded IRQ: %d us\n",
			timer);
	debug_irq_jiffies_start = jiffies;
#endif

	mutex_lock(&pon->irq_lock);

#ifdef DEBUG_MBOX_RESPONSE_TIME
	timer = jiffies_to_usecs(jiffies - debug_irq_jiffies_start);
	if (time_is_before_jiffies(debug_irq_jiffies_start +
					msecs_to_jiffies(100)))
		dev_err(pon->dev, "IRQ handling took more than 100ms get irq_lock: %d us\n",
			timer);
	debug_irq_jiffies_start = jiffies;
#endif
	/* mask all irqs for this block so we can activate the global
	 * irq again
	 */
	ret = pon_mbox_write32(pon, PON_MBOX_IEN, 0);

	for (i = 0; i < ARRAY_SIZE(pon->irqs); i++) {
		if (pon->irqs[i])
			enable_irq(pon->irqs[i]);
	}

	ret = pon_mbox_read32(pon, PON_MBOX_STAT, &irq_status);
	if (ret) {
		pon_mbox_set_irq_mask(pon);
		mutex_unlock(&pon->irq_lock);
		return IRQ_NONE;
	}

	irq_status &= pon->irq_mask;

	if (irq_status & PON_MBOX_STAT_BOOT_REQ) {
		pon->pon_ip_debug_mode = true;
		dev_warn(pon->dev, "Boot debug request received, block PON FW download");

		/* Acknowledge the request to the PON FW */
		ret = pon_mbox_write32(pon, PON_MBOX_CMD,
				       PON_MBOX_CMD_BOOT_ACK);
		if (ret)
			goto out_ack;

		irq_ack |= PON_MBOX_STAT_BOOT_REQ;
	}

	if (irq_status & PON_MBOX_STAT_BOOT_END) {
		pon->pon_ip_debug_mode = false;
		dev_warn(pon->dev, "Boot debug end received, trigger PON IP reset");
		irq_ack |= PON_MBOX_STAT_BOOT_END;

		pon_mbox_write32(pon, PON_MBOX_STAT, irq_ack);
		pon_mbox_set_irq_mask(pon);

		/* full reset will trigger the interrupt again */
		ret = pon_mbox_genl_reset_full(pon);
		if (ret)
			dev_err(pon->dev, "Error while trying to reset\n");

		goto out_reset;
	}

	if (irq_status & PON_MBOX_STAT_BOOT_KEEP) {
		pon->pon_ip_debug_mode = false;
		dev_warn(pon->dev, "Boot debug keep received, allow PON FW download");
		irq_ack |= PON_MBOX_STAT_BOOT_KEEP;
	}

	if (irq_status & PON_MBOX_STAT_BOOT_FAIL) {
		dev_warn(pon->dev, "Boot debug fail received");
		irq_ack |= PON_MBOX_STAT_BOOT_FAIL;
	}

	if (irq_status & PON_MBOX_STAT_BOOT_VAL) {
		u32 boot_cause;
		u32 boot_stat;
		u32 boot_version;

		ret = pon_mbox_read32(pon, PON_MBOX_BOOT_CAUSE, &boot_cause);
		if (ret)
			goto out_ack;
		if (boot_cause != PON_MBOX_BOOT_CAUSE_NORMAL)
			dev_warn(pon->dev, "Unexpected boot_cause: %i",
				 boot_cause);

		ret = pon_mbox_read32(pon, PON_MBOX_BOOT_STAT, &boot_stat);
		if (ret)
			goto out_ack;
		if (boot_stat != PON_MBOX_BOOT_STAT_NORMAL)
			dev_warn(pon->dev, "Unexpected boot_stat: %i",
				 boot_stat);

		ret = pon_mbox_read32(pon, PON_MBOX_BOOT_VERSION,
				      &boot_version);
		if (ret)
			goto out_ack;

		/*
		 * In case of failure retry firmware download.
		 * fw_dl_retry_cnt is initialized before requesting the
		 * interrupt.
		 */
		ret = pon_mbox_download_firmware(pon, boot_stat);
		if (ret && pon->fw_dl_retry_cnt) {
			dev_err(pon->dev, "Retrying download firmware...\n");
			pon->fw_dl_retry_cnt--;

			/*
			 * acknowledge the interrupt to execute this part of
			 * interrupt handling after reset
			 */
			irq_ack |= PON_MBOX_STAT_BOOT_VAL;
			pon_mbox_write32(pon, PON_MBOX_STAT, irq_ack);
			pon_mbox_set_irq_mask(pon);

			/* full reset will trigger the interrupt again */
			ret = pon_mbox_genl_reset_full(pon);
			if (ret)
				dev_err(pon->dev, "Error while trying to reset\n");

			goto out_reset;
		}

		/* reset counter to initial value - FW download is successful */
		pon->fw_dl_retry_cnt = PON_FW_DOWNLOAD_MAX_RETRIES;
		irq_ack |= PON_MBOX_STAT_BOOT_VAL;

#if !defined(SKIP_TEP) && \
	(defined(CONFIG_X86_INTEL_LGM) || defined(CONFIG_SOC_LGM))
		/* Lock PON FW download on URX800.
		 * TEP ensures that the register can only be written once.
		 */
		if (pon->sectop) {
			dev_dbg(pon->dev, "Locking PON FW download\n");
			ret = regmap_update_bits(pon->sectop, SECTOP_PON_PPM,
						 BIT(1), BIT(1));
			if (ret)
				dev_err(pon->dev,
					"Locking PON FW download failed %d\n",
					ret);
			else
				pon->urx_fw_dl_locked = true;
		}
#endif
		/* do not handle other IRQs after reset */
		goto out_ack;
	}

	if (irq_status & PON_MBOX_STAT_RXERR) {
		pon_mbox_fw_err(pon);
		irq_ack |= PON_MBOX_STAT_RXERR;
		/* if the firmware reported an error don't do anything else */
		goto out_ack;
	}

	if (irq_status & PON_MBOX_STAT_R1_UFL) {
		dev_err(pon->dev, "Receiver 1 Underflow detected\n");
		irq_ack |= PON_MBOX_STAT_R1_UFL;
	}

	if (irq_status & PON_MBOX_STAT_X1_OFL) {
		dev_err(pon->dev, "Transmitter 1 Overflow detected\n");
		irq_ack |= PON_MBOX_STAT_X1_OFL;
	}

	/* there are messages in the mailbox, read them */
	if (irq_status & PON_MBOX_STAT_R1_DATA)
		pon_mbox_read_msg(pon);

	if (irq_status & PON_MBOX_STAT_X1_RDY) {
		pon->tx_ready = true;
		wake_up_interruptible_all(&pon->tx_waitqueue);

		pon->irq_mask &= ~PON_MBOX_STAT_X1_RDY;
	}

out_ack:
	if (irq_ack)
		pon_mbox_write32(pon, PON_MBOX_STAT, irq_ack);

#ifdef DEBUG_MBOX_RESPONSE_TIME
	timer = jiffies_to_usecs(jiffies - debug_irq_jiffies_start);
	if (time_is_before_jiffies(debug_irq_jiffies_start +
					msecs_to_jiffies(100)))
		dev_err(pon->dev, "IRQ handling took more than 100ms to process: %d us\n",
			timer);
#endif
	/*
	 * activate the error irqs again, the data irq is activated when the
	 * package was ready by the userspace.
	 */
	pon_mbox_set_irq_mask(pon);

out_reset:
	mutex_unlock(&pon->irq_lock);
	return IRQ_HANDLED;
}

/**
 * Use threaded interrupts here because the system might sleep when we access
 * the SPI bus. The IRQ is acknowledged in the IP core in
 * \ref pon_mbox_irq_thread.
 */
static irqreturn_t pon_mbox_irq_hard(int irq, void *cookie)
{
	struct pon_mbox *pon = cookie;
	int i;
#ifdef DEBUG_MBOX_RESPONSE_TIME
	debug_irq_jiffies_start = jiffies;
#endif

	for (i = 0; i < ARRAY_SIZE(pon->irqs); i++) {
		if (pon->irqs[i])
			disable_irq_nosync(pon->irqs[i]);
	}

	return IRQ_WAKE_THREAD;
}

static void pon_mbox_init_config(struct work_struct *work)
{
	struct pon_mbox *pon = pon_mbox_dev;
	struct ponfw_version fw_ver = {0};
	struct ponfw_hw_version hw_ver = {0};
	int ret;

	kfree(work);

	ret = pon_mbox_send(PONFW_VERSION_CMD_ID, PONFW_READ,
			    NULL, 0, &fw_ver, sizeof(fw_ver));
	if (ret < 0) {
		dev_err(pon->dev, "Unable to get FW version, stop init. (ret: %d)\n",
			ret);
		return;
	}

	pr_info("FW version: %d.%d.%d.%d.%d-%d\n",
		fw_ver.maj, fw_ver.min, fw_ver.standard,
		fw_ver.platform, fw_ver.patch, fw_ver.time);

	/* Skip configuring the hw_vers for firmware versions not supporting
	 * the message
	 */
	if (fw_ver.maj < 3)
		goto init_complete;

	if (fw_ver.maj == 3 && fw_ver.min < 19)
		goto init_complete;

	if (fw_ver.maj == 3 && fw_ver.min == 19 && fw_ver.patch < 16)
		goto init_complete;

	hw_ver.version = get_soc_rev();
	/* indicate a predefined/alternate HW version to the firmware */
	if (pon->soc_spec->hw_version_firmware)
		hw_ver.version = pon->soc_spec->hw_version_firmware;

	ret = pon_mbox_send(PONFW_HW_VERSION_CMD_ID, PONFW_WRITE,
			    &hw_ver, sizeof(hw_ver), NULL, 0);
	if (ret < 0)
		dev_err(pon->dev, "Unable to set HW version (ignored), ret: %d\n",
			ret);

init_complete:
	pon_mbox_send_nl_message(pon, PON_MBOX_C_FW_INIT_COMPLETE);
}

/**
 * Queue initial config (including FW version printout)
 */
static void pon_mbox_queue_initial_config(struct pon_mbox *pon)
{
	int ret;
	struct work_struct *work = kzalloc(sizeof(*work), GFP_KERNEL);

	if (!work)
		return;

	INIT_WORK(work, pon_mbox_init_config);
	ret = queue_work(system_long_wq, work);
	if (!ret) {
		dev_err(pon->dev, "Can not queue for init config\n");
		kfree(work);
	}
}

/**
 * Loads the PON IP firmware into the hardware block. This gets called by the
 * IRQ handler when the PON IP is in boot loader state. This will load a
 * firmware with the filename given in the fw_filename member. The firmware
 * has a checksum which is stored in the last 32 bit of the file. We calculate
 * it in the driver and compare it to the value stored in the file. The PON IP
 * BIOS also calculates it and compares it to the value given in the file. This
 * is used to make sure we did not have any corruption. To load the firmware it
 * is written to the mailbox and later we check the BOOT_STAT register to see
 * if it was loaded. This register will be set by the BIOS when the FW was
 * loaded correctly or indicate an error number in case something went wrong.
 * After the firmware was successful loaded the system will accept NetLink
 * messages again.
 */
static int pon_mbox_download_firmware(struct pon_mbox *pon, u32 boot_stat)
{
	const struct firmware *fw_entry;
	int ret;
	int pos = 0;
	int free_space;
	int delay_counter = 0;
	u32 boot_id;
	int i;
	u32 checksum = 0;
	u32 *fw_data;
	const char *fw_filename;

	if (pon->pon_ip_debug_mode) {
		dev_warn(
			pon->dev,
			"FW download was triggered by IRQ in debug mode, ignore it");
		return 0;
	}

	if (!pon->in_reset) {
		/*
		 * Put mailbox into reset when FW reset triggered by FW.
		 * When the reset is triggered by the SW, this was already
		 * done, only if this was not triggered by the SW we still have
		 * to block the mailbox till the new FW was loaded.
		 */
		pon->in_reset = true;
		pon->tx_ready = false;
		wake_up_interruptible_all(&pon->tx_waitqueue);
	}

	/*
	 * This should not happen.
	 * We saw this error when the FPGA lost the clock.
	 */
	if (boot_stat == PON_MBOX_BOOT_STAT_TERM) {
		dev_err(pon->dev,
			"FW download was triggered by IRQ while FW was already loaded (BOOT_STAT_TERM), abort download\n");
		return -EINVAL;
	}

	fw_filename = pon_mbox_get_fw_filename(pon, pon->mode);
	if (!fw_filename) {
		dev_err(pon->dev, "unsupported pon mode: %i\n", pon->mode);
		return -EINVAL;
	}

	ret = request_firmware(&fw_entry, fw_filename, pon->dev);
	if (ret) {
		dev_err(pon->dev, "failed to get firmware file: ret: %i\n",
			ret);
		return ret;
	}

	/* calculate and check the firmware checksum first in SW. */
	fw_data = (u32 *)fw_entry->data;
	for (i = 0; i < ((fw_entry->size / 4) - 1); i++)
		checksum += be32_to_cpu(fw_data[i]);

	if (checksum != be32_to_cpu(fw_data[(fw_entry->size / 4) - 1])) {
		dev_err(pon->dev,
			"firmware file checksum mismatch. File 0x%x, calculated: 0x%x\n",
			be32_to_cpu(fw_data[(fw_entry->size / 4) - 1]),
				    checksum);
		goto out;
	}

	/*
	 * Clear inbox/TX FIFO (SW->FW)
	 *
	 * In case this reset was not triggered by the driver there could
	 * still be some FW messages in the mailbox. Reset the content of
	 * the TX mailbox before we write the PON FW into this mailbox.
	 * This should make the PON FW download more reliable.
	 */
	ret = pon_mbox_write32(pon,
			       PON_MBOX_CMD,
			       PON_MBOX_CMD_X1 | PON_MBOX_CMD_RST);
	if (ret)
		goto out;

	ret = pon_mbox_write32(pon, PON_MBOX_BOOT_CMD, PON_MBOX_BOOT_CMD_HOST);
	if (ret)
		goto out;

	ret = pon_mbox_write32(pon, PON_MBOX_CMD, PON_MBOX_CMD_BOOT_MODE);
	if (ret)
		goto out;

	/*
	 * Put the WAN XPCS into reset now, it is not used when PON is active.
	 * We can only do this after the PON IP was started otherwise we will
	 * only get into O23. When we get the FW download IRQ from the PON BIOS
	 * we should be able to put the WAN XPCS into reset.
	 * In case serdes_init is executed, the xpcs reset is performed there.
	 */
	if (!pon->soc_spec->serdes_init &&
	    pon->reset_xpcs && !pon->reset_xpcs_done) {
		ret = reset_control_assert(pon->reset_xpcs);
		if (ret) {
			dev_err(pon->dev, "WAN XPCS reset assert failed: %i",
				ret);
			goto out;
		}
		pon->reset_xpcs_done = true;
	}

	/* Perform SerDes reset here to ensure that we
	 * have same starting conditions even in case of SW
	 * triggered restart
	 */
	if (pon->soc_spec->serdes_init) {
		ret = reset_control_assert(pon->reset_serdes);
		if (ret) {
			dev_err(pon->dev, "SerDes reset assert failed: %i",
				ret);
			goto out;
		}
	}

	/*
	 * Set the BOOT_ID register to the defined value 0 to remove old
	 * values. After the FW download it should contain the checksum again.
	 */
	ret = pon_mbox_write32(pon, PON_MBOX_BOOT_ID, 0);
	if (ret)
		goto out;

	/* check how much space is in the mbox and write the firmware
	 * into it
	 */
	while (pos < fw_entry->size) {
		free_space = pon_mbox_tx_free_space(pon);
		if (free_space < 0) {
			ret = free_space;
			goto out;
		}

		/*
		 * In the normal PON IP download case the PON IP is able to
		 * read back the data fast enough and we never trigger this
		 * code. We only saw this being triggered when we assumed
		 * that the PON IP crashed.
		 */
		if (free_space == 0) {
			delay_counter++;
			udelay(5);
			/* if we waited here 10.000 times, just abort */
			if (delay_counter >= 10000) {
				dev_err(pon->dev,
					"timeout while writing the firmware at pos: %i",
					pos);
				ret = -EINVAL;
				break;
			}
			continue;
		}
		delay_counter = 0;

		if (free_space > fw_entry->size - pos)
			free_space = fw_entry->size - pos;

		ret = pon->write(pon, PON_MBOX_DATA1, fw_entry->data + pos,
				 free_space, PON_WR_FLAG_FWDL);
		if (ret) {
			dev_err(pon->dev,
				"cannot write to data1 register: ret: %i\n",
				ret);
			goto out;
		}
		pos += free_space;
	}

	/*
	 * Check if the firmware was loaded successfully, it takes some time,
	 * but normally it is less than 10 iterations. In case it was not
	 * successfully loaded, print the error code.
	 */
	for (i = 0; i < 100; i++) {
		ret = pon_mbox_read32(pon, PON_MBOX_BOOT_STAT, &boot_stat);
		if (ret)
			goto out;

		/* Firmware successfully loaded */
		if (boot_stat == PON_MBOX_BOOT_STAT_TERM) {
			ret = pon_mbox_read32(pon, PON_MBOX_BOOT_ID, &boot_id);
			if (ret)
				goto out;

			if (cpu_to_be32(checksum) != cpu_to_be32(boot_id)) {
				dev_err(pon->dev,
					"firmware file checksum mismatch. boot_id: 0x%x, calculated: 0x%x\n",
					boot_id, cpu_to_be32(checksum));
				goto out;
			}
			pon_mbox_release_pending_all(pon, -EIO);
			pon->tx_ready = true;
			pon->in_reset = false;
			/* Queue initial config to avoid a deadlock
			 * from sending messages.
			 */
			pon_mbox_queue_initial_config(pon);
			goto out;
		}
		usleep_range(30, 60);
	}

	dev_err(pon->dev, "bootloader could not load FW: %i\n",
		boot_stat);
	ret = -EIO;

out:
	release_firmware(fw_entry);
	return ret;
}

/**
 * This function is the generic initialization function. The code is the same
 * for devices connected via SPI or directly memory mapped devices. We setup
 * the pon_mbox struct, check if this IP core has the correct PON IP magic and
 * setup the IRQ handler.
 */
static int pon_mbox_register(struct pon_mbox *pon)
{
	int ret;
	u32 fuse0;
	u32 magic;
	u32 boot_stat;
	u32 stat = 0;
	int i = 0;

	if (pon_mbox_dev) {
		dev_err(pon->dev, "only one device supported");
		return -ENODEV;
	}

	pon->fw_dl_retry_cnt = PON_FW_DOWNLOAD_MAX_RETRIES;
	pon->irq_mask = PON_MBOX_STAT_R1_UFL |
			PON_MBOX_STAT_X1_OFL |
			PON_MBOX_STAT_R1_DATA |
			PON_MBOX_STAT_RXERR |
			PON_MBOX_STAT_BOOT_VAL |
			PON_MBOX_STAT_BOOT_REQ |
			PON_MBOX_STAT_BOOT_END |
			PON_MBOX_STAT_BOOT_KEEP |
			PON_MBOX_STAT_BOOT_FAIL;

	mutex_init(&pon->irq_lock);
	mutex_init(&pon->biterror_lock);
	init_waitqueue_head(&pon->tx_waitqueue);

	/* check for valid boot mode */
	do {
		usleep_range(30, 50);
		ret = pon_mbox_read32(pon, PON_MBOX_STAT, &stat);
		if (ret)
			return ret;
	} while ((stat & PON_MBOX_STAT_BOOT_VAL) == 0 && i++ < 10);

	if (stat & PON_MBOX_STAT_BOOT_VAL) {
		dev_dbg(pon->dev, "Boot mode valid\n");
		ret = pon_mbox_write32(pon, PON_MBOX_STAT, 0);
		if (ret)
			return ret;
	} else {
		dev_err(pon->dev, "Boot mode not valid\n");
		return -ENODEV;
	}

	ret = pon_mbox_read32(pon, PON_MBOX_BOOT_STAT, &boot_stat);
	if (ret)
		return ret;

	/* Block it when no FW is loaded */
	if (boot_stat == PON_MBOX_BOOT_STAT_TERM) {
		pon->tx_ready = true;
		pon->in_reset = false;
	} else {
		pon->tx_ready = false;
		pon->in_reset = true;
	}

	ret = pon_mbox_read32(pon, PON_MBOX_FUSE0, &fuse0);
	if (ret)
		return ret;
	if (pon->soc_spec->hw_version_override)
		pon->hw_ver = pon->soc_spec->hw_version_override;
	else
		pon->hw_ver = (fuse0 & PON_MBOX_FUSE0_VER_MASK) >>
				PON_MBOX_FUSE0_VER_SHIFT;
	magic = (fuse0 & PON_MBOX_FUSE0_ID_MASK) >> PON_MBOX_FUSE0_ID_SHIFT;

	/*
	 * Check if the magic is the expected "ON" to check if this is really
	 * a PON IP or something else.
	 */
	if (magic != PON_MBOX_MAGIC) {
		dev_warn(pon->dev, "unexpected magic: 0x%x\n", magic);
		return -ENODEV;
	}

	ret = pon_mbox_write32(pon, PON_MBOX_IEN, 0);
	if (ret)
		return ret;

	memset(&pon->serdes_cfg, 0, sizeof(pon->serdes_cfg));
	memset(&pon->lt_cfg, 0, sizeof(pon->lt_cfg));
	memset(&pon->iop_cfg, 0, sizeof(pon->iop_cfg));

	pon_mbox_register_event_handler(PONFW_CLEAR_ALARM_CMD_ID,
					pon_mbox_handle_alarm_clear, pon);
	pon_mbox_register_event_handler(PONFW_REPORT_ALARM_CMD_ID,
					pon_mbox_handle_alarm_report, pon);
	pon_mbox_register_event_handler(PONFW_PLOAM_STATE_CMD_ID,
					pon_mbox_handle_ploam_state, pon);
	pon_mbox_register_event_handler(PONFW_RAND_NUM_CMD_ID,
					handle_ponip_event_rand_num, pon);

	return 0;
}

#if IS_ENABLED(CONFIG_SPI)
/**
 * This function is called to read some data from one PON MBOX Host register
 * through SPI. This only supports 32 bit aligned messages. Normally a buffer
 * is 32 bit, only for the messages itself it could be bigger than 32 bit.
 */
static int pon_mbox_spi_read(struct pon_mbox *pon, int reg, void *buf,
			     size_t size)
{
	struct spi_device *spi = pon->spi;
	union pon_mbox_spi_header header;
	int ret, i;
	u32 *pbuf = buf;

	if (WARN((size % 4) || !size || size > sizeof(buf_byte_order_conv),
		 "unsupported size of %zu", size))
		return -EINVAL;

	header.full = 0;
	header.field.read = 1;
	header.field.addr = reg;
	header.field.len = (size / 4) - 1;

	header.full = htonl(header.full);

	/* if dma support is needed, use another function */
	ret = spi_write_then_read(spi, &header.full, sizeof(header),
				  &buf_byte_order_conv, size);

	if (size == 4) {
		dev_dbg(pon->dev,
			"read: reg: %i, size: %zu, spi_header: %#010x, data: %#010x\n",
			reg, size, header.full, buf_byte_order_conv[0]);
	} else {
		dev_dbg(pon->dev,
			"read: reg: %i, size: %zu, spi_header: %#010x\n", reg,
			size, header.full);
		if (size < 100)
			print_hex_dump_debug("data: ", DUMP_PREFIX_OFFSET, 16,
					     1, buf_byte_order_conv, size,
					     false);
	}

	for (i = 0; i < (size / 4); i++)
		*pbuf++ = ntohl(buf_byte_order_conv[i]);

	return ret;
}

/**
 * This function is called to write some data into one PON MBOX Host register
 * through SPI. This only supports 32 bit aligned messages. Normally a buffer
 * is 32 bit, only for the messages itself it could be bigger than 32 bit.
 */
static int pon_mbox_spi_write(struct pon_mbox *pon, int reg, const void *buf,
			      size_t size, int flag)
{
	struct spi_device *spi = pon->spi;
	union pon_mbox_spi_header header;
	const u32 *pbuf = buf;
	struct spi_message m;
	struct spi_transfer header_t, body_t;
	int i;

	if (WARN((size % 4) || !size || size > sizeof(buf_byte_order_conv),
		 "unsupported size of %zu", size))
		return -EINVAL;

	header.full = 0;
	header.field.write = 1;
	header.field.addr = reg;
	header.field.len = (size / 4) - 1;
	header.full = htonl(header.full);

	memset(&header_t, 0, sizeof(header_t));
	header_t.tx_buf = &header.full;
	header_t.len = sizeof(header);

	memset(&body_t, 0, sizeof(body_t));

	if (flag == PON_WR_FLAG_REGULAR) {
		for (i = 0; i < (size / 4); i++)
			buf_byte_order_conv[i] = htonl(*pbuf++);

		body_t.tx_buf = buf_byte_order_conv;

		if (size == 4) {
			dev_dbg(pon->dev,
				"write: reg: %i, size: %zu, spi_header: %#010x, data: %#010x\n",
				reg, size, *((u32 *)&header.full),
				buf_byte_order_conv[0]);
		} else {
			dev_dbg(pon->dev,
				"write: reg: %i, size: %zu, spi_header: %#010x\n",
				reg, size, *((u32 *)&header.full));
			if (size < 100)
				print_hex_dump_debug("data: ",
						     DUMP_PREFIX_OFFSET, 16, 1,
						     buf_byte_order_conv, size,
						     false);
		}
	} else {
		body_t.tx_buf = buf;

		if (size == 4) {
			dev_dbg(pon->dev, "write: reg: %i, size: %zu, spi_header: %#010x, data: %#010x\n",
				reg, size, *((u32 *)&header.full),
				*((u32 *)buf));
		} else {
			dev_dbg(pon->dev, "write: reg: %i, size: %zu, spi_header: %#010x\n",
				reg, size, *((u32 *)&header.full));
			if (size < 100)
				print_hex_dump_debug("data: ",
						     DUMP_PREFIX_OFFSET, 16,
						     1, buf, size, false);
		}
	}

	body_t.len = size;

	spi_message_init(&m);
	spi_message_add_tail(&header_t, &m);
	spi_message_add_tail(&body_t, &m);

	return spi_sync(spi, &m);
}

static const struct pon_soc_data spi_data = {
	.pon_shell_init = NULL,
	.serdes_basic_init = NULL,
	.ref_clk_sel = NULL,
	.pll5_init = NULL,
	.serdes_init = NULL,
};

#if (KERNEL_VERSION(6, 3, 0) > LINUX_VERSION_CODE)
/* spi_get_chipselect was added in 6.3 */
static inline u8 spi_get_chipselect(const struct spi_device *spi, u8 idx)
{
	return spi->chip_select;
}
#endif

/**
 * This function does the SPI specific init for one device.
 */
static int pon_mbox_spi_probe(struct spi_device *spi)
{
	struct pon_mbox *pon;
	struct device *dev = &spi->dev;
	int ret;

	dev_notice(dev,
		   "found device (CLK: %u hz, CS: %u, M: %u, BTW: %u, irq: %i)\n",
		   spi->max_speed_hz, spi_get_chipselect(spi, 0), spi->mode,
		   spi->bits_per_word, spi->irq);

	pon = devm_kzalloc(dev, sizeof(*pon), GFP_KERNEL);
	if (!pon)
		return -ENOMEM;

	spi->bits_per_word = 32;
	pon->spi = spi;
	pon->dev = dev;
	pon->irqs[0] = spi->irq;
	pon->read = pon_mbox_spi_read;
	pon->write = pon_mbox_spi_write;
	pon->soc_spec = &spi_data;

	ret = pon_mbox_register(pon);
	if (ret)
		return ret;

	ret = devm_request_threaded_irq(pon->dev, pon->irqs[0],
					pon_mbox_irq_hard,
					pon_mbox_irq_thread,
					0,
					dev_name(pon->dev), pon);
	if (ret) {
		dev_err(pon->dev,
			"cannot register irq handler: ret: %i\n",
			ret);
		return ret;
	}

	spi_set_drvdata(spi, pon);

	pon->cnt_state = pon_mbox_cnt_state_init();
	if (!pon->cnt_state) {
		dev_err(dev, "can't initialize counters state");
		return -ENOMEM;
	}

	pon->cnt_autoupdate =
		pon_mbox_cnt_autoupdate_create(pon,
					       counters_update_timer);
	if (!pon->cnt_autoupdate) {
		dev_err(dev, "can't initialize counters autoupdate");
		return -ENOMEM;
	}

	pon_mbox_dev = pon;

	return 0;
}

static int _pon_mbox_spi_remove(struct spi_device *spi)
{
	struct pon_mbox *pon = spi_get_drvdata(spi);

	if (!pon) {
		dev_err(&spi->dev, "no device data to remove\n");
		return -ENODEV;
	}

	pon_mbox_cnt_autoupdate_destroy(pon->cnt_autoupdate);
	pon_mbox_cnt_state_release(pon->cnt_state);

	pon_mbox_unregister_all_event_handlers();

	pon_mbox_dev = NULL;

	return 0;
}

#if (KERNEL_VERSION(5, 18, 0) > LINUX_VERSION_CODE)
#define pon_mbox_spi_remove _pon_mbox_spi_remove
#else
static void pon_mbox_spi_remove(struct spi_device *spi)
{
	(void)_pon_mbox_spi_remove(spi);
}
#endif
#endif /* CONFIG_SPI */

static const struct pon_soc_data prx300_data = {
	.pon_shell_init = NULL,
	.serdes_basic_init = prx300_serdes_basic_init,
	.ref_clk_sel = prx300_ref_clk_sel,
	.pll5_init = prx300_pll5_init,
	.serdes_init = prx300_serdes_init,
};

static const struct pon_soc_data urx800_data = {
	.pon_shell_init = urx800_pon_shell_init,
	.serdes_basic_init = urx800_serdes_basic_init,
	.ref_clk_sel = urx800_ref_clk_sel,
	.pll5_init = urx800_pll5_init,
	.serdes_init = urx800_serdes_init,
};

static const struct pon_soc_data urx800c_data = {
	.pon_shell_init = urx800_pon_shell_init,
	.serdes_basic_init = urx800_serdes_basic_init,
	.ref_clk_sel = urx800_ref_clk_sel,
	.pll5_init = urx800c_pll5_init,
	.serdes_init = urx800_serdes_init,
	.hw_version_override = PON_MBOX_HW_VER_URX_C_TYPE,
	.hw_version_firmware = PONFW_HW_VERSION_VERSION_LGM_C0,
};

static const struct pon_soc_data tpz_emu_data = {
	.pon_shell_init = NULL, // the emulator has no pon shell
	.serdes_basic_init = NULL, // the emulator has no serdes
	.ref_clk_sel = urx800_ref_clk_sel,
	.pll5_init = urx800_pll5_init,
	.serdes_init = NULL, // the emulator has no serdes
	.hw_version_override = PON_MBOX_HW_VER_TPZ_EMU_TYPE, // we do not have FUSE regs in emulator
};

static const struct of_device_id pon_mbox_of_match[] = {
	{ .compatible = "intel,pon_mbox", .data = &prx300_data },
	{ .compatible = "intel,falcon_mx_pon_mbox", .data = &prx300_data },
	{ .compatible = "intel,prx300-pon-mbox", .data = &prx300_data },
	{ .compatible = "mxl,prx300-pon-mbox", .data = &prx300_data },
	{ .compatible = "intel,urx800-pon-mbox", .data = &urx800_data },
	{ .compatible = "mxl,urx800-pon-mbox", .data = &urx800_data },
	{ .compatible = "mxl,urx800c-pon-mbox", .data = &urx800c_data },
	{ .compatible = "mxl,tpz-emu-pon-mbox", .data = &tpz_emu_data },
	{ },
};
MODULE_DEVICE_TABLE(of, pon_mbox_of_match);

#if IS_ENABLED(CONFIG_SPI)
static const struct spi_device_id pon_mbox_spi_ids[] = {
	{ .name = "pon_mbox" },
	{ .name = "falcon_mx_pon_mbox" },
	{ .name = "prx300-pon-mbox" },
	{ .name = "urx800-pon-mbox" },
	{ .name = "urx800c-pon-mbox" },
	{ .name = "tpz-emu-pon-mbox" },
	{ }
};
MODULE_DEVICE_TABLE(spi, pon_mbox_spi_ids);

static struct spi_driver pon_mbox_spi_driver = {
	.probe		= pon_mbox_spi_probe,
	.remove		= pon_mbox_spi_remove,
	.id_table	= pon_mbox_spi_ids,
	.driver	= {
		.name	= DRV_NAME,
		.of_match_table = pon_mbox_of_match,
	}
};
#endif /* CONFIG_SPI */

/**
 * This function is called to read some data from one PON MBOX Host register
 * through the memory mapped interface. This only supports 32 bit aligned
 * messages. Normally a buffer is 32 bit, only for the messages itself it
 * could be bigger than 32 bit.
 */
static int pon_mbox_pdev_read(struct pon_mbox *pon, int reg,
			      void *buf, size_t size)
{
	u32 *result = buf;
	int i;

	if (WARN((size % 4) || !size, "unsupported size of %zu", size))
		return -EINVAL;

	for (i = 0; i < (size / 4); i++) {
		*result = __raw_readl(pon->regbase + (reg * 4));
		result++;
	}

	if (size == 4) {
		dev_dbg(pon->dev, "read: reg: %i, size: %zu, data: %#010x\n",
			reg, size, *--result);
	} else {
		dev_dbg(pon->dev, "read: reg: %i, size: %zu\n", reg, size);
		if (size < 100)
			print_hex_dump_debug("data: ", DUMP_PREFIX_OFFSET, 16,
					     1, buf, size, false);
	}

	return 0;
}

/**
 * This function is called to write some data into one PON MBOX Host register
 * through the memory mapped interface. This only supports 32 bit aligned
 * messages. Normally a buffer is 32 bit, only for the messages itself
 * it could be bigger than 32 bit.
 */
static int pon_mbox_pdev_write(struct pon_mbox *pon, int reg,
			       const void *buf, size_t size, int flag)
{
	const u32 *result = buf;
	int i;

	if (WARN((size % 4) || !size || size > sizeof(buf_byte_order_conv),
		 "unsupported size of %zu", size))
		return -EINVAL;

	if (flag == PON_WR_FLAG_FWDL) {
		for (i = 0; i < (size / 4); i++)
			buf_byte_order_conv[i] = ntohl(*result++);

		result = (const u32 *)&buf_byte_order_conv;
	}

	if (size == 4) {
		dev_dbg(pon->dev, "write: reg: %i, size: %zu, data: %#010x\n",
			reg, size, *result);
	} else {
		dev_dbg(pon->dev, "write: reg: %i, size: %zu\n", reg, size);
		if (size < 100)
			print_hex_dump_debug("data: ", DUMP_PREFIX_OFFSET, 16,
					     1, buf, size, false);
	}

	for (i = 0; i < (size / 4); i++) {
		__raw_writel(*result, pon->regbase + (reg * 4));
		result++;
	}

	return 0;
}

enum pon_mode pon_mbox_get_pon_mode(void)
{
	if (pon_mbox_dev)
		return pon_mbox_dev->mode;
	return PON_MODE_UNKNOWN;
}
EXPORT_SYMBOL(pon_mbox_get_pon_mode);

void pon_mbox_save_pon_dp_flags(const struct pon_dp_flags *dp_flags)
{
	if (pon_mbox_dev)
		pon_mbox_dev->dp_flags = *dp_flags;
}
EXPORT_SYMBOL(pon_mbox_save_pon_dp_flags);

/*
 * This function registers a callback function of the PON Ethernet driver to
 * forward all pon mode changes. The Ethernet driver should check if it is
 * ok to change the mode and reject it in case it would cause trouble.
 */
void pon_mbox_mode_callback_register(int(*func)(enum pon_mode))
{
	pon_mbox_mode_change_callback_func = func;
}
EXPORT_SYMBOL(pon_mbox_mode_callback_register);

static int pon_mbox_pdev_request_irq(struct pon_mbox *pon, const char *name)
{
	int irq;
	int ret;

	irq = platform_get_irq_byname(pon->pdev, name);
	if (irq < 0)
		return irq;

	ret = devm_request_threaded_irq(pon->dev, irq,
					pon_mbox_irq_hard,
					pon_mbox_irq_thread,
					0,
					dev_name(pon->dev), pon);
	if (ret)
		return ret;

	return irq;
}

/* Local variant of "pon_devm_platform_ioremap_resource_byname"
 * which avoids an error printout if the resource is not found in device-tree.
 */
static void __iomem *
pon_devm_platform_ioremap_resource_byname(struct platform_device *pdev,
					  const char *name)
{
	struct resource *res;

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, name);
	if (!res)
		return IOMEM_ERR_PTR(-ENOENT);
	return devm_ioremap_resource(&pdev->dev, res);
}

/**
 * This function does the memory mapped specific init for one device.
 */
static int pon_mbox_pdev_probe(struct platform_device *pdev)
{
	struct pon_mbox *pon;
	struct device *dev = &pdev->dev;
	struct device_node *np = dev_of_node(dev);
	const struct pon_soc_data *soc_data = NULL;
	struct device *mbox_char_dev;
	int ret;

	pon = devm_kzalloc(dev, sizeof(*pon), GFP_KERNEL);
	if (!pon)
		return -ENOMEM;

	pon->pdev = pdev;
	pon->dev = dev;
	pon->read = pon_mbox_pdev_read;
	pon->write = pon_mbox_pdev_write;

	soc_data = of_device_get_match_data(dev);
	if (!soc_data) {
		dev_err(dev, "No data found!\n");
		return -EINVAL;
	}
	pon->soc_spec = soc_data;

	pon->regbase = pon_devm_platform_ioremap_resource_byname(pdev, "ponip");
	if (IS_ERR(pon->regbase))
		return PTR_ERR(pon->regbase);

	pon->serdes = pon_devm_platform_ioremap_resource_byname(pdev, "serdes");
	if (IS_ERR(pon->serdes))
		/* Can fail if already used for AON.
		 * Continue as those registers are not used in
		 * case of AON.
		 */
		pon->serdes = NULL;

	if (pon->serdes) {
		pon->serdes_sram = pon_devm_platform_ioremap_resource_byname(
			pdev, "serdes_sram");
		if (IS_ERR(pon->serdes_sram)) {
			if (PTR_ERR(pon->serdes_sram) == -ENOENT)
				pon->serdes_sram = NULL;
			else
				return PTR_ERR(pon->serdes_sram);
		}
	}

	/* pon_app and pon_apb_app are optional for emulated platforms */
	pon->pon_app =
		pon_devm_platform_ioremap_resource_byname(pdev, "pon_app");
	if (IS_ERR(pon->pon_app)) {
		if (PTR_ERR(pon->pon_app) == -ENOENT) {
			dev_info(dev, "pon_app not present (OK for emulation)\n");
			pon->pon_app = NULL;
		} else {
			return PTR_ERR(pon->pon_app);
		}
	}

	pon->pon_apb_app =
		pon_devm_platform_ioremap_resource_byname(pdev, "pon_apb_app");
	if (IS_ERR(pon->pon_apb_app)) {
		if (PTR_ERR(pon->pon_apb_app) == -ENOENT) {
			dev_info(dev, "pon_apb_app not present (OK for emulation)\n");
			pon->pon_apb_app = NULL;
		} else {
			return PTR_ERR(pon->pon_apb_app);
		}
	}

	pon->reset_wanss = devm_reset_control_get_optional_shared(dev, "wanss");
	if (IS_ERR(pon->reset_wanss)) {
		dev_err(dev, "failed to get wanss reset: %li\n",
			PTR_ERR(pon->reset_wanss));
		return PTR_ERR(pon->reset_wanss);
	}

	pon->reset_ponip = devm_reset_control_get_optional_exclusive(dev,
								     "ponip");
	if (IS_ERR(pon->reset_ponip)) {
		dev_err(dev, "failed to get ponip reset: %li\n",
			PTR_ERR(pon->reset_ponip));
		return PTR_ERR(pon->reset_ponip);
	}

	pon->reset_serdes = devm_reset_control_get_optional_exclusive(dev,
								      "serdes");
	if (IS_ERR(pon->reset_serdes)) {
		dev_err(dev, "failed to get serdes reset: %li\n",
			PTR_ERR(pon->reset_serdes));
		return PTR_ERR(pon->reset_serdes);
	}

	pon->reset_xpcs = devm_reset_control_get_optional_exclusive(dev,
								    "xpcs");
	if (IS_ERR(pon->reset_xpcs)) {
		dev_err(dev, "failed to get xpcs reset: %li\n",
			PTR_ERR(pon->reset_xpcs));
		return PTR_ERR(pon->reset_xpcs);
	}

	/* find related SFP */
	ret = pon_sfp_probe(pon, np);
	if (ret < 0) {
		if (ret == -ENODEV) {
			dev_info(dev, "no link to sfp provided in DT\n");
		} else {
			dev_err(dev, "pon_sfp_probe failed with %i\n", ret);
			return ret;
		}
	}

	/* Optional, try first "mxl" and then "intel" if not found */
	pon->cgu =
		syscon_regmap_lookup_by_phandle_optional(np, "mxl,cgu-syscon");
	if (!pon->cgu)
		pon->cgu = syscon_regmap_lookup_by_phandle_optional(
			np, "intel,cgu-syscon");
	if (IS_ERR(pon->cgu)) {
		dev_err(dev, "failed to get cgu-syscon phandle: %li\n",
			PTR_ERR(pon->cgu));
		return PTR_ERR(pon->cgu);
	}

	/* The chiptop functions are optional, accept with "mxl" or "intel" */
	pon->chiptop = syscon_regmap_lookup_by_phandle_optional(
		np, "mxl,chiptop-syscon");
	if (!pon->chiptop)
		pon->chiptop = syscon_regmap_lookup_by_phandle_optional(
			np, "intel,chiptop-syscon");
	if (IS_ERR(pon->chiptop)) {
		dev_err(dev,
			"failed to get chiptop-syscon phandle: %li\n",
			PTR_ERR(pon->chiptop));
		return PTR_ERR(pon->chiptop);
	}
#if defined(CONFIG_X86_INTEL_LGM) || defined(CONFIG_SOC_LGM)
	if (pon->chiptop) {
		/* Enable voltage comparator for dying gasp on URX */
		ret = regmap_update_bits(pon->chiptop, URX_CHIPTOP_PON_CR,
					 BIT(0), BIT(0));
		if (ret)
			dev_err(pon->dev,
				"Enabling of voltage comparator failed %d\n",
				ret);
	}
#endif

#if !defined(SKIP_TEP) && \
	(defined(CONFIG_X86_INTEL_LGM) || defined(CONFIG_SOC_LGM))
	/* Optional locking of FW download on URX800.
	 * FW can only be downloaded once
	 */
	pon->sectop = syscon_regmap_lookup_by_phandle_optional(
		np, "intel,sectop-syscon");
	if (IS_ERR(pon->sectop)) {
		dev_err(dev, "failed to get intel,sec_top phandle: %li\n",
			PTR_ERR(pon->sectop));
		return PTR_ERR(pon->sectop);
	}
#endif
	/* start with default */
	pon->uart_mode = PON_MBOX_UART_ASC0;

	/* Gate clock is optional - not used on all platforms */
	pon->clk_gate = devm_clk_get(&pdev->dev, "pon");
	if (PTR_ERR(pon->clk_gate) == -ENOENT) {
		pon->clk_gate = NULL;
	} else if (IS_ERR(pon->clk_gate)) {
		dev_err(dev, "failed to get pon clock: %li\n",
			PTR_ERR(pon->clk_gate));
		return PTR_ERR(pon->clk_gate);
	}

	pon->clk_freq = devm_clk_get(&pdev->dev, "freq");
	if (IS_ERR(pon->clk_freq)) {
		dev_err(dev, "failed to get pon freq clock: %li\n",
			PTR_ERR(pon->clk_freq));

		/* For backwards compatibility do not fail if not found */
		if (PTR_ERR(pon->clk_freq) != -ENOENT)
			return PTR_ERR(pon->clk_freq);
		pon->clk_freq = NULL;
	}

	/* Check for Serdes FW to be downloaded - optional */
	pon->serdes_fw_dl =
		device_property_present(&pdev->dev, "mxl,serdes-fw-dl");
	if (!pon->serdes_fw_dl)
		pon->serdes_fw_dl = device_property_present(
			&pdev->dev, "intel,serdes-fw-dl");
	if (pon->serdes_fw_dl)
		dev_dbg(dev, "Serdes FW download enabled\n");
	else
		dev_dbg(dev, "Serdes FW download disabled\n");

	ret = reset_control_deassert(pon->reset_wanss);
	if (ret)
		return ret;

	ret = reset_control_deassert(pon->reset_ponip);
	if (ret)
		return ret;

	/* Serdes reset is deasserted inside function serdes_init which
	 * is executed as soon as all config parameters have been received.
	 * Hence, deassert the reset here only if serdes_init is not available.
	 */
	if (!pon->soc_spec->serdes_init) {
		ret = reset_control_deassert(pon->reset_serdes);
		if (ret)
			return ret;
	}

	ret = clk_set_rate(pon->clk_freq, 625000000);
	if (ret) {
		dev_err(dev, "failed to set pon clock rate: %i\n", ret);
		return ret;
	}

	ret = clk_prepare_enable(pon->clk_freq);
	if (ret) {
		dev_err(dev, "failed to enable pon freq clock: %i\n", ret);
		return ret;
	}

	ret = clk_prepare_enable(pon->clk_gate);
	if (ret) {
		dev_err(dev, "failed to enable pon gate clock: %i\n", ret);
		goto err_clk_freq_disable;
	}

	if (pon->soc_spec->pon_shell_init) {
		ret = pon->soc_spec->pon_shell_init(pon);
		if (ret) {
			dev_err(dev, "failed to init PON shell: %i\n", ret);
			goto err_clk_freq_disable;
		}
	}

	ret = pon_mbox_register(pon);
	if (ret)
		goto err_clk_gate_disable;

	/* IRQs are optional for emulated platforms */
	pon->irqs[0] = pon_mbox_pdev_request_irq(pon, "receiver");
	if (pon->irqs[0] < 0) {
		if (pon->irqs[0] == -EINVAL || pon->irqs[0] == -ENOENT) {
			dev_info(dev, "IRQ receiver not available (OK for emulation)\n");
			pon->irqs[0] = 0;
		} else {
			ret = pon->irqs[0];
			dev_err(dev, "mbox request irq receiver failed\n");
			goto err_clk_gate_disable;
		}
	}

	pon->irqs[1] = pon_mbox_pdev_request_irq(pon, "transmitter");
	if (pon->irqs[1] < 0) {
		if (pon->irqs[1] == -EINVAL || pon->irqs[1] == -ENOENT) {
			dev_info(dev, "IRQ transmitter not available (OK for emulation)\n");
			pon->irqs[1] = 0;
		} else {
			ret = pon->irqs[1];
			dev_err(dev, "mbox request irq transmitter failed\n");
			goto err_clk_gate_disable;
		}
	}

	pon->irqs[2] = pon_mbox_pdev_request_irq(pon, "general");
	if (pon->irqs[2] < 0) {
		if (pon->irqs[2] == -EINVAL || pon->irqs[2] == -ENOENT) {
			dev_info(dev, "IRQ general not available (OK for emulation)\n");
			pon->irqs[2] = 0;
		} else {
			ret = pon->irqs[2];
			dev_err(dev, "mbox request irq general failed\n");
			goto err_clk_gate_disable;
		}
	}

	platform_set_drvdata(pdev, pon);

	pon->cnt_state = pon_mbox_cnt_state_init();
	if (!pon->cnt_state) {
		dev_err(dev, "can't initialize counters state");
		return -ENOMEM;
	}

	pon->cnt_autoupdate =
		pon_mbox_cnt_autoupdate_create(pon,
					       counters_update_timer);
	if (!pon->cnt_autoupdate) {
		dev_err(dev, "can't initialize counters autoupdate");
		goto err_clk_gate_disable;
	}

	mbox_char_dev = device_create(pon_mbox_class, dev, MKDEV(0, 0), NULL,
				      "pon_mbox%u", 0);
	if (IS_ERR(mbox_char_dev)) {
		dev_err(dev, "can't create device: %li\n",
			PTR_ERR(mbox_char_dev));
		ret = PTR_ERR(mbox_char_dev);
		goto err_autoupdate_destroy;
	}

	pon_mbox_dev = pon;

	return 0;

err_autoupdate_destroy:
	pon_mbox_cnt_autoupdate_destroy(pon->cnt_autoupdate);

err_clk_gate_disable:
	clk_disable_unprepare(pon->clk_gate);

err_clk_freq_disable:
	clk_disable_unprepare(pon->clk_freq);

	return ret;
}

static int pon_mbox_pdev_remove(struct platform_device *pdev)
{
	struct pon_mbox *pon = platform_get_drvdata(pdev);

	if (!pon) {
		dev_err(&pdev->dev, "no device data to remove\n");
		return -ENODEV;
	}

	pon_mbox_unregister_all_event_handlers();

	device_destroy(pon_mbox_class, MKDEV(0, 0));

	pon_sfp_remove(pon);

	pon_mbox_cnt_autoupdate_destroy(pon->cnt_autoupdate);
	pon_mbox_cnt_state_release(pon->cnt_state);

	pon_mbox_dev = NULL;

	reset_control_assert(pon->reset_serdes);
	reset_control_assert(pon->reset_ponip);
	reset_control_assert(pon->reset_wanss);

	clk_disable_unprepare(pon->clk_gate);
	clk_disable_unprepare(pon->clk_freq);

	return 0;
}

static struct platform_driver pon_mbox_pdev_driver = {
	.probe		= pon_mbox_pdev_probe,
	.remove		= pon_mbox_pdev_remove,
	.driver	= {
		.name	= DRV_NAME,
		.of_match_table = pon_mbox_of_match,
	}
};

static const struct of_device_id pinselect_match_table[] = {
	{ .compatible = "intel,pon-pinselect-rx-los",
	  .data = &pinctrl_array[PON_MBOX_GPIO_PIN_ID_RX_LOS] },
	{ .compatible = "mxl,pon-pinselect-rx-los",
	  .data = &pinctrl_array[PON_MBOX_GPIO_PIN_ID_RX_LOS] },
	{ .compatible = "intel,pon-pinselect-1pps",
	  .data = &pinctrl_array[PON_MBOX_GPIO_PIN_ID_1PPS] },
	{ .compatible = "mxl,pon-pinselect-1pps",
	  .data = &pinctrl_array[PON_MBOX_GPIO_PIN_ID_1PPS] },
	{ .compatible = "intel,pon-pinselect-ntr",
	  .data = &pinctrl_array[PON_MBOX_GPIO_PIN_ID_NTR] },
	{ .compatible = "mxl,pon-pinselect-ntr",
	  .data = &pinctrl_array[PON_MBOX_GPIO_PIN_ID_NTR] },
	{},
};

static int pinselect_probe(struct platform_device *pdev)
{
	struct pinctrl **pinctrl = NULL;
	const struct of_device_id *match;

	/* getting pointer to the pinselect_match_table
	 * to get access to pinctrl_array
	 */
	match = of_match_node(pinselect_match_table, pdev->dev.of_node);
	if (match && match->data) {
		pinctrl = (struct pinctrl **)match->data;
		/* is this pinctrl already used? */
		if (*pinctrl)
			return -ENODEV;
	}
	if (!pinctrl)
		return -ENODEV;

	*pinctrl = devm_pinctrl_get(&pdev->dev);
	if (IS_ERR(*pinctrl))
		return PTR_ERR(*pinctrl);

	dev_dbg(&pdev->dev, "pinselect %p stored for %p\n", *pinctrl, pinctrl);
	return 0;
}

static struct platform_driver pinselect_drv = {
	.probe = pinselect_probe,
	.driver = {
		.name = "pon-pinselect",
		.of_match_table = pinselect_match_table,
	}
};

static int __init pon_mbox_driver_init(void)
{
	int ret;
	enum {
		REG_NONE,
		REG_MBOX_CLASS,
		REG_GENL_FAMILY,
		REG_PLATFORM_DRV,
		REG_SPI_DRV
	};
	u32 mod_regs = REG_NONE;


#if (KERNEL_VERSION(6, 2, 0) > LINUX_VERSION_CODE)
	pon_mbox_class = class_create(THIS_MODULE, "pon_mbox");
#else
	pon_mbox_class = class_create("pon_mbox");
#endif
	if (IS_ERR(pon_mbox_class)) {
		pr_err("Error %ld creating class 'pon_mbox'!\n",
		       PTR_ERR(pon_mbox_class));
		return PTR_ERR(pon_mbox_class);
	}
	mod_regs |= BIT(REG_MBOX_CLASS);

	ret = genl_register_family(&pon_mbox_genl_family);
	if (ret) {
		pr_err("can't register generic netlink");
		goto out_destroy;
	}
	mod_regs |= BIT(REG_GENL_FAMILY);

#if IS_ENABLED(CONFIG_SPI)
	ret = spi_register_driver(&pon_mbox_spi_driver);
	if (ret) {
		pr_err("can't register spi driver");
		goto out_destroy;
	}
	mod_regs |= BIT(REG_SPI_DRV);
#endif
	ret = platform_driver_register(&pon_mbox_pdev_driver);
	if (ret) {
		pr_err("can't register platform driver");
		goto out_destroy;
	}
	mod_regs |= BIT(REG_PLATFORM_DRV);

	ret = platform_driver_register(&pinselect_drv);
	if (ret) {
		pr_err("can't register pinselect driver");
		goto out_destroy;
	}

	return 0;

out_destroy:
	if (mod_regs & BIT(REG_PLATFORM_DRV))
		platform_driver_unregister(&pon_mbox_pdev_driver);
#if IS_ENABLED(CONFIG_SPI)
	if (mod_regs & BIT(REG_SPI_DRV))
		spi_unregister_driver(&pon_mbox_spi_driver);
#endif
	if (mod_regs & BIT(REG_GENL_FAMILY))
		genl_unregister_family(&pon_mbox_genl_family);
	if (mod_regs & BIT(REG_MBOX_CLASS))
		class_destroy(pon_mbox_class);

	return ret;
}
module_init(pon_mbox_driver_init);

static void __exit pon_mbox_driver_exit(void)
{
	platform_driver_unregister(&pinselect_drv);
	platform_driver_unregister(&pon_mbox_pdev_driver);
#if IS_ENABLED(CONFIG_SPI)
	spi_unregister_driver(&pon_mbox_spi_driver);
#endif
	genl_unregister_family(&pon_mbox_genl_family);

	class_destroy(pon_mbox_class);
}
module_exit(pon_mbox_driver_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("MaxLinear Inc");
MODULE_VERSION(__stringify(PACKAGE_VERSION));
MODULE_FIRMWARE(PON_MBOX_FW_GPON_NAME_A);
MODULE_FIRMWARE(PON_MBOX_FW_GPON_NAME_B);
MODULE_FIRMWARE(PON_MBOX_FW_GPON_NAME_URX_A);
MODULE_FIRMWARE(PON_MBOX_FW_GPON_NAME_URX_B);
MODULE_FIRMWARE(PON_MBOX_FW_GPON_NAME_URX_C);
MODULE_FIRMWARE(PON_MBOX_FW_GPON_NAME_TPZ);
MODULE_FIRMWARE(PON_MBOX_FW_XPON_NAME_A);
MODULE_FIRMWARE(PON_MBOX_FW_XPON_NAME_B);
MODULE_FIRMWARE(PON_MBOX_FW_XPON_NAME_URX_A);
MODULE_FIRMWARE(PON_MBOX_FW_XPON_NAME_URX_B);
MODULE_FIRMWARE(PON_MBOX_FW_XPON_NAME_URX_C);
MODULE_FIRMWARE(PON_MBOX_FW_XPON_NAME_TPZ);

/** @} */
