/*
 * JoystickSampler.cpp
 *
 *  Created on: 20/04/2015
 *      Author: raulMrello
 */

#include "JoystickSampler.h"

//------------------------------------------------------------------------------------
//-- PRIVATE TYPEDEFS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//-- PRIVATE DEFINITIONS -------------------------------------------------------------
//------------------------------------------------------------------------------------

/** Milliseconds timeout to publish a /joys update when a joystick has changed */
#define REPEAT_TIMEOUT	500

//------------------------------------------------------------------------------------
//-- STATIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

static void scanCb(void const *handler){
	JoystickSampler * me = (JoystickSampler*)handler;
	me->scan();
}

//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

JoystickSampler::JoystickSampler(AnalogIn *joystick_A1, AnalogIn *joystick_A2, AnalogIn *joystick_B1, AnalogIn *joystick_B2) {
	_stat = 0;
	_joystick_A1 = joystick_A1;
	_joystick_A2 = joystick_A2;
	_joystick_B1 = joystick_B1;
	_joystick_B2 = joystick_B2;
	_throttle_rate = 5;
	_tmr = new RtosTimer(scanCb, osTimerPeriodic, this);
}

//------------------------------------------------------------------------------------
JoystickSampler::~JoystickSampler() {
	delete(_tmr);
}

//------------------------------------------------------------------------------------
void JoystickSampler::startScan() {
	_tmr->start(REPEAT_TIMEOUT);
}

//------------------------------------------------------------------------------------
void JoystickSampler::stopScan() {
	_tmr->stop();
}


//------------------------------------------------------------------------------------
void JoystickSampler::scan(){
	static int8_t ja1z, ja2z, jb1z, jb2z, last_ja2;
	int8_t new_ja2;
	float raw_throttle, raw_yaw, raw_pitch, raw_roll;
	raw_throttle = _joystick_A2->read();
	raw_yaw = _joystick_A1->read();
	raw_pitch = _joystick_B2->read();
	raw_roll = _joystick_B1->read();
	// get zero values
	while(_stat < 3){
		_stat++;
		ja1z = 50-(int8_t)(100*raw_yaw);
		ja2z = 50-(int8_t)(100*raw_throttle);
		jb1z = 50-(int8_t)(100*raw_roll);
		jb2z = 50-(int8_t)(100*raw_pitch);
		if(_stat == 3){
			// initializes data
			last_ja2 = 50;
			_joystickdata.yaw = 50;	
			_joystickdata.throttle = 0;
			_joystickdata.roll = 50;
			_joystickdata.pitch = 50;
		}		
		return;
	}
	// read controls applying zero corrections
	int8_t ja1, ja2, jb1, jb2;
	ja1 = ja1z + (int8_t)(100*raw_yaw);
	ja2 = ja2z + (int8_t)(100*raw_throttle);
	jb1 = jb1z + (int8_t)(100*raw_roll);
	jb2 = jb2z + (int8_t)(100*raw_pitch);
	// set new data
	_joystickdata.yaw = (ja1 <= 100)? ja1 : 100;		
	_joystickdata.roll = (jb1 <= 100)? jb1 : 100;
	_joystickdata.pitch = (jb2 <= 100)? jb2 : 100;
	new_ja2 = (ja2 <= 100)? ja2 : 100;
	// read dynamic control
	_joystickdata.throttle += (new_ja2 - last_ja2)/_throttle_rate;
	last_ja2 = new_ja2;
	
	// publish topic update			
	MsgBroker::publish("/joys", &_joystickdata, sizeof(Topic::JoystickData_t));
		
}

//------------------------------------------------------------------------------------
//-- PROTECTED/PRIVATE FUNCTIONS -----------------------------------------------------
//------------------------------------------------------------------------------------


