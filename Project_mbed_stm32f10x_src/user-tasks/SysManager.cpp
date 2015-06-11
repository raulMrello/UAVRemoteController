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
	// si se recibe un update del topic /keyb...
	if(strcmp(topicname, "/keyb") == 0){
		// obtiene un puntero a los datos del topic con el formato correspondiente
		Topic::KeyData_t * topic = (Topic::KeyData_t *)MsgBroker::getTopicData("/keyb");
		// chequea el tipo de topic y activa los eventos habilitados en este módulo
		if(topic && topic->isHold && topic->data.keys.key_B_Ok){
			((Hsm*)me)->raiseEvent(new Event(SysManager::evHoldB));
		}
		else if(topic && !topic->isHold && topic->data.keys.key_A_Ok){
			((Hsm*)me)->raiseEvent(new Event(SysManager::evOkA));
		}
		else if(topic && !topic->isHold && topic->data.keys.key_B_Ok){
			((Hsm*)me)->raiseEvent(new Event(SysManager::evOkB));
		}
		// marca el topic como consumido
		MsgBroker::consumed("/keyb");
		// activa la señal del thread para procesar el evento marcado
		me->getThread()->signal_set(1);
		return;
	}
	// idem en el caso de recibir un topic /joys
	if(strcmp(topicname, "/joys") == 0){
		// creo un evento tipo Gps (ver arriba)
		SysManager::JoystickEvent * ev = new SysManager::JoystickEvent(SysManager::evJoystick);
		// obtengo puntero a los datos del topic
		Topic::JoystickData_t * data = (Topic::JoystickData_t *)MsgBroker::getTopicData("/joys");
		// chequea el tipo de topic y activa los eventos habilitados en este módulo
		if(data){
			// copio los datos al evento creado
			memcpy(&ev->joysticks, data, sizeof(Topic::JoystickData_t));
			// lanzo evento a la máquina de estados
			((Hsm*)me)->raiseEvent(ev);
			// marco topic como consumido
			MsgBroker::consumed("/joys");
			// activo flag para para activar el thread
			me->getThread()->signal_set(1);
		}
		return;
	}
	// idem en el caso de recibir un topic /gps
	if(strcmp(topicname, "/gps") == 0){
		// creo un evento tipo Gps (ver arriba)
		SysManager::GpsEvent * ev = new SysManager::GpsEvent(SysManager::evGps);
		// obtengo puntero a los datos del topic
		Topic::GpsData_t * data = (Topic::GpsData_t *)MsgBroker::getTopicData("/gps");
		// chequea el tipo de topic y activa los eventos habilitados en este módulo
		if(data){
			// copio los datos al evento creado
			memcpy(&ev->gpsdata, data, sizeof(Topic::GpsData_t));
			// lanzo evento a la máquina de estados
			((Hsm*)me)->raiseEvent(ev);
			// marco topic como consumido
			MsgBroker::consumed("/gps");
			// activo flag para para activar el thread
			me->getThread()->signal_set(1);
		}
		return;
	}
	// idem en el caso de recibir un topic /ack
	if(strcmp(topicname, "/ack") == 0){
		// obtengo puntero a los datos del topic
		Topic::AckData_t * data = (Topic::AckData_t *)MsgBroker::getTopicData("/gps");
		// chequea el tipo de topic y activa los eventos habilitados en este módulo
		if(data && data->ackCode == Topic::ACK_OK){
			// lanzo evento a la máquina de estados
			((Hsm*)me)->raiseEvent(new Event(SysManager::evAck));			
		}
		else if(data && data->ackCode != Topic::ACK_OK){
			// creo un evento tipo Nack (ver arriba)
			SysManager::NackEvent * ev = new SysManager::NackEvent(SysManager::evNack);
			// copio los datos al evento creado
			memcpy(&ev->nack, data, sizeof(Topic::AckData_t));
			// lanzo evento a la máquina de estados
			((Hsm*)me)->raiseEvent(ev);
		}
		// marco topic como consumido
		MsgBroker::consumed("/ack");
		// activo flag para para activar el thread
		me->getThread()->signal_set(1);
		return;
	}
}

