/*************************************************************************//**
 * @file    dev_GUI_hsm.h
 * @mcu		STM32F1x
 * @brief   GUI Device State Machine protected header
 * @date    15.06.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef __dev_GUI_HSM_H__
#define __dev_GUI_HSM_H__

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <string.h>
#include <stdio.h>
#include "SYS_Config.h"
#include "StateFramework.h"
#include "IRunnable.h"
#include "dev_GUI.h"
#include "TextsAndLanguages.h"
#include "FileSystem.h"
#include "DialogBoxEx.h"
#include "FlexMenu.h"
#include "ComMenu.h"
#include "FileMenu.h"
#include "UserIcons.h"
#include "Lib_GPS.h"		// includes GPS data types


/****************************************************************************************************//**
 *                        	REQUIRED INTERFACES
 ********************************************************************************************************/
#define IBeepShort()							dev_GUI_Requires_IBeepShort()
#define IBeepLong()								dev_GUI_Requires_IBeepLong()
#define IBeepConfirm()							dev_GUI_Requires_IBeepConfirm()
#define IEnDisCalendarEvents(endis)				dev_GUI_Requires_IEnDisCalendarEvents(endis)
#define IGetCalendar(cal)						dev_GUI_Requires_IGetCalendar(cal)
#define ISetCalendar(cal)						dev_GUI_Requires_ISetCalendar(cal)
#define IGetCalendarCounter()					dev_GUI_Requires_IGetCalendarCounter()
#define ICaptureLocation()						dev_GUI_Requires_ICaptureLocation()
#define IReleaseLocation()						dev_GUI_Requires_IReleaseCapture()
#define ICaptureIsDown()						dev_GUI_Requires_ICaptureIsDown()
#define ISendRemote(pdu,len)					dev_GUI_Requires_ISendRemote(pdu,len)					
#define IReadRemote(pdu,len)					dev_GUI_Requires_IReadRemote(pdu,len)		
#define ICancelRemote()							dev_GUI_Requires_ICancelRemote()		
#define IPowerRemote()							dev_GUI_Requires_IPowerRemote()		
#define IUnpowerRemote()						dev_GUI_Requires_IUnpowerRemote()		
#define ICheckRemoteReady()						dev_GUI_Requires_ICheckRemoteReady()
#define IDraw()									dev_GUI_Requires_IDraw()		
#define IDrawMem()								dev_GUI_Requires_IDrawMem()		
#define IDisplayOn(b)							dev_GUI_Requires_IDisplayOn(b)		
#define IDisplayOff()							dev_GUI_Requires_IDisplayOff()		
#define IDisplayStandby()						dev_GUI_Requires_IDisplayStandby()		
#define IBacklightOn()							dev_GUI_Requires_IBacklightOn()		
#define IBacklightOff()							dev_GUI_Requires_IBacklightOff()		
#define IBacklightPermit()						dev_GUI_Requires_IBacklightPermit()		
#define IBacklightForbid()						dev_GUI_Requires_IBacklightForbid()		
#define IBacklightGetContrast()					dev_GUI_Requires_IBacklightGetContrast()		
#define IBacklightSetContrast(c)				dev_GUI_Requires_IBacklightSetContrast(c)		
#define IGetLogo()								dev_GUI_Requires_IGetLogo()
#define IBatteryStartScan()						dev_GUI_Requires_IBatteryStartScan()
#define IBatteryGetLevel()						dev_GUI_Requires_IBatteryGetLevel()
#define IBatteryCancelScan()					dev_GUI_Requires_IBatteryCancelScan()
#define IEnterStopMode()						dev_GUI_Requires_IEnterStopMode()		
#define IResartClocks()							dev_GUI_Requires_IResartClocks()
#define IGetKey()								dev_GUI_Requires_IGetKey()
		

/****************************************************************************************************//**
 *                        PROTECTED STATE DEFINITIONS
 ********************************************************************************************************/

State GUI_State_Initial			(Object* pThis, Event const * e);
State GUI_State_Formatting		(Object* pThis, Event const * e);
State GUI_State_Top				(Object* pThis, Event const * e);
State GUI_State_Menu			(Object* pThis, Event const * e);
State GUI_State_Devices			(Object* pThis, Event const * e);
State GUI_State_Configuration	(Object* pThis, Event const * e);
State GUI_State_Location		(Object* pThis, Event const * e);
State GUI_State_CaptureLocation (Object* pThis, Event const * e);
State GUI_State_StoredLocation	(Object* pThis, Event const * e);
State GUI_State_SaveLocation	(Object* pThis, Event const * e);
State GUI_State_StoredCities	(Object* pThis, Event const * e);
State GUI_State_SendLocation	(Object* pThis, Event const * e);
State GUI_State_Language		(Object* pThis, Event const * e);
State GUI_State_DateTime		(Object* pThis, Event const * e);
State GUI_State_Memory			(Object* pThis, Event const * e);
State GUI_State_Version			(Object* pThis, Event const * e);
State GUI_State_Contrast		(Object* pThis, Event const * e);
State GUI_State_ReadDevice		(Object* pThis, Event const * e);
State GUI_State_SendDevice		(Object* pThis, Event const * e);
State GUI_State_SaveDevice		(Object* pThis, Event const * e);
State GUI_State_StoredDevice	(Object* pThis, Event const * e);

