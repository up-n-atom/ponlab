// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2022 Maxlinear,Inc
 */
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/rawnand.h>
#include <linux/mtd/nand.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/spinlock.h>
#include <linux/of.h>
#include <linux/wait.h>

#define NONANDSUBPAGEREAD

#define NAND_MFR_MICRON   0x2c
#define NAND_MFR_MACRONIX 0xc2
#define NAND_MFR_WINBOND  0xef

/* Standard SPI-NAND flash commands */
#define CMD_READ                        0x13
#define CMD_READ_RDM                    0x03
#define CMD_PROG_PAGE_CLRCACHE          0x02
#define CMD_PROG_PAGE_CLRCACHE_X4       0x32
#define CMD_PROG_PAGE                   0x84
#define CMD_PROG_PAGE_EXC               0x10
#define CMD_ERASE_BLK                   0xd8
#define CMD_WR_ENABLE                   0x06
#define CMD_WR_DISABLE                  0x04
#define CMD_READ_ID                     0x9f
#define CMD_RESET                       0xff
#define CMD_READ_REG                    0x0f
#define CMD_WRITE_REG                   0x1f
#define CMD_READ_CACHE_X2               0x3b
#define CMD_READ_CACHE_X4               0x6b

/* feature registers */
#define REG_BLOCK_LOCK                  0xa0
#define REG_OTP                         0xb0
#define REG_STATUS                      0xc0 /* timing */

/* status */
#define STATUS_OIP_MASK                 0x01
#define STATUS_READY                    0
#define STATUS_BUSY                     BIT(1)

#define STATUS_E_FAIL_MASK              0x04
#define STATUS_E_FAIL                   BIT(2)

#define STATUS_P_FAIL_MASK              0x08
#define STATUS_P_FAIL                   BIT(3)

/* OTP register defines*/
#define OTP_QE_BIT          0x01
#define OTP_BUF_MODE                    0x08
#define OTP_ECC_ENABLE                  0x10
#define OTP_ENABLE                      0x40

/* block lock */
#define BL_ALL_LOCKED                   0x38
#define BL_1_2_LOCKED                   0x30
#define BL_1_4_LOCKED                   0x28
#define BL_1_8_LOCKED                   0x20
#define BL_1_16_LOCKED                  0x18
#define BL_1_32_LOCKED                  0x10
#define BL_1_64_LOCKED                  0x08
#define BL_ALL_UNLOCKED                 0x00

#define SPI_NAND_MT29F_ECC_MASK         3
#define SPI_NAND_MT29F_ECC_CORRECTED    1
#define SPI_NAND_MT29F_ECC_UNCORR       2
#define SPI_NAND_MT29F_ECC_RESERVED     3
#define SPI_NAND_MT29F_ECC_SHIFT        4

#define SPI_NAND_GD5F_ECC_MASK          7
#define SPI_NAND_GD5F_ECC_UNCORR        7
#define SPI_NAND_GD5F_ECC_SHIFT         4

struct spinand_info {
    struct nand_ecclayout *ecclayout;
    struct spi_device *spi;
    void *priv;
};

struct spinand_privbuf {
    u32 col;
    u32 row;
    int     buf_ptr;
    u8      *buf;
};

struct spinand_cmd {
    u8      cmd;
    u32     n_addr;     /* Number of address */
    u8      addr[3];    /* Reg Offset */
    u32     n_dummy;    /* Dummy use */
    u32     n_tx;       /* Number of tx bytes */
    u8      *tx_buf;    /* Tx buf */
    u32     n_rx;       /* Number of rx bytes */
    u8      *rx_buf;    /* Rx buf */
};

#define NAND_MAX_OOBSIZE    640
#define NAND_MAX_PAGESIZE   8192

#define GIGADEVICE_DUMMY_TYPE 0x01000000
#define SPINAND_NEED_PLANE_SELECT 0x02000000


static int spinand_set_otp(struct spi_device *spi, u8 *otp);
static int spinand_get_otp(struct spi_device *spi, u8 *otp);
static int spinand_set_otp_mask(struct spi_device *spi, u8 otp, u8 mask);
static int spinand_lock_block(struct spi_device *spi_nand, u8 lock);
static int wait_till_ready(struct spi_device *spi);
static int spinand_read_status(struct spi_device *spi, uint8_t *status);

static inline struct spinand_privbuf *mtd_to_privbuf(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct spinand_info *info = (struct spinand_info *)chip->priv;
	struct spinand_privbuf *state = (struct spinand_privbuf *)info->priv;

	return state;
}

static inline struct spinand_info *spi_to_info(struct spi_device *spi)
{
	struct mtd_info *mtd = (struct mtd_info *)
				dev_get_drvdata((const struct device *)
						&spi->dev);
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct spinand_info *info = (struct spinand_info *)chip->priv;

	return info;
}

