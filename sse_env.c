/******************************************************************************

  Copyright (c) 2022-2024 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/***************************************************************************** *
 *     File Name  : sse_env.c                                                  *
 *     Project    : UGW                                                        *
 ******************************************************************************/
#include <ctype.h>
#include <dirent.h>
#include <pthread.h>
#include "sec_upgrade.h"
#include "help_error.h"
#include "help_logging.h"
#include "safe_str_lib.h"

#define MAX_PATH_LEN	256
#define TEMP_ENV_FILE	"/tmp/ubootconfig"
#define SYSFS_BLOCK_DIR	"/sys/class/block/"
#define SYSFS_MTD_DIR	"/sys/class/mtd/"
#define SYSFS_UBI_DIR	"/sys/class/ubi/"
#define ENV_A_PART	"env_a"
#define ENV_B_PART	"env_b"

#define CFG_ENV_SIZE CONFIG_UBOOT_CONFIG_ENV_SIZE
#ifdef CONFIG_SYS_REDUNDAND_ENVIRONMENT
#define ENV_HEADER_SIZE (sizeof(uint32_t) + sizeof(char))
#else
#define ENV_HEADER_SIZE sizeof(uint32_t)
#endif
#define ENV_SIZE (CFG_ENV_SIZE - ENV_HEADER_SIZE)

typedef struct environment_s {
     uint32_t  crc;  /* CRC32 over data bytes        */
#ifdef CONFIG_SYS_REDUNDAND_ENVIRONMENT
	unsigned char flags;      /* active/obsolete flags    */
#endif
     unsigned char data[ENV_SIZE];   /* Environment data             */
} env_t;

static env_t env;
static char active_cfg;

char *getDevFromPartition(char *name, char type);
#ifdef PLATFORM_LGM
int check_boardtype(void);
#else
inline int check_boardtype(void) {
	return -1;
}
#endif

const uint32_t crc32_table1[256] = {
	0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
	0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
	0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
	0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
	0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
	0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
	0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
	0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
	0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
	0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
	0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
	0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
	0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
	0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
	0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
	0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
	0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
	0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
	0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
	0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
	0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
	0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
	0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
	0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
	0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
	0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
	0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
	0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
	0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
	0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
	0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
	0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
	0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
	0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
	0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
	0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
	0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
	0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
	0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
	0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
	0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
	0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
	0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
	0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
	0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
	0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
	0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
	0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
	0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
	0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
	0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
	0x2d02ef8dL
};

/* Return a 32-bit CRC of the contents of the buffer. */

/*!\
   \brief static integer type function Returns a 32-bit CRC of the contents of the buffer
   \param[in] val
   \param[in] ss
   \param[in] len
   \return uint32_t
*/
static inline uint32_t crc32(uint32_t val, const void *ss, int len)
{
	const unsigned char *s = ss;
	while (--len >= 0)
		val = crc32_table1[(val ^ *s++) & 0xff] ^ (val >> 8);
	return val;
}

static int readDeviceFile(char *filename, char *buffer)
{
	FILE *fp;

	if (!filename || !buffer)
		return -EINVAL;

	fp = fopen(filename, "r");
	if (!fp) {
		LOGF_LOG_ERROR("Error: could not open file %s", filename);
		return -ENOENT;
	}

	fread(buffer, BUFSIZ, 1, fp);
	fclose(fp);

	return 0;
}

static char *parseBlkDevInfo(char *path, const char *dir, char *arg)
{
	static char dev_name[MAX_PATH_LEN] = { 0 };
	char partname[MAX_PATH_LEN] = { "PARTNAME=" };
	char file[MAX_PATH_LEN] = { 0 };
	char buffer[BUFSIZ] = { 0 };

	if (!path || !dir || !arg)
		return NULL;

	sprintf_s(file, sizeof(file), "%s%s/uevent", path, dir);
	if (readDeviceFile(file, buffer))
		return NULL;

	/* Check if PARTNAME var is present */
	strcat_s(partname, sizeof(partname), arg);
	if (strstr(buffer, partname) == NULL)
		return NULL;

	LOGF_LOG_DEBUG("partition found under %s%s!", path, dir);
	sprintf_s(dev_name, sizeof(dev_name), "%s", dir);

	return dev_name;
}

