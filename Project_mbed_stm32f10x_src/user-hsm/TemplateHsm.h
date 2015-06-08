
#ifndef TEMPLATEHSM_H_
#define TEMPLATEHSM_H_

#include "State.h"

/*! \file Header of the state machine 'SysManagerHSM'.
*/
class BasicState : public Hsm {	
public:	
	class S0 : public State{
	public:
		S0(State * parent = (State*)0, void * xif = 0) : State(parent, xif){
			// inserto manejadores de evento
			attach(BasicState::ev1, this, (State* (State::*)(Event*))&S0::ev1S0);
			attach(BasicState::ev3, this, (State* (State::*)(Event*))&S0::ev3S0);		
		}
		// Implementaciones entry/exit
		virtual State* entry(){
			((BasicState*)_parent)->_data = 0;
			DONE(this);
		}
		virtual void exit(){
		}	
		// Manejadores de eventos
		State* ev1S0(Event* e){
			((BasicState*)_parent)->_data++;
			TRAN(((BasicState*)_parent)->s1);
		}
		State* ev3S0(Event* e){
			((BasicState*)_parent)->_data--;
			DONE(this);
		}

	};


	class S1 : public State{
	public:
		S1(State * parent = (State*)0, void * xif = 0) : State(parent, xif){
			// Inserto manejadores de evento
			attach(BasicState::ev2, this, (State* (State::*)(Event*))&S1::ev2S1);
			attach(BasicState::ev3, this, (State* (State::*)(Event*))&S1::ev3S1);		
		}
		// Implementaciones entry/exit
		virtual State* entry(){
			((BasicState*)_parent)->_data += 100;
			DONE(this);
		}
		virtual void exit(){
		}
		// Manejadores de evento
		State* ev2S1(Event* e){
			((BasicState*)_parent)->_data -= 100;
			 TRAN(((BasicState*)_parent)->s0);
		}
		State* ev3S1(Event* e){
			((BasicState*)_parent)->_data++;
			DONE(this);
		}

	};

	// Estados
	S0 *s0;
	S1 *s1;
	// Eventos
	static const uint32_t ev1 = (USER_SIG << 0);
	static const uint32_t ev2 = (USER_SIG << 1);
	static const uint32_t ev3 = (USER_SIG << 2);
	static const uint32_t ev4 = (USER_SIG << 3);
	// Variables
	int _data;
	
	BasicState() : Hsm(){
		// creo estados
		s0 = new S0(this);
		s1 = new S1(this);
		// Inserto estados
		attachState(s0);
		attachState(s1);
		// Inserto manejadores de evento
		attach(ev4, this, (State* (State::*)(Event*))&BasicState::ev4Sx);		
		// Establezco estado inicial
		setActiveState(this);
	}
		
	/** Interface for inheritance */
	virtual State* entry(){
		setActiveState(s0->init());
		DONE(getActiveState());
	}
	virtual void exit(){
	}

	// Manejadores de eventos
	State* ev4Sx(Event* e){
		_data =0;
		DONE(getActiveState());
	}

			
};

#endif /* TEMPLATEHSM_H_ */
