#!/bin/bash

# Define source and destination directories
HOME=~

mbedTLS_source="$HOME/STM32Cube/Repository/Packs/ARM/mbedTLS/3.1.1/library/"
mbedTLS_destination="./Middlewares/Third_Party/ARM_Security/"

LwIP_source="$HOME/STM32Cube/Repository/Packs/lwIP/lwIP/2.2.0/lwip/src/apps/"
LwIP_destination="./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/"

# FreeRTOSConfig_source="./FreeRTOSConfig.h"
# FreeRTOSConfig_destination="./Core/Inc/FreeRTOSConfig.h"

# Create the destination directory if it doesn't exist
if [ ! -d "$mbedTLS_destination" ]; then
    mkdir -p "$mbedTLS_destination"
fi

echo "Home : " $HOME

# Copy the contents from mbedTLS_source to mbedTLS_destination
cp -r "$mbedTLS_source" "$mbedTLS_destination"
echo "Content copied from $mbedTLS_source to $mbedTLS_destination"

# Copy the contents from LwIP_source to LwIP_destination
# cp -r "$LwIP_source" "$LwIP_destination"
# echo "Content copied from $LwIP_source to $LwIP_destination"



FILE_PATH="./Core/Src/sysmem.c"
echo "Deleting $FILE_PATH"
rm -f $FILE_PATH

FILE_PATH="./Middlewares/Third_Party/lwIP_Network_lwIP/ports/freertos/sys_arch.c"
echo "Deleting $FILE_PATH"
rm -f $FILE_PATH

FOLDER_PATH="./Middlewares/Third_Party/lwIP_Network_lwIP/ports/freertos/include/arch"
echo "Deleting $FOLDER_PATH"
rm -rf $FOLDER_PATH

FOLDER_PATH="./Middlewares/Third_Party/lwIP_Network_lwIP/rte"
echo "Deleting $FOLDER_PATH"
rm -rf $FOLDER_PATH

FILE_PATH="./Src/sysmem.c"
echo "Deleting $FILE_PATH"
rm -f $FILE_PATH

FOLDER_PATH="./Libraries/littlefs/runners"
echo "Deleting $FOLDER_PATH"
rm -rf $FOLDER_PATH

FOLDER_PATH="./Libraries/corePKCS11/test"
echo "Deleting $FOLDER_PATH"
rm -rf $FOLDER_PATH
