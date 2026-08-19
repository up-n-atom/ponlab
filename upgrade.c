/******************************************************************************

  Copyright (C) 2019 Intel Corporation
  Copyright (C) 2020-2023 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/***************************************************************************** *
 *     File Name  : upgrade.c	 					       *
 *     Project    : UGW                                                        *
 ******************************************************************************/


#include <sys/ioctl.h>
#include<ctype.h>
#include<unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/reboot.h>
#include "sse_env.h"

#define SEMAPHORE_KEY 1238
#define MAX_RETRIES 3
#define MICRO_SEC_INTERVAL 100

#define VERSION             "1.0.5"

#define MAX_LAN_MAC_ADDR	2
#define MAX_WAN_MAC_ADDR	15

void mac_usage(void)
{
	printf("\nEnter MAC Address in xx:xx:xx:xx:xx:xx format Only!\n \
			where xx should be in the range [0-9][A-F]\n");
}

int ValidateMACAddress(unsigned char *mac)
{
	int i, j;
	unsigned char str[12], value[15];
	unsigned int mac_val = 0;

	memset(str, 0x00, sizeof(str));
	memset(value, 0x00, sizeof(value));

	if (mac == NULL) {
		printf("Null MAC Address\n");
		mac_usage();
		return -1;
	}
	/* Total MAC Address Len should be <=17 */
	else if ((strnlen_s((const char *)mac, 17)) > 17) {
		printf("Invalid MAC Address length\n");
		mac_usage();
		return -1;
	} else {
		/* Tokenize each field in MAC Address seperator 
		 * Seperator ':' is only handled */
		for (i = 2, j = 0; i <= 17; i += 3) {
			str[j] = mac[i - 2];
			j++;
			str[j] = mac[i - 1];
			j++;
			if (mac[i] != ':' && i < 17) {
				printf("Wrong MAC Address Format\n");
				mac_usage();
				return -1;
			}
		}
	}

	/* Validate each filed in MAC Address 
	 * The fileds should be in the range [0-9][A-F] */
	for (i = 0; i < 12; i++) {
		if (isalpha(str[i]))
			str[i] = toupper(str[i]);
		if ((str[i] < '0') || (str[i] > '9')) {
			if ((str[i] < 'A') || (str[i] > 'F')) {
				printf("Wrong MAC Address Fields\n");
				mac_usage();
				return -1;
			} else {
				if ((str[i] >= 'A') && (str[i] <= 'F'))
					str[i] -= ('A' - 10);
			}
		} else {
			if ((str[i] >= '0') && (str[i] <= '9'))
				str[i] -= '0';
		}
	}

	/* Sum up each Byte in MAC Address */
	for (i = 0; i < 12 / 2; i++) {
		value[i] = (str[i << 1] << 4) + str[(i << 1) + 1];
		mac_val += value[i];
	}

	if (mac_val) {
		/* Check if any address is a MAC broadcast address FF:FF:FF:FF:FF:FF 
		 * mac_val + MAX_LAN_MAC_ADDR + MAX_WAN_MAC_ADDR >= FF:FF:FF:FF:FF:FF
		 * MAC Address is a broadcast */
		if ((mac_val + MAX_LAN_MAC_ADDR + MAX_WAN_MAC_ADDR) >=
				1530) {
			printf("MAC Address results in Broadcast Address!\n");
			mac_usage();
			return -1;
		} else
			return 0;
	} else {
		printf("ZERO - MAC Address Invalid!");
		mac_usage();
		return -1;
	}
}

union semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
};

int init_sem(key_t key)
{
	int semid, i;
	struct sembuf sem_buf;
	struct semid_ds sem_id_ds;
	union semun sem_union;

	semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
	if (semid >= 0) {	// Our process is first in the race
		sem_buf.sem_num = 0;
		sem_buf.sem_op = 1;	//Clear semaphore variable (or) initialize
		sem_buf.sem_flg = 0;	//Set default flag

		if (semop(semid, &sem_buf, 1) == -1) {
			printf
				("\nERROR:[PARENT] Process [%d] failed to initialize semaphore\n",
				 getpid());
			return -1;
		}
	} else if (errno == EEXIST) {	// Not the parent process, Need to wait until parent initialize semaphore
		int sem_initialized = 0;
		semid = semget(key, 1, 0);
		if (semid < 0) {
			return semid;
		} else {
			sem_union.buf = &sem_id_ds;

			for (i = 0; i < MAX_RETRIES && !sem_initialized; i++) {
				/*Check whether Parent process has initialized semaphore */
				semctl(semid, 0, IPC_STAT, sem_union);
				if (sem_union.buf->sem_otime > 0) {	// Last sem_op time is set
					sem_initialized = 1;
				} else {
					usleep(MICRO_SEC_INTERVAL);
				}
			}

			if (!sem_initialized) {
				errno = ETIME;
				return -1;
			}
		}
	} else {
		return semid;	// This is also an error condition 
	}

	return semid;
}

