// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2022 MaxLinear, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/crypto.h>
#include <crypto/hash.h>
#include <crypto/hmac.h>
#include <crypto/aes.h>

#include "vpn.h"

/* control0 */
#define CONTEXT_TYPE_ENCRYPT_HASH_OUT	0x6
#define CONTEXT_TYPE_HASH_DECRYPT_IN	0xf
#define CONTEXT_SIZE(n)			((n) << 8)
#define CONTEXT_KEY_EN			BIT(16)
#define CONTEXT_CRYPTO_ALG_DES		(0x0 << 17)
#define CONTEXT_CRYPTO_ALG_3DES		(0x2 << 17)
#define CONTEXT_CRYPTO_ALG_AES128	(0x5 << 17)
#define CONTEXT_CRYPTO_ALG_AES192	(0x6 << 17)
#define CONTEXT_CRYPTO_ALG_AES256	(0x7 << 17)
#define CONTEXT_DIGEST_HMAC		(0x3 << 21)
#define CONTEXT_CRYPTO_ALG_MD5		(0x0 << 23)
#define CONTEXT_CRYPTO_ALG_SHA1		(0x2 << 23)
#define CONTEXT_CRYPTO_ALG_SHA224	(0x4 << 23)
#define CONTEXT_CRYPTO_ALG_SHA256	(0x3 << 23)
#define CONTEXT_CRYPTO_ALG_SHA384	(0x6 << 23)
#define CONTEXT_CRYPTO_ALG_SHA512	(0x5 << 23)
#define CONTEXT_SPI			BIT(27)
#define CONTEXT_SEQ			BIT(28)
#define CONTEXT_MASK0			BIT(30)

/* control1 */
#define CONTEXT_CRYPTO_MODE_ECB		(0 << 0)
#define CONTEXT_CRYPTO_MODE_CBC		(1 << 0)
#define CONTEXT_IV0			BIT(5)
#define CONTEXT_IV1			BIT(6)
#define CONTEXT_IV2			BIT(7)
#define CONTEXT_IV3			BIT(8)
#define CONTEXT_DIGEST_CNT		BIT(9)
#define CONTEXT_COUNTER_MODE		BIT(10)
#define CONTEXT_CRYPTO_STORE		BIT(12)
#define CONTEXT_PAD			BIT(16)
#define CONTEXT_HASH_STORE		BIT(19)
#define CONTEXT_SEQ_NUM_STORE		BIT(22)
#define CONTEXT_SEQ_NUM_PTR(offset)	((offset) << 24)
#define CONTEXT_SEQ_NUM_PTR_AVAILABLE	BIT(30)

/* token configuration */
#define EIP197_MAX_TOKENS 4
#define TOKEN_WORDS_SIZE_OUTBOUND 6
#define TOKEN_WORDS_SIZE_INBOUND 5

#define MAX_ALGO_NAME 32

enum token_offset {
	TKN_OFFSET_HEADER,
	TKN_OFFSET_CIPHER,
	TKN_OFFSET_PAD
};

enum vpn_cipher_alg {
	CIPHER_DES,
	CIPHER_3DES,
	CIPHER_AES,
};

enum ctx_seq_num {
	SEQ_NUM_INBOUND = 0x5,
	SEQ_NUM_OUTBOUND = 0xa
};

enum token_opcode {
	OPCODE_DIRECTION = 0x0,
	OPCODE_INSERT = 0x2,
	OPCODE_RETRIEVE = 0x4,
	OPCODE_VERIFY = 0xd,
	OPCODE_CTX_ACCESS = 0xe
};

enum token_stat {
	STAT_LAST_HASH = BIT(0),
	STAT_LAST_PACKET = BIT(1)
};

