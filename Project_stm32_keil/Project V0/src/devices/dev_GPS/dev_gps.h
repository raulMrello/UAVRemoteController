/*************************************************************************//**
 * @file    dev_GPS.h
 * @mcu		STM32F1x
 * @brief   GPS Device Manager
 * @date    11.06.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef _DEV_GPS_H
#define _DEV_GPS_H

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <stdint.h>

/****************************************************************************************************//**
 *                        PROVIDED INTERFACES
 ********************************************************************************************************/
uint8_t	dev_GPS_Init	  	(uint8_t priority);
uint8_t dev_GPS_GetStatus 	(void);
void 	dev_GPS_PowerOn	  	(void);
void 	dev_GPS_PowerOff  	(void);	
void 	dev_GPS_NewRecvData	(void);
void 	dev_GPS_NewSentData (void);
uint8_t	dev_GPS_IsReadyForPowerDown (void);

/****************************************************************************************************//**
 *                        REQUIRED INTERFACES
 *
 * To be implemented in <SYS_Link.c>
 ********************************************************************************************************/
extern void (* const dev_GPS_Requires_IDriverInit)(uint32_t baudrate);
extern void (* const dev_GPS_Requires_IDriverEnableRx)(void);
extern void (* const dev_GPS_Requires_IDriverDisableRx)(void);
extern void (* const dev_GPS_Requires_IDriverEnable)(void);
extern void (* const dev_GPS_Requires_IDriverTurnOn)(void);
extern void (* const dev_GPS_Requires_IDriverTurnOff)(void);
extern int  (* const dev_GPS_Requires_IGetData)(void);
extern void (* const dev_GPS_Requires_IDataReady)(uint8_t *pdata, uint16_t size);
extern uint8_t	(* const dev_GPS_Requires_ISend)(uint8_t *ptr, uint8_t size);


/****************************************************************************************************//**
 *                        PUBLIC TYPES 
 ********************************************************************************************************/
   


#endif

