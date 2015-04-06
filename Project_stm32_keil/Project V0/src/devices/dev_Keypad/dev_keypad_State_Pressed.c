/*************************************************************************//**
 * @file    dev_KeyPad_State_Pressed.h
 * @mcu		STM32F1x
 * @brief   KeyPad Device State Machine Pressed state implementation
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
static State OnForgetKey 	(T_DEV_KeyPad * me, Event const * m);
static State OnTimeout		(T_DEV_KeyPad * me, Event const * m);


/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/*************************************************************************//**
 * @fun     KeyPad_State_Pressed
 * @brief   Manage pressed states until key release or unmatch detection
 *
 * @param   pThis      	this object as void
 * @param	e			Events received by this state
 *
 * @return  n/a
 ****************************************************************************/
State KeyPad_State_Pressed(Object* pThis, Event const * e){
	T_DEV_KeyPad * me = (T_DEV_KeyPad*)pThis;
	switch(e->sig){
		case ENTRY_SIG:
			return(OnEntry(me,e));

		case EXIT_SIG:
			return(OnExit(me,e));

		case EV_NEWKEY:
			return(OnNewKey(me,e));

		case EV_FORGETKEY:
			return(OnForgetKey(me,e));
				
		case TIMEOUT_SIG:	
			return(OnTimeout(me,e));

	}
    SUPER(&KeyPad_State_Top);
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
	me->timeout = KeyPad_SCAN_TIMEOUT;		// waits to rescan the keyboard
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
	(void)m;	// not used
	// read current key
	me->keycode = IGetKey();
	if(me->keycode != (uint32_t)-1){		
		ISendKey(me->keycode);
	}
	HANDLED();
}

/*************************************************************************//**
 * @fun     OnForgetKey
 * @brief   Handles EV_SEND events
 *
 * @param   This      	this object
 * @param	m			Event received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnForgetKey(T_DEV_KeyPad * me, Event const * m){
	(void)m;	// not used
	// reset current key
	me->keycode = (uint32_t)-1;
	TRAN(&KeyPad_State_Top);
}

/*************************************************************************//**
 * @fun     OnTimeout
 * @brief   Handles TIEMOUT_SIG event. Timeouts only can be due to event flag
 *			wait timeouts. In those cases, action to perform is rescanning the
 *			keyboard to detect key repeated or released events.
 *
 * @param   This      	this object
 * @param	m			Event in process <me->msg>
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnTimeout(T_DEV_KeyPad * me, Event const * m){
	(void)m; // not used
	// scan the keyboard
	me->keycode = IGetKey();
	if(me->keycode == (uint32_t)-1){
		TRAN(&KeyPad_State_Top);
	}
	ISendKey(me->keycode);
	HANDLED();
}