enum token_instructions {
	INS_HASH_RESULT = 0x1c,
	INS_INSERT_SPI = 0x12,
	INS_SEQ_NUM = 0xa,
	INS_SEQ_NUM_INBOUND = 0x5,
	INS_IPSEC_PADDING = 0x04,
	INS_TYPE_OUTPUT = BIT(5),
	INS_TYPE_HASH = BIT(6),
	INS_TYPE_CRYPTO = BIT(7),
	INS_LAST = BIT(8)
};

enum reuse_context_options {
	REUSE_CONTEXT_OFF = 0,
	REUSE_CONTEXT_ON = 1,
	REUSE_CONTEXT_AUTO = 2
};

enum iv_options {
	IV_OPT_PACKET = 0,
	IV_OPT_PRNG = 1
};

enum type_of_output_options {
	OUTPUT_HEADER_UPDATE = BIT(0),
	OUTPUT_HEADER_APPEND = BIT(1),
	OUTPUT_PAD_REMOVE = BIT(2)
};

enum type_options {
	TYPE_TOKEN_FETCH_OFF = 0x0,
	TYPE_TOKEN_FETCH_ON = 0x1,
	TYPE_TOKEN_FETCH_AUTO = 0x3
};

struct vpn_sa {
	u32 spi;
};

struct token_desc {
	union {
		struct {
			u32 packet_length:17;
			u32 stat:2;
			u32 instructions:9;
			u32 opcode:4;
		} __packed;

		struct {
			u32 offset:8;
			u32 res:3;
			u32 d:1;
			u32 p:1;
			u32 f:1;
			u32 u:1;
			u32 sel:2;
			u32 stat:2;
			u32 seq_num:5;
			u32 length:4;
			u32 opcode:4;
		} __packed ctx;
	};
} __packed;

/* Processing Engine Control Data  */
struct control_data_desc {
	u32 packet_length:17;

	/* 13-bit options field */
	u32 input_pointer:1;
	u32 context_pointer:1;
	u32 context_type:1;
	u32 reuse_context:2;
	u32 type_of_output:3;
	u32 context_in_token:1;
	u32 iv_option:3;
	u32 upper_header:1;

	u32 type:2;

	u16 application_id;
	u16 rsvd;

	u8 refresh:2;
	u32 context_lo:30;
	u32 context_hi;

	u32 control0;
	u32 control1;

	u32 token[EIP197_MAX_TOKENS];
} __packed;

/* Basic Command Descriptor format */
struct command_desc {
	u32 particle_size:17;
	u8 rsvd0:5;
	u8 last_seg:1;
	u8 first_seg:1;
	u16 additional_cdata_size:8;

	u32 rsvd1;

	u32 data_lo;
	u32 data_hi;

	u32 additional_data_lo;
	u32 additional_data_hi;

	struct control_data_desc control_data;
} __packed;

struct cipher_algo {
	char *name;
	u32 algo;
	u32 mode;
};

struct auth_algo {
	char *name;
	u32 algo;
	char *digest_name;
};

static struct cipher_algo cipher_algo_list[] = {
	{"cbc(aes)", CIPHER_AES, CONTEXT_CRYPTO_MODE_CBC},
	{"cbc(des)", CIPHER_DES, CONTEXT_CRYPTO_MODE_CBC},
	{"cbc(des3_ede)", CIPHER_3DES, CONTEXT_CRYPTO_MODE_CBC},
	{"ecb(des)", CIPHER_DES, CONTEXT_CRYPTO_MODE_ECB},
	{"ecb(des3_ede)", CIPHER_3DES, CONTEXT_CRYPTO_MODE_ECB},
};

static struct auth_algo auth_algo_list[] = {
	{"hmac(md5)", CONTEXT_CRYPTO_ALG_MD5, "md5"},
	{"hmac(sha1)", CONTEXT_CRYPTO_ALG_SHA1, "sha1"},
	{"hmac(sha224)", CONTEXT_CRYPTO_ALG_SHA224, "sha224"},
	{"hmac(sha256)", CONTEXT_CRYPTO_ALG_SHA256, "sha256"},
	{"hmac(sha384)", CONTEXT_CRYPTO_ALG_SHA384, "sha384"},
	{"hmac(sha512)", CONTEXT_CRYPTO_ALG_SHA512, "sha512"},
};

