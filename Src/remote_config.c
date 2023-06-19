/*******************************************************************************
* Title                 :   Application Remote Configuration Handling
* Filename              :   remote_config.c
* Author                :   Hrishikesh Limaye
* Origin Date           :   08/03/2022
* Version               :   1.0.0
* Compiler              :
* Target                :   STM32F437 - Tor4Eth
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
*    Date    Version   Author         	  Description
*  08/03/22   1.0.0    Hrishikesh Limaye   Initial Release.
*
*******************************************************************************/

/** @file  remote_config.c
 *  @brief Application Remote Configuration Handling Functions
 */

/******************************************************************************
* Includes
*******************************************************************************/

#include "externs.h"
#include "remote_configuration.h"
#include "string.h"
#include <stdio.h>
#include<stdlib.h>
#include "ip_addr.h"
#include "user_flash.h"
#include "user_timer.h"
#include <ip4_addr.h>
#include <inet.h>
#include "payload.h"

/* Example String
 * (,2,5400968468775475453211317,NS,1,http://59.163.219.179:8025/api/Values/PostStringData,http://59.163.219.178:81/modbustwowaycomm/api/insertData/getData,airteliot.com,60000,100000,1,1,0,30000,NE,MS,1,1,1,44,9600,1,1,0,1,3,8,1,17,25,33,201,205,306,315,327,375,668,673,678,680,685,758,301,309,319,323,325,348,359,361,373,375,387,391,394,397,399,401,403,409,439,455,471,502,505,511,521,1,1,1,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,4,4,15,7,6,7,3,1,3,6,8,2,1,1,1,4,4,1,3,9,2,1,14,9,1,4,1,10,3,2,2,1,1,1,2,3,1,2,2,1,1,1,1,2000,2000,ME,ms,1,5,192.168.000.100,255.255.255.000,192.168.000.200,192.168.000.1,502,2,2,2,2,2,4,4,4,4,4,7,6,5,4,3,1000,2000,me,)
 *
 * */

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
/**
 * Doxygen tag for documenting variables and constants
 */
#define CONFIG_MB485_MAXPARAMS (75)
#define CONFIG_MBTCP_MAXPARAMS (CONFIG_MB485_MAXPARAMS)
#define CONFIG_NW_PARAM	(11)
#define CONFIG_MB485_PARAM	(11)
/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define IP_IS_TYPE_V4(ipaddr)   (IP_GET_TYPE(&ipaddr) == IPADDR_TYPE_V4 )
/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
char gau8RemoteDeviceID[40]={'0'};
char gau8RemoteDeviceType[2] = "";
uint32_t gu32ConfigLen = 0;
uint32_t gu32ParseConfigAvailable = 0;
uint32_t gu32ConfigOperationStatus = FALSE;
uint32_t gu32ExtractParamCounter = 0;

/* Network Configuration Update Variables */
char gau8ConfigNewtorkIsEnabled[2] = "";
char gau8ConfigNewtorkServerURL[150] = "";
char gau8ConfigNewtorkConfigURL[150] = "";
char gau8ConfigNewtorkAPN[50] = "airteliot.com";
char gau8ConfigNewtorkUpFreqBatt[10] = "";
char gau8ConfigNewtorkUpFreqSupply[10] = "";
char gau8ConfigNewtorkType[2] = "";
char gau8ConfigNewtorkGPSEnable[2] = "";
char gau8ConfigNewtorkSendData[2] = "";
char gau8ConfigNewtorkSendDataDisableTime[10] = "";

/* Modbus 485 Configuration Update Variables */
char gau8ConfigModbus485IsEnabled[2] = "";
char gau8ConfigModbus485SlaveID[6] = "";
char gau8ConfigModbus485Termination[2] = "";
char gau8ConfigModbus485DataPoints[3] = "";
char gau8ConfigModbus485UARTBR[8] = "9600";
char gau8ConfigModbus485UARTStartBit[2] = "";
char gau8ConfigModbus485UARTStopBit[2] = "";
char gau8ConfigModbus485UARTParity[2] = "";
char gau8ConfigModbus485PollingTime[7] = "";
char gau8ConfigModbus485ResponseTime[7] = "";
uint32_t gau32ConfigModbus485Address[CONFIG_MB485_MAXPARAMS] = {'0'};
uint32_t gau32ConfigModbus485DFunctionCode[CONFIG_MB485_MAXPARAMS] = {'0'};
uint32_t gau32ConfigModbus485NoPoints[CONFIG_MB485_MAXPARAMS] = {'0'};

/* Modbus TCP Configuration Update Variables */
char gau8ConfigModbusTCPIsEnabled[2] = "";
char gau8ConfigModbusTCPDataPoints[3] = "";
char gau8ConfigModbusTCPIPDEVICE[16] = "";
char gau8ConfigModbusTCPIPSERVER[16] = "";
char gau8ConfigModbusTCPIPSUBNET[16] = "";
char gau8ConfigModbusTCPIPGATEWAY[16] = "";
char gau8ConfigModbusTCPPORT[6] = "";
char gau8ConfigModbusTCPPollingTime[7] = "";
char gau8ConfigModbusTCPResponseTime[7] = "";
uint32_t gau32ConfigModbusTCPAddress[CONFIG_MBTCP_MAXPARAMS] = {'0'};
uint32_t gau32ConfigModbusTCPDFunctionCode[CONFIG_MBTCP_MAXPARAMS] = {'0'};
uint32_t gau32ConfigModbusTCPNoPoints[CONFIG_MBTCP_MAXPARAMS] = {'0'};
char IP_ADDRESS_DESTI[4] ={192,168,0,200};
uint32_t gu32ModbusTCPPort = 502;

char * tempconfig = "";

/* Test String . Used to test logic by bypassing over the air reception */
//= "(,2,5400968468775475453211317,NS,1,http://59.163.219.179:8025/api/Values/PostStringData,http://59.163.219.178:81/modbustwowaycomm/api/insertData/getData,airteliot.com,60000,100000,1,1,0,30000,NE,MS,1,1,1,44,9600,1,1,0,1,3,8,1,17,25,33,201,205,306,315,327,375,668,673,678,680,685,758,301,309,319,323,325,348,359,361,373,375,387,391,394,397,399,401,403,409,439,455,471,502,505,511,521,1,1,1,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,4,4,15,7,6,7,3,1,3,6,8,2,1,1,1,4,4,1,3,9,2,1,14,9,1,4,1,10,3,2,2,1,1,1,2,3,1,2,2,1,1,1,1,2000,2000,192.168.0.51,255.255.255.000,192.168.0.200,192.168.0.1,502,ME)";
char gau8LastKnownConfiguration[2000];
char gau8ConfigurationCopy[2000] = {'0'};	//TODO Remove

uint32_t ga8ConfigNWLength = 0;
uint32_t ga8ConfigMB485Length = 0;
uint32_t ga8ConfigMBTCPLength = 0;

uint32_t gu32ConfigNetworkErrorDatabase = 0;
uint32_t gu32ConfigModbus485ErrorDatabase = 0;
uint32_t gu32ConfigModbusTCPErrorDatabase = 0;


/* This will be configured based on feature / peripheral available of hardware during release
 * Peripheral marked TRUE are considered for update by the firmware .
 * Make sure that the appropriate peripheral is enable before final release .
 * 1 -  Feature Available
 * 0 -  Feature Not Available
 * 1.  ADC
 * 2.  DI
 * 3.  DO
 * 4.  RPM IN
 * 5.  Network
 * 6.  Modbus-485
 * 7.  Modbus-TCP
 * 8.  Data Mgmt
 * 9.  CAN_1
 * 10. CAN_2
 * 11. Debug
 * 12. MQTT
 * */