#define LP_OPTIONS NAND_SAMSUNG_LP_OPTIONS

struct nand_flash_dev spinand_flash_ids[] = {
	{"SPI NAND Gigadevice 1Gbit 3.3v",
		{ .id = {0xc8, 0xb1} }, 2048, 128, 0x20000,
		 LP_OPTIONS | GIGADEVICE_DUMMY_TYPE, 2, 128},
	{"SPI NAND Gigadevice 2Gbit 3.3v",
		{ .id = {0xc8, 0xb2} }, 2048, 256, 0x20000,
		 LP_OPTIONS | GIGADEVICE_DUMMY_TYPE, 2, 128},
	{"SPI NAND Gigadevice 4Gbit 3.3v",
		{ .id = {0xc8, 0xb4} }, 4096, 512, 0x40000,
		 LP_OPTIONS | GIGADEVICE_DUMMY_TYPE, 2, 256},
	{"SPI NAND Gigadevice 4Gbit 1.8v",
		{ .id = {0xc8, 0xA4} }, 4096, 512, 0x40000,
		 LP_OPTIONS | GIGADEVICE_DUMMY_TYPE, 2, 256},
	{"SPI NAND Micron 128MiB 3,3V 8-bit",
		{ .id = {0x2c, 0x11} }, 2048, 128, 0x20000, LP_OPTIONS, 2, 64},
	{"SPI NAND Micron 128MiB 3,3V 8-bit",
		{ .id = {0x2c, 0x12} }, 2048, 128, 0x20000, LP_OPTIONS, 2, 64},
	{"SPI NAND MT29F2G01AAAED 256MiB",
		{ .id = {0x2c, 0x22} }, 2048, 256, 0x20000,
		 LP_OPTIONS | SPINAND_NEED_PLANE_SELECT, 2, 64},
	{"SPI NAND MT29F2G01ABAGD 256MiB",
		{ .id = {0x2c, 0x24} }, 2048, 256, 0x20000,
		 LP_OPTIONS | SPINAND_NEED_PLANE_SELECT, 2, 128},
	{"SPI NAND MT29F4G01AAADD 512MiB",
		{ .id = {0x2c, 0x32} }, 2048, 512, 0x20000,
		 LP_OPTIONS | SPINAND_NEED_PLANE_SELECT, 2, 64},
	{"SPI NAND MT29F1G01ABB 1Gbit",
		{ .id = {0x2c, 0x15} }, 2048, 128, 0x20000, LP_OPTIONS, 2, 128},
	{"SPI NAND MT29F2G01ABBG 2Gbit 1,8v",
		{ .id = {0x2c, 0x25} }, 2048, 256, 0x20000,
		 LP_OPTIONS | SPINAND_NEED_PLANE_SELECT, 2, 64},
	{"SPI NAND TC58CVG0S3 1Gbit",
		{ .id = {0x98, 0xc2} }, 2048, 128, 0x20000, LP_OPTIONS, 2, 64},
	{"SPI NAND TC58CVG1S3 2Gbit",
		{ .id = {0x98, 0xcb} }, 2048, 256, 0x20000, LP_OPTIONS, 2, 64},
	{"SPI NAND TC58CVG2S0 4Gbit",
		{ .id = {0x98, 0xcd} }, 4096, 512, 0x40000, LP_OPTIONS, 2, 128},
	{"SPI NAND MX35LFE4AB 1Gbit",
		{ .id = {0xc2, 0x12} }, 2048, 128, 0x20000, LP_OPTIONS, 2, 64},
	{"SPI NAND MX35LFE4AB 2Gbit",
		{ .id = {0xc2, 0x22} }, 2048, 256, 0x20000,
		 LP_OPTIONS | SPINAND_NEED_PLANE_SELECT, 2, 64},
	{"SPI NAND 1Gbit 3,3v WN25N01GV",
		{ .id = {0xef, 0xaa} }, 2048, 128, 0x20000, LP_OPTIONS, 2, 64},
	{"SPI NAND 1Gbit 1,8v WN25N01GW",
		{ .id = {0xef, 0xba} }, 2048, 128, 0x20000, LP_OPTIONS, 2, 64},
	{"SPI NAND 2Gbit 3,3v WN25M02GV",
		{ .id = {0xef, 0xab} }, 2048, 256, 0x20000, LP_OPTIONS, 2, 64},
};

static int spi_nand_manufacture_init(struct mtd_info *mtd, struct nand_chip *chip,
										struct nand_flash_dev *type)
{
	struct spinand_info *info = (struct spinand_info *)chip->priv;
	u8 config = 0;

