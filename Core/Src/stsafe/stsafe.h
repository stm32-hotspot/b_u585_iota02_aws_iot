
#ifndef _STSAFE_H_
#define _STSAFE_H_

#include "main.h"

#include "pkcs11.h"
#include <stdbool.h>

/* Mbedtls */
#include "mbedtls/ecdsa.h"
#include "mbedtls/pem.h"
#include "mbedtls/x509_crt.h"

#include "stsafea_types.h"
#define STSAFE_VAL_MAX_LEN 64
#define STSAFE_DeviceCertificate_ZONE  0 /* Device Cert in zone 0      */
#define STSAFE_KVSTORE_ZONE            1 /* KVSTORE is inSTSAFE Zone 1 */
#define STSAFE_ServerCert_ZONE         7 /* CA Cert in zone 2          */

#define STSAFE_ZONE_1_SIZE             700 /* Zone 1 size */
#define STSAFE_ZONE_HEADER_SIZE        4   /* Header size */

bool SAFEA1_Init(void);
uint8_t SAFEA1_GenerateRandom(uint8_t size, uint8_t *random);
CK_RV SAFEA1_getDevicePublicKey  (CK_BYTE_PTR * ppucData,CK_ULONG_PTR pulDataSize);
CK_RV SAFEA1_getDeviceCertificate(CK_BYTE_PTR * ppucData,CK_ULONG_PTR pulDataSize);
CK_RV SAFEA1_getDeviceCommonName (CK_BYTE_PTR * ppucData,CK_ULONG_PTR pulDataSize);
bool STSAFE1_Read (CK_BYTE_PTR *ppucData, CK_ULONG_PTR pulDataSize, uint8_t InZoneIndex);
bool STSAFE1_Write(CK_BYTE_PTR pucData, CK_ULONG ulDataSize, uint8_t InZoneIndex);
bool STSAFE1_Erase(uint8_t InZoneIndex);

StSafeA_ResponseCode_t SAFEA1_ECDSA_Sign( uint8_t stsafe_prv_key_slot,
                                          const uint8_t *hash,
                                          StSafeA_HashTypes_t digest_type,
                                          uint16_t RSLength,
                                          StSafeA_LVBuffer_t *SignR,
                                          StSafeA_LVBuffer_t *SignS );

#endif /* _STSAFE_H_ */
