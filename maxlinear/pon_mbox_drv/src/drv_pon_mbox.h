/*****************************************************************************
 *
 * Copyright (c) 2020 - 2025 MaxLinear, Inc.
 * Copyright (c) 2017 - 2020 Intel Corporation
 * Copyright (c) 2016 Lantiq Beteiligungs-GmbH & Co. KG
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/**
 * \file drv_pon_mbox.h
 *
 */

#ifndef __DRV_PON_MBOX_H
#define __DRV_PON_MBOX_H

#include <pon/pon_ip_msg.h>
#include <pon/pon_mbox_ikm.h>
#include <pon/pon_mbox.h>

/** \addtogroup PON_MBOX
 *  @{
 */

/** PON HW register definitions */

/** This register provides the interrupt enable (mask) bits,
 *  of the corresponding bits in the Status Register.
 */
#define PON_MBOX_IEN			0x00

/** This register provides the status.
 *  The status flags are divided into status conditions,
 *  coming from the hardware (FIFOs) and status bits reported by FW/SW (events).
 */
#define PON_MBOX_STAT			0x01

/** This register provides the status: Bootmode valid. */
#define  PON_MBOX_STAT_BOOT_VAL		BIT(0)

/** This register provides the status: RX command error .*/
#define  PON_MBOX_STAT_RXERR		BIT(1)

/** This register provides the status: TX command error */
#define  PON_MBOX_STAT_TXERR		BIT(2)

/** This register provides the status: Boot debug request */
#define  PON_MBOX_STAT_BOOT_REQ		BIT(15)

/** This register provides the status: Boot debug end */
#define  PON_MBOX_STAT_BOOT_END		BIT(14)

/** This register provides the status: Boot debug keep */
#define  PON_MBOX_STAT_BOOT_KEEP	BIT(13)

/** This register provides the status: Boot debug fail */
#define  PON_MBOX_STAT_BOOT_FAIL	BIT(11)

/** This register provides the status: Transmitter 1 ready */
#define  PON_MBOX_STAT_X1_RDY		BIT(17)

/** This register provides the status: Transmitter 1 empty */
#define  PON_MBOX_STAT_X1_EMP		BIT(18)

/** This register provides the status: Receiver 1 Data */
#define  PON_MBOX_STAT_R1_DATA		BIT(22)

/** This register provides the status: Transmitter 1 Data */
#define  PON_MBOX_STAT_X1_DATA		BIT(23)

/** This register provides the status: Receiver 1 Underflow */
#define  PON_MBOX_STAT_R1_UFL		BIT(26)

/** This register provides the status: Transmitter 1 Overflow */
#define  PON_MBOX_STAT_X1_OFL		BIT(27)

/** This register provides the data interface to the bidirectional FIFO 1. */
#define PON_MBOX_DATA1			0x02
/** This register provides the length status of the bidirectional FIFO 1. */
#define PON_MBOX_LEN1			0x04
/** This field returns the number of available elements in the receiver. */
#define  PON_MBOX_LEN1_RLEN_MASK	0x000001FF
/** This field returns the number of available elements in the receiver. */
#define  PON_MBOX_LEN1_RLEN_SHIFT	0
/** Mask number of messages in the receiver queue of the mailbox type A. */
#define  PON_MBOX_LEN1_RMSG_MASK_A	0x0000F000
/** Shift number of messages in the receiver queue of the mailbox type A. */
#define  PON_MBOX_LEN1_RMSG_SHIFT_A	12
/** Mask number of messages in the receiver queue of the mailbox type B. */
#define  PON_MBOX_LEN1_RMSG_MASK_B	0x0000FE00
/** Shift number of messages in the receiver queue of the mailbox type B. */
#define  PON_MBOX_LEN1_RMSG_SHIFT_B	9
/** This field returns the number of free elements in the transmitter. */
#define  PON_MBOX_LEN1_XLEN_MASK	0x01FF0000
/** This field returns the number of free elements in the transmitter. */
#define  PON_MBOX_LEN1_XLEN_SHIFT	16
/** Mask number of messages in the transmitter queue of the mailbox type A. */
#define  PON_MBOX_LEN1_XMSG_MASK_A	0xF0000000
/** Shift number of messages in the transmitter queue of the mailbox type A. */
#define  PON_MBOX_LEN1_XMSG_SHIFT_A	28
/** Mask number of messages in the transmitter queue of the mailbox type B. */
#define  PON_MBOX_LEN1_XMSG_MASK_B	0xFE000000
/** Shift number of messages in the transmitter queue of the mailbox type B. */
#define  PON_MBOX_LEN1_XMSG_SHIFT_B	25