	/* Default config for all flash:
	 * clear OTP mode, enable ECC, enable QE for quad
	 */
	config &= ~OTP_ENABLE;
	config |= OTP_ECC_ENABLE;
	if (info->spi->mode & (SPI_RX_QUAD | SPI_TX_QUAD))
		config |= OTP_QE_BIT;

	switch (type->mfr_id) {
	case NAND_MFR_MICRON:

		/* 4 bits/sector for 64 Bytes OOB, doubled for 128 Bytes OOB */
		chip->ecc.strength = mtd->oobsize / 16;
		chip->ecc.size     = mtd->writesize >> 2;
		chip->ecc.bytes    = 8;
		if (mtd->oobsize == 64)
			chip->bbt_options |= NAND_BBT_NO_OOB;
		break;
	case NAND_MFR_MACRONIX:
		/* 4 bits correction per page region (512 bytes) */
		chip->ecc.strength = 4;
		chip->ecc.size     = mtd->writesize >> 2;
		chip->ecc.bytes    = 8; /* !<TODO : ??? */
		break;

	case 0xC8: /* NAND_MFR_GIGADEVICE: */
		 /* 8 bits correction per page region (512 bytes) */
		chip->ecc.strength = 8;

		/* page region (512 bytes) */
		chip->ecc.size     = mtd->writesize >> 2;

		if (mtd->oobsize == 128)
			/* 64 bytes ecc total so 16 bytes per 4 sub pages */
			chip->ecc.bytes  = 16;
		else if (mtd->oobsize == 256)
			/* !<TODO : sorry i havent seen one chip like this,
			 * so had to assume 4 sub page so 128 / 4 = ????
			 */
			chip->ecc.bytes  = 32;

		break;

	case NAND_MFR_WINBOND:
		chip->ecc.strength = 1;
		chip->ecc.size = 512;
		chip->ecc.bytes = 8;

		chip->bbt_options |= NAND_BBT_NO_OOB;

		/* no QE bit, set buffer mode */
		config &= ~OTP_QE_BIT;
		config |= OTP_BUF_MODE;
		break;

	default:
		break;
	}

	spinand_set_otp_mask(info->spi, config, OTP_ENABLE |
			     OTP_ECC_ENABLE | OTP_BUF_MODE |
			     OTP_QE_BIT);

	return 0;
}

struct nand_flash_dev *spinand_flash_detect(struct mtd_info *mtd,
					    struct nand_chip *chip)
{
	u8 id_data[8];
	int i;
	struct nand_flash_dev *type = NULL;

	chip->legacy.cmdfunc(chip, NAND_CMD_READID, 0x00, -1);
	for (i = 0; i < 3; i++)
		id_data[i] = chip->legacy.read_byte(chip);

	if (!type)
		type = spinand_flash_ids;

	for (; type->name != NULL; type++)
		if (id_data[0] == type->mfr_id && id_data[1] == type->dev_id)
			break;

	if (!type->name) {
		pr_err("Unable to find flash id 0x%x 0x%x\n", id_data[0],
		       id_data[1]);
		goto probe_done;
	}

	/* found */
	mtd->writesize = type->pagesize;
	mtd->erasesize = type->erasesize;
	mtd->oobsize   = type->oobsize;

	mtd->size = (type->chipsize) << 20;

	nand_is_slc(chip); 
	chip->ecc.engine_type = NAND_ECC_ENGINE_TYPE_ON_HOST;
	chip->ecc.strength = 4;
	chip->ecc.size     = mtd->writesize >> 2;
	chip->ecc.bytes    = 8; /* !<TODO : ??? */

	/* do we need it ? */
	chip->options |= type->options;

	/* manufacturer init */
	spi_nand_manufacture_init(mtd, chip, type);

probe_done:
	return type;

};
EXPORT_SYMBOL(spinand_flash_detect);

static int spinand_cmd(struct spi_device *spi, struct spinand_cmd *cmd)
{
	struct spi_message message;
	struct spi_transfer x[4];
	u8 dummy = 0xff;

	spi_message_init(&message);
	memset(x, 0, sizeof(x));

	x[0].len = 1;
	x[0].tx_buf = &cmd->cmd;
	spi_message_add_tail(&x[0], &message);

	if (cmd->n_addr) {
		x[1].len = cmd->n_addr;
		x[1].tx_buf = cmd->addr;
		spi_message_add_tail(&x[1], &message);
	}

	if (cmd->n_dummy) {
		x[2].len = cmd->n_dummy;
		x[2].tx_buf = &dummy;
		spi_message_add_tail(&x[2], &message);
	}

	if (cmd->n_tx) {
		x[3].len = cmd->n_tx;
		x[3].tx_buf = cmd->tx_buf;
		spi_message_add_tail(&x[3], &message);
	}

	if (cmd->n_rx) {
		x[3].len = cmd->n_rx;
		x[3].rx_buf = cmd->rx_buf;
		spi_message_add_tail(&x[3], &message);
	}

	return spi_sync(spi, &message);
}