int enter_critical_section(int semid)
{
	int lock_status;
	struct sembuf sem_buf;
	int retry = 0;

	sem_buf.sem_num = 0;
	sem_buf.sem_op = -1;	/*By default set to -1 before entering critic sec */
	/*if sem_op = 1 [UNLOCKED] this will decrement to sem_op = 0 stating locked */
	sem_buf.sem_flg = SEM_UNDO;

	while (1) {
		lock_status = semop(semid, &sem_buf, 1);
		if (lock_status == 0) {
			goto success;
		} else {
			usleep(MICRO_SEC_INTERVAL);
			if (retry >= MAX_RETRIES) {
				//#ifdef DBG_ENABLED
				printf
					("\nERROR: Lock failed at process [%d]\n",
					 getpid());
				//#endif
				goto failure;
			}
		}
		retry++;
		continue;
	}

success:
	return 0;
failure:
	return -1;
}

int exit_critical_section(int semid)
{
	int lock_status;
	struct sembuf sem_buf;

	sem_buf.sem_num = 0;
	sem_buf.sem_op = 1;	/*By default set to -1 before entering critic sec */
	/*if sem_op = 0 [UNLOCKED] this will increment to sem_op = 1 stating unlocked */
	sem_buf.sem_flg = SEM_UNDO;

	lock_status = semop(semid, &sem_buf, 1);

	if (lock_status == -1) {
		return -1;
	}
	return 0;
}

int del_sem(int semid)
{
	int status;
	union semun sem_union;

	status = semctl(semid, 0, IPC_RMID, sem_union);

	if (status == -1) {
		printf("\nERROR: Unable to destroy semaphore\n");
		exit(1);
	}

	return 0;
}

void sig_handler(int signo)
{
	if (signo == SIGTERM)
		printf("UPGRADE util:received SIGTERM\n");

	if (signo == SIGINT)
		printf("UPGRADE util:received SIGINT\n");

	if (signo == SIGQUIT)
		printf("UPGRADE util:received SIGQUIT\n");

	if (signo == SIGPIPE)
		printf("UPGRADE util:received SIGPIPE\n");

	if (signo == SIGUSR1)
		printf("UPGRADE util:received SIGUSR1\n");

	if (signo == SIGUSR2)
		printf("UPGRADE util:received SIGUSR2\n");

	if (signo == SIGABRT)
		printf("UPGRADE util:received SIGABRT\n");

}

