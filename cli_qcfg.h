/******************************************************************************

                         Copyright (c) 2012, 2014, 2015
                        Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

/*defined for Queue CLI from MSB, only 16 Most Significant Bits can be used */
/*!
  \brief QOS queue set operation flag
*/
#define QOS_Q_F_QUEUE 0x10000000
/*!
  \brief Queue get operation flag
*/
#define QOS_Q_F_GET 0x20000000
/*!
  \brief QOS port set operation flag
*/
#define QOS_Q_F_PORT 0x40000000
/*!
  \brief Dump QoS configuration
*/
#define QOS_Q_F_STATS_GET 0x80000000

int32_t qcfg_main(int32_t,char**);