static int spinand_read_id(struct spi_device *spi, u8 *id)
{
	int retval;
	u8 nand_id[3];
	struct spinand_cmd cmd = {0};

	cmd.cmd = CMD_READ_ID;
	cmd.n_rx = 3;
	cmd.rx_buf = &nand_id[0];

	retval = spinand_cmd(spi, &cmd);
	if (retval < 0) {
		dev_err(&spi->dev, "error %d reading id\n", retval);
		return retval;
	}

	id[0] = nand_id[0];
	id[1] = nand_id[1];
	if (nand_id[0] == 0 || nand_id[0] == 0xFF) {
		id[0] = nand_id[1];
		id[1] = nand_id[2];
	}

	return retval;
}

static int spinand_read_status(struct spi_device *spi, uint8_t *status)
{
	struct spinand_cmd cmd = {0};
	int ret;

	cmd.cmd = CMD_READ_REG;
	cmd.n_addr = 1;
	cmd.addr[0] = REG_STATUS;
	cmd.n_rx = 1;
	cmd.rx_buf = status;

	ret = spinand_cmd(spi, &cmd);
	if (ret < 0)
		dev_err(&spi->dev, "err: %d read status register\n", ret);

	return ret;
}

#define MAX_WAIT_MS  40
static int wait_till_ready(struct spi_device *spi)
{
	int retval;
	u8 stat = 0;
	unsigned long deadline;

	deadline = jiffies + msecs_to_jiffies(MAX_WAIT_MS);
	do {
		retval = spinand_read_status(spi, &stat);
		if (retval < 0)
			return -1;
		else if (!(stat & 0x1))
			break;

		cond_resched();
	} while (!time_after_eq(jiffies, deadline));

	if ((stat & 0x1) == 0)
		return 0;

	return -1;
}

static int spinand_get_otp(struct spi_device *spi, u8 *otp)
{
	struct spinand_cmd cmd = {0};
	int retval;

	cmd.cmd = CMD_READ_REG;
	cmd.n_addr = 1;
	cmd.addr[0] = REG_OTP;
	cmd.n_rx = 1;
	cmd.rx_buf = otp;

	retval = spinand_cmd(spi, &cmd);
	if (retval < 0)
		dev_err(&spi->dev, "error %d get otp\n", retval);
	return retval;
}

static int spinand_set_otp(struct spi_device *spi, u8 *otp)
{
	int retval;
	struct spinand_cmd cmd = {0};

	cmd.cmd = CMD_WRITE_REG,
	cmd.n_addr = 1,
	cmd.addr[0] = REG_OTP,
	cmd.n_tx = 1,
	cmd.tx_buf = otp,

	retval = spinand_cmd(spi, &cmd);
	if (retval < 0)
		dev_err(&spi->dev, "error %d set otp\n", retval);

	return retval;
}

static int spinand_set_otp_mask(struct spi_device *spi, u8 otp, u8 mask)
{
	u8 val;
	int ret;

	ret = spinand_get_otp(spi, &val);
	if (ret)
		return ret;

	val = (val & ~mask) | otp;

	return spinand_set_otp(spi, &otp);
}

static int spinand_write_enable(struct spi_device *spi)
{
	struct spinand_cmd cmd = {0};

	cmd.cmd = CMD_WR_ENABLE;
	return spinand_cmd(spi, &cmd);
}

static int spinand_read_page_to_cache(struct spi_device *spi, int page_id)
{
	struct spinand_cmd cmd = {0};
	u16 row;

	row = page_id;
	cmd.cmd = CMD_READ;
	cmd.n_addr = 3;
	cmd.addr[0] = (u8)(page_id >> 16);
	cmd.addr[1] = (u8)(page_id >> 8);
	cmd.addr[2] = (u8)(page_id);

	return spinand_cmd(spi, &cmd);
}