static char *parseMtdDevInfo(char *path, const char *dir, const char *arg)
{
	static char dev_name[MAX_PATH_LEN] = { 0 };
	char file[MAX_PATH_LEN] =  { 0 };
	char buffer[BUFSIZ] = { 0 };
	int i = 0;

	if (!path || !dir || !arg)
		return NULL;

	sprintf_s(file, sizeof(file), "%s%s/name", path, dir);
	if (readDeviceFile(file, buffer))
		return NULL;

	/* check if partition name is matching or not */
	if (strstr(buffer, arg) == NULL) {
		LOGF_LOG_DEBUG("string not found!");
		return NULL;
	}
	LOGF_LOG_DEBUG("partition found under %s%s!", path, dir);

	/* Get partition number from dir name */
	while (dir[i] != '\0') {
		if (isdigit(dir[i]))
			break;
		i++;
	}
	/* Check if number found in dir name */
	if (dir[i] == '\0')
		return NULL;
	snprintf_s(dev_name, MAX_PATH_LEN, "mtdblock%s", &dir[i]);

	return dev_name;
}

static char *parseUbiDevInfo(char *path, const char *dir, const char *arg)
{
	static char dev_name[MAX_PATH_LEN] = { 0 };
	char file[MAX_PATH_LEN] =  { 0 };
	char buffer[BUFSIZ] = { 0 };

	if (!path || !dir || !arg)
		return NULL;

	sprintf_s(file, sizeof(file), "%s%s/name", path, dir);
	if (readDeviceFile(file, buffer))
		return NULL;

	/* check if partition name is matching or not */
	if (strstr(buffer, arg) == NULL) {
		LOGF_LOG_DEBUG("string not found!");
		return NULL;
	}

	LOGF_LOG_DEBUG("partition found under %s%s!", path, dir);
	snprintf_s(dev_name, MAX_PATH_LEN, "%s", dir);

	return dev_name;
}

/*
 * Function to extract device name from partition name
 *
 * @name - name of the partition
 * @type - type of device to return for NAND flash:
 *         B/b - for block device /dev/mtdblockX (e.g. /dev/mtdblock16)
 *         C/c - for character device /dev/ubiX_Y (e.g. /dev/ubi1_0)
 */
char *getDevFromPartition(char *name, char type)
{
	DIR *FD;
	struct dirent *in_file;
	char dir[MAX_PATH_LEN] = { 0 };
	char *parse;
	int boardtype;

	if (!name)
		return NULL;

	boardtype = check_boardtype();
	if (boardtype == FLASH_TYPE_EMMC) {
		sprintf_s(dir, MAX_PATH_LEN, SYSFS_BLOCK_DIR);
	} else {
		if (type == 'b' || type == 'B') {
			sprintf_s(dir, MAX_PATH_LEN, SYSFS_MTD_DIR);
		} else if (type == 'c' || type == 'C') {
			sprintf_s(dir, MAX_PATH_LEN, SYSFS_UBI_DIR);
		} else {
			fprintf(stderr, "Invalid argument for device type\n");
			return NULL;
		}
	}

	/* Scanning the in directory */
	if (!(FD = opendir(dir))) {
		fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
		return NULL;
	}
	while ((in_file = readdir(FD))) {
		if (!strcmp(in_file->d_name, "."))
			continue;
		if (!strcmp(in_file->d_name, ".."))
			continue;
		if (boardtype == FLASH_TYPE_EMMC) {
			parse = parseBlkDevInfo(dir, in_file->d_name, name);
		} else if (boardtype == FLASH_TYPE_NAND) {
			if (type == 'b' || type == 'B')
				parse = parseMtdDevInfo(dir, in_file->d_name, name);
			else if (type == 'c' || type == 'C')
				parse = parseUbiDevInfo(dir, in_file->d_name, name);
		}
		if (parse != NULL) {
			closedir(FD);
			return parse;
		}
	}

	closedir(FD);
	return NULL;
}

#define MIN(a, b)({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
#ifdef PLATFORM_LGM
char dev_path[MAX_PATH_LEN];
static int lgmBoardType;
int nandwrite(void);
int mtdwrite(void);

/**=====================================================================
 * @brief identify the board type
 *
 * set global variable based on the cmdline information
 *
 * @return
 *  board type on success
 *  UGW_FAILURE on failure
 =======================================================================
 */
