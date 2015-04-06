/*************************************************************************//**
 * @file    dev_IR_State_Top.h
 * @mcu		STM32F1x
 * @brief   IR Device State Machine Top parent state implementation
 * @date    15.06.2010
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
static State OnSend  		(T_DEV_IR * me, T_IR_Message const * m);
static State OnRead 		(T_DEV_IR * me, T_IR_Message const * m);
static State OnTimeout		(T_DEV_IR * me, T_IR_Message const * m);
static State OnDataReceived	(T_DEV_IR * me, T_IR_Message const * m);
static State OnDataSent		(T_DEV_IR * me, T_IR_Message const * m);
static State OnCancel 		(T_DEV_IR * me, T_IR_Message const * m);


/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/*************************************************************************//**
 * @fun     IR_State_Initial
 * @brief   Initialize internal properties
 *
 * @param   pThis      	this object as void
 * @param	e			Events received by this state
 *
 * @return  n/a
 ****************************************************************************/
State IR_State_Initial(Object* pThis, Event const * e){
	T_DEV_IR * me = (T_DEV_IR*)pThis;
	(void)e;
	me->uid = me->runnable.tid;
	me->crc16 = 0;
	TRAN(&IR_State_Top);
}


/*************************************************************************//**
 * @fun     IR_State_Top
 * @brief   Parent state
 *
 * @param   pThis      	this object as void
 * @param	e			Events received by this state
 *
 * @return  n/a
 ****************************************************************************/
