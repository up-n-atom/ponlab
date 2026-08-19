#!/bin/sh
set -e  # Exit on any error

MODULE="/usr/lib/libopencryptoki.so"
PIN="12345678"
SO_PIN="87654321"

echo "Starting PKCS11 Sanity Test For EC Keys..."
echo "=================================="

if pkcs11-tool --module "$MODULE" --init-token --label MXL --so-pin "$SO_PIN" --init-pin --pin "$PIN";  then
    echo "Initialize Token Passed"
else
    echo "Initialize Token Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" --label MXL --login --so-pin 87654321 --init-pin --pin "$PIN"; then
    echo "Initialize User PIN Passed"
else
    echo "Initialize User PIN Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" -l --keypairgen --key-type EC:prime256v1 --label EC256Key -e 10 -y privkey -d 10 --pin "$PIN" ; then
    echo "EC Keypair:prime256v1 Generation Passed"
else
    echo "EC Keypair:prime256v1 Generation Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" -l --keypairgen --key-type EC:prime384v1 --label EC384Key -e 11 -y privkey -d 11 --pin "$PIN" ; then
    echo "EC Keypair:prime384v1 Generation Passed"
else
    echo "EC Keypair:prime384v1 Generation Failed"
    exit 1
fi

echo "Hello World" > bar.txt

if pkcs11-tool --module "$MODULE" --sign --label EC256Key -y privkey -d 10 --input-file bar.txt --output-file bar.txt.sign -l --mechanism ECDSA-SHA256 --pin "$PIN" ; then
    echo "EC Key:prime256v1 Signing using ECDSA-SHA256 Passed"
else
    echo "EC Key:prime256v1 Signing using ECDSA-SHA256 Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" --verify --label EC256Key -y pubkey -d 10 --input-file bar.txt --signature-file bar.txt.sign --mechanism ECDSA-SHA256 ; then
    echo "EC Key:prime256v1 Verify using ECDSA-SHA256 Passed"
else
    echo "EC Key:prime256v1 Verify using ECDSA-SHA256 Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" --sign --label EC384Key -y privkey -d 11 --input-file bar.txt --output-file bar.txt.sign -l --mechanism ECDSA-SHA256 --pin "$PIN" ; then
    echo "EC Key:prime384v1 Signing using ECDSA-SHA256 Passed"
else
    echo "EC Key:prime384v1 Signing using ECDSA-SHA256 Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" --verify --label EC384Key -y pubkey -d 11 --input-file bar.txt --signature-file bar.txt.sign --mechanism ECDSA-SHA256 ; then
    echo "EC Key:prime384v1 Verify using ECDSA-SHA256 Passed"
else
    echo "EC Key:prime384v1 Verify using ECDSA-SHA256 Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" --sign --label EC384Key -y privkey -d 11 --input-file bar.txt --output-file bar.txt.sign -l --mechanism ECDSA-SHA384 --pin "$PIN" ; then
    echo "EC Key:prime384v1 Signing using ECDSA-SHA384 Passed"
else
    echo "EC Key:prime384v1 Signing using ECDSA-SHA384 Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" --verify --label EC384Key -y pubkey -d 11 --input-file bar.txt --signature-file bar.txt.sign --mechanism ECDSA-SHA384 ; then
    echo "EC Key:prime384v1 Verify using ECDSA-SHA384 Passed"
else
    echo "EC Key:prime384v1 Verify using ECDSA-SHA384 Failed"
    exit 1
fi

openssl dgst -sha256 -binary -out hash.bin bar.txt

if pkcs11-tool --module "$MODULE" --sign --label EC256Key -y privkey -d 10 --input-file hash.bin --output-file hash.sign -l --mechanism ECDSA --pin "$PIN" ; then
    echo "EC Key:prime256v1 Signing using ECDSA Passed"
else
    echo "EC Key:prime256v1 Signing using ECDSA Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" --verify --label EC256Key -y pubkey -d 10 --input-file hash.bin --signature-file hash.sign --mechanism ECDSA ; then
    echo "EC Key:prime256v1 Verify using ECDSA Passed"
else
    echo "EC Key:prime256v1 Verify using ECDSA Failed"
    exit 1
fi

openssl dgst -sha384 -binary -out hash.bin bar.txt

if pkcs11-tool --module "$MODULE" --sign --label EC384Key -y privkey -d 11 --input-file hash.bin --output-file hash.sign -l --mechanism ECDSA --pin "$PIN" ; then
    echo "EC Key:prime384v1 Signing using ECDSA Passed"
else
    echo "EC Key:prime384v1 Signing using ECDSA Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" --verify --label EC384Key -y pubkey -d 11 --input-file hash.bin --signature-file hash.sign --mechanism ECDSA ; then
    echo "EC Key:prime384v1 Verify using ECDSA Passed"
else
    echo "EC Key:prime384v1 Verify using ECDSA Failed"
    exit 1
fi

if openssl dgst -sha256 -sign "pkcs11:object=EC256Key;type=private" -out bar.txt.sign bar.txt ;  then
    echo "EC Key:prime256v1 Openssl sha256 Signing Passed"
else
    echo "EC Key:prime256v1 Openssl sha256 Signing Failed"
    exit 1
fi

if openssl dgst -sha256 -verify "pkcs11:object=EC256Key;type=public" -signature bar.txt.sign bar.txt ;  then
    echo "EC Key:prime256v1 Openssl sha256 Verify Passed"
else
    echo "EC Key:prime256v1 Openssl sha256 Verify Failed"
    exit 1
fi

if openssl dgst -sha256 -sign "pkcs11:object=EC384Key;type=private" -out bar.txt.sign bar.txt ;  then
    echo "EC Key:prime384v1 Openssl sha256 Signing Passed"
else
    echo "EC Key:prime384v1 Openssl sha256 Signing Failed"
    exit 1
fi

if openssl dgst -sha256 -verify "pkcs11:object=EC384Key;type=public" -signature bar.txt.sign bar.txt ;  then
    echo "EC Key:prime384v1 Openssl sha256 Verify Passed"
else
    echo "EC Key:prime384v1 Openssl sha256 Verify Failed"
    exit 1
fi

if openssl dgst -sha384 -sign "pkcs11:object=EC384Key;type=private" -out bar.txt.sign bar.txt ;  then
    echo "EC Key:prime384v1 Openssl sha256 Signing Passed"
else
    echo "EC Key:prime384v1 Openssl sha256 Signing Failed"
    exit 1
fi

if openssl dgst -sha384 -verify "pkcs11:object=EC384Key;type=public" -signature bar.txt.sign bar.txt ;  then
    echo "EC Key:prime384v1 Openssl sha384 Verify Passed"