static struct cipher_algo *get_cipher_by_name(char *name)
{
	struct cipher_algo *p;
	int i;

	for (i = 0; i < ARRAY_SIZE(cipher_algo_list); i++) {
		p = &cipher_algo_list[i];
		if (!strncmp(p->name, name, MAX_ALGO_NAME))
			return p;
	}

	return NULL;
}

static struct auth_algo *get_auth_by_name(char *name)
{
	struct auth_algo *p;
	int i;

	for (i = 0; i < ARRAY_SIZE(auth_algo_list); i++) {
		p = &auth_algo_list[i];
		if (!strncmp(p->name, name, MAX_ALGO_NAME))
			return p;
	}

	return NULL;
}

static bool get_hash_alg_be(const char *algo_name)
{
	if (!strcmp(algo_name, "md5"))
		return false;

	/* shaX are big endians */
	return true;
}

/* Derive ipad/opad from key.
 * EIP197 needs inner/outter hash (ipad/opad) for hmac operation
 * (instead of key).
 */
static int vpn_eip197_hash_pad(struct vpn_data *priv, const char *name,
			       const u8 *key, unsigned int keylen,
			       u8 *ipad, u8 *opad)
{
	int block_size;
	int state_size;
	struct crypto_shash *shash = NULL;
	bool is_be = get_hash_alg_be(name);
	struct shash_desc *desc = NULL;
	int ret;
	int i;
	u32 *state = NULL;
	u8 *ipad_block = NULL;
	u8 *opad_block = NULL;

	dev_dbg(priv->dev, "%s algo %s\n", __func__, name);

	shash = crypto_alloc_shash(name, CRYPTO_ALG_TYPE_SHASH, 0);
	if (IS_ERR(shash)) {
		dev_err(priv->dev, "Fail to allocate shash: %s\n", name);
		return PTR_ERR(shash);
	}

	block_size = crypto_shash_blocksize(shash);
	state_size = crypto_shash_descsize(shash);
	desc = devm_kzalloc(priv->dev,
			    sizeof(struct shash_desc) + state_size,
			    GFP_KERNEL);
	if (!desc) {
		ret = -ENOMEM;
		dev_err(priv->dev, "Fail to allocate desc: %s\n", name);
		goto error;
	}

	dev_dbg(priv->dev, "State size 0x%x block size 0x%x\n", state_size,
		block_size);

	state = devm_kzalloc(priv->dev, state_size, GFP_KERNEL);
	if (!state) {
		ret = -ENOMEM;
		dev_err(priv->dev, "Fail to allocate state: %s\n", name);
		goto error;
	}

	ipad_block = devm_kzalloc(priv->dev, block_size, GFP_KERNEL);
	if (!ipad_block) {
		ret = -ENOMEM;
		goto error;
	}

	opad_block = devm_kzalloc(priv->dev, block_size, GFP_KERNEL);
	if (!opad_block) {
		ret = -ENOMEM;
		goto error;
	}

	desc->tfm = shash;
	memset(ipad_block, 0, block_size);

	if (keylen > block_size) {
		ret = crypto_shash_digest(desc, key, keylen, ipad_block);
		if (ret) {
			dev_err(priv->dev, "Fail to digest inner shash\n");
			goto error;
		}

		keylen = crypto_shash_digestsize(shash);
	} else {
		memcpy(ipad_block, key, keylen);
	}

	/* copy to outer hash */
	memcpy(opad_block, ipad_block, block_size);

	for (i = 0; i < block_size; i++) {
		opad_block[i] ^= HMAC_OPAD_VALUE;
		ipad_block[i] ^= HMAC_IPAD_VALUE;
	}

	/* get inner hash */
	ret = crypto_shash_init(desc);
	if (ret) {
		dev_err(priv->dev, "Failed to init shash ret %d\n", ret);
		goto error;
	}

	ret = crypto_shash_update(desc, ipad_block, block_size);
	if (ret) {
		dev_err(priv->dev, "Failed to update shash\n");
		goto error;
	}

	ret = crypto_shash_export(desc, state);
	if (ret) {
		dev_err(priv->dev, "Failed to export shash\n");
		goto error;
	}

	for (i = 0; i < keylen; i += sizeof(u32)) {
		u32 *p = (u32 *)&ipad[i];

		if (is_be)
			*p = cpu_to_be32(state[i / sizeof(u32)]);
		else
			*p = cpu_to_le32(state[i / sizeof(u32)]);
	}

	/* get outer hash */
	ret = crypto_shash_init(desc);
	if (ret) {
		dev_err(priv->dev, "Failed to init outer shash ret %d\n", ret);
		goto error;
	}

	ret = crypto_shash_update(desc, opad_block, block_size);
	if (ret) {
		dev_err(priv->dev, "Failed to update shash\n");
		goto error;
	}

	ret = crypto_shash_export(desc, state);
	if (ret) {
		dev_err(priv->dev, "Failed to export shash\n");
		goto error;
	}

	for (i = 0; i < keylen; i += sizeof(u32)) {
		u32 *p = (u32 *)&opad[i];

		if (is_be)
			*p = cpu_to_be32(state[i / sizeof(u32)]);
		else
			*p = cpu_to_le32(state[i / sizeof(u32)]);
	}

error:
	if (ipad_block)
		devm_kfree(priv->dev, ipad_block);
	if (opad_block)
		devm_kfree(priv->dev, opad_block);
	if (state)
		devm_kfree(priv->dev, state);
	if (desc)
		devm_kfree(priv->dev, desc);
	crypto_free_shash(shash);
	return ret;
}

