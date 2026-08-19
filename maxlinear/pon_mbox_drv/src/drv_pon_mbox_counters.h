/*****************************************************************************
 *
 * Copyright (c) 2020 - 2025 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#ifndef DRV_PON_MBOX_COUNTERS_H
#define DRV_PON_MBOX_COUNTERS_H

/** \addtogroup PON_MBOX_REFERENCE_COUNTERS
 *  @{
 */

/* Counters table usage flow - reading:
 * 1. Get the counter using *_get function
 * 2. acquire a lock
 * 3. Do the reading operation on the table
 * 4. release lock
 * 5. release counter table pointer (this step is not required for current
 * implementation)
 *
 * Writing (and optional reading) to table:
 * 1. convert fw message(s) to PON structure
 * 2. acquire lock
 * 3. call *_add function
 * 4. call *_get function (in lock, to make sure you will get same table)
 * 5. use the counter table
 * 6. release lock
 * 7. release counter table
 *
 * Use *_fw2pon functions to convert firmware messages to wide structures. If
 * counter is composed from two different firmware messages, simply use two
 * corresponding functions with the same output structure, every function will
 * overwrite only needed fields in output structure.
 */

#include <linux/err.h>
#include <linux/types.h>
#include <pon/pon_mbox_counters.h>

/** Maximum number of upstream TWDM wavelength */
#define TWDM_US_MAX 8

/** Maximum number of downstream TWDM wavelength */
#define TWDM_DS_MAX 8

/** Maximum number of GEM ports */
#define GEM_PORTS_MAX 256

/** Maximum number of Allocation IDs */
#define ALLOC_IDS_MAX 64

/** Types of available counters */
enum counter_type {
	/** GEM port based counters (ITU PON only) */
	GEM_PORT_CNT		= 0,
	/** Allocation ID based counters (ITU PON only) */
	ALLOC_CNT		= 1,
	/** GTC counters (ITU-T G.984 only) */
	GTC_CNT			= 2,
	/** XGTC counters (ITU-T G.987, 9807, 989 only) */
	XGTC_CNT		= 3,
	/** Lost allocations counters (ITU PON only) */
	ALLOC_LOST_CNT		= 4,
	/** Ethernet counters in receive (downstream) direction */
	RX_ETH_CNT		= 5,
	/** Ethernet counters in transmit (upstream) direction */
	TX_ETH_CNT		= 6,

	/** TWDM LODS counters (NG-PON2 only) */
	TWDM_LODS_CNT		= 7,
	/** TWDM Optic Power Level counters (NG-PON2 only) */
	TWDM_OPTIC_PL_CNT	= 8,
	/** XGTC PLOAM DS message counters (not in GPON mode) */
	XGTC_PLOAM_DS_CNT	= 9,
	/** GTC PLOAM DS message counters (GPON mode only) */
	GTC_PLOAM_DS_CNT	= 10,
	/** XGTC PLOAM US message counters (not in GPON mode) */
	XGTC_PLOAM_US_CNT	= 11,
	/** GTC PLOAM US message counters (GPON mode only) */
	GTC_PLOAM_US_CNT	= 12,
	/** TWDM Tuning Control counters (NG-PON2 only) */
	TWDM_TC_CNT		= 13,
	/** Number of counter types */
	MAX_CTRL_CNT		= 14,
};

/**
 * Information about when the counters were last time updated.
 * Time values are given in jiffies.
 */
struct counters_last_update {
	/** Last update time of gtc counters */
	unsigned long gtc_counters;
	/** Last update time of xgtc counters */
	unsigned long xgtc_counters;
	/** Last update time of GEM_port counters */
	unsigned long gem_port_counters[GEM_PORTS_MAX];
	/** Last update time of Alloc ID counters */
	unsigned long alloc_counters[ALLOC_IDS_MAX];
	/** Last update time of Alloc ID counters */
	unsigned long alloc_discard_counters;
	/** Last update time of Eth RX counters */
	unsigned long eth_rx_counters[GEM_PORTS_MAX];
	/** Last update time of Eth TX counters */
	unsigned long eth_tx_counters[GEM_PORTS_MAX];
	unsigned long twdm_lods_counters;
	/** Last update time of NG-PON2 Optic Power Level counters */
	unsigned long twdm_optic_pl_counters;
	/** Last update time of NG-PON2 TWDM Tuning Control counters */
	unsigned long twdm_tc_counters;
	/** Last update time of XGTC PLOAM DS message counters */
	unsigned long xgtc_ploam_ds_counters;
	/** Last update time of GTC PLOAM DS message counters */
	unsigned long gtc_ploam_ds_counters;
	/** Last update time of XGTC PLOAM US message counters */
	unsigned long xgtc_ploam_us_counters;
	/** Last update time of GTC PLOAM US message counters */
	unsigned long gtc_ploam_us_counters;
};

 /** GPON related counters to be stored inside temporary storage */
