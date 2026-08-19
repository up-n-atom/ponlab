/* Copyright 2017, Intel Corporation */
/******************************************************************************

                          Copyright (c) 2015-2018
                     Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/
#include "dsl_fapi_config.h"
#include "dsl_fapi_common.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <asm/ioctl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include "dsl_fapi_debug.h"
#ifdef UGW_VENDOR_PATH
#include "vendor.h"
#endif

#ifdef LOGGING_ID
#include "ulogging.h"

#ifndef LOG_LEVEL
uint16_t LOGLEVEL = SYS_LOG_DEBUG + 1;
#else
uint16_t LOGLEVEL = LOG_LEVEL + 1;
#endif

#ifndef LOG_TYPE
uint16_t LOGTYPE = SYS_LOG_TYPE_FILE;
#else
uint16_t LOGTYPE = LOG_TYPE;
#endif
#else				/* LOGGING_ID */
#define LOGF_LOG_CRITICAL printf
#define LOGF_LOG_ERROR    printf
#define LOGF_LOG_INFO     printf
#define LOGF_LOG_DEBUG    printf
#endif				/* LOGGING_ID */

#define FAPI_DEBUG 0

/* what string definition */
static const char *dsl_fapi_version = "@(#)" PACKAGE_VERSION;

static const key_t fapi_shared_mem_key = 5882;

#ifdef VENDOR_PATH
#define CPE_SCPIRT_PATH VENDOR_PATH"/etc/init.d/"
#else
#define CPE_SCPIRT_PATH "/opt/lantiq/etc/init.d/"
#endif /* VENDOR_PATH */

struct dsl_fapi_x_lantiq_com_config_obj x_lantiq_com_config_obj = { 0 };

void fapi_dsl_log_set(int16_t log_level, int16_t log_type)
{
#ifdef LOGGING_ID
	LOGLEVEL = log_level;
	LOGTYPE = log_type;
	LOGF_LOG_INFO("new loglevel = %d ; new logtype = %d \n", LOGLEVEL, LOGTYPE);
#endif
	return;
}

/* Adapted from libscapi package */
static FILE * dsl_getFilePtr(const char *fname, char *mode)
{
    FILE *file;
    if ((file = fopen(fname, mode)))
    {
        return file;
    }
    return NULL;
}

/* Adapted from libscapi package */
char process_name[16]; /*  To BSS */
#define FAIL "INVAL PROCESS"
static char *dsl_get_process_name(IN pid_t process_num)
{
	char cBuff[128] = {0};
	snprintf(cBuff, sizeof(cBuff), "/proc/%d/stat", (int)process_num);
	char* start = NULL, *end = NULL;
	char* ret = NULL;
	FILE *file;


	FILE* process_stat_file = dsl_getFilePtr(cBuff, "r");
	if(NULL == process_stat_file)
	{
		ret = FAIL; /* String literals are stored in data/code memory of the process. So no prob */
		goto returnHandler;
	}

	/*  Using the same cBuff for reading line. Reading one line is enough as process
	 *  name will definetely be in first line */
	if(NULL == fgets(cBuff, sizeof(cBuff), process_stat_file))
	{
		ret = FAIL;
		goto returnHandler;
	}
	if(NULL == (start = strchr(cBuff, '(') ))
	{
		ret = FAIL;
		goto returnHandler;
	}

	++start;

	if(NULL == (end = strchr(cBuff, ')') ))
	{
		ret = FAIL;
		goto returnHandler;
	}
	/*  null terminating after our string of interest */
	*end = '\0';
	memset(process_name, 0, sizeof(process_name));
	snprintf(process_name, sizeof(process_name), "%s", start);
	ret = process_name;

returnHandler:
	if(process_stat_file != NULL)
		fclose(process_stat_file);
	return ret;
}

/* Adapted from libscapi package */
static int dsl_spawn(char *pcBuf, int nBlockingFlag, int* pnChildExitStatus)
{
	int nStatus = 0;
	int nRet = 0;
	pid_t pid = -1;
	char sLDLibPath[255]={0};
	char *envp[1];

	snprintf(sLDLibPath, sizeof(sLDLibPath), "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:%s/usr/lib:%s/lib", VENDOR_PATH, VENDOR_PATH);
	envp[0] = sLDLibPath;


	if(pnChildExitStatus == NULL || pcBuf == NULL)
	{
		nRet = -EINVAL;
		LOGF_LOG_ERROR("ERROR = %d\n", nRet);
		goto returnHandler;
	}
	*pnChildExitStatus = 0;
	pid = fork();
	switch(pid)
	{
		case -1:
			nRet = -errno;                           //can't fork
			LOGF_LOG_ERROR( "ERROR = %d -> %s, PROCESS = %s\n", nRet, strerror(-nRet), dsl_get_process_name(getpid()));
			goto returnHandler;
			break;
		case 0: //child
			/* Doesn't return on success. Text, stack, heap, data segments of the forked process over written. use vfork() instead??*/
			nRet = execl("/bin/sh","sh","-c", pcBuf, envp,NULL);
			if(-1 == nRet) {
				nRet = -errno;
				LOGF_LOG_ERROR("ERROR = %d\n", nRet);
				goto returnHandler;
			}
			break;
		default: //parent
			if(nBlockingFlag == 1) //block parent till child exits
			{
				/*After this call 'nStatus' is an encoded exit value. WIF macros will extract how it exited*/
				if( waitpid(pid, &nStatus, 0) < 0 )
				{
					nRet = -errno;
					*pnChildExitStatus = errno;
					LOGF_LOG_ERROR("ERROR = %d\n", nRet);
				}
				//use only exit with +ve values in child as WEXITSTATUS() macro will only consider LSB 8 bits
				if(WIFEXITED(nStatus))
				{
					nRet = EXIT_SUCCESS;
					*pnChildExitStatus = WEXITSTATUS(nStatus);
				}
				goto returnHandler;
			}
			break;
	}
returnHandler:
	return nRet;
}

DLL_LOCAL enum fapi_dsl_status shared_mem_create(key_t key)
{
	int shmid;
	struct fapi_shmem shmem_ctx = { 0 };
	struct fapi_global_vars global_vars = { 0 };
	/** try to open first */
	if ((shmid = shmget(key, sizeof(struct fapi_shmem), IPC_CREAT | IPC_EXCL | 0666)) < 0) {
		if (errno == EEXIST) {
			/** memory is available */
			return DSL_SUCCESS;
		}
		LOGF_LOG_CRITICAL("DSL FAPI: fail to create shared memory (errno - %d)\n", errno);
		return FAPI_DSL_STATUS_ERROR;
	}
	/** memory is created and filled with 0's */
	return FAPI_DSL_STATUS_SUCCESS;
}

DLL_LOCAL bool shared_mem_test(key_t key)
{
	int shmid;

	if ((shmid = shmget(key, sizeof(struct fapi_shmem), 0666)) < 0) {
		return false;
	}

	return true;
}

DLL_LOCAL enum fapi_dsl_status shared_mem_delete(key_t key)
{
	int shmid;

	if ((shmid = shmget(key, sizeof(struct fapi_shmem), 0666)) < 0) {
		LOGF_LOG_CRITICAL("DSL FAPI: fail to open shared memory\n");
		return FAPI_DSL_STATUS_ERROR;
	}

	if (shmctl(shmid, IPC_RMID, NULL) < 0) {
		LOGF_LOG_CRITICAL("DSL FAPI: fail to delete shared memory\n");
		return FAPI_DSL_STATUS_ERROR;
	}

	return FAPI_DSL_STATUS_SUCCESS;
}

DLL_LOCAL enum fapi_dsl_status shared_mem_global_vars_get(key_t key, struct fapi_global_vars *global_vars)
{
	int shmid;
	struct fapi_shmem *shmem_ctx = NULL;

	if ((shmid = shmget(key, sizeof(struct fapi_shmem), 0666)) < 0) {
		LOGF_LOG_CRITICAL("DSL FAPI: fail to open shared memory\n");
		return FAPI_DSL_STATUS_ERROR;
	}

	if ((shmem_ctx = shmat(shmid, NULL, 0)) == (void *)-1) {
		LOGF_LOG_CRITICAL("DSL FAPI: fail to attach shared memory\n");
		return FAPI_DSL_STATUS_ERROR;
	}

	/* load from shared memory */
	fapi_dsl_memcpy_s(global_vars, sizeof(struct fapi_global_vars),
			  &shmem_ctx->global_vars, sizeof(struct fapi_global_vars));

	if (shmdt(shmem_ctx) == -1) {
		LOGF_LOG_CRITICAL("DSL FAPI: fail to detache shared memory\n");
		return FAPI_DSL_STATUS_ERROR;
	}

	return FAPI_DSL_STATUS_SUCCESS;
}

DLL_LOCAL enum fapi_dsl_status shared_mem_global_vars_set(key_t key, struct fapi_global_vars *global_vars)
{
	int shmid;
	struct fapi_shmem *shmem_ctx = NULL;

	if ((shmid = shmget(key, sizeof(struct fapi_shmem), 0666)) < 0) {
		LOGF_LOG_CRITICAL("DSL FAPI: fail to open shared memory\n");
		return FAPI_DSL_STATUS_ERROR;
	}

	if ((shmem_ctx = shmat(shmid, NULL, 0)) == (void *)-1) {
		LOGF_LOG_CRITICAL("DSL FAPI: fail to attach shared memory\n");
		return FAPI_DSL_STATUS_ERROR;
	}

	/* store to shared memory */
	fapi_dsl_memcpy_s(&shmem_ctx->global_vars, sizeof(struct fapi_global_vars),
			  global_vars, sizeof(struct fapi_global_vars));

	if (shmdt(shmem_ctx) == -1) {
		LOGF_LOG_CRITICAL("DSL FAPI: fail to detache shared memory\n");
		return FAPI_DSL_STATUS_ERROR;
	}

	return FAPI_DSL_STATUS_SUCCESS;
}

DLL_LOCAL enum fapi_dsl_status shared_mem_fapi_ctx_save(key_t key, struct fapi_dsl_ctx *ctx)
{
	enum fapi_dsl_status ret = FAPI_DSL_STATUS_SUCCESS;
	int shmid;
	struct fapi_shmem *shmem_ctx = NULL;
	int i;

	if (ctx == NULL) {
		LOGF_LOG_CRITICAL("DSL FAPI: internal error (NULL ptr)\n");
		return FAPI_DSL_STATUS_ERROR;
	}

	if ((shmid = shmget(key, sizeof(struct fapi_shmem), 0666)) < 0) {
		LOGF_LOG_CRITICAL("DSL FAPI: fail to open shared memory\n");
		return FAPI_DSL_STATUS_ERROR;
	}

	if ((shmem_ctx = shmat(shmid, NULL, 0)) == (void *)-1) {
		LOGF_LOG_CRITICAL("DSL FAPI: fail to attach shared memory\n");
		return FAPI_DSL_STATUS_ERROR;
	}

	if (ctx->entity >= shmem_ctx->global_vars.device_count) {
		LOGF_LOG_CRITICAL("DSL FAPI: invalid context entity (%d)\n", ctx->entity);
		ret = FAPI_DSL_STATUS_ERROR;
	}
	fapi_dsl_memcpy_s(&shmem_ctx->data[ctx->entity].ctx, sizeof(struct fapi_dsl_ctx),
			  ctx, sizeof(struct fapi_dsl_ctx));

	if (shmdt(shmem_ctx) == -1) {
		LOGF_LOG_CRITICAL("DSL FAPI: fail to detache shared memory\n");
		return FAPI_DSL_STATUS_ERROR;
	}

	return ret;
}

DLL_LOCAL int fapi_dsl_open_device(unsigned int entity)
{
	int fd;
	char dev_name[128];
	int dev_num;

	snprintf(dev_name, sizeof(dev_name), "%s/%d", "/dev/dsl_cpe_api", entity);
	fd = open(dev_name, O_RDWR, 0644);

	if (fd < 0) {
		LOGF_LOG_CRITICAL("DSL FAPI: fail to open device %s (errno:%d)\n", dev_name, errno);
	}
	fcntl(fd, F_SETFD, FD_CLOEXEC);

	return fd;
}

DLL_LOCAL int fapi_dsl_close_device(int fd)
{
	return close(fd);
}

DLL_LOCAL int fapi_dsl_open_mei_device(unsigned int entity)
{
	int fd;
	char dev_name[128];
	int dev_num;
	snprintf(dev_name, sizeof(dev_name), "/dev/mei_cpe/%u", entity);
	fd = open(dev_name, O_RDWR, 0644);

	if (fd < 0) {
		LOGF_LOG_CRITICAL("DSL FAPI: fail to open mei device %s (errno:%d)\n", dev_name, errno);
	}
	fcntl(fd, F_SETFD, FD_CLOEXEC);

	return fd;
}

#define MAX_COMMAND_LEN     256
DLL_PUBLIC struct fapi_dsl_ctx *fapi_dsl_open(unsigned int entity)
{
	int ret, i, fd;
	struct fapi_global_vars global_vars = { 0 };
	int drv_load_flag = 0;
	struct fapi_dsl_ctx *ctx = NULL;
	char pCommand[MAX_COMMAND_LEN] = { 0 };
	int nBlockingFlag = 1;
	int nExitStatus = 0;

	if (entity > DSL_CPE_FAPI_MAX_SUPPORTED_ENTITIES - 1) {
		LOGF_LOG_CRITICAL("DSL FAPI: entity %d is not supported\n", entity);
		return NULL;
	}
    
	if(shared_mem_test(fapi_shared_mem_key) == false) {
		shared_mem_create(fapi_shared_mem_key);
	}

	/* read global variables from shared memory */
	shared_mem_global_vars_get(fapi_shared_mem_key, &global_vars);

	LOGF_LOG_DEBUG("DSL FAPI: open context drv_load_cnt %d reboot_needed %d\n",
                  global_vars.drv_load_cnt, global_vars.reboot_needed);

	if (!global_vars.reboot_needed) {
		if (global_vars.drv_load_cnt++ == 0) {
			struct fapi_dsl_ctx empty_ctx = { 0 };

/* DSL drivers are autoloaded. So we donot need to load from here.
			ret = fapi_sys_load(DSL_xTM);
			if (ret != UGW_SUCCESS) {
				LOGF_LOG_CRITICAL("DSL FAPI: fail to load PP driver\n");
			}
*/

			LOGF_LOG_INFO("DSL FAPI: load MEI driver\n");
			snprintf(&pCommand[0], MAX_COMMAND_LEN, CPE_SCPIRT_PATH "ltq_load_cpe_mei_drv.sh start");
			ret = dsl_spawn(&pCommand[0], nBlockingFlag, &nExitStatus);
			if (ret != 0) {
				LOGF_LOG_CRITICAL("DSL FAPI: fail to start dsl cpe mei driver\n");
				return NULL;
			}
			/* get number of available devices */
			else {
				IOCTL_MEI_devinfo_t devinfo;
				int mei_fd = fapi_dsl_open_mei_device(entity);
				if (mei_fd < 0) {
					LOGF_LOG_CRITICAL("DSL FAPI: Cannot determine number of devices \n");
					return NULL;
				}
				ret = ioctl(mei_fd, FIO_MEI_DRV_DEVINFO_GET, &devinfo);
				if (!ret) {
					close(mei_fd);
					global_vars.device_count = devinfo.maxDeviceNumber * devinfo.linesPerDevice;
					if (global_vars.device_count > DSL_CPE_FAPI_MAX_SUPPORTED_ENTITIES) {
						LOGF_LOG_CRITICAL
							 ("DSL FAPI: invalid number of available devices (%d)\n",
							  devinfo.maxDeviceNumber * devinfo.linesPerDevice);
						return NULL;
					} else if (entity >= global_vars.device_count) {
						LOGF_LOG_CRITICAL("DSL FAPI: entity %d does not exists\n", entity);
						return NULL;
					} else {
						shared_mem_global_vars_set(fapi_shared_mem_key, &global_vars);
					}
				} else {
					close(mei_fd);
					LOGF_LOG_CRITICAL("DSL FAPI: cannot get number of available devices\n");
					return NULL;
				}
			}

			LOGF_LOG_INFO("DSL FAPI: load DSL driver\n");
			memset(pCommand, 0, MAX_COMMAND_LEN);
			snprintf(&pCommand[0], MAX_COMMAND_LEN, CPE_SCPIRT_PATH "ltq_load_dsl_cpe_api.sh start");
			ret = dsl_spawn(&pCommand[0], nBlockingFlag, &nExitStatus);

			if (ret != 0) {
				LOGF_LOG_CRITICAL("DSL FAPI: fail to start dsl api cpe driver\n");
				return NULL;
			}

			/** reset contexts */
			for (i = 0; i < global_vars.device_count; i++) {
				empty_ctx.entity = i;
				shared_mem_fapi_ctx_save(fapi_shared_mem_key, &empty_ctx);
			}
		} else {
			if (entity >= global_vars.device_count) {
				LOGF_LOG_CRITICAL("DSL FAPI: entity %d does not exists\n", entity);
				return NULL;
			}
#if defined(FAPI_DEBUG) && (FAPI_DEBUG == 1)
			LOGF_LOG_DEBUG("DSL FAPI: DSL & MEI drivers already loaded\n");
#endif
		}
		/** test if driver is running */
		fd = fapi_dsl_open_device(entity);
		if (fd < 0) {
			if (--global_vars.drv_load_cnt == 0) {
				LOGF_LOG_INFO("DSL FAPI: unload DSL driver\n");
				/* stop DSL CPE & MEI CPE drivers */
				memset(pCommand, 0, MAX_COMMAND_LEN);
				snprintf(&pCommand[0], MAX_COMMAND_LEN,
							CPE_SCPIRT_PATH "ltq_load_dsl_cpe_api.sh stop");
				ret = dsl_spawn(&pCommand[0], nBlockingFlag, &nExitStatus);
				if (ret != 0) {
					LOGF_LOG_ERROR("DSL FAPI: fail to stop dsl api cpe driver\n");
				}

				sleep(2);
				LOGF_LOG_INFO("DSL FAPI: unload MEI driver\n");
				memset(pCommand, 0, MAX_COMMAND_LEN);
				snprintf(&pCommand[0], MAX_COMMAND_LEN,
							CPE_SCPIRT_PATH "ltq_load_cpe_mei_drv.sh stop");
				ret = dsl_spawn(&pCommand[0], nBlockingFlag, &nExitStatus);
				if (ret != 0) {
					LOGF_LOG_ERROR("DSL FAPI: fail to stop dsl cpe mei driver\n");
				}

				/* delete shared memory */
				shared_mem_delete(fapi_shared_mem_key);
			}
		} else {
			fapi_dsl_close_device(fd);
		}
		/* save global variables to shared memory */
		shared_mem_global_vars_set(fapi_shared_mem_key, &global_vars);
	}

	ctx = fapi_dsl_context_get(entity);

#if defined(FAPI_DEBUG) && (FAPI_DEBUG == 1)
	LOGF_LOG_DEBUG("DSL FAPI: open context %p\n", ctx);
#endif

	return ctx;
}

DLL_PUBLIC struct fapi_dsl_ctx *fapi_dsl_context_get(int nEntity)
{
	struct fapi_dsl_ctx *ctx = NULL;
	int shmid;
	key_t key = fapi_shared_mem_key;
	struct fapi_shmem *shmem_ctx = NULL;

	if ((shmid = shmget(key, sizeof(struct fapi_shmem), 0666)) < 0) {
		LOGF_LOG_CRITICAL("DSL FAPI: fail to open shared memory\n");
		return NULL;
	}

	if ((shmem_ctx = shmat(shmid, NULL, 0)) == (void *)-1) {
		LOGF_LOG_CRITICAL("DSL FAPI: fail to attach shared memory\n");
		return NULL;
	}

	/* load from shared memory */

	if (nEntity >= shmem_ctx->global_vars.device_count) {
		LOGF_LOG_CRITICAL("DSL FAPI: invalid context entity (%d)\n", nEntity);
		return NULL;
	} else {
		ctx = malloc(sizeof(struct fapi_dsl_ctx));
		if (ctx == NULL) {
			LOGF_LOG_CRITICAL("DSL FAPI: fail to allocate memory\n");
			return NULL;
		}
		fapi_dsl_memcpy_s(ctx, sizeof(struct fapi_dsl_ctx),
				  &shmem_ctx->data[nEntity].ctx, sizeof(struct fapi_dsl_ctx));
	}

	if (shmdt(shmem_ctx) == -1) {
		LOGF_LOG_CRITICAL("DSL FAPI: fail to detache shared memory\n");
		free(ctx);
		return NULL;
	}

	return ctx;
}

DLL_PUBLIC void fapi_dsl_context_free(struct fapi_dsl_ctx *ctx)
{
	free(ctx);
	return;
}

