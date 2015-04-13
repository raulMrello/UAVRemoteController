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
 *		- 1 pwm output pin direction (pwm functions handled by drv_TIM3)
 *
 *  On initialization "drv_GPIO_Init()", io lines are configured as:
 *
 *  	- Inputs interrupt on falling edges
 *  	- Outputs push-pull set to 0
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

#ifdef __cplusplus
 extern "C" {
#endif 


//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include <stdint.h>	 
#include "stm32f10x.h"	    	///< STM32F10x Library Definitions


//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** Uart reference */
typedef struct GpioCtrl_t* drv_GPIO;

/** Callbacks for input change detection */
typedef void* GpioHandlerObj;
typedef void (*OnGpioInputChange)(GpioHandlerObj handler);

/** Input/Output channel enumeration */
typedef enum{
	IC_N	= (1 << 0),
	IC_NE	= (1 << 1),
	IC_E	= (1 << 2),
	IC_ES	= (1 << 3),
	IC_S	= (1 << 4),
	IC_SW	= (1 << 5),
	IC_W	= (1 << 6),
	IC_WN	= (1 << 7),
	IC_ARM	= (1 << 8),
	IC_LOC	= (1 << 9),
	IC_ALT	= (1 << 10),
	IC_RTH	= (1 << 11),
	OC_0	= (1 << 12),
	OC_1	= (1 << 13),
	OC_PWM	= (1 << 14)
}GpioChannelsEnum;

/** GpioChannels flag combination */
typedef uint32_t GpioFlags;

//------------------------------------------------------------------------------------
//-- PROTOTYPES ----------------------------------------------------------------------
//------------------------------------------------------------------------------------


/** \fun drv_GPIO_Init
 *  \brief Initializes GPIO peripheral
 *	\param cbInputChange Input change callback
 *	\param handler Callback handler
 *	\return Reference to GPIO or null.
 */
drv_GPIO drv_GPIO_Init(	OnGpioInputChange cbInputChange, GpioHandlerObj handler);

/** \fun drv_GPIO_endisEvents
 *  \brief Enables/disables input events
 *	\param drv Gpio reference
 *	\params channels Channels to be updated
 *	\param endis En/dis bitmask flag
 */
void drv_GPIO_endisEvents(drv_GPIO drv, GpioFlags channels, uint8_t endis_flag);

/** \fun drv_GPIO_write
 *  \brief Writes an output value
 *	\param drv GPIO reference
 *	\param channels Ouput channels to update
 *	\param value Value
 */
void drv_GPIO_write(drv_GPIO drv, GpioFlags channels, int value);

/** \fun drv_GPIO_read
 *  \brief Reads GPIO channel
 *	\param drv GPIO refrerence
 *  \return Input/Ouput channels status
 */
GpioFlags drv_GPIO_read(drv_GPIO drv);


#ifdef __cplusplus
}
#endif



#endif	/*__DRV_GPIO_H__*/

