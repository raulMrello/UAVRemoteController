/*
 * KeyDecoder.h
 *
 *  Created on: 20/4/2015
 *      Author: raulMrello
 *
 *  KeyDecoder es un módulo activo (contiene su propio hilo de ejecución) además de 
 *	ser una clase derivada de la clase Hsm (máquina de estados jerárquica).
 *	Su modelo se corresponde con ./models/KeyDecoder.jpg
 *	Cuando detecta una pulsación corta/larga publica un tópico relativo a la tecla que
 *	se ha pulsado y cómo ha sido la duración de la pulsación.
 *	
 */

#ifndef SRC_ACTIVE_MODULES_KEYDECODER_H_
#define SRC_ACTIVE_MODULES_KEYDECODER_H_


//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "mbed.h"
#include "rtos.h"
#include "JoystickSampler.h"
#include "MsgBroker.h"
#include "Topics.h"
#include "State.h"
#include "Logger.h"
using namespace hsm;

//------------------------------------------------------------------------------------
//-- CLASS ---------------------------------------------------------------------------
//------------------------------------------------------------------------------------

class KeyDecoder : public JoystickSampler, Hsm{
public:
	
	//----------------------
	//------- EVENTS -------
	//----------------------
	
	class KeyEvent : public Event{
	public:
		KeyEvent(uint32_t sig, uint32_t key) : Event(sig){
			_key = key;
		}
		uint32_t _key;
	};
	
	
	//-----------------------------
	//------- STATE MACHINE -------
	//-----------------------------
	
	//-------------------------------------------------------------------------
	
	class StInactive : public State{
	public:
		StInactive(State * parent = (State*)0, void * xif = 0) : State(parent, xif){
			// inserto manejadores de evento
			attach(KeyDecoder::evPressed, this, (State* (State::*)(Event*))&StInactive::onPressed);
		}
		// Implementaciones entry/exit
		virtual State* entry(){
			DONE();
		}
		virtual void exit(){
		}	
		// Manejadores de eventos
		State* onPressed(Event* e){
			((KeyDecoder*)_xif)->_pressedKey = ((KeyEvent*)e)->_key;
			((KeyDecoder*)_xif)->_timeout = KeyDecoder::HOLD_TIMEOUT;
			TRAN(((KeyDecoder*)_parent)->stPressed);
		}
	};friend class StInactive;
	
	//-------------------------------------------------------------------------
	
	class StPressed : public State{
	public:
		StPressed(State * parent = (State*)0, void * xif = 0) : State(parent, xif){
			// inserto manejadores de evento
			attach(KeyDecoder::evReleased, this, (State* (State::*)(Event*))&StPressed::onReleased);
			attach(KeyDecoder::evTimer, this, (State* (State::*)(Event*))&StPressed::onTimeout);
		}
		// Implementaciones entry/exit
		virtual State* entry(){
			DONE();
		}
		
		virtual void exit(){
		}
		
		// Manejadores de eventos
		State* onReleased(Event* e){
			((KeyDecoder*)_xif)->_timeout = osWaitForever;
			((KeyDecoder*)_parent)->publish (((KeyDecoder*)_xif)->_pressedKey, false);
			TRAN(((KeyDecoder*)_parent)->stInactive);
		}
		
		State* onTimeout(Event* e){
			((KeyDecoder*)_xif)->_timeout = osWaitForever;
			((KeyDecoder*)_xif)->publish (((KeyDecoder*)_xif)->_pressedKey, true);
			TRAN(((KeyDecoder*)_parent)->stHold);
		}
	};friend class StPressed;
	
	//-------------------------------------------------------------------------
	
	class StHold : public State{
	public:
		StHold(State * parent = (State*)0, void * xif = 0) : State(parent, xif){
			// inserto manejadores de evento
			attach(KeyDecoder::evReleased, this, (State* (State::*)(Event*))&StHold::onReleased);
		}
		// Implementaciones entry/exit
		virtual State* entry(){
			DONE();
		}
		virtual void exit(){
		}	
		// Manejadores de eventos
		State* onReleased(Event* e){
			TRAN(((KeyDecoder*)_parent)->stInactive);
		}
	};friend class StHold;
	
	//-------------------------------------------------------------------------
	
	// Implementaciones entry/exit
	virtual State* entry(){
		TRAN(stInactive);
	}
	virtual void exit(){
	}	

protected:	
	// Estados
	StInactive *stInactive;
	StPressed *stPressed;
	StHold *stHold;
	
