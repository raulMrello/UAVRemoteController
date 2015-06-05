
#ifndef TEMPLATEHSM_H_
#define TEMPLATEHSM_H_

#include "State.h"

/*! \file Header of the state machine 'SysManagerHSM'.
*/
class BasicState : public Hsm {	
public:	
	class S0 : public State{
	public:
		S0(State * parent = (State*)0) : State(parent){}
		// Implementaciones entry/exit
		virtual State* entry(){
			((BasicState*)_parent)->_data = 0;
			HANDLED();
		}
		virtual void exit(){
		}	
	};


	class S1 : public State{
	public:
		S1(State * parent = (State*)0) : State(parent){}
		// Implementaciones entry/exit
		virtual State* entry(){
			((BasicState*)_parent)->_data += 100;
			HANDLED();
		}
		virtual void exit(){
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
	State* _state;
	
	BasicState() : Hsm(){
		// creo estados
		s0 = new S0(this);
		s1 = new S1(this);
		// Inserto estados
		attachState(s0);
		attachState(s1);
		// Inserto manejadores de evento
		attach(ev1, s0, (State* (State::*)(Event*))&BasicState::ev1S0);
		attach(ev2, s1, (State* (State::*)(Event*))&BasicState::ev2S1);
		attach(ev3, s0, (State* (State::*)(Event*))&BasicState::ev3S0);
		attach(ev3, s1, (State* (State::*)(Event*))&BasicState::ev3S1);
		attach(ev4, this, (State* (State::*)(Event*))&BasicState::ev4Sx);		
	}
		
	/** Interface for inheritance */
	virtual State* entry(){
		_state = s0->init();
		HANDLED();
	}
	virtual void exit(){
	}

	// Manejadores de eventos
	State* ev1S0(Event* e){
		_data++;
		TRAN(s1);
	}
	State* ev2S1(Event* e){
		_data -= 100;
		 TRAN(s0);
	}
	State* ev3S0(Event* e){
		_data--;
		HANDLED();
	}
	State* ev3S1(Event* e){
		_data++;
		HANDLED();
	}
	State* ev4Sx(Event* e){
		_data =0;
		HANDLED();
	}
			
};

#endif /* TEMPLATEHSM_H_ */