struct gpon_temporary_counters {
	struct pon_mbox_gtc_counters gtc;			/* DS */
	struct pon_mbox_xgtc_counters xgtc;			/* DS */
	struct pon_mbox_gem_port_counters gem_port[GEM_PORTS_MAX]; /* DS+US */
	struct pon_alloc_counters alloc[ALLOC_IDS_MAX];		/* US */
	struct pon_alloc_discard_counters alloc_discard;	/* US */
	struct pon_eth_counters rx_eth[GEM_PORTS_MAX];		/* DS */
	struct pon_eth_counters tx_eth[GEM_PORTS_MAX];		/* US */
	struct pon_mbox_twdm_lods_counters twdm_lods;
	struct pon_mbox_twdm_optic_pl_counters twdm_optic_pl;
	struct pon_mbox_twdm_tc_counters twdm_tc;
	struct pon_mbox_xgtc_ploam_ds_counters xgtc_ploam_ds;
	struct pon_mbox_gtc_ploam_ds_counters gtc_ploam_ds;
	struct pon_mbox_xgtc_ploam_us_counters xgtc_ploam_us;
	struct pon_mbox_gtc_ploam_us_counters gtc_ploam_us;
};

/** GPON related total counters to store the FW/HW total counter */
struct gpon_storage_total_counters {
	struct pon_mbox_gtc_counters gtc_counters;		/* DS */
	struct pon_mbox_xgtc_counters xgtc_counters;		/* DS */
	struct pon_mbox_gem_port_counters gem_port_counters[GEM_PORTS_MAX];
	struct pon_alloc_counters
		alloc_counters[ALLOC_IDS_MAX];	/* US */
	struct pon_alloc_discard_counters
		alloc_discard_counters;		/* US */
	struct pon_eth_counters eth_rx_counters[GEM_PORTS_MAX];	/* DS */
	struct pon_eth_counters eth_tx_counters[GEM_PORTS_MAX];	/* US */
	struct pon_mbox_twdm_lods_counters twdm_lods_counter;
	struct pon_mbox_twdm_optic_pl_counters twdm_optic_pl_counter;
	struct pon_mbox_twdm_tc_counters twdm_tc_counter;
	struct pon_mbox_xgtc_ploam_ds_counters xgtc_ploam_ds_counter;
	struct pon_mbox_gtc_ploam_ds_counters gtc_ploam_ds_counter;
	struct pon_mbox_xgtc_ploam_us_counters xgtc_ploam_us_counter;
	struct pon_mbox_gtc_ploam_us_counters gtc_ploam_us_counter;
};

/** GPON related wavelength specific counters */
struct gpon_storage_counters {
	struct pon_mbox_gtc_counters gtc_counters[TWDM_DS_MAX];
	struct pon_mbox_xgtc_counters xgtc_counters[TWDM_DS_MAX];
	struct pon_mbox_gem_port_counters
		gem_port_counters[TWDM_DS_MAX][TWDM_US_MAX][GEM_PORTS_MAX];
	struct pon_mbox_gem_port_counters
		gem_all_counters[TWDM_DS_MAX][TWDM_US_MAX];
	struct pon_alloc_counters alloc_counters[TWDM_US_MAX][ALLOC_IDS_MAX];
	struct pon_alloc_discard_counters alloc_discard_counters[TWDM_US_MAX];
	struct pon_eth_counters eth_rx_counters[TWDM_DS_MAX][GEM_PORTS_MAX];
	struct pon_eth_counters eth_tx_counters[TWDM_US_MAX][GEM_PORTS_MAX];
	struct pon_mbox_twdm_lods_counters twdm_lods[TWDM_DS_MAX];
	struct pon_mbox_twdm_optic_pl_counters twdm_optic_pl[TWDM_DS_MAX];
	struct pon_mbox_twdm_tc_counters twdm_tc[TWDM_DS_MAX];
	struct pon_mbox_xgtc_ploam_ds_counters xgtc_ploam_ds[TWDM_DS_MAX];
	struct pon_mbox_gtc_ploam_ds_counters gtc_ploam_ds[TWDM_DS_MAX];
	struct pon_mbox_xgtc_ploam_us_counters xgtc_ploam_us[TWDM_US_MAX];
	struct pon_mbox_gtc_ploam_us_counters gtc_ploam_us[TWDM_US_MAX];
};

