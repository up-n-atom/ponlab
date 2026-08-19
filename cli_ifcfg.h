/******************************************************************************

                         Copyright (c) 2012, 2014, 2015
                        Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

/*defined for Interface CLI from MSB, only 16 Most Significant Bits can be used */
/*!
  \brief Notification flag to indicate port/interface rate change
*/
#define QOS_PORT_F_RT_CHG 0x10000000
/*!
  \brif Interface group mapping get flag
*/
#define CL_IF_ABS_GET 0x20000000
/*!
  \brif Interface group mapping set flag
*/
#define CL_IF_ABS_SET 0x40000000
/*!
  \brif Interface Ingress group mapping get flag
*/
#define CL_IF_INGR_GET 0x80000000

int32_t ifcfg_main(int32_t, char **);