else
    echo "EC Key:prime384v1 Openssl sha384 Verify Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" -b --type privkey --label EC256Key -l --pin "$PIN" ; then
    echo "EC Key:prime256v1 Delete Private Key Passed"
else
    echo "EC Key:prime256v1 Delete Private Key Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" -b --type pubkey --label EC256Key  ; then
    echo "EC Key:prime256v1 Delete Public Key Passed"
else
    echo "EC Key:prime256v1 Delete Public Key Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" -b --type privkey --label EC384Key -l --pin "$PIN" ; then
    echo "EC Key:prime256v1 Delete Private Key Passed"
else
    echo "EC Key:prime256v1 Delete Private Key Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" -b --type pubkey --label EC384Key  ; then
    echo "EC Key:prime384v1 Delete Public Key Passed"
else
    echo "EC Key:prime384v1 Delete Public Key Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" --init-token --label MXL --so-pin "$SO_PIN" --init-pin --pin "$PIN";  then
    echo "Initialize Token Passed"
else
    echo "Initialize Token Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" --label MXL --login --so-pin 87654321 --init-pin --pin "$PIN"; then
    echo "Initialize User PIN Passed"
else
    echo "Initialize User PIN Failed"
    exit 1
fi

openssl ecparam -genkey -name prime256v1 -noout -out ec_private-256.pem
openssl ec -in ec_private-256.pem -pubout -out ec_public-256.pem
		
openssl ecparam -name secp384r1 -genkey -noout -out ec_private-384.pem
openssl ec -in ec_private-384.pem -pubout -out ec_public-384.pem

if pkcs11-tool --module "$MODULE" -l --write-object ec_private-256.pem --type privkey --id 20 --label EC256Key --pin "$PIN" ; then
    echo "EC Key:prime256v1 Import Private Key Passed"
else
    echo "EC Key:prime256v1 Import Private Key Failed"
    exit 1
fi
if pkcs11-tool --module "$MODULE" -l --write-object ec_public-256.pem --type pubkey --id 20 --label EC256Key --pin "$PIN" ; then
    echo "EC Key:prime256v1 Import Public Key Passed"
else
    echo "EC Key:prime256v1 Import Public Key Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" -l --write-object ec_private-384.pem --type privkey --id 21 --label EC384Key --pin "$PIN" ; then
    echo "EC Key:prime384v1 Import Private Key Passed"
else
    echo "EC Key:prime384v1 Import Private Key Failed"
    exit 1
fi
if pkcs11-tool --module "$MODULE" -l --write-object ec_public-384.pem --type pubkey --id 21 --label EC384Key --pin "$PIN" ; then
    echo "EC Key:prime384v1 Import Public Key Passed"
else
    echo "EC Key:prime384v1 Import Public Key Failed"
    exit 1
fi

rm -f ec_*

if pkcs11-tool --module "$MODULE" --sign --label EC256Key -y privkey -d 20 --input-file bar.txt --output-file bar.txt.sign -l --mechanism ECDSA-SHA256 --pin "$PIN" ; then
    echo "EC Key:prime256v1 Signing using ECDSA-SHA256 Passed"
else
    echo "EC Key:prime256v1 Signing using ECDSA-SHA256 Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" --verify --label EC256Key -y pubkey -d 20 --input-file bar.txt --signature-file bar.txt.sign --mechanism ECDSA-SHA256 ; then
    echo "EC Key:prime256v1 Verify using ECDSA-SHA256 Passed"
else
    echo "EC Key:prime256v1 Verify using ECDSA-SHA256 Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" --sign --label EC384Key -y privkey -d 21 --input-file bar.txt --output-file bar.txt.sign -l --mechanism ECDSA-SHA256 --pin "$PIN" ; then
    echo "EC Key:prime384v1 Signing using ECDSA-SHA256 Passed"
else
    echo "EC Key:prime384v1 Signing using ECDSA-SHA256 Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" --verify --label EC384Key -y pubkey -d 21 --input-file bar.txt --signature-file bar.txt.sign --mechanism ECDSA-SHA256 ; then
    echo "EC Key:prime384v1 Verify using ECDSA-SHA256 Passed"
else
    echo "EC Key:prime384v1 Verify using ECDSA-SHA256 Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" --sign --label EC384Key -y privkey -d 21 --input-file bar.txt --output-file bar.txt.sign -l --mechanism ECDSA-SHA384 --pin "$PIN" ; then
    echo "EC Key:prime384v1 Signing using ECDSA-SHA384 Passed"
else
    echo "EC Key:prime384v1 Signing using ECDSA-SHA384 Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" --verify --label EC384Key -y pubkey -d 21 --input-file bar.txt --signature-file bar.txt.sign --mechanism ECDSA-SHA384 ; then
    echo "EC Key:prime384v1 Verify using ECDSA-SHA384 Passed"
else
    echo "EC Key:prime384v1 Verify using ECDSA-SHA384 Failed"
    exit 1
fi

openssl dgst -sha256 -binary -out hash.bin bar.txt

if pkcs11-tool --module "$MODULE" --sign --label EC256Key -y privkey -d 20 --input-file hash.bin --output-file hash.sign -l --mechanism ECDSA --pin "$PIN" ; then
    echo "EC Key:prime256v1 Signing using ECDSA Passed"
else
    echo "EC Key:prime256v1 Signing using ECDSA Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" --verify --label EC256Key -y pubkey -d 20 --input-file hash.bin --signature-file hash.sign --mechanism ECDSA ; then
    echo "EC Key:prime256v1 Verify using ECDSA Passed"
else
    echo "EC Key:prime256v1 Verify using ECDSA Failed"
    exit 1
fi

openssl dgst -sha384 -binary -out hash.bin bar.txt

if pkcs11-tool --module "$MODULE" --sign --label EC384Key -y privkey -d 21 --input-file hash.bin --output-file hash.sign -l --mechanism ECDSA --pin "$PIN" ; then
    echo "EC Key:prime384v1 Signing using ECDSA Passed"
else
    echo "EC Key:prime384v1 Signing using ECDSA Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" --verify --label EC384Key -y pubkey -d 21 --input-file hash.bin --signature-file hash.sign --mechanism ECDSA ; then
    echo "EC Key:prime384v1 Verify using ECDSA Passed"
else
    echo "EC Key:prime384v1 Verify using ECDSA Failed"
    exit 1
fi

if openssl dgst -sha256 -sign "pkcs11:object=EC256Key;type=private" -out bar.txt.sign bar.txt ;  then
    echo "EC Key:prime256v1 Openssl sha256 Signing Passed"
else
    echo "EC Key:prime256v1 Openssl sha256 Signing Failed"
    exit 1
fi

if openssl dgst -sha256 -verify "pkcs11:object=EC256Key;type=public" -signature bar.txt.sign bar.txt ;  then
    echo "EC Key:prime256v1 Openssl sha256 Verify Passed"
