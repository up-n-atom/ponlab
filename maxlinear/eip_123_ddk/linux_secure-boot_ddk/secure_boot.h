/*
 * secure_boot.h : all type and structure definition, related to secure boot authentication
 * can be found here
 *
 * Author : William Widjaja <w.widjaja.ee@lantiq.com>
 * Date : 22-Dec-2014
*/

#ifndef __SECURE_BOOT_H__
#define __SECURE_BOOT_H__

/* -----------------
 * Include
* ------------------ */

#include "config.h"
#include <stdint.h>

/* -----------------
 * Macro
* ------------------ */

/** Tag for image type BLTp. */
#define SBIF_IMAGE_BLTp            0x424c70

/** Tag for image type BLTw. */
#define SBIF_IMAGE_BLTw            0x424c77

/** Tag for image type BLTe. */
#define SBIF_IMAGE_BLTe            0x424c65

/** Tag for image type BLTx. */
#define SBIF_IMAGE_BLTx            0x424c78

/** Current image version number for BL images.  */
#define SBIF_VERSION               2U

/** Values for PubKeyType field: describes the location of the public key. */
#define SBIF_PUBKEY_TYPE_ROM       0x1
#define SBIF_PUBKEY_TYPE_OTP       0x2
#define SBIF_PUBKEY_TYPE_IMAGE     0x3

/** Macro to get version from the type field. */
#define SBIF_TYPE_VERSION(type)    ((type) & 0xff)

/** Macro to get type from the type field. */
#define SBIF_TYPE_TYPE(type)       ((type) >> 8)

#ifdef SBIF_CFG_ECDSA_BITS
#define SBIF_ECDSA_BITS_DO_U(a) a##U
#define SBIF_ECDSA_BITS_U(a)    SBIF_ECDSA_BITS_DO_U(a)
#define SBIF_ECDSA_BITS         SBIF_ECDSA_BITS_U(SBIF_CFG_ECDSA_BITS)
#endif /* SBIF_CFG_ECDSA_BITS */

#if SBIF_CFG_ECDSA_BITS == 224
#define SBIF_ECDSA_PAD_BITS 32
#endif /* SBIF_CFG_ECDSA_BITS */

/** ECDSA bytes. */
#define SBIF_ECDSA_BYTES        	(SBIF_ECDSA_BITS >> 3)

/** ECDSA words. */
#define SBIF_ECDSA_WORDS            (SBIF_ECDSA_BITS >> 5)

/** Encryption key length. */
#define SBIF_ENCRYPTIONKEY_LEN    	((SBIF_CFG_CONFIDENTIALITY_BITS / 32) + 2)
#define SBIF_ENCRYPTIONKEY256_LEN	((256 / 32) + 2)

/** Encryption key iv length. */
#define SBIF_ENCRYPTIONIV_LEN 		(128 / 32)

/** Maximum number of attribute elements. */
#define SBIF_NUM_ATTRIBUTES 		8

/* -----------------
 * Structures
* ------------------ */

/** ECDSA signature. */
typedef struct
{
	uint8_t r[SBIF_ECDSA_BYTES];	/** r. */
	uint8_t s[SBIF_ECDSA_BYTES];	/** s. */
#ifdef SBIF_ECDSA_PAD_BITS
	/* Notice: the padding is in the end of the structure. */
	uint8_t pad[SBIF_ECDSA_PAD_BITS/8 * 2];
#endif /* SBIF_ECDSA_PAD_BITS */
}
SBIF_ECDSA_Signature_t;

/** ECDSA public key. */
typedef struct
{
	uint8_t Qx[SBIF_ECDSA_BYTES];    /** Qx. */
	uint8_t Qy[SBIF_ECDSA_BYTES];    /** Qy. */
}
SBIF_ECDSA_PublicKey_t;

/** ECDSA certificate. */
typedef struct
{
    SBIF_ECDSA_PublicKey_t PublicKey;    /** Public key. */
    SBIF_ECDSA_Signature_t Signature;    /** Signature. */
}
SBIF_ECDSA_Certificate_t;

