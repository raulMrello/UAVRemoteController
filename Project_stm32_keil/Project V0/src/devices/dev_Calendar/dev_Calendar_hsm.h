/*************************************************************************//**
 * @file    dev_Calendar_hsm.h
 * @mcu		STM32F1x
 * @brief   Calendar Device State Machine protected header
 * @date    08.07.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef __dev_Calendar_HSM_H__
#define __dev_Calendar_HSM_H__

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <stdint.h>
#include <string.h>
#include "StateFramework.h"
#include "IRunnable.h"
#include "dev_Calendar.h"


/****************************************************************************************************//**
 *                        	REQUIRED INTERFACES
 ********************************************************************************************************/
#define IDriverInit()			dev_Calendar_Requires_IDriverInit()
#define IDriverEnable()			dev_Calendar_Requires_IDriverEnable()
#define IDriverDisable()		dev_Calendar_Requires_IDriverDisable()
#define IGetTime()				dev_Calendar_Requires_IGetTime()					
#define ISetTime(t)				dev_Calendar_Requires_ISetTime(t)					
#define ISendTime(t)			dev_Calendar_Requires_ISendTime(t)		


/****************************************************************************************************//**
 *                        PROTECTED STATE DEFINITIONS
 ********************************************************************************************************/

State Calendar_State_Initial(Object* pThis, Event const * e);
State Calendar_State_Top	(Object* pThis, Event const * e);

/****************************************************************************************************//**
 *                        PROTECTED TYPES
 ********************************************************************************************************/

/*************************************************************************//**
 * @def	CAL_ALLOW_EVENT_NOTIFICATION_MASK
 * @scope	protected
 * @brief   Mask for <ctrlFlags> in order to remind endis event propagation setup
 ****************************************************************************/
#define CAL_ALLOW_EVENT_NOTIFICATION_MASK	1

/*************************************************************************//**
 * @macro	AreEventNotificationAllowed
 * @scope	protected
 * @brief   Macro to check if event notification is allowed	into statemaachine
 ****************************************************************************/
#define AreEventNotificationAllowed()	(me->ctrlFlags & CAL_ALLOW_EVENT_NOTIFICATION_MASK)

/*************************************************************************//**
 * @enum	E_CALENDAR_EVENTS
 * @scope	protected
 * @brief   Event signals accepted by the state machine of this device
 ****************************************************************************/
typedef enum {
	EV_NEWTIME = USER_SIG		//!< event through NewTime interface
}E_CALENDAR_EVENTS;

/*************************************************************************//**
 * @enum	E_CALENDAR_FLAGS
 * @scope	protected
 * @brief   EventFlags masks accepted by this device during os_evt_wait_or
 ****************************************************************************/
typedef enum {
	EVF_NEWTIME   = 0x0001	//!< event through NewTime interface
}E_CALENDAR_FLAGS;


/*************************************************************************//**
 * @struct	T_DEV_Calendar
 * @scope	protected
 * @brief   This structure is the internal device structure with all the static
 *			variable required for functionality.
 ****************************************************************************/
typedef struct{
	IRunnable 		runnable;	//!< Interface to execute a thread with a mailhox
	uint32_t		uid;		//!< Component UID
	uint32_t		timeout;	//!< timeout for os_evt_wait_or
	uint32_t		ctrlFlags;	//!< flags de control
	CCalendar		calendar;	//!< calendar data type for season control
}T_DEV_Calendar;


/****************************************************************************************************//**
 *                        PROTECTED FUNTIONS
 ********************************************************************************************************/
void 	 GetCalendarFromTime(CCalendar *cal, uint32_t counter);
uint32_t GetTimeFromCalendar(CCalendar *cal);

#endif