uint32_t gau32RemoteConfigSupport[12]={ FALSE,FALSE,FALSE,FALSE,
										FALSE ,TRUE ,TRUE ,FALSE ,
									    FALSE,FALSE,FALSE,FALSE};

enmRemoteConfig enmDeviceConfigState = enmCONFIG_DEVTYPE;
enmMyDevice enmMyDeviceType = enmDEVICETYPE_TOR4G;

/*Config Variables - Global*/
char * sof = "";
char * eof = "";

/*Config Variables - Network */
char * nwsof = "";
char * nweof = "";
char * nwconfig = "";

/*Config Variables - Modbus485 */
char * mb485sof = "";
char * mb485eof = "";
char * mb485config = "";

/*Config Variables - ModbusTCP */
char * mbTCPsof = "";
char * mbTCPeof = "";
char * mbTCPconfig = "";


/*Config Variables - Device Reset and Use Default config - Added on 29-1-22 */
char * devReset = "";
char * useDefaultConfig = "";
char * getConfig = "";

size_t nwmlen = 0;
size_t mb485mlen = 0;
size_t mbtcpmlen = 0;//mbTCPeof - mbTCPsof ;

/*System Boot Vairables for configuration update */

/* Network Config */
uint32_t gu32UploadTimeSupply = ONE_MIN;//TEN_SEC;//TWO_MIN;
uint32_t gu32UploadTimeBatt = FIVE_MIN;

/* Modbus 485 Config */
uint32_t gu32OperateModbus485 = 1;
uint32_t gu32Modbus485UARTBR = 9600;
uint32_t gu32Modbus485PollingTime = ONE_SEC;
uint32_t gu32Modbus485ResponseTime = ONE_SEC;
uint32_t gu32Modbus485RegisterFetch = 49;
uint32_t gu32Modbus485SlaveID = 1;
uint32_t gu32LoopCounter = 0;

/* Modbus TCP config */
uint32_t gu32OperateModbusTCP = 1;
uint32_t gu32ModbusTCPRegisterFetch = 49;
uint32_t gu32ModbusTCPPollingTime = ONE_SEC;
uint32_t gu32ModbusTCPResponseTime = ONE_SEC;

