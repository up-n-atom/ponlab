/******************************************************************************
 * Copyright (c) 2022 MaxLinear, Inc.
 * Copyright (c) 2017 Intel Corporation
 * Copyright (c) 2011 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 ******************************************************************************/
#include "ifx_types.h"
#include "lib_cli_config.h"
#include "lib_cli_osmap.h"
#include "lib_cli_debug.h"
#include "lib_cli_core.h"

#if (CLI_SUPPORT_PIPE == 1)
#include "lib_cli_pipe.h"

#ifdef CLI_STATIC
#	undef CLI_STATIC
#endif

#ifdef CLI_DEBUG
#	define CLI_STATIC
#else
#	define CLI_STATIC   static
#endif

/** Pipe task stack size */
#ifndef CLI_OS_PIPE_STACK_SIZE
#	define CLI_OS_PIPE_STACK_SIZE	(8192)
#endif

/** Default thread priority.
   This setting is a relative value. */
#ifndef CLI_OS_THREAD_PRIO
#define CLI_OS_THREAD_PRIO	40
#endif


#define CLI_PIPECBWAIT_TIMEOUT	3000

#define CLI_PIPE_EVENT	1
#define CLI_PIPE_DUMP	2
#define CLI_PIPE_EVENT_WAIT_TIMEOUT	1000

#define CLI_LOG_FIFOSIZE	(4096 * 2)
#define CLI_LOG_MAX_ELEM_SIZE	1024


#define CLI_PIPE_NAME_LEN	32
#define CLI_PIPE_CMD_BUFFER_LEN	64000

/** Pipe struct */
struct cli_pipe_env_s {
	/** reference to the used CLI core context (?) */
	struct cli_core_context_s *p_cli_core_ctx;
	/** user data for callback handling */
	struct cli_user_context_s *p_user_ctx;

	/** backward reference to the CLI pipe context */
	struct cli_pipe_context_s *p_cli_pipe_context;

	/** Running status of pipe task */
	volatile IFX_boolean_t b_run;

	/** Thread id of the pipe task */
	clios_thread_t tid_x;
	/** Thread control of the pipe task */
	clios_thr_ctrl_t thr_control;

	/** Number of this pipe interface */
	unsigned short number;
	/** name of event pipe */
	char name_pipe[CLI_PIPE_NAME_LEN];
	/** name of event pipe */
	char name_pipe_event[CLI_PIPE_NAME_LEN];
	/** name of dump pipe */
	char name_pipe_dump[CLI_PIPE_NAME_LEN];
	/** Command buffer */
	char cmd_buffer[CLI_PIPE_CMD_BUFFER_LEN];
	/** pipe in fd */
	clios_pipe_t *p_pipe_in;
	/** pipe out fd */
	clios_pipe_t *p_pipe_out;
};



/**
   Forward message dump to the pipe remote server, if configured and connected.

   \param[in] p_pipe_env  Pipe context
   \param[in] p_msg  Message
   \param[in] user_id  User Id
*/
CLI_STATIC int pipe_cb(
	struct cli_pipe_env_s *p_pipe_env,
	char *p_msg,
	unsigned long user_id)
{
	unsigned long *p_element;
	unsigned int len;
	char *ptr;
	const unsigned int max_elem_len = (CLI_LOG_MAX_ELEM_SIZE - sizeof(unsigned long));
	struct cli_pipe_context_s *p_cli_pipe_context = p_pipe_env->p_cli_pipe_context;

	len = clios_strlen(p_msg);
	if (len >= max_elem_len) {
		len = max_elem_len;
	}

	if (!IFXOS_LOCK_INIT_VALID(&p_cli_pipe_context->lock_cb)) {
		CLI_USR_ERR(LIB_CLI,
			("ERR CLI Pipe: callback lock invalid" CLI_CRLF));
		return IFX_ERROR;
	}

	if (clios_lockget_timout(
		&p_cli_pipe_context->lock_cb,
		CLI_PIPECBWAIT_TIMEOUT, IFX_NULL) != IFX_SUCCESS) {
		CLI_USR_ERR(LIB_CLI,
			("ERR CLI Pipe: callback lock timeout" CLI_CRLF));
		return IFX_ERROR;
	}
	p_element = clios_vfifo_write_element(
		&p_cli_pipe_context->log_fifo, len + sizeof(unsigned long));
	if (p_element) {
		p_element[0] = user_id | (p_pipe_env->number << 16);
		clios_memcpy(&p_element[1], p_msg, len);
		ptr = (char *)&p_element[1];
		ptr[len - 1] = 0;
	} else {
		CLI_USR_DBG(LIB_CLI, CLI_PRN_HIGH,
			("WRN CLI Pipe: callback fifo overflow" CLI_CRLF));
	}
	(void)clios_lockrelease(&p_cli_pipe_context->lock_cb);

	(void)clios_event_wakeup(&p_cli_pipe_context->event_fifo);

	return IFX_SUCCESS;
}

