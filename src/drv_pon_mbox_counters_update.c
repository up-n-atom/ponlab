/*****************************************************************************
 *
 * Copyright (c) 2020 - 2025 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <linux/device.h>

#include <pon/pon_ip_msg.h>
#include <pon/pon_mbox.h>
#include <pon/pon_mbox_ikm.h>
#include <drv_pon_mbox.h>
#include <drv_pon_mbox_counters_conv.h>
#include <drv_pon_mbox_counters_update.h>

#ifndef BIT_ULL
#define BIT_ULL(nr) (1ULL << (nr))
#endif

/* Each of indexes below points to counter-specific auxiliary control data
 * stored in the internal auxiliary array in generic_counters_update(),
 * and at the same time being assigned to a field of primary counter-specific
 * data it marks the need of auxiliary pon_mbox_send() call
 */
enum cnt_ctrl_aux_idx {
	NONE_CNT = -1,
	ALLOC_BW_CNT = 0,
	GTC_ENHANCED_CNT = 1,
	MAX_CTRL_AUX_CNT = 2,
};

/* Results of counters-specific functions *_counters_init() marking if given
 * pon mode supports a given counter and if to proceed updating this counter
 */
enum cnt_init_status {
	/* Update counter */
	CNT_UPDATE_READY = 0,
	/* Do not update counter */
	CNT_UPDATE_SKIP = 1
};

/* Unified counter-specific, caller interface data
 * to be updated on the base of firmware
 */
union pon_cnt {
	struct pon_mbox_gem_port_counters gem_port;
	struct pon_alloc_counters alloc;
	struct pon_mbox_gtc_counters gtc;
	struct pon_mbox_xgtc_counters xgtc;
	struct pon_alloc_discard_counters alloc_discard;
	struct pon_eth_counters rx_eth;
	struct pon_eth_counters tx_eth;
	struct pon_mbox_twdm_lods_counters twdm_lods;
	struct pon_mbox_twdm_optic_pl_counters twdm_optic_pl;
	struct pon_mbox_twdm_tc_counters twdm_tc;
	struct pon_mbox_xgtc_ploam_ds_counters xgtc_ploam_ds;
	struct pon_mbox_gtc_ploam_ds_counters gtc_ploam_ds;
	struct pon_mbox_xgtc_ploam_us_counters xgtc_ploam_us;
	struct pon_mbox_gtc_ploam_us_counters gtc_ploam_us;
};

/* Unified counter-specific data to be updated by pon_mbox_send() */
union ponfw_cnt {
	struct ponfw_gem_port_counters gem_port;
	struct ponfw_alloc_id_counters aid;
	struct ponfw_alloc_bw abw;
	struct ponfw_gtc_counters gtc;
	struct ponfw_gtc_enhanced_counters gtc_enh;
	struct ponfw_xgtc_counters xgtc;
	struct ponfw_alloc_lost_counters alloc_lost;
	struct ponfw_rx_eth_counters rx_eth;
	struct ponfw_tx_eth_counters tx_eth;
	struct ponfw_twdm_lods_counters twdm_lods;
	struct ponfw_twdm_onu_optic_pl_counters onu_optic_pl;
	struct ponfw_twdm_tc_counters twdm_tc;
	struct ponfw_xgtc_ploam_ds_counters xgtc_ploam_ds;
	struct ponfw_gtc_ploam_ds_counters gtc_ploam_ds;
	struct ponfw_xgtc_ploam_us_counters xgtc_ploam_us;
	struct ponfw_gtc_ploam_us_counters gtc_ploam_us;
};

/* Counter-specific firmware interface data
 * grouped for single call of pon_mbox_send()
 */
struct ponfw_cnt_io {
	/* Input to be passed to firmware */
	union ponfw_cnt in;
	/* Output handling result from firmware */
	union ponfw_cnt out;
};

/* Counter-specific firmware interface data grouped for
 * all pon_mbox_send() calls which are required for one counter
 */
struct ponfw_cnt_io_grp {
	/* Primary data used in all counters */
	struct ponfw_cnt_io prime;
	/* Auxiliary data used only in few cases */
	struct ponfw_cnt_io aux;
};

/* Common structure for counter-specific parameters for pon_mbox_send() */
struct cnt_ctrl {
	/* Index to counter-specific auxiliary pon_mbox_send() parameters stored
	 * in auxiliary array of 'struct cnt_ctrl_aux' elements
	 */
	int ctrl_aux_idx;
	/* Command id */
	int cmd;
	/* Length of 'in' data */
	int lenr;
	/* Length of 'out' data */
	int len;
	/* Pointer to a function preparing 'in' data for pon_mbox_send()
	 * and checking pon mode
	 * - [in] cnt_io_grp: counter-specific data for the firmware
	 * - [in] twdm_dswlch_id: TWDM DS Wavelength ID
	 * - [in] port_idx: port index for counter (e.g. link index, gem index)
	 * - [in] pon_mbox_dev: pointer to PON mailbox driver data structure
	 * Return value:
	 * - CNT_UPDATE_READY - proceed with updating
	 * - CNT_UPDATE_SKIP - skip updating
	 */
	enum cnt_init_status (*init_fp)(struct ponfw_cnt_io_grp *cnt_io_grp,
					u8 twdm_dswlch_id, u8 port_idx,
					struct pon_mbox *pon_mbox_dev);
	/* Pointer to a function updating counters on the base of result
	 * from firmware (from pon_mbox_send())
	 * - [out] cnt: counter-specific data to update
	 * - [in] port_idx: port index for counter (e.g. link index, gem index)
	 * - [in] cnt_io_grp: counter-specific data from firmware
	 * - [in] pon_mbox_dev: pointer to PON mailbox driver data structure
	 */
	int (*update_fp)(union pon_cnt *cnt, u8 port_idx,
			 struct ponfw_cnt_io_grp *cnt_io_grp,
			 struct pon_mbox *pon_mbox_dev);

	/* Pointer to a function updating last_update time for suitable counters
	 * - [in] port_idx: port index for counter (e.g. link index, gem index)
	 * - [in/out] state: pointer to counter_state data structure
	 */
	void (*last_update_fp)(u8 port_idx, struct counters_state *state);
};

/* Auxiliary data structure for cases when it is required to update counter */
struct cnt_ctrl_aux {
	/* Command id to pass to auxiliary pon_mbox_send() */
	int cmd;
	/* Length of auxiliary data structure 'in' for pon_mbox_send() */
	int lenr;
	/* Length of auxiliary data structure 'out' for pon_mbox_send() */
	int len;
};

void gem_port_id_write_update(u8 gem_port_idx)
{
	struct pon_mbox *pon = pon_mbox_dev;
	int err;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return;
	}

	err = pon_mbox_cnt_gem_port_add(gem_port_idx, pon->cnt_state);
	if (err == -EEXIST) {
		dev_dbg(pon->dev,
			"Adding GEM port %u to counter storage was not necessary, because it was already added\n",
			gem_port_idx);
		return;
	}
}
EXPORT_SYMBOL(gem_port_id_write_update);

void gem_port_id_remove_update(u8 gem_port_idx)
{
	struct pon_mbox *pon = pon_mbox_dev;
	int err;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return;
	}

	err = pon_mbox_cnt_gem_port_del(gem_port_idx, pon->cnt_state);
	if (err < 0 && err != -ENOENT)
		dev_err(pon->dev,
			"cannot delete GEM %u port from storage: err: %i\n",
			gem_port_idx, err);
}
EXPORT_SYMBOL(gem_port_id_remove_update);

static bool is_ngpon2_mode(struct pon_mbox *pon_mbox_dev)
{
	if (pon_mbox_dev->mode == PON_MODE_989_NGPON2_2G5 ||
	    pon_mbox_dev->mode == PON_MODE_989_NGPON2_10G)
		return 1;

	return 0;
}

static bool need_ngpon2_update(struct pon_mbox *pon_mbox_dev, u8 twdm_dswlch_id)
{
	if (pon_mbox_dev->cnt_state->twdm_ds_idx == twdm_dswlch_id ||
	    twdm_dswlch_id == PON_MBOX_D_DSWLCH_ID_ACC)
		return 1;

	return 0;
};

static enum cnt_init_status
gem_port_counters_init(struct ponfw_cnt_io_grp *cnt_io_grp, u8 twdm_dswlch_id,
		       u8 port_idx, struct pon_mbox *pon_mbox_dev)
{
	if (is_ngpon2_mode(pon_mbox_dev))
		if (!need_ngpon2_update(pon_mbox_dev, twdm_dswlch_id))
			return CNT_UPDATE_SKIP;

	cnt_io_grp->prime.in.gem_port.gem_port_idx = port_idx;

	return CNT_UPDATE_READY;
}