else
    echo "EC Key:prime256v1 Openssl sha256 Verify Failed"
    exit 1
fi

if openssl dgst -sha256 -sign "pkcs11:object=EC384Key;type=private" -out bar.txt.sign bar.txt ;  then
    echo "EC Key:prime384v1 Openssl sha256 Signing Passed"
else
    echo "EC Key:prime384v1 Openssl sha256 Signing Failed"
    exit 1
fi

if openssl dgst -sha256 -verify "pkcs11:object=EC384Key;type=public" -signature bar.txt.sign bar.txt ;  then
    echo "EC Key:prime384v1 Openssl sha256 Verify Passed"
else
    echo "EC Key:prime384v1 Openssl sha256 Verify Failed"
    exit 1
fi

if openssl dgst -sha384 -sign "pkcs11:object=EC384Key;type=private" -out bar.txt.sign bar.txt ;  then
    echo "EC Key:prime384v1 Openssl sha256 Signing Passed"
else
    echo "EC Key:prime384v1 Openssl sha256 Signing Failed"
    exit 1
fi

if openssl dgst -sha384 -verify "pkcs11:object=EC384Key;type=public" -signature bar.txt.sign bar.txt ;  then
    echo "EC Key:prime384v1 Openssl sha384 Verify Passed"
else
    echo "EC Key:prime384v1 Openssl sha384 Verify Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" -b --type privkey --label EC256Key -l --pin "$PIN" ; then
    echo "EC Key:prime256v1 Delete Private Key Passed"
else
    echo "EC Key:prime256v1 Delete Private Key Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" -b --type pubkey --label EC256Key  ; then
    echo "EC Key:prime256v1 Delete Public Key Passed"
else
    echo "EC Key:prime256v1 Delete Public Key Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" -b --type privkey --label EC384Key -l --pin "$PIN" ; then
    echo "EC Key:prime256v1 Delete Private Key Passed"
else
    echo "EC Key:prime256v1 Delete Private Key Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" -b --type pubkey --label EC384Key  ; then
    echo "EC Key:prime384v1 Delete Public Key Passed"
else
    echo "EC Key:prime384v1 Delete Public Key Failed"
    exit 1
fi

echo "=================================="
echo "Starting PKCS11 Sanity Test For RSA Keys..."
echo "=================================="

openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:1024 -out rsa1024_private.pem
openssl pkey -in rsa1024_private.pem -pubout -out rsa1024_public.pem

openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:2048 -out rsa2048_private.pem
openssl pkey -in rsa2048_private.pem -pubout -out rsa2048_public.pem

openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:3072 -out rsa3072_private.pem
openssl pkey -in rsa3072_private.pem -pubout -out rsa3072_public.pem

if pkcs11-tool --module "$MODULE" --init-token --label MXL --so-pin "$SO_PIN" --init-pin --pin "$PIN"; then
    echo "Initialize Token Passed"
else
    echo "Initialize Token Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" --label MXL --login --so-pin 87654321 --init-pin --pin "$PIN"; then
    echo "Initialize User PIN Passed"
else
    echo "Initialize User PIN Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" -l --write-object rsa1024_private.pem --type privkey --id 1 --label rsa1024 --pin "$PIN" ; then
    echo "RSA 1024 Import Private Key Passed"
else
    echo "RSA 1024 Import Private Key Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" -l --write-object rsa1024_public.pem --type pubkey --id 1 --label rsa1024 --pin "$PIN" ; then
    echo "RSA 1024 Import Public Key Passed"
else
    echo "RSA 1024 Import Public Key Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" -l --write-object rsa2048_private.pem --type privkey --id 2 --label rsa2048 --pin "$PIN" ; then
    echo "RSA 2048 Import Private Key Passed"
else
    echo "RSA 2048 Import Private Key Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" -l --write-object rsa2048_public.pem --type pubkey --id 2 --label rsa2048 --pin "$PIN" ; then
    echo "RSA 2048 Import Public Key Passed"
else
    echo "RSA 2048 Import Public Key Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" -l --write-object rsa3072_private.pem --type privkey --id 3 --label rsa3072 --pin "$PIN" ; then
    echo "RSA 3072 Import Private Key Passed"
else
    echo "RSA 3072 Import Private Key Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" -l --write-object rsa3072_public.pem --type pubkey --id 3 --label rsa3072 --pin "$PIN" ; then
    echo "RSA 3072 Import Public Key Passed"
else
    echo "RSA 3072 Import Public Key Failed"
    exit 1
fi

openssl dgst -sha1 -binary -out hash.bin bar.txt

if openssl pkeyutl -sign -inkey "pkcs11:object=rsa2048;type=private" -in hash.bin -out signature.bin --pkeyopt digest:sha1 -pkeyopt rsa_padding_mode:pkcs1 ; then
    echo "RSA 2048: Openssl Sign With SHA1 PKCS1.5 Padding Passed"
else
    echo "RSA 2048: Openssl Sign With SHA1 PKCS1.5 Padding Failed"
    exit 1openssl dgst -sha1 -binary -out hash.bin bar.txt
fi

if openssl pkeyutl -verify -pubin -inkey rsa2048_public.pem -in hash.bin -sigfile signature.bin -pkeyopt digest:sha1 -pkeyopt rsa_padding_mode:pkcs1 ; then
    echo "RSA 2048: Openssl Verify With SHA1 PKCS1.5 Padding Passed"
else
    echo "RSA 2048: Openssl Verify With SHA1 PKCS1.5 Padding Failed"
    exit 1
fi

if openssl pkeyutl -sign -inkey "pkcs11:object=rsa3072;type=private" -in hash.bin -out signature.bin -pkeyopt digest:sha1 -pkeyopt rsa_padding_mode:pkcs1 ; then
    echo "RSA 3072: Openssl Sign With SHA1 PKCS1.5 Padding Passed"
else
    echo "RSA 3072: Openssl Sign With SHA1 PKCS1.5 Padding Failed"
    exit 1
fi

if openssl pkeyutl -verify -pubin -inkey rsa3072_public.pem -in hash.bin -sigfile signature.bin -pkeyopt digest:sha1 -pkeyopt rsa_padding_mode:pkcs1 ; then
    echo "RSA 3072: Openssl Verify With SHA1 PKCS1.5 Padding Passed"
else
    echo "RSA 3072: Openssl Verify With SHA1 PKCS1.5 Padding Failed"
    exit 1
fi

if openssl pkeyutl -sign -engine pkcs11 -keyform engine -inkey "pkcs11:object=rsa2048;type=private" -in hash.bin -out signature.bin -pkeyopt digest:sha1 -pkeyopt rsa_padding_mode:pss ; then
    echo "RSA 2048: Openssl Sign With SHA1 PSS Padding Passed"
