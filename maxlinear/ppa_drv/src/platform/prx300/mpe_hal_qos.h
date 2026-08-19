#ifndef __MPE_QOS_H__20230607__
#define __MPE_QOS_H__20230607__

/*******************************************************************************
 **
 ** FILE NAME   : mpe_hal_qos.h
 ** PROJECT : MPE HAL
 ** MODULES : MPE (Routing/Bridging Acceleration )
 **
 ** DATE    : 5 July 2023
 ** AUTHOR  : Ramulu Komme
 ** DESCRIPTION : MPE HAL Layer
 ** COPYRIGHT   : Copyright © 2023-2024 MaxLinear, Inc.
 **
 **  For licensing information, see the file 'LICENSE' in the root folder of
 **  this software module.
 **
 *******************************************************************************/

int mpe_hal_create_mpe_netdev(void);
int mpe_hal_add_sched(struct dp_dequeue_res *dq_res);
int mpe_hal_add_ds_queue(int sch_id);
void mpe_hal_remove_mpe_netdev(void);
void mpe_hal_update_mpe_qos_info(u32 dp_port, int32_t q_id);

#endif  /* __MPE_QOS_H__20230607__ */