static int vpn_eip197_context_control(struct vpn_data *priv,
				      struct vpn_sa_params *params,
				      u32 enc_algo, u32 auth_algo, u32 mode,
				      u32 *ctx)
{
	const u32 seq_no_offset = 2 * sizeof(u32) /* cw0/1 */ +
				  params->key_len + 2 * params->authkeylen +
				  sizeof(u32) /* SPI */;
	int ctrl_size = params->key_len / sizeof(u32);
	u32 cw0 = 0, cw1 = 0;

	cw0 |= CONTEXT_SPI | CONTEXT_SEQ;
	ctrl_size += 2;

	if (params->direction == VPN_DIRECTION_IN) {
		cw0 |= CONTEXT_MASK0;
		ctrl_size += 2;
	}

	if (params->direction == VPN_DIRECTION_OUT)
		cw1 = mode | CONTEXT_SEQ_NUM_PTR_AVAILABLE |
		      CONTEXT_SEQ_NUM_PTR(seq_no_offset / sizeof(u32)) |
		      CONTEXT_SEQ_NUM_STORE;
	else
		cw1 = mode | CONTEXT_PAD;

	/* Take in account the ipad+opad digests */
	ctrl_size += params->authkeylen / sizeof(u32) * 2;
	cw0 |= CONTEXT_KEY_EN | CONTEXT_DIGEST_HMAC | auth_algo |
	       CONTEXT_SIZE(ctrl_size);

	if (params->direction == VPN_DIRECTION_OUT)
		cw0 |= CONTEXT_TYPE_ENCRYPT_HASH_OUT;
	else
		cw0 |= CONTEXT_TYPE_HASH_DECRYPT_IN;

	if (enc_algo == CIPHER_DES) {
		cw0 |= CONTEXT_CRYPTO_ALG_DES;
	} else if (enc_algo == CIPHER_3DES) {
		cw0 |= CONTEXT_CRYPTO_ALG_3DES;
	} else if (enc_algo == CIPHER_AES) {
		switch (params->key_len) {
		case AES_KEYSIZE_128:
			cw0 |= CONTEXT_CRYPTO_ALG_AES128;
			break;
		case AES_KEYSIZE_192:
			cw0 |= CONTEXT_CRYPTO_ALG_AES192;
			break;
		case AES_KEYSIZE_256:
			cw0 |= CONTEXT_CRYPTO_ALG_AES256;
			break;
		default:
			dev_err(priv->dev, "aes keysize not supported: %u\n",
				params->key_len);
			return -EINVAL;
		}
	}
	ctx[0] = cw0;
	ctx[1] = cw1;

	dev_dbg(priv->dev, "Control words: 0x%x 0x%x\n", cw0, cw1);

	return 0;
}

