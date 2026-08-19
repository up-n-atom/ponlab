/*****************************************************************************
 *
 * Copyright (c) 2023- 2024 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <linux/bits.h>
#include <byteswap.h>

#include <net/switch_api/lantiq_gsw_api.h>
#include "lif_api.h"

#define sys_le16_to_cpu(val) __bswap_16(val)
#define sys_cpu_to_le16(val) __bswap_16(val)

#define CTRL_BUSY_MASK		BIT(15)
#define CTRL_CMD_MASK		(BIT(15) - 1)
#define MAX_BUSY_LOOP		1000	/* roughly 10ms */
#define THR_RST_DATA		5

#define GSW_MMD_DEV			30
#define GSW_MMD_REG_CTRL		0
#define GSW_MMD_REG_LEN_RET		1
#define GSW_MMD_REG_DATA_FIRST		2
#define GSW_MMD_REG_DATA_LAST		95
#define GSW_MMD_REG_DATA_MAX_SIZE \
	(GSW_MMD_REG_DATA_LAST - GSW_MMD_REG_DATA_FIRST + 1)

#define MMD_API_SET_DATA_0		(0x0 + 0x2)
#define MMD_API_SET_DATA_1		(0x0 + 0x3)
#define MMD_API_SET_DATA_2		(0x0 + 0x4)
#define MMD_API_GET_DATA_0		(0x0 + 0x5)
#define MMD_API_GET_DATA_1		(0x0 + 0x6)
#define MMD_API_GET_DATA_2		(0x0 + 0x7)
#define MMD_API_RST_DATA		(0x0 + 0x8)

typedef struct {
	uint8_t phy_addr;
	uint8_t lif_id;
	int32_t fd;
} GSW_Device_t;

static struct {
	uint16_t ctrl;
	int16_t ret;
	uint16_t data[GSW_MMD_REG_DATA_MAX_SIZE * 3];
} shadow = {
	.ctrl = ~0,
	.ret = -1,
	.data = {0}
};

static GSW_Device_t gsw_dev;

static void _usleep(unsigned long us)
{
	for (volatile int j = 0; j < us; j++) {
		for (volatile int i = 0; i < 400; i++);
	}
}

/**
    Implements the MDIO library (PRX) Read Register Clause 22.

   \param
	uint8_t lif_id, Link Index number
	uint8_t pad, PHY address
	uint8_t dad, Register Address

   \return
	>= 0 No error: Register value
	LIF_API_RET_PINS_ERROR	Pins validity error
	LIF_API_RET_ACCESS_ERROR  Pins RW Access error
*/
int32_t lif_mdio_c22_read(int32_t fd, uint8_t lif_id, uint8_t pad, uint8_t dad)
{
	int32_t ret;
	uint16_t val;
	GSW_MAC_Cli_t Param;

	Param.cmdType = MAC_CLI_SET_MDIO_CL;
	Param.get = 0;
	Param.mac_idx = lif_id;	// <MAC 2 or 3>;
	Param.val[0]  = pad;	// <PHY Address>;
	Param.val[1]  = 1;	// 0: C45, 1 C22>;
	ret = ioctl(fd, GSW_XGMAC_CFG, &Param);
	if (ret < 0)
		return ret;
	Param.cmdType = MAC_CLI_SET_MDIO_RD;
	Param.get = 0;
	Param.mac_idx = lif_id;	// <MAC 2 or 3>;
	Param.val[0] = 0;	// <MMD Dev>;
	Param.val[1] = pad;	// <PHY Address>;
	Param.val[2] = dad;	// <Register Offset>;
	Param.val[3] = val;	// <Value>;
	val = ioctl(fd, GSW_XGMAC_CFG, &Param);

	return val;
}

