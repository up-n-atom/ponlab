/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/
#ifndef __WHM_MXL_PARSER_H__
#define __WHM_MXL_PARSER_H__

#include <netlink/attr.h>

#include "wld/wld.h"

swl_rc_ne mxl_parseNaStaStats(T_Radio* pRad, struct nlattr* tb[], uint8_t reqType, bool syncDm);
swl_rc_ne mxl_parseChanDataEvt(T_Radio* pRad, struct nlattr* tb[]);
swl_rc_ne mxl_parseCsiStatsEvt(T_Radio* pRad, struct nlattr* tb[]);
swl_rc_ne mxl_parseWiphyInfo(swl_rc_ne rc, struct nlmsghdr* nlh, void* priv);

#endif /* __WHM_MXL_PARSER_H__ */
