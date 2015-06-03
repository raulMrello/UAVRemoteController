/*
 * Event.h
 *
 *  Created on: 06/06/2015
 *      Author: raulMrello
 *
 *  Event defines an event basic structure class
 */

#ifndef EVENT_H_
#define EVENT_H_

#include <stdint.h>

namespace HSM {

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




}  /* namespace */

#endif
