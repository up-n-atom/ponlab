/*
 * Config.h : all adjustable macro and value, related to secure boot authentication
 * can be found here
 *
 * Author : William Widjaja <w.widjaja.ee@lantiq.com>
 * Date : 22-Dec-2014
*/

#ifndef __CONFIG_H__
#define __CONFIG_H__

#define SBIF_CFG_ECDSA_BITS                 256 // 224 or 256
#define SBIF_CFG_CONFIDENTIALITY_BITS       256 //128 or 256, AES bits in confidentiality protection
#define SBLIB_CFG_CERTIFICATES_MAX          3   // maximum certificates supported

#define SBIF_CFG_DERIVE_WRAPKEY_FROM_KDK // direct SBCR or derive AES WRAP from It

/**
   Minimum value for ROLLBACK ID attribute.
   (Optional: if specified, SBIF will enforce values for rollback ID.)
 */
#define SBIF_CFG_ATTRIBUTE_MINIMUM_ROLLBACK_ID  1

// Index number of the Static Asset in NVM that is used as the unwrap key for
// BLw images. This must be an 128-bit or 256-bit AES-decrypt capable asset,
// depending on SBIF_CFG_CONFIDENTIALITY_BITS. Alternatively a key derivation
// key can be selected.
// The exact index number depends on the NVM contents.
// This value is only used by CM (EIP123), and only if
// SBLIB_CF_IMAGE_TYPE_W_SBCR_KEY is not defined.
// See also: cf_sblib.h:SBLIB_CF_IMAGE_TYPE_W_SBCR_KEY
// NOTE : !<WW : most like we only need the index of derive key for 256 Confidentiality bits
//            The rests are just for completeness 
#define SBLIB_CFG_CM_IMAGE_TYPE_W_ASSET_KEY_128   15
#define SBLIB_CFG_CM_IMAGE_TYPE_W_ASSET_KEY_256   16
#define SBLIB_CFG_CM_IMAGE_TYPE_W_ASSET_DERIVE_KEY_128   5
#define SBLIB_CFG_CM_IMAGE_TYPE_W_ASSET_DERIVE_KEY_256   6

// Index number of the Public Asset in NVM that is used as the ECDSA Public key for
// Verifying Image or Chip Manufacturer Public Key
#define SBLIB_CFG_CM_CHIP_MANUFACTURER_PUBLIC_KEY   8

/* These are for single block ECB: State is reused as single block buffer.  */
// NOTE : !<WW : This is around 1MB , and this control how much you can cut 
// the image per processing, e.g. 1 MB each time over 16 MB size image. Change
// as you wish but I think the EIP123 IP HW can support 2 MB max Enc/Decrypt
#define SBHYBRID_MAX_SIZE_PE_JOB_BLOCKS     (0x3FFF * 64)

#endif /* __CONFIG_H__ */

