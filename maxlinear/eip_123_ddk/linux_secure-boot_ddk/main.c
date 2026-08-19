/*
** Secure Boot Image Authentication usign EIP-123 DDK
** Author : William Widjaja <w.widjaja.ee@lantiq.com>
** Date : 22-Dec-2014
**
*/

/*
 * Headers
*/

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>        // offsetof, size_t
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sfzcryptoapi.h>
#include <sfzcrypto_context.h>
#include "secure_boot.h"
#ifdef ICC_IMAGE
#include <icc.h>
#endif /* ICC_IMAGE */

/*
 * Macro define
*/

// Select the correct index in NVM of the unwrap or derive key.
#ifdef SBIF_CFG_DERIVE_WRAPKEY_FROM_KDK
#if SBIF_CFG_CONFIDENTIALITY_BITS == 128
#define SBLIB_CFG_CM_IMAGE_TYPE_W_ASSET_KEY SBLIB_CFG_CM_IMAGE_TYPE_W_ASSET_DERIVE_KEY_128
#else
#define SBLIB_CFG_CM_IMAGE_TYPE_W_ASSET_KEY SBLIB_CFG_CM_IMAGE_TYPE_W_ASSET_DERIVE_KEY_256
#endif
#else
#if SBIF_CFG_CONFIDENTIALITY_BITS == 128
#define SBLIB_CFG_CM_IMAGE_TYPE_W_ASSET_KEY SBLIB_CFG_CM_IMAGE_TYPE_W_ASSET_KEY_128
#else
#define SBLIB_CFG_CM_IMAGE_TYPE_W_ASSET_KEY SBLIB_CFG_CM_IMAGE_TYPE_W_ASSET_KEY_256
#endif
#endif

#define SBLIB_CFG_STORAGE_SIZE         4096

#if SBIF_ECDSA_WORDS == 7
#define ECDSA_SHA2XX SFZCRYPTO_ALGO_HASH_SHA224;
#elif SBIF_ECDSA_WORDS == 8
#define ECDSA_SHA2XX SFZCRYPTO_ALGO_HASH_SHA256;
#else
    #error "Unsupported SBIF_ECDSA_WORDS"
    /* No hash to use available */
#endif /* SBIF_ECDSA_WORDS */

// dump necessary info , ksbcr , kimg , cert hashes, total atribute+image hashes
// #define DBG_DUMP

/*
 * Type definition
*/

/** Vector. */
typedef struct
{
    uint32_t  *Data_p;          /** Data. */
    uint32_t   DataLen;         /** Data length. */
}
SBIF_SGVector_t;

/** Storage area for SB_ECDSA_* and SB_NVM_*. */
/* !<WW: I don't like this, this is just a buffer to hold all Security context information, like AES Key , IV and placeholder for ECDSA Calculation
 * TODO : Remove this, For now I keep it , since look like I am stuck with ECDSA Algo from SecureBoot, and they need a place to hold temp calculation
*/
typedef struct
{
    union
    {
        uint32_t Alignment;                     /** Alignment. */
        uint8_t  Size[SBLIB_CFG_STORAGE_SIZE];     /** Size. */
    }
    Union;
}
SB_StorageArea_t;

typedef struct
{
    uint8_t CertDigest[SBLIB_CFG_CERTIFICATES_MAX][SBIF_ECDSA_BYTES];
    uint8_t Digest[SBIF_ECDSA_BYTES];       // plain 8bit byte array format, final image digest
}SBHYBRID_SymmContext_t;

#ifdef ECDSA_SW

// internal ECDSA Verification state structure
typedef struct
{
    /* Verification parameters, including domain parameters. */
    uint8_t  e[SBIF_ECDSA_BYTES]; //sha256 hash digest
    bool     has_e;

    SfzCryptoSign    RS; //signature
    SfzCryptoAsymKey Public; //Public key, Qx, Qy and domainsparam or curve parameter.
}
SBHYBRID_ECDSA_Verify_t;

// Use count of error codes as internal PENDING status return code
#define SFZCRYPTO_PENDING (SFZCRYPTO_INTERNAL_ERROR+1)

#endif /* ECDSA_SW */

struct AES_IF_Ctx
{
    uint8_t  KeyData[SBIF_CFG_CONFIDENTIALITY_BITS / 8];
    uint32_t IV[4];

    #ifdef ECDSA_SW
    /* when you need SW ECDSA from sec boot */
    SBHYBRID_ECDSA_Verify_t EcdsaContext; /* Context for SW. */
    #endif /* ECDSA_SW */

    /* Cert SHA2 hash and what else ? */
    SBHYBRID_SymmContext_t SymmContext;

    /* Common variables. */
    int CertNr;
    int CertificateCount;
    int NeedFinalDigest;

	/* Kimg */
	uint32_t AssetId;
    uint32_t AssetFilled;
};

typedef struct AES_IF_Ctx * const AES_IF_Ctx_Ptr_t;

/*
 * Extern, since not avail in include header
*/
extern SfzCryptoStatus
sfzcrypto_cm_asset_aesunwrap(
        SfzCryptoAssetId TargetAssetId,
        SfzCryptoTrustedAssetId KekAssetId,
        SfzCryptoOctetsIn * KeyBlob_p,
        SfzCryptoSize KeyBlobSize);


/*
 * Local / Static Data
*/
static SB_StorageArea_t StorageArea;

#if defined(ICC_IMAGE)
typedef struct {
    uint8_t chunk_mode;
}ICC_INFO_S;

static ICC_INFO_S icc_info = {0};
#endif /* defined(ICC_IMAGE) */
/*
 * Local Functions
*/

#ifdef ECDSA_SW

void
SBHYBRID_SW_Ecdsa_Verify_SetPublicKey(
        SBHYBRID_ECDSA_Verify_t * const Verify_p,
        const SBIF_ECDSA_PublicKey_t * const PublicKey_p)
{
    assert(PublicKey_p != NULL);

    memset((void *)&Verify_p->Public, 0, sizeof(SfzCryptoAsymKey));

    /* setup param for ecdsa verify */
    Verify_p->Public.cmd_type  = SFZCRYPTO_CMD_SIG_VERIFY;
    Verify_p->Public.algo_type = SFZCRYPTO_ALGO_ASYMM_ECDSA_WITH_SHA256;
    // Verify_p->Public.mod_bits = ; /* Modulus length in bits , seem not needed for ECDSA, it for RSA*/

    /* domainparam */
    Verify_p->Public.Key.ecPubKey.domainParam.modulus.p_num    = (uint8_t *)"\xff\xff\xff\xff\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff";
    Verify_p->Public.Key.ecPubKey.domainParam.modulus.byteLen  = 32;
    Verify_p->Public.Key.ecPubKey.domainParam.a.p_num          = (uint8_t *)"\xff\xff\xff\xff\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xfc";
    Verify_p->Public.Key.ecPubKey.domainParam.a.byteLen        = 32;
    Verify_p->Public.Key.ecPubKey.domainParam.b.p_num          = (uint8_t *)"\x5a\xc6\x35\xd8\xaa\x3a\x93\xe7\xb3\xeb\xbd\x55\x76\x98\x86\xbc\x65\x1d\x06\xb0\xcc\x53\xb0\xf6\x3b\xce\x3c\x3e\x27\xd2\x60\x4b";
    Verify_p->Public.Key.ecPubKey.domainParam.b.byteLen        = 32;
    Verify_p->Public.Key.ecPubKey.domainParam.g_order.p_num    = (uint8_t *)"\xFF\xFF\xFF\xFF\x00\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xBC\xE6\xFA\xAD\xA7\x17\x9E\x84\xF3\xB9\xCA\xC2\xFC\x63\x25\x51";
    Verify_p->Public.Key.ecPubKey.domainParam.g_order.byteLen  = 32;
    Verify_p->Public.Key.ecPubKey.domainParam.G.x_cord.p_num   = (uint8_t *)"\x6B\x17\xD1\xF2\xE1\x2C\x42\x47\xF8\xBC\xE6\xE5\x63\xA4\x40\xF2\x77\x03\x7D\x81\x2D\xEB\x33\xA0\xF4\xA1\x39\x45\xD8\x98\xC2\x96";
    Verify_p->Public.Key.ecPubKey.domainParam.G.x_cord.byteLen = 32;
    Verify_p->Public.Key.ecPubKey.domainParam.G.y_cord.p_num   = (uint8_t *)"\x4F\xE3\x42\xE2\xFE\x1A\x7F\x9B\x8E\xE7\xEB\x4A\x7C\x0F\x9E\x16\x2B\xCE\x33\x57\x6B\x31\x5E\xCE\xCB\xB6\x40\x68\x37\xBF\x51\xF5";
    Verify_p->Public.Key.ecPubKey.domainParam.G.y_cord.byteLen = 32;

    /* public key / coord */
    Verify_p->Public.Key.ecPubKey.Q.x_cord.p_num   = (uint8_t *)PublicKey_p->Qx;
    Verify_p->Public.Key.ecPubKey.Q.x_cord.byteLen = SBIF_ECDSA_BYTES;
    Verify_p->Public.Key.ecPubKey.Q.y_cord.p_num   = (uint8_t *)PublicKey_p->Qy;
    Verify_p->Public.Key.ecPubKey.Q.y_cord.byteLen = SBIF_ECDSA_BYTES;

}

