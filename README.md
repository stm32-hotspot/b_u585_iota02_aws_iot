
# FreeRTOS IoT B-U585I-IOT02A STM32CubeMX AWS Reference
## Introduction
This project demonstrates how to integrate modular [ FreeRTOS kernel ](https://www.freertos.org/RTOS.html) and [ libraries ](https://www.freertos.org/libraries/categories.html). The project is pre-configured to run on the [ STM32U585 IoT Discovery Kit ](https://www.st.com/en/evaluation-tools/b-u585i-iot02a.html) which includes an kit which includes an [ STM32U5 ](https://www.st.com/en/microcontrollers-microprocessors/stm32u5-series.html) microcontroller.

The *Project* is a [Non-TrustZone](Projects/b_u585i_iot02a_ntz) project which  demonstrate connecting to AWS IoT Core and utilizing many of the services available via the MQTT protocol.





This includes demonstration tasks for the following AWS services:
* [AWS IoT Fleet Provisioning](https://docs.aws.amazon.com/iot/latest/developerguide/provision-wo-cert.html)
* [AWS IoT Device Shadow](https://docs.aws.amazon.com/iot/latest/developerguide/iot-device-shadows.html)
* [AWS IoT Device Defender](https://docs.aws.amazon.com/iot/latest/developerguide/device-defender.html)
* [AWS IoT OTA Update](https://docs.aws.amazon.com/freertos/latest/userguide/freertos-ota-dev.html)
* [AWS IoT Jobs](https://docs.aws.amazon.com/iot/latest/developerguide/iot-jobs.html)
* [MQTT File Delivery](https://docs.aws.amazon.com/iot/latest/developerguide/mqtt-based-file-delivery.html)



The demo project connect to AWS IoT core via the included Wi-Fi module and use the [CoreMQTT-Agent](https://github.com/FreeRTOS/coreMQTT-Agent) library to share a single MQTT connection among multiple tasks. These tasks publish data from a PubSub task, and demonstrate use of the AWS IoT Device Shadow and Device Defender services.

You can use [AWS Fleet Provisioning by Claim](https://aws.amazon.com/blogs/iot/how-to-automate-onboarding-of-iot-devices-to-aws-iot-core-at-scale-with-fleet-provisioning/) to automatically onboard IoT devices to AWS IoT Core

This project is based on the [iot-reference-stm32u5](https://github.com/FreeRTOS/iot-reference-stm32u5) with two major differences:
* Generated using STM32CubeMX and using CMSIS packs
* Enables Fleet provisioning.

## AWS IoT Core Demo Tasks
* MQTT Agent
* IoT Defender
* OTA Update
* Fleet Provisionong
* Publish and Subscribe
* EnvironmentSensor
* MotionSensors
* ShadowDevice

## Board provisioning

There are three methods to provision your board with AWS

### [Provision automatically with provision.py](https://github.com/FreeRTOS/iot-reference-stm32u5/blob/main/Getting_Started_Guide.md#option-8a-provision-automatically-with-provisionpy)

This method involves using a Python script (provision.py) to automate the onboarding process of IoT devices to AWS IoT Core. It simplifies the process by handling the device identity creation, registration, and policy attachment automatically.

### [Provision Manually via CLI](https://github.com/FreeRTOS/iot-reference-stm32u5/blob/main/Getting_Started_Guide.md#option-8b-provision-manually-via-cli)

This method requires manually provisioning devices using the AWS Command Line Interface (CLI). It involves creating device identities, registering them with AWS IoT Core, and attaching the necessary policies for device communication.

### [Use Fleet Provisioning](https://github.com/SlimJallouli/STM32_FleetProvisioning)
Fleet Provisioning is a feature of AWS IoT Core that automates the end-to-end device onboarding process. It securely delivers unique digital identities to devices, validates device attributes via Lambda functions, and sets up devices with all required permissions and registry metadata. This method is ideal for large-scale device deployments.



## Key Software Components
### LWIP  2.3.0 TCP/IP Stack
See [ lwIP ](https://www.keil.arm.com/packs/lwip-lwip/versions/) for details.

### Mbedtls 3.1.1 TLS and Cryptography library
See [ MbedTLS ](https://www.keil.arm.com/packs/mbedtls-arm/versions/) for details.

### Command Line Interface (CLI)
The CLI interface located in the Common/cli directory is used to provision the device. It also provides other Unix-like utilities. See [Common/cli](Common/cli/ReadMe.md) for details.

### Key-Value Store
The key-value store located in the Common/kvstore directory is used to store runtime configuration values in STM32's internal flash memory.
See [Common/kvstore](Common/kvstore/ReadMe.md) for details.

### PkiObject API
The PkiObject API takes care of some of the mundane tasks in converting between different representations of cryptographic objects such as public keys, private keys, and certificates. See [Common/crypto](Common/crypto/ReadMe.md) for details.

### Mbedtls Transport
The *Common/net/mbedtls_transport.c* file contains a transport layer implementation for coreMQTT and coreHTTP which uses mbedtls to encrypt the connection in a way supported by AWS IoT Core.

## Cloning the Repository
To clone using HTTPS:
```
git clone https://github.com/SlimJallouli/b_u585_iota02_aws_iot.git --recurse-submodules
```
Using SSH:
```
git clone git@github.com:SlimJallouli/b_u585_iota02_aws_iot.git --recurse-submodules
```
If you have downloaded the repo without using the `--recurse-submodules` argument, you should run:
```
git submodule update --init --recursive
```

## CMSIS Packs

If you need to regenerate the project with STM32CubeMX, then you need to dowload and install the following CMSIS packs.

[lwIP 2.3.0](https://www.keil.com/pack/lwIP.lwIP.2.3.0.pack)

[mbedTLS 3.1.1](https://www.keil.com/pack/ARM.mbedTLS.3.1.1.pack)

[AWS_IoT_Over-the-air_Update 5.0.1](https://d1pm0k3vkcievw.cloudfront.net/AWS.AWS_IoT_Over-the-air_Update.5.0.1.pack)

[AWS_IoT_Device_Defender 4.1.1](https://d1pm0k3vkcievw.cloudfront.net/AWS.AWS_IoT_Device_Defender.4.1.1.pack)

[AWS_IoT_Device_Shadow 5.0.1](https://d1pm0k3vkcievw.cloudfront.net/AWS.AWS_IoT_Device_Shadow.5.0.1.pack)

[AWS_IoT_Fleet_Provisioning 1.0.1](https://d1pm0k3vkcievw.cloudfront.net/AWS.AWS_IoT_Fleet_Provisioning.1.0.1.pack)

[backoffAlgorithm 4.1.1](https://d1pm0k3vkcievw.cloudfront.net/AWS.backoffAlgorithm.4.1.1.pack)

[coreJSON 4.1.1](https://d1pm0k3vkcievw.cloudfront.net/AWS.coreJSON.4.1.1.pack)

[coreMQTT 5.0.1](https://d1pm0k3vkcievw.cloudfront.net/AWS.coreMQTT.5.0.1.pack)

[coreMQTT_Agent 5.0.1](https://d1pm0k3vkcievw.cloudfront.net/AWS.coreMQTT_Agent.5.0.1.pack)

The following packs are automatically downloaded by STM32CubeMX

[X-CUBE-MEMS1](https://www.st.com/en/embedded-software/x-cube-mems1.html)

[X-CUBE-SAFEA1](https://www.st.com/en/embedded-software/x-cube-safea1.html)

[X-CUBE-FREERTOS](https://www.st.com/en/embedded-software/x-cube-freertos.html)


## Git submodules

[corePKCS11](https://github.com/FreeRTOS/corePKCS11)

[littlefs](https://github.com/littlefs-project/littlefs)

[tinycbor](https://github.com/intel/tinycbor)


## Generate the project using STM32CubeMX

After making changes with STM32CubeMX, be sure to run the **update.sh** script. Failure to do so will result in build errors.
