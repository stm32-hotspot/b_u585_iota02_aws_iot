#include "main.h"

#if defined(__USE_STSAFE__)
#include "logging_levels.h"
/* define LOG_LEVEL here if you want to modify the logging level from the default */
#define LOG_LEVEL    LOG_INFO

#include "FreeRTOS.h"
#include "semphr.h"

#include "stsafea_core.h"
#include "stsafea_types.h"
#include "pkcs11.h"
#include "stsafe.h"
#include <stdbool.h>
#include <string.h>

#define CERT_PEM 1
#define PUB_PEM  1

const uint32_t zone_size[8] =
{ 1000, /* Zone 0 size */
  700,  /* Zone 1 size */
  600,  /* Zone 2 size */
  600,  /* Zone 3 size */
  1696, /* Zone 4 size */
  64,   /* Zone 5 size */
  64,   /* Zone 6 size */
  1578  /* Zone 7 size */
};

/* Buffer used to STSAFE-A Simple Drivers for I2C buss communications */
static StSafeA_Handle_t stsafea_handle;
static uint8_t a_rx_tx_stsafea_data[STSAFEA_BUFFER_MAX_SIZE];
static SemaphoreHandle_t xSTSAFEMutex = NULL;
static mbedtls_x509_crt stsafea_certificate;

int32_t STSAFE_Enable(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Configure GPIO pin : RST/EN Pin */
  GPIO_InitStruct.Pin = STSAFE_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(STSAFE_EN_GPIO_Port, &GPIO_InitStruct);

  // Check Board revision.
  // Rev-C: reading VREG pin will return 0 => enable = 1
  // Rev-D: reading VREG pin will return 1 => enable = 0
  GPIO_PinState enable = HAL_GPIO_ReadPin(STSAFE_EN_GPIO_Port, STSAFE_EN_Pin) == GPIO_PIN_RESET ? GPIO_PIN_SET : GPIO_PIN_RESET;

  /* Configure GPIO pin : RST/EN Pin */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(STSAFE_EN_GPIO_Port, &GPIO_InitStruct);

  /* Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(STSAFE_EN_GPIO_Port, STSAFE_EN_Pin, enable);

  vTaskDelay(pdMS_TO_TICKS(50));
}

bool SAFEA1_Init(void)
{
  StSafeA_ResponseCode_t status = STSAFEA_COMMUNICATION_ERROR;

  taskENTER_CRITICAL();
  if (xSTSAFEMutex == NULL)
  {
    taskEXIT_CRITICAL();
    STSAFE_Enable();
    taskENTER_CRITICAL();

    xSTSAFEMutex = xSemaphoreCreateMutex();
    status = StSafeA_Init(&stsafea_handle, a_rx_tx_stsafea_data);

    /* Initialize a certificate structure */
    mbedtls_x509_crt_init(&stsafea_certificate);
  }
  taskEXIT_CRITICAL();

  vTaskDelay(pdMS_TO_TICKS(50));

  return status == STSAFEA_OK;
}

uint8_t SAFEA1_GenerateRandom(uint8_t size, uint8_t *random)
{
  uint8_t xReturn = pdTRUE;
  StSafeA_LVBuffer_t TrueRandom;
  StSafeA_ResponseCode_t status = STSAFEA_COMMUNICATION_ERROR;

  if (random == NULL)
  {
    xReturn = pdFALSE;
  }
  else
  {
    TrueRandom.Data = random;

    xSemaphoreTake(xSTSAFEMutex, portMAX_DELAY);
    status = StSafeA_GenerateRandom(&stsafea_handle, STSAFEA_EPHEMERAL_RND, size, &TrueRandom, STSAFEA_MAC_NONE);
    xSemaphoreGive(xSTSAFEMutex);

    if (((uint8_t) status) != STSAFEA_OK)
    {
      xReturn = pdFALSE;
    }
  }

  return xReturn;
}

