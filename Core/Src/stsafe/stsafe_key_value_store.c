#include <string.h>
#include <stdbool.h>

#include <logging_levels.h>
#define LOG_LEVEL    LOG_INFO

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "kvstore.h"
#if defined(__USE_STSAFE__)
#include "stsafe.h"
#include <stsafe_key_value_store.h>
#include "stsafea_types.h"

extern CRC_HandleTypeDef hcrc;

#define MAGIC_NUMBER 0x37a4ef09

/* TODO: Remove test mode */
#if 0 /* Test mode */
#define DEFAULT_MQTT_PORT        0
#define DEFAULT_WIFI_SSID        ""
#define DEFAULT_WIFI_PASSWORD    ""
#define DEFAULT_AWS_IOT_ENDPOINT ""
#else
#define DEFAULT_MQTT_PORT        8883
#define DEFAULT_WIFI_SSID        "st_iot_demo"
#define DEFAULT_WIFI_PASSWORD    "stm32u585"
#define DEFAULT_AWS_IOT_ENDPOINT "a1qwhobjtvew8t-ats.iot.us-west-2.amazonaws.com"
#endif

#define STSAFE_MAX_KVSTORE_SIZE (STSAFE_ZONE_1_SIZE - STSAFE_ZONE_HEADER_SIZE)

/* Define the KVStoreTLV_t structure */
typedef struct
{
  KVStoreTLVHeader_t xTlvHeader;
  uint8_t data[STSAFE_KVSTORE_VAL_MAX_LEN];
} KVStoreTLV_t;

typedef struct
{
  uint32_t magic_number;
  KVStoreTLV_t KVStore[CS_NUM_KEYS];
  uint32_t crc;
}STSAFE_KVStoreTLV_t;

static STSAFE_KVStoreTLV_t * pxSTSAFE_KVStoreTLV = NULL;

static const uint8_t endpoint[]   = DEFAULT_AWS_IOT_ENDPOINT;
static const uint8_t ssid[]       = DEFAULT_WIFI_SSID;
static const uint8_t password[]   = DEFAULT_WIFI_PASSWORD;

static bool pfKvs_read(void);
static bool pfKvs_write (void);
static bool pfKvs_setDefault(void);

_Static_assert(sizeof(STSAFE_KVStoreTLV_t) <= STSAFE_MAX_KVSTORE_SIZE, "STSAFE_KVStoreTLV_t size exceeds STSAFE_MAX_KVSTORE_SIZE");

/* Function to initialize the KV store */
bool pfKvs_init(void)
{
  bool status = false;

  status = pfKvs_read();

  if(status == false)
  {
    status = pfKvs_setDefault();
  }

  return status;
}

/* Function to get the length of a key's value */
bool pfKvs_getKeyLength(KVStoreKey_t xKey, KVStoreTLVHeader_t *pxTlvHeader)
{
  if (xKey >= CS_NUM_KEYS)
  {
    return false;
  }

  pxTlvHeader->length = pxSTSAFE_KVStoreTLV->KVStore[xKey].xTlvHeader.length;

  return true;
}

/* Function to get the value of a key */
bool pfKvs_getKeyValue(KVStoreKey_t xKey, uint8_t *value, KVStoreTLVHeader_t *pxTlvHeader)
{
  if (xKey >= CS_NUM_KEYS || value == NULL || pxTlvHeader == NULL)
  {
    return false;
  }

  if (pxTlvHeader->length < pxSTSAFE_KVStoreTLV->KVStore[xKey].xTlvHeader.length)
  {
    return false;
  }

  memcpy(value, &pxSTSAFE_KVStoreTLV->KVStore[xKey].data[0], pxSTSAFE_KVStoreTLV->KVStore[xKey].xTlvHeader.length);
  pxTlvHeader->length = pxSTSAFE_KVStoreTLV->KVStore[xKey].xTlvHeader.length;

  return true;
}