/** Temporary storage, used for calculation counters increase. Contains
 * last response from the firmware, converted to wide PON structures.
 */
struct temporary_counters {
	union {
		struct gpon_temporary_counters gpon;
	};
};

struct counters_storage {
	struct gpon_storage_total_counters gpon_total;
	union {
		struct gpon_storage_counters gpon;
	};

	/** tables_lock is used to protect counters tables from corruption while
	 *  modification, should be used while reading or writing data from
	 *  the tables.
	 *  It is used only in pon_mbox_cnt_lock/unlock functions.
	 */
	struct mutex tables_lock;
};

/* counters state variables */
struct counters_state {
	u8 twdm_us_idx;
	u8 twdm_ds_idx;

	struct temporary_counters last_counters;

	/* Used Alloc IDs (1 when is in use, 0 if is free) */
	u8 alloc_id_used[ALLOC_IDS_MAX];

	/* Used GEM ports (in same way as in Alloc IDs) */
	u8 gem_ports_used[GEM_PORTS_MAX];

	struct counters_last_update last_update;

	/** lock is used internally to protect accessing tables while
	 *  changing state (TWDM indexes), GEM Ports or Allocation IDs
	 *  tables. Protects "counters_state" structure elements.
	 */
	struct mutex lock;

	struct counters_storage storage;
};

/**
 * @brief Call this function when a new GEM port is created.
 *
 * @param index
 * @param state pointer to counters state
 *
 * @return 0 for success
 */
int pon_mbox_cnt_gem_port_add(u8 index, struct counters_state *state);

/**
 * @brief Call this function when a GEM port is deleted.
 *
 * @param index
 * @param state pointer to counters state
 *
 * @return 0 for success
 */
int pon_mbox_cnt_gem_port_del(u8 index, struct counters_state *state);

/**
 * @brief Call this function when a new Alloc ID is created.
 *
 * @param index
 * @param state pointer to counters state
 *
 * @return 0 for success
 */
int pon_mbox_cnt_alloc_id_add(u8 index, struct counters_state *state);

/**
 * @brief Call this function when an Alloc ID is deleted.
 *
 * @param index
 * @param state pointer to counters state
 *
 * @return 0 for success
 */
int pon_mbox_cnt_alloc_id_del(u8 index, struct counters_state *state);

/* counters access functions */

/**
 * @brief Get the table for GTC_COUNTERS and GTC_ENHANCED_COUNTERS messages.
 *
 * @param twdm_dswlch_id TWDM DS Wavelength Channel ID
 * @param state pointer to counters state
 *
 * @return pointer to the table
 */
struct pon_mbox_gtc_counters
*pon_mbox_cnt_gtc_table_get(u8 twdm_dswlch_id,
			    struct counters_state *state);

/**
 * @brief Add new counter set to the GTC_COUNTERS table.
 *
 * @param cnt pointer to the new requested FW counter.
 * @param state pointer to counters state.
 *
 * @return 0 on success
 */
int pon_mbox_cnt_gtc_table_add(struct pon_mbox_gtc_counters *cnt,
			       struct counters_state *state);

/**
 * @brief Get the table for XGTC_COUNTERS.
 *
 * @param twdm_dswlch_id TWDM DS Wavelength Channel ID
 * @param state pointer to counters state
 *
 * @return pointer to the counter. It has to be released with
 *	   pon_mbox_cnt_release
 */
