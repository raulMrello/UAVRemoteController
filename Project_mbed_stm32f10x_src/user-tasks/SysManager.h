/*
 * SysManager.h
 *
 *  Created on: 20/4/2015
 *      Author: raulMrello
 *
 *	SysManager es un módulo activo (Tarea) que ejecuta una máquina de estados (Hsm) para
 *	controlar el funcionamiento general del control remoto del Quad.
 *	Su funcionamiento básico es el siguiente, tal y como se indica en el modelo
 * 	./models/SysManager.jpg
 *
 *		1. Controlará el pitido del buzzer la forma de activar los leds en función del 
 *		   estado de funcionamiento.
 *		2. Se subscribirá a las publicaciones del KeyDecoder, JoystickSampler, GpsReader
 *		   y VirtualReceiver para enviar publicaciones de cambios de modo (comandos RC, 
 *		   perfiles, etc...).
 *
 *	Las diferente notificaciones de modo serán:
 *	Pitidos:
 *		BEEP_PENDING: PITIDO_LARGO (2s) al enviar un cambio de modo y no haber recibido respuesta
 *		BEEP_CONFIRMED: 2_PITIDOS_CORTOS (100ms) al recibir respuesta tras un comando
 *		BEEP_ERROR: PITIDOS_CORTOS_SIN_FIN (100ms) tras una condición de error
 *		BEEP_KEY: 1_PITIDO_CORTO (100ms) tras pulsar una tecla
 *
 *	Leds:
 *		LEDS_PENDING: PARPADEO_RAPIDO (50ms/50ms) al enviar un cambio de modo y no haber recibido respuesta
 *		LEDS_CONFIRMED: FIJO al recibir respuesta tras un comando
 *		LEDS_ERROR: PARPADEO_LENTO_CORTO (200ms/1s) tras una condición de error
 *		LEDS_SELECT: PARPADEO_LENTO_CORTO_INVERSO (RESTO FIJO) (1s/200ms) 
 */

#ifndef SRC_ACTIVE_MODULES_SYSMANAGER_H_
#define SRC_ACTIVE_MODULES_SYSMANAGER_H_


//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "mbed.h"
#include "rtos.h"
#include "MsgBroker.h"
#include "BeepGenerator.h"
#include "LedFlasher.h"
#include "Topics.h"
#include "State.h"
using namespace hsm;

//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
//-- CLASS ---------------------------------------------------------------------------
//------------------------------------------------------------------------------------

class SysManager : public BeepGenerator, LedFlasher, Hsm {
public:
	
	//----------------------
	//------- EVENTS -------
	//----------------------
	
	class GpsEvent : public Event{
	public:
		GpsEvent(uint32_t sig) : Event(sig){}
		Topic::GpsData_t gpsdata;
	};

	class NackEvent : public Event{
	public:
		NackEvent(uint32_t sig, Topic::AckData_t* data) : Event(sig){
			nack.ackCode = data->ackCode;
			nack.req = data->req;
		}
		Topic::AckData_t nack;
	}; typedef NackEvent AckEvent;

	class JoystickEvent : public Event{
	public:
		JoystickEvent(uint32_t sig) : Event(sig){}
		Topic::JoystickData_t joysticks;
	};
	
	
	//-----------------------------
	//------- STATE MACHINE -------
	//-----------------------------
	
	
	class StError : public State{
	public:
		StError(State * parent = (State*)0, void * xif = 0) : State(parent, xif){
			// inserto manejadores de evento
			attach(SysManager::evHoldB, this, (State* (State::*)(Event*))&StError::onHoldB);
		}
		
		// Implementaciones entry/exit
		virtual State* entry(){
			((SysManager*)_xif)->setBeep(SysManager::BEEP_ERROR);
			((SysManager*)_xif)->setLeds(SysManager::LEDS_ERROR);
			DONE();
		}
		virtual void exit(){
		}	
		
