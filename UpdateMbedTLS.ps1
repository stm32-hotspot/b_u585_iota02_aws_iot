# Define source and destination directories
$source = "$env:USERPROFILE\STM32Cube\Repository\Packs\ARM\mbedTLS\3.1.1\library"
$destination = ".\Middlewares\Third_Party\ARM_Security\library"
$fileToCopy = ".\FreeRTOSConfig.h"
$destinationFile = ".\Core\Inc\FreeRTOSConfig.h"

# Create the destination directory if it doesn't exist
if (-not (Test-Path -Path $destination)) {
    New-Item -ItemType Directory -Path $destination
}

# Copy the contents from source to destination
Copy-Item -Path $source\* -Destination $destination -Recurse -Force

# Force copy the FreeRTOSConfig.h file to the destination
Copy-Item -Path $fileToCopy -Destination $destinationFile -Force

Write-Output "Contents copied from $source to $destination"
Write-Output "File $fileToCopy copied to $destinationFile"