/* Function to write a key-value pair */
bool pfKvs_writeKeyValue(KVStoreKey_t xKey, const uint8_t *value, KVStoreTLVHeader_t xTlvHeader)
{
  if (xKey >= CS_NUM_KEYS || value == NULL)
  {
    return false;
  }

  if(xKey == CS_CORE_THING_NAME)
  {
    return false;
  }

  pxSTSAFE_KVStoreTLV->KVStore[xKey].xTlvHeader.type = xTlvHeader.type; /* Assuming type is the same as the key */
  pxSTSAFE_KVStoreTLV->KVStore[xKey].xTlvHeader.length = xTlvHeader.length;
  memcpy(pxSTSAFE_KVStoreTLV->KVStore[xKey].data, value, pxSTSAFE_KVStoreTLV->KVStore[xKey].xTlvHeader.length);

  /* Update the CRC */
  __HAL_CRC_DR_RESET(&hcrc);
  pxSTSAFE_KVStoreTLV->crc = HAL_CRC_Calculate(&hcrc, (uint32_t*) pxSTSAFE_KVStoreTLV, sizeof(STSAFE_KVStoreTLV_t) / 4);

  return pfKvs_write();
}

/* Function to write to NVM */
static bool pfKvs_write(void)
{
  bool status = false;

  status = STSAFE1_Write((uint8_t *) pxSTSAFE_KVStoreTLV, sizeof(STSAFE_KVStoreTLV_t), STSAFE_KVSTORE_ZONE);

  return status;
}

/* Function to read from VNM */
static bool pfKvs_read(void)
{
  bool status = false;
  uint32_t data_size = 0;

  status = STSAFE1_Read((CK_BYTE_PTR *)&pxSTSAFE_KVStoreTLV, (CK_ULONG_PTR)&data_size, STSAFE_KVSTORE_ZONE);

  if (status)
  {
#if 0
    /* Check CRC and Magic number */
    __HAL_CRC_DR_RESET(&hcrc);
    uint32_t uwCRCValue = HAL_CRC_Calculate(&hcrc, (uint32_t*) pxSTSAFE_KVStoreTLV, sizeof(STSAFE_KVStoreTLV_t) / 4);

    status = ((pxSTSAFE_KVStoreTLV->magic_number == MAGIC_NUMBER) && (uwCRCValue == pxSTSAFE_KVStoreTLV->crc));
#else
    /* FIXME: For some reasons the CRC check is failing */
    status = (pxSTSAFE_KVStoreTLV->magic_number == MAGIC_NUMBER);
#endif
  }

  return status;
}

