/*******************************************************************************
* Title                 :   Internal ADC Interface using DMA
* Filename              :   user_adc.h
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

/** @file user_adc.h
*  @brief Internal ADC Implementation
*
*  This is the header file for the definition(s) related to Internal ADC
*  implementation
*/

#ifndef USER_ADC_H_
#define USER_ADC_H_
/******************************************************************************
* Includes
*******************************************************************************/
/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define ADC_VIN	(0)		/* Hardware Connection :  Vin_Sys */
#define ADC_CH1 (1)		/* Hardware Connection :  MCU ADC 1*/
#define ADC_CH2	(2)		/* Hardware Connection :  MCU ADC 2*/
#define ADC_CH3 (3)		/* Hardware Connection :  MCU ADC 3*/
#define ADC_CH4	(4)		/* Hardware Connection :  MCU ADC 4*/
/**
* \brief Defines diode drop in Vtg \a .
  \param None.
  \returns None \a .
*/
#define INPUT_DIODE_DROP	(float)(1.4)
/**
* \brief Defines Max supply vtg for the device \a .
  \param None.
  \returns None \a .
*/
#define MAX_INPUT_VTG			   (24)
/**
* \brief Defines Max battery vtg for the device \a .
  \param None.
  \returns None \a .
*/
#define MAX_BATT_VTG		(float)(4.2)
/**
* \brief Defines Adc value (max) \a .
  \param None.
  \returns None \a .
*/
#define ADC_RESOLUTION			   (2979)
/**
* \brief Defines Adc value (max for batt) \a .
  \param None.
  \returns None \a .
*/
#define ADC_RESOLUTION_BATT		   (2730)

/* System timings will be delayed if supply is below this value
 * Tracks Input supply to battery supply transition  */

/**
* \brief Defines Threshold for battery switch alert \a .
  \param None.
  \returns None \a .
*/
#define VIN_BATTERY_VTG		(float)(12)	/* System timings will be delayed if supply is switched to battery */

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/**
* This Enumeration lists ADC states
*/
typedef enum
{
	enmADC_IDLE,
	enmADC_GETDATA,
	enmADC_PROCESSDATA,
	enmADC_ERROR
}enmAdcState;
typedef struct
{
	uint32_t  ADC_Input_Voltage_Count;
	uint32_t  ADC_Input_1_Count;
	uint32_t  ADC_Input_2_Count;

	char	  u8array_ADC_IP_Vtg_Cnt[10];
	char	  u8array_ADC_IP1_Cnt[5];
	char	  u8array_ADC_IP2_Cnt[5];

	_Bool 	  bitFlag_ADC_DMA_STOPPED;

	float 	  ADC_Input_Voltage;
	float 	  ADC_Batt_Voltage;

}strctADC;
/******************************************************************************
* Function Prototypes
*******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

void operateadc(void);
void calculateInputVoltage(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* USER_ADC_H_ */

//***************************************** End of File ********************************************************
