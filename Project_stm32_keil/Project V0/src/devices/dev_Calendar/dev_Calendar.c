/*************************************************************************//**
 * @file    dev_Calendar.h
 * @mcu		STM32F1x
 * @brief   Calendar Device Manager
 * @date    08.07.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <RTL.h>
#include "dev_Calendar_hsm.h"		// includes dev_Calendar.h


 
/****************************************************************************************************//**
 *                        PRIVATE MEMBERS
 ********************************************************************************************************/

#define CALENDAR_STACK_SIZE			300	// tamaño en bytes
static volatile unsigned long long 	Calendar_Stack[(CALENDAR_STACK_SIZE/8)];


static T_DEV_Calendar This;
static void 	Calendar_Task 		(Object *pThis);

/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/****************************************************************************************************//**
 * @fun		dev_Calendar_Init
 * @brief	Provided interface used by the initial system loading to carry out Calendar Device initialization
 * @return	
 **********************************************************************************/
uint8_t dev_Calendar_Init(uint8_t priority){
	ErrorCode err = ERR_OK;

	// disable event propagation to GUI manager
	This.ctrlFlags &= ~CAL_ALLOW_EVENT_NOTIFICATION_MASK;

	// set default settings for auxiliar calendar
	// mode 24h, seasons: Enabled & Automatic, latitude: North
	This.calendar.status = CAL_DEF_STATUS;

	//Initialize hw_driver
	IDriverInit();

	// Initialize IRunnable interface. If succesful, an OS_TID is assigned.
	err = IRunnable_Init   (&This.runnable, 
							&This, 
							&This.runnable.tid, 
							0, 
							0, 
							(void*)&Calendar_Stack, 
							(U16)CALENDAR_STACK_SIZE,
							priority,
							&Calendar_Task,
							&Calendar_State_Initial);
	return((uint8_t)err);
}

/****************************************************************************************************//**
 * @fun		dev_Calendar_EnDisEvents
 * @brief	Provided interface to enable/disable event propagation to GUI manager
 * @param	endis	setup config: if 1 then enable, if 0 then disable
 * @return	
 **********************************************************************************/
void dev_Calendar_EnDisEvents(uint8_t endis){
	EnterCritical();
	if(endis){
		This.ctrlFlags |= CAL_ALLOW_EVENT_NOTIFICATION_MASK;
		IDriverEnable();
	}
	else{
		This.ctrlFlags &= ~CAL_ALLOW_EVENT_NOTIFICATION_MASK;
		IDriverDisable();
	}
	ExitCritical();
}

/****************************************************************************************************//**
 * @fun		dev_Calendar_NewTime
 * @brief	Provided interface used by the RTC driver to notify time events
 * @return	
 **********************************************************************************/
void dev_Calendar_NewTime(void){
	isr_evt_set (EVF_NEWTIME, This.runnable.tid);
}


/****************************************************************************************************//**
 * @fun		dev_Calendar_Get
 * @brief	Provided interface to get the actual time and/or date
 * @return	
 **********************************************************************************/
void dev_Calendar_Get(CCalendar *c){
	uint32_t counter;
	uint8_t notused;
	// read current time
	counter = IGetTime();
	EnterCritical();
	// get calendar. If date < 2010, sets 1/1/2010 and updates hardware counter
	if(CCalendar_GetFromSeconds(&This.calendar, counter) > 0){
		counter = CCalendar_GetInSeconds(&This.calendar);
		ISetTime(counter);
	}
	CCalendar_CalcSeason(&This.calendar, &notused);
	CCalendar_CheckSeason(&This.calendar, &notused);
	CRtc_CalcWeekday(&This.calendar.objRtc_sistema, 1);
	memcpy(c, &This.calendar, sizeof(CCalendar));
	ExitCritical();
}


/****************************************************************************************************//**
 * @fun		dev_Calendar_GetCounter
 * @brief	Provided interface to get the actual time and/or date in seconds
 * @return	time in seconds
 **********************************************************************************/
uint32_t dev_Calendar_GetCounter(void){
	// read current time
	return(IGetTime());
}

/****************************************************************************************************//**
 * @fun		dev_Calendar_Set
 * @brief	Provided interface to set the actual time and/or date
 * @return	
 **********************************************************************************/
void dev_Calendar_Set(CCalendar *c){
	uint32_t counter;
	uint8_t notused;
	EnterCritical();
	memcpy(&This.calendar, c, sizeof(CCalendar));
	CCalendar_CalcSeason(&This.calendar, &notused);
	CCalendar_CheckSeason(&This.calendar, &notused);
	CRtc_CalcWeekday(&This.calendar.objRtc_sistema, 1);
	counter = CCalendar_GetInSeconds(&This.calendar);
	ExitCritical();
	ISetTime(counter);
}
/****************************************************************************************************//**
 * @fun		Calendar_Task
 * @brief	Task for this device. Launches the initial state of the HSM and afterwards waits for 
 *			incoming  eventflags (os_evt_wait_or) 
 *			If timeout is overrun then a timeout event is dispatched to the
 *			HSM. In other case, the proper event is dispatched.
 * @return	
 **********************************************************************************/
static void Calendar_Task(Object *pThis){
	T_DEV_Calendar * me = (T_DEV_Calendar*)pThis;
    // Starts the state machine
    StateMachine_Init(me);       

	for(;;){
		OS_RESULT errno;
		const Event newTimeEvt = {EV_NEWTIME};
		errno = os_evt_wait_or(EVF_NEWTIME, me->timeout);
		// if timeout, launch timeout event
		if(errno == OS_R_TMO){
			StateMachine_Dispatch(me, (Event const *)&timeoutEvt);
		}
		// if EVF_NEWTIME flag, check and dispatch
		else{
			os_evt_clr(EVF_NEWTIME, me->runnable.tid);
			StateMachine_Dispatch(me, (Event const *)&newTimeEvt);
		}
	}
}









