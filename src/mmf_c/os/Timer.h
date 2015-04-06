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

#include "Exception.h"
#include <stdint.h>

/**
 *  \enum TimerStat
 *  \brief Timer status codes
 */
typedef enum {
	TMR_STOPPED = 	      0 ,		///< timer stopped
	TMR_TIMMING = 	(1 << 0),		///< timer running
	TMR_REPEAT  = 	(1 << 1)		///< timer repeat enabled
}TimerStat;

/**
 *  \def Type definitions for callback declaration
 */
typedef void (*TimeoutCallback)(void* cbhandler);

/** \struct Timer
 *  \brief Timer data structure
 */
typedef struct {
	TimerStat status;			///< status
	int count;					///< count (in ticks)
	int repeat_count;			///< repeating count on repeat mode
	TimeoutCallback timeout;	///< timeout callback to invoke on timer countdown
	void * cbhandler;			///< timeout callback handler
}Timer;


/** \fn Timer_initialize
 *  \brief Timer initialization
 *  \param tmr Timer reference
 *  \param microseconds Timming count in microseconds
 *  \param repeat Flag to enable repeated mode (autoreload)
 *  \param timeout On time out callback
 *  \param cbhandler Callback handler object
 *  \param e Exception code
 */
void Timer_initialize(	Timer* tmr,
						int microseconds,
						char repeat,
						TimeoutCallback timeout,
						void * cbhandler,
						Exception *e);

/** \fn Timer_start
 *  \brief Starts a timer
 *  \param tmr Timer
 *  \param microseconds Timer in microseconds
 *  \param e Exception code (0: success)
 */
void Timer_start(Timer* tmr, int microseconds, Exception *e);

/** \fn Timer_startEx
 *  \brief Timer start with parameter settings
 *  \param tmr Timer reference
 *  \param microseconds Timming count in microseconds
 *  \param repeat Flag to enable repeated mode (autoreload)
 *  \param timeout On time out callback
 *  \param cbhandler Callback handler object
 *  \param e Exception code
 */
void Timer_startEx(	Timer* tmr,
						int microseconds,
						char repeat,
						TimeoutCallback timeout,
						void * cbhandler,
						Exception *e);

/** \fn Timer_stop
 *  \brief Stops a timer
 *  \param tmr Timer
 *  \param e Exception code (0: success)
 */
void Timer_stop(Timer* tmr, Exception *e);

/** \fn Timer_tick
 *  \brief Executes a timer tick step. If timer reaches 0 then invokes its installed callback
 *  \param tmr Timer
 *  \param e Exception code (0: success)
 */
void Timer_tick(Timer* tmr, Exception *e);

#endif /* MMF_OS_C_TIMER_H_ */