		// Manejadores de eventos
		State* onHoldB(Event* e){
			TRAN(((SysManager*)_xif)->history);
		}
	};friend class StError;	
	
	
	class StSelect : public State{
	public:
		StSelect(State * parent = (State*)0, void * xif = 0) : State(parent, xif){
			// inserto manejadores de evento
			attach(SysManager::evOkA, this, (State* (State::*)(Event*))&StSelect::onOkA);
			attach(SysManager::evOkB, this, (State* (State::*)(Event*))&StSelect::onOkB);
			attach(SysManager::evTimer, this, (State* (State::*)(Event*))&StSelect::onTimeout);
		}
		
		// Implementaciones entry/exit
		virtual State* entry(){
			if(((SysManager*)_xif)->history ==((SysManager*)_xif)->stManual)
				tmp = SysManager::MODE_MANUAL;
			else if(((SysManager*)_xif)->history ==((SysManager*)_xif)->stFollow)
				tmp = SysManager::MODE_FOLLOW;
			else if(((SysManager*)_xif)->history ==((SysManager*)_xif)->stRth)
				tmp = SysManager::MODE_RTH;
			else
				tmp = SysManager::MODE_DISARMED;
			
			((SysManager*)_xif)->setBeep(SysManager::BEEP_KEY);
			((SysManager*)_xif)->setLeds(SysManager::LEDS_SELECT, tmp);
			((SysManager*)_xif)->startKeyTimeout(SysManager::KEY_TIMEOUT);
			DONE();
		}
		virtual void exit(){
		}	
		
		// Manejadores de eventos
		State* onOkA(Event* e){
			tmp = (tmp < SysManager::MODE_RTH)? (tmp+1) : SysManager::MODE_DISARMED;
			((SysManager*)_xif)->startKeyTimeout(SysManager::KEY_TIMEOUT);
			((SysManager*)_xif)->setBeep(SysManager::BEEP_KEY);
			((SysManager*)_xif)->setLeds(SysManager::LEDS_SELECT, tmp);
			DONE();
		}
		
		State* onOkB(Event* e){		
			((SysManager*)_xif)->stopKeyTimeout();			
			((SysManager*)_xif)->_confirmed = false;
			switch(tmp){
				default:
				case SysManager::MODE_DISARMED:
					TRAN(((SysManager*)_xif)->stDisarmed);
				case SysManager::MODE_FOLLOW:
					TRAN(((SysManager*)_xif)->stFollow);
				case SysManager::MODE_MANUAL:
					TRAN(((SysManager*)_xif)->stManual);
				case SysManager::MODE_RTH:
					TRAN(((SysManager*)_xif)->stRth);
			}			
		}
		
		State* onTimeout(Event* e){
			((SysManager*)_xif)->stopKeyTimeout();
			TRAN(((SysManager*)_xif)->history);
		}

	private:
		uint8_t tmp;
	};friend class StSelect;		
	
	
	class StDisarmed : public State{
	public:
		StDisarmed(State * parent = (State*)0, void * xif = 0) : State(parent, xif){
			// inserto manejadores de evento						
		}
		
		// Implementaciones entry/exit
		virtual State* entry(){
			((SysManager*)_xif)->history = this;
			if(!((SysManager*)_xif)->_confirmed){
				((SysManager*)_xif)->setBeep(SysManager::BEEP_PENDING);
				((SysManager*)_xif)->setLeds(SysManager::LEDS_PENDING);
				((SysManager*)_xif)->publish(SysManager::PUB_MODE);
			}
			DONE();
		}
		virtual void exit(){
		}	
	};friend class StDisarmed;		
	
	
	
	class StFollow : public State{
	public:
		StFollow(State * parent = (State*)0, void * xif = 0) : State(parent, xif){
			// inserto manejadores de evento			
			attach(SysManager::evOkA, this, (State* (State::*)(Event*))&StFollow::onOkA);
			attach(SysManager::evJoystick, this, (State* (State::*)(Event*))&StFollow::onJoystick);
		}
		
