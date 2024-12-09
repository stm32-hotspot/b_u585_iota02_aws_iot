/*
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
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#include "main.h"
#include "logging_levels.h"
#include "logging.h"
#include "kvstore_prv.h"
#include <string.h>
#include "semphr.h"

#include "key_value_store.h"

#ifdef KV_STORE_NVIMPL_INTERNAL

/*
 * @brief Get the length of a value stored in the KVStore implementation
 * @param[in] xKey Key to lookup
 * @return length of the value stored in the KVStore or 0 if not found.
 */
size_t xprvGetValueLengthFromImpl(KVStoreKey_t xKey)
{
  size_t xLength = 0;

  uint8_t value[256];
  size_t valueLength;

  switch (xKey)
  {
  case CS_CORE_THING_NAME:
    pfKvs_getKeyValue(CONF_KEY_THINGNAME, value, &valueLength);
    xLength = valueLength;
    break;

  case CS_CORE_MQTT_ENDPOINT:
    pfKvs_getKeyValue(CONF_KEY_ENDPOINT, value, &valueLength);
    xLength = valueLength;
    break;

  case CS_CORE_MQTT_PORT:
    pfKvs_getKeyValue(CONF_KEY_MQTT_PORT, value, &valueLength);
    xLength = valueLength;
    break;

  case CS_WIFI_SSID:
    pfKvs_getKeyValue(CONF_KEY_WIFI_SSID, value, &valueLength);
    xLength = valueLength;
    break;

  case CS_WIFI_CREDENTIAL:
    pfKvs_getKeyValue(CONF_KEY_WIFI_PASS, value, &valueLength);
    xLength = valueLength;
    break;

  case CS_TIME_HWM_S_1970:
    pfKvs_getKeyValue(CONF_KEY_TIME_HWM_S_1970, value, &valueLength);
    xLength = valueLength;
    break;

  case CS_NUM_KEYS:
    pfKvs_getKeyValue(CONF_KEY_TIME_NUM_KEYS, value, &valueLength);
    xLength = valueLength;
    break;

  default:
    break;
  }

  return xLength;
}

BaseType_t xprvReadValueFromImpl(KVStoreKey_t xKey, KVStoreValueType_t *pxType, size_t *pxLength, void *pvBuffer, size_t xBufferSize)
{
  BaseType_t lReturn = 0;

  uint8_t value[256];
  size_t valueLength = 256;

  explicit_bzero(value, valueLength);

  if ((pvBuffer == NULL) || (xBufferSize == 0))
  {
    lReturn = -1;
  }

  if (lReturn == 0)
  {
    if (xBufferSize < valueLength)
    {
      switch (xKey)
        {
        case CS_CORE_THING_NAME:
          *pxType = KV_TYPE_STRING;
          pfKvs_getKeyValue(CONF_KEY_THINGNAME, value, &valueLength);
          *pxLength = valueLength;
          strcpy((char *)pvBuffer, (char *)value);
          break;

        case CS_CORE_MQTT_ENDPOINT:
          *pxType = KV_TYPE_STRING;
          pfKvs_getKeyValue(CONF_KEY_ENDPOINT, value, &valueLength);
          *pxLength = valueLength;
          strcpy((char *)pvBuffer, (char *)value);
          break;

        case CS_CORE_MQTT_PORT:
          *pxType = KV_TYPE_UINT32;
          pfKvs_getKeyValue(CONF_KEY_MQTT_PORT, value, &valueLength);
          *pxLength = valueLength;
          memcpy((char *)pvBuffer, (char *)value, valueLength);
          break;

        case CS_WIFI_SSID:
          *pxType = KV_TYPE_STRING;
          pfKvs_getKeyValue(CONF_KEY_WIFI_SSID, value, &valueLength);
          *pxLength = valueLength;
          strcpy((char *)pvBuffer, (char *)value);
          break;

        case CS_WIFI_CREDENTIAL:
          *pxType = KV_TYPE_STRING;
          pfKvs_getKeyValue(CONF_KEY_WIFI_PASS, value, &valueLength);
          *pxLength = valueLength;
          strcpy((char *)pvBuffer, (char *)value);
          break;

        case     CS_TIME_HWM_S_1970:
          *pxType = KV_TYPE_UINT32;
          pfKvs_getKeyValue(CONF_KEY_TIME_HWM_S_1970, value, &valueLength);
          *pxLength = valueLength;
          memcpy((char *)pvBuffer, (char *)value, valueLength);
          break;

        case     CS_NUM_KEYS:
          *pxType = KV_TYPE_UINT32;
          pfKvs_getKeyValue(CONF_KEY_TIME_NUM_KEYS, value, &valueLength);
          *pxLength = valueLength;
          memcpy((char *)pvBuffer, (char *)value, valueLength);
          break;
        }
    }
  }

  return (lReturn);
}

