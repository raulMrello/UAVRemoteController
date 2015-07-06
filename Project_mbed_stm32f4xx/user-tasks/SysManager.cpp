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

static void onKeyTimeout(void const * obj){
	SysManager *me = (SysManager*)obj;
	((Hsm*)me)->raiseEvent(new Event(SysManager::evTimer));
	// activa la señal del thread para procesar el evento marcado
	me->getThread()->signal_set(1);
}

static void OnTopicUpdateCallback(void *subscriber, const char * topicname){
	SysManager *me = (SysManager*)subscriber;
	bool raised = false;
	// si se recibe un update del topic /keyb...
	if(strcmp(topicname, "/keyb") == 0){
		// obtiene un puntero a los datos del topic con el formato correspondiente
		Topic::KeyData_t * topic = (Topic::KeyData_t *)MsgBroker::getTopicData("/keyb");
		// chequea el tipo de topic y activa los eventos habilitados en este módulo
		if(topic && topic->isHold && topic->data.keys.key_B_Ok){
			((Hsm*)me)->raiseEvent(new Event(SysManager::evHoldB));
			raised = true;
		}
		else if(topic && !topic->isHold && topic->data.keys.key_A_Ok){
			((Hsm*)me)->raiseEvent(new Event(SysManager::evOkA));
			raised = true;
		}
		else if(topic && !topic->isHold && topic->data.keys.key_B_Ok){
			((Hsm*)me)->raiseEvent(new Event(SysManager::evOkB));
			raised = true;
		}
		// marca el topic como consumido
		MsgBroker::consumed("/keyb");
		if(raised){
			// activa la señal del thread para procesar el evento marcado
			me->getThread()->signal_set(1);
		}
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
			raised = true;
		}
		// marco topic como consumido
		MsgBroker::consumed("/joys");		
		if(raised){
			// activa la señal del thread para procesar el evento marcado
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
			raised = true;			
		}
		// marco topic como consumido
		MsgBroker::consumed("/gps");
		if(raised){
			// activa la señal del thread para procesar el evento marcado
			me->getThread()->signal_set(1);
		}
		return;
	}
	// idem en el caso de recibir un topic /ack
	if(strcmp(topicname, "/ack") == 0){
		// obtengo puntero a los datos del topic
		Topic::AckData_t * data = (Topic::AckData_t *)MsgBroker::getTopicData("/ack");
		// chequea el tipo de topic y activa los eventos habilitados en este módulo
		if(data && data->ackCode == Topic::ACK_OK && data->req != Topic::ACK_START){
			// lanzo evento a la máquina de estados
			((Hsm*)me)->raiseEvent(new SysManager::AckEvent(SysManager::evAck, data));			
			raised = true;
		}
		else if(data && data->ackCode == Topic::ACK_OK && data->req == Topic::ACK_START){
			// lanzo evento a la máquina de estados
			((Hsm*)me)->raiseEvent(new Event(SysManager::evStart));			
			raised = true;
		}
		else if(data && data->ackCode != Topic::ACK_OK){
			// creo un evento tipo Nack (ver arriba)
			SysManager::NackEvent * ev = new SysManager::NackEvent(SysManager::evNack, data);
			// copio los datos al evento creado
			memcpy(&ev->nack, data, sizeof(Topic::AckData_t));
			// lanzo evento a la máquina de estados
			((Hsm*)me)->raiseEvent(ev);
			raised = true;
		}
		// marco topic como consumido
		MsgBroker::consumed("/ack");
		if(raised){
			// activa la señal del thread para procesar el evento marcado
			me->getThread()->signal_set(1);
		}
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
	_tmr = new RtosTimer(onKeyTimeout, osTimerOnce, this);
	
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
	}	
}

//------------------------------------------------------------------------------------
//-- PROTECTED/PRIVATE FUNCTIONS -----------------------------------------------------
//------------------------------------------------------------------------------------

void SysManager::setJoystick(Event* e){
	memcpy(&_navdata.rcdata, &((JoystickEvent*)e)->joysticks, sizeof(Topic::JoystickData_t));
}

//------------------------------------------------------------------------------------
void SysManager::startKeyTimeout(uint32_t millis){
	_tmr->stop();
	_tmr->start(millis);
}

