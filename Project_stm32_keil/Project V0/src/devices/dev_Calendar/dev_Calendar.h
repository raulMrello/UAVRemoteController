/*************************************************************************//**
 * @file    dev_Calendar.h
 * @mcu		STM32F1x
 * @brief   Calendar Device Manager
 * @date    11.06.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef __dev_Calendar_H__
#define __dev_Calendar_H__

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <stdint.h>
#include "CCalendar.h"


/****************************************************************************************************//**
 *                        PROVIDED INTERFACES
 ********************************************************************************************************/
uint8_t  dev_Calendar_Init	 		(uint8_t priority);
void	 dev_Calendar_EnDisEvents	(uint8_t endis);
void 	 dev_Calendar_Get	 		(CCalendar *c);
void 	 dev_Calendar_Set	 		(CCalendar *c);
uint32_t dev_Calendar_GetCounter	(void);
void 	 dev_Calendar_NewTime 		(void);


/****************************************************************************************************//**
 *                        REQUIRED INTERFACES
 *
 * To be implemented in <SYS_Link.c>
 ********************************************************************************************************/
extern void 	 (* const dev_Calendar_Requires_IDriverInit)(void);
extern void 	 (* const dev_Calendar_Requires_IDriverEnable)(void);
extern void 	 (* const dev_Calendar_Requires_IDriverDisable)(void);
extern uint32_t  (* const dev_Calendar_Requires_IGetTime)(void);
extern void 	 (* const dev_Calendar_Requires_ISetTime)(uint32_t t);
extern void 	 (* const dev_Calendar_Requires_ISendTime)(uint32_t tEv);



/****************************************************************************************************//**
 *                        PUBLIC TYPES
 ********************************************************************************************************/

/*************************************************************************//**
 * @enum	E_WEEKDAY
 * @scope	public
 * @brief   mask definitions to codify weekdays
 ****************************************************************************/
typedef enum{
	WEEK_SUN = 0x1,
	WEEK_SAT = 0x2,
	WEEK_FRI = 0x4,
	WEEK_THU = 0x8,
	WEEK_WED = 0x10,
	WEEK_TUE = 0x20,
	WEEK_MON = 0x40
}E_WEEKDAY;

/*************************************************************************//**
 * @enum	E_CAL_CHANGED
 * @scope	public
 * @brief   Defines different conditions detected after reading a new time
 ****************************************************************************/
typedef enum{
	CAL_NEW_SEC = 0x1,
	CAL_NEW_MIN = 0x2,
	CAL_NEW_HOU = 0x4,
	CAL_NEW_DAY = 0x8,
	CAL_NEW_WEE = 0x10,
	CAL_NEW_MON = 0x20,
	CAL_NEW_YEA = 0x40
}E_CAL_CHANGED;

#endif

