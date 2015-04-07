/*
 * drv_GPIO.h
 *
 *  Created on: 07/04/2015
 *      Author: raulMrello
 *
 *  GPIO driver for STM32F103C8T6. It controls:
 *
 *  	- 8 keyboard inputs
 *  	- 4 selector mode inputs
 *  	- 2 digital outputs
 *
 *  On initialization "drv_GPIO_Init()", io lines are configured as:
 *
 *  	- Inputs interrupt on falling edges
 *  	- Outputs push-pull set to 0
 *  	- Internal function "setOutput" will be subscribed to OutputTopics updates
 *
 *  When an input interrupt occurs, all inputs are read and stored in a bitmask "key" value.
 *  Then a Topic_notify is published for all subscribed objects.
 *
 *  When an external module want to set an output value, it must publish a Topic_notify, in
 *  this case, the topic type must be "OutputTopic" and should include output_id and value.
 *
 *
 */

#ifndef __DRV_GPIO_H
#define	__DRV_GPIO_H

#include "stm32f10x.h"	    	///< STM32F10x Library Definitions
#include "InputTopics.h"		///< required to publish /key topic updates
#include "OutputTopics.h"		///< required to receive /out topic updates
#include "mmf.h"				///< required for external TopicData type



/** \fun drv_GPIO_Init
 *  \brief Initializes GPIO lines
 *	\param e Exception handler
 */
void drv_GPIO_Init(Exception *e);

/** \fn drv_GPIO_OnTopicUpdate
 *  \brief Interface to receive topic updates from external objects
 *  \param obj Void pointer to the object who manages this callback
 *  \param td Reference to the topic data which has been updated
 */
void drv_GPIO_OnTopicUpdate(void * obj, TopicData * td);

#endif