	// Eventos de la máquina de estados
	static const uint32_t evPressed = (USER_SIG << 0);
	static const uint32_t evReleased = (USER_SIG << 1);
	static const uint32_t evTimer = (USER_SIG << 2);

	// Constantes
	static const uint32_t HOLD_TIMEOUT = 2000; //2 seconds
	
	// Variables
	int _pressedKey;
	
	// Interfaces
	void publish(uint32_t key, bool isHold);

	
	
	//--------------------------------
	//------- KEYDECODER CLASS -------
	//--------------------------------
	
public:
	/** Constructor, destructor, getter and setter */
	KeyDecoder(	osPriority prio, InterruptIn *ii_A_Ok, InterruptIn *ii_B_Ok, InterruptIn *ii_ARM, 
				InterruptIn *ii_LOC, InterruptIn *ii_ALT, InterruptIn *ii_RTH, AnalogIn *joystick_A1, 
				AnalogIn *joystick_A2, AnalogIn *joystick_B1, AnalogIn *joystick_B2, Logger * logger = 0, 
				bool enableRepeatedEvt = false) : JoystickSampler(joystick_A1, joystick_A2, joystick_B1, joystick_B2), Hsm(){
						
		// creo estados
		stInactive = new StInactive(this, this);
		stPressed = new StPressed(this, this);
		stHold = new StHold(this, this);
		// Inserto estados
		attachState(stInactive);
		attachState(stPressed);
		attachState(stHold);
		
		ii_A_Ok->fall(this, &KeyDecoder::KeyPressedISRCallback);
		ii_A_Ok->rise(this, &KeyDecoder::KeyReleasedISRCallback);
		ii_A_Ok->disable_irq();
		ii_A_Ok->mode(PullUp);
		_ii_A_Ok = ii_A_Ok;

		ii_B_Ok->fall(this, &KeyDecoder::KeyPressedISRCallback);
		ii_B_Ok->rise(this, &KeyDecoder::KeyReleasedISRCallback);
		ii_B_Ok->disable_irq();
		ii_B_Ok->mode(PullUp);
		_ii_B_Ok = ii_B_Ok;
								
		ii_ARM->fall(this, &KeyDecoder::KeyPressedISRCallback);
		ii_ARM->rise(this, &KeyDecoder::KeyReleasedISRCallback);
		ii_ARM->disable_irq();
		ii_ARM->mode(PullUp);
		_ii_ARM = ii_ARM;

		ii_LOC->fall(this, &KeyDecoder::KeyPressedISRCallback);
		ii_LOC->rise(this, &KeyDecoder::KeyReleasedISRCallback);
		ii_LOC->disable_irq();
		ii_LOC->mode(PullUp);
		_ii_LOC = ii_LOC;

		ii_ALT->fall(this, &KeyDecoder::KeyPressedISRCallback);
		ii_ALT->rise(this, &KeyDecoder::KeyReleasedISRCallback);
		ii_ALT->disable_irq();
		ii_ALT->mode(PullUp);
		_ii_ALT = ii_ALT;

		ii_RTH->fall(this, &KeyDecoder::KeyPressedISRCallback);
		ii_RTH->rise(this, &KeyDecoder::KeyReleasedISRCallback);
		ii_RTH->disable_irq();
		ii_RTH->mode(PullUp);			
		_ii_RTH = ii_RTH;
		
		_logger = logger;
		_th = 0;
		_th = new Thread(&KeyDecoder::task, this, prio);
	}
	
	virtual ~KeyDecoder();
	Thread *getThread();
	
	/** Input Interrupt callbacks */
	void KeyPressedISRCallback(void);
	void KeyReleasedISRCallback(void);
	
	/** Task */
	static void task(void const *arg){
		KeyDecoder *me = (KeyDecoder*)arg;
		me->run();
	}	
		
protected:
	Topic::KeyData_t _keydata;
	Thread *_th;
	Logger * _logger;
	InterruptIn *_ii_A_Ok;
	InterruptIn *_ii_B_Ok;
	InterruptIn *_ii_ARM;
	InterruptIn *_ii_LOC;
	InterruptIn *_ii_ALT;
	InterruptIn *_ii_RTH;
	int32_t _signals;
	uint32_t _timeout;
	uint32_t _currentKey;

	void run();
	uint32_t readKeyboard();

	/** Key event enumeration */
	typedef enum {
		KEY_EV_PRESSED 	= (1 << 0),
		KEY_EV_RELEASED = (1 << 1)
	}EventEnum;	
	
};


#endif /* SRC_ACTIVE_MODULES_KEYDECODER_H_ */