/** Event pipe

   \param[in] p_cb_ctx Context
   \param[in] p_msg Message
*/
CLI_STATIC int pipe_event(void *p_cb_ctx, char *p_msg)
{
	return pipe_cb(p_cb_ctx, p_msg, CLI_PIPE_EVENT);
}

/** Dump pipe

   \param[in] p_cb_ctx Context
   \param[in] p_msg Message
*/
CLI_STATIC int pipe_dump(void *p_cb_ctx, char *p_msg)
{
	return pipe_cb(p_cb_ctx, p_msg, CLI_PIPE_DUMP);
}

CLI_STATIC int pipe_exit(void *p_cb_ctx)
{
	return -1;
}

/**
   Split the command buffer in single lines and execute them

   \param[in] context Context
   \param[in] cmd Command
   \param[in] len  Command length
   \param[in] out  Output pipe
*/
CLI_STATIC int pipe_exec(
	struct cli_core_context_s *p_cli_core_ctx,
	char *p_cmd,
	int len,
	clios_pipe_t *p_out)
{
	int ret = 0;
	char *p_arg = IFX_NULL;

	char *p_line;
	char *p_tokbuf;
	char *p_cmd_part;
	char *p_cmd_tok;

	p_line = clios_strtok_r(p_cmd, CLI_CRLF, &p_tokbuf);

	while (p_line != IFX_NULL) {
		if (clios_strlen(p_line) > 0) {
			/* get command name */
			p_cmd_part = clios_strtok_r(p_line, " \t" CLI_CRLF, &p_cmd_tok);
			if (p_cmd_part == IFX_NULL)
				goto pipe_exec_skip;
			p_arg = clios_strtok_r(IFX_NULL, "" CLI_CRLF, &p_cmd_tok);

			if (cli_core_cmd_arg_exec__file(
				p_cli_core_ctx, p_cmd_part, p_arg, p_out) == 1) {
				/* "quit" was found, ignore following commands */
				return 1;
			}
		}
		pipe_exec_skip:
		p_line = clios_strtok_r(IFX_NULL, "" CLI_CRLF, &p_tokbuf);
	};

	return ret;
}

