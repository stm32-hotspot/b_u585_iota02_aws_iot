/**
 ******************************************************************************
 * @file           : stsafe_key_value_store.h
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

#include <stdbool.h>
#include "kvstore.h"

#define STSAFE_KVSTORE_VAL_MAX_LEN 64

typedef struct
{
  KVStoreValueType_t type;
  size_t length; /* Length of value portion (excludes type and length fields */
} KVStoreTLVHeader_t;

bool pfKvs_writeKeyValue(KVStoreKey_t xKey, const uint8_t *value, KVStoreTLVHeader_t xTlvHeader);
bool pfKvs_getKeyValue  (KVStoreKey_t xKey,       uint8_t *value, KVStoreTLVHeader_t *pxTlvHeader);
bool pfKvs_getKeyLength (KVStoreKey_t xKey, KVStoreTLVHeader_t *pxTlvHeader);
bool pfKvs_init(void);
#endif /* PLATFORM_KVS */
