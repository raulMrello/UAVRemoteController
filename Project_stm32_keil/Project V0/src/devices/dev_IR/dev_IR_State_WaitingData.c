/*************************************************************************//**
 * @file    dev_IR_State_WaitingData.h
 * @mcu		STM32F1x
 * @brief   IR Device State Machine WaitingAck state implementation
 * @date    16.06.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        	REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <RTL.h>
#include <string.h>
#include "dev_IR_hsm.h"		// includes drv_IR.h

/****************************************************************************************************//**
 *                        	PRIVATE MEMBERS
 ********************************************************************************************************/

/*		Event handling routines		*/
static State OnEntry		(T_DEV_IR * me, T_IR_Message const * m);
static State OnExit			(T_DEV_IR * me, T_IR_Message const * m);
/* Action performed by the Parent state IR_State_Top
static State OnDataReceived	(T_DEV_IR * me, T_IR_Message const * m);	// delegated to parent <IR_State_Top>
static State OnDataSent		(T_DEV_IR * me, T_IR_Message const * m);	// delegated to parent <IR_State_Top>
static State OnTimeout		(T_DEV_IR * me, T_IR_Message const * m);	// delegated to parent <IR_State_Top>
static State OnCancel 		(T_DEV_IR * me, T_IR_Message const * m);	// delegated to parent <IR_State_Top>
*/

/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/*************************************************************************//**
 * @fun     IR_State_WaitingData
 * @brief   Waits for an Ack PDU during some time.
 *
 * @param   pThis      	this object as void
 * @param	e			Events received by this state
 *
 * @return  n/a
 ****************************************************************************/
State IR_State_WaitingData(Object* pThis, Event const * e){
	T_DEV_IR * me = (T_DEV_IR*)pThis;
	switch(e->sig){
		case ENTRY_SIG:
			return(OnEntry(me,(T_IR_Message const *)e));

		case EXIT_SIG:
			return(OnExit(me,(T_IR_Message const *)e));
						
		case TIMEOUT_SIG:	// delegate to parent implementation
			SUPER(&IR_State_Top);

		case EV_DATARECV:	// delegate to parent implementation
			SUPER(&IR_State_Top);

		case EV_DATASENT:	// delegate to parent implementation
			SUPER(&IR_State_Top);

		case EV_CANCELLED:	// delegate to parent implementation
			SUPER(&IR_State_Top);
	}
    SUPER(&IR_State_Top);
}


/****************************************************************************************************//**
 *                        EVENT HANDLERS 
 ********************************************************************************************************/

/*************************************************************************//**
 * @fun     OnEntry
 * @brief   Handles ENTRY_SIG events
 *
 * @param   This      	this object
 * @param	m			T_IR_Message received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnEntry(T_DEV_IR * me, T_IR_Message const * m){
	(void)m;							// not used
	me->waitfor = WAITFOR_EVT;	   		// wait in os_evt_wait_or for this state
	me->timeout = IR_RECV_EVT_TIMEOUT;	// at least during that time for an answer
										// clear all processable events
	os_evt_clr(EVF_IR_ALLFLASG, me->runnable.tid);
	InitReceptionProcess(me);			// initialize buffer and decoder status
	HANDLED();
}

/*************************************************************************//**
 * @fun     OnExit
 * @brief   Handles EXIT_SIG events
 *
 * @param   This      	this object
 * @param	m			T_IR_Message received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnExit(T_DEV_IR * me, T_IR_Message const * m){
	(void)me; (void)m;	// not used
	HANDLED();
}

/*************************************************************************//**
 * @fun     OnTimeout
 * @brief   Handles TIEMOUT_SIG event. Uses the message in process in order 
 *			know which action should be carry out.
 *
 * @param   This      	this object
 * @param	m			T_IR_Message in process <me->msg>
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
/* Action performed by the Parent state IR_State_Top
static State OnTimeout(T_DEV_IR * me, T_IR_Message const * m){
	(void)me; (void)me; //not used
	// Action performed by the Parent state IR_State_Top
	// ignore events
	IGNORED();
}
*/

/*************************************************************************//**
 * @fun     OnDataReceived
 * @brief   Handles EV_DATARECV events
 *
 * @param   This      	this object
 * @param	m			T_IR_Message received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
/* Action performed by the Parent state IR_State_Top
static State OnDataReceived(T_DEV_IR * me, T_IR_Message const * m){
	uint8_t data;
	E_IR_ERRORS errcode;

	// Read received byte and apply decodification
	data = IGetData();
	if(DecodeManchester(&me->manchester, data) == ERR_IR_PENDING){
		HANDLED();
	}

	// if decodification completed... process byte
	errcode = ProcessByteReceived(me, data);
    // if errors then initialize receiver operation
    if(errcode != ERR_IR_OK && errcode != ERR_IR_PENDING){
        InitReceptionProcess(me);
		HANDLED();
    }
    // if data valid then keep waiting to the end of stream
    else if(errcode == ERR_IR_PENDING){
		HANDLED();
    }
    // if end of stream valid then check the operation to process
    else{
		IDataReady(&me->rxBuff[2], me->rxBuff[1]);	// send only pdu data and its size
        TRAN(&IR_State_Top);                
    }
}
*/

/*************************************************************************//**
 * @fun     OnDataSent
 * @brief   Handles EV_DATASENT events. It must wait till Tx buff finish sending
 *			the last byte, waiting in a blocking mode, and afterwards it must
 *			enable the driver reception. This procedure is to avoid ECHO detection
 *			during the last byte transmission.
 *
 * @param   This      	this object
 * @param	m			T_IR_Message received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
/* Action performed by the Parent state IR_State_Top
static State OnDataSent(T_DEV_IR * me, T_IR_Message const * m){
	uint8_t data;
	E_IR_ERRORS errcode;

	// blocks during >1 milliseconds to complete transmission
	os_dly_wait(1);
	// enable driver reception
	IDriverEnableRx();
	HANDLED();
}
*/

/*************************************************************************//**
 * @fun     OnCancel
 * @brief   Handles EV_READ events
 *
 * @param   This      	this object
 * @param	m			T_IR_Message received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
/* Action performed by the Parent state IR_State_Top
static State OnCancel(T_DEV_IR * me, T_IR_Message const * m){
	(void)me; (void)m;	// not used
	// Action performed by the Parent state IR_State_Top
	// ignore events
	IGNORED();
}
*/


