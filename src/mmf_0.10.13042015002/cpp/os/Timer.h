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


//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include <stdint.h>
#include "Exception.h"

namespace MMF {

//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** Type definitions for callback handler  */
typedef void *	TimerHandlerObj;

/** Type definitions for callback declaration  */
typedef void (*TimeoutCallback)(TimerHandlerObj cbhandler);



//------------------------------------------------------------------------------------
//-- CLASS ---------------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** \class Timer
 *  \brief Timer management class
 */
class Timer {
public:
	/** \fn Timer_create
	 *  \brief Timer creation and initialization
	 *  \param _timeoutCallback Timeout callback
	 *  \param _cbhandler Timeout callback handler
	 */
	Timer();

	/** \fn Timer_kill
	 *  \brief Timer deallocation
	 */
	~Timer();

	/** \fn start
	 *  \brief Timer start with parameter settings
	 *  \param microseconds Timming count in microseconds
	 *  \param repeat Flag to enable repeated mode (autoreload)
	 *  \throw exception
	 */
	void start(uint32_t microseconds, bool repeat, TimeoutCallback timeoutCallback, TimerHandlerObj cbhandler) throw (Exception);

	/** \fn stop
	 *  \brief Stops a timer
	 */
	void stop();

	/** \fn Ttick
	 *  \brief Executes a timer tick step. If timer reaches 0 then invokes its installed callback
	 */
	void tick();

	/** Timer status codes */
	typedef enum {
		TMR_STOPPED = 	      0 ,		///< timer stopped
		TMR_TIMMING = 	(1 << 0),		///< timer running
		TMR_REPEAT  = 	(1 << 1)		///< timer repeat enabled
	}TimerStatEnum;

private:
	typedef uint32_t TimerStatEnumFlags;
	TimerStatEnumFlags _status;			///< status flags
	uint32_t _count;					///< count (in ticks)
	uint32_t _repeat_count;				///< repeating count on repeat mode
	TimeoutCallback _timeoutCallback;	///< timeout callback
	TimerHandlerObj _cbhandler;			///< callback handler
};
}


#endif /* MMF_OS_C_TIMER_H_ */
