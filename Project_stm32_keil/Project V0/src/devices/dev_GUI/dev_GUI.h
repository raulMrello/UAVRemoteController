/*************************************************************************//**
 * @file    dev_GUI.h
 * @mcu		STM32F1x
 * @brief   GUI Device Manager
 * @date    15.07.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef __dev_GUI_H__
#define __dev_GUI_H__

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <stdint.h>
#include "CCalendar.h"

/*************************************************************************//**
 * @struct	GOL_MSG
 * @scope	public
 * @brief   Data estructure for message posting
 ****************************************************************************/
typedef struct{
    uint8_t    type;                   //!< Type of input device.
    uint8_t    uiEvent;                //!< The generic events for input device.
    uint16_t   param1;                 //!< Parameter 1 meaning is dependent on the type of input device.
    uint16_t   param2;                 //!< Parameter 2 meaning is dependent on the type of input device.
} GOL_MSG;

/****************************************************************************************************//**
 *                        PROVIDED INTERFACES
 ********************************************************************************************************/
uint8_t	dev_GUI_Init   		(uint8_t priority, uint8_t isFSready, void *initStack);
void 	dev_GUI_NewIrData	(uint8_t *pdata, uint16_t size);
void 	dev_GUI_NewGpsData	(uint8_t *pdata, uint16_t size);
void 	dev_GUI_NewKey		(uint32_t keycode);
void 	dev_GUI_NewTime		(uint32_t timecode);
int8_t 	dev_GUI_SendMsg		(GOL_MSG *msg);

/****************************************************************************************************//**
 *                        REQUIRED INTERFACES
 *
 * To be implemented in <SYS_Link.c>
 ********************************************************************************************************/
extern uint8_t 	 (* const dev_GUI_Requires_IBeepShort)(void);
extern uint8_t 	 (* const dev_GUI_Requires_IBeepLong)(void);
extern uint8_t	 (* const dev_GUI_Requires_IBeepConfirm)(void);
extern void 	 (* const dev_GUI_Requires_IEnDisCalendarEvents)(uint8_t endis);
extern void 	 (* const dev_GUI_Requires_ICaptureLocation)(void);
extern void 	 (* const dev_GUI_Requires_IReleaseCapture)(void);
extern uint8_t 	 (* const dev_GUI_Requires_ICaptureIsDown)(void);
extern uint8_t	 (* const dev_GUI_Requires_ISendRemote)(uint8_t *pdu, uint8_t len);
extern uint8_t	 (* const dev_GUI_Requires_IReadRemote)(uint8_t *pdu, uint8_t len);
extern uint8_t	 (* const dev_GUI_Requires_ICancelRemote)(void);
extern uint8_t	 (* const dev_GUI_Requires_IPowerRemote)(void);
extern uint8_t	 (* const dev_GUI_Requires_IUnpowerRemote)(void);
extern uint8_t	 (* const dev_GUI_Requires_ICheckRemoteReady)(void);
extern void 	 (* const dev_GUI_Requires_IDraw)(void);
extern void 	 (* const dev_GUI_Requires_IDrawMem)(void);
extern void 	 (* const dev_GUI_Requires_IDisplayOn)(uint8_t b);
extern void 	 (* const dev_GUI_Requires_IDisplayOff)(void);
extern void 	 (* const dev_GUI_Requires_IDisplayStandby)(void);
extern void 	 (* const dev_GUI_Requires_IBacklightOn)(void);
extern void 	 (* const dev_GUI_Requires_IBacklightOff)(void);
extern void 	 (* const dev_GUI_Requires_IBacklightPermit)(void);
extern void 	 (* const dev_GUI_Requires_IBacklightForbid)(void);
extern uint8_t	 (* const dev_GUI_Requires_IBacklightGetContrast)(void);
extern void 	 (* const dev_GUI_Requires_IBacklightSetContrast)(uint8_t c);
extern void *	 (* const dev_GUI_Requires_IGetLogo)(void);
extern void 	 (* const dev_GUI_Requires_IGetCalendar)(CCalendar *c);
extern void 	 (* const dev_GUI_Requires_ISetCalendar)(CCalendar *c);
extern uint32_t  (* const dev_GUI_Requires_IGetCalendarCounter)(void);
extern void 	 (* const dev_GUI_Requires_IBatteryStartScan)(void);
extern uint8_t 	 (* const dev_GUI_Requires_IBatteryGetLevel)(void);
extern void 	 (* const dev_GUI_Requires_IBatteryCancelScan)(void);
extern void 	 (* const dev_GUI_Requires_IEnterStopMode)(void);
extern void 	 (* const dev_GUI_Requires_IResartClocks)(void);
extern int8_t    (* const dev_GUI_Requires_IGetKey)(void);

/****************************************************************************************************//**
 *                        PUBLIC TYPES 
 ********************************************************************************************************/   

/*************************************************************************//**
 * @enum	T_KEYCODE
 * @scope	public
 * @brief   Data enumeration to define different types of keystrokes
 ****************************************************************************/
typedef enum{
	KEY_LEFT 	= 'l',
	KEY_RIGHT 	= 'r',
	KEY_UP 		= 'u',
	KEY_DOWN 	= 'd',
	KEY_POWER 	= 'p',
	KEY_ENTER 	= 'e',
	KEY_CANCEL 	= 'c',
	KEY_NUM 	= 'n'
}T_KEYCODE;

/*************************************************************************//**
 * @enum	INPUT_DEVICE_TYPE
 * @scope	public
 * @brief   Data enumeration to define different input devices
 ****************************************************************************/
typedef enum{
    TYPE_UNKNOWN    = 0,            //!< Unknown device.
    TYPE_KEYBOARD,                  //!< Keyboard.
    TYPE_TOUCHSCREEN,               //!< Touchscreen.
    TYPE_MOUSE,                     //!< Mouse.
    TYPE_TIMER,                     //!< Timer.
    TYPE_SYSTEM,                    //!< System Messages.
	TYPE_GPS,						//!< GPS device
	TYPE_IR,						//!< IR device
	TYPE_BAT,						//!< BAT device
	TYPE_TRANSITION					//!< reserved to TRAN requests
} INPUT_DEVICE_TYPE;

/*************************************************************************//**
 * @enum	INPUT_DEVICE_EVENT
 * @scope	public
 * @brief   Data enumeration with definition of different input device events
 ****************************************************************************/
typedef enum {
    EVENT_INVALID   = 0,     		//!< An invalid event.
    EVENT_MOVE,                     //!< A move event.
    EVENT_PRESS,                    //!< A press event.
    EVENT_STILLPRESS,               //!< A continuous press event.
    EVENT_RELEASE,                  //!< A release event.
    EVENT_KEYSCAN,                  //!< A keyscan event, parameters has the object ID keyboard scan code.
    EVENT_CHARCODE,                 //!< Character code is presented at the parameters.
    EVENT_SET,                      //!< A generic set event.
    EVENT_SET_STATE,                //!< A set state event.	
    EVENT_CLR_STATE,                //!< A clear state event.
	ENVENT_KEY_LONG
}INPUT_DEVICE_EVENT;

void 	*Graphics_GetLogo(void);


#endif

