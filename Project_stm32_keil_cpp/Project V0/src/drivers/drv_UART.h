/*
 * drv_UART.h
 *
 *  Created on: 07/04/2015
 *      Author: raulMrello
 *
 *  UART driver for STM32F103C8T6. It controls:
 *
 *  	- GPS module (uart 1)
 *		- WIFI module (uart 2)
 *
 *  On initialization "drv_UART_Init()", port is configured as:
 *
 *  	- Full-duplex, interrupts disabled and peripheral ready
 *		- Callback handlers can be set null
 *		- Reception must be enabled in order to receive data (callback is invoked when an IDLE event detected)
 *		- Transmission is enabled invoking drv_UART_write (callback invoked on last byte sent)
 *
 */
 
#ifndef __DRV_UART_H__
#define __DRV_UART_H__

#ifdef __cplusplus
 extern "C" {
#endif 


//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include <stdint.h>
#include <stdlib.h>				///< malloc
#include "stm32f10x.h"	    	///< STM32F10x Library Definitions


//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** Uart reference */
typedef struct UartCtrl_t* drv_UART;

/** Callbacks for data transmission, reception and handler objects */
typedef void* UartHandlerObj;
typedef void (*OnUartDataReady)(UartHandlerObj handler);
typedef void (*OnUartDataSent)(UartHandlerObj handler);


//------------------------------------------------------------------------------------
//-- PROTOTYPES ----------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** \fun drv_UART_Init
 *  \brief Initializes UART peripheral
 *	\param channel Uart channel
 *	\param tbufsize Size of transmission buffer (2,4,8,16,32,64,128,256...)
 *	\param rbufsize Size of reception buffer (2,4,8,16,32,64,128,256...)
 *	\param cbDataReady Data ready callback
 *	\param cbDataSent Data sent callback
 *	\param handler Callback handler
 *	\return Reference to UART or null.
 */
drv_UART drv_UART_Init(	uint8_t channel, uint16_t tbufsize, uint16_t rbufsize,
						OnUartDataReady cbDataReady, OnUartDataSent cbDataSent, UartHandlerObj handler);

/** \fun drv_UART_endisRx
 *  \brief Enables/disables reception
 *	\param drv Uart reference
 *	\param endis En/dis flag
 */
void drv_UART_endisRx(drv_UART drv, uint8_t endis);

/** \fun drv_UART_write
 *  \brief Writes data to UART peripheral
 *	\param drv Uart reference
 *	\param buffer Data to send
 *	\param size Data size
 *	\param cbDataSent Data sent callback
 *	\param handler Callback handler
 *	\return Number of bytes sent
 */
int drv_UART_write(drv_UART drv, char *buffer, uint16_t size, OnUartDataSent cbDataSent, UartHandlerObj handler);

/** \fun drv_UART_read
 *  \brief Reads data from UART peripheral
 *	\param drv Uart refrerence
 *  \param buffer Buffer to receive data
 *  \param size Max number of bytes to be read
 *  \return Number of bytes read
 */
int drv_UART_read(drv_UART drv, char *buffer, uint16_t size);


#ifdef __cplusplus
}
#endif


#endif	/*__DRV_UART_H__*/