/**
	Implements the MDIO library (PRX) Write Register Clause 22.

   \param
	uint8_t lif_id,	 Link Index number
	uint8_t pad,	 PHY address
	uint8_t dad,	 Register Address
	uint16_t val,	 Register value to write
   \return
	LIF_API_RET_SUCCESS	 No error
	LIF_API_RET_PINS_ERROR   Pins validity error
	LIF_API_RET_ACCESS_ERROR Pins RW Access error
*/
int32_t lif_mdio_c22_write(int32_t fd, uint8_t lif_id, uint8_t pad,
	uint8_t dad, uint16_t val)
{
	int32_t ret;
	GSW_MAC_Cli_t Param;

	Param.cmdType = MAC_CLI_SET_MDIO_CL;
	Param.get = 0;
	Param.mac_idx = lif_id;	// <MAC 2 or 3>;
	Param.val[0] = pad;	// <PHY Address>;
	Param.val[1] = 1;	// 0: C45, 1 C22>;

	ret = ioctl(fd, GSW_XGMAC_CFG, &Param);
	if (ret < 0)
		return ret;

	Param.cmdType = MAC_CLI_SET_MDIO_WR;
	Param.get = 0;
	Param.mac_idx = lif_id;	// <MAC 2 or 3>;
	Param.val[0] = 0;	// <MMD Dev>;
	Param.val[1] = pad;	// <PHY Address>;
	Param.val[2] = dad;	// <Register Offset>;
	Param.val[3] = val;	// <Value>;

	ret = ioctl(fd, GSW_XGMAC_CFG, &Param);

	return ret;
}

/**
    Implements the MDIO library (PyRPIO only) Write Register Clause 45.

   \param
	uint8_t lif_id, Link Index number
	uint8_t pad, PHY address
	uint8_t dad, DEVICE Address
	uint16_t reg, Register Address
	uint16_t val, Register value to write
   \return
	LIF_API_RET_SUCCESS	  No error
	LIF_API_RET_PINS_ERROR   Pins validity error
	LIF_API_RET_ACCESS_ERROR Pins RW Access error
*/
int32_t lif_mdio_c45_write(int32_t fd, uint8_t lif_id, uint8_t pad,
	uint8_t dad, uint16_t reg, uint16_t val)
{
	int32_t ret;
	GSW_MAC_Cli_t Param;

	Param.cmdType = MAC_CLI_SET_MDIO_CL;
	Param.get = 0;
	Param.mac_idx = lif_id;	// <MAC 2 or 3>;
	Param.val[0]  = pad;	// <PHY Address>;
	Param.val[1]  = 0;	// 0: C45, 1 C22>;

	ret = ioctl(fd, GSW_XGMAC_CFG, &Param);
	if (ret < 0)
		return ret;

	Param.cmdType = MAC_CLI_SET_MDIO_WR;
	Param.get = 0;
	Param.mac_idx = lif_id;	// <MAC 2 or 3>;
	Param.val[0] = dad;	// <MMD Dev>;
	Param.val[1] = pad;	// <PHY Address>;
	Param.val[2] = reg;	// <Register Offset>;
	Param.val[3] = val;	// <Value>;

	ret = ioctl(gsw_dev.fd, GSW_XGMAC_CFG, &Param);

	return ret;
}

/**
	Implements the MDIO library Read Register Clause 45.

   \param
	uint8_t lif_id,	 Link Index number
	uint8_t pad,	   PHY address
	uint8_t dad,	   DEVICE address
	uint16_t reg,	   Register Address

   \return
	>= 0  No error: Register value
	LIF_API_RET_PINS_ERROR	Pins validity error
	LIF_API_RET_ACCESS_ERROR  Pins RW Access error
*/
int32_t lif_mdio_c45_read(int32_t fd, uint8_t lif_id, uint8_t pad,
	uint8_t dad, uint16_t reg)
{
	int32_t ret;
	uint16_t val;
	GSW_MAC_Cli_t Param;

	Param.cmdType = MAC_CLI_SET_MDIO_CL;
	Param.get = 0;
	Param.mac_idx = lif_id;	// <MAC 2 or 3>;
	Param.val[0]  = pad;	// <PHY Address>;
	Param.val[1]  = 0;	// 0: C45, 1 C22>;
	ret = ioctl(fd, GSW_XGMAC_CFG, &Param);
	if (ret < 0)
		return ret;

	Param.cmdType = MAC_CLI_SET_MDIO_RD;
	Param.get = 0;
	Param.mac_idx = lif_id;	// <MAC 2 or 3>;
	Param.val[0] = dad;	// <MMD Dev>;
	Param.val[1] = pad;	// <PHY Address>;
	Param.val[2] = reg;	// <Regsister  Offset>;

	val = ioctl(fd, GSW_XGMAC_CFG, &Param);

	return val;
}

static int gsw_read(const GSW_Device_t *dev, uint32_t regaddr)
{
	return lif_mdio_c45_read(dev->fd, dev->lif_id, dev->phy_addr,
		GSW_MMD_DEV, regaddr);
}