void
SBHYBRID_SW_Ecdsa_Verify_SetSignature(
        SBHYBRID_ECDSA_Verify_t * const Verify_p,
        const SBIF_ECDSA_Signature_t * const Signature_p)
{
    assert(Signature_p != NULL);

    memset((void *)&Verify_p->RS, 0, sizeof(SfzCryptoSign));

    /* signature */
    Verify_p->RS.r.p_num   = (uint8_t *)Signature_p->r;
    Verify_p->RS.r.byteLen = SBIF_ECDSA_BYTES;
    Verify_p->RS.s.p_num   = (uint8_t *)Signature_p->s;
    Verify_p->RS.s.byteLen = SBIF_ECDSA_BYTES;

}


void
SBHYBRID_SW_Ecdsa_Verify_SetDigest(
        SBHYBRID_ECDSA_Verify_t * const Verify_p,
        uint8_t * Digest_p)
{
    assert(Digest_p != NULL);

    memcpy ((void *)Verify_p->e, (const void *)Digest_p, (size_t) SBIF_ECDSA_BYTES);
    Verify_p->has_e = true;
}

/* A front-end for SBHYBRID_Ecdsa_Verify*:
   set public key and signature and mark digest as not yet available. */
static inline
void
SBHYBRID_SW_Ecdsa_Verify_Init(
        SBHYBRID_ECDSA_Verify_t * const Verify_p,
        const SBIF_ECDSA_PublicKey_t * const PublicKey_p,
        const SBIF_ECDSA_Signature_t * const Signature_p)
{
    SBHYBRID_SW_Ecdsa_Verify_SetPublicKey(Verify_p, PublicKey_p);
    SBHYBRID_SW_Ecdsa_Verify_SetSignature(Verify_p, Signature_p);
    Verify_p->has_e = false;
}

/* A front-end for SBHYBRID_Ecdsa_Verify: run verify at once when
   digest is known. */
static inline
SfzCryptoStatus
SBHYBRID_SW_Ecdsa_Verify_RunFsm(
        SBHYBRID_ECDSA_Verify_t * const Verify_p)
{
    /* Return pending if digest is not yet available. */
    if (!Verify_p->has_e) return SFZCRYPTO_PENDING;

    /* Calculate ECDSA Verify if the digest is available. */
    return sfzcrypto_ecdsa_verify( (SfzCryptoContext * const) sfzcrypto_context_get(),
                                   (SfzCryptoAsymKey * const) &(Verify_p->Public),
                                   (SfzCryptoSign * const) &(Verify_p->RS),
                                   (uint8_t *)Verify_p->e,
                                   (uint32_t) SBIF_ECDSA_BYTES);
}

#endif /* ECDSA_SW */

#ifdef DBG_DUMP
void dump(AES_IF_Ctx_Ptr_t ctx_p)
{
    FILE * Outf_p;
    unsigned int i ;

    // kimg
    Outf_p = fopen("/tmp/kimg_clear.bin", "wb");
    if (Outf_p)
    {
        printf("dump /tmp/kimg_clear.bin...\n");
        fflush(stdout);
        if (0 == fwrite(ctx_p->KeyData,
                        SBIF_CFG_CONFIDENTIALITY_BITS >> 3, 1,
                        Outf_p))
        {
            printf("Dump Failed\n");
        }
        else
        {
            printf("Dump /tmp/kimg_clear.bin SUCCESS\n");
        }

        fclose(Outf_p);
        Outf_p = NULL;
    }

    // kimg_iv
    Outf_p = fopen("/tmp/kimg_iv.bin", "wb");
    if (Outf_p)
    {
        printf("dump /tmp/kimg_iv.bin...\n");
        fflush(stdout);
        if (0 == fwrite(ctx_p->IV,
                        sizeof(ctx_p->IV), 1,
                        Outf_p))
        {
            printf("Dump Failed\n");
        }
        else
        {
            printf("Dump /tmp/kimg_iv.bin SUCCESS\n");
        }

        fclose(Outf_p);
        Outf_p = NULL;
    }

    // cert hashes
    for( i = 0 ; i < SBLIB_CFG_CERTIFICATES_MAX; i++)
    {
        char tmp[32];

        snprintf(tmp, 32, "/tmp/sha256_cert%d.bin", i+1);

        Outf_p = fopen(tmp, "wb");
        if (Outf_p)
        {
            printf("dump %s...\n", tmp);
            fflush(stdout);
            if (0 == fwrite(ctx_p->SymmContext.CertDigest[i],
                            SBIF_ECDSA_BYTES, 1,
                            Outf_p))
            {
                printf("Dump %s Failed\n", tmp);
            }
            else
            {
                printf("Dump %s SUCCESS\n", tmp);
            }

            fclose(Outf_p);
            Outf_p = NULL;
        }
    }

	// dump img+attrib sha256
	Outf_p = fopen("/tmp/sha256_img_attrib.bin", "wb");
    if (Outf_p)
    {
        printf("dump /tmp/sha256_img_attrib.bin...\n");
        fflush(stdout);
        if (0 == fwrite(ctx_p->SymmContext.Digest,
                        sizeof(ctx_p->SymmContext.Digest), 1,
                        Outf_p))
        {
            printf("Dump Failed\n");
        }
        else
        {
            printf("Dump /tmp/sha256_img_attrib.bin SUCCESS\n");
        }

        fclose(Outf_p);
        Outf_p = NULL;
    }
	
}
#endif /* DBG_DUMP */

static void usage(char* exename)
{
    fprintf(
    stderr,
    "usage:\n"
    "\t%s <image file>\n\n"
,
    exename);
}

static inline uint32_t
Load_BE32(
        const void * const Value_p)
{
    const uint8_t * const p = (const uint8_t *)Value_p;

    return (p[0] << 24 |
            p[1] << 16 |
            p[2] << 8  |
            p[3]);
}