struct pon_mbox_xgtc_counters
*pon_mbox_cnt_xgtc_table_get(u8 twdm_dswlch_id,
			     struct counters_state *state);

/**
 * @brief Add new counter set to the XGTC_COUNTERS table.
 *
 * @param cnt pointer to the new requested counter.
 * @param state pointer to counters state.
 *
 * @return 0 on success
 */
int pon_mbox_cnt_xgtc_table_add(struct pon_mbox_xgtc_counters *cnt,
				struct counters_state *state);

/**
 * @brief Get the pointer to the GEM port counters table.
 *        It increments the reference for the table.
 *
 * @param index GEM port index to get table
 * @param state pointer to counters state
 *
 * @return pointer to the counter, or NULL on error. It has to be released with
 *	   pon_mbox_cnt_release
 */
struct pon_mbox_gem_port_counters
*pon_mbox_cnt_gem_port_table_get(u8 twdm_dswlch_id, u8 index,
				 struct counters_state *state);

/**
 * @brief Add new counter set to the GEM_PORT_COUNTERS table.
 *
 * @param index GEM port index to get table
 * @param cnt pointer to the new requested counter.
 * @param state pointer to counters state
 *
 * @return 0 on success
 */
int pon_mbox_cnt_gem_port_table_add(u8 index,
				    struct pon_mbox_gem_port_counters *cnt,
				    struct counters_state *state);

/**
 * @brief Get the pointer to the GEM port accumulated counters table.
 *
 * @param twdm_dswlch_id TWDM DS Wavelength Channel ID
 * @param state          pointer to counters state
 *
 * @return pointer to the counter, or NULL on error. It has to be released with
 *	   pon_mbox_cnt_release
 */
struct pon_mbox_gem_port_counters
*pon_mbox_cnt_gem_all_table_get(u8 twdm_dswlch_id,
				struct counters_state *state);

/**
 * @brief Get the pointer to the Alloc ID counters table.
 *	  It increments the reference for the table.
 *
 * @param index Alloc ID index to get table
 * @param state pointer to counters state
 *
 * @return pointer to the counter, or NULL on error. It has to be released with
 *	   pon_mbox_cnt_release
 */
struct pon_alloc_counters
*pon_mbox_cnt_alloc_id_table_get(u8 index, struct counters_state *state);
int pon_mbox_cnt_alloc_id_table_add(u8 index,
				    struct pon_alloc_counters *cnt,
				    struct counters_state *state);

/**
 * @brief Get the pointer to the Allocation Discarded Counters.
 *
 * @param state pointer to counters state
 *
 * @return pointer to the counter, or NULL on error. It has to be released with
 *	   pon_mbox_cnt_release
 */
struct pon_alloc_discard_counters
*pon_mbox_cnt_alloc_lost_table_get(struct counters_state *state);
int pon_mbox_cnt_alloc_lost_table_add(struct pon_alloc_discard_counters *cnt,
				      struct counters_state *state);

/**
 * @brief Get the pointer to the Ethernet receive counters.
 *
 * @param index GEM Port Index
 * @param state pointer to counters state
 *
 * @return pointer to the counter, or NULL on error. It has to be released with
 *	   pon_mbox_cnt_release
 */
struct pon_eth_counters
*pon_mbox_cnt_rx_eth_table_get(u8 index, struct counters_state *state);
int pon_mbox_cnt_rx_eth_table_add(u8 index,
				  struct pon_eth_counters *cnt,
				  struct counters_state *state);

/**
 * @brief Get the pointer to the Ethernet transmit counters.
 *
 * @param index GEM Port Index
 * @param state pointer to counters state
 *
 * @return pointer to the counter, or NULL on error. It has to be released with
 *	   pon_mbox_cnt_release
 */
struct pon_eth_counters
*pon_mbox_cnt_tx_eth_table_get(u8 index, struct counters_state *state);
int pon_mbox_cnt_tx_eth_table_add(u8 index,
				  struct pon_eth_counters *cnt,
				  struct counters_state *state);

