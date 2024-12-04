/**
 ******************************************************************************
 * @file           : key_value_store.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PLATFORM_KVS
#define PLATFORM_KVS

#include "main.h"
#include <stdbool.h>

#define CONFIG_MAX_LENGTH_HEADER            ( 16   )
#define CONFIG_MAX_LENGTH_THING_NAME        ( 128U )
#define CONFIG_MAX_LENGTH_ENDPOINT          ( 128U )
#define CONFIG_MAX_LENGTH_WIFI_SSID         ( 32U  )
#define CONFIG_MAX_LENGTH_WIFI_PASS         ( 64U  )
#define CONFIG_MAX_LENGTH_MQTT_PORT         ( 16   )
#define CONFIG_MAX_LENGTH_TIME_HWM_S_1970   ( 16   )
#define CONFIG_MAX_LENGTH_NUM_KEYS          ( 16   )
#define CONFIG_MAX_LENGTH_CRC               ( 4    )

#define CONF_KEY_ENDPOINT        "Endpoint"
#define CONF_KEY_WIFI_SSID       "SSID"
#define CONF_KEY_WIFI_PASS       "Passphrase"
#define CONF_KEY_THINGNAME       "ThingName"
#define CONF_KEY_MQTT_PORT       "MQTT_PORT"
#define CONF_KEY_TIME_HWM_S_1970 "TIME_HWM_S_1970"
#define CONF_KEY_TIME_NUM_KEYS   "NUM_KEYS"
#define CONF_KEY_ROOTCA          "RootCA"
#define CONF_KEY_CERT            "Certificate"
#define CONF_KEY_PRV_KEY         "PrvKey"
#define CONF_KEY_PUB_KEY         "PubKey"

typedef struct metadata_t
{
  char *pTHINGNAME;
  char *pENDPOINT;
  char *pWIFI_SSID;
  char *pWIFI_PASSWORD;
  uint32_t *pMQTT_PORT;
  uint32_t *pTIME_HWM_S_1970;
  uint32_t *pNUM_KEYS;
  uint32_t *pCRC;
  char *pROOT_CERT;
  uint32_t *pROOT_CERT_SIZE;
  char *pCERT;
  uint32_t *pCERT_SIZE;
  char *pPRV_KEY;
  uint32_t *pPRV_KEY_SIZE;
  char *pPB_KEY;
  uint32_t *pPB_KEY_SIZE;
} metadata_t;

bool pfKvs_writeKeyValue(const char *key, const uint8_t *value, size_t   valueLength);
bool pfKvs_getKeyValue  (const char *key,       uint8_t *value, size_t * valueLength);
bool pfKvs_init(void);
#endif /* PLATFORM_KVS */