int check_boardtype(void)
{
	FILE *fp;
	char temp[512];

	fp = fopen("/proc/cmdline", "r");
	if (fp == NULL)
		return -1;

	fgets(temp, 512, fp);
	if ((strstr(temp, "ubi.mtd")) == NULL)
		lgmBoardType = FLASH_TYPE_EMMC;
	else
		lgmBoardType = FLASH_TYPE_NAND;

	fclose(fp);
	return lgmBoardType;
}

static char *getEnvAdev(void)
{
	char *name = getDevFromPartition(ENV_A_PART, 'c');

	sprintf_s(dev_path, sizeof(dev_path), "/dev/%s", name);
	return dev_path;
}

static char *getEnvBdev(void)
{
	char *name = getDevFromPartition(ENV_B_PART, 'c');

	sprintf_s(dev_path, sizeof(dev_path), "/dev/%s", name);
	return dev_path;
}
#endif

unsigned long sse_crc(unsigned long crc, char *sBuf, int bread)
{
	return crc32(crc, sBuf, bread);
}

int readenv(void)
{
	int dev_fd;
	unsigned long crc = 0;

	/* Dual uboot config Mode with ubootconfigA and ubootconfigB
	 * Read the both configs and get the one which have a correct CRC.
	 * If both CRC is proper, then read flags - flags contain latest
	 * revision number. The one which have latest revision is the valid
	 * uboot config to use.
	 */
	env_t envB;
	int flgA = 0, flgB = 0;

#ifdef PLATFORM_LGM
	check_boardtype();
	dev_fd = open(getEnvAdev(), O_RDONLY);
	if (dev_fd < 0) {
		printf(" Opening the ENV device failed\n");
		return -1;
	} else {
		read(dev_fd, (void *)&env, sizeof(env));
		close(dev_fd);
		flgA = 1;
	}
	dev_fd = open(getEnvBdev(), O_RDONLY);
	if (dev_fd < 0) {
		printf(" Opening the ENV device failed\n");
		return -1;
	} else {
		read(dev_fd, (void *)&envB, sizeof(env));
		close(dev_fd);
		flgB = 1;
	}

#endif
	if ((!flgA) && (!flgB)) {
		active_cfg = 0;
		printf("The device could not be opened\n");
		return 1;
	}
	if (flgA) {
		crc ^= 0xffffffffL;
		crc = sse_crc(crc, (char *)env.data, ENV_SIZE);
		crc ^= 0xffffffffL;

		if (crc != env.crc)
			flgA = 1;
		else
			flgA = 2;
	}

	crc = 0;
	if (flgB) {
		crc ^= 0xffffffffL;
		crc = sse_crc(crc, (char *)envB.data, ENV_SIZE);
		crc ^= 0xffffffffL;

		if (crc != envB.crc)
			flgB = 1;
		else
			flgB = 2;
	}

	/* If CRC of flgA and flgB is Ok, then read env.flags which contain
	   updated revision of the config. Always take latest config. So
	   take the env which have greater env.flags value. */
	if ((flgA < 2) && (flgB < 2)) {
		/* Both CRC is failure */
		active_cfg = 0;
		printf("For enviornment CRC32 is not OK\n");
		return 1;
	} else if (flgB > flgA) {
		/* If only flgB CRC is OK, then use flgB */
		active_cfg = 2;
		env = envB;
	} else if (flgA == flgB) {
		/* If flgA and flgB is OK, then read flags and use latest */
#ifdef CONFIG_SYS_REDUNDAND_ENVIRONMENT
		if (env.flags == 255 && envB.flags == 0) {
			/* if we get flags as 255 and 0, then 0 is latest */
			active_cfg = 2;
			env = envB;
		} else if (env.flags == 0 && envB.flags == 255) {
			active_cfg = 1;
		} else if (env.flags > envB.flags) {
			active_cfg = 1;
		} else if (envB.flags > env.flags) {
			active_cfg = 2;
			env = envB;
		} else {
			active_cfg = 1;
		}
#else
		active_cfg = 2;
		env = envB;
#endif
	} else {
		active_cfg = 1;
	}

	return 0;
}