static int vpn_eip197_ctx(struct vpn_data *priv,
			  struct vpn_sa_params *params,
			  u32 enc_algo, u32 auth_algo, u32 mode)
{
	struct auth_algo *auth;
	u32 *ctx;
	u32 *p;

	ctx = params->ctx_buffer;
	if (!ctx)
		return -EINVAL;

	memset(ctx, 0, sizeof(struct ctx));

	/* cw0/cw1 */
	vpn_eip197_context_control(priv, params, enc_algo, auth_algo, mode,
				   ctx);

	/* key */
	memcpy(ctx + 2, params->key, params->key_len);

	auth = get_auth_by_name(params->auth_algo);
	if (!auth)
		return -EINVAL;

	/* inner/outer hash */
	p = ctx + 2 + params->key_len / sizeof(u32);
	vpn_eip197_hash_pad(priv, auth->digest_name, params->authkey,
			    params->authkeylen, (u8 *)p,
			    (u8 *)(p + params->authkeylen / sizeof(u32)));

	/* spi/seqmask */
	p += 2 * params->authkeylen / sizeof(u32);
	*p++ = params->spi;
	*p++ = 0; /* seq0 */
	if (params->direction == VPN_DIRECTION_IN) {
		*p++ = 1; /* seqmask0 */
		*p++ = 0; /* seqmask1 */
	}

	return 0;
}