/**
   Task for handling the commands received in the ..._cmd pipe

   \param[in] thr_params Thread parameters
*/
CLI_STATIC int pipe_thread(IFXOS_ThreadParams_t *thr_params)
{
	struct cli_pipe_env_s *p_pipe_env = (struct cli_pipe_env_s *)thr_params->nArg1;
	/* struct cli_pipe_context_s *p_cli_pipe_context = IFX_NULL; */

	char pipe_name_cmd[50];
	char pipe_name_ack[50];
	int len = 0;
	int ret = 0;
#if defined(CLI_SPECIAL_IO_FILE_TYPE) && (CLI_SPECIAL_IO_FILE_TYPE == 1)
	struct cli_spec_file_io_s pipe_file_out = {IFX_NULL, IFX_NULL, 0};
#endif

	clios_snprintf(pipe_name_cmd, sizeof(pipe_name_cmd),
		CLI_PIPE_NAME_FORMAT "cmd", p_pipe_env->name_pipe, p_pipe_env->number);
	clios_snprintf(pipe_name_ack, sizeof(pipe_name_ack),
		CLI_PIPE_NAME_FORMAT "ack", p_pipe_env->name_pipe, p_pipe_env->number);
	do {
#if 1
		if (p_pipe_env->p_pipe_in) {
			clios_pipe_close(p_pipe_env->p_pipe_in);
			p_pipe_env->p_pipe_in = IFX_NULL;
		}
#endif
		p_pipe_env->p_pipe_in =
			clios_pipe_open(pipe_name_cmd, IFX_TRUE, IFX_TRUE);
		if (p_pipe_env->p_pipe_in == IFX_NULL) {
			CLI_USR_ERR(LIB_CLI,
				(CLI_CRLF "ERR CLI Pipe: pipe thr - cannot open pipe %s" CLI_CRLF,
				pipe_name_cmd));
			ret = -1;
			goto CLI_PIPE_THREAD_ERROR;
		}

		if (!p_pipe_env->b_run) {
			continue;
		}

		clios_memset(&p_pipe_env->cmd_buffer[0], 0, sizeof(p_pipe_env->cmd_buffer));

		len = sizeof(p_pipe_env->cmd_buffer);
		if (len > 0) {
			ret = clios_pipe_read(
				&p_pipe_env->cmd_buffer[0], 1, len, p_pipe_env->p_pipe_in);

			if (!p_pipe_env->b_run) {
				continue;
			}

			if (ret > 0) {
				(void)clios_pipe_close(p_pipe_env->p_pipe_in);
				p_pipe_env->p_pipe_in = IFX_NULL;

				p_pipe_env->p_pipe_out =
					clios_pipe_open(pipe_name_ack, IFX_FALSE, IFX_TRUE);
				if (p_pipe_env->p_pipe_out == IFX_NULL) {
					CLI_USR_ERR(LIB_CLI,
						(CLI_CRLF "ERR CLI Pipe: pipe thr - cannot open pipe %s" CLI_CRLF,
						pipe_name_ack));
					ret = -1;
					goto CLI_PIPE_THREAD_ERROR;
				}
#if defined(CLI_SPECIAL_IO_FILE_TYPE) && (CLI_SPECIAL_IO_FILE_TYPE == 1)
				pipe_file_out.p_file = p_pipe_env->p_pipe_out;
				pipe_exec(p_pipe_env->p_cli_core_ctx,
					p_pipe_env->cmd_buffer,  len,
					(void*)&pipe_file_out);
#else
				pipe_exec(p_pipe_env->p_cli_core_ctx,
					p_pipe_env->cmd_buffer,  len,
					p_pipe_env->p_pipe_out);
#endif

				clios_pipe_close(p_pipe_env->p_pipe_out);
				p_pipe_env->p_pipe_out = IFX_NULL;
			} else {
				clios_sleep_msec(50);
			}
		}
	} while (p_pipe_env->b_run && (thr_params->bShutDown != IFX_TRUE));

CLI_PIPE_THREAD_ERROR:

	if (p_pipe_env->p_pipe_out != IFX_NULL) {
		clios_pipe_close(p_pipe_env->p_pipe_out);
		p_pipe_env->p_pipe_out = IFX_NULL;
	}
	if (p_pipe_env->p_pipe_in != IFX_NULL) {
		clios_pipe_close(p_pipe_env->p_pipe_in);
		p_pipe_env->p_pipe_in = IFX_NULL;
	}

	return ret;
}

