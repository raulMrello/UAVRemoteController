/*
 * Timer.cpp
 *
 *  Created on: 11/3/2015
 *      Author: raulMrello
 */

#include "OS.h"
#include "Timer.h"
#include "Memory.h"
#include "../port/platforms.h" 
using namespace MMF;


//------------------------------------------------------------------------------------
//-- REQUIRED MMF DEPENDENCIES -------------------------------------------------------
//------------------------------------------------------------------------------------

/** Required local members from other mmf modules */
extern int OS_getTimeTicks(int microseconds);

//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------


Timer::Timer(){
	PLATFORM_ENTER_CRITICAL();
	_status = TMR_STOPPED;
	_count = 0;
	_repeat_count = 0;
	_timeoutCallback = 0;
	_cbhandler = 0;
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
Timer::~Timer(){

}


//------------------------------------------------------------------------------------
void Timer::start(uint32_t microseconds, bool repeat, TimeoutCallback timeoutCallback, TimerHandlerObj cbhandler) throw (Exception){
	if(!timeoutCallback){
		throw Exception(Exception::BAD_ARGUMENT, "Timer::start callback is null");
	}
	int ticks = OS::getTimeTicks(microseconds);
	if(!ticks){
		throw Exception(Exception::BAD_ARGUMENT, "Timer::start microseconds is invalid");
	}
	PLATFORM_ENTER_CRITICAL();
	_count = ticks;
	_repeat_count = ticks;
	if(repeat){
		_status |= TMR_REPEAT;
	}
	_status |= TMR_TIMMING;
	_timeoutCallback = timeoutCallback;
	_cbhandler = cbhandler;
	PLATFORM_EXIT_CRITICAL();
}


//------------------------------------------------------------------------------------
void Timer::stop(){
	PLATFORM_ENTER_CRITICAL();
	_status &= ~TMR_TIMMING;
	PLATFORM_EXIT_CRITICAL();
}


//------------------------------------------------------------------------------------
void Timer::tick(){
	PLATFORM_ENTER_CRITICAL();
	if((_status & TMR_TIMMING) == 0){
		PLATFORM_EXIT_CRITICAL();
		return;
	}
	if(--_count <= 0){
		_status &= ~TMR_TIMMING;
		if((_status & TMR_REPEAT) != 0){
			_count = _repeat_count;
			_status |= TMR_TIMMING;
		}
		_timeoutCallback(_cbhandler);
	}
	PLATFORM_EXIT_CRITICAL();
}

