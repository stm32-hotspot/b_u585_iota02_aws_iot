
# FreeRTOS IoT B-U585I-IOT02A STM32CubeMX AWS Reference
## 1. Introduction
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

You can provision devices at scale using AWS Fleet Provisioning by Claim or Multi-Account Registration to automatically onboard IoT devices to AWS IoT Core.

With these methods, you can streamline the process of registering and managing a large number of IoT devices, ensuring they are securely connected and properly configured for AWS IoT Core. This automation helps reduce the time and effort required for device provisioning, especially when dealing with extensive IoT deployments.

This project is based on the [iot-reference-stm32u5](https://github.com/FreeRTOS/iot-reference-stm32u5) with three major differences:
* Generated using STM32CubeMX and use CMSIS packs
* Enables Fleet provisioning.
* Enables Multi-Account Registration using STSAFE

## 2. AWS IoT Core Demo Tasks
* MQTT Agent
* IoT Defender
* OTA Update
* Fleet Provisionong
* Publish and Subscribe
* EnvironmentSensor
* MotionSensors
* ShadowDevice

## 3. Board provisioning

There are multiple methods to provision your board with AWS

### 3.1 [Single Thing Provisioning](https://docs.aws.amazon.com/iot/latest/developerguide/single-thing-provisioning.html)

[Single Thing Provisioning](https://docs.aws.amazon.com/iot/latest/developerguide/single-thing-provisioning.html), is a method used to provision individual IoT devices in AWS IoT Core. This method is ideal for scenarios where you need to provision devices one at a time.

### 3.2 [Fleet Provisioning](https://docs.aws.amazon.com/iot/latest/developerguide/provision-wo-cert.html#claim-based)
[Fleet Provisioning](https://docs.aws.amazon.com/iot/latest/developerguide/provision-wo-cert.html#claim-based) is a feature of AWS IoT Core that automates the end-to-end device onboarding process. It securely delivers unique digital identities to devices, validates device attributes via Lambda functions, and sets up devices with all required permissions and registry metadata. This method is ideal for large-scale device deployments.

### 3.3 [Multi-Account Registration](https://aws.amazon.com/about-aws/whats-new/2020/04/simplify-iot-device-registration-and-easily-move-devices-between-aws-accounts-with-aws-iot-core-multi-account-registration/)
[Multi-Account Registration](https://aws.amazon.com/about-aws/whats-new/2020/04/simplify-iot-device-registration-and-easily-move-devices-between-aws-accounts-with-aws-iot-core-multi-account-registration/) registration method uses a secure element [(STSAFE)](https://www.st.com/en/secure-mcus/stsafe-a110.html) for added security. The device certificate, private key, and configuration parameters are saved on [(STSAFE)](https://www.st.com/en/secure-mcus/stsafe-a110.html). This method simplifies device registration and allows for easy movement of devices between multiple AWS accounts. It eliminates the need for a Certificate Authority (CA) to be registered with AWS IoT. The secure element provides additional security by storing sensitive information securely on the device. This method is ideal for large-scale device deployments.


## 4. Key Software Components
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

## 5. Get started with the project

### 5.1 Cloning the Repository
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

### 5.2 Build the project
* Import the project with [STM32CubeIDE](http://www.st.com/stm32cubeide)
* Select the provisioning configuration using the drop-down menu
* Build the project
* Flash the board

![alt text](<Screenshot 2025-02-26 175312-1.png>)
### 5.3 Provision your board
There are three methods to provision your board with AWS

#### [Single Thing Provisioning](https://docs.aws.amazon.com/iot/latest/developerguide/single-thing-provisioning.html)

In this method you have two options. Automated using Python script or manual.

1. [Provision automatically with provision.py](https://github.com/FreeRTOS/iot-reference-stm32u5/blob/main/Getting_Started_Guide.md#option-8a-provision-automatically-with-provisionpy)

This method involves using a Python script [(provision.py)](https://github.com/FreeRTOS/iot-reference-stm32u5/blob/main/tools/provision.py) to automate the onboarding process of IoT devices to AWS IoT Core. It simplifies the process by handling the device identity creation, registration, and policy attachment automatically. follow this [link](https://github.com/FreeRTOS/iot-reference-stm32u5/blob/main/Getting_Started_Guide.md#option-8a-provision-automatically-with-provisionpy) for instructions

2. [Provision Manually via CLI](https://github.com/FreeRTOS/iot-reference-stm32u5/blob/main/Getting_Started_Guide.md#option-8b-provision-manually-via-cli)

This method requires manually provisioning devices using the AWS Command Line Interface (CLI). It involves creating device identities, registering them with AWS IoT Core, and attaching the necessary policies for device communication. Follow this  [link](https://github.com/FreeRTOS/iot-reference-stm32u5/blob/main/Getting_Started_Guide.md#option-8b-provision-manually-via-cli) for instructions.

#### [Fleet Provisioning](https://docs.aws.amazon.com/iot/latest/developerguide/provision-wo-cert.html#claim-based)
This method is ideal for large-scale device deployments. Follow this [link](https://github.com/SlimJallouli/STM32_FleetProvisioning) for instructions

#### [Multi-Account Registration](https://aws.amazon.com/about-aws/whats-new/2020/04/simplify-iot-device-registration-and-easily-move-devices-between-aws-accounts-with-aws-iot-core-multi-account-registration/)
This method is ideal for large-scale device deployments with added security. Follow this [link]() for instructions

## 6. CMSIS Packs

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


## 7. Git submodules

[corePKCS11](https://github.com/FreeRTOS/corePKCS11)

[littlefs](https://github.com/littlefs-project/littlefs)

[tinycbor](https://github.com/intel/tinycbor)


## 8. Generate the project using STM32CubeMX

After making changes with STM32CubeMX, be sure to run the **update.sh** script. Failure to do so will result in build errors.