/*----------------------------------------------------------------------------
 * SBIF_Attribute_Fetch
 */
bool
SBIF_Attribute_Fetch(
        const SBIF_Attributes_t * const Attributes_p,
        SBIF_AttributeElementType_t     ElementType,
        uint32_t * const                ElementValue_p)
{
    int i;

    /* Scan for insertion point. */
    for(i = 0; i < SBIF_NUM_ATTRIBUTES; i++)
    {
        SBIF_AttributeElementType_t type =
            Load_BE32(&Attributes_p->AttributeElements[i].ElementType);

        if (type == ElementType)
        {
            *ElementValue_p =
                Load_BE32(&Attributes_p->AttributeElements[i].ElementValue);
            return true;
        }
    }

    return false;
}


/*----------------------------------------------------------------------------
 * SBIF_Attribute_Check
 */
bool
SBIF_Attribute_Check(
        const SBIF_Attributes_t * const Attributes_p)
{
    SBIF_AttributeElementType_t type;
    int i;

    /* Get first attribute. */
    type = Load_BE32(&Attributes_p->AttributeElements[0].ElementType);

    /* Check there is version attribute with correct value. */
    if (type != SBIF_ATTRIBUTE_VERSION ||
        Load_BE32(&Attributes_p->AttributeElements[0].ElementValue) !=
        SBIF_ATTRIBUTE_VERSION_CURRENT)
    {
        return false;
    }

    for(i = 1; i < SBIF_NUM_ATTRIBUTES; i++)
    {
        SBIF_AttributeElementType_t nextType;

        nextType = Load_BE32(&Attributes_p->AttributeElements[i].ElementType);

        if (nextType != SBIF_ATTRIBUTE_UNUSED)
        {
            /* Check attribute order or dup. */
            if (nextType <= type)
                return false;

            /* Check for unknown non-extension attributes. */
            if (nextType > SBIF_ATTRIBUTE_ROLLBACK_ID &&
                (nextType & 0x80000000) == 0)
            {
                return false;
            }

#ifdef SBIF_ATTRIBUTE_MINIMUM_ROLLBACK_ID
    /* Check attribute value (ROLLBACK ID attribute only) */
#if SBIF_ATTRIBUTE_MINIMUM_ROLLBACK_ID > 0
            if (nextType == SBIF_ATTRIBUTE_ROLLBACK_ID)
            {
                if (Load_BE32(&Attributes_p->AttributeElements[i].ElementValue) <
                    SBIF_ATTRIBUTE_MINIMUM_ROLLBACK_ID)
                {
                    return false;
                }
            }
#endif /* SBIF_ATTRIBUTE_MINIMUM_ROLLBACK_ID > 0 */
#endif /* SBIF_ATTRIBUTE_MINIMUM_ROLLBACK_ID */
        }

        type = nextType;
    }

    return true;
}

static void
Print_OctetString(
        const char * const Description_p,
        const void * const Data_p,
        const unsigned int DataLen)
{
    const uint8_t * Byte_p = Data_p;
    int Column = 16;
    unsigned int PrintedLen;

    printf("%s:", Description_p);

    for (PrintedLen = 0; PrintedLen < DataLen; PrintedLen++)
    {
        if (Column++ == 16)
        {
            printf("\n\t");
            Column = 1;
        }

        printf("%.2x ", (int) *Byte_p);
        Byte_p++;
    }

    printf("\n");
}

static void
Print_EcdsaPublicKey(
        const char * const Description_p,
        const SBIF_ECDSA_PublicKey_t * const PublicKey_p)
{
    printf("%s:\n", Description_p);
    Print_OctetString("Qx", PublicKey_p->Qx, sizeof(PublicKey_p->Qx));
    Print_OctetString("Qy", PublicKey_p->Qy, sizeof(PublicKey_p->Qy));
}

static void
Print_EcdsaSignature(
        const char * const Description_p,
        const SBIF_ECDSA_Signature_t * const Signature_p)
{
    printf("%s:\n", Description_p);
    Print_OctetString(
            "r", &Signature_p->r,
            sizeof(Signature_p->r));

    Print_OctetString(
            "s",
            &Signature_p->s,
            sizeof(Signature_p->s));
}

static void
Print_ImageInfo(
        const SBIF_ECDSA_Header_t * const Header_p)
{
    uint32_t TypeWord;
    uint32_t PubKeyType;
    uint32_t Type;
    uint8_t TypeChar[3];
    int header_image_len;

    TypeWord = Load_BE32(&Header_p->Type);
    TypeChar[2] = (TypeWord >> 8) & 0xff;
    TypeChar[1] = (TypeWord >> 16) & 0xff;
    TypeChar[0] = (TypeWord >> 24) & 0xff;

    printf("Image header:\n");
    if (TypeChar[0] >= 'A' && TypeChar[0] <= 'Z' &&
        TypeChar[1] >= 'A' && TypeChar[1] <= 'Z' &&
        TypeChar[2] >= 'a' && TypeChar[2] <= 'z')
    {
        printf("Type:\t%c%c%c\n", TypeChar[0], TypeChar[1], TypeChar[2]);
    }
    else
    {
        printf("Type:\tUnknown (corrupt header)\n");
    }
    printf("Version: %d\n", (int) SBIF_TYPE_VERSION(TypeWord));

    PubKeyType = Load_BE32(&Header_p->PubKeyType);
    {
        if (PubKeyType == SBIF_PUBKEY_TYPE_ROM)
        {
            printf("Public key in ROM\n");
        }
        else if (PubKeyType == SBIF_PUBKEY_TYPE_OTP)
        {
            printf("Public key in OTP\n");
        }
        else if (PubKeyType == SBIF_PUBKEY_TYPE_IMAGE)
        {
            printf("Public key in image header\n");
        }
        else
        {
            printf("Unknow public key type\n");
        }

    }

    Print_OctetString(
            "r",
            Header_p->Signature.r,
            sizeof(Header_p->Signature.r));

    Print_OctetString(
            "s",
            Header_p->Signature.s,
            sizeof(Header_p->Signature.r));

    Print_OctetString(
            "Encryption key",
            Header_p->EncryptionKey,
            sizeof(Header_p->EncryptionKey));

    Print_OctetString(
            "iv",
            Header_p->EncryptionIV,
            sizeof(Header_p->EncryptionIV));

    header_image_len = Load_BE32(&Header_p->ImageLen);
    printf("len:\t%u\n", (uint32_t) header_image_len);

    Type = SBIF_TYPE_TYPE(TypeWord);

    if (Type == SBIF_IMAGE_BLTp  ||
        Type == SBIF_IMAGE_BLTw  ||
        Type == SBIF_IMAGE_BLTe)
    {
        SBIF_ECDSA_Certificate_t *Certs_p;
        int CertCount;
        int CertIndex;

        Certs_p = (void *) (Header_p + 1);

        CertCount = Load_BE32(&Header_p->CertificateCount);
        printf("cert count: %d\n", CertCount);

        for (CertIndex = 0; CertIndex < CertCount; CertIndex++)
        {
            if (CertIndex == 4 && CertIndex < CertCount)
            {
                printf("%u more certificates\n", CertCount - CertIndex + 1);
                break;
            }

            printf("cert %d\n", CertIndex);
            Print_EcdsaPublicKey(
                    "public key",
                    &Certs_p[CertIndex].PublicKey);

            Print_EcdsaSignature(
                    "signature",
                    &Certs_p[CertIndex].Signature);
        }
    }

    if (SBIF_Attribute_Check(&Header_p->ImageAttributes))
    {
        uint32_t Value32 = 0;

        printf("Image attributes are valid\n");

        // check for the rollback attribute
        if (SBIF_Attribute_Fetch(
                    &Header_p->ImageAttributes,
                    SBIF_ATTRIBUTE_ROLLBACK_ID,
                    &Value32))
        {
            printf("  RollbackID: 0x%08x\n", Value32);
        }
        else
        {
            printf("  RollbackID: [not found]\n");
        }
    }
    else
    {
        printf("Image attributes check failed\n");
    }
}

