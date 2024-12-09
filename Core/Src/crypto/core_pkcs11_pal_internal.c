#include "pkcs11.h"
#include "core_pkcs11_pal.h"
#include "core_pkcs11_config.h"
#include "core_pkcs11.h"

#include "key_value_store.h"
#include <string.h>

extern metadata_t Metadata;
/*------------------------ PKCS #11 PAL functions -------------------------*/

/**
 * @brief Initializes the PKCS #11 PAL.
 *
 * This is always called first in C_Initialize if the module is not already
 * initialized.
 *
 * @return CKR_OK on success.
 * CKR_FUNCTION_FAILED on failure.
 */
CK_RV PKCS11_PAL_Initialize(void)
{
//    #error "PKCS11_PAL_Initialize not implemented. Remove this error when implemented."
  CK_RV xRet = CKR_OK;
  return xRet;
}

/**
 * @brief Saves an object in non-volatile storage.
 *
 * Port-specific file write for cryptographic information.
 *
 * @param[in] pxLabel       Attribute containing label of the object to be stored.
 * @param[in] pucData       The object data to be saved.
 * @param[in] ulDataSize    Size (in bytes) of object data.
 *
 * @return The object handle if successful.
 * eInvalidHandle = 0 if unsuccessful.
 */
CK_OBJECT_HANDLE PKCS11_PAL_SaveObject(CK_ATTRIBUTE_PTR pxLabel, CK_BYTE_PTR pucData, CK_ULONG ulDataSize)
{
//    #error "PKCS11_PAL_SaveObject not implemented. Remove this error when implemented."
  CK_OBJECT_HANDLE xRet = 0;

  char *value;
  value = pxLabel->pValue;

  if (strcmp(value, pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS) == 0)
  {
    pfKvs_writeKeyValue(CONF_KEY_PRV_KEY, pucData, ulDataSize);
    xRet = 1;
  }
  else if (strcmp(value, pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS) == 0)
  {
    pfKvs_writeKeyValue(CONF_KEY_CERT, pucData, ulDataSize);
    xRet = 2;
  }
  else if (strcmp(value, pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS) == 0)
  {
    pfKvs_writeKeyValue(CONF_KEY_PUB_KEY, pucData, ulDataSize);
    xRet = 3;
  }
  else if (strcmp(value, pkcs11configLABEL_ROOT_CERTIFICATE) == 0)
  {
    pfKvs_writeKeyValue(CONF_KEY_ROOTCA, pucData, ulDataSize);
    xRet = 4;
  }

  return xRet;
}

/**
 * @brief Delete an object from NVM.
 *
 * @param[in] xHandle       Handle to a PKCS #11 object.
 */
CK_RV PKCS11_PAL_DestroyObject(CK_OBJECT_HANDLE xHandle)
{
//    #error "PKCS11_PAL_DestroyObject not implemented. Remove this error when implemented."
  CK_RV xRet = CKR_OK;

  return xRet;
}

/**
 * @brief Translates a PKCS #11 label into an object handle.
 *
 * Port-specific object handle retrieval.
 *
 *
 * @param[in] pxLabel         Pointer to the label of the object
 *                           who's handle should be found.
 * @param[in] usLength       The length of the label, in bytes.
 *
 * @return The object handle if operation was successful.
 * Returns eInvalidHandle if unsuccessful.
 */
CK_OBJECT_HANDLE PKCS11_PAL_FindObject(CK_BYTE_PTR pxLabel, CK_ULONG usLength)
{
//    #error "PKCS11_PAL_FindObject not implemented. Remove this error when implemented."
  CK_OBJECT_HANDLE xRet = 0;

  if (strcmp(pxLabel, pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS) == 0)
  {
    xRet = 1;
  }
  else if (strcmp(pxLabel, pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS) == 0)
  {
    xRet = 2;
  }
  else if (strcmp(pxLabel, pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS) == 0)
  {
    xRet = 3;
  }
  else if (strcmp(pxLabel, pkcs11configLABEL_ROOT_CERTIFICATE) == 0)
  {
    xRet = 4;
  }

  return xRet;
}

/**
 * @brief Gets the value of an object in storage, by handle.
 *
 * Port-specific file access for cryptographic information.
 *
 * This call dynamically allocates the buffer which object value
 * data is copied into.  PKCS11_PAL_GetObjectValueCleanup()
 * should be called after each use to free the dynamically allocated
 * buffer.
 *
 * @sa PKCS11_PAL_GetObjectValueCleanup
 *
 * @param[in]  xHandle      The PKCS #11 object handle of the object to get the value of.
 * @param[out] ppucData     Pointer to buffer for file data.
 * @param[out] pulDataSize  Size (in bytes) of data located in file.
 * @param[out] pIsPrivate   Boolean indicating if value is private (CK_TRUE)
 *                          or exportable (CK_FALSE)
 *
 * @return CKR_OK if operation was successful.  CKR_KEY_HANDLE_INVALID if
 * no such object handle was found, CKR_DEVICE_MEMORY if memory for
 * buffer could not be allocated, CKR_FUNCTION_FAILED for device driver
 * error.
 */
CK_RV PKCS11_PAL_GetObjectValue(CK_OBJECT_HANDLE xHandle, CK_BYTE_PTR *ppucData, CK_ULONG_PTR pulDataSize, CK_BBOOL *pIsPrivate)
{
//    #error "PKCS11_PAL_GetObjectValue not implemented. Remove this error when implemented."
  CK_RV xRet = CKR_CANCEL;

  if (xHandle == 1)
  {
    *ppucData    =  Metadata.pPRV_KEY;
    *pulDataSize = *Metadata.pPRV_KEY_SIZE;
    *pIsPrivate  =  true;
    xRet         =  CKR_OK;
  }
  else if (xHandle == 2)
  {
    *ppucData    =  Metadata.pCERT;
    *pulDataSize = *Metadata.pCERT_SIZE;
    *pIsPrivate  =  false;
    xRet         =  CKR_OK;
  }
  else if (xHandle == 3)
  {
    *ppucData    =  Metadata.pPB_KEY;
    *pulDataSize = *Metadata.pPB_KEY_SIZE;
    *pIsPrivate  =  false;
    xRet         =  CKR_OK;
  }
  else if (xHandle == 4)
  {
    *ppucData    =  Metadata.pROOT_CERT;
    *pulDataSize = *Metadata.pROOT_CERT_SIZE;
    *pIsPrivate  =  false;
    xRet         =  CKR_OK;
  }

  return xRet;
}

/**
 * @brief Cleanup after PKCS11_GetObjectValue().
 *
 * @param[in] pucData       The buffer to free.
 *                          (*ppucData from PKCS11_PAL_GetObjectValue())
 * @param[in] ulDataSize    The length of the buffer to free.
 *                          (*pulDataSize from PKCS11_PAL_GetObjectValue())
 */
void PKCS11_PAL_GetObjectValueCleanup(CK_BYTE_PTR pucData, CK_ULONG ulDataSize)
{
//    #error "PKCS11_PAL_GetObjectValueCleanup not implemented. Remove this error when implemented."
  return;
}
