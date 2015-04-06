/*************************************************************************//**
 * @file    dev_Buzzer_State_Top.h
 * @mcu		STM32F1x
 * @brief   Buzzer Device State Machine Top parent state implementation
 * @date    15.06.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        	REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <RTL.h>
#include "dev_Buzzer_hsm.h"		// includes drv_Buzzer.h

/****************************************************************************************************//**
 *                        	PRIVATE MEMBERS
 ********************************************************************************************************/

/*		Event handling routines		*/
static State OnEntry		(T_DEV_Buzzer * me, T_BUZ_Message const * m);
static State OnExit			(T_DEV_Buzzer * me, T_BUZ_Message const * m);
static State OnTimeout		(T_DEV_Buzzer * me, T_BUZ_Message const * m);
static State OnBeepShort	(T_DEV_Buzzer * me, T_BUZ_Message const * m);
static State OnBeepLong		(T_DEV_Buzzer * me, T_BUZ_Message const * m);
static State OnBeepConfirm	(T_DEV_Buzzer * me, T_BUZ_Message const * m);


/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/*************************************************************************//**
 * @fun     BUZ_State_Initial
 * @brief   Initialize internal properties
 *
 * @param   pThis      	this object as void
 * @param	e			Events received by this state
 *
 * @return  n/a
 ****************************************************************************/
State BUZ_State_Initial(Object* pThis, Event const * e){
	T_DEV_Buzzer * me = (T_DEV_Buzzer*)pThis;
	(void)e;
	me->uid = me->runnable.tid;
	me->cnfrmCnt = 0;
	TRAN(&BUZ_State_Top);
}


/*************************************************************************//**
 * @fun     BUZ_State_Initial
 * @brief   Initialize internal properties
 *
 * @param   pThis      	this object as void
 * @param	e			Events received by this state
 *
 * @return  n/a
 ****************************************************************************/
State BUZ_State_Top(Object* pThis, Event const * e){
	T_DEV_Buzzer * me = (T_DEV_Buzzer*)pThis;
	switch(e->sig){
		case ENTRY_SIG:
			return(OnEntry(me,(T_BUZ_Message const *)e));

		case EXIT_SIG:
			return(OnExit(me,(T_BUZ_Message const *)e));
						
		case TIMEOUT_SIG:	
			return(OnTimeout(me,(T_BUZ_Message const *)e));

		case EV_BEEPSHORT:
			return(OnBeepShort(me,(T_BUZ_Message const *)e));

		case EV_BEEPLONG:
			return(OnBeepLong(me,(T_BUZ_Message const *)e));

		case EV_BEEPCONFIRM:
			return(OnBeepConfirm(me,(T_BUZ_Message const *)e));
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
 * @param	m			T_BUZ_Message received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnEntry(T_DEV_Buzzer * me, T_BUZ_Message const * m){
	(void)m;	// not used
	me->waitfor = WAITFOR_EVT;		// waits on os_evt_wait_or
	me->timeout = 0xFFFF; 			// forever till a msg is received
									// clear all processable events
	os_evt_clr(EVF_BUZ_ALLFLAGS, me->runnable.tid);
	HANDLED();
}

/*************************************************************************//**
 * @fun     OnExit
 * @brief   Handles EXIT_SIG events
 *
 * @param   This      	this object
 * @param	m			T_BUZ_Message received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnExit(T_DEV_Buzzer * me, T_BUZ_Message const * m){
	(void)me; (void)m;	// not used
	HANDLED();
}

/*************************************************************************//**
 * @fun     OnBeepShort
 * @brief   Handle BeepShort requests. Starts the sound an setup the waiting time
 *
 * @param   This      	this object
 * @param	m			T_BUZ_Message received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnBeepShort(T_DEV_Buzzer * me, T_BUZ_Message const * m){
	// statrs the sound
	IStart();
	// wait for timer to stop
	me->timeout = BUZ_SHORT_TIME;
	HANDLED();
}

/*************************************************************************//**
 * @fun     OnBeepLong
 * @brief   Handle BeepLong requests. Starts the sound an setup the waiting time
 *
 * @param   This      	this object
 * @param	m			T_BUZ_Message received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnBeepLong(T_DEV_Buzzer * me, T_BUZ_Message const * m){
	// statrs the sound
	IStart();
	// wait for timer to stop
	me->timeout = BUZ_LONG_TIME;
	HANDLED();
}

/*************************************************************************//**
 * @fun     OnBeepConfirm
 * @brief   Handle BeepConfirm requests. Starts the sound an setup the waiting time
 *
 * @param   This      	this object
 * @param	m			T_BUZ_Message received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnBeepConfirm(T_DEV_Buzzer * me, T_BUZ_Message const * m){
	// statrs the sound
	IStart();
	// wait for timer to stop
	me->timeout = BUZ_CONFIRM_TIME;
	me->cnfrmCnt = 1;
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
 * @param	m			T_BUZ_Message in process <me->msg>
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnTimeout(T_DEV_Buzzer * me, T_BUZ_Message const * m){
	// disable the sound
	IStop();
	// if confirmation in progress then...
	if(me->cnfrmCnt > 0 && me->cnfrmCnt < (BUZ_N_CONFIRM_BEEPS*2)){
		me->cnfrmCnt++;
		me->timeout = BUZ_SLEEP_TIME;
		// if it is pair resound, else only wait for SHORT_TIME
		if((me->cnfrmCnt & 1) == 1){
			IStart();
			me->timeout = BUZ_CONFIRM_TIME;
		}
		// last beep is long
		if(me->cnfrmCnt == (BUZ_N_CONFIRM_BEEPS*2)-1){
			me->timeout = BUZ_LONG_TIME;
		}

		HANDLED();
	}
	// wait for new messages forever
	me->timeout = 0xFFFF;
	me->cnfrmCnt = 0;
	HANDLED();
}




