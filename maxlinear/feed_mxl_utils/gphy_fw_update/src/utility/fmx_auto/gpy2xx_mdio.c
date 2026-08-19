#include <sys/ioctl.h>

#include "gpy2xx_mdio.h"
#include "lantiq_gsw.h"
#include "mac_ops.h"

static int _gpy211_c22mdio_read(void *busdata, u16 phyaddr, u32 regnum)
{
	GSW_MAC_Cli_t param = {0};
	struct gpy2xx_mdio *priv = (struct gpy2xx_mdio *)busdata;

	param.cmdType = MAC_CLI_SET_MDIO_RD;
	param.argc = 3;
	param.get = 1;
	param.set_all = 0;
	param.mac_idx = priv->xgmac_id;
	param.val[0] = 0;
	param.val[1] = phyaddr;
	param.val[2] = regnum;

	//printf("In read:- Mac id: %u, Dev id: %u, Phy id: %u, Reg num: %u\n", param.mac_idx, param.val[0], param.val[1], param.val[2]);

	//g_mdio_rd_cnt++;
	return ioctl(priv->fd, GSW_XGMAC_CFG, &param);
}

static int _gpy211_c22mdio_write(void *busdata, u16 phyaddr, u32 regnum, u16 data)
{
	GSW_MAC_Cli_t param = {0};
	struct gpy2xx_mdio *priv = (struct gpy2xx_mdio *)busdata;

	param.cmdType = MAC_CLI_SET_MDIO_WR;
	param.argc = 4;
	param.get = 0;
	param.set_all = 0;
	param.mac_idx = priv->xgmac_id;
	param.val[0] = 0;
	param.val[1] = phyaddr;
	param.val[2] = regnum;
	param.val[3] = data;

	//printf("In write:- Mac id: %u, Dev id: %u, Phy id: %u, Reg num: %u, Data: %u\n", param.mac_idx, param.val[0], param.val[1], param.val[2], param.val[3]);

	//g_mdio_wr_cnt++;
	return ioctl(priv->fd, GSW_XGMAC_CFG, &param);
}

static int _gpy211_c45mdio_read(void *busdata, u16 phyaddr, u32 regnum)
{
	u16 _reg_num, _dev_id;
	GSW_MAC_Cli_t param = {0};
	struct gpy2xx_mdio *priv = (struct gpy2xx_mdio *)busdata;

	_dev_id = (regnum >> 16) & 0x1F;
	_reg_num = regnum & 0xFFFF;

	param.cmdType = MAC_CLI_SET_MDIO_RD;
	param.argc = 3;
	param.get = 1;
	param.set_all = 0;
	param.mac_idx = priv->xgmac_id;
	param.val[0] = _dev_id;
	param.val[1] = phyaddr;
	param.val[2] = _reg_num;

	//printf("In read:- Mac id: %u, Dev id: %u, Phy id: %u, Reg num: %u\n", param.mac_idx, param.val[0], param.val[1], param.val[2]);

	//g_mdio_rd_cnt++;
	return ioctl(priv->fd, GSW_XGMAC_CFG, &param);
}

static int _gpy211_c45mdio_write(void *busdata, u16 phyaddr, u32 regnum, u16 data)
{
	u16 _reg_num, _dev_id;
	GSW_MAC_Cli_t param = {0};
	struct gpy2xx_mdio *priv = (struct gpy2xx_mdio *)busdata;

	_dev_id = (regnum >> 16) & 0x1F;
	_reg_num = regnum & 0xFFFF;

	param.cmdType = MAC_CLI_SET_MDIO_WR;
	param.argc = 4;
	param.get = 0;
	param.set_all = 0;
	param.mac_idx = priv->xgmac_id;
	param.val[0] = _dev_id;
	param.val[1] = phyaddr;
	param.val[2] = _reg_num;
	param.val[3] = data;

	//printf("In write:- Mac id: %u, Dev id: %u, Phy id: %u, Reg num: %u, Data: %u\n", param.mac_idx, param.val[0], param.val[1], param.val[2], param.val[3]);

	//g_mdio_wr_cnt++;
	return ioctl(priv->fd, GSW_XGMAC_CFG, &param);
}