static int gem_port_counters_update(union pon_cnt *cnt, u8 port_idx,
				    struct ponfw_cnt_io_grp *cnt_io_grp,
				    struct pon_mbox *pon_mbox_dev)
{
	struct pon_mbox_gem_port_counters *counters_copy = &cnt->gem_port;
	struct ponfw_gem_port_counters *fw_output =
		&cnt_io_grp->prime.out.gem_port;
	struct pon_mbox_gem_port_counters *counters;
	struct pon_mbox_gem_port_counters fw_counters = { 0 };
	int err;

	(void)pon_mbox_cnt_gem_port_fw2pon(fw_output, &fw_counters);

	pon_mbox_cnt_lock(pon_mbox_dev->cnt_state);

	counters = pon_mbox_cnt_gem_port_table_get(
		PON_MBOX_D_DSWLCH_ID_CURR, port_idx, pon_mbox_dev->cnt_state);

	/* This workaround is added because some of GEM ports are not created by
	 * calling FAPI functions and in this case they are not added to the
	 * storage. It is called when we want to get counters for GEM port which
	 * is not in storage only for the first time.
	 */
	if (!counters) {
		err = pon_mbox_cnt_gem_port_add(port_idx,
						pon_mbox_dev->cnt_state);
		if (err < 0) {
			dev_err(pon_mbox_dev->dev,
				"cannot add GEM port index to storage: err: %i\n",
				err);
			goto cnt_err;
		}

		counters = pon_mbox_cnt_gem_port_table_get(
			PON_MBOX_D_DSWLCH_ID_CURR, port_idx,
			pon_mbox_dev->cnt_state);
		if (!counters) {
			dev_err(pon_mbox_dev->dev,
				"cannot get counters table from storage\n");
			err = -ENOENT;
			goto cnt_err;
		}
	}

	err = pon_mbox_cnt_gem_port_table_add(port_idx, &fw_counters,
					      pon_mbox_dev->cnt_state);
	if (err) {
		dev_err(pon_mbox_dev->dev,
			"cannot add counters to storage: err: %i\n", err);
		goto cnt_err;
	}

	if (counters_copy)
		memcpy(counters_copy, counters, sizeof(*counters_copy));

cnt_err:
	pon_mbox_cnt_unlock(pon_mbox_dev->cnt_state);
	pon_mbox_cnt_release(counters);

	return err;
}

static void
pon_mbox_cnt_gem_port_last_update_update(u8 port_idx,
					 struct counters_state *state)
{
	mutex_lock(&state->lock);
	state->last_update.gem_port_counters[port_idx] = jiffies;
	mutex_unlock(&state->lock);
}

void alloc_id_write_update(u8 alloc_idx)
{
	struct pon_mbox *pon = pon_mbox_dev;
	int err;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return;
	}

	err = pon_mbox_cnt_alloc_id_add(alloc_idx, pon->cnt_state);
	if (err < 0 && err != -EEXIST)
		dev_err(pon->dev,
			"cannot add Alloc ID %u to storage: err: %i\n",
			alloc_idx, err);
}
EXPORT_SYMBOL(alloc_id_write_update);

void alloc_id_remove_update(u8 alloc_idx)
{
	struct pon_mbox *pon = pon_mbox_dev;
	int err;

	if (!pon) {
		pr_err("no pon mailbox device found\n");
		return;
	}

	err = pon_mbox_cnt_alloc_id_del(alloc_idx, pon->cnt_state);
	if (err < 0 && err != -ENOENT)
		dev_err(pon->dev,
			"cannot delete Alloc ID %u from storage: err: %i\n",
			alloc_idx, err);
}
EXPORT_SYMBOL(alloc_id_remove_update);

static enum cnt_init_status
alloc_counters_init(struct ponfw_cnt_io_grp *cnt_io_grp, u8 twdm_dswlch_id,
		    u8 port_idx, struct pon_mbox *pon_mbox_dev)
{
	if (is_ngpon2_mode(pon_mbox_dev))
		if (!need_ngpon2_update(pon_mbox_dev, twdm_dswlch_id))
			return CNT_UPDATE_SKIP;

	/* Some FW commands are not allowed when PLOAM state is not appropriate
	 */
	if (pon_mbox_dev->mode == PON_MODE_984_GPON) {
		/* In G.984 mode (GPON) do not fetch Allocation Bandwidth
		 * if PLOAM state is lower than O3
		 */
		if (pon_mbox_dev->ploam_state < 30)
			return CNT_UPDATE_SKIP;
	} else {
		/* In other ITU modes, do not fetch Allocation Bandwidth
		 * if PLOAM state is lower than O23
		 */
		if (pon_mbox_dev->ploam_state < 23)
			return CNT_UPDATE_SKIP;
	}

	cnt_io_grp->prime.in.aid.alloc_idx = port_idx;
	cnt_io_grp->aux.in.abw.alloc_idx = port_idx;

	return CNT_UPDATE_READY;
}

static int alloc_counters_update(union pon_cnt *cnt, u8 port_idx,
				 struct ponfw_cnt_io_grp *cnt_io_grp,
				 struct pon_mbox *pon_mbox_dev)
{
	struct pon_alloc_counters *counters_copy = &cnt->alloc;
	struct ponfw_alloc_id_counters *aid_fw_output =
		&cnt_io_grp->prime.out.aid;
	struct ponfw_alloc_bw *abw_fw_output = &cnt_io_grp->aux.out.abw;
	struct pon_alloc_counters *counters;
	struct pon_alloc_counters fw_counters = { 0 };
	int err;

	(void)pon_mbox_cnt_alloc_id_fw2pon(aid_fw_output, &fw_counters);

	(void)pon_mbox_cnt_alloc_bw_fw2pon(abw_fw_output, &fw_counters);

	pon_mbox_cnt_lock(pon_mbox_dev->cnt_state);

	counters = pon_mbox_cnt_alloc_id_table_get(port_idx,
						   pon_mbox_dev->cnt_state);

	/* This workaround is added because some of Alloc ID can be not created
	 * by calling FAPI functions and in this case they are not added to the
	 * storage. It is called when we want to get counters for Alloc ID which
	 * is not in storage only for the first time.
	 */
	if (!counters) {
		err = pon_mbox_cnt_alloc_id_add(port_idx,
						pon_mbox_dev->cnt_state);
		if (err < 0) {
			dev_err(pon_mbox_dev->dev,
				"cannot add Alloc ID index to storage: err: %i\n",
				err);
			goto cnt_err;
		}

		counters = pon_mbox_cnt_alloc_id_table_get(
			port_idx, pon_mbox_dev->cnt_state);
		if (!counters) {
			dev_err(pon_mbox_dev->dev,
				"cannot get counters table from storage\n");
			err = -ENOENT;
			goto cnt_err;
		}
	}

	err = pon_mbox_cnt_alloc_id_table_add(port_idx, &fw_counters,
					      pon_mbox_dev->cnt_state);
	if (err) {
		dev_err(pon_mbox_dev->dev,
			"cannot add counters to storage: err: %i\n", err);
		goto cnt_err;
	}

	if (counters_copy)
		memcpy(counters_copy, counters, sizeof(*counters_copy));

cnt_err:
	pon_mbox_cnt_unlock(pon_mbox_dev->cnt_state);
	pon_mbox_cnt_release(counters);

	return err;
}

static void pon_mbox_cnt_alloc_last_update_update(u8 port_idx,
						  struct counters_state *state)
{
	mutex_lock(&state->lock);
	state->last_update.alloc_counters[port_idx] = jiffies;
	mutex_unlock(&state->lock);
}

static enum cnt_init_status
gtc_counters_init(struct ponfw_cnt_io_grp *cnt_io_grp, u8 twdm_dswlch_id,
		  u8 port_idx, struct pon_mbox *pon_mbox_dev)
{
	(void)port_idx;
	(void)cnt_io_grp;
	if (is_ngpon2_mode(pon_mbox_dev))
		if (!need_ngpon2_update(pon_mbox_dev, twdm_dswlch_id))
			return CNT_UPDATE_SKIP;

	return CNT_UPDATE_READY;
}

static int gtc_counters_update(union pon_cnt *cnt, u8 port_idx,
			       struct ponfw_cnt_io_grp *cnt_io_grp,
			       struct pon_mbox *pon_mbox_dev)
{
	struct pon_mbox_gtc_counters *counters_copy = &cnt->gtc;
	struct ponfw_gtc_counters *gtc_fw_output = &cnt_io_grp->prime.out.gtc;
	struct ponfw_gtc_enhanced_counters *gtc_enh_fw_output =
		&cnt_io_grp->aux.out.gtc_enh;
	struct pon_mbox_gtc_counters *counters;
	struct pon_mbox_gtc_counters fw_counters = { 0 };

