/******************************************************************************

                         Copyright (c) 2012, 2014, 2015
                        Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

/*!
  \brief QOS general configuration init flag
*/
#define QOS_CFG_INIT 0x10000000
/*!
  \brief QOS configuration set operation flag
*/
#define QOS_STATUS_SET 0x20000000
/*!
  \brief QOS configuration get operation flag
*/
#define QOS_STATUS_GET 0x40000000
/*!
  \brief QOS general configuration shut flag
*/
#define QOS_CFG_SHUT 0x80000000
int32_t qoscfg_main(int32_t argc, char**argv);