static int _gpy211_c22extnd_read(void *busdata, u16 phyaddr, u32 regnum)
{
	int ret;
	u16 _reg_num, _dev_id;

	/* CLAUSE 45 type read */
	_dev_id = (regnum >> 16) & 0x1F;
	_reg_num = regnum & 0xFFFF;

	/* Write MMDCTRL address register */
	ret = _gpy211_c22mdio_write(busdata, phyaddr, STD_MMDCTRL / 2, _dev_id);

	if (ret < 0)
		return ret;

	/* Write MMDDATA address register */
	ret = _gpy211_c22mdio_write(busdata, phyaddr, STD_MMDDATA / 2, _reg_num);

	if (ret < 0)
		return ret;

	/* Write MMDCTRL address register */
	ret = _gpy211_c22mdio_write(busdata, phyaddr, STD_MMDCTRL / 2, (0x4000 + _dev_id));

	if (ret < 0)
		return ret;

	/* Read target register */
	return _gpy211_c22mdio_read(busdata, phyaddr, STD_MMDDATA / 2);
}

static int _gpy211_c22extnd_write(void *busdata, u16 phyaddr, u32 regnum, u16 data)
{
	int ret;
	u16 _reg_num, _dev_id;

	_dev_id = (regnum >> 16) & 0x1F;
	_reg_num = regnum & 0xFFFF;

	/* write MMDCTRL address register */
	ret = _gpy211_c22mdio_write(busdata, phyaddr, STD_MMDCTRL / 2, _dev_id);

	if (ret < 0)
		return ret;

	/* write MMDDATA address register */
	ret = _gpy211_c22mdio_write(busdata, phyaddr, STD_MMDDATA / 2, _reg_num);

	if (ret < 0)
		return ret;

	/* write MMDCTRL address register */
	ret = _gpy211_c22mdio_write(busdata, phyaddr, STD_MMDCTRL / 2, (0x4000 + _dev_id));

	if (ret < 0)
		return ret;

	/* write target register */
	ret = _gpy211_c22mdio_write(busdata, phyaddr, STD_MMDDATA / 2, data);

	if (ret < 0)
		return ret;

	return 0;
}

int gpy211_mdio_read(void *busdata, u16 phyaddr, u32 regnum)
{
	u8 _mdio_clause;
	GSW_MAC_Cli_t param = {0};
	struct gpy2xx_mdio *priv = (struct gpy2xx_mdio *)busdata;

	param.cmdType = MAC_CLI_GET_MDIO_CL;
	param.get = 1;
	param.mac_idx = priv->xgmac_id;
	param.argc = 2;
	param.val[0] = 0;
	param.val[1] = phyaddr;

	_mdio_clause = ioctl(priv->fd, GSW_XGMAC_CFG, &param);
	//printf("Clause: %s\n", _mdio_clause ? "CL22" : "CL45");

	if (_mdio_clause == 0) {
		//printf("Calling C45 read\n");
		/* CLAUSE 45 type read */
		return _gpy211_c45mdio_read(busdata, phyaddr, regnum);
	} else if (regnum & MII_ADDR_C45) {
		//printf("Calling C22 extended read\n");
		/* CLAUSE C22 extended type read */
		return _gpy211_c22extnd_read(busdata, phyaddr, regnum);
	} else {
		//printf("Calling C22 read\n");
		/* CLAUSE C22 extended type read */
		return _gpy211_c22mdio_read(busdata, phyaddr, regnum);
	}
}

int gpy211_mdio_write(void *busdata, u16 phyaddr, u32 regnum, u16 data)
{
	u8 _mdio_clause;
	GSW_MAC_Cli_t param = {0};
	struct gpy2xx_mdio *priv = (struct gpy2xx_mdio *)busdata;

	param.cmdType = MAC_CLI_GET_MDIO_CL;
	param.get = 1;
	param.mac_idx = priv->xgmac_id;
	param.argc = 2;
	param.val[0] = 0;
	param.val[1] = phyaddr;

	_mdio_clause = ioctl(priv->fd, GSW_XGMAC_CFG, &param);
	//printf("Clause: %s\n", _mdio_clause ? "CL22" : "CL45");

	if (_mdio_clause == 0) {
		//printf("Calling C45 write\n");
		/* CLAUSE 45 type read */
		return _gpy211_c45mdio_write(busdata, phyaddr, regnum, data);
	} else if (regnum & MII_ADDR_C45) {
		//printf("Calling C22 extended write\n");
		/* CLAUSE 22 extended type read */
		return _gpy211_c22extnd_write(busdata, phyaddr, regnum, data);
	} else {
		//printf("Calling C22 write\n");
		/* CLAUSE 22 extended type read */
		return _gpy211_c22mdio_write(busdata, phyaddr, regnum, data);
	}
}
