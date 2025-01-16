#include "main.h"
#include "FreeRTOS.h"

#if defined(__SAFEA1_CONF_H__)
#include "safea1_conf.h"
#include "stsafea_core.h"
#include "stsafea_types.h"

/* Buffer used to STSAFE-A Simple Drivers for I2C buss communications */
static uint8_t  cmd_resp_buffer[ STSAFEA_BUFFER_DATA_PACKET_SIZE ];

static StSafeA_Handle_t stsafea_handle;
static uint8_t a_rx_tx_stsafea_data [STSAFEA_BUFFER_MAX_SIZE];

StSafeA_ResponseCode_t SAFEA1_Init(void)
{
  StSafeA_ResponseCode_t readstatus = STSAFEA_UNEXPECTED_ERROR;

  readstatus = StSafeA_Init(&stsafea_handle, a_rx_tx_stsafea_data);

  return readstatus;
}

uint8_t SAFEA1_GenerateRandom(uint8_t size, uint8_t *random)
{
  if (random == NULL)
  {
    return pdFALSE;
  }

  StSafeA_LVBuffer_t TrueRandom;
  TrueRandom.Data = random;

  if(((uint8_t)StSafeA_GenerateRandom(&stsafea_handle, STSAFEA_EPHEMERAL_RND, size, &TrueRandom, STSAFEA_MAC_NONE)) == STSAFEA_OK)
  {
    return pdTRUE;
  }
  else
  {
    return pdFALSE;
  }
}

#endif