CK_RV SAFEA1_getDevicePublicKey(CK_BYTE_PTR *ppucData, CK_ULONG_PTR pulDataSize)
{
  *pulDataSize = 0;

  if (stsafea_certificate.pk_raw.p == NULL)
  {
    return ( CKR_FUNCTION_FAILED);
  }

  *pulDataSize = 256;

  if (stsafea_certificate.pk_raw.len > (size_t) *pulDataSize)
  {
    *pulDataSize = 0;
    return ( CKR_FUNCTION_FAILED);
  }

  *ppucData = pvPortMalloc(*pulDataSize);
  configASSERT(ppucData!=NULL);


#if PUB_PEM
  /* Convert the public key into PEM format */
  if ((mbedtls_pem_write_buffer("-----BEGIN PUBLIC KEY-----\n", "-----END PUBLIC KEY-----\n", stsafea_certificate.pk_raw.p, stsafea_certificate.pk_raw.len, *ppucData, (size_t) *pulDataSize, (size_t*) pulDataSize)) < 0)
  {
    *pulDataSize = 0;
    vPortFree(*ppucData);
    return ( CKR_FUNCTION_FAILED);
  }
#else
    /* Copy DER data */
    *pulDataSize = stsafea_certificate.pk_raw.len;
    *ppucData = stsafea_certificate.raw.p;
  #endif

  return ( CKR_OK);

}

CK_RV SAFEA1_getDeviceCertificate(CK_BYTE_PTR *ppucData, CK_ULONG_PTR pulDataSize)
{
  int ret = 0; /* mbedtls return code */
  StSafeA_ResponseCode_t stsafe_status;

  uint16_t certificate_size = 0;
  uint8_t InZoneIndex = 0;
#if CERT_PEM
  size_t pem_len = 0;
#endif
  uint16_t amount_read = 0;
  uint32_t amount_to_read = 0;
  uint16_t length = 0;
  uint8_t header[STSAFE_ZONE_HEADER_SIZE];

  StSafeA_LVBuffer_t buf;
  uint8_t *buf_data;

  buf.Length = 0;
  buf.Data = header;

  /* Extract the first four bytes of STSAFE-A1xx's x509 DER formatted certificate from Zone 0 to get its size */
  xSemaphoreTake(xSTSAFEMutex, portMAX_DELAY);
  stsafe_status = StSafeA_Read(&stsafea_handle, STSAFEA_FLAG_FALSE, STSAFEA_FLAG_FALSE, STSAFEA_AC_ALWAYS, InZoneIndex, 0, STSAFE_ZONE_HEADER_SIZE, STSAFE_ZONE_HEADER_SIZE, &buf, STSAFEA_MAC_NONE);
  xSemaphoreGive(xSTSAFEMutex);

  if (stsafe_status != STSAFEA_OK)
  {
    return ( CKR_FUNCTION_FAILED);
  }

  /* Determine the certificates size */
  if (buf.Data[0] != 0x30)
  {
    return ( CKR_FUNCTION_FAILED);
  }

  switch (buf.Data[1])
  {
  case 0x81U:
    certificate_size = (uint16_t) buf.Data[2] + 3;
    break;

  case 0x82U:
    certificate_size = (((uint16_t) buf.Data[2]) << 8) + buf.Data[3] + 4;
    break;

  default:
    if (buf.Data[1] < 0x81)
    {
      certificate_size = (uint16_t) buf.Data[1];
    }
    break;
  }

  if (certificate_size == 0)
  {
    return ( CKR_FUNCTION_FAILED);
  }

  if (certificate_size > zone_size[InZoneIndex])
  {
    return ( CKR_FUNCTION_FAILED);
  }

  buf_data = pvPortMalloc(zone_size[InZoneIndex]);
  configASSERT(buf_data!=NULL);

  memset(buf_data, 0, zone_size[InZoneIndex]);
  buf.Length = 0;
  buf.Data = buf_data;

  /* Reading STSAFE-A1xx's DER formatted X509 certificate from Zone 0 */
  length = certificate_size;
  amount_read = 0;

  while ((amount_read < certificate_size) && (stsafe_status == STSAFEA_OK))
  {
    if (length > STSAFEA_BUFFER_DATA_CONTENT_SIZE)
    {
      amount_to_read = STSAFEA_BUFFER_DATA_CONTENT_SIZE;
    }
    else
    {
      amount_to_read = length;
    }

    xSemaphoreTake(xSTSAFEMutex, portMAX_DELAY);
    /* Read data from STSAFE-A1x0 Zone 0 */
    stsafe_status = StSafeA_Read(&stsafea_handle, STSAFEA_FLAG_FALSE, STSAFEA_FLAG_FALSE, STSAFEA_AC_ALWAYS, InZoneIndex, amount_read, // Offset
    amount_to_read, amount_to_read, &buf,
    STSAFEA_MAC_NONE);
    xSemaphoreGive(xSTSAFEMutex);

    /* Update the amount of data read */
    amount_read += amount_to_read;
    length -= amount_to_read;
    buf.Data = &buf_data[amount_read];
  }

  if (stsafe_status != STSAFEA_OK)
  {
    return ( CKR_FUNCTION_FAILED);
  }

  /* Read the certificate data into a mbedtls certificate structure */
  ret = mbedtls_x509_crt_parse(&stsafea_certificate, buf_data, certificate_size);

  if (ret < 0)
  {
    return ( CKR_FUNCTION_FAILED);
  }

#if CERT_PEM
  /* Convert the certificate into PEM format */
  ret = mbedtls_pem_write_buffer("-----BEGIN CERTIFICATE-----\n", "-----END CERTIFICATE-----\n", stsafea_certificate.raw.p, stsafea_certificate.raw.len, (unsigned char*) buf_data, zone_size[InZoneIndex], &pem_len);

  if ((ret < 0) || (pem_len == 0) /*|| ( pem_len > *pulDataSize )*/)
  {
    return ( CKR_FUNCTION_FAILED);
  }

  *ppucData = buf_data;
  *pulDataSize = pem_len;
#else
  /* Copy out DER format */
  *pulDataSize = stsafea_certificate.raw.len;
  *ppucData    = stsafea_certificate.raw.p;
#endif

  return ( CKR_OK);
}

