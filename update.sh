#!/bin/bash

# Define source and destination directories
HOME="C:/Users/stred"

mbedTLS_source="$HOME/STM32Cube/Repository/Packs/ARM/mbedTLS/3.1.1/library/"
mbedTLS_destination="./Middlewares/Third_Party/ARM_Security/"

LwIP_source="$HOME/STM32Cube/Repository/Packs/lwIP/lwIP/2.2.0/lwip/src/apps/"
LwIP_destination="./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/"

FreeRTOSConfig_source="./FreeRTOSConfig.h"
FreeRTOSConfig_destination="./Core/Inc/FreeRTOSConfig.h"

# Create the destination directory if it doesn't exist
if [ ! -d "$mbedTLS_destination" ]; then
    mkdir -p "$mbedTLS_destination"
fi

echo "Home : " $HOME

# Copy the contents from mbedTLS_source to mbedTLS_destination
cp -r "$mbedTLS_source" "$mbedTLS_destination"
echo "Contents copied from $mbedTLS_source to $mbedTLS_destination"

# Copy the contents from LwIP_source to LwIP_destination
cp -r "$LwIP_source" "$LwIP_destination"
echo "Contents copied from $LwIP_source to $LwIP_destination"

# Force copy the FreeRTOSConfig.h file to the destination
# cp -f "$FreeRTOSConfig_source" "$FreeRTOSConfig_destination"
# echo "File $FreeRTOSConfig_source copied to $FreeRTOSConfig_destination"

# Delete the files
rm -f "Middlewares/Third_Party/lwIP_Network_lwIP/lwip/apps/altcp_tls_mbedtls.c"
rm -f "Middlewares/Third_Party/lwIP_Network_lwIP/lwip/apps/altcp_tls_mbedtls_mem.c"
rm -f "Middlewares/Third_Party/lwIP_Network_lwIP/lwip/apps/altcp_tls_mbedtls_mem.h"
rm -f "Middlewares/Third_Party/lwIP_Network_lwIP/lwip/apps/altcp_tls_mbedtls_structs.h"



