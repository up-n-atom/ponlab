#!/bin/sh

openssl req \
  -x509 \
  -newkey rsa:4096 \
  -keyout autocert/testing_key1.pem \
  -out autocert/testing_cert1.pem \
  -sha512 \
  -days 1095 \
  -nodes \
  -subj "/C=US/O=PrplFoundation/OU=prplOS/CN=prplOS.lan"

openssl req \
  -x509 \
  -newkey ec \
  -keyout autocert/testing_key2.pem \
  -pkeyopt ec_paramgen_curve:prime256v1 \
  -out autocert/testing_cert2.pem \
  -sha512 \
  -days 730 \
  -nodes \
  -subj "/C=US/O=PrplFoundation/OU=prplOS/CN=prplOS.lan"