	(void)port_idx;
	(void)pon_mbox_cnt_gtc_fw2pon(gtc_fw_output, &fw_counters);

	(void)pon_mbox_cnt_gtc_enh_fw2pon(gtc_enh_fw_output, &fw_counters);

	pon_mbox_cnt_lock(pon_mbox_dev->cnt_state);

	counters = pon_mbox_cnt_gtc_table_get(PON_MBOX_D_DSWLCH_ID_CURR,
					      pon_mbox_dev->cnt_state);

	(void)pon_mbox_cnt_gtc_table_add(&fw_counters, pon_mbox_dev->cnt_state);

	if (counters_copy)
		memcpy(counters_copy, counters, sizeof(*counters_copy));

	pon_mbox_cnt_unlock(pon_mbox_dev->cnt_state);
	pon_mbox_cnt_release(counters);

	return 0;
}

static void pon_mbox_cnt_gtc_last_update_update(u8 port_idx,
						struct counters_state *state)
{
	(void)port_idx;
	mutex_lock(&state->lock);
	state->last_update.gtc_counters = jiffies;
	mutex_unlock(&state->lock);
}

static enum cnt_init_status
xgtc_counters_init(struct ponfw_cnt_io_grp *cnt_io_grp, u8 twdm_dswlch_id,
		   u8 port_idx, struct pon_mbox *pon_mbox_dev)
{
	(void)port_idx;
	(void)cnt_io_grp;
	/* XGTC counters are not supported by ITU-T G.984 */
	if (pon_mbox_dev->mode == PON_MODE_984_GPON)
		return CNT_UPDATE_SKIP;

	if (is_ngpon2_mode(pon_mbox_dev))
		if (!need_ngpon2_update(pon_mbox_dev, twdm_dswlch_id))
			return CNT_UPDATE_SKIP;

	return CNT_UPDATE_READY;
}

static int xgtc_counters_update(union pon_cnt *cnt, u8 port_idx,
				struct ponfw_cnt_io_grp *cnt_io_grp,
				struct pon_mbox *pon_mbox_dev)
{
	struct pon_mbox_xgtc_counters *counters_copy = &cnt->xgtc;
	struct ponfw_xgtc_counters *fw_output = &cnt_io_grp->prime.out.xgtc;
	struct pon_mbox_xgtc_counters *counters;
	struct pon_mbox_xgtc_counters fw_counters = { 0 };

	(void)port_idx;
	(void)pon_mbox_cnt_xgtc_fw2pon(fw_output, &fw_counters);

	pon_mbox_cnt_lock(pon_mbox_dev->cnt_state);

	counters = pon_mbox_cnt_xgtc_table_get(PON_MBOX_D_DSWLCH_ID_CURR,
					       pon_mbox_dev->cnt_state);

	(void)pon_mbox_cnt_xgtc_table_add(&fw_counters,
					  pon_mbox_dev->cnt_state);

	if (counters_copy)
		memcpy(counters_copy, counters, sizeof(*counters_copy));

	pon_mbox_cnt_unlock(pon_mbox_dev->cnt_state);
	pon_mbox_cnt_release(counters);

	return 0;
}

static void pon_mbox_cnt_xgtc_last_update_update(u8 port_idx,
						 struct counters_state *state)
{
	(void)port_idx;
	mutex_lock(&state->lock);
	state->last_update.xgtc_counters = jiffies;
	mutex_unlock(&state->lock);
}

static enum cnt_init_status
alloc_lost_counters_init(struct ponfw_cnt_io_grp *cnt_io_grp, u8 twdm_dswlch_id,
			 u8 port_idx, struct pon_mbox *pon_mbox_dev)
{
	(void)port_idx;
	(void)cnt_io_grp;
	if (is_ngpon2_mode(pon_mbox_dev))
		if (!need_ngpon2_update(pon_mbox_dev, twdm_dswlch_id))
			return CNT_UPDATE_SKIP;

	/* Some FW commands are not allowed when PLOAM state is not appropriate
	 */
	if (pon_mbox_dev->mode == PON_MODE_984_GPON) {
		/* In G.984 mode (GPON) do not fetch Allocation Lost Counters
		 * if PLOAM state is lower than O3
		 */
		if (pon_mbox_dev->ploam_state < 30)
			return CNT_UPDATE_SKIP;
	} else {
		/* In other ITU modes, do not fetch Allocation Lost Counters
		 * if PLOAM state is lower than O23
		 */
		if (pon_mbox_dev->ploam_state < 23)
			return CNT_UPDATE_SKIP;
	}

	return CNT_UPDATE_READY;
}

static int alloc_lost_counters_update(union pon_cnt *cnt, u8 port_idx,
				      struct ponfw_cnt_io_grp *cnt_io_grp,
				      struct pon_mbox *pon_mbox_dev)
{
	struct pon_alloc_discard_counters *counters_copy = &cnt->alloc_discard;
	struct ponfw_alloc_lost_counters *fw_output =
		&cnt_io_grp->prime.out.alloc_lost;
	struct pon_alloc_discard_counters *counters;
	struct pon_alloc_discard_counters fw_counters = { 0 };

	(void)port_idx;
	(void)pon_mbox_cnt_alloc_lost_fw2pon(fw_output, &fw_counters);

	pon_mbox_cnt_lock(pon_mbox_dev->cnt_state);

	counters = pon_mbox_cnt_alloc_lost_table_get(pon_mbox_dev->cnt_state);

	(void)pon_mbox_cnt_alloc_lost_table_add(&fw_counters,
						pon_mbox_dev->cnt_state);

	if (counters_copy)
		memcpy(counters_copy, counters, sizeof(*counters_copy));

	pon_mbox_cnt_unlock(pon_mbox_dev->cnt_state);
	pon_mbox_cnt_release(counters);

	return 0;
}

static void
pon_mbox_cnt_alloc_lost_last_update_update(u8 port_idx,
					   struct counters_state *state)
{
	(void)port_idx;
	mutex_lock(&state->lock);
	state->last_update.alloc_discard_counters = jiffies;
	mutex_unlock(&state->lock);
}

static enum cnt_init_status
rx_eth_counters_init(struct ponfw_cnt_io_grp *cnt_io_grp, u8 twdm_dswlch_id,
		     u8 port_idx, struct pon_mbox *pon_mbox_dev)
{
	if (is_ngpon2_mode(pon_mbox_dev))
		if (!need_ngpon2_update(pon_mbox_dev, twdm_dswlch_id))
			return CNT_UPDATE_SKIP;

	cnt_io_grp->prime.in.rx_eth.gem_idx = port_idx;

	return CNT_UPDATE_READY;
}

static int rx_eth_counters_update(union pon_cnt *cnt, u8 port_idx,
				  struct ponfw_cnt_io_grp *cnt_io_grp,
				  struct pon_mbox *pon_mbox_dev)
{
	struct pon_eth_counters *counters_copy = &cnt->rx_eth;
	struct ponfw_rx_eth_counters *fw_output = &cnt_io_grp->prime.out.rx_eth;
	struct pon_eth_counters *counters;
	struct pon_eth_counters fw_counters = { 0 };
	int err;

	(void)pon_mbox_cnt_rx_eth_fw2pon(fw_output, &fw_counters);

	pon_mbox_cnt_lock(pon_mbox_dev->cnt_state);

	counters = pon_mbox_cnt_rx_eth_table_get(port_idx,
						 pon_mbox_dev->cnt_state);

	/* This workaround is added because some of GEM ports are not created by
	 * calling FAPI functions and in this case they are not added to the
	 * storage. It is called when we want to get counters for GEM port which
	 * is not in storage only for the first time.
	 */
	if (!counters) {
		err = pon_mbox_cnt_gem_port_add(port_idx,
						pon_mbox_dev->cnt_state);
		if (err < 0) {
			dev_err(pon_mbox_dev->dev,
				"cannot add GEM port index to storage: err: %i\n",
				err);
			goto cnt_err;
		}

		counters = pon_mbox_cnt_rx_eth_table_get(
			port_idx, pon_mbox_dev->cnt_state);
		if (!counters) {
			dev_err(pon_mbox_dev->dev,
				"cannot get counters table from storage\n");
			err = -ENOENT;
			goto cnt_err;
		}
	}

	err = pon_mbox_cnt_rx_eth_table_add(port_idx, &fw_counters,
					    pon_mbox_dev->cnt_state);
	if (err) {
		dev_err(pon_mbox_dev->dev,
			"cannot add counters to storage: err: %i\n", err);
		goto cnt_err;
	}

