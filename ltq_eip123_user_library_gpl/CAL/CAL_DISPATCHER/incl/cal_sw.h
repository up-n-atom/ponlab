/* cal_sw.h
 *
 * Function provides the CAL API with specialized names for the CAL_SW
 * implementation. Details area in the real CAL API header files.
 */

/*****************************************************************************
* Copyright (c) 2010-2013 INSIDE Secure B.V. All Rights Reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "sfzcryptoapi.h"

SfzCryptoStatus
sfzcrypto_sw_init(void);

void
CALSW_FeatureMatrix_Amend(
        SfzCryptoFeatureMatrix * const p_features);

uint32_t
sfzcrypto_sw_read_version(
        char * version_p);

SfzCryptoStatus
sfzcrypto_sw_hash_data(
        SfzCryptoHashContext * const p_ctxt,
        uint8_t * p_data,
        uint32_t length,
        bool init,
        bool final);

SfzCryptoStatus
sfzcrypto_sw_hmac_data(
        SfzCryptoHmacContext * const p_ctxt,
        SfzCryptoCipherKey * const p_key,
        uint8_t * p_data,
        uint32_t length,
        bool init,
        bool final);

SfzCryptoStatus
sfzcrypto_sw_symm_crypt(
        SfzCryptoCipherContext * const p_ctxt,
        SfzCryptoCipherKey * const p_key,
        uint8_t * p_src,
        uint32_t src_len,
        uint8_t * p_dst,
        uint32_t * const p_dst_len,
        SfzCipherOp direction);

SfzCryptoStatus
sfzcrypto_sw_auth_crypt(
        SfzCryptoAuthCryptContext * const p_ctxt,
        SfzCryptoCipherKey * const p_key,
        uint8_t * p_nonce,
        uint32_t nonce_len,
        uint8_t * p_aad,
        uint32_t aad_len,
        uint32_t mac_len,
        uint32_t data_len,
        uint8_t * p_src,
        uint32_t src_len,
        uint8_t * p_dst,
        uint32_t * const p_dst_len,
        SfzCipherOp direction,
        bool init,
        bool finish);

SfzCryptoStatus
sfzcrypto_sw_cipher_mac_data(
        SfzCryptoCipherMacContext * const p_ctxt,
        SfzCryptoCipherKey * const p_key,
        uint8_t * p_data,
        uint32_t length,
        bool init,
        bool final);

/* note: not part of CAL API, but exposed for use from CAL_CM */
SfzCryptoStatus
sfzcrypto_sw_cipher_mac_data_s2v(
        SfzCryptoCipherMacContext * const ctxt_p,
        SfzCryptoCipherKey * const key_p,
        uint8_t * data_p,
        uint32_t length,
        bool init,
        bool final);

SfzCryptoStatus
sfzcrypto_sw_aes_wrap_unwrap(
        SfzCryptoCipherContext * const ctxt_p,
        SfzCryptoCipherKey * const kek_p,
        const uint8_t * src_p,
        uint32_t src_len,
        uint8_t * dst_p,
        uint32_t * const dst_len_p,
        SfzCipherOp direction,
        const uint8_t * initial_value_p);

SfzCryptoStatus
sfzcrypto_sw_rand_data(
        uint32_t rand_num_size_bytes,
        uint8_t * p_rand_num);

SfzCryptoStatus
sfzcrypto_sw_random_selftest(
        uint32_t control_flags,
        uint32_t * const p_result_flags);

SfzCryptoStatus
sfzcrypto_sw_ecdsa_sign(
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoSign * const p_signature,
        uint8_t * p_hash_msg,
        uint32_t hash_msglen);

SfzCryptoStatus
sfzcrypto_sw_ecdsa_verify(
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoSign * const p_signature,
        uint8_t * p_hash_msg,
        uint32_t hash_msglen);

