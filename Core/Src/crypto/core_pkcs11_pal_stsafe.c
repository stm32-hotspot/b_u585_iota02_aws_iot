/*
 * FreeRTOS STM32 Reference Integration
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file pkcs11_pal_stsafe.c
 * @brief STSAFE save and read implementation
 * for PKCS #11 based on mbedTLS with for software keys. This
 * file deviates from the FreeRTOS style standard for some function names and
 * data types in order to maintain compliance with the PKCS #11 standard.
 */
/*-----------------------------------------------------------*/

/**************************************************/
/******* DO NOT CHANGE the following order ********/
/**************************************************/

/* Include logging header files and define logging macros in the following order:
 * 1. Include the header file "logging_levels.h".
 * 2. Define the LIBRARY_LOG_NAME and LIBRARY_LOG_LEVEL macros depending on
 * the logging configuration for PKCS #11.
 * 3. Include the header file "logging_stack.h", if logging is enabled for PKCS #11.
 */

#include "logging_levels.h"
#define LOG_LEVEL    LOG_INFO
#include "logging.h"
#include "FreeRTOS.h"
#include "atomic.h"

/* PKCS 11 includes. */
#include "core_pkcs11_config.h"
#include "core_pkcs11_config_defaults.h"
#include "core_pkcs11.h"
#include "core_pkcs11_pal_utils.h"
#include "core_pkcs11_pal.h"
#include "stsafe_key_value_store.h"
#include "mbedtls/asn1.h"

#if PKCS11_PAL_STSAFE
#include "stsafe.h"
#include <string.h>
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

CK_RV PKCS11_PAL_Initialize(void)
{
  LogInfo("* Certs from STSFE *");

  CK_BYTE_PTR pucData;
  CK_ULONG ulDataSize;

  SAFEA1_getDeviceCertificate(&pucData, &ulDataSize);
  vPortFree(pucData);

  return CKR_OK;
}

CK_OBJECT_HANDLE PKCS11_PAL_SaveObject(CK_ATTRIBUTE_PTR pxLabel, CK_BYTE_PTR pucData, CK_ULONG ulDataSize)
{
  CK_OBJECT_HANDLE xHandle = (CK_OBJECT_HANDLE) eInvalidHandle;

  configASSERT(pxLabel!=NULL);
  configASSERT(pucData!=NULL);
  configASSERT(ulDataSize > 0);

  xHandle = PKCS11_PAL_FindObject((CK_BYTE_PTR) pxLabel->pValue, pxLabel->ulValueLen);

  switch (xHandle)
  {
  case eAwsDevicePrivateKey: /**< Private Key. */
    xHandle = eInvalidHandle;
    break;

  case eAwsDevicePublicKey: /**< Public Key. */
    xHandle = eInvalidHandle;
    break;

  case eAwsDeviceCertificate: /**< Certificate. */
    xHandle = eInvalidHandle;
    break;

  case eAwsCodeSigningKey: /**< Code Signing Key. */
    xHandle = eInvalidHandle;
    break;

  case eAwsHMACSecretKey: /**< HMAC Secret Key. */
    xHandle = eInvalidHandle;
    break;

  case eAwsCMACSecretKey: /**< CMAC Secret Key. */
    xHandle = eInvalidHandle;
    break;

  case eAwsClaimPrivateKey: /**< Provisioning Claim Private Key. */
    xHandle = eInvalidHandle;
    break;

  case eAwsClaimCertificate: /**< Provisioning Claim Certificate. */
    xHandle = eInvalidHandle;
    break;

  case eAwsCaCertificate:
    if (STSAFE1_Write(pucData, ulDataSize, STSAFE_ServerCert_ZONE) != true)
    {
      xHandle = eInvalidHandle;
    }
    break;
  }

  return xHandle;
}

/*-----------------------------------------------------------*/

CK_OBJECT_HANDLE PKCS11_PAL_FindObject(CK_BYTE_PTR pxLabel, CK_ULONG usLength)
{
  CK_OBJECT_HANDLE xHandle = (CK_OBJECT_HANDLE) eInvalidHandle;

  if (pxLabel != NULL)
  {
    if (0 == strncmp((const char*) pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS, (const char*) pxLabel, sizeof( pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS)))
    {
      xHandle = (CK_OBJECT_HANDLE) eAwsDevicePrivateKey;
    }
    else if (0 == strncmp((const char*) pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS, (const char*) pxLabel, sizeof( pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS)))
    {
      xHandle = (CK_OBJECT_HANDLE) eAwsDevicePublicKey;
    }
    else if (0 == strncmp((const char*) pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS, (const char*) pxLabel, sizeof( pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS)))
    {
      xHandle = (CK_OBJECT_HANDLE) eAwsDeviceCertificate;
    }
    else if (0 == strncmp((const char*) pkcs11configLABEL_CODE_VERIFICATION_KEY, (const char*) pxLabel, sizeof( pkcs11configLABEL_CODE_VERIFICATION_KEY)))
    {
      xHandle = (CK_OBJECT_HANDLE) eAwsCodeSigningKey;
    }
    else if (0 == strncmp((const char*) pkcs11configLABEL_HMAC_KEY, (const char*) pxLabel, sizeof( pkcs11configLABEL_HMAC_KEY)))
    {
      xHandle = (CK_OBJECT_HANDLE) eAwsHMACSecretKey;
    }
    else if (0 == strncmp((const char*) pkcs11configLABEL_CMAC_KEY, (const char*) pxLabel, sizeof( pkcs11configLABEL_CMAC_KEY)))
    {
      xHandle = (CK_OBJECT_HANDLE) eAwsCMACSecretKey;
    }
    else if (0 == strncmp((const char*) pkcs11configLABEL_CLAIM_CERTIFICATE, (const char*) pxLabel, sizeof( pkcs11configLABEL_CLAIM_CERTIFICATE)))
    {
      xHandle = (CK_OBJECT_HANDLE) eAwsClaimCertificate;
    }
    else if (0 == strncmp((const char*) pkcs11configLABEL_CLAIM_PRIVATE_KEY, (const char*) pxLabel, sizeof( pkcs11configLABEL_CLAIM_PRIVATE_KEY)))
    {
      xHandle = (CK_OBJECT_HANDLE) eAwsClaimPrivateKey;
    }
    else if (0 == strncmp((const char*) pkcs11_ROOT_CA_CERT_LABEL, (const char*) pxLabel, sizeof( pkcs11_ROOT_CA_CERT_LABEL)))
    {
      xHandle = (CK_OBJECT_HANDLE) eAwsCaCertificate;
    }
    else
    {
      xHandle = (CK_OBJECT_HANDLE) eInvalidHandle;
    }
  }
  else
  {
    LogError(( "Could not find object. Received a NULL label." ));
  }

  return xHandle;
}
/*-----------------------------------------------------------*/

