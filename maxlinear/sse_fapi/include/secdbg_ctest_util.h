/*******************************************************************************

  Copyright © 2022 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/*  *****************************************************************************
 *         File Name    : secdbg_ctest_util.h									*
 *         Description  : API declarations for ctest application of secure debug*
 *  *****************************************************************************/

#include <stdio.h>
#include <stdbool.h>

#ifndef __SECDBG_CTEST_UTIL
#define __SECDBG_CTEST_UTIL

/*
 * @brief function to display the sst access policy
 * @param
 *  pcxObjConfig pointer to sst object config strcuture
 * @return
 *  void
 */
void display_object_config(sst_obj_config_t *pcxObjConfig);

/*
 * @brief function to fill up the sst object config structure
 * @param
 *  pcxObjConfig pointer to sst object config strcuture
 *  unAccessFlag flag used to fill params of sst_obj_config_t strcuture
 * @return
 *  void
 */
void set_object_config(sst_obj_config_t *pcxObjConfig, uint32_t unAccessFlag);

/*
 * @brief backup and restore authkey file
 * @param
 *  nPort port number
 *  bBackup flag to backup or not
 * @return
 *  returns 0 on success or -ve on failure
 */
int ctest_backupnrestore_authkey_file(int nPort, bool nBackup);

/*
 * @brief backup and restore private keypair file
 * @param
 *  nPort port number
 *  bBackup flag to backup or not
 * @return
 *  returns 0 on success or -ve on failure
 */
int ctest_backupnrestore_keypair_file(int nPort, bool nBackup);

/*
 * @brief rename keypair file
 * @param
 *  nPort port number
 *  bRename flag to rename or not
 * @return
 *  returns 0 on success or -ve on failure
 */
int ctest_rename_keypair_file(int nPort, bool nRename);

/*
 * @brief rename wrapped authkey file
 * @param
 *  nPort port number
 *  bRename flag to rename or not
 * @return
 *  returns 0 on success or -ve on failure
 */
int ctest_rename_authkey_file(int nPort, bool nRename);

/*
 * @brief removes the backup files
 * @param
 *  nPort port number
 * @return
 *  void
 */
void ctest_remove_backup_files(int nPort);
#endif