static int spinand_read_from_cache(struct spi_device *spi, int page_id,
				   u16 byte_id, u16 len, u8 *rbuf)
{
	struct mtd_info *mtd = (struct mtd_info *)
				dev_get_drvdata((const struct device *)
						&spi->dev);
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct spinand_info *info = (struct spinand_info *)chip->priv;
	struct spinand_cmd cmd = {0};
	u16 column;
	u16 plane_select;

	column = byte_id;
	if (chip->options & SPINAND_NEED_PLANE_SELECT) {
		plane_select = (page_id >> 6) & 0x1;
		column |= plane_select << 12;
	}

	if (info->spi->mode & SPI_RX_QUAD)
		cmd.cmd = CMD_READ_CACHE_X4;
	else if (info->spi->mode & SPI_RX_DUAL)
		cmd.cmd = CMD_READ_CACHE_X2;
	else
		cmd.cmd = CMD_READ_RDM;

	if (chip->options & GIGADEVICE_DUMMY_TYPE) {
		cmd.addr[0] = 0xff;
		cmd.addr[1] = (u8)(column >> 8);
		cmd.addr[2] = (u8)column;
		cmd.n_addr = 3;
		cmd.n_dummy = 0;
	} else {
		cmd.addr[0] = (u8)(column >> 8);
		cmd.addr[1] = (u8)(column);
		cmd.n_addr = 2;
		cmd.n_dummy = 1;
	}

	cmd.n_rx = len;
	cmd.rx_buf = rbuf;

	return spinand_cmd(spi, &cmd);
}

static void spinand_ecc_status(struct spi_device *spi,
			       unsigned int status, unsigned int *ecc_error)
{
	struct mtd_info *mtd = (struct mtd_info *)
				dev_get_drvdata((const struct device *)
						&spi->dev);
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct spinand_info *info = (struct spinand_info *)chip->priv;
	unsigned int ecc_status;

	if (info->spi->mode & SPI_RX_QUAD) {
		ecc_status = (status >> SPI_NAND_MT29F_ECC_SHIFT) &
				SPI_NAND_MT29F_ECC_MASK;
		*ecc_error = (ecc_status == SPI_NAND_MT29F_ECC_UNCORR);
	} else if (chip->options & GIGADEVICE_DUMMY_TYPE) {
		ecc_status = (status >> SPI_NAND_GD5F_ECC_SHIFT) &
				SPI_NAND_GD5F_ECC_MASK;
		*ecc_error = (ecc_status == SPI_NAND_GD5F_ECC_UNCORR);

	} else {
		ecc_status = (status >> SPI_NAND_MT29F_ECC_SHIFT) &
				SPI_NAND_MT29F_ECC_MASK;
		*ecc_error = (ecc_status == SPI_NAND_MT29F_ECC_UNCORR);
	}

	if (*ecc_error == 0) /* if not unrecoverable error */
		mtd->ecc_stats.corrected += ecc_status;
	else /* exceed correctable bits or error*/
		mtd->ecc_stats.failed++;
}

static int spinand_read_page(struct spi_device *spi, int page_id,
			     u16 offset, u16 len, u8 *rbuf)
{
	int ret, ecc_error;
	u8 status = 0;

	ret = spinand_read_page_to_cache(spi, page_id);
	if (ret < 0)
		return ret;

	if (wait_till_ready(spi))
		dev_err(&spi->dev, "WAIT timedout!!!\n");

	while (1) {
		ret = spinand_read_status(spi, &status);
		if (ret < 0) {
			dev_err(&spi->dev,
				"err %d read status register\n", ret);
			return ret;
		}

		if ((status & STATUS_OIP_MASK) == STATUS_READY) {
			spinand_ecc_status(spi, status, &ecc_error);
			if (ecc_error) {
				dev_err(&spi->dev, "ecc error, page=%d\n",
					page_id);
			}
			break;
		}
	}

	ret = spinand_read_from_cache(spi, page_id, offset, len, rbuf);
	if (ret < 0) {
		dev_err(&spi->dev, "read from cache failed!!\n");
		return ret;
	}

	return ret;
}

static int spinand_program_data_to_cache(struct spi_device *spi, int page_id,
					 u16 byte_id, u16 len, u8 *wbuf)
{
	struct mtd_info *mtd = (struct mtd_info *)
				dev_get_drvdata((const struct device *)
						&spi->dev);
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct spinand_info *info = (struct spinand_info *)chip->priv;
	struct spinand_cmd cmd = {0};
	u16 column;
	u16 plane_select;

	column = byte_id;
	if (chip->options & SPINAND_NEED_PLANE_SELECT) {
		plane_select = (page_id >> 6) & 0x1;
		column |= plane_select << 12;
	}

	if (info->spi->mode & SPI_TX_QUAD)
		cmd.cmd = CMD_PROG_PAGE_CLRCACHE_X4;
	else
		cmd.cmd = CMD_PROG_PAGE_CLRCACHE;
	cmd.n_addr = 2;
	cmd.addr[0] = (u8)((column & 0xff00) >> 8);
	cmd.addr[1] = (u8)(column & 0x00ff);
	cmd.n_tx = len;
	cmd.tx_buf = wbuf;

	return spinand_cmd(spi, &cmd);
}