/******************************************************************************
* Function Prototypes
*******************************************************************************/
static void verifyNetworkConfigData(void);
static void verifyModbus485ConfigData(void);
static void verifyModbusTCPConfigData(void);
/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function : verifyRemoteConfiguration()
*//**
* \b Description:
*
* This function is used to verify the received remote config request
*
* PRE-CONDITION:
*
* POST-CONDITION: None
*
* @return 		None.
*
* \b Example Example:
* @code
*
* 	verifyRemoteConfiguration();
*
* @endcode
*
* @see
*
* <br><b> - HISTORY OF CHANGES - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* <tr><td> 08/03/2022 </td><td> 0.0.1            </td><td> HL100133 </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/
void verifyRemoteConfiguration(void)
{
	if(gu32NewConfigAvailable == TRUE)
	{
		sof = strstr(gau8ConfigData,"(,");
		eof = strstr(gau8ConfigData,",)");
		/* Network */
		nwsof = strstr(gau8ConfigData,"NS,");
		nweof = strstr(gau8ConfigData,",NE");
		/* Modbus 485*/
		mb485sof = strstr(gau8ConfigData,"MS,");
		mb485eof = strstr(gau8ConfigData,",ME");
//		/* Modbus TCP*/
//		mbTCPsof = strstr(gau8ConfigData,"ms,");
//		mbTCPeof = strstr(gau8ConfigData,",me");

		/* Special Requests - Reset / restore default config */
		devReset = strstr(gau8ConfigData,"!");
		useDefaultConfig = strstr(gau8ConfigData,"^");
		getConfig = strstr(gau8ConfigData,"%");

		if(sof != NULL)
		{
			/* Start of frame found */
			if(eof != NULL)
			{
				/* End of Frame Found . Parse the frame for config Data */
				gu32ConfigLen = eof - sof ;	// Total Config Bytes
				memset(gau8RemoteDeviceID, 0, sizeof(gau8RemoteDeviceID));
				memcpy(gau8RemoteDeviceID, sof+4, strlen(strtok(sof+4,",")));
				gu32ParseConfigAvailable = TRUE;
			}

		}

	}
	gu32NewConfigAvailable = FALSE;

}
/******************************************************************************
* Function : parseRemoteconfig()
*//**
* \b Description:
*
* This function is used to parse the received remote config request
*
* PRE-CONDITION: The config string should be verified
*
* POST-CONDITION: None
*
* @return 		None.
*
* \b Example Example:
* @code
*
* 	verifyRemoteConfiguration();
* 	parseRemoteconfig();
*
* @endcode
*
* @see
*
* <br><b> - HISTORY OF CHANGES - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* <tr><td> 08/03/2022 </td><td> 0.0.1            </td><td> HL100133 </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/
void parseRemoteconfig(void)
{
	if(gu32ParseConfigAvailable == TRUE)
	{
		/*Fresh Config Available */
		if(strcmp(dinfo,gau8RemoteDeviceID) == 0)
		{
			/* Device ID is matching . Update system as per config */
			/* TODO : State Machine to extract received configuration
			 * 1.  ADC
			 * 2.  DI
			 * 3.  DO
			 * 4.  RPM IN
			 * 5.  Network
			 * 6.  Modbus-485
			 * 7.  Modbus-TCP
			 * 8.  Data Mgmt
			 * 9.  CAN_1
			 * 10. CAN_2
			 * 11. Debug
			 * 12. MQTT */
			HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);	// LED ON
			/* Check for Special Requests */

			if(devReset != NULL)
			{
				/* Take Device Reset . Initiated by the user */
				HAL_Delay(1000);
				HAL_NVIC_SystemReset();
			}
			else if(useDefaultConfig != NULL)
			{
				/* Clear Internal Configuration Memory Segment .
				 * Restore default configuration
				 * Initialted by the user */
				if(FLASH_If_Erase(ADDR_FLASH_SECTOR_22) != FLASHIF_OK)
				{
					/*Error Erasing Flash*/
				}
				else
					HAL_NVIC_SystemReset();
			}
			else if(getConfig != NULL)
			{
				/*Send Last Known configuration in the payload */
				enqueue(&gsmPayload,(char *)getSystemConfig());
			}
			else
			{
				/* No special requests found in the configuration . Parse the received configuration */
				switch(enmDeviceConfigState)
				{
					case enmCONFIG_DEVTYPE:
						memcpy(gau8RemoteDeviceType, sof+2, strlen(strtok(sof+2,",")));
						if(atoi(gau8RemoteDeviceType) == enmMyDeviceType)
						{
							/* Device Type Verified */
							enmDeviceConfigState++;
						}
						else
						{
							/* Device Type Not valid . Do not parse the config */
							gu32ParseConfigAvailable = FALSE;
						}

						break;

					case enmCONFIG_ADC:
						if(gau32RemoteConfigSupport[enmCONFIG_ADC] == TRUE)
						{
							/* Parse ADC configuration */
						}
						else
							enmDeviceConfigState++;
						break;
					case enmCONFIG_DI:
						if(gau32RemoteConfigSupport[enmCONFIG_DI] == TRUE)
						{
							/* Parse DI configuration */
						}
						else
							enmDeviceConfigState++;
						break;
					case enmCONFIG_DO:
						if(gau32RemoteConfigSupport[enmCONFIG_DO] == TRUE)
						{
							/* Parse DO configuration */
						}
						else
							enmDeviceConfigState++;
						break;
					case enmCONFIG_RPM:
						if(gau32RemoteConfigSupport[enmCONFIG_RPM] == TRUE)
						{
							/* Parse RPM configuration */
						}
						else
							enmDeviceConfigState++;

						break;
					case enmCONFIG_NETWORK:
						/* Completed */
						if(gau32RemoteConfigSupport[enmCONFIG_NETWORK] == TRUE)
						{
							if(gu32ConfigOperationStatus == FALSE)
							{
								if((nwsof != NULL) && (nweof != NULL))
								{
									/*Make configuration extraction calculations
									 * */
									nwmlen = (nweof - nwsof) + 4 ;
									tempconfig = (char *)malloc(sizeof(char) * 2000);
									memset(tempconfig,0x00,2000 * sizeof(char));
									if(tempconfig != NULL)
									{
										strncat((char *)tempconfig,nwsof,nwmlen);
									}
									else
									{
										/* Error in alloting memory for network configuration
										 * TODO : Report Error  */
										gau32RemoteConfigSupport[enmCONFIG_NETWORK] = FALSE;
										enmDeviceConfigState++;
									}
									gu32ConfigOperationStatus = TRUE;
								}
								else
								{
									/* Network configuration not found. Parse Next Configuration */
									enmDeviceConfigState++;
								}

							}
							else
							{
								/* Extract Network configuration */
								gu32ConfigOperationStatus = FALSE;
								enmDeviceConfigState++;
							}

						}
						else
							enmDeviceConfigState++;
						break;
					case enmCONFIG_MODBUS485:
						if(gau32RemoteConfigSupport[enmCONFIG_MODBUS485] == TRUE)
						{
							/* Parse Modbus485 configuration */
							if(gu32ConfigOperationStatus == FALSE)
							{
								if((mb485sof != NULL) && (mb485eof != NULL))
								{
									/*Make configuration extraction calculations
									 * */
									mb485mlen = (mb485eof - mb485sof) + 4 ;
									if(tempconfig != NULL)
									{
										strncat((char *)tempconfig,mb485sof,mb485mlen);
									}
									else
									{
										/* Error in alloting memory for modbus485 configuration
										 * TODO : Report Error  */
										gau32RemoteConfigSupport[enmCONFIG_NETWORK] = FALSE;
										enmDeviceConfigState++;
									}
									gu32ConfigOperationStatus = TRUE;
								}
								else
								{
									/* Modbus485 configuration not found. Parse Next Configuration */
									enmDeviceConfigState++;
								}
							}
							else
							{
								/* Extract Network configuration */
								gu32ConfigOperationStatus = FALSE;
								enmDeviceConfigState++;
							}
						}
						else
							enmDeviceConfigState++;
						break;
					case enmCONFIG_MODBUSTCP:
						if(gau32RemoteConfigSupport[enmCONFIG_MODBUSTCP] == TRUE)
						{
							/* Parse Modbus TCP configuration */
							if(gau32RemoteConfigSupport[enmCONFIG_MODBUSTCP] == TRUE)
							{
								/* Parse Modbus485 configuration */
								if(gu32ConfigOperationStatus == FALSE)
								{
									if((mbTCPsof != NULL) && (mbTCPeof != NULL))
									{
										/*Make configuration extraction calculations
										 * */
										mbtcpmlen = (mbTCPeof-mbTCPsof) + 4;

										if(tempconfig != NULL)
										{
											strncat((char *)tempconfig,mbTCPsof,mbtcpmlen);
										}
										else
										{
											/* Error in alloting memory for modbus485 configuration
											 * TODO : Report Error  */
											gau32RemoteConfigSupport[enmCONFIG_MODBUSTCP] = FALSE;
											enmDeviceConfigState++;
										}
										gu32ConfigOperationStatus = TRUE;
									}
									else
									{
										/* Modbus485 configuration not found. Parse Next Configuration */
										enmDeviceConfigState++;
									}
								}
								else
								{
									/* Extract Network configuration */
									gu32ConfigOperationStatus = FALSE;
									enmDeviceConfigState++;

								}
							}
						}
						else
							enmDeviceConfigState++;
						break;
					case enmCONFIG_DATAMGMT:
						if(gau32RemoteConfigSupport[enmCONFIG_DATAMGMT] == TRUE)
						{
							/* Parse Data Mgmt configuration */
						}
						else
							enmDeviceConfigState++;
						break;
					case enmCONFIG_CAN_1:
						if(gau32RemoteConfigSupport[enmCONFIG_CAN_1] == TRUE)
						{
							/* Parse CAN_1 configuration */
						}
						else
							enmDeviceConfigState++;
						break;
					case enmCONFIG_CAN_2:
						if(gau32RemoteConfigSupport[enmCONFIG_CAN_2] == TRUE)
						{
							/* Parse CAN_2 configuration */
						}
						else
							enmDeviceConfigState++;
						break;
					case enmCONFIG_DEBUG:
						if(gau32RemoteConfigSupport[enmCONFIG_DEBUG] == TRUE)
						{
							/* Parse Debug configuration */
						}
						else
							enmDeviceConfigState++;
						break;
					case enmCONFIG_MQTT:
						if(gau32RemoteConfigSupport[enmCONFIG_MQTT] == TRUE)
						{
							/* Parse MQTT configuration */
						}
						else
							enmDeviceConfigState++ ;

					break;

					case enmCONFIG_WRITETOMEMORY:
						/* Write Received Configuration to memory */
						if(FLASH_If_Erase(ADDR_FLASH_SECTOR_11) != FLASHIF_OK)
						{
							/*Error Erasing Flash*/
						}
						else
						{
							/* Configuration Storage Process */
							if(u32LastMemoryWriteLocation == 0)
								u32LastMemoryWriteLocation = ADDR_FLASH_SECTOR_11; // against 22

							if(WriteDatatoFlash(u32LastMemoryWriteLocation,(uint8_t *)tempconfig,(nwmlen+mb485mlen+mbtcpmlen+4),1) == SUCCESS)
							{
								/*Config Stored successfully . Take system reset*/

								/* Added on 29-1-22 */
								tempconfig = NULL;
								HAL_Delay(3000);
								HAL_Delay(1000000);
								HAL_Delay(1000000);
							}
						}
						gu32ParseConfigAvailable = FALSE;
						break;

					default :

						gu32ParseConfigAvailable = FALSE;
						break;
				}
			}
			//HAL_Delay(3000);
		}
	}
}

