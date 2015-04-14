/*
 * drv_POW.h
 *
 *  Created on: 07/04/2015
 *      Author: raulMrello
 *
 *  POW driver for STM32F103C8T6. It controls:
 *
 *  	- Clock configuration and STOP modes
 *
 *  On initialization "drv_POW_Init()":
 *
 *  	- Attaches to /sys topic updates
 *
 *  Basic operation:
 *		1. Module is ready for operation
 *		2. After a /sys topic update it manages accordingly. 
 *
 */
 
#ifndef __DRV_POW_H__
#define __DRV_POW_H__

#ifdef __cplusplus
 extern "C" {
#endif 


//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------


#include <stdint.h>
#include "stm32f10x.h"	    	///< STM32F10x Library Definitions

//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** Pow reference */
typedef struct PowCtrl_t* drv_POW;

/** Pow status enum */
typedef enum {
	POW_RUN_MODE = (1 << 0)
}PowStatusEnum;

/** Pow status flag combination */
typedef uint32_t PowFlags;

//------------------------------------------------------------------------------------
//-- PROTOTYPES ----------------------------------------------------------------------
//------------------------------------------------------------------------------------


/** \fun drv_POW_Init
 *  \brief Initializes POW peripheral
 *	\param e Exception handler
 */
drv_POW drv_POW_Init(void);

/** \fn drv_POW_setStop
 *  \brief Set stop mode
 *  \param drv Pow reference
*	\param mode Mode
 */
void drv_POW_setMode(drv_POW drv, PowFlags mode);


#ifdef __cplusplus
}
#endif



#endif	//__DRV_POW_H__