/** This register provides the threshold configuration,
 *  of the bidirectional FIFO 1.
 */
#define PON_MBOX_LIMIT1			0x06

/** This register provides the command interface,
 *  for the FIFOs and the FW/SW events.
 */
#define PON_MBOX_CMD			0x08

/** Boot mode. */
#define  PON_MBOX_CMD_BOOT_MODE		BIT(0)

/** Send acknowledge for PON_MBOX_STAT_BOOT_REQ to PON FW */
#define  PON_MBOX_CMD_BOOT_ACK		BIT(15)

/** FIFO Command Control: Transmitter 1. */
#define  PON_MBOX_CMD_X1		(0x00 << 24)

/** FIFO Command Control: Receiver 1. */
#define  PON_MBOX_CMD_R1		(0x01 << 24)

/** FIFO command: End of message. */
#define  PON_MBOX_CMD_EOM		(0x01 << 28)

/** FIFO command: FIFO Reset. */
#define  PON_MBOX_CMD_RST		(0x03 << 28)

/** This register provides the configuration,
 *  for the connected controller (HOST/CPU).
 */
#define PON_MBOX_CFG			0x09

/** May be used to reset the connected controller with the generated pulse. */
#define  PON_MBOX_CFG_RST		BIT(0)

/** May be used to enable the connected controller. */
#define  PON_MBOX_CFG_EN		BIT(1)

#define PON_MBOX_REG(x)			(0x10 + (x))

/** This register is available to FW/SW layer,
 *  for general purpose use and used as BOOT_CMD register.
 */
#define PON_MBOX_BOOT_CMD		PON_MBOX_REG(0)

/** Boot from ROM code, but in PONIP the same as BOOT_RAM. */
#define  PON_MBOX_BOOT_CMD_ROM		0
/** Boot with XMODEM-Download via terminal (115k). */
#define  PON_MBOX_BOOT_CMD_TERM		1
/** Boot with XMODEM-Download via terminal (230k). */
#define  PON_MBOX_BOOT_CMD_TERMH	2
/** Boot from HOST interface. */
#define  PON_MBOX_BOOT_CMD_HOST		3
/** Boot with bootloader debugger. */
#define  PON_MBOX_BOOT_CMD_DEBUG	4
/** Boot with direct entry to (preloaded) RAM (warm start). */
#define  PON_MBOX_BOOT_CMD_RAM		5
/** Delete memory, DRAM=0, CRAM filled with illegal memory location traps */
#define  PON_MBOX_BOOT_CMD_INIT		6
/** Boot with simple memory test. */
#define  PON_MBOX_BOOT_CMD_TEST		7

/** This register is available to FW/SW layer,
 *  for general purpose use and used as BOOT_CAUSE register.
 */
#define PON_MBOX_BOOT_CAUSE		PON_MBOX_REG(1)

/* Temporary define for expected value */
/* TODO: check boot_cause for all unexpected values */
#define  PON_MBOX_BOOT_CAUSE_NORMAL	 1

/** This register is available to FW/SW layer,
 *  for general purpose use and used as BOOT_STAT register.
 */
#define PON_MBOX_BOOT_STAT		PON_MBOX_REG(2)