DLL_PUBLIC enum fapi_dsl_status fapi_dsl_close(struct fapi_dsl_ctx *ctx)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;
	int ret;
	struct fapi_global_vars global_vars = { 0 };
	char pCommand[MAX_COMMAND_LEN] = { 0 };
	int nBlockingFlag = 1;
	int nExitStatus = 0;

	if (ctx == NULL)
		return FAPI_DSL_STATUS_ERROR;

	/* read global variables from shared memory */
	shared_mem_global_vars_get(fapi_shared_mem_key, &global_vars);

	LOGF_LOG_DEBUG("DSL FAPI: close context %p, drv_load_cnt %d reboot_needed %d\n",
						ctx, global_vars.drv_load_cnt, global_vars.reboot_needed);

	fapi_dsl_context_free(ctx);

	if (!global_vars.reboot_needed) {
		LOGF_LOG_INFO("DSL FAPI: call to unload DSL driver\n");
		/* stop DSL CPE & MEI CPE drivers */
		memset(pCommand, 0, MAX_COMMAND_LEN);
		snprintf(&pCommand[0], MAX_COMMAND_LEN, CPE_SCPIRT_PATH "ltq_load_dsl_cpe_api.sh stop");
		ret = dsl_spawn(&pCommand[0], nBlockingFlag, &nExitStatus);

		if (ret != 0) {
			LOGF_LOG_CRITICAL("DSL FAPI: fail to stop dsl api cpe driver\n");
			fapi_status = FAPI_DSL_STATUS_ERROR;
		}

		sleep(2);
		LOGF_LOG_INFO("DSL FAPI: call to unload MEI driver\n");
		memset(pCommand, 0, MAX_COMMAND_LEN);
		snprintf(&pCommand[0], MAX_COMMAND_LEN, CPE_SCPIRT_PATH "ltq_load_cpe_mei_drv.sh stop");
		ret = dsl_spawn(&pCommand[0], nBlockingFlag, &nExitStatus);
		if (ret != 0) {
			LOGF_LOG_CRITICAL("DSL FAPI: fail to stop dsl cpe mei driver\n");
			fapi_status = FAPI_DSL_STATUS_ERROR;
		}

		if (--global_vars.drv_load_cnt == 0) {
			global_vars.reboot_needed = true;
		}

		/* save global variables */
		shared_mem_global_vars_set(fapi_shared_mem_key, &global_vars);
	}

	return fapi_status;
}

DLL_PUBLIC enum fapi_dsl_status fapi_dsl_init(struct fapi_dsl_ctx *ctx, const struct fapi_dsl_init_cfg *cfg)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;
	WAN_TYPE_t tc_mode = DSL_PTM;
	struct fapi_global_vars global_vars = { 0 };

	char pCommand[MAX_COMMAND_LEN] = { 0 };
	int ret, nBlockingFlag = 0, nExitStatus = 0;

#if defined(FAPI_DEBUG) && (FAPI_DEBUG == 1)
	LOGF_LOG_DEBUG("DSL FAPI: init ctx %p\n", ctx);
#endif

	/* print what string */
	printf("Lantiq DSL functional API: DSL FAPI version %s\n", &dsl_fapi_version[4]);

	/* read global variables */
	shared_mem_global_vars_get(fapi_shared_mem_key, &global_vars);

	if (global_vars.appl_start_cnt++ == 0) {

		fapi_dsl_update_web_config(cfg, &tc_mode);

/* DSL drivers are autoloaded. So we donot need to load from here.
		ret = fapi_sys_load(tc_mode);
		if (ret != UGW_SUCCESS) {
			LOGF_LOG_CRITICAL("sys fapi get failed\n");
		}
*/
		LOGF_LOG_INFO("DSL FAPI: start control application\n");
		/* start DSL CPE control application */

		memset(pCommand, 0, MAX_COMMAND_LEN);
		snprintf(&pCommand[0], MAX_COMMAND_LEN, CPE_SCPIRT_PATH "ltq_cpe_control_init.sh start");
		ret = dsl_spawn(&pCommand[0], nBlockingFlag, &nExitStatus);

		if (ret != 0) {
			LOGF_LOG_CRITICAL("DSL FAPI: fail to start dsl cpe control application\n");
			fapi_status = FAPI_DSL_STATUS_ERROR;
		}
	}
#if defined(FAPI_DEBUG) && (FAPI_DEBUG == 1)
	else
		LOGF_LOG_DEBUG("DSL FAPI: control application already started\n");
#endif

	/* save global variables */
	shared_mem_global_vars_set(fapi_shared_mem_key, &global_vars);

	return fapi_status;
}

DLL_PUBLIC enum fapi_dsl_status fapi_dsl_uninit(struct fapi_dsl_ctx *ctx)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;
	int ret;
	struct fapi_global_vars global_vars = { 0 };
	char pCommand[MAX_COMMAND_LEN] = { 0 };
	int nBlockingFlag = 1;
	int nExitStatus = 0;

#if defined(FAPI_DEBUG) && (FAPI_DEBUG == 1)
	LOGF_LOG_DEBUG("DSL FAPI: uninit ctx %p\n", ctx);
#endif

	LOGF_LOG_INFO("DSL FAPI: call to stop control application\n");
	/* stop DSL CPE control application */
	memset(pCommand, 0, MAX_COMMAND_LEN);
	snprintf(&pCommand[0], MAX_COMMAND_LEN, CPE_SCPIRT_PATH "ltq_cpe_control_init.sh stop");
	ret = dsl_spawn(&pCommand[0], nBlockingFlag, &nExitStatus);

	if (ret != 0) {
		LOGF_LOG_CRITICAL("DSL FAPI: fail to stop dsl cpe control application\n");
		fapi_status = FAPI_DSL_STATUS_ERROR;
	}

	/* read global variables */
	shared_mem_global_vars_get(fapi_shared_mem_key, &global_vars);

	--global_vars.appl_start_cnt;

	sleep(5);
/* DSL drivers are autoloaded. So we donot need to unload from here.
	ret = fapi_sys_unload(0);
	if (ret != UGW_SUCCESS) {
		LOGF_LOG_CRITICAL("unload drivers failed\n");
	} else {
		LOGF_LOG_DEBUG("unload drivers successful\n");
	}
*/

	/* save global variables */
	shared_mem_global_vars_set(fapi_shared_mem_key, &global_vars);

	return fapi_status;
}

DLL_PUBLIC enum fapi_dsl_status fapi_dsl_line_get(struct fapi_dsl_ctx *ctx, struct dsl_fapi_line_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;
	union dsl_driver_data_types {
		DSL_AutobootStatus_t autoboot_status;
		DSL_LineState_t line_state;
		DSL_VersionInformation_t fw_vers_info;
		DSL_G997_XTUSystemEnabling_t xtu_config;
		DSL_BandPlanSupport_t profiles_support;
		DSL_BandPlanStatus_t profiles_status;
		DSL_G997_PowerManagementStatus_t power_status;
		DSL_G997_LineInitStatus_t init_status;
		DSL_G997_LastStateTransmitted_t last_state;
		DSL_LineFeature_t line_feature;
		DSL_G997_LineStatus_t line_status;
		DSL_G997_LineStatusPerBand_t band_status;
		DSL_G997_LineInventory_t line_inventory;
		DSL_G997_UsPowerBackOffStatus_t us_power_back_off_status;
		DSL_G997_RateAdaptationStatus_t rate_adaptation_status;
		DSL_T1413RevisionStatus_t t1413_revision_status;
		DSL_T1413VendorRevisionStatus_t t1413_vendor_revision_status;
	} dsl_driver_data;
	unsigned int offset, buf_len;
	unsigned int octet, bits, i, j;
	char *name;
	char buf[32];
	int ret, fd;

	if (obj == NULL)
		return FAPI_DSL_STATUS_ERROR;

	if (ctx == NULL)
		return FAPI_DSL_STATUS_ERROR;

	fd = fapi_dsl_open_device(ctx->entity);

	if (fd < 0)
		return FAPI_DSL_STATUS_ERROR;

	memset(obj, 0x00, sizeof(struct dsl_fapi_line_obj));

	/* Device.DSL.Line.Enable */
	memset(&dsl_driver_data.autoboot_status, 0x00, sizeof(DSL_AutobootStatus_t));

	ret = ioctl(fd, DSL_FIO_AUTOBOOT_STATUS_GET, (int)&dsl_driver_data.autoboot_status);

	if ((ret < 0) && (dsl_driver_data.autoboot_status.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.Enable: error code %d\n",
			       dsl_driver_data.autoboot_status.accessCtl.nReturn);
	} else {
		switch (dsl_driver_data.autoboot_status.data.nStatus) {
		case DSL_AUTOBOOT_STATUS_DISABLED:
		case DSL_AUTOBOOT_STATUS_STOPPED:
			obj->enable = false;
			break;

		default:
			obj->enable = true;
			break;
		}
	}

	/* Device.DSL.Line.Status,  Device.DSL.Line.LinkStatus */
	memset(&dsl_driver_data.line_state, 0x00, sizeof(DSL_LineState_t));

	ret = ioctl(fd, DSL_FIO_LINE_STATE_GET, (int)&dsl_driver_data.line_state);

	if ((ret < 0) && (dsl_driver_data.line_state.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.Status: error code %d\n",
			       dsl_driver_data.line_state.accessCtl.nReturn);
	} else {
		switch (dsl_driver_data.line_state.data.nLineState) {
		case DSL_LINESTATE_NOT_INITIALIZED:
		case DSL_LINESTATE_NOT_UPDATED:
		case DSL_LINESTATE_DISABLED:
		case DSL_LINESTATE_IDLE_REQUEST:
			strlcpy(obj->status, "Down", sizeof(obj->status));
			strlcpy(obj->link_status, "Initializing", sizeof(obj->link_status));
			break;

		case DSL_LINESTATE_IDLE:
			strlcpy(obj->status, "Down", sizeof(obj->status));
			strlcpy(obj->link_status, "Disabled", sizeof(obj->link_status));
			break;

		case DSL_LINESTATE_SHOWTIME_TC_SYNC:
			strlcpy(obj->status, "Up", sizeof(obj->status));
			strlcpy(obj->link_status, "Up", sizeof(obj->link_status));
			break;

		case DSL_LINESTATE_EXCEPTION:
			strlcpy(obj->status, "Error", sizeof(obj->status));
			strlcpy(obj->link_status, "Error", sizeof(obj->link_status));
			break;

		default:
			strlcpy(obj->status, "Down", sizeof(obj->status));
			strlcpy(obj->link_status, "EstablishingLink", sizeof(obj->link_status));
			break;
		}
	}

	/* Device.DSL.Line.Upstream */
	/* always true */
	obj->upstream = true;

	/* Device.DSL.Line.FirmwareVersion */
	memset(&dsl_driver_data.fw_vers_info, 0x00, sizeof(DSL_VersionInformation_t));

	ret = ioctl(fd, DSL_FIO_VERSION_INFORMATION_GET, (int)&dsl_driver_data.fw_vers_info);

	if ((ret < 0) && (dsl_driver_data.fw_vers_info.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.FirmwareVersion: error code %d\n",
			       dsl_driver_data.fw_vers_info.accessCtl.nReturn);
	} else {
		strlcpy(obj->firmware_version,
			dsl_driver_data.fw_vers_info.data.DSL_ChipSetFWVersion,
			sizeof(obj->firmware_version));
	}

	/* Device.DSL.Line.StandardsSupported */
	memset(&dsl_driver_data.xtu_config, 0x00, sizeof(DSL_G997_XTUSystemEnabling_t));

	ret = ioctl(fd, DSL_FIO_G997_XTU_SYSTEM_ENABLING_CONFIG_GET, (int)&dsl_driver_data.xtu_config);

	if ((ret < 0) && (dsl_driver_data.xtu_config.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.StandardsSupported: error code %d\n",
			       dsl_driver_data.xtu_config.accessCtl.nReturn);
	} else {
		offset = 0;
		buf_len = sizeof(obj->standards_supported);
		for (i = 0; i < sizeof(standarts_table) / sizeof(struct standarts); i++) {
			octet = standarts_table[i].octet;
			bits = standarts_table[i].bit;
			if (dsl_driver_data.xtu_config.data.XTSE[octet] & bits) {
				/* select name from the table and exclude leading comma */
				name = standarts_table[i].name + ((offset) ? 0 : 1);
				buf_len = (sizeof(obj->standards_supported) > offset) ?
				    sizeof(obj->standards_supported) - offset : 0;
				strlcpy(obj->standards_supported + offset, name, buf_len);
				offset += strlen(name);
			}
		}
		for (octet = 0; octet < 8; octet++) {
			obj->xtse[octet] = dsl_driver_data.xtu_config.data.XTSE[octet];
		}
	}

	/* Device.DSL.Line.StandardUsed */
	memset(&dsl_driver_data.xtu_config, 0x00, sizeof(DSL_G997_XTUSystemEnabling_t));

	ret = ioctl(fd, DSL_FIO_G997_XTU_SYSTEM_ENABLING_STATUS_GET, (int)&dsl_driver_data.xtu_config);

	if ((ret < 0) && (dsl_driver_data.xtu_config.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.StandardUsed: error code %d\n",
			       dsl_driver_data.xtu_config.accessCtl.nReturn);
	} else {
		for (i = 0; i < sizeof(standarts_table) / sizeof(struct standarts); i++) {
			octet = standarts_table[i].octet;
			bits = standarts_table[i].bit;
			if (dsl_driver_data.xtu_config.data.XTSE[octet] & bits) {
				name = standarts_table[i].name + 1;
				strlcpy(obj->standard_used, name, sizeof(obj->standard_used));
				break;
			}
		}
		for (octet = 0; octet < 8; octet++) {
			obj->xtse_used[octet] = dsl_driver_data.xtu_config.data.XTSE[octet];
		}

	}

	/* Device.DSL.Line.LineEncoding */
	/* fixed value */
	strlcpy(obj->line_encoding, "DMT", sizeof(obj->line_encoding));

	/* Device.DSL.Line.AllowedProfiles */
	memset(&dsl_driver_data.profiles_support, 0x00, sizeof(DSL_BandPlanSupport_t));

	ret = ioctl(fd, DSL_FIO_BAND_PLAN_SUPPORT_GET, (int)&dsl_driver_data.profiles_support);

	if ((ret < 0) && (dsl_driver_data.profiles_support.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.AllowedProfiles: error code %d\n",
			       dsl_driver_data.profiles_support.accessCtl.nReturn);
	} else {
		offset = 0;
		buf_len = sizeof(obj->allowed_profiles);
		for (i = 0; i < DSL_PROFILE_LAST; i++) {
			for (j = 0; j < DSL_BANDPLAN_LAST; j++) {
				if (dsl_driver_data.profiles_support.data.bSupported[j][i]) {
					/* select name from the table and exclude leading comma */
					name = profiles_table[i] + ((offset) ? 0 : 1);
					buf_len = (sizeof(obj->allowed_profiles) > offset) ?
					    sizeof(obj->allowed_profiles) - offset : 0;
					strlcpy(obj->allowed_profiles + offset, name, buf_len);
					offset += strlen(name);
					break;
				}
			}
		}
	}

	/* Device.DSL.Line.CurrentProfile */
	memset(&dsl_driver_data.profiles_status, 0x00, sizeof(DSL_BandPlanStatus_t));

	ret = ioctl(fd, DSL_FIO_BAND_PLAN_STATUS_GET, (int)&dsl_driver_data.profiles_status);

	if ((ret < 0) && (dsl_driver_data.profiles_status.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.CurrentProfile: error code %d\n",
			       dsl_driver_data.profiles_status.accessCtl.nReturn);
	} else {
		for (i = 0; i < DSL_PROFILE_LAST; i++) {
			if (i == dsl_driver_data.profiles_status.data.nProfile) {
				name = profiles_table[i] + 1;
				strlcpy(obj->current_profile, name, sizeof(obj->current_profile));
				break;
			}
		}
	}

	/* Device.DSL.Line.PowerManagementState */
	memset(&dsl_driver_data.power_status, 0x00, sizeof(DSL_G997_PowerManagementStatus_t));

	ret = ioctl(fd, DSL_FIO_G997_POWER_MANAGEMENT_STATUS_GET, (int)&dsl_driver_data.power_status);

	if ((ret < 0) && (dsl_driver_data.power_status.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.PowerManagementState: error code %d\n",
			       dsl_driver_data.power_status.accessCtl.nReturn);
	} else {
		for (i = 0; i < sizeof(power_table) / sizeof(char *); i++) {
			if (i == dsl_driver_data.power_status.data.nPowerManagementStatus) {
				strlcpy(obj->power_management_state, power_table[i],
					sizeof(obj->power_management_state));
				break;
			}
		}
	}

	/* Device.DSL.Line.SuccessFailureCause */
	memset(&dsl_driver_data.init_status, 0x00, sizeof(DSL_G997_LineInitStatus_t));

	ret = ioctl(fd, DSL_FIO_G997_LINE_INIT_STATUS_GET, (int)&dsl_driver_data.init_status);

	if ((ret < 0) && (dsl_driver_data.init_status.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.SuccessFailureCause: error code %d\n",
			       dsl_driver_data.init_status.accessCtl.nReturn);
	} else {
		obj->success_failure_cause = dsl_driver_data.init_status.data.nLineInitStatus;
	}

	/* Device.DSL.Line.LastStateTransmittedDownstream */
	memset(&dsl_driver_data.last_state, 0x00, sizeof(DSL_G997_LastStateTransmitted_t));

	dsl_driver_data.last_state.nDirection = DSL_DOWNSTREAM;
	ret = ioctl(fd, DSL_FIO_G997_LAST_STATE_TRANSMITTED_GET, (int)&dsl_driver_data.last_state);

	if ((ret < 0) && (dsl_driver_data.last_state.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.LastStateTransmittedDownstream: error code %d\n",
			       dsl_driver_data.last_state.accessCtl.nReturn);
	} else {
		obj->last_state_transmitted_downstream =
		    dsl_driver_data.last_state.data.nLastStateTransmitted;
	}

	/* Device.DSL.Line.LastStateTransmittedUpstream */
	memset(&dsl_driver_data.last_state, 0x00, sizeof(DSL_G997_LastStateTransmitted_t));

	dsl_driver_data.last_state.nDirection = DSL_UPSTREAM;
	ret = ioctl(fd, DSL_FIO_G997_LAST_STATE_TRANSMITTED_GET, (int)&dsl_driver_data.last_state);

	if ((ret < 0) && (dsl_driver_data.last_state.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.LastStateTransmittedUpstream: error code %d\n",
			       dsl_driver_data.last_state.accessCtl.nReturn);
	} else {
		obj->last_state_transmitted_upstream = dsl_driver_data.last_state.data.nLastStateTransmitted;
	}

	/* Device.DSL.Line.US0MASK */
	/* all types supported, use fix value */
	obj->us0_mask = 0xF3F3;

	/* Device.DSL.Line.TRELLISds & Device.DSL.Line.ACTSNRMODEds */
	memset(&dsl_driver_data.line_feature, 0x00, sizeof(DSL_LineFeature_t));

	dsl_driver_data.line_feature.nDirection = DSL_DOWNSTREAM;
	ret = ioctl(fd, DSL_FIO_LINE_FEATURE_STATUS_GET, (int)&dsl_driver_data.line_feature);

	if ((ret < 0) && (dsl_driver_data.line_feature.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.TRELLISds: error code %d\n",
			       dsl_driver_data.line_feature.accessCtl.nReturn);
	} else {
		obj->trellis_ds = dsl_driver_data.line_feature.data.bTrellisEnable;
		obj->act_snr_mode_ds = dsl_driver_data.line_feature.data.bVirtualNoiseSupport;
	}

	/* Device.DSL.Line.TRELLISus  & Device.DSL.Line.ACTSNRMODEus */
	memset(&dsl_driver_data.line_feature, 0x00, sizeof(DSL_LineFeature_t));

	dsl_driver_data.line_feature.nDirection = DSL_UPSTREAM;
	ret = ioctl(fd, DSL_FIO_LINE_FEATURE_STATUS_GET, (int)&dsl_driver_data.line_feature);

	if ((ret < 0) && (dsl_driver_data.line_feature.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.TRELLISus: error code %d\n",
			       dsl_driver_data.line_feature.accessCtl.nReturn);
	} else {
		obj->trellis_us = dsl_driver_data.line_feature.data.bTrellisEnable;
		obj->act_snr_mode_us = dsl_driver_data.line_feature.data.bVirtualNoiseSupport;
	}

	/* Device.DSL.Line.LineNumber */
	/* Todo: clear mapping */
	obj->line_number = 0;

	/* Device.DSL.Line.UpstreamMaxBitRate
	   Device.DSL.Line.UpstreamNoiseMargin
	   Device.DSL.Line.UpstreamPower
	   Device.DSL.Line.UpstreamAttenuation
	 */
	memset(&dsl_driver_data.line_status, 0x00, sizeof(DSL_G997_LineStatus_t));

	dsl_driver_data.line_status.nDirection = DSL_UPSTREAM;
	dsl_driver_data.line_status.nDeltDataType = DSL_DELT_DATA_SHOWTIME;
	ret = ioctl(fd, DSL_FIO_G997_LINE_STATUS_GET, (int)&dsl_driver_data.line_status);

	if ((ret < 0) && (dsl_driver_data.line_status.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.UpstreamMaxBitRate: error code %d\n",
			       dsl_driver_data.line_status.accessCtl.nReturn);
	} else {
		obj->upstream_max_bit_rate = dsl_driver_data.line_status.data.ATTNDR;
		obj->upstream_noise_margin = dsl_driver_data.line_status.data.SNR;
		obj->upstream_power = dsl_driver_data.line_status.data.ACTATP;
		obj->upstream_attenuation = dsl_driver_data.line_status.data.LATN;
	}

	/* Device.DSL.Line.DownstreamMaxBitRate
	   Device.DSL.Line.DownstreamNoiseMargin
	   Device.DSL.Line.DownstreamPower
	   Device.DSL.Line.DownstreamAttenuation
	 */
	memset(&dsl_driver_data.line_status, 0x00, sizeof(DSL_G997_LineStatus_t));

	dsl_driver_data.line_status.nDirection = DSL_DOWNSTREAM;
	dsl_driver_data.line_status.nDeltDataType = DSL_DELT_DATA_SHOWTIME;
	ret = ioctl(fd, DSL_FIO_G997_LINE_STATUS_GET, (int)&dsl_driver_data.line_status);

	if ((ret < 0) && (dsl_driver_data.line_status.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.DownstreamMaxBitRate: error code %d\n",
			       dsl_driver_data.line_status.accessCtl.nReturn);
	} else {
		obj->downstream_max_bit_rate = dsl_driver_data.line_status.data.ATTNDR;
		obj->downstream_noise_margin = dsl_driver_data.line_status.data.SNR;
		obj->downstream_power = dsl_driver_data.line_status.data.ACTATP;
		obj->downstream_attenuation = dsl_driver_data.line_status.data.LATN;
	}

	/* Device.DSL.Line.SNRMpbus */
	memset(&dsl_driver_data.band_status, 0x00, sizeof(DSL_G997_LineStatusPerBand_t));

	dsl_driver_data.band_status.nDirection = DSL_UPSTREAM;
	ret = ioctl(fd, DSL_FIO_G997_LINE_STATUS_PER_BAND_GET, (int)&dsl_driver_data.band_status);

	if ((ret < 0) && (dsl_driver_data.band_status.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.SNRMpbus: error code %d\n",
			       dsl_driver_data.band_status.accessCtl.nReturn);
	} else {
		offset = 0;
		buf_len = sizeof(obj->snr_mpb_us);
		for (i = 0; i < DSL_G997_MAX_NUMBER_OF_BANDS; i++) {
			snprintf(buf, sizeof(buf), ",%d", dsl_driver_data.band_status.data.SNR[i]);
			/* select name from the buf and exclude leading comma */
			name = buf + ((offset) ? 0 : 1);
			buf_len = (sizeof(obj->snr_mpb_us) > offset) ? sizeof(obj->snr_mpb_us) - offset : 0;
			strlcpy(obj->snr_mpb_us + offset, name, buf_len);
			offset += strlen(name);
		}
	}

	/* Device.DSL.Line.SNRMpbds */
	memset(&dsl_driver_data.band_status, 0x00, sizeof(DSL_G997_LineStatusPerBand_t));

	dsl_driver_data.band_status.nDirection = DSL_DOWNSTREAM;
	ret = ioctl(fd, DSL_FIO_G997_LINE_STATUS_PER_BAND_GET, (int)&dsl_driver_data.band_status);

	if ((ret < 0) && (dsl_driver_data.band_status.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.SNRMpbds: error code %d\n",
			       dsl_driver_data.band_status.accessCtl.nReturn);
	} else {
		offset = 0;
		buf_len = sizeof(obj->snr_mpb_ds);
		for (i = 0; i < DSL_G997_MAX_NUMBER_OF_BANDS; i++) {
			snprintf(buf, sizeof(buf), ",%d", dsl_driver_data.band_status.data.SNR[i]);
			/* select name from the buf and exclude leading comma */
			name = buf + ((offset) ? 0 : 1);
			buf_len = (sizeof(obj->snr_mpb_ds) > offset) ? sizeof(obj->snr_mpb_ds) - offset : 0;
			strlcpy(obj->snr_mpb_ds + offset, name, buf_len);
			offset += strlen(name);
		}
	}

	/* Device.DSL.Line.XTURVendor & Device.DSL.Line.XTURCountry */
	memset(&dsl_driver_data.line_inventory, 0x00, sizeof(DSL_G997_LineInventory_t));

	dsl_driver_data.line_inventory.nDirection = DSL_NEAR_END;
	ret = ioctl(fd, DSL_FIO_G997_LINE_INVENTORY_GET, (int)&dsl_driver_data.line_inventory);

	if ((ret < 0) && (dsl_driver_data.line_inventory.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.XTURVendor: error code %d\n",
			       dsl_driver_data.line_inventory.accessCtl.nReturn);
	} else {
		obj->xtur_country[0] = dsl_driver_data.line_inventory.data.G994VendorID[0];
		obj->xtur_country[1] = dsl_driver_data.line_inventory.data.G994VendorID[1];

		obj->xtur_vendor[0] = dsl_driver_data.line_inventory.data.G994VendorID[2];
		obj->xtur_vendor[1] = dsl_driver_data.line_inventory.data.G994VendorID[3];
		obj->xtur_vendor[2] = dsl_driver_data.line_inventory.data.G994VendorID[4];
		obj->xtur_vendor[3] = dsl_driver_data.line_inventory.data.G994VendorID[5];
	}

	/* Device.DSL.Line.XTURANSIStd */
	memset(&dsl_driver_data.t1413_revision_status, 0x00, sizeof(DSL_T1413RevisionStatus_t));

	ret = ioctl(fd, DSL_FIO_T1413_XTUR_REVISION_GET, (int)&dsl_driver_data.t1413_revision_status);
	if ((ret < 0) && (dsl_driver_data.t1413_revision_status.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.XTURANSIStd: error code %d\n",
			       dsl_driver_data.t1413_revision_status.accessCtl.nReturn);
	} else {
		obj->xtur_ansi_std = dsl_driver_data.t1413_revision_status.data.nT1413RevisionNum;
	}

	/* Device.DSL.Line.XTURANSIRev */
	memset(&dsl_driver_data.t1413_vendor_revision_status, 0x00, sizeof(DSL_T1413VendorRevisionStatus_t));

	ret = ioctl(fd, DSL_FIO_T1413_XTUR_VENDOR_REVISION_GET,
		    (int)&dsl_driver_data.t1413_vendor_revision_status);
	if ((ret < 0) && (dsl_driver_data.t1413_vendor_revision_status.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.XTURANSIRev: error code %d\n",
			       dsl_driver_data.t1413_vendor_revision_status.accessCtl.nReturn);
	} else {
		obj->xtur_ansi_rev =
		    dsl_driver_data.t1413_vendor_revision_status.data.nT1413VendorRevisionNum;
	}

	/* Device.DSL.Line.XTUCVendor & Device.DSL.Line.XTUCCountry */
	memset(&dsl_driver_data.line_inventory, 0x00, sizeof(DSL_G997_LineInventory_t));

	dsl_driver_data.line_inventory.nDirection = DSL_FAR_END;
	ret = ioctl(fd, DSL_FIO_G997_LINE_INVENTORY_GET, (int)&dsl_driver_data.line_inventory);

	if ((ret < 0) && (dsl_driver_data.line_inventory.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.XTUCVendor: error code %d\n",
			       dsl_driver_data.line_inventory.accessCtl.nReturn);
	} else {
		obj->xtuc_country[0] = dsl_driver_data.line_inventory.data.G994VendorID[0];
		obj->xtuc_country[1] = dsl_driver_data.line_inventory.data.G994VendorID[1];

		obj->xtuc_vendor[0] = dsl_driver_data.line_inventory.data.G994VendorID[2];
		obj->xtuc_vendor[1] = dsl_driver_data.line_inventory.data.G994VendorID[3];
		obj->xtuc_vendor[2] = dsl_driver_data.line_inventory.data.G994VendorID[4];
		obj->xtuc_vendor[3] = dsl_driver_data.line_inventory.data.G994VendorID[5];
	}

	/* Device.DSL.Line.XTUCANSIStd */
	memset(&dsl_driver_data.t1413_revision_status, 0x00, sizeof(DSL_T1413RevisionStatus_t));

	ret = ioctl(fd, DSL_FIO_T1413_XTUO_REVISION_GET, (int)&dsl_driver_data.t1413_revision_status);
	if ((ret < 0) && (dsl_driver_data.t1413_revision_status.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.XTUCANSIStd: error code %d\n",
			       dsl_driver_data.t1413_revision_status.accessCtl.nReturn);
	} else {
		obj->xtuc_ansi_std = dsl_driver_data.t1413_revision_status.data.nT1413RevisionNum;
	}

	/* Device.DSL.Line.XTUCANSIRev */
	memset(&dsl_driver_data.t1413_vendor_revision_status, 0x00, sizeof(DSL_T1413VendorRevisionStatus_t));

	ret = ioctl(fd, DSL_FIO_T1413_XTUO_VENDOR_REVISION_GET,
		    (int)&dsl_driver_data.t1413_vendor_revision_status);
	if ((ret < 0) && (dsl_driver_data.t1413_vendor_revision_status.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.XTUCANSIRev: error code %d\n",
			       dsl_driver_data.t1413_vendor_revision_status.accessCtl.nReturn);
	} else {
		obj->xtuc_ansi_rev =
		    dsl_driver_data.t1413_vendor_revision_status.data.nT1413VendorRevisionNum;
	}

	/* Device.DSL.Line.UPBOKLERPb & Device.DSL.Line.RXTHRSHds */
	memset(&dsl_driver_data.us_power_back_off_status, 0x00, sizeof(DSL_G997_UsPowerBackOffStatus_t));
	ret = ioctl(fd, DSL_FIO_G997_US_POWER_BACK_OFF_STATUS_GET,
		    (int)&dsl_driver_data.us_power_back_off_status);

	if ((ret < 0) && (dsl_driver_data.us_power_back_off_status.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.UPBOKLERPb: error code %d\n",
			       dsl_driver_data.us_power_back_off_status.accessCtl.nReturn);
	} else {
		offset = 0;
		buf_len = sizeof(obj->upbokler_pb);
		for (i = 0; i < DSL_G997_MAX_NUMBER_OF_BANDS; i++) {
			snprintf(buf, sizeof(buf), ",%hu", dsl_driver_data.us_power_back_off_status.data.nKl0EstimRPb[i]);
			/* out of bounds control: max length 5 = comma + 4 decimal digits (0..2048) */
			buf[5] = '\0';
			/* select name from the buf and exclude leading comma */
			name = buf + ((offset) ? 0 : 1);
			buf_len = (sizeof(obj->upbokler_pb) > offset) ? sizeof(obj->upbokler_pb) - offset : 0;
			strlcpy(obj->upbokler_pb + offset, name, buf_len);
			offset += strlen(name);
		}
		offset = 0;
		buf_len = sizeof(obj->rxthrsh_ds);
		for (i = 0; i < DSL_G997_MAX_NUMBER_OF_BANDS; i++) {
			snprintf(buf, sizeof(buf), ",%hu", dsl_driver_data.us_power_back_off_status.data.nKl0EstimOPb[i]);
			/* out of bounds control: max length 5 = comma + 4 decimal digits (0..2048) */
			buf[5] = '\0';
			/* select name from the buf and exclude leading comma */
			name = buf + ((offset) ? 0 : 1);
			buf_len = (sizeof(obj->rxthrsh_ds) > offset) ? sizeof(obj->rxthrsh_ds) - offset : 0;
			strlcpy(obj->rxthrsh_ds + offset, name, buf_len);
			offset += strlen(name);
		}
	}

	/* Device.DSL.Line.ACTRAMODEds */
	memset(&dsl_driver_data.rate_adaptation_status, 0x00, sizeof(DSL_G997_RateAdaptationStatus_t));

	dsl_driver_data.rate_adaptation_status.nDirection = DSL_DOWNSTREAM;
	ret = ioctl(fd, DSL_FIO_G997_RATE_ADAPTATION_STATUS_GET,
		    (int)&dsl_driver_data.rate_adaptation_status);

	if ((ret < 0) && (dsl_driver_data.rate_adaptation_status.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.ACTRAMODEds: error code %d\n",
			       dsl_driver_data.rate_adaptation_status.accessCtl.nReturn);
	} else {
		obj->act_ra_mode_ds = dsl_driver_data.rate_adaptation_status.data.RA_MODE;
	}

	/* Device.DSL.Line.ACTRAMODEus */
	memset(&dsl_driver_data.rate_adaptation_status, 0x00, sizeof(DSL_G997_RateAdaptationStatus_t));

	dsl_driver_data.rate_adaptation_status.nDirection = DSL_UPSTREAM;
	ret = ioctl(fd, DSL_FIO_G997_RATE_ADAPTATION_STATUS_GET,
		    (int)&dsl_driver_data.rate_adaptation_status);

	if ((ret < 0) && (dsl_driver_data.rate_adaptation_status.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.ACTRAMODEus: error code %d\n",
			       dsl_driver_data.rate_adaptation_status.accessCtl.nReturn);
	} else {
		obj->act_ra_mode_us = dsl_driver_data.rate_adaptation_status.data.RA_MODE;
	}

	/* Device.DSL.Line.SNRMROCus */
	/* todo: clarify mapping */
	obj->snr_mroc_us = 0;
	fapi_dsl_close_device(fd);
	return fapi_status;
}