/** Pipe log

   \param[in] thr_params Thread parameters
*/
CLI_STATIC int pipe_log_thread(
	IFXOS_ThreadParams_t *thr_params)
{
	struct cli_pipe_context_s *p_cli_pipe_context = (struct cli_pipe_context_s *)thr_params->nArg1;
	unsigned long *p_msg;
	unsigned int el_size;
	unsigned long *p_buffer;

	/*
	   int errno = 0;
	 */

	p_buffer = (unsigned long *)
		clios_memalloc(CLI_LOG_MAX_ELEM_SIZE * sizeof(unsigned long));

	if (p_buffer == IFX_NULL) {
		CLI_USR_ERR(LIB_CLI,
			("ERR CLI Pipe: log thr - no memory" CLI_CRLF));
		return IFX_ERROR;
	}

	while ((thr_params->bRunning) && (thr_params->bShutDown == IFX_FALSE)) {
		if ((thr_params->bRunning == IFX_FALSE)
		    || (thr_params->bShutDown == IFX_TRUE))
			break;

		if (clios_vfifo_is_empty(&p_cli_pipe_context->log_fifo)) {
			if (clios_event_wait(&p_cli_pipe_context->event_fifo,
					    CLI_PIPE_EVENT_WAIT_TIMEOUT, IFX_NULL) < IFX_SUCCESS)
				continue;
		}

		if (clios_lockget_timout(
				&p_cli_pipe_context->lock_cb,
				CLI_PIPECBWAIT_TIMEOUT, IFX_NULL) != IFX_SUCCESS) {
			CLI_USR_DBG(LIB_CLI, CLI_PRN_HIGH,
				("WRN CLI Pipe: log thr - callback timeout - read" CLI_CRLF));
			continue;
		}

		el_size = 0;
		p_msg = clios_vfifo_read_element(&p_cli_pipe_context->log_fifo, &el_size);

		if (el_size && p_msg) {
			if (el_size <= CLI_LOG_MAX_ELEM_SIZE * sizeof(unsigned long))
				clios_memcpy(p_buffer, p_msg, el_size);
		}

		clios_lockrelease(&p_cli_pipe_context->lock_cb);

		if (el_size && p_msg) {
			struct cli_pipe_env_s *p_pipe_env = IFX_NULL;
			clios_pipe_t *p_pipe;

			if (((p_buffer[0] >> 16) & 0xFFFF) >= p_cli_pipe_context->n_pipes) {
				CLI_USR_DBG(LIB_CLI, CLI_PRN_HIGH,
					("WRN CLI Pipe: log thr - buffer overflow" CLI_CRLF));
			} else {
				unsigned short pipe_num = p_buffer[0] >> 16;
				if(pipe_num >= p_cli_pipe_context->n_pipes)
					pipe_num = 0;

				p_pipe_env = p_cli_pipe_context->p_cli_pipe_env[pipe_num];
				switch (p_buffer[0] & 0xFFFF) {
				case CLI_PIPE_DUMP:
					p_pipe = clios_pipe_open(
						p_pipe_env->name_pipe_dump, IFX_FALSE, IFX_FALSE);

					if (p_pipe != IFX_NULL) {
						if (IFXOS_PipePrintf(p_pipe, "%s" CLI_CRLF, (char *)&p_buffer[1]) <= 0)
							CLI_USR_ERR(LIB_CLI,
								("ERR CLI Pipe: log thr - dump pipe[%02d], errno = %d" CLI_CRLF,
								p_pipe_env->number, errno));
						clios_pipe_close(p_pipe);
					}
					break;

				case CLI_PIPE_EVENT:
					p_pipe = clios_pipe_open(
						p_pipe_env->name_pipe_event, IFX_FALSE, IFX_FALSE);
					if (p_pipe != IFX_NULL) {
						if (IFXOS_PipePrintf(p_pipe, "%s" CLI_CRLF,(char *)&p_buffer[1]) <= 0)
							CLI_USR_ERR(LIB_CLI,
								("ERR CLI Pipe: log thr - event pipe[%02d], errno = %d" CLI_CRLF,
								p_pipe_env->number, errno));
						clios_pipe_close(p_pipe);
					}
					break;
				}
			}
		}
	}

	clios_memfree(p_buffer);

	return 0;
}

