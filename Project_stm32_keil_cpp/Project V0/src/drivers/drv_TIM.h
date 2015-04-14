/*
 * drv_TIM.h
 *
 *  Created on: 07/04/2015
 *      Author: raulMrello
 *
 *  TIM driver for STM32F103C8T6. It controls:
 *
 *  	- Buzzer output (pwm) (resonant freq = 4KHz)
 *
 *  On initialization "drv_TIM_Init()", port is configured as:
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
 
#ifndef __DRV_TIM_H__
#define __DRV_TIM_H__

#ifdef __cplusplus
 extern "C" {
#endif 


//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------


#include <stdint.h>
#include "stm32f10x.h"	    	///< STM32F10x Library Definitions


//------------------------------------------------------------------------------------
//-- DEFINES -------------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** Channel attached to pwm output */	 
#define TIM_CHANNEL_PWM		3

//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** Uart reference */
typedef struct TimCtrl_t* drv_TIM;

/** Tim status enum */
typedef enum {
	TIM_TIMMING = (1 << 0)
}TimStatusEnum;

/** Tim flag combination */
typedef uint32_t TimFlags;

//------------------------------------------------------------------------------------
//-- PROTOTYPES ----------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** \fun drv_TIM_Init
 *  \brief Initializes TIM peripheral
 *	\param channel TIM channel
 *	\param period Period in microseconds
 *	\param duty Duty in % (0 = 0.00%, 10000 = 100.00%)
 *	\return Reference to TIM or null.
 */
drv_TIM drv_TIM_Init(uint8_t channel, uint32_t period, uint16_t duty);

/** \fun drv_TIM_update
 *  \brief Updates TIM duty
 *	\param drv Tim reference
 *	\param duty New duty
 */
void drv_TIM_update(drv_TIM drv, uint16_t duty);

/** \fun drv_TIM_getStatus
 *  \brief Get TIM status
 *	\param drv Tim reference
 */
TimFlags drv_TIM_getStatus(drv_TIM drv);


#ifdef __cplusplus
}
#endif


#endif	//__DRV_TIM3_H__