static int spinand_program_execute(struct spi_device *spi, int page_id)
{
	struct spinand_cmd cmd = {0};

	cmd.cmd = CMD_PROG_PAGE_EXC;
	cmd.n_addr = 3;
	cmd.addr[0] = (u8)(page_id >> 16);
	cmd.addr[1] = (u8)(page_id >> 8);
	cmd.addr[2] = (u8)(page_id);

	return spinand_cmd(spi, &cmd);
}

static int spinand_program_page(struct spi_device *spi, int page_id,
				u16 offset, u16 len, u8 *buf)
{
	int retval;
	u8 status = 0;
	u8 *wbuf;
	int retry = 3;

	wbuf = buf;

	retval = spinand_write_enable(spi);
	if (retval < 0) {
		dev_err(&spi->dev, "write enable failed!!\n");
		return retval;
	}
	if (wait_till_ready(spi))
		dev_err(&spi->dev, "wait timedout!!!\n");

	while (retry--) {
		retval = spinand_program_data_to_cache(spi, page_id, offset,
						       len, wbuf);
		if (retval != -EAGAIN)
			break;

		dev_dbg(&spi->dev, "fail to program data to cache, retrying\n");
		cpu_relax();
	}

	if (retval < 0) {
		dev_err(&spi->dev, "error %d program data to cache\n", retval);
		return retval;
	}

	retval = spinand_program_execute(spi, page_id);
	if (retval < 0)
		return retval;
	while (1) {
		retval = spinand_read_status(spi, &status);
		if (retval < 0) {
			dev_err(&spi->dev,
				"error %d reading status register\n",
				retval);
			return retval;
		}

		if ((status & STATUS_OIP_MASK) == STATUS_READY) {
			if ((status & STATUS_P_FAIL_MASK) == STATUS_P_FAIL) {
				dev_err(&spi->dev,
					"program error, page %d\n", page_id);
				return -1;
			}
			break;
		}
	}

	return 0;
}

static int spinand_erase_block_erase(struct spi_device *spi, u32 block_id)
{
	struct spinand_cmd cmd = {0};
	u16 row;

	row = block_id;
	cmd.cmd = CMD_ERASE_BLK;
	cmd.n_addr = 3;
	cmd.addr[0] = (u8)(block_id >> 16);
	cmd.addr[1] = (u8)(block_id >> 8);
	cmd.addr[2] = (u8)(block_id);

	return spinand_cmd(spi, &cmd);
}

static int spinand_erase_block(struct spi_device *spi, u32 block_id)
{
	int retval;
	u8 status = 0;

	retval = spinand_write_enable(spi);
	if (wait_till_ready(spi))
		dev_err(&spi->dev, "WAIT timedout!!!\n");

	retval = spinand_erase_block_erase(spi, block_id);
	while (1) {
		retval = spinand_read_status(spi, &status);
		if (retval < 0) {
			dev_err(&spi->dev,
				"error %d reading status register\n", retval);
			return retval;
		}

		if ((status & STATUS_OIP_MASK) == STATUS_READY) {
			if ((status & STATUS_E_FAIL_MASK) == STATUS_E_FAIL) {
				dev_err(&spi->dev,
					"erase error, block %d\n", block_id);
				return -1;
			}
			break;
		}
	}
	return 0;
}

static int spinand_lock_block(struct spi_device *spi_nand, u8 lock)
{
	struct spinand_cmd cmd = {0};
	int ret;
	u8 otp = 0;

	ret = spinand_get_otp(spi_nand, &otp);

	cmd.cmd = CMD_WRITE_REG;
	cmd.n_addr = 1;
	cmd.addr[0] = REG_BLOCK_LOCK;
	cmd.n_tx = 1;
	cmd.tx_buf = &lock;

	ret = spinand_cmd(spi_nand, &cmd);
	if (ret < 0)
		dev_err(&spi_nand->dev, "error %d lock block\n", ret);

	return ret;
}

static void spinand_reset(struct spi_device *spi)
{
	struct spinand_cmd cmd = {0};

	cmd.cmd = CMD_RESET;

	if (spinand_cmd(spi, &cmd) < 0)
		pr_info("spinand reset failed!\n");

	/* elapse 1ms before issuing any other command */
	udelay(1000);

	if (wait_till_ready(spi))
		dev_err(&spi->dev, "WAIT timedout!\n");

	/* safety try to unlock after reset */
	spinand_lock_block(spi, BL_ALL_UNLOCKED);

	if (wait_till_ready(spi))
		dev_err(&spi->dev, "WAIT timedout!\n");

	/* safety enable On-Die ECC again in case */
	spinand_set_otp_mask(spi, OTP_ECC_ENABLE, OTP_ECC_ENABLE);
}

