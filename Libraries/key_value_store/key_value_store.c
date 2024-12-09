/**
 ******************************************************************************
 * @file           : key_value_store.c
 * @version        : v 1.0.0
 * @brief          : This file implements key_value_store
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * ththe "License"; You may not use this file except in compliance with the
 *                             opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "logging_levels.h"
#define LOG_LEVEL    LOG_INFO

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "kvstore_prv.h"
#include "key_value_store.h"
#include "flash_interface.h"

#ifdef KV_STORE_NVIMPL_INTERNAL

#if defined(NO_STSAFE)
#include "flash_interface.h"
#else
#include "stsafea_functions.h"
#include "stsafea.h"
#endif

#define BANK2_OFFSET 0x00100000
#include "certs.h"

#define METADATA_HEADER                "AWSMETADATA01" /* This is a Metadata configuration. */

#define ADDRESS_HEADER          0
#define ADDRESS_THINGNAME       (ADDRESS_HEADER          + CONFIG_MAX_LENGTH_HEADER         )
#define ADDRESS_ENDPOINT        (ADDRESS_THINGNAME       + CONFIG_MAX_LENGTH_THING_NAME     )
#define ADDRESS_SSID            (ADDRESS_ENDPOINT        + CONFIG_MAX_LENGTH_ENDPOINT       )
#define ADDRESS_PSWD            (ADDRESS_SSID            + CONFIG_MAX_LENGTH_WIFI_SSID      )
#define ADDRESS_MQTT_PORT       (ADDRESS_PSWD            + CONFIG_MAX_LENGTH_WIFI_PASS      )
#define ADDRESS_TIME_HWM_S_1970 (ADDRESS_MQTT_PORT       + CONFIG_MAX_LENGTH_MQTT_PORT      )
#define ADDRESS_NUM_KEYS        (ADDRESS_TIME_HWM_S_1970 + CONFIG_MAX_LENGTH_TIME_HWM_S_1970)
#define ADDRESS_CRC             (ADDRESS_NUM_KEYS        + CONFIG_MAX_LENGTH_NUM_KEYS       )

#define BUFFER_SIZE         (CONFIG_MAX_LENGTH_HEADER           + \
		                         CONFIG_MAX_LENGTH_THING_NAME       + \
                             CONFIG_MAX_LENGTH_ENDPOINT         + \
                             CONFIG_MAX_LENGTH_WIFI_SSID        + \
                             CONFIG_MAX_LENGTH_WIFI_PASS        + \
                             CONFIG_MAX_LENGTH_MQTT_PORT        + \
                             CONFIG_MAX_LENGTH_TIME_HWM_S_1970  + \
                             CONFIG_MAX_LENGTH_NUM_KEYS         + \
                             CONFIG_MAX_LENGTH_CRC)

static char metadata_buffer[BUFFER_SIZE] =
{ 0 };

#if defined(NO_STSAFE)
static const char metadat[BUFFER_SIZE] __attribute__((section(".metadata")));
#endif

static char *name     = &metadata_buffer[ADDRESS_THINGNAME      ];
static char *header   = &metadata_buffer[ADDRESS_HEADER         ];
static char *endpoint = &metadata_buffer[ADDRESS_ENDPOINT       ];
static char *ssid     = &metadata_buffer[ADDRESS_SSID           ];
static char *pswd     = &metadata_buffer[ADDRESS_PSWD           ];
static char *mqtt     = &metadata_buffer[ADDRESS_MQTT_PORT      ];
static char *time     = &metadata_buffer[ADDRESS_TIME_HWM_S_1970];
static char *keys     = &metadata_buffer[ADDRESS_NUM_KEYS       ];
static char *crc      = &metadata_buffer[ADDRESS_CRC            ];

#if (BUFFER_SIZE > FLASH_PAGE_SIZE)
#error BUFFER_SIZE bigger than  FLASH_PAGE_SIZE
#endif

