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
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/version.h>

#include <pon/pon_ip_msg.h>
#include <pon/pon_mbox.h>
#include <pon/pon_mbox_ikm.h>
#include <drv_pon_mbox.h>
#include <drv_pon_mbox_counters_conv.h>
#include <drv_pon_mbox_counters_update.h>
#include <drv_pon_mbox_counters_autoupdate.h>

/** Information about counters */
struct counter_info {
	/** Contains gem port indexes that are in use */
	u8 used_gem_ports[GEM_PORTS_MAX];
	/** The size of used_gem_ports */
	unsigned int used_gem_ports_count;
	/** Contains alloc ids that are in use */
	u8 used_alloc_ids[ALLOC_IDS_MAX];
	/** The size of used_alloc_ids */
	unsigned int used_alloc_ids_count;
	/** Last update times for each counter */
	struct counters_last_update last_update;
};

/** Counter TWDM wavelength switching */
struct counter_twdm_wl_info {
	/** Trigger wavelength switching */
	u8 trigger_update;
	/** The new TWDM DS wavelength channel ID */
	u8 new_twdm_dswlch_id;
	/** The new TWDM US wavelength channel ID */
	u8 new_twdm_uswlch_id;
};

/* Fills counter info with data */
static
void counter_info_get(struct counter_info *info, struct counters_state *state)
{
	info->used_gem_ports_count =
		pon_mbox_used_gem_ports_get(info->used_gem_ports,
					    GEM_PORTS_MAX,
					    state);

	info->used_alloc_ids_count =
		pon_mbox_used_alloc_ids_get(info->used_alloc_ids,
					    ALLOC_IDS_MAX,
					    state);

	pon_mbox_last_update_get(&info->last_update, state);
}

/** Context for automatic update of counters */
struct cnt_autoupdate {
	/**
	 * The work performing actual counter update.
	 * We use a workqueue because the update of counters calls
	 * wait_for_completion(), which will block the timer softirq
	 */
	struct work_struct work;
	/** A timer for refreshing counters */
	struct timer_list timer;
	/** Update time in seconds */
	unsigned long update_time_in_s;
	/** Should work be running. */
	atomic_t running;
	/** PON mailbox device */
	struct pon_mbox *dev;
	/** Counter info */
	struct counter_info info;
	/** TWDM wavelength switching */
	struct counter_twdm_wl_info twdm_wl_info;
};

static void schedule_counters_update(struct cnt_autoupdate *c,
				     unsigned long since)
{
	unsigned long time =
		since + msecs_to_jiffies(c->update_time_in_s * 1000);

	/* If time is from the past this will execute the callback ASAP */
	mod_timer(&c->timer, time);
}

/*
 * A callback for work in workqueue. Performs the counter update.
 * 1. Update all counters that have not been updated for
 *    'update_time_in_s' time.
 * 2. Find the oldest update time
 * 3. Schedule next update at oldest_update_time + update_time_in_s
 */
static void counters_update(struct work_struct *work)
{
	struct cnt_autoupdate *cnt_autoupdate =
		container_of(work, struct cnt_autoupdate, work);
	u64 mask;
	unsigned long oldest_time;
	struct pon_mbox *dev = cnt_autoupdate->dev;

	if (cnt_autoupdate->update_time_in_s == PON_COUNTERS_NO_UPDATE)
		return;

	if (dev->pon_ip_debug_mode)
		return;

	if (dev->mode == PON_MODE_AON)
		return;

	pon_mbox_cnt_autoupdate_update(cnt_autoupdate);

	if (!atomic_read(&cnt_autoupdate->running))
		return;

	mask = pon_mbox_counters_enabled_get(dev);
	oldest_time = pon_mbox_cnt_oldest_update_time_get(dev->cnt_state, mask);
	schedule_counters_update(cnt_autoupdate, oldest_time);
}