/**
 * @brief Get the table for the TWDM_LODS_COUNTERS.
 *
 * @param twdm_dswlch_id TWDM DS Wavelength Channel ID
 * @param state pointer to counters state
 *
 * @return pointer to the counter. It has to be released with
 *	   pon_mbox_cnt_release
 */
struct pon_mbox_twdm_lods_counters
*pon_mbox_cnt_twdm_lods_table_get(u8 twdm_dswlch_id,
				  struct counters_state *state);

/**
 * @brief Add new counter set to the TWDM_LODS counter table.
 *
 * @param cnt pointer to the new requested counter.
 * @param state pointer to counters state.
 *
 * @return 0 on success
 */
int pon_mbox_cnt_twdm_lods_table_add(struct pon_mbox_twdm_lods_counters *cnt,
				     struct counters_state *state);

/**
 * @brief Get the table for the TWDM_OPTIC_PL_COUNTERS.
 *
 * @param twdm_dswlch_id TWDM DS Wavelength Channel ID
 * @param state pointer to counters state
 *
 * @return pointer to the counter. It has to be released with
 *	   pon_mbox_cnt_release
 */
struct pon_mbox_twdm_optic_pl_counters
*pon_mbox_cnt_twdm_optic_pl_table_get(u8 twdm_dswlch_id,
				      struct counters_state *state);

/**
 * @brief Add new counter set to the twdm_optic_pl counter table.
 *
 * @param cnt pointer to the new requested counter.
 * @param state pointer to counters state.
 *
 * @return 0 on success
 */
int pon_mbox_cnt_twdm_optic_pl_table_add(
				struct pon_mbox_twdm_optic_pl_counters *cnt,
				struct counters_state *state);

/**
 * @brief Get the table for the TWDM_TC_COUNTERS.
 *
 * @param twdm_dswlch_id TWDM DS Wavelength Channel ID
 * @param state pointer to counters state
 *
 * @return pointer to the counter. It has to be released with
 *	   pon_mbox_cnt_release
 */
struct pon_mbox_twdm_tc_counters
*pon_mbox_cnt_twdm_tc_table_get(u8 twdm_dswlch_id,
				struct counters_state *state);

/**
 * @brief Add new counter set to the twdm_tc counter table.
 *
 * @param cnt pointer to the new requested counter.
 * @param state pointer to counters state.
 *
 * @return 0 on success
 */
int pon_mbox_cnt_twdm_tc_table_add(
				struct pon_mbox_twdm_tc_counters *cnt,
				struct counters_state *state);

/**
 * @brief Get the table for the GTC_PLOAM_DS_COUNTERS.
 *
 * @param state pointer to counters state
 *
 * @return pointer to the counter. It has to be released with
 *	   pon_mbox_cnt_release
 */
struct pon_mbox_gtc_ploam_ds_counters
*pon_mbox_cnt_gtc_ploam_ds_table_get(struct counters_state *state);

/**
 * @brief Add new counter set to the gtc_ploam_ds counter table.
 *
 * @param cnt pointer to the new requested counter.
 * @param state pointer to counters state.
 *
 * @return 0 on success
 */
int pon_mbox_cnt_gtc_ploam_ds_table_add(
				struct pon_mbox_gtc_ploam_ds_counters *cnt,
				struct counters_state *state);

/**
 * @brief Get the table for the XGTC_PLOAM_DS_COUNTERS.
 *
 * @param twdm_dswlch_id TWDM DS Wavelength Channel ID
 * @param state pointer to counters state
 *
 * @return pointer to the counter. It has to be released with
 *	   pon_mbox_cnt_release
 */
struct pon_mbox_xgtc_ploam_ds_counters
*pon_mbox_cnt_xgtc_ploam_ds_table_get(u8 twdm_dswlch_id,
				      struct counters_state *state);

int pon_mbox_cnt_xgtc_ploam_ds_table_add(
				struct pon_mbox_xgtc_ploam_ds_counters *cnt,
				struct counters_state *state);

/**
 * @brief Get the table for the GTC_PLOAM_US_COUNTERS.
 *
 * @param state pointer to counters state
 *
 * @return pointer to the counter. It has to be released with
 *	   pon_mbox_cnt_release
 */
