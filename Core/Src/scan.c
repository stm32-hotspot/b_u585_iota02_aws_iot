/*
 * scan.c
 *
 *  Created on: Dec 5, 2024
 *      Author: stred
 */

#include "logging_levels.h"
#define LOG_LEVEL         LOG_INFO
#include "logging.h"

#include "sys_evt.h"
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>

#include "mx_prv.h"

void vScanTask(void *pvParameters)
{
  IPCError_t xErr = IPC_ERROR_INTERNAL;

  /* Block until the network interface is connected */
  (void) xEventGroupWaitBits(xSystemEvents, EVT_MASK_NET_CONNECTED, 0x00, pdTRUE, portMAX_DELAY);

  while (1)
  {
    {
#define SCAN_BUFFER_LENGTH 512
      uint32_t ulScannLength = SCAN_BUFFER_LENGTH;
      char *pcScan;
      pcScan = pvPortMalloc(ulScannLength);
      memset(pcScan, 0, ulScannLength);

      xErr = mx_Scan(pcScan, ulScannLength, 10 * 1000);

      if (xErr == IPC_SUCCESS)
      {
        for (int i = 0; i < ulScannLength; i++)
        {
          if ((pcScan[i] == '-') || (pcScan[i] == '_') || (pcScan[i] == ' ') || (pcScan[i] == '[') || (pcScan[i] == ']') || (pcScan[i] == '.') || ((pcScan[i] >= 'a') && (pcScan[i] <= 'z')) || ((pcScan[i] >= 'A') && (pcScan[i] <= 'Z')) || ((pcScan[i] >= '0') && (pcScan[i] <= '9')))
          {
          }
          else
          {
            pcScan[i] = '\0';
          }
        }

        pcScan[ulScannLength - 1] = '\0';

        char *table[MAX_AP]; // Adjust the size as needed
        int index = 0;
        int index2 = 0;
        char *ptr = pcScan;

        while ((ptr < pcScan + SCAN_BUFFER_LENGTH) && (index2 < SCAN_BUFFER_LENGTH) && (index < MAX_AP))
        {
          if (*ptr != '\0')
          {
            table[index] = malloc(64); // Allocate memory for the string and copy it to the table
            int i = 0;
            do
            {
              table[index][i] = *ptr;
              i++;
              ptr++;
            } while (*ptr != '\0');

            if (i > 3)
            {
              index++;
            }
            else
            {
              vPortFree(table[index]);
            }
          }
          else
          {
            // Move the pointer to the next character
            ptr++;
          }

          index2++;
        }

        LogInfo("Number of networks: %d\r\n", index);

        for (int i = 0; i < index; i++)
        {
          LogInfo("SSID %d: %s\r\n", i, table[i]);
          vPortFree(table[i]); // Free the allocated memory
        }
      }

      vPortFree(pcScan);
    }

    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}
