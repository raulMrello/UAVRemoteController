/*
 * State.h
 *
 *  Created on: 06/06/2015
 *      Author: raulMrello
 *
 *  State define a simple StateMachine which handles different events. An event can be handled or
 *	delegated to a parent State if provided.
 *	Entering a State implies to execute "entry" action.
 *	Event handling can result in a State transition, in that case "exit" action must be executed.
 */

#ifndef STATE_H_
#define STATE_H_

#include "List.h"
#include "FunctionPointer.h"
#include <stdint.h>

//namespace hsm {

/***************************************************************************************************/	
/***** Event ***************************************************************************************/	
/***************************************************************************************************/	

/** \enum Signals
 *  \brief Signal enumeration. Users can extend them from USER_SIG value. These are reserved.
 */
enum Signals {
	INIT_SIG = 0,
	ENTRY_SIG,		        /**< evento de entrada */
	EXIT_SIG,               /**< evento de salida */
	TIMEOUT_SIG,			/**< evento de timeout */
	USER_SIG                /**< primer evento libre para el usuario */
};	
	
/** \class Event
 *  \brief Event class
 */
class Event{
 public:	 
	 /** Constructor */
	 Event(uint32_t sig){ 
		_sig = sig; 
	}
	 
	 /** Getter/Setter */
	 uint32_t getSig(){ 
		return _sig;
	}
	 void setSig(uint32_t sig){ 
		_sig = sig;
	}
	 
 private:
	 uint32_t _sig;      /**< event signal */
};


/***************************************************************************************************/	
/***** EventHandler ********************************************************************************/	
/***************************************************************************************************/	

	
/** \class EventHandler
 *  \brief EventHandler class
 */

template <typename S>	
class EventHandler {
public:	
	EventHandler(){
		_handler = new mbed::FunctionPointerArg1<S*, Event* >();
		_sig = 0;
	}
		
    void attach(uint32_t signal, S* obj, S* (S::*member)(Event*)) {
		_sig = signal;
		_handler->attach(obj, member);
    }
	
	bool match(Event* e){
		if((e->getSig() & _sig) != 0)
			return true;
		return false;
	}
	
	S* dispatch(Event* e){
		return _handler->call(e);
	}
private:
	uint32_t _sig;	

	mbed::FunctionPointerArg1<S*, Event* > *_handler;
};


/***************************************************************************************************/	
/***** State ***************************************************************************************/	
/***************************************************************************************************/	

#define HANDLED()		getTarget()
#define IGNORED()		this
#define TRAN(s)			{setTarget(s); return getTarget();}
	
	
/** \class State
 *  \brief State class
 */
class State{
 public:	 
	 /** Constructor */
	State(State * parent = (State*)0){ 
		_parent = parent; 
		_target = this;
		_handlers = new List<EventHandler<State > >();
	}
 
	/** Interface for inheritance */
	virtual State* entry()=0;
	virtual void exit()=0;

	/** Starts Initialization */
	State * init(){
		return dispatch(0);
	}
	
    /** Attach an event handler
     *	@param sig Event signal
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     */
    void attach(uint32_t signal, State* tptr, State* (State::*mptr)(Event*)) {
		EventHandler<State > *evh = new EventHandler<State >();
		evh->attach(signal, tptr, mptr);
		_handlers->addItem(evh);        
    }
	
	EventHandler<State > * getFirstHandler(){
		return (EventHandler<State > *)_handlers->getFirstItem();
	}
	
	EventHandler<State > * getNextHandler(){
		return (EventHandler<State > *)_handlers->getNextItem();
	}

    /** Dispatches an event. If no handler found then delegates to parent
     *	@param e Event
     */
	State* dispatch(Event * e){
		// If no event then is Entry trigger 
		if(!e){
			State* next = entry();
			if(next != this && next != 0){
				_target = next;
				exit();
			}
			return HANDLED();
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
				return HANDLED();
			}
		}
		// only reaches this point if no event handler found
		if(_parent){
			_target = _parent->dispatch(e);
			if(_target == _parent){
				_target = this;
			}		
		}
		return HANDLED();
	}
	
	State* getTarget(){
		return _target;
	}
	void setTarget(State* t){
		_target = t;
	}
	
	State* getParent(){
		return _parent;
	}
	
	
 private:
	List<EventHandler<State > > *_handlers;
	State* _parent;
	State* _target;
};


/***************************************************************************************************/	
/***** Hsm *****************************************************************************************/	
/***************************************************************************************************/	

class Hsm : public State {
public:
	Hsm() : State(){
		_events = new List<Event >();
		_states = new List<State >();
	}
	 
	/** Interface for inheritance */
	virtual State* entry()=0;
	virtual void exit()=0;

	void attachState(State* st){
		_states->addItem(st);
	}
	
	void raiseEvent(Event * e){
		_events->addItem(e);
	}
	
	State* dispatchEvents(){
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
		return HANDLED();
	}
	
	
 private:
	 List<State > *_states;
	List<Event > *_events;
	State* _history;
	State* _target;
	State* _state;	
};



//}  /* namespace */

#endif