/****************************************************************************************************//**
 *                        PROTECTED TYPES
 ********************************************************************************************************/

/*************************************************************************//**
 * @def		ID_WINDOW1
 * @def		ID_OBJ_BUTTON_1
 * @scope	protected
 * @brief   First object ID codes to share across User Forms.
 ****************************************************************************/
#define ID_WINDOW1          	0xFFFF
#define ID_OBJ_BUTTON_1			1

/*************************************************************************//**
 * @def		WAIT_UNTIL_FINISH
 * @scope	protected
 * @brief   Condition shared across User Forms
 ****************************************************************************/
#define WAIT_UNTIL_FINISH(x)    while(!x)

/*************************************************************************//**
 * @def		MAX_FIELDS
 * @scope	protected
 * @brief   Max number of fields formed by T_PDU_Full structure
 ****************************************************************************/
#define MAX_FIELDS		5

/*************************************************************************//**
 * @def		GUI_MBX_TIMEOUT
 * @scope	protected
 * @brief   Returns the number of OS_TICK for a delay of 100 milliseconds while
 *			waiting for a new display refresh action
 ****************************************************************************/
#define GUI_MBX_TIMEOUT		GetOSTicks(100)

/*************************************************************************//**
 * @enum	E_GUI_EVENTS
 * @scope	protected
 * @brief   Event signals accepted by the state machine of this device
 ****************************************************************************/
typedef enum {
    EV_GOLMSG   = USER_SIG,     //!< A GOL Message
	EV_TRANSITION				//!< transition request from graphic handlers
}E_GUI_EVENTS;

/*************************************************************************//**
 * @struct	T_GUI_Message
 * @scope	protected
 * @brief   This structure is the message type accepted by the object. As can
 *			be seen, its content can vary according with the public interface
 *			invoked from an external component. This implies than both SendMsg
 *			and ReadMsg can be thrown to this object and inserted into the mem-
 *			pool as a block of size = sizeof(T_GUI_Message).
 ****************************************************************************/
typedef struct  {
    Event 	super;          	//!< derives from Event
	GOL_MSG msg;				
}T_GUI_Message;

/*************************************************************************//**
 * @struct	T_PDU_Full
 * @scope	protected
 * @brief   This structure is the composition for PDU management during
 *			read/write remote IR communications.
 ****************************************************************************/
typedef struct{
	uint8_t Field[MAX_FIELDS][22];
	uint8_t *progs;
}T_PDU_Full;


/*************************************************************************//**
 * @struct	T_DEV_GUI
 * @scope	protected
 * @brief   This structure is the internal device structure with all the static
 *			variable required for functionality.
 ****************************************************************************/
typedef struct{
	IRunnable	runnable;		//!< Interface to execute a thread with a mailhox
	uint32_t	uid;			//!< Component UID
	uint32_t	timeout;		//!< timeout for os_mbx_wait or os_evt_wait_or
	void *		logo;			//!< external logo reference to be printed
	void *	    initStack;		//!< stack from init task which must be freed.
	void *		altScheme;      //!< pointer to GOL_SCHEME                          
	void *		WindowsScheme;	//!< pointer to GOL_SCHEME
	void *		hugeScheme;		//!< pointer to GOL_SCHEME
	void *		minScheme;		//!< pointer to GOL_SCHEME
	void *		OnCreateForm; 	//!< pointer to GOL_callback
	void *		OnDisplayForm;	//!< pointer to GOL_callback
	void *		OnCallbackForm;	//!< pointer to GOL_callback
	void *		mustBeFreed;	//!< pointer to dynamic memory that must be deallocated
	STRGps *	pGps;			//!< pointer to GPS data structure 
	T_PDU_Full *rwPdu;			//!< referencia to PDU in process
	GOL_MSG 	msg;			//!< message in process
	uint16_t	auxCounter;		//!< auxiliar counter for standby timeout operation
	uint16_t	statCounter;	//!< auxiliar counter for state timed-delays (for tilt or similar)
	uint8_t		selectedOption;	//!< auxiliar global index pointing to selected option on listbox
	char *		selectedText;	//!< auxiliar global pointer to selected text option
	uint8_t		deepness;		//!< auxiliar global deep index for FlexMenu control option
	uint8_t		language;		//!< language selection index
	CCalendar	calendar;		//!< calendar for auxiliary operations
	uint8_t		fsReady;		//!< flag to identify if filesystem is ready for operation
	uint8_t		blContrast;		//!< contrast assigned to backlight status.
	uint8_t		isDataReady;	//!< flag to notify data ready 
	uint8_t		isReceiving;	//!< flag to indicate recv/send operations
	uint8_t		isGpsSaved;		//!< flag to indicate that a new location was saved
	uint8_t		isToutAllowed;	//!< flag to indicate that Timeout to State_Top is allowed or not.
	uint8_t		battLevel;		//!< data with battery voltage level
	uint8_t		discardSeasonFlags; //!< flag to indicate if seasons must be overriden during clock transfer
	StateHandler OnFormatExit;
}T_DEV_GUI;