		// Implementaciones entry/exit
		virtual State* entry(){
			((SysManager*)_xif)->history = this;
			if(!((SysManager*)_xif)->_confirmed){
				((SysManager*)_xif)->setBeep(SysManager::BEEP_PENDING);
				((SysManager*)_xif)->setLeds(SysManager::LEDS_PENDING);
				((SysManager*)_xif)->publish(SysManager::PUB_MODE);
			}
			DONE();//DONE(this);
		}
		virtual void exit(){
		}	
		// Manejadores de eventos
		State* onOkA(Event* e){
			((SysManager*)_xif)->setBeep(SysManager::BEEP_KEY);
			((SysManager*)_xif)->publish(SysManager::PUB_PROFILE);
			DONE();//DONE(this);
		}
		State* onJoystick(Event* e){
			((SysManager*)_xif)->setJoystick(e);
			DONE();//DONE(this);
		}
	};friend class StFollow;		
	
	
	class StManual : public State{
	public:
		StManual(State * parent = (State*)0, void * xif = 0) : State(parent, xif){
			// inserto manejadores de evento			
			attach(SysManager::evJoystick, this, (State* (State::*)(Event*))&StManual::onJoystick);
		}
		
		// Implementaciones entry/exit
		virtual State* entry(){
			((SysManager*)_xif)->history = this;
			if(!((SysManager*)_xif)->_confirmed){
				((SysManager*)_xif)->setBeep(SysManager::BEEP_PENDING);
				((SysManager*)_xif)->setLeds(SysManager::LEDS_PENDING);
				((SysManager*)_xif)->publish(SysManager::PUB_MODE);
			}
			DONE();
		}
		virtual void exit(){
		}	
		// Manejadores de eventos
		State* onJoystick(Event* e){
			((SysManager*)_xif)->publish(SysManager::PUB_RC);
			DONE();
		}
	};friend class StManual;	
	
	
	class StRth : public State{
	public:
		StRth(State * parent = (State*)0, void * xif = 0) : State(parent, xif){
			// inserto manejadores de evento			
			attach(SysManager::evAck, this, (State* (State::*)(Event*))&StRth::onAck);			
		}
		
		// Implementaciones entry/exit
		virtual State* entry(){
			((SysManager*)_xif)->history = this;
			if(!((SysManager*)_xif)->_confirmed){
				((SysManager*)_xif)->setBeep(SysManager::BEEP_PENDING);
				((SysManager*)_xif)->setLeds(SysManager::LEDS_PENDING);
				((SysManager*)_xif)->publish(SysManager::PUB_MODE);
			}
			DONE();
		}
		virtual void exit(){
		}	
		// Manejadores de eventos
		State* onAck(Event* e){
			((SysManager*)_xif)->setBeep(SysManager::BEEP_CONFIRMED);
			((SysManager*)_xif)->setLeds(SysManager::LEDS_CONFIRMED);
			((SysManager*)_xif)->_timeout = ACK_TIMEOUT;
			DONE();
		}
	};friend class StRth;		
	
	// Implementaciones entry/exit
	virtual State* entry(){		
		_confirmed = false;
		setBeep(BEEP_KEY);
		setLeds(LEDS_OFF);		
		DONE();
	}
	
	virtual void exit(){		
	}	
		
	// Manejadores de eventos
	State* onStart(Event* e){
		_confirmed = false;
		TRAN(stDisarmed);		
	}
	State* onAck(Event* e){
		_confirmed = true;
		setBeep(BEEP_CONFIRMED);
		setLeds(LEDS_CONFIRMED);
		DONE();
	}
	State* onHoldB(Event* e){
		TRAN(stSelect);
	}
	State* onNack(Event* e){
		TRAN(stError);
	}
	
	// Estados
	StError *stError;
	StSelect *stSelect;
	StDisarmed *stDisarmed;
	StManual *stManual;
	StFollow *stFollow;
	StRth *stRth;
	State *history;
	
	// Eventos de la máquina de estados
	static const uint32_t evOkA 		= (USER_SIG << 0);
	static const uint32_t evOkB 		= (USER_SIG << 1);
	static const uint32_t evHoldB 		= (USER_SIG << 2);
	static const uint32_t evJoystick 	= (USER_SIG << 3);
	static const uint32_t evJoysHoldB 	= (USER_SIG << 4);
	static const uint32_t evAck 		= (USER_SIG << 5);
	static const uint32_t evNack 		= (USER_SIG << 6);
	static const uint32_t evTimer 		= (USER_SIG << 7);
	static const uint32_t evGps 		= (USER_SIG << 8);
	static const uint32_t evStart 		= (USER_SIG << 9);