/* Hardware reset */
#define  PON_MBOX_BOOT_STAT_HWRES	 0
/* Temporary define for expected value */
/* TODO: check boot_stat for all unexpected values */
#define  PON_MBOX_BOOT_STAT_NORMAL	 1
/* Host reset */
#define  PON_MBOX_BOOT_STAT_HOSTRES	 2
/* Firmware reset */
#define  PON_MBOX_BOOT_STAT_FWRES	 3
/* Terminal reset */
#define  PON_MBOX_BOOT_STAT_TERMRES	 4
/* Debugger reset */
#define  PON_MBOX_BOOT_STAT_DEBUGRES	 5
/* Download error */
#define  PON_MBOX_BOOT_STAT_LOADERR	 8
/* Interrupt error */
#define  PON_MBOX_BOOT_STAT_INTERR	 9
/* Watchdog error */
#define  PON_MBOX_BOOT_STAT_WDGERR	10
/* Division-by-zero error */
#define  PON_MBOX_BOOT_STAT_DIVERR	11
/* Software reset */
#define  PON_MBOX_BOOT_STAT_SWRES	12
/* Memory error */
#define  PON_MBOX_BOOT_STAT_MEMERR	13
/* Memory cleared */
#define  PON_MBOX_BOOT_STAT_MEMCLEAR	14
/* Memory test pass */
#define  PON_MBOX_BOOT_STAT_MEMPASS	15
/* Memory test fail */
#define  PON_MBOX_BOOT_STAT_MEMFAIL	16
/* Request download of firmware image 0 issued by the FW itself */
#define  PON_MBOX_BOOT_STAT_LOAD0	24
/* Request download of firmware image 1 issued by the FW itself */
#define  PON_MBOX_BOOT_STAT_LOAD1	25
/* Request download of firmware image 2 issued by the FW itself */
#define  PON_MBOX_BOOT_STAT_LOAD2	26
/* Request download of firmware image 3 issued by the FW itself */
#define  PON_MBOX_BOOT_STAT_LOAD3	27
/* Request download of firmware image 4 issued by the FW itself */
#define  PON_MBOX_BOOT_STAT_LOAD4	28
/* Request download of firmware image 5 issued by the FW itself */
#define  PON_MBOX_BOOT_STAT_LOAD5	29
/* Terminal setup complete */
#define  PON_MBOX_BOOT_STAT_TERM	0x43

/* This register is available to FW/SW layer,
 * for general purpose use and used as BOOT_ID register.
 */
#define PON_MBOX_BOOT_ID		PON_MBOX_REG(3)

/* This register is available to FW/SW layer,
 * for general purpose use and used as BOOT_VERSION register.
 */
#define PON_MBOX_BOOT_VERSION		PON_MBOX_REG(4)

/* This register is available to FW/SW layer,
 * for general purpose use and used for FUSE_DATA0 reporting.
 */
#define PON_MBOX_FUSE0			PON_MBOX_REG(6)

/* PON IP Hardware Version. */
#define  PON_MBOX_FUSE0_VER_MASK	0x000000ff
#define  PON_MBOX_FUSE0_VER_SHIFT	0

/* The HW version of the PON IP used in PRX300 mailbox type A */
#define PON_MBOX_HW_VER_A_TYPE		2
/* The HW version of the PON IP used in PRX300 mailbox type B */
#define PON_MBOX_HW_VER_B_TYPE		(PONFW_VERSION_HWVERSION_PRX_B)
/* The HW version of the PON IP used in URX800 mailbox type A */
#define PON_MBOX_HW_VER_URX_A_TYPE	(PONFW_VERSION_HWVERSION_URX_A)
/* The HW version of the PON IP used in URX800 mailbox type B */
#define PON_MBOX_HW_VER_URX_B_TYPE	(PONFW_VERSION_HWVERSION_URX_B)
/* The HW version of the PON IP used in URX800 mailbox type C */
#define PON_MBOX_HW_VER_URX_C_TYPE	(PONFW_VERSION_HWVERSION_URX_C)
/* The HW version of the PON IP used in TPZ EMULATION mailbox */
#define PON_MBOX_HW_VER_TPZ_EMU_TYPE	(PONFW_VERSION_HWVERSION_TPZ)

/* PON IP Hardware Identifier. */
#define  PON_MBOX_FUSE0_ID_MASK		0x00ffff00
#define  PON_MBOX_FUSE0_ID_SHIFT	8
/* Shows whether this is a simulation run. */
#define  PON_MBOX_FUSE0_SIM		BIT(28)

/* This register is available to FW/SW layer,
 * for general purpose use and used for FUSE_DATA1 reporting.
 */
#define PON_MBOX_FUSE1			PON_MBOX_REG(7)

/* If 1 all debugging features are switched off.
 * This must be the case on all non-engineering samples.
 */
#define  PON_MBOX_FUSE1_DISDEB		BIT(0)

/* If 1 all encryption hardware is switched off. */
#define  PON_MBOX_FUSE1_DISEC		BIT(1)
/* If 1 all decryption hardware is switched off. */
#define  PON_MBOX_FUSE1_DISDC		BIT(2)

/* This register is available to FW/SW layer,
 * for general purpose use and used for FUSE_DATA2 reporting.
 */
#define PON_MBOX_FUSE2			PON_MBOX_REG(8)

