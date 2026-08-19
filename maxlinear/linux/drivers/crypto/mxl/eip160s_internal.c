// SPDX-License-Identifier: GPL-2.0
/*
 * Driver for MACsec Engine inside Maxlinear GSWIP or GPHYs
 * Copyright 2020 - 2022 Maxlinear, Inc.
 */
#include <linux/kernel.h>
#include <linux/module.h>

#include "eip160s_internal.h"

/* Populate Context Control Word */
int _macsec_pop_ccw(enum msec_dir dir, struct ctx_ctrl_word *ccw, u32 *_ccw)
{
	if (dir == INGRESS_DIR) {
		if (ccw->ca_type == CA_AES_CTR_128) {
			if (ccw->sn_type == SN_32_BIT)
				*_ccw = ING_32BPN_128KEY_DEF_CCW;
			else if (ccw->sn_type == SN_64_BIT)
				*_ccw = ING_64BPN_128KEY_DEF_CCW;
			else
				return -1;
		} else if (ccw->ca_type == CA_AES_CTR_256) {
			if (ccw->sn_type == SN_32_BIT)
				*_ccw = ING_32BPN_256KEY_DEF_CCW;
			else if (ccw->sn_type == SN_64_BIT)
				*_ccw = ING_64BPN_256KEY_DEF_CCW;
			else
				return -1;
		} else {
			return -1;
		}
	} else {
		if (ccw->ca_type == CA_AES_CTR_128) {
			if (ccw->sn_type == SN_32_BIT)
				*_ccw = EGR_32BPN_128KEY_DEF_CCW;
			else if (ccw->sn_type == SN_64_BIT)
				*_ccw = EGR_64BPN_128KEY_DEF_CCW;
			else
				return -1;
		} else if (ccw->ca_type == CA_AES_CTR_256) {
			if (ccw->sn_type == SN_32_BIT)
				*_ccw = EGR_32BPN_256KEY_DEF_CCW;
			else if (ccw->sn_type == SN_64_BIT)
				*_ccw = EGR_64BPN_256KEY_DEF_CCW;
			else
				return -1;
		} else
			return -1;

		/* Insert the variable field 'AN' */
		if (ccw->an < MACSEC_MAX_AN) {
			*_ccw = *_ccw | (ccw->an << CCW_EIPX_TR_CCW_AN_POS);
		}  else
			return -1;
	}

	return 0;
}

/* Populate AES Key */
int _macsec_pop_key(struct crypto_parms *cp, u32 *le_key)
{
	int i;

	memcpy((u8 *)le_key, cp->key, MACSEC_KEY_LEN_BYTE);
	for (i = 0; i < (MACSEC_KEY_LEN_BYTE / 4); i++)
		le_key[i] = cpu_to_le32(le_key[i]);

	return 0;
}

/* Populate AES Hash Key */
int _macsec_pop_hkey(struct crypto_parms *cp, u32 *le_hkey)
{
	int i;

	memcpy((u8 *)le_hkey, cp->hkey, MACSEC_HKEY_LEN_BYTE);
	for (i = 0; i < (MACSEC_HKEY_LEN_BYTE / 4); i++)
		le_hkey[i] = cpu_to_le32(le_hkey[i]);

	return 0;
}

/* Populate Integrity Vector */
int _macsec_pop_sci(struct crypto_parms *cp, u32 *le_iv)
{
	int i;

	memcpy((u8 *)le_iv, (u8 *)&cp->scid, MACSEC_SCI_LEN_BYTE);
	for (i = 0; i < (MACSEC_SCI_LEN_BYTE / 4); i++)
		le_iv[i] = cpu_to_le32(le_iv[i]);

	return 0;
}

/* Populate Context Salt */
int _macsec_pop_csalt(struct crypto_parms *cp, u32 *le_ctx_salt)
{
	int i;
	u32 le_short_sci;

	memcpy((u8 *)le_ctx_salt, cp->c_salt.salt, MACSEC_SALT_LEN_BYTE);
	memcpy((u8 *)&le_short_sci, cp->c_salt.s_sci, MACSEC_SSCI_LEN_BYTE);
	le_ctx_salt[0] = le_ctx_salt[0] ^ le_short_sci;
	for (i = 0; i < (MACSEC_SALT_LEN_BYTE / 4); i++)
		le_ctx_salt[i] = cpu_to_le32(le_ctx_salt[i]);

	return 0;
}

/* Copy AES Key */
int _macsec_copy_key(struct crypto_parms *cp, u32 *le_key)
{
	int i;

	for (i = 0; i < (MACSEC_KEY_LEN_BYTE / 4); i++)
		le_key[i] = cpu_to_le32(le_key[i]);

	memcpy(cp->key, (u8 *)le_key, MACSEC_KEY_LEN_BYTE);

	return 0;
}

/* Copy AES Hash Key */
int _macsec_copy_hkey(struct crypto_parms *cp, u32 *le_hkey)
{
	int i;

	for (i = 0; i < (MACSEC_HKEY_LEN_BYTE / 4); i++)
		le_hkey[i] = cpu_to_le32(le_hkey[i]);

	memcpy(cp->hkey, (u8 *)le_hkey, MACSEC_HKEY_LEN_BYTE);

	return 0;
}

/* Copy Integrity Vector */
int _macsec_copy_sci(struct crypto_parms *cp, u32 *le_iv)
{
	int i;

	for (i = 0; i < (MACSEC_SCI_LEN_BYTE / 4); i++)
		le_iv[i] = cpu_to_le32(le_iv[i]);

	memcpy((u8 *)&cp->scid, (u8 *)le_iv, MACSEC_SCI_LEN_BYTE);

	return 0;
}

/* Copy Context Salt */
int _macsec_copy_csalt(struct crypto_parms *cp, u32 *le_ctx_salt)
{
	int i;

	for (i = 0; i < (MACSEC_SALT_LEN_BYTE / 4); i++)
		le_ctx_salt[i] = cpu_to_le32(le_ctx_salt[i]);
	memcpy(cp->c_salt.salt, (u8 *)le_ctx_salt, MACSEC_SALT_LEN_BYTE);

	return 0;
}

MODULE_AUTHOR("Govindaiah Mudepalli");
MODULE_DESCRIPTION("EIP160S MACsec APIs");
MODULE_LICENSE("GPL v2");