else
    echo "RSA 2048: Openssl Sign With SHA1 PSS Padding Failed"
    exit 1
fi

if openssl pkeyutl -verify -pubin -inkey rsa2048_public.pem -in hash.bin -sigfile signature.bin -pkeyopt digest:sha1 -pkeyopt rsa_padding_mode:pss ; then
    echo "RSA 2048: Openssl Verify With SHA1 PSS Padding Passed"
else
    echo "RSA 2048: Openssl Verify With SHA1 PSS Padding Failed"
    exit 1
fi

if openssl pkeyutl -sign -engine pkcs11 -keyform engine -inkey "pkcs11:object=rsa3072;type=private;" -in hash.bin -out signature.bin -pkeyopt digest:sha1 -pkeyopt rsa_padding_mode:pss ; then
    echo "RSA 3072: Openssl Sign With SHA1 PSS Padding Passed"
else
    echo "RSA 3072: Openssl Sign With SHA1 PSS Padding Failed"
    exit 1
fi

if openssl pkeyutl -verify -pubin -inkey rsa3072_public.pem -in hash.bin -sigfile signature.bin -pkeyopt digest:sha1 -pkeyopt rsa_padding_mode:pss ; then
    echo "RSA 3072: Openssl Verify With SHA1 PSS Padding Passed"
else
    echo "RSA 3072: Openssl Sign With SHA1 PSS Padding Failed"
    exit 1
fi

openssl dgst -sha256 -binary -out hash.bin bar.txt

if openssl pkeyutl -sign -inkey "pkcs11:object=rsa2048;type=private" -in hash.bin -out signature.bin -pkeyopt digest:sha256 -pkeyopt rsa_padding_mode:pkcs1 ; then
    echo "RSA 2048: Openssl Sign With SHA256 PKCS1.5 Padding Passed"
else
    echo "RSA 2048: Openssl Sign With SHA256 PKCS1.5 Padding Failed"
    exit 1
fi

if openssl pkeyutl -verify -pubin -inkey rsa2048_public.pem -in hash.bin -sigfile signature.bin -pkeyopt digest:sha256 -pkeyopt rsa_padding_mode:pkcs1 ; then
    echo "RSA 2048: Openssl Verify With SHA256 PKCS1.5 Padding Passed"
else
    echo "RSA 2048: Openssl Verify With SHA256 PKCS1.5 Padding Failed"
    exit 1
fi

if openssl pkeyutl -sign -inkey "pkcs11:object=rsa3072;type=private" -in hash.bin -out signature.bin -pkeyopt digest:sha256 -pkeyopt rsa_padding_mode:pkcs1 ; then
    echo "RSA 3072: Openssl Sign With SHA256 PKCS1.5 Padding Passed"
else
    echo "RSA 3072: Openssl Sign With SHA256 PKCS1.5 Padding Failed"
    exit 1
fi

if openssl pkeyutl -verify -pubin -inkey rsa3072_public.pem -in hash.bin -sigfile signature.bin -pkeyopt digest:sha256 -pkeyopt rsa_padding_mode:pkcs1 ; then
    echo "RSA 3072: Openssl Verify With SHA256 PKCS1.5 Padding Passed"
else
    echo "RSA 3072: Openssl Verify With SHA256 PKCS1.5 Padding Failed"
    exit 1
fi

if openssl pkeyutl -sign -engine pkcs11 -keyform engine -inkey "pkcs11:object=rsa2048;type=private" -in hash.bin -out signature.bin -pkeyopt digest:sha256 -pkeyopt rsa_padding_mode:pss ; then
    echo "RSA 2048: Openssl Sign With SHA256 PSS Padding Passed"
else
    echo "RSA 2048: Openssl Sign With SHA256 PSS Padding Failed"
    exit 1
fi

if openssl pkeyutl -verify -pubin -inkey rsa2048_public.pem -in hash.bin -sigfile signature.bin -pkeyopt digest:sha256 -pkeyopt rsa_padding_mode:pss ; then
    echo "RSA 2048: Openssl Verify With SHA256 PSS Padding Passed"
else
    echo "RSA 2048: Openssl Verify With SHA256 PSS Padding Failed"
    exit 1
fi

if openssl pkeyutl -sign -engine pkcs11 -keyform engine -inkey "pkcs11:object=rsa3072;type=private;" -in hash.bin -out signature.bin -pkeyopt digest:sha256 -pkeyopt rsa_padding_mode:pss ; then
    echo "RSA 3072: Openssl Sign With SHA256 PSS Padding Passed"
else
    echo "RSA 3072: Openssl Sign With SHA256 PSS Padding Failed"
fi

if openssl pkeyutl -verify -pubin -inkey rsa3072_public.pem -in hash.bin -sigfile signature.bin -pkeyopt digest:sha256 -pkeyopt rsa_padding_mode:pss ; then
    echo "RSA 3072: Openssl Verify With SHA256 PSS Padding Passed"
else
    echo "RSA 3072: Openssl Sign With SHA256 PSS Padding Failed"
    exit 1
fi

openssl dgst -sha384 -binary -out hash.bin bar.txt

if openssl pkeyutl -sign -inkey "pkcs11:object=rsa2048;type=private" -in hash.bin -out signature.bin -pkeyopt digest:sha384 -pkeyopt rsa_padding_mode:pkcs1 ; then
    echo "RSA 2048: Openssl Sign With SHA384 PKCS1.5 Padding Passed"
else
    echo "RSA 2048: Openssl Sign With SHA384 PKCS1.5 Padding Failed"
    exit 1
fi

if openssl pkeyutl -verify -pubin -inkey rsa2048_public.pem -in hash.bin -sigfile signature.bin -pkeyopt digest:sha384 -pkeyopt rsa_padding_mode:pkcs1 ; then
    echo "RSA 2048: Openssl Verify With SHA384 PKCS1.5 Padding Passed"
else
    echo "RSA 2048: Openssl Verify With SHA384 PKCS1.5 Padding Failed"
    exit 1
fi

if openssl pkeyutl -sign -inkey "pkcs11:object=rsa3072;type=private" -in hash.bin -out signature.bin -pkeyopt digest:sha384 -pkeyopt rsa_padding_mode:pkcs1 ; then
    echo "RSA 3072: Openssl Sign With SHA384 PKCS1.5 Padding Passed"
else
    echo "RSA 3072: Openssl Sign With SHA384 PKCS1.5 Padding Failed"
    exit 1
fi

if openssl pkeyutl -verify -pubin -inkey rsa3072_public.pem -in hash.bin -sigfile signature.bin -pkeyopt digest:sha384 -pkeyopt rsa_padding_mode:pkcs1 ; then
    echo "RSA 3072: Openssl Verify With SHA384 PKCS1.5 Padding Passed"