	if (counters_copy)
		memcpy(counters_copy, counters, sizeof(*counters_copy));

cnt_err:
	pon_mbox_cnt_unlock(pon_mbox_dev->cnt_state);
	pon_mbox_cnt_release(counters);

	return err;
}

static void pon_mbox_cnt_eth_rx_last_update_update(u8 port_idx,
						   struct counters_state *state)
{
	mutex_lock(&state->lock);
	state->last_update.eth_rx_counters[port_idx] = jiffies;
	mutex_unlock(&state->lock);
}

static enum cnt_init_status
tx_eth_counters_init(struct ponfw_cnt_io_grp *cnt_io_grp, u8 twdm_dswlch_id,
		     u8 port_idx, struct pon_mbox *pon_mbox_dev)
{
	if (is_ngpon2_mode(pon_mbox_dev))
		if (!need_ngpon2_update(pon_mbox_dev, twdm_dswlch_id))
			return CNT_UPDATE_SKIP;

	cnt_io_grp->prime.in.tx_eth.idx = port_idx;

	return CNT_UPDATE_READY;
}

static int tx_eth_counters_update(union pon_cnt *cnt, u8 port_idx,
				  struct ponfw_cnt_io_grp *cnt_io_grp,
				  struct pon_mbox *pon_mbox_dev)
{
	struct pon_eth_counters *counters_copy = &cnt->tx_eth;
	struct ponfw_tx_eth_counters *fw_output = &cnt_io_grp->prime.out.tx_eth;
	struct pon_eth_counters *counters;
	struct pon_eth_counters fw_counters = { 0 };
	int err;

	(void)pon_mbox_cnt_tx_eth_fw2pon(fw_output, &fw_counters);

	pon_mbox_cnt_lock(pon_mbox_dev->cnt_state);

	counters = pon_mbox_cnt_tx_eth_table_get(port_idx,
						 pon_mbox_dev->cnt_state);

	/* This workaround is added because some of GEM ports are not created by
	 * calling FAPI functions and in this case they are not added to the
	 * storage. It is called when we want to get counters for GEM port which
	 * is not in storage only for the first time.
	 */
	if (!counters) {
		err = pon_mbox_cnt_gem_port_add(port_idx,
						pon_mbox_dev->cnt_state);
		if (err < 0) {
			dev_err(pon_mbox_dev->dev,
				"cannot add GEM port index to storage: err: %i\n",
				err);
			goto cnt_err;
		}

		counters = pon_mbox_cnt_tx_eth_table_get(
			port_idx, pon_mbox_dev->cnt_state);
		if (!counters) {
			dev_err(pon_mbox_dev->dev,
				"cannot get counters table from storage\n");
			err = -ENOENT;
			goto cnt_err;
		}
	}

	err = pon_mbox_cnt_tx_eth_table_add(port_idx, &fw_counters,
					    pon_mbox_dev->cnt_state);
	if (err) {
		dev_err(pon_mbox_dev->dev,
			"cannot add counters to storage: err: %i\n", err);
		goto cnt_err;
	}

	if (counters_copy)
		memcpy(counters_copy, counters, sizeof(*counters_copy));

cnt_err:
	pon_mbox_cnt_unlock(pon_mbox_dev->cnt_state);
	pon_mbox_cnt_release(counters);

	return err;
}

static void pon_mbox_cnt_eth_tx_last_update_update(u8 port_idx,
						   struct counters_state *state)
{
	mutex_lock(&state->lock);
	state->last_update.eth_tx_counters[port_idx] = jiffies;
	mutex_unlock(&state->lock);
}

static enum cnt_init_status
pon_lods_counters_init(struct ponfw_cnt_io_grp *cnt_io_grp, u8 twdm_dswlch_id,
		       u8 port_idx, struct pon_mbox *pon_mbox_dev)
{
	(void)port_idx;
	(void)cnt_io_grp;
	/* Counters are only supported in non GPON mode */
	if (pon_mbox_dev->mode == PON_MODE_984_GPON)
		return CNT_UPDATE_SKIP;

	/* Update NGPON2 counter for the current WL Ch ID and accumulated */
	if (need_ngpon2_update(pon_mbox_dev, twdm_dswlch_id))
		return CNT_UPDATE_READY;

	return CNT_UPDATE_SKIP;
}

static int twdm_lods_counters_update(union pon_cnt *cnt, u8 port_idx,
				     struct ponfw_cnt_io_grp *cnt_io_grp,
				     struct pon_mbox *pon_mbox_dev)
{
	struct pon_mbox_twdm_lods_counters *counters_copy = &cnt->twdm_lods;
	struct ponfw_twdm_lods_counters *fw_output =
		&cnt_io_grp->prime.out.twdm_lods;
	struct pon_mbox_twdm_lods_counters *counters;
	struct pon_mbox_twdm_lods_counters fw_counters = { 0 };

	(void)port_idx;
	(void)pon_mbox_cnt_twdm_lods_fw2pon(fw_output, &fw_counters);

	pon_mbox_cnt_lock(pon_mbox_dev->cnt_state);

	counters = pon_mbox_cnt_twdm_lods_table_get(PON_MBOX_D_DSWLCH_ID_CURR,
						    pon_mbox_dev->cnt_state);

	(void)pon_mbox_cnt_twdm_lods_table_add(&fw_counters,
					       pon_mbox_dev->cnt_state);

	if (counters_copy)
		memcpy(counters_copy, counters, sizeof(*counters_copy));

	pon_mbox_cnt_unlock(pon_mbox_dev->cnt_state);
	pon_mbox_cnt_release(counters);

	return 0;
}

static void
pon_mbox_cnt_twdm_lods_last_update_update(u8 port_idx,
					  struct counters_state *state)
{
	(void)port_idx;
	mutex_lock(&state->lock);
	state->last_update.twdm_lods_counters = jiffies;
	mutex_unlock(&state->lock);
}

static enum cnt_init_status
twdm_optic_pl_counters_init(struct ponfw_cnt_io_grp *cnt_io_grp,
			    u8 twdm_dswlch_id, u8 port_idx,
			    struct pon_mbox *pon_mbox_dev)
{
	(void)port_idx;
	(void)cnt_io_grp;
	/* Counters are only supported in NG-PON2 mode */
	if (!is_ngpon2_mode(pon_mbox_dev))
		return CNT_UPDATE_SKIP;
	/* Update NG-PON2 counter for the current WL Ch ID and accumulated */
	if (need_ngpon2_update(pon_mbox_dev, twdm_dswlch_id))
		return CNT_UPDATE_READY;

	return CNT_UPDATE_SKIP;
}

static int twdm_optic_pl_counters_update(union pon_cnt *cnt, u8 port_idx,
					 struct ponfw_cnt_io_grp *cnt_io_grp,
					 struct pon_mbox *pon_mbox_dev)
{
	struct pon_mbox_twdm_optic_pl_counters *counters_copy =
		&cnt->twdm_optic_pl;
	struct ponfw_twdm_onu_optic_pl_counters *fw_output =
		&cnt_io_grp->prime.out.onu_optic_pl;
	struct pon_mbox_twdm_optic_pl_counters *counters;
	struct pon_mbox_twdm_optic_pl_counters fw_counters = { 0 };

	(void)port_idx;
	(void)pon_mbox_cnt_twdm_optic_pl_fw2pon(fw_output, &fw_counters);

	pon_mbox_cnt_lock(pon_mbox_dev->cnt_state);

	counters = pon_mbox_cnt_twdm_optic_pl_table_get(
		PON_MBOX_D_DSWLCH_ID_CURR, pon_mbox_dev->cnt_state);

	(void)pon_mbox_cnt_twdm_optic_pl_table_add(&fw_counters,
						   pon_mbox_dev->cnt_state);

	if (counters_copy)
		memcpy(counters_copy, counters, sizeof(*counters_copy));

	pon_mbox_cnt_unlock(pon_mbox_dev->cnt_state);
	pon_mbox_cnt_release(counters);

	return 0;
}

static void twdm_optic_pl_last_update_update(u8 port_idx,
					     struct counters_state *state)
{
	(void)port_idx;
	mutex_lock(&state->lock);
	state->last_update.twdm_optic_pl_counters = jiffies;
	mutex_unlock(&state->lock);
}

static enum cnt_init_status
twdm_tc_counters_init(struct ponfw_cnt_io_grp *cnt_io_grp, u8 twdm_dswlch_id,
		      u8 port_idx, struct pon_mbox *pon_mbox_dev)
{
	(void)port_idx;
	(void)cnt_io_grp;
	/* Counters are only supported in NG-PON2 mode */
	if (!is_ngpon2_mode(pon_mbox_dev))
		return CNT_UPDATE_SKIP;
	/* Update NG-PON2 counter for the current WL Ch ID and accumulated */
	if (need_ngpon2_update(pon_mbox_dev, twdm_dswlch_id))
		return CNT_UPDATE_READY;

	return CNT_UPDATE_SKIP;
}

