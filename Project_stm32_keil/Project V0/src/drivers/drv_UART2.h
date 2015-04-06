/*************************************************************************//**
 * @file    drv_UART2.h
 * @mcu		STM32F1x
 * @brief   USART2 peripheral controller
 * @date    11.06.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef __UART2_H__
#define __UART2_H__

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <stdint.h>

/****************************************************************************************************//**
 *                        PROVIDED INTERFACES 
 ********************************************************************************************************/
void 	drv_UART2_Init			(uint32_t baudrate);				//!< setups the driver with a baudrate
int 	drv_UART2_SendChar		(char c);							//!< Sends a character via serial link
uint8_t drv_UART2_SendMsg		(uint8_t *ptr, uint8_t size);		//!< Sends message
int 	drv_UART2_GetChar		(void);								//!< Read a received character
void	drv_UART2_EnableRx		(void);								//!< enables reception interrupts
void	drv_UART2_DisableRx		(void);								//!< disables reception interrupts

/****************************************************************************************************//**
 *                        REQUIRED INTERFACES
 *
 * To be implemented in <SYS_Link.c>
 ********************************************************************************************************/
extern void (* const drv_UART2_Requires_IRecvEvent)(void);
extern void (* const drv_UART2_Requires_ISentEvent)(void);


/****************************************************************************************************//**
 *                        PUBLIC TYPES 
 ********************************************************************************************************/

/****************************************************************************************************//**
 * @fun		UART2_ENABLE_FULLDUPLEX
 * @brief	Enable/Disable full duplex capability.
 * 			if 0 then Half duplex
 * 			if 1 then Full duplex
 **********************************************************************************/
#define UART2_ENABLE_FULLDUPLEX		1


#endif