static int gsw_write(const GSW_Device_t *dev, uint32_t regaddr, uint16_t data)
{
	return lif_mdio_c45_write(dev->fd, dev->lif_id, dev->phy_addr,
		GSW_MMD_DEV, regaddr, data);
}

static int __wait_ctrl_busy(const GSW_Device_t *dev)
{
	int ret, i;

	for (i = 0; i < MAX_BUSY_LOOP; i++) {
		ret = gsw_read(dev, GSW_MMD_REG_CTRL);
		if (ret < 0)
			return ret;

		if (!(ret & CTRL_BUSY_MASK))
			return 0;

		_usleep(10);
	}

	return -ETIMEDOUT;
}

static int __gsw_rst_data(const GSW_Device_t *dev)
{
	int ret;

	ret = gsw_write(dev, GSW_MMD_REG_LEN_RET, 0);
	if (ret < 0)
		return ret;

	ret = gsw_write(dev, GSW_MMD_REG_CTRL,
			MMD_API_RST_DATA | CTRL_BUSY_MASK);
	if (ret < 0)
		return ret;

	return __wait_ctrl_busy(dev);
}

static int __gsw_set_data(const GSW_Device_t *dev, uint16_t words)
{
	int ret;
	uint16_t cmd;

	cmd = words / GSW_MMD_REG_DATA_MAX_SIZE - 1;
	if (cmd > 1)
		return -1;
	ret = gsw_write(dev, GSW_MMD_REG_LEN_RET,
			GSW_MMD_REG_DATA_MAX_SIZE * sizeof(uint16_t));
	if (ret < 0)
		return ret;

	cmd += MMD_API_SET_DATA_0;
	ret = gsw_write(dev, GSW_MMD_REG_CTRL,
			cmd | CTRL_BUSY_MASK);
	if (ret < 0)
		return ret;

	return __wait_ctrl_busy(dev);
}

static int __gsw_get_data(const GSW_Device_t *dev, uint16_t words)
{
	int ret;
	uint16_t cmd;

	cmd = words / GSW_MMD_REG_DATA_MAX_SIZE;
	if (cmd == 0 || cmd > 2)
		return -1;
	ret = gsw_write(dev, GSW_MMD_REG_LEN_RET,
			GSW_MMD_REG_DATA_MAX_SIZE * sizeof(uint16_t));
	if (ret < 0)
		return ret;

	cmd += MMD_API_SET_DATA_0;
	ret = gsw_write(dev, GSW_MMD_REG_CTRL,
			cmd | CTRL_BUSY_MASK);
	if (ret < 0)
		return ret;

	return __wait_ctrl_busy(dev);
}

static int __gsw_send_cmd(const GSW_Device_t *dev, uint16_t cmd,
			  uint16_t size, int16_t *presult)
{
	int ret;

	ret = gsw_write(dev, GSW_MMD_REG_LEN_RET, size);
	if (ret < 0)
		return ret;

	ret = gsw_write(dev, GSW_MMD_REG_CTRL,
			cmd | CTRL_BUSY_MASK);
	if (ret < 0)
		return ret;

	ret = __wait_ctrl_busy(dev);
	if (ret < 0)
		return ret;

	ret = gsw_read(dev, GSW_MMD_REG_LEN_RET);
	if (ret < 0)
		return ret;

	*presult = ret;
	return 0;
}

static bool __gsw_cmd_r_valid(uint16_t cmd_r)
{
	return (shadow.ctrl == cmd_r && shadow.ret >= 0) ? true : false;
}

/* This is usually used to implement CFG_SET command.
 * With previous CFG_GET command executed properly, the retrieved data
 * are shadowed in local structure. WSP FW has a set of shadow too,
 * so that only the difference to be sent over SMDIO.
 */