/* Returns a string containing the Common Name and the string length */
CK_RV SAFEA1_getDeviceCommonName(CK_BYTE_PTR *ppucData, CK_ULONG_PTR pulDataSize)
{
  int ret;
  char buf[255] =   { 0 };
  char *chr_ptr;
  unsigned char *buf_ptr;
  uint8_t i = 0;

  buf_ptr = pvPortMalloc(STSAFE_VAL_MAX_LEN);
  configASSERT(buf_ptr!=NULL);
  memset(buf_ptr, 0, STSAFE_VAL_MAX_LEN);

  /* Extract and print the certificate information */
  ret = mbedtls_x509_dn_gets((char*) buf, (sizeof(buf) - 1), &stsafea_certificate.subject);

  if ((ret <= 0) || (ret > 255))
  {
    return ( CKR_ARGUMENTS_BAD);
  }

  /* Locate the subject name common name field and print */
  chr_ptr = strstr(buf, "CN=");
  chr_ptr = chr_ptr + (char) 3; //Skip over "CN="

  while ((*chr_ptr != '\0') && (*chr_ptr != ',') && (i < 128 /* max name field size */))
  {
    buf_ptr[i] = *chr_ptr;
    i++;
    chr_ptr++;
  }

  if (i == 0)
  {
    return ( CKR_ARGUMENTS_BAD);
  }

  *ppucData = buf_ptr;
  *pulDataSize = i;

  return ( CKR_OK);
}

/* STSAFE ECDSA Sign */
StSafeA_ResponseCode_t SAFEA1_ECDSA_Sign(uint8_t stsafe_prv_key_slot, const uint8_t *hash, StSafeA_HashTypes_t digest_type, uint16_t RSLength, StSafeA_LVBuffer_t *SignR, StSafeA_LVBuffer_t *SignS)
{
  StSafeA_ResponseCode_t stsafe_status;

  xSemaphoreTake(xSTSAFEMutex, portMAX_DELAY);

  stsafe_status = StSafeA_GenerateSignature(&stsafea_handle, stsafe_prv_key_slot, hash, digest_type, RSLength, SignR, SignS,
  STSAFEA_MAC_NONE,
  STSAFEA_ENCRYPTION_NONE);

  xSemaphoreGive(xSTSAFEMutex);

  return stsafe_status;
}