/** Init pipe

   \param p_cli_core_ctx  context CLI core context pointer
   \param pipe_num_off  start offset of the pipe number
   \param n_pipes  number of pipes
   \param p_pipe_name  base name of the pipe
   \param cli_pipe_context_s  returns the created pipe context
*/
CLI_STATIC int pipe_init(
	struct cli_core_context_s *p_cli_core_ctx,
	unsigned short pipe_num_off,
	unsigned short n_pipes,
	char const *p_pipe_name,
	struct cli_pipe_context_s **pp_cli_pipe_context)
{
	unsigned short i;
	struct cli_pipe_context_s *p_cli_pipe_context = IFX_NULL;
	struct cli_pipe_env_s *p_pipe_env = IFX_NULL;
	char pipe_name[CLI_PIPE_NAME_LEN];

	if ((n_pipes == 0) || (n_pipes > MAX_CLI_PIPES)) {
		CLI_USR_ERR(LIB_CLI,
			("ERR CLI Pipe: init - invalid arg, n_pipes = %u (max = %u)" CLI_CRLF,
			n_pipes, MAX_CLI_PIPES));
		return IFX_ERROR;
	}
	if (!p_pipe_name) {
		CLI_USR_ERR(LIB_CLI,
			("ERR CLI Pipe: init - invalid arg, missing pipe name" CLI_CRLF));
		return IFX_ERROR;
	}
	if (*pp_cli_pipe_context != IFX_NULL) {
		CLI_USR_ERR(LIB_CLI,
			("ERR CLI Pipe: init - invalid arg, context pointer not zero" CLI_CRLF));
		return IFX_ERROR;
	}

	p_cli_pipe_context = (struct cli_pipe_context_s *)
		clios_memalloc(sizeof(struct cli_pipe_context_s));
	if (p_cli_pipe_context == IFX_NULL) {
		CLI_USR_ERR(LIB_CLI,
			("ERR CLI Pipe: init - no memory" CLI_CRLF));
		return IFX_ERROR;
	}
	clios_memset(p_cli_pipe_context, 0x00, sizeof(struct cli_pipe_context_s));

	p_cli_pipe_context->p_fifo_mem =
		clios_memalloc(CLI_LOG_FIFOSIZE * sizeof(unsigned long));
	p_cli_pipe_context->p_pipe_env_mem =
		clios_memalloc(n_pipes * sizeof(struct cli_pipe_env_s));
	if ((p_cli_pipe_context->p_fifo_mem == IFX_NULL) ||
	    (p_cli_pipe_context->p_pipe_env_mem == IFX_NULL)) {
		CLI_USR_ERR(LIB_CLI,
			("ERR CLI Pipe: init - no memory (dump fifo, interface)" CLI_CRLF));
		if (p_cli_pipe_context->p_fifo_mem)
			clios_memfree(p_cli_pipe_context->p_fifo_mem);
		if (p_cli_pipe_context->p_pipe_env_mem)
			clios_memfree(p_cli_pipe_context->p_pipe_env_mem);
		clios_memfree(p_cli_pipe_context);
		return IFX_ERROR;
	}
	clios_memset(p_cli_pipe_context->p_pipe_env_mem, 0x00, n_pipes * sizeof(struct cli_pipe_env_s));
	p_cli_pipe_context->n_pipes = n_pipes;

	(void)clios_event_init(&p_cli_pipe_context->event_fifo);
	(void)clios_lockinit(&p_cli_pipe_context->lock_cb, "pipe_cb", 0);
	clios_vfifo_init(
		&p_cli_pipe_context->log_fifo,
		p_cli_pipe_context->p_fifo_mem,
		p_cli_pipe_context->p_fifo_mem + CLI_LOG_FIFOSIZE,
		 CLI_LOG_MAX_ELEM_SIZE);

