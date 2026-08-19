/* sfzcryptoapi_cprm.h
 *
 * The Cryptographic Abstraction Layer API: CPRM-specific Asset Management.
 */

/*****************************************************************************
* Copyright (c) 2011-2013 INSIDE Secure B.V. All Rights Reserved.
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

#ifndef INCLUDE_GUARD_SFZCRYPTOAPI_CPRM_H
#define INCLUDE_GUARD_SFZCRYPTOAPI_CPRM_H

#include "public_defs.h"
#include "sfzcryptoapi_enum.h"
#include "sfzcryptoapi_init.h"       // SfzCryptoContext
#include "sfzcryptoapi_buffers.h"    // SfzCryptoOctet*Ptr*


/*----------------------------------------------------------------------------
 * SfzCryptoCprmC2KeyDeriveFunction
 *
 * Enumeration of C2 key derivation functions.
 */
typedef enum
{
    SFZCRYPTO_CPRM_C2_KZ_DERIVE = 0,
    SFZCRYPTO_CPRM_C2_KZ_DERIVE2,
    SFZCRYPTO_CPRM_C2_AKE_PHASE1,
    SFZCRYPTO_CPRM_C2_AKE_PHASE2,
    SFZCRYPTO_CPRM_C2_KMU_DERIVE,
    SFZCRYPTO_CPRM_C2_KM_UPDATE,
    SFZCRYPTO_CPRM_C2_KM_VERIFY,
    SFZCRYPTO_CPRM_C2_KM_DERIVE
} SfzCryptoCprmC2KeyDeriveFunction;


