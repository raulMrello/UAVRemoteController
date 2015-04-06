/*************************************************************************//**
 * @file    IRunnable.c
 *
* @brief   Interface to implement a software component with its own thread
 *			which accepts income messages and them are dispatched by an
 *			internal state machine.
 *
 * @date    28.05.2010
 *
 * @author  Raúl M.
 ****************************************************************************/
#include "IRunnable.h"
#include <stdlib.h>			// heap management definitions

/*************************************************************************//**
 * @var  	maxHeapAllocated
 * @brief   Reports the maximum heap allocation size during runtime. It can be
 *			checked during debug sessions in order to define a proper HEAP size
 *			for release versions. 
 ****************************************************************************/
static volatile uint32_t maxHeapAllocated = 0;

/****************************************************************************************************//**
 *                        PROVIDED INTERFACES MACRO SIMPLIFICATION 
 ********************************************************************************************************/
#define Inbox	This->inbox

/****************************************************************************************************//**
 *                        PUBLIC STATIC INTERFACE
 ********************************************************************************************************/
/*************************************************************************//**
 * @fun  	IRunnable_Init
 * @brief   Initialize internal properties 
 *
 * @param	This			own-reference
 * @param	objHandler		object who handles the interface instance
 * @param	tid				task Id assigned
 * @param	msgSize			Size in bytes for each message associated to a memory pool
 * @param	numBlocks		Number of blocks for each memory pool in which divide them
 * @param	stk				Pointer to a user stack memory
 * @param 	stkSize			size in bytes of the user stack
 * @param	priority		task priority to be assigned
 * @param	Run				Pointer to the routine which handles the interface thread
 * @param	HSMInit			Pointer to the routine which enters the HSM_State_Initial
 *
 * @retval	ERR_MEM			Failing allocating memory from the heap
 * @retval  ERR_OK			Initialization succesfully executed
 ****************************************************************************/
ErrorCode IRunnable_Init	   (IRunnable*	This, 			
								Object*		objHandler,
								OS_TID*		tid, 				
								U32 		msgSize, 
						 		U32 		numBlocks, 
						 		void*		stk,
								U16			stkSize,
								U8			priority,
								TaskHandler	Run,
								StateHandler HSMInit){

	int err;
	// initialize own members
	This->objHandler = objHandler;
	Inbox.msgSize = msgSize;
	Inbox.numBlocks = numBlocks;

	// interface can be implemented without mailbox
	if(msgSize == 0 || numBlocks == 0){
		Inbox.mailBox = 0;
		Inbox.memPool = 0;
	}
	else{
		// use the HEAP to allocate RAM dynamically for the MsgBox
		// RTX use os_mbx_declare(name,numBlocks) to allocate RAM statically
		Inbox.mailBox = malloc(sizeof(U32)*(4+numBlocks));
		if(Inbox.mailBox == 0)	return ERR_MEM;
		maxHeapAllocated += (sizeof(U32)*(4+numBlocks));
	
		// initialize the mailbox
		os_mbx_init(Inbox.mailBox, sizeof(U32)*(4+numBlocks));
	
		// use the HEAP to allocate RAM dynamically for the MemoryPool
		// RTX use _declare_box(name,msgSize,numBlocks) to allocate RAM statically
		Inbox.memPool = malloc(sizeof(U32)*(((msgSize+3)/4)*(numBlocks) + 3));
		if(Inbox.memPool == 0)	return ERR_MEM;
		maxHeapAllocated += (sizeof(U32)*(((msgSize+3)/4)*(numBlocks) + 3));
	
		// initialize the memory pool to accept messages of msgSize bytes
		err = _init_box(Inbox.memPool, sizeof(U32)*(((msgSize+3)/4)*(numBlocks) + 3), msgSize);
		if(err != 0)	return ERR_RTOS;
	}

	// check user stack arguments to create task
	if(stk != (void*)0 && stkSize != 0){
		This->priority = priority;
		This->tid = os_tsk_create_user_ex ( Run, priority, stk, stkSize, objHandler);
		if(This->tid == 0)	return ERR_RTOS;
	}
	else{
		This->priority = priority;
		This->tid = os_tsk_create_ex ( Run, priority, objHandler);
		if(This->tid == 0)	return ERR_RTOS;
	}
	
	// Creates the state machine
	StateMachine_Ctor(This->objHandler, HSMInit);

	return ERR_OK;
}



