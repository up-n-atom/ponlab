/* cal_cm.h
 *
 * Function provides the CAL API with specialized names for the CAL_CM
 * implementation. Details area in the real CAL API header files.
 */

/*****************************************************************************
* Copyright (c) 2010-2015 INSIDE Secure B.V. All Rights Reserved.
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
sfzcrypto_cm_init(void);

void
CALCM_FeatureMatrix_Amend(
        SfzCryptoFeatureMatrix * const features_p);

uint32_t
sfzcrypto_cm_read_version(
        char * version_p);

SfzCryptoStatus
sfzcrypto_cm_hash_data(
        SfzCryptoHashContext * const ctxt_p,
        uint8_t * data_p,
        uint32_t length,
        bool init,
        bool final);

SfzCryptoStatus
sfzcrypto_cm_hmac_data(
        SfzCryptoHmacContext * const ctxt_p,
        SfzCryptoCipherKey * const key_p,
        uint8_t * data_p,
        uint32_t length,
        bool init,
        bool final);

SfzCryptoStatus
sfzcrypto_cm_symm_crypt(
        SfzCryptoCipherContext * const ctxt_p,
        SfzCryptoCipherKey * const key_p,
        uint8_t * src_p,
        uint32_t src_len,
        uint8_t * dst_p,
        uint32_t * const dst_len_p,
        SfzCipherOp direction);

SfzCryptoStatus
sfzcrypto_cm_cipher_mac_data(
        SfzCryptoCipherMacContext * const ctxt_p,
        SfzCryptoCipherKey * const key_p,
        uint8_t * data_p,
        uint32_t length,
        bool init,
        bool final);

SfzCryptoStatus
sfzcrypto_cm_rand_data(
        uint32_t rand_num_size_bytes,
        uint8_t * rand_num_p);

SfzCryptoStatus
sfzcrypto_cm_random_selftest(
        uint32_t control_flags,
        uint32_t * const result_flags_p);

SfzCryptoStatus
sfzcrypto_cm_random_reseed(void);

SfzCryptoStatus
sfzcrypto_cm_nop(
        SfzCryptoOctetsOut * dst_p,
        SfzCryptoOctetsIn * src_p,
        uint32_t len);

SfzCryptoStatus
sfzcrypto_cm_nvm_publicdata_read(
        uint32_t ObjectNr,
        SfzCryptoOctetsOut * Data_p,
        uint32_t * const DataLen_p);

SfzCryptoStatus
sfzcrypto_cm_asset_alloc(
        SfzCryptoPolicyMask DesiredPolicy,
        SfzCryptoSize AssetSize,
        SfzCryptoAssetId * const NewAssetId_p);

SfzCryptoStatus
sfzcrypto_cm_asset_alloc_temporary(
        SfzCryptoSymKeyType KeyType,
        SfzCryptoModeType FbMode,
        SfzCryptoHashAlgo HashAlgo,
        SfzCryptoAssetId KeyAssetId,
        SfzCryptoAssetId * const NewTempAssetId_p);

SfzCryptoStatus
sfzcrypto_cm_asset_free(
        SfzCryptoAssetId AssetId);

SfzCryptoStatus
sfzcrypto_cm_asset_search(
        uint32_t StaticAssetNumber,
        SfzCryptoAssetId * const NewAssetId_p);

SfzCryptoAssetId
sfzcrypto_cm_asset_get_root_key(void);

SfzCryptoStatus
sfzcrypto_cm_asset_import(
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoTrustedAssetId KekAssetId,
        SfzCryptoOctetsIn * AdditionalData_p,
        SfzCryptoSize AdditionalDataSize,
        SfzCryptoOctetsIn * KeyBlob_p,
        SfzCryptoSize KeyBlobSize);

SfzCryptoStatus
sfzcrypto_cm_asset_derive(
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoTrustedAssetId KdkAssetId,
        SfzCryptoOctetsIn * Label_p,
        SfzCryptoSize LabelLen);

SfzCryptoStatus
sfzcrypto_cm_asset_load_key(
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoOctetsIn * Data_p,
        SfzCryptoSize AssetSize);

SfzCryptoStatus
sfzcrypto_cm_asset_gen_key(
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoSize AssetSize);

SfzCryptoStatus
sfzcrypto_cm_asset_load_key_and_wrap(
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoOctetsIn * Data_p,
        SfzCryptoSize AssetSize,
        SfzCryptoTrustedAssetId KekAssetId,
        SfzCryptoOctetsIn * AdditionalData_p,
        SfzCryptoSize AdditionalDataSize,
        SfzCryptoOctetsOut * KeyBlob_p,
        SfzCryptoSize * const KeyBlobSize_p);

SfzCryptoStatus
sfzcrypto_cm_asset_gen_key_and_wrap(
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoSize AssetSize,
        SfzCryptoTrustedAssetId KekAssetId,
        SfzCryptoOctetsIn * AdditionalData_p,
        SfzCryptoSize AdditionalDataSize,
        SfzCryptoOctetsOut * KeyBlob_p,
        SfzCryptoSize * const KeyBlobSize_p);

SfzCryptoStatus
sfzcrypto_cm_asset_aesunwrap(
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoTrustedAssetId KekAssetId,
        SfzCryptoOctetsIn * KeyBlob_p,
        SfzCryptoSize KeyBlobSize);

SfzCryptoStatus
sfzcrypto_cm_cprm_c2_derive(
        SfzCryptoCprmC2KeyDeriveFunction  FunctionSelect,
        SfzCryptoAssetId AssetIn,
        SfzCryptoAssetId AssetIn2,
        SfzCryptoAssetId AssetOut,
        SfzCryptoOctetsIn * InputData_p,
        SfzCryptoSize InputDataSize,
        SfzCryptoOctetsOut * OutputData_p,
        SfzCryptoSize * const OutputDataSize_p);

SfzCryptoStatus
sfzcrypto_cm_cprm_c2_devicekeyobject_rownr_get(
        SfzCryptoAssetId DeviceKeyAssetId,
        uint16_t * const RowNumber_p);

SfzCryptoStatus
sfzcrypto_cm_multi2_configure(
        uint8_t NumberOfRounds,
        SfzCryptoAssetId SystemKeyAssetId,
        const uint8_t * SystemKey_p);

SfzCryptoStatus
sfzcrypto_cm_auth_crypt(
        SfzCryptoAuthCryptContext * const p_actxt,
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
sfzcrypto_cm_authenticated_unlock_start(
        const uint16_t AuthKeyNumber,
        SfzCryptoAssetId * p_AuthStateASId,
        uint8_t * p_Nonce,
        uint32_t * p_NonceLength);

SfzCryptoStatus
sfzcrypto_cm_authenticated_unlock_verify(
        const SfzCryptoAssetId AuthStateASId,
        const uint8_t * p_Nonce,
        const uint32_t NonceLength,
        const uint8_t * p_Signature,
        const uint32_t SignatureLength);

SfzCryptoStatus
sfzcrypto_cm_secure_debug(
        const SfzCryptoAssetId AuthStateASId,
        const bool bSet);

/* end of file cal_cm.h */