//TODO: Set the default apn
#define DEFAULT_THING_NAME       "stm32u5-6eaa0920373033M6"
#define DEFAULT_WIFI_SSID        WIFI_SSID
#define DEFAULT_WIFI_PASSWORD    WIFI_CREDENTIAL
#define DEFAULT_AWS_IOT_ENDPOINT "a1qwhobjtvew8t-ats.iot.us-west-1.amazonaws.com"
#define DEFAULT_MQTT_PORT        8883
#define DEFAULT_TIME_HWM_S_1970  1733257942
#define DEFAULT_NUM_KEYS         1
extern CRC_HandleTypeDef hcrc;

metadata_t Metadata;

static bool metadata_get_data(metadata_t *pMetadata);
static bool metadata_set_data(void);
static bool metadata_erase_data(void);
static bool metadata_update(void);
static bool metadata_set_default(void);

/**
 * @brief  Ask user if they want to change config
 * @param  none
 * @retval status
 */
bool pfKvs_init(void)
{
  bool status;

  status = metadata_get_data(&Metadata);

  if (status == false)
  {
    status = metadata_set_default();

    if (status == true)
    {
      status = metadata_get_data(&Metadata);
    }
  }

  return status;
}

/**
 * @brief  Process the commands
 * @param  none
 * @retval status
 */
bool pfKvs_writeKeyValue(const char *key, const uint8_t *value, size_t valueLength)
{
  bool status = false;

  if (strcmp(key, CONF_KEY_ENDPOINT) == 0)
  {
    if (valueLength <= CONFIG_MAX_LENGTH_ENDPOINT)
    {
      memset(endpoint, 0, CONFIG_MAX_LENGTH_ENDPOINT);
      memcpy(endpoint, (char*) value, valueLength);
      status = metadata_update();
    }
  }
  else if (strcmp(key, CONF_KEY_WIFI_SSID) == 0)
  {
    if (valueLength <= CONFIG_MAX_LENGTH_WIFI_SSID)
    {
      memset(ssid, 0, CONFIG_MAX_LENGTH_WIFI_SSID);
      memcpy(ssid, (char*) value, valueLength);
      status = metadata_update();
    }
  }
  else if (strcmp(key, CONF_KEY_WIFI_PASS) == 0)
  {
    if (valueLength <= CONFIG_MAX_LENGTH_WIFI_PASS)
    {
      memset(pswd, 0, CONFIG_MAX_LENGTH_WIFI_PASS);
      memcpy(pswd, (char*) value, valueLength);
      status = metadata_update();
    }
  }
  else if (strcmp(key, CONF_KEY_THINGNAME) == 0)
  {
    if (valueLength <= CONFIG_MAX_LENGTH_THING_NAME)
    {
      memset(name, 0, CONFIG_MAX_LENGTH_THING_NAME);
      memcpy(name, (char*) value, valueLength);
      status = metadata_update();
    }
  }
  /**********************************************/
  else if (strcmp(key, CONF_KEY_MQTT_PORT) == 0)
  {
    if (valueLength <= CONFIG_MAX_LENGTH_THING_NAME)
    {
      memset(mqtt, 0, CONFIG_MAX_LENGTH_MQTT_PORT);
      memcpy(mqtt, value, valueLength);
      status = metadata_update();
    }
  }

  else if (strcmp(key, CONF_KEY_TIME_HWM_S_1970) == 0)
  {
    if (valueLength <= CONFIG_MAX_LENGTH_TIME_HWM_S_1970)
    {
      memset(time, 0, CONFIG_MAX_LENGTH_TIME_HWM_S_1970);
      memcpy(time, value, valueLength);
      status = metadata_update();
    }
  }

  else if (strcmp(key, CONF_KEY_TIME_NUM_KEYS) == 0)
  {
    if (valueLength <= CONFIG_MAX_LENGTH_NUM_KEYS)
    {
      memset(keys, 0, CONFIG_MAX_LENGTH_NUM_KEYS);
      memcpy(keys, value, valueLength);
      status = metadata_update();
    }
  }

  /**********************************************/
  else if (strcmp(key, CONF_KEY_ROOTCA) == 0)
  {
    FLASH_Erase((uint32_t) AmazonRootCA3);
    FLASH_Write((uint32_t) AmazonRootCA3, (uint32_t*) value, valueLength);
    status = FLASH_Write((uint32_t) &AmazonRootCA3_size, (uint32_t*) &valueLength, 4) == HAL_OK;
  }

  else if (strcmp(key, CONF_KEY_CERT) == 0)
  {
    FLASH_Erase((uint32_t) device_cert);
    FLASH_Write((uint32_t) device_cert, (uint32_t*) value, valueLength);
    status = FLASH_Write((uint32_t) &device_cert_size, (uint32_t*) &valueLength, 4) == HAL_OK;
  }

  else if (strcmp(key, CONF_KEY_PRV_KEY) == 0)
  {
    FLASH_Erase((uint32_t) device_pvk);
    FLASH_Write((uint32_t) device_pvk, (uint32_t*) value, valueLength);
    status = FLASH_Write((uint32_t) &device_pvk_size, (uint32_t*) &valueLength, 4) == HAL_OK;
  }

  else if (strcmp(key, CONF_KEY_PUB_KEY) == 0)
  {
    FLASH_Erase((uint32_t) device_pbk);
    FLASH_Write((uint32_t) device_pbk, (uint32_t*) value, valueLength);
    status = FLASH_Write((uint32_t) &device_pbk_size, (uint32_t*) &valueLength, 4) == HAL_OK;
  }

  if (status == true)
  {
    status = metadata_get_data(&Metadata);
  }

  return status;
}

