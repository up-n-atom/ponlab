/******************************************************************************

  Copyright © 2020-2023 MaxLinear, Inc. 
  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/




/* ===========================================================================================================
 *
 *       Function Name   : insmod
 **
 **      Description     : API to insert kernel module
 **
 **      Parameter       : Module name to be inserted with parameter of the modules (optional)
 **
 **      Returns         : Returns 0 on success and error number on failure.
 ** ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <asm/unistd.h>
#include <glob.h>

#include <ulogging.h>
#include <ltq_api_include.h>

#pragma GCC diagnostic ignored "-Wpointer-arith"

#ifdef __UCLIBC__
extern int init_module(void *module, unsigned long len, const char *options);
#else
#include <sys/syscall.h>
#define init_module(mod, len, opts) syscall(__NR_init_module, mod, len, opts)
#endif

#define DEFAULT_MODULES_DIR "/lib/modules/*/"
#define DEFAULT_BUF_SIZE 1024

static const char *scapi_moderror(int err)
{
	switch (err) {
		case ENOEXEC:
			return "Invalid module format";
		case ENOENT:
			return "Unknown symbol in module";
		case ESRCH:
			return "Module has wrong symbol version";
		case EINVAL:
			return "Invalid parameters";
		default:
			return strerror(err);
	}
}

static void *scapi_getFileData(const char *pcFileName, unsigned long *pulSize)
{
	unsigned int unMax = 16384;
	int nRet, nFd, err_save;
	void *pBuffer;

	if (!strcmp(pcFileName, "-"))
		nFd = dup(STDIN_FILENO);
	else
		nFd = open(pcFileName, O_RDONLY, 0);

	if (nFd < 0)
		return NULL;

	pBuffer = calloc(1, unMax);
	if (!pBuffer){
		LOGF_LOG_ERROR("ERROR = %d -> %s\n", -errno, strerror(errno));
		goto returnHandler;
	}

	*pulSize = 0;
	while ((nRet = read(nFd, pBuffer + *pulSize, unMax - *pulSize)) > 0) {
		*pulSize += nRet;
		if (*pulSize == unMax) {
			void *p;

			p = realloc(pBuffer, unMax *= 2);
			if (!p){
				LOGF_LOG_ERROR("ERROR = %d -> %s\n", -errno, strerror(errno));
				goto returnHandler;
			}
			pBuffer = p;
		}
	}
	if (nRet < 0){
		LOGF_LOG_ERROR("ERROR = %d -> %s\n", -errno, strerror(errno));
		goto returnHandler;
	}

	close(nFd);
	return pBuffer;

returnHandler:
	err_save = errno;
	free(pBuffer);
	close(nFd);
	errno = err_save;
	return NULL;
}

/* 
 ** =============================================================================
 **   Function Name    : scapi_insmod
 **
 **   Description      : Inserts a kernel module
 **
 **   Parameters       : pcModulePath(IN) --> Full path of the module. Wildcard supported
 **
 **   Return Value     :Success -> EXIT_SUCCESS
 **                     Failure -> Different -ve values depending on the error 
 ** 
 **   Notes            : Compatible with kernel versions > 2.6 
 **
 ** ============================================================================
 */