static int twdm_tc_counters_update(union pon_cnt *cnt, u8 port_idx,
				   struct ponfw_cnt_io_grp *cnt_io_grp,
				   struct pon_mbox *pon_mbox_dev)
{
	struct pon_mbox_twdm_tc_counters *counters_copy = &cnt->twdm_tc;
	struct ponfw_twdm_tc_counters *fw_output =
		&cnt_io_grp->prime.out.twdm_tc;
	struct pon_mbox_twdm_tc_counters *counters;
	struct pon_mbox_twdm_tc_counters fw_counters = { 0 };

	(void)port_idx;
	(void)pon_mbox_cnt_twdm_tc_fw2pon(fw_output, &fw_counters);

	pon_mbox_cnt_lock(pon_mbox_dev->cnt_state);

	counters = pon_mbox_cnt_twdm_tc_table_get(PON_MBOX_D_DSWLCH_ID_CURR,
						  pon_mbox_dev->cnt_state);

	(void)pon_mbox_cnt_twdm_tc_table_add(&fw_counters,
					     pon_mbox_dev->cnt_state);

	if (counters_copy)
		memcpy(counters_copy, counters, sizeof(*counters_copy));

	pon_mbox_cnt_unlock(pon_mbox_dev->cnt_state);
	pon_mbox_cnt_release(counters);

	return 0;
}

static void twdm_tc_last_update_update(u8 port_idx,
				       struct counters_state *state)
{
	(void)port_idx;
	mutex_lock(&state->lock);
	state->last_update.twdm_tc_counters = jiffies;
	mutex_unlock(&state->lock);
}

static enum cnt_init_status
xgtc_ploam_ds_counters_init(struct ponfw_cnt_io_grp *cnt_io_grp,
			    u8 twdm_dswlch_id, u8 port_idx,
			    struct pon_mbox *pon_mbox_dev)
{
	(void)port_idx;
	(void)cnt_io_grp;
	/* XGTC counters are supported by
	 * XGS-PON, XG-PON, NG-PON2
	 */
	if (!(pon_mbox_dev->mode == PON_MODE_987_XGPON ||
	      pon_mbox_dev->mode == PON_MODE_9807_XGSPON ||
	      pon_mbox_dev->mode == PON_MODE_989_NGPON2_2G5 ||
	      pon_mbox_dev->mode == PON_MODE_989_NGPON2_10G))
		return CNT_UPDATE_SKIP;

	if (is_ngpon2_mode(pon_mbox_dev))
		if (!need_ngpon2_update(pon_mbox_dev, twdm_dswlch_id))
			return CNT_UPDATE_SKIP;

	return CNT_UPDATE_READY;
}

static int xgtc_ploam_ds_counters_update(union pon_cnt *cnt, u8 port_idx,
					 struct ponfw_cnt_io_grp *cnt_io_grp,
					 struct pon_mbox *pon_mbox_dev)
{
	struct pon_mbox_xgtc_ploam_ds_counters *counters_copy =
		&cnt->xgtc_ploam_ds;
	struct ponfw_xgtc_ploam_ds_counters *fw_output =
		&cnt_io_grp->prime.out.xgtc_ploam_ds;
	struct pon_mbox_xgtc_ploam_ds_counters *counters;
	struct pon_mbox_xgtc_ploam_ds_counters fw_counters = { 0 };

	(void)port_idx;
	(void)pon_mbox_cnt_xgtc_ploam_ds_fw2pon(fw_output, &fw_counters);

	pon_mbox_cnt_lock(pon_mbox_dev->cnt_state);

	counters = pon_mbox_cnt_xgtc_ploam_ds_table_get(
		PON_MBOX_D_DSWLCH_ID_CURR, pon_mbox_dev->cnt_state);

	(void)pon_mbox_cnt_xgtc_ploam_ds_table_add(&fw_counters,
						   pon_mbox_dev->cnt_state);

	if (counters_copy)
		memcpy(counters_copy, counters, sizeof(*counters_copy));

	pon_mbox_cnt_unlock(pon_mbox_dev->cnt_state);
	pon_mbox_cnt_release(counters);

	return 0;
}

static void xgtc_ploam_ds_last_update_update(u8 port_idx,
					     struct counters_state *state)
{
	(void)port_idx;
	mutex_lock(&state->lock);
	state->last_update.xgtc_ploam_ds_counters = jiffies;
	mutex_unlock(&state->lock);
}

static enum cnt_init_status
gtc_ploam_ds_counters_init(struct ponfw_cnt_io_grp *cnt_io_grp,
			   u8 twdm_dswlch_id, u8 port_idx,
			   struct pon_mbox *pon_mbox_dev)
{
	(void)port_idx;
	(void)cnt_io_grp;
	(void)twdm_dswlch_id;
	/* GTC counters only supported by GPON */
	if (!(pon_mbox_dev->mode == PON_MODE_984_GPON))
		return CNT_UPDATE_SKIP;

	return CNT_UPDATE_READY;
}

static int gtc_ploam_ds_counters_update(union pon_cnt *cnt, u8 port_idx,
					struct ponfw_cnt_io_grp *cnt_io_grp,
					struct pon_mbox *pon_mbox_dev)
{
	struct pon_mbox_gtc_ploam_ds_counters *counters_copy =
		&cnt->gtc_ploam_ds;
	struct ponfw_gtc_ploam_ds_counters *fw_output =
		&cnt_io_grp->prime.out.gtc_ploam_ds;
	struct pon_mbox_gtc_ploam_ds_counters *counters;
	struct pon_mbox_gtc_ploam_ds_counters fw_counters = { 0 };

	(void)port_idx;
	(void)pon_mbox_cnt_gtc_ploam_ds_fw2pon(fw_output, &fw_counters);

	pon_mbox_cnt_lock(pon_mbox_dev->cnt_state);

	counters = pon_mbox_cnt_gtc_ploam_ds_table_get(pon_mbox_dev->cnt_state);

	(void)pon_mbox_cnt_gtc_ploam_ds_table_add(&fw_counters,
						  pon_mbox_dev->cnt_state);

	if (counters_copy)
		memcpy(counters_copy, counters, sizeof(*counters_copy));

	pon_mbox_cnt_unlock(pon_mbox_dev->cnt_state);
	pon_mbox_cnt_release(counters);

	return 0;
}

static void gtc_ploam_ds_last_update_update(u8 port_idx,
					    struct counters_state *state)
{
	(void)port_idx;
	mutex_lock(&state->lock);
	state->last_update.gtc_ploam_ds_counters = jiffies;
	mutex_unlock(&state->lock);
}

static enum cnt_init_status
xgtc_ploam_us_counters_init(struct ponfw_cnt_io_grp *cnt_io_grp,
			    u8 twdm_dswlch_id, u8 port_idx,
			    struct pon_mbox *pon_mbox_dev)
{
	(void)port_idx;
	(void)cnt_io_grp;
	/* XGTC counters are supported by
	 * XGS-PON, XG-PON, NG-PON2
	 */
	if (!(pon_mbox_dev->mode == PON_MODE_987_XGPON ||
	      pon_mbox_dev->mode == PON_MODE_9807_XGSPON ||
	      pon_mbox_dev->mode == PON_MODE_989_NGPON2_2G5 ||
	      pon_mbox_dev->mode == PON_MODE_989_NGPON2_10G))
		return CNT_UPDATE_SKIP;

	if (is_ngpon2_mode(pon_mbox_dev))
		if (!need_ngpon2_update(pon_mbox_dev, twdm_dswlch_id))
			return CNT_UPDATE_SKIP;

	return CNT_UPDATE_READY;
}

static int xgtc_ploam_us_counters_update(union pon_cnt *cnt, u8 port_idx,
					 struct ponfw_cnt_io_grp *cnt_io_grp,
					 struct pon_mbox *pon_mbox_dev)
{
	struct pon_mbox_xgtc_ploam_us_counters *counters_copy =
		&cnt->xgtc_ploam_us;
	struct ponfw_xgtc_ploam_us_counters *fw_output =
		&cnt_io_grp->prime.out.xgtc_ploam_us;
	struct pon_mbox_xgtc_ploam_us_counters *counters;
	struct pon_mbox_xgtc_ploam_us_counters fw_counters = { 0 };

	(void)port_idx;
	(void)pon_mbox_cnt_xgtc_ploam_us_fw2pon(fw_output, &fw_counters);