/* If this bit is 1, the mentioned standard is supported. */
#define  PON_MBOX_FUSE2_GPON		BIT(0)
/* If this bit is 1, the mentioned standard is supported. */
#define  PON_MBOX_FUSE2_XGSPON		BIT(1)
/* If this bit is 1, the mentioned standard is supported. */
#define  PON_MBOX_FUSE2_XGPON1		BIT(2)
/* If this bit is 1, the mentioned standard is supported. */
#define  PON_MBOX_FUSE2_NGPON2		BIT(3)

/* If this bit is 1, the mentioned data rate,
 * is supported in downstream direction.
 */
#define  PON_MBOX_FUSE2_DS1G25		BIT(16)

/* If this bit is 1, the mentioned data rate,
 * is supported in downstream direction.
 */
#define  PON_MBOX_FUSE2_DS2G5		BIT(17)

/* If this bit is 1, the mentioned data rate,
 * is supported in downstream direction.
 */
#define  PON_MBOX_FUSE2_DS10G		BIT(18)

/* If this bit is 1, the downstream contains the decryption logic. */
#define  PON_MBOX_FUSE2_DSDC		BIT(23)

/* If this bit is 1, the mentioned data rate,
 * is supported in upstream direction.
 */
#define  PON_MBOX_FUSE2_US1G25		BIT(24)

/* If this bit is 1, the mentioned data rate,
 * is supported in upstream direction.
 */
#define  PON_MBOX_FUSE2_US2G5		BIT(25)

/* If this bit is 1, the mentioned data rate,
 * is supported in upstream direction.
 */
#define  PON_MBOX_FUSE2_US10G		BIT(26)

/* If this bit is 1, the mentioned data rate,
 * is supported in upstream direction.
 */
#define  PON_MBOX_FUSE2_USEC		BIT(31)

/** ASCII: ON */
#define PON_MBOX_MAGIC			0x4f4e

/*
 * Maximal number of IRQ lines supported by this driver,
 * Falcon MX has 3 IRQ lines, PON IP FPGA has 1 IRQ line.
 */
#define PON_MBOX_MAX_IRQ_LINES		3

/* The mail box is organized in 32 bit / 4 bytes words */
#define PON_MBOX_MAX_SIZE		1000	/* 1000 bytes or 256 words */
#define PON_MBOX_BYTES_PER_WORD		4	/* 4 bytes per word */

#define PON_MBOX_PENDING_MAX 8

/** \defgroup PON_MBOX_DRV Basic PON Mailbox Driver Structures
 *  This chapter describes the basic structures used by the PON Mailbox driver.
 *  @{
 */

/** Message header structure for mailbox access.
 */
struct pon_msg_header {
#ifdef __BIG_ENDIAN_BITFIELD
	/** Read/Write selection.
	 * - 0: RD, Read access.
	 * - 1: WR, Write access.
	 */
	u32 rw : 1;
	/** Command ID */
	u32 cmd : 15;
	/** Acknowledge */
	u32 ack : 4;
	/** Source message initiator */
	u32 src : 1;
	/** Sequence Number, a value from 0 to 7. */
	u32 seq : 3;
	/** Length of the following DATA fields, up to 255 byte. */
	u32 len : 8;
#elif defined(__LITTLE_ENDIAN_BITFIELD)
	/** Length of the following DATA fields, up to 255  */
	u32 len : 8;
	/** Sequence Number, a value from 0 to 7. */
	u32 seq : 3;
	/** Source message initiator */
	u32 src : 1;
	/** Acknowledge */
	u32 ack : 4;
	/** Command ID */
	u32 cmd : 15;
	/** Read/Write selection.
	 * - 0: RD, Read access.
	 * - 1: WR, Write access.
	 */
	u32 rw : 1;
#endif
} __packed;

#define UNUSED(x) (void)(x)

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))
#endif

/** Loss of signal (GTC, XGTC, optical interface) */
#define PON_ALARM_ID_STATIC_LOS 0x0000
/** Persistent crossbar access error */
#define PON_ALARM_ID_STATIC_PCE 0x0017
/** Persistent SerDes Error */
#define PON_ALARM_ID_STATIC_PSE 0x0018
/**
 * Firmware version error, the loaded firmware does not fit to the PON IP
 * hardware version.
 */
#define PON_ALARM_ID_EDGE_FWVERR 0x0101

/** Sources where the message could be coming from */
enum pon_mbox_msg_origin {
	/** The message was send through NetLink */
	PON_MBOX_MSG_ORIGIN_NETLINK,
	/** The message was send through the in kernel interface */
	PON_MBOX_MSG_ORIGIN_KERNEL
};

