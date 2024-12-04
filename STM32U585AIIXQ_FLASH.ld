/*
******************************************************************************
**
**  File        : LinkerScript.ld
**
**  Author      : STM32CubeIDE
**
**  Abstract    : Linker script for STM32U575xI Device from STM32U5 series
**                      2048Kbytes FLASH
**                      784Kbytes RAM
**
**                Set heap size, stack size and stack location according
**                to application requirements.
**
**                Set memory bank area and size if external memory is used.
**
**  Target      : STMicroelectronics STM32
**
**  Distribution: The file is distributed as is without any warranty
**                of any kind.
**
*****************************************************************************
** @attention
**
** Copyright (c) 2023 STMicroelectronics.
** All rights reserved.
**
** This software is licensed under terms that can be found in the LICENSE file
** in the root directory of this software component.
** If no LICENSE file comes with this software, it is provided AS-IS.
**
*****************************************************************************
*/

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = ORIGIN(RAM) + LENGTH(RAM);	/* end of "RAM" Ram type memory */

_Min_Heap_Size = 0x200;	/* required amount of heap  */
_Min_Stack_Size = 0x400;	/* required amount of stack */

/*BOOTLOADER_SIZE     = 0x00010000;*//* 64K */
BOOTLOADER_SIZE     = 0x00000000;
FLASH_B1_ADDR       = 0x08000000 + BOOTLOADER_SIZE;
FLASH_B2_ADDR       = 0x08100000;

DEVICE_PVK_ADDR        = 0x000F2000 + FLASH_B1_ADDR;
DEVICE_PVK_SIZE_ADDR   = 0x000F3FF0 + FLASH_B1_ADDR;
DEVICE_PBK_ADDR        = 0x000F4000 + FLASH_B1_ADDR;
DEVICE_PBK_SIZE_ADDR   = 0x000F5FF0 + FLASH_B1_ADDR;
DEVICE_CERT_ADDR       = 0x000F6000 + FLASH_B1_ADDR;
DEVICE_CERT_SIZE_ADDR  = 0x000F7FF0 + FLASH_B1_ADDR;
ROOT_CA_ADDR           = 0x000F8000 + FLASH_B1_ADDR;
ROOT_CA_SIZE_ADDR      = 0x000F9FF0 + FLASH_B1_ADDR;
METADATA_ADDRESS       = 0x000FE000 + FLASH_B1_ADDR;

/* Memories definition */
MEMORY
{
  RAM (xrw)      : ORIGIN = 0x20000000,          LENGTH = 768K
  SRAM4 (xrw)    : ORIGIN = 0x28000000,          LENGTH = 16K

  FLASH (rx)     : ORIGIN = FLASH_B1_ADDR,       LENGTH = 960K /* Taking in accuont the secure bootloader  */
  
  /*OTA            : ORIGIN = FLASH_B2_ADDR,       LENGTH = 992K */ /* Section to receive OTA images            */
  OTA            : ORIGIN = FLASH_B2_ADDR,       LENGTH = 960K /* Section to receive OTA images            */
  /*********************************************************************************************************/
  DEVICE_PVK        : ORIGIN = DEVICE_PVK_ADDR         LENGTH = 4K   /* Section that holds the device private key*/
  DEVICE_PVK_SIZE   : ORIGIN = DEVICE_PVK_SIZE_ADDR    LENGTH = 16   /* 16 bytes resrved as  */
  
  DEVICE_CERT       : ORIGIN = DEVICE_CERT_ADDR         LENGTH = 4K   /* Section that holds the device certificate*/
  DEVICE_CERT_SIZE  : ORIGIN = DEVICE_CERT_SIZE_ADDR    LENGTH = 16   /* 16 bytes resrved as  */
  
  DEVICE_PBK        : ORIGIN = DEVICE_PBK_ADDR          LENGTH = 4K   /* Section that holds the device private key*/
  DEVICE_PBK_SIZE   : ORIGIN = DEVICE_PBK_SIZE_ADDR     LENGTH = 16   /* 16 bytes resrved as  */
  /*********************************************************************************************************/
  
  ROOT_CA        : ORIGIN = ROOT_CA_ADDR         LENGTH = 4K   /* Section that holds the AWS RootCA        */
  ROOT_CA_SIZE   : ORIGIN = ROOT_CA_SIZE_ADDR    LENGTH = 16   /* 16 bytes resrved as  */
   
  METADATA       : ORIGIN = METADATA_ADDRESS     LENGTH = 8K  /* Metadata section, SSID, Passphrase, ...   */
}

/* Sections */
SECTIONS
{
  /* The startup code into "FLASH" Rom type memory */
  .isr_vector :
  {
    KEEP(*(.isr_vector)) /* Startup code */
  } >FLASH

  /* The program code and other data into "FLASH" Rom type memory */
  .text :
  {
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    _etext = .;        /* define a global symbols at end of code */
  } >FLASH

  /* Constant data into "FLASH" Rom type memory */
  .rodata :
  {
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
  } >FLASH
  
  /*********************************************************************************************************/
  /* Device private key */
  .device_pvk :
  {
    _device_pvk_start = .;/* create a global symbol at data start */
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
  } >DEVICE_PVK
  
  /* Device private key size */
  .device_pvk_size :
  {
    _device_pvk_size_start = .; /* create a global symbol at data start */
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
  } >DEVICE_PVK_SIZE    
  
    /*********************************************************************************************************/
    /* Device public key */
  .device_pbk :
  {
    _device_pbk_start = .;/* create a global symbol at data start */
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
  } >DEVICE_PBK
  
  /* Device public key size */
  .device_pbk_size :
  {
    _device_pbk_size_start = .; /* create a global symbol at data start */
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
  } >DEVICE_PBK_SIZE   

  /*********************************************************************************************************/  
  /* Device Cert */
  .device_cert :
  {
    _device_cert_start = .;/* create a global symbol at data start */
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
  } >DEVICE_CERT
  
  /* Device Cert size */
  .device_cert_size :
  {
    device_cert_size_start = .; /* create a global symbol at data start */
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
  } >DEVICE_CERT_SIZE
   
  /*********************************************************************************************************/
  /* AmazonRootCA3 */
  .root_ca :
  {
    _root_ca_start = .;/* create a global symbol at data start */
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
  } >ROOT_CA
  
  /* AmazonRootCA3 size */
  .root_ca_size :
  {
    _root_ca_size_start = .; /* create a global symbol at data start */
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
  } >ROOT_CA_SIZE  
    
  /*********************************************************************************************************/
  /* Metadata */
  .metadata :
  {
    _metadata_start = .;/* create a global symbol at data start */
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
  } >METADATA  

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } >FLASH

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH

  /* Used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Initialized data sections into "RAM" Ram type memory */
  .data :
  {
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */
    *(.RamFunc)        /* .RamFunc sections */
    *(.RamFunc*)       /* .RamFunc* sections */

    _edata = .;        /* define a global symbol at data end */
  } >RAM AT> FLASH

  /* Uninitialized data section into "RAM" Ram type memory */
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >RAM

  /* User_heap_stack section, used to check that there is enough "RAM" Ram type memory left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >RAM

  /* Remove information from the compiler libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