struct pon_mbox_gtc_ploam_us_counters
*pon_mbox_cnt_gtc_ploam_us_table_get(struct counters_state *state);
int pon_mbox_cnt_gtc_ploam_us_table_add(
				struct pon_mbox_gtc_ploam_us_counters *cnt,
				struct counters_state *state);

/**
 * @brief Get the table for the XGTC_PLOAM_US_COUNTERS.
 *
 * @param twdm_dswlch_id TWDM DS Wavelength Channel ID
 * @param state pointer to counters state
 *
 * @return pointer to the counter. It has to be released with
 *	   pon_mbox_cnt_release
 */
struct pon_mbox_xgtc_ploam_us_counters
*pon_mbox_cnt_xgtc_ploam_us_table_get(u8 twdm_dswlch_id,
				      struct counters_state *state);

int pon_mbox_cnt_xgtc_ploam_us_table_add(
				struct pon_mbox_xgtc_ploam_us_counters *cnt,
				struct counters_state *state);

/**
 * @brief Initialize the memory for the counters state.
 *
 * @return pointer to the counters state
 */
struct counters_state *pon_mbox_cnt_state_init(void);

/**
 * @brief Release the given counters state from memory.
 *
 * @param cnt pointer to counters state
 */
void pon_mbox_cnt_state_release(struct counters_state *cnt);

/**
 * @brief Release the given counter. Internally it decrements the reference
 *	  counter, and frees if the counter becomes 0.
 *	  This is for future use, it does nothing for now.
 *
 * @param counter pointer to counter table
 */
void pon_mbox_cnt_release(void *counter); /* function releases counters, if
					   * refcnt is 0. Needed for Alloc ID/
					   * GEM port, as both can be deleted
					   */
/**
 * @brief Reset the counters state to initial values.
 *
 * @param state pointer to counters state
 */
void pon_mbox_cnt_reset(struct counters_state *state);

/**
 * @brief Activate the counter set for the given TWDM US and DS wavelength
 *	  index.
 *	  The function prepares new internal storage, or reuses it if DS/US
 *	  was used before.
 *
 * @param us_index TWDM upstream wavelength index value
 * @param ds_index TWDM downstream wavelength index value
 * @param state pointer to counters state
 *
 * @return -EINVAL when arguments are out of allowed range, 0 on success
 */
int pon_mbox_activate_counter_set(u8 us_index,
				  u8 ds_index,
				  struct counters_state *state);

/**
 * @brief Obtain an array of used GEM ports.
 *
 * @param[out] gem_port_idx Array of GEM port indexes.
 * @param[in]  size Capacity of provided gem_port_idx array
 * @param[in]  state pointer to counters state
 *
 * @return number of used GEM ports
 */
int pon_mbox_used_gem_ports_get(u8 *gem_port_idx,
				unsigned int size,
				struct counters_state *state);

/**
 * @brief Obtain an array of used Alloc IDs
 *
 * @param[out] alloc_id_used Array of used Alloc IDs.
 * @param[in]  size Capacity of provided alloc_id_used array
 * @param[in]  state pointer to counters state
 *
 * @return number of used Alloc IDs
 */
int pon_mbox_used_alloc_ids_get(u8 *alloc_id_used,
				unsigned int size,
				struct counters_state *state);

/**
 * @brief Read the update times of each counter.
 *
 * @param[in] last_update times of the last update of every counter
 * @param[in] state pointer to counters state
 */
void pon_mbox_last_update_get(struct counters_last_update *last_update,
			      struct counters_state *state);

/**
 * @brief Returns the oldest update time of the counters.
 *	  We need this to know how soon should we schedule the
 *	  next counters update
 *
 * @param state pointer to counters state
 * @param mask
 *
 * @return oldest update time
 */
unsigned long
pon_mbox_cnt_oldest_update_time_get(struct counters_state *state,
				    u64 mask);

/**
 * @brief Acquire tables lock to prevent modification while operating on it
 *
 * @param state pointer to counters state
 */
void pon_mbox_cnt_lock(struct counters_state *state);

/**
 * @brief Release tables lock.
 *
 * @param state pointer to counters state
 */
void pon_mbox_cnt_unlock(struct counters_state *state);

/*! @} */

#endif /* end of include guard: DRV_PON_MBOX_COUNTERS_H */