static int vpn_eip197_tkn(struct vpn_data *priv, struct vpn_sa_params *params)
{
	const u32 seq_no_offset = 2 * sizeof(u32) /* cw0/1 */ +
				  params->key_len + 2 * params->authkeylen +
				  sizeof(u32) /* SPI */;
	const u32 seq_mask_size = 2 * sizeof(u32);
	const u32 seq_no_size = sizeof(u32);
	struct token_desc *tkn;

	tkn = params->token_buffer;
	if (!tkn)
		return -EINVAL;

	memset(tkn, 0, sizeof(struct token_desc));

	if (params->direction == VPN_DIRECTION_OUT) {
		tkn[0].opcode = OPCODE_INSERT;
		tkn[0].instructions = INS_TYPE_HASH | INS_TYPE_OUTPUT |
				      INS_INSERT_SPI;
		tkn[0].packet_length = sizeof(struct ip_esp_hdr) +
				       params->iv_size;

		 /* actual length will be set by fw */
		tkn[1].opcode = OPCODE_DIRECTION;
		tkn[1].instructions = INS_TYPE_CRYPTO | INS_TYPE_HASH |
				      INS_TYPE_OUTPUT;
		tkn[1].packet_length = 0;

		/* next header & pad will be set by fw */
		tkn[2].opcode = OPCODE_INSERT;
		tkn[2].instructions = INS_LAST | INS_TYPE_CRYPTO |
				      INS_TYPE_HASH | INS_TYPE_OUTPUT |
				      INS_IPSEC_PADDING;
		tkn[2].stat = STAT_LAST_HASH;
		tkn[2].packet_length = 0;

		tkn[3].opcode = OPCODE_INSERT;
		tkn[3].instructions = INS_TYPE_OUTPUT | INS_HASH_RESULT;
		tkn[3].stat = STAT_LAST_PACKET | STAT_LAST_HASH;
		tkn[3].packet_length = params->icv_size;

		tkn[4].opcode = OPCODE_VERIFY;
		tkn[4].stat = STAT_LAST_PACKET | STAT_LAST_HASH;

		tkn[5].ctx.opcode = OPCODE_CTX_ACCESS;
		tkn[5].ctx.length = seq_no_size / sizeof(u32);
		tkn[5].ctx.seq_num = SEQ_NUM_OUTBOUND;
		tkn[5].ctx.stat = STAT_LAST_PACKET | STAT_LAST_HASH;
		tkn[5].ctx.d = true;
		tkn[5].ctx.offset = seq_no_offset / sizeof(u32);
	} else {
		tkn[0].opcode = OPCODE_RETRIEVE;
		tkn[0].instructions = INS_TYPE_HASH | INS_INSERT_SPI;
		tkn[0].packet_length = sizeof(struct ip_esp_hdr) +
				       params->iv_size;

		/* payload length will be set by fw */
		tkn[1].opcode = OPCODE_DIRECTION;
		tkn[1].instructions = INS_LAST | INS_TYPE_CRYPTO |
				      INS_TYPE_HASH | INS_TYPE_OUTPUT;
		tkn[1].stat = STAT_LAST_HASH;
		tkn[1].packet_length = 0;

		tkn[2].opcode = OPCODE_RETRIEVE;
		tkn[2].instructions = INS_HASH_RESULT;
		tkn[2].stat = STAT_LAST_PACKET | STAT_LAST_HASH;
		tkn[2].packet_length = params->icv_size;

		tkn[3].opcode = OPCODE_VERIFY;
		tkn[3].instructions = INS_LAST | INS_TYPE_CRYPTO |
				      INS_TYPE_OUTPUT;
		tkn[3].stat = STAT_LAST_PACKET | STAT_LAST_HASH;
		tkn[3].packet_length = BIT(16) /* header */ | params->icv_size;

		tkn[4].ctx.opcode = OPCODE_CTX_ACCESS;
		tkn[4].ctx.length = (seq_no_size + seq_mask_size) / sizeof(u32);
		tkn[4].ctx.seq_num = SEQ_NUM_INBOUND;
		tkn[4].ctx.stat = STAT_LAST_PACKET | STAT_LAST_HASH;
		tkn[4].ctx.p = true;
		tkn[4].ctx.d = true;
		tkn[4].ctx.offset = seq_no_offset / sizeof(u32);
	}

	return 0;
}

static int vpn_eip197_cdr(struct vpn_data *priv, struct vpn_sa_params *params)
{
	struct command_desc *cdr;

	cdr = params->cdr_buffer;
	if (!cdr)
		return -EINVAL;

	memset(cdr, 0, sizeof(struct command_desc));

	if (params->direction == VPN_DIRECTION_OUT) {
		cdr->additional_cdata_size = TOKEN_WORDS_SIZE_OUTBOUND;
		cdr->first_seg = true;
		cdr->last_seg = true;

		cdr->control_data.type = TYPE_TOKEN_FETCH_AUTO;
		cdr->control_data.input_pointer = true;
		cdr->control_data.context_pointer = true;
		cdr->control_data.context_type = true;
		cdr->control_data.reuse_context = REUSE_CONTEXT_AUTO;
		cdr->control_data.iv_option = IV_OPT_PRNG;
	} else {
		cdr->additional_cdata_size = TOKEN_WORDS_SIZE_INBOUND;
		cdr->first_seg = true;
		cdr->last_seg = true;

		cdr->control_data.type = TYPE_TOKEN_FETCH_AUTO;
		cdr->control_data.input_pointer = true;
		cdr->control_data.context_pointer = true;
		cdr->control_data.context_type = true;
		cdr->control_data.reuse_context = REUSE_CONTEXT_AUTO;
		cdr->control_data.type_of_output = OUTPUT_PAD_REMOVE |
						   OUTPUT_HEADER_UPDATE;
		cdr->control_data.iv_option = IV_OPT_PACKET;
	};

	return 0;
}

