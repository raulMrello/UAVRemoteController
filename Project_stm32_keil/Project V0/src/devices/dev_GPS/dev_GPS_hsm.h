/*************************************************************************//**
 * @file    dev_GPS_hsm.h
 * @mcu		STM32F1x
 * @brief   GPS Device State Machine protected header
 * @date    15.06.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef __dev_GPS_HSM_H__
#define __dev_GPS_HSM_H__

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "StateFramework.h"
#include "dev_GPS.h"
#include "Lib_GPS.h"

/****************************************************************************************************//**
 *                        	REQUIRED INTERFACES
 ********************************************************************************************************/
#define IDriverInit			dev_GPS_Requires_IDriverInit
#define IDriverEnableRx		dev_GPS_Requires_IDriverEnableRx
#define IDriverDisableRx	dev_GPS_Requires_IDriverDisableRx
#define IDriverEnable		dev_GPS_Requires_IDriverEnable
#define IDriverTurnOn		dev_GPS_Requires_IDriverTurnOn
#define IDriverTurnOff		dev_GPS_Requires_IDriverTurnOff
#define ISend(ptr,size)		dev_GPS_Requires_ISend(ptr,size)					
#define IGetData			dev_GPS_Requires_IGetData					
#define IDataReady			dev_GPS_Requires_IDataReady		


/****************************************************************************************************//**
 *                        PROTECTED STATE DEFINITIONS
 ********************************************************************************************************/

State GPS_State_Initial		(Object* pThis, Event const * e);
State GPS_State_Top			(Object* pThis, Event const * e);
State GPS_State_Disabling	(Object* pThis, Event const * e);
State GPS_State_Running		(Object* pThis, Event const * e);

/****************************************************************************************************//**
 *                        PROTECTED TYPES
 ********************************************************************************************************/

/*************************************************************************//**
 * @def		GPS_ASSERTED_TIMEOUT
 * @scope	protected
 * @brief   Returns the number of OS_TICK for a delay of 100 milliseconds after
 *			asserting the powerup pin of the hw-driver
 ****************************************************************************/
#define GPS_ASSERTED_TIMEOUT		GetOSTicks(200)

/*************************************************************************//**
 * @def		GPS_DEASSERTED_TIMEOUT
 * @scope	protected
 * @brief   Returns the number of OS_TICK for a delay of 1000 milliseconds after
 *			asserting the powerup pin of the hw-driver
 ****************************************************************************/
#define GPS_DEASSERTED_TIMEOUT		GetOSTicks(1500)

/*************************************************************************//**
 * @enum	E_GPS_STATUS
 * @scope	protected
 * @brief   Event signals accepted by the state machine of this device
 ****************************************************************************/
typedef enum {
	STAT_UNKNOWN 		= 0x0,		//!< status after powerup
	STAT_INITIATED		= 0x1,		//!< status after first entry into State_Top
	STAT_INITSEQUENCE   = 0x2,		//!< status during timming for starting powseq
	STAT_POWSEQUENCE	= 0x4,		//!< status during powering On/Off sequence
	STAT_WAITDATA		= 0x8,		//!< status after powering sequence		
	STAT_WAITFIRSTRECV  = 0x10		//!< status after receiving the first NMEA sentence
}E_GPS_STATUS;


/*************************************************************************//**
 * @enum	E_GPS_EVENTS
 * @scope	protected
 * @brief   Event signals accepted by the state machine of this device
 ****************************************************************************/
typedef enum {
	EV_NEWDATA = USER_SIG,		//!< event through NewData interface
	EV_POWERON,					//!< event through PowerOn interface
	EV_POWEROFF					//!< event through PowerOff interface
}E_GPS_EVENTS;

/*************************************************************************//**
 * @enum	E_GPS_FLAGS
 * @scope	protected
 * @brief   EventFlags masks accepted by this device during os_evt_wait_or
 ****************************************************************************/
typedef enum {
	EVF_NEWDATA   = 0x0001,	//!< event through NewData interface
	EVF_POWERON	  = 0x0002,	//!< event through PowerOn interface
	EVF_POWEROFF  = 0x0004,	//!< event through PowerOff interface
	EVF_GPS_ALLFLAGS = (EVF_NEWDATA | EVF_POWERON | EVF_POWEROFF)
}E_GPS_FLAGS;


/*************************************************************************//**
 * @struct	T_DEV_Calendar
 * @scope	protected
 * @brief   This structure is the internal device structure with all the static
 *			variable required for functionality.
 ****************************************************************************/
typedef struct{
	IRunnable 		runnable;	//!< Interface to execute a thread with a mailhox
	uint32_t		uid;		//!< Component UID
	uint32_t		timeout;	//!< timeout for os_evt_wait_or
	E_GPS_STATUS	ctrlFlags;	//!< control flags
	STRGps			gps;		//!< gps information data
}T_DEV_Gps;


/****************************************************************************************************//**
 *                        PROTECTED FUNTIONS
 ********************************************************************************************************/

#endif