SfzCryptoStatus
sfzcrypto_sw_dsa_sign(
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoSign * const p_signature,
        uint8_t * p_hash_msg,
        uint32_t hash_msglen);

SfzCryptoStatus
sfzcrypto_sw_dsa_verify(
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoSign * const p_signature,
        uint8_t * p_hash_msg,
        uint32_t hash_msglen);

SfzCryptoStatus
sfzcrypto_sw_rsa_encrypt(
        SfzCryptoAsymKey * const p_enctx,
        SfzCryptoBigInt * const p_plaintext,
        SfzCryptoBigInt * const p_ciphertext);

SfzCryptoStatus
sfzcrypto_sw_rsa_decrypt(
        SfzCryptoAsymKey * const p_dectx,
        SfzCryptoBigInt * const p_ciphertext,
        SfzCryptoBigInt * const p_plaintext);

SfzCryptoStatus
sfzcrypto_sw_rsa_sign(
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoBigInt * const p_signature,
        uint8_t * p_hash_msg,
        uint32_t hash_msglen);

SfzCryptoStatus
sfzcrypto_sw_rsa_verify(
        SfzCryptoAsymKey * const p_sigctx,
        SfzCryptoBigInt * const p_signature,
        uint8_t * p_hash_msg,
        uint32_t hash_msglen);

SfzCryptoStatus
sfzcrypto_sw_ecdh_publicpart_gen(
        SfzCryptoAsymKey * const p_dhctx,
        SfzCryptoECCPoint * const p_mypubpart);

SfzCryptoStatus
sfzcrypto_sw_ecdh_sharedsecret_gen(
        SfzCryptoAsymKey * const p_dhctx,
        SfzCryptoECCPoint * const p_otherpubpart,
        uint8_t * p_sharedsecret,
        uint32_t * const p_sharedsecretlen);

SfzCryptoStatus
sfzcrypto_sw_dh_publicpart_gen(
        SfzCryptoAsymKey * const p_dhctx,
        SfzCryptoBigInt * const p_mypubpart);

SfzCryptoStatus
sfzcrypto_sw_dh_sharedsecret_gen(
        SfzCryptoAsymKey * const p_dhctx,
        SfzCryptoBigInt * const p_otherpubpart,
        uint8_t * p_sharedsecret,
        uint32_t * const p_sharedsecretlen);

SfzCryptoStatus
sfzcrypto_sw_gen_dh_domain_param(
        SfzCryptoDHDomainParam * const p_dh_dom_param,
        uint32_t primeBits);

SfzCryptoStatus
sfzcrypto_sw_gen_dsa_domain_param(
        SfzCryptoDSADomainParam * const p_dsa_dom_param,
        uint32_t primeBits,
        uint32_t subPrimeBits);

SfzCryptoStatus
sfzcrypto_sw_gen_dsa_key_pair(
        SfzCryptoDSADomainParam * const p_dsa_dom_param,
        SfzCryptoBigInt * const p_dsa_pubkey,
        SfzCryptoBigInt * const p_dsa_priv_key);

SfzCryptoStatus
sfzcrypto_sw_gen_ecdsa_key_pair(
        SfzCryptoECPDomainParam * const p_ec_dom_param,
        SfzCryptoECCPoint * const p_ecdsa_pub_key,
        SfzCryptoBigInt * const p_ecdsa_priv_key,
        uint32_t ec_bits_key_len);

SfzCryptoStatus
sfzcrypto_sw_gen_rsa_key_pair(
        SfzCryptoAsymKey * const p_rsa_pub_key,
        SfzCryptoAsymKey * const p_rsa_priv_key,
        uint32_t rsa_mod_bits);

SfzCryptoStatus
sfzcrypto_sw_random_reseed(void);

SfzCryptoStatus
sfzcrypto_sw_nop(
        SfzCryptoOctetsOut * dst_p,
        SfzCryptoOctetsIn * src_p,
        uint32_t len);

