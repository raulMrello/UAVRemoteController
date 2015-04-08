/*
 * drv_POW.h
 *
 *  Created on: 07/04/2015
 *      Author: raulMrello
 *
 *  POW driver for STM32F103C8T6. It controls:
 *
 *  	- Clock configuration and STOP modes
 *
 *  On initialization "drv_POW_Init()":
 *
 *  	- Attaches to /sys topic updates
 *
 *  Basic operation:
 *		1. Module is ready for operation
 *		2. After a /sys topic update it manages accordingly. 
 *
 */
 
#ifndef __DRV_POW_H__
#define __DRV_POW_H__


#include "stm32f10x.h"	    	///< STM32F10x Library Definitions
#include "SystemTopics.h"		///< required to receive /sys topic updates
#include "mmf.h"				///< required for external TopicData type


//------------------------------------------------------------------------------------
//-- BSP DEFINES  --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#define BSP_POW_init				drv_POW_Init
#define BSP_POW_OnTopicUpdate		drv_POW_OnTopicUpdate


//------------------------------------------------------------------------------------

/** \fun drv_POW_Init
 *  \brief Initializes POW peripheral
 *	\param e Exception handler
 */
void drv_POW_Init(Exception *e);

/** \fn drv_POW_OnTopicUpdate
 *  \brief Interface to receive topic updates from external objects
 *  \param obj Void pointer to the object who manages this callback
 *  \param td Reference to the topic data which has been updated
 */
void drv_POW_OnTopicUpdate(void * obj, TopicData * td);

#endif	//__DRV_POW_H__