/* UGW_SW-87803: This is a wrapper function called from fwupgrade daemon */
int read_env(void)
{
	return readenv();
}

int envmatch(char *s1, int i2)
{
	while (*s1 == env.data[i2++])
		if (*s1++ == '=')
			return i2;
	if (*s1 == '\0' && env.data[i2 - 1] == '=')
		return i2;
	return -1;
}
unsigned char *get_env(char *name)
{
	unsigned int i, nxt;

	for (i = 0; i < ENV_SIZE && env.data[i] != '\0'; i = nxt + 1) {
		int val;

		for (nxt = i; nxt < ENV_SIZE && env.data[nxt] != '\0'; nxt++) {
			if (nxt >= ENV_SIZE) {
				printf("Did not get var %s with nxt = %d\n",
					name, nxt);
				return NULL;
			}
		}
		val = envmatch(name, i);
		if (val < 0)
			continue;
		return &env.data[val];
	}

	return NULL;
}

void env_crc_update(void)
{
	env.crc = 0x00000000 ^ 0xffffffff;
	env.crc = sse_crc(env.crc, (char *)env.data, ENV_SIZE);
	env.crc ^= 0xffffffff;
}

int addenv(char *name, char *val)
{
	int len, val_len, oldval;
	unsigned char *envptr, *nxt = NULL;
	unsigned char *env_data = env.data;

	if (!env_data) {        /* need copy in RAM */
		printf("addenv(): env_data doesn't exist\n");
		return 1;
	}

	/*
	 * search if variable with this name already exists
	 */
	oldval = -1;
	for (envptr = env_data; *envptr; envptr = nxt + 1) {
		for (nxt = envptr; *nxt; ++nxt)
		;
		oldval = envmatch(name, envptr - env_data);
		if (oldval >= 0) {
			printf("addenv : the value of %s = %s alreay existes..\n", name, envptr);
			return -1;
		}
	}

	/*
	 * Append new definition at the end
	 */
	for (envptr = env_data; *envptr || *(envptr + 1); ++envptr)
	;
	if (envptr > env_data)
		++envptr;

	len = strlen(name);
	if (len <= 0) {
		printf("strnlen failed\n");
		return -1;
	}
	len += 2;
	/* add '=' for first arg, ' ' for all others */

	val_len = strlen(val);
	if (val_len <= 0) {
		printf("strnlen failed\n");
		return -1;
	}
	len += val_len;
	len += 1;

	printf("addenv : setting %s=%s for %d bytes\n", name, val, len);

	if (len > (&env_data[ENV_SIZE] - envptr)) {
		printf("## Error: environment overflow, \"%s\" deleted\n", name);
		return -1;
	}
	while ((*envptr = *name++) != '\0')
		envptr++;

	*envptr = '=';
	while ((*++envptr = *val++) != '\0')
	;

	/* end is marked with double '\0' */
	*++envptr = '\0';

	/* end is marked with double '\0' */
	*++envptr = '\0';

	/* Update CRC */
	env_crc_update();
	printf("addenv(): CRC updated\n");

	return 0;
}

int set_env(char *name, char *val)
{
	int len, val_len, oldval;
	unsigned char *envptr, *nxt = NULL;
	unsigned char *env_data = env.data;

	if (!env_data) {	/* need copy in RAM */
		printf("set_env(): env_data doesn't exist\n");
		return 1;
	}

	/*
	 * search if variable with this name already exists
	 */
	oldval = -1;
	for (envptr = env_data; *envptr; envptr = nxt + 1) {
		for (nxt = envptr; *nxt; ++nxt)
		;
		oldval = envmatch(name, envptr - env_data);
		if (oldval >= 0)
			break;
	}

	printf("set_env : the old value of %s\n", envptr);
	/*
	 * Delete any existing definition
	 */
	if (oldval >= 0) {
		if (*++nxt == '\0') {
			if (envptr > env_data)
				envptr--;
			else
				*envptr = '\0';
		} else {
			for (;;) {
				*envptr = *nxt++;
				if ((*envptr == '\0') && (*nxt == '\0'))
					break;
				++envptr;
			}
		}
		*++envptr = '\0';
	}

	/*
	 * Append new definition at the end
	 */
	for (envptr = env_data; *envptr || *(envptr + 1); ++envptr)
	;
	if (envptr > env_data)
		++envptr;
	len = strlen(name);
	if (len <= 0) {
		printf("strnlen failed\n");
		return -1;
	}
	len += 2;
	/* add '=' for first arg, ' ' for all others */

	val_len = strlen(val);
	if (val_len <= 0) {
		printf("strnlen for value failed %s\n", val);
		return -1;
	}
	len += val_len;
	len += 1;

	printf("set_env : setting %s=%s for %d bytes\n", name, val, len);

	if (len > (&env_data[ENV_SIZE] - envptr)) {
		printf("## Error: environment overflow, \"%s\" deleted\n", name);
		return 1;
	}
	while ((*envptr = *name++) != '\0')
		envptr++;

	*envptr = '=';
	while ((*++envptr = *val++) != '\0')
	;

	/* end is marked with double '\0' */
	*++envptr = '\0';

	/* Update CRC */
	env_crc_update();
	printf("set_env(): CRC updated\n");

	return 0;
}