#ifdef SBIF_CFG_DERIVE_WRAPKEY_FROM_KDK
void AES_IF_Ctx_DeriveKEK(SfzCryptoAssetId *KEKAssetId_p,
                                const uint8_t * const DeriveInfo_p,
                                const uint32_t DeriveInfoSize)
{
    SfzCryptoAssetId KDK_AssetId = 0;
    SfzCryptoAssetId KEK_AssetId = 0;
    SfzCryptoStatus  result = SFZCRYPTO_UNSUPPORTED;
    
    SfzCryptoPolicyMask KEKPolicy = SFZCRYPTO_POLICY_ALGO_CIPHER_AES |
                                    SFZCRYPTO_POLICY_FUNCTION_ENCRYPT |
                                    SFZCRYPTO_POLICY_FUNCTION_DECRYPT;

    /* First load the Key Decryption Key. */
    result = sfzcrypto_asset_search(sfzcrypto_context_get(),
                                    SBLIB_CFG_CM_IMAGE_TYPE_W_ASSET_KEY,
                                    &KDK_AssetId);

    if(SFZCRYPTO_SUCCESS != result)
    {
        fprintf(stderr, "[%s]fail to get KDK Asset (%d) Error: %d\n", __FUNCTION__, SBLIB_CFG_CM_IMAGE_TYPE_W_ASSET_KEY, result);
        return;
    }

    /* NOTE: TODO: !WW> : I remember wuq said about something must be in temporary asset , is this it ?  */
    /* Create additional asset for the derived key */
    result = sfzcrypto_asset_alloc(sfzcrypto_context_get(),
                                     KEKPolicy,
                                     SBIF_CFG_CONFIDENTIALITY_BITS >> 3,
                                    &KEK_AssetId);

    if(SFZCRYPTO_SUCCESS != result)
    {
        fprintf(stderr, "[%s]fail to get Create New KEK Asset Error: %d\n", __FUNCTION__, result);
        return;
    }
    
    /* Call key derive function */
    result = sfzcrypto_asset_derive(sfzcrypto_context_get(),
                                     KEK_AssetId,
                                     KDK_AssetId,
                                     DeriveInfo_p,
                                     DeriveInfoSize);
    if(SFZCRYPTO_SUCCESS != result)
    {
        fprintf(stderr, "[%s]fail to get Derive KEK Asset Error: %d\n", __FUNCTION__, result);
        return;
    }

    *KEKAssetId_p = KEK_AssetId;
    return;
}
#endif /* SBIF_CFG_DERIVE_WRAPKEY_FROM_KDK */

void AES_IF_Ctx_SetKEK(SfzCryptoAssetId *KEKAssetId_p)
{
    SfzCryptoAssetId KEK_AssetId = 0;
    SfzCryptoStatus  result = SFZCRYPTO_UNSUPPORTED;
    
    /* Search for asset (SBLIB_CFG_CM_IMAGE_TYPE_W_ASSET_KEY) and
           use it for AES Key Unwrap. */

    /* First load the Key Decryption Key. */
    result = sfzcrypto_asset_search(sfzcrypto_context_get(),
                                    SBLIB_CFG_CM_IMAGE_TYPE_W_ASSET_KEY,
                                    &KEK_AssetId);

    if(SFZCRYPTO_SUCCESS != result)
    {
        fprintf(stderr, "[%s]fail to get KEK Asset (%d) Error: %d\n", __FUNCTION__, SBLIB_CFG_CM_IMAGE_TYPE_W_ASSET_KEY, result);
        return;
    }

    *KEKAssetId_p = KEK_AssetId;
    return;
}

/*
 * Decrypted Kimg is in Wrap_p
*/
void AES_IF_Ctx_LoadWrappedKey(AES_IF_Ctx_Ptr_t Ctx_p,
										SfzCryptoAssetId KEKAssetId,
                                        const void * const Wrap_p,
                                        const uint32_t WrapLength)
{
    uint32_t NewAssetId;
    uint32_t Policy = SFZCRYPTO_POLICY_ALGO_CIPHER_AES |
        SFZCRYPTO_POLICY_FUNCTION_ENCRYPT |
        SFZCRYPTO_POLICY_FUNCTION_DECRYPT;
    SfzCryptoStatus        result = SFZCRYPTO_UNSUPPORTED;
    SfzCryptoCipherContext aes_ctx;
    SfzCryptoCipherKey     kek_key;
    uint32_t               unwrap_length;
    /* safety */
    memset((void*)&aes_ctx,0,sizeof(aes_ctx));
    memset((void*)&kek_key,0,sizeof(kek_key));

    /* !<WW: I dont think I need to create additional asset for unwrapped key, since this is Unwrap is SW routine and they return to me , the kimg directly 
         * TODO: Remove */
    /* Create additional asset for the unwrapped key */
    result = sfzcrypto_asset_alloc(sfzcrypto_context_get(),
                                     Policy,
                                     SBIF_CFG_CONFIDENTIALITY_BITS >> 3,
                                    &NewAssetId);

    if(SFZCRYPTO_SUCCESS != result)
    {
        fprintf(stderr, "[%s]fail to get Create New Unwrapped Key Asset Error: %d\n", __FUNCTION__, result);
        return;
    }

	result = sfzcrypto_cm_asset_aesunwrap( NewAssetId,
        									KEKAssetId,
        									(SfzCryptoOctetsIn *) Wrap_p,
        									(SfzCryptoSize) (WrapLength >> 3)); // 256 + 64 , 64 bits extra is added when encrypt due to HKDF
	if(SFZCRYPTO_SUCCESS != result)
    {
        fprintf(stderr, "[%s]fail to Unwrap Kimg: %d\n", __FUNCTION__, result);
        return;
    }

	Ctx_p->AssetId     = NewAssetId;
	Ctx_p->AssetFilled = 1;

    return;
}