/******************************************************************************
* Function : extractNewtorkConfiguration()
*//**
* \b Description:
*
* This function is used to extract network config from received remote config strings
*
* PRE-CONDITION: The config string should be verified
*
* POST-CONDITION: None
*
* @return 		None.
*
* \b Example Example:
* @code
*
* 	extractNewtorkConfiguration();
*
* @endcode
*
* @see
*
* <br><b> - HISTORY OF CHANGES - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* <tr><td> 08/03/2022 </td><td> 0.0.1            </td><td> HL100133 </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/
void extractNewtorkConfiguration(void)
{
	gu32ExtractParamCounter = 0;
	char *configptr = strtok(nwsof, ",");
	while(configptr != NULL)
	{
		switch(gu32ExtractParamCounter)
		{
			case 0:
				/*SOF Do not copy */
			break;
			case 1:
				strcpy(gau8ConfigNewtorkIsEnabled,configptr);
				break;
			case 2:
				strcpy(gau8ConfigNewtorkServerURL,configptr);
				break;
			case 3:
				strcpy(gau8ConfigNewtorkConfigURL,configptr);
				break;
			case 4:
				strcpy(gau8ConfigNewtorkAPN,configptr);
				break;
			case 5:
				strcpy(gau8ConfigNewtorkUpFreqSupply,configptr);
				//gu32UploadTimeSupply = atoi(gau8ConfigNewtorkUpFreqSupply);
				break;
			case 6:
				strcpy(gau8ConfigNewtorkUpFreqBatt,configptr);
				//gu32UploadTimeBatt = atoi(gau8ConfigNewtorkUpFreqBatt);
				break;
			case 7:
				strcpy(gau8ConfigNewtorkType,configptr);
				break;
			case 8:
				strcpy(gau8ConfigNewtorkGPSEnable,configptr);
				break;
			case 9:
				strcpy(gau8ConfigNewtorkSendData,configptr);
				break;
			case 10:
				strcpy(gau8ConfigNewtorkSendDataDisableTime,configptr);
				break;
			default:
				break;
		}
		gu32ExtractParamCounter++;
		configptr = strtok(NULL, ",");
	}
	gu32ExtractParamCounter = 0;
	verifyNetworkConfigData();
}

/******************************************************************************
* Function : verifyNetworkConfigData()
*//**
* \b Description:
*
* This function is used to verify network config data as defined in configuration doc.
* This function verifies whether the data is appropriate else updates error database
* variable w.r.t found error.
*
* PRE-CONDITION: The config string should be verified
*
* POST-CONDITION: None
*
* @return 		None.
*
* \b Example Example:
* @code
*
* 	verifyNetworkConfigData();
*
* @endcode
*
* @see
*
* <br><b> - HISTORY OF CHANGES - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* <tr><td> 08/03/2022 </td><td> 0.0.1            </td><td> HL100133 </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/
static void verifyNetworkConfigData(void)
{
	/*Log Errors Related to */
	if(atoi(gau8ConfigNewtorkIsEnabled) > 2)
		gu32ConfigNetworkErrorDatabase |= (1 << enmCONFIG_NWISENABLED);
	else
		gu32ConfigNetworkErrorDatabase &= ~(1 << enmCONFIG_NWISENABLED);

	if(strlen(gau8ConfigNewtorkServerURL) > 150)
		gu32ConfigNetworkErrorDatabase |= (1 << enmCONFIG_NWSERURLLEN);
	else
		gu32ConfigNetworkErrorDatabase &= ~(1 << enmCONFIG_NWSERURLLEN);

	if(strlen(gau8ConfigNewtorkConfigURL) > 150)
		gu32ConfigNetworkErrorDatabase |= (1 << enmCONFIG_NWCONFURLLEN);
	else
		gu32ConfigNetworkErrorDatabase &= ~(1 << enmCONFIG_NWCONFURLLEN);

	if(strlen(gau8ConfigNewtorkAPN) > 50)
		gu32ConfigNetworkErrorDatabase |= (1 << enmCONFIG_NWAPN);
	else
		gu32ConfigNetworkErrorDatabase &= ~(1 << enmCONFIG_NWAPN);

	if(atoi(gau8ConfigNewtorkUpFreqSupply) > 10000000)
		gu32ConfigNetworkErrorDatabase |= (1 << enmCONFIG_NWUPFREQSUPPLY);
	else
	{
		gu32UploadTimeSupply = atoi(gau8ConfigNewtorkUpFreqSupply);
		gu32ConfigNetworkErrorDatabase &= ~(1 << enmCONFIG_NWUPFREQSUPPLY);
	}

	if(atoi(gau8ConfigNewtorkUpFreqBatt) > 10000000)
		gu32ConfigNetworkErrorDatabase |= (1 << enmCONFIG_NWUPFREQBATT);
	else
	{
		gu32UploadTimeBatt = atoi(gau8ConfigNewtorkUpFreqBatt);
		gu32ConfigNetworkErrorDatabase &= ~(1 << enmCONFIG_NWUPFREQBATT);
	}
	if(atoi(gau8ConfigNewtorkType) > 2)
		gu32ConfigNetworkErrorDatabase |= (1 << enmCONFIG_NWTYPE);
	else
		gu32ConfigNetworkErrorDatabase &= ~(1 << enmCONFIG_NWTYPE);

	if(atoi(gau8ConfigNewtorkGPSEnable) > 1)
		gu32ConfigNetworkErrorDatabase |= (1 << enmCONFIG_NWGPSEN);
	else
		gu32ConfigNetworkErrorDatabase &= ~(1 << enmCONFIG_NWGPSEN);

	if(atoi(gau8ConfigNewtorkSendData) > 1)
		gu32ConfigNetworkErrorDatabase |= (1 << enmCONFIG_NWSENDDATAEN);
	else
		gu32ConfigNetworkErrorDatabase &= ~(1 << enmCONFIG_NWSENDDATAEN);

	if(atoi(gau8ConfigNewtorkSendDataDisableTime) > 10000000)
		gu32ConfigNetworkErrorDatabase |= (1 << enmCONFIG_NWSENDDATADISABLE);
	else
		gu32ConfigNetworkErrorDatabase &= ~(1 << enmCONFIG_NWSENDDATADISABLE);

}