#define getenv(x)		get_env(x)
#ifdef PLATFORM_LGM
int mtdwrite(void)
{
	char cmd[2000] = {0};
	cmd[0] = '\0';
	int fd, fd_temp;
	int bytes_read = 0;
	int len = 0, total_len = 0;

	if (active_cfg <= 2) {
		if (active_cfg < 2)
			fd = open(getEnvBdev(), O_WRONLY | O_TRUNC | O_CREAT, 0644);
		else
			fd = open(getEnvAdev(), O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (fd < 0) {
			printf("Saving enviornment failed for NAND\n");
			return 1;
		}
		fd_temp = open(TEMP_ENV_FILE, O_RDONLY , 0644);
		if (fd_temp < 0) {
			close(fd);
			printf("Saving enviornment failed for NAND\n");
			return 1;
		}
		do {
			bytes_read = read(fd_temp, cmd, sizeof(cmd));
			len = write(fd, cmd, bytes_read);
			total_len += len;
		} while (bytes_read != 0);

		if (total_len < (int)sizeof(cmd))
			printf("len written %d sizeof(env) %ld\n", total_len, sizeof(env));

		close(fd_temp);
		close(fd);
	} else {
		printf("Invalid active_cfg\n");
		return -1;
	}
	printf("Saving environment with CRC 0x%08x\n", env.crc);
	return 0;
}

int nandwrite(void)
{
	char cmd[255] = {0};
	cmd[0] = '\0';

	if (active_cfg < 2) {
		sprintf_s(cmd, sizeof(cmd), "ubiupdatevol  %s %s", getEnvBdev(), TEMP_ENV_FILE);
		if (system(cmd) != 0) { /* If unable to write in ubootconfigB partition, try ubootconfigA */
			sprintf_s(cmd, sizeof(cmd), "ubiupdatevol  %s %s", getEnvAdev(), TEMP_ENV_FILE);
			system(cmd);
		}
	} else {
		sprintf_s(cmd, sizeof(cmd), "ubiupdatevol  %s %s", getEnvAdev(), TEMP_ENV_FILE);
		if (system(cmd) != 0) { /* If unable to write in ubootconfigA partition, try ubootconfigB */
			sprintf_s(cmd, sizeof(cmd), "ubiupdatevol  %s %s", getEnvBdev(), TEMP_ENV_FILE);
			system(cmd);
		}
	}
	printf("Saving environment with CRC 0x%08x\n", env.crc);
	return 0;
}
#endif

int saveenv(void)
{
	int fd, len = 0;

	fd = open(TEMP_ENV_FILE, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd < 0) {
		printf("Saving enviornment failed for NAND\n");
		return 1;
	}
#ifdef CONFIG_SYS_REDUNDAND_ENVIRONMENT
	env.flags++;
#endif

	len = write(fd, (void *)&env, sizeof(env));
	if (len < (int)sizeof(env)) {
		printf
			("Saving enviornment failed for NAND. bytes written = %d acutal size = %zd\n",
			 len, sizeof(env));
	}
	printf("len written %d sizeof(env) %ld\n", len, sizeof(env));
	close(fd);
#ifdef PLATFORM_LGM
	if (lgmBoardType == FLASH_TYPE_EMMC)
		mtdwrite();
	else
		nandwrite();
#endif
	return 0;
}

