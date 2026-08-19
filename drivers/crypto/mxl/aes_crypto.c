// SPDX-License-Identifier: GPL-2.0
/*
 * Driver for MACsec Engine inside Intel CHD GSWIP or GPHY
 * Copyright 2020 - 2024 Maxlinear, Inc.
 * Copyright 2019 - 2020 Intel Corporation
 */

#include <linux/scatterlist.h>
#include <crypto/aes.h>
#include <crypto/hash.h>
#include <crypto/aead.h>
#include <crypto/skcipher.h>

int derive_aes_hkey(const u8 *key, u8 *output, u32 keylen)
{
	struct crypto_skcipher *tfm;
	struct skcipher_request *req = NULL;
	struct scatterlist src, dst;
	DECLARE_CRYPTO_WAIT(wait);
	u8 *_input = NULL, *_output = NULL;
	int ret;

	tfm = crypto_alloc_skcipher("ecb(aes)", 0, 0);
	if (IS_ERR(tfm))
		return PTR_ERR(tfm);

	req = skcipher_request_alloc(tfm, GFP_KERNEL);
	if (!req) {
		ret = -ENOMEM;
		goto OUT;
	}

	skcipher_request_set_callback(req, CRYPTO_TFM_REQ_MAY_BACKLOG |
					CRYPTO_TFM_REQ_MAY_SLEEP, 
					crypto_req_done, &wait);

	ret = crypto_skcipher_setkey(tfm, key, keylen);
	if (ret < 0)
		goto OUT;

	_input = kmalloc(keylen, GFP_KERNEL);
	_output = kmalloc(keylen, GFP_KERNEL);
	if (!_input || !_output) {
		ret = -ENOMEM;
		goto OUT;
	}

	memset(_input, 0, keylen);
	memset(_output, 0, keylen);

	sg_init_one(&src, _input, keylen);
	sg_init_one(&dst, _output, keylen);
	skcipher_request_set_crypt(req, &src, &dst, keylen, NULL);

	ret = crypto_wait_req(crypto_skcipher_encrypt(req), &wait);
	if (ret)
		goto OUT;

        memcpy(output, _output, keylen);

OUT:
	skcipher_request_free(req);
	crypto_free_skcipher(tfm);
	kfree(_input);
	kfree(_output);
	return ret;
}
