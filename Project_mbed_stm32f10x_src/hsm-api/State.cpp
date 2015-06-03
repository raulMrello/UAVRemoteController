/*
 * State.cpp
 *
 *  Created on: 06/06/2015
 *      Author: raulMrello
 *
 */

#include "State.h"
using namespace HSM;


/***************************************************************************************************/	
/***** State ***************************************************************************************/	
/***************************************************************************************************/	

State::State(State * parent){ 
	_parent = parent; 
	_target = this;
	_handlers = new List<EventHandler<State > >();
}

//------------------------------------------------------------------------------------
State* State::init(){
	return dispatch(0);
}

//------------------------------------------------------------------------------------
State* State::dispatch(Event * e){
	// If no event then is Entry trigger 
	if(!e){
		State* next = entry();
		if(next != this && next != 0){
			_target = next;
			exit();
			return HANDLED;
		}
	}
	// else, search an event handler
	EventHandler<State > * handler = getFirstHandler(); 
	while(handler){
		if(!handler->match(e)){
			handler = getNextHandler();
			continue;
		}
		State* next = handler->dispatch(e);
		if(next != this && next != 0){
			_target = next;
			exit();
			return HANDLED;
		}
	}
	// only reaches this point if no event handler found
	if(_parent){
		_target = _parent->dispatch(e);
		if(_target == _parent){
			_target = this;
		}		
	}
	return HANDLED;
}


/***************************************************************************************************/	
/***** Hsm *****************************************************************************************/	
/***************************************************************************************************/	

State* Hsm::dispatchEvents(){
	// else, search an event handler
	Event* ev = (Event*) _events->getFirstItem(); 
	while(ev){
		State* next = _state->dispatch(ev);
		while(next != _state){
			_state = next;
			next = _state->dispatch(0);
			_target = next;
		}
		_events->removeItem(ev);
		ev = (Event*) _events->getFirstItem(); 	
	}
	_state = _target;
	return HANDLED;
}
