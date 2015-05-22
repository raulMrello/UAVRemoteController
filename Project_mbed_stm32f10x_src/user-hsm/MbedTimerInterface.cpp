
#include "MbedTimerInterface.h"
#include "TimedStatemachineInterface.h"


static void RtosTimer_task(const void * arg){
	MbedTimerInterface* me = (MbedTimerInterface*)arg;
	me->tsi->raiseTimeEvent(me->eventid);
	me->th->signal_set(me->signal);
}

MbedTimerInterface::MbedTimerInterface(TimedStatemachineInterface* statemachine, Thread * th, int32_t signal){
	_tmr = NULL;
	tsi = statemachine;
	eventid = NULL;
	this->th = th;
	this->signal = signal;
}

MbedTimerInterface::~MbedTimerInterface(){
	cancel();
}

/*
 * Starts the timing for a time event.
 */ 
void MbedTimerInterface::setTimer(TimedStatemachineInterface* statemachine, sc_eventid event, sc_integer time, sc_boolean isPeriodic){
	if(_tmr){
		cancel();
	}
	eventid = event;
	_tmr = new RtosTimer(&RtosTimer_task, ((isPeriodic)? osTimerPeriodic : osTimerOnce) , this);
	_tmr->start(time);
}

/*
 * Unsets the given time event.
 */
void MbedTimerInterface::unsetTimer(TimedStatemachineInterface* statemachine, sc_eventid event){
	cancel();
}

/*
 * Cancel timer service. Use this to end possible timing threads and free
 * memory resources.
 */
void MbedTimerInterface::cancel(){
	if(_tmr){
		_tmr->stop();
		delete(_tmr);
		_tmr = NULL;
	}
}
