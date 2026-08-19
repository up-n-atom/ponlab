/******************************************************************************
**
** FILE NAME	: lgm_lro_hal.h
** PROJECT	: LGM
** MODULES	: PPA LRO HAL
**
** DATE		: 19 October 2022
** AUTHOR	: Anath Bandhu Garai
** DESCRIPTION	: LRO hardware abstraction layer
** COPYRIGHT	: Copyright (c) 2022 MaxLinear, Inc.
**
**	 For licensing information, see the file 'LICENSE' in the root folder of
**	 this software module.
**
** HISTORY
** $Date		$Author			$Comment
** 19 October 2022	Anath Bandhu Garai	Initial Version
*******************************************************************************/
#ifndef __LGM_LRO_HAL_H__
#define __LGM_LRO_HAL_H__

#define MAX_LRO_ENTRIES		256

#if IS_ENABLED(CONFIG_LGM_TOE)
void	init_lro_hal(void);
void	uninit_lro_hal(void);
int32_t	add_lro_entry(PPA_LRO_INFO *lro_entry);
int32_t	del_lro_entry(uint8_t sessionid);
#else
#define init_lro_hal()
#define uninit_lro_hal()

static inline int32_t add_lro_entry(PPA_LRO_INFO *lro_entry)
{
	return PPA_ENOTAVAIL;
}

static int32_t del_lro_entry(uint8_t sessionid)
{
	return PPA_ENOTAVAIL;
}
#endif /* CONFIG_LGM_TOE */

#endif /* __LGM_LRO_HAL_H__ */