/* Image header conversion. Also give you the clear unwrapped Kimg key */
int
SBIFTOOLS_Image_ConvertHeader(
        const SBIF_ECDSA_Header_t * const Header_p,
        const uint32_t NewSbifType,
        const uint8_t NewSbifVersion,
        SBIF_ECDSA_Header_t * const HeaderNew_p,
        size_t * const HeaderNewSize_p,
        void * const CryptoCtx)
{
    const size_t hdrSizeNew = SBIF_ECDSA_GET_HEADER_SIZE(Header_p,
                                                         (uint32_t)-1);

    const uint32_t imgTypeVers = Load_BE32(&Header_p->Type);
    const uint32_t imgType = SBIF_TYPE_TYPE(imgTypeVers);
    const uint32_t imgVers = SBIF_TYPE_VERSION(imgTypeVers);

    SfzCryptoAssetId KEK_AssetId;

    AES_IF_Ctx_Ptr_t Ctx_p = (AES_IF_Ctx_Ptr_t)CryptoCtx;

     /* Validate old image */
    if (imgType != SBIF_IMAGE_BLTw)
        return 1;

    if (imgVers != SBIF_VERSION)
        return 2;

    /* Validate requested image equivalently. */
    if (NewSbifType != SBIF_IMAGE_BLTp)
        return 1;

    if (NewSbifVersion != SBIF_VERSION)
        return 2;

    if (*HeaderNewSize_p < hdrSizeNew)
    {
        *HeaderNewSize_p = hdrSizeNew;
        return 3;
    }

    if (imgType == SBIF_IMAGE_BLTw &&
        NewSbifType == SBIF_IMAGE_BLTp)
    {
        /* Check for non-inplace operations. */
        if (HeaderNew_p != Header_p)
        {
            /* Construct new header using old header as a template. */
            memcpy(HeaderNew_p, Header_p, hdrSizeNew);

            /* Ensure HeaderNew_p matches the original header in size. */
            if (SBIF_ECDSA_GET_HEADER_SIZE(HeaderNew_p, hdrSizeNew) !=
                hdrSizeNew)
            {
                return 3;
            }
        }

        /* Begin decrypt BLTw image. */
#ifdef SBIF_CFG_DERIVE_WRAPKEY_FROM_KDK
        AES_IF_Ctx_DeriveKEK(&KEK_AssetId,
                             (uint8_t*)&Header_p->Signature,
                             64);
#else
        AES_IF_Ctx_SetKEK(&KEK_AssetId);
#endif
        AES_IF_Ctx_LoadWrappedKey(Ctx_p,
        						  KEK_AssetId,
                                  &HeaderNew_p->EncryptionKey,
                                  (SBIF_CFG_CONFIDENTIALITY_BITS+64));

        /* Load IV to Context */
        memcpy(Ctx_p->IV, &Header_p->EncryptionIV, 16);
        
        /* Write new ImageType Blp ? to  new header */

        /* BLp: empty encryption key field. */
        memset(&HeaderNew_p->EncryptionKey, 0, SBIF_ENCRYPTIONKEY_LEN*4);
        
    }

    return 0;
}

#ifdef ECDSA_SW
/*----------------------------------------------------------------------------
 * SBHYBRID_Do_Asym_RunFsm
 *
 * Single step of asymmetric processing:
 * Performs run EIP28 RunFsm. If the current certificate gets
 * handled, starts processing next certificate.
 * Assumes all digests have been precalculated.
 * Calls polling as appropriate.
 */
static
SfzCryptoStatus
SBHYBRID_Do_Asym_RunFsm(AES_IF_Ctx_Ptr_t Context_p,
                      const SBIF_ECDSA_Header_t * Header_p)
{
    SfzCryptoStatus res;

    res = SBHYBRID_SW_Ecdsa_Verify_RunFsm(&Context_p->EcdsaContext);
    if (res == SFZCRYPTO_SUCCESS)
    {
#ifdef SBLIB_CF_REMOVE_CERTIFICATE_SUPPORT
        /* This is success from the final EcdsaVerify. */
        return res;
#else /* !SBLIB_CF_REMOVE_CERTIFICATE_SUPPORT */
        /* A certificate has been processed, move to the next
           certificate, with the previous certificates public key.
        */
        const SBIF_ECDSA_Certificate_t * Certificates_p =
            (const SBIF_ECDSA_Certificate_t *) (Header_p + 1);
        const SBIF_ECDSA_Certificate_t * Certificate_p;
        const SBIF_ECDSA_Certificate_t * PrevCertificate_p;
        int CertNr;

        PrevCertificate_p = Certificates_p + Context_p->CertNr;
        CertNr = ++Context_p->CertNr;
        Certificate_p = Certificates_p + CertNr;

        if (Context_p->CertNr == Context_p->CertificateCount + 1)
        {
            /* This is success from the final EcdsaVerify. */
            return res;
        }
        else if (Context_p->CertNr == Context_p->CertificateCount)
        {
            /* Start the final EcdsaVerify */
            SBHYBRID_SW_Ecdsa_Verify_Init(
                    &Context_p->EcdsaContext,
                    &PrevCertificate_p->PublicKey,
                    &Header_p->Signature);

            /* Final digest is available in my case,
               I did the hash beforehand. */
            SBHYBRID_SW_Ecdsa_Verify_SetDigest(
                    &Context_p->EcdsaContext,
                    Context_p->SymmContext.Digest);
        }
        else
        {
            /* Continue with the next certificate. */
            SBHYBRID_SW_Ecdsa_Verify_Init(
                    &Context_p->EcdsaContext,
                    &PrevCertificate_p->PublicKey,
                    &Certificate_p->Signature);

            SBHYBRID_SW_Ecdsa_Verify_SetDigest(
                    &Context_p->EcdsaContext,
                    Context_p->SymmContext.CertDigest[CertNr]);
        }
        /* Operation pending, with the next ecdsa verify started. */
        return SFZCRYPTO_PENDING;
#endif /* SBLIB_CF_REMOVE_CERTIFICATE_SUPPORT */
    }

    return res;
}

#endif /* ECDSA_SW */

/*----------------------------------------------------------------------------
 * SB_ECDSA_Image_CopyOrDecrypt_Verify
 */
