
#include "KeyDecoderHSM.h"
#include <string.h>
/*! \file Implementation of the state machine 'KeyDecoderHSM'
*/

KeyDecoderHSM::KeyDecoderHSM() {
	
	iface_OCB = null;
	
	stateConfVectorPosition = 0;
	
	timer = NULL;
}

KeyDecoderHSM::~KeyDecoderHSM() {
}

void KeyDecoderHSM::init()
{
	for (int i = 0; i < maxOrthogonalStates; ++i)
		stateConfVector[i] = KeyDecoderHSM_last_state;
	
	stateConfVectorPosition = 0;

	clearInEvents();
	clearOutEvents();
	

}

void KeyDecoderHSM::enter()
{
	/* Default enter sequence for statechart KeyDecoderHSM */
	enact_SequenceImpl();
	/* 'default' enter sequence for region main region */
	/* Default react sequence for initial entry  */
	/* 'default' enter sequence for state Inactive */
	stateConfVector[0] = main_region_Inactive;
	stateConfVectorPosition = 0;
}

void KeyDecoderHSM::exit()
{
	/* Default exit sequence for statechart KeyDecoderHSM */
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of main region) at position 0... */
	switch(stateConfVector[ 0 ]) {
		case main_region_Inactive : {
			/* Default exit sequence for state Inactive */
			stateConfVector[0] = KeyDecoderHSM_last_state;
			stateConfVectorPosition = 0;
			break;
		}
		case main_region_Active_r1_Pressed : {
			/* Default exit sequence for state Pressed */
			stateConfVector[0] = KeyDecoderHSM_last_state;
			stateConfVectorPosition = 0;
			/* Exit action for state 'Pressed'. */
			timer->unsetTimer(this, &timeEvents[0]);
			break;
		}
		case main_region_Active_r1_Hold : {
			/* Default exit sequence for state Hold */
			stateConfVector[0] = KeyDecoderHSM_last_state;
			stateConfVectorPosition = 0;
			break;
		}
		default: break;
	}
	exact_SequenceImpl();
}

