/*
 * EventHanlder.h
 *
 *  Created on: 06/06/2015
 *      Author: raulMrello
 *
 *  EventHandler defines an event processor.
 */

#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_



namespace HSM {
	
	
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


}  /* namespace */

#endif