/* A callback for timer */
static void refresh_counters(struct timer_list *tl)
{
	struct cnt_autoupdate *cnt_autoupdate =
		container_of(tl, struct cnt_autoupdate, timer);
	schedule_work(&cnt_autoupdate->work);
}

struct cnt_autoupdate *
pon_mbox_cnt_autoupdate_create(struct pon_mbox *pon_mbox_dev,
			       unsigned int update_time_in_s)
{
	struct cnt_autoupdate *cnt_autoupdate;

	cnt_autoupdate = kmalloc(sizeof(*cnt_autoupdate), GFP_KERNEL);
	if (!cnt_autoupdate)
		return NULL;

	INIT_WORK(&cnt_autoupdate->work, counters_update);
	cnt_autoupdate->update_time_in_s = update_time_in_s;
	cnt_autoupdate->dev = pon_mbox_dev;
	cnt_autoupdate->twdm_wl_info.trigger_update = 0;
	atomic_set(&cnt_autoupdate->running, 0);

	timer_setup(&cnt_autoupdate->timer, refresh_counters, 0);

	if (update_time_in_s != PON_COUNTERS_NO_UPDATE) {
		atomic_set(&cnt_autoupdate->running, 1);
		/* Schedule the first update immediately by using current jiffies */
		schedule_counters_update(cnt_autoupdate, jiffies);
	}
	return cnt_autoupdate;
}

static void ploam_cnt_autoupdate_update(struct cnt_autoupdate *cnt_autoupdate,
					unsigned long threshold)
{
	union {
		struct pon_mbox_xgtc_ploam_ds_counters xgtc_ploam_ds;
		struct pon_mbox_gtc_ploam_ds_counters gtc_ploam_ds;
		struct pon_mbox_xgtc_ploam_us_counters xgtc_ploam_us;
		struct pon_mbox_gtc_ploam_us_counters gtc_ploam_us;
	} tmp;
	struct counter_info *info = &cnt_autoupdate->info;
	unsigned long delta;
	unsigned long current_time = jiffies;
	unsigned int num_xgtc_ploam_ds_updates = 0;
	unsigned int num_gtc_ploam_ds_updates = 0;
	unsigned int num_xgtc_ploam_us_updates = 0;
	unsigned int num_gtc_ploam_us_updates = 0;
	int ret;

	if (!atomic_read(&cnt_autoupdate->running))
		return;
	delta = current_time - info->last_update.xgtc_ploam_ds_counters;
	if (delta >= threshold) {
		ret = pon_mbox_xgtc_ploam_ds_counters_update(
					PON_MBOX_D_DSWLCH_ID_CURR,
					&tmp.xgtc_ploam_ds,
					cnt_autoupdate->dev);
		if (ret)
			dev_err(cnt_autoupdate->dev->dev,
				"xgtc_ploam_ds_counters_update failed: %d\n",
				ret);

		num_xgtc_ploam_ds_updates++;
	}

	if (!atomic_read(&cnt_autoupdate->running))
		return;
	delta = current_time - info->last_update.gtc_ploam_ds_counters;
	if (delta >= threshold) {
		ret = pon_mbox_gtc_ploam_ds_counters_update(
					PON_MBOX_D_DSWLCH_ID_CURR,
					&tmp.gtc_ploam_ds,
					cnt_autoupdate->dev);
		if (ret)
			dev_err(cnt_autoupdate->dev->dev,
				"gtc_ploam_ds_counters_update failed: %d\n",
				ret);

		num_gtc_ploam_ds_updates++;
	}

	if (!atomic_read(&cnt_autoupdate->running))
		return;
	delta = current_time - info->last_update.xgtc_ploam_us_counters;
	if (delta >= threshold) {
		ret = pon_mbox_xgtc_ploam_us_counters_update(
					PON_MBOX_D_DSWLCH_ID_CURR,
					&tmp.xgtc_ploam_us,
					cnt_autoupdate->dev);
		if (ret)
			dev_err(cnt_autoupdate->dev->dev,
				"xgtc_ploam_us_counters_update failed: %d\n",
				ret);

		num_xgtc_ploam_us_updates++;
	}

	if (!atomic_read(&cnt_autoupdate->running))
		return;
	delta = current_time - info->last_update.gtc_ploam_us_counters;
	if (delta >= threshold) {
		ret = pon_mbox_gtc_ploam_us_counters_update(
					PON_MBOX_D_DSWLCH_ID_CURR,
					&tmp.gtc_ploam_us,
					cnt_autoupdate->dev);
		if (ret)
			dev_err(cnt_autoupdate->dev->dev,
				"gtc_ploam_us_counters_update failed: %d\n",
				ret);

		num_gtc_ploam_us_updates++;
	}

	dev_dbg(cnt_autoupdate->dev->dev,
		"Triggered update of %d XGTC_PLOAM_DS %d GTC_PLOAM_DS %d XGTC_PLOAM_US %d GTC_PLOAM_US counters\n",
		num_xgtc_ploam_ds_updates, num_gtc_ploam_ds_updates,
		num_xgtc_ploam_us_updates, num_gtc_ploam_us_updates);
}

