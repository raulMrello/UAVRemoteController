/*************************************************************************//**
 * @file    dev_Buzzer_hsm.h
 * @mcu		STM32F1x
 * @brief   Buzzer Device State Machine protected header
 * @date    15.06.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef __dev_BUZZER_HSM_H__
#define __dev_BUZZER_HSM_H__

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <stdint.h>
#include "StateFramework.h"
#include "IRunnable.h"
#include "dev_Buzzer.h"

/****************************************************************************************************//**
 *                        	REQUIRED INTERFACES
 ********************************************************************************************************/
#define IDriverInit()		dev_Buzzer_Requires_IDriverInit()
#define IStart()			dev_Buzzer_Requires_IStart()
#define IStop()				dev_Buzzer_Requires_IStop()


/****************************************************************************************************//**
 *                        PROTECTED STATE DEFINITIONS
 ********************************************************************************************************/

State BUZ_State_Initial		(Object* pThis, Event const * e);
State BUZ_State_Top			(Object* pThis, Event const * e);

/****************************************************************************************************//**
 *                        PROTECTED TYPES
 ********************************************************************************************************/

/*************************************************************************//**
 * @def		BUZ_SHORT_TIME
 * @def		BUZ_LONG_TIME
 * @def		BUZ_CONFIRM_TIME
 * @scope	protected
 * @brief   Defines two types of delays for the beep operation. Short beeps 
 *			will sound during 150ms and long ones, will do it during 600ms.
 ****************************************************************************/
#define BUZ_SHORT_TIME		GetOSTicks(150)
#define BUZ_LONG_TIME		GetOSTicks(600)
#define BUZ_CONFIRM_TIME	GetOSTicks(100)
#define BUZ_SLEEP_TIME		GetOSTicks(50)

/*************************************************************************//**
 * @def		BUZ_N_CONFIRM_BEEPS
 * @scope	protected
 * @brief   Defines the number of short beeps for confirmation sounds
 ****************************************************************************/
#define BUZ_N_CONFIRM_BEEPS		3

/*************************************************************************//**
 * @enum	E_BUZ_EVENTS
 * @scope	protected
 * @brief   Event signals accepted by the state machine of this device
 ****************************************************************************/
typedef enum {
	EV_BEEPSHORT = USER_SIG,//!< event due to EVF_BEEPSHORT through BeepShort interface
	EV_BEEPLONG,			//!< event due to EVF_BEEPLONG through BeepLong interface
	EV_BEEPCONFIRM			//!< event due to EVF_BEEPCONFIRM through BeepConfirm interface
}E_BUZ_EVENTS;

/*************************************************************************//**
 * @enum	E_BUZ_FLAGS
 * @scope	protected
 * @brief   EventFlags masks accepted by this device during os_evt_wait_or
 ****************************************************************************/
typedef enum {
	EVF_BEEPSHORT   = 0x0001,	//!< event through BeepShort interface
	EVF_BEEPLONG    = 0x0002,	//!< event through BeepLong interface
	EVF_BEEPCONFIRM = 0x0004,	//!< event through BeepConfirm interface
	EVF_BUZ_ALLFLAGS = (EVF_BEEPSHORT + EVF_BEEPLONG + EVF_BEEPCONFIRM)
}E_BUZ_FLAGS;


/*************************************************************************//**
 * @struct	T_BUZ_Message
 * @scope	protected
 * @brief   This structure is the message type accepted by the object. As this
 *			component only accepts event flags, it only can process simple 
 *			Events.
 ****************************************************************************/
typedef Event T_BUZ_Message;


/*************************************************************************//**
 * @struct	E_BUZ_ERRORS
 * @scope	protected
 * @brief   Error codes for Buzzer operation
 ****************************************************************************/
typedef enum {
	ERR_BUZ_OK = 0,
	ERR_BUZ_UNKNOWN
} E_BUZ_ERRORS;

/*************************************************************************//**
 * @struct	T_DEV_Buzzer
 * @scope	protected
 * @brief   This structure is the internal device structure with all the static
 *			variable required for functionality.
 ****************************************************************************/
typedef struct{
	IRunnable 		runnable;	//!< Interface to execute a thread with a mailhox
	uint32_t		uid;		//!< Component UID
	uint32_t		timeout;	//!< timeout for or os_evt_wait_or
	E_WAITFOR 		waitfor;	//!< waiting method: mbx or evt
	uint8_t			cnfrmCnt;	//!< confirmation beep count for 3-short beeps
}T_DEV_Buzzer;


/****************************************************************************************************//**
 *                        PROTECTED FUNTIONS
 ********************************************************************************************************/

#endif