int
SB_ECDSA_Image_CopyOrDecrypt_Verify(
        SB_StorageArea_t * const             Storage_p,
        const SBIF_ECDSA_PublicKey_t * const PublicKey_p,
        const SBIF_ECDSA_Header_t *          Header_p,
        const SBIF_SGVector_t *              DataVectorsIn_p,
        const SBIF_SGVector_t *              DataVectorsOut_p)
{
    AES_IF_Ctx_Ptr_t Context_p   = (AES_IF_Ctx_Ptr_t)Storage_p;
    //const uint32_t TypeField        = Load_BE32(&Header_p->Type);
    const uint32_t ImageLen         = Load_BE32(&Header_p->ImageLen);
    const uint32_t CertificateCount = Load_BE32(&Header_p->CertificateCount);

    SfzCryptoStatus        res = SFZCRYPTO_UNSUPPORTED;
    int                    ret = 0;
    SfzCryptoHashContext   sha_ctx;
    SfzCryptoCipherContext aes_ctx;
    SfzCryptoCipherKey     aes_key;
    uint32_t               tmp_dst_len;

    #ifdef ECDSA_SW
    SfzCryptoStatus  ecdsa_res;
    #endif /* ECDSA_SW */

    // below used to be parameter in function call , i just hardcoded the expected value now,
    // so you can easily refer to original code
    uint32_t VectorCount = 1;
    bool     DoDecrypt = true;

    assert(Storage_p != NULL);
    assert(PublicKey_p != NULL);
    assert(Header_p != NULL);
    assert(DataVectorsIn_p != NULL);
    assert(DataVectorsOut_p != NULL);

    // safety, init struct with 0
    memset((void *)&sha_ctx, 0, sizeof(sha_ctx));
    memset((void *)&aes_ctx, 0, sizeof(aes_ctx));
    memset((void *)&aes_key, 0, sizeof(aes_key));

    #ifdef CONFIG_ENCRYPT_ATTRIBUTE
    {
        /* Instantiate a key */
        aes_key.type     = SFZCRYPTO_KEY_AES;
        aes_key.length   = SBIF_CFG_CONFIDENTIALITY_BITS >> 3;
        aes_key.asset_id = Context_p->AssetId;

        /* set up context and iv */
        aes_ctx.fbmode      = SFZCRYPTO_MODE_CBC;
        memcpy((void *)aes_ctx.iv, (const void *)Context_p->IV, sizeof (Context_p->IV));
        aes_ctx.iv_asset_id = SFZCRYPTO_ASSETID_INVALID;
        aes_ctx.iv_loc      = SFZ_IN_CONTEXT;

        /* setup var */
        tmp_dst_len = 64;

        #ifdef CONFIG_OBFUSCATION
        //TODO: or perhap not needed in linux image auth
        #endif /*CONFIG_OBFUSCATION*/

        res = sfzcrypto_symm_crypt( sfzcrypto_context_get(),
                                    (SfzCryptoCipherContext * const) &aes_ctx,
                                    (SfzCryptoCipherKey * const) &aes_key,
                                    (uint8_t *) &Header_p->ImageAttributes,
                                    64,
                                    (uint8_t *) &Header_p->ImageAttributes,
                                    (uint32_t * const) &tmp_dst_len,
                                    SFZ_DECRYPT);

        if ((res != SFZCRYPTO_SUCCESS) ||
            (64 != tmp_dst_len))
        {
            fprintf(stderr,
                    "aes decrypt failed (res=%d)", res);
            return 6;
        }

        #ifdef CONFIG_OBFUSCATION
        //TODO: or perhap not needed in linux image auth
        #endif /* CONFIG_OBFUSCATION */

        /* copy back IV from result to context*/
        //memcpy((void *)Context_p->IV, (const void *)aes_ctx.iv, sizeof (Context_p->IV));

        #ifdef DBG_DUMP
        {
            FILE * Outf_p;
            unsigned int i ;

            // kimg
            Outf_p = fopen("/tmp/img_attrib_clear.bin", "wb");
            if (Outf_p)
            {
                printf("dump /tmp/img_attrib_clear.bin...\n");
                fflush(stdout);
                if (0 == fwrite(&Header_p->ImageAttributes,
                                64, 1,
                                Outf_p))
                {
                    printf("Dump Failed\n");
                }
                else
                {
                    printf("Dump /tmp/img_attrib_clear.bin SUCCESS\n");
                }

                fclose(Outf_p);
                Outf_p = NULL;
            }
        }
        #endif /**/
    }
    #endif /* CONFIG_ENCRYPT_ATTRIBUTE */
	
    if (CertificateCount > SBLIB_CFG_CERTIFICATES_MAX)
    {
        fprintf(stderr,
                "CertificateCount %u too big, "
                "maximum supported is %u.",
                CertificateCount,
                SBLIB_CFG_CERTIFICATES_MAX);

        return 1;
    }
    #ifndef CONFIG_ENCRYPT_ATTRIBUTE
    // Check the attributes
    else if (SBIF_Attribute_Check(&Header_p->ImageAttributes) == false)
    {
        /* Invalid attributes. */
        return 2;
    }
    #endif /* CONFIG_ENCRYPT_ATTRIBUTE */

    // All sanity checks done

    printf("Initializing verify operation for image signature.\n");

    if (CertificateCount != 0)
    {
        const SBIF_ECDSA_Certificate_t * Certificate_p;
        unsigned int                     CertNr;

        Certificate_p = (const SBIF_ECDSA_Certificate_t *)(Header_p + 1);
        sha_ctx.algo  = ECDSA_SHA2XX;

        for (CertNr = 0; CertNr < CertificateCount; CertNr++, Certificate_p++)
        {
            // calculate the hash over the public key in each certificate
            res = sfzcrypto_hash_data( sfzcrypto_context_get(),
                                       (SfzCryptoHashContext * const) &sha_ctx,
                                       (uint8_t *) Certificate_p,                 // public key is first field
                                       (uint32_t) sizeof(SBIF_ECDSA_PublicKey_t),
                                       true, // init
                                       true); // final

            // done with this certificate
            if (res != SFZCRYPTO_SUCCESS)
            {
                fprintf(stderr,
                        "Certificate %u hash failed (res=%d)", CertNr, res);
                ret = 3;
                break;  // from the for
            }

            // successfully calculated the digest
            memcpy((void *)Context_p->SymmContext.CertDigest[CertNr] , (const void*)sha_ctx.digest, (size_t)SBIF_ECDSA_BYTES);

            if (CertNr == 0)
            {
                /* 
                             * start certificate verification , remember the chain start from nvm public key to verify first cert signature , well one thing they don't
                             * mention is the use of SHA2 digest of first cert public key as initial ecdsa digest.
                            */
                #ifdef ECDSA_SW

                /* Set first EcdsaVerify target. */
                SBHYBRID_SW_Ecdsa_Verify_Init(
                            &Context_p->EcdsaContext,
                            PublicKey_p,
                            &Certificate_p->Signature);

                SBHYBRID_SW_Ecdsa_Verify_SetDigest(
                            &Context_p->EcdsaContext,
                            Context_p->SymmContext.CertDigest[CertNr]);
                #endif /* ECDSA_SW */
            }
        }
    }

    /* Keep track of ecdsa verify's certificate under processing. */
    Context_p->CertNr           = 0;
    Context_p->CertificateCount = CertificateCount;

    // hash
    if( ret == 0 )
    {
        uint32_t     ImageLenLeft = ImageLen;
        unsigned int idx;

        /* calculate the hash over the attributes and the image */

        // start with image attribute 
        sha_ctx.algo  = ECDSA_SHA2XX;
        res = sfzcrypto_hash_data( sfzcrypto_context_get(),
                                   (SfzCryptoHashContext * const) &sha_ctx,
                                   (uint8_t *) &Header_p->ImageAttributes,                 
                                   (uint32_t) sizeof(Header_p->ImageAttributes),
                                   true, // init
                                   false); // final

        if (res != SFZCRYPTO_SUCCESS)
        {
            fprintf(stderr,
                    "hash image attribute failed (res=%d)", res);
            return 3;
        }

        /* setup one time cipher context */
        if(DoDecrypt)
        {
            /* Instantiate a key */
            aes_key.type     = SFZCRYPTO_KEY_AES;
            aes_key.length   = SBIF_CFG_CONFIDENTIALITY_BITS >> 3;
            aes_key.asset_id = Context_p->AssetId;
			
            /* set up context and iv */
            aes_ctx.fbmode      = SFZCRYPTO_MODE_CBC;
            memcpy((void *)aes_ctx.iv, (const void *)Context_p->IV, sizeof (Context_p->IV));
            aes_ctx.iv_asset_id = SFZCRYPTO_ASSETID_INVALID;
            aes_ctx.iv_loc      = SFZ_IN_CONTEXT;
        }

        // add the image blocks
        // !<WW: I don't actually intend to support multiple vectors or mailbox ..... but oh well , just copying code as it is
        // who know those reading now will find a way to make use of it.
        for (idx = 0; idx < VectorCount; idx++)
        {
            /* !<WW: now , in original code , they suppose to do parallel processing between hash , ecdsa verify and decrypt 
                      * but I don't think it will work with the ddk, I suppose they are all blocking functions and their parallel codes gross me so much 
                      * so let make it simple, decrypt then hash then continue the ecdsa with final digest on last cert */

            SBIF_SGVector_t VectorIn  = DataVectorsIn_p[idx];
            SBIF_SGVector_t VectorOut = DataVectorsOut_p[idx];

            if (ImageLenLeft < VectorIn.DataLen ||
                VectorIn.DataLen != VectorOut.DataLen)
            {
                ret = 4;
                break;
            }

            /* loop over whole image in blocklen over 1 MB */
            while (ret == 0 && VectorIn.DataLen)
            {
                uint32_t Blocklen;
                uint32_t Blocklen_in = VectorIn.DataLen;
                
                /* Automatically block to around 1M blocks. */
                if (Blocklen_in > SBHYBRID_MAX_SIZE_PE_JOB_BLOCKS)
                {
                    Blocklen_in = SBHYBRID_MAX_SIZE_PE_JOB_BLOCKS;
                }

                // !<WW: if you wonder why ? this is because original codes support polling call to determine possible block size
                // and i remove it here but i keep the code structure, nasty..
                Blocklen = Blocklen_in;

                // Decrypt encrypted image
                if (DoDecrypt)
                {
                    /* setup var */
                    tmp_dst_len = Blocklen;

                    res = sfzcrypto_symm_crypt( sfzcrypto_context_get(),
                                                (SfzCryptoCipherContext * const) &aes_ctx,
                                                (SfzCryptoCipherKey * const) &aes_key,
                                                (uint8_t *) VectorIn.Data_p,
                                                Blocklen,
                                                (uint8_t *) VectorOut.Data_p,
                                                (uint32_t * const) &tmp_dst_len,
                                                SFZ_DECRYPT);

                    if ((res != SFZCRYPTO_SUCCESS) ||
                        (Blocklen != tmp_dst_len))
                    {
                        fprintf(stderr,
                                "aes decrypt failed (res=%d)", res);
                        ret = 5;
                        break;
                    }
                }

                ImageLenLeft -= Blocklen; /* Update image length counter. */

                /* hash sha2 on decrypted block content */
                res = sfzcrypto_hash_data( sfzcrypto_context_get(),
                                           (SfzCryptoHashContext * const) &sha_ctx,
                                           (uint8_t *) VectorOut.Data_p,
                                           Blocklen,
                                           false, // init
                                           ImageLenLeft == 0); // final

                if (res != SFZCRYPTO_SUCCESS)
                {
                    fprintf(stderr,
                            "hash image block failed (res=%d)", res);
                    ret = 3;
                    break;
                }

                /* Update data pointers. */
                VectorIn.Data_p += Blocklen / sizeof(VectorIn.Data_p[0]);
                VectorIn.DataLen -= Blocklen;
                VectorOut.Data_p += Blocklen / sizeof(VectorOut.Data_p[0]);
                VectorOut.DataLen -= Blocklen;

                #if defined(ICC_IMAGE)
                if (icc_info.chunk_mode   &&
                    VectorIn.DataLen == 0 &&
                    ImageLenLeft )
                {
                    uint8_t lastchunk = 0;
                    /* need to request more chunk, still hv leftover*/

                    /* reply ok */
                    if (icc_reply(1)) {// 0 = fail, 1 = success
                        fprintf(stderr,
                            "icc reply fail\n");
                        ret = 7;
                        break;
                    }

                    /* get more chunk */
                    if ( icc_recv_chunk( (uint8_t**)(&VectorIn.Data_p),
                                         &VectorIn.DataLen,
                                         &lastchunk)) {
                        fprintf(stderr, "fail to receive chunk \n");
                        ret = 8;
                        break;
                    }

                    /* currently can still safely assume output is in place or same buffer as input */
                    VectorOut = VectorIn;

                    if (lastchunk){
                        icc_info.chunk_mode = 0;
                    }
                }
                #endif /* defined(ICC_IMAGE) */
				
            }
        }

        if (ret == 5 &&
            DoDecrypt &&
            res != SFZCRYPTO_SUCCESS)
        {
            fprintf(stderr, "Image decrypt error");
            return ret;
        }

        if (ret == 3 &&
            res != SFZCRYPTO_SUCCESS)
        {
            fprintf(stderr, "Image hash error");
            return ret;
        }

         // check all bytes were consumed.
        if (ret == 0 && ImageLenLeft != 0)
        {
            fprintf(stderr, "Image hash not all bytes consumed");
            return ret;
        }

        if (0 == ret)
        {
            // successfully calculated the atribute + image sha2 digest
            memcpy((void *)Context_p->SymmContext.Digest, (const void*)sha_ctx.digest, (size_t)SBIF_ECDSA_BYTES);

            // do ecdsa processing ?
            #ifdef ECDSA_SW
            do
            {
                /* Continue Ecdsa processing. */
                ecdsa_res = SBHYBRID_Do_Asym_RunFsm(Context_p, Header_p);
                if (ecdsa_res == SFZCRYPTO_SUCCESS)
                {
                    fprintf(stderr, "Image signature verify succcess");
                    ret = 0;
                    break;
                }
				else if ( ecdsa_res != SFZCRYPTO_PENDING)
				{
					fprintf(stderr, "Image signature verify failed (res=%d)", ecdsa_res);
                    ret = 6;
                    break;
				}
            }
            while (ecdsa_res == SFZCRYPTO_PENDING);
            #endif /* ECDSA_SW */
        }
    }

    return ret;
}

