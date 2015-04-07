/*************************************************************************//**
 * @file    drv_UART1.h
 * @mcu		STM32F1x
 * @brief   USART1 peripheral controller
 * @date    14.06.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef __SRC_DRIVERS_BSP_RC_H__
#define __SRC_DRIVERS_BSP_RC_H__

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/

#include <stdint.h>
#include "drv_GPIO.h"



/****************************************************************************************************//**
 * @fun		BSP_init
 * @brief	Initializes BSP of the UAVRC:
 *			USART1 (PA9,PA10) 	-> WifiLink
 *			USART2 (PA2,PA3)  	-> GpsModule
 *			GPIO   (PA4,5,6,7)	-> ModeSelector
 *			GPIO   (PB0,1,2,10
 *					11,12,13,14)-> Keyboard
 *			GPIO   (PA8)		-> Output_0
 *			GPIO   (PB15)		-> Output_1
 *			TIM4   (PB6,7,8,9)	-> PwmOutputs

 * @return	n/a
 **********************************************************************************/
void BSP_RC_init(void);


#define BSP_GPS_init	drv_UART1_init

int WifiLink_write(void * data, int size);
int WifiLink_read(void * data, int size);
int GpsModule_read(void * data, int size);
int ModeSelector_read(void);
int Keyboard_read(void);
void Output_write(int id, int value);
void PwmOutput_start(int id, int duty, int freq);
void PwmOutput_stop(int id);



#endif	// __SRC_DRIVERS_BSP_RC_H__