CK_RV PKCS11_PAL_GetObjectValue(CK_OBJECT_HANDLE xHandle, CK_BYTE_PTR *ppucData, CK_ULONG_PTR pulDataSize, CK_BBOOL *pIsPrivate)
{
  CK_RV xReturn = CKR_OK;

  if ((ppucData == NULL) || (pulDataSize == NULL) || (pIsPrivate == NULL))
  {
    xReturn = CKR_ARGUMENTS_BAD;
    LogError(( "Could not get object value. Received a NULL argument." ));
  }
  else
  {
    switch (xHandle)
    {
    case eAwsDevicePrivateKey: /**< Private Key. */
      *pIsPrivate = (CK_BBOOL) CK_TRUE;
      xReturn = CKR_ACTION_PROHIBITED;
      break;

    case eAwsDevicePublicKey: /**< Public Key. */
      xReturn = SAFEA1_getDevicePublicKey(ppucData, pulDataSize);
      *pIsPrivate = (CK_BBOOL) CK_FALSE;
      break;

    case eAwsDeviceCertificate: /**< Certificate. */
      xReturn = SAFEA1_getDeviceCertificate(ppucData, pulDataSize);
      *pIsPrivate = (CK_BBOOL) CK_FALSE;
      break;

    case eAwsCodeSigningKey: /**< Code Signing Key. */
      xReturn = CKR_FUNCTION_FAILED;
      break;

    case eAwsHMACSecretKey: /**< HMAC Secret Key. */
      xReturn = CKR_FUNCTION_FAILED;
      break;

    case eAwsCMACSecretKey: /**< CMAC Secret Key. */
      xReturn = CKR_FUNCTION_FAILED;
      break;

    case eAwsClaimPrivateKey: /**< Provisioning Claim Private Key. */
      xReturn = CKR_FUNCTION_FAILED;
      break;

    case eAwsClaimCertificate: /**< Provisioning Claim Certificate. */
      xReturn = CKR_FUNCTION_FAILED;
      break;

    case eAwsCaCertificate:
      if (STSAFE1_Read(ppucData, pulDataSize, STSAFE_ServerCert_ZONE) != true)
      {
        xReturn = CKR_FUNCTION_FAILED;
      }
      *pIsPrivate = (CK_BBOOL) CK_FALSE;
      break;

    default:
      xReturn = CKR_ARGUMENTS_BAD;
      break;
    }
  }

  return xReturn;
}

/*-----------------------------------------------------------*/

void PKCS11_PAL_GetObjectValueCleanup(CK_BYTE_PTR pucData, CK_ULONG ulDataSize)
{
  /* Unused parameters. */
  (void) ulDataSize;

  if ( NULL != pucData)
  {
    vPortFree(pucData);
  }
}

/*-----------------------------------------------------------*/

CK_RV PKCS11_PAL_DestroyObject(CK_OBJECT_HANDLE xHandle)
{
  CK_RV xResult = CKR_FUNCTION_FAILED;

  configASSERT_CONTINUE(xHandle != CK_INVALID_HANDLE);

  switch (xHandle)
  {
  case eAwsDevicePrivateKey: /**< Private Key. */
    break;

  case eAwsDevicePublicKey: /**< Public Key. */
    break;

  case eAwsDeviceCertificate: /**< Certificate. */
    break;

  case eAwsCodeSigningKey: /**< Code Signing Key. */
    break;

  case eAwsHMACSecretKey: /**< HMAC Secret Key. */
    break;

  case eAwsCMACSecretKey: /**< CMAC Secret Key. */
    break;

  case eAwsClaimPrivateKey: /**< Provisioning Claim Private Key. */
    break;

  case eAwsClaimCertificate: /**< Provisioning Claim Certificate. */
    break;

  case eAwsCaCertificate:
    STSAFE1_Erase(STSAFE_ServerCert_ZONE);
    xResult = CKR_OK;
    break;

  default:
    xResult = CKR_ARGUMENTS_BAD;
    break;
  }

  return xResult;
}

#endif
/*-----------------------------------------------------------*/