	pon_mbox_cnt_lock(pon_mbox_dev->cnt_state);

	counters = pon_mbox_cnt_xgtc_ploam_us_table_get(
		PON_MBOX_D_DSWLCH_ID_CURR, pon_mbox_dev->cnt_state);

	(void)pon_mbox_cnt_xgtc_ploam_us_table_add(&fw_counters,
						   pon_mbox_dev->cnt_state);

	if (counters_copy)
		memcpy(counters_copy, counters, sizeof(*counters_copy));

	pon_mbox_cnt_unlock(pon_mbox_dev->cnt_state);
	pon_mbox_cnt_release(counters);

	return 0;
}

static void xgtc_ploam_us_last_update_update(u8 port_idx,
					     struct counters_state *state)
{
	(void)port_idx;
	mutex_lock(&state->lock);
	state->last_update.xgtc_ploam_us_counters = jiffies;
	mutex_unlock(&state->lock);
}

static enum cnt_init_status
gtc_ploam_us_counters_init(struct ponfw_cnt_io_grp *cnt_io_grp,
			   u8 twdm_dswlch_id, u8 port_idx,
			   struct pon_mbox *pon_mbox_dev)
{
	(void)port_idx;
	(void)cnt_io_grp;
	(void)twdm_dswlch_id;
	/* GTC counters only supported by GPON */
	if (!(pon_mbox_dev->mode == PON_MODE_984_GPON))
		return CNT_UPDATE_SKIP;

	return CNT_UPDATE_READY;
}

static int gtc_ploam_us_counters_update(union pon_cnt *cnt, u8 port_idx,
					struct ponfw_cnt_io_grp *cnt_io_grp,
					struct pon_mbox *pon_mbox_dev)
{
	struct pon_mbox_gtc_ploam_us_counters *counters_copy =
		&cnt->gtc_ploam_us;
	struct ponfw_gtc_ploam_us_counters *fw_output =
		&cnt_io_grp->prime.out.gtc_ploam_us;
	struct pon_mbox_gtc_ploam_us_counters *counters;
	struct pon_mbox_gtc_ploam_us_counters fw_counters = { 0 };

	(void)port_idx;
	(void)pon_mbox_cnt_gtc_ploam_us_fw2pon(fw_output, &fw_counters);

	pon_mbox_cnt_lock(pon_mbox_dev->cnt_state);

	counters = pon_mbox_cnt_gtc_ploam_us_table_get(pon_mbox_dev->cnt_state);

	(void)pon_mbox_cnt_gtc_ploam_us_table_add(&fw_counters,
						  pon_mbox_dev->cnt_state);

	if (counters_copy)
		memcpy(counters_copy, counters, sizeof(*counters_copy));

	pon_mbox_cnt_unlock(pon_mbox_dev->cnt_state);
	pon_mbox_cnt_release(counters);

	return 0;
}

static void gtc_ploam_us_last_update_update(u8 port_idx,
					    struct counters_state *state)
{
	(void)port_idx;
	mutex_lock(&state->lock);
	state->last_update.gtc_ploam_us_counters = jiffies;
	mutex_unlock(&state->lock);
}

static const struct cnt_ctrl cnt_ctrl_array[MAX_CTRL_CNT] = {
	[GEM_PORT_CNT] = {
		.ctrl_aux_idx = NONE_CNT,
		.cmd = PONFW_GEM_PORT_COUNTERS_CMD_ID,
		.lenr = PONFW_GEM_PORT_COUNTERS_LENR,
		.len = PONFW_GEM_PORT_COUNTERS_LEN,
		.init_fp = gem_port_counters_init,
		.update_fp = gem_port_counters_update,
		.last_update_fp = pon_mbox_cnt_gem_port_last_update_update,
	},
	[ALLOC_CNT] = {
		.ctrl_aux_idx = ALLOC_BW_CNT,
		.cmd = PONFW_ALLOC_ID_COUNTERS_CMD_ID,
		.lenr = PONFW_ALLOC_ID_COUNTERS_LENR,
		.len = PONFW_ALLOC_ID_COUNTERS_LEN,
		.init_fp = alloc_counters_init,
		.update_fp = alloc_counters_update,
		.last_update_fp = pon_mbox_cnt_alloc_last_update_update,
	},
	[GTC_CNT] = {
		.ctrl_aux_idx = GTC_ENHANCED_CNT,
		.cmd = PONFW_GTC_COUNTERS_CMD_ID,
		.lenr = 0,
		.len = PONFW_GTC_COUNTERS_LEN,
		.init_fp = gtc_counters_init,
		.update_fp = gtc_counters_update,
		.last_update_fp = pon_mbox_cnt_gtc_last_update_update,
	},
	[XGTC_CNT] = {
		.ctrl_aux_idx = NONE_CNT,
		.cmd = PONFW_XGTC_COUNTERS_CMD_ID,
		.lenr = 0,
		.len = PONFW_XGTC_COUNTERS_LEN,
		.init_fp = xgtc_counters_init,
		.update_fp = xgtc_counters_update,
		.last_update_fp = pon_mbox_cnt_xgtc_last_update_update,
	},
	[ALLOC_LOST_CNT] = {
		.ctrl_aux_idx = NONE_CNT,
		.cmd = PONFW_ALLOC_LOST_COUNTERS_CMD_ID,
		.lenr = 0,
		.len = PONFW_ALLOC_LOST_COUNTERS_LEN,
		.init_fp = alloc_lost_counters_init,
		.update_fp = alloc_lost_counters_update,
		.last_update_fp = pon_mbox_cnt_alloc_lost_last_update_update,
	},
	[RX_ETH_CNT] = {
		.ctrl_aux_idx = NONE_CNT,
		.cmd = PONFW_RX_ETH_COUNTERS_CMD_ID,
		.lenr = PONFW_RX_ETH_COUNTERS_LENR,
		.len = PONFW_RX_ETH_COUNTERS_LEN,
		.init_fp = rx_eth_counters_init,
		.update_fp = rx_eth_counters_update,
		.last_update_fp = pon_mbox_cnt_eth_rx_last_update_update,
	},
	[TX_ETH_CNT] = {
		.ctrl_aux_idx = NONE_CNT,
		.cmd = PONFW_TX_ETH_COUNTERS_CMD_ID,
		.lenr = PONFW_TX_ETH_COUNTERS_LENR,
		.len = PONFW_TX_ETH_COUNTERS_LEN,
		.init_fp = tx_eth_counters_init,
		.update_fp = tx_eth_counters_update,
		.last_update_fp = pon_mbox_cnt_eth_tx_last_update_update,
	},
	[TWDM_LODS_CNT] = {
		.ctrl_aux_idx = NONE_CNT,
		.cmd = PONFW_TWDM_LODS_COUNTERS_CMD_ID,
		.lenr = 0,
		.len = PONFW_TWDM_LODS_COUNTERS_LEN,
		.init_fp = pon_lods_counters_init,
		.update_fp = twdm_lods_counters_update,
		.last_update_fp = pon_mbox_cnt_twdm_lods_last_update_update,
	},
	[TWDM_OPTIC_PL_CNT] = {
		.ctrl_aux_idx = NONE_CNT,
		.cmd = PONFW_TWDM_ONU_OPTIC_PL_COUNTERS_CMD_ID,
		.lenr = 0,
		.len = PONFW_TWDM_ONU_OPTIC_PL_COUNTERS_LEN,
		.init_fp = twdm_optic_pl_counters_init,
		.update_fp = twdm_optic_pl_counters_update,
		.last_update_fp = twdm_optic_pl_last_update_update,
	},
	[XGTC_PLOAM_DS_CNT] = {
		.ctrl_aux_idx = NONE_CNT,
		.cmd = PONFW_XGTC_PLOAM_DS_COUNTERS_CMD_ID,
		.lenr = 0,
		.len = PONFW_XGTC_PLOAM_DS_COUNTERS_LEN,
		.init_fp = xgtc_ploam_ds_counters_init,
		.update_fp = xgtc_ploam_ds_counters_update,
		.last_update_fp = xgtc_ploam_ds_last_update_update,
	},
	[GTC_PLOAM_DS_CNT] = {
		.ctrl_aux_idx = NONE_CNT,
		.cmd = PONFW_GTC_PLOAM_DS_COUNTERS_CMD_ID,
		.lenr = 0,
		.len = PONFW_GTC_PLOAM_DS_COUNTERS_LEN,
		.init_fp = gtc_ploam_ds_counters_init,
		.update_fp = gtc_ploam_ds_counters_update,
		.last_update_fp = gtc_ploam_ds_last_update_update,
	},
	[XGTC_PLOAM_US_CNT] = {
		.ctrl_aux_idx = NONE_CNT,
		.cmd = PONFW_XGTC_PLOAM_US_COUNTERS_CMD_ID,
		.lenr = 0,
		.len = PONFW_XGTC_PLOAM_US_COUNTERS_LEN,
		.init_fp = xgtc_ploam_us_counters_init,
		.update_fp = xgtc_ploam_us_counters_update,
		.last_update_fp = xgtc_ploam_us_last_update_update,
	},
	[GTC_PLOAM_US_CNT] = {
		.ctrl_aux_idx = NONE_CNT,
		.cmd = PONFW_GTC_PLOAM_US_COUNTERS_CMD_ID,
		.lenr = 0,
		.len = PONFW_GTC_PLOAM_US_COUNTERS_LEN,
		.init_fp = gtc_ploam_us_counters_init,
		.update_fp = gtc_ploam_us_counters_update,
		.last_update_fp = gtc_ploam_us_last_update_update,
	},
	[TWDM_TC_CNT] = {
		.ctrl_aux_idx = NONE_CNT,
		.cmd = PONFW_TWDM_TC_COUNTERS_CMD_ID,
		.lenr = 0,
		.len = PONFW_TWDM_TC_COUNTERS_LEN,
		.init_fp = twdm_tc_counters_init,
		.update_fp = twdm_tc_counters_update,
		.last_update_fp = twdm_tc_last_update_update,
	},
};