SfzCryptoStatus
sfzcrypto_sw_nvm_publicdata_read(
        uint32_t ObjectNr,
        SfzCryptoOctetsOut * Data_p,
        uint32_t * const DataLen_p);

SfzCryptoStatus
sfzcrypto_sw_asset_alloc(
        SfzCryptoPolicyMask DesiredPolicy,
        SfzCryptoSize AssetSize,
        SfzCryptoAssetId * const NewAssetId_p);

SfzCryptoStatus
sfzcrypto_sw_asset_alloc_temporary(
        SfzCryptoSymKeyType KeyType,
        SfzCryptoModeType FbMode,
        SfzCryptoHashAlgo HashAlgo,
        SfzCryptoAssetId KeyAssetId,
        SfzCryptoAssetId * const NewTempAssetId_p);

SfzCryptoStatus
sfzcrypto_sw_asset_free(
        SfzCryptoAssetId AssetId);

SfzCryptoStatus
sfzcrypto_sw_asset_search(
        uint32_t StaticAssetNumber,
        SfzCryptoAssetId * const NewAssetId_p);

SfzCryptoAssetId
sfzcrypto_sw_asset_get_root_key(void);

SfzCryptoStatus
sfzcrypto_sw_asset_import(
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoTrustedAssetId KekAssetId,
        SfzCryptoOctetsIn * AdditionalData_p,
        SfzCryptoSize AdditionalDataSize,
        SfzCryptoOctetsIn * KeyBlob_p,
        SfzCryptoSize KeyBlobSize);

SfzCryptoStatus
sfzcrypto_sw_asset_derive(
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoTrustedAssetId KdkAssetId,
        SfzCryptoOctetsIn * Label_p,
        SfzCryptoSize LabelLen);

SfzCryptoStatus
sfzcrypto_sw_asset_load_key(
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoOctetsIn * Data_p,
        SfzCryptoSize AssetSize);

SfzCryptoStatus
sfzcrypto_sw_asset_gen_key(
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoSize AssetSize);

SfzCryptoStatus
sfzcrypto_sw_asset_load_key_and_wrap(
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoOctetsIn * Data_p,
        SfzCryptoSize AssetSize,
        SfzCryptoTrustedAssetId KekAssetId,
        SfzCryptoOctetsIn * AdditionalData_p,
        SfzCryptoSize AdditionalDataSize,
        SfzCryptoOctetsOut * KeyBlob_p,
        SfzCryptoSize * const KeyBlobSize_p);

SfzCryptoStatus
sfzcrypto_sw_asset_gen_key_and_wrap(
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoSize AssetSize,
        SfzCryptoTrustedAssetId KekAssetId,
        SfzCryptoOctetsIn * AdditionalData_p,
        SfzCryptoSize AdditionalDataSize,
        SfzCryptoOctetsOut * KeyBlob_p,
        SfzCryptoSize * const KeyBlobSize_p);

SfzCryptoStatus
sfzcrypto_sw_cprm_c2_derive(
        SfzCryptoCprmC2KeyDeriveFunction  FunctionSelect,
        SfzCryptoAssetId AssetIn,
        SfzCryptoAssetId AssetIn2,
        SfzCryptoAssetId AssetOut,
        SfzCryptoOctetsIn * InputData_p,
        SfzCryptoSize InputDataSize,
        SfzCryptoOctetsOut * OutputData_p,
        SfzCryptoSize * const OutputDataSize_p);

SfzCryptoStatus
sfzcrypto_sw_cprm_c2_devicekeyobject_rownr_get(
        SfzCryptoAssetId DeviceKeyAssetId,
        uint16_t * const RowNumber_p);

SfzCryptoStatus
sfzcrypto_sw_multi2_configure(
        uint8_t NumberOfRounds,
        SfzCryptoAssetId SystemKeyAssetId,
        const uint8_t * SystemKey_p);

/* end of file cal_sw.h */
