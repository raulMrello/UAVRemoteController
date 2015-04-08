/*
 * drv_RTC.h
 *
 *  Created on: 07/04/2015
 *      Author: raulMrello
 *
 *  RTC driver for STM32F103C8T6. It controls:
 *
 *  	- Internal Real time clock
 *
 *  On initialization "drv_RTC_Init()":
 *
 *  	- Attaches to /sys topic updates
 *
 *  Basic operation:
 *		1. Module is ready for operation
 *		2. After a /sys topic update with (time, alarmPace) params it manages accordingly. 
 *		3. It can generate /time topic updates
 *
 */
 
#ifndef __DRV_RTC_H__
#define __DRV_RTC_H__


#include "stm32f10x.h"	    	///< STM32F10x Library Definitions
#include "SystemTopics.h"		///< required to receive /sys topic updates
#include "TimeTopics.h"			///< required to publish /time topic updates
#include "mmf.h"				///< required for external TopicData type


//------------------------------------------------------------------------------------
//-- BSP DEFINES  --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#define BSP_RTC_init				drv_RTC_Init
#define BSP_RTC_OnTopicUpdate		drv_RTC_OnTopicUpdate


//------------------------------------------------------------------------------------

/** \fun drv_RTC_Init
 *  \brief Initializes RTC peripheral
 *	\param e Exception handler
 */
void drv_RTC_Init(Exception *e);

/** \fn drv_RTC_OnTopicUpdate
 *  \brief Interface to receive topic updates from external objects
 *  \param obj Void pointer to the object who manages this callback
 *  \param td Reference to the topic data which has been updated
 */
void drv_RTC_OnTopicUpdate(void * obj, TopicData * td);

#endif	//__DRV_RTC_H__
