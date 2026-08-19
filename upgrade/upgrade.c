/******************************************************************************

Copyright (c) 2014
Lantiq Deutschland GmbH

For licensing information, see the file 'LICENSE' in the root folder of
this software module.

******************************************************************************/

//#include "ifx_common.h"
#include "common.h"
#include <sys/ioctl.h>
#include<ctype.h>
#include<unistd.h>

#define SEMAPHORE_KEY 1238
#define MAX_RETRIES 3
#define MICRO_SEC_INTERVAL 100

#define VERSION             "1.0.5"

#include "cmd_upgrade.h"
#include "command.h"
#include <sys/stat.h>
#include <signal.h>
#include <sys/reboot.h>

int do_upgrade(int file_fd, int srcLen);
extern int upgrade_img(ulong, ulong, char *, enum ExpandDir dir, int);

env_t env;

#ifdef CONFIG_FEATURE_WIRELESS
#ifndef CONFIG_FEATURE_WIRELESS_WAVE300
/* Possible interfaces: eth0(br0), usb0 (device, host), wlan0..4 (1- AP, 4 - VAPs) */
#define MAX_LAN_MAC_ADDR	7
#else
#define MAX_LAN_MAC_ADDR    2
#endif
#else /* CONFIG_FEATURE_WIRELESS */
/* Possible interfaces: eth0(br0), usb0 (device, host) */
#define MAX_LAN_MAC_ADDR	2
#endif /* CONFIG_FEATURE_WIRELESS */

/* Possible interfaces corresponding to WAN1 - WAN15 */
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

#ifdef LOG_DBG
	printf("\n[PARENT] Process [%d] is inside init_sem() \n", getpid());
#endif
	semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
	if (semid >= 0) {	// Our process is first in the race
		sem_buf.sem_num = 0;
		sem_buf.sem_op = 1;	//Clear semaphore variable (or) initialize
		sem_buf.sem_flg = 0;	//Set default flag
#ifdef LOG_DBG
		printf("\n[PARENT] Process [%d] created the semaphore\n",
			getpid());
#endif

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
#ifdef LOG_DBG
			printf
			    ("\n[Child] Child process unable to get semid\n");
#endif
			return semid;
		} else {
			sem_union.buf = &sem_id_ds;

			for (i = 0; i < MAX_RETRIES && !sem_initialized; i++) {
				/*Check whether Parent process has initialized semaphore */
				semctl(semid, 0, IPC_STAT, sem_union);
				if (sem_union.buf->sem_otime > 0) {	// Last sem_op time is set
#ifdef LOG_DBG
					printf
					    ("\n[CHILD] Parent process initialized semaphore successfully\n");
#endif
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
#ifdef LOG_DBG
	printf("\n[SUCCESS] Process [%d] enter_critical_section()\n",
		getpid());
#endif
	return 0;
      failure:
#ifdef LOG_DBG
	printf("\n[FAILURE] Process [%d] enter_critical_section()\n",
		getpid());
#endif
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
#ifdef LOG_DBG
		printf("\nERROR: UnLock failed at process [%d]\n", getpid());
#endif
		return -1;
	}
#ifdef LOG_DBG
	printf("\n[SUCCESS] Process [%d] exit_critical_section()\n", getpid());
#endif
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
	char *fileData = NULL;
	struct stat filestat;
	int bRead = 0;
	// char sCommand[32];
	int bSaveEnvCopy = 0;
	int ret = 0;
	int semid = 0;
	unsigned long flen = 0;
	unsigned char *env_data = NULL;
#ifdef CONFIG_FEATURE_IFX_UPGRADE_VENDOR_EXT
	unsigned int offset = 0, newLen = 0;
#endif				// CONFIG_FEATURE_IFX_UPGRADE_VENDOR_EXT

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

#ifndef CONFIG_TARGET_UBI_MTD_SUPPORT
		if (read_env()) {
			printf("read_env fail\n");
			ret = 1;
			goto abort;
		}
#endif
		if (strtol(argv[4], NULL, 10) == 1)
			bSaveEnvCopy = 1;

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

#ifdef CONFIG_BOOT_FROM_NAND
			char temp_buf[256];
			if (!strcmp(argv[2], "sysconfig")) {
  #ifdef CONFIG_TARGET_UBI_MTD_SUPPORT
	sprintf_s(temp_buf, sizeof(temp_buf), "/usr/sbin/vol_mgmt write_vol sysconfig %s", argv[1]);
  #else
				sprintf_s(temp_buf, sizeof(temp_buf), 
					"cp -f %s /etc/sysconfig/rc.conf.current.gz; sync",
					argv[1]);
  #endif
				system(temp_buf);
				ret = 0;
				goto abort;
  #ifdef CONFIG_TARGET_UBI_MTD_SUPPORT
			} else if (!strcmp(argv[2], "wlanconfig") || !strcmp(argv[2], "dectconfig")) {
				//sprintf_s(temp_buf, sizeof(temp_buf), "/usr/sbin/vol_mgmt write_vol %s %s",
				//	argv[2], argv[1]);

				//write/update on calibration mtd partition. Failsafe added on UBI volumes (backward compatability).
				sprintf_s(temp_buf, sizeof(temp_buf), "/usr/sbin/vol_mgmt update_calibration %s %s",
					argv[2], argv[1]);
				system(temp_buf);
				ret = 0;
				goto abort;
  #endif
			} else {
				printf
				    ("upgrade is invoked for data block in NAND %s\n",
				     argv[2]);
				ret = 1;
				goto abort;
			}
#endif

			fileData = (char *)malloc(flen * sizeof(char));
			if (fileData == NULL) {
				printf
				    ("Can not allocate %ld bytes for the buffer\n",
				     flen);
				ret = 1;
				goto abort;
			}
			bRead = read(file_fd, fileData, flen);
			if (bRead < (int)flen) {
				printf
				    ("Could read only read %d bytes out of %ld bytes of the file\n",
				     bRead, flen);
				free(fileData);
				ret = 1;
				goto abort;
			}

			close(file_fd);
//              printf("Erasing the input file %s\n",argv[1]);
			if (strcmp(argv[1], "/etc/rc.conf.gz") != 0)
				unlink(argv[1]);

			printf
			    ("upgrade : calling upgrade_img with srcAddr = 0x%08lx and size %ld\n",
			     (unsigned long int )fileData, flen);
			if (upgrade_img
			    ((unsigned long)fileData, flen, argv[2],
			     strtol(argv[3], NULL, 10), bSaveEnvCopy)) {
				printf
				    ("Image %s could not be updated in dir=%s\n",
				     argv[2], argv[3]);
				ret = 1;
			} else {
				//printf("Upgrade : successfully upgraded %s\n", argv[2]);
			}

			free(fileData);
			ret = 0;
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
			//         printf("Erasing the input file %s\n",argv[1]);
			if (strcmp(argv[1], "/etc/rc.conf.gz") != 0) {
					unlink(argv[1]);
			}

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
	return ret;
    }   // file check
}