int main(int argc, char *argv[])
{
	int file_fd = 0;
	struct stat filestat;
	int ret = 0;
	int semid = 0;
	unsigned long flen = 0;
	unsigned char *env_data = NULL;

	semid = init_sem(SEMAPHORE_KEY);

	if (signal(SIGTERM, sig_handler) == SIG_ERR)
		printf("\ncan't catch SIGTERM\n");

	if (signal(SIGQUIT,sig_handler) == SIG_ERR)
		printf("\ncan't catch SIGQUIT\n");

	if (signal(SIGINT,sig_handler) == SIG_ERR)
		printf("\ncan't catch SIGINT\n");

	if (signal(SIGPIPE,sig_handler) == SIG_ERR)
		printf("\ncan't catch SIGPIPE\n");

	if (signal(SIGUSR1,sig_handler) == SIG_ERR)
		printf("\ncan't catch SIGUSR1\n");

	if (signal(SIGUSR2,sig_handler) == SIG_ERR)
		printf("\ncan't catch SIGUSR2\n");

	if ( access( "/tmp/upgrade_chk.txt", F_OK ) == 0 )
	{
		printf("\nReboot in progress....! upgrade operation is not possible\n");
		return 0;
	}else{
		if (enter_critical_section(semid) == 0) {
			/* Sumedh: Support MAC address updation */
			if (argc == 3
					&& ((strcmp(argv[1], "mac_set") == 0)
						|| strcmp(argv[1], "mac_get") == 0)) {
				if (strcmp(argv[1], "mac_set") == 0) {
					if (ValidateMACAddress((unsigned char *)argv[2])
							< 0) {
						ret = 0;
						goto exit_handler;
					} else {
						read_env();
						set_env("ethaddr", argv[2]);
						saveenv();
						printf
							("System is going down for reboot!\n");
						system("/etc/rc.d/rebootcpe.sh 5");
					}
				} else {
					read_env();
					env_data = get_env("ethaddr");
					if (env_data) {
						printf("%s", env_data);
					}
					ret = 0;
					goto exit_handler;
				}
			}
			/* Sumedh: Support ChipID & BoardID get/set */
			else if (argc == 3
					&& ((strcmp(argv[1], "chipid_set") == 0)
						|| (strcmp(argv[1], "chipid_get") == 0)
						|| (strcmp(argv[1], "boardid_set") == 0)
						|| (strcmp(argv[1], "boardid_get") == 0))) {
				if (strcmp(argv[1], "chipid_set") == 0) {
					read_env();
					set_env("chipid", argv[2]);
					saveenv();
					ret = 0;
					goto exit_handler;
				} else if (strcmp(argv[1], "chipid_get") == 0) {
					read_env();
					env_data = get_env("chipid");
					if (env_data) {
						printf("%s", env_data);
					}
					ret = 0;
					goto exit_handler;
				} else if (strcmp(argv[1], "boardid_set") == 0) {
					read_env();
					set_env("boardid", argv[2]);
					saveenv();
					ret = 0;
					goto exit_handler;
				} else if (strcmp(argv[1], "boardid_get") == 0) {
					read_env();
					env_data = get_env("boardid");
					if (env_data) {
						printf("%s", env_data);
					}
					ret = 0;
					goto exit_handler;
				}
			} else if ((argc >= 2)
					&& (strcmp(argv[1], "ethwan_set") == 0
						|| strcmp(argv[1], "ethwan_get") == 0)) {
				if (strcmp(argv[1], "ethwan_set") == 0) {
					read_env();
					set_env("ethwan", argv[2]);
					saveenv();
					printf("System is going down for reboot!\n");
					system("/etc/rc.d/rebootcpe.sh 5");
				} else if (strcmp(argv[1], "ethwan_get") == 0) {
					read_env();
					env_data = get_env("ethwan_get");
					if (env_data) {
						printf("%s", env_data);
					}
					ret = 0;
					goto exit_handler;
				}
			}

			else if (argc < 5 || argc > 6) {
				printf
					("Usage : upgrade file_name image_type expand_direction saveenv_copy [reboot]\n");
				printf("ToolChain:" CONFIG_GCC_VERSION "/"
						CONFIG_LIBC_VERSION "\n");
				printf("Version:" VERSION "\n");
				ret = 1;
				goto exit_handler;
			}

			if (argc == 6 && (strcmp(argv[5], "reboot") == 0)) {
				printf
					("upgrade : reboot option is found and so killing all the processes\n");

				//303002:JackLee
				sleep(2);

				signal(SIGTERM, SIG_IGN);
				signal(SIGHUP, SIG_IGN);
				setpgrp();
				//kill all the processes except you
				kill(-1, SIGTERM);
				sleep(5);
			}

			file_fd = open(argv[1], O_RDONLY);
			if (file_fd < 0) {
				printf("The file %s could not be opened\n", argv[1]);
				ret = 1;
				goto abort;
			}

			fstat(file_fd, &filestat);
			flen = filestat.st_size;
			//806121:<IFTW-leon> IOP bit: add fwdiag
			if (!strcmp(argv[2], "ubootconfig")
					|| !strcmp(argv[2], "sysconfig")
					|| !strcmp(argv[2], "factoryconfig")
					|| !strcmp(argv[2], "wlanconfig")
					|| !strcmp(argv[2], "voip")
					|| !strcmp(argv[2], "dectconfig")) {

				char temp_buf[256];
				if (!strcmp(argv[2], "sysconfig")) {
					sprintf_s(temp_buf, sizeof(temp_buf), "/usr/sbin/vol_mgmt write_vol sysconfig %s", argv[1]);
					system(temp_buf);
					ret = 0;
					goto abort;
				} else if (!strcmp(argv[2], "wlanconfig") || !strcmp(argv[2], "dectconfig")) {
					sprintf_s(temp_buf, sizeof(temp_buf), "/usr/sbin/vol_mgmt update_calibration %s %s",
							argv[2], argv[1]);
					system(temp_buf);
					ret = 0;
					goto abort;
				} else {
					printf
						("upgrade is invoked for data block in NAND %s\n",
						 argv[2]);
					ret = 1;
					goto abort;
				}
			} else {

				printf
					("upgrade : calling do_upgrade with filename = %s and size %ld\n",
					 argv[1], flen);
				if (do_upgrade(file_fd, flen)) {
					printf
						("Image %s could not be updated in dir=%s\n",
						 argv[2], argv[3]);
					ret = 1;
				} else {
					printf("Upgrade : successfully upgraded %s\n",
							argv[2]);
				}

				close(file_fd);
			}
		} else {
			printf("enter_critical_section() function failed\n");
			return 1;
		}
abort:
		if (argc == 6 && (strcmp(argv[5], "reboot") == 0)) {
			close(file_fd);
			printf("upgrade : Rebooting the system\n");
			sync();
			sleep(25);
			reboot(RB_AUTOBOOT);
		}
exit_handler:

		if (exit_critical_section(semid) != 0)
			printf("exit_critical_section @ update.c function failed\n");
		close(file_fd);
	}  
	return ret;
}