else
    echo "RSA 3072: Openssl Verify With SHA384 PKCS1.5 Padding Failed"
    exit 1
fi

if openssl pkeyutl -sign -engine pkcs11 -keyform engine -inkey "pkcs11:object=rsa2048;type=private" -in hash.bin -out signature.bin -pkeyopt digest:sha384 -pkeyopt rsa_padding_mode:pss ; then
    echo "RSA 2048: Openssl Sign With SHA384 PSS Padding Passed"
else
    echo "RSA 2048: Openssl Sign With SHA384 PSS Padding Failed"
    exit 1
fi

if openssl pkeyutl -verify -pubin -inkey rsa2048_public.pem -in hash.bin -sigfile signature.bin -pkeyopt digest:sha384 -pkeyopt rsa_padding_mode:pss ; then
    echo "RSA 2048: Openssl Verify With SHA384 PSS Padding Passed"
else
    echo "RSA 2048: Openssl Verify With SHA384 PSS Padding Failed"
    exit 1
fi

if openssl pkeyutl -sign -engine pkcs11 -keyform engine -inkey "pkcs11:object=rsa3072;type=private;" -in hash.bin -out signature.bin -pkeyopt digest:sha384 -pkeyopt rsa_padding_mode:pss ; then
    echo "RSA 3072: Openssl Sign With SHA384 PSS Padding Passed"
else
    echo "RSA 3072: Openssl Sign With SHA384 PSS Padding Failed"
fi

if openssl pkeyutl -verify -pubin -inkey rsa3072_public.pem -in hash.bin -sigfile signature.bin -pkeyopt digest:sha384 -pkeyopt rsa_padding_mode:pss ; then
    echo "RSA 3072: Openssl Verify With SHA384 PSS Padding Passed"
else
    echo "RSA 3072: Openssl Sign With SHA384 PSS Padding Failed"
    exit 1
fi

openssl dgst -sha512 -binary -out hash.bin bar.txt

if openssl pkeyutl -sign -inkey "pkcs11:object=rsa2048;type=private" -in hash.bin -out signature.bin -pkeyopt digest:sha512 -pkeyopt rsa_padding_mode:pkcs1 ; then
    echo "RSA 2048: Openssl Sign With SHA512 PKCS1.5 Padding Passed"
else
    echo "RSA 2048: Openssl Sign With SHA512 PKCS1.5 Padding Failed"
    exit 1
fi

if openssl pkeyutl -verify -pubin -inkey rsa2048_public.pem -in hash.bin -sigfile signature.bin -pkeyopt digest:sha512 -pkeyopt rsa_padding_mode:pkcs1 ; then
    echo "RSA 2048: Openssl Verify With SHA512 PKCS1.5 Padding Passed"
else
    echo "RSA 2048: Openssl Verify With SHA512 PKCS1.5 Padding Failed"
    exit 1
fi

if openssl pkeyutl -sign -inkey "pkcs11:object=rsa3072;type=private" -in hash.bin -out signature.bin -pkeyopt digest:sha512 -pkeyopt rsa_padding_mode:pkcs1 ; then
    echo "RSA 3072: Openssl Sign With SHA512 PKCS1.5 Padding Passed"
else
    echo "RSA 3072: Openssl Sign With SHA512 PKCS1.5 Padding Failed"
    exit 1
fi

if openssl pkeyutl -verify -pubin -inkey rsa3072_public.pem -in hash.bin -sigfile signature.bin -pkeyopt digest:sha512 -pkeyopt rsa_padding_mode:pkcs1 ; then
    echo "RSA 3072: Openssl Verify With SHA512 PKCS1.5 Padding Passed"
else
    echo "RSA 3072: Openssl Verify With SHA512 PKCS1.5 Padding Failed"
    exit 1
fi

if openssl pkeyutl -sign -engine pkcs11 -keyform engine -inkey "pkcs11:object=rsa2048;type=private" -in hash.bin -out signature.bin -pkeyopt digest:sha512 -pkeyopt rsa_padding_mode:pss ; then
    echo "RSA 2048: Openssl Sign With SHA512 PSS Padding Passed"
else
    echo "RSA 2048: Openssl Sign With SHA512 PSS Padding Failed"
    exit 1
fi

if openssl pkeyutl -verify -pubin -inkey rsa2048_public.pem -in hash.bin -sigfile signature.bin -pkeyopt digest:sha512 -pkeyopt rsa_padding_mode:pss ; then
    echo "RSA 2048: Openssl Verify With SHA512 PSS Padding Passed"
else
    echo "RSA 2048: Openssl Verify With SHA512 PSS Padding Failed"
    exit 1
fi

if openssl pkeyutl -sign -engine pkcs11 -keyform engine -inkey "pkcs11:object=rsa3072;type=private;" -in hash.bin -out signature.bin -pkeyopt digest:sha512 -pkeyopt rsa_padding_mode:pss ; then
    echo "RSA 3072: Openssl Sign With SHA512 PSS Padding Passed"
else
    echo "RSA 3072: Openssl Sign With SHA512 PSS Padding Failed"
fi

if openssl pkeyutl -verify -pubin -inkey rsa3072_public.pem -in hash.bin -sigfile signature.bin -pkeyopt digest:sha512 -pkeyopt rsa_padding_mode:pss ; then
    echo "RSA 3072: Openssl Verify With SHA512 PSS Padding Passed"
else
    echo "RSA 3072: Openssl Sign With SHA512 PSS Padding Failed"
    exit 1
fi

if openssl pkeyutl -encrypt -pubin -inkey rsa1024_public.pem -in bar.txt -out bar_encrypted.bin -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha1 -pkeyopt rsa_mgf1_md:sha1 ; then
    echo "RSA 1024: Openssl Encrypt With SHA1 OAEP Padding Passed"
else
    echo "RSA 1024: Openssl Encrypt With SHA1 OAEP Padding Failed"
    exit 1
fi

if openssl pkeyutl -decrypt -engine pkcs11 -keyform engine -in bar_encrypted.bin -out bar_decrypted.bin -inkey "pkcs11:object=rsa1024;type=private" -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha1 -pkeyopt rsa_mgf1_md:sha1 ; then
    echo "RSA 1024: Openssl Decrypt With SHA1 OAEP Padding Passed"
else
    echo "RSA 1024: Openssl Decrypt With SHA1 OAEP Padding Failed"
    exit 1
fi


if cmp -s bar.txt bar_decrypted.bin; then
    echo "RSA 1024: Openssl Decrypt With SHA1 OAEP Padding Verified"
else
    echo "RSA 1024: Openssl Decrypt With SHA1 OAEP Padding Not Verified"
    exit 1
fi

