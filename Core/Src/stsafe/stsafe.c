#include "main.h"

#if defined(__SAFEA1_CONF_H__)
#include "safea1_conf.h"
#include "stsafea_core.h"
#include "stsafea_types.h"

/* Buffer used to STSAFE-A Simple Drivers for I2C buss communications */
static uint8_t  cmd_resp_buffer[ STSAFEA_BUFFER_DATA_PACKET_SIZE ];

StSafeA_Handle_t stsafea_handle;
uint8_t a_rx_tx_stsafea_data [STSAFEA_BUFFER_MAX_SIZE];

StSafeA_ResponseCode_t SAFEA1_Init(void)
{
  StSafeA_ResponseCode_t readstatus = STSAFEA_UNEXPECTED_ERROR;

  readstatus = StSafeA_Init(&stsafea_handle, a_rx_tx_stsafea_data);

  return readstatus;
}
#endif
