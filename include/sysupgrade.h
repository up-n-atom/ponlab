/********************************************************************************

  Copyright (C) 2020-2022 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.
********************************************************************************/

#ifndef _SYSUPGRADE_H
#define _SYSUPGRADE_H

/*!
  \brief Sysupgrade's Private options to customize the daemon on run-time.
  */
typedef struct {
    int8_t  nFork;  /*!< fork enable or disable option */
    int8_t  nDumpMsg; /* !< Dump Object messages messages to console or logs */
    int16_t nLogValue; /* !< Sysupgrade's internal logvalue */
} SysupgradeOptions;

void sysupgrade_freeAll(void);
void sysupgrade_reUpdateSid(void);

#endif /* _SYSUPGRADE_H */