if openssl pkeyutl -encrypt -pubin -inkey rsa1024_public.pem -in bar.txt -out bar_encrypted.bin -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha224 -pkeyopt rsa_mgf1_md:sha224 ; then
    echo "RSA 1024: Openssl Encrypt With SHA224 OAEP Padding Passed"
else
    echo "RSA 1024: Openssl Encrypt With SHA224 OAEP Padding Failed"
    exit 1
fi

if openssl pkeyutl -decrypt -engine pkcs11 -keyform engine -in bar_encrypted.bin -out bar_decrypted.bin -inkey "pkcs11:object=rsa1024;type=private" -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha224 -pkeyopt rsa_mgf1_md:sha224 ; then
    echo "RSA 1024: Openssl Decrypt With SHA224 OAEP Padding Passed"
else
    echo "RSA 1024: Openssl Decrypt With SHA224 OAEP Padding Failed"
    exit 1
fi


if cmp -s bar.txt bar_decrypted.bin; then
    echo "RSA 1024: Openssl Decrypt With SHA224 OAEP Padding Verified"
else
    echo "RSA 1024: Openssl Decrypt With SHA224 OAEP Padding Not Verified"
    exit 1
fi

if openssl pkeyutl -encrypt -pubin -inkey rsa1024_public.pem -in bar.txt -out bar_encrypted.bin -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha256 -pkeyopt rsa_mgf1_md:sha256 ; then
    echo "RSA 1024: Openssl Encrypt With SHA256 OAEP Padding Passed"
else
    echo "RSA 1024: Openssl Encrypt With SHA256 OAEP Padding Failed"
    exit 1
fi

if openssl pkeyutl -decrypt -engine pkcs11 -keyform engine -in bar_encrypted.bin -out bar_decrypted.bin -inkey "pkcs11:object=rsa1024;type=private" -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha256 -pkeyopt rsa_mgf1_md:sha256 ; then
    echo "RSA 1024: Openssl Decrypt With SHA256 OAEP Padding Passed"
else
    echo "RSA 1024: Openssl Decrypt With SHA256 OAEP Padding Failed"
    exit 1
fi


if cmp -s bar.txt bar_decrypted.bin; then
    echo "RSA 1024: Openssl Decrypt With SHA256 OAEP Padding Verified"
else
    echo "RSA 1024: Openssl Decrypt With SHA256 OAEP Padding Not Verified"
    exit 1
fi

if openssl pkeyutl -encrypt -pubin -inkey rsa1024_public.pem -in bar.txt -out bar_encrypted.bin -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha384 -pkeyopt rsa_mgf1_md:sha384 ; then
    echo "RSA 1024: Openssl Encrypt With SHA384 OAEP Padding Passed"
else
    echo "RSA 1024: Openssl Encrypt With SHA384 OAEP Padding Failed"
    exit 1
fi

if openssl pkeyutl -decrypt -engine pkcs11 -keyform engine -in bar_encrypted.bin -out bar_decrypted.bin -inkey "pkcs11:object=rsa1024;type=private" -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha384 -pkeyopt rsa_mgf1_md:sha384 ; then
    echo "RSA 1024: Openssl Decrypt With SHA384 OAEP Padding Passed"
else
    echo "RSA 1024: Openssl Decrypt With SHA384 OAEP Padding Failed"
    exit 1
fi


if cmp -s bar.txt bar_decrypted.bin; then
    echo "RSA 1024: Openssl Decrypt With SHA384 OAEP Padding Verified"
else
    echo "RSA 1024: Openssl Decrypt With SHA384 OAEP Padding Not Verified"
    exit 1
fi

if openssl pkeyutl -encrypt -pubin -inkey rsa2048_public.pem -in bar.txt -out bar_encrypted.bin -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha1 -pkeyopt rsa_mgf1_md:sha1 ; then
    echo "RSA 2048: Openssl Encrypt With SHA1 OAEP Padding Passed"
else
    echo "RSA 2048: Openssl Encrypt With SHA1 OAEP Padding Failed"
    exit 1
fi

if openssl pkeyutl -decrypt -engine pkcs11 -keyform engine -in bar_encrypted.bin -out bar_decrypted.bin -inkey "pkcs11:object=rsa2048;type=private" -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha1 -pkeyopt rsa_mgf1_md:sha1 ; then
    echo "RSA 2048: Openssl Decrypt With SHA1 OAEP Padding Passed"
else
    echo "RSA 2048: Openssl Decrypt With SHA1 OAEP Padding Failed"
    exit 1
fi


if cmp -s bar.txt bar_decrypted.bin; then
    echo "RSA 2048: Openssl Decrypt With SHA1 OAEP Padding Verified"

else
    echo "RSA 2048: Openssl Decrypt With SHA1 OAEP Padding Not Verified"
    exit 1
fi

if openssl pkeyutl -encrypt -pubin -inkey rsa2048_public.pem -in bar.txt -out bar_encrypted.bin -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha224 -pkeyopt rsa_mgf1_md:sha224 ; then
    echo "RSA 2048: Openssl Encrypt With SHA224 OAEP Padding Passed"
else
    echo "RSA 2048: Openssl Encrypt With SHA224 OAEP Padding Failed"
    exit 1
fi

if openssl pkeyutl -decrypt -engine pkcs11 -keyform engine -in bar_encrypted.bin -out bar_decrypted.bin -inkey "pkcs11:object=rsa2048;type=private" -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha224 -pkeyopt rsa_mgf1_md:sha224 ; then
    echo "RSA 2048: Openssl Decrypt With SHA224 OAEP Padding Passed"
else
    echo "RSA 2048: Openssl Decrypt With SHA224 OAEP Padding Failed"
    exit 1
fi


if cmp -s bar.txt bar_decrypted.bin; then
    echo "RSA 2048: Openssl Decrypt With SHA224 OAEP Padding Verified"
else
    echo "RSA 2048: Openssl Decrypt With SHA224 OAEP Padding Not Verified"
    exit 1
fi

if openssl pkeyutl -encrypt -pubin -inkey rsa2048_public.pem -in bar.txt -out bar_encrypted.bin -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha256 -pkeyopt rsa_mgf1_md:sha256 ; then
    echo "RSA 2048: Openssl Encrypt With SHA256 OAEP Padding Passed"
else
    echo "RSA 2048: Openssl Encrypt With SHA256 OAEP Padding Failed"
    exit 1
fi

if openssl pkeyutl -decrypt -engine pkcs11 -keyform engine -in bar_encrypted.bin -out bar_decrypted.bin -inkey "pkcs11:object=rsa2048;type=private" -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha256 -pkeyopt rsa_mgf1_md:sha256 ; then
    echo "RSA 2048: Openssl Decrypt With SHA256 OAEP Padding Passed"