void pon_mbox_cnt_autoupdate_update(struct cnt_autoupdate *cnt_autoupdate)
{
	union {
		struct pon_mbox_gtc_counters gtc;
		struct pon_mbox_xgtc_counters xgtc;
		struct pon_mbox_gem_port_counters gem_port;
		struct pon_alloc_counters alloc;
		struct pon_alloc_discard_counters alloc_discard;
		struct pon_eth_counters rx_eth;
		struct pon_eth_counters tx_eth;
		struct pon_mbox_twdm_lods_counters twdm_lods;
		struct pon_mbox_twdm_optic_pl_counters twdm_optic_pl;
		struct pon_mbox_twdm_tc_counters twdm_tc;
	} tmp;
	unsigned int i;
	struct counter_info *info = &cnt_autoupdate->info;
	unsigned long update_interval_jiffies;
	unsigned long delta;
	unsigned long current_time = jiffies;
	unsigned int num_gem_updates = 0;
	unsigned int num_alloc_updates = 0;
	unsigned int num_alloc_lost_updates = 0;
	unsigned int num_gtc_updates = 0;
	unsigned int num_xgtc_updates = 0;
	unsigned int num_twdm_lods_updates = 0;
	unsigned int num_twdm_optic_pl_updates = 0;
	unsigned int num_twdm_tc_updates = 0;
	int ret = 0;

	update_interval_jiffies =
		msecs_to_jiffies(cnt_autoupdate->update_time_in_s * 1000);

	/* Don't try to refresh counters if mailbox is in_reset */
	if (cnt_autoupdate->dev->in_reset)
		return;

	/* TWDM wavelength switch: force counter update (old wl channel) */
	if (cnt_autoupdate->twdm_wl_info.trigger_update)
		update_interval_jiffies = 0;

	counter_info_get(info, cnt_autoupdate->dev->cnt_state);

	for (i = 0; i < info->used_gem_ports_count; ++i) {
		if (!atomic_read(&cnt_autoupdate->running))
			return;

		delta = current_time -
		  info->last_update.gem_port_counters[info->used_gem_ports[i]];
		if (delta < update_interval_jiffies)
			continue;

		ret = pon_mbox_gem_port_counters_update(
			PON_MBOX_D_DSWLCH_ID_CURR, info->used_gem_ports[i],
			&tmp.gem_port, cnt_autoupdate->dev);
		if (ret)
			dev_err(cnt_autoupdate->dev->dev,
				"gem_port_counters_update failed: %d\n", ret);

		ret = pon_mbox_rx_eth_counters_update(info->used_gem_ports[i],
						      &tmp.rx_eth,
						      cnt_autoupdate->dev);
		if (ret)
			dev_err(cnt_autoupdate->dev->dev,
				"rx_eth_counters_update failed: %d\n", ret);

		ret = pon_mbox_tx_eth_counters_update(info->used_gem_ports[i],
						      &tmp.tx_eth,
						      cnt_autoupdate->dev);
		if (ret)
			dev_err(cnt_autoupdate->dev->dev,
				"tx_eth_counters_update failed: %d\n", ret);

		num_gem_updates++;
	}

	for (i = 0; i < info->used_alloc_ids_count; ++i) {
		if (!atomic_read(&cnt_autoupdate->running))
			return;

		delta = current_time -
		      info->last_update.alloc_counters[info->used_alloc_ids[i]];
		if (delta < update_interval_jiffies)
			continue;

		ret = pon_mbox_alloc_counters_update(info->used_alloc_ids[i],
						     &tmp.alloc,
						     cnt_autoupdate->dev);
		if (ret)
			dev_err(cnt_autoupdate->dev->dev,
				"alloc_counters_update failed: %d\n", ret);

		num_alloc_updates++;
	}

	if (!atomic_read(&cnt_autoupdate->running))
		return;
	delta = current_time - info->last_update.alloc_discard_counters;
	if (delta >= update_interval_jiffies) {
		ret = pon_mbox_alloc_lost_counters_update(&tmp.alloc_discard,
							  cnt_autoupdate->dev);
		if (ret)
			dev_err(cnt_autoupdate->dev->dev,
				"alloc_lost_counters_update failed: %d\n", ret);

		num_alloc_lost_updates++;
	}

	if (!atomic_read(&cnt_autoupdate->running))
		return;
	delta = current_time - info->last_update.gtc_counters;
	if (delta >= update_interval_jiffies) {
		ret = pon_mbox_gtc_counters_update(PON_MBOX_D_DSWLCH_ID_CURR,
						   &tmp.gtc,
						   cnt_autoupdate->dev);
		if (ret)
			dev_err(cnt_autoupdate->dev->dev,
				"gtc_counters_update failed: %d\n", ret);

		num_gtc_updates++;
	}

	if (!atomic_read(&cnt_autoupdate->running))
		return;
	delta = current_time - info->last_update.xgtc_counters;
	if (delta >= update_interval_jiffies) {
		ret = pon_mbox_xgtc_counters_update(PON_MBOX_D_DSWLCH_ID_CURR,
						    &tmp.xgtc,
						    cnt_autoupdate->dev);
		if (ret)
			dev_err(cnt_autoupdate->dev->dev,
				"xgtc_counters_update failed: %d\n", ret);

		num_xgtc_updates++;
	}

	if (!atomic_read(&cnt_autoupdate->running))
		return;
	delta = current_time - info->last_update.twdm_lods_counters;
	if (delta >= update_interval_jiffies) {
		ret = pon_mbox_twdm_lods_counters_update(
					PON_MBOX_D_DSWLCH_ID_CURR,
					&tmp.twdm_lods,
					cnt_autoupdate->dev);
		if (ret)
			dev_err(cnt_autoupdate->dev->dev,
				"twdm_lods_counters_update failed: %d\n", ret);

		num_twdm_lods_updates++;
	}

	if (!atomic_read(&cnt_autoupdate->running))
		return;
	delta = current_time - info->last_update.twdm_optic_pl_counters;
	if (delta >= update_interval_jiffies) {
		ret = pon_mbox_twdm_optic_pl_counters_update(
					PON_MBOX_D_DSWLCH_ID_CURR,
					&tmp.twdm_optic_pl,
					cnt_autoupdate->dev);
		if (ret)
			dev_err(cnt_autoupdate->dev->dev,
				"twdm_optic_pl_counters_update failed: %d\n",
				ret);

		num_twdm_optic_pl_updates++;
	}

	if (!atomic_read(&cnt_autoupdate->running))
		return;
	delta = current_time - info->last_update.twdm_tc_counters;
	if (delta >= update_interval_jiffies) {
		ret = pon_mbox_twdm_tc_counters_update(
					PON_MBOX_D_DSWLCH_ID_CURR,
					&tmp.twdm_tc,
					cnt_autoupdate->dev);
		if (ret)
			dev_err(cnt_autoupdate->dev->dev,
				"twdm_tc_counters_update failed: %d\n",
				ret);

		num_twdm_tc_updates++;
	}

	if (cnt_autoupdate->twdm_wl_info.trigger_update) {
		dev_dbg(cnt_autoupdate->dev->dev,
			"TWDM wlch_id switch: DS %d -> %d, US %d -> %d\n",
			cnt_autoupdate->dev->cnt_state->twdm_ds_idx,
			cnt_autoupdate->twdm_wl_info.new_twdm_dswlch_id,
			cnt_autoupdate->dev->cnt_state->twdm_us_idx,
			cnt_autoupdate->twdm_wl_info.new_twdm_uswlch_id);
		cnt_autoupdate->dev->cnt_state->twdm_ds_idx =
			cnt_autoupdate->twdm_wl_info.new_twdm_dswlch_id;
		cnt_autoupdate->dev->cnt_state->twdm_us_idx =
			cnt_autoupdate->twdm_wl_info.new_twdm_uswlch_id;
		cnt_autoupdate->twdm_wl_info.trigger_update = 0;
	}

	dev_dbg(cnt_autoupdate->dev->dev,
		"Triggered update of %d GEM, %d Alloc Id, %d Alloc Lost, %d GTC, %d XGTC, %d TWDM LODS %d TWDM OPTIC_PL %d TWDM TC\n",
		num_gem_updates, num_alloc_updates, num_alloc_lost_updates,
		num_gtc_updates, num_xgtc_updates, num_twdm_lods_updates,
		num_twdm_optic_pl_updates, num_twdm_tc_updates);

	if (atomic_read(&cnt_autoupdate->running))
		ploam_cnt_autoupdate_update(cnt_autoupdate,
					    update_interval_jiffies);
}