static int __gsw_api_wrap_cmd_r(const GSW_Device_t *dev, uint16_t cmd,
		void *pdata, uint16_t size, uint16_t r_size)
{
	int ret;
	uint16_t max, i;
	uint16_t *data;
	int16_t result = 0;

	max = (size + 1) / 2;
	if ((max >= GSW_MMD_REG_DATA_MAX_SIZE * 3) ||
		(r_size >= GSW_MMD_REG_DATA_MAX_SIZE * 3))
		return -1;
	data = pdata;

	ret = __wait_ctrl_busy(dev);
	if (ret < 0)
		return ret;

	for (i = 0; i < max; i++) {
		uint16_t off = i % GSW_MMD_REG_DATA_MAX_SIZE;

		if (i && off == 0) {
			/* Send command to set data when every
			 * GSW_MMD_REG_DATA_MAX_SIZE of WORDs are written
			 * and reload next batch of data from last CFG_GET.
			 */
			ret = __gsw_set_data(dev, i);
			if (ret < 0)
				return ret;
		}

		if (data[i] == shadow.data[i])
			continue;

		ret = gsw_write(dev, GSW_MMD_REG_DATA_FIRST + off,
			  sys_le16_to_cpu(data[i]));
		if (ret < 0)
			return ret;
	}

	ret = __gsw_send_cmd(dev, cmd, size, &result);
	if (ret < 0)
		return ret;

	if (result < 0)
		return result;

	max = (r_size + 1) / 2;
	for (i = 0; i < max; i++) {
		uint16_t off = i % GSW_MMD_REG_DATA_MAX_SIZE;

		if (i && off == 0) {
			/* Send command to fetch next batch of data
			 * when every GSW_MMD_REG_DATA_MAX_SIZE of WORDs
			 * are read.
			 */
			ret = __gsw_get_data(dev, i);
			if (ret < 0)
				return ret;
		}

		ret = gsw_read(dev, GSW_MMD_REG_DATA_FIRST + off);
		if (ret < 0)
			return ret;

		if ((i * 2 + 1) == r_size) {
			/* Special handling for last BYTE
			 * if it's not WORD aligned.
			 */
			*(uint8_t *)&data[i] = ret & 0xFF;
		} else {
			data[i] = sys_cpu_to_le16((uint16_t)ret);
		}
	}

	shadow.data[max] = 0;
	memcpy(shadow.data, data, r_size);

	return result;
}

int gsw_api_cmd(const void *ctx, uint16_t cmd, void *pdata,
		 uint16_t size, uint16_t cmd_r, uint16_t r_size)
{
	int ret;
	uint16_t max, i, cnt;
	uint16_t *data;
	int16_t result = 0;
	const GSW_Device_t *dev = ctx;

	if (!dev || (!pdata && size))
		return -EINVAL;

	if (__gsw_cmd_r_valid(cmd_r)) {
		/* Special handling for GET and SET command pair. */
		ret = __gsw_api_wrap_cmd_r(dev, cmd, pdata, size, r_size);
		goto EXIT;
	}

	max = (size + 1) / 2;
	data = pdata;

	/* Check whether it's worth to issue RST_DATA command. */
	for (i = cnt = 0; i < max && cnt < THR_RST_DATA; i++) {
		if (!data[i])
			cnt++;
	}

	ret = __wait_ctrl_busy(dev);
	if (ret < 0)
		goto EXIT;

	if (cnt >= THR_RST_DATA) {
		/* Issue RST_DATA commdand. */
		ret = __gsw_rst_data(dev);
		if (ret < 0)
			goto EXIT;

		for (i = 0, cnt = 0; i < max; i++) {
			uint16_t off = i % GSW_MMD_REG_DATA_MAX_SIZE;

			if (i && off == 0) {
				uint16_t cnt_old = cnt;

				cnt = 0;

				/* No actual data was written. */
				if (!cnt_old)
					continue;

				/* Send command to set data when every
				 * GSW_MMD_REG_DATA_MAX_SIZE of WORDs are
				 * written and clear the MMD registe space.
				 */
				ret = __gsw_set_data(dev, i);
				if (ret < 0)
					goto EXIT;
			}

			/* Skip '0' data. */
			if (!data[i])
				continue;
			gsw_write(dev, GSW_MMD_REG_DATA_FIRST + off,
				  sys_le16_to_cpu(data[i]));
			cnt++;
		}
	} else {
		for (i = 0; i < max; i++) {
			uint16_t off = i % GSW_MMD_REG_DATA_MAX_SIZE;

			if (i && off == 0) {
				/* Send command to set data when every
				 * GSW_MMD_REG_DATA_MAX_SIZE of WORDs
				 * are written.
				 */
				ret = __gsw_set_data(dev, i);
				if (ret < 0)
					goto EXIT;
			}
			gsw_write(dev, GSW_MMD_REG_DATA_FIRST + off,
				  sys_le16_to_cpu(data[i]));
		}
	}

	ret = __gsw_send_cmd(dev, cmd, size, &result);
	if (ret < 0)
		goto EXIT;

	if (result < 0) {
		ret = result;
		goto EXIT;
	}

	max = (r_size + 1) / 2;
	for (i = 0; i < max; i++) {
		uint16_t off = i % GSW_MMD_REG_DATA_MAX_SIZE;

		if (i && off == 0) {
			/* Send command to fetch next batch of data
			 * when every GSW_MMD_REG_DATA_MAX_SIZE of WORDs
			 * are read.
			 */
			ret = __gsw_get_data(dev, i);
			if (ret < 0)
				goto EXIT;
		}

		ret = gsw_read(dev, GSW_MMD_REG_DATA_FIRST + off);
		if (ret < 0)
			goto EXIT;

		if ((i * 2 + 1) == r_size) {
			/* Special handling for last BYTE
			 * if it's not WORD aligned.
			 */
			*(uint8_t *)&data[i] = ret & 0xFF;
		} else {
			data[i] = sys_cpu_to_le16((uint16_t)ret);
		}
	}

	shadow.data[max] = 0;
	memcpy(shadow.data, data, r_size);

	ret = result;

EXIT:
	shadow.ctrl = cmd;
	shadow.ret = ret;

	return ret;
}