	/* start log thread */
	clios_thr_init(
		&p_cli_pipe_context->thr_ctrl_log, "pipelog",
		pipe_log_thread,
		CLI_OS_PIPE_STACK_SIZE, CLI_OS_THREAD_PRIO,
		(unsigned long)p_cli_pipe_context, 0);

	/* create pipes, start pipe threads */
	for (i = 0; i < n_pipes; i++) {
		p_pipe_env = &p_cli_pipe_context->p_pipe_env_mem[i];
		p_pipe_env->number = i + pipe_num_off;
		clios_snprintf(p_pipe_env->name_pipe, sizeof(p_pipe_env->name_pipe), "%s", p_pipe_name);

		clios_snprintf(pipe_name, sizeof(pipe_name), CLI_PIPE_NAME_FORMAT "cmd",
			p_pipe_env->name_pipe, i + pipe_num_off);
		clios_pipe_create(pipe_name);

		clios_snprintf(pipe_name, sizeof(pipe_name), CLI_PIPE_NAME_FORMAT "ack",
			p_pipe_env->name_pipe, i + pipe_num_off);
		clios_pipe_create(pipe_name);

		clios_snprintf(p_pipe_env->name_pipe_event, sizeof(p_pipe_env->name_pipe_event),
			CLI_PIPE_NAME_FORMAT "event", p_pipe_name, i + pipe_num_off);
		clios_pipe_create(p_pipe_env->name_pipe_event);

		clios_snprintf(p_pipe_env->name_pipe_dump, sizeof(p_pipe_env->name_pipe_dump),
			CLI_PIPE_NAME_FORMAT "dump", p_pipe_name, i + pipe_num_off);
		clios_pipe_create(p_pipe_env->name_pipe_dump);

		clios_snprintf(pipe_name, sizeof(pipe_name), "tPipe_%u", i + pipe_num_off);
		p_pipe_env->p_cli_core_ctx = p_cli_core_ctx;
		p_pipe_env->p_cli_pipe_context = p_cli_pipe_context;

		p_cli_pipe_context->p_cli_pipe_env[i] = p_pipe_env;
		p_pipe_env->b_run = IFX_TRUE;

		/* start pipe thread */
		clios_thr_init(
			&p_pipe_env->thr_control, pipe_name,
			pipe_thread,
			CLI_OS_PIPE_STACK_SIZE, CLI_OS_THREAD_PRIO,
			(unsigned long)p_pipe_env, 0);

		(void)cli_user_if_register(
			p_cli_core_ctx, p_pipe_env,
			pipe_exit, pipe_event, pipe_dump,
			&p_pipe_env->p_user_ctx);
	}
	*pp_cli_pipe_context = p_cli_pipe_context;

	return IFX_SUCCESS;
}

/** Init pipe
    (pipe number offset = 0, fix)

   \param p_cli_core_ctx  context CLI core context pointer
   \param n_pipes  number of pipes
   \param p_pipe_name  base name of the pipe
   \param cli_pipe_context_s  returns the created pipe context
*/
int cli_pipe_init(
	struct cli_core_context_s *p_cli_core_ctx,
	unsigned short n_pipes,
	char const *p_pipe_name,
	struct cli_pipe_context_s **pp_cli_pipe_context)
{
	return pipe_init(p_cli_core_ctx, 0, n_pipes, p_pipe_name, pp_cli_pipe_context);
}


/** Single Init pipe
    (number of pipes = 1, fix)

   \param p_cli_core_ctx  context CLI core context pointer
   \param pipe_num  pipe number
   \param p_pipe_name  base name of the pipe
   \param cli_pipe_context_s  returns the created pipe context
*/
int cli_single_pipe_init(
	struct cli_core_context_s *p_cli_core_ctx,
	unsigned short pipe_num,
	char const *p_pipe_name,
	struct cli_pipe_context_s **pp_cli_pipe_context)
{
	return pipe_init(p_cli_core_ctx, pipe_num, 1, p_pipe_name, pp_cli_pipe_context);
}

