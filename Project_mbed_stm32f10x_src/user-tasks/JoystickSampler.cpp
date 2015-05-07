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
	float ja1z, ja2z, jb1z, jb2z;
	float ja1last, ja2last, jb1last, jb2last;
	while(_th == 0){
		Thread::wait(100);
	}
	// get zero values
	ja1z = _joystick_A1->read();
	ja2z = _joystick_A2->read();
	jb1z = _joystick_B1->read();
	jb2z = _joystick_B2->read();
	// initializes data
	_joystickdata.valueA1 = 0;
	_joystickdata.valueA2 = 0;
	_joystickdata.valueB1 = 0;
	_joystickdata.valueB2 = 0;
	// initializes last read
	ja1last = _joystickdata.valueA1;
	ja2last = _joystickdata.valueA2;
	jb1last = _joystickdata.valueB1;
	jb2last = _joystickdata.valueB2;

	// Starts execution 
	for(;;){
		// Wait pooling
		Thread::wait(REPEAT_TIMEOUT);
		// read direct controls applying zero correction
		_joystickdata.valueA2 = _joystick_A2->read() - ja2z;
		_joystickdata.valueB1 = _joystick_B1->read() - jb1z;
		_joystickdata.valueB2 = _joystick_B2->read() - jb2z;
		// read dynamic control
		float difstep = (_joystick_A1->read() - ja1z)/10;
		_joystickdata.valueA1 += difstep;
		
		// if anything changes, publish topic update
		if(ja1last != _joystickdata.valueA1 || ja2last != _joystickdata.valueA2 ||jb1last != _joystickdata.valueB1 ||jb2last != _joystickdata.valueB2){
			MsgBroker::Exception e;
			MsgBroker::publish("/joys", &_joystickdata, sizeof(Topic::JoystickData_t), &e);
			if(e != MsgBroker::NO_ERRORS){
				// TODO: add error handling ...
			}
			// updates data
			ja1last = _joystickdata.valueA1;
			ja2last = _joystickdata.valueA2;
			jb1last = _joystickdata.valueB1;
			jb2last = _joystickdata.valueB2;			
		}
	}	
}

//------------------------------------------------------------------------------------
//-- PROTECTED/PRIVATE FUNCTIONS -----------------------------------------------------
//------------------------------------------------------------------------------------