bool STSAFE1_Read(CK_BYTE_PTR *ppucData, CK_ULONG_PTR pulDataSize, uint8_t InZoneIndex)
{
  StSafeA_ResponseCode_t stsafe_status;
  uint16_t data_size = 0;
  uint16_t amount_read = 0;
  uint32_t amount_to_read = 0;
  uint16_t length = 0;
  uint8_t header[STSAFE_ZONE_HEADER_SIZE];

  StSafeA_LVBuffer_t buf;
  uint8_t *buf_data;

  buf.Length = 0;
  buf.Data = header;

  xSemaphoreTake(xSTSAFEMutex, portMAX_DELAY);
  stsafe_status = StSafeA_Read(&stsafea_handle, STSAFEA_FLAG_FALSE, STSAFEA_FLAG_FALSE, STSAFEA_AC_ALWAYS, InZoneIndex, amount_read, STSAFE_ZONE_HEADER_SIZE, STSAFE_ZONE_HEADER_SIZE, &buf, STSAFEA_MAC_NONE);
  xSemaphoreGive(xSTSAFEMutex);

  if (stsafe_status != STSAFEA_OK)
  {
    return false;
  }

  if (header[0] != 0x30)
  {
    return false;
  }

  switch (header[1])
  {
  case 0x81U:
    data_size = (uint16_t) header[2] + 3;
    break;

  case 0x82U:
    data_size = (((uint16_t) header[2]) << 8) + header[3];
    break;

  default:
    if (header[1] < 0x81)
    {
      data_size = (uint16_t) header[1];
    }
    break;
  }

  if (data_size + STSAFE_ZONE_HEADER_SIZE > zone_size[InZoneIndex])
  {
    return false;
  }

  buf_data = pvPortMalloc(data_size);
  configASSERT(buf_data!=NULL);

  memset(buf_data, 0, data_size);
  buf.Length = 0;
  buf.Data = buf_data;

  /* Reading from STSAFE */
  length = data_size;
  amount_read = STSAFE_ZONE_HEADER_SIZE;

  while ((amount_read < data_size + STSAFE_ZONE_HEADER_SIZE) && (stsafe_status == STSAFEA_OK))
  {
    if (length > STSAFEA_BUFFER_DATA_CONTENT_SIZE)
    {
      amount_to_read = STSAFEA_BUFFER_DATA_CONTENT_SIZE;
    }
    else
    {
      amount_to_read = length;
    }

    if((amount_to_read !=(amount_to_read + amount_read)%STSAFEA_BUFFER_DATA_CONTENT_SIZE) && ((amount_to_read + amount_read) > STSAFEA_BUFFER_DATA_CONTENT_SIZE))
    {
      amount_to_read -=(amount_to_read + amount_read)%STSAFEA_BUFFER_DATA_CONTENT_SIZE;
    }

    xSemaphoreTake(xSTSAFEMutex, portMAX_DELAY);
    stsafe_status = StSafeA_Read(&stsafea_handle, STSAFEA_FLAG_FALSE, STSAFEA_FLAG_FALSE, STSAFEA_AC_ALWAYS, InZoneIndex, amount_read, amount_to_read, amount_to_read, &buf, STSAFEA_MAC_NONE);
    xSemaphoreGive(xSTSAFEMutex);

    amount_read += amount_to_read;
    length -= amount_to_read;
    buf.Data = &buf_data[amount_read - STSAFE_ZONE_HEADER_SIZE];
  }

  if (stsafe_status == STSAFEA_OK)
  {
    *ppucData = buf_data;
    *pulDataSize = data_size;
  }

  return stsafe_status == STSAFEA_OK;
}