/******************************************************************************
* Function : extractModbus485Configuration()
*//**
* \b Description:
*
* This function is used to extract MB485 config from received remote config strings
*
* PRE-CONDITION:
*
* POST-CONDITION: None
*
* @return 		None.
*
* \b Example Example:
* @code
*
* 	extractModbus485Configuration();
*
* @endcode
*
* @see
*
* <br><b> - HISTORY OF CHANGES - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* <tr><td> 08/03/2022 </td><td> 0.0.1            </td><td> HL100133 </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/
void extractModbus485Configuration(void)
{
	/* Check if expected configuration is received
	 * If yes then proceed else exit */
	uint32_t u32Loopcounter = 0;
	char *configptr = strtok(mb485config, ",");//mb485config
	gu32ExtractParamCounter = 0;
	while(configptr != NULL)
	{
		switch(gu32ExtractParamCounter)
		{
			case 0:
				/* SOF */
				break;
			case 1:
				/*Is enabled*/
				strcpy(gau8ConfigModbus485IsEnabled,configptr);
				break;

			case 2:
				/* Slave Address */
				strcpy(gau8ConfigModbus485SlaveID,configptr);
				break;
			case 3:
				/* Termination */
				strcpy(gau8ConfigModbus485Termination,configptr);
				break;
			case 4:
				strcpy(gau8ConfigModbus485DataPoints,configptr);
				break;
			case 5:
				/* BR */
				strcpy(gau8ConfigModbus485UARTBR,configptr);
				break;
			case 6:
				/* Start Bit*/
				strcpy(gau8ConfigModbus485UARTStartBit,configptr);
				break;
			case 7:
				/* Stop Bit*/
				strcpy(gau8ConfigModbus485UARTStopBit,configptr);
				break;
			case 8:
				/* Parity*/
				strcpy(gau8ConfigModbus485UARTParity,configptr);
				break;
			case 9:
				/* Modbus Address , FC and Data Points */
				if(atoi(gau8ConfigModbus485DataPoints) <= CONFIG_MB485_MAXPARAMS)
				{
					for(u32Loopcounter = 0; u32Loopcounter < atoi(gau8ConfigModbus485DataPoints) ;u32Loopcounter++)
					{
						/*Populate Modbus Address Array from config received */
						gau32ConfigModbus485Address[u32Loopcounter] = atoi(configptr);
						gau32ConfigModbusTCPAddress[u32Loopcounter] = atoi(configptr);
						configptr = strtok(NULL, ",");
					}
					u32Loopcounter = 0;
					for(u32Loopcounter = 0; u32Loopcounter < atoi(gau8ConfigModbus485DataPoints) ;u32Loopcounter++)
					{
						/*Populate Modbus Address Array from config received */
						gau32ConfigModbus485DFunctionCode[u32Loopcounter] = atoi(configptr);
						gau32ConfigModbusTCPDFunctionCode[u32Loopcounter] = atoi(configptr);
						configptr = strtok(NULL, ",");
					}
					u32Loopcounter = 0;
					for(u32Loopcounter = 0; u32Loopcounter < atoi(gau8ConfigModbus485DataPoints) ;u32Loopcounter++)
					{
						/*Populate Modbus Address Array from config received */
						gau32ConfigModbus485NoPoints[u32Loopcounter] = atoi(configptr);
						gau32ConfigModbusTCPNoPoints[u32Loopcounter] = atoi(configptr);
						configptr = strtok(NULL, ",");
					}
					u32Loopcounter = 0;
				}
				break;
			case 10:
				/* Polling Time */
				strcpy(gau8ConfigModbus485PollingTime,configptr);
				//gu32Modbus485PollingTime = atoi(gau8ConfigModbus485PollingTime);
				break;
			case 11:
				/* Response Time */
				strcpy(gau8ConfigModbus485ResponseTime,configptr);
				//gu32ModbusResponseTimeout = atoi(gau8ConfigModbus485ResponseTime);
				break;
				/* TCP IP Added . From Here */
			case 12:
				strcpy(gau8ConfigModbusTCPIPDEVICE,configptr);
				break;

			case 13:
				/* BR */
				strcpy(gau8ConfigModbusTCPIPSUBNET,configptr);
				break;

			case 14:
				/* Start Bit*/
				strcpy(gau8ConfigModbusTCPIPSERVER,configptr);
				break;

			case 15:
				/* Stop Bit*/
				strcpy(gau8ConfigModbusTCPIPGATEWAY,configptr);
				break;

			case 16:
				/* Parity*/
				strcpy(gau8ConfigModbusTCPPORT,configptr);
				break;

			default:
				break;
		}
		if(gu32ExtractParamCounter != 9)
			configptr = strtok(NULL, ",");

		gu32ExtractParamCounter++;

	}
	gu32ExtractParamCounter = 0;
	verifyModbus485ConfigData();
}

/******************************************************************************
* Function : verifyModbus485ConfigData()
*//**
* \b Description:
*
* This function is used to verify MB485 config data as defined in configuration doc.
* This function verifies whether the data is appropriate else updates error database
* variable w.r.t found error.
*
* PRE-CONDITION:
*
* POST-CONDITION: None
*
* @return 		None.
*
* \b Example Example:
* @code
*
* 	verifyModbus485ConfigData();
*
* @endcode
*
* @see
*
* <br><b> - HISTORY OF CHANGES - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* <tr><td> 08/03/2022 </td><td> 0.0.1            </td><td> HL100133 </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/
static void verifyModbus485ConfigData(void)
{
   /* Log Errors Related to Modbus485 Configuration */
	uint32_t u32LoopCounter = 0;

	if(atoi(gau8ConfigModbus485IsEnabled) > 2)
		gu32ConfigModbus485ErrorDatabase |= (1 << enmCONFIG_MB485ISENABLED);
	else
		gu32ConfigModbus485ErrorDatabase &= ~(1 << enmCONFIG_MB485ISENABLED);

	if((atoi(gau8ConfigModbus485SlaveID) <= 0) | (atoi(gau8ConfigModbus485SlaveID) > 255)) // 1 to 255
		gu32ConfigModbus485ErrorDatabase |= (1 << enmCONFIG_MB485SLAVEID);
	else
		gu32ConfigModbus485ErrorDatabase &= ~(1 << enmCONFIG_MB485SLAVEID);

	if(atoi(gau8ConfigModbus485Termination) > 1)
		gu32ConfigModbus485ErrorDatabase |= (1 << enmCONFIG_MB485TERMINATE);
	else
		gu32ConfigModbus485ErrorDatabase &= ~(1 << enmCONFIG_MB485TERMINATE);

	if(atoi(gau8ConfigModbus485DataPoints) > CONFIG_MB485_MAXPARAMS)
		gu32ConfigModbus485ErrorDatabase |= (1 << enmCONFIG_MB485DATAPOINTS);
	else
		gu32ConfigModbus485ErrorDatabase &= ~(1 << enmCONFIG_MB485DATAPOINTS);

	if(atoi(gau8ConfigModbus485UARTBR) > 115200)
		gu32ConfigModbus485ErrorDatabase |= (1 << enmCONFIG_MB485UARTBR);
	else
		gu32ConfigModbus485ErrorDatabase &= ~(1 << enmCONFIG_MB485UARTBR);

	if(atoi(gau8ConfigModbus485UARTStartBit) > 3)
		gu32ConfigModbus485ErrorDatabase |= (1 << enmCONFIG_MB485UARTSTARTBIT);
	else
		gu32ConfigModbus485ErrorDatabase &= ~(1 << enmCONFIG_MB485UARTSTARTBIT);

	if(atoi(gau8ConfigModbus485UARTStopBit) > 3)
		gu32ConfigModbus485ErrorDatabase |= (1 << enmCONFIG_MB485UARTSTOPBIT);
	else
		gu32ConfigModbus485ErrorDatabase &= ~(1 << enmCONFIG_MB485UARTSTOPBIT);

	if(atoi(gau8ConfigModbus485PollingTime) > 1000000)
		gu32ConfigModbus485ErrorDatabase |= (1 << enmCONFIG_MB485POLLTIME);
	else
	{
		gu32Modbus485PollingTime = atoi(gau8ConfigModbus485PollingTime);
		gu32ConfigModbus485ErrorDatabase &= ~(1 << enmCONFIG_MB485POLLTIME);
	}

	if(atoi(gau8ConfigModbus485ResponseTime) > 1000000)
		gu32ConfigModbus485ErrorDatabase |= (1 << enmCONFIG_MB485RESPTIME);
	else
	{	gu32ModbusResponseTimeout = atoi(gau8ConfigModbus485ResponseTime);
		gu32ConfigModbus485ErrorDatabase &= ~(1 << enmCONFIG_MB485RESPTIME);
	}
	for(u32LoopCounter = 0 ; u32LoopCounter < atoi(gau8ConfigModbus485DataPoints) ; u32LoopCounter++ )
	{
		if((gau32ConfigModbus485Address[u32LoopCounter] == 0) || (gau32ConfigModbus485Address[u32LoopCounter] > 65535))
		{
			/* Due to Address Dereferenceing No address can be 0 .
			 * Also No address can be greater tha 65535
			 * Log Error*/
			gu32ConfigModbus485ErrorDatabase |= (1 << enmCONFIG_MB485ADDRESS);
			break;
		}
		else
			gu32ConfigModbus485ErrorDatabase &= ~(1 << enmCONFIG_MB485ADDRESS);
	}

	u32LoopCounter = 0;
	for(u32LoopCounter = 0 ; u32LoopCounter < atoi(gau8ConfigModbus485DataPoints) ; u32LoopCounter++ )
	{
		if((gau32ConfigModbus485DFunctionCode[u32LoopCounter] == 0) || (gau32ConfigModbus485DFunctionCode[u32LoopCounter] > 5))
		{
			/* Due to Address Dereferenceing No address can be 0 .
			 * Also No address can be greater tha 65535
			 * Log Error*/
			gu32ConfigModbus485ErrorDatabase |= (1 << enmCONFIG_MB485FUCNTIONCODE);
			break;
		}
		else
			gu32ConfigModbus485ErrorDatabase &= ~(1 << enmCONFIG_MB485FUCNTIONCODE);
	}
	u32LoopCounter = 0;
	for(u32LoopCounter = 0 ; u32LoopCounter < atoi(gau8ConfigModbus485DataPoints) ; u32LoopCounter++ )
	{
		if((gau32ConfigModbus485NoPoints[u32LoopCounter] == 0) || (gau32ConfigModbus485NoPoints[u32LoopCounter] > 255))
		{
			/* Due to Address Dereferenceing No address can be 0 .
			 * Also No address can be greater tha 65535
			 * Log Error*/
			gu32ConfigModbus485ErrorDatabase |= (1 << enmCONFIG_MB485DATAPOINTS);
			break;
		}
		else
			gu32ConfigModbus485ErrorDatabase &= ~(1 << enmCONFIG_MB485DATAPOINTS);
	}

	if(IP_IS_TYPE_V4(gau8ConfigModbusTCPIPDEVICE) != 1)
		gu32ConfigModbusTCPErrorDatabase |= (1 << enmCONFIG_MBTCPIPDEVICE);
	else
		gu32ConfigModbusTCPErrorDatabase &= ~(1 << enmCONFIG_MBTCPIPDEVICE);

	if(IP_IS_TYPE_V4(gau8ConfigModbusTCPIPSERVER) != 1)
		gu32ConfigModbusTCPErrorDatabase |= (1 << enmCONFIG_MB485UARTSTOPBIT);
	else
		gu32ConfigModbusTCPErrorDatabase &= ~(1 << enmCONFIG_MB485UARTSTOPBIT);

	if(IP_IS_TYPE_V4(gau8ConfigModbusTCPIPGATEWAY) != 1)
		gu32ConfigModbusTCPErrorDatabase |= (1 << enmCONFIG_MB485POLLTIME);
	else
		gu32ConfigModbusTCPErrorDatabase &= ~(1 << enmCONFIG_MB485POLLTIME);

	if(atoi(gau8ConfigModbusTCPPORT) > 65535)
		gu32ConfigModbusTCPErrorDatabase |= (1 << enmCONFIG_MBTCPIPPORT);
	else
		gu32ConfigModbusTCPErrorDatabase &= ~(1 << enmCONFIG_MBTCPIPPORT);

}