/** Internal data structure which is filled up with the data of a response
 *  the FW sends to the SW as an answer to and previous request.
 *  This structure is used by \ref pon_mbox_read_msg, \ref pon_mbox_write_msg,
 *  \ref pon_mbox_release_pending_or_complete and \ref pon_mbox_send.
 */
struct pon_mbox_ikm {
	/** Used for waiting for response */
	struct completion response_received;
	/** Pointer with result data */
	void *result_data;
	/** Size of result_data in bytes */
	int result_size;
	/** Status of the operation (0 - success) */
	int status;
};

/** This represents one FW message which was send to the FW and where no
 *  answer was received from the FW. This stores the metadata in the driver
 *  corresponding to a message send to the FW.
 */
struct pon_mbox_pending {
	/** Used for reservation of 'pending' object */
	bool in_use;
	/** Netlink port ID*/
	u32 snd_portid;
	/** NetLink Sequence number */
	u32 snd_seq;
	/** Origin of request: netlink or in-kernel messaging */
	enum pon_mbox_msg_origin origin;
	/** Net structure (Netlink) if origin == netlink */
	struct net *net;
	/** Reference to IKM object if origin == ikm */
	struct pon_mbox_ikm ikm;
	/** Timestamp for ordering */
	u64 timestamp;
};

/** Internal data structure containing configuration
 *  parameters used for SW loop timing.
 */
struct pon_lt_cfg {
	/** Loop timing config received and valid */
	bool valid;
	/** Loop Timing Power Save mode enable */
	bool loop_ps_en;
};

/** Internal data structure containing iop configuration
 */
struct pon_iop_cfg {
	/** Loop timing config received and valid */
	bool valid;
	/** Configured IOP mask */
	struct ponfw_onu_interop_config msk;
};

/** Internal data structure containing SerDes configuration
 *  parameters used by \ref pon_mbox_write_msg prx300_serdes_init
 *  and \ref urx800_serdes_init
 */
struct pon_serdes_cfg {
	/** SerDes basic init done */
	bool srds_binit_done;
	/** SerDes init done */
	bool srds_init_done;
	/** SerDes config received and valid */
	bool valid;
	/** Serdes config parameter array based on netlink defines */
	u32 param[PON_MBOX_SRDS_MAX + 1];
};

/* Forward declaration of structure pon_mbox */
struct pon_mbox;

/** Internal data structure containing SoC specific
 *  function pointers and data
 *  for SerDes, clk and PLL initialization.
 */
struct pon_soc_data {
	/** Function to perform PON shell init settings */
	int (*pon_shell_init)(struct pon_mbox *pon);
	/** Function to perform basic SerDes settings */
	int (*serdes_basic_init)(struct pon_mbox *pon);
	/** Function to select the reference clk for SerDes and PON IP */
	int (*ref_clk_sel)(struct pon_mbox *pon);
	/** Function to perform PPL5 initialization */
	int (*pll5_init)(struct pon_mbox *pon);
	/** Function to perform the SerDes initialization */
	int (*serdes_init)(struct pon_mbox *pon);
	/** Use this as hardcoded fake version
	 *  in case the device cannot identify (e.g. unfused)
	 */
	int hw_version_override;
	/** Specify here a specific/alternate HW version
	 *  to indicate to the FW via PONFW_HW_VERSION_CMD_ID message
	 */
	int hw_version_firmware;
};

struct pon_sfp_priv;

/** Driver data structure representing one PON IP device.
 */
