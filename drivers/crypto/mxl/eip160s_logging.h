/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Driver for MACsec Engine inside Intel CHD GSWIP or GPHY
 * Copyright 2020 - 2022 Maxlinear, Inc.
 * Copyright 2019 - 2020 Intel Corporation
 */

#ifndef _EIP160S_LOGGING_H_
#define _EIP160S_LOGGING_H_

#ifdef __KERNEL__
#include <linux/kernel.h>
#define PR_CRIT		pr_crit
#define PR_ERR		pr_err
#define PR_WARN		pr_warning
#define PR_INFO		pr_info
#else
#include <stdio.h>
#define PR_CRIT		printf
#define PR_ERR		printf
#define PR_WARN		printf
#define PR_INFO		printf
#endif

#define LOG_SEVERITY_NILL 0
#define LOG_SEVERITY_CRIT 1
#define LOG_SEVERITY_ERR  2
#define LOG_SEVERITY_WARN 3
#define LOG_SEVERITY_INFO 4

#define LOG_SEVERITY_MAX LOG_SEVERITY_WARN

#ifndef LOG_SEVERITY_MAX
#define LOG_SEVERITY_MAX LOG_SEVERITY_CRIT
#endif

#if LOG_SEVERITY_MAX >= LOG_SEVERITY_CRIT
#define LOG_CRIT_ENABLED
#endif

#if LOG_SEVERITY_MAX >= LOG_SEVERITY_ERR
#define LOG_ERR_ENABLED
#endif

#if LOG_SEVERITY_MAX >= LOG_SEVERITY_WARN
#define LOG_WARN_ENABLED
#endif

#if LOG_SEVERITY_MAX >= LOG_SEVERITY_INFO
#define LOG_INFO_ENABLED
#endif

#undef LOG_CRIT
#ifdef LOG_CRIT_ENABLED
#define LOG_CRIT	PR_CRIT
#else
#define LOG_CRIT(...)
#endif

#undef LOG_ERR
#ifdef LOG_ERR_ENABLED
#define LOG_ERR	PR_ERR
#else
#define LOG_ERR(...)
#endif

#undef LOG_WARN
#ifdef LOG_WARN_ENABLED
#define LOG_WARN	PR_WARN
#else
#define LOG_WARN(...)
#endif

#undef LOG_INFO
#ifdef LOG_INFO_ENABLED
#define LOG_INFO	PR_INFO
#else
#define LOG_INFO(...)
#endif

#endif /* _EIP160S_LOGGING_H_ */

