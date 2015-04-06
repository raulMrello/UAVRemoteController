/*************************************************************************//**
 * @file    dev_KeyPad_State_Top.h
 * @mcu		STM32F1x
 * @brief   KeyPad Device State Machine Top parent state implementation
 * @date    08.07.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        	REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <RTL.h>
#include "dev_KeyPad_hsm.h"		// includes drv_KeyPad.h

/****************************************************************************************************//**
 *                        	PRIVATE MEMBERS
 ********************************************************************************************************/

/*		Event handling routines		*/
static State OnEntry		(T_DEV_KeyPad * me, Event const * m);
static State OnExit			(T_DEV_KeyPad * me, Event const * m);
static State OnNewKey 		(T_DEV_KeyPad * me, Event const * m);
static State OnTimeout		(T_DEV_KeyPad * me, Event const * m);


/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/*************************************************************************//**
 * @fun     KeyPad_State_Initial
 * @brief   Initialize internal properties
 *
 * @param   pThis      	this object as void
 * @param	e			Events received by this state
 *
 * @return  n/a
 ****************************************************************************/
State KeyPad_State_Initial(Object* pThis, Event const * e){
	T_DEV_KeyPad * me = (T_DEV_KeyPad*)pThis;
	(void)e;
	me->uid = me->runnable.tid;
	TRAN(&KeyPad_State_Top);
}


/*************************************************************************//**
 * @fun     KeyPad_State_Top
 * @brief   Initialize internal properties
 *
 * @param   pThis      	this object as void
 * @param	e			Events received by this state
 *
 * @return  n/a
 ****************************************************************************/
State KeyPad_State_Top(Object* pThis, Event const * e){
	T_DEV_KeyPad * me = (T_DEV_KeyPad*)pThis;
	switch(e->sig){
		case ENTRY_SIG:
			return(OnEntry(me,e));

		case EXIT_SIG:
			return(OnExit(me,e));

		case EV_NEWKEY:
			return(OnNewKey(me,e));

		case EV_FORGETKEY:	
			IGNORED();
				
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
static State OnEntry(T_DEV_KeyPad * me, Event const * m){
	(void)m;	// not used
	me->keycode = (uint32_t)-1;
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
static State OnExit(T_DEV_KeyPad * me, Event const * m){
	(void)me; (void)m;	// not used
	HANDLED();
}

/*************************************************************************//**
 * @fun     OnNewKey
 * @brief   Handles EV_SEND events
 *
 * @param   This      	this object
 * @param	m			Event received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnNewKey(T_DEV_KeyPad * me, Event const * m){
	uint32_t key;
	(void)m;	// not used
	// read current key
	key = IGetKey();
	if(me->keycode != key && key != (uint32_t)-1){//(uint32_t)-1){
		me->keycode = key;
		ISendKey(me->keycode);
		TRAN(&KeyPad_State_Pressed);
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
static State OnTimeout(T_DEV_KeyPad * me, Event const * m){
	(void)me; (void)m; // not used
	HANDLED();
}




