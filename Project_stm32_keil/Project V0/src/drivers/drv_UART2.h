/*
 * drv_UART2.h
 *
 *  Created on: 07/04/2015
 *      Author: raulMrello
 *
 *  UART2 driver for STM32F103C8T6. It controls:
 *
 *  	- LINK module
 *
 *  On initialization "drv_UART2_Init()", port is configured as:
 *
 *  	- Full-duplex, interrupts disabled and peripheral ready
 *  	- Subscribed to /uart, /rc, /gps topic updates
 *
 *  Basic operation:
 *		1. Module is ready for operation
 *		2. After a /uart topic update it manages accordingly. These topics are configuration topics to start, stop...
 *		3. On received data, it processes and builds /uart topic updates
 *		4. On completion, publishes /uart topic update.
 *
 */
 
#ifndef __DRV_UART2_H__
#define __DRV_UART2_H__


#include "stm32f10x.h"	    	///< STM32F10x Library Definitions
#include "UART_defines.h"		///< Common UART types
#include "UartTopics.h"			///< required to publish, receive /uart topic updates
#include "DataTopics.h"			///< required to publish, /rc /gps topic updates
#include "mmf.h"				///< required for external TopicData type



/** \fun drv_UART2_Init
 *  \brief Initializes UART2 peripheral
 *	\param e Exception handler
 */
void drv_UART2_Init(Exception *e);

/** \fn drv_UART2_OnTopicUpdate
 *  \brief Interface to receive topic updates from external objects
 *  \param obj Void pointer to the object who manages this callback
 *  \param td Reference to the topic data which has been updated
 */
void drv_UART2_OnTopicUpdate(void * obj, TopicData * td);



#endif	//__DRV_UART2_H__
