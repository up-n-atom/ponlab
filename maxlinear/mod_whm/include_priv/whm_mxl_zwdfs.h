/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/
#ifndef __WHM_MXL_ZWDFS_H__
#define __WHM_MXL_ZWDFS_H__

#include "wld/wld.h"

/**
 * Create an internal radio context for ZwDfs interface without mapping to the datamodel
 *
 * @param name interface name
 * @param vendor vendor module context
 * @param index interface index
 *
 * @return
 *  - Success: SWL_RC_OK
 *  - Failure: SWL_RC_ERROR
 */
int mxl_rad_addZwDfsRadio(const char* name, vendor_t* vendor, int index);

/**
 * Delete the created ZwDfs radio interface
 */
void mxl_rad_deleteZwDfsRadio();

/**
 * Get the ZwDfs radio context
 * @return
 *  - Success: ZwDfs radio context
 *  - Failure: NULL
 */
T_Radio* mxl_rad_getZwDfsRadio();

/**
 * Background DFS FTA implementation:
 * - Enable ZwDfs antenna
 * - Start background clearing
 * - Stop background clearing
 */
int whm_mxl_rad_bgDfsEnable(T_Radio* pRad, int enable);
int whm_mxl_rad_bgDfsStart(T_Radio* pRad, int channel);
int whm_mxl_rad_bgDfsStartExt(T_Radio* pRad, wld_startBgdfsArgs_t* args);
int whm_mxl_rad_bgDfs_stop(T_Radio* pRad);

/**
 * Update the ZwDfs Reg domain in case 5G Reg domain is updated
 */
void mxl_rad_updateZwDfsRegDomain(T_Radio* pRad);

#endif /* __WHM_MXL_ZWDFS_H__ */