bool STSAFE1_Write(CK_BYTE_PTR pucData, CK_ULONG ulDataSize, uint8_t InZoneIndex)
{
  bool status = false;
  StSafeA_ResponseCode_t stsafe_status;
  uint32_t amount_written = 0;
  uint32_t amount_to_write = 0;
  uint32_t length = ulDataSize;
  StSafeA_LVBuffer_t buf;
  uint8_t header[STSAFE_ZONE_HEADER_SIZE];

  if (ulDataSize + STSAFE_ZONE_HEADER_SIZE > zone_size[InZoneIndex])
  {
    return false;
  }

  /* Set Zone header */
  header[0] = 0x30;
  header[1] = 0x82;
  header[2] = ulDataSize >> 8;
  header[3] = ulDataSize;

  if ((pucData != NULL) && (ulDataSize <= (zone_size[InZoneIndex] - STSAFE_ZONE_HEADER_SIZE)))
  {
    xSemaphoreTake(xSTSAFEMutex, portMAX_DELAY);

    /* Write the header */
    buf.Length = STSAFE_ZONE_HEADER_SIZE;
    buf.Data = header;
    stsafe_status = StSafeA_Update(&stsafea_handle, STSAFEA_FLAG_TRUE, STSAFEA_FLAG_FALSE, STSAFEA_FLAG_FALSE, STSAFEA_AC_ALWAYS, InZoneIndex, amount_written, &buf, STSAFEA_MAC_NONE);

    amount_written = STSAFE_ZONE_HEADER_SIZE;
    buf.Length = ulDataSize;
    buf.Data = pucData;

    while ((amount_written < ulDataSize + STSAFE_ZONE_HEADER_SIZE) && (stsafe_status == STSAFEA_OK))
    {
      if (length > STSAFEA_BUFFER_DATA_CONTENT_SIZE)
      {
        amount_to_write = STSAFEA_BUFFER_DATA_CONTENT_SIZE;
      }
      else
      {
        amount_to_write = length;
      }

      if((amount_to_write !=(amount_to_write + amount_written)%STSAFEA_BUFFER_DATA_CONTENT_SIZE) && ((amount_to_write + amount_written) > STSAFEA_BUFFER_DATA_CONTENT_SIZE))
      {
        amount_to_write -=(amount_to_write + amount_written)%STSAFEA_BUFFER_DATA_CONTENT_SIZE;
      }

      buf.Length = amount_to_write;

      vTaskDelay(50);

      /* Write data to STSAFE */
      stsafe_status = StSafeA_Update(&stsafea_handle, STSAFEA_FLAG_TRUE, STSAFEA_FLAG_FALSE, STSAFEA_FLAG_FALSE, STSAFEA_AC_ALWAYS, InZoneIndex, amount_written, &buf, STSAFEA_MAC_NONE);

      /* Update the amount of data read */
      amount_written += amount_to_write;
      length -= amount_to_write;
      buf.Data = &pucData[amount_written - STSAFE_ZONE_HEADER_SIZE];
    }

    status = stsafe_status == STSAFEA_OK;

    xSemaphoreGive(xSTSAFEMutex);
  }

  return status;
}

bool STSAFE1_Erase(uint8_t InZoneIndex)
{
  bool status = false;
  StSafeA_ResponseCode_t stsafe_status;
  StSafeA_LVBuffer_t buf;
  uint8_t data[STSAFEA_BUFFER_DATA_CONTENT_SIZE] = { 0 };

  if (STSAFE_DeviceCertificate_ZONE == InZoneIndex)
  {
    return false;
  }

  /* Overwrite the data */
  buf.Length = STSAFEA_BUFFER_DATA_CONTENT_SIZE;
  buf.Data = data;

  xSemaphoreTake(xSTSAFEMutex, portMAX_DELAY);
  stsafe_status = StSafeA_Update(&stsafea_handle, STSAFEA_FLAG_TRUE, STSAFEA_FLAG_FALSE, STSAFEA_FLAG_FALSE, STSAFEA_AC_ALWAYS, InZoneIndex, 0, &buf, STSAFEA_MAC_NONE);
  xSemaphoreGive(xSTSAFEMutex);

  status = stsafe_status == STSAFEA_OK;

  return status;
}

#endif /* __SAFEA1_CONF_H__ */
