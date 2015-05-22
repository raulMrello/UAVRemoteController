
#ifndef MBEDTIMERINTERFACE_H_
#define MBEDTIMERINTERFACE_H_

#include "TimerInterface.h"
#include "mbed.h"
#include "rtos.h"

/*
 * Basic mbed interface 
 */
 class MbedTimerInterface : public TimerInterface {
	public:
		
		MbedTimerInterface(TimedStatemachineInterface* statemachine, Thread * th, int32_t signal);
		virtual ~MbedTimerInterface();
	
		/*
		 * Starts the timing for a time event.
		 */ 
		virtual void setTimer(TimedStatemachineInterface* statemachine, sc_eventid event, sc_integer time, sc_boolean isPeriodic);
		
		/*
		 * Unsets the given time event.
		 */
		virtual void unsetTimer(TimedStatemachineInterface* statemachine, sc_eventid event);
	
		/*
		 * Cancel timer service. Use this to end possible timing threads and free
		 * memory resources.
		 */
		virtual void cancel();
		
		/** Fields for callback event raising */
		TimedStatemachineInterface* tsi;
		Thread *th;
		int32_t signal;
		sc_eventid eventid;
		
	private:	
		RtosTimer *_tmr;
};

#endif /* MBEDTIMERINTERFACE_H_ */
