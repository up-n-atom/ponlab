/****************************************************************************
****************************************************************************
**
** COMPONENT:      Header File
**
** MODULE:         $File: ../develop/src/sys/common/mtlkpack.h $
**
** VERSION:        $Revision: #1 $
**
** DATED:          $Date: 2007/3/11 $
**
** AUTHOR:         Mark Tomarov
** DESCRIPTION:    This file is used for packing in shared files
**
** LAST MODIFIED BY:   $Author: Mark Tomarov
**                     $Modtime:   11/3/07
**
** COPYRIGHT: (c) 2020, MaxLinear, Inc.
** COPYRIGHT: 2016 - 2020 Intel Corporation
** COPYRIGHT: 2014 - 2016 Lantiq Beteiligungs - GmbH & Co.KG
** COPYRIGHT: 2010 - 2014 Lantiq Deutschland GmbH
** COPYRIGHT: 2007 - 2010 Metalink Ltd.
**
****************************************************************************/

#ifdef MTLK_PACK_OFF
#undef MTLK_PACK_OFF
#undef MTLK_PACK_ON
#endif


#if defined(MTLK_PACK_ON) && !defined(__INTERNAL_MTLK_PACK_ON__)
#define __INTERNAL_MTLK_PACK_ON__

/**********************************************************/
/* Packing ON here                                        */
#define __MTLK_PACKED
//#pragma pack(1)
/**********************************************************/

/**********************************************************/
/* Padding ON here                                        */
#define MTLK_PAD4(S)       (((4 - ((S) & 0x3)) & 0x3) + (S)) /* for uin8 arrays padding */
#define MTLK_PAD2(S)       (((2 - ((S) & 0x1)) & 0x1) + (S)) /* for uin16 arrays padding */

/**********************************************************/

#elif !defined(MTLK_PACK_ON) && defined(__INTERNAL_MTLK_PACK_ON__)

/**********************************************************/
/* Padding OFF here                                       */
#undef MTLK_PAD4
/**********************************************************/

/**********************************************************/
/* Packing OFF here                                       */
//#pragma pack()
#undef __MTLK_PACKED
/**********************************************************/
#undef __INTERNAL_MTLK_PACK_ON__
#endif