//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
SysManager::~SysManager() {

}

//------------------------------------------------------------------------------------
Thread * SysManager::getThread() {
	return _th;
}

//------------------------------------------------------------------------------------
void SysManager::run(){
	while(_th == 0){
		Thread::wait(100);
	}
	// attaches to topic updates 
	MsgBroker::Exception e;
	MsgBroker::attach("/gps", this, OnTopicUpdateCallback, &e);
	MsgBroker::attach("/keyb", this, OnTopicUpdateCallback, &e);
	MsgBroker::attach("/ack", this, OnTopicUpdateCallback, &e);
	MsgBroker::attach("/joys", this, OnTopicUpdateCallback, &e);
	
	ledStop(_arm_ch);
	ledStop(_loc_ch);
	ledStop(_alt_ch);
	ledStop(_rth_ch);
	beepStop();
	
	// Starts execution 
	_th->signal_clr(0xffff);
	_timeout = osWaitForever;
	this->init();
	for(;;){
		// executes state machine
		this->dispatchEvents();
		// Wait topic updates changes ... 
		osEvent oe = _th->signal_wait(0, _timeout);
		
		// if event clear signal and continue to process state machine
		if(oe.status == osEventSignal){	
			_th->signal_clr(0xFFFF);
			continue;
		}
		// if timeout, raise event and continue
		if(oe.status == osEventTimeout){	
			raiseEvent(new Event(SysManager::evTimer));
			continue;
		}
		
//		// if /keyb topic update, starts one short beep
//		if(oe.status == osEventSignal && (oe.value.signals & KEY_EV_READY) != 0){
//			_th->signal_clr(KEY_EV_READY);
//			MsgBroker::consumed("/keyb", &e);
//			beepStart(ONE_SHOT, SHORT_TIME, NO_REPEAT);
//		}
//		// if /gps topic update
//		if(oe.status == osEventSignal && (oe.value.signals & GPS_EV_READY) != 0){
//			_th->signal_clr(GPS_EV_READY);
//			MsgBroker::consumed("/gps", &e);
//			beepStart(DUAL_SHOT, SHORT_TIME, NO_REPEAT);
//		}
//		// if /stat topic update
//		if(oe.status == osEventSignal && (oe.value.signals & STAT_EV_READY) != 0){
//			_th->signal_clr(STAT_EV_READY);
//			Topic::StatusData_t * statdata = (Topic::StatusData_t *)MsgBroker::getTopicData("/stat", &e);
//			ledStop(_arm_ch);
//			ledStop(_loc_ch);
//			ledStop(_alt_ch);
//			ledStop(_rth_ch);					
//			switch(statdata->mode){
//				case Topic::MODE_DISARMED:{
//					break;
//				}
//				case Topic::MODE_MANUAL:{
//					ledStart(_arm_ch, LedFlasher::SLOW_FLASHING);	// 1s ON, 1s OFF, forever
//					break;
//				}
//				default: { /* LOC, ALT, RTH combination flags */
//					ledStart(_arm_ch, LedFlasher::ON_FOREVER);
//					if((statdata->mode & Topic::MODE_LOC) != 0 ){
//						ledStart(_loc_ch, LedFlasher::SINGLE_FAST_FLASHING);	// 500ms ON, 1s OFF, forever
//					}
//					if((statdata->mode & Topic::MODE_ALT) != 0 ){
//						ledStart(_alt_ch, LedFlasher::DUAL_FAST_FLASHING);	// 500ms ON, 500mss OFF, twice-shots forever
//					}
//					if((statdata->mode & Topic::MODE_RTH) != 0 ){
//						ledStart(_rth_ch, LedFlasher::CONTINUOUS_FAST_FLASHING);	// 500ms ON, 500ms OFF, forever
//					}
//					break;
//				}
//			}
//			MsgBroker::consumed("/stat", &e);
//			beepStart(DUAL_SHOT, SHORT_TIME, NO_REPEAT);
//		}
	}	
}

//------------------------------------------------------------------------------------
//-- PROTECTED/PRIVATE FUNCTIONS -----------------------------------------------------
//------------------------------------------------------------------------------------

