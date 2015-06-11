/*
 * State.h
 *
 *  Created on: 06/06/2015
 *      Author: raulMrello
 *
 *  State es un módulo que permite la implementación de máquinas de estados jerárquicas (HSM). Para crear
 *  una máquina de estados es necesario implementar la clase abstracta Hsm y añadir tantos estados como se
 *  deseen. Cada estado debe implementarse como una clase derivada de la clase abstracta State. Además tanto
 *  la máquina de estados como los propios estados deben activar los eventos a los que reaccionarán.
 *  Los eventos son objetos de la clase Event que además pueden especializarse creando clases derivadas para
 *  añadir más parámetros a un evento de un tipo concreto.
 *  
 *  Para que un estado pueda funcionar de forma correcta es necesario crearlo con dos parámetros fundamentales:
 *	- State *parent : estado padre que procesará los eventos que él mismo no tenga registrados.
 *  - void  *xif    : un interface que proporciona acceso a datos y métodos comunes con los que trabajar.
 *
 */

#ifndef STATE_H_
#define STATE_H_

#include "List.h"
#include "FunctionPointer.h"
#include <stdint.h>

/** Add here your porting code for Mutex-based applications */
#include "rtos.h"

namespace hsm {

#define HSM_MUTEX_OBJ			 Mutex
#define HSM_MUTEX_LOCK(m)        m.lock(osWaitForever)
#define HSM_MUTEX_UNLOCK(m)      m.unlock()
	

/***************************************************************************************************/	
/***** Event ***************************************************************************************/	
/***************************************************************************************************/	


/** \enum Signals
 *  \brief Signal enumeration. Users can extend them from USER_SIG value. These are reserved.
 */
enum Signals {
	USER_SIG=1                /**< primer evento libre para el usuario */
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

#define DONE()			return((State*)0)
#define TRAN(s)			return(s)
	
	
/** \class State
 *  \brief State class
 */
class State{
 public:	 
	 /** Constructor */
	State(State * parent = (State*)0, void * xif = 0){ 
		_parent = parent; 
		_xif = xif;
		_handlers = new List<EventHandler<State > >();
	}
 
	/** Interface for inheritance */
	virtual State* entry()=0;
	virtual void exit()=0;
	
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
		State *next, *st = this;
		// If no event then is Entry trigger 
		if(!e){
			next = entry();
			while(next && next != st){
				st->exit();
				st = next;
				next = st->entry();
			}
			return st;
		}
		// else, search an event handler
		EventHandler<State > * handler = getFirstHandler(); 
		while(handler){
			if(!handler->match(e)){
				handler = getNextHandler();
				continue;
			}
			next = handler->dispatch(e);
			return next;
		}
		// only reaches this point if no event handler found
		if(_parent){
			next = _parent->dispatch(e);
			return next;
		}
		return (State*)0;
	}

	
 protected:
	List<EventHandler<State > > *_handlers;
	State* _parent;
	void * _xif;	// External interface object
};


/***************************************************************************************************/	
/***** Hsm *****************************************************************************************/	
/***************************************************************************************************/	

class Hsm : public State {
public:
	Hsm(State* parent = 0, void * xif = 0) : State(parent, xif){
		_events = new List<Event >();
		_states = new List<State >();
		_state = this;
	}
	 
	void attachState(State* st){
		HSM_MUTEX_LOCK(_mutex);
		_states->addItem(st);
		HSM_MUTEX_UNLOCK(_mutex);
	}
	
	void raiseEvent(Event * e){
		HSM_MUTEX_LOCK(_mutex);
		_events->addItem(e);
		HSM_MUTEX_UNLOCK(_mutex);
	}	

	/** Starts Initialization */
	State * init(){
		State* next = dispatch(0);
		_state = (next)? next : this;
		return _state;
	}
	
	State* dispatchEvents(){
		// else, search an event handler
		Event* ev = (Event*) _events->getFirstItem(); 
		while(ev){
			State* next = _state->dispatch(ev);
			while(next && next != _state){
				_state->exit();
				_state = next;
				next = _state->entry();				
			}
			HSM_MUTEX_LOCK(_mutex);
			_events->removeItem(ev);
			HSM_MUTEX_UNLOCK(_mutex);
			ev = (Event*) _events->getFirstItem(); 	
		}
		return _state;
	}
	
 protected:
	List<State > *_states;
	List<Event > *_events;
 	State* 			_state;	
	HSM_MUTEX_OBJ	_mutex;
};



}  /* namespace */

#endif