/*
 * @brief Write a value for a given key to non-volatile storage.
 * @param[in] xKey Key to store the given value in.
 * @param[in] xType Type of value to record.
 * @param[in] xLength length of the value given in pxDataUnion.
 * @param[in] pxData Pointer to a buffer containing the value to be stored.
 * The caller must free any heap allocated buffers passed into this function.
 */
BaseType_t xprvWriteValueToImpl(KVStoreKey_t xKey, KVStoreValueType_t xType, size_t xLength, const void *pvData)
{
  BaseType_t lReturn = 1;
  void *pvBuffer = NULL;

  if ((xKey > CS_NUM_KEYS) || (xType == KV_TYPE_NONE) || (xLength < 0) || (pvData == NULL))
  {
    lReturn = 0;
  }

  if (lReturn)
  {
    pvBuffer = pvPortMalloc(xLength);

    if (pvBuffer == NULL)
    {
      configASSERT_CONTINUE(pvBuffer != NULL);
      lReturn = 0;
    }
  }

  if (lReturn)
  {
    (void) memcpy(pvBuffer, pvData, xLength);

    switch (xKey)
    {
    case CS_CORE_THING_NAME:
      lReturn = pfKvs_writeKeyValue(CONF_KEY_THINGNAME, pvBuffer, xLength);
      break;

    case CS_CORE_MQTT_ENDPOINT:
      lReturn = pfKvs_writeKeyValue(CONF_KEY_ENDPOINT, pvBuffer, xLength);
      break;

    case CS_CORE_MQTT_PORT:
      lReturn = pfKvs_writeKeyValue(CONF_KEY_MQTT_PORT, pvBuffer, xLength);
      break;

    case CS_WIFI_SSID:
      lReturn = pfKvs_writeKeyValue(CONF_KEY_WIFI_SSID, pvBuffer, xLength);
      break;

    case CS_WIFI_CREDENTIAL:
      lReturn = pfKvs_writeKeyValue(CONF_KEY_WIFI_PASS, pvBuffer, xLength);
      break;

    case CS_TIME_HWM_S_1970:
      lReturn = pfKvs_writeKeyValue(CONF_KEY_TIME_HWM_S_1970, pvBuffer, xLength);
      break;

    case CS_NUM_KEYS:
      lReturn = pfKvs_writeKeyValue(CONF_KEY_TIME_NUM_KEYS, pvBuffer, xLength);
      break;
    }
  }

  /*
   * Clear any sensitive data stored in ram temporarily
   * Free heap allocated buffer
   */
  if (pvBuffer != NULL)
  {
    explicit_bzero(pvBuffer, xLength);

    vPortFree(pvBuffer);
    pvBuffer = NULL;
  }

  return lReturn;
}

void vprvNvImplInit(void)
{
  pfKvs_init();
  /*TODO: Wait for filesystem initialization */
}
#endif /* KV_STORE_NVIMPL_LITTLEFS */
