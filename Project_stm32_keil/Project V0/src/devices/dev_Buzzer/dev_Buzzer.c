/*************************************************************************//**
 * @file    dev_Buzzer.c
 * @mcu		STM32F1x
 * @brief   Buzzer device driver
 * @date    14.06.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include "dev_Buzzer_hsm.h"		// includes dev_Buzzer.h


/****************************************************************************************************//**
 *                        	PRIVATE MEMBERS
 ********************************************************************************************************/
static T_DEV_Buzzer	This;
static void BUZ_Task(Object *pThis);
#define BUZ_STACK_SIZE			300	// tamaño en bytes
static volatile unsigned long long 	BUZ_Stack[(BUZ_STACK_SIZE/8)];

/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/****************************************************************************************************//**
 * @fun		dev_Buzzer_init
 * @brief	Provided interface to setup the device
 * @return	errcodes relative to RTOS
 **********************************************************************************/
uint8_t dev_Buzzer_Init(uint8_t priority){	
	ErrorCode err = ERR_OK;

	IDriverInit();
	IStop();

	// Initialize IRunnable interface. If succesful, an OS_TID is assigned.
	err = IRunnable_Init   (&This.runnable, 
							&This, 
							&This.runnable.tid, 
							0,					// no mailbox is required 
							0, 					// no mailbox is required
							(void*)&BUZ_Stack, 
							(U16)BUZ_STACK_SIZE,
							priority,
							&BUZ_Task,
							&BUZ_State_Initial);
	return((uint8_t)err);
}


/****************************************************************************************************//**
 * @fun		dev_Buzzer_BeepShort
 * @brief	Provided interface to start a short beep sound
 * @return	succesfull
 **********************************************************************************/
uint8_t dev_Buzzer_BeepShort(void){
	// if not waiting events, then discard it
	if(This.waitfor != WAITFOR_EVT){
		return((uint8_t)ERR_OK);
	}
	os_evt_set(EVF_BEEPSHORT, This.runnable.tid);
	return((uint8_t)ERR_OK);
}


/****************************************************************************************************//**
 * @fun		dev_Buzzer_BeepLong
 * @brief	Provided interface to start a long beep sound
 * @return	succesfull
 **********************************************************************************/
uint8_t dev_Buzzer_BeepLong(void){
	// if not waiting events, then discard it
	if(This.waitfor != WAITFOR_EVT){
		return((uint8_t)ERR_OK);
	}
	os_evt_set(EVF_BEEPLONG, This.runnable.tid);
	return((uint8_t)ERR_OK);
}


/****************************************************************************************************//**
 * @fun		dev_Buzzer_BeepConfirm
 * @brief	Provided interface to start a 3-short confirmation beep sounds
 * @return	succesfull
 **********************************************************************************/
uint8_t dev_Buzzer_BeepConfirm(void){
	// if not waiting events, then discard it
	if(This.waitfor != WAITFOR_EVT){
		return((uint8_t)ERR_OK);
	}
	os_evt_set(EVF_BEEPCONFIRM, This.runnable.tid);
	return((uint8_t)ERR_OK);
}

/****************************************************************************************************//**
 * @fun		BUZ_Task
 * @brief	Task for this device. Launches the initial state of the HSM and afterwards waits for 
 *			incoming eventflags (os_evt_wait_or).
 *			If timeout is overrun then a timeout event is dispatched to the
 *			HSM. In other case, the proper event is dispatched.
 * @return	
 **********************************************************************************/
static void BUZ_Task(Object *pThis){
	T_DEV_Buzzer * me = (T_DEV_Buzzer*)pThis;
	//T_BUZ_Message * msg;	// not used
    // Starts the state machine
    StateMachine_Init(me);       

	for(;;){
		switch(me->waitfor){
			// wait for mbx message then force to wait for event flags
			case WAITFOR_MBX:{
				me->waitfor = WAITFOR_EVT;		
				break;
			}
			// wait for event flag
			case WAITFOR_EVT:{
				OS_RESULT errno;
				uint16_t recvflag;
				const Event beepShortEvt 	= {EV_BEEPSHORT};
				const Event beepLongEvt  	= {EV_BEEPLONG};
				const Event beepConfirmEvt  = {EV_BEEPCONFIRM};

				errno = os_evt_wait_or(EVF_BUZ_ALLFLAGS, me->timeout);
				// if timeout, launch timeout event
				if(errno == OS_R_TMO){
					StateMachine_Dispatch(me, (Event const *)&timeoutEvt);
				}
				// if event flag, check and dispatch
				else{
					recvflag = os_evt_get();			// get event received
					if((recvflag & EVF_BEEPSHORT) != 0){	
						os_evt_clr(EVF_BEEPSHORT, me->runnable.tid);
						StateMachine_Dispatch(me, (Event const *)&beepShortEvt);
					}
					if((recvflag & EVF_BEEPLONG) != 0){	
						os_evt_clr(EVF_BEEPLONG, me->runnable.tid);
						StateMachine_Dispatch(me, (Event const *)&beepLongEvt);
					}
					if((recvflag & EVF_BEEPCONFIRM) != 0){	
						os_evt_clr(EVF_BEEPCONFIRM, me->runnable.tid);
						StateMachine_Dispatch(me, (Event const *)&beepConfirmEvt);
					}
					
				}
				break;
			}
		}
	}
}