	// Constantes
	static const uint8_t MODE_DISARMED = 0;
	static const uint8_t MODE_MANUAL = 1;
	static const uint8_t MODE_FOLLOW = 2;
	static const uint8_t MODE_RTH = 3;
	
	static const uint8_t BEEP_PENDING = 0;
	static const uint8_t BEEP_CONFIRMED = 1;
	static const uint8_t BEEP_SENT = 2;
	static const uint8_t BEEP_KEY = 3;
	static const uint8_t BEEP_ERROR = 4;
	
	static const uint8_t LEDS_PENDING = 0;
	static const uint8_t LEDS_CONFIRMED = 1;
	static const uint8_t LEDS_SELECT = 3;
	static const uint8_t LEDS_ERROR = 4;
	static const uint8_t LEDS_OFF = 5;
	
	static const uint8_t PUB_MODE = 0;
	static const uint8_t PUB_RC = 1;
	static const uint8_t PUB_PROFILE = 2;
	static const uint8_t PUB_KAS = 3;
	
	static const uint32_t ACK_TIMEOUT = 1000;
	static const uint32_t KEY_TIMEOUT = 5000;

protected:
	// Variables
	bool _confirmed;					///< Flag para indicar si un request ha sido confirmado por el quad
	Topic::AckData_t _mode_req;			///< Topic para publicar mode request del topic "/mode"
	Topic::JoystickData_t _joysticks;	///< Topic para publicar topics /rc
	Topic::ProfileData_t _profile;		///< Topic para publicar topics /profile

	
	// Interfaces
	void setBeep(uint8_t beep_mode);
	void setJoystick(Event* e);
	void setLeds(uint8_t leds_mode, uint8_t tmp_mode = 0);
	void startKeyTimeout(uint32_t milis);
	void stopKeyTimeout();
	void publish(uint8_t pub_mode);

		
	//--------------------------------
	//------- SYSMANAGER CLASS -------
	//--------------------------------
	
public:
	/** Constructor, destructor, getter and setter */
	SysManager(	osPriority prio, DigitalOut *led_arm, DigitalOut *led_loc, DigitalOut *led_alt, 
				DigitalOut *led_rth, PwmOut *buzzer) : 
				BeepGenerator(buzzer), 
				LedFlasher(4), 
				Hsm() {
					
		// creo estados
		stDisarmed = new StDisarmed(this, this);
		stManual = new StManual(this, this);
		stFollow = new StFollow(this, this);
		stRth = new StRth(this, this);
		stError = new StError(0, this);
		stSelect = new StSelect(0, this);
		// Inserto estados
		attachState(stDisarmed);
		attachState(stManual);
		attachState(stFollow);
		attachState(stRth);
		attachState(stError);
		attachState(stSelect);
		// Inserto manejadores de evento
		attach(SysManager::evAck, this, (State* (State::*)(Event*))&SysManager::onAck);
		attach(SysManager::evHoldB, this, (State* (State::*)(Event*))&SysManager::onHoldB);
		attach(SysManager::evNack, this, (State* (State::*)(Event*))&SysManager::onNack);
		attach(SysManager::evStart, this, (State* (State::*)(Event*))&SysManager::onStart);
		
		// setup led flasher channels
		_arm_ch = addLedChannel(led_arm);
		_loc_ch = addLedChannel(led_loc);
		_alt_ch = addLedChannel(led_alt);
		_rth_ch = addLedChannel(led_rth);		
		_th = 0;
		_th = new Thread(&SysManager::task, this, prio);
	}
	
	virtual ~SysManager();
	Thread *getThread();
		
	/** Task */
	static void task(void const *arg){
		SysManager *me = (SysManager*)arg;
		me->run();
	}	

protected:
	int8_t _arm_ch;				///< Canales led
	int8_t _loc_ch;
	int8_t _alt_ch;
	int8_t _rth_ch;
	Thread *_th;						///< Thread integrado
	uint32_t _timeout;					///< Timeout para control del thread
	RtosTimer *_tmr;
	void run();

private:

};


#endif /* SRC_ACTIVE_MODULES_SYSMANAGER_H_ */