static int spinand_write_page_hwecc(struct nand_chip *chip, const uint8_t *buf,
				    int oob_required, int page)
{
	struct mtd_info *mtd = nand_to_mtd(chip);
	const u8 *p = buf;

	int eccsize = chip->ecc.size;
	int eccsteps = chip->ecc.steps;

	nand_prog_page_begin_op(chip, page, 0, p, eccsize * eccsteps);
	if (oob_required)
		chip->legacy.write_buf(chip, chip->oob_poi, mtd->oobsize);

	return nand_prog_page_end_op(chip);
}

static int spinand_read_page_hwecc(struct nand_chip *chip,
				   u8 *buf, int oob_required, int page)
{
	struct mtd_info *mtd = nand_to_mtd(chip);
	u8 retval, status;
	u8 *p = buf;
	int eccsize = chip->ecc.size;
	int eccsteps = chip->ecc.steps;
	struct spinand_info *info = (struct spinand_info *)chip->priv;
	int ecc_error;

	nand_read_page_op(chip, page, 0, p, eccsize * eccsteps);
	if (oob_required)
		chip->legacy.read_buf(chip, chip->oob_poi, mtd->oobsize);

	while (1) {
		retval = spinand_read_status(info->spi, &status);
		if ((status & STATUS_OIP_MASK) == STATUS_READY) {
			spinand_ecc_status(info->spi, status, &ecc_error);
			break;
		}
	}
	return 0;
}

static void spinand_select_chip(struct nand_chip *chip, int dev)
{
}

static uint8_t spinand_read_byte(struct nand_chip *chip)
{
	struct mtd_info *mtd = nand_to_mtd(chip);
	struct spinand_privbuf *state = mtd_to_privbuf(mtd);
	u8 data;

	data = state->buf[state->buf_ptr];
	state->buf_ptr++;
	return data;
}

static int spinand_wait(struct nand_chip *chip)
{
	struct spinand_info *info = (struct spinand_info *)chip->priv;

	unsigned long timeo = jiffies;
	int retval;
	u8 status;

//	if (state == FL_ERASING)  VADIVEL: check if not erasing properly
//		timeo += msecs_to_jiffies(400);
//	else
		timeo += msecs_to_jiffies(20);

	while (time_before(jiffies, timeo)) {
		retval = spinand_read_status(info->spi, &status);
		if ((status & STATUS_OIP_MASK) == STATUS_READY)
			return 0;

		cond_resched();
	}
	return 0;
}

static void spinand_write_buf(struct nand_chip *chip, const uint8_t *buf, int len)
{  
	struct mtd_info *mtd = nand_to_mtd(chip);
	struct spinand_privbuf *state = mtd_to_privbuf(mtd);

	memcpy(state->buf + state->buf_ptr, buf, len);
	state->buf_ptr += len;
}

static void spinand_read_buf(struct nand_chip *chip, uint8_t *buf, int len)
{
	struct mtd_info *mtd = nand_to_mtd(chip);
	struct spinand_privbuf *state = mtd_to_privbuf(mtd);

	memcpy(buf, state->buf + state->buf_ptr, len);
	state->buf_ptr += len;
}

static void spinand_cmdfunc(struct nand_chip *chip, unsigned int command,
			    int column, int page_addr)
{
	struct mtd_info *mtd = nand_to_mtd(chip);
	struct spinand_info *info = (struct spinand_info *)chip->priv;
	struct spinand_privbuf *state = (struct spinand_privbuf *)info->priv;

	switch (command) {
	/*
	 * READ0 - read in page + oob bytes
	 */
	case NAND_CMD_READ1:
	case NAND_CMD_READ0:
		state->buf_ptr = 0;
		spinand_read_page(info->spi, page_addr, 0x0,
				  mtd->writesize + mtd->oobsize, state->buf);
		break;
	/* READOOB reads only the OOB because no ECC is performed. */
	case NAND_CMD_READOOB:
		state->buf_ptr = 0;
		spinand_read_page(info->spi, page_addr, mtd->writesize,
				  mtd->oobsize, state->buf);
		break;
	case NAND_CMD_RNDOUT:
		state->buf_ptr = column;
		break;
	case NAND_CMD_READID:
		state->buf_ptr = 0;
		spinand_read_id(info->spi, (u8 *)state->buf);
		break;
	case NAND_CMD_PARAM:
		state->buf_ptr = 0;
		break;
	/* ERASE1 stores the block and page address */
	case NAND_CMD_ERASE1:
		spinand_erase_block(info->spi, page_addr);
		break;
	/* ERASE2 uses the block and page address from ERASE1 */
	case NAND_CMD_ERASE2:
		break;
	/* SEQIN sets up the addr buffer and all registers except the length */
	case NAND_CMD_SEQIN:
		state->col = column;
		state->row = page_addr;
		state->buf_ptr = 0;
		break;
	/* PAGEPROG reuses all of the setup from SEQIN and adds the length */
	case NAND_CMD_PAGEPROG:
		spinand_program_page(info->spi, state->row, state->col,
				     state->buf_ptr, state->buf);
		break;
	case NAND_CMD_STATUS:
		spinand_get_otp(info->spi, state->buf);
		if (!(state->buf[0] & 0x80))
			state->buf[0] = 0x80;
		state->buf_ptr = 0;
		break;
	/* RESET command */
	case NAND_CMD_RESET:
		if (wait_till_ready(info->spi))
			dev_err(&info->spi->dev, "WAIT timedout!!!\n");
		/* a minimum of 250us must elapse before issuing RESET cmd*/
		udelay(250);
		spinand_reset(info->spi);
		break;
	default:
		dev_err(&mtd->dev, "Unknown CMD: 0x%x\n", command);
	}
}

