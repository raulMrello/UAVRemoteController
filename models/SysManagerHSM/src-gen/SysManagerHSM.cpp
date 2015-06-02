
#include "SysManagerHSM.h"
#include <string.h>
/*! \file Implementation of the state machine 'SysManagerHSM'
*/

SysManagerHSM::SysManagerHSM() {
	
	ifaceInternalSCI_OCB = null;
	
	for (int i = 0; i < maxHistoryStates; ++i)
		historyVector[i] = SysManagerHSM_last_state;
	
	stateConfVectorPosition = 0;
	
	timer = NULL;
}

SysManagerHSM::~SysManagerHSM() {
}

void SysManagerHSM::init()
{
	for (int i = 0; i < maxOrthogonalStates; ++i)
		stateConfVector[i] = SysManagerHSM_last_state;
	
	for (int i = 0; i < maxHistoryStates; ++i)
		historyVector[i] = SysManagerHSM_last_state;
	
	stateConfVectorPosition = 0;

	clearInEvents();
	clearOutEvents();
	
	/* Default init sequence for statechart SysManagerHSM */
	iface.confirmed = false;
	ifaceInternalSCI.touch = false;
	ifaceInternalSCI.sent = true;
	ifaceInternalSCI.mode = 0;
	ifaceInternalSCI.tmp = 0;
	ifaceInternalSCI.keyTimeout = 5;
	ifaceInternalSCI.ackTimeout = 5;
	ifaceInternalSCI.keepAliveTimeout = 5;

}

void SysManagerHSM::enter()
{
	/* Default enter sequence for statechart SysManagerHSM */
	enact_SequenceImpl();
	/* 'default' enter sequence for region main region */
	/* Default react sequence for initial entry  */
	/* 'default' enter sequence for state Pending */
	/* Entry action for state 'Pending'. */
	timer->setTimer(this, &timeEvents[4], 1 * 1000, false);
	ifaceInternalSCI_OCB->beepPending();
	ifaceInternalSCI_OCB->ledsDisarmed(0);
	stateConfVector[0] = main_region_Disarmed_r1_Pending;
	stateConfVectorPosition = 0;
	historyVector[0] = stateConfVector[0];
	/* 'default' enter sequence for region r1 */
	/* Default react sequence for initial entry  */
	/* 'default' enter sequence for state Select */
	/* Entry action for state 'Select'. */
	timer->setTimer(this, &timeEvents[13], 100, false);
	stateConfVector[1] = r1_Select;
	stateConfVectorPosition = 1;
	historyVector[1] = stateConfVector[1];
}

void SysManagerHSM::exit()
{
	/* Default exit sequence for statechart SysManagerHSM */
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of main region) at position 0... */
	switch(stateConfVector[ 0 ]) {
		case main_region_SelectMode : {
			/* Default exit sequence for state SelectMode */
			stateConfVector[0] = SysManagerHSM_last_state;
			stateConfVectorPosition = 0;
			/* Exit action for state 'SelectMode'. */
			timer->unsetTimer(this, &timeEvents[0]);
			break;
		}
		case main_region_Armed : {
			/* Default exit sequence for state Armed */
			stateConfVector[0] = SysManagerHSM_last_state;
			stateConfVectorPosition = 0;
			/* Exit action for state 'Armed'. */
			timer->unsetTimer(this, &timeEvents[1]);
			break;
		}
		case main_region_Error : {
			/* Default exit sequence for state Error */
			stateConfVector[0] = SysManagerHSM_last_state;
			stateConfVectorPosition = 0;
			/* Exit action for state 'Error'. */
			timer->unsetTimer(this, &timeEvents[2]);
			break;
		}
		case main_region_Sending : {
			/* Default exit sequence for state Sending */
			stateConfVector[0] = SysManagerHSM_last_state;
			stateConfVectorPosition = 0;
			/* Exit action for state 'Sending'. */
			timer->unsetTimer(this, &timeEvents[3]);
			break;
		}
		case main_region_Disarmed_r1_Pending : {
			/* Default exit sequence for state Pending */
			stateConfVector[0] = SysManagerHSM_last_state;
			stateConfVectorPosition = 0;
			/* Exit action for state 'Pending'. */
			timer->unsetTimer(this, &timeEvents[4]);
			break;
		}
		case main_region_Disarmed_r1_Confirmed : {
			/* Default exit sequence for state Confirmed */
			stateConfVector[0] = SysManagerHSM_last_state;
			stateConfVectorPosition = 0;
			break;
		}
		default: break;
	}
	/* Default exit sequence for region r1 */
	/* Handle exit of all possible states (of r1) at position 1... */
	switch(stateConfVector[ 1 ]) {
		case r1_RTH_r1_Pending : {
			/* Default exit sequence for state Pending */
			stateConfVector[1] = SysManagerHSM_last_state;
			stateConfVectorPosition = 1;
			/* Exit action for state 'Pending'. */
			timer->unsetTimer(this, &timeEvents[6]);
			/* Exit action for state 'RTH'. */
			timer->unsetTimer(this, &timeEvents[5]);
			break;
		}
		case r1_RTH_r1_Confirmed : {
			/* Default exit sequence for state Confirmed */
			stateConfVector[1] = SysManagerHSM_last_state;
			stateConfVectorPosition = 1;
			/* Exit action for state 'RTH'. */
			timer->unsetTimer(this, &timeEvents[5]);
			break;
		}
		case r1_Follow_r1_Pending : {
			/* Default exit sequence for state Pending */
			stateConfVector[1] = SysManagerHSM_last_state;
			stateConfVectorPosition = 1;
			/* Exit action for state 'Pending'. */
			timer->unsetTimer(this, &timeEvents[8]);
			/* Exit action for state 'Follow'. */
			timer->unsetTimer(this, &timeEvents[7]);
			break;
		}
		case r1_Follow_r1_Confirmed : {
			/* Default exit sequence for state Confirmed */
			stateConfVector[1] = SysManagerHSM_last_state;
			stateConfVectorPosition = 1;
			/* Exit action for state 'Follow'. */
			timer->unsetTimer(this, &timeEvents[7]);
			break;
		}
		case r1_Idle_r1_Pending : {
			/* Default exit sequence for state Pending */
			stateConfVector[1] = SysManagerHSM_last_state;
			stateConfVectorPosition = 1;
			/* Exit action for state 'Pending'. */
			timer->unsetTimer(this, &timeEvents[10]);
			/* Exit action for state 'Idle'. */
			timer->unsetTimer(this, &timeEvents[9]);
			break;
		}
		case r1_Idle_r1_Confirmed : {
			/* Default exit sequence for state Confirmed */
			stateConfVector[1] = SysManagerHSM_last_state;
			stateConfVectorPosition = 1;
			/* Exit action for state 'Idle'. */
			timer->unsetTimer(this, &timeEvents[9]);
			break;
		}
		case r1_Manual_r1_Confirmed : {
			/* Default exit sequence for state Confirmed */
			stateConfVector[1] = SysManagerHSM_last_state;
			stateConfVectorPosition = 1;
			/* Exit action for state 'Manual'. */
			timer->unsetTimer(this, &timeEvents[11]);
			break;
		}
		case r1_Manual_r1_Pending : {
			/* Default exit sequence for state Pending */
			stateConfVector[1] = SysManagerHSM_last_state;
			stateConfVectorPosition = 1;
			/* Exit action for state 'Pending'. */
			timer->unsetTimer(this, &timeEvents[12]);
			/* Exit action for state 'Manual'. */
			timer->unsetTimer(this, &timeEvents[11]);
			break;
		}
		case r1_Select : {
			/* Default exit sequence for state Select */
			stateConfVector[1] = SysManagerHSM_last_state;
			stateConfVectorPosition = 1;
			/* Exit action for state 'Select'. */
			timer->unsetTimer(this, &timeEvents[13]);
			break;
		}
		default: break;
	}
	exact_SequenceImpl();
}