void pon_mbox_cnt_autoupdate_wl_switch(struct cnt_autoupdate *cnt_autoupdate,
				       u8 new_dswlch_id, u8 new_uswlch_id)
{
	unsigned long since;

	if (cnt_autoupdate->update_time_in_s == PON_COUNTERS_NO_UPDATE)
		return;

	/* prepare wavelength switching */
	cnt_autoupdate->twdm_wl_info.new_twdm_dswlch_id = new_dswlch_id;
	cnt_autoupdate->twdm_wl_info.new_twdm_uswlch_id = new_uswlch_id;
	cnt_autoupdate->twdm_wl_info.trigger_update = 1;

	/* trigger counter update: current time - counter update time */
	since = jiffies -
		msecs_to_jiffies(cnt_autoupdate->update_time_in_s * 1000);
	schedule_counters_update(cnt_autoupdate, since);
}

/**
 * Destroys the automatic counter update context and releases its resources.
 */
void pon_mbox_cnt_autoupdate_destroy(struct cnt_autoupdate *cnt_autoupdate)
{
	if (!cnt_autoupdate)
		return;
	/*
	 * In general each of pon_mbox_*_counter_update() can take a lot of time
	 * to complete. This is because they wait for the firmware response.
	 * If firmware doesn't respond then this functions will wait and block
	 * until timeout is reached. This can cause module exit to take a
	 * very long time. To fix this problem, here we set the 'running'
	 * variable to 0 and the work checks for it before calling any of the
	 * pon_mbox_*_counter_update() functions
	 */
	atomic_set(&cnt_autoupdate->running, 0);
	del_timer_sync(&cnt_autoupdate->timer);
	flush_work(&cnt_autoupdate->work);
	kfree(cnt_autoupdate);
}

