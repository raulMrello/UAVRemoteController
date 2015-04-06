/*************************************************************************//**
 * @file    dev_Calendar_State_Top.h
 * @mcu		STM32F1x
 * @brief   Calendar Device State Machine Top parent state implementation
 * @date    08.07.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        	REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <RTL.h>
#include "dev_Calendar_hsm.h"		// includes drv_Calendar.h

/****************************************************************************************************//**
 *                        	PRIVATE MEMBERS
 ********************************************************************************************************/

/*		Event handling routines		*/
static State OnEntry		(T_DEV_Calendar * me, Event const * m);
static State OnExit			(T_DEV_Calendar * me, Event const * m);
static State OnNewTime 		(T_DEV_Calendar * me, Event const * m);
static State OnTimeout		(T_DEV_Calendar * me, Event const * m);


/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/*************************************************************************//**
 * @fun     Calendar_State_Initial
 * @brief   Initialize internal properties
 *
 * @param   pThis      	this object as void
 * @param	e			Events received by this state
 *
 * @return  n/a
 ****************************************************************************/
State Calendar_State_Initial(Object* pThis, Event const * e){
	T_DEV_Calendar * me = (T_DEV_Calendar*)pThis;
	(void)e;
	me->uid = me->runnable.tid;
	dev_Calendar_Get(&me->calendar);
	//CCalendar_Init(&me->calendar);
	dev_Calendar_Set(&me->calendar);
	TRAN(&Calendar_State_Top);
}


/*************************************************************************//**
 * @fun     Calendar_State_Top
 * @brief   Initialize internal properties
 *
 * @param   pThis      	this object as void
 * @param	e			Events received by this state
 *
 * @return  n/a
 ****************************************************************************/
State Calendar_State_Top(Object* pThis, Event const * e){
	T_DEV_Calendar * me = (T_DEV_Calendar*)pThis;
	switch(e->sig){
		case ENTRY_SIG:
			return(OnEntry(me,e));

		case EXIT_SIG:
			return(OnExit(me,e));

		case EV_NEWTIME:
			return(OnNewTime(me,e));
				
		case TIMEOUT_SIG:	
			return(OnTimeout(me,e));

	}
    SUPER(&StateMachine_Top);
}


/****************************************************************************************************//**
 *                        EVENT HANDLERS in TOP state
 ********************************************************************************************************/

/*************************************************************************//**
 * @fun     OnEntry
 * @brief   Handles ENTRY_SIG events
 *
 * @param   This      	this object
 * @param	m			Event received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnEntry(T_DEV_Calendar * me, Event const * m){
	(void)m;	// not used
	me->timeout = 0xFFFF; 			// waits forever
	HANDLED();
}

/*************************************************************************//**
 * @fun     OnExit
 * @brief   Handles EXIT_SIG events
 *
 * @param   This      	this object
 * @param	m			Event received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnExit(T_DEV_Calendar * me, Event const * m){
	(void)me; (void)m;	// not used
	HANDLED();
}

/*************************************************************************//**
 * @fun     OnNewTime
 * @brief   Handles EV_SEND events
 *
 * @param   This      	this object
 * @param	m			Event received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnNewTime(T_DEV_Calendar * me, Event const * m){
	(void)m;	// not used
	// notify calendar events
	if(AreEventNotificationAllowed()){
		ISendTime(0);
	}
	HANDLED();

}


/*************************************************************************//**
 * @fun     OnTimeout
 * @brief   Handles TIEMOUT_SIG event. Timeouts only can be due to event flag
 *			wait timeouts. In those cases, action to perform is resending the
 *			pdu data previously received from the Send/Read interface. Offspring
 *			states as WaitingAck and WaitingData will delegate timeout processing
 *			to this parent state. This keeps the StateOriented encapsulation.
 *
 * @param   This      	this object
 * @param	m			Event in process <me->msg>
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnTimeout(T_DEV_Calendar * me, Event const * m){
	(void)me; (void)m; // not used
	HANDLED();
}