/******************************************************************************
* Function : extractModbusTCPConfiguration()
*//**
* \b Description:
*
* This function is used to extract MBTCP config from received remote config strings
*
* PRE-CONDITION:
*
* POST-CONDITION: None
*
* @return 		None.
*
* \b Example Example:
* @code
*
* 	extractModbusTCPConfiguration();
*
* @endcode
*
* @see
*
* <br><b> - HISTORY OF CHANGES - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* <tr><td> 08/03/2022 </td><td> 0.0.1            </td><td> HL100133 </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/
void extractModbusTCPConfiguration(void)
{
	/* Check if expected configuration is received
	 * If yes then proceed else exit */
	uint32_t u32Loopcounter = 0;
	char *configptr = strtok(mbTCPconfig, ",");
	gu32ExtractParamCounter = 0;
	while(configptr != NULL)
	{
		switch(gu32ExtractParamCounter)
		{
			case 0:
				/* SOF */
				break;
			case 1:
				/*Is enabled*/
				strcpy(gau8ConfigModbusTCPIsEnabled,configptr);
				break;
			case 2:
				/* Termination */
				strcpy(gau8ConfigModbusTCPDataPoints,configptr);
				break;
			case 3:
				strcpy(gau8ConfigModbusTCPIPDEVICE,configptr);
				break;
			case 4:
				/* BR */
				strcpy(gau8ConfigModbusTCPIPSUBNET,configptr);
				break;
			case 5:
				/* Start Bit*/
				strcpy(gau8ConfigModbusTCPIPSERVER,configptr);
				break;
			case 6:
				/* Stop Bit*/
				strcpy(gau8ConfigModbusTCPIPGATEWAY,configptr);
				break;
			case 7:
				/* Parity*/
				strcpy(gau8ConfigModbusTCPPORT,configptr);
				break;
			case 8:
				/* Modbus Address , FC and Data Points */
				if(atoi(gau8ConfigModbusTCPDataPoints) <= CONFIG_MBTCP_MAXPARAMS)
				{
					for(u32Loopcounter = 0; u32Loopcounter < atoi(gau8ConfigModbusTCPDataPoints) ;u32Loopcounter++)
					{
						/*Populate Modbus Address Array from config received */
						gau32ConfigModbusTCPAddress[u32Loopcounter] = atoi(configptr);
						configptr = strtok(NULL, ",");
					}
					u32Loopcounter = 0;
					for(u32Loopcounter = 0; u32Loopcounter < atoi(gau8ConfigModbusTCPDataPoints) ;u32Loopcounter++)
					{
						/*Populate Modbus Address Array from config received */
						gau32ConfigModbusTCPDFunctionCode[u32Loopcounter] = atoi(configptr);
						configptr = strtok(NULL, ",");
					}
					u32Loopcounter = 0;
					for(u32Loopcounter = 0; u32Loopcounter < atoi(gau8ConfigModbusTCPDataPoints) ;u32Loopcounter++)
					{
						/*Populate Modbus Address Array from config received */
						gau32ConfigModbusTCPNoPoints[u32Loopcounter] = atoi(configptr);
						configptr = strtok(NULL, ",");
					}
					u32Loopcounter = 0;
				}
				break;
			case 9:
				/* Polling Time */
				strcpy(gau8ConfigModbusTCPPollingTime,configptr);
				break;
			case 10:
				/* Response Time */
				strcpy(gau8ConfigModbusTCPResponseTime,configptr);
				break;
			default:
				break;
		}
		if(gu32ExtractParamCounter != 8)
			configptr = strtok(NULL, ",");

		gu32ExtractParamCounter++;

	}
	gu32ExtractParamCounter = 0;
	verifyModbusTCPConfigData();
}