else
    echo "RSA 2048: Openssl Decrypt With SHA256 OAEP Padding Failed"
    exit 1
fi


if cmp -s bar.txt bar_decrypted.bin; then
    echo "RSA 2048: Openssl Decrypt With SHA256 OAEP Padding Verified"
else
    echo "RSA 2048: Openssl Decrypt With SHA256 OAEP Padding Not Verified"
    exit 1
fi

if openssl pkeyutl -encrypt -pubin -inkey rsa2048_public.pem -in bar.txt -out bar_encrypted.bin -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha384 -pkeyopt rsa_mgf1_md:sha384 ; then
    echo "RSA 2048: Openssl Encrypt With SHA384 OAEP Padding Passed"
else
    echo "RSA 2048: Openssl Encrypt With SHA384 OAEP Padding Failed"
    exit 1
fi

if openssl pkeyutl -decrypt -engine pkcs11 -keyform engine -in bar_encrypted.bin -out bar_decrypted.bin -inkey "pkcs11:object=rsa2048;type=private" -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha384 -pkeyopt rsa_mgf1_md:sha384 ; then
    echo "RSA 2048: Openssl Decrypt With SHA384 OAEP Padding Passed"
else
    echo "RSA 2048: Openssl Decrypt With SHA384 OAEP Padding Failed"
    exit 1
fi


if cmp -s bar.txt bar_decrypted.bin; then
    echo "RSA 2048: Openssl Decrypt With SHA384 OAEP Padding Verified"
else
    echo "RSA 2048: Openssl Decrypt With SHA384 OAEP Padding Not Verified"
    exit 1
fi

if openssl pkeyutl -encrypt -pubin -inkey rsa2048_public.pem -in bar.txt -out bar_encrypted.bin -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha512 -pkeyopt rsa_mgf1_md:sha512 ; then
    echo "RSA 2048: Openssl Encrypt With SHA512 OAEP Padding Passed"
else
    echo "RSA 2048: Openssl Encrypt With SHA512 OAEP Padding Failed"
    exit 1
fi

if openssl pkeyutl -decrypt -engine pkcs11 -keyform engine -in bar_encrypted.bin -out bar_decrypted.bin -inkey "pkcs11:object=rsa2048;type=private" -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha512 -pkeyopt rsa_mgf1_md:sha512 ; then
    echo "RSA 2048: Openssl Decrypt With SHA512 OAEP Padding Passed"
else
    echo "RSA 2048: Openssl Decrypt With SHA512 OAEP Padding Failed"
    exit 1
fi


if cmp -s bar.txt bar_decrypted.bin; then
    echo "RSA 2048: Openssl Decrypt With SHA512 OAEP Padding Verified"
else
    echo "RSA 2048: Openssl Decrypt With SHA512 OAEP Padding Not Verified"
    exit 1
fi

if openssl pkeyutl -encrypt -pubin -inkey rsa3072_public.pem -in bar.txt -out bar_encrypted.bin -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha1 -pkeyopt rsa_mgf1_md:sha1 ; then
    echo "RSA 3072: Openssl Encrypt With SHA1 OAEP Padding Passed"
else
    echo "RSA 3072: Openssl Encrypt With SHA1 OAEP Padding Failed"
    exit 1
fi

if openssl pkeyutl -decrypt -engine pkcs11 -keyform engine -in bar_encrypted.bin -out bar_decrypted.bin -inkey "pkcs11:object=rsa3072;type=private" -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha1 -pkeyopt rsa_mgf1_md:sha1 ; then
    echo "RSA 3072: Openssl Decrypt With SHA1 OAEP Padding Passed"
else
    echo "RSA 3072: Openssl Decrypt With SHA1 OAEP Padding Failed"
    exit 1
fi


if cmp -s bar.txt bar_decrypted.bin; then
    echo "RSA 3072: Openssl Decrypt With SHA1 OAEP Padding Verified"
else
    echo "RSA 3072: Openssl Decrypt With SHA1 OAEP Padding Not Verified"
    exit 1
fi

if openssl pkeyutl -encrypt -pubin -inkey rsa3072_public.pem -in bar.txt -out bar_encrypted.bin -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha224 -pkeyopt rsa_mgf1_md:sha224 ; then
    echo "RSA 3072: Openssl Encrypt With SHA224 OAEP Padding Passed"
else
    echo "RSA 3072: Openssl Encrypt With SHA224 OAEP Padding Failed"
    exit 1
fi

if openssl pkeyutl -decrypt -engine pkcs11 -keyform engine -in bar_encrypted.bin -out bar_decrypted.bin -inkey "pkcs11:object=rsa3072;type=private" -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha224 -pkeyopt rsa_mgf1_md:sha224 ; then
    echo "RSA 3072: Openssl Decrypt With SHA224 OAEP Padding Passed"
else
    echo "RSA 3072: Openssl Decrypt With SHA224 OAEP Padding Failed"
    exit 1
fi


if cmp -s bar.txt bar_decrypted.bin; then
    echo "RSA 3072: Openssl Decrypt With SHA224 OAEP Padding Verified"
else
    echo "RSA 3072: Openssl Decrypt With SHA224 OAEP Padding Not Verified"
    exit 1
fi

if openssl pkeyutl -encrypt -pubin -inkey rsa3072_public.pem -in bar.txt -out bar_encrypted.bin -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha256 -pkeyopt rsa_mgf1_md:sha256 ; then
    echo "RSA 3072: Openssl Encrypt With SHA256 OAEP Padding Passed"
else
    echo "RSA 3072: Openssl Encrypt With SHA256 OAEP Padding Failed"
    exit 1
fi

if openssl pkeyutl -decrypt -engine pkcs11 -keyform engine -in bar_encrypted.bin -out bar_decrypted.bin -inkey "pkcs11:object=rsa3072;type=private" -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha256 -pkeyopt rsa_mgf1_md:sha256 ; then
    echo "RSA 3072: Openssl Decrypt With SHA256 OAEP Padding Passed"
else
    echo "RSA 3072: Openssl Decrypt With SHA256 OAEP Padding Failed"
    exit 1
fi


if cmp -s bar.txt bar_decrypted.bin; then
    echo "RSA 3072: Openssl Decrypt With SHA256 OAEP Padding Verified"
else
    echo "RSA 3072: Openssl Decrypt With SHA256 OAEP Padding Not Verified"
    exit 1
fi

if openssl pkeyutl -encrypt -pubin -inkey rsa3072_public.pem -in bar.txt -out bar_encrypted.bin -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha384 -pkeyopt rsa_mgf1_md:sha384 ; then
    echo "RSA 3072: Openssl Encrypt With SHA384 OAEP Padding Passed"
else
    echo "RSA 3072: Openssl Encrypt With SHA384 OAEP Padding Failed"
    exit 1
