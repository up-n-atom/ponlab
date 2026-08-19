/****************************************************************************** 

  Copyright © 2020-2021 MaxLinear, Inc.
  Copyright 2016 - 2020 Intel Corporation

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.
 
******************************************************************************/

/*
 * This module contains all the vrx518 tc driver callback function.
 */

#include <linux/skbuff.h>
#include <net/lantiq_cbm_api.h>
#include "common/ima_vrx518_tc_interface.h"
#include "atm/ima_atm_rx.h"
#include "atm/ima_rx_bonding.h"
#include "atm/ima_tx_module.h"
#include "asm/ima_asm.h"
#include "common/ima_modules.h"

/*
 * Called by vrx518 tc driver. Task of this function is to return the statistics.
 */
static void bonding_get_stats(int p_i_epid, struct atm_bonding_stats *p_st_p_bonding_stats)
{
pr_info("[%s:%d] Entry\n", __func__, __LINE__);
	/* TODO : fetch the Tx/Rx stats and return */
pr_info("[%s:%d] Exit\n", __func__, __LINE__);
}

/*
 * Called by vrx518 tc driver. Task of this function is to indicate that do
 * not send any traffic to the corresponding line.
 */
static int showtime_exit(int p_i_ep_id)
{
pr_info("[%s:%d] Entry\n", __func__, __LINE__);

	/* update asm module */
	asm_showtime_exit(p_i_ep_id);
pr_info("[%s:%d] Exit\n", __func__, __LINE__);

	return 0;
}

/*
 * Call by vrx518 tc driver. Task of this function is to indicate that the
 * corresponding line is ready to send the traffic.
 */
static int showtime_enter(int p_i_ep_id)
{
pr_info("[%s:%d] Entry\n", __func__, __LINE__);

	/* update asm module */
	asm_showtime_enter(p_i_ep_id);
pr_info("[%s:%d] Exit\n", __func__, __LINE__);

	return 0;
}

/*
 * Called by vrx518 tc driver. Task of this function is to remove the
 * corresponding device parameters from private data. This function shall be
 * called upon vrx518 device remove.
 */
static int bonding_device_deinit(void)
{
	int i_ret = 0;

pr_info("[%s:%d] Entry\n", __func__, __LINE__);
	/******************************
	 * Stop common polling thread
	 ******************************/
	stop_modules();

	/*****************************
	 * Cleanup device asm metadata
	 *****************************/
	cleanup_device_asm_metadata();

	/*****************************
	 * Cleanup device rx metadata
	 *****************************/
	cleanup_device_rx_metadata();

	/*****************************
	 * Cleanup device rx metadata
	 *****************************/
	cleanup_device_tx_metadata();

pr_info("[%s:%d] Exit\n", __func__, __LINE__);

	return i_ret;
}

/*
 * Called by vrx518 tc driver with device parameters information. Task of
 * this function is to store the passed device parameters. This function shall
 * be called upon vrx518 device probe.
 */
static int bonding_device_init(struct bond_drv_cfg_params *p_st_p_device_info)
{
	int i_ret = 0;

pr_info("[%s:%d] Entry\n", __func__, __LINE__);
	do {
		if (!p_st_p_device_info) {
			ERROR_PRINTK("null pointer argument");
			i_ret = -1;
			break;
		}

		/**********************************
		 * Initialize device tx metadata
		 **********************************/
		init_device_tx_metadata(p_st_p_device_info);

		/********************************
		 * Initialize device rx metadata
		 ********************************/
		init_device_rx_metadata(p_st_p_device_info);

		/********************************
		 * Initialize device asm metadata
		 ********************************/
		init_device_asm_metadata(p_st_p_device_info);

		/*******************************
		 * Start common polling thread
		 *******************************/
		i_ret = start_modules(1000 /*usecs*/ );

		if (i_ret != RS_SUCCESS) {
			i_ret = -1;
			break;
		}

	} while (0);
pr_info("[%s:%d] Exit\n", __func__, __LINE__);

	return i_ret;
}

struct bonding_cb_ops g_st_bonding_cb_ops = {
	.cb_bonding_device_init = bonding_device_init,
	.cb_bonding_device_deinit = bonding_device_deinit,
	.cb_showtime_enter = showtime_enter,
	.cb_showtime_exit = showtime_exit,
	.cb_bonding_get_stats = bonding_get_stats
};

struct tc_cb_ops g_st_tc_cb_ops = {
	.rx_atm_frame_cb = NULL,
	.tc_drv_cbm_buf_free = NULL,
	.tc_drv_cbm_dqptr_read = NULL,
	.cb_tc_receive_asm_ind = NULL,
	.cb_tc_decap_rx_frame = NULL,
	.cb_tc_encap_tx_frame = NULL
};

/*
 * Task of this function is to initialize the vrx518 tc interface module and
 * register vrx518 tc driver callbacks.
 */
s32 init_vrx518_tc_interface(void)
{
	s32 ret = RS_SUCCESS;

	ret = vrx_register_cb_init(&g_st_bonding_cb_ops, &g_st_tc_cb_ops);

	if (ret != RS_SUCCESS)
		ERROR_PRINTK("vrx_register_cb_init() failed");

	return ret;
}

