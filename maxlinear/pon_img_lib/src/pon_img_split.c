/******************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 * Copyright (c) 2018 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <getopt.h>
#include <arpa/inet.h>

/** \addtogroup PON_IMG_LIB
 *  @{
 */

/** Image Name Length */
#define IH_NMLEN		32
/** Maximum volume name length */
#define MAX_VOLUME_NAME		64

/** Image file names */
#define IMG_ROOTFS	"img-rootfs"
#define IMG_KERNEL	"img-kernel"
#define IMG_BOOTCORE	"img-bootcore"
#define IMG_VERSION	"img-version"

/** OS Kernel Image */
#define IH_TYPE_KERNEL		2
/** Multi-file Image */
#define IH_TYPE_MULTI		4
/** Filesystem Image (any type) */
#define IH_TYPE_FILESYSTEM	7

/** This structure decode image headers */
struct image_header {
	/** Image Header Magic Number */
	uint32_t	ih_magic;
	/** Image Header CRC Checksum */
	uint32_t	ih_hcrc;
	/** Image Creation Timestamp */
	uint32_t	ih_time;
	/** Image Data Size */
	uint32_t	ih_size;
	/** Data Load Address */
	uint32_t	ih_load;
	/** Entry Point Address */
	uint32_t	ih_ep;
	/** Image Data CRC Checksum */
	uint32_t	ih_dcrc;
	/** Operating System */
	uint8_t		ih_os;
	/** CPU Architecture */
	uint8_t		ih_arch;
	/** Image Type */
	uint8_t		ih_type;
	/** Compression Type */
	uint8_t		ih_comp;
	/** Image Name */
	uint8_t		ih_name[IH_NMLEN];
};

static const char *help =
	"Options:\n"
	"-f, --filename	Mandatory! Name of the file containing image.\n"
	"-h, --help	Print help and exit.\n"
	"-v, --verbose	Enable verbose mode for more debug data.\n"
	;

static void print_help(char *app_name)
{
	printf("Usage: %s [options]\n", app_name);
	printf("%s", help);
}

static struct option long_opts[] = {
	{"filename", required_argument, 0, 'f'},
	{"help", no_argument, 0, 'h'},
	{"dryrun", no_argument, 0, 'd'},
	{"verbose", no_argument, 0, 'v'},
	{0, 0, 0, 0}
};

/** Options string */
static const char opt_string[] = "f:hdv";

static bool verbose;
static bool dryrun;
static char *filename;

static int parse_args(int argc, char *argv[])
{
	int c;
	int index;
	int error = 0;

	do {
		c = getopt_long(argc, argv, opt_string, long_opts, &index);

		if (c == -1)
			return 0;

		switch (c) {
		case 'h':
			print_help(argv[0]);
			error = 1;
			break;
		case 'v':
			verbose = true;
			break;
		case 'd':
			dryrun = true;
			break;
		case 'f':
			if (!optarg) {
				printf("Missing value for argument '-f'\n");
				error = 1;
				break;
			}
			filename = optarg;
			break;
		default:
			break;
		}
	} while (!error);

	return 1;
}

static long padded_len(long len, int pad)
{
	return ((len + pad - 1) / pad) * pad;
}

static int write_output(const char *filename, int fd_in, size_t len)
{
	static unsigned char buffer[1024];
	size_t remaining = len;
	int fd_out;
	int ret = 0;

	fd_out = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd_out < 0) {
		fprintf(stderr, "Could not create file \"%s\": %s\n",
			filename, strerror(errno));
		return -1;
	}

	while (remaining > 0) {
		int to_copy = sizeof(buffer);

		if (to_copy > remaining)
			to_copy = remaining;

		ret = read(fd_in, buffer, to_copy);
		if (ret <= 0) {
			fprintf(stderr, "read error: %s\n", strerror(errno));
			ret = -1;
			goto error_out;
		}

		ret = write(fd_out, buffer, ret);
		if (ret <= 0) {
			fprintf(stderr, "write error: %s\n", strerror(errno));
			ret = -1;
			goto error_out;
		}

		remaining -= ret;
	}
	ret = 0;

error_out:
	close(fd_out);

	return ret;
}

static int write_version(const char *filename, const char *version, size_t len)
{
	int fd_out;
	int ret = 0;
	size_t real_len = strnlen(version, len);

	fd_out = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd_out < 0) {
		fprintf(stderr, "Could not create file \"%s\": %s\n",
			filename, strerror(errno));
		return -1;
	}

	ret = write(fd_out, version, real_len);
	if (ret <= 0) {
		fprintf(stderr, "write error: %s\n", strerror(errno));
		ret = -1;
		goto error_out;
	}
	ret = 0;