struct pon_mbox {
	/** Low level platform reference */
	union {
		/** Reference to SPI device */
		struct spi_device *spi;
		/** Reference to platform device */
		struct platform_device *pdev;
	};
	/** Reference to Linux device */
	struct device *dev;
	/** Array with the PON MBOX IRQs */
	int irqs[PON_MBOX_MAX_IRQ_LINES];
	/** Normal PON interrupt mask, stored to restore it after it was
	 *  overwritten in interrupt handler
	 */
	u32 irq_mask;
	/** Mirror of the limit HW value */
	u32 limit1;
	/** Function to read data from a mailbox register */
	int (*read)(struct pon_mbox *pon, int reg, void *buf, size_t size);
	/** Function to write data to a mailbox register
	 *  Convert byte order depending on parameter flag.
	 *  (Different handling required depending on platform (SoC/HAPS)
	 *  and on the action to be performed (e.g. FW download or regular
	 *  register access)
	 */
	int (*write)(struct pon_mbox *pon, int reg, const void *buf,
		     size_t size, int flag);
	/** Mapped mailbox memory region */
	void __iomem *regbase;
	/** Mapped PON serdes memory region */
	void __iomem *serdes;
	/** Mapped PON serdes SRAM memory region for serdes FW download */
	void __iomem *serdes_sram;
	/** Mapped PON application logic memory region */
	void __iomem *pon_app;
	/** Mapped PON APB application logic memory region */
	void __iomem *pon_apb_app;
	/** WAN subsystem reset */
	struct reset_control *reset_wanss;
	/** PON IP reset */
	struct reset_control *reset_ponip;
	/** SerDes reset */
	struct reset_control *reset_serdes;
	/** WAN XPCS reset */
	struct reset_control *reset_xpcs;
	/** PON IP gating clock */
	struct clk *clk_gate;
	/** PON IP frequency clock */
	struct clk *clk_freq;
	/** CGU registers */
	struct regmap *cgu;
	/** CHIPTOP registers */
	struct regmap *chiptop;
#if !defined(SKIP_TEP) && \
	(defined(CONFIG_X86_INTEL_LGM) || defined(CONFIG_SOC_LGM))
	/** URX security registers */
	struct regmap *sectop;
	/** URX PON FW download locked */
	bool urx_fw_dl_locked;
#endif
	/** Wait for the next message to TX */
	wait_queue_head_t tx_waitqueue;
	/** Protect the interrupt mask register access */
	struct mutex irq_lock;
	/** True if TX of next message is possible */
	bool tx_ready;
	/** True if mailbox is in reset mode */
	bool in_reset;
	/* True if the PON IP is in debug mode, deactivate FW download and
	 * counter update.
	 */
	bool pon_ip_debug_mode;
	/** Loop timing configuration */
	struct pon_lt_cfg lt_cfg;
	/** True if the WAN XPCS was already put into reset */
	bool reset_xpcs_done;
	/** Next mailbox sequence number to use */
	int next_seq;
	/** The pending FW messages which were send, but no answer received yet.
	 */
	struct pon_mbox_pending pending[PON_MBOX_PENDING_MAX];
	/** PON mode which should be used to configure the PON to */
	enum pon_mode mode;
	/** Context for counters autoupdate */
	struct cnt_autoupdate *cnt_autoupdate;
	/** Current PLOAM state */
	u32 ploam_state;
	/** HW version */
	unsigned int hw_ver;
	/** Counters state */
	struct counters_state *cnt_state;
	/** Last configured UART mode */
	u8 uart_mode;
	/** PON FW download retry counter in case of an error */
	int fw_dl_retry_cnt;
	/** Start time for in ns for SerDes bit error measurement */
	u64 biterror_start_time;
	/** Last update time for in ns for SerDes bit error measurement */
	u64 biterror_update_time;
	/** Accumulated bit error counter */
	u64 biterror_counter;
	/** Lock for updates of counter and time */
	struct mutex biterror_lock;
	/** Reference to bit error polling thread */
	struct task_struct *biterror_count_thread;
	/** SoC specific function pointers and data */
	const struct pon_soc_data *soc_spec;
	/** SerDes configuration */
	struct pon_serdes_cfg serdes_cfg;
	/** Indication whether to download SerDes FW */
	bool serdes_fw_dl;
	/** True if we got a Persistent SerDes error (PSE).
	 * Will be cleared when reaching O5.
	 */
	bool serdes_error;
	/** IOP configuration */
	struct pon_iop_cfg iop_cfg;
	/** Related SFP */
	struct pon_sfp_priv *sfp;
	/** Store the flags provided by the eth driver */
	struct pon_dp_flags dp_flags;
};

#define PON_MBOX_MAX_CMD_USERS 3
/** User UID array used on access management for command groups */
struct pon_mbox_user_perm {
	kuid_t uid_list[PON_MBOX_MAX_CMD_USERS];
};

/** Pointer to the pon device. Use only internally in the driver. */
extern struct pon_mbox *pon_mbox_dev;

/** Function to probe for a reference to a related SFP I2C bus */
int pon_sfp_probe(struct pon_mbox *pon, struct device_node *np);

/** Remove SFP related data */
void pon_sfp_remove(struct pon_mbox *pon);

/** @} */ /* PON_MBOX_DRV */

/** @} */ /* PON_MBOX */

#endif /* __DRV_PON_MBOX_H */
