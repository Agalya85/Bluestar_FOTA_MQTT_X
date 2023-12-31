/*******************************************************************************
* Title                 :   Internal Flash Interface
* Filename              :   user_flash.h
* Author                :   Hrishikesh Limaye
* Origin Date           :   08/03/2022
* Version               :   1.0.0
* Compiler              :
* Target                :   STM32F437VITx - Tor4GEth
* Notes                 :   None
*
* Copyright (c) by KloudQ Technologies Limited.

  This software is copyrighted by and is the sole property of KloudQ
  Technologies Limited.
  All rights, title, ownership, or other interests in the software remain the
  property of  KloudQ Technologies Limited. This software may only be used in
  accordance with the corresponding license agreement. Any unauthorized use,
  duplication, transmission, distribution, or disclosure of this software is
  expressly forbidden.

  This Copyright notice may not be removed or modified without prior written
  consent of KloudQ Technologies Limited.

  KloudQ Technologies Limited reserves the right to modify this software
  without notice.
*
*
*******************************************************************************/


/*************** FILE REVISION LOG *****************************************
*
*    Date    Version   Author         	   Description
*  08/03/22   1.0.0    Hrishikesh Limaye   Initial Release.
*******************************************************************************/

/** @file user_flash.h
*  @brief Internal flash Implementation
*
*/

#ifndef USER_FLASH_H_
#define USER_FLASH_H_
/******************************************************************************
* Includes
*******************************************************************************/
#include "stm32f4xx_hal.h"
/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_12    ((uint32_t)0x08100000) /* Base @ of Sector 12, 16 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_13    ((uint32_t)0x08104000) /* Base @ of Sector 13, 16 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_14    ((uint32_t)0x08108000) /* Base @ of Sector 14, 16 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_15    ((uint32_t)0x0810C000) /* Base @ of Sector 15, 16 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_16    ((uint32_t)0x08110000) /* Base @ of Sector 16, 64 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_17    ((uint32_t)0x08120000) /* Base @ of Sector 17, 128 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_18    ((uint32_t)0x08140000) /* Base @ of Sector 18, 128 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_19    ((uint32_t)0x08160000) /* Base @ of Sector 19, 128 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_20    ((uint32_t)0x08180000) /* Base @ of Sector 20, 128 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_21    ((uint32_t)0x081A0000) /* Base @ of Sector 21, 128 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_22    ((uint32_t)0x081C0000) /* Base @ of Sector 22, 128 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
#define ADDR_FLASH_SECTOR_23    ((uint32_t)0x081E0000) /* Base @ of Sector 23, 128 Kbyte */
/**
* \brief Defines base address of the sector \a .
  \param None.
  \returns None \a .
*/
/* Sector 23 stores JUMPing details */
#define BOOTMEMORYINFO			(*((uint32_t *)  0x08100000))  /* Store info related to FW Boot point for FW Jump */

/* Define the address from where user application will be loaded.
   Note: the 1st sector 0x08000000-0x08003FFF is reserved for the IAP code */
#define APPLICATION_ADDRESS   (uint32_t)0x08008000
/* End of the Flash address */
#define USER_FLASH_END_ADDRESS        (ADDR_FLASH_SECTOR_23)
/* Define the user application size */
#define USER_FLASH_SIZE   (USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS + 1)

/* Define bitmap representing user flash area that could be write protected (check restricted to pages 8-39). */
#define FLASH_SECTOR_TO_BE_PROTECTED (OB_WRP_SECTOR_0 | OB_WRP_SECTOR_1 | OB_WRP_SECTOR_2 | OB_WRP_SECTOR_3 |\
                                      OB_WRP_SECTOR_4 | OB_WRP_SECTOR_5 | OB_WRP_SECTOR_6 | OB_WRP_SECTOR_7 |\
                                      OB_WRP_SECTOR_8 | OB_WRP_SECTOR_9 | OB_WRP_SECTOR_10 | OB_WRP_SECTOR_11 )

#define APPLICATION_ADD_X  (0x08004000)
#define APPLICATION_ADD_X_END (0x080E0000)

#define APPLICATION_ADD_Y  (0x08104000)
#define APPLICATION_ADD_Y_END (0x081E0000)

#define BOOT_MEM_X_JUMP			((uint32_t)0x58)
#define BOOT_MEM_Y_JUMP			((uint32_t)0x59)

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/**
* This Enumeration lists Flash operation errors
*/
/* Error code */
enum
{
  FLASHIF_OK = 0,
  FLASHIF_ERASEKO,
  FLASHIF_WRITINGCTRL_ERROR,
  FLASHIF_WRITING_ERROR

};
/**
* This Enumeration lists Flash protection(s)
*/
enum{
  FLASHIF_PROTECTION_NONE         = 0,
  FLASHIF_PROTECTION_PCROPENABLED = 0x1,
  FLASHIF_PROTECTION_WRPENABLED   = 0x2,
  FLASHIF_PROTECTION_RDPENABLED   = 0x4,
};

/**
* This Enumeration lists FW Jump region(s)
*/
typedef enum
{
	enmBOOTSEGMENT_X_JUMP = 0x58,
	enmBOOTSEGMENT_Y_JUMP = 0x59,
	enmBOOTSEGMENT_UNKNOWN
}enmBootMemorySegment;


/******************************************************************************
* Function Prototypes
*******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

void              FLASH_If_Init(void);
uint32_t          FLASH_If_Erase(uint32_t StartSector);
uint32_t          FLASH_If_Write(uint32_t FlashAddress, uint32_t* Data, uint32_t DataLength);
uint16_t          FLASH_If_GetWriteProtectionStatus(void);
HAL_StatusTypeDef FLASH_If_WriteProtectionConfig(uint32_t modifier);


uint32_t WritetoFlashMemory(uint8_t byte0,uint8_t byte1,uint8_t byte2,uint8_t byte3,uint32_t u32MemoryDestination);
uint32_t ConfigFLASHIfWrite(uint8_t *p_source, uint32_t length,uint32_t memorySector ,uint32_t writeCycle);
uint32_t WriteDatatoFlash(uint32_t Sector,uint8_t *data,uint32_t configLen,uint32_t writeCycle);

uint32_t flashWriteBootSection(enmBootMemorySegment memorySegment);
enmBootMemorySegment getNewFirmwareStorageLocation(void);
uint32_t FLASH_If_Erase_User(uint32_t start,enmBootMemorySegment memorySegment);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* USER_FLASH_H_ */