int scapi_insmod(const char* pcModulePath, const char* pcOptions)
{
	int nRet = -EXIT_FAILURE;
	char *pcModuleName = NULL;
	char *pcOptionsDup = NULL;
	glob_t xPath = {0};
	struct stat xStat = {0};
	char module_buffer[1024] = {'\0'};

	if(pcModulePath == NULL || strnlen_s(pcModulePath,DEFAULT_BUF_SIZE) <= 0){
		nRet = -EINVAL;
		LOGF_LOG_ERROR("ERROR = %d -> %s\n", nRet, strerror(-nRet));
		goto returnHandler;
	}

	/*  Check if '.ko' extension is present for module name. If not,
	 *  add it */
	int nModuleLen = strnlen_s(pcModulePath,DEFAULT_BUF_SIZE);
	int nSuffixLen = sizeof(".ko") - 1;

	if(nModuleLen > nSuffixLen && !strcmp(".ko", &pcModulePath[nModuleLen - 3])){
		pcModuleName = strdup(pcModulePath);
		if(pcModuleName == NULL){
			nRet = -errno;
			LOGF_LOG_ERROR("ERROR = %d -> %s\n", nRet, strerror(-nRet));
			goto returnHandler;
		}
	}
	/* If module len is <= suffix len, ".ko" extension is not
	 * present
	 */
	else{
		int pcModuleNameLen = strnlen_s(pcModulePath, DEFAULT_BUF_SIZE)+ 4*sizeof(char);
		pcModuleName = calloc(1, pcModuleNameLen);
		if(pcModuleName == NULL){
			nRet = -errno;
			LOGF_LOG_ERROR("ERROR = %d -> %s\n", nRet, strerror(-nRet));
			goto returnHandler;
		}
		if (sprintf_s(pcModuleName + strnlen_s(pcModuleName, DEFAULT_BUF_SIZE), pcModuleNameLen, "%s", pcModulePath)<=0) {
			nRet=EXIT_FAILURE;
			LOGF_LOG_ERROR("ERROR = sprintf_s failed -> %s\n", strerror(errno));
			goto returnHandler;
		}
		if (sprintf_s(pcModuleName + strnlen_s(pcModuleName, DEFAULT_BUF_SIZE), pcModuleNameLen, ".ko") <=0 ) {
			nRet=EXIT_FAILURE;
			LOGF_LOG_ERROR("ERROR = sprintf_s failed -> %s\n", strerror(errno));
			goto returnHandler;
		}
	}

	/* If there are no wildcard entries and you are not able to stat the file */
	if (strchr(pcModuleName, '*') == NULL && (stat(pcModuleName, &xStat) != 0)){
		char* pcTmp = calloc(1, strnlen_s(pcModuleName,DEFAULT_BUF_SIZE)+ strnlen_s(DEFAULT_MODULES_DIR,DEFAULT_BUF_SIZE) + (2*sizeof(char)));
		if(pcTmp == NULL){
			nRet = -errno;
			LOGF_LOG_ERROR("ERROR = %d -> %s\n", nRet, strerror(-nRet));
			goto returnHandler;
		}
		/*pcTmp has enough space*/
		if (sprintf_s(pcTmp, (strnlen_s(pcModuleName,DEFAULT_BUF_SIZE)+strnlen_s(DEFAULT_MODULES_DIR,DEFAULT_BUF_SIZE)+(2*sizeof(char))), "%s%s", DEFAULT_MODULES_DIR, pcModuleName) <=0 ) 
		{
			nRet=EXIT_FAILURE;
			LOGF_LOG_ERROR("ERROR = sprintf_s failed -> %s\n", strerror(errno));
			free(pcTmp);
			goto returnHandler;
		}
		free(pcModuleName);
		pcModuleName = pcTmp;
	}

	glob(pcModuleName, 0, NULL, &xPath);

	if(xPath.gl_pathv == NULL){
		nRet = -EINVAL;
		LOGF_LOG_ERROR("ERROR = %d -> %s\n", nRet, strerror(-nRet));
		goto returnHandler;
	}
	else if(*(xPath.gl_pathv) != NULL){
		free(pcModuleName);
		pcModuleName = calloc(1, strnlen_s(*(xPath.gl_pathv),DEFAULT_BUF_SIZE) + 2*sizeof(char));
		if(pcModuleName == NULL){
			nRet = -errno;
			LOGF_LOG_ERROR("ERROR = %d -> %s\n", nRet, strerror(-nRet));
			goto returnHandler;
		}
		/* pcModuleName is guaranteed to be bigger than source*/
		if (sprintf_s(pcModuleName, (strnlen_s(*(xPath.gl_pathv),DEFAULT_BUF_SIZE) + 2*sizeof(char)), "%s", *(xPath.gl_pathv)) <= 0) {
			nRet=EXIT_FAILURE;
			LOGF_LOG_ERROR("ERROR = sprintf_s failed -> %s\n", strerror(errno));
			goto returnHandler;
		}
	}
	if(pcOptions == NULL){
		pcOptionsDup = pcModuleName;
	}
	else{
		int pcOptionsDupLen = strnlen_s(pcModuleName,DEFAULT_BUF_SIZE) + strnlen_s(pcOptions,DEFAULT_BUF_SIZE) + 3*sizeof(char);
		pcOptionsDup = calloc(1, pcOptionsDupLen);
		if(pcOptionsDup == NULL){
			nRet = -errno;
			LOGF_LOG_ERROR("ERROR = %d -> %s\n", nRet, strerror(-nRet));
			goto returnHandler;
		}
		if (sprintf_s(pcOptionsDup + strnlen_s(pcOptionsDup, pcOptionsDupLen), pcOptionsDupLen, "%s", pcModuleName)<=0) {
			nRet=EXIT_FAILURE;
			LOGF_LOG_ERROR("ERROR = sprintf_s failed -> %s\n", strerror(errno));
			goto returnHandler;
		}
		if (sprintf_s(pcOptionsDup + strnlen_s(pcOptionsDup, pcOptionsDupLen), pcOptionsDupLen, " ") <= 0) {
			nRet=EXIT_FAILURE;
			LOGF_LOG_ERROR("ERROR = sprintf_s failed -> %s\n", strerror(errno));
			goto returnHandler;
		}
		if (sprintf_s(pcOptionsDup + strnlen_s(pcOptionsDup, pcOptionsDupLen), pcOptionsDupLen, "%s", pcOptions) <=0 ) {
			nRet=EXIT_FAILURE;
			LOGF_LOG_ERROR("ERROR = sprintf_s failed -> %s\n", strerror(errno));
			goto returnHandler;
		}
	}

	/* Insmoding using system command */
	if (sprintf_s(module_buffer, 1024, "insmod %s", pcOptionsDup)<=0) {
			nRet=EXIT_FAILURE;
			LOGF_LOG_ERROR("ERROR = sprintf_s failed -> %s\n", strerror(errno));
			goto returnHandler;
	}

	nRet = system(module_buffer);
	if (nRet) {
		LOGF_LOG_WARNING("Execute command -> '%s', returned=%d with %s\n",
				 module_buffer, nRet, strerror(errno));
	}

	if (pcOptions == NULL) {
		pcOptionsDup = NULL;
	}

        nRet = EXIT_SUCCESS;

returnHandler:
	if(pcModuleName != NULL)
		free(pcModuleName);
	if(pcOptionsDup != NULL)
		free(pcOptionsDup);
	globfree(&xPath);
	return nRet;
}
