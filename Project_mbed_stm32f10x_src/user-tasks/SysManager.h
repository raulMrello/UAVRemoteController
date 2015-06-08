/*
 * SysManager.h
 *
 *  Created on: 20/4/2015
 *      Author: raulMrello
 *
 *	SysManager is an active module (Task) which main function is system management. This
 *	agent will attach to /gps, /keyb, /alarm topic updates to carry out management operations
 *	accordingly. These are its main functions:
 *		- On /gps topic it will start a short dual beep-beep.
 *		- On /keyb topic it will start a short single beep.
 *		- On /alarm topic it will start a short burst_x5 beeps until user interaction
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
#include "State.h"

//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
//-- CLASS ---------------------------------------------------------------------------
//------------------------------------------------------------------------------------

class SysManager : public BeepGenerator, LedFlasher, Hsm {
public:
	
	class StError : public State{
	public:
		StError(State * parent = (State*)0, void * xif = 0) : State(parent, xif){
			// inserto manejadores de evento
			attach(SysManager::evHoldB, this, (State* (State::*)(Event*))&StError::onHoldB);
		}
		
		// Implementaciones entry/exit
		virtual State* entry(){
			((SysManager*)_xif)->beepError();
			((SysManager*)_xif)->ledsError();
			DONE(this);
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
			tmp = ((SysManager*)_xif)->_mode;
			((SysManager*)_xif)->beepKey();
			((SysManager*)_xif)->ledsSelect();
			DONE(this);
		}
		virtual void exit(){
		}	
		
		// Manejadores de eventos
		State* onOkA(Event* e){
			tmp = (tmp < SysManager::MODE__RTH)? (tmp+1) : SysManager::MODE_DISARMED;
			((SysManager*)_xif)->beepKey();
			((SysManager*)_xif)->ledsSelect();
			DONE(this);
		}
		
		State* onOkB(Event* e){			
			if(tmp == ((SysManager*)_xif)->_mode){
				TRAN(((SysManager*)_xif)->history);
			}
			((SysManager*)_xif)->_confirmed = false;
			switch(tmp){
				default:
				case SysManager::MODE_DISARMED:
					TRAN(((SysManager*)_xif)->stDisarmed);
				case SysManager::MODE_MANUAL:
					TRAN(((SysManager*)_xif)->stManual);
				case SysManager::MODE_FOLLOW:
					TRAN(((SysManager*)_xif)->stFollow);
				case SysManager::MODE_RTH:
					TRAN(((SysManager*)_xif)->stRth);
			}			
		}
		
		State* onTimeout(Event* e){
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
			if(!((SysManager*)_xif)->_confirmed){
				((SysManager*)_xif)->beepConfirm(0);
				((SysManager*)_xif)->ledsMode();
				((SysManager*)_xif)->publishMode();
				((SysManager*)_xif)->_timeout = ACK_TIMEOUT;				
			}
			DONE(this);
		}
		virtual void exit(){
		}	
	};friend class StDisarmed;		
	
	
	class StManual : public State{
	public:
		StManual(State * parent = (State*)0, void * xif = 0) : State(parent, xif){
			// inserto manejadores de evento			
			attach(SysManager::evJoystick, this, (State* (State::*)(Event*))&StManual::onJoystick);
		}
		
		// Implementaciones entry/exit
		virtual State* entry(){
			if(!((SysManager*)_xif)->_confirmed){
				((SysManager*)_xif)->beepConfirm(0);
				((SysManager*)_xif)->ledsMode();
				((SysManager*)_xif)->publishMode();
				((SysManager*)_xif)->_timeout = ACK_TIMEOUT;				
			}
			DONE(this);
		}
		virtual void exit(){
		}	
		// Manejadores de eventos
		State* onJoystick(Event* e){
			((SysManager*)_xif)->ledsMode(2);
			((SysManager*)_xif)->publishRc();
			((SysManager*)_xif)->_timeout = ACK_TIMEOUT;
			DONE(this);
		}
	};friend class StManual;		
	
	// Implementaciones entry/exit
	virtual State* entry(){
		DONE(this);
	}
	virtual void exit(){
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
	static const uint32_t evTimer 		= (USER_SIG << 6);

	// Constantes
	static const uint8_t MODE_DISARMED = 0;
	static const uint8_t MODE_MANUAL = 1;
	static const uint8_t MODE_FOLLOW = 2;
	static const uint8_t MODE_RTH = 3;
	
	static const uint32_t ACK_TIMEOUT = 1000;
	
	/** Constructor, destructor, getter and setter */
	SysManager(	osPriority prio, DigitalOut *led_arm, DigitalOut *led_loc, DigitalOut *led_alt, DigitalOut *led_rth, PwmOut *buzzer) : BeepGenerator(buzzer), LedFlasher(4), Hsm() {
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
		
	/** Topic updates callbacks */
	void notifyUpdate(uint32_t event);

	/** Topic updates event enumeration */
	typedef enum {
		GPS_EV_READY 	= (1 << 0),
		KEY_EV_READY 	= (1 << 1),
		ALARM_EV_READY 	= (1 << 2),
		STAT_EV_READY 	= (1 << 3),
	}EventEnum;	
	
	/** Task */
	static void task(void const *arg){
		SysManager *me = (SysManager*)arg;
		me->run();
	}	

private:
	int8_t _arm_ch;
	int8_t _loc_ch;
	int8_t _alt_ch;
	int8_t _rth_ch;
	Thread *_th;
	uint32_t _timeout;


	// Variables
	uint8_t _mode;
	bool _confirmed;

	void run();

};


#endif /* SRC_ACTIVE_MODULES_SYSMANAGER_H_ */
