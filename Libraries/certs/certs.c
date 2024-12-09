/*
 * certs.c
 *
 *  Created on: Mar 6, 2024
 *      Author: stred
 */
#include "main.h"

#include "kvstore_prv.h"

#ifdef KV_STORE_NVIMPL_INTERNAL
const char __attribute__((section(".root_ca"))) AmazonRootCA3[] = {};

const uint32_t __attribute__((section(".root_ca_size"))) AmazonRootCA3_size = sizeof(AmazonRootCA3);

#if defined(NO_STSAFE)
/**************************** Device PVK  ***************************/
const char __attribute__((section(".device_pvk"))) device_pvk[]={};

const uint32_t __attribute__((section(".device_pvk_size"))) device_pvk_size  = sizeof(device_pvk);

/**************************** Device PBK  ***************************/
const char __attribute__((section(".device_pbk"))) device_pbk[]={};

const uint32_t __attribute__((section(".device_pbk_size"))) device_pbk_size  = sizeof(device_pbk);

/**************************** Device cert  ***************************/
const char __attribute__((section(".device_cert"))) device_cert[]={};

const uint32_t __attribute__((section(".device_cert_size"))) device_cert_size  = sizeof(device_cert);
#endif

#endif /* NO_STSAFE */