//------------------------------------------------------------------------------------
void SysManager::stopKeyTimeout(){
	_tmr->stop();
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
		case LEDS_OFF:
			ledStop(_arm_ch);
			ledStop(_loc_ch);
			ledStop(_alt_ch);
			ledStop(_rth_ch);	
			break;
		case LEDS_PENDING:
			ledStop(_arm_ch);
			ledStop(_loc_ch);
			ledStop(_alt_ch);
			ledStop(_rth_ch);	
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
			ledStop(_arm_ch);
			ledStop(_loc_ch);
			ledStop(_alt_ch);
			ledStop(_rth_ch);	
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
			ledStart(_arm_ch, LedFlasher::ON_FOREVER);
			ledStart(_loc_ch, LedFlasher::ON_FOREVER);
			ledStart(_alt_ch, LedFlasher::ON_FOREVER);
			ledStart(_rth_ch, LedFlasher::ON_FOREVER);
			if(tmp_mode == MODE_DISARMED){
				ledStart(_arm_ch, LedFlasher::SLOW_FLASHING);	
			}
			else if(tmp_mode == MODE_MANUAL){
				ledStart(_alt_ch, LedFlasher::SLOW_FLASHING);	
			}
			else if(tmp_mode == MODE_FOLLOW){
				ledStart(_loc_ch, LedFlasher::SLOW_FLASHING);	
			}
			else if(tmp_mode == MODE_RTH){
				ledStart(_rth_ch, LedFlasher::SLOW_FLASHING);	
			}
			break;
		case LEDS_ERROR:
			if(tmp_mode == Topic::ACK_TIMEOUT){
				ledStart(_rth_ch, LedFlasher::DUAL_FAST_FLASHING);	
				ledStart(_alt_ch, LedFlasher::DUAL_FAST_FLASHING);
				ledStart(_arm_ch, LedFlasher::DUAL_FAST_FLASHING);
				ledStart(_loc_ch, LedFlasher::DUAL_FAST_FLASHING);	
			}
			else{
				ledStart(_rth_ch, LedFlasher::SINGLE_FAST_FLASHING);	
				ledStart(_alt_ch, LedFlasher::SINGLE_FAST_FLASHING);
				ledStart(_arm_ch, LedFlasher::SINGLE_FAST_FLASHING);
				ledStart(_loc_ch, LedFlasher::SINGLE_FAST_FLASHING);	
			}
			break;
	}
}
//------------------------------------------------------------------------------------
void SysManager::publish(uint8_t pub_mode){
	switch(pub_mode){
		case PUB_MODE:
			if(_state == stDisarmed){
				_navdata.modedata.ackCode = Topic::ACK_OK;
				_navdata.modedata.req = Topic::ACK_DISARMED;
				_navdata.modeupd = true;
			}
			else if(_state == stManual){
				_navdata.modedata.ackCode = Topic::ACK_OK;
				_navdata.modedata.req = Topic::ACK_MANUAL;
				_navdata.modeupd = true;
			}
			else if(_state == stFollow){
				_navdata.modedata.ackCode = Topic::ACK_OK;
				_navdata.modedata.req = Topic::ACK_FOLLOW;
				_navdata.modeupd = true;
			}
			else if(_state == stRth){
				_navdata.modedata.ackCode = Topic::ACK_OK;
				_navdata.modedata.req = Topic::ACK_RTH;
				_navdata.modeupd = true;
			}
			break;
			
		case PUB_RC:
			_navdata.rcdata.profile = 0;
			_navdata.rcupd = true;
			break;
		
		case PUB_PROFILE:
			_navdata.rcdata.profile = 0;
			if(_navdata.rcdata.pitch > 75 && (_navdata.rcdata.roll > 35 && _navdata.rcdata.roll < 65))
				_navdata.rcdata.profile = 60;
			else if((_navdata.rcdata.pitch > 50 && _navdata.rcdata.pitch < 90) && (_navdata.rcdata.roll > 50 && _navdata.rcdata.roll < 90))
				_navdata.rcdata.profile = 7;
			else if((_navdata.rcdata.pitch > 35 && _navdata.rcdata.pitch < 65) && (_navdata.rcdata.roll > 75))
				_navdata.rcdata.profile = 15;
			else if((_navdata.rcdata.pitch > 15 && _navdata.rcdata.pitch < 35) && (_navdata.rcdata.roll > 50 && _navdata.rcdata.roll < 90))
				_navdata.rcdata.profile = 22;
			else if((_navdata.rcdata.pitch < 15) && (_navdata.rcdata.roll > 35 && _navdata.rcdata.roll < 65))
				_navdata.rcdata.profile = 30;
			else if((_navdata.rcdata.pitch > 15 && _navdata.rcdata.pitch < 35) && (_navdata.rcdata.roll > 15 && _navdata.rcdata.roll < 35))
				_navdata.rcdata.profile = 37;
			else if((_navdata.rcdata.pitch > 35 && _navdata.rcdata.pitch < 65) && (_navdata.rcdata.roll < 15))
				_navdata.rcdata.profile = 45;
			else if((_navdata.rcdata.pitch > 65 && _navdata.rcdata.pitch < 90) && (_navdata.rcdata.roll > 15 && _navdata.rcdata.roll < 35))
				_navdata.rcdata.profile = 52;
			_navdata.rcupd = true;
			break;
			
		default:
			return;
	}
	MsgBroker::publish("/nav", &_navdata, sizeof(Topic::NavigationData_t));
	if(_logger){ _logger->print((char*)"SYS: NAV! # ", 12);}
}
