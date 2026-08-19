#ifndef __LTQ_LGM_PP_HAL_H_2018011_11_1145__
#define __LTQ_LGM_PP_HAL_H_2018011_11_1145__

/******************************************************************************
**
** FILE NAME    : lgm_pp_hal.h
** PROJECT      : LGM
** MODULES      : PPA PPv4 HAL
**
** DATE         : 19 Nov 2018
** AUTHOR       : Kamal Eradath
** DESCRIPTION  : PPv4 Hardware Abstraction Layer
** COPYRIGHT    :       Copyright (c) 2018
**               	Intel Corporation
**
**   For licensing information, see the file 'LICENSE' in the root folder of
**   this software module.
**
** HISTORY
** $Date        $Author         $Comment
** 19 NOV 2018  Kamal Eradath   Initiate Version
*******************************************************************************/
/*
 * ####################################
 *              Definition
 * ####################################
 */

/*
 *  Compilation Switch
 */

/*
 * PPv4 MAX entries
 */

#define MAX_LGM_PORTS	16
#define MAX_SUBIF_IDS	16

#define MAX_UC_SESSION_ENTRIES		(65536U)   /*64k */
#define	MAX_TUN_ENTRIES			256

#ifndef MC_CLIENT_SIXTEEN
#define MAX_MC_GROUP_ENTRIES		512	/*Upto 512 multicast group support */
#define MAX_MC_CLIENT_PER_GRP		8	/*Upto 8 clients per group */
#else
#define MAX_MC_GROUP_ENTRIES		256	/*Upto 256 multicast group support */
#define MAX_MC_CLIENT_PER_GRP		16	/*Upto 16 clients per group */
#endif /*MC_CLIENT_SIXTEEN*/

/*#define PPA_DEST_LIST_CPU0	0x01    //0000000000000001 */
#define PPA_DEST_LIST_ETH0	0x02    /*0000000000000010 */
#define PPA_DEST_LIST_ETH0_1	0x04	/*0000000000000100 */
#define PPA_DEST_LIST_ETH0_2	0x08	/*0000000000001000 */
#define PPA_DEST_LIST_ETH0_3	0x010	/*0000000000010000 */
#define PPA_DEST_LIST_ETH0_4	0x020	/*0000000000100000 */
/*#define PPA_DEST_LIST_ATM	0x2000	// 0010000000000000 */
#define PPA_DEST_LIST_ETH1	0x8000  /*1000000000000000 */

#define	IP_PROTO_UDP		0x11	/*17*/
#define	IP_PROTO_TCP		0x06
#define	IP_PROTO_ESP		0x32	/*50 */

/*To be used for all the uCs */
struct nf_node {
	uint16_t gpid;		/*allocated gpid */
	uint16_t qid;		/*allocated qid*/
	uint16_t subif;		/*allocated subif*/
	uint16_t uc_id;		/*uC id returned by pp*/
	struct net_device *dev;	/*dummy netdevice*/
};

struct	lp_info * pp_hal_db_get_lp_rxinfo(int32_t sess_id);
bool	is_pp_sess_valid(int32_t sess_id);

/*
 * ####################################
 *              Data Type
 * ####################################
 */

#endif /*__LTQ_LGM_PP_HAL_H_2018011_11_1145__*/