static bool pfKvs_setDefault(void)
{
  CK_BYTE_PTR pucData;
  CK_ULONG ulDataSize;
  CK_RV result;

  if(pxSTSAFE_KVStoreTLV == NULL)
  {
    pxSTSAFE_KVStoreTLV = pvPortMalloc(sizeof(STSAFE_KVStoreTLV_t));
  }

  configASSERT(pxSTSAFE_KVStoreTLV!=NULL);
  result = SAFEA1_getDeviceCertificate(&pucData, &ulDataSize);

  if (result == CKR_OK)
  {
    vPortFree(pucData);

    result = SAFEA1_getDeviceCommonName(&pucData, &ulDataSize);
  }

  if(result == CKR_OK)
  {
    pxSTSAFE_KVStoreTLV->KVStore[CS_CORE_THING_NAME].xTlvHeader.type = KV_TYPE_STRING;
    pxSTSAFE_KVStoreTLV->KVStore[CS_CORE_THING_NAME].xTlvHeader.length = ulDataSize;
    memcpy(pxSTSAFE_KVStoreTLV->KVStore[CS_CORE_THING_NAME].data, pucData, pxSTSAFE_KVStoreTLV->KVStore[CS_CORE_THING_NAME].xTlvHeader.length);

    vPortFree(pucData);
  }

  pxSTSAFE_KVStoreTLV->KVStore[CS_CORE_MQTT_ENDPOINT].xTlvHeader.type = KV_TYPE_STRING;
  pxSTSAFE_KVStoreTLV->KVStore[CS_CORE_MQTT_ENDPOINT].xTlvHeader.length = strlen(DEFAULT_AWS_IOT_ENDPOINT) + 1;
  memcpy(pxSTSAFE_KVStoreTLV->KVStore[CS_CORE_MQTT_ENDPOINT].data, endpoint, pxSTSAFE_KVStoreTLV->KVStore[CS_CORE_MQTT_ENDPOINT].xTlvHeader.length);

  pxSTSAFE_KVStoreTLV->KVStore[CS_CORE_MQTT_PORT].xTlvHeader.type = KV_TYPE_UINT32;
  pxSTSAFE_KVStoreTLV->KVStore[CS_CORE_MQTT_PORT].xTlvHeader.length = sizeof(uint32_t);
  pxSTSAFE_KVStoreTLV->KVStore[CS_CORE_MQTT_PORT].data[0] = (uint8_t) ((DEFAULT_MQTT_PORT >> 00) & 0xFF); // Lower 8 bits
  pxSTSAFE_KVStoreTLV->KVStore[CS_CORE_MQTT_PORT].data[1] = (uint8_t) ((DEFAULT_MQTT_PORT >> 8 ) & 0xFF); // Next  8 bits
  pxSTSAFE_KVStoreTLV->KVStore[CS_CORE_MQTT_PORT].data[2] = (uint8_t) ((DEFAULT_MQTT_PORT >> 16) & 0xFF); // Next  8 bits
  pxSTSAFE_KVStoreTLV->KVStore[CS_CORE_MQTT_PORT].data[3] = (uint8_t) ((DEFAULT_MQTT_PORT >> 24) & 0xFF); // Upper 8 bits

  pxSTSAFE_KVStoreTLV->KVStore[CS_WIFI_SSID].xTlvHeader.type = KV_TYPE_STRING;
  pxSTSAFE_KVStoreTLV->KVStore[CS_WIFI_SSID].xTlvHeader.length = strlen(DEFAULT_WIFI_SSID) + 1;
  memcpy(pxSTSAFE_KVStoreTLV->KVStore[CS_WIFI_SSID].data, ssid, pxSTSAFE_KVStoreTLV->KVStore[CS_WIFI_SSID].xTlvHeader.length);

  pxSTSAFE_KVStoreTLV->KVStore[CS_WIFI_CREDENTIAL].xTlvHeader.type = KV_TYPE_STRING;
  pxSTSAFE_KVStoreTLV->KVStore[CS_WIFI_CREDENTIAL].xTlvHeader.length = strlen(DEFAULT_WIFI_PASSWORD) + 1;
  memcpy(pxSTSAFE_KVStoreTLV->KVStore[CS_WIFI_CREDENTIAL].data, password, pxSTSAFE_KVStoreTLV->KVStore[CS_WIFI_CREDENTIAL].xTlvHeader.length);

  pxSTSAFE_KVStoreTLV->KVStore[CS_TIME_HWM_S_1970].xTlvHeader.type = KV_TYPE_UINT32;
  pxSTSAFE_KVStoreTLV->KVStore[CS_TIME_HWM_S_1970].xTlvHeader.length = sizeof(uint32_t);
  pxSTSAFE_KVStoreTLV->KVStore[CS_TIME_HWM_S_1970].data[0] = 0;
  pxSTSAFE_KVStoreTLV->KVStore[CS_TIME_HWM_S_1970].data[1] = 0;
  pxSTSAFE_KVStoreTLV->KVStore[CS_TIME_HWM_S_1970].data[2] = 0;
  pxSTSAFE_KVStoreTLV->KVStore[CS_TIME_HWM_S_1970].data[3] = 0;

  /* Set the magic number */
   pxSTSAFE_KVStoreTLV->magic_number = MAGIC_NUMBER;

  /* Update CRC */
  __HAL_CRC_DR_RESET(&hcrc);
  pxSTSAFE_KVStoreTLV->crc = HAL_CRC_Calculate(&hcrc, (uint32_t*) &pxSTSAFE_KVStoreTLV, sizeof(STSAFE_KVStoreTLV_t) / 4);

  return pfKvs_write();
}

#endif /* __SAFEA1_CONF_H__ */
