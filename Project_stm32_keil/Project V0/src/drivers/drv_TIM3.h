/*
 * drv_TIM3.h
 *
 *  Created on: 07/04/2015
 *      Author: raulMrello
 *
 *  TIM3 driver for STM32F103C8T6. It controls:
 *
 *  	- Buzzer output (pwm) (resonant freq = 4KHz)
 *
 *  On initialization "drv_TIM3_Init()", port is configured as:
 *
 *  	- TIM3 clock comes from APB1 (HCLK/2) and HCLK=72MHz, then 36MHz
 *		- Is desired a tick = 1us, then max PWM period can reach 65.5ms (65535 ticks)
 *		- TIM3 preescaler is set to 36, then TIM3_clk = 1MHz and tick=1us
 *		- TIM3 period = 250 (250us => 4KHz => buzzer_resonant_frequency)
 *		- TIM3 duty = 125 (50%)
 *		- Attaches to /pwm topic updates
 *
 *  Basic operation:
 *		1. Module is ready for operation
 *		2. After a /pwm topic update it manages accordingly.
 */
 
#ifndef __DRV_TIM3_H__
#define __DRV_TIM3_H__


#include "stm32f10x.h"	    	///< STM32F10x Library Definitions
#include "OutputTopics.h"		///< required to receive /pwm topic updates
#include "mmf.h"				///< required for external TopicData type


//------------------------------------------------------------------------------------
//-- BSP DEFINES  --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#define BPS_BUZZER
//#define BSP_PPM

#if defined(BSP_BUZZER)
#define BSP_BUZZER_init					drv_TIM3_Init
#define BSP_BUZZER_OnTopicUpdate		drv_TIM3_OnTopicUpdate
#endif
#if defined(BSP_PPM)
#define BSP_PPM_init					drv_TIM3_Init
#define BSP_PPM_OnTopicUpdate			drv_TIM3_OnTopicUpdate
#endif

//------------------------------------------------------------------------------------

/** \fun drv_TIM3_Init
 *  \brief Initializes TIM3 peripheral
 *	\param e Exception handler
 */
void drv_TIM3_Init(Exception *e);

/** \fn drv_TIM3_OnTopicUpdate
 *  \brief Interface to receive topic updates from external objects
 *  \param obj Void pointer to the object who manages this callback
 *  \param td Reference to the topic data which has been updated
 */
void drv_TIM3_OnTopicUpdate(void * obj, TopicData * td);



#endif	//__DRV_TIM3_H__
