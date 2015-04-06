/*************************************************************************//**
 * @file    dev_Bat_hsm.h
 * @mcu		STM32F1x
 * @brief   Bat Device State Machine protected header
 * @date    08.07.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef __dev_Bat_HSM_H__
#define __dev_Bat_HSM_H__

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <stdint.h>
#include "dev_Bat.h"

/****************************************************************************************************//**
 *                        	REQUIRED INTERFACES
 ********************************************************************************************************/
#define IDriverInit()			dev_Bat_Requires_IDriverInit()
#define IDriverScan()			dev_Bat_Requires_IDriverScan()
#define IDriverCancel()			dev_Bat_Requires_IDriverCancel()
#define IGetValue()				dev_Bat_Requires_IGetValue()					


/****************************************************************************************************//**
 *                        PROTECTED STATE DEFINITIONS
 ********************************************************************************************************/


/****************************************************************************************************//**
 *                        PROTECTED TYPES
 ********************************************************************************************************/

/*************************************************************************//**
 * @def		BAT_LEVEL_xxx
 * @scope	protected
 * @brief   Battery levels according with its charge state: 4 is max and 0 is min.
 ****************************************************************************/
#define BAT_LEVEL_MAX		4
#define BAT_LEVEL_HI		3
#define BAT_LEVEL_MED		2
#define BAT_LEVEL_LO		1
#define BAT_LEVEL_MIN		0


/*************************************************************************//**
 * @def		BAT_CROSS_xxx_yyy
 * @scope	protected
 * @brief   ADC measured values as frontiers between level xxx and level yyy.
 *
 * @notes	These are real measured values:
 *			3.2v = FC0h, 3.0v = EA0h, 2.8v = DB0h, 2.6v = C90h
 *			2.4v = B70h, 2.2v = A70h, 2.0v = 940h, 1.8v = 860h, 1.6v = 710h
 *	
 ****************************************************************************/
#define BAT_CROSS_MAX_HI	0x0D48	//2.7v
#define BAT_CROSS_HI_MED	0x0C48  //2.5v
#define BAT_CROSS_MED_LO	0x0B48	//2.3v
#define BAT_CROSS_LO_MIN	0x0A48	//2.1v

/*************************************************************************//**
 * @struct	T_DEV_Bat
 * @scope	protected
 * @brief   This structure is the internal device structure with all the static
 *			variable required for functionality.
 ****************************************************************************/
typedef struct{
	uint32_t		accValue;		//!< accumulated sum of scanned values
	uint16_t		rawValue;		//!< last battery value scanned
	uint8_t			numScans;		//!< number of averaged scans.	
	uint8_t			batLevel;		//!< 0=empty, 1=low, 2=med, 3=high 4=full
}T_DEV_Bat;


/****************************************************************************************************//**
 *                        PROTECTED FUNTIONS
 ********************************************************************************************************/

#endif

