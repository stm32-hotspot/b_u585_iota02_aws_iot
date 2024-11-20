/**
  *
  *  Portions COPYRIGHT 2016 STMicroelectronics
  *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
  *
  ******************************************************************************
  * @file    mbedtls_config.h
  * @author  MCD Application Team
  * @brief   the mbedtls custom config header file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef MBEDTLS_CONFIG_H
#define MBEDTLS_CONFIG_H

#define MBEDTLS_NO_PLATFORM_ENTROPY
#define MBEDTLS_ENTROPY_HARDWARE_ALT
#define MBEDTLS_NIST_KW_C
#define MBEDTLS_CMAC_C
#define MBEDTLS_ARC4_C

#define MBEDTLS_SSL_OUT_CONTENT_LEN    16384
#define MBEDTLS_SSL_MAX_CONTENT_LEN    16384
#define MBEDTLS_SSL_IN_CONTENT_LEN     16384

#define MBEDTLS_ALLOW_PRIVATE_ACCESS

//#define MBEDTLS_DEBUG_C
#define MBEDTLS_LIB_DEBUG_LEVEL           2

#define MBEDTLS_THREADING_ALT
#define MBEDTLS_THREADING_C

#define MBEDTLS_CIPHER_MODE_CBC

#define MBEDTLS_ECP_DP_SECP256R1_ENABLED
#define MBEDTLS_ECP_DP_SECP384R1_ENABLED
#define MBEDTLS_ECP_DP_SECP256K1_ENABLED
#define MBEDTLS_ECP_DP_BP256R1_ENABLED
#define MBEDTLS_ECP_DP_BP384R1_ENABLED
#define MBEDTLS_PKCS1_V15
#define MBEDTLS_SHA256_SMALLER
#define MBEDTLS_SSL_SERVER_NAME_INDICATION
#define MBEDTLS_AES_C
#define MBEDTLS_ASN1_PARSE_C
#define MBEDTLS_BIGNUM_C
#define MBEDTLS_CIPHER_C
#define MBEDTLS_CTR_DRBG_C
#define MBEDTLS_ENTROPY_C
#define MBEDTLS_ERROR_C
#define MBEDTLS_MD_C
#define MBEDTLS_MD5_C
#define MBEDTLS_OID_C
#define MBEDTLS_PKCS5_C
#define MBEDTLS_PK_C
#define MBEDTLS_PK_PARSE_C
#define MBEDTLS_PLATFORM_C
#define MBEDTLS_SHA1_C

//TODO SHA1 shouldn't be used anymore (being deprecated)
#define MBEDTLS_SHA224_C
#define MBEDTLS_SHA256_C
#define MBEDTLS_SHA512_C
#define MBEDTLS_SSL_CLI_C
#define MBEDTLS_SSL_SRV_C
#define MBEDTLS_SSL_TLS_C
#define MBEDTLS_X509_CRT_PARSE_C
#define MBEDTLS_X509_USE_C
#define MBEDTLS_AES_FEWER_TABLES

#if defined NO_STSAFE
#define MBEDTLS_ECP_DP_SECP192R1_ENABLED
#define MBEDTLS_ECP_DP_SECP224R1_ENABLED
#define MBEDTLS_ECP_DP_SECP521R1_ENABLED
#define MBEDTLS_ECP_DP_SECP192K1_ENABLED
#define MBEDTLS_ECP_DP_SECP224K1_ENABLED
#define MBEDTLS_ECP_DP_BP512R1_ENABLED
#define MBEDTLS_ECP_DP_CURVE25519_ENABLED
#define MBEDTLS_KEY_EXCHANGE_RSA_ENABLED
#define MBEDTLS_RSA_C
#else
/* Enable STSAFE for ECDH and ECDSA Key Gen */
#define MBEDTLS_ECDSA_GENKEY_ALT
#endif

/* TLS 1.2 */
#define MBEDTLS_SSL_PROTO_TLS1_2
#define MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED
#define MBEDTLS_GCM_C
#define MBEDTLS_ECDH_C
#define MBEDTLS_ECP_C
#define MBEDTLS_ECDSA_C
#define MBEDTLS_ASN1_WRITE_C

/* Certificate parsing */
#define MBEDTLS_PEM_PARSE_C
#define MBEDTLS_BASE64_C

#define CONFIG_MEDTLS_USE_AFR_MEMORY

#ifdef CONFIG_MEDTLS_USE_AFR_MEMORY
#define MBEDTLS_PLATFORM_MEMORY
    #include <stddef.h>

    extern void * pvCalloc( size_t xNumElements, size_t xSize ) ;
    extern void vPortFree( void *pv );

    #define MBEDTLS_PLATFORM_CALLOC_MACRO pvCalloc
    #define MBEDTLS_PLATFORM_FREE_MACRO   vPortFree
    #define mbedtls_calloc                MBEDTLS_PLATFORM_CALLOC_MACRO
#endif

#include "mbedtls/check_config.h"


#endif /* MBEDTLS_CONFIG_H */
