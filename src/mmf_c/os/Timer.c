/*
 * task.cpp
 *
 *  Created on: 11/3/2015
 *      Author: raulMrello
 */

#include "Timer.h"
#include "../port/platforms.h" 


//------------------------------------------------------------------------------------
//-- REQUIRED MMF DEPENDENCIES -------------------------------------------------------
//------------------------------------------------------------------------------------

/** Required local members from other mmf modules */
extern int OS_getTimeTicks(int microseconds);

//------------------------------------------------------------------------------------
//-- PRIVATE MEMBERS -----------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
void Timer_initialize(Timer* tmr, int microseconds, char repeat, TimeoutCallback timeout, void * cbhandler, Exception *e){
	int ticks = OS_getTimeTicks(microseconds);
	if(!tmr || !timeout){
		Exception_throw(e, BAD_ARGUMENT, "Timer_initialize param is null");
		return; 
	}
	tmr->status = TMR_STOPPED;
	tmr->count = ticks;
	tmr->repeat_count = ticks;
	if(repeat){
		tmr->status |= TMR_REPEAT;
	}
	tmr->timeout = timeout;
	tmr->cbhandler = cbhandler;
}


//------------------------------------------------------------------------------------
void Timer_start(Timer* tmr, int microseconds, Exception *e){
	int ticks = OS_getTimeTicks(microseconds);
	if(!tmr || !ticks){
		Exception_throw(e, BAD_ARGUMENT, "Timer_start tmr is null");
		return; 
	}
	PLATFORM_ENTER_CRITICAL();
	tmr->count = ticks;
	tmr->repeat_count = ticks;
	tmr->status |= TMR_TIMMING;
	PLATFORM_EXIT_CRITICAL();
}


//------------------------------------------------------------------------------------
void Timer_startEx(Timer* tmr, int microseconds, char repeat, TimeoutCallback timeout, void * cbhandler, Exception *e){
	int ticks = OS_getTimeTicks(microseconds);
		if(!tmr || !ticks || !timeout){
		Exception_throw(e, BAD_ARGUMENT, "Timer_startEx param is null");
		return;
	}
	PLATFORM_ENTER_CRITICAL();
	tmr->count = ticks;
	tmr->repeat_count = ticks;
	if(repeat){
		tmr->status |= TMR_REPEAT;
	}
	tmr->timeout = timeout;
	tmr->cbhandler = cbhandler;
	tmr->status |= TMR_TIMMING;
	PLATFORM_EXIT_CRITICAL();
}


//------------------------------------------------------------------------------------
void Timer_stop(Timer* tmr, Exception *e){
	if(!tmr){
		Exception_throw(e, BAD_ARGUMENT, "Timer_stop tmr is null");
		return; 
	}
	PLATFORM_ENTER_CRITICAL();
	tmr->status &= ~TMR_TIMMING;
	PLATFORM_EXIT_CRITICAL();
}


//------------------------------------------------------------------------------------
void Timer_tick(Timer* tmr, Exception *e){
	if(!tmr){
		Exception_throw(e, BAD_ARGUMENT, "Timer_tick tmr is null");
		return; 
	}
	PLATFORM_ENTER_CRITICAL();
	if((tmr->status & TMR_TIMMING) == 0){
		PLATFORM_EXIT_CRITICAL();
		return;
	}
	if(--tmr->count <= 0){
		tmr->status &= ~TMR_TIMMING;
		if((tmr->status & TMR_REPEAT) != 0){
			tmr->count = tmr->repeat_count;
			tmr->status |= TMR_TIMMING;
		}
		tmr->timeout(tmr->cbhandler);
	}
	PLATFORM_EXIT_CRITICAL();
}