int BLW_Image_DecryptWithKeyAndVerify(
    SB_StorageArea_t * const             Storage_p,
    const SBIF_ECDSA_PublicKey_t * const PublicKey_p,
    const SBIF_ECDSA_Header_t *          Header_p,
    const SBIF_SGVector_t *              DataVectorsInput_p,
    const SBIF_SGVector_t *              DataVectorsOutput_p)
{
     /* This function processes BLe/BLw images into BLp using SBIF Tools.
       After conversion, standard SB_ECDSA_Image_Verify is used.
       In case of BLp image, optional copying takes place, but nothing else. */

    /* Struct for temporary BLTp header to pass to SB_ECDSA_Image_Verify. */
    typedef struct
    {
        SBIF_ECDSA_Header_t      Header;
#if SBLIB_CFG_CERTIFICATES_MAX > 0
        SBIF_ECDSA_Certificate_t Certificates[SBLIB_CFG_CERTIFICATES_MAX];
#endif /* SBLIB_CFG_CERTIFICATES_MAX > 0 */
    } sbifHeaderStorage_t;

    /* Use temporary local buffer. */
    sbifHeaderStorage_t   NewHeaderStorage;
    sbifHeaderStorage_t * NewHeader_p = &NewHeaderStorage;
    uint32_t              image_type;
    int                   sbiftcres, sbres;
    size_t                newHeaderSize = sizeof(sbifHeaderStorage_t);

    image_type = Load_BE32(&Header_p->Type);
    image_type = SBIF_TYPE_TYPE(image_type);

    if (image_type != SBIF_IMAGE_BLTw)
        return -1;

    /* pointless */
    memset(Storage_p, 0, sizeof(SB_StorageArea_t));

    /* For encrypted image, we process it via
       SBIFTOOLS_Image_Convert. */
    /* this convert to new blp type header and get u the kimg  */
    sbiftcres = SBIFTOOLS_Image_ConvertHeader(Header_p,
                                              SBIF_IMAGE_BLTp,
                                              SBIF_VERSION,
                                              &(NewHeader_p->Header),
                                              &newHeaderSize,
                                              (void * const)Storage_p);

    if (sbiftcres != 0)
    {
        fprintf(stderr, "fail to Convert to Blp Header Error: %d\n", sbiftcres);
        return sbiftcres;
    }

    /* decrypt + verify Cert */
    sbres = SB_ECDSA_Image_CopyOrDecrypt_Verify(Storage_p,
                                                PublicKey_p,
                                                &(NewHeader_p->Header),
                                                DataVectorsInput_p,
                                                DataVectorsOutput_p);

    if (sbres != 0)
    {
        fprintf(stderr, "fail to Decrypt or Verify Image Error: %d\n", sbres);
        return sbres;
    }

    return 0;
}
/*
 * Public Functions
*/

