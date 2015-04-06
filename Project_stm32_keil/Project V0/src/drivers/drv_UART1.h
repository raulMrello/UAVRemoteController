/*************************************************************************//**
 * @file    drv_UART1.h
 * @mcu		STM32F1x
 * @brief   USART1 peripheral controller
 * @date    14.06.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef __UART1_H__
#define __UART1_H__

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <stdint.h>

/****************************************************************************************************//**
 *                        PROVIDED INTERFACES 
 ********************************************************************************************************/
void 	drv_UART1_Init			(uint32_t baudrate);				//!< setups the driver with a baudrate
int 	drv_UART1_SendChar		(char c);							//!< Sends a character via serial link
uint8_t drv_UART1_SendMsg		(uint8_t *ptr, uint8_t size);		//!< Sends message
int 	drv_UART1_GetChar		(void);								//!< Read a received character
void	drv_UART1_EnableRx		(void);								//!< enables reception interrupts
void	drv_UART1_DisableRx		(void);								//!< disables reception interrupts

/****************************************************************************************************//**
 *                        REQUIRED INTERFACES
 *
 * To be implemented in <SYS_Link.c>
 ********************************************************************************************************/
extern void (* const drv_UART1_Requires_IRecvEvent)(void);
extern void (* const drv_UART1_Requires_ISentEvent)(void);


/****************************************************************************************************//**
 *                        PUBLIC TYPES 
 ********************************************************************************************************/

/****************************************************************************************************//**
 * @fun		UART1_ENABLE_FULLDUPLEX
 * @brief	Enable/Disable full duplex capability.
 * 			if 0 then Half duplex
 * 			if 1 then Full duplex
 **********************************************************************************/
#define UART1_ENABLE_FULLDUPLEX		1

#endif