bool pfKvs_getKeyValue(const char *key, uint8_t *value, size_t *valueLength)
{
  bool status = false;

  if (strcmp(key, CONF_KEY_ENDPOINT) == 0)
  {
    strcpy((char*) value, Metadata.pENDPOINT);
    *valueLength = strlen(Metadata.pENDPOINT) + 1;
    return true;
  }
  else if (strcmp(key, CONF_KEY_WIFI_SSID) == 0)
  {
    strcpy((char*) value, Metadata.pWIFI_SSID);
    *valueLength = strlen(Metadata.pWIFI_SSID) + 1;
    status = true;
  }
  else if (strcmp(key, CONF_KEY_WIFI_PASS) == 0)
  {
    strcpy((char*) value, Metadata.pWIFI_PASSWORD);
    *valueLength = strlen(Metadata.pWIFI_PASSWORD) + 1;
    status = true;
  }
  else if (strcmp(key, CONF_KEY_THINGNAME) == 0)
  {
    strcpy((char*) value, Metadata.pTHINGNAME);
    *valueLength = strlen(Metadata.pTHINGNAME) + 1;
    status = true;
  }
  /**********************************************/
  else if (strcmp(key, CONF_KEY_MQTT_PORT) == 0)
  {
      *(uint32_t *)value = *Metadata.pMQTT_PORT;
      *valueLength = 5;
      status = true;
  }
  else if (strcmp(key, CONF_KEY_TIME_HWM_S_1970) == 0)
  {
    *(uint32_t *)value = *Metadata.pTIME_HWM_S_1970;
      *valueLength = 5;
      status = true;
  }
  else if (strcmp(key, CONF_KEY_TIME_NUM_KEYS) == 0)
  {
    *(uint32_t *)value = *Metadata.pNUM_KEYS;
      *valueLength = 5;
      status = true;
  }
  /**********************************************/

  else if (strcmp(key, CONF_KEY_ROOTCA) == 0)
  {
    value = Metadata.pROOT_CERT;
    *valueLength = *Metadata.pROOT_CERT_SIZE;
    status = true;
  }
  else if (strcmp(key, CONF_KEY_CERT) == 0)
  {
    if (*Metadata.pCERT_SIZE != 0xFFFFFFFF)
    {
      value = Metadata.pCERT;
      *valueLength = Metadata.pCERT_SIZE;
      status = true;
    }
  }
  else if (strcmp(key, CONF_KEY_PRV_KEY) == 0)
  {
    if (*Metadata.pPRV_KEY_SIZE != 0xFFFFFFFF)
    {
      value = Metadata.pPRV_KEY;
      *valueLength = Metadata.pPRV_KEY_SIZE;
      status = true;
    }
  }
  else if (strcmp(key, CONF_KEY_PUB_KEY) == 0)
  {
    if (*Metadata.pPB_KEY_SIZE != 0xFFFFFFFF)
    {
      value = Metadata.pPB_KEY;
      *valueLength = *Metadata.pPB_KEY_SIZE;
      status = true;
    }
  }

  return status;
}