/*************************************************************************//**
 * @enum	SCREEN_STATES
 * @scope	protected
 * @brief   This enumeration report the state of the screen in order to draw
 *			objects or execute callbacks.
 ****************************************************************************/
typedef enum{
    GUI_CREATE = 0,
	GUI_DISPLAY,
	CREATE_CALIBRATE_TOUCH,
    DISPLAY_CALIBRATE_TOUCH,
    CREATE_BUTTONS,
    DISPLAY_BUTTONS,
	CREATE_CHECKBOXES,
    DISPLAY_CHECKBOXES,
    CREATE_RADIOBUTTONS,
    DISPLAY_RADIOBUTTONS,
    CREATE_STATICTEXT,
    DISPLAY_STATICTEXT,
    CREATE_PICTURE,
    DISPLAY_PICTURE,
    CREATE_SLIDER,
    DISPLAY_SLIDER,
    CURSOR_DRAW_SLIDER,
    CREATE_PROGRESSBAR,
    DISPLAY_PROGRESSBAR,
    CREATE_LISTBOX,
    DISPLAY_LISTBOX,
    CREATE_EDITBOX,
    DISPLAY_EDITBOX,
    CREATE_METER,
    DISPLAY_METER,
    CREATE_DIAL,
    DISPLAY_DIAL,
    CREATE_CUSTOMCONTROL,
    DISPLAY_CUSTOMCONTROL,
    CREATE_SIGNATURE,
    DISPLAY_SIGNATURE,
    BOX_DRAW_SIGNATURE,
    CREATE_POT,
    DISPLAY_POT,
    BOX_DRAW_POT,
    CREATE_ECG,
    DISPLAY_ECG,
    BOX_DRAW_ECG,
    CREATE_PULLDOWN,
    DISPLAY_PULLDOWN,
    // these states are for time and date settings 0xF3xx is used here as a
    // state ID to check when date and time are to be updated or not.
    CREATE_DATETIME         = 0xF300,                       // creates the date and time setting menu		
    DISPLAY_DATETIME        = 0xF301,                       // displays the menu for the date and time setting
    DISPLAY_DATE_PDMENU     = 0xF302,                       // displays the pulldown menu to set one of the date items (month, day or year)
    SHOW_DATE_PDMENU        = 0xF303,                       // creates the pulldown menu to set one of the date items (month, day or year)
    HIDE_DATE_PDMENU        = 0xF304                        // hides the pulldown menu to set one of the date items (month, day or year)
}SCREEN_STATES;

/*************************************************************************//**
 * @var		progs
 * @scope	protected
 * @brief   Array of device_programs for rw operations
 ****************************************************************************/
extern uint8_t 		progs[MAX_PROGS][22];

/*************************************************************************//**
 * @var		commPdu
 * @scope	protected
 * @brief   device_structed_data for rw operations
 ****************************************************************************/
extern T_PDU_Full 	commPdu;

/****************************************************************************************************//**
 *                        PROTECTED FUNTIONS
 ********************************************************************************************************/
void 	NotifyTransition (StateHandler target);
void 	GUI_NewData_Writing_Hook(T_DEV_GUI * pThis, uint8_t *pdata, uint16_t size);
void 	GUI_LoadScreen (void (*create_func)(void *pThis),void (*display_func)(void *pThis),uint32_t (*callback_func)(void *pThis,uint32_t objMsg, OBJ_HEADER *pObj,GOL_MSG *pMsg));
void 	GUI_LoadSchemes(void);
uint8_t GUI_RestoreNonVolatileConfig(T_DEV_GUI * me);
void 	GUI_FormatLocationToSend(void *pThis);
void 	GUI_FormatClockToSend(void *pThis);
#endif