/** Exit pipe

   \param[in] context OMCI context pointer
*/
int cli_pipe_release(
	struct cli_core_context_s *p_cli_core_ctx,
	struct cli_pipe_context_s **pp_cli_pipe_context)
{
	int i;
	struct cli_pipe_context_s *p_cli_pipe_context = IFX_NULL;
	struct cli_pipe_env_s *p_pipe_env = IFX_NULL;

	if (*pp_cli_pipe_context) {
		p_cli_pipe_context = *pp_cli_pipe_context;
		*pp_cli_pipe_context = IFX_NULL;

		if (IFXOS_LOCK_INIT_VALID(&p_cli_pipe_context->lock_cb)) {
			clios_lockget(&p_cli_pipe_context->lock_cb);
		}

		/* shutdown pipe log thread */
		if (clios_thr_shutdown(
				&p_cli_pipe_context->thr_ctrl_log,
				CLI_PIPECBWAIT_TIMEOUT * 4) != IFX_SUCCESS) {
			(void)clios_event_wakeup(&p_cli_pipe_context->event_fifo);
			(void)clios_thr_delete(
				&p_cli_pipe_context->thr_ctrl_log,
				CLI_PIPECBWAIT_TIMEOUT * 4);
		}

		for (i = 0; i < p_cli_pipe_context->n_pipes; i++) {
			if (p_cli_pipe_context->p_cli_pipe_env[p_cli_pipe_context->n_pipes] == IFX_NULL)
				continue;
			p_pipe_env = p_cli_pipe_context->p_cli_pipe_env[p_cli_pipe_context->n_pipes];

			if (p_pipe_env->p_cli_core_ctx && p_pipe_env->p_user_ctx)
				(void)cli_user_if_unregister(
					p_pipe_env->p_cli_core_ctx,
					&p_pipe_env->p_user_ctx);

			/* shutdown pipe thread */
			if (clios_thr_shutdown(
					&p_pipe_env->thr_control,
					CLI_PIPECBWAIT_TIMEOUT * 4) != IFX_SUCCESS) {
				(void)clios_thr_delete(
					&p_pipe_env->thr_control,
					CLI_PIPECBWAIT_TIMEOUT * 4);
			}
			if (p_pipe_env->p_pipe_out != IFX_NULL) {
				clios_pipe_close(p_pipe_env->p_pipe_out);
				p_pipe_env->p_pipe_out = IFX_NULL;
			}
			if (p_pipe_env->p_pipe_in != IFX_NULL) {
				clios_pipe_close(p_pipe_env->p_pipe_in);
				p_pipe_env->p_pipe_in = IFX_NULL;
			}
		}

		if (p_cli_pipe_context->p_pipe_env_mem)
		{
			clios_memfree(p_cli_pipe_context->p_pipe_env_mem);
			p_cli_pipe_context->p_pipe_env_mem = IFX_NULL;
		}

		if (p_cli_pipe_context->p_fifo_mem)
		{
			clios_vfifo_clear(&p_cli_pipe_context->log_fifo);
			clios_memfree(p_cli_pipe_context->p_fifo_mem);
			p_cli_pipe_context->p_fifo_mem = IFX_NULL;
		}

		if (IFXOS_LOCK_INIT_VALID(&p_cli_pipe_context->lock_cb)) {
			clios_lockdelete(&p_cli_pipe_context->lock_cb);
		}
		(void)clios_event_delete(&p_cli_pipe_context->event_fifo);
		clios_memfree(p_cli_pipe_context);
	}

	return IFX_SUCCESS;
}

#endif	/* #if (CLI_SUPPORT_PIPE == 1) */