/** Define Attribute type and its allowed constants. */
typedef uint32_t SBIF_AttributeElementType_t;

#define SBIF_ATTRIBUTE_UNUSED      0 /* All element positions not used. */
#define SBIF_ATTRIBUTE_VERSION     1 /* Version field for attribute array. */
#define SBIF_ATTRIBUTE_ROLLBACK_ID 2 /* Optional rollback identifier */

/** Minimum attribute version. */
#define SBIF_ATTRIBUTE_VERSION_CURRENT 0 /** First version. */

/** Minimum current rollback identifier.
    SecureBoot shall not process images with rollback counter less than this. */
#ifdef SBIF_CFG_ATTRIBUTE_MINIMUM_ROLLBACK_ID
#define SBIF_ATTRIBUTE_MINIMUM_ROLLBACK_ID \
        SBIF_CFG_ATTRIBUTE_MINIMUM_ROLLBACK_ID
#endif /* SBIF_CFG_ATTRIBUTE_MINIMUM_ROLLBACK_ID */

/** Attribute data (incl. version id). */
typedef struct
{
	SBIF_AttributeElementType_t ElementType;
	uint32_t ElementValue;
}
SBIF_AttributeElement_t;

typedef struct
{
	/** Attribute data element. */
	SBIF_AttributeElement_t AttributeElements[SBIF_NUM_ATTRIBUTES];
}
SBIF_Attributes_t;


/**
	Signing header for the images.
 */
typedef struct {
	uint32_t               Type;                /** Type. */
	uint32_t               PubKeyType;          /** Type of public key */
	SBIF_ECDSA_Signature_t Signature;           /** Signature. */
	SBIF_ECDSA_PublicKey_t PublicKey;           /** Public key (if included in image). */
#ifdef SBIF_ECDSA_PAD_BITS
	/* Notice: add padding to Public key to make it the same size whether we
	   do 224 or 256-bit ECC. */
	uint8_t pad[SBIF_ECDSA_PAD_BITS/8 * 2];
#endif /* SBIF_ECDSA_PAD_BITS */
	uint32_t               EncryptionKey[SBIF_ENCRYPTIONKEY256_LEN]; /** Key. */
	uint32_t               EncryptionIV[SBIF_ENCRYPTIONIV_LEN]; /** IV. */
	uint32_t               ImageLen;            /** Image length. */
	SBIF_Attributes_t      ImageAttributes;     /** Image attributes. */
	uint32_t               CertificateCount;    /** Certificate count. */
} SBIF_ECDSA_Header_t;

/*
  SBIF_ECDSA_GET_HEADER_SIZE

  Return total size of header including the space required by
  certificates. Returns 0 on error.
  Macro needs to be provided with known maximum number of bytes
  it is allowed to examine.
*/

/** Header size. */
#define SBIF_ECDSA_GET_HEADER_SIZE(Header_p, AccessibleByteSize)  \
    SBIF_ECDSA_GetHeaderSize((const void *)(Header_p), (AccessibleByteSize))

/* Helper inline function for fetching image size.
   Conventionally used via SBIF_ECDSA_GET_HEADER_SIZE macro. */
static inline uint32_t SBIF_ECDSA_GetHeaderSize(
        const SBIF_ECDSA_Header_t * const Header_p,
        const uint32_t AccessibleByteSize)
{
    uint32_t sizeRequired = sizeof(SBIF_ECDSA_Header_t);
    uint8_t certificateCount = 0;

    if (AccessibleByteSize >= sizeRequired)
    {
        /* NOTE: Currently up-to 255 certificates supported. */
        certificateCount = *(((uint8_t *) &(Header_p->CertificateCount)) + 3);
        sizeRequired += certificateCount * sizeof(SBIF_ECDSA_Certificate_t);
    }

    return AccessibleByteSize >= sizeRequired? sizeRequired: 0;
}

#endif /* __SECURE_BOOT_H__ */
