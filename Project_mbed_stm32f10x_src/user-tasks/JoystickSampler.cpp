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
static void OnTopicUpdateCallback(void *subscriber, const char * topicname){
	JoystickSampler *me = (JoystickSampler*)subscriber;
	if(strcmp(topicname, "/alarm") == 0){
		MsgBroker::Exception e;
		Topic::AlarmData_t * alarm = (Topic::AlarmData_t*)MsgBroker::getTopicData("/alarm", &e);
		if(alarm->code == Topic::ALARM_FORCE_DISARM){
			me->notifyUpdate(JoystickSampler::DISARM_EV);
		}
		MsgBroker::consumed("/alarm", &e);
		return;
	}
}

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
void JoystickSampler::notifyUpdate(uint32_t event){
	_th->signal_set(event);
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
	_joystickdata.yaw = 50;	
	_joystickdata.throttle = 0;
	_joystickdata.roll = 50;
	_joystickdata.pitch = 50;
	last_throttle = 0;
	
	// Attaches to topic updates
	MsgBroker::Exception e;
	MsgBroker::attach("/alarm", this, OnTopicUpdateCallback, &e);
	
	_signals = DISARM_EV;
	
	// Starts execution 
	for(;;){
		// Wait incoming data ... 
		osEvent oe = _th->signal_wait_or(_signals, REPEAT_TIMEOUT);
		// if timeout...
		if(oe.status == osEventTimeout){
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
			MsgBroker::publish("/joys", &_joystickdata, sizeof(Topic::JoystickData_t), &e);
			if(e != MsgBroker::NO_ERRORS){
				// TODO: add error handling ...
			}
		}
		if(oe.status == osEventSignal && (oe.value.signals & DISARM_EV) != 0){	
			_th->signal_clr(DISARM_EV);
			_joystickdata.yaw = 50;	
			_joystickdata.throttle = 0;
			_joystickdata.roll = 50;
			_joystickdata.pitch = 50;
			last_throttle = 0;
			MsgBroker::publish("/joys", &_joystickdata, sizeof(Topic::JoystickData_t), &e);
			if(e != MsgBroker::NO_ERRORS){
				// TODO: add error handling ...
			}
		}
	}	
}

//------------------------------------------------------------------------------------
//-- PROTECTED/PRIVATE FUNCTIONS -----------------------------------------------------
//------------------------------------------------------------------------------------