/******************************************************************************
* Function : verifyModbusTCPConfigData()
*//**
* \b Description:
*
* This function is used to verify MBTCP config data as defined in configuration doc.
* This function verifies whether the data is appropriate else updates error database
* variable w.r.t found error.
*
* PRE-CONDITION: The config string should be verified
*
* POST-CONDITION: None
*
* @return 		None.
*
* \b Example Example:
* @code
*
* 	verifyModbus485ConfigData();
*
* @endcode
*
* @see
*
* <br><b> - HISTORY OF CHANGES - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* <tr><td> 08/03/2022 </td><td> 0.0.1            </td><td> HL100133 </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/
static void verifyModbusTCPConfigData(void)
{
	/* Log Errors Related to Modbus485 Configuration */
	uint32_t u32LoopCounter = 0;

	if(atoi(gau8ConfigModbusTCPIsEnabled) > 2)
		gu32ConfigModbusTCPErrorDatabase |= (1 << enmCONFIG_MBTCPISENABLED);
	else
		gu32ConfigModbusTCPErrorDatabase &= ~(1 << enmCONFIG_MBTCPISENABLED);

	if(atoi(gau8ConfigModbusTCPDataPoints) > CONFIG_MBTCP_MAXPARAMS)
		gu32ConfigModbusTCPErrorDatabase |= (1 << enmCONFIG_MBTCPDATAPOINTS);
	else
		gu32ConfigModbusTCPErrorDatabase &= ~(1 << enmCONFIG_MBTCPDATAPOINTS);

	if(IP_IS_TYPE_V4(gau8ConfigModbusTCPIPDEVICE) != 1)
		gu32ConfigModbusTCPErrorDatabase |= (1 << enmCONFIG_MBTCPIPDEVICE);
	else
		gu32ConfigModbusTCPErrorDatabase &= ~(1 << enmCONFIG_MBTCPIPDEVICE);

	if(IP_IS_TYPE_V4(gau8ConfigModbusTCPIPSERVER) != 1)
		gu32ConfigModbusTCPErrorDatabase |= (1 << enmCONFIG_MB485UARTSTOPBIT);
	else
		gu32ConfigModbusTCPErrorDatabase &= ~(1 << enmCONFIG_MB485UARTSTOPBIT);

	if(IP_IS_TYPE_V4(gau8ConfigModbusTCPIPGATEWAY) != 1)
		gu32ConfigModbusTCPErrorDatabase |= (1 << enmCONFIG_MB485POLLTIME);
	else
		gu32ConfigModbusTCPErrorDatabase &= ~(1 << enmCONFIG_MB485POLLTIME);

	if(atoi(gau8ConfigModbusTCPPORT) > 65535)
		gu32ConfigModbusTCPErrorDatabase |= (1 << enmCONFIG_MBTCPIPPORT);
	else
		gu32ConfigModbusTCPErrorDatabase &= ~(1 << enmCONFIG_MBTCPIPPORT);

	/*Added on 28-1-2022 */
	if(atoi(gau8ConfigModbusTCPPollingTime) > 1000000)
		gu32ConfigModbusTCPErrorDatabase |= (1 << enmCONFIG_MBTCPPOLLTIME);
	else
	{
		gu32ModbusTCPPollingTime = atoi(gau8ConfigModbusTCPPollingTime);
		gu32ConfigModbusTCPErrorDatabase &= ~(1 << enmCONFIG_MBTCPPOLLTIME);
	}

	if(atoi(gau8ConfigModbusTCPResponseTime) > 1000000)
		gu32ConfigModbusTCPErrorDatabase |= (1 << enmCONFIG_MB485RESPTIME);
	else
	{
		/* Will not update this field as LWIP TCP stack requires min 3 SEC to update the state */
		//gu32ModbusTCPResponseTimeout = atoi(gau8ConfigModbusTCPResponseTime);
		gu32ConfigModbusTCPErrorDatabase &= ~(1 << enmCONFIG_MB485RESPTIME);
	}

	for(u32LoopCounter = 0 ; u32LoopCounter < atoi(gau8ConfigModbusTCPDataPoints) ; u32LoopCounter++ )
	{
		if((gau32ConfigModbusTCPAddress[u32LoopCounter] == 0) || (gau32ConfigModbusTCPAddress[u32LoopCounter] > 65535))
		{
			/*Due to Address Dereferenceing No address can be 0 .
			 * Also No address can be greater tha 65535
			 * Log Error*/
			gu32ConfigModbusTCPErrorDatabase |= (1 << enmCONFIG_MBTCPADDRESS);
			break;
		}
		else
			gu32ConfigModbusTCPErrorDatabase &= ~(1 << enmCONFIG_MBTCPADDRESS);
	}

	u32LoopCounter = 0;
	for(u32LoopCounter = 0 ; u32LoopCounter < atoi(gau8ConfigModbusTCPDataPoints) ; u32LoopCounter++ )
	{
		if((gau32ConfigModbusTCPDFunctionCode[u32LoopCounter] == 0) || (gau32ConfigModbusTCPDFunctionCode[u32LoopCounter] > 5))
		{
			/*Due to Address Dereferenceing No address can be 0 .
			 * Also No address can be greater tha 65535
			 * Log Error*/
			gu32ConfigModbusTCPErrorDatabase |= (1 << enmCONFIG_MBTCPFUNCTIONCODE);
			break;
		}
		else
			gu32ConfigModbusTCPErrorDatabase &= ~(1 << enmCONFIG_MBTCPFUNCTIONCODE);
	}
	u32LoopCounter = 0;
	for(u32LoopCounter = 0 ; u32LoopCounter < atoi(gau8ConfigModbusTCPDataPoints) ; u32LoopCounter++ )
	{
		if((gau32ConfigModbusTCPNoPoints[u32LoopCounter] == 0) || (gau32ConfigModbusTCPNoPoints[u32LoopCounter] > 255))
		{
			/*Due to Address Dereferenceing No address can be 0 .
			 * Also No address can be greater tha 65535
			 * Log Error*/
			gu32ConfigModbusTCPErrorDatabase |= (1 << enmCONFIG_MBTCPDATALEN);
			break;
		}
		else
			gu32ConfigModbusTCPErrorDatabase &= ~(1 << enmCONFIG_MBTCPDATALEN);
	}
}

/******************************************************************************
* Function : getLastKnownConfiguration()
*//**
* \b Description:
*
* This function is used to get last confguartion from intenal memory (SECTOR_22)
* This function is called before initialising the application .
*
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @return 		None.
*
* \b Example Example:
* @code
*
* 	getLastKnownConfiguration();
*
* @endcode
*
* @see
*
* <br><b> - HISTORY OF CHANGES - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* <tr><td> 08/03/2022 </td><td> 0.0.1            </td><td> HL100133 </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/
/*
 * TODO - If we parse network configuration before modbus then modbus configuration gets corrupted */
