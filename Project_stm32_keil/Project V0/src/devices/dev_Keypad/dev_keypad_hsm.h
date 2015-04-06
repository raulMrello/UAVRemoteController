/*************************************************************************//**
 * @file    dev_KeyPad_hsm.h
 * @mcu		STM32F1x
 * @brief   KeyPad Device State Machine protected header
 * @date    08.07.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef __dev_KeyPad_HSM_H__
#define __dev_KeyPad_HSM_H__

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <stdint.h>
#include "StateFramework.h"
#include "IRunnable.h"
#include "dev_KeyPad.h"

/****************************************************************************************************//**
 *                        	REQUIRED INTERFACES
 ********************************************************************************************************/
#define IDriverInit()			dev_KeyPad_Requires_IDriverInit()
#define IGetKey()				dev_KeyPad_Requires_IGetKey()					
#define ISendKey(k)				dev_KeyPad_Requires_ISendKey(k)		


/****************************************************************************************************//**
 *                        PROTECTED STATE DEFINITIONS
 ********************************************************************************************************/

State KeyPad_State_Initial	(Object* pThis, Event const * e);
State KeyPad_State_Top		(Object* pThis, Event const * e);
State KeyPad_State_Pressed	(Object* pThis, Event const * e);

/****************************************************************************************************//**
 *                        PROTECTED TYPES
 ********************************************************************************************************/

/*************************************************************************//**
 * @def		KeyPad_SCAN_TIMEOUT
 * @scope	protected
 * @brief   Returns the number of OS_TICK for a delay of 500 milliseconds after
 *			detecting a keyboard press
 ****************************************************************************/
#define KeyPad_SCAN_TIMEOUT		GetOSTicks(500)

/*************************************************************************//**
 * @enum	E_KeyPad_EVENTS
 * @scope	protected
 * @brief   Event signals accepted by the state machine of this device
 ****************************************************************************/
typedef enum {
	EV_NEWKEY = USER_SIG,		//!< event through NewKey interface
	EV_FORGETKEY				//!< event through ForgetKey interface
}E_KeyPad_EVENTS;

/*************************************************************************//**
 * @enum	E_KeyPad_FLAGS
 * @scope	protected
 * @brief   EventFlags masks accepted by this device during os_evt_wait_or
 ****************************************************************************/
typedef enum {
	EVF_NEWKEY    = 0x0001,		//!< event through NewKey interface
	EVF_FORGETKEY = 0x0002,		//!< event through ForgetKey interface
	EVF_KEY_ALLFLAGS = (EVF_NEWKEY | EVF_FORGETKEY)
}E_KeyPad_FLAGS;


/*************************************************************************//**
 * @struct	T_DEV_KeyPad
 * @scope	protected
 * @brief   This structure is the internal device structure with all the static
 *			variable required for functionality.
 ****************************************************************************/
typedef struct{
	IRunnable 		runnable;	//!< Interface to execute a thread with a mailhox
	uint32_t		uid;		//!< Component UID
	uint32_t		timeout;	//!< timeout for os_evt_wait_or
	uint32_t		ctrlFlags;	//!< flags de control
	uint32_t		keycode;	//!< last keycode detected
}T_DEV_KeyPad;


/****************************************************************************************************//**
 *                        PROTECTED FUNTIONS
 ********************************************************************************************************/

#endif