/**
 * @brief Counters update function for firmware message.
 *
 * @param[in] cnt_index    Number determining the counter
 * @param[in] dswlch_id    TWDM DS Wavelength Channel ID
 * @param[in] port_idx     Port index for counter (e.g. link index, gem index)
 * @param[out] cnt	   Pointer to counter specific data to update
 * @param[in] pon_mbox_dev Pointer to PON mailbox driver data structure
 *
 * @return 0 for success
 *	   PONFW_NACK, PONFW_NACK_DUP, -ENOENT or -EINVAL for failure
 */
static int generic_counters_update(enum counter_type cnt_index, u8 dswlch_id,
				   u8 port_idx, union pon_cnt *cnt,
				   struct pon_mbox *pon_mbox_dev)
{
	/* The array of auxiliary counter-specific parameters
	 * for auxiliary pon_mbox_send() call
	 */
	static const struct
		cnt_ctrl_aux cnt_ctrl_aux_array[MAX_CTRL_AUX_CNT] = {
		{
			.cmd = PONFW_ALLOC_BW_CMD_ID,
			.lenr = PONFW_ALLOC_BW_LENR,
			.len = PONFW_ALLOC_BW_LEN,
		},
		{
			.cmd = PONFW_GTC_ENHANCED_COUNTERS_CMD_ID,
			.lenr = 0,
			.len = PONFW_GTC_ENHANCED_COUNTERS_LEN,
		},
	};
	/* Data of two identical type fields (prime, aux), each consists of
	 * in,out, to be passed to primary and auxiliary pon_mbox_send()
	 */
	struct ponfw_cnt_io_grp cnt_io_grp = { 0 };
	/* Pointer to the primary table element where counter-specific
	 * parameters are stored for primary pon_mbox_send() call
	 */
	const struct cnt_ctrl *cnt_ctrl = cnt_ctrl_array + cnt_index;
	/* Pointer to the auxiliary table element where counter-specific data
	 * parameters are stored for auxiliary call of pon_mbox_send().
	 * The need of it is determined by the field of primary table element
	 */
	const struct cnt_ctrl_aux *cnt_ctrl_aux =
		(cnt_ctrl->ctrl_aux_idx > NONE_CNT) ?
			(cnt_ctrl_aux_array + cnt_ctrl->ctrl_aux_idx) :
			NULL;
	int err;
	/* A request for the current DS Wavelength and also for the total
	 * counters needs a counter update from the PON FW.
	 */
	u8 twdm_dswlch_id = ((dswlch_id == PON_MBOX_D_DSWLCH_ID_CURR) ||
			     (dswlch_id == PON_MBOX_D_DSWLCH_ID_ACC)) ?
				    pon_mbox_dev->cnt_state->twdm_ds_idx :
				    dswlch_id;
	if (twdm_dswlch_id >= TWDM_DS_MAX)
		return -EINVAL;

	/* Prepare cnt_io_grp.prime.in and check if the mode supports counter.
	 * Counter-specific init function is defined in table element 'cnt_ctrl'
	 */
	if (cnt_ctrl->init_fp(&cnt_io_grp, twdm_dswlch_id, port_idx,
			      pon_mbox_dev) == CNT_UPDATE_SKIP)
		return 0;

	/* Get counter-specific primary cnt_io_grp.prime.out * from the firmware
	 * on the base of cnt_io_grp.prime.in. Counter-specific pon_mbox_send()
	 * parameters are defined in the table element pointed by 'cnt_ctrl'
	 */
	err = pon_mbox_send(cnt_ctrl->cmd, PONFW_READ, &cnt_io_grp.prime.in,
			    cnt_ctrl->lenr, &cnt_io_grp.prime.out,
			    cnt_ctrl->len);
	if (err < 0) {
		cnt_ctrl->last_update_fp(port_idx, pon_mbox_dev->cnt_state);
		return err;
	}

	/* If auxiliary pon_mbox_send() is required then get cnt_io_grp.aux.out
	 * from the firmware on the base of cnt_io_grp.aux.in. Counter-specific
	 * auxiliary pon_mbox_send() parameters are in the auxiliary table
	 * element pointed by 'cnt_ctrl_aux'
	 */
	if (cnt_ctrl_aux) {
		err = pon_mbox_send(cnt_ctrl_aux->cmd, PONFW_READ,
				    &cnt_io_grp.aux.in, cnt_ctrl_aux->lenr,
				    &cnt_io_grp.aux.out, cnt_ctrl_aux->len);
		if (err < 0) {
			cnt_ctrl->last_update_fp(port_idx,
						 pon_mbox_dev->cnt_state);
			return err;
		}
	}

	/* Update the caller counter on the base of cnt_io_grp.prime.out
	 * (and optionally also cnt_io_grp.aux.out) obtained from the firmware.
	 * Counter-specific update function is in the table element 'cnt_ctrl'
	 */
	err = cnt_ctrl->update_fp(cnt, port_idx, &cnt_io_grp, pon_mbox_dev);

	return err;
}

/**
 * @brief Generic function type to get wavelength specific counter table
 *
 * @param[in] dswlch_id  TWDM DS Wavelength Channel ID.
 * @param[in] state      MBox driver counter context.
 *
 * @return pointer to the requested counter table
 */
typedef union pon_cnt *(twdm_counter_table_get)(u8 dswlch_id,
						struct counters_state *state);

