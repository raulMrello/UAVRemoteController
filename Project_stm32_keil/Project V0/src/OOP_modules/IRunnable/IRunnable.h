/*************************************************************************//**
 * @file    IRunnable.h
 *
 * @brief   Interface to implement a software component with its own thread
 *			which accepts income messages and them are dispatched by an
 *			internal state machine.
 *
 * @date    28.05.2010
 *
 * @author  Raúl M.
 ****************************************************************************/
#ifndef _IRUNNABLE_H
#define _IRUNNABLE_H
/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <RTL.h>				// OS and data types definitions
#include "StateFramework.h"		// state machine definitions

/****************************************************************************************************//**
 *                        PUBLIC TYPEDEFS 
 ********************************************************************************************************/


/*************************************************************************//**
 * @fun		GetOSTicks
 * @brief   Returns the number of OS_TICKs according with  <val> milliseconds
 * @param	val		Number of milliseconds
 ****************************************************************************/
extern uint32_t GetOSTicks(uint32_t val);


/*************************************************************************//**
 * @enum	ErrorCode
 * @brief   Enumeration defining different error conditions
 ****************************************************************************/
typedef enum E_ErrorCodes{
	ERR_OK = 0,			//!< no errors detected
	ERR_MEM,			//!< error due to memory operations
	ERR_ARGS,			//!< error due to invalid arguments
	ERR_MBOX_TOUT,		//!< error due to mailbox timeout
	ERR_MBOX_FULL,		//!< error due to mailbox full condition
	ERR_RTOS,			//!< error due to RTOS operations
	ERR_USER_FREE		//!< error code base address for user error handling			
}ErrorCode;

/*************************************************************************//**
 * @enum	E_WAITFOR
 * @brief   Enumeration to set two types of waiting for threads: os_mbx_wait or
 *			os_evt_wait_or.
 ****************************************************************************/
typedef enum {
	WAITFOR_MBX = 0,		//!< task must wait for os_mbx_send
	WAITFOR_EVT				//!< task must wait for isr_evt_set
}E_WAITFOR;

/*************************************************************************//**
 * @def  	TaskHandler
 * @brief   Typedef to handle running task routines.
 ****************************************************************************/
typedef void (*TaskHandler)(Object *pThis);

/*************************************************************************//**
 * @struct  MBox
 * @brief   Message inbox to receive messages from other components.
 ****************************************************************************/
typedef struct T_MBox{
	uint32_t * mailBox;		//!< MsgBox pointer
	uint32_t * memPool;		//!< Memory pool pointer
	uint32_t   msgSize;		//!< Message size accepted by the inbox
	uint32_t   numBlocks;	//!< Number of blocks accepted by the memory pool
}MBox;

/****************************************************************************************************//**
 *                        PUBLIC DATA STRUCTURE
 *
 * @struct	IRunnable
 ********************************************************************************************************/

typedef struct I_Runnable{
	StateMachine 	super;			//!< Simple inheritance in C: Class I_Runnable extends StateMachine
	OS_TID	  	 	tid;			//!< task id assigned by the RTOS
	Object* 	 	objHandler;		//!< reference to the object who implements the interface
	MBox		 	inbox;			//!< Message inbox
	uint8_t	 		priority;		//!< task priority
								//!< pointer to the routine which executes the associated thread
	void (*Run)(Object *objHandler);			
}IRunnable;

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
ErrorCode IRunnable_Init	   (IRunnable*	 This, 			
								Object*		 objHandler,
								OS_TID*		 tid, 				
								uint32_t 	 msgSize, 
						 		uint32_t 	 numBlocks, 
						 		void*		 stk,
								uint16_t	 stkSize,
								uint8_t		 priority,
								TaskHandler	 Run,
								StateHandler HSMInit);



#endif

