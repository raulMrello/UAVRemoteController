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

/** Rtc reference */
typedef struct RtcCtrl_t* drv_RTC;

/** Callbacks for time events and handler objects */
typedef void* RtcHandlerObj;
typedef void (*OnRtcTimeEvent)(RtcHandlerObj handler);

/** Rtc status enumeration */
typedef enum{
	RTC_EVENTS_ENABLED = (1 << 0)
}RtcStatusEnum;

/** Rtc status flag combination */
typedef uint32_t RtcFlags;

//------------------------------------------------------------------------------------
//-- PROTOTYPES ----------------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------

/** \fun drv_RTC_Init
 *  \brief Initializes RTC peripheral
 *	\param cbTimeEvent Time events callback
 *	\param handler Callback handler
 *	\return Rtc driver reference
 */
drv_RTC drv_RTC_Init(OnRtcTimeEvent cbTimeEvent, RtcHandlerObj handler);

/** \fn drv_RTC_endisEvents
 *  \brief Enables/Disables Rtc 1-second events
 *  \param drv Rtc reference
 *  \param endis En/dis flag
 */
void drv_RTC_endisEvents(drv_RTC drv, uint8_t flag);

/** \fn drv_RTC_getTime
 *  \brief Get Rtc time
 *  \param drv Rtc reference
 *  \return time
 */
uint32_t drv_RTC_getTime(drv_RTC drv);

/** \fn drv_RTC_setTime
 *  \brief Get Rtc time
 *  \param drv Rtc reference
 *  \param time New time
 */
void drv_RTC_setTime(drv_RTC drv, uint32_t time);

#ifdef __cplusplus
}
#endif



#endif	//__DRV_RTC_H__
