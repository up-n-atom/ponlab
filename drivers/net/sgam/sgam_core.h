/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) 2022 MaxLinear, Inc.
 */
#ifndef _SGAM_CORE_H_
#define _SGAM_CORE_H_

long sgam_ioctl_handler(struct file *file, unsigned int cmd, unsigned long arg);
int sgam_core_init(void);
int sgam_core_exit(void);

#endif /* _SGAM_CORE_H_ */