error_out:
	close(fd_out);

	return ret;
}

static int pon_img_split(const char *filename)
{
	const size_t img_hdr_sz = sizeof(struct image_header);
	struct image_header img_hdr;
	long filesize = -1, file_offset = 0, img_read;
	const char *name;
	/* include \0 here, might be missing in header */
	char img_version[IH_NMLEN + 1] = { 0 };
	bool img_version_set = false;
	int ret = 0, err = 0;
	int fd_in;

	fd_in = open(filename, O_RDONLY);
	if (fd_in < 0) {
		fprintf(stderr, "Could not open file \"%s\": %s\n",
			filename, strerror(errno));
		return -1;
	}

	do {
		/* Reposition file read offset to the start of new header */
		if (lseek(fd_in, file_offset, SEEK_SET) == -1) {
			fprintf(stderr, "lseek error: %s\n", strerror(errno));
			err = -1;
			goto exit;
		}

		/* Read image header */
		ret = read(fd_in, &img_hdr, img_hdr_sz);
		if (ret < img_hdr_sz || ntohl(img_hdr.ih_size) < 1) {
			fprintf(stderr, "read error: %s\n", strerror(errno));
			err = -1;
			goto exit;
		}

		if (verbose)
			fprintf(stderr,
				"Image Header:\n"
				"- Data Size = %d\n"
				"- Image Name = %s\n"
				"- Image Type = %d\n",
				ntohl(img_hdr.ih_size),
				img_hdr.ih_name,
				img_hdr.ih_type);

		switch (img_hdr.ih_type) {
		case IH_TYPE_MULTI:
			/* The first multi image is defining the file size. */
			if (filesize == -1)
				filesize = file_offset + ntohl(img_hdr.ih_size);
			/* use the highest available level image version */
			if (!img_version_set) {
				snprintf(img_version, sizeof(img_version), "%s",
					 img_hdr.ih_name);
				img_version_set = true;
			}
			/* Recalculate offset to next header
			 * TODO: Explain the + 8
			 */
			file_offset += img_hdr_sz + 8;
			continue;
		case IH_TYPE_FILESYSTEM:
			/* Reposition offset to rootfs data */
			file_offset += img_hdr_sz;
			/* Calculate image scope */
			img_read = padded_len(ntohl(img_hdr.ih_size), 16);
			/* Prepare partition name */
			name = IMG_ROOTFS;
			break;
		case IH_TYPE_KERNEL:
			/* Calculate image scope */
			img_read = img_hdr_sz +
				   padded_len(ntohl(img_hdr.ih_size), 16);
			if (!img_version_set) {
				snprintf(img_version, sizeof(img_version), "%s",
					 img_hdr.ih_name);
				img_version_set = true;
			}
			/* Prepare partition name */
			if (strncmp((char *)img_hdr.ih_name, "MIPS 4Kec Bootcore",
				    sizeof(img_hdr.ih_name)) == 0)
				name = IMG_BOOTCORE;
			else
				name = IMG_KERNEL;
			break;
		default:
			/* Calculate unknown/unsupported image scope */
			img_read = img_hdr_sz +
				   padded_len(ntohl(img_hdr.ih_size), 16);
			/* Recalculate offset to next header
			 * TODO: Explain the + 8
			 */
			file_offset += (img_read + 8);
			/* Print warning - all types should be supported! */
			fprintf(stderr, "Unknown or unsupported image type: %d\n",
				img_hdr.ih_type);
			continue;
		}

		/* Reposition the file read offset to the start of image */
		if (lseek(fd_in, file_offset, SEEK_SET) == -1) {
			fprintf(stderr, "lseek error: %s\n", strerror(errno));
			err = -1;
			goto exit;
		}

		if (!dryrun) {
			err = write_output(name, fd_in, img_read);
			if (err)
				goto exit;
		} else {
			fprintf(stderr, "Skip writing '%s', offset 0x%lx, length 0x%lx\n",
				name, file_offset, img_read);
		}

		file_offset += img_read;

	} while (filesize == -1 || filesize > file_offset);

	if (img_version_set) {
		/* store version */
		err = write_version(IMG_VERSION, img_version,
				    sizeof(img_version));
		if (err)
			goto exit;
	}

exit:
	close(fd_in);
	return err;
}

int main(int argc, char *argv[])
{
	int ret = 0;

	/* parse commands arguments */
	if (parse_args(argc, argv)) {
		/* return here if we print help or if we have problem */
		return 0;
	}

	if (filename)
		ret = pon_img_split(filename);

	return ret;
}
