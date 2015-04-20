/*
 * SysManager.cpp
 *
 *  Created on: 20/04/2015
 *      Author: raulMrello
 */

#include "SysManager.h"

//------------------------------------------------------------------------------------
//-- PRIVATE TYPEDEFS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//-- PRIVATE DEFINITIONS -------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//-- STATIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

static void OnTopicUpdateCallback(void *subscriber, const char * topicname){
	SysManager *me = (SysManager*)subscriber;
	if(strcmp(topicname, "/alarm") == 0){
		me->notifyUpdate(SysManager::ALARM_EV_READY);
		return;
	}
	if(strcmp(topicname, "/keyb") == 0){
		me->notifyUpdate(SysManager::KEY_EV_READY);
		return;
	}
	if(strcmp(topicname, "/gps") == 0){
		me->notifyUpdate(SysManager::GPS_EV_READY);
		return;
	}
}

//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

void SysManager::init(osPriority prio, DigitalOut *led) {
	_th = new Thread(&SysManager::task, this, prio);
	_led = led;
}

//------------------------------------------------------------------------------------
SysManager::~SysManager() {

}

//------------------------------------------------------------------------------------
Thread * SysManager::getThread() {
	return _th;
}

//------------------------------------------------------------------------------------
void SysManager::notifyUpdate(uint32_t event){
	_th->signal_set(event);
}


//------------------------------------------------------------------------------------
void SysManager::run(){
	// attaches to topic updates 
	MsgBroker::Exception e;
	MsgBroker::attach("/gps", this, OnTopicUpdateCallback, &e);
	MsgBroker::attach("/keyb", this, OnTopicUpdateCallback, &e);
	MsgBroker::attach("/alarm", this, OnTopicUpdateCallback, &e);
	_timeout = osWaitForever;
	// Starts execution 
	for(;;){
		// Wait topic updates changes ... 
		osEvent oe = _th->signal_wait((GPS_EV_READY | KEY_EV_READY | ALARM_EV_READY), _timeout);
		// if /alarm topic update
		if(oe.status == osEventSignal && (oe.value.signals & ALARM_EV_READY) != 0){	
			beepStart(QUAD_SHOT, SHORT_TIME, REPEAT_FOREVER);
		}
		// if /keyb topic update, starts one short beep
		if(oe.status == osEventSignal && (oe.value.signals & KEY_EV_READY) != 0){
			beepStart(ONE_SHOT, SHORT_TIME, NO_REPEAT);
		}
		// if /gps topic update
		if(oe.status == osEventSignal && (oe.value.signals & GPS_EV_READY) != 0){
			beepStart(DUAL_SHOT, SHORT_TIME, NO_REPEAT);
		}
	}	
}

//------------------------------------------------------------------------------------
//-- PROTECTED/PRIVATE FUNCTIONS -----------------------------------------------------
//------------------------------------------------------------------------------------