State IR_State_Top(Object* pThis, Event const * e){
	T_DEV_IR * me = (T_DEV_IR*)pThis;
	switch(e->sig){
		case ENTRY_SIG:
			return(OnEntry(me,(T_IR_Message const *)e));

		case EXIT_SIG:
			return(OnExit(me,(T_IR_Message const *)e));

		case EV_SEND:
			return(OnSend(me,(T_IR_Message const *)e));

		case EV_READ:
			return(OnRead(me,(T_IR_Message const *)e));
		
		/*** Next events are processed on behalf of WaitingData and WaitingAck ***/	
					
		case TIMEOUT_SIG:	
			return(OnTimeout(me,(T_IR_Message const *)e));

		case EV_DATARECV:	
			return(OnDataReceived(me,(T_IR_Message const *)e));

		case EV_DATASENT:	
			return(OnDataSent(me,(T_IR_Message const *)e));

		case EV_CANCELLED:
			return(OnCancel(me,(T_IR_Message const *)e));
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
 * @param	m			T_IR_Message received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnEntry(T_DEV_IR * me, T_IR_Message const * m){
	(void)m;	// not used
	me->waitfor = WAITFOR_MBX;		// waits on os_mbx_wait
	me->timeout = IR_MBX_TIMEOUT; 	// a safety time till a msg is received
	me->ntx = 0;					// tx buffer initialization
	IDriverDisableRx();				// disable incomming unexpected data
	IDriverInit(0);					// initialize ONLY tx/rx buffers
	InitReceptionProcess(me);		// initialize decoder 
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
 * @fun     OnSend
 * @brief   Handles EV_SEND events
 *
 * @param   This      	this object
 * @param	m			T_IR_Message received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnSend(T_DEV_IR * me, T_IR_Message const * m){
	// get a reference to the message in process
	me->msg = (T_IR_Message *)m;

	// enable power to the driver
	IDriverTurnOn();

	// Code PDU to Manchester stream getting the message size
	me->ntx = CodeToManchester(me->txBuff, (uint8_t *)&m->msgContent.SendMsg.pdu[0], m->msgContent.SendMsg.len);

	// send data through the required interface
	ISend(me->txBuff, me->ntx);
	
	// switch to WaitingAck state
	TRAN(&IR_State_WaitingAck);
}

/*************************************************************************//**
 * @fun     OnRead
 * @brief   Handles EV_READ events
 *
 * @param   This      	this object
 * @param	m			T_IR_Message received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnRead(T_DEV_IR * me, T_IR_Message const * m){
	// get a reference to the message in process
	me->msg = (T_IR_Message *)m;

	// enable power to the driver
	IDriverTurnOn();

	// Code PDU to Manchester stream getting the message size
	me->ntx = CodeToManchester(me->txBuff, (uint8_t *)&m->msgContent.ReadMsg.pdu[0], m->msgContent.ReadMsg.len);

	// send data through the required interface
	ISend(me->txBuff, me->ntx);

	// switch to WaitingData state
	TRAN(&IR_State_WaitingData);
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
 * @param	m			T_IR_Message in process <me->msg>
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnTimeout(T_DEV_IR * me, T_IR_Message const * m){
	(void)m; // not used	
	// if required...resend last databuffer sent through the required interface
	if(me->ntx > 0){
		IDriverDisableRx();				// disable incomming unexpected data
		IDriverInit(0);					// initialize ONLY rx buffer
		InitReceptionProcess(me);		// initialize decoder to discard bad data
		ISend(me->txBuff, me->ntx);		// resend request
	}
	HANDLED();
}

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
static State OnDataSent(T_DEV_IR * me, T_IR_Message const * m){
	// start recv stream timing if enable
	IR_StartRecvTimeout();

	// blocking delay to avoid ECHO error sync
	os_dly_wait(2);

	// enables driver reception
	IDriverEnableRx();
	HANDLED();
}

/*************************************************************************//**
 * @fun     OnDataReceived
 * @brief   Handles EV_DATARECV events
 *
 * @param   This      	this object
 * @param	m			T_IR_Message received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnDataReceived(T_DEV_IR * me, T_IR_Message const * m){
	uint8_t data;
	int		retData;
	uint8_t	outType = 0;	//if 0 then HANDLED. if 1 then TRAN(IR_State_Top)
	E_IR_ERRORS errcode;

	// Read received byte and apply decodification while data in buffer 
	do{
		retData = IGetData();
		if(retData == -1){
			break;
		}
		data = (uint8_t)retData;
		// if decodification completed... process byte
		errcode = ProcessByteReceived(me, data);
	    // if errors then initialize receiver operation
	    if(errcode != ERR_IR_OK && errcode != ERR_IR_PENDING){
	        IDriverDisableRx();				// disable incomming unexpected data
			IDriverInit(0);					// initialize ONLY tx/rx buffers
			InitReceptionProcess(me);
			outType = 0;
	    }
	    // if data valid then keep waiting to the end of stream
	    else if(errcode == ERR_IR_PENDING){
			outType = 0;
	    }
	    // if end of stream valid then check the operation to process
	    else{
			IR_ResetRecvTimeout();
	        IDriverDisableRx();				// disable incomming unexpected data
			IDataReady(&me->rxBuff[2], me->rxBuff[1]);	// send only pdu data and its size
			outType = 1;
			break;
	    }
	}while(retData != -1);

	// then exits as it must according with outType
	if(outType == 1){
        TRAN(&IR_State_Top);                
	}
	HANDLED();
		
}

/*************************************************************************//**
 * @fun     OnCancel
 * @brief   Handles EV_CANCELLER event. Once a Cancel interface is invoked a
 *			transition to IR_State_Top must be performed. This implies to
 *			restore the initial conditions and no message will be in process.
 *
 * @param   This      	this object
 * @param	m			T_IR_Message received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnCancel(T_DEV_IR * me, T_IR_Message const * m){
	// disable power to the driver
	#if HARDWARE_RELEASE != HW_REV2
	IDriverTurnOff();
	#else
	IR_ResetRecvTimeout();
	IDriverDisableRx();				// disable incomming unexpected data
	IDriverInit(0);					// initialize ONLY tx/rx buffers
	InitReceptionProcess(me);
	#endif
	TRAN(&IR_State_Top);
}