#define SMIDO_SB_PHY_ADDR_REG 0x1F
#define SMIDO_SB_PHY_DATA_REG 0x0

int smdio_write(uint8_t lif_id, uint8_t phy_addr, uint16_t phy_reg,
	uint16_t phy_reg_data)
{
	int ret = 0;

	/* Write address to register 0x1f */
	ret = lif_mdio_c22_write(gsw_dev.fd, lif_id, phy_addr,
		 SMIDO_SB_PHY_ADDR_REG, phy_reg);
	if (ret == 0) {
		/* Write data to offset 0x0 */
		ret = lif_mdio_c22_write(gsw_dev.fd, lif_id, phy_addr,
			SMIDO_SB_PHY_DATA_REG, phy_reg_data);
	}
	return ret;
}

int smdio_cont_write(uint8_t lif_id, uint8_t phy_addr, uint16_t phy_reg,
	uint16_t phy_reg_data[8], uint8_t num)
{
	int ret = 0;

	/* Write address to register 0x1f */
	ret = lif_mdio_c22_write(gsw_dev.fd, lif_id, phy_addr,
				SMIDO_SB_PHY_ADDR_REG, phy_reg);
	if (ret != 0)
		return ret;

	/* Write data to offset 0x0 */
	for (uint8_t i = 0; i < (num & 0xF); i++) {
		ret = lif_mdio_c22_write(gsw_dev.fd, lif_id, phy_addr,
				SMIDO_SB_PHY_DATA_REG, phy_reg_data[i]);
	}

	return ret;
}

int smdio_read(uint8_t lif_id, uint8_t phy_addr, uint16_t phy_reg)
{
	int ret = 0;
	uint16_t readdata = -1;

	/* Write address to register 0x1f */
	ret = lif_mdio_c22_write(gsw_dev.fd, lif_id, phy_addr,
		SMIDO_SB_PHY_ADDR_REG, phy_reg);
	if (ret == 0) {
		/* Read data from offset 0x0 */
		readdata = lif_mdio_c22_read(gsw_dev.fd, lif_id,
			phy_addr, SMIDO_SB_PHY_DATA_REG);
	}

	return readdata;
}

int mdio_read(uint8_t lif_id, uint8_t phy_addr, uint16_t phy_reg)
{
	int ret = 0;

	ret = lif_mdio_c22_read(gsw_dev.fd, lif_id, phy_addr, phy_reg);
	return ret;
}

void *lif_mdio_open(int devIdx, uint8_t lif_id, uint8_t phy_id)
{
	int32_t fd;
	char buffer[32];

	sprintf(buffer, "/dev/switch_api/%d", devIdx);
	fd = open(buffer, O_RDONLY);

	if (fd < 0) {
		printf("lif_mdio_open: error open - fd = %d\n", fd);
		return NULL;
	}

	memset(&gsw_dev, 0, sizeof(gsw_dev));
	gsw_dev.phy_addr = phy_id;
	gsw_dev.lif_id = lif_id;
	gsw_dev.fd = fd;

	return &gsw_dev;
}

void lif_mdio_close(void *ctx)
{
	close(gsw_dev.fd);
	gsw_dev.fd = -1;
}
