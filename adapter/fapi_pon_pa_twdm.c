/*****************************************************************************
 *
 * Copyright (c) 2023 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include "fapi_pon_pa_common.h"
#include "fapi_pon_pa_twdm.h"

struct pon_twdm_ops {
	enum pon_ddmi_page eeprom;
	int (*wl_get)(const uint8_t ch_id);
	enum fapi_pon_errorcode (*write_us)(struct fapi_pon_wrapper_ctx *ctx,
					    struct pon_ctx *pon_ctx,
					    const uint8_t ch_id);
	enum fapi_pon_errorcode (*write_ds)(struct fapi_pon_wrapper_ctx *ctx,
					    struct pon_ctx *pon_ctx,
					    const uint8_t ch_id);
	enum fapi_pon_errorcode (*tuning)(struct fapi_pon_wrapper_ctx *ctx,
					  struct pon_ctx *pon_ctx,
					  const uint8_t ch_id);
};

static enum fapi_pon_errorcode
twdm_write_method_dummy(struct fapi_pon_wrapper_ctx *ctx,
			struct pon_ctx *pon_ctx,
			const uint8_t ch_id)
{
	(void)ctx;
	(void)pon_ctx;
	(void)ch_id;

	return PON_STATUS_OK;
}

/** Address to 4-byte password to allow wavelength configuration.
 *  Source document: ligentphotonics-ltw2601cbc
 */
#define PON_LIGENT_PASSWD_WR 0x7B
static enum fapi_pon_errorcode
ligent_passwd_write(struct fapi_pon_wrapper_ctx *ctx,
		    struct pon_ctx *pon_ctx)
{
	unsigned char passwd[4] = {0x12, 0x34, 0x56, 0x78};
	enum fapi_pon_errorcode ret;

	ret = fapi_pon_eeprom_data_set(pon_ctx, ctx->twdm_ops->eeprom,
				       passwd, PON_LIGENT_PASSWD_WR,
				       sizeof(passwd));
	if (ret != PON_STATUS_OK) {
		dbg_wrn("Could not write to eeprom file!\n");
		return ret;
	}

	return ret;
}

/** Address to byte containing upstream wavelength configuration.
 *  Source document: ligentphotonics-ltw2601cbc
 */
#define PON_LIGENT_US_WL_CONF_WR 0x70
/** Address to byte containing downstream wavelength configuration.
 *  Source document: ligentphotonics-ltw2601cbc
 */
#define PON_LIGENT_DS_WL_CONF_WR 0x71

/*
 * Password protected access.
 * Prepared for model: ligentphotonics-ltw2601cbc
 */
