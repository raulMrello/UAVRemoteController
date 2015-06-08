
#ifndef KEYDECODERHSM_H_
#define KEYDECODERHSM_H_

#include "State.h"

/*! \file Header of the state machine 'KeyDecoderHsm'.
*/
class KeyDecoderHsm : public Hsm {	
public:
	
	class Inactive : public State{
	public:
		Inactive(State * parent = (State*)0) : State(parent){
			// inserto manejadores de evento
			attach(KeyDecoderHsm::evPressed, this, (State* (State::*)(Event*))&Inactive::onPressed);
		}
		// Implementaciones entry/exit
		virtual State* entry(){
			((KeyDecoderHsm*)_parent)->_key = readKey();
			DONE(this);
		}
		virtual void exit(){
		}	
		// Manejadores de eventos
		State* onPressed(Event* e){
			startTimer();
			TRAN(((KeyDecoderHsm*)_parent)->stPressed);
		}
	};
	
	class Pressed : public State{
	public:
		Pressed(State * parent = (State*)0) : State(parent){
			// inserto manejadores de evento
			attach(KeyDecoderHsm::evReleased, this, (State* (State::*)(Event*))&Inactive::onReleased);
			attach(KeyDecoderHsm::evTimer, this, (State* (State::*)(Event*))&Inactive::onTimeout);
		}
		// Implementaciones entry/exit
		virtual State* entry(){
			DONE(this);
		}
		virtual void exit(){
		}	
		// Manejadores de eventos
		State* onReleased(Event* e){
			stopTimer();
			publish ("/key", readKey(), false);
			TRAN(((KeyDecoderHsm*)_parent)->stInactive);
		}
		
		State* onTimeout(Event* e){
			publish ("/key", readKey(), true);
			TRAN(((KeyDecoderHsm*)_parent)->stHold);
		}
	};
	
	class Hold : public State{
	public:
		Hold(State * parent = (State*)0) : State(parent){
			// inserto manejadores de evento
			attach(KeyDecoderHsm::evReleased, this, (State* (State::*)(Event*))&Inactive::onReleased);
		}
		// Implementaciones entry/exit
		virtual State* entry(){
			DONE(this);
		}
		virtual void exit(){
		}	
		// Manejadores de eventos
		State* onReleased(Event* e){
			TRAN(((KeyDecoderHsm*)_parent)->stInactive);
		}
	};

	// Estados
	Inactive *stInactive;
	Pressed *stPressed;
	Hold *stHold;
	
	// Eventos
	static const uint32_t evPressed = (USER_SIG << 0);
	static const uint32_t evReleased = (USER_SIG << 1);
	static const uint32_t evTimer = (USER_SIG << 2);

	// Variables
	int _key;
	
	KeyDecoderHsm() : Hsm(){
		// creo estados
		stInactive = new Inactive(this);
		stPressed = new Pressed(this);
		stHold = new Hold(this);
		
		// Inserto estados
		attachState(stInactive);
		attachState(stPressed);
		attachState(stHold);
		
		// Inserto manejadores de evento
		//attach(evPressed, I, (State* (State::*)(Event*))&BasicState::ev4Sx);		
		// Establezco estado inicial
		setActiveState(this);
	}
		
	/** Interface for inheritance */
	virtual State* entry(){
		setActiveState(stInactive->init());
		DONE(getActiveState());
	}
	virtual void exit(){
	}			
};

#endif /* KEYDECODERHSM_H_ */