static int get_iv_size(u32 crypto_alg, u32 crypto_mode)
{
	int iv_words = 0;
	int iv_bytes;

	if (crypto_alg == CIPHER_AES)
		iv_words = 4;
	else if (crypto_alg == CIPHER_DES || crypto_alg == CIPHER_3DES)
		iv_words = 2;
	else
		iv_words = 0;

	switch (crypto_mode) {
	case CONTEXT_CRYPTO_MODE_ECB:
	case CONTEXT_CRYPTO_MODE_CBC:
		iv_bytes = iv_words * 4;
		break;
	default:
		return -EINVAL;
	}

	return iv_bytes;
}

static int get_pad_blk_size(u32 crypto_alg, u32 crypto_mode)
{
	int pad_blk_size = 4;

	switch (crypto_alg) {
	case CIPHER_3DES:
	case CIPHER_DES:
		pad_blk_size = 8;
		break;
	case CIPHER_AES:
		if (crypto_mode == CONTEXT_CRYPTO_MODE_CBC)
			pad_blk_size = 16;
		break;
	default:
		return -EINVAL;
	}

	return pad_blk_size;
}

void *vpn_eip197_create_sa(struct vpn_data *priv,
			   struct vpn_sa_params *params)
{
	struct cipher_algo *cipher;
	struct auth_algo *auth;
	struct vpn_sa *sa;

	dev_dbg(priv->dev, "create sa for spi 0x%x enc %s auth %s\n",
		params->spi, params->enc_algo, params->auth_algo);

	cipher = get_cipher_by_name(params->enc_algo);
	if (!cipher) {
		dev_err(priv->dev, "Cipher algo %s is not supported\n",
			params->enc_algo);
		return NULL;
	}

	auth = get_auth_by_name(params->auth_algo);
	if (!auth) {
		dev_err(priv->dev, "Auth algo %s is not supported\n",
			params->auth_algo);
		return NULL;
	}

	params->pad_blk_size = get_pad_blk_size(cipher->algo, cipher->mode);
	if (params->pad_blk_size < 0) {
		dev_err(priv->dev, "Failed to get pad size for algo %s\n",
			params->enc_algo);
		return NULL;
	}

	params->iv_size = get_iv_size(cipher->algo, cipher->mode);
	if (params->iv_size < 0) {
		dev_err(priv->dev, "Failed to get iv size for algo %s\n",
			params->enc_algo);
		return NULL;
	}

	/* offset to token instructions */
	params->crypto_offs = TKN_OFFSET_CIPHER * sizeof(u32);
	params->hash_pad_offs = 0;
	params->ipcsum_offs = 0;
	params->hash_pad_offs = 0;
	params->msg_len_offs = 0;
	if (params->direction == VPN_DIRECTION_OUT)
		params->total_pad_offs = TKN_OFFSET_PAD * sizeof(u32);
	else
		params->total_pad_offs = 0;

	if (vpn_eip197_ctx(priv, params, cipher->algo, auth->algo,
			   cipher->mode)) {
		dev_err(priv->dev, "Failed to construct context\n");
		return NULL;
	}

	if (vpn_eip197_tkn(priv, params)) {
		dev_err(priv->dev, "Failed to construct token\n");
		return NULL;
	}

	if (vpn_eip197_cdr(priv, params)) {
		dev_err(priv->dev, "Failed to construct cdr\n");
		return NULL;
	}

	sa = devm_kzalloc(priv->dev, sizeof(struct vpn_sa), GFP_KERNEL);
	if (!sa)
		return NULL;

	sa->spi = params->spi;

	return sa;
}

void vpn_eip197_delete_sa(struct vpn_data *priv, void *ctx)
{
	struct vpn_sa *sa = ctx;

	dev_dbg(priv->dev, "delete sa for spi 0x%x\n", sa->spi);
	devm_kfree(priv->dev, sa);
}
