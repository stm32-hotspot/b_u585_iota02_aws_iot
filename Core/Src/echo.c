/*
 * echo.c
 *
 *  Created on: Dec 5, 2024
 *      Author: stred
 */

#include "logging.h"

#include "sys_evt.h"
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>

#include "lwip/sockets.h"

#define ECHO_PORT 7
#define REMOTE_IP_ADDRESS "192.168.1.31"
//#define REMOTE_IP_ADDRESS "192.168.1.25"
//#define REMOTE_IP_ADDRESS "192.168.137.173"

static void lwip_socket_send(const char *message, const char *dest_ip, uint16_t dest_port);

char buffer1[1024];
char buffer2[1024];

void vEchoReceiverTask(void *pvParameters)
{
  int sock = *(int*) pvParameters;

  int bytes_read;

  (void) pvParameters;

  while ((bytes_read = lwip_recv(sock, buffer2, sizeof(buffer2), 0)) > 0)
  {
    LogInfo("Data received size %d\n", bytes_read);
    buffer2[bytes_read] = '\0';
    LogInfo("Data received %s\n", buffer2);

    lwip_send(sock, buffer2, bytes_read, 0);
  }

  lwip_close(sock);
  vTaskDelete(NULL);
}

static void lwip_socket_send(const char *message, const char *dest_ip, uint16_t dest_port)
{
  int sock = lwip_socket(AF_INET, SOCK_STREAM, 0);

  if (sock < 0)
  {
    LogError("Error: Unable to create socket");
    return;
  }

  struct sockaddr_in dest_addr;
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = lwip_htons(dest_port);
  dest_addr.sin_addr.s_addr = inet_addr(dest_ip);

  if (lwip_connect(sock, (struct sockaddr*) &dest_addr, sizeof(dest_addr)) < 0)
  {
    LogError("Error: Unable to connect to server\n");
    lwip_close(sock);
    return;
  }

  ssize_t sent_bytes = lwip_send(sock, message, strlen(message), 0);

  if (sent_bytes < 0)
  {
    LogError("Error: Unable to send message");
  }
  else
  {
    LogInfo("%d bytes sent to %s:%d", sent_bytes, dest_ip, dest_port);

    ssize_t received_bytes = lwip_recv(sock, buffer1, sizeof(buffer1), 0);

    if (received_bytes < 0)
    {
      LogError("Error: Unable to receive message\n");
    }
    else if (received_bytes == 0)
    {
      LogError("Connection closed by server\n");
    }
    else
    {
      buffer1[received_bytes] = '\0';
      LogInfo("Received: %s\n", buffer1);
    }
  }

  lwip_close(sock);
}

void vEchoSenderTask(void *pvParameters)
{
  uint16_t message_id = 0;

  (void) pvParameters;

  for (;;)
  {
    // Perform main task operations
    char msg[256];
    snprintf(msg, sizeof(msg), "Aware lwip socket test msg id: %d", message_id++);
    lwip_socket_send(msg, REMOTE_IP_ADDRESS, ECHO_PORT);

    vTaskDelay(pdMS_TO_TICKS(5000));
  }

  LogError("Error: We should never be here");
}

void vEchoServerTask(void *pvParameters)
{
  int sock, new_sock;
  struct sockaddr_in address, client;
  socklen_t client_len = sizeof(client);

  (void) pvParameters;

  vTaskDelay(pdMS_TO_TICKS(20000));

  // Create socket
  sock = lwip_socket(AF_INET, SOCK_STREAM, 0);

  if (sock < 0)
  {
    LogError("Socket creation failed\n");
    vTaskDelete(NULL);
    return;
  }

  // Bind socket to port 7 (echo protocol)
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(ECHO_PORT);

  if (lwip_bind(sock, (struct sockaddr*) &address, sizeof(address)) < 0)
  {
    LogError("Socket bind failed\n");
    lwip_close(sock);
    vTaskDelete(NULL);
    return;
  }

  // Listen for incoming connections
  if (lwip_listen(sock, 5) < 0)
  {
    LogError("Socket listen failed\n");
    lwip_close(sock);
    vTaskDelete(NULL);
    return;
  }

  LogInfo("Echo server is listening on port %d\n", ECHO_PORT);

  xTaskCreate(vEchoSenderTask, "SenderTask", 2 * configMINIMAL_STACK_SIZE, &new_sock, tskIDLE_PRIORITY + 1, NULL);

  while (1)
  {
    // Accept incoming connection
    new_sock = lwip_accept(sock, (struct sockaddr*) &client, &client_len);

    if (new_sock < 0)
    {
      LogError("Socket accept failed\n");
      lwip_close(sock);
      vTaskDelete(NULL);
      return;
    }

    // Create tasks for sending and receiving
    xTaskCreate(vEchoReceiverTask, "ReceiverTask", configMINIMAL_STACK_SIZE, &new_sock, tskIDLE_PRIORITY + 1, NULL);
  }

  lwip_close(sock);
  vTaskDelete(NULL);
}