void getLastKnownConfiguration(void)
{
	/* Read Sector 22 till we get FF */
	uint32_t flashAddress = ADDR_FLASH_SECTOR_11;
	uint32_t loopCounter = 0;

	if(strlen(tempconfig) == 0)
	{
		for(loopCounter = 0 ; loopCounter < 2000; loopCounter++)
		{
			if(*(uint8_t *)flashAddress == 0xFF)
			{
				/* Configuration End . Parse the configuration */
				if(strlen(gau8LastKnownConfiguration) != 0 )
				{
					/* Data corrupting hence coping into a var */
					memset(nwsof, 0, 5);
					memset(nweof, 0, 5);
					memset(mb485sof, 0, 5);
					memset(mb485eof, 0,5);
					memset(mbTCPsof, 0, 5);
					memset(mbTCPeof, 0, 5);

					strcpy(gau8ConfigurationCopy, gau8LastKnownConfiguration);
					/* Validate the configuration */
					nwsof = strstr(gau8LastKnownConfiguration,"NS,");
					nweof = strstr(gau8LastKnownConfiguration,",NE");
					mb485sof =  strstr(gau8LastKnownConfiguration,"MS,");
					mb485eof =  strstr(gau8LastKnownConfiguration,",ME");
					//mbTCPsof =  strstr(gau8LastKnownConfiguration,"ms,");
					//mbTCPeof =  strstr(gau8LastKnownConfiguration,",me");

					if((mb485sof == NULL )|(mb485eof == NULL))
					{
						/*SOF / EOF Not found . Raise Error */
						gu32ConfigModbus485ErrorDatabase |= (1 << enmCONFIG_MB485CONFIGNOTFOUND);
					}
					else
					{
						mb485mlen = mb485eof - mb485sof;
						mb485config = (char *)malloc((sizeof(char) *mb485mlen));
						if(mb485config!= NULL)
						{
							memset(mb485config, 0, mb485mlen);
							memcpy(mb485config,mb485sof,mb485mlen);
							extractModbus485Configuration();
						}
						else
							gu32ConfigModbus485ErrorDatabase |= (1 << enmCONFIG_MB485CONFIGNOTFOUND);
					}

//					if((mbTCPsof == NULL )|(mbTCPeof == NULL))
//					{
//						/*SOF / EOF Not found . Raise Error */
//						gu32ConfigModbusTCPErrorDatabase |= (1 << enmCONFIG_MBTCPCONFIGNOTFOUND);
//					}
//					else
//					{
//						mbtcpmlen = mbTCPeof - mbTCPsof;
//						mbTCPconfig = (char *)malloc((sizeof(char) * mbtcpmlen));
//						if(mbTCPconfig!= NULL)
//						{
//							char u8temp_array[mbtcpmlen];
//							memset(mbTCPconfig, 0, mbtcpmlen);
//							memset(u8temp_array, 0, sizeof(u8temp_array));
//							memcpy(u8temp_array,mbTCPsof,mbtcpmlen);
//							strcpy(mbTCPconfig,u8temp_array);
//							extractModbusTCPConfiguration();
//						}
//						else
//							gu32ConfigModbus485ErrorDatabase |= (1 << enmCONFIG_MB485CONFIGNOTFOUND);
//
//					}

					if((nwsof == NULL )|(nweof == NULL))
					{
						/*SOF / EOF Not found . Raise Error */
						gu32ConfigNetworkErrorDatabase |= (1 << enmCONFIG_NWCONFIGNOTFOUND);
					}
					else
					{
						nwmlen = nweof - nwsof;
						nwconfig = (char *)malloc((sizeof(char) *nwmlen));
						if(nwconfig!= NULL)
						{
							char u8temp_array[nwmlen];
							memset(nwconfig, 0, nwmlen);
							memset(u8temp_array, 0, sizeof(u8temp_array));
							memcpy(u8temp_array,nwsof,nwmlen);
							strcpy(nwconfig,u8temp_array);
							extractNewtorkConfiguration();
						}
						else
							gu32ConfigNetworkErrorDatabase |= (1 << enmCONFIG_NWCONFIGNOTFOUND);

					}
					if((gu32ConfigNetworkErrorDatabase == 0 ) && (nwconfig != NULL))
					{
						/* Update System with N/W Configuration */
						/* APN */
						memset(gau8GSM4G_apn,0x00,sizeof(char) * 100);
						memcpy(gau8GSM4G_apn,gau8ConfigNewtorkAPN, strlen(gau8ConfigNewtorkAPN));
						/* URL */
						memset(gau8GSM_url,0x00,sizeof(char) * 100);
						memcpy( gau8GSM_url, gau8ConfigNewtorkServerURL, strlen(gau8ConfigNewtorkServerURL));
						/* Config URL */
						memset(gau8RemoteConfigurationURL,0x00,sizeof(char) * 100);
						memcpy( gau8RemoteConfigurationURL, gau8ConfigNewtorkConfigURL, strlen(gau8ConfigNewtorkConfigURL));
						/* Upload Time */
						gu32UploadTimeSupply = atoi(gau8ConfigNewtorkUpFreqSupply);
						gu32UploadTimeBatt = atoi(gau8ConfigNewtorkUpFreqBatt);
					}
					if((gu32ConfigModbus485ErrorDatabase == 0 ) && (mb485config != NULL))
					{
						/* Update System with Modbus485 Configuration */
						gu32OperateModbus485 = atoi(gau8ConfigModbus485IsEnabled);
						gu32Modbus485SlaveID = atoi(gau8ConfigModbus485SlaveID);
						gu32Modbus485RegisterFetch = atoi(gau8ConfigModbus485DataPoints);
						gu32Modbus485PollingTime = atoi(gau8ConfigModbus485PollingTime);
						gu32Modbus485ResponseTime = atoi(gau8ConfigModbus485ResponseTime);
						gu32LoopCounter = 0;
						for(gu32LoopCounter = 0; gu32LoopCounter <gu32Modbus485RegisterFetch; gu32LoopCounter++)
						{
							/*Copy MB485 Address/ FC / Points to respective array */
							gu32MBRTUClientAddress[gu32LoopCounter] = gau32ConfigModbus485Address[gu32LoopCounter];
							gu32MBRTUClientFuncCode[gu32LoopCounter] = gau32ConfigModbus485DFunctionCode[gu32LoopCounter];
							gu32MBRTUClientNoofPoints[gu32LoopCounter] = gau32ConfigModbus485NoPoints[gu32LoopCounter];
							/*TCP */
							gu32MBTCPClientAddress[gu32LoopCounter] = gau32ConfigModbusTCPAddress[gu32LoopCounter];
							gu32MBTCPClientFuncCode[gu32LoopCounter] = gau32ConfigModbusTCPDFunctionCode[gu32LoopCounter];
							gu32MBTCPClientNoofPoints[gu32LoopCounter] = gau32ConfigModbusTCPNoPoints[gu32LoopCounter];
						}
						/* TCP Added */
						inet_aton(gau8ConfigModbusTCPIPDEVICE,IP_ADDRESS);
						inet_aton(gau8ConfigModbusTCPIPSUBNET,NETMASK_ADDRESS);
						inet_aton(gau8ConfigModbusTCPIPGATEWAY,GATEWAY_ADDRESS);
						inet_aton(gau8ConfigModbusTCPIPSERVER,IP_ADDRESS_DESTI);
						gu32ModbusTCPPort = atoi(gau8ConfigModbusTCPPORT);

					}
//					if((gu32ConfigModbusTCPErrorDatabase == 0 ) && (mbTCPconfig != NULL))
//					{
//						/* Update System with Modbus485 Configuration */
//						gu32OperateModbusTCP = atoi(gau8ConfigModbusTCPIsEnabled);
//						gu32ModbusTCPRegisterFetch = atoi(gau8ConfigModbusTCPDataPoints);
//						gu32ModbusTCPPollingTime = atoi(gau8ConfigModbusTCPPollingTime);
//						gu32ModbusTCPResponseTime = atoi(gau8ConfigModbusTCPResponseTime);
//						gu32LoopCounter = 0;
//						for(gu32LoopCounter = 0; gu32LoopCounter <gu32ModbusTCPRegisterFetch; gu32LoopCounter++)
//						{
//							/*Copy MB485 Address/ FC / Points to respective array */
//							gu32MBTCPClientAddress[gu32LoopCounter] = gau32ConfigModbusTCPAddress[gu32LoopCounter];
//							gu32MBTCPClientFuncCode[gu32LoopCounter] = gau32ConfigModbusTCPDFunctionCode[gu32LoopCounter];
//							gu32MBTCPClientNoofPoints[gu32LoopCounter] = gau32ConfigModbusTCPNoPoints[gu32LoopCounter];
//						}
//
//						inet_aton(gau8ConfigModbusTCPIPDEVICE,IP_ADDRESS);
//						inet_aton(gau8ConfigModbusTCPIPSUBNET,NETMASK_ADDRESS);
//						inet_aton(gau8ConfigModbusTCPIPGATEWAY,GATEWAY_ADDRESS);
//						inet_aton(gau8ConfigModbusTCPIPSERVER,IP_ADDRESS_DESTI);
//						gu32ModbusTCPPort = atoi(gau8ConfigModbusTCPPORT);
//					}
					/* Data corrupting hence coping into a var */
					strcpy(gau8LastKnownConfiguration,gau8ConfigurationCopy);
				}
				break;
			}
			else
			{
				gau8LastKnownConfiguration[loopCounter] = *(uint8_t *)flashAddress;
				flashAddress++;
			}
		}
	}
}

//******************************* End of File *******************************************************************