DLL_PUBLIC enum fapi_dsl_status fapi_dsl_line_set(struct fapi_dsl_ctx *ctx,
						  const struct dsl_fapi_line_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;
	union dsl_driver_data_types {
		DSL_AutobootControl_t autoboot_cotrol;
	} dsl_driver_data;
	int ret, fd;

	if (ctx == NULL)
		return FAPI_DSL_STATUS_ERROR;

	if (obj == NULL)
		return FAPI_DSL_STATUS_ERROR;

	fd = fapi_dsl_open_device(ctx->entity);
	if (fd < 0)
		return FAPI_DSL_STATUS_ERROR;

	/* Device.DSL.Line.Enable */
	memset(&dsl_driver_data.autoboot_cotrol, 0x00, sizeof(DSL_AutobootControl_t));
	if (obj->enable) {
		dsl_driver_data.autoboot_cotrol.data.nCommand = DSL_AUTOBOOT_CTRL_START;
	} else {
		dsl_driver_data.autoboot_cotrol.data.nCommand = DSL_AUTOBOOT_CTRL_STOP;
	}

	ret = ioctl(fd, DSL_FIO_AUTOBOOT_CONTROL_SET, (int)&dsl_driver_data.autoboot_cotrol);

	if ((ret < 0) && (dsl_driver_data.autoboot_cotrol.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.Enable: error code %d\n",
			       dsl_driver_data.autoboot_cotrol.accessCtl.nReturn);
	}
	fapi_dsl_close_device(fd);
	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_line_stats_get(struct fapi_dsl_ctx *ctx, struct dsl_fapi_line_stats_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;
	union dsl_driver_data_types {
		DSL_PM_ChannelCountersTotal_t pm_channel_counters_total;
		DSL_PM_LineInitCounters_t pm_line_init_counters;
		DSL_PM_ChannelCounters_t pm_channel_counters;
	} dsl_driver_data;
	int ret, fd;

	if (ctx == NULL)
		return FAPI_DSL_STATUS_ERROR;

	if (obj == NULL)
		return FAPI_DSL_STATUS_ERROR;

	fd = fapi_dsl_open_device(ctx->entity);
	if (fd < 0)
		return FAPI_DSL_STATUS_ERROR;

	memset(obj, 0x00, sizeof(struct dsl_fapi_line_stats_obj));

	/* Device.DSL.Line.Stats.TotalStart */
	memset(&dsl_driver_data.pm_channel_counters_total, 0x00, sizeof(DSL_PM_ChannelCountersTotal_t));

	dsl_driver_data.pm_channel_counters_total.nChannel = 0;
	/* todo: clarify direction */
	dsl_driver_data.pm_channel_counters_total.nDirection = DSL_NEAR_END;
	ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_TOTAL_GET,
		    (int)&dsl_driver_data.pm_channel_counters_total);

	if ((ret < 0) && (dsl_driver_data.pm_channel_counters_total.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.Stats.TotalStart: error code %d\n",
			       dsl_driver_data.pm_channel_counters_total.accessCtl.nReturn);
	} else {
		obj->total_start = dsl_driver_data.pm_channel_counters_total.total.nElapsedTime;
	}

	/* Device.DSL.Line.Stats.ShowtimeStart */
	memset(&dsl_driver_data.pm_channel_counters, 0x00, sizeof(DSL_PM_ChannelCounters_t));

	dsl_driver_data.pm_channel_counters.nChannel = 0;
	dsl_driver_data.pm_channel_counters.nDirection = DSL_NEAR_END;
	dsl_driver_data.pm_channel_counters.nHistoryInterval = 0;
	ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_SHOWTIME_GET, (int)&dsl_driver_data.pm_channel_counters);

	if ((ret < 0) && (dsl_driver_data.pm_channel_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.Stats.ShowtimeStart: error code %d\n",
			       dsl_driver_data.pm_channel_counters.accessCtl.nReturn);
	} else {
		obj->showtime_start = dsl_driver_data.pm_channel_counters.interval.nElapsedTime;
	}

	/* Device.DSL.Line.Stats.LastShowtimeStart */
	memset(&dsl_driver_data.pm_channel_counters, 0x00, sizeof(DSL_PM_ChannelCounters_t));

	dsl_driver_data.pm_channel_counters.nChannel = 0;
	dsl_driver_data.pm_channel_counters.nDirection = DSL_NEAR_END;
	dsl_driver_data.pm_channel_counters.nHistoryInterval = 1;
	ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_SHOWTIME_GET, (int)&dsl_driver_data.pm_channel_counters);

	if ((ret < 0) && (dsl_driver_data.pm_channel_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.Stats.LastShowtimeStart: error code %d\n",
			       dsl_driver_data.pm_channel_counters.accessCtl.nReturn);
	} else {
		obj->last_showtime_start = dsl_driver_data.pm_channel_counters.interval.nElapsedTime;
	}

	/* Device.DSL.Line.Stats.CurrentDayStart */
	memset(&dsl_driver_data.pm_line_init_counters, 0x00, sizeof(DSL_PM_LineInitCounters_t));

	dsl_driver_data.pm_line_init_counters.nHistoryInterval = 0;
	ret = ioctl(fd, DSL_FIO_PM_LINE_INIT_COUNTERS_1DAY_GET, (int)&dsl_driver_data.pm_line_init_counters);

	if ((ret < 0) && (dsl_driver_data.pm_line_init_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.Stats.CurrentDayStart: error code %d\n",
			       dsl_driver_data.pm_line_init_counters.accessCtl.nReturn);
	} else {
		obj->current_day_start = dsl_driver_data.pm_line_init_counters.interval.nElapsedTime;
	}

	/* Device.DSL.Line.Stats.QuarterHourStart */
	memset(&dsl_driver_data.pm_channel_counters, 0x00, sizeof(DSL_PM_ChannelCounters_t));

	dsl_driver_data.pm_channel_counters.nChannel = 0;
	dsl_driver_data.pm_channel_counters.nDirection = DSL_NEAR_END;
	dsl_driver_data.pm_channel_counters.nHistoryInterval = 0;
	ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_15MIN_GET, (int)&dsl_driver_data.pm_channel_counters);

	if ((ret < 0) && (dsl_driver_data.pm_channel_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.Stats.QuarterHourStart: error code %d\n",
			       dsl_driver_data.pm_channel_counters.accessCtl.nReturn);
	} else {
		obj->quarter_hour_start = dsl_driver_data.pm_channel_counters.interval.nElapsedTime;
	}
	fapi_dsl_close_device(fd);
	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_line_stats_set(struct fapi_dsl_ctx *ctx,
					     const struct dsl_fapi_line_stats_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_line_stats_total_get(struct fapi_dsl_ctx *ctx,
						   struct dsl_fapi_line_stats_interval_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;
	union dsl_driver_data_types {
		DSL_PM_LineSecCountersTotal_t pm_line_sec_counters_total;
	} dsl_driver_data;
	int ret, fd;

	if (ctx == NULL)
		return FAPI_DSL_STATUS_ERROR;

	if (obj == NULL)
		return FAPI_DSL_STATUS_ERROR;

	fd = fapi_dsl_open_device(ctx->entity);

	if (fd < 0)
		return FAPI_DSL_STATUS_ERROR;

	memset(obj, 0x00, sizeof(struct dsl_fapi_line_stats_interval_obj));

	/* Device.DSL.Line.Stats.Total.ErroredSecs */
	/* Device.DSL.Line.Stats.Total.SeverelyErroredSecs */
	memset(&dsl_driver_data.pm_line_sec_counters_total, 0x00, sizeof(DSL_PM_LineSecCountersTotal_t));

	dsl_driver_data.pm_line_sec_counters_total.nDirection = DSL_NEAR_END;
	ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_COUNTERS_TOTAL_GET,
		    (int)&dsl_driver_data.pm_line_sec_counters_total);

	if ((ret < 0) && (dsl_driver_data.pm_line_sec_counters_total.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.Stats.Total: error code %d\n",
			       dsl_driver_data.pm_line_sec_counters_total.accessCtl.nReturn);
	} else {
		obj->errored_secs = dsl_driver_data.pm_line_sec_counters_total.data.nES;
		obj->severely_errored_secs = dsl_driver_data.pm_line_sec_counters_total.data.nSES;
	}
	fapi_dsl_close_device(fd);
	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_line_stats_total_set(struct fapi_dsl_ctx *ctx,
						   const struct dsl_fapi_line_stats_interval_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_line_stats_showtime_get(struct fapi_dsl_ctx *ctx,
						      struct dsl_fapi_line_stats_interval_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;
	union dsl_driver_data_types {
		DSL_PM_LineSecCounters_t pm_line_sec_counters;
	} dsl_driver_data;
	int ret, fd;

	if (ctx == NULL)
		return FAPI_DSL_STATUS_ERROR;

	if (obj == NULL)
		return FAPI_DSL_STATUS_ERROR;

	fd = fapi_dsl_open_device(ctx->entity);
	if (fd < 0)
		return FAPI_DSL_STATUS_ERROR;

	memset(obj, 0x00, sizeof(struct dsl_fapi_line_stats_interval_obj));

	/* Device.DSL.Line.Stats.Showtime.ErroredSecs */
	/* Device.DSL.Line.Stats.Showtime.SeverelyErroredSecs */
	memset(&dsl_driver_data.pm_line_sec_counters, 0x00, sizeof(DSL_PM_LineSecCounters_t));

	dsl_driver_data.pm_line_sec_counters.nDirection = DSL_NEAR_END;
	dsl_driver_data.pm_line_sec_counters.nHistoryInterval = 0;
	ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_COUNTERS_SHOWTIME_GET,
		    (int)&dsl_driver_data.pm_line_sec_counters);

	if ((ret < 0) && (dsl_driver_data.pm_line_sec_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.Stats.Showtime: error code %d\n",
			       dsl_driver_data.pm_line_sec_counters.accessCtl.nReturn);
	} else {
		obj->errored_secs = dsl_driver_data.pm_line_sec_counters.data.nES;
		obj->severely_errored_secs = dsl_driver_data.pm_line_sec_counters.data.nSES;
	}
	fapi_dsl_close_device(fd);
	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_line_stats_showtime_set(struct fapi_dsl_ctx *ctx,
						      const struct dsl_fapi_line_stats_interval_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_line_stats_last_showtime_get(struct fapi_dsl_ctx *ctx,
							   struct dsl_fapi_line_stats_interval_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;
	union dsl_driver_data_types {
		DSL_PM_LineSecCounters_t pm_line_sec_counters;
	} dsl_driver_data;
	int ret, fd;

	if (ctx == NULL)
		return FAPI_DSL_STATUS_ERROR;

	if (obj == NULL)
		return FAPI_DSL_STATUS_ERROR;

	fd = fapi_dsl_open_device(ctx->entity);
	if (fd < 0)
		return FAPI_DSL_STATUS_ERROR;

	memset(obj, 0x00, sizeof(struct dsl_fapi_line_stats_interval_obj));

	/* Device.DSL.Line.Stats.LastShowtime.ErroredSecs */
	/* Device.DSL.Line.Stats.LastShowtime.SeverelyErroredSecs */
	memset(&dsl_driver_data.pm_line_sec_counters, 0x00, sizeof(DSL_PM_LineSecCounters_t));

	dsl_driver_data.pm_line_sec_counters.nDirection = DSL_NEAR_END;
	dsl_driver_data.pm_line_sec_counters.nHistoryInterval = 1;
	ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_COUNTERS_SHOWTIME_GET,
		    (int)&dsl_driver_data.pm_line_sec_counters);

	if ((ret < 0) && (dsl_driver_data.pm_line_sec_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.Stats.LastShowtime: error code %d\n",
			       dsl_driver_data.pm_line_sec_counters.accessCtl.nReturn);
	} else {
		obj->errored_secs = dsl_driver_data.pm_line_sec_counters.data.nES;
		obj->severely_errored_secs = dsl_driver_data.pm_line_sec_counters.data.nSES;
	}
	fapi_dsl_close_device(fd);
	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_line_stats_last_showtime_set(struct fapi_dsl_ctx *ctx,
							   const struct dsl_fapi_line_stats_interval_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_line_stats_current_day_get(struct fapi_dsl_ctx *ctx,
							 struct dsl_fapi_line_stats_interval_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;
	union dsl_driver_data_types {
		DSL_PM_LineSecCounters_t pm_line_sec_counters;
	} dsl_driver_data;
	int ret, fd;

	if (ctx == NULL)
		return FAPI_DSL_STATUS_ERROR;

	if (obj == NULL)
		return FAPI_DSL_STATUS_ERROR;

	fd = fapi_dsl_open_device(ctx->entity);
	if (fd < 0)
		return FAPI_DSL_STATUS_ERROR;

	memset(obj, 0x00, sizeof(struct dsl_fapi_line_stats_interval_obj));

	/* Device.DSL.Line.Stats.CurrentDay.ErroredSecs */
	/* Device.DSL.Line.Stats.CurrentDay.SeverelyErroredSecs */
	memset(&dsl_driver_data.pm_line_sec_counters, 0x00, sizeof(DSL_PM_LineSecCounters_t));

	dsl_driver_data.pm_line_sec_counters.nDirection = DSL_NEAR_END;
	dsl_driver_data.pm_line_sec_counters.nHistoryInterval = 0;
	ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_COUNTERS_1DAY_GET, (int)&dsl_driver_data.pm_line_sec_counters);

	if ((ret < 0) && (dsl_driver_data.pm_line_sec_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.Stats.CurrentDay: error code %d\n",
			       dsl_driver_data.pm_line_sec_counters.accessCtl.nReturn);
	} else {
		obj->errored_secs = dsl_driver_data.pm_line_sec_counters.data.nES;
		obj->severely_errored_secs = dsl_driver_data.pm_line_sec_counters.data.nSES;
	}
	fapi_dsl_close_device(fd);
	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_line_stats_current_day_set(struct fapi_dsl_ctx *ctx,
							 const struct dsl_fapi_line_stats_interval_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_line_stats_quarter_hour_get(struct fapi_dsl_ctx *ctx,
							  struct dsl_fapi_line_stats_interval_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;
	union dsl_driver_data_types {
		DSL_PM_LineSecCounters_t pm_line_sec_counters;
	} dsl_driver_data;
	int ret, fd;

	if (ctx == NULL)
		return FAPI_DSL_STATUS_ERROR;

	if (obj == NULL)
		return FAPI_DSL_STATUS_ERROR;

	fd = fapi_dsl_open_device(ctx->entity);
	if (fd < 0)
		return FAPI_DSL_STATUS_ERROR;

	memset(obj, 0x00, sizeof(struct dsl_fapi_line_stats_interval_obj));

	/* Device.DSL.Line.Stats.QuarterHour.ErroredSecs */
	/* Device.DSL.Line.Stats.QuarterHour.SeverelyErroredSecs */
	memset(&dsl_driver_data.pm_line_sec_counters, 0x00, sizeof(DSL_PM_LineSecCounters_t));

	dsl_driver_data.pm_line_sec_counters.nDirection = DSL_NEAR_END;
	dsl_driver_data.pm_line_sec_counters.nHistoryInterval = 0;
	ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_COUNTERS_15MIN_GET, (int)&dsl_driver_data.pm_line_sec_counters);

	if ((ret < 0) && (dsl_driver_data.pm_line_sec_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.Stats.QuarterHour: error code %d\n",
			       dsl_driver_data.pm_line_sec_counters.accessCtl.nReturn);
	} else {
		obj->errored_secs = dsl_driver_data.pm_line_sec_counters.data.nES;
		obj->severely_errored_secs = dsl_driver_data.pm_line_sec_counters.data.nSES;
	}
	fapi_dsl_close_device(fd);
	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_line_stats_quarter_hour_set(struct fapi_dsl_ctx *ctx,
							  const struct dsl_fapi_line_stats_interval_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_line_test_params_get(struct fapi_dsl_ctx *ctx,
						   struct dsl_fapi_line_test_params_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;
	union dsl_driver_data_types {
		DSL_G997_DeltHlog_t delt_hlog;
		DSL_G997_DeltQln_t delt_qln;
		DSL_G997_DeltSnr_t delt_snr;
		DSL_G997_LineStatus_t line_status;
	} dsl_driver_data;
	int i;
	int ret, fd;

	if (ctx == NULL)
		return FAPI_DSL_STATUS_ERROR;

	if (obj == NULL)
		return FAPI_DSL_STATUS_ERROR;

	fd = fapi_dsl_open_device(ctx->entity);
	if (fd < 0)
		return FAPI_DSL_STATUS_ERROR;

	memset(obj, 0x00, sizeof(struct dsl_fapi_line_test_params_obj));

	/* Device.DSL.Line.TestParams.HLOG_DS */
	memset(&dsl_driver_data.delt_hlog, 0x00, sizeof(DSL_G997_DeltHlog_t));

	dsl_driver_data.delt_hlog.nDirection = DSL_DOWNSTREAM;
	dsl_driver_data.delt_hlog.nDeltDataType = DSL_DELT_DATA_SHOWTIME;
	ret = ioctl(fd, DSL_FIO_G997_DELT_HLOG_GET, (int)&dsl_driver_data.delt_hlog);

	if ((ret < 0) && (dsl_driver_data.delt_hlog.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.TestParams.HLOG_DS: error code %d\n",
			       dsl_driver_data.delt_hlog.accessCtl.nReturn);
	} else {
		obj->hlogg_ds = dsl_driver_data.delt_hlog.data.nGroupSize;
		obj->hlogmt_ds = dsl_driver_data.delt_hlog.data.nMeasurementTime;

		for (i = 0; i < dsl_driver_data.delt_hlog.data.deltHlog.nNumData; i++) {
			if (i >= 512)
				break;
			snprintf(obj->hlogps_ds + 5 * i, sizeof(obj->hlogps_ds) - 5 * i, "%04d,",
				dsl_driver_data.delt_hlog.data.deltHlog.nNSCData[i]);
		}
		if (i)
			obj->hlogps_ds[5 * i - 1] = '\0';
	}

	/* Device.DSL.Line.TestParams.HLOG_US */
	memset(&dsl_driver_data.delt_hlog, 0x00, sizeof(DSL_G997_DeltHlog_t));

	dsl_driver_data.delt_hlog.nDirection = DSL_UPSTREAM;
	dsl_driver_data.delt_hlog.nDeltDataType = DSL_DELT_DATA_SHOWTIME;
	ret = ioctl(fd, DSL_FIO_G997_DELT_HLOG_GET, (int)&dsl_driver_data.delt_hlog);

	if ((ret < 0) && (dsl_driver_data.delt_hlog.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.TestParams.HLOG_US: error code %d\n",
			       dsl_driver_data.delt_hlog.accessCtl.nReturn);
	} else {
		obj->hlogg_us = dsl_driver_data.delt_hlog.data.nGroupSize;
		obj->hlogmt_us = dsl_driver_data.delt_hlog.data.nMeasurementTime;

		for (i = 0; i < dsl_driver_data.delt_hlog.data.deltHlog.nNumData; i++) {
			if (i >= 512)
				break;
			snprintf(obj->hlogps_us + 5 * i, sizeof(obj->hlogps_us) - 5 * i, "%04d,",
				dsl_driver_data.delt_hlog.data.deltHlog.nNSCData[i]);
		}
		if (i)
			obj->hlogps_us[5 * i - 1] = '\0';
	}

	/* Device.DSL.Line.TestParams.QLN_DS */
	memset(&dsl_driver_data.delt_qln, 0x00, sizeof(DSL_G997_DeltQln_t));

	dsl_driver_data.delt_qln.nDirection = DSL_DOWNSTREAM;
	dsl_driver_data.delt_qln.nDeltDataType = DSL_DELT_DATA_SHOWTIME;
	ret = ioctl(fd, DSL_FIO_G997_DELT_QLN_GET, (int)&dsl_driver_data.delt_qln);

	if ((ret < 0) && (dsl_driver_data.delt_qln.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.TestParams.QLN_DS: error code %d\n",
			       dsl_driver_data.delt_qln.accessCtl.nReturn);
	} else {
		obj->qlng_ds = dsl_driver_data.delt_qln.data.nGroupSize;
		obj->qlnmt_ds = dsl_driver_data.delt_qln.data.nMeasurementTime;

		for (i = 0; i < dsl_driver_data.delt_qln.data.deltQln.nNumData; i++) {
			if (i >= 512)
				break;
			snprintf(obj->qlnps_ds + 4 * i, sizeof(obj->qlnps_ds) - 4 * i, "%03d,",
				dsl_driver_data.delt_qln.data.deltQln.nNSCData[i]);
		}
		if (i)
			obj->qlnps_ds[4 * i - 1] = '\0';
	}

	/* Device.DSL.Line.TestParams.QLN_US */
	memset(&dsl_driver_data.delt_qln, 0x00, sizeof(DSL_G997_DeltQln_t));

	dsl_driver_data.delt_qln.nDirection = DSL_UPSTREAM;
	dsl_driver_data.delt_qln.nDeltDataType = DSL_DELT_DATA_SHOWTIME;
	ret = ioctl(fd, DSL_FIO_G997_DELT_QLN_GET, (int)&dsl_driver_data.delt_qln);

	if ((ret < 0) && (dsl_driver_data.delt_qln.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.TestParams.QLN_US: error code %d\n",
			       dsl_driver_data.delt_qln.accessCtl.nReturn);
	} else {
		obj->qlng_us = dsl_driver_data.delt_qln.data.nGroupSize;
		obj->qlnmt_us = dsl_driver_data.delt_qln.data.nMeasurementTime;

		for (i = 0; i < dsl_driver_data.delt_qln.data.deltQln.nNumData; i++) {
			if (i >= 512)
				break;
			snprintf(obj->qlnps_us + 4 * i, sizeof(obj->qlnps_us) - 4 * i, "%03d,",
				dsl_driver_data.delt_qln.data.deltQln.nNSCData[i]);
		}
		if (i)
			obj->qlnps_us[4 * i - 1] = '\0';
	}

	/* Device.DSL.Line.TestParams.SNR_DS */
	memset(&dsl_driver_data.delt_snr, 0x00, sizeof(DSL_G997_DeltSnr_t));

	dsl_driver_data.delt_snr.nDirection = DSL_DOWNSTREAM;
	dsl_driver_data.delt_snr.nDeltDataType = DSL_DELT_DATA_SHOWTIME;
	ret = ioctl(fd, DSL_FIO_G997_DELT_SNR_GET, (int)&dsl_driver_data.delt_snr);

	if ((ret < 0) && (dsl_driver_data.delt_snr.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.TestParams.SNR_DS: error code %d\n",
			       dsl_driver_data.delt_snr.accessCtl.nReturn);
	} else {
		obj->snrg_ds = dsl_driver_data.delt_snr.data.nGroupSize;
		obj->snrmt_ds = dsl_driver_data.delt_snr.data.nMeasurementTime;

		for (i = 0; i < dsl_driver_data.delt_snr.data.deltSnr.nNumData; i++) {
			if (i >= 512)
				break;
			snprintf(obj->snrps_ds + 4 * i, sizeof(obj->snrps_ds) - 4 * i, "%03d,",
				dsl_driver_data.delt_snr.data.deltSnr.nNSCData[i]);
		}
		if (i)
			obj->snrps_ds[4 * i - 1] = '\0';
	}

	/* Device.DSL.Line.TestParams.SNR_US */
	memset(&dsl_driver_data.delt_snr, 0x00, sizeof(DSL_G997_DeltSnr_t));

	dsl_driver_data.delt_snr.nDirection = DSL_UPSTREAM;
	dsl_driver_data.delt_snr.nDeltDataType = DSL_DELT_DATA_SHOWTIME;
	ret = ioctl(fd, DSL_FIO_G997_DELT_SNR_GET, (int)&dsl_driver_data.delt_snr);

	if ((ret < 0) && (dsl_driver_data.delt_snr.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.TestParams.SNR_US: error code %d\n",
			       dsl_driver_data.delt_snr.accessCtl.nReturn);
	} else {
		obj->snrg_us = dsl_driver_data.delt_snr.data.nGroupSize;
		obj->snrmt_us = dsl_driver_data.delt_snr.data.nMeasurementTime;

		for (i = 0; i < dsl_driver_data.delt_snr.data.deltSnr.nNumData; i++) {
			if (i >= 512)
				break;
			snprintf(obj->snrps_us + 4 * i, sizeof(obj->snrps_us) - 4 * i, "%03d,",
				dsl_driver_data.delt_snr.data.deltSnr.nNSCData[i]);
		}
		if (i)
			obj->snrps_us[4 * i - 1] = '\0';
	}

	/* Device.DSL.Line.TestParams.LATN_DS */
	/* Device.DSL.Line.TestParams.SATN_DS */
	memset(&dsl_driver_data.line_status, 0x00, sizeof(DSL_G997_LineStatus_t));

	dsl_driver_data.line_status.nDirection = DSL_DOWNSTREAM;
	dsl_driver_data.line_status.nDeltDataType = DSL_DELT_DATA_SHOWTIME;
	ret = ioctl(fd, DSL_FIO_G997_LINE_STATUS_GET, (int)&dsl_driver_data.line_status);

	if ((ret < 0) && (dsl_driver_data.line_status.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.TestParams.LATN_DS: error code %d\n",
			       dsl_driver_data.line_status.accessCtl.nReturn);
	} else {
		snprintf(obj->latn_ds, sizeof(obj->latn_ds), "%d", dsl_driver_data.line_status.data.LATN);

		snprintf(obj->satn_ds, sizeof(obj->satn_ds), "%d", dsl_driver_data.line_status.data.SATN);
	}

	/* Device.DSL.Line.TestParams.LATN_US */
	/* Device.DSL.Line.TestParams.SATN_US */
	memset(&dsl_driver_data.line_status, 0x00, sizeof(DSL_G997_LineStatus_t));

	dsl_driver_data.line_status.nDirection = DSL_UPSTREAM;
	dsl_driver_data.line_status.nDeltDataType = DSL_DELT_DATA_SHOWTIME;
	ret = ioctl(fd, DSL_FIO_G997_LINE_STATUS_GET, (int)&dsl_driver_data.line_status);

	if ((ret < 0) && (dsl_driver_data.line_status.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.TestParams.LATN_US: error code %d\n",
			       dsl_driver_data.line_status.accessCtl.nReturn);
	} else {
		snprintf(obj->latn_us, sizeof(obj->latn_us), "%d", dsl_driver_data.line_status.data.LATN);

		snprintf(obj->satn_us, sizeof(obj->satn_us), "%d", dsl_driver_data.line_status.data.SATN);
	}
	fapi_dsl_close_device(fd);
	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_line_test_params_set(struct fapi_dsl_ctx *ctx,
						   const struct dsl_fapi_line_test_params_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_channel_get(struct fapi_dsl_ctx *ctx, struct dsl_fapi_channel_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;
	union dsl_driver_data_types {
		DSL_AutobootStatus_t autoboot_status;
		DSL_LineState_t line_state;
		DSL_SystemInterfaceConfig_t system_interface_config;
		DSL_SystemInterfaceStatus_t system_interface_status;
		DSL_G997_FramingParameterStatus_t framing_parameter_status;
		DSL_G997_ChannelStatus_t channel_status;
		DSL_G997_XTUSystemEnabling_t xtu_status;
	} dsl_driver_data;
	DSL_DslModeSelection_t dsl_mode = DSL_MODE_NA;
	unsigned int offset;
	unsigned int max_offset;
	int ret, fd;

	if (ctx == NULL)
		return FAPI_DSL_STATUS_ERROR;

	if (obj == NULL)
		return FAPI_DSL_STATUS_ERROR;

	fd = fapi_dsl_open_device(ctx->entity);
	if (fd < 0)
		return FAPI_DSL_STATUS_ERROR;

	memset(obj, 0x00, sizeof(struct dsl_fapi_channel_obj));

	/* Device.DSL.Channel.Enable */
	memset(&dsl_driver_data.autoboot_status, 0x00, sizeof(DSL_AutobootStatus_t));

	ret = ioctl(fd, DSL_FIO_AUTOBOOT_STATUS_GET, (int)&dsl_driver_data.autoboot_status);

	if ((ret < 0) && (dsl_driver_data.autoboot_status.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.Enable: error code %d\n",
			       dsl_driver_data.autoboot_status.accessCtl.nReturn);
	} else {
		switch (dsl_driver_data.autoboot_status.data.nStatus) {
		case DSL_AUTOBOOT_STATUS_DISABLED:
		case DSL_AUTOBOOT_STATUS_STOPPED:
			obj->enable = false;
			break;

		default:
			obj->enable = true;
			break;
		}
	}

	/* Device.DSL.Channel.Status */
	memset(&dsl_driver_data.line_state, 0x00, sizeof(DSL_LineState_t));

	ret = ioctl(fd, DSL_FIO_LINE_STATE_GET, (int)&dsl_driver_data.line_state);

	if ((ret < 0) && (dsl_driver_data.line_state.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.Status: error code %d\n",
			       dsl_driver_data.line_state.accessCtl.nReturn);
	} else {
		switch (dsl_driver_data.line_state.data.nLineState) {
		case DSL_LINESTATE_SHOWTIME_TC_SYNC:
			strlcpy(obj->status, "Up", sizeof(obj->status));
			break;

		case DSL_LINESTATE_EXCEPTION:
			strlcpy(obj->status, "Error", sizeof(obj->status));
			break;

		default:
			strlcpy(obj->status, "Down", sizeof(obj->status));
			break;
		}
	}

	/* Device.DSL.Channel.LinkEncapsulationSupported */
	memset(&dsl_driver_data.system_interface_config, 0x00, sizeof(DSL_SystemInterfaceConfig_t));
	offset = max_offset = 0;
	dsl_driver_data.system_interface_config.nDslMode = DSL_MODE_ADSL;
	ret = ioctl(fd, DSL_FIO_SYSTEM_INTERFACE_CONFIG_GET, (int)&dsl_driver_data.system_interface_config);
	if ((ret < 0) && (dsl_driver_data.system_interface_config.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.LinkEncapsulationSupported:"
			       "error code %d\n", dsl_driver_data.system_interface_config.accessCtl.nReturn);
	} else {
		switch (dsl_driver_data.system_interface_config.data.nTcLayer) {
		case DSL_TC_ATM:
			strlcpy(obj->link_encapsulation_supported,
				"G.992.3_Annex_K_ATM", sizeof(obj->link_encapsulation_supported));
			break;

		case DSL_TC_EFM:
		case DSL_TC_EFM_FORCED:
			strlcpy(obj->link_encapsulation_supported,
				"G.992.3_Annex_K_PTM", sizeof(obj->link_encapsulation_supported));
			break;

		case DSL_TC_AUTO:
			strlcpy(obj->link_encapsulation_supported,
				"G.994.1 (Auto)", sizeof(obj->link_encapsulation_supported));
			break;

		default:
			obj->link_encapsulation_supported[0] = '\0';
		}
		offset = strlen(obj->link_encapsulation_supported);
	}
	if (offset) {
		++offset;
		max_offset = (offset < sizeof(obj->link_encapsulation_supported)) ?
			offset : sizeof(obj->link_encapsulation_supported) - 1;
		obj->link_encapsulation_supported[max_offset] = ',';
	}

	memset(&dsl_driver_data.system_interface_config, 0x00, sizeof(DSL_SystemInterfaceConfig_t));

	dsl_driver_data.system_interface_config.nDslMode = DSL_MODE_VDSL;
	ret = ioctl(fd, DSL_FIO_SYSTEM_INTERFACE_CONFIG_GET, (int)&dsl_driver_data.system_interface_config);
	if ((ret < 0) && (dsl_driver_data.system_interface_config.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.LinkEncapsulationSupported:"
			       "error code %d\n", dsl_driver_data.system_interface_config.accessCtl.nReturn);
	} else {
		switch (dsl_driver_data.system_interface_config.data.nTcLayer) {
		case DSL_TC_ATM:
			strlcpy(obj->link_encapsulation_supported + offset,
				"G.993.2_Annex_K_ATM", sizeof(obj->link_encapsulation_supported) - offset);
			break;

		case DSL_TC_EFM:
		case DSL_TC_EFM_FORCED:
			strlcpy(obj->link_encapsulation_supported + offset,
				"G.993.2_Annex_K_PTM", sizeof(obj->link_encapsulation_supported) - offset);
			break;

		case DSL_TC_AUTO:
			strlcpy(obj->link_encapsulation_supported + offset,
				"G.994.1 (Auto)", sizeof(obj->link_encapsulation_supported) - offset);
			break;

		default:
			obj->link_encapsulation_supported[max_offset] = '\0';
		}
	}

	/* get current xDSL mode */
	DSL_G997_XTUSystemEnabling_t xtu_status;
	memset(&dsl_driver_data.xtu_status, 0x00, sizeof(DSL_G997_XTUSystemEnabling_t));

	ret = ioctl(fd, DSL_FIO_G997_XTU_SYSTEM_ENABLING_STATUS_GET, (int)&dsl_driver_data.xtu_status);

	if ((ret < 0) && (dsl_driver_data.xtu_status.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.XTUStatus: error code %d\n",
			       dsl_driver_data.xtu_status.accessCtl.nReturn);
	} else {
		if (dsl_driver_data.xtu_status.data.XTSE[7]) {
			dsl_mode = DSL_MODE_VDSL;
		} else {
			dsl_mode = DSL_MODE_ADSL;
		}
	}

	/* Device.DSL.Channel.LinkEncapsulationUsed */
	memset(&dsl_driver_data.system_interface_status, 0x00, sizeof(DSL_SystemInterfaceStatus_t));
	ret = ioctl(fd, DSL_FIO_SYSTEM_INTERFACE_STATUS_GET, (int)&dsl_driver_data.system_interface_status);
	if ((ret < 0) && (dsl_driver_data.system_interface_status.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.LinkEncapsulationUsed:"
			       "error code %d\n", dsl_driver_data.system_interface_status.accessCtl.nReturn);
	} else {
		switch (dsl_driver_data.system_interface_status.data.nTcLayer) {
		case DSL_TC_ATM:
			if (dsl_mode == DSL_MODE_ADSL) {
				strlcpy(obj->link_encapsulation_used,
					"G.992.3_Annex_K_ATM", sizeof(obj->link_encapsulation_used));
			} else {
				strlcpy(obj->link_encapsulation_used,
					"G.993.2_Annex_K_ATM", sizeof(obj->link_encapsulation_used));
			}
			break;

		case DSL_TC_EFM:
		case DSL_TC_EFM_FORCED:
			if (dsl_mode == DSL_MODE_ADSL) {
				strlcpy(obj->link_encapsulation_used,
					"G.992.3_Annex_K_PTM", sizeof(obj->link_encapsulation_used));
			} else {
				strlcpy(obj->link_encapsulation_used,
					"G.993.2_Annex_K_PTM", sizeof(obj->link_encapsulation_used));
			}
			break;

		case DSL_TC_AUTO:
			strlcpy(obj->link_encapsulation_used,
				"G.994.1 (Auto)", sizeof(obj->link_encapsulation_used));
			break;

		default:
			obj->link_encapsulation_used[0] = '\0';
		}
	}

	/* Device.DSL.Channel.LPATH */
	/* Device.DSL.Channel.INTLVDEPTH */
	/* Device.DSL.Channel.INTLVBLOCK */
	/* Device.DSL.Channel.NFEC */
	/* Device.DSL.Channel.RFEC */
	/* Device.DSL.Channel.LSYMB */
	/* todo: investigate necessary direction - use DS now */
	memset(&dsl_driver_data.framing_parameter_status, 0x00, sizeof(DSL_G997_FramingParameterStatus_t));
	dsl_driver_data.framing_parameter_status.nChannel = 0;
	dsl_driver_data.framing_parameter_status.nDirection = DSL_DOWNSTREAM;
	ret = ioctl(fd, DSL_FIO_G997_FRAMING_PARAMETER_STATUS_GET,
		    (int)&dsl_driver_data.framing_parameter_status);
	if ((ret < 0) && (dsl_driver_data.framing_parameter_status.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.LPATH:"
			       "error code %d\n", dsl_driver_data.framing_parameter_status.accessCtl.nReturn);
	} else {
		obj->lpath = dsl_driver_data.framing_parameter_status.data.nLPATH;
		obj->intlvdepth = dsl_driver_data.framing_parameter_status.data.nINTLVDEPTH;
		obj->intlvblock = dsl_driver_data.framing_parameter_status.data.nINTLVBLOCK;
		obj->nfec = dsl_driver_data.framing_parameter_status.data.nNFEC;
		obj->rfec = dsl_driver_data.framing_parameter_status.data.nRFEC;
		obj->lsymb = dsl_driver_data.framing_parameter_status.data.nLSYMB;
	}

	/* Device.DSL.Channel.ActualInterleavingDelay */
	/* Device.DSL.Channel.ACTINP */
	/* Device.DSL.Channel.DownstreamCurrRate */
	/* Device.DSL.Channel.ACTNDRds */
	/* Device.DSL.Channel.ACTINPREINds */
	memset(&dsl_driver_data.channel_status, 0x00, sizeof(DSL_G997_ChannelStatus_t));
	dsl_driver_data.channel_status.nChannel = 0;
	dsl_driver_data.channel_status.nDirection = DSL_DOWNSTREAM;
	ret = ioctl(fd, DSL_FIO_G997_CHANNEL_STATUS_GET, (int)&dsl_driver_data.channel_status);
	if ((ret < 0) && (dsl_driver_data.channel_status.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.ActualInterleavingDelay:"
			       "error code %d\n", dsl_driver_data.channel_status.accessCtl.nReturn);
	} else {
		obj->actual_interleaving_delay = dsl_driver_data.channel_status.data.ActualInterleaveDelay;
		obj->actinp = dsl_driver_data.channel_status.data.ActualImpulseNoiseProtection;
		obj->downstream_curr_rate = dsl_driver_data.channel_status.data.ActualDataRate;
		obj->actndr_ds = dsl_driver_data.channel_status.data.ActualNetDataRate;
		obj->actinprein_ds = dsl_driver_data.channel_status.data.ActualImpulseNoiseProtectionRein;
	}

	/* Device.DSL.Channel.UpstreamCurrRate */
	/* Device.DSL.Channel.ACTNDRus */
	/* Device.DSL.Channel.ACTINPREINus */
	memset(&dsl_driver_data.channel_status, 0x00, sizeof(DSL_G997_ChannelStatus_t));
	dsl_driver_data.channel_status.nChannel = 0;
	dsl_driver_data.channel_status.nDirection = DSL_UPSTREAM;
	ret = ioctl(fd, DSL_FIO_G997_CHANNEL_STATUS_GET, (int)&dsl_driver_data.channel_status);
	if ((ret < 0) && (dsl_driver_data.channel_status.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.UpstreamCurrRate:"
			       "error code %d\n", dsl_driver_data.channel_status.accessCtl.nReturn);
	} else {
		obj->upstream_curr_rate = dsl_driver_data.channel_status.data.ActualDataRate;
		obj->actndr_us = dsl_driver_data.channel_status.data.ActualNetDataRate;
		obj->actinprein_us = dsl_driver_data.channel_status.data.ActualImpulseNoiseProtectionRein;
	}

	/* Device.DSL.Channel.INPREPORT */
	/* todo: investigate mapping */
	obj->inpreport = 0;
	fapi_dsl_close_device(fd);
	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_channel_set(struct fapi_dsl_ctx *ctx, const struct dsl_fapi_channel_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	union dsl_driver_data_types {
		DSL_AutobootControl_t autoboot_cotrol;
	} dsl_driver_data;

	int ret, fd;

	if (ctx == NULL)
		return FAPI_DSL_STATUS_ERROR;

	if (obj == NULL)
		return FAPI_DSL_STATUS_ERROR;

	fd = fapi_dsl_open_device(ctx->entity);
	if (fd < 0)
		return FAPI_DSL_STATUS_ERROR;

	/* Device.DSL.Channel.Enable */
	memset(&dsl_driver_data.autoboot_cotrol, 0x00, sizeof(DSL_AutobootControl_t));
	if (obj->enable) {
		dsl_driver_data.autoboot_cotrol.data.nCommand = DSL_AUTOBOOT_CTRL_START;
	} else {
		dsl_driver_data.autoboot_cotrol.data.nCommand = DSL_AUTOBOOT_CTRL_STOP;
	}

	ret = ioctl(fd, DSL_FIO_AUTOBOOT_CONTROL_SET, (int)&dsl_driver_data.autoboot_cotrol);

	if ((ret < 0) && (dsl_driver_data.autoboot_cotrol.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.Enable: error code %d\n",
			       dsl_driver_data.autoboot_cotrol.accessCtl.nReturn);
	}

	fapi_dsl_close_device(fd);
	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_channel_stats_get(struct fapi_dsl_ctx *ctx,
						struct dsl_fapi_channel_stats_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;
	union dsl_driver_data_types {
		DSL_PM_ChannelCountersTotal_t pm_channel_counters_total;
		DSL_PM_LineInitCounters_t pm_line_init_counters;
		DSL_PM_ChannelCounters_t pm_channel_counters;
	} dsl_driver_data;
	int ret, fd;

	if (ctx == NULL)
		return FAPI_DSL_STATUS_ERROR;

	if (obj == NULL)
		return FAPI_DSL_STATUS_ERROR;

	fd = fapi_dsl_open_device(ctx->entity);
	if (fd < 0)
		return FAPI_DSL_STATUS_ERROR;

	memset(obj, 0x00, sizeof(struct dsl_fapi_channel_stats_obj));

	/* Device.DSL.Channel.Stats.TotalStart */
	memset(&dsl_driver_data.pm_channel_counters_total, 0x00, sizeof(DSL_PM_ChannelCountersTotal_t));

	dsl_driver_data.pm_channel_counters_total.nChannel = 0;
	/* todoL clarify direction */
	dsl_driver_data.pm_channel_counters_total.nDirection = DSL_NEAR_END;
	ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_TOTAL_GET,
		    (int)&dsl_driver_data.pm_channel_counters_total);

	if ((ret < 0) && (dsl_driver_data.pm_channel_counters_total.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.Stats.TotalStart: error code %d\n",
			       dsl_driver_data.pm_channel_counters_total.accessCtl.nReturn);
	} else {
		obj->total_start = dsl_driver_data.pm_channel_counters_total.total.nElapsedTime;
	}

	/* Device.DSL.Channel.Stats.ShowtimeStart */
	memset(&dsl_driver_data.pm_channel_counters, 0x00, sizeof(DSL_PM_ChannelCounters_t));

	dsl_driver_data.pm_channel_counters.nChannel = 0;
	dsl_driver_data.pm_channel_counters.nDirection = DSL_NEAR_END;
	dsl_driver_data.pm_channel_counters.nHistoryInterval = 0;
	ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_SHOWTIME_GET, (int)&dsl_driver_data.pm_channel_counters);

	if ((ret < 0) && (dsl_driver_data.pm_channel_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.Stats.ShowtimeStart: error code %d\n",
			       dsl_driver_data.pm_channel_counters.accessCtl.nReturn);
	} else {
		obj->showtime_start = dsl_driver_data.pm_channel_counters.interval.nElapsedTime;
	}

	/* Device.DSL.Channel.Stats.LastShowtimeStart */
	memset(&dsl_driver_data.pm_channel_counters, 0x00, sizeof(DSL_PM_ChannelCounters_t));

	dsl_driver_data.pm_channel_counters.nChannel = 0;
	dsl_driver_data.pm_channel_counters.nDirection = DSL_NEAR_END;
	dsl_driver_data.pm_channel_counters.nHistoryInterval = 1;
	ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_SHOWTIME_GET, (int)&dsl_driver_data.pm_channel_counters);

	if ((ret < 0) && (dsl_driver_data.pm_channel_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.Stats.LastShowtimeStart: error code %d\n",
			       dsl_driver_data.pm_channel_counters.accessCtl.nReturn);
	} else {
		obj->last_showtime_start = dsl_driver_data.pm_channel_counters.interval.nElapsedTime;
	}

	/* Device.DSL.Channel.Stats.CurrentDayStart */
	memset(&dsl_driver_data.pm_channel_counters, 0x00, sizeof(DSL_PM_ChannelCounters_t));

	dsl_driver_data.pm_channel_counters.nChannel = 0;
	dsl_driver_data.pm_channel_counters.nDirection = DSL_NEAR_END;
	dsl_driver_data.pm_channel_counters.nHistoryInterval = 0;
	ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_1DAY_GET, (int)&dsl_driver_data.pm_channel_counters);

	if ((ret < 0) && (dsl_driver_data.pm_channel_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.Stats.CurrentDayStart: error code %d\n",
			       dsl_driver_data.pm_channel_counters.accessCtl.nReturn);
	} else {
		obj->current_day_start = dsl_driver_data.pm_channel_counters.interval.nElapsedTime;
	}

	/* Device.DSL.Channel.Stats.QuarterHourStart */
	memset(&dsl_driver_data.pm_channel_counters, 0x00, sizeof(DSL_PM_ChannelCounters_t));

	dsl_driver_data.pm_channel_counters.nChannel = 0;
	dsl_driver_data.pm_channel_counters.nDirection = DSL_NEAR_END;
	dsl_driver_data.pm_channel_counters.nHistoryInterval = 0;
	ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_15MIN_GET, (int)&dsl_driver_data.pm_channel_counters);

	if ((ret < 0) && (dsl_driver_data.pm_channel_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.Stats.QuarterHourStart: error code %d\n",
			       dsl_driver_data.pm_channel_counters.accessCtl.nReturn);
	} else {
		obj->quarter_hour_start = dsl_driver_data.pm_channel_counters.interval.nElapsedTime;
	}
	fapi_dsl_close_device(fd);
	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_channel_stats_set(struct fapi_dsl_ctx *ctx,
						const struct dsl_fapi_channel_stats_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_channel_stats_total_get(struct fapi_dsl_ctx *ctx,
						      struct dsl_fapi_channel_stats_interval_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;
	union dsl_driver_data_types {
		DSL_PM_ChannelCountersTotal_t channel_counters_total;
		DSL_PM_DataPathCountersTotal_t data_path_counters_total;
	} dsl_driver_data;
	int ret, fd;

	if (ctx == NULL)
		return FAPI_DSL_STATUS_ERROR;

	if (obj == NULL)
		return FAPI_DSL_STATUS_ERROR;

	fd = fapi_dsl_open_device(ctx->entity);
	if (fd < 0)
		return FAPI_DSL_STATUS_ERROR;

	memset(obj, 0x00, sizeof(struct dsl_fapi_channel_stats_interval_obj));

	/* Device.DSL.Channel.Stats.Total.XTURFECErrors */
	/* Device.DSL.Channel.Stats.Total.XTURCRCErrors */
	memset(&dsl_driver_data.channel_counters_total, 0x00, sizeof(DSL_PM_ChannelCountersTotal_t));

	dsl_driver_data.channel_counters_total.nChannel = 0;
	dsl_driver_data.channel_counters_total.nDirection = DSL_NEAR_END;

	ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_TOTAL_GET, (int)&dsl_driver_data.channel_counters_total);

	if ((ret < 0) && (dsl_driver_data.channel_counters_total.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.Stats.Total.XTURFECErrors: error code %d\n",
			       dsl_driver_data.channel_counters_total.accessCtl.nReturn);
	} else {
		obj->xtu_rfec_errors = dsl_driver_data.channel_counters_total.data.nFEC;
		obj->xtu_rcrc_errors = dsl_driver_data.channel_counters_total.data.nCodeViolations;
	}

	/* Device.DSL.Channel.Stats.Total.XTUCFECErrors */
	/* Device.DSL.Channel.Stats.Total.XTUCCRCErrors */
	memset(&dsl_driver_data.channel_counters_total, 0x00, sizeof(DSL_PM_ChannelCountersTotal_t));

	dsl_driver_data.channel_counters_total.nChannel = 0;
	dsl_driver_data.channel_counters_total.nDirection = DSL_FAR_END;

	ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_TOTAL_GET, (int)&dsl_driver_data.channel_counters_total);

	if ((ret < 0) && (dsl_driver_data.channel_counters_total.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.Stats.Total.XTUCFECErrors: error code %d\n",
			       dsl_driver_data.channel_counters_total.accessCtl.nReturn);
	} else {
		obj->xtu_cfec_errors = dsl_driver_data.channel_counters_total.data.nFEC;
		obj->xtu_ccrc_errors = dsl_driver_data.channel_counters_total.data.nCodeViolations;
	}

	/* Device.DSL.Channel.Stats.Total.XTURHECErrors */
	memset(&dsl_driver_data.data_path_counters_total, 0x00, sizeof(DSL_PM_DataPathCountersTotal_t));

	dsl_driver_data.data_path_counters_total.nChannel = 0;
	dsl_driver_data.data_path_counters_total.nDirection = DSL_NEAR_END;

	ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_COUNTERS_TOTAL_GET,
		    (int)&dsl_driver_data.data_path_counters_total);

	if ((ret < 0) && (dsl_driver_data.data_path_counters_total.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.Stats.Total.XTURHECErrors: error code %d\n",
			       dsl_driver_data.data_path_counters_total.accessCtl.nReturn);
	} else {
		obj->xtu_rhec_errors = dsl_driver_data.data_path_counters_total.data.nHEC;
	}

	/* Device.DSL.Channel.Stats.Total.XTUCHECErrors */
	memset(&dsl_driver_data.data_path_counters_total, 0x00, sizeof(DSL_PM_DataPathCountersTotal_t));

	dsl_driver_data.data_path_counters_total.nChannel = 0;
	dsl_driver_data.data_path_counters_total.nDirection = DSL_FAR_END;

	ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_COUNTERS_TOTAL_GET,
		    (int)&dsl_driver_data.data_path_counters_total);

	if ((ret < 0) && (dsl_driver_data.data_path_counters_total.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.Stats.Total.XTURHECErrors: error code %d\n",
			       dsl_driver_data.data_path_counters_total.accessCtl.nReturn);
	} else {
		obj->xtu_chec_errors = dsl_driver_data.data_path_counters_total.data.nHEC;
	}
	fapi_dsl_close_device(fd);
	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_channel_stats_total_set(struct fapi_dsl_ctx *ctx,
						      const struct dsl_fapi_channel_stats_interval_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_channel_stats_showtime_get(struct fapi_dsl_ctx *ctx,
							 struct dsl_fapi_channel_stats_interval_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;
	union dsl_driver_data_types {
		DSL_PM_ChannelCounters_t channel_counters;
		DSL_PM_DataPathCounters_t data_path_counters;
	} dsl_driver_data;
	int ret, fd;

	if (ctx == NULL)
		return FAPI_DSL_STATUS_ERROR;

	if (obj == NULL)
		return FAPI_DSL_STATUS_ERROR;

	fd = fapi_dsl_open_device(ctx->entity);
	if (fd < 0)
		return FAPI_DSL_STATUS_ERROR;

	memset(obj, 0x00, sizeof(struct dsl_fapi_channel_stats_interval_obj));

	/* Device.DSL.Channel.Stats.Showtime.XTURFECErrors */
	/* Device.DSL.Channel.Stats.Showtime.XTURCRCErrors */
	memset(&dsl_driver_data.channel_counters, 0x00, sizeof(DSL_PM_ChannelCounters_t));

	dsl_driver_data.channel_counters.nChannel = 0;
	dsl_driver_data.channel_counters.nHistoryInterval = 0;
	dsl_driver_data.channel_counters.nDirection = DSL_NEAR_END;

	ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_SHOWTIME_GET, (int)&dsl_driver_data.channel_counters);

	if ((ret < 0) && (dsl_driver_data.channel_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.Stats.Showtime.XTURFECErrors: error code %d\n",
			       dsl_driver_data.channel_counters.accessCtl.nReturn);
	} else {
		obj->xtu_rfec_errors = dsl_driver_data.channel_counters.data.nFEC;
		obj->xtu_rcrc_errors = dsl_driver_data.channel_counters.data.nCodeViolations;
	}

	/* Device.DSL.Channel.Stats.Showtime.XTUCFECErrors */
	/* Device.DSL.Channel.Stats.Showtime.XTUCCRCErrors */
	memset(&dsl_driver_data.channel_counters, 0x00, sizeof(DSL_PM_ChannelCounters_t));

	dsl_driver_data.channel_counters.nChannel = 0;
	dsl_driver_data.channel_counters.nHistoryInterval = 0;
	dsl_driver_data.channel_counters.nDirection = DSL_FAR_END;

	ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_SHOWTIME_GET, (int)&dsl_driver_data.channel_counters);

	if ((ret < 0) && (dsl_driver_data.channel_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.Stats.Showtime.XTUCFECErrors: error code %d\n",
			       dsl_driver_data.channel_counters.accessCtl.nReturn);
	} else {
		obj->xtu_cfec_errors = dsl_driver_data.channel_counters.data.nFEC;
		obj->xtu_ccrc_errors = dsl_driver_data.channel_counters.data.nCodeViolations;
	}

	/* Device.DSL.Channel.Stats.Showtime.XTURHECErrors */
	memset(&dsl_driver_data.data_path_counters, 0x00, sizeof(DSL_PM_DataPathCounters_t));

	dsl_driver_data.data_path_counters.nChannel = 0;
	dsl_driver_data.data_path_counters.nHistoryInterval = 0;
	dsl_driver_data.data_path_counters.nDirection = DSL_NEAR_END;

	ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_COUNTERS_SHOWTIME_GET, (int)&dsl_driver_data.data_path_counters);

	if ((ret < 0) && (dsl_driver_data.data_path_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.Stats.Showtime.XTURHECErrors: error code %d\n",
			       dsl_driver_data.data_path_counters.accessCtl.nReturn);
	} else {
		obj->xtu_rhec_errors = dsl_driver_data.data_path_counters.data.nHEC;
	}

	/* Device.DSL.Channel.Stats.Showtime.XTUCHECErrors */
	memset(&dsl_driver_data.data_path_counters, 0x00, sizeof(DSL_PM_DataPathCounters_t));

	dsl_driver_data.data_path_counters.nChannel = 0;
	dsl_driver_data.data_path_counters.nHistoryInterval = 0;
	dsl_driver_data.data_path_counters.nDirection = DSL_FAR_END;

	ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_COUNTERS_SHOWTIME_GET, (int)&dsl_driver_data.data_path_counters);

	if ((ret < 0) && (dsl_driver_data.data_path_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.Stats.Showtime.XTURHECErrors: error code %d\n",
			       dsl_driver_data.data_path_counters.accessCtl.nReturn);
	} else {
		obj->xtu_chec_errors = dsl_driver_data.data_path_counters.data.nHEC;
	}
	fapi_dsl_close_device(fd);
	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_channel_stats_showtime_set(struct fapi_dsl_ctx *ctx, const struct dsl_fapi_channel_stats_interval_obj
							 *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_channel_stats_last_showtime_get(struct fapi_dsl_ctx *ctx,
							      struct dsl_fapi_channel_stats_interval_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;
	union dsl_driver_data_types {
		DSL_PM_ChannelCounters_t channel_counters;
		DSL_PM_DataPathCounters_t data_path_counters;
	} dsl_driver_data;
	int ret, fd;

	if (ctx == NULL)
		return FAPI_DSL_STATUS_ERROR;

	if (obj == NULL)
		return FAPI_DSL_STATUS_ERROR;

	fd = fapi_dsl_open_device(ctx->entity);
	if (fd < 0)
		return FAPI_DSL_STATUS_ERROR;

	memset(obj, 0x00, sizeof(struct dsl_fapi_channel_stats_interval_obj));

	/* Device.DSL.Channel.Stats.LastShowtime.XTURFECErrors */
	/* Device.DSL.Channel.Stats.LastShowtime.XTURCRCErrors */
	memset(&dsl_driver_data.channel_counters, 0x00, sizeof(DSL_PM_ChannelCounters_t));

	dsl_driver_data.channel_counters.nChannel = 0;
	dsl_driver_data.channel_counters.nHistoryInterval = 1;
	dsl_driver_data.channel_counters.nDirection = DSL_NEAR_END;

	ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_SHOWTIME_GET, (int)&dsl_driver_data.channel_counters);

	if ((ret < 0) && (dsl_driver_data.channel_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR
		    ("DSL FAPI: Device.DSL.Channel.Stats.LastShowtime.XTURFECErrors: error code %d\n",
		     dsl_driver_data.channel_counters.accessCtl.nReturn);
	} else {
		obj->xtu_rfec_errors = dsl_driver_data.channel_counters.data.nFEC;
		obj->xtu_rcrc_errors = dsl_driver_data.channel_counters.data.nCodeViolations;
	}

	/* Device.DSL.Channel.Stats.LastShowtime.XTUCFECErrors */
	/* Device.DSL.Channel.Stats.LastShowtime.XTUCCRCErrors */
	memset(&dsl_driver_data.channel_counters, 0x00, sizeof(DSL_PM_ChannelCounters_t));

	dsl_driver_data.channel_counters.nChannel = 0;
	dsl_driver_data.channel_counters.nHistoryInterval = 1;
	dsl_driver_data.channel_counters.nDirection = DSL_FAR_END;

	ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_SHOWTIME_GET, (int)&dsl_driver_data.channel_counters);

	if ((ret < 0) && (dsl_driver_data.channel_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR
		    ("DSL FAPI: Device.DSL.Channel.Stats.LastShowtime.XTUCFECErrors: error code %d\n",
		     dsl_driver_data.channel_counters.accessCtl.nReturn);
	} else {
		obj->xtu_cfec_errors = dsl_driver_data.channel_counters.data.nFEC;
		obj->xtu_ccrc_errors = dsl_driver_data.channel_counters.data.nCodeViolations;
	}

	/* Device.DSL.Channel.Stats.LastShowtime.XTURHECErrors */
	memset(&dsl_driver_data.data_path_counters, 0x00, sizeof(DSL_PM_DataPathCounters_t));

	dsl_driver_data.data_path_counters.nChannel = 0;
	dsl_driver_data.data_path_counters.nHistoryInterval = 1;
	dsl_driver_data.data_path_counters.nDirection = DSL_NEAR_END;

	ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_COUNTERS_SHOWTIME_GET, (int)&dsl_driver_data.data_path_counters);

	if ((ret < 0) && (dsl_driver_data.data_path_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR
		    ("DSL FAPI: Device.DSL.Channel.Stats.LastShowtime.XTURHECErrors: error code %d\n",
		     dsl_driver_data.data_path_counters.accessCtl.nReturn);
	} else {
		obj->xtu_rhec_errors = dsl_driver_data.data_path_counters.data.nHEC;
	}

	/* Device.DSL.Channel.Stats.LastShowtime.XTUCHECErrors */
	memset(&dsl_driver_data.data_path_counters, 0x00, sizeof(DSL_PM_DataPathCounters_t));

	dsl_driver_data.data_path_counters.nChannel = 0;
	dsl_driver_data.data_path_counters.nHistoryInterval = 1;
	dsl_driver_data.data_path_counters.nDirection = DSL_FAR_END;

	ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_COUNTERS_SHOWTIME_GET, (int)&dsl_driver_data.data_path_counters);

	if ((ret < 0) && (dsl_driver_data.data_path_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR
		    ("DSL FAPI: Device.DSL.Channel.Stats.LastShowtime.XTURHECErrors: error code %d\n",
		     dsl_driver_data.data_path_counters.accessCtl.nReturn);
	} else {
		obj->xtu_chec_errors = dsl_driver_data.data_path_counters.data.nHEC;
	}
	fapi_dsl_close_device(fd);
	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_channel_stats_last_showtime_set(struct fapi_dsl_ctx *ctx, const struct dsl_fapi_channel_stats_interval_obj
							      *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_channel_stats_current_day_get(struct fapi_dsl_ctx *ctx,
							    struct dsl_fapi_channel_stats_interval_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;
	union dsl_driver_data_types {
		DSL_PM_ChannelCounters_t channel_counters;
		DSL_PM_DataPathCounters_t data_path_counters;
	} dsl_driver_data;
	int ret, fd;

	if (ctx == NULL)
		return FAPI_DSL_STATUS_ERROR;

	if (obj == NULL)
		return FAPI_DSL_STATUS_ERROR;

	fd = fapi_dsl_open_device(ctx->entity);
	if (fd < 0)
		return FAPI_DSL_STATUS_ERROR;

	memset(obj, 0x00, sizeof(struct dsl_fapi_channel_stats_interval_obj));

	/* Device.DSL.Channel.Stats.CurrentDay.XTURFECErrors */
	/* Device.DSL.Channel.Stats.CurrentDay.XTURCRCErrors */
	memset(&dsl_driver_data.channel_counters, 0x00, sizeof(DSL_PM_ChannelCounters_t));

	dsl_driver_data.channel_counters.nChannel = 0;
	dsl_driver_data.channel_counters.nHistoryInterval = 0;
	dsl_driver_data.channel_counters.nDirection = DSL_NEAR_END;

	ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_1DAY_GET, (int)&dsl_driver_data.channel_counters);

	if ((ret < 0) && (dsl_driver_data.channel_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.Stats.CurrentDay.XTURFECErrors: error code %d\n",
			       dsl_driver_data.channel_counters.accessCtl.nReturn);
	} else {
		obj->xtu_rfec_errors = dsl_driver_data.channel_counters.data.nFEC;
		obj->xtu_rcrc_errors = dsl_driver_data.channel_counters.data.nCodeViolations;
	}

	/* Device.DSL.Channel.Stats.CurrentDay.XTUCFECErrors */
	/* Device.DSL.Channel.Stats.CurrentDay.XTUCCRCErrors */
	memset(&dsl_driver_data.channel_counters, 0x00, sizeof(DSL_PM_ChannelCounters_t));

	dsl_driver_data.channel_counters.nChannel = 0;
	dsl_driver_data.channel_counters.nHistoryInterval = 0;
	dsl_driver_data.channel_counters.nDirection = DSL_FAR_END;

	ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_1DAY_GET, (int)&dsl_driver_data.channel_counters);

	if ((ret < 0) && (dsl_driver_data.channel_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.Stats.CurrentDay.XTUCFECErrors: error code %d\n",
			       dsl_driver_data.channel_counters.accessCtl.nReturn);
	} else {
		obj->xtu_cfec_errors = dsl_driver_data.channel_counters.data.nFEC;
		obj->xtu_ccrc_errors = dsl_driver_data.channel_counters.data.nCodeViolations;
	}

	/* Device.DSL.Channel.Stats.CurrentDay.XTURHECErrors */
	memset(&dsl_driver_data.data_path_counters, 0x00, sizeof(DSL_PM_DataPathCounters_t));

	dsl_driver_data.data_path_counters.nChannel = 0;
	dsl_driver_data.data_path_counters.nHistoryInterval = 0;
	dsl_driver_data.data_path_counters.nDirection = DSL_NEAR_END;

	ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_COUNTERS_1DAY_GET, (int)&dsl_driver_data.data_path_counters);

	if ((ret < 0) && (dsl_driver_data.data_path_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.Stats.CurrentDay.XTURHECErrors: error code %d\n",
			       dsl_driver_data.data_path_counters.accessCtl.nReturn);
	} else {
		obj->xtu_rhec_errors = dsl_driver_data.data_path_counters.data.nHEC;
	}

	/* Device.DSL.Channel.Stats.CurrentDay.XTUCHECErrors */
	memset(&dsl_driver_data.data_path_counters, 0x00, sizeof(DSL_PM_DataPathCounters_t));

	dsl_driver_data.data_path_counters.nChannel = 0;
	dsl_driver_data.data_path_counters.nHistoryInterval = 0;
	dsl_driver_data.data_path_counters.nDirection = DSL_FAR_END;

	ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_COUNTERS_1DAY_GET, (int)&dsl_driver_data.data_path_counters);

	if ((ret < 0) && (dsl_driver_data.data_path_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.Stats.CurrentDay.XTURHECErrors: error code %d\n",
			       dsl_driver_data.data_path_counters.accessCtl.nReturn);
	} else {
		obj->xtu_chec_errors = dsl_driver_data.data_path_counters.data.nHEC;
	}
	fapi_dsl_close_device(fd);
	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_channel_stats_current_day_set(struct fapi_dsl_ctx *ctx, const struct dsl_fapi_channel_stats_interval_obj
							    *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_channel_stats_quarter_hour_get(struct fapi_dsl_ctx *ctx,
							     struct dsl_fapi_channel_stats_interval_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;
	union dsl_driver_data_types {
		DSL_PM_ChannelCounters_t channel_counters;
		DSL_PM_DataPathCounters_t data_path_counters;
	} dsl_driver_data;
	int ret, fd;

	if (ctx == NULL)
		return FAPI_DSL_STATUS_ERROR;

	if (obj == NULL)
		return FAPI_DSL_STATUS_ERROR;

	fd = fapi_dsl_open_device(ctx->entity);
	if (fd < 0)
		return FAPI_DSL_STATUS_ERROR;

	memset(obj, 0x00, sizeof(struct dsl_fapi_channel_stats_interval_obj));

	/* Device.DSL.Channel.Stats.QuarterHour.XTURFECErrors */
	/* Device.DSL.Channel.Stats.QuarterHour.XTURCRCErrors */
	memset(&dsl_driver_data.channel_counters, 0x00, sizeof(DSL_PM_ChannelCounters_t));

	dsl_driver_data.channel_counters.nChannel = 0;
	dsl_driver_data.channel_counters.nHistoryInterval = 0;
	dsl_driver_data.channel_counters.nDirection = DSL_NEAR_END;

	ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_15MIN_GET, (int)&dsl_driver_data.channel_counters);

	if ((ret < 0) && (dsl_driver_data.channel_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR
		    ("DSL FAPI: Device.DSL.Channel.Stats.QuarterHour.XTURFECErrors: error code %d\n",
		     dsl_driver_data.channel_counters.accessCtl.nReturn);
	} else {
		obj->xtu_rfec_errors = dsl_driver_data.channel_counters.data.nFEC;
		obj->xtu_rcrc_errors = dsl_driver_data.channel_counters.data.nCodeViolations;
	}

	/* Device.DSL.Channel.Stats.QuarterHour.XTUCFECErrors */
	/* Device.DSL.Channel.Stats.QuarterHour.XTUCCRCErrors */
	memset(&dsl_driver_data.channel_counters, 0x00, sizeof(DSL_PM_ChannelCounters_t));

	dsl_driver_data.channel_counters.nChannel = 0;
	dsl_driver_data.channel_counters.nHistoryInterval = 0;
	dsl_driver_data.channel_counters.nDirection = DSL_FAR_END;

	ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_15MIN_GET, (int)&dsl_driver_data.channel_counters);

	if ((ret < 0) && (dsl_driver_data.channel_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR
		    ("DSL FAPI: Device.DSL.Channel.Stats.QuarterHour.XTUCFECErrors: error code %d\n",
		     dsl_driver_data.channel_counters.accessCtl.nReturn);
	} else {
		obj->xtu_cfec_errors = dsl_driver_data.channel_counters.data.nFEC;
		obj->xtu_ccrc_errors = dsl_driver_data.channel_counters.data.nCodeViolations;
	}

	/* Device.DSL.Channel.Stats.QuarterHour.XTURHECErrors */
	memset(&dsl_driver_data.data_path_counters, 0x00, sizeof(DSL_PM_DataPathCounters_t));

	dsl_driver_data.data_path_counters.nChannel = 0;
	dsl_driver_data.data_path_counters.nHistoryInterval = 0;
	dsl_driver_data.data_path_counters.nDirection = DSL_NEAR_END;

	ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_COUNTERS_15MIN_GET, (int)&dsl_driver_data.data_path_counters);

	if ((ret < 0) && (dsl_driver_data.data_path_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR
		    ("DSL FAPI: Device.DSL.Channel.Stats.QuarterHour.XTURHECErrors: error code %d\n",
		     dsl_driver_data.data_path_counters.accessCtl.nReturn);
	} else {
		obj->xtu_rhec_errors = dsl_driver_data.data_path_counters.data.nHEC;
	}

	/* Device.DSL.Channel.Stats.QuarterHour.XTUCHECErrors */
	memset(&dsl_driver_data.data_path_counters, 0x00, sizeof(DSL_PM_DataPathCounters_t));

	dsl_driver_data.data_path_counters.nChannel = 0;
	dsl_driver_data.data_path_counters.nHistoryInterval = 0;
	dsl_driver_data.data_path_counters.nDirection = DSL_FAR_END;

	ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_COUNTERS_15MIN_GET, (int)&dsl_driver_data.data_path_counters);

	if ((ret < 0) && (dsl_driver_data.data_path_counters.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR
		    ("DSL FAPI: Device.DSL.Channel.Stats.QuarterHour.XTURHECErrors: error code %d\n",
		     dsl_driver_data.data_path_counters.accessCtl.nReturn);
	} else {
		obj->xtu_chec_errors = dsl_driver_data.data_path_counters.data.nHEC;
	}
	fapi_dsl_close_device(fd);
	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_bonding_get(struct fapi_dsl_ctx *ctx, struct dsl_fapi_bonding_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	memset(obj, 0x00, sizeof(struct dsl_fapi_bonding_obj));

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_bonding_set(struct fapi_dsl_ctx *ctx, struct dsl_fapi_bonding_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	memset(obj, 0x00, sizeof(struct dsl_fapi_bonding_obj));

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_bonding_channel_get(struct fapi_dsl_ctx *ctx,
						  struct dsl_fapi_bonding_channel_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	memset(obj, 0x00, sizeof(struct dsl_fapi_bonding_channel_obj));

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_bonding_channel_set(struct fapi_dsl_ctx *ctx,
						  struct dsl_fapi_bonding_channel_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	memset(obj, 0x00, sizeof(struct dsl_fapi_bonding_channel_obj));

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_bonding_channel_ethernet_get(struct fapi_dsl_ctx *ctx,
							   struct dsl_fapi_bonding_channel_ethernet_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	memset(obj, 0x00, sizeof(struct dsl_fapi_bonding_channel_ethernet_obj));

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_bonding_channel_ethernet_set(struct fapi_dsl_ctx *ctx,
							   struct dsl_fapi_bonding_channel_ethernet_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	memset(obj, 0x00, sizeof(struct dsl_fapi_bonding_channel_ethernet_obj));

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_bonding_channel_ethernet_stats_get(struct fapi_dsl_ctx *ctx, struct
								 dsl_fapi_bonding_channel_ethernet_stats_obj
								 *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	memset(obj, 0x00, sizeof(struct dsl_fapi_bonding_channel_ethernet_stats_obj));

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_bonding_channel_ethernet_stats_set(struct fapi_dsl_ctx *ctx, struct
								 dsl_fapi_bonding_channel_ethernet_stats_obj
								 *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	memset(obj, 0x00, sizeof(struct dsl_fapi_bonding_channel_ethernet_stats_obj));

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_bonding_stats_get(struct fapi_dsl_ctx *ctx,
						struct dsl_fapi_bonding_stats_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	memset(obj, 0x00, sizeof(struct dsl_fapi_bonding_stats_obj));

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_bonding_stats_set(struct fapi_dsl_ctx *ctx,
						struct dsl_fapi_bonding_stats_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	memset(obj, 0x00, sizeof(struct dsl_fapi_bonding_stats_obj));

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_bonding_stats_total_get(struct fapi_dsl_ctx *ctx,
						      struct dsl_fapi_bonding_stats_total_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	memset(obj, 0x00, sizeof(struct dsl_fapi_bonding_stats_total_obj));

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_bonding_stats_total_set(struct fapi_dsl_ctx *ctx,
						      struct dsl_fapi_bonding_stats_total_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	memset(obj, 0x00, sizeof(struct dsl_fapi_bonding_stats_total_obj));

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_bonding_stats_day_get(struct fapi_dsl_ctx *ctx,
						    struct dsl_fapi_bonding_stats_day_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	memset(obj, 0x00, sizeof(struct dsl_fapi_bonding_stats_day_obj));

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_bonding_stats_day_set(struct fapi_dsl_ctx *ctx,
						    struct dsl_fapi_bonding_stats_day_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	memset(obj, 0x00, sizeof(struct dsl_fapi_bonding_stats_day_obj));

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_bonding_stats_hour_get(struct fapi_dsl_ctx *ctx,
						     struct dsl_fapi_bonding_stats_hour_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	memset(obj, 0x00, sizeof(struct dsl_fapi_bonding_stats_hour_obj));

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_bonding_stats_hour_set(struct fapi_dsl_ctx *ctx,
						     struct dsl_fapi_bonding_stats_hour_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	memset(obj, 0x00, sizeof(struct dsl_fapi_bonding_stats_hour_obj));

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_bonding_ethernet_get(struct fapi_dsl_ctx *ctx,
						   struct dsl_fapi_bonding_ethernet_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	memset(obj, 0x00, sizeof(struct dsl_fapi_bonding_ethernet_obj));

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_bonding_ethernet_set(struct fapi_dsl_ctx *ctx,
						   struct dsl_fapi_bonding_ethernet_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	memset(obj, 0x00, sizeof(struct dsl_fapi_bonding_ethernet_obj));

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_bonding_ethernet_stats_get(struct fapi_dsl_ctx *ctx,
							 struct dsl_fapi_bonding_ethernet_stats_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	memset(obj, 0x00, sizeof(struct dsl_fapi_bonding_ethernet_stats_obj));

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_bonding_ethernet_stats_set(struct fapi_dsl_ctx *ctx,
							 struct dsl_fapi_bonding_ethernet_stats_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	memset(obj, 0x00, sizeof(struct dsl_fapi_bonding_ethernet_stats_obj));

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_channel_stats_quarter_hour_set(struct fapi_dsl_ctx *ctx, const struct dsl_fapi_channel_stats_interval_obj
							     *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_channel_stats_last_quarter_hour_set(struct fapi_dsl_ctx *ctx, const struct
								  dsl_fapi_channel_stats_interval_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_x_lantiq_com_config_get(struct fapi_dsl_ctx *ctx,
						      struct dsl_fapi_x_lantiq_com_config_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;

	fapi_dsl_memcpy_s(obj, sizeof(struct dsl_fapi_x_lantiq_com_config_obj),
			  &x_lantiq_com_config_obj, sizeof(struct dsl_fapi_x_lantiq_com_config_obj));

	return fapi_status;
}

enum fapi_dsl_status fapi_dsl_x_lantiq_com_config_set(struct fapi_dsl_ctx *ctx,
						      const struct dsl_fapi_x_lantiq_com_config_obj *obj)
{
	enum fapi_dsl_status fapi_status = FAPI_DSL_STATUS_SUCCESS;
	union dsl_driver_data_types {
		DSL_LineFeature_t line_feature;
		DSL_SystemInterfaceConfig_t system_interface_config;
		DSL_G997_XTUSystemEnabling_t xtu_system_enabling;
		DSL_AutobootControl_t autoboot_cotrol;
		DSL_G997_RateAdaptationConfig_t rate_adaptation_config;
	} dsl_driver_data;

	DSL_FirmwareType_t next_mode;
	DSL_ActivationSequence_t activation_sequence;
	DSL_ActivationMode_t activation_mode;
	IOCTL_MEI_dsmConfig_t vector_control;
	DSL_TcLayerSelection_t tc_layer_a, tc_layer_v;
	char *str_ptr;
	char *token;
	char buf[100];
	int i, ret, fd, mei_fd;

	if (ctx == NULL)
		return FAPI_DSL_STATUS_ERROR;

	if (obj == NULL)
		return FAPI_DSL_STATUS_ERROR;

	fd = fapi_dsl_open_device(ctx->entity);
	if (fd < 0)
		return FAPI_DSL_STATUS_ERROR;
	mei_fd = fapi_dsl_open_mei_device(ctx->entity);
	if (fd < 0) {
		fapi_dsl_close_device(fd);
		return FAPI_DSL_STATUS_ERROR;
	}

	/* Device.DSL.X_LANTIQ_COM_Config.LineFeature */
	memset(&dsl_driver_data.line_feature, 0x00, sizeof(DSL_LineFeature_t));

	if (!strcasecmp(obj->vectoring, "OFF")) {
		vector_control.eVectorControl = e_MEI_VECTOR_CTRL_OFF;
	} else if (!strcasecmp(obj->vectoring, "ON")) {
		vector_control.eVectorControl = e_MEI_VECTOR_CTRL_ON;
	} else if (!strcasecmp(obj->vectoring, "Friendly")) {
		vector_control.eVectorControl = e_MEI_VECTOR_CTRL_FRIENDLY_ON;
	} else if (!strcasecmp(obj->vectoring, "Auto")) {
		/* 3 is auto */
		vector_control.eVectorControl = 3;
		LOGF_LOG_INFO("DSL FAPI: Device.DSL.X_LANTIQ_COM_Config.Vectoring:"
			      "Auto settings doesn't take effect before reboot");
	} else {
		/* set default value */
		/* todo: investigate what is Auto(default) value for vectoring */
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.X_LANTIQ_COM_Config.Vectoring:"
			       " unknown id %s\n", obj->vectoring);
	}
	ret = ioctl(mei_fd, FIO_MEI_DSM_CONFIG_SET, &vector_control);
	if (ret < 0) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.X_LANTIQ_COM_Config.Vectoring: error code %d\n",
			       vector_control.ictl.retCode);
	}

	if (!strcasecmp(obj->dsl_next_mode, "VDSL")) {
		next_mode = DSL_FW_TYPE_VDSL;
	} else if (!strcasecmp(obj->dsl_next_mode, "ADSL")) {
		next_mode = DSL_FW_TYPE_ADSL;
	} else {
		/* set default value */
		next_mode = DSL_FW_TYPE_VDSL;
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.X_LANTIQ_COM_Config.DSLNextMode:"
			       " unknown id %s\n", obj->dsl_next_mode);
	}

	if (!strcasecmp(obj->dsl_act_seq, "Auto")) {
		activation_sequence = DSL_ACT_SEQ_AUTO;
	} else if (!strcasecmp(obj->dsl_act_seq, "StandardGHS")) {
		activation_sequence = DSL_ACT_SEQ_STD;
	} else if (!strcasecmp(obj->dsl_act_seq, "NonStandard")) {
		activation_sequence = DSL_ACT_SEQ_NON_STD;
	} else {
		/* set default value */
		activation_sequence = DSL_ACT_SEQ_STD;
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.X_LANTIQ_COM_Config.DSLActSeq:"
			       " unknown id %s\n", obj->dsl_act_seq);
	}

	if (!strcasecmp(obj->dsl_act_mode, "GHS")) {
		activation_mode = DSL_ACT_MODE_GHS;
	} else if (!strcasecmp(obj->dsl_act_mode, "T1413")) {
		activation_mode = DSL_ACT_MODE_ANSI_T1413;
	} else {
		/* set default value */
		activation_mode = DSL_ACT_MODE_GHS;
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.X_LANTIQ_COM_Config.DSLActMode:"
			       " unknown id %s\n", obj->dsl_act_seq);
	}

	tc_layer_a = DSL_TC_UNKNOWN;
	tc_layer_v = DSL_TC_UNKNOWN;
	strlcpy(buf, obj->link_encapsulation_config, sizeof(buf));
	str_ptr = buf;
	while (token = strtok_r(str_ptr, ",", &str_ptr)) {
		if (!strcasecmp(token, "G.992.3_Annex_K_ATM")) {
			tc_layer_a = DSL_TC_ATM;
		} else if (!strcasecmp(token, "G.992.3_Annex_K_PTM")) {
			if (tc_layer_a != DSL_TC_UNKNOWN) {
				tc_layer_a = DSL_TC_AUTO;
			} else {
				tc_layer_a = DSL_TC_EFM;
			}
		} else if (!strcasecmp(token, "G.993.2_Annex_K_ATM")) {
			tc_layer_v = DSL_TC_ATM;
		} else if (!strcasecmp(token, "G.993.2_Annex_K_PTM")) {
			if (tc_layer_v != DSL_TC_UNKNOWN) {
				tc_layer_v = DSL_TC_AUTO;
			} else {
				tc_layer_v = DSL_TC_EFM;
			}
		} else {
			/* set default value */
			tc_layer_a = DSL_TC_ATM;
			tc_layer_v = DSL_TC_EFM;
			LOGF_LOG_ERROR("DSL FAPI: Device.DSL.X_LANTIQ_COM_Config.LinkEncapsulationConfig:"
				       " unknown id %s\n", token);
		}
	}

	/* Device.DSL.X_LANTIQ_COM_Config.LineFeature ADSL+DS */
	memset(&dsl_driver_data.line_feature, 0x00, sizeof(DSL_LineFeature_t));

	dsl_driver_data.line_feature.nDslMode = DSL_MODE_ADSL;
	dsl_driver_data.line_feature.nDirection = DSL_DOWNSTREAM;

	ret = ioctl(fd, DSL_FIO_LINE_FEATURE_CONFIG_GET, (int)&dsl_driver_data.line_feature);

	if ((ret < 0) && (dsl_driver_data.line_feature.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.X_LANTIQ_COM_Config.LineFeature: error code %d\n",
			       dsl_driver_data.line_feature.accessCtl.nReturn);
	}

	dsl_driver_data.line_feature.data.bBitswapEnable = obj->bitswap_ds_a;
	dsl_driver_data.line_feature.data.bReTxEnable = obj->retx_ds_a;
	dsl_driver_data.line_feature.data.bVirtualNoiseSupport = obj->virtual_noise_ds;

	ret = ioctl(fd, DSL_FIO_LINE_FEATURE_CONFIG_SET, (int)&dsl_driver_data.line_feature);

	if ((ret < 0) && (dsl_driver_data.line_feature.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.X_LANTIQ_COM_Config.LineFeature: error code %d\n",
			       dsl_driver_data.line_feature.accessCtl.nReturn);
	}

	/* Device.DSL.X_LANTIQ_COM_Config.LineFeature ADSL+US */
	memset(&dsl_driver_data.line_feature, 0x00, sizeof(DSL_LineFeature_t));

	dsl_driver_data.line_feature.nDslMode = DSL_MODE_ADSL;
	dsl_driver_data.line_feature.nDirection = DSL_UPSTREAM;

	ret = ioctl(fd, DSL_FIO_LINE_FEATURE_CONFIG_GET, (int)&dsl_driver_data.line_feature);

	if ((ret < 0) && (dsl_driver_data.line_feature.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.X_LANTIQ_COM_Config.LineFeature: error code %d\n",
			       dsl_driver_data.line_feature.accessCtl.nReturn);
	}

	dsl_driver_data.line_feature.data.bBitswapEnable = obj->bitswap_us_a;
	dsl_driver_data.line_feature.data.bReTxEnable = obj->retx_us;
	dsl_driver_data.line_feature.data.bVirtualNoiseSupport = obj->virtual_noise_us;

	ret = ioctl(fd, DSL_FIO_LINE_FEATURE_CONFIG_SET, (int)&dsl_driver_data.line_feature);

	if ((ret < 0) && (dsl_driver_data.line_feature.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.X_LANTIQ_COM_Config.LineFeature: error code %d\n",
			       dsl_driver_data.line_feature.accessCtl.nReturn);
	}

	/* Device.DSL.X_LANTIQ_COM_Config.LineFeature VDSL+DS */
	memset(&dsl_driver_data.line_feature, 0x00, sizeof(DSL_LineFeature_t));

	dsl_driver_data.line_feature.nDslMode = DSL_MODE_VDSL;
	dsl_driver_data.line_feature.nDirection = DSL_DOWNSTREAM;

	ret = ioctl(fd, DSL_FIO_LINE_FEATURE_CONFIG_GET, (int)&dsl_driver_data.line_feature);

	if ((ret < 0) && (dsl_driver_data.line_feature.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.X_LANTIQ_COM_Config.LineFeature: error code %d\n",
			       dsl_driver_data.line_feature.accessCtl.nReturn);
	}

	dsl_driver_data.line_feature.data.bBitswapEnable = obj->bitswap_ds_v;
	dsl_driver_data.line_feature.data.bReTxEnable = obj->retx_ds_v;
	dsl_driver_data.line_feature.data.bVirtualNoiseSupport = obj->virtual_noise_ds;

	ret = ioctl(fd, DSL_FIO_LINE_FEATURE_CONFIG_SET, (int)&dsl_driver_data.line_feature);

	if ((ret < 0) && (dsl_driver_data.line_feature.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.X_LANTIQ_COM_Config.LineFeature: error code %d\n",
			       dsl_driver_data.line_feature.accessCtl.nReturn);
	}

	/* Device.DSL.X_LANTIQ_COM_Config.LineFeature VDSL+US */
	memset(&dsl_driver_data.line_feature, 0x00, sizeof(DSL_LineFeature_t));

	dsl_driver_data.line_feature.nDslMode = DSL_MODE_VDSL;
	dsl_driver_data.line_feature.nDirection = DSL_UPSTREAM;

	ret = ioctl(fd, DSL_FIO_LINE_FEATURE_CONFIG_GET, (int)&dsl_driver_data.line_feature);

	if ((ret < 0) && (dsl_driver_data.line_feature.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.X_LANTIQ_COM_Config.LineFeature: error code %d\n",
			       dsl_driver_data.line_feature.accessCtl.nReturn);
	}

	dsl_driver_data.line_feature.data.bBitswapEnable = obj->bitswap_us_v;
	dsl_driver_data.line_feature.data.bReTxEnable = obj->retx_us;
	dsl_driver_data.line_feature.data.bVirtualNoiseSupport = obj->virtual_noise_us;

	ret = ioctl(fd, DSL_FIO_LINE_FEATURE_CONFIG_SET, (int)&dsl_driver_data.line_feature);

	if ((ret < 0) && (dsl_driver_data.line_feature.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.X_LANTIQ_COM_Config.LineFeature: error code %d\n",
			       dsl_driver_data.line_feature.accessCtl.nReturn);
	}

	/* Device.DSL.X_LANTIQ_COM_Config.RateAdaptation ADSL+DS */
	memset(&dsl_driver_data.rate_adaptation_config, 0x00, sizeof(DSL_G997_RateAdaptationConfig_t));

	dsl_driver_data.rate_adaptation_config.nDslMode = DSL_MODE_ADSL;
	dsl_driver_data.rate_adaptation_config.nDirection = DSL_DOWNSTREAM;
	dsl_driver_data.rate_adaptation_config.data.RA_MODE = obj->sra_ds_a;

	ret = ioctl(fd, DSL_FIO_G997_RATE_ADAPTATION_CONFIG_SET,
		    (int)&dsl_driver_data.rate_adaptation_config);

	if ((ret < 0) && (dsl_driver_data.rate_adaptation_config.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.X_LANTIQ_COM_Config.RateAdaptation: error code %d\n",
			       dsl_driver_data.rate_adaptation_config.accessCtl.nReturn);
	}

	/* Device.DSL.X_LANTIQ_COM_Config.RateAdaptation ADSL+US */
	memset(&dsl_driver_data.rate_adaptation_config, 0x00, sizeof(DSL_G997_RateAdaptationConfig_t));

	dsl_driver_data.rate_adaptation_config.nDslMode = DSL_MODE_ADSL;
	dsl_driver_data.rate_adaptation_config.nDirection = DSL_UPSTREAM;
	dsl_driver_data.rate_adaptation_config.data.RA_MODE = obj->sra_us_a;

	ret = ioctl(fd, DSL_FIO_G997_RATE_ADAPTATION_CONFIG_SET,
		    (int)&dsl_driver_data.rate_adaptation_config);

	if ((ret < 0) && (dsl_driver_data.rate_adaptation_config.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.X_LANTIQ_COM_Config.RateAdaptation: error code %d\n",
			       dsl_driver_data.rate_adaptation_config.accessCtl.nReturn);
	}

	/* Device.DSL.X_LANTIQ_COM_Config.RateAdaptation VDSL+DS */
	memset(&dsl_driver_data.rate_adaptation_config, 0x00, sizeof(DSL_G997_RateAdaptationConfig_t));

	dsl_driver_data.rate_adaptation_config.nDslMode = DSL_MODE_VDSL;
	dsl_driver_data.rate_adaptation_config.nDirection = DSL_DOWNSTREAM;
	dsl_driver_data.rate_adaptation_config.data.RA_MODE = obj->sra_ds_v;

	ret = ioctl(fd, DSL_FIO_G997_RATE_ADAPTATION_CONFIG_SET,
		    (int)&dsl_driver_data.rate_adaptation_config);

	if ((ret < 0) && (dsl_driver_data.rate_adaptation_config.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.X_LANTIQ_COM_Config.RateAdaptation: error code %d\n",
			       dsl_driver_data.rate_adaptation_config.accessCtl.nReturn);
	}

	/* Device.DSL.X_LANTIQ_COM_Config.RateAdaptation VDSL+US */
	memset(&dsl_driver_data.rate_adaptation_config, 0x00, sizeof(DSL_G997_RateAdaptationConfig_t));

	dsl_driver_data.rate_adaptation_config.nDslMode = DSL_MODE_VDSL;
	dsl_driver_data.rate_adaptation_config.nDirection = DSL_UPSTREAM;
	dsl_driver_data.rate_adaptation_config.data.RA_MODE = obj->sra_us_v;

	ret = ioctl(fd, DSL_FIO_G997_RATE_ADAPTATION_CONFIG_SET,
		    (int)&dsl_driver_data.rate_adaptation_config);

	if ((ret < 0) && (dsl_driver_data.rate_adaptation_config.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.X_LANTIQ_COM_Config.RateAdaptation: error code %d\n",
			       dsl_driver_data.rate_adaptation_config.accessCtl.nReturn);
	}

	/* Device.DSL.X_LANTIQ_COM_Config.LinkEncapsulationConfig */
	memset(&dsl_driver_data.system_interface_config, 0x00, sizeof(DSL_SystemInterfaceConfig_t));

	dsl_driver_data.system_interface_config.nDslMode = DSL_MODE_ADSL;
	dsl_driver_data.system_interface_config.data.nTcLayer = tc_layer_a;
	dsl_driver_data.system_interface_config.data.nEfmTcConfigUs = DSL_EMF_TC_NORMAL;
	dsl_driver_data.system_interface_config.data.nEfmTcConfigDs = DSL_EMF_TC_NORMAL;
	dsl_driver_data.system_interface_config.data.nSystemIf = DSL_SYSTEMIF_MII;

	ret = ioctl(fd, DSL_FIO_SYSTEM_INTERFACE_CONFIG_SET, (int)&dsl_driver_data.system_interface_config);
	if ((ret < 0) && (dsl_driver_data.system_interface_config.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.LinkEncapsulationConfig:"
			       "error code %d\n", dsl_driver_data.system_interface_config.accessCtl.nReturn);
	}

	memset(&dsl_driver_data.system_interface_config, 0x00, sizeof(DSL_SystemInterfaceConfig_t));

	dsl_driver_data.system_interface_config.nDslMode = DSL_MODE_VDSL;
	dsl_driver_data.system_interface_config.data.nTcLayer = tc_layer_v;
	dsl_driver_data.system_interface_config.data.nEfmTcConfigUs = DSL_EMF_TC_NORMAL;
	dsl_driver_data.system_interface_config.data.nEfmTcConfigDs = DSL_EMF_TC_NORMAL;
	dsl_driver_data.system_interface_config.data.nSystemIf = DSL_SYSTEMIF_MII;

	ret = ioctl(fd, DSL_FIO_SYSTEM_INTERFACE_CONFIG_SET, (int)&dsl_driver_data.system_interface_config);
	if ((ret < 0) && (dsl_driver_data.system_interface_config.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.LinkEncapsulationConfig:"
			       "error code %d\n", dsl_driver_data.system_interface_config.accessCtl.nReturn);
	}

	/* Device.DSL.X_LANTIQ_COM_Config.XTSE */
	memset(&dsl_driver_data.xtu_system_enabling, 0x00, sizeof(DSL_G997_XTUSystemEnabling_t));

	for (i = 0; i < 8; i++) {
		dsl_driver_data.xtu_system_enabling.data.XTSE[i] = obj->xtse[i];
	}

	ret = ioctl(fd, DSL_FIO_G997_XTU_SYSTEM_ENABLING_CONFIG_SET,
		    (int)&dsl_driver_data.xtu_system_enabling);
	if ((ret < 0) && (dsl_driver_data.xtu_system_enabling.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Channel.XTSE: error code %d\n",
			       dsl_driver_data.xtu_system_enabling.accessCtl.nReturn);
	}

	/* restart autoboot to apply configuration */
	memset(&dsl_driver_data.autoboot_cotrol, 0x00, sizeof(DSL_AutobootControl_t));
	dsl_driver_data.autoboot_cotrol.data.nCommand = DSL_AUTOBOOT_CTRL_RESTART;

	ret = ioctl(fd, DSL_FIO_AUTOBOOT_CONTROL_SET, (int)&dsl_driver_data.autoboot_cotrol);

	if ((ret < 0) && (dsl_driver_data.autoboot_cotrol.accessCtl.nReturn < DSL_SUCCESS)) {
		LOGF_LOG_ERROR("DSL FAPI: Device.DSL.Line.Enable: error code %d\n",
			       dsl_driver_data.autoboot_cotrol.accessCtl.nReturn);
	}
	fapi_dsl_close_device(fd);
	fapi_dsl_close_device(mei_fd);
	return fapi_status;
}

int fapi_dsl_get_device_count()
{
	enum fapi_dsl_status status;
	struct fapi_global_vars global_vars;
	status = shared_mem_global_vars_get(fapi_shared_mem_key, &global_vars);
	if (status != FAPI_DSL_STATUS_SUCCESS) {
		return -1;
	}
	return global_vars.device_count;
}

int fapi_dsl_get_entity_shutdown_count(unsigned int entity) {
	IOCTL_MEI_devinfo_t devinfo;
	enum fapi_dsl_status status;
	struct fapi_global_vars global_vars = { 0 };
	
	int mei_fd = fapi_dsl_open_mei_device(entity), ret = 0;
	int dsl_entities_available = 0, dsl_entities_enabled = 0, dsl_entity_shutdown_cnt = 0;

	if (mei_fd < 0) {
		LOGF_LOG_CRITICAL("DSL FAPI: Cannot determine number of entities \n");
		return 0;
	}

	/* Get dsl_entities_available from IOCTL FIO_MEI_DRV_DEVINFO_GET */
	ret = ioctl(mei_fd, FIO_MEI_DRV_DEVINFO_GET, &devinfo);
	if (!ret) 
	{
		close(mei_fd);
		dsl_entities_available = devinfo.entitiesEnabled;
	} 
	else 
	{
		close(mei_fd);
		LOGF_LOG_CRITICAL("DSL FAPI: cannot get number of available entities\n");
		return 0;
	}

	/* Get dsl_entities_enabled from shared memory */
	status = shared_mem_global_vars_get(fapi_shared_mem_key, &global_vars);
	if (status != FAPI_DSL_STATUS_SUCCESS) 
	{
		return 0;
	}

	dsl_entities_enabled = global_vars.dsl_entities_enabled;
	dsl_entity_shutdown_cnt = dsl_entities_available - dsl_entities_enabled;
	dsl_entity_shutdown_cnt = (dsl_entity_shutdown_cnt > 0) ? dsl_entity_shutdown_cnt : 0;

	return dsl_entity_shutdown_cnt;
}

DLL_PUBLIC void fapi_dsl_update_web_config(const struct fapi_dsl_init_cfg *cfg, WAN_TYPE_t *tc_mode) {
	struct fapi_dsl_init_parameter *cfg_ptr;
	struct fapi_global_vars global_vars = { 0 };
	IOCTL_MEI_VectorControl_t vector_control;
	DSL_TcLayerSelection_t tc_layer_a, tc_layer_v, next_tc_mode;
	DSL_ActivationSequence_t activation_sequence;
	DSL_ActivationMode_t activation_mode;
	DSL_FirmwareType_t next_mode;

	char *str_ptr, *token, buf[100];
	char file_name[] = "/tmp/dsl_web.cfg";
	FILE *fp;

	int nArraySize = 0, ret = 0;

	fp = fopen(file_name, "w");
	if (!fp) {
		LOGF_LOG_CRITICAL("DSL FAPI: fail to create file %s\n", file_name);
		return;
	}

	memset(&x_lantiq_com_config_obj, 0x00, sizeof(struct dsl_fapi_x_lantiq_com_config_obj));
	if(tc_mode) {
		memset(tc_mode, 0x00, sizeof(WAN_TYPE_t));
	}

	if(shared_mem_test(fapi_shared_mem_key) == false) {
		shared_mem_create(fapi_shared_mem_key);
	}

	shared_mem_global_vars_get(fapi_shared_mem_key, &global_vars);

	if (cfg) 
	{
		nArraySize = cfg->params.array_size;
		cfg_ptr = cfg->params.array;
		LOGF_LOG_INFO("fapi config set called with %d parameters\n", nArraySize);

		for (; nArraySize > 0; nArraySize--, cfg_ptr++) 
		{
			if ((cfg_ptr->name == NULL) || (cfg_ptr->value == NULL)) 
			{
				continue;
			}

			if (!strcasecmp(cfg_ptr->name, "BitswapUs_V")) {
				fprintf(fp, "%s=\"%s\"\n", cfg_ptr->name, cfg_ptr->value);
				x_lantiq_com_config_obj.bitswap_us_v = atoi(cfg_ptr->value);
			} else if (!strcasecmp(cfg_ptr->name, "BitswapUs_A")) {
				fprintf(fp, "%s=\"%s\"\n", cfg_ptr->name, cfg_ptr->value);
				x_lantiq_com_config_obj.bitswap_us_a = atoi(cfg_ptr->value);
			} else if (!strcasecmp(cfg_ptr->name, "BitswapDs_V")) {
				fprintf(fp, "%s=\"%s\"\n", cfg_ptr->name, cfg_ptr->value);
				x_lantiq_com_config_obj.bitswap_ds_v = atoi(cfg_ptr->value);
			} else if (!strcasecmp(cfg_ptr->name, "BitswapDs_A")) {
				fprintf(fp, "%s=\"%s\"\n", cfg_ptr->name, cfg_ptr->value);
				x_lantiq_com_config_obj.bitswap_ds_a = atoi(cfg_ptr->value);
			} else if (!strcasecmp(cfg_ptr->name, "SraUs_V")) {
				fprintf(fp, "%s=\"%s\"\n", cfg_ptr->name, cfg_ptr->value);
				x_lantiq_com_config_obj.sra_us_v = atoi(cfg_ptr->value);
			} else if (!strcasecmp(cfg_ptr->name, "SraUs_A")) {
				fprintf(fp, "%s=\"%s\"\n", cfg_ptr->name, cfg_ptr->value);
				x_lantiq_com_config_obj.sra_us_a = atoi(cfg_ptr->value);
			} else if (!strcasecmp(cfg_ptr->name, "SraDs_V")) {
				fprintf(fp, "%s=\"%s\"\n", cfg_ptr->name, cfg_ptr->value);
				x_lantiq_com_config_obj.sra_ds_v = atoi(cfg_ptr->value);
			} else if (!strcasecmp(cfg_ptr->name, "SraDs_A")) {
				fprintf(fp, "%s=\"%s\"\n", cfg_ptr->name, cfg_ptr->value);
				x_lantiq_com_config_obj.sra_ds_a = atoi(cfg_ptr->value);
			} else if (!strcasecmp(cfg_ptr->name, "ReTxUs")) {
				fprintf(fp, "%s=\"%s\"\n", cfg_ptr->name, cfg_ptr->value);
				x_lantiq_com_config_obj.retx_us = atoi(cfg_ptr->value);
			} else if (!strcasecmp(cfg_ptr->name, "ReTxDs_V")) {
				fprintf(fp, "%s=\"%s\"\n", cfg_ptr->name, cfg_ptr->value);
				x_lantiq_com_config_obj.retx_ds_v = atoi(cfg_ptr->value);
			} else if (!strcasecmp(cfg_ptr->name, "ReTxDs_A")) {
				fprintf(fp, "%s=\"%s\"\n", cfg_ptr->name, cfg_ptr->value);
				x_lantiq_com_config_obj.retx_ds_a = atoi(cfg_ptr->value);
			} else if (!strcasecmp(cfg_ptr->name, "VirtualNoiseUs")) {
				fprintf(fp, "%s=\"%s\"\n", cfg_ptr->name, cfg_ptr->value);
				x_lantiq_com_config_obj.virtual_noise_us = atoi(cfg_ptr->value);
			} else if (!strcasecmp(cfg_ptr->name, "VirtualNoiseDs")) {
				fprintf(fp, "%s=\"%s\"\n", cfg_ptr->name, cfg_ptr->value);
				x_lantiq_com_config_obj.virtual_noise_ds = atoi(cfg_ptr->value);
			} else if (!strcasecmp(cfg_ptr->name, "Vectoring")) {
				if (!strcasecmp(cfg_ptr->value, "OFF")) {
					vector_control = e_MEI_VECTOR_CTRL_OFF;
				} else if (!strcasecmp(cfg_ptr->value, "ON")) {
					vector_control = e_MEI_VECTOR_CTRL_ON;
				} else if (!strcasecmp(cfg_ptr->value, "Friendly")) {
					vector_control = e_MEI_VECTOR_CTRL_FRIENDLY_ON;
				} else {
					vector_control = 3;
				}

				fprintf(fp, "%s=\"%d\"\n", cfg_ptr->name, vector_control);
				strlcpy(x_lantiq_com_config_obj.vectoring,
					cfg_ptr->value, sizeof(x_lantiq_com_config_obj.vectoring));
			}else if (!strcasecmp(cfg_ptr->name, "XTSE")) {
				sscanf(cfg_ptr->value, "%hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx",
					&x_lantiq_com_config_obj.xtse[0],
					&x_lantiq_com_config_obj.xtse[1],
					&x_lantiq_com_config_obj.xtse[2],
					&x_lantiq_com_config_obj.xtse[3],
					&x_lantiq_com_config_obj.xtse[4],
					&x_lantiq_com_config_obj.xtse[5],
					&x_lantiq_com_config_obj.xtse[6],
					&x_lantiq_com_config_obj.xtse[7]);

				fprintf(fp, "%s=\"%02X_%02X_%02X_%02X_%02X_%02X_%02X_%02X\"\n",
					cfg_ptr->name,
					x_lantiq_com_config_obj.xtse[0],
					x_lantiq_com_config_obj.xtse[1],
					x_lantiq_com_config_obj.xtse[2],
					x_lantiq_com_config_obj.xtse[3],
					x_lantiq_com_config_obj.xtse[4],
					x_lantiq_com_config_obj.xtse[5],
					x_lantiq_com_config_obj.xtse[6],
					x_lantiq_com_config_obj.xtse[7]);
			} else if (!strcasecmp(cfg_ptr->name, "DSLNextMode")) {
				if (!strcasecmp(cfg_ptr->value, "VDSL")) {
					next_mode = DSL_FW_TYPE_VDSL;
				} else if (!strcasecmp(cfg_ptr->value, "ADSL")) {
					next_mode = DSL_FW_TYPE_ADSL;
				} else {
					/* set default value */
					next_mode = DSL_FW_TYPE_VDSL;
				}

				strlcpy(x_lantiq_com_config_obj.dsl_next_mode,
					cfg_ptr->value,
					sizeof(x_lantiq_com_config_obj.dsl_next_mode));

				fprintf(fp, "%s=\"%d\"\n", cfg_ptr->name, next_mode);
			} else if (!strcasecmp(cfg_ptr->name, "NextTCMode")) {
				if (!strcasecmp(cfg_ptr->value, "atm")) {
					next_tc_mode = DSL_TC_ATM;
					if(tc_mode) {
						*tc_mode = DSL_ATM;
					}
				} else {
					next_tc_mode = DSL_TC_EFM;
					if(tc_mode) {
						*tc_mode = DSL_PTM;
					}
				}

				strlcpy(x_lantiq_com_config_obj.next_tc_mode,
					cfg_ptr->value, sizeof(x_lantiq_com_config_obj.next_tc_mode));

				fprintf(fp, "%s=\"%d\"\n", cfg_ptr->name, next_tc_mode);
			} else if (!strcasecmp(cfg_ptr->name, "LinkEncapsulationConfig")) {
				tc_layer_a = DSL_TC_UNKNOWN;
				tc_layer_v = DSL_TC_UNKNOWN;
				strlcpy(buf, cfg_ptr->value, sizeof(buf));
				str_ptr = buf;
				while (token = strtok_r(str_ptr, ",", &str_ptr)) {
					if (!strcasecmp(token, "G.992.3_Annex_K_ATM")) {
						tc_layer_a = DSL_TC_ATM;
					} else if (!strcasecmp(token, "G.992.3_Annex_K_PTM")) {
						if (tc_layer_a != DSL_TC_UNKNOWN) {
							tc_layer_a = DSL_TC_AUTO;
						} else {
							tc_layer_a = DSL_TC_EFM;
						}
					} else if (!strcasecmp(token, "G.993.2_Annex_K_ATM")) {
						tc_layer_v = DSL_TC_ATM;
					} else if (!strcasecmp(token, "G.993.2_Annex_K_PTM")) {
						if (tc_layer_v != DSL_TC_UNKNOWN) {
							tc_layer_v = DSL_TC_AUTO;
						} else {
							tc_layer_v = DSL_TC_EFM;
						}
					} else {
						/* set default value */
						tc_layer_a = DSL_TC_ATM;
						tc_layer_v = DSL_TC_EFM;
					}
				}

				fprintf(fp, "%s=\"%d:0x1:0x1_%d:0x1:0x1\"\n", cfg_ptr->name,
					tc_layer_a, tc_layer_v);

				strlcpy(x_lantiq_com_config_obj.link_encapsulation_config,
					cfg_ptr->value,
					sizeof(x_lantiq_com_config_obj.link_encapsulation_config));
			} else if (!strcasecmp(cfg_ptr->name, "DSLActSeq")) {
				if (!strcasecmp(cfg_ptr->value, "Auto")) {
					activation_sequence = DSL_ACT_SEQ_AUTO;
				} else if (!strcasecmp(cfg_ptr->value, "StandardGHS")) {
					activation_sequence = DSL_ACT_SEQ_STD;
				} else if (!strcasecmp(cfg_ptr->value, "NonStandard")) {
					activation_sequence = DSL_ACT_SEQ_NON_STD;
				} else {
					/* set default value */
					activation_sequence = DSL_ACT_SEQ_STD;
				}

				strlcpy(x_lantiq_com_config_obj.dsl_act_seq,
					cfg_ptr->value, sizeof(x_lantiq_com_config_obj.dsl_act_seq));

				fprintf(fp, "%s=\"%d\"\n", cfg_ptr->name, activation_sequence);
			}else if (!strcasecmp(cfg_ptr->name, "DSLActMode")) {
				if (!strcasecmp(cfg_ptr->value, "GHS")) {
					activation_mode = DSL_ACT_MODE_GHS;
				} else if (!strcasecmp(cfg_ptr->value, "T1413")) {
					activation_mode = DSL_ACT_MODE_ANSI_T1413;
				} else {
					/* set default value */
					activation_mode = DSL_ACT_MODE_GHS;
				}
				strlcpy(x_lantiq_com_config_obj.dsl_act_mode,
					cfg_ptr->value, sizeof(x_lantiq_com_config_obj.dsl_act_mode));
				fprintf(fp, "%s=\"%d\"\n", cfg_ptr->name, activation_mode);
			}else if (!strcasecmp(cfg_ptr->name, "DSLRemember")) {
				x_lantiq_com_config_obj.dsl_remember = atoi(cfg_ptr->value);
				fprintf(fp, "%s=\"%s\"\n", cfg_ptr->name, cfg_ptr->value);
			} else if (!strcasecmp(cfg_ptr->name, "EntitiesEnabled")) {
				if (!strcasecmp(cfg_ptr->value, "None")) {
					global_vars.dsl_entities_enabled = 0;
				} else if (!strcasecmp(cfg_ptr->value, "Single link")) {
					global_vars.dsl_entities_enabled = DSL_CPE_FAPI_SINGLE_LINK;
				} else if (!strcasecmp(cfg_ptr->value, "Bonding")) {
					global_vars.dsl_entities_enabled = DSL_CPE_FAPI_BONDING;
				} else {
					/* set default "Auto" value */
					global_vars.dsl_entities_enabled = DSL_CPE_FAPI_MAX_SUPPORTED_ENTITIES;
				}
				strlcpy(x_lantiq_com_config_obj.dsl_entities_enabled,
					cfg_ptr->value, sizeof(x_lantiq_com_config_obj.dsl_entities_enabled));

				fprintf(fp, "%s=\"%d\"\n", cfg_ptr->name, global_vars.dsl_entities_enabled);
			}
		}
	}

	shared_mem_global_vars_set(fapi_shared_mem_key, &global_vars);
	fclose(fp);

	return ;
}

DLL_PUBLIC bool fapi_dsl_uninit_entities_status_get()
{
	struct fapi_global_vars global_vars = { 0 };

	shared_mem_global_vars_get(fapi_shared_mem_key, &global_vars);

	LOGF_LOG_DEBUG("DSL FAPI: uninit_entities_status get %d\n",
						global_vars.uninit_entities_performed);

	return global_vars.uninit_entities_performed;
}

DLL_PUBLIC void fapi_dsl_uninit_entities_status_set(const bool new_status)
{
	struct fapi_global_vars global_vars = { 0 };

	shared_mem_global_vars_get(fapi_shared_mem_key, &global_vars);

	LOGF_LOG_DEBUG("DSL FAPI: uninit_entities_status set %d (old %d)\n",
						new_status, global_vars.uninit_entities_performed);

	global_vars.uninit_entities_performed = new_status;
	shared_mem_global_vars_set(fapi_shared_mem_key, &global_vars);

	return;
}