/*
 * Task of this function is to cleanup all the module initialized data and
 * unregister vrx518 tc driver callbacks.
 */
s32 cleanup_vrx518_tc_interface(void)
{
	s32 ret = RS_SUCCESS;

	vrx_deregister_cb_uninit();

	return ret;
}

/*
 * This is an interface function between vrx518 tc module and atm receive
 * module. Implementation of this function is in vrx518 tc module. Task of
 * this function is to pass the received aal5 frame to vrx518 tc driver. This
 * function shall be called by atm receive module.
 */
int send_to_vrx518_tc_driver(struct atm_aal5_t *p_st_p_aal5_frame)
{
	int ret = 0;

	do {
		if (!g_st_tc_cb_ops.rx_atm_frame_cb) {
			ret = -1;
			ERROR_PRINTK("null pointer, g_st_tc_cb_ops.rx_atm_frame_cb");
			kfree_skb(p_st_p_aal5_frame->skb);
			break;
		}

		ret = g_st_tc_cb_ops.rx_atm_frame_cb(p_st_p_aal5_frame);

	} while (0);

	return ret;
}

int tc_decap_rx_pkt(struct atm_aal5_t *atm_rx_pkt)
{
	if (!g_st_tc_cb_ops.cb_tc_decap_rx_frame) {
		pr_err("TC handler (cb_tc_decap_rx_frame) is NULL\n");
		return -1;
	}

	return g_st_tc_cb_ops.cb_tc_decap_rx_frame(atm_rx_pkt);
}

/*
 * This is an interface function between atm transmit module and vrx518 tc
 * interface module, implementation is in vrx518 tc interface module. This
 * function is called by atm transmit tasklet. Task of this function is to
 * release the host cbm buffer.
 */
s32 release_cbm_buffer(void *p_v_p_cbm_buff_ptr)
{
	s32 ret = RS_SUCCESS;

	do {
		if (!p_v_p_cbm_buff_ptr) {
			ERROR_PRINTK("null cbm buffer pointer");
			ret = RS_FAILURE;
			break;
		}

		if (!g_st_tc_cb_ops.tc_drv_cbm_buf_free) {
			ERROR_PRINTK("null function pointer");
			ret = RS_FAILURE;
			break;
		}

		g_st_tc_cb_ops.tc_drv_cbm_buf_free(p_v_p_cbm_buff_ptr);

	} while (0);

	return ret;
}

/*
 * This is an interface function between vrx518 tc module and polling thread
 * module. Implementation of this function is in vrx518 tc module. Task of
 * this function is to return the umt counter via tc callback.
 */
s32 get_umt(u32 *counter)
{
	s32 ret = RS_FAILURE;

	if (g_st_tc_cb_ops.tc_drv_cbm_dqptr_read)
		ret = g_st_tc_cb_ops.tc_drv_cbm_dqptr_read(counter);

	return ret;
}

#define is_mpoa_pt_set(id) ((id >> 2) & 0x1)
#define set_mpoa_pt(id) (id | 0x4)
void *tc_encap_tx_pkt(u8 *buff, u32 *bufflen, u16 *dest_id, u8 *unrecognized)
{
	s32 ret;
	u32 real_buf_start;
	u32 headroom;
	int real_len;
	struct sk_buff *skb;
	struct atm_aal5_t atm_tx_pkt = {0};

	*unrecognized = 0;

	if (is_mpoa_pt_set(*dest_id))
		return buff;

	/* MPOA_PT not set, encapsulate it into aal5 frame */
	real_buf_start = ((u32)buff & 0xfffff800);
	headroom = (u32)buff - real_buf_start;
	real_len = SKB_DATA_ALIGN(headroom + *bufflen) +
		SKB_DATA_ALIGN(sizeof(struct skb_shared_info));
	skb = cbm_build_skb((void *)real_buf_start, real_len, GFP_ATOMIC);
	if (skb) {
		/* Create headroom from cbm buffer's reserved space */
		skb_reserve(skb, headroom);
		skb_put(skb, *bufflen);

		atm_tx_pkt.skb = skb;
		atm_tx_pkt.conn = (*dest_id >> 3) & 0xf;
		if (g_st_tc_cb_ops.cb_tc_encap_tx_frame)
			ret = g_st_tc_cb_ops.cb_tc_encap_tx_frame(&atm_tx_pkt);
		else
			ret = -1;
		if (ret) *unrecognized = 1;
		buff = skb->data;
		*bufflen = skb->len;

		skb->head = NULL;
		kfree_skb(skb);
	}

	*dest_id = set_mpoa_pt(*dest_id);
	return buff;
}

void first_asm_received(u8 linkid)
{
	if (g_st_tc_cb_ops.cb_tc_receive_asm_ind) {
		pr_crit("Bonding driver: First ASM received after SHOWTIME-ENTRY on link %d indicated to TC\n", linkid);
		g_st_tc_cb_ops.cb_tc_receive_asm_ind(linkid);
	}
}