#if defined(NO_STSAFE)
static bool metadata_set_data(void)
{
  bool status = false;
  uint32_t *uwCRCValue;
  uint32_t crc_value1;

#if defined(HAL_ICACHE_MODULE_ENABLED)
  HAL_ICACHE_Disable();
#endif

#if defined (HAL_ICACHE_MODULE_ENABLED)
  HAL_DCACHE_Disable(&hdcache1);
#endif

  uwCRCValue = (uint32_t*) crc;

  __HAL_CRC_DR_RESET(&hcrc);

  crc_value1 = HAL_CRC_Calculate(&hcrc, (uint32_t*) metadata_buffer, (BUFFER_SIZE - CONFIG_MAX_LENGTH_CRC) / 4);

  *uwCRCValue = crc_value1;

  status = FLASH_Write((uint32_t) metadat, (uint32_t*) metadata_buffer, BUFFER_SIZE) == HAL_OK;

#if defined (HAL_DCACHE_MODULE_ENABLED)
  HAL_DCACHE_Enable(&hdcache1);
#endif

#if defined(HAL_ICACHE_MODULE_ENABLED)
  HAL_ICACHE_Enable();
#endif

  return status;
}

static bool metadata_erase_data(void)
{
  bool status;

  status = FLASH_Erase((uint32_t) metadat) == HAL_OK;

  return status;
}

static bool metadata_get_data(metadata_t *pMetadata)
{
  bool status = true;

#if defined(HAL_ICACHE_MODULE_ENABLED)
  HAL_ICACHE_Disable();
#endif

#if defined (HAL_ICACHE_MODULE_ENABLED)
  HAL_DCACHE_Disable(&hdcache1);
#endif

  memcpy(metadata_buffer, (char*) metadat, BUFFER_SIZE);

  if (strcmp(header, METADATA_HEADER) != 0)
  {
    status = false;
  }

  if (status)
  {
    pMetadata->pTHINGNAME       = name;
    pMetadata->pENDPOINT        = endpoint;
    pMetadata->pWIFI_SSID       = ssid;
    pMetadata->pWIFI_PASSWORD   = pswd;
    pMetadata->pMQTT_PORT       = (uint32_t*)mqtt;
    pMetadata->pTIME_HWM_S_1970 = (uint32_t*)time;
    pMetadata->pNUM_KEYS        = (uint32_t*)keys;
    pMetadata->pCRC             = (uint32_t*)crc;

    pMetadata->pROOT_CERT       = (char*)      (AmazonRootCA3     );
    pMetadata->pROOT_CERT_SIZE  = (uint32_t*) &(AmazonRootCA3_size);
    pMetadata->pCERT            = (char*)      (device_cert       );
    pMetadata->pCERT_SIZE       = (uint32_t*) &(device_cert_size  );
    pMetadata->pPRV_KEY         = (char*)      (device_pvk        );
    pMetadata->pPRV_KEY_SIZE    = (uint32_t*) &(device_pvk_size   );
    pMetadata->pPB_KEY          = (char*)      (device_pbk        );
    pMetadata->pPB_KEY_SIZE     = (uint32_t*) &(device_pbk_size   );

    __HAL_CRC_DR_RESET(&hcrc);

    uint32_t uwCRCValue;

    uwCRCValue = HAL_CRC_Calculate(&hcrc, (uint32_t*) metadata_buffer, (BUFFER_SIZE - CONFIG_MAX_LENGTH_CRC) / 4);

    if (uwCRCValue != *pMetadata->pCRC)
    {
      status = false;
    }
  }

#if defined (HAL_DCACHE_MODULE_ENABLED)
  HAL_DCACHE_Enable(&hdcache1);
#endif

#if defined(HAL_ICACHE_MODULE_ENABLED)
  HAL_ICACHE_Enable();
#endif

  return status;
}
#else
static bool metadata_set_data(void)
{
  uint32_t *uwCRCValue;
  uint32_t crc_value1;

  StSafeA_ResponseCode_t status;
  StSafeA_LVBuffer_t data = { .Length = (uint16_t) BUFFER_SIZE, .Data = (uint8_t*) metadata_buffer };

  uwCRCValue = (uint32_t*) crc;

  __HAL_CRC_DR_RESET(&hcrc);

  crc_value1 = HAL_CRC_Calculate(&hcrc, (uint32_t*) metadata_buffer, (BUFFER_SIZE - METADATA_MAX_LEN_CRC) / 4);

  *uwCRCValue = crc_value1;

  status = STSAFEA_Write_A_Zone((uint8_t) ZONE_2, 0, &data);

  return (status == STSAFEA_OK);
}