/*----------------------------------------------------------------------------
 * sfzcrypto_cprm_c2_derive
 *
 * This function requests one of the CPRM C2 key derivation functions. In
 * general this causes a freshly created C2 key asset to be loaded with a value.
 * The following table gives an overview of the available CPRM C2 key deriviation
 * functions. It is assumed here that the reader is familiar with the applicable
 * CPRM standards, in particular:
 *
 * [CPRM-BASE] CPRM Specification, Introduction and Common Cryptographic Elements
 * [SD-COMMON] CPRM Specification, SD Memory Card Book, Common Part
 *
 * |---------------------------------------------------------------------------|
 * | Derive    | Input    | Input Data        | Output   | Output Data  | Note |
 * | Function  | Asset(s) |                   | Asset    |              |      |
 * |-----------+----------+-------------------+----------+--------------+------|
 * | KmDerive  | Kd_i     | Dke_r             | Km       | -            |  1   |
 * |-----------+----------+-------------------+----------+--------------+------|
 * | KmVerify  | Km       | Dve (or Dce)      | -        | result       |  2   |
 * |-----------+----------+-------------------+----------+--------------+------|
 * | KmUpdate  | Km, Kd_i | Dkde_r            | Km (new) | -            |  3   |
 * |-----------+----------+-------------------+----------+--------------+------|
 * | KmuDerive | Km       | IDmedia           | Kmu      | -            |  4   |
 * |-----------+----------+-------------------+----------+--------------+------|
 * | AKEPhase1 | Kmu      | arg               | Ks*      | C1           |  5   |
 * |-----------+----------+-------------------+----------+--------------+------|
 * | AKEPhase2 | Kmu      | R1 || C2          | Ks       | R2           |  6   |
 * |-----------+----------+-------------------+----------+--------------+------|
 * | KzDerive  | Kmu      | ENC(Kz_usg)       | Kz       | usg          |  7   |
 * |-----------+----------+-------------------+----------+--------------+------|
 * | KzDerive2 | Kmu      | ID || ENC(Kz_usg) | Kz       | usg          |  8   |
 * |---------------------------------------------------------------------------|
 *
 * Note 1: Calculates Km = [C2_D(Kd_i, Dke_r)]lsb_56 XOR f(c, r), as defined
 *         in [CPRM-BASE], 3.1.2.2 (Calculate Media Key Record) and [SD-COMMON],
 *         3.2. IMPORTANT NOTE: the latter spec defines f(c, r) as 0. The
 *         current implementation of KmDerive does NOT do the XOR and hence is
 *         specific for CPRM for SD cards!
 *         Returns a "Verify" error if Km == all-zero.
 * Note 2: Calculates Dv = C2_D(Km, Dve), see [CPRM-BASE], 3.1.2.1 (Verify Media
 *         Key Record). If [Dv]msb_32 == DEADBEEF, result = Dv. Otherwise, a
 *         "Verify" error is returned and result is empty.
 *         This function is also used to calculate Dc = C2_D(Km, Dce), see
 *         [CPRM-BASE], 3.1.2.3 (Conditionally Calculate Media Key Record).
 *         If no "Verify" error is returned, the result can be used to finish
 *         processing a Conditionally Calculate Media Record with KmUpdate.
 * Note 3: Calculates d = C2_D(Km, Dkde_r), and Km = [C2_D(Kd_i, d)]lsb_56
 *         XOR f(c, r), as defined in [CPRM-BASE], 3.1.2.3 (Conditionally
 *         Calculate Media Key Record). Refer to Note 1 for the SD-card-specific
 *         implementation (actually removal) of the XOR function.
 * Note 4: Calculates Kmu = [C2_G(Km, IDmedia)]lsb_56, as defined in [CPRM-BASE]
 *         3.2.2 (Media Unique Key).
 * Note 5: Generates a 4-byte random value RN and calculates:
 *         Challenge1 = Ks* = C1 = C2_E(Kmu, arg || RN), as defined in
 *         [SD-COMMON], 3.4.1 (AKE, 3a-1 and 3a-2). The notation Ks* intends
 *         to convey that after this step the output asset holds an unfinished
 *         Session Key value.
 * Note 6: Verifies that the output asset initially is an unfinished Session
 *         Key Ks*, holding a Challenge1 value C1. Next, calculates V1 =
 *         C2_G(Kmu, C1) and compares V1 with R1. If not equal, returns a
 *         "Verify" error and moves the output asset to a state that makes
 *         it unusable for any operation except asset delete.
 *         If V1 equals R1, calculates: R2 = C2_G(Kmu, C2) and Ks =
 *         [C2_G(~Kmu, C1 XOR C2)]lsb_56.
 *         The caculations and compare just described cover steps 3c, 3d, 3e
 *         and 3f of the AKE protocol for the "accessing device" as defined
 *         in [SD-COMMON], 3.4.1.
 * Note 7: Calculates Kz_usg = DEC(Kmu, ENC(Kz_usg)), where "ENC(Kz_usg)"
 *         represents the data from the Protected Area of the SD card.
 *         In the basic case (see [SD-COMMON], 3.4, 4b (Decrypt Title Key),
 *         this data is only 8 bytes. In that case, DEC is C2_D, Kz is the
 *         Title Key and 'usg' consists of the CCI bits.
 *         In other cases, the data consists of more than 8 bytes. In those
 *         cases, DEC = C2_DCBC, Kz is some Content Key or User Key and 'usg'
 *         consists of Usage Rule data.
 *         Let DEC() -> D1 || D2 || D3... represent the output of the decrypt
 *         operation, where each block Di is 8 bytes.
 *         Then the result of the KzDerive operations is:
 *           Output Asset Kz = [D1]lsb_56  and
 *           Output Data usg = [D1]msb_8 || 7 zero bytes || D2 || D3 ...
 * Note 8: The KzDerive function is very similar to the KzDerive function,
 *         except that "Additional ID Binding", as defined in [SD-COMMON],
 *         3.11.3 is used. The key used for the DEC() function is not Kmu
 *         but Kmu XOR ID.
 *
 *
 * Decription of the parameters of sfzcrypto_cprm_c2_derive:
 *
 * FunctionSelect
 *     One of the SfzCryptoCprmC2KeyDeriveFunction values that specifies
 *     which key derivation function is being requested.
 *
 * AssetIn
 *     Reference to the primary asset needed to derive the value for AssetOut.
 *
 * AssetIn2
 *     Reference to a second asset needed to derive the value for AssetOut.
 *     Only needed when the selected derivation function is KmUpdate. This
 *     argument must refer to the relevant Device Key in that case.
 *
 * AssetOut
 *     Reference to the asset whose value is being derived. Typically a
 *     freshly created asset, except in the case of:
 *     - KmVerify: the value of AssetOut is ignored since this function does
 *                 not derive a key value.
 *     - KmUpdate: besides a freshly allocated asset, this function also
 *                 accepts the Km (primary) input asset as AssetOut (for
 *                 an "in-place" update of that asset).
 *
 * InputData_p
 * InputDataSize
 *     Together define the input data for the selected derivation function.
 *     Most functions expect 8 bytes of input, except AKEPhase1 (4 bytes),
 *     KzDerive (8N bytes, N > 0) or KzDerive2 (8N bytes, N > 1).
 *
 * OutputData_p
 * OutDataSize_p
 *     Together define the buffer reserved for receiving (additional) output
 *     of the selected derivation function. The following derive functions
 *     never generate additional output and hence accept NULL/0 as output
 *     data arguments: KmDerive, KmUpdate, KmuDerive.
 *     The other functions only return output data (i.e. *OutDataSize_p > 0)
 *     if the return value equals SFZCRYPTO_SUCCESS.
 *
 * Return Value:
 *     One of the SfzCryptoStatus values.
 *     Some typical error values are:
 *     SFZCRYPTO_OPERATION_FAILED when an invalid asset type is used.
 *     SFZCRYPTO_VERIFY_FAILED when a "Verify" error (see Notes) occurred.
 *
 */
SfzCryptoStatus
sfzcrypto_cprm_c2_derive(
        SfzCryptoCprmC2KeyDeriveFunction  FunctionSelect,
        SfzCryptoAssetId                  AssetIn,
        SfzCryptoAssetId                  AssetIn2,
        SfzCryptoAssetId                  AssetOut,
        SfzCryptoOctetsIn *               InputData_p,
        SfzCryptoSize                     InputDataSize,
        SfzCryptoOctetsOut *              OutputData_p,
        SfzCryptoSize *                   const OutputDataSize_p);


/*----------------------------------------------------------------------------
 * sfzcrypto_cprm_c2_devicekeyobject_rownr_get
 *
 * This function returns the Row number associated with a given Device Key
 * asset.
 *
 * DeviceKeyAssetId
 *     Reference to the Device Key asset whose Row number is being queried.
 *
 * RowNumber_p
 *     Location for storing the result.
 *
 * Return Value:
 *     One of the SfzCryptoStatus values.
 */
SfzCryptoStatus
sfzcrypto_cprm_c2_devicekeyobject_rownr_get(
        SfzCryptoAssetId DeviceKeyAssetId,
        uint16_t * const RowNumber_p);


#endif /* Include Guard */

/* end of file sfzcryptoapi_cprm.h */