static int spinand_probe(struct spi_device *spi)
{
	struct mtd_info *mtd;
	struct nand_chip *chip;
	struct spinand_info *info;
	struct spinand_privbuf *state;
	struct mtd_part_parser_data ppdata;
	const char __maybe_unused	*of_mtd_name = NULL;

	of_property_read_string(spi->dev.of_node,
				"linux,mtd-name", &of_mtd_name);

	info  = devm_kzalloc(&spi->dev, sizeof(struct spinand_info),
			     GFP_KERNEL);

	if (!info)
		return -ENOMEM;

	/* back reference to spi_device */
	info->spi = spi;

	//spinand_lock_block(spi, BL_ALL_UNLOCKED);

	state = devm_kzalloc(&spi->dev, sizeof(struct spinand_privbuf),
			     GFP_KERNEL);
	if (!state)
		return -ENOMEM;

	info->priv	= state;
	state->buf_ptr	= 0;
	state->buf	= devm_kzalloc(&spi->dev,
				       NAND_MAX_PAGESIZE + NAND_MAX_OOBSIZE,
				       GFP_KERNEL);

	if (!state->buf)
		return -ENOMEM;

	chip = devm_kzalloc(&spi->dev, sizeof(struct nand_chip), GFP_KERNEL);
	if (!chip)
		return -ENOMEM;

	chip->ecc.engine_type	= NAND_ECC_ENGINE_TYPE_NONE;

	chip->ecc.read_page = spinand_read_page_hwecc;
	chip->ecc.write_page = spinand_write_page_hwecc;

	/* -- replace and overload SPI NAND functions to NAND CHIP struct --- */
	chip->priv	= info;
	chip->legacy.read_buf	= spinand_read_buf;
	chip->legacy.write_buf	= spinand_write_buf;
	chip->legacy.read_byte	= spinand_read_byte;
	chip->legacy.cmdfunc	= spinand_cmdfunc;
	chip->legacy.waitfunc	= spinand_wait;
	chip->options	|= NAND_CACHEPRG;
	#ifdef NONANDSUBPAGEREAD
	chip->options	|= NAND_NO_SUBPAGE_WRITE;
	#endif /* NONANDSUBPAGEREAD */
	chip->bbt_options = NAND_BBT_USE_FLASH | NAND_BBT_CREATE_EMPTY;
	chip->legacy.select_chip = spinand_select_chip;

	mtd = nand_to_mtd(chip);

	dev_set_drvdata(&spi->dev, mtd);

	if (of_mtd_name)
		mtd->name = of_mtd_name;
	else
		mtd->name = dev_name(&spi->dev);

	mtd->owner = THIS_MODULE;
	mtd_set_of_node(mtd, spi->dev.of_node);

	if (nand_scan(chip, 1))
		return -ENXIO;

	return mtd_device_parse_register(mtd, NULL, &ppdata, NULL, 0);
}

static int spinand_remove(struct spi_device *spi)
{
	mtd_device_unregister(dev_get_drvdata(&spi->dev));

	return 0;
}

static const struct of_device_id mxl_spinand_dt[] = {
	{ .compatible = "mxl,spinand", },
	{}
};

/*
 * SPI Driver name structure description
 */
static struct spi_driver mxl_spinand_driver = {
	.driver = {
		.name		= "mxl_spinand",
		.bus		= &spi_bus_type,
		.owner		= THIS_MODULE,
		.of_match_table	= mxl_spinand_dt,
	},
	.probe		= spinand_probe,
	.remove		= spinand_remove,
};

module_spi_driver(mxl_spinand_driver);
