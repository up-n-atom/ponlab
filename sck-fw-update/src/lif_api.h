/*****************************************************************************
 *
 * Copyright (c) 2023 - 2024 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/
#ifndef LIF_API_H
#define LIF_API_H

#include <stdint.h>

#define SYS_MISC_MAGIC				0x1900
#define SYS_MISC_FW_UPDATE			(SYS_MISC_MAGIC + 0x01)
#define SYS_MISC_FW_VERSION			(SYS_MISC_MAGIC + 0x02)

#define GPY_GPY2XX_MAGIC			0x1800
#define INT_GPHY_READ				(GPY_GPY2XX_MAGIC + 0x01)
#define INT_GPHY_WRITE				(GPY_GPY2XX_MAGIC + 0x02)
#define INT_GPHY_MOD				(GPY_GPY2XX_MAGIC + 0x03)

struct sys_fw_image_version {
	uint8_t iv_major;
	uint8_t iv_minor;
	uint16_t iv_revision;
	uint32_t iv_build_num;
};

struct __attribute__((scalar_storage_order("little-endian"))) mdio_relay_data {
	/* data to be read or written */
	uint16_t data;
	/* PHY index (0~7) for internal PHY
	 * PHY address (0~31) for external PHY access via MDIO bus
	 */
	uint8_t phy;
	/* MMD device (0~31) */
	uint8_t mmd;
	/* Register Index
	 * 0~31 if mmd is 0 (CL22)
	 * 0~65535 otherwise (CL45)
	 */
	uint16_t reg;
};

struct __attribute__((scalar_storage_order("little-endian"))) mdio_relay_mod_data {
	/* data to be written with mask */
	uint16_t data;
	/* PHY index (0~7) for internal PHY
	 * PHY address (0~31) for external PHY access via MDIO bus
	 */
	uint8_t phy;
	/* MMD device (0~31) */
	uint8_t mmd;
	/* Register Index
	 * 0~31 if mmd is 0 (CL22)
	 * 0~65535 otherwise (CL45)
	 */
	uint16_t reg;
	/* mask of bit fields to be updated
	 * 1 to write the bit
	 * 0 to ignore
	 */
	uint16_t mask;
};

void* lif_mdio_open(int devIdx, uint8_t lif_id, uint8_t phy_id);
void lif_mdio_close(void *ctx);
int gsw_api_cmd(const void *ctx, uint16_t cmd, void *pdata,
	uint16_t size, uint16_t cmd_r, uint16_t r_size);

int smdio_write(uint8_t lif_id, uint8_t phy_addr,
	uint16_t phy_reg, uint16_t phy_reg_data);
int smdio_cont_write(uint8_t lif_id, uint8_t phy_addr,
	uint16_t phy_reg, uint16_t phy_reg_data[8], uint8_t num);
int smdio_read(uint8_t lif_id, uint8_t phy_addr, uint16_t phy_reg);
int mdio_read(uint8_t lif_id, uint8_t phy_addr, uint16_t phy_reg);

int check_registers(uint8_t lif_id);
int ssb_fw_download(void *ctx, uint8_t lif_id, uint8_t phy_id, uint8_t *pdata);

#endif /* LIF_API_H */