static enum fapi_pon_errorcode
twdm_write_method1_us(struct fapi_pon_wrapper_ctx *ctx,
		      struct pon_ctx *pon_ctx,
		      const uint8_t ch_id)
{
	unsigned char data;
	enum fapi_pon_errorcode ret;

	ret = ligent_passwd_write(ctx, pon_ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	if (!ctx->twdm_ops->wl_get)
		return PON_STATUS_ERR;
	data = ctx->twdm_ops->wl_get(ch_id);

	return fapi_pon_eeprom_data_set(pon_ctx, ctx->twdm_ops->eeprom,
					&data, PON_LIGENT_US_WL_CONF_WR,
					sizeof(data));
}

static enum fapi_pon_errorcode
twdm_write_method1_ds(struct fapi_pon_wrapper_ctx *ctx,
		      struct pon_ctx *pon_ctx,
		      const uint8_t ch_id)
{
	unsigned char data;
	enum fapi_pon_errorcode ret;

	ret = ligent_passwd_write(ctx, pon_ctx);
	if (ret != PON_STATUS_OK)
		return ret;

	if (!ctx->twdm_ops->wl_get)
		return PON_STATUS_ERR;
	data = ctx->twdm_ops->wl_get(ch_id);

	return fapi_pon_eeprom_data_set(pon_ctx, ctx->twdm_ops->eeprom,
					&data, PON_LIGENT_DS_WL_CONF_WR,
					sizeof(data));
}

/** Address to byte containing wavelength configuration.
 *  4 MSB: Upstream wavelength type
 *  4 LSB: Downstream wavelength type
 *  Source document: lightroninc-0013c5-lwekrrxx8a
 */
#define PON_LIGHTRON_WL_CONF_WR 0xFC

/*
 * Read-write access.
 * Prepared for model: lightroninc-0013c5-lwekrrxx8a
 */
static enum fapi_pon_errorcode
twdm_write_method2_us(struct fapi_pon_wrapper_ctx *ctx,
		      struct pon_ctx *pon_ctx,
		      const uint8_t ch_id)
{
	int wl_type;
	unsigned char data;
	enum fapi_pon_errorcode ret;

	ret = fapi_pon_eeprom_data_get(pon_ctx, ctx->twdm_ops->eeprom,
				       &data, PON_LIGHTRON_WL_CONF_WR,
				       sizeof(data));
	if (ret != PON_STATUS_OK) {
		dbg_wrn("Could not read from dmi eeprom file!\n");
		return ret;
	}

	if (!ctx->twdm_ops->wl_get)
		return PON_STATUS_ERR;
	wl_type = ctx->twdm_ops->wl_get(ch_id);

	data &= 0xF0;
	data |= wl_type;

	return fapi_pon_eeprom_data_set(pon_ctx, ctx->twdm_ops->eeprom,
					&data, PON_LIGHTRON_WL_CONF_WR,
					sizeof(data));
}

static enum fapi_pon_errorcode
twdm_write_method2_ds(struct fapi_pon_wrapper_ctx *ctx,
		      struct pon_ctx *pon_ctx,
		      const uint8_t ch_id)
{
	int wl_type;
	unsigned char data;
	enum fapi_pon_errorcode ret;

	ret = fapi_pon_eeprom_data_get(pon_ctx, ctx->twdm_ops->eeprom,
				       &data, PON_LIGHTRON_WL_CONF_WR,
				       sizeof(data));
	if (ret != PON_STATUS_OK) {
		dbg_wrn("Could not read from dmi eeprom file!\n");
		return ret;
	}

	if (!ctx->twdm_ops->wl_get)
		return PON_STATUS_ERR;
	wl_type = ctx->twdm_ops->wl_get(ch_id);

	data = data & 0x0F;
	data |= (wl_type << 4);

	return fapi_pon_eeprom_data_set(pon_ctx, ctx->twdm_ops->eeprom,
					&data, PON_LIGHTRON_WL_CONF_WR,
					sizeof(data));
}

/** Address to byte containing wavelength configuration.
 *  4 LSB: US and DS channel id set together
 *  for write_method3
 */
#define PON_PICADV_WL_CONF_WR 0x90

/* For transceivers with locked upstream/downstream wavelengths:
 * One wavelength channel ID is used for both directions.
 */
static enum fapi_pon_errorcode
twdm_write_method3_ds(struct fapi_pon_wrapper_ctx *ctx,
		      struct pon_ctx *pon_ctx,
		      const uint8_t ch_id)
{
	unsigned char data = ch_id;
	enum fapi_pon_errorcode ret;

	ret = fapi_pon_eeprom_data_set(pon_ctx, ctx->twdm_ops->eeprom,
				       &data, PON_PICADV_WL_CONF_WR,
				       sizeof(data));
	if (ret == PON_STATUS_OK)
		ctx->used_dwlch_id = (int)ch_id;
	return ret;
}

static int wl_get(const uint8_t ch_id)
{
	/* TODO: implement the channel id to wavelength type translation */
	(void)ch_id;

	return PON_TWDM_WL_TYPE0;
}

static const struct pon_twdm_ops twdm_ops[] = {
[0] = {
	/* The value of 0 allows to perform
	 * a "dummy wavelength switching"
	 * while no real optical transceiver is available.
	 * If this is selected, the wavelength switching
	 * functions shall work as intended,
	 * just the switching is not done.
	 */
	.eeprom = PON_DDMI_A0,
	.write_us = twdm_write_method_dummy,
	.write_ds = twdm_write_method_dummy,
},
[1] = {
	.eeprom = PON_DDMI_A0,
	.write_us = twdm_write_method1_us,
	.write_ds = twdm_write_method1_ds,
	.wl_get = wl_get,
},
[2] = {
	.eeprom = PON_DDMI_A2,
	.write_us = twdm_write_method2_us,
	.write_ds = twdm_write_method2_ds,
	.wl_get = wl_get,
},
[3] = {
	.eeprom = PON_DDMI_A2,
	.write_us = twdm_write_method_dummy,
	.write_ds = twdm_write_method3_ds,
},
};

const struct pon_twdm_ops *pon_twdm_select_ops(uint8_t twdm_config_method)
{
	if (twdm_config_method >= ARRAY_SIZE(twdm_ops))
		return &twdm_ops[PON_TWDM_CONF_METHOD0];

	return &twdm_ops[twdm_config_method];
}

enum fapi_pon_errorcode pon_twdm_write_us(struct fapi_pon_wrapper_ctx *ctx,
					  struct pon_ctx *pon_ctx,
					  const uint8_t ch_id)
{
	if (!ctx->twdm_ops->write_us)
		return PON_STATUS_ERR;

	return ctx->twdm_ops->write_us(ctx, pon_ctx, ch_id);
}

enum fapi_pon_errorcode pon_twdm_write_ds(struct fapi_pon_wrapper_ctx *ctx,
					  struct pon_ctx *pon_ctx,
					  const uint8_t ch_id)
{
	if (!ctx->twdm_ops->write_ds)
		return PON_STATUS_ERR;

	return ctx->twdm_ops->write_ds(ctx, pon_ctx, ch_id);
}

enum fapi_pon_errorcode pon_twdm_tuning(struct fapi_pon_wrapper_ctx *ctx,
					struct pon_ctx *pon_ctx,
					const uint8_t ch_id)
{
	if (!ctx->twdm_ops->tuning)
		return PON_STATUS_ERR;

	return ctx->twdm_ops->tuning(ctx, pon_ctx, ch_id);
}
