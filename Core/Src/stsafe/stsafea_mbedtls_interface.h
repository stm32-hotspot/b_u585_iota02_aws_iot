#ifndef __STSAFEA_MBEDTLS_INTERFACE__
#define __STSAFEA_MBEDTLS_INTERFACE__

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include "string.h"

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#include "config_mbedtls_ntz.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

/* STSAFE */
#include "stsafe.h"

/* Mbedtls */
#include "mbedtls/error.h"
#include "mbedtls/ecp.h"
#include "mbedtls/pk.h"
#include "mbedtls/platform.h"
#include "mbedtls/asn1write.h"
#include "mbedtls/ecdsa.h"

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _STSAFEA_MBEDTLS_INTERFACE_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

