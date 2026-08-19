/******************************************************************************
 * Copyright (c) 2021 - 2022, MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#ifndef DATAPATH_DFLT_Q_MAP_PRX300
#define DATAPATH_DFLT_Q_MAP_PRX300

#define subif_q_map_reset_lookup_f ((CBM_QUEUE_MAP_F_TC_DONTCARE) | \
			     (CBM_QUEUE_MAP_F_COLOR_DONTCARE) | \
			     (CBM_QUEUE_MAP_F_EN_DONTCARE) | \
			     (CBM_QUEUE_MAP_F_DE_DONTCARE) | \
			     (CBM_QUEUE_MAP_F_MPE1_DONTCARE) | \
			     (CBM_QUEUE_MAP_F_MPE2_DONTCARE) | \
			     (CBM_QUEUE_MAP_F_FLOWID_L_DONTCARE) | \
			     (CBM_QUEUE_MAP_F_FLOWID_H_DONTCARE))
#define drop_lookup_f -1
/* PRX300 CPU rule */
static struct dp_q_map_entry q_map_cpu[] = {
	/* Rule 1: Normal CPU RX traffic */
	{
		.lookup_f = 0,
		.enc = 0,
		.mpe1 = 0,
		.mpe2 = 0,
		.cls1 = 0,
		.cls2 = MAX_TC_ID,
		.q_type = DP_MAP_Q_CPU,
	},
};

/* ethernet based streaming port rule */
static struct dp_q_map_entry q_map_stream_ethernet[] = {
	/* Rule 1: Normal traffic to streaming port */
	{
		.lookup_f = 0,
		.enc = 0,
		.mpe1 = 0,
		.mpe2 = 0,
		.cls1 = 0,
		.cls2 = MAX_TC_ID,
		.q_type = DP_MAP_Q_SUBIF,
	},
	/* Rule 2: Extraction traffic to CPU */
	{
		.lookup_f = 0,
		.enc = 0,
		.mpe1 = 0,
		.mpe2 = 1,
		.cls1 = 0,
		.cls2 = MAX_TC_ID,
		.q_type = DP_MAP_Q_CPU,
	}
};

/* Reinsert port rule */
static struct dp_q_map_entry q_map_reinsert[] = {
	/* Rule 1: re-insertion traffic
	 * ENC=1, DEC=0, MPE2/1=0
	 */
	{
		.lookup_f = 0,
		.enc = 1,
		.mpe1 = 0,
		.mpe2 = 0,
		.cls1 = 0,
		.cls2 = MAX_TC_ID,
		.q_type = DP_MAP_Q_REINSERT,
	}
};

/* PON port rule */
static struct dp_q_map_entry q_map_pon[] = {
	/* rule 1: Normal traffic to this port */
	{
		.lookup_f = 0,
		.enc = 0,
		.mpe1 = 0,
		.mpe2 = 0,
		.cls1 = 0,
		.cls2 = MAX_TC_ID,
		.q_type = DP_MAP_Q_SUBIF,
	},
	/* Rule 2: Extraction traffic to CPU */
	{
		.lookup_f = 0,
		.enc = 0,
		.mpe1 = 0,
		.mpe2 = 1,
		.cls1 = 0,
		.cls2 = MAX_TC_ID,
		.q_type = DP_MAP_Q_CPU,
	}
};

/* vUNI rule */
static struct dp_q_map_entry q_map_vUNI[] = {
	/* rule 1: vUNI from GSWIP to CPU */
	{
		.lookup_f = 0,
		.enc = 0,
		.mpe1 = 0,
		.mpe2 = 0,
		.cls1 = 0,
		.cls2 = MAX_TC_ID,
		.q_type = DP_MAP_Q_CPU,
	},
	/* Rule 2: vUNI upstream traffic */
	{
		.lookup_f = 0,
		.enc = 0,
		.mpe1 = 1,
		.mpe2 = 1,
		.cls1 = 0,
		.cls2 = MAX_TC_ID,
		.q_type = DP_MAP_Q_SUBIF,
	},
	/* Rule 3: Extraction traffic to CPU */
	{
		.lookup_f = 0,
		.enc = 0,
		.mpe1 = 0,
		.mpe2 = 1,
		.cls1 = 0,
		.cls2 = MAX_TC_ID,
		.q_type = DP_MAP_Q_CPU,
	}
};

/* PRX300 Directpath (IP Host ) rule */
static struct dp_q_map_entry q_map_directpath[] = {
	/* rule 1: Traffic to this port/device via CPU Port
	 *         For example: PON downstream IP host traffic to CPU
	 */
	{
		.lookup_f = 0,
		.enc = 0,
		.mpe1 = 0,
		.mpe2 = 0,
		.cls1 = 0,
		.cls2 = MAX_TC_ID,
		.q_type = DP_MAP_Q_CPU,
	},
	/* Rule 2: From Directpath Network driver to other device via GSWIP
	 *         For example: CPU IP host network traffic to PON
	 */
	{
		.lookup_f = 0,
		.enc = 0,
		.mpe1 = 1,
		.mpe2 = 1,
		.cls1 = 0,
		.cls2 = MAX_TC_ID,
		.q_type = DP_MAP_Q_SUBIF,
	}
};

/* ACA/DC rule ( like DSL/WIFIF) */
static struct dp_q_map_entry q_map_aca[] = {
	/* rule 1: CPU/MPE FW to this device */
	{
		.lookup_f = 0,
		.enc = 0,
		.mpe1 = 0,
		.mpe2 = 0,
		.cls1 = 0,
		.cls2 = MAX_TC_ID,
		.q_type = DP_MAP_Q_SUBIF,
	},
	/* Rule 2: Extraction traffic to CPU */
	{
		.lookup_f = 0,
		.enc = 0,
		.mpe1 = 0,
		.mpe2 = 1,
		.cls1 = 0,
		.cls2 = MAX_TC_ID,
		.q_type = DP_MAP_Q_CPU,
	}
};

/* spl_conn's IGP'e egp queue map */
static struct dp_q_map_entry q_map_spl_conn_igp[] = {
	/* Not valid for PRX */
};

#endif /* DATAPATH_DFLT_Q_MAP_PRX300 */