/* Create the corresponding counter table get function name */
#define PON_MBOX_COUNTER_TABLE_GET_FCT(CNT_TYPE) \
	((twdm_counter_table_get *)pon_mbox_cnt_##CNT_TYPE##_table_get)

/**
 * @brief Returns the counter table for not active TWDM DS Wavelength ID's
 *        Only for current DS wavelength and the total counters a counter
 *        update from the PON FW is required to get the actual status.
 *        If the counters for a not active DS wavelength is requested the
 *        corresponding table is returned.
 *
 * @param[in] dswlch_id     TWDM DS Wavelength Channel ID
 * @param[out] cnt	    Pointer to counter specific data to update
 * @param[in] pon_mbox_dev  Pointer to PON mailbox driver data structure
 * @param[in] table_size    Size of the requested counter table
 * @param[in] table_get_fct Table get function to get the requested table.
 *
 */
static void generic_twdm_counter_table_get(
	u8 dswlch_id, union pon_cnt *cnt, struct pon_mbox *pon_mbox_dev,
	unsigned long table_size, twdm_counter_table_get *table_get_fct)
{
	union pon_cnt *counters;

	if (!is_ngpon2_mode(pon_mbox_dev) ||
	    dswlch_id == PON_MBOX_D_DSWLCH_ID_CURR)
		return;

	counters = table_get_fct(dswlch_id, pon_mbox_dev->cnt_state);
	if (cnt && counters)
		memcpy(cnt, counters, table_size);
}

u64 pon_mbox_counters_enabled_get(struct pon_mbox *pon_mbox_dev)
{
	unsigned int i = 0;
	const struct cnt_ctrl *cnt_ctrl;
	struct ponfw_cnt_io_grp cnt_io_grp = { 0 };
	u64 mask = 0;
	u8 twdm_dswlch_id = pon_mbox_dev->cnt_state->twdm_ds_idx;

	for (i = 0; i < MAX_CTRL_CNT; ++i) {
		cnt_ctrl = &cnt_ctrl_array[i];

		if (cnt_ctrl->init_fp(&cnt_io_grp, twdm_dswlch_id, 0,
				      pon_mbox_dev) == CNT_UPDATE_SKIP)
			continue;

		mask |= BIT_ULL(i);
	}

	return mask;
}

int pon_mbox_gem_port_counters_update(u8 dswlch_id, u8 gem_port_idx,
				      struct pon_mbox_gem_port_counters *cnt,
				      struct pon_mbox *pon_mbox_dev)
{
	int err;
	struct pon_mbox_gem_port_counters *counters;

	err = generic_counters_update(GEM_PORT_CNT, dswlch_id, gem_port_idx,
				      (union pon_cnt *)cnt, pon_mbox_dev);
	if (err < 0)
		return err;

	if (!is_ngpon2_mode(pon_mbox_dev) ||
	    dswlch_id == PON_MBOX_D_DSWLCH_ID_CURR)
		return err;

	counters = pon_mbox_cnt_gem_port_table_get(dswlch_id, gem_port_idx,
						   pon_mbox_dev->cnt_state);
	if (!counters) {
		dev_err(pon_mbox_dev->dev,
			"cannot get counters table from storage\n");
		return -EINVAL;
	}
	if (cnt)
		memcpy(cnt, counters, sizeof(*cnt));

	return err;
}

void pon_mbox_gem_all_counters_update(u8 dswlch_id,
				      struct pon_mbox_gem_port_counters *cnt,
				      struct pon_mbox *pon_mbox_dev)
{
	struct pon_mbox_gem_port_counters *counters;

	counters = pon_mbox_cnt_gem_all_table_get(dswlch_id,
						  pon_mbox_dev->cnt_state);

	if (cnt)
		memcpy(cnt, counters, sizeof(*cnt));
}

int pon_mbox_alloc_counters_update(u8 alloc_idx, struct pon_alloc_counters *cnt,
				   struct pon_mbox *pon_mbox_dev)
{
	return generic_counters_update(ALLOC_CNT, PON_MBOX_D_DSWLCH_ID_CURR,
				       alloc_idx, (union pon_cnt *)cnt,
				       pon_mbox_dev);
}

int pon_mbox_gtc_counters_update(u8 dswlch_id,
				 struct pon_mbox_gtc_counters *cnt,
				 struct pon_mbox *pon_mbox_dev)
{
	int err;

	err = generic_counters_update(GTC_CNT, dswlch_id, 0,
				      (union pon_cnt *)cnt, pon_mbox_dev);
	if (err < 0)
		return err;

	generic_twdm_counter_table_get(dswlch_id, (union pon_cnt *)cnt,
				       pon_mbox_dev, sizeof(*cnt),
				       PON_MBOX_COUNTER_TABLE_GET_FCT(gtc));

	return err;
}

int pon_mbox_xgtc_counters_update(u8 dswlch_id,
				  struct pon_mbox_xgtc_counters *cnt,
				  struct pon_mbox *pon_mbox_dev)
{
	int err;

	err = generic_counters_update(XGTC_CNT, dswlch_id, 0,
				      (union pon_cnt *)cnt, pon_mbox_dev);
	if (err < 0)
		return err;

	generic_twdm_counter_table_get(dswlch_id, (union pon_cnt *)cnt,
				       pon_mbox_dev, sizeof(*cnt),
				       PON_MBOX_COUNTER_TABLE_GET_FCT(xgtc));

	return err;
}

int pon_mbox_alloc_lost_counters_update(struct pon_alloc_discard_counters *cnt,
					struct pon_mbox *pon_mbox_dev)
{
	return generic_counters_update(ALLOC_LOST_CNT,
				       PON_MBOX_D_DSWLCH_ID_CURR, 0,
				       (union pon_cnt *)cnt, pon_mbox_dev);
}

int pon_mbox_rx_eth_counters_update(u8 gem_port_idx,
				    struct pon_eth_counters *cnt,
				    struct pon_mbox *pon_mbox_dev)
{
	return generic_counters_update(RX_ETH_CNT, PON_MBOX_D_DSWLCH_ID_CURR,
				       gem_port_idx, (union pon_cnt *)cnt,
				       pon_mbox_dev);
}

int pon_mbox_tx_eth_counters_update(u8 gem_port_idx,
				    struct pon_eth_counters *cnt,
				    struct pon_mbox *pon_mbox_dev)
{
	return generic_counters_update(TX_ETH_CNT, PON_MBOX_D_DSWLCH_ID_CURR,
				       gem_port_idx, (union pon_cnt *)cnt,
				       pon_mbox_dev);
}

int pon_mbox_twdm_lods_counters_update(u8 dswlch_id,
				       struct pon_mbox_twdm_lods_counters *cnt,
				       struct pon_mbox *pon_mbox_dev)
{
	int err;

	err = generic_counters_update(TWDM_LODS_CNT, dswlch_id, 0,
				      (union pon_cnt *)cnt, pon_mbox_dev);
	if (err < 0)
		return err;

	generic_twdm_counter_table_get(
		dswlch_id, (union pon_cnt *)cnt, pon_mbox_dev, sizeof(*cnt),
		PON_MBOX_COUNTER_TABLE_GET_FCT(twdm_lods));

	return err;
}

int pon_mbox_twdm_optic_pl_counters_update(
	u8 dswlch_id, struct pon_mbox_twdm_optic_pl_counters *cnt,
	struct pon_mbox *pon_mbox_dev)
{
	int err;

	err = generic_counters_update(TWDM_OPTIC_PL_CNT, dswlch_id, 0,
				      (union pon_cnt *)cnt, pon_mbox_dev);
	if (err < 0)
		return err;

	generic_twdm_counter_table_get(
		dswlch_id, (union pon_cnt *)cnt, pon_mbox_dev, sizeof(*cnt),
		PON_MBOX_COUNTER_TABLE_GET_FCT(twdm_optic_pl));

	return err;
}

int pon_mbox_twdm_tc_counters_update(u8 dswlch_id,
				     struct pon_mbox_twdm_tc_counters *cnt,
				     struct pon_mbox *pon_mbox_dev)
{
	int err;

	err = generic_counters_update(TWDM_TC_CNT, dswlch_id, 0,
				      (union pon_cnt *)cnt, pon_mbox_dev);
	if (err < 0)
		return err;

	generic_twdm_counter_table_get(dswlch_id, (union pon_cnt *)cnt,
				       pon_mbox_dev, sizeof(*cnt),
				       PON_MBOX_COUNTER_TABLE_GET_FCT(twdm_tc));

	return err;
}

int pon_mbox_gtc_ploam_ds_counters_update(
	u8 dswlch_id, struct pon_mbox_gtc_ploam_ds_counters *cnt,
	struct pon_mbox *pon_mbox_dev)
{
	return generic_counters_update(GTC_PLOAM_DS_CNT, dswlch_id, 0,
				       (union pon_cnt *)cnt, pon_mbox_dev);
}

int pon_mbox_gtc_ploam_us_counters_update(
	u8 dswlch_id, struct pon_mbox_gtc_ploam_us_counters *cnt,
	struct pon_mbox *pon_mbox_dev)
{
	return generic_counters_update(GTC_PLOAM_US_CNT, dswlch_id, 0,
				       (union pon_cnt *)cnt, pon_mbox_dev);
}

int pon_mbox_xgtc_ploam_ds_counters_update(
	u8 dswlch_id, struct pon_mbox_xgtc_ploam_ds_counters *cnt,
	struct pon_mbox *pon_mbox_dev)
{
	int err;

	err = generic_counters_update(XGTC_PLOAM_DS_CNT, dswlch_id, 0,
				      (union pon_cnt *)cnt, pon_mbox_dev);
	if (err < 0)
		return err;

	generic_twdm_counter_table_get(
		dswlch_id, (union pon_cnt *)cnt, pon_mbox_dev, sizeof(*cnt),
		PON_MBOX_COUNTER_TABLE_GET_FCT(xgtc_ploam_ds));

	return err;
}

int pon_mbox_xgtc_ploam_us_counters_update(
	u8 dswlch_id, struct pon_mbox_xgtc_ploam_us_counters *cnt,
	struct pon_mbox *pon_mbox_dev)
{
	int err;

	err = generic_counters_update(XGTC_PLOAM_US_CNT, dswlch_id, 0,
				      (union pon_cnt *)cnt, pon_mbox_dev);
	if (err < 0)
		return err;

	generic_twdm_counter_table_get(
		dswlch_id, (union pon_cnt *)cnt, pon_mbox_dev, sizeof(*cnt),
		PON_MBOX_COUNTER_TABLE_GET_FCT(xgtc_ploam_us));

	return err;
}