void SysManagerHSM::runCycle() {
	
	clearOutEvents();
	
	for (stateConfVectorPosition = 0;
		stateConfVectorPosition < maxOrthogonalStates;
		stateConfVectorPosition++) {
			
		switch (stateConfVector[stateConfVectorPosition]) {
		case main_region_SelectMode : {
			react_main_region_SelectMode();
			break;
		}
		case main_region_Armed : {
			react_main_region_Armed();
			break;
		}
		case main_region_Error : {
			react_main_region_Error();
			break;
		}
		case main_region_Sending : {
			react_main_region_Sending();
			break;
		}
		case main_region_Disarmed_r1_Pending : {
			react_main_region_Disarmed_r1_Pending();
			break;
		}
		case main_region_Disarmed_r1_Confirmed : {
			react_main_region_Disarmed_r1_Confirmed();
			break;
		}
		case r1_RTH_r1_Pending : {
			react_r1_RTH_r1_Pending();
			break;
		}
		case r1_RTH_r1_Confirmed : {
			react_r1_RTH_r1_Confirmed();
			break;
		}
		case r1_Follow_r1_Pending : {
			react_r1_Follow_r1_Pending();
			break;
		}
		case r1_Follow_r1_Confirmed : {
			react_r1_Follow_r1_Confirmed();
			break;
		}
		case r1_Idle_r1_Pending : {
			react_r1_Idle_r1_Pending();
			break;
		}
		case r1_Idle_r1_Confirmed : {
			react_r1_Idle_r1_Confirmed();
			break;
		}
		case r1_Manual_r1_Confirmed : {
			react_r1_Manual_r1_Confirmed();
			break;
		}
		case r1_Manual_r1_Pending : {
			react_r1_Manual_r1_Pending();
			break;
		}
		case r1_Select : {
			react_r1_Select();
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents();
}

void SysManagerHSM::clearInEvents() {
	iface.evKeyHoldOkB_raised = false;
	iface.evKeyOkA_raised = false;
	iface.evKeyOkB_raised = false;
	iface.evDisarm_raised = false;
	iface.evAck_raised = false;
	iface.evNack_raised = false;
	iface.evConfirmReq_raised = false;
	iface.evDisarmed_raised = false;
	iface.evJoysHold_raised = false;
	iface.evJoysMove_raised = false;
	ifaceInternalSCI.evInit_raised = false; 
	timeEvents[0] = false; 
	timeEvents[1] = false; 
	timeEvents[2] = false; 
	timeEvents[3] = false; 
	timeEvents[4] = false; 
	timeEvents[5] = false; 
	timeEvents[6] = false; 
	timeEvents[7] = false; 
	timeEvents[8] = false; 
	timeEvents[9] = false; 
	timeEvents[10] = false; 
	timeEvents[11] = false; 
	timeEvents[12] = false; 
	timeEvents[13] = false; 
}

void SysManagerHSM::clearOutEvents() {
}


void SysManagerHSM::setTimer(TimerInterface* timer){
	this->timer = timer;
}

TimerInterface* SysManagerHSM::getTimer(){
	return timer;
}

void SysManagerHSM::raiseTimeEvent(sc_eventid evid) {
	if ((evid >= &timeEvents) && (evid < &timeEvents + sizeof(timeEvents))) {
		*(sc_boolean*)evid = true;
	}
}

sc_boolean SysManagerHSM::isActive(SysManagerHSMStates state) {
	switch (state) {
		case main_region_SelectMode : 
			return (sc_boolean) (stateConfVector[0] == main_region_SelectMode
			);
		case main_region_Armed : 
			return (sc_boolean) (stateConfVector[0] == main_region_Armed
			);
		case main_region_Error : 
			return (sc_boolean) (stateConfVector[0] == main_region_Error
			);
		case main_region_Sending : 
			return (sc_boolean) (stateConfVector[0] == main_region_Sending
			);
		case main_region_Disarmed : 
			return (sc_boolean) (stateConfVector[0] >= main_region_Disarmed
				&& stateConfVector[0] <= main_region_Disarmed_r1_Confirmed);
		case main_region_Disarmed_r1_Pending : 
			return (sc_boolean) (stateConfVector[0] == main_region_Disarmed_r1_Pending
			);
		case main_region_Disarmed_r1_Confirmed : 
			return (sc_boolean) (stateConfVector[0] == main_region_Disarmed_r1_Confirmed
			);
		case r1_RTH : 
			return (sc_boolean) (stateConfVector[1] >= r1_RTH
				&& stateConfVector[1] <= r1_RTH_r1_Confirmed);
		case r1_RTH_r1_Pending : 
			return (sc_boolean) (stateConfVector[1] == r1_RTH_r1_Pending
			);
		case r1_RTH_r1_Confirmed : 
			return (sc_boolean) (stateConfVector[1] == r1_RTH_r1_Confirmed
			);
		case r1_Follow : 
			return (sc_boolean) (stateConfVector[1] >= r1_Follow
				&& stateConfVector[1] <= r1_Follow_r1_Confirmed);
		case r1_Follow_r1_Pending : 
			return (sc_boolean) (stateConfVector[1] == r1_Follow_r1_Pending
			);
		case r1_Follow_r1_Confirmed : 
			return (sc_boolean) (stateConfVector[1] == r1_Follow_r1_Confirmed
			);
		case r1_Idle : 
			return (sc_boolean) (stateConfVector[1] >= r1_Idle
				&& stateConfVector[1] <= r1_Idle_r1_Confirmed);
		case r1_Idle_r1_Pending : 
			return (sc_boolean) (stateConfVector[1] == r1_Idle_r1_Pending
			);
		case r1_Idle_r1_Confirmed : 
			return (sc_boolean) (stateConfVector[1] == r1_Idle_r1_Confirmed
			);
		case r1_Manual : 
			return (sc_boolean) (stateConfVector[1] >= r1_Manual
				&& stateConfVector[1] <= r1_Manual_r1_Pending);
		case r1_Manual_r1_Confirmed : 
			return (sc_boolean) (stateConfVector[1] == r1_Manual_r1_Confirmed
			);
		case r1_Manual_r1_Pending : 
			return (sc_boolean) (stateConfVector[1] == r1_Manual_r1_Pending
			);
		case r1_Select : 
			return (sc_boolean) (stateConfVector[1] == r1_Select
			);
		default: return false;
	}
}

SysManagerHSM::DefaultSCI* SysManagerHSM::getDefaultSCI() {
	return &iface;
}

void SysManagerHSM::DefaultSCI::raise_evKeyHoldOkB() {
	evKeyHoldOkB_raised = true;
}

void SysManagerHSM::raise_evKeyHoldOkB() {
	iface.raise_evKeyHoldOkB();
}

void SysManagerHSM::DefaultSCI::raise_evKeyOkA() {
	evKeyOkA_raised = true;
}

void SysManagerHSM::raise_evKeyOkA() {
	iface.raise_evKeyOkA();
}

void SysManagerHSM::DefaultSCI::raise_evKeyOkB() {
	evKeyOkB_raised = true;
}

void SysManagerHSM::raise_evKeyOkB() {
	iface.raise_evKeyOkB();
}

void SysManagerHSM::DefaultSCI::raise_evDisarm() {
	evDisarm_raised = true;
}

void SysManagerHSM::raise_evDisarm() {
	iface.raise_evDisarm();
}

void SysManagerHSM::DefaultSCI::raise_evAck() {
	evAck_raised = true;
}

void SysManagerHSM::raise_evAck() {
	iface.raise_evAck();
}

void SysManagerHSM::DefaultSCI::raise_evNack() {
	evNack_raised = true;
}

void SysManagerHSM::raise_evNack() {
	iface.raise_evNack();
}

void SysManagerHSM::DefaultSCI::raise_evConfirmReq() {
	evConfirmReq_raised = true;
}

void SysManagerHSM::raise_evConfirmReq() {
	iface.raise_evConfirmReq();
}

void SysManagerHSM::DefaultSCI::raise_evDisarmed() {
	evDisarmed_raised = true;
}

void SysManagerHSM::raise_evDisarmed() {
	iface.raise_evDisarmed();
}

void SysManagerHSM::DefaultSCI::raise_evJoysHold() {
	evJoysHold_raised = true;
}

void SysManagerHSM::raise_evJoysHold() {
	iface.raise_evJoysHold();
}

void SysManagerHSM::DefaultSCI::raise_evJoysMove() {
	evJoysMove_raised = true;
}

void SysManagerHSM::raise_evJoysMove() {
	iface.raise_evJoysMove();
}


sc_boolean SysManagerHSM::DefaultSCI::get_confirmed() {
	return confirmed;
}

sc_boolean SysManagerHSM::get_confirmed() {
	return iface.confirmed;
}

void SysManagerHSM::DefaultSCI::set_confirmed(sc_boolean value) {
	confirmed = value;
}

void SysManagerHSM::set_confirmed(sc_boolean value) {
	iface.confirmed = value;
}


void SysManagerHSM::InternalSCI::raise_evInit() {
	evInit_raised = true;
}

sc_boolean SysManagerHSM::InternalSCI::isRaised_evInit() {
	return evInit_raised;
}

sc_boolean SysManagerHSM::InternalSCI::get_touch() {
	return touch;
}

void SysManagerHSM::InternalSCI::set_touch(sc_boolean value) {
	touch = value;
}

sc_boolean SysManagerHSM::InternalSCI::get_sent() {
	return sent;
}

void SysManagerHSM::InternalSCI::set_sent(sc_boolean value) {
	sent = value;
}

sc_integer SysManagerHSM::InternalSCI::get_mode() {
	return mode;
}

void SysManagerHSM::InternalSCI::set_mode(sc_integer value) {
	mode = value;
}

sc_integer SysManagerHSM::InternalSCI::get_tmp() {
	return tmp;
}

void SysManagerHSM::InternalSCI::set_tmp(sc_integer value) {
	tmp = value;
}

sc_integer SysManagerHSM::InternalSCI::get_keyTimeout() {
	return keyTimeout;
}

void SysManagerHSM::InternalSCI::set_keyTimeout(sc_integer value) {
	keyTimeout = value;
}

sc_integer SysManagerHSM::InternalSCI::get_ackTimeout() {
	return ackTimeout;
}

void SysManagerHSM::InternalSCI::set_ackTimeout(sc_integer value) {
	ackTimeout = value;
}

sc_integer SysManagerHSM::InternalSCI::get_keepAliveTimeout() {
	return keepAliveTimeout;
}

void SysManagerHSM::InternalSCI::set_keepAliveTimeout(sc_integer value) {
	keepAliveTimeout = value;
}

void SysManagerHSM::setInternalSCI_OCB(InternalSCI_OCB* operationCallback) {
	ifaceInternalSCI_OCB = operationCallback;
}

// implementations of all internal functions

/* Entry action for statechart 'SysManagerHSM'. */
void SysManagerHSM::enact_SequenceImpl() {
}

/* Exit action for state 'SysManagerHSM'. */
void SysManagerHSM::exact_SequenceImpl() {
}

/* shallow enterSequence with history in child r1 */
void SysManagerHSM::shenseq_main_region_Disarmed_SequenceImpl() {
	/* shallow enterSequence with history in child r1 */
	/* Handle shallow history entry of r1 */
	switch(historyVector[ 0 ]) {
		case main_region_Disarmed_r1_Pending : {
			/* 'default' enter sequence for state Pending */
			/* Entry action for state 'Pending'. */
			timer->setTimer(this, &timeEvents[4], 1 * 1000, false);
			ifaceInternalSCI_OCB->beepPending();
			ifaceInternalSCI_OCB->ledsDisarmed(0);
			stateConfVector[0] = main_region_Disarmed_r1_Pending;
			stateConfVectorPosition = 0;
			historyVector[0] = stateConfVector[0];
			break;
		}
		case main_region_Disarmed_r1_Confirmed : {
			/* 'default' enter sequence for state Confirmed */
			/* Entry action for state 'Confirmed'. */
			ifaceInternalSCI_OCB->beepDisarmed();
			ifaceInternalSCI_OCB->ledsDisarmed(1);
			stateConfVector[0] = main_region_Disarmed_r1_Confirmed;
			stateConfVectorPosition = 0;
			historyVector[0] = stateConfVector[0];
			break;
		}
		default: break;
	}
}

/* shallow enterSequence with history in child r1 */
void SysManagerHSM::shenseq_SequenceImpl() {
	/* shallow enterSequence with history in child r1 */
	/* Handle shallow history entry of r1 */
	switch(historyVector[ 1 ]) {
		case r1_RTH_r1_Pending : {
			/* 'default' enter sequence for state RTH */
			/* Entry action for state 'RTH'. */
			timer->setTimer(this, &timeEvents[5], ifaceInternalSCI.keepAliveTimeout * 1000, true);
			/* 'default' enter sequence for region r1 */
			/* Default react sequence for initial entry  */
			/* The reactions of state null. */
			if (! iface.confirmed) { 
				/* 'default' enter sequence for state Pending */
				/* Entry action for state 'Pending'. */
				timer->setTimer(this, &timeEvents[6], 250, false);
				ifaceInternalSCI_OCB->beepPending();
				ifaceInternalSCI_OCB->ledsRth(0);
				stateConfVector[1] = r1_RTH_r1_Pending;
				stateConfVectorPosition = 1;
			}  else {
				/* 'default' enter sequence for state Confirmed */
				/* Entry action for state 'Confirmed'. */
				ifaceInternalSCI_OCB->beepRth();
				ifaceInternalSCI_OCB->ledsRth(1);
				stateConfVector[1] = r1_RTH_r1_Confirmed;
				stateConfVectorPosition = 1;
			}
			historyVector[1] = stateConfVector[1];
			break;
		}
		case r1_RTH_r1_Confirmed : {
			/* 'default' enter sequence for state RTH */
			/* Entry action for state 'RTH'. */
			timer->setTimer(this, &timeEvents[5], ifaceInternalSCI.keepAliveTimeout * 1000, true);
			/* 'default' enter sequence for region r1 */
			/* Default react sequence for initial entry  */
			/* The reactions of state null. */
			if (! iface.confirmed) { 
				/* 'default' enter sequence for state Pending */
				/* Entry action for state 'Pending'. */
				timer->setTimer(this, &timeEvents[6], 250, false);
				ifaceInternalSCI_OCB->beepPending();
				ifaceInternalSCI_OCB->ledsRth(0);
				stateConfVector[1] = r1_RTH_r1_Pending;
				stateConfVectorPosition = 1;
			}  else {
				/* 'default' enter sequence for state Confirmed */
				/* Entry action for state 'Confirmed'. */
				ifaceInternalSCI_OCB->beepRth();
				ifaceInternalSCI_OCB->ledsRth(1);
				stateConfVector[1] = r1_RTH_r1_Confirmed;
				stateConfVectorPosition = 1;
			}
			historyVector[1] = stateConfVector[1];
			break;
		}
		case r1_Follow_r1_Pending : {
			/* 'default' enter sequence for state Follow */
			/* Entry action for state 'Follow'. */
			timer->setTimer(this, &timeEvents[7], ifaceInternalSCI.keepAliveTimeout * 1000, true);
			/* 'default' enter sequence for region r1 */
			/* Default react sequence for initial entry  */
			/* The reactions of state null. */
			if (! iface.confirmed) { 
				/* 'default' enter sequence for state Pending */
				/* Entry action for state 'Pending'. */
				timer->setTimer(this, &timeEvents[8], 250, false);
				ifaceInternalSCI_OCB->beepPending();
				ifaceInternalSCI_OCB->ledsFollow(0);
				stateConfVector[1] = r1_Follow_r1_Pending;
				stateConfVectorPosition = 1;
			}  else {
				/* 'default' enter sequence for state Confirmed */
				/* Entry action for state 'Confirmed'. */
				ifaceInternalSCI_OCB->beepFollow();
				ifaceInternalSCI_OCB->ledsFollow(1);
				stateConfVector[1] = r1_Follow_r1_Confirmed;
				stateConfVectorPosition = 1;
			}
			historyVector[1] = stateConfVector[1];
			break;
		}
		case r1_Follow_r1_Confirmed : {
			/* 'default' enter sequence for state Follow */
			/* Entry action for state 'Follow'. */
			timer->setTimer(this, &timeEvents[7], ifaceInternalSCI.keepAliveTimeout * 1000, true);
			/* 'default' enter sequence for region r1 */
			/* Default react sequence for initial entry  */
			/* The reactions of state null. */
			if (! iface.confirmed) { 
				/* 'default' enter sequence for state Pending */
				/* Entry action for state 'Pending'. */
				timer->setTimer(this, &timeEvents[8], 250, false);
				ifaceInternalSCI_OCB->beepPending();
				ifaceInternalSCI_OCB->ledsFollow(0);
				stateConfVector[1] = r1_Follow_r1_Pending;
				stateConfVectorPosition = 1;
			}  else {
				/* 'default' enter sequence for state Confirmed */
				/* Entry action for state 'Confirmed'. */
				ifaceInternalSCI_OCB->beepFollow();
				ifaceInternalSCI_OCB->ledsFollow(1);
				stateConfVector[1] = r1_Follow_r1_Confirmed;
				stateConfVectorPosition = 1;
			}
			historyVector[1] = stateConfVector[1];
			break;
		}
		case r1_Idle_r1_Pending : {
			/* 'default' enter sequence for state Idle */
			/* Entry action for state 'Idle'. */
			timer->setTimer(this, &timeEvents[9], ifaceInternalSCI.keepAliveTimeout * 1000, true);
			/* 'default' enter sequence for region r1 */
			/* Default react sequence for initial entry  */
			/* The reactions of state null. */
			if (! iface.confirmed) { 
				/* 'default' enter sequence for state Pending */
				/* Entry action for state 'Pending'. */
				timer->setTimer(this, &timeEvents[10], 250, false);
				ifaceInternalSCI_OCB->beepPending();
				ifaceInternalSCI_OCB->ledsArmed(0);
				stateConfVector[1] = r1_Idle_r1_Pending;
				stateConfVectorPosition = 1;
			}  else {
				/* 'default' enter sequence for state Confirmed */
				/* Entry action for state 'Confirmed'. */
				ifaceInternalSCI_OCB->beepArmed();
				ifaceInternalSCI_OCB->ledsArmed(1);
				stateConfVector[1] = r1_Idle_r1_Confirmed;
				stateConfVectorPosition = 1;
			}
			historyVector[1] = stateConfVector[1];
			break;
		}
		case r1_Idle_r1_Confirmed : {
			/* 'default' enter sequence for state Idle */
			/* Entry action for state 'Idle'. */
			timer->setTimer(this, &timeEvents[9], ifaceInternalSCI.keepAliveTimeout * 1000, true);
			/* 'default' enter sequence for region r1 */
			/* Default react sequence for initial entry  */
			/* The reactions of state null. */
			if (! iface.confirmed) { 
				/* 'default' enter sequence for state Pending */
				/* Entry action for state 'Pending'. */
				timer->setTimer(this, &timeEvents[10], 250, false);
				ifaceInternalSCI_OCB->beepPending();
				ifaceInternalSCI_OCB->ledsArmed(0);
				stateConfVector[1] = r1_Idle_r1_Pending;
				stateConfVectorPosition = 1;
			}  else {
				/* 'default' enter sequence for state Confirmed */
				/* Entry action for state 'Confirmed'. */
				ifaceInternalSCI_OCB->beepArmed();
				ifaceInternalSCI_OCB->ledsArmed(1);
				stateConfVector[1] = r1_Idle_r1_Confirmed;
				stateConfVectorPosition = 1;
			}
			historyVector[1] = stateConfVector[1];
			break;
		}
		case r1_Manual_r1_Confirmed : {
			/* 'default' enter sequence for state Manual */
			/* Entry action for state 'Manual'. */
			timer->setTimer(this, &timeEvents[11], ifaceInternalSCI.keepAliveTimeout * 1000, true);
			/* 'default' enter sequence for region r1 */
			/* Default react sequence for initial entry  */
			/* The reactions of state null. */
			if (! iface.confirmed) { 
				/* 'default' enter sequence for state Pending */
				/* Entry action for state 'Pending'. */
				timer->setTimer(this, &timeEvents[12], 250, false);
				ifaceInternalSCI_OCB->beepPending();
				ifaceInternalSCI_OCB->ledsManual(0);
				stateConfVector[1] = r1_Manual_r1_Pending;
				stateConfVectorPosition = 1;
			}  else {
				/* 'default' enter sequence for state Confirmed */
				/* Entry action for state 'Confirmed'. */
				ifaceInternalSCI_OCB->beepManual();
				ifaceInternalSCI_OCB->ledsManual(1);
				stateConfVector[1] = r1_Manual_r1_Confirmed;
				stateConfVectorPosition = 1;
			}
			historyVector[1] = stateConfVector[1];
			break;
		}
		case r1_Manual_r1_Pending : {
			/* 'default' enter sequence for state Manual */
			/* Entry action for state 'Manual'. */
			timer->setTimer(this, &timeEvents[11], ifaceInternalSCI.keepAliveTimeout * 1000, true);
			/* 'default' enter sequence for region r1 */
			/* Default react sequence for initial entry  */
			/* The reactions of state null. */
			if (! iface.confirmed) { 
				/* 'default' enter sequence for state Pending */
				/* Entry action for state 'Pending'. */
				timer->setTimer(this, &timeEvents[12], 250, false);
				ifaceInternalSCI_OCB->beepPending();
				ifaceInternalSCI_OCB->ledsManual(0);
				stateConfVector[1] = r1_Manual_r1_Pending;
				stateConfVectorPosition = 1;
			}  else {
				/* 'default' enter sequence for state Confirmed */
				/* Entry action for state 'Confirmed'. */
				ifaceInternalSCI_OCB->beepManual();
				ifaceInternalSCI_OCB->ledsManual(1);
				stateConfVector[1] = r1_Manual_r1_Confirmed;
				stateConfVectorPosition = 1;
			}
			historyVector[1] = stateConfVector[1];
			break;
		}
		case r1_Select : {
			/* 'default' enter sequence for state Select */
			/* Entry action for state 'Select'. */
			timer->setTimer(this, &timeEvents[13], 100, false);
			stateConfVector[1] = r1_Select;
			stateConfVectorPosition = 1;
			historyVector[1] = stateConfVector[1];
			break;
		}
		default: break;
	}
}

/* The reactions of state SelectMode. */
void SysManagerHSM::react_main_region_SelectMode() {
	/* The reactions of state SelectMode. */
	if (iface.evKeyOkB_raised) { 
		/* Default exit sequence for state SelectMode */
		stateConfVector[0] = SysManagerHSM_last_state;
		stateConfVectorPosition = 0;
		/* Exit action for state 'SelectMode'. */
		timer->unsetTimer(this, &timeEvents[0]);
		iface.confirmed = false;
		ifaceInternalSCI.mode = ifaceInternalSCI.tmp;
		/* The reactions of state null. */
		if (ifaceInternalSCI.mode > 0) { 
			ifaceInternalSCI.sent = false;
			/* 'default' enter sequence for state Armed */
			/* Entry action for state 'Armed'. */
			timer->setTimer(this, &timeEvents[1], ifaceInternalSCI.keepAliveTimeout * 1000, true);
			stateConfVector[0] = main_region_Armed;
			stateConfVectorPosition = 0;
		}  else {
			/* 'default' enter sequence for state Pending */
			/* Entry action for state 'Pending'. */
			timer->setTimer(this, &timeEvents[4], 1 * 1000, false);
			ifaceInternalSCI_OCB->beepPending();
			ifaceInternalSCI_OCB->ledsDisarmed(0);
			stateConfVector[0] = main_region_Disarmed_r1_Pending;
			stateConfVectorPosition = 0;
			historyVector[0] = stateConfVector[0];
		}
	}  else {
		if (timeEvents[0]) { 
			/* Default exit sequence for state SelectMode */
			stateConfVector[0] = SysManagerHSM_last_state;
			stateConfVectorPosition = 0;
			/* Exit action for state 'SelectMode'. */
			timer->unsetTimer(this, &timeEvents[0]);
			/* The reactions of state null. */
			if (! ifaceInternalSCI.touch && ifaceInternalSCI.mode == 0) { 
				/* Default react sequence for shallow history entry  */
				/* Enter the region with shallow history */
				if (historyVector[0] != SysManagerHSM_last_state) {
					shenseq_main_region_Disarmed_SequenceImpl();
				} 
			}  else {
				/* The reactions of state null. */
				if (ifaceInternalSCI.touch) { 
					ifaceInternalSCI.touch = false;
					/* 'default' enter sequence for state SelectMode */
					/* Entry action for state 'SelectMode'. */
					timer->setTimer(this, &timeEvents[0], ifaceInternalSCI.keyTimeout * 1000, true);
					ifaceInternalSCI_OCB->ledsSelect();
					ifaceInternalSCI_OCB->beepKey();
					stateConfVector[0] = main_region_SelectMode;
					stateConfVectorPosition = 0;
				}  else {
					/* Default react sequence for shallow history entry ArmedHistory */
					/* Enter the region with shallow history */
					if (historyVector[1] != SysManagerHSM_last_state) {
						shenseq_SequenceImpl();
					} 
				}
			}
		}  else {
			if (iface.evKeyOkA_raised) { 
				ifaceInternalSCI.tmp = (ifaceInternalSCI.tmp < 4) ? (ifaceInternalSCI.tmp + 1) : 0;
				ifaceInternalSCI_OCB->ledsSelect();
				ifaceInternalSCI_OCB->beepKey();
				ifaceInternalSCI.touch = true;
			} 
		}
	}
}

/* The reactions of state Armed. */
void SysManagerHSM::react_main_region_Armed() {
	/* The reactions of state Armed. */
	if (iface.evKeyHoldOkB_raised) { 
		/* Default exit sequence for state Armed */
		stateConfVector[0] = SysManagerHSM_last_state;
		stateConfVectorPosition = 0;
		/* Exit action for state 'Armed'. */
		timer->unsetTimer(this, &timeEvents[1]);
		ifaceInternalSCI.tmp = ifaceInternalSCI.mode;
		ifaceInternalSCI.touch = false;
		/* 'default' enter sequence for state SelectMode */
		/* Entry action for state 'SelectMode'. */
		timer->setTimer(this, &timeEvents[0], ifaceInternalSCI.keyTimeout * 1000, true);
		ifaceInternalSCI_OCB->ledsSelect();
		ifaceInternalSCI_OCB->beepKey();
		stateConfVector[0] = main_region_SelectMode;
		stateConfVectorPosition = 0;
	}  else {
		if (iface.evDisarm_raised) { 
			/* Default exit sequence for state Armed */
			stateConfVector[0] = SysManagerHSM_last_state;
			stateConfVectorPosition = 0;
			/* Exit action for state 'Armed'. */
			timer->unsetTimer(this, &timeEvents[1]);
			iface.confirmed = false;
			/* 'default' enter sequence for state Disarmed */
			/* 'default' enter sequence for region r1 */
			/* Default react sequence for shallow history entry  */
			/* Enter the region with shallow history */
			if (historyVector[0] != SysManagerHSM_last_state) {
				shenseq_main_region_Disarmed_SequenceImpl();
			} 
		}  else {
			if (iface.evDisarmed_raised) { 
				/* Default exit sequence for state Armed */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				/* Exit action for state 'Armed'. */
				timer->unsetTimer(this, &timeEvents[1]);
				ifaceInternalSCI.mode = 0;
				iface.confirmed = true;
				/* 'default' enter sequence for state Confirmed */
				/* Entry action for state 'Confirmed'. */
				ifaceInternalSCI_OCB->beepDisarmed();
				ifaceInternalSCI_OCB->ledsDisarmed(1);
				stateConfVector[0] = main_region_Disarmed_r1_Confirmed;
				stateConfVectorPosition = 0;
				historyVector[0] = stateConfVector[0];
			}  else {
				if (timeEvents[1]) { 
					/* Default exit sequence for state Armed */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					/* Exit action for state 'Armed'. */
					timer->unsetTimer(this, &timeEvents[1]);
					/* The reactions of state null. */
					if (! ifaceInternalSCI.sent) { 
						ifaceInternalSCI_OCB->publishKAT();
						/* 'default' enter sequence for state Sending */
						/* Entry action for state 'Sending'. */
						timer->setTimer(this, &timeEvents[3], ifaceInternalSCI.ackTimeout * 1000, false);
						stateConfVector[0] = main_region_Sending;
						stateConfVectorPosition = 0;
					}  else {
						ifaceInternalSCI.sent = false;
						/* Default react sequence for shallow history entry ArmedHistory */
						/* Enter the region with shallow history */
						if (historyVector[1] != SysManagerHSM_last_state) {
							shenseq_SequenceImpl();
						} 
					}
				} 
			}
		}
	}
}

/* The reactions of state Error. */
void SysManagerHSM::react_main_region_Error() {
	/* The reactions of state Error. */
	if (timeEvents[2]) { 
		/* Default exit sequence for state Error */
		stateConfVector[0] = SysManagerHSM_last_state;
		stateConfVectorPosition = 0;
		/* Exit action for state 'Error'. */
		timer->unsetTimer(this, &timeEvents[2]);
		/* The reactions of state null. */
		if (ifaceInternalSCI.mode == 0) { 
			/* Default react sequence for shallow history entry  */
			/* Enter the region with shallow history */
			if (historyVector[0] != SysManagerHSM_last_state) {
				shenseq_main_region_Disarmed_SequenceImpl();
			} 
		}  else {
			/* Default react sequence for shallow history entry ArmedHistory */
			/* Enter the region with shallow history */
			if (historyVector[1] != SysManagerHSM_last_state) {
				shenseq_SequenceImpl();
			} 
		}
	} 
}

/* The reactions of state Sending. */
void SysManagerHSM::react_main_region_Sending() {
	/* The reactions of state Sending. */
	if (timeEvents[3] || iface.evNack_raised) { 
		/* Default exit sequence for state Sending */
		stateConfVector[0] = SysManagerHSM_last_state;
		stateConfVectorPosition = 0;
		/* Exit action for state 'Sending'. */
		timer->unsetTimer(this, &timeEvents[3]);
		/* 'default' enter sequence for state Error */
		/* Entry action for state 'Error'. */
		timer->setTimer(this, &timeEvents[2], 2 * 1000, false);
		ifaceInternalSCI_OCB->beepError();
		ifaceInternalSCI_OCB->ledsError();
		stateConfVector[0] = main_region_Error;
		stateConfVectorPosition = 0;
	}  else {
		if (iface.evAck_raised) { 
			/* Default exit sequence for state Sending */
			stateConfVector[0] = SysManagerHSM_last_state;
			stateConfVectorPosition = 0;
			/* Exit action for state 'Sending'. */
			timer->unsetTimer(this, &timeEvents[3]);
			iface.confirmed = true;
			ifaceInternalSCI_OCB->beepAck();
			/* The reactions of state null. */
			if (ifaceInternalSCI.mode == 0) { 
				/* Default react sequence for shallow history entry  */
				/* Enter the region with shallow history */
				if (historyVector[0] != SysManagerHSM_last_state) {
					shenseq_main_region_Disarmed_SequenceImpl();
				} 
			}  else {
				/* Default react sequence for shallow history entry ArmedHistory */
				/* Enter the region with shallow history */
				if (historyVector[1] != SysManagerHSM_last_state) {
					shenseq_SequenceImpl();
				} 
			}
		} 
	}
}

/* The reactions of state Pending. */
void SysManagerHSM::react_main_region_Disarmed_r1_Pending() {
	/* The reactions of state Pending. */
	if (iface.evKeyHoldOkB_raised) { 
		/* Default exit sequence for state Disarmed */
		/* Default exit sequence for region r1 */
		/* Handle exit of all possible states (of r1) at position 0... */
		switch(stateConfVector[ 0 ]) {
			case main_region_Disarmed_r1_Pending : {
				/* Default exit sequence for state Pending */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				/* Exit action for state 'Pending'. */
				timer->unsetTimer(this, &timeEvents[4]);
				break;
			}
			case main_region_Disarmed_r1_Confirmed : {
				/* Default exit sequence for state Confirmed */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		ifaceInternalSCI.tmp = ifaceInternalSCI.mode;
		ifaceInternalSCI.touch = false;
		/* 'default' enter sequence for state SelectMode */
		/* Entry action for state 'SelectMode'. */
		timer->setTimer(this, &timeEvents[0], ifaceInternalSCI.keyTimeout * 1000, true);
		ifaceInternalSCI_OCB->ledsSelect();
		ifaceInternalSCI_OCB->beepKey();
		stateConfVector[0] = main_region_SelectMode;
		stateConfVectorPosition = 0;
	}  else {
		if (timeEvents[4]) { 
			/* Default exit sequence for state Pending */
			stateConfVector[0] = SysManagerHSM_last_state;
			stateConfVectorPosition = 0;
			/* Exit action for state 'Pending'. */
			timer->unsetTimer(this, &timeEvents[4]);
			/* The reactions of state null. */
			if (! iface.confirmed) { 
				/* Default exit sequence for state Disarmed */
				/* Default exit sequence for region r1 */
				/* Handle exit of all possible states (of r1) at position 0... */
				switch(stateConfVector[ 0 ]) {
					case main_region_Disarmed_r1_Pending : {
						/* Default exit sequence for state Pending */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						/* Exit action for state 'Pending'. */
						timer->unsetTimer(this, &timeEvents[4]);
						break;
					}
					case main_region_Disarmed_r1_Confirmed : {
						/* Default exit sequence for state Confirmed */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						break;
					}
					default: break;
				}
				ifaceInternalSCI_OCB->publishMode();
				/* 'default' enter sequence for state Sending */
				/* Entry action for state 'Sending'. */
				timer->setTimer(this, &timeEvents[3], ifaceInternalSCI.ackTimeout * 1000, false);
				stateConfVector[0] = main_region_Sending;
				stateConfVectorPosition = 0;
			}  else {
				/* 'default' enter sequence for state Confirmed */
				/* Entry action for state 'Confirmed'. */
				ifaceInternalSCI_OCB->beepDisarmed();
				ifaceInternalSCI_OCB->ledsDisarmed(1);
				stateConfVector[0] = main_region_Disarmed_r1_Confirmed;
				stateConfVectorPosition = 0;
				historyVector[0] = stateConfVector[0];
			}
		} 
	}
}

/* The reactions of state Confirmed. */
void SysManagerHSM::react_main_region_Disarmed_r1_Confirmed() {
	/* The reactions of state Confirmed. */
	if (iface.evKeyHoldOkB_raised) { 
		/* Default exit sequence for state Disarmed */
		/* Default exit sequence for region r1 */
		/* Handle exit of all possible states (of r1) at position 0... */
		switch(stateConfVector[ 0 ]) {
			case main_region_Disarmed_r1_Pending : {
				/* Default exit sequence for state Pending */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				/* Exit action for state 'Pending'. */
				timer->unsetTimer(this, &timeEvents[4]);
				break;
			}
			case main_region_Disarmed_r1_Confirmed : {
				/* Default exit sequence for state Confirmed */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		ifaceInternalSCI.tmp = ifaceInternalSCI.mode;
		ifaceInternalSCI.touch = false;
		/* 'default' enter sequence for state SelectMode */
		/* Entry action for state 'SelectMode'. */
		timer->setTimer(this, &timeEvents[0], ifaceInternalSCI.keyTimeout * 1000, true);
		ifaceInternalSCI_OCB->ledsSelect();
		ifaceInternalSCI_OCB->beepKey();
		stateConfVector[0] = main_region_SelectMode;
		stateConfVectorPosition = 0;
	}  else {
	}
}

/* The reactions of state Pending. */
void SysManagerHSM::react_r1_RTH_r1_Pending() {
	/* The reactions of state Pending. */
	if (timeEvents[5]) { 
		/* Default exit sequence for state RTH */
		/* Default exit sequence for region r1 */
		/* Handle exit of all possible states (of r1) at position 1... */
		switch(stateConfVector[ 1 ]) {
			case r1_RTH_r1_Pending : {
				/* Default exit sequence for state Pending */
				stateConfVector[1] = SysManagerHSM_last_state;
				stateConfVectorPosition = 1;
				/* Exit action for state 'Pending'. */
				timer->unsetTimer(this, &timeEvents[6]);
				break;
			}
			case r1_RTH_r1_Confirmed : {
				/* Default exit sequence for state Confirmed */
				stateConfVector[1] = SysManagerHSM_last_state;
				stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Exit action for state 'RTH'. */
		timer->unsetTimer(this, &timeEvents[5]);
		/* The reactions of state null. */
		if (! ifaceInternalSCI.sent) { 
			ifaceInternalSCI_OCB->publishKAT();
			/* 'default' enter sequence for state Sending */
			/* Entry action for state 'Sending'. */
			timer->setTimer(this, &timeEvents[3], ifaceInternalSCI.ackTimeout * 1000, false);
			stateConfVector[0] = main_region_Sending;
			stateConfVectorPosition = 0;
		}  else {
			ifaceInternalSCI.sent = false;
			/* Default react sequence for shallow history entry ArmedHistory */
			/* Enter the region with shallow history */
			if (historyVector[1] != SysManagerHSM_last_state) {
				shenseq_SequenceImpl();
			} 
		}
	}  else {
		if (iface.evKeyHoldOkB_raised) { 
			/* Default exit sequence for state RTH */
			/* Default exit sequence for region r1 */
			/* Handle exit of all possible states (of r1) at position 1... */
			switch(stateConfVector[ 1 ]) {
				case r1_RTH_r1_Pending : {
					/* Default exit sequence for state Pending */
					stateConfVector[1] = SysManagerHSM_last_state;
					stateConfVectorPosition = 1;
					/* Exit action for state 'Pending'. */
					timer->unsetTimer(this, &timeEvents[6]);
					break;
				}
				case r1_RTH_r1_Confirmed : {
					/* Default exit sequence for state Confirmed */
					stateConfVector[1] = SysManagerHSM_last_state;
					stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* Exit action for state 'RTH'. */
			timer->unsetTimer(this, &timeEvents[5]);
			/* 'default' enter sequence for state SelectMode */
			/* Entry action for state 'SelectMode'. */
			timer->setTimer(this, &timeEvents[0], ifaceInternalSCI.keyTimeout * 1000, true);
			ifaceInternalSCI_OCB->ledsSelect();
			ifaceInternalSCI_OCB->beepKey();
			stateConfVector[0] = main_region_SelectMode;
			stateConfVectorPosition = 0;
		}  else {
			if (timeEvents[6]) { 
				/* Default exit sequence for state RTH */
				/* Default exit sequence for region r1 */
				/* Handle exit of all possible states (of r1) at position 1... */
				switch(stateConfVector[ 1 ]) {
					case r1_RTH_r1_Pending : {
						/* Default exit sequence for state Pending */
						stateConfVector[1] = SysManagerHSM_last_state;
						stateConfVectorPosition = 1;
						/* Exit action for state 'Pending'. */
						timer->unsetTimer(this, &timeEvents[6]);
						break;
					}
					case r1_RTH_r1_Confirmed : {
						/* Default exit sequence for state Confirmed */
						stateConfVector[1] = SysManagerHSM_last_state;
						stateConfVectorPosition = 1;
						break;
					}
					default: break;
				}
				/* Exit action for state 'RTH'. */
				timer->unsetTimer(this, &timeEvents[5]);
				ifaceInternalSCI_OCB->publishMode();
				ifaceInternalSCI.sent = true;
				/* 'default' enter sequence for state Sending */
				/* Entry action for state 'Sending'. */
				timer->setTimer(this, &timeEvents[3], ifaceInternalSCI.ackTimeout * 1000, false);
				stateConfVector[0] = main_region_Sending;
				stateConfVectorPosition = 0;
			} 
		}
	}
}

/* The reactions of state Confirmed. */
void SysManagerHSM::react_r1_RTH_r1_Confirmed() {
	/* The reactions of state Confirmed. */
	if (timeEvents[5]) { 
		/* Default exit sequence for state RTH */
		/* Default exit sequence for region r1 */
		/* Handle exit of all possible states (of r1) at position 1... */
		switch(stateConfVector[ 1 ]) {
			case r1_RTH_r1_Pending : {
				/* Default exit sequence for state Pending */
				stateConfVector[1] = SysManagerHSM_last_state;
				stateConfVectorPosition = 1;
				/* Exit action for state 'Pending'. */
				timer->unsetTimer(this, &timeEvents[6]);
				break;
			}
			case r1_RTH_r1_Confirmed : {
				/* Default exit sequence for state Confirmed */
				stateConfVector[1] = SysManagerHSM_last_state;
				stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Exit action for state 'RTH'. */
		timer->unsetTimer(this, &timeEvents[5]);
		/* The reactions of state null. */
		if (! ifaceInternalSCI.sent) { 
			ifaceInternalSCI_OCB->publishKAT();
			/* 'default' enter sequence for state Sending */
			/* Entry action for state 'Sending'. */
			timer->setTimer(this, &timeEvents[3], ifaceInternalSCI.ackTimeout * 1000, false);
			stateConfVector[0] = main_region_Sending;
			stateConfVectorPosition = 0;
		}  else {
			ifaceInternalSCI.sent = false;
			/* Default react sequence for shallow history entry ArmedHistory */
			/* Enter the region with shallow history */
			if (historyVector[1] != SysManagerHSM_last_state) {
				shenseq_SequenceImpl();
			} 
		}
	}  else {
		if (iface.evKeyHoldOkB_raised) { 
			/* Default exit sequence for state RTH */
			/* Default exit sequence for region r1 */
			/* Handle exit of all possible states (of r1) at position 1... */
			switch(stateConfVector[ 1 ]) {
				case r1_RTH_r1_Pending : {
					/* Default exit sequence for state Pending */
					stateConfVector[1] = SysManagerHSM_last_state;
					stateConfVectorPosition = 1;
					/* Exit action for state 'Pending'. */
					timer->unsetTimer(this, &timeEvents[6]);
					break;
				}
				case r1_RTH_r1_Confirmed : {
					/* Default exit sequence for state Confirmed */
					stateConfVector[1] = SysManagerHSM_last_state;
					stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* Exit action for state 'RTH'. */
			timer->unsetTimer(this, &timeEvents[5]);
			/* 'default' enter sequence for state SelectMode */
			/* Entry action for state 'SelectMode'. */
			timer->setTimer(this, &timeEvents[0], ifaceInternalSCI.keyTimeout * 1000, true);
			ifaceInternalSCI_OCB->ledsSelect();
			ifaceInternalSCI_OCB->beepKey();
			stateConfVector[0] = main_region_SelectMode;
			stateConfVectorPosition = 0;
		}  else {
		}
	}
}

/* The reactions of state Pending. */
void SysManagerHSM::react_r1_Follow_r1_Pending() {
	/* The reactions of state Pending. */
	if (timeEvents[7]) { 
		/* Default exit sequence for state Follow */
		/* Default exit sequence for region r1 */
		/* Handle exit of all possible states (of r1) at position 1... */
		switch(stateConfVector[ 1 ]) {
			case r1_Follow_r1_Pending : {
				/* Default exit sequence for state Pending */
				stateConfVector[1] = SysManagerHSM_last_state;
				stateConfVectorPosition = 1;
				/* Exit action for state 'Pending'. */
				timer->unsetTimer(this, &timeEvents[8]);
				break;
			}
			case r1_Follow_r1_Confirmed : {
				/* Default exit sequence for state Confirmed */
				stateConfVector[1] = SysManagerHSM_last_state;
				stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Exit action for state 'Follow'. */
		timer->unsetTimer(this, &timeEvents[7]);
		/* The reactions of state null. */
		if (! ifaceInternalSCI.sent) { 
			ifaceInternalSCI_OCB->publishKAT();
			/* 'default' enter sequence for state Sending */
			/* Entry action for state 'Sending'. */
			timer->setTimer(this, &timeEvents[3], ifaceInternalSCI.ackTimeout * 1000, false);
			stateConfVector[0] = main_region_Sending;
			stateConfVectorPosition = 0;
		}  else {
			ifaceInternalSCI.sent = false;
			/* Default react sequence for shallow history entry ArmedHistory */
			/* Enter the region with shallow history */
			if (historyVector[1] != SysManagerHSM_last_state) {
				shenseq_SequenceImpl();
			} 
		}
	}  else {
		if (iface.evKeyHoldOkB_raised) { 
			/* Default exit sequence for state Follow */
			/* Default exit sequence for region r1 */
			/* Handle exit of all possible states (of r1) at position 1... */
			switch(stateConfVector[ 1 ]) {
				case r1_Follow_r1_Pending : {
					/* Default exit sequence for state Pending */
					stateConfVector[1] = SysManagerHSM_last_state;
					stateConfVectorPosition = 1;
					/* Exit action for state 'Pending'. */
					timer->unsetTimer(this, &timeEvents[8]);
					break;
				}
				case r1_Follow_r1_Confirmed : {
					/* Default exit sequence for state Confirmed */
					stateConfVector[1] = SysManagerHSM_last_state;
					stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* Exit action for state 'Follow'. */
			timer->unsetTimer(this, &timeEvents[7]);
			/* 'default' enter sequence for state SelectMode */
			/* Entry action for state 'SelectMode'. */
			timer->setTimer(this, &timeEvents[0], ifaceInternalSCI.keyTimeout * 1000, true);
			ifaceInternalSCI_OCB->ledsSelect();
			ifaceInternalSCI_OCB->beepKey();
			stateConfVector[0] = main_region_SelectMode;
			stateConfVectorPosition = 0;
		}  else {
			if (timeEvents[8]) { 
				/* Default exit sequence for state Follow */
				/* Default exit sequence for region r1 */
				/* Handle exit of all possible states (of r1) at position 1... */
				switch(stateConfVector[ 1 ]) {
					case r1_Follow_r1_Pending : {
						/* Default exit sequence for state Pending */
						stateConfVector[1] = SysManagerHSM_last_state;
						stateConfVectorPosition = 1;
						/* Exit action for state 'Pending'. */
						timer->unsetTimer(this, &timeEvents[8]);
						break;
					}
					case r1_Follow_r1_Confirmed : {
						/* Default exit sequence for state Confirmed */
						stateConfVector[1] = SysManagerHSM_last_state;
						stateConfVectorPosition = 1;
						break;
					}
					default: break;
				}
				/* Exit action for state 'Follow'. */
				timer->unsetTimer(this, &timeEvents[7]);
				ifaceInternalSCI_OCB->publishMode();
				ifaceInternalSCI.sent = true;
				/* 'default' enter sequence for state Sending */
				/* Entry action for state 'Sending'. */
				timer->setTimer(this, &timeEvents[3], ifaceInternalSCI.ackTimeout * 1000, false);
				stateConfVector[0] = main_region_Sending;
				stateConfVectorPosition = 0;
			} 
		}
	}
}

/* The reactions of state Confirmed. */
void SysManagerHSM::react_r1_Follow_r1_Confirmed() {
	/* The reactions of state Confirmed. */
	if (timeEvents[7]) { 
		/* Default exit sequence for state Follow */
		/* Default exit sequence for region r1 */
		/* Handle exit of all possible states (of r1) at position 1... */
		switch(stateConfVector[ 1 ]) {
			case r1_Follow_r1_Pending : {
				/* Default exit sequence for state Pending */
				stateConfVector[1] = SysManagerHSM_last_state;
				stateConfVectorPosition = 1;
				/* Exit action for state 'Pending'. */
				timer->unsetTimer(this, &timeEvents[8]);
				break;
			}
			case r1_Follow_r1_Confirmed : {
				/* Default exit sequence for state Confirmed */
				stateConfVector[1] = SysManagerHSM_last_state;
				stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Exit action for state 'Follow'. */
		timer->unsetTimer(this, &timeEvents[7]);
		/* The reactions of state null. */
		if (! ifaceInternalSCI.sent) { 
			ifaceInternalSCI_OCB->publishKAT();
			/* 'default' enter sequence for state Sending */
			/* Entry action for state 'Sending'. */
			timer->setTimer(this, &timeEvents[3], ifaceInternalSCI.ackTimeout * 1000, false);
			stateConfVector[0] = main_region_Sending;
			stateConfVectorPosition = 0;
		}  else {
			ifaceInternalSCI.sent = false;
			/* Default react sequence for shallow history entry ArmedHistory */
			/* Enter the region with shallow history */
			if (historyVector[1] != SysManagerHSM_last_state) {
				shenseq_SequenceImpl();
			} 
		}
	}  else {
		if (iface.evKeyHoldOkB_raised) { 
			/* Default exit sequence for state Follow */
			/* Default exit sequence for region r1 */
			/* Handle exit of all possible states (of r1) at position 1... */
			switch(stateConfVector[ 1 ]) {
				case r1_Follow_r1_Pending : {
					/* Default exit sequence for state Pending */
					stateConfVector[1] = SysManagerHSM_last_state;
					stateConfVectorPosition = 1;
					/* Exit action for state 'Pending'. */
					timer->unsetTimer(this, &timeEvents[8]);
					break;
				}
				case r1_Follow_r1_Confirmed : {
					/* Default exit sequence for state Confirmed */
					stateConfVector[1] = SysManagerHSM_last_state;
					stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* Exit action for state 'Follow'. */
			timer->unsetTimer(this, &timeEvents[7]);
			/* 'default' enter sequence for state SelectMode */
			/* Entry action for state 'SelectMode'. */
			timer->setTimer(this, &timeEvents[0], ifaceInternalSCI.keyTimeout * 1000, true);
			ifaceInternalSCI_OCB->ledsSelect();
			ifaceInternalSCI_OCB->beepKey();
			stateConfVector[0] = main_region_SelectMode;
			stateConfVectorPosition = 0;
		}  else {
			if (iface.evJoysHold_raised) { 
				/* Default exit sequence for state Follow */
				/* Default exit sequence for region r1 */
				/* Handle exit of all possible states (of r1) at position 1... */
				switch(stateConfVector[ 1 ]) {
					case r1_Follow_r1_Pending : {
						/* Default exit sequence for state Pending */
						stateConfVector[1] = SysManagerHSM_last_state;
						stateConfVectorPosition = 1;
						/* Exit action for state 'Pending'. */
						timer->unsetTimer(this, &timeEvents[8]);
						break;
					}
					case r1_Follow_r1_Confirmed : {
						/* Default exit sequence for state Confirmed */
						stateConfVector[1] = SysManagerHSM_last_state;
						stateConfVectorPosition = 1;
						break;
					}
					default: break;
				}
				/* Exit action for state 'Follow'. */
				timer->unsetTimer(this, &timeEvents[7]);
				ifaceInternalSCI_OCB->beepKey();
				ifaceInternalSCI_OCB->getAction();
				ifaceInternalSCI_OCB->publishProfile();
				ifaceInternalSCI.sent = true;
				/* 'default' enter sequence for state Sending */
				/* Entry action for state 'Sending'. */
				timer->setTimer(this, &timeEvents[3], ifaceInternalSCI.ackTimeout * 1000, false);
				stateConfVector[0] = main_region_Sending;
				stateConfVectorPosition = 0;
			} 
		}
	}
}

/* The reactions of state Pending. */
void SysManagerHSM::react_r1_Idle_r1_Pending() {
	/* The reactions of state Pending. */
	if (timeEvents[9]) { 
		/* Default exit sequence for state Idle */
		/* Default exit sequence for region r1 */
		/* Handle exit of all possible states (of r1) at position 1... */
		switch(stateConfVector[ 1 ]) {
			case r1_Idle_r1_Pending : {
				/* Default exit sequence for state Pending */
				stateConfVector[1] = SysManagerHSM_last_state;
				stateConfVectorPosition = 1;
				/* Exit action for state 'Pending'. */
				timer->unsetTimer(this, &timeEvents[10]);
				break;
			}
			case r1_Idle_r1_Confirmed : {
				/* Default exit sequence for state Confirmed */
				stateConfVector[1] = SysManagerHSM_last_state;
				stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Exit action for state 'Idle'. */
		timer->unsetTimer(this, &timeEvents[9]);
		/* The reactions of state null. */
		if (! ifaceInternalSCI.sent) { 
			ifaceInternalSCI_OCB->publishKAT();
			/* 'default' enter sequence for state Sending */
			/* Entry action for state 'Sending'. */
			timer->setTimer(this, &timeEvents[3], ifaceInternalSCI.ackTimeout * 1000, false);
			stateConfVector[0] = main_region_Sending;
			stateConfVectorPosition = 0;
		}  else {
			ifaceInternalSCI.sent = false;
			/* Default react sequence for shallow history entry ArmedHistory */
			/* Enter the region with shallow history */
			if (historyVector[1] != SysManagerHSM_last_state) {
				shenseq_SequenceImpl();
			} 
		}
	}  else {
		if (iface.evKeyHoldOkB_raised) { 
			/* Default exit sequence for state Idle */
			/* Default exit sequence for region r1 */
			/* Handle exit of all possible states (of r1) at position 1... */
			switch(stateConfVector[ 1 ]) {
				case r1_Idle_r1_Pending : {
					/* Default exit sequence for state Pending */
					stateConfVector[1] = SysManagerHSM_last_state;
					stateConfVectorPosition = 1;
					/* Exit action for state 'Pending'. */
					timer->unsetTimer(this, &timeEvents[10]);
					break;
				}
				case r1_Idle_r1_Confirmed : {
					/* Default exit sequence for state Confirmed */
					stateConfVector[1] = SysManagerHSM_last_state;
					stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* Exit action for state 'Idle'. */
			timer->unsetTimer(this, &timeEvents[9]);
			/* 'default' enter sequence for state SelectMode */
			/* Entry action for state 'SelectMode'. */
			timer->setTimer(this, &timeEvents[0], ifaceInternalSCI.keyTimeout * 1000, true);
			ifaceInternalSCI_OCB->ledsSelect();
			ifaceInternalSCI_OCB->beepKey();
			stateConfVector[0] = main_region_SelectMode;
			stateConfVectorPosition = 0;
		}  else {
			if (timeEvents[10]) { 
				/* Default exit sequence for state Idle */
				/* Default exit sequence for region r1 */
				/* Handle exit of all possible states (of r1) at position 1... */
				switch(stateConfVector[ 1 ]) {
					case r1_Idle_r1_Pending : {
						/* Default exit sequence for state Pending */
						stateConfVector[1] = SysManagerHSM_last_state;
						stateConfVectorPosition = 1;
						/* Exit action for state 'Pending'. */
						timer->unsetTimer(this, &timeEvents[10]);
						break;
					}
					case r1_Idle_r1_Confirmed : {
						/* Default exit sequence for state Confirmed */
						stateConfVector[1] = SysManagerHSM_last_state;
						stateConfVectorPosition = 1;
						break;
					}
					default: break;
				}
				/* Exit action for state 'Idle'. */
				timer->unsetTimer(this, &timeEvents[9]);
				ifaceInternalSCI_OCB->publishMode();
				ifaceInternalSCI.sent = true;
				/* 'default' enter sequence for state Sending */
				/* Entry action for state 'Sending'. */
				timer->setTimer(this, &timeEvents[3], ifaceInternalSCI.ackTimeout * 1000, false);
				stateConfVector[0] = main_region_Sending;
				stateConfVectorPosition = 0;
			} 
		}
	}
}

/* The reactions of state Confirmed. */
void SysManagerHSM::react_r1_Idle_r1_Confirmed() {
	/* The reactions of state Confirmed. */
	if (timeEvents[9]) { 
		/* Default exit sequence for state Idle */
		/* Default exit sequence for region r1 */
		/* Handle exit of all possible states (of r1) at position 1... */
		switch(stateConfVector[ 1 ]) {
			case r1_Idle_r1_Pending : {
				/* Default exit sequence for state Pending */
				stateConfVector[1] = SysManagerHSM_last_state;
				stateConfVectorPosition = 1;
				/* Exit action for state 'Pending'. */
				timer->unsetTimer(this, &timeEvents[10]);
				break;
			}
			case r1_Idle_r1_Confirmed : {
				/* Default exit sequence for state Confirmed */
				stateConfVector[1] = SysManagerHSM_last_state;
				stateConfVectorPosition = 1;
				break;
			}
			default: break;
		}
		/* Exit action for state 'Idle'. */
		timer->unsetTimer(this, &timeEvents[9]);
		/* The reactions of state null. */
		if (! ifaceInternalSCI.sent) { 
			ifaceInternalSCI_OCB->publishKAT();
			/* 'default' enter sequence for state Sending */
			/* Entry action for state 'Sending'. */
			timer->setTimer(this, &timeEvents[3], ifaceInternalSCI.ackTimeout * 1000, false);
			stateConfVector[0] = main_region_Sending;
			stateConfVectorPosition = 0;
		}  else {
			ifaceInternalSCI.sent = false;
			/* Default react sequence for shallow history entry ArmedHistory */
			/* Enter the region with shallow history */
			if (historyVector[1] != SysManagerHSM_last_state) {
				shenseq_SequenceImpl();
			} 
		}
	}  else {
		if (iface.evKeyHoldOkB_raised) { 
			/* Default exit sequence for state Idle */
			/* Default exit sequence for region r1 */
			/* Handle exit of all possible states (of r1) at position 1... */
			switch(stateConfVector[ 1 ]) {
				case r1_Idle_r1_Pending : {
					/* Default exit sequence for state Pending */
					stateConfVector[1] = SysManagerHSM_last_state;
					stateConfVectorPosition = 1;
					/* Exit action for state 'Pending'. */
					timer->unsetTimer(this, &timeEvents[10]);
					break;
				}
				case r1_Idle_r1_Confirmed : {
					/* Default exit sequence for state Confirmed */
					stateConfVector[1] = SysManagerHSM_last_state;
					stateConfVectorPosition = 1;
					break;
				}
				default: break;
			}
			/* Exit action for state 'Idle'. */
			timer->unsetTimer(this, &timeEvents[9]);
			/* 'default' enter sequence for state SelectMode */
			/* Entry action for state 'SelectMode'. */
			timer->setTimer(this, &timeEvents[0], ifaceInternalSCI.keyTimeout * 1000, true);
			ifaceInternalSCI_OCB->ledsSelect();
			ifaceInternalSCI_OCB->beepKey();
			stateConfVector[0] = main_region_SelectMode;
			stateConfVectorPosition = 0;
		}  else {
		}
	}
}

/* The reactions of state Confirmed. */
void SysManagerHSM::react_r1_Manual_r1_Confirmed() {
	/* The reactions of state Confirmed. */
	if (timeEvents[11]) { 
		/* Default exit sequence for state Manual */
		/* Default exit sequence for region r1 */
		/* Handle exit of all possible states (of r1) at position 1... */
		switch(stateConfVector[ 1 ]) {
			case r1_Manual_r1_Confirmed : {
				/* Default exit sequence for state Confirmed */
				stateConfVector[1] = SysManagerHSM_last_state;
				stateConfVectorPosition = 1;
				break;
			}
			case r1_Manual_r1_Pending : {
				/* Default exit sequence for state Pending */
				stateConfVector[1] = SysManagerHSM_last_state;
				stateConfVectorPosition = 1;
				/* Exit action for state 'Pending'. */
				timer->unsetTimer(this, &timeEvents[12]);
				break;
			}
			default: break;
		}
		/* Exit action for state 'Manual'. */
		timer->unsetTimer(this, &timeEvents[11]);
		/* The reactions of state null. */
		if (! ifaceInternalSCI.sent) { 
			ifaceInternalSCI_OCB->publishKAT();
			/* 'default' enter sequence for state Sending */
			/* Entry action for state 'Sending'. */
			timer->setTimer(this, &timeEvents[3], ifaceInternalSCI.ackTimeout * 1000, false);
			stateConfVector[0] = main_region_Sending;
			stateConfVectorPosition = 0;
		}  else {
			ifaceInternalSCI.sent = false;
			/* Default react sequence for shallow history entry ArmedHistory */
			/* Enter the region with shallow history */
			if (historyVector[1] != SysManagerHSM_last_state) {
				shenseq_SequenceImpl();
			} 
		}
	}  else {
		if (iface.evKeyHoldOkB_raised) { 
			/* Default exit sequence for state Manual */
			/* Default exit sequence for region r1 */
			/* Handle exit of all possible states (of r1) at position 1... */
			switch(stateConfVector[ 1 ]) {
				case r1_Manual_r1_Confirmed : {
					/* Default exit sequence for state Confirmed */
					stateConfVector[1] = SysManagerHSM_last_state;
					stateConfVectorPosition = 1;
					break;
				}
				case r1_Manual_r1_Pending : {
					/* Default exit sequence for state Pending */
					stateConfVector[1] = SysManagerHSM_last_state;
					stateConfVectorPosition = 1;
					/* Exit action for state 'Pending'. */
					timer->unsetTimer(this, &timeEvents[12]);
					break;
				}
				default: break;
			}
			/* Exit action for state 'Manual'. */
			timer->unsetTimer(this, &timeEvents[11]);
			/* 'default' enter sequence for state SelectMode */
			/* Entry action for state 'SelectMode'. */
			timer->setTimer(this, &timeEvents[0], ifaceInternalSCI.keyTimeout * 1000, true);
			ifaceInternalSCI_OCB->ledsSelect();
			ifaceInternalSCI_OCB->beepKey();
			stateConfVector[0] = main_region_SelectMode;
			stateConfVectorPosition = 0;
		}  else {
			if (iface.evJoysMove_raised) { 
				/* Default exit sequence for state Manual */
				/* Default exit sequence for region r1 */
				/* Handle exit of all possible states (of r1) at position 1... */
				switch(stateConfVector[ 1 ]) {
					case r1_Manual_r1_Confirmed : {
						/* Default exit sequence for state Confirmed */
						stateConfVector[1] = SysManagerHSM_last_state;
						stateConfVectorPosition = 1;
						break;
					}
					case r1_Manual_r1_Pending : {
						/* Default exit sequence for state Pending */
						stateConfVector[1] = SysManagerHSM_last_state;
						stateConfVectorPosition = 1;
						/* Exit action for state 'Pending'. */
						timer->unsetTimer(this, &timeEvents[12]);
						break;
					}
					default: break;
				}
				/* Exit action for state 'Manual'. */
				timer->unsetTimer(this, &timeEvents[11]);
				ifaceInternalSCI_OCB->beepKey();
				ifaceInternalSCI_OCB->getAction();
				ifaceInternalSCI_OCB->publishRc();
				ifaceInternalSCI.sent = true;
				/* 'default' enter sequence for state Sending */
				/* Entry action for state 'Sending'. */
				timer->setTimer(this, &timeEvents[3], ifaceInternalSCI.ackTimeout * 1000, false);
				stateConfVector[0] = main_region_Sending;
				stateConfVectorPosition = 0;
			} 
		}
	}
}

/* The reactions of state Pending. */
void SysManagerHSM::react_r1_Manual_r1_Pending() {
	/* The reactions of state Pending. */
	if (timeEvents[11]) { 
		/* Default exit sequence for state Manual */
		/* Default exit sequence for region r1 */
		/* Handle exit of all possible states (of r1) at position 1... */
		switch(stateConfVector[ 1 ]) {
			case r1_Manual_r1_Confirmed : {
				/* Default exit sequence for state Confirmed */
				stateConfVector[1] = SysManagerHSM_last_state;
				stateConfVectorPosition = 1;
				break;
			}
			case r1_Manual_r1_Pending : {
				/* Default exit sequence for state Pending */
				stateConfVector[1] = SysManagerHSM_last_state;
				stateConfVectorPosition = 1;
				/* Exit action for state 'Pending'. */
				timer->unsetTimer(this, &timeEvents[12]);
				break;
			}
			default: break;
		}
		/* Exit action for state 'Manual'. */
		timer->unsetTimer(this, &timeEvents[11]);
		/* The reactions of state null. */
		if (! ifaceInternalSCI.sent) { 
			ifaceInternalSCI_OCB->publishKAT();
			/* 'default' enter sequence for state Sending */
			/* Entry action for state 'Sending'. */
			timer->setTimer(this, &timeEvents[3], ifaceInternalSCI.ackTimeout * 1000, false);
			stateConfVector[0] = main_region_Sending;
			stateConfVectorPosition = 0;
		}  else {
			ifaceInternalSCI.sent = false;
			/* Default react sequence for shallow history entry ArmedHistory */
			/* Enter the region with shallow history */
			if (historyVector[1] != SysManagerHSM_last_state) {
				shenseq_SequenceImpl();
			} 
		}
	}  else {
		if (iface.evKeyHoldOkB_raised) { 
			/* Default exit sequence for state Manual */
			/* Default exit sequence for region r1 */
			/* Handle exit of all possible states (of r1) at position 1... */
			switch(stateConfVector[ 1 ]) {
				case r1_Manual_r1_Confirmed : {
					/* Default exit sequence for state Confirmed */
					stateConfVector[1] = SysManagerHSM_last_state;
					stateConfVectorPosition = 1;
					break;
				}
				case r1_Manual_r1_Pending : {
					/* Default exit sequence for state Pending */
					stateConfVector[1] = SysManagerHSM_last_state;
					stateConfVectorPosition = 1;
					/* Exit action for state 'Pending'. */
					timer->unsetTimer(this, &timeEvents[12]);
					break;
				}
				default: break;
			}
			/* Exit action for state 'Manual'. */
			timer->unsetTimer(this, &timeEvents[11]);
			/* 'default' enter sequence for state SelectMode */
			/* Entry action for state 'SelectMode'. */
			timer->setTimer(this, &timeEvents[0], ifaceInternalSCI.keyTimeout * 1000, true);
			ifaceInternalSCI_OCB->ledsSelect();
			ifaceInternalSCI_OCB->beepKey();
			stateConfVector[0] = main_region_SelectMode;
			stateConfVectorPosition = 0;
		}  else {
			if (timeEvents[12]) { 
				/* Default exit sequence for state Manual */
				/* Default exit sequence for region r1 */
				/* Handle exit of all possible states (of r1) at position 1... */
				switch(stateConfVector[ 1 ]) {
					case r1_Manual_r1_Confirmed : {
						/* Default exit sequence for state Confirmed */
						stateConfVector[1] = SysManagerHSM_last_state;
						stateConfVectorPosition = 1;
						break;
					}
					case r1_Manual_r1_Pending : {
						/* Default exit sequence for state Pending */
						stateConfVector[1] = SysManagerHSM_last_state;
						stateConfVectorPosition = 1;
						/* Exit action for state 'Pending'. */
						timer->unsetTimer(this, &timeEvents[12]);
						break;
					}
					default: break;
				}
				/* Exit action for state 'Manual'. */
				timer->unsetTimer(this, &timeEvents[11]);
				ifaceInternalSCI_OCB->publishMode();
				ifaceInternalSCI.sent = true;
				/* 'default' enter sequence for state Sending */
				/* Entry action for state 'Sending'. */
				timer->setTimer(this, &timeEvents[3], ifaceInternalSCI.ackTimeout * 1000, false);
				stateConfVector[0] = main_region_Sending;
				stateConfVectorPosition = 0;
			} 
		}
	}
}

/* The reactions of state Select. */
void SysManagerHSM::react_r1_Select() {
	/* The reactions of state Select. */
	if (timeEvents[13]) { 
		/* Default exit sequence for state Select */
		stateConfVector[1] = SysManagerHSM_last_state;
		stateConfVectorPosition = 1;
		/* Exit action for state 'Select'. */
		timer->unsetTimer(this, &timeEvents[13]);
		/* The reactions of state null. */
		if (ifaceInternalSCI.mode == 1) { 
			/* 'default' enter sequence for state Idle */
			/* Entry action for state 'Idle'. */
			timer->setTimer(this, &timeEvents[9], ifaceInternalSCI.keepAliveTimeout * 1000, true);
			/* 'default' enter sequence for region r1 */
			/* Default react sequence for initial entry  */
			/* The reactions of state null. */
			if (! iface.confirmed) { 
				/* 'default' enter sequence for state Pending */
				/* Entry action for state 'Pending'. */
				timer->setTimer(this, &timeEvents[10], 250, false);
				ifaceInternalSCI_OCB->beepPending();
				ifaceInternalSCI_OCB->ledsArmed(0);
				stateConfVector[1] = r1_Idle_r1_Pending;
				stateConfVectorPosition = 1;
			}  else {
				/* 'default' enter sequence for state Confirmed */
				/* Entry action for state 'Confirmed'. */
				ifaceInternalSCI_OCB->beepArmed();
				ifaceInternalSCI_OCB->ledsArmed(1);
				stateConfVector[1] = r1_Idle_r1_Confirmed;
				stateConfVectorPosition = 1;
			}
			historyVector[1] = stateConfVector[1];
		}  else {
			if (ifaceInternalSCI.mode == 2) { 
				/* 'default' enter sequence for state Manual */
				/* Entry action for state 'Manual'. */
				timer->setTimer(this, &timeEvents[11], ifaceInternalSCI.keepAliveTimeout * 1000, true);
				/* 'default' enter sequence for region r1 */
				/* Default react sequence for initial entry  */
				/* The reactions of state null. */
				if (! iface.confirmed) { 
					/* 'default' enter sequence for state Pending */
					/* Entry action for state 'Pending'. */
					timer->setTimer(this, &timeEvents[12], 250, false);
					ifaceInternalSCI_OCB->beepPending();
					ifaceInternalSCI_OCB->ledsManual(0);
					stateConfVector[1] = r1_Manual_r1_Pending;
					stateConfVectorPosition = 1;
				}  else {
					/* 'default' enter sequence for state Confirmed */
					/* Entry action for state 'Confirmed'. */
					ifaceInternalSCI_OCB->beepManual();
					ifaceInternalSCI_OCB->ledsManual(1);
					stateConfVector[1] = r1_Manual_r1_Confirmed;
					stateConfVectorPosition = 1;
				}
				historyVector[1] = stateConfVector[1];
			}  else {
				if (ifaceInternalSCI.mode == 3) { 
					/* 'default' enter sequence for state Follow */
					/* Entry action for state 'Follow'. */
					timer->setTimer(this, &timeEvents[7], ifaceInternalSCI.keepAliveTimeout * 1000, true);
					/* 'default' enter sequence for region r1 */
					/* Default react sequence for initial entry  */
					/* The reactions of state null. */
					if (! iface.confirmed) { 
						/* 'default' enter sequence for state Pending */
						/* Entry action for state 'Pending'. */
						timer->setTimer(this, &timeEvents[8], 250, false);
						ifaceInternalSCI_OCB->beepPending();
						ifaceInternalSCI_OCB->ledsFollow(0);
						stateConfVector[1] = r1_Follow_r1_Pending;
						stateConfVectorPosition = 1;
					}  else {
						/* 'default' enter sequence for state Confirmed */
						/* Entry action for state 'Confirmed'. */
						ifaceInternalSCI_OCB->beepFollow();
						ifaceInternalSCI_OCB->ledsFollow(1);
						stateConfVector[1] = r1_Follow_r1_Confirmed;
						stateConfVectorPosition = 1;
					}
					historyVector[1] = stateConfVector[1];
				}  else {
					if (ifaceInternalSCI.mode == 4) { 
						/* 'default' enter sequence for state RTH */
						/* Entry action for state 'RTH'. */
						timer->setTimer(this, &timeEvents[5], ifaceInternalSCI.keepAliveTimeout * 1000, true);
						/* 'default' enter sequence for region r1 */
						/* Default react sequence for initial entry  */
						/* The reactions of state null. */
						if (! iface.confirmed) { 
							/* 'default' enter sequence for state Pending */
							/* Entry action for state 'Pending'. */
							timer->setTimer(this, &timeEvents[6], 250, false);
							ifaceInternalSCI_OCB->beepPending();
							ifaceInternalSCI_OCB->ledsRth(0);
							stateConfVector[1] = r1_RTH_r1_Pending;
							stateConfVectorPosition = 1;
						}  else {
							/* 'default' enter sequence for state Confirmed */
							/* Entry action for state 'Confirmed'. */
							ifaceInternalSCI_OCB->beepRth();
							ifaceInternalSCI_OCB->ledsRth(1);
							stateConfVector[1] = r1_RTH_r1_Confirmed;
							stateConfVectorPosition = 1;
						}
						historyVector[1] = stateConfVector[1];
					}  else {
						/* 'default' enter sequence for state Pending */
						/* Entry action for state 'Pending'. */
						timer->setTimer(this, &timeEvents[4], 1 * 1000, false);
						ifaceInternalSCI_OCB->beepPending();
						ifaceInternalSCI_OCB->ledsDisarmed(0);
						stateConfVector[0] = main_region_Disarmed_r1_Pending;
						stateConfVectorPosition = 0;
						historyVector[0] = stateConfVector[0];
					}
				}
			}
		}
	} 
}


