/*
 * Hsm.h
 *
 *  Created on: 03/06/2015
 *      Author: raulMrello
 */

#include "Hsm.h"
using namespace HSM;

//------------------------------------------------------------------------------------
//-- PRIVATE TYPEDEFS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//-- PRIVATE DEFINITIONS -------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//-- STATIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

Hsm::Hsm(){
	_timeoutEvt = new Event((uint32_t)TIMEOUT_SIG);
	_init = &Hsm::init;
	_state = _init;
	_target = (State (Hsm::*)(Event const *))0;
	dispatch((Event const *)new Event(0));
	}

//------------------------------------------------------------------------------------
State Hsm::retFromState(State retVal, State (Hsm::*state)(Event const *)){
    switch(retVal){
		case RET_HANDLED:
        case RET_IGNORED:
            return retVal;
        case RET_TRAN:
        case RET_SUPER:
            EnterCritical();
            _target = state;         
            ExitCritical();
            return retVal;
    }
    return retVal;
}

//------------------------------------------------------------------------------------
State Hsm::top(Event const *e) {
    IGNORED();                   /* the top state ignores all events */
}

//------------------------------------------------------------------------------------
#define IS_FIRST_TIME   (flags & FIRST_TIME)
#define MUST_CONTINUE   (!(flags & EXIT_NOW))
void Hsm::dispatch(Event const *e){
    Event *localEvt;
    State result;
	State (Hsm::*exitState)(Event const *);
	State (Hsm::*superState)(Event const *);
	uint8_t flags = FIRST_TIME;
    
    // bucle hasta que la propagación de eventos y/o transiciones termine
    while(MUST_CONTINUE){
        // la primera vez, procesa el evento que invoca al dispatcher
        if(IS_FIRST_TIME){
            result = this->_state(e);
            flags &= ~FIRST_TIME;            
        }
        // para la primera y sucesivas transicion y/o delegación de eventos...
        // verifica el resultado de la última ejecución del dispatcher        
        switch(result){
            // si el evento se ha controlado, termina abruptamente.
            case RET_HANDLED:
            case RET_IGNORED:
                return;       //flags |= EXIT_NOW;break;
            // si hay que realizar una transición de estado...                
            case RET_TRAN:               
                // primero ejecuta el EXIT del estado actual y posteriormente el ENTRY del nuevo estado.
                localEvt.sig = EXIT_SIG;
                exitState = state;
                EnterCritical();
                state = target;
                ExitCritical();
                if(exitState != init)
                    exitState(&localEvt);
                localEvt.sig = ENTRY_SIG;
                result = state(&localEvt);
                break;
            // si el evento se delega al estado padre...    
            case RET_SUPER:
                // se invoca al manejador de estados del padre, pasándole el evento delegado.
                EnterCritical();
                superState = target;
                ExitCritical();
                result = superState(e);
                break;
        }
        
    }
    
}