static bool metadata_erase_data(void)
{
  return true;
}

static bool metadata_get_data(metadata_t *pMetadata)
{
  bool status = false;
  StSafeA_LVBuffer_t data = { .Length = (uint16_t) BUFFER_SIZE, .Data = (uint8_t*) metadata_buffer };

  if (STSAFEA_Read_A_Zone((uint8_t) ZONE_2, 0, &data) == STSAFEA_OK)
  {
    status = true;
  }

  if (strcmp(header, METADATA_HEADER) != 0)
  {
    status = false;
  }

  if (status)
  {
    pMetadata->pENDPOINT = endpoint;
    pMetadata->pWIFI_SSID = ssid;
    pMetadata->pWIFI_PASSWORD = pswd;
    pMetadata->pAPN = apn;
    pMetadata->pCUSTM_NAME = custom_name;
    pMetadata->pDEFENDER = defender;
    pMetadata->pSHADOW_TOKEN = shadowToken;
    pMetadata->pCRC = (uint32_t*) crc;
    pMetadata->pROOT_CERT = (char*) AmazonRootCA3;
    pMetadata->pROOT_CERT_SIZE = (uint32_t*) &AmazonRootCA3_size;
    pMetadata->pHOTA_CERT = (char*) hota_cert;
    pMetadata->pHOTA_CERT_SIZE = (uint32_t*) &hota_cert_size;
    pMetadata->pOTA_CERT = (char*) ota_cert;
    pMetadata->pOTA_CERT_SIZE = (uint32_t*) &ota_cert_size;

    __HAL_CRC_DR_RESET(&hcrc);

    uint32_t uwCRCValue;

    uwCRCValue = HAL_CRC_Calculate(&hcrc, (uint32_t*) metadata_buffer, (BUFFER_SIZE - METADATA_MAX_LEN_CRC) / 4);

    if (uwCRCValue != *pMetadata->pCRC)
    {
      status = false;
    }
  }

  return status;
}
#endif
static bool metadata_update(void)
{
  snprintf(header, CONFIG_MAX_LENGTH_HEADER, "%s", METADATA_HEADER);
  metadata_erase_data();
  metadata_set_data();
  return true;
}

/**
 * @brief  Set the metadata to default.
 * @param  none 
 * @retval status
 */
static bool metadata_set_default(void)
{
  uint32_t *mqttValue;
  uint32_t *timeValue;
  uint32_t *keysValue;
  mqttValue = (uint32_t*)mqtt;
  timeValue = (uint32_t*)time;
  keysValue = (uint32_t*)keys;

  memset(metadata_buffer, 0xFF, BUFFER_SIZE);

  snprintf(header  , CONFIG_MAX_LENGTH_HEADER    , "%s", METADATA_HEADER         );
  snprintf(name    , CONFIG_MAX_LENGTH_THING_NAME, "%s", DEFAULT_THING_NAME      );
  snprintf(ssid    , CONFIG_MAX_LENGTH_WIFI_SSID , "%s", DEFAULT_WIFI_SSID       );
  snprintf(pswd    , CONFIG_MAX_LENGTH_WIFI_PASS , "%s", DEFAULT_WIFI_PASSWORD   );
  snprintf(endpoint, CONFIG_MAX_LENGTH_ENDPOINT  , "%s", DEFAULT_AWS_IOT_ENDPOINT);
  *mqttValue = DEFAULT_MQTT_PORT;
  *timeValue = DEFAULT_TIME_HWM_S_1970;
  *keysValue = DEFAULT_NUM_KEYS;


  return metadata_update();
}

#endif
