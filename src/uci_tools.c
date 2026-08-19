/******************************************************************************

         Copyright (c) 2020, MaxLinear, Inc.
         Copyright 2016 - 2020 Intel Corporation

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#include <sys/param.h>
#include <sys/wait.h>
#include <unistd.h>
#include <paths.h>

#include "spal_logging.h"
#include "uci_wrapper.h"

extern char **environ;

int uci_converter_system_print(const char *cmd)
{
	int st;
	char print_cmd[MAX_LEN_PARAM_VALUE];

	if (cmd == NULL) {
		ERROR("%s, cmd is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}
	st = sprintf_s(print_cmd, MAX_LEN_PARAM_VALUE, "%s >> /dev/console 2>&1", cmd);
	if (st <= 0) {
		ERROR("%s failed sprintf_s", __func__);
		return RETURN_ERR;
	}
	return uci_converter_system(print_cmd);
}

/**************************************************************************/
/*! \fn int uci_converter_system(char *cmd)
 **************************************************************************
 *  \brief run command using system - not recommended to use (plus - need root permissions!)
 *  \param[in] char *cmd - command string to run via system
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_system(char *cmd)
{
	return uci_converter_popen(cmd, NULL, -1);
}

/**************************************************************************/
/*! \fn int uci_converter_popen(char *cmd, char *output, int outputSize)
 **************************************************************************
 *  \brief run command using system, saving result - not recommended to use (plus - need root permissions!)
 *  \param[in] char *cmd - command string to run via system
 *  \param[out] char *output - save result
 *  \param[in] int outputSize - size of output buffer
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int uci_converter_popen(char *cmd, char *output, int outputSize)
{
	int link[2];
	pid_t pid;
	int pstat = 0;
	int ret = 0;
	int res = RETURN_OK;
	char * const argp[] = {"sh", "-c", cmd, NULL};
	int stringSize = outputSize - 1;

	if (cmd == NULL) {
		ERROR("%s, cmd is NULL!\n", __FUNCTION__);
		return RETURN_ERR;
	}

	if (pipe(link)==-1) {
		ERROR("%s, pipe error\n", __FUNCTION__);
		return RETURN_ERR;
	}

	if ((pid = fork()) == -1) {
		ERROR("%s, fork error\n", __FUNCTION__);
		close(link[0]);
		close(link[1]);
		return RETURN_ERR;
	}

	if (pid == 0) {
		dup2(link[1], STDOUT_FILENO);
		close(link[0]);
		close(link[1]);
		execve(_PATH_BSHELL, argp, environ);
		return RETURN_ERR;
	} else {
		int bytesRead = 0;
		close(link[1]);
		if (output) {
			while (bytesRead < stringSize) {
				ret = read(link[0], output + bytesRead, stringSize - bytesRead);

				if (ret == 0) {
					break;
				}
				if (ret == -1) {
					ERROR("%s, read error\n", __FUNCTION__);
					break;
				}
				bytesRead += ret;
			}
			output[bytesRead] = '\0';
		}
		do {
			ret = waitpid(pid, &pstat, 0);
			if (ret == -1 && errno == EINTR) {
				DEBUG("%s: EINTR recieved, restarting cur_pid=%d\n", __FUNCTION__, pid);
				continue;
			}

			if (WIFEXITED(pstat)) {
				int status = WEXITSTATUS(pstat);
				/* if there is a pipe the status is of the last cmd,
				 * so it is meaningless in term of success or failure */
				if (status && !strstr(cmd, "|") && !strstr(cmd, ">> /dev/console")) {
					ERROR("%s: cmd:%s - exited, status=%d\n", __FUNCTION__, cmd, status);
					res = RETURN_ERR;
					goto end;
				}
				DEBUG("%s: exited, status=%d\n", __FUNCTION__, status);
				break;
			} else if (WIFSIGNALED(pstat)) {
				ERROR("%s: killed by signal %d\n", __FUNCTION__, WTERMSIG(pstat));
				res = RETURN_ERR;
				goto end;
			} else if (WIFSTOPPED(pstat)) {
				ERROR("%s: stopped by signal %d\n", __FUNCTION__, WSTOPSIG(pstat));
				res = RETURN_ERR;
				goto end;
			}
		} while (ret != -1);
	}

	/* Remove trailing new line character if needed */
	if (output && (output[strnlen_s(output, outputSize)-1] == '\n'))
		output[strnlen_s(output, outputSize)-1] = '\0';

end:
	close(link[0]);
	return res;
}
