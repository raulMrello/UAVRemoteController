/*
 * Hsm.h
 *
 *  Created on: 06/06/2015
 *      Author: raulMrello
 *
 *  Hsm provides state machine semantics to any application.
 */

#ifndef HSM_H_
#define HSM_H_


#define HSM_RELEASE   "0.x.20150603001"

#include <stdint.h>
/** Add here your porting code for RTOS-based applications */
#include "rtos.h"


namespace HSM {

#define EnterCritical()			//tsk_lock()
#define ExitCritical()			//tsk_unlock()

/** Typedefs */	
typedef uint8_t State;
typedef void    Object;

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
 *  \brief This class provides Event definition capabilities for inheritance
 */
class Event{
 public:	 
	 /** Constructor */
	 Event(uint32_t sig){ _sig = sig; }
	 /** Getter/Setter */
	 uint32_t getSig(){ return _sig; }
	 void setSig(uint32_t sig){ _sig = sig; }
 private:
	 uint32_t _sig;      /**< event signal */
};

/** \class Hsm
 *  \brief This class provides Hierarchical State Machine semantics
 */
class Hsm {
public:
	/** Interface for inheritance */
virtual State init(Event const * e) = 0;     /**< manejador del estado Initial */
	virtual State top(Event const * e) = 0;      /**< manejador del estado Padre */
	/** Constructor */
	Hsm();
	/** Destructor */
	virtual ~Hsm();
	/** Public methods */
	void dispatch(Event const *e);
private:
	State (Hsm::*_init)(Event const *e);
	State (Hsm::*_state)(Event const *e);
	State (Hsm::*_target)(Event const *e);
	State retFromState(uint8_t retVal, State (Hsm::*state)(Event const *));	
	Event *_timeoutEvt;
	enum DispatchingFlags{
		FIRST_TIME  = 1u,
		EXIT_NOW    = 2u,
	};

};

/** Typedef for Event handling */
typedef State (Hsm::*StateHandler)(Event const *e);

/** Common transition definitions and event consuming flags */
#define RET_HANDLED       ((State)0u)     /**< indica que el evento se ha procesado */
#define RET_IGNORED       ((State)1u)     /**< indica que el evento se ha controlado pero se ignora */
#define RET_TRAN          ((State)2u)     /**< indica que hay una transición de estados */
#define RET_SUPER         ((State)3u)     /**< indica que el evento se ha delegado al estado padre */	
#define HANDLED()		return(RET_HANDLED)    
#define IGNORED()       return(RET_IGNORED)
#define TRAN(st_)       {_target = (st_);return(RET_TRAN);}
#define SUPER(st_)      {_target = (Hsm::*StateHandler)(st_);return(RET_SUPER);}
#define DELEGATE(st_)	SUPER(st_)


} /* namespace */



#endif

