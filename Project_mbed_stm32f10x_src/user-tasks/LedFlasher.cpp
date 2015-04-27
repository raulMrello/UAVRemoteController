/*
 * LedFlasher.cpp
 *
 *  Created on: 20/04/2015
 *      Author: raulMrello
 */

#include "LedFlasher.h"

//------------------------------------------------------------------------------------
//-- PRIVATE TYPEDEFS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//-- PRIVATE DEFINITIONS -------------------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
//-- STATIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

static void ledTimer(void const *handler){
	LedFlasher::LedChannel * me = (LedFlasher::LedChannel *)handler;
	switch((int)me->status){
		case LedFlasher::STAT_FLASH:{
			me->status = LedFlasher::STAT_OFF;			
			me->led->write(0);
			me->tmr->start(me->off_time);
			return;
		}
		case LedFlasher::STAT_OFF:{
			if(me->wait_time == LedFlasher::TIME_DISABLED){
				me->status = LedFlasher::STAT_FLASH;			
				me->led->write(1);
				me->tmr->start(me->on_time);
			}
			else if(me->mode == LedFlasher::SINGLE_FAST_FLASHING){
				me->status = LedFlasher::STAT_WAIT;			
				me->tmr->start(me->wait_time);
			}
			else if(me->shots == 0){
				me->shots++;
				me->status = LedFlasher::STAT_FLASH;			
				me->led->write(1);
				me->tmr->start(me->on_time);
			}
			else{
				me->shots = 0;
				me->status = LedFlasher::STAT_WAIT;			
				me->tmr->start(me->wait_time);
			}
			break;
		}
		case LedFlasher::STAT_WAIT:{
			me->status = LedFlasher::STAT_FLASH;			
			me->led->write(1);
			me->tmr->start(me->on_time);
			break;
		}

	}
}

//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

LedFlasher::LedFlasher(uint8_t led_channels) {
	if(led_channels){
		_channels = led_channels;
		_led_list = (LedChannel *)malloc(_channels * sizeof(LedChannel));
		if(_led_list){
			uint8_t i = 0;
			for(i=0; i<_channels; i++){
				_led_list[i].led = 0;
			}
		}
	}
}

//------------------------------------------------------------------------------------
int8_t LedFlasher::addLedChannel(DigitalOut *led) {
	uint8_t i; 
	for(i=0;i<_channels;i++){
		if(_led_list[i].led == 0){
			_led_list[i].led = led;
			_led_list[i].led->write(0);
			_led_list[i].status = STOPPED;
			_led_list[i].tmr = new RtosTimer(ledTimer, osTimerPeriodic, &_led_list[i]);
			return i;
		}
	}
	return -1;
}

//------------------------------------------------------------------------------------
LedFlasher::~LedFlasher() {
	uint8_t i;
	for(i=0;i<_channels;i++){
		if(_led_list[i].led != 0){
			_led_list[i].led->write(0);
			_led_list[i].tmr->stop();
			delete(_led_list[i].tmr);
			_led_list[i].led = 0;
		}
	}
	free(_led_list);
}

//------------------------------------------------------------------------------------
void LedFlasher::ledStart(int8_t channel, ShotModeEnum mode){
	_led_list[channel].tmr->stop();
	_led_list[channel].mode = mode;
	switch((int)mode){
		case ON_FOREVER:{
			_led_list[channel].status = STAT_FLASH;			
			_led_list[channel].led->write(1);
			return;
		}
		case SLOW_FLASHING:{
			_led_list[channel].status = STAT_FLASH;			
			_led_list[channel].led->write(1);
			_led_list[channel].on_time = TIME_SLOW;
			_led_list[channel].off_time = TIME_SLOW;
			_led_list[channel].wait_time = TIME_DISABLED;
			break;
		}
		case SINGLE_FAST_FLASHING:
		case DUAL_FAST_FLASHING:{
			_led_list[channel].status = STAT_FLASH;			
			_led_list[channel].led->write(1);
			_led_list[channel].on_time = TIME_FAST;
			_led_list[channel].off_time = TIME_FAST;
			_led_list[channel].wait_time = TIME_WAIT;
			_led_list[channel].shots = 0;
			break;
		}
		case CONTINUOUS_FAST_FLASHING:{
			_led_list[channel].status = STAT_FLASH;			
			_led_list[channel].led->write(1);
			_led_list[channel].on_time = TIME_FAST;
			_led_list[channel].off_time = TIME_FAST;
			_led_list[channel].wait_time = TIME_DISABLED;
			break;
		}
	}
	_led_list[channel].tmr->start(_led_list[channel].on_time);
}


//------------------------------------------------------------------------------------
void LedFlasher::ledStop(int8_t channel) {
	_led_list[channel].tmr->stop();
	_led_list[channel].status = STOPPED;
	_led_list[channel].led->write(0);
}

//------------------------------------------------------------------------------------
//-- PROTECTED/PRIVATE FUNCTIONS -----------------------------------------------------
//------------------------------------------------------------------------------------