void SysManager::setJoystick(Event* e){
	memcpy(&_joysticks, &((JoystickEvent*)e)->joysticks, sizeof(Topic::JoystickData_t));
}

//------------------------------------------------------------------------------------
void SysManager::setBeep(uint8_t beep_mode){
	switch(beep_mode){
		case BEEP_PENDING:
			beepStart(ONE_SHOT, LONG_TIME, NO_REPEAT);
			break;
		case BEEP_CONFIRMED:
			beepStart(DUAL_SHOT, SHORT_TIME, NO_REPEAT);
			break;
		case BEEP_KEY:
			beepStart(ONE_SHOT, SHORT_TIME, NO_REPEAT);
			break;
		case BEEP_ERROR:
			beepStart(QUAD_SHOT, SHORT_TIME, REPEAT_FOREVER);
			break;
	}
}
//------------------------------------------------------------------------------------
void SysManager::setLeds(uint8_t leds_mode, uint8_t tmp_mode){
	switch(leds_mode){
		case LEDS_PENDING:
			if(_state == stDisarmed || _state == this){
				ledStart(_arm_ch, LedFlasher::CONTINUOUS_FAST_FLASHING);
			}
			if(_state == stManual){
				ledStart(_alt_ch, LedFlasher::CONTINUOUS_FAST_FLASHING);	
			}
			else if(_state == stFollow){
				ledStart(_loc_ch, LedFlasher::CONTINUOUS_FAST_FLASHING);	
			}
			else if(_state == stRth){
				ledStart(_rth_ch, LedFlasher::CONTINUOUS_FAST_FLASHING);	
			}
			break;
		case LEDS_CONFIRMED:
			if(_state == stDisarmed || _state == this){
				ledStart(_arm_ch, LedFlasher::ON_FOREVER);
			}
			else if(_state == stManual){
				ledStart(_alt_ch, LedFlasher::ON_FOREVER);	
			}
			else if(_state == stFollow){
				ledStart(_loc_ch, LedFlasher::ON_FOREVER);	
			}
			else if(_state == stRth){
				ledStart(_rth_ch, LedFlasher::ON_FOREVER);	
			}
			break;
		case LEDS_SELECT:
			if(tmp_mode == MODE_DISARMED){
				ledStart(_arm_ch, LedFlasher::SLOW_FLASHING);	
				ledStart(_loc_ch, LedFlasher::ON_FOREVER);
				ledStart(_alt_ch, LedFlasher::ON_FOREVER);
				ledStart(_rth_ch, LedFlasher::ON_FOREVER);
			}
			else if(tmp_mode == MODE_MANUAL){
				ledStart(_alt_ch, LedFlasher::SLOW_FLASHING);	
				ledStart(_loc_ch, LedFlasher::ON_FOREVER);
				ledStart(_arm_ch, LedFlasher::ON_FOREVER);
				ledStart(_rth_ch, LedFlasher::ON_FOREVER);	
			}
			else if(tmp_mode == MODE_FOLLOW){
				ledStart(_loc_ch, LedFlasher::SLOW_FLASHING);	
				ledStart(_alt_ch, LedFlasher::ON_FOREVER);
				ledStart(_arm_ch, LedFlasher::ON_FOREVER);
				ledStart(_rth_ch, LedFlasher::ON_FOREVER);	
			}
			else if(tmp_mode == MODE_RTH){
				ledStart(_rth_ch, LedFlasher::SLOW_FLASHING);	
				ledStart(_alt_ch, LedFlasher::ON_FOREVER);
				ledStart(_arm_ch, LedFlasher::ON_FOREVER);
				ledStart(_loc_ch, LedFlasher::ON_FOREVER);	
			}
			break;
		case LEDS_ERROR:
			if(tmp_mode == Topic::ACK_MISSING_RESPONSE){
				ledStart(_rth_ch, LedFlasher::SLOW_FLASHING);	
				ledStart(_alt_ch, LedFlasher::SLOW_FLASHING);
				ledStart(_arm_ch, LedFlasher::SLOW_FLASHING);
				ledStart(_loc_ch, LedFlasher::SLOW_FLASHING);				
			}
			else if(tmp_mode == Topic::ACK_WRONG_RESPONSE){
				ledStart(_rth_ch, LedFlasher::SINGLE_FAST_FLASHING);	
				ledStart(_alt_ch, LedFlasher::SINGLE_FAST_FLASHING);
				ledStart(_arm_ch, LedFlasher::SINGLE_FAST_FLASHING);
				ledStart(_loc_ch, LedFlasher::SINGLE_FAST_FLASHING);	
			}
			else if(tmp_mode == Topic::ACK_TIMEOUT){
				ledStart(_rth_ch, LedFlasher::DUAL_FAST_FLASHING);	
				ledStart(_alt_ch, LedFlasher::DUAL_FAST_FLASHING);
				ledStart(_arm_ch, LedFlasher::DUAL_FAST_FLASHING);
				ledStart(_loc_ch, LedFlasher::DUAL_FAST_FLASHING);	
			}
			break;
	}
}
//------------------------------------------------------------------------------------
void SysManager::publish(uint8_t pub_mode){
	switch(pub_mode){
		case PUB_MODE:
			if(_state == stDisarmed){
				_mode_req.ackCode = Topic::ACK_OK;
				_mode_req.req = Topic::ACK_DISARMED;
				MsgBroker::publish("/mode", &_mode_req, sizeof(Topic::AckData_t));
			}
			else if(_state == stManual){
				_mode_req.ackCode = Topic::ACK_OK;
				_mode_req.req = Topic::ACK_MANUAL;
				MsgBroker::publish("/mode", &_mode_req, sizeof(Topic::AckData_t));
			}
			else if(_state == stFollow){
				_mode_req.ackCode = Topic::ACK_OK;
				_mode_req.req = Topic::ACK_FOLLOW;
				MsgBroker::publish("/mode", &_mode_req, sizeof(Topic::AckData_t));
			}
			else if(_state == stRth){
				_mode_req.ackCode = Topic::ACK_OK;
				_mode_req.req = Topic::ACK_RTH;
				MsgBroker::publish("/mode", &_mode_req, sizeof(Topic::AckData_t));
			}
			break;
		case PUB_RC:
			MsgBroker::publish("/joys", &_joysticks, sizeof(Topic::JoystickData_t));
			break;
		case PUB_PROFILE:
			_profile.profile = Topic::PROFILE_NONE;
			if(_joysticks.pitch > 75 && (_joysticks.roll > 35 && _joysticks.roll < 65))
				_profile.pos = Topic::POS_FRONT;
			else if((_joysticks.pitch > 50 && _joysticks.pitch < 90) && (_joysticks.roll > 50 && _joysticks.roll < 90))
				_profile.pos = Topic::POS_FRONTRIGHT;
			else if((_joysticks.pitch > 35 && _joysticks.pitch < 65) && (_joysticks.roll > 75))
				_profile.pos = Topic::POS_RIGHT;
			else if((_joysticks.pitch > 15 && _joysticks.pitch < 35) && (_joysticks.roll > 50 && _joysticks.roll < 90))
				_profile.pos = Topic::POS_RIGHTBACK;
			else if((_joysticks.pitch < 15) && (_joysticks.roll > 35 && _joysticks.roll < 65))
				_profile.pos = Topic::POS_BACK;
			else if((_joysticks.pitch > 15 && _joysticks.pitch < 35) && (_joysticks.roll > 15 && _joysticks.roll < 35))
				_profile.pos = Topic::POS_BACKLEFT;
			else if((_joysticks.pitch > 35 && _joysticks.pitch < 65) && (_joysticks.roll < 15))
				_profile.pos = Topic::POS_LEFT;
			else if((_joysticks.pitch > 65 && _joysticks.pitch < 90) && (_joysticks.roll > 15 && _joysticks.roll < 35))
				_profile.pos = Topic::POS_LEFTFRONT;
			MsgBroker::publish("/profile", &_profile, sizeof(Topic::JoystickData_t));
			break;
	}
}
