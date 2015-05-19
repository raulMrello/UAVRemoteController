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

//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

JoystickSampler::JoystickSampler(osPriority prio, AnalogIn *joystick_A1, AnalogIn *joystick_A2, AnalogIn *joystick_B1, AnalogIn *joystick_B2) {

	_joystick_A1 = joystick_A1;
	_joystick_A2 = joystick_A2;
	_joystick_B1 = joystick_B1;
	_joystick_B2 = joystick_B2;
	_throttle_rate = 5;
	_th = 0;	
	_th = new Thread(&JoystickSampler::task, this, prio);
}

//------------------------------------------------------------------------------------
JoystickSampler::~JoystickSampler() {
}

//------------------------------------------------------------------------------------
Thread * JoystickSampler::getThread() {
	return _th;
}

//------------------------------------------------------------------------------------
void JoystickSampler::run(){
	int8_t ja1z, ja2z, jb1z, jb2z, last_throttle;
	while(_th == 0){
		Thread::wait(100);
	}
	// get zero values
	for(int i = 0;i < 3; i++){
		ja1z = 50-(int8_t)(100*_joystick_A1->read());
		ja2z = 50-(int8_t)(100*_joystick_A2->read());
		jb1z = 50-(int8_t)(100*_joystick_B1->read());
		jb2z = 50-(int8_t)(100*_joystick_B2->read());
		Thread::wait(REPEAT_TIMEOUT);
	}
	// initializes data
	_joystickdata.yaw = 50;	// throttle (dynamic control)
	_joystickdata.throttle = 0;
	_joystickdata.roll = 50;
	_joystickdata.pitch = 50;
	last_throttle = 0;
	// Starts execution 
	for(;;){
		// Wait pooling
		Thread::wait(REPEAT_TIMEOUT);
		// read controls applying zero corrections
		int8_t ja1, ja2, jb1, jb2;
		ja1 = ja1z + (int8_t)(100*_joystick_A1->read());
		ja2 = ja2z + (int8_t)(100*_joystick_A2->read());
		jb1 = jb1z + (int8_t)(100*_joystick_B1->read());
		jb2 = jb2z + (int8_t)(100*_joystick_B2->read());
		// set new data
		
		_joystickdata.yaw = ja1;		
		_joystickdata.roll = jb1;
		_joystickdata.pitch = jb2;
		// read dynamic control
		_joystickdata.throttle += (ja2 - last_throttle)/_throttle_rate;
		
		// publish topic update
		MsgBroker::Exception e;
		MsgBroker::publish("/joys", &_joystickdata, sizeof(Topic::JoystickData_t), &e);
		if(e != MsgBroker::NO_ERRORS){
			// TODO: add error handling ...
		}
	}	
}

//------------------------------------------------------------------------------------
//-- PROTECTED/PRIVATE FUNCTIONS -----------------------------------------------------
//------------------------------------------------------------------------------------


