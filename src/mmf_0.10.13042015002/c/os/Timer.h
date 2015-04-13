/*
 * Timer.h
 *
 *  Created on: 05/4/2015
 *      Author: raulMrello
 *
 *  Timers are data structures that handle tick events into the MMF framework, and allow task suspension
 *  and other kind of timming operations.
 */

#ifndef MMF_OS_C_TIMER_H_
#define MMF_OS_C_TIMER_H_

#ifdef __cplusplus
 extern "C" {
#endif

//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include <stdint.h>
#include "Exception.h"

//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** Pointer to Timer object */
typedef struct Timer_t * 		TimerPtr;

/** Timer status codes */
typedef enum {
	TMR_STOPPED = 	      0 ,		///< timer stopped
	TMR_TIMMING = 	(1 << 0),		///< timer running
	TMR_REPEAT  = 	(1 << 1)		///< timer repeat enabled
}TimerStatEnum;

/** Type definitions for callback declaration  */
typedef void (*TimeoutCallback)(void* cbhandler);

/** Type definitions for callback handler  */
typedef void *	TimerHandlerObj;


//------------------------------------------------------------------------------------
//-- PROTOTYPES ----------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** \fn Timer_create
 *  \brief Timer creation and initialization
 *  \param e Exception code
 *  \return Pointer to the allocated Timer object
 */
TimerPtr Timer_create(ExceptionPtr e);

/** \fn Timer_kill
 *  \brief Timer deallocation
 *  \param tmr Timer
 *  \param e Exception code
 */
void Timer_kill(TimerPtr* ptmr, ExceptionPtr e);

/** \fn Timer_start
 *  \brief Timer start with parameter settings
 *  \param tmr Timer reference
 *  \param microseconds Timming count in microseconds
 *  \param repeat Flag to enable repeated mode (autoreload)
 *  \param timeout On time out callback
 *  \param cbhandler Callback handler object
 *  \param e Exception code
 */
void Timer_start(TimerPtr tmr, uint32_t microseconds, uint8_t repeat, TimeoutCallback timeout, TimerHandlerObj cbhandler,	ExceptionPtr e);

/** \fn Timer_stop
 *  \brief Stops a timer
 *  \param tmr Timer
 *  \param e Exception code (0: success)
 */
void Timer_stop(TimerPtr tmr, ExceptionPtr e);

/** \fn Timer_tick
 *  \brief Executes a timer tick step. If timer reaches 0 then invokes its installed callback
 *  \param tmr Timer
 *  \param e Exception code (0: success)
 */
void Timer_tick(TimerPtr tmr, ExceptionPtr e);

#ifdef __cplusplus
}
#endif

#endif /* MMF_OS_C_TIMER_H_ */