fi

if openssl pkeyutl -decrypt -engine pkcs11 -keyform engine -in bar_encrypted.bin -out bar_decrypted.bin -inkey "pkcs11:object=rsa3072;type=private" -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha384 -pkeyopt rsa_mgf1_md:sha384 ; then
    echo "RSA 3072: Openssl Decrypt With SHA384 OAEP Padding Passed"
else
    echo "RSA 3072: Openssl Decrypt With SHA384 OAEP Padding Failed"
    exit 1
fi


if cmp -s bar.txt bar_decrypted.bin; then
    echo "RSA 3072: Openssl Decrypt With SHA384 OAEP Padding Verified"
else
    echo "RSA 3072: Openssl Decrypt With SHA384 OAEP Padding Not Verified"
    exit 1
fi

if openssl pkeyutl -encrypt -pubin -inkey rsa3072_public.pem -in bar.txt -out bar_encrypted.bin -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha512 -pkeyopt rsa_mgf1_md:sha512 ; then
    echo "RSA 3072: Openssl Encrypt With SHA512 OAEP Padding Passed"
else
    echo "RSA 3072: Openssl Encrypt With SHA512 OAEP Padding Failed"
    exit 1
fi

if openssl pkeyutl -decrypt -engine pkcs11 -keyform engine -in bar_encrypted.bin -out bar_decrypted.bin -inkey "pkcs11:object=rsa3072;type=private" -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_oaep_md:sha512 -pkeyopt rsa_mgf1_md:sha512 ; then
    echo "RSA 3072: Openssl Decrypt With SHA512 OAEP Padding Passed"
else
    echo "RSA 3072: Openssl Decrypt With SHA512 OAEP Padding Failed"
    exit 1
fi


if cmp -s bar.txt bar_decrypted.bin; then
    echo "RSA 3072: Openssl Decrypt With SHA512 OAEP Padding Verified"
else
    echo "RSA 3072: Openssl Decrypt With SHA512 OAEP Padding Not Verified"
    exit 1
fi

if openssl pkeyutl -encrypt -pubin -inkey rsa1024_public.pem -in bar.txt -out bar_encrypted.bin -pkeyopt rsa_padding_mode:pkcs1 ;  then
    echo "RSA 1024: Openssl Encrypt With PKCS1.5 Padding Passed"
else
    echo "RSA 1024: Openssl Encrypt With PKCS1.5 Padding Failed"
    exit 1
fi

if openssl pkeyutl -decrypt -in bar_encrypted.bin -out bar_decrypted.bin -inkey "pkcs11:object=rsa1024;type=private" -pkeyopt rsa_padding_mode:pkcs1 ; then
    echo "RSA 1024: Openssl Decrypt With PKCS1.5 Padding Passed"
else
    echo "RSA 1024: Openssl Decrypt With PKCS1.5 Padding Failed"
    exit 1
fi

if cmp -s bar.txt bar_decrypted.bin; then
    echo "RSA 1024: Openssl Decrypt With PKCS1.5 Padding Verified"
else
    echo "RSA 1024: Openssl Decrypt With PKCS1.5 Padding Not Verified"
    exit 1
fi

if openssl pkeyutl -encrypt -pubin -inkey rsa2048_public.pem -in bar.txt -out bar_encrypted.bin -pkeyopt rsa_padding_mode:pkcs1 ;  then
    echo "RSA 2048: Openssl Encrypt With PKCS1.5 Padding Passed"
else
    echo "RSA 2048: Openssl Encrypt With PKCS1.5 Padding Failed"
    exit 1
fi

if openssl pkeyutl -decrypt -in bar_encrypted.bin -out bar_decrypted.bin -inkey "pkcs11:object=rsa2048;type=private" -pkeyopt rsa_padding_mode:pkcs1 ; then
    echo "RSA 2048: Openssl Decrypt With PKCS1.5 Padding Passed"
else
    echo "RSA 2048: Openssl Decrypt With PKCS1.5 Padding Failed"
    exit 1
fi

if cmp -s bar.txt bar_decrypted.bin; then
    echo "RSA 2048: Openssl Decrypt With PKCS1.5 Padding Verified"
else
    echo "RSA 2048: Openssl Decrypt With PKCS1.5 Padding Not Verified"
    exit 1
fi

if openssl pkeyutl -encrypt -pubin -inkey rsa3072_public.pem -in bar.txt -out bar_encrypted.bin -pkeyopt rsa_padding_mode:pkcs1 ;  then
    echo "RSA 3072: Openssl Encrypt With PKCS1.5 Padding Passed"
else
    echo "RSA 3072: Openssl Encrypt With PKCS1.5 Padding Failed"
    exit 1
fi

if openssl pkeyutl -decrypt -in bar_encrypted.bin -out bar_decrypted.bin -inkey "pkcs11:object=rsa3072;type=private" -pkeyopt rsa_padding_mode:pkcs1 ; then
    echo "RSA 3072: Openssl Decrypt With PKCS1.5 Padding Passed"
else
    echo "RSA 3072: Openssl Decrypt With PKCS1.5 Padding Failed"
    exit 1
fi

if cmp -s bar.txt bar_decrypted.bin; then
    echo "RSA 3072: Openssl Decrypt With PKCS1.5 Padding Verified"
else
    echo "RSA 3072: Openssl Decrypt With PKCS1.5 Padding Not Verified"
    exit 1
fi

if pkcs11-tool --module "$MODULE" -b --type privkey --label rsa1024 -l --pin "$PIN" ; then
    echo "RSA 1024: Delete Private Key Passed"
else
    echo "RSA 1024: Delete Private Key Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" -b --type pubkey --label rsa1024  ; then
    echo "RSA 1024: Delete Public Key Passed"
else
    echo "RSA 1024: Delete Public Key Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" -b --type privkey --label rsa2048 -l --pin "$PIN" ; then
    echo "RSA 2048: Delete Private Key Passed"
else
    echo "RSA 2048: Delete Private Key Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" -b --type pubkey --label rsa2048  ; then
    echo "RSA 2048: Delete Public Key Passed"
else
    echo "RSA 2048: Delete Public Key Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" -b --type privkey --label rsa3072 -l --pin "$PIN" ; then
    echo "RSA 3072: Delete Private Key Passed"
else
    echo "RSA 3072: Delete Private Key Failed"
    exit 1
fi

if pkcs11-tool --module "$MODULE" -b --type pubkey --label rsa3072  ; then
    echo "RSA 3072: Delete Public Key Passed"
else
    echo "RSA 3072: Delete Public Key Failed"
    exit 1
fi

rm -f rsa*
rm -f bar*
rm -f hash*
rm -f signature.bin

echo "=================================="
echo "All Test passed successfully!"