int main( int argc, char **argv )
{
    SfzCryptoStatus       result;
    unsigned int          ImageSize;     // as read from file
    uint8_t*              AllocatedInputBuffer_p = NULL;
    SBIF_ECDSA_Header_t * Header_p = NULL;
    int                   ret = 1;
    SBIF_ECDSA_PublicKey_t PublicKey;
    #ifdef ICC_IMAGE
    uint8_t*              IccInput_p = NULL;
    #endif /* ICC_IMAGE */

    /* init some local vars, safety get it ? */;
    result = SFZCRYPTO_INVALID_PARAMETER;

    /* argument check */
    #ifdef ICC_IMAGE
    if( argc > 2 )
    #else /* ICC_IMAGE */
    if( argc < 2 )
    #endif /* ICC_IMAGE */
    {
        usage(argv[0]);
        exit(1);
    }

    /* init eip-123 ddk lib */
    result = sfzcrypto_init(sfzcrypto_context_get());

    if( SFZCRYPTO_SUCCESS != result )
    {
        fprintf(stderr, "fail to initialise SafeZone Crypto Error: %d\n", result);
        exit(1);
    }

    #ifdef ICC_IMAGE
    /* if user supply an argument it mean it is from file else we gonna read from icc */
    if( argc == 2 && argv[1] )
    #endif /* ICC_IMAGE */
    // load the image
    {
        FILE * ImageFile;

        ImageFile = fopen(argv[1], "rb");
        if (!ImageFile)
        {
            fprintf(stderr, "SBSIM: error opening image file\n");
            exit(1);
        }

        // get fie size
        fseek(ImageFile, 0, SEEK_END); // seek to end of file
        ImageSize = (unsigned int)ftell(ImageFile); // get current file pointer
        fseek(ImageFile, 0, SEEK_SET); // seek back to beginning of fil

        // allocate the input buffer
        AllocatedInputBuffer_p = malloc(ImageSize);
        if (!AllocatedInputBuffer_p)
        {
            fprintf(stderr, "SBSIM: error allocating memory\n");
            exit(2);
        }

        //  try to allocate an exact-size buffer
        ImageSize = fread(
                        AllocatedInputBuffer_p,
                        1, ImageSize,
                        ImageFile);

        if (ferror(ImageFile))
        {
            fprintf(stderr, "SBSIM: error reading image file\n");
            ret = 1;
            goto free_and_abort;
        }

        // other check, min size of image etc
        if (ImageSize < sizeof(SBIF_ECDSA_Header_t))
        {
            fprintf(stderr, "SBSIM: image file shorter than header\n");
            ret = 1;
            goto free_and_abort;
        }

        fclose(ImageFile);
    }// end load the image
    #ifdef ICC_IMAGE
    /* if user supply an argument it mean it is from file else we gonna read from icc */
    else
    {
        uint32_t ret = 1;

        printf("WAITING FOR IMAGE from ICC\n");
        ret = icc_loadimage(&IccInput_p, &ImageSize, &icc_info.chunk_mode);

        if( 0 != ret )
        {
            fprintf(stderr, "fail to retrive Image from ICC: %d\n", ret);
        }

        // other check, min size of image etc
        if (ImageSize < sizeof(SBIF_ECDSA_Header_t))
        {
            fprintf(stderr, "SBSIM: image file shorter than header\n");
            ret = 1;
            goto free_and_abort;
        }

        AllocatedInputBuffer_p = IccInput_p;
    }
    #endif /* ICC_IMAGE */

    /* print image info */
    Header_p = (SBIF_ECDSA_Header_t *)AllocatedInputBuffer_p;
    Print_ImageInfo(Header_p);

    // load the public key
    /* !<WW: only support read from nvm public asset index 8 */
    {
        uint32_t PublicKey_len = sizeof(PublicKey);

        result = sfzcrypto_nvm_publicdata_read( sfzcrypto_context_get(),
                                                SBLIB_CFG_CM_CHIP_MANUFACTURER_PUBLIC_KEY,
                                                (SfzCryptoOctetsOut *) &PublicKey,
                                                (uint32_t * const) &PublicKey_len);

        if( (SFZCRYPTO_SUCCESS != result) || (PublicKey_len != sizeof(PublicKey)) )
        {
            fprintf(stderr, "fail to retrive Public Key Error: %d\n", result);
            ret = 1;
            goto free_and_abort;
        }
    }
    Print_EcdsaPublicKey("Public key input", &PublicKey);

    /* do the secure boot [decrypt+] verify */
    {
        SBIF_SGVector_t    DataVectorsInput;    // 1 contiguous block only
        SBIF_SGVector_t    DataVectorsOutput;
        const unsigned int HeaderLen = SBIF_ECDSA_GET_HEADER_SIZE(Header_p, ImageSize);
        const unsigned int DataLen   = ImageSize - HeaderLen;

        // Check for overflows in image size calculation
        if (HeaderLen == 0 ||
            HeaderLen > ImageSize ||
            DataLen > ImageSize ||
            DataLen + HeaderLen > ImageSize)
        {
            fprintf(stderr, "SBSIM: invalid image header\n");
            ret = 1;
            goto free_and_abort;
        }

        #if defined(ICC_IMAGE)
        /* more safety */
        if ( (Load_BE32(&Header_p->ImageLen) + HeaderLen) < ImageSize){
            fprintf(stderr, "SBSIM: size less or mismatch %u < %u\n", (Load_BE32(&Header_p->ImageLen) + HeaderLen), ImageSize);
            ret = 1;
            goto free_and_abort;
        }
        #endif /* defined(ICC_IMAGE) */

        DataVectorsInput.Data_p  = (void *)(AllocatedInputBuffer_p + HeaderLen);
        DataVectorsInput.DataLen = DataLen;

        /* output is in place or same buffer as input */
        DataVectorsOutput.Data_p  = DataVectorsInput.Data_p;
        DataVectorsOutput.DataLen = DataLen;

        memset(&StorageArea, 0x55, sizeof(StorageArea));

        /* verification and decrypt */
        ret = BLW_Image_DecryptWithKeyAndVerify(
                    &StorageArea,
                    &PublicKey,
                    Header_p,
                    &DataVectorsInput,
                    &DataVectorsOutput);

        if (ret == 0)
        {
            printf("Result: PASS\n");
        }
        else
        {
            printf("Result: FAIL\n");
        }

        #ifdef DBG_DUMP // for dump
        //if( 0 == ret )
        {
            FILE * Outf_p = fopen("/tmp/output.bin", "wb");
            if (Outf_p)
            {
                printf("Writing verified image to output.bin... \n");
                fflush(stdout);
                if (0 == fwrite(DataVectorsOutput.Data_p,
                                DataLen, 1,
                                Outf_p))
                {
                    printf("Failed\n");
                }
                else
                {
                    printf("SUCCESS\n");
                }

                fclose(Outf_p);
            }
        }

        #ifdef DBG_DUMP
        dump((AES_IF_Ctx_Ptr_t)&StorageArea);
        #endif /* DBG_DUMP */

        #endif /* DBG_DUMP */
    } /* END do the secure boot [decrypt+] verify */

    free_and_abort:
    /* be a nice guy, free the malloc */
    #ifdef ICC_IMAGE //!<WW: cant diff between from ICC or malloc, let it be for now
    if(IccInput_p){
        icc_reply((uint8_t)(!ret)); // 0 = fail, 1 = success
        icc_freeimage(AllocatedInputBuffer_p, ImageSize);
        IccInput_p = NULL;}
    else
    #endif /* ICC_IMAGE */
    free(AllocatedInputBuffer_p);
    return ret;
}