void KeyDecoderHSM::runCycle() {
	
	clearOutEvents();
	
	for (stateConfVectorPosition = 0;
		stateConfVectorPosition < maxOrthogonalStates;
		stateConfVectorPosition++) {
			
		switch (stateConfVector[stateConfVectorPosition]) {
		case main_region_Inactive : {
			react_main_region_Inactive();
			break;
		}
		case main_region_Active_r1_Pressed : {
			react_main_region_Active_r1_Pressed();
			break;
		}
		case main_region_Active_r1_Hold : {
			react_main_region_Active_r1_Hold();
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents();
}

void KeyDecoderHSM::clearInEvents() {
	iface.evPressed_raised = false;
	iface.evReleased_raised = false;
	timeEvents[0] = false; 
}

void KeyDecoderHSM::clearOutEvents() {
}


void KeyDecoderHSM::setTimer(TimerInterface* timer){
	this->timer = timer;
}

TimerInterface* KeyDecoderHSM::getTimer(){
	return timer;
}

void KeyDecoderHSM::raiseTimeEvent(sc_eventid evid) {
	if ((evid >= &timeEvents) && (evid < &timeEvents + sizeof(timeEvents))) {
		*(sc_boolean*)evid = true;
	}
}

sc_boolean KeyDecoderHSM::isActive(KeyDecoderHSMStates state) {
	switch (state) {
		case main_region_Inactive : 
			return (sc_boolean) (stateConfVector[0] == main_region_Inactive
			);
		case main_region_Active : 
			return (sc_boolean) (stateConfVector[0] >= main_region_Active
				&& stateConfVector[0] <= main_region_Active_r1_Hold);
		case main_region_Active_r1_Pressed : 
			return (sc_boolean) (stateConfVector[0] == main_region_Active_r1_Pressed
			);
		case main_region_Active_r1_Hold : 
			return (sc_boolean) (stateConfVector[0] == main_region_Active_r1_Hold
			);
		default: return false;
	}
}

KeyDecoderHSM::DefaultSCI* KeyDecoderHSM::getDefaultSCI() {
	return &iface;
}

void KeyDecoderHSM::DefaultSCI::raise_evPressed() {
	evPressed_raised = true;
}

void KeyDecoderHSM::raise_evPressed() {
	iface.raise_evPressed();
}

void KeyDecoderHSM::DefaultSCI::raise_evReleased() {
	evReleased_raised = true;
}

void KeyDecoderHSM::raise_evReleased() {
	iface.raise_evReleased();
}


void KeyDecoderHSM::setDefaultSCI_OCB(DefaultSCI_OCB* operationCallback) {
	iface_OCB = operationCallback;
}

// implementations of all internal functions

/* Entry action for statechart 'KeyDecoderHSM'. */
void KeyDecoderHSM::enact_SequenceImpl() {
}

/* Exit action for state 'KeyDecoderHSM'. */
void KeyDecoderHSM::exact_SequenceImpl() {
}

/* The reactions of state Inactive. */
void KeyDecoderHSM::react_main_region_Inactive() {
	/* The reactions of state Inactive. */
	if (iface.evPressed_raised) { 
		/* Default exit sequence for state Inactive */
		stateConfVector[0] = KeyDecoderHSM_last_state;
		stateConfVectorPosition = 0;
		/* 'default' enter sequence for state Pressed */
		/* Entry action for state 'Pressed'. */
		timer->setTimer(this, &timeEvents[0], 2 * 1000, false);
		stateConfVector[0] = main_region_Active_r1_Pressed;
		stateConfVectorPosition = 0;
	} 
}

/* The reactions of state Pressed. */
void KeyDecoderHSM::react_main_region_Active_r1_Pressed() {
	/* The reactions of state Pressed. */
	if (iface.evReleased_raised) { 
		/* Default exit sequence for state Active */
		/* Default exit sequence for region r1 */
		/* Handle exit of all possible states (of r1) at position 0... */
		switch(stateConfVector[ 0 ]) {
			case main_region_Active_r1_Pressed : {
				/* Default exit sequence for state Pressed */
				stateConfVector[0] = KeyDecoderHSM_last_state;
				stateConfVectorPosition = 0;
				/* Exit action for state 'Pressed'. */
				timer->unsetTimer(this, &timeEvents[0]);
				break;
			}
			case main_region_Active_r1_Hold : {
				/* Default exit sequence for state Hold */
				stateConfVector[0] = KeyDecoderHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		iface_OCB->publish(iface_OCB->readKey(), false);
		/* 'default' enter sequence for state Inactive */
		stateConfVector[0] = main_region_Inactive;
		stateConfVectorPosition = 0;
	}  else {
		if (timeEvents[0]) { 
			/* Default exit sequence for state Pressed */
			stateConfVector[0] = KeyDecoderHSM_last_state;
			stateConfVectorPosition = 0;
			/* Exit action for state 'Pressed'. */
			timer->unsetTimer(this, &timeEvents[0]);
			/* 'default' enter sequence for state Hold */
			/* Entry action for state 'Hold'. */
			iface_OCB->publish(iface_OCB->readKey(), true);
			stateConfVector[0] = main_region_Active_r1_Hold;
			stateConfVectorPosition = 0;
		} 
	}
}

/* The reactions of state Hold. */
void KeyDecoderHSM::react_main_region_Active_r1_Hold() {
	/* The reactions of state Hold. */
	if (iface.evReleased_raised) { 
		/* Default exit sequence for state Active */
		/* Default exit sequence for region r1 */
		/* Handle exit of all possible states (of r1) at position 0... */
		switch(stateConfVector[ 0 ]) {
			case main_region_Active_r1_Pressed : {
				/* Default exit sequence for state Pressed */
				stateConfVector[0] = KeyDecoderHSM_last_state;
				stateConfVectorPosition = 0;
				/* Exit action for state 'Pressed'. */
				timer->unsetTimer(this, &timeEvents[0]);
				break;
			}
			case main_region_Active_r1_Hold : {
				/* Default exit sequence for state Hold */
				stateConfVector[0] = KeyDecoderHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		/* 'default' enter sequence for state Inactive */
		stateConfVector[0] = main_region_Inactive;
		stateConfVectorPosition = 0;
	} 
}


