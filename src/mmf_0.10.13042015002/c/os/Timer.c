/*
 * task.cpp
 *
 *  Created on: 11/3/2015
 *      Author: raulMrello
 */

#include "Timer.h"
#include "Memory.h"
#include "../port/platforms.h" 


//------------------------------------------------------------------------------------
//-- REQUIRED MMF DEPENDENCIES -------------------------------------------------------
//------------------------------------------------------------------------------------

/** Required local members from other mmf modules */
extern int OS_getTimeTicks(int microseconds);

//------------------------------------------------------------------------------------
//-- PRIVATE TYPEDEFS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

/** \struct Timer
 *  \brief Timer data structure
 */
typedef struct {
	TimerStatEnum status;		///< status
	uint32_t count;				///< count (in ticks)
	uint32_t repeat_count;		///< repeating count on repeat mode
	TimeoutCallback timeout;	///< timeout callback to invoke on timer countdown
	TimerHandlerObj cbhandler;	///< timeout callback handler
}Timer;

//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------


TimerPtr Timer_create(ExceptionPtr e){
	Timer *tmr = (Timer*)Memory_alloc(sizeof(Timer), e);
	catch(e){
		return 0;
	}
	PLATFORM_ENTER_CRITICAL();
	tmr->status = TMR_STOPPED;
	tmr->count = 0;
	tmr->repeat_count = 0;
	tmr->timeout = 0;
	tmr->cbhandler = 0;
	PLATFORM_EXIT_CRITICAL();
	return (TimerPtr)tmr;
}

//------------------------------------------------------------------------------------
void Timer_kill(TimerPtr* ptimer, ExceptionPtr e){
	Timer** pthis = (Timer**)ptimer;
	if(!ptimer || !pthis){
		Exception_throw(e, BAD_ARGUMENT, "Timer_kill ptimer is null");
		return;
	}
	PLATFORM_ENTER_CRITICAL();
	Timer* this;
	this = *pthis;
	if(!this){
		PLATFORM_EXIT_CRITICAL();
		Exception_throw(e, BAD_ARGUMENT, "Timer_kill *ptimer is null");
		return;
	}
	Memory_free(this, e);
	*pthis = 0;
	PLATFORM_EXIT_CRITICAL();
}


//------------------------------------------------------------------------------------
void Timer_start(TimerPtr tmr, uint32_t microseconds, uint8_t repeat, TimeoutCallback timeout, TimerHandlerObj cbhandler,	ExceptionPtr e){
	int ticks = OS_getTimeTicks(microseconds);
		if(!tmr || !ticks || !timeout){
		Exception_throw(e, BAD_ARGUMENT, "Timer_startEx param is null");
		return;
	}
	Timer *this = (Timer*)tmr;
	PLATFORM_ENTER_CRITICAL();
	this->count = ticks;
	this->repeat_count = ticks;
	if(repeat){
		this->status |= TMR_REPEAT;
	}
	this->timeout = timeout;
	this->cbhandler = cbhandler;
	this->status |= TMR_TIMMING;
	PLATFORM_EXIT_CRITICAL();
}


//------------------------------------------------------------------------------------
void Timer_stop(TimerPtr tmr, ExceptionPtr e){
	if(!tmr){
		Exception_throw(e, BAD_ARGUMENT, "Timer_stop tmr is null");
		return; 
	}
	Timer *this = (Timer*)tmr;
	PLATFORM_ENTER_CRITICAL();
	this->status &= ~TMR_TIMMING;
	PLATFORM_EXIT_CRITICAL();
}


//------------------------------------------------------------------------------------
void Timer_tick(TimerPtr tmr, ExceptionPtr e){
	if(!tmr){
		Exception_throw(e, BAD_ARGUMENT, "Timer_tick tmr is null");
		return; 
	}
	Timer *this = (Timer*)tmr;
	PLATFORM_ENTER_CRITICAL();
	if((this->status & TMR_TIMMING) == 0){
		PLATFORM_EXIT_CRITICAL();
		return;
	}
	if(--this->count <= 0){
		this->status &= ~TMR_TIMMING;
		if((this->status & TMR_REPEAT) != 0){
			this->count = this->repeat_count;
			this->status |= TMR_TIMMING;
		}
		this->timeout(this->cbhandler);
	}
	PLATFORM_EXIT_CRITICAL();
}

