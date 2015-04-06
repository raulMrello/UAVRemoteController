/*************************************************************************//**
 * @file    dev_KeyPad.h
 * @mcu		STM32F1x
 * @brief   Keypad Device Manager
 * @date    11.06.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <RTL.h>
#include "dev_keypad_hsm.h"

 
/****************************************************************************************************//**
 *                        PRIVATE MEMBERS
 ********************************************************************************************************/

static T_DEV_KeyPad This;
static void KeyPad_Task (Object *pThis);

#define KEYPAD_STACK_SIZE			300	// tamaño en bytes
static volatile unsigned long long 	KeyPad_Stack[(KEYPAD_STACK_SIZE/8)];

#define DRV_KEYPAD_INT_FLAG			0x0008
#define KEY_MODE_NEW_KEYSTROKE	    0
#define KEY_MODE_NEXT_KEYVALUE		1
#define KEY_MODE_BLOQNUM			2
#define KEY_MODE_BLOQALFA			3
#define DELAY_RELEASE_KEY			DELAY_20MS
#define DELAY_LONG_KEYSTROKE		DELAY_1SEG

uint8_t key_Values[][4]= {"   ","@@@","ABC","DEF","GHI","JHL","MNO","PQRS","TUV","WXYZ"};


/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/****************************************************************************************************//**
 * @fun		dev_KeyPad_Init
 * @brief	Provided interface used by the initial system loading to carry out KeyPad Device initialization
 * @return	
 **********************************************************************************/
uint8_t dev_KeyPad_Init(uint8_t priority){
	ErrorCode err = ERR_OK;

	IDriverInit();
	This.ctrlFlags = 0;
	// Initialize IRunnable interface. If succesful, an OS_TID is assigned.
	err = IRunnable_Init   (&This.runnable, 
							&This, 
							&This.runnable.tid, 
							0, 
							0, 
							(void*)&KeyPad_Stack, 
							(U16)KEYPAD_STACK_SIZE,
							priority,
							&KeyPad_Task,
							&KeyPad_State_Initial);
	return((uint8_t)err);
}

/****************************************************************************************************//**
 * @fun		dev_KeyPad_NewKey
 * @brief	Provided interface used by the GPIO driver to notify key events
 * @return	
 **********************************************************************************/
void dev_KeyPad_NewKey(void){
	isr_evt_set (EVF_NEWKEY, This.runnable.tid);
}

/****************************************************************************************************//**
 * @fun		dev_KeyPad_ForgetKey
 * @brief	Provided interface used by the GUI to disable all the pressing keyboard process
 * @return	
 **********************************************************************************/
void dev_KeyPad_ForgetKey(void){
	isr_evt_set (EVF_FORGETKEY, This.runnable.tid);
}


/****************************************************************************************************//**
 * @fun		KeyPad_Task
 * @brief	Task for this device
 * @return	
 **********************************************************************************/
static void KeyPad_Task(Object *pThis){
	T_DEV_KeyPad * me = (T_DEV_KeyPad*)pThis;
	uint16_t recvflag;
    // Starts the state machine
    StateMachine_Init(me);       

	for(;;){
		OS_RESULT errno;
		const Event newKeyEvt = {EV_NEWKEY};
		const Event forgetKeyEvt = {EV_FORGETKEY};
		errno = os_evt_wait_or(EVF_KEY_ALLFLAGS, me->timeout);
		// if timeout, launch timeout event
		if(errno == OS_R_TMO){
			StateMachine_Dispatch(me, (Event const *)&timeoutEvt);
		}
		// if EVF_NEWKEY flag, check and dispatch
		else{
			recvflag = os_evt_get();			// get event received
			if((recvflag & EVF_FORGETKEY) != 0){	// process forget events with high priority and avoid the rest
				os_evt_clr(EVF_NEWKEY | EVF_FORGETKEY, me->runnable.tid);
				StateMachine_Dispatch(me, (Event const *)&forgetKeyEvt);
			}
			else if((recvflag & EVF_NEWKEY) != 0){	
				os_evt_clr(EVF_NEWKEY, me->runnable.tid);
				StateMachine_Dispatch(me, (Event const *)&newKeyEvt);
			}
		}
	}
}





