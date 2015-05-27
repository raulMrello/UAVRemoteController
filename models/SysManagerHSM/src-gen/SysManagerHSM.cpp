
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
	ifaceInternalSCI.mode = 0;
	ifaceInternalSCI.tmp = 0;
	ifaceInternalSCI.keyTimeout = 5;
	ifaceInternalSCI.ackTimeout = 5;

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
	historyVector[1] = stateConfVector[0];
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
		case main_region_Armed_r1_Manual : {
			/* Default exit sequence for state Manual */
			stateConfVector[0] = SysManagerHSM_last_state;
			stateConfVectorPosition = 0;
			break;
		}
		case main_region_Armed_r1_Follow : {
			/* Default exit sequence for state Follow */
			stateConfVector[0] = SysManagerHSM_last_state;
			stateConfVectorPosition = 0;
			break;
		}
		case main_region_Armed_r1_Idle_idle_Pending : {
			/* Default exit sequence for state Pending */
			stateConfVector[0] = SysManagerHSM_last_state;
			stateConfVectorPosition = 0;
			break;
		}
		case main_region_Armed_r1_Idle_idle_Confirmed : {
			/* Default exit sequence for state Confirmed */
			stateConfVector[0] = SysManagerHSM_last_state;
			stateConfVectorPosition = 0;
			break;
		}
		case main_region_Armed_r1_RTH_r1_Pending : {
			/* Default exit sequence for state Pending */
			stateConfVector[0] = SysManagerHSM_last_state;
			stateConfVectorPosition = 0;
			/* Exit action for state 'Pending'. */
			timer->unsetTimer(this, &timeEvents[1]);
			break;
		}
		case main_region_Armed_r1_RTH_r1_Confirmed : {
			/* Default exit sequence for state Confirmed */
			stateConfVector[0] = SysManagerHSM_last_state;
			stateConfVectorPosition = 0;
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
		case main_region_Armed_r1_Manual : {
			react_main_region_Armed_r1_Manual();
			break;
		}
		case main_region_Armed_r1_Follow : {
			react_main_region_Armed_r1_Follow();
			break;
		}
		case main_region_Armed_r1_Idle_idle_Pending : {
			react_main_region_Armed_r1_Idle_idle_Pending();
			break;
		}
		case main_region_Armed_r1_Idle_idle_Confirmed : {
			react_main_region_Armed_r1_Idle_idle_Confirmed();
			break;
		}
		case main_region_Armed_r1_RTH_r1_Pending : {
			react_main_region_Armed_r1_RTH_r1_Pending();
			break;
		}
		case main_region_Armed_r1_RTH_r1_Confirmed : {
			react_main_region_Armed_r1_RTH_r1_Confirmed();
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
	ifaceInternalSCI.evInit_raised = false; 
	timeEvents[0] = false; 
	timeEvents[1] = false; 
	timeEvents[2] = false; 
	timeEvents[3] = false; 
	timeEvents[4] = false; 
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
			return (sc_boolean) (stateConfVector[0] >= main_region_Armed
				&& stateConfVector[0] <= main_region_Armed_r1_RTH_r1_Confirmed);
		case main_region_Armed_r1_Manual : 
			return (sc_boolean) (stateConfVector[0] == main_region_Armed_r1_Manual
			);
		case main_region_Armed_r1_Follow : 
			return (sc_boolean) (stateConfVector[0] == main_region_Armed_r1_Follow
			);
		case main_region_Armed_r1_Idle : 
			return (sc_boolean) (stateConfVector[0] >= main_region_Armed_r1_Idle
				&& stateConfVector[0] <= main_region_Armed_r1_Idle_idle_Confirmed);
		case main_region_Armed_r1_Idle_idle_Pending : 
			return (sc_boolean) (stateConfVector[0] == main_region_Armed_r1_Idle_idle_Pending
			);
		case main_region_Armed_r1_Idle_idle_Confirmed : 
			return (sc_boolean) (stateConfVector[0] == main_region_Armed_r1_Idle_idle_Confirmed
			);
		case main_region_Armed_r1_RTH : 
			return (sc_boolean) (stateConfVector[0] >= main_region_Armed_r1_RTH
				&& stateConfVector[0] <= main_region_Armed_r1_RTH_r1_Confirmed);
		case main_region_Armed_r1_RTH_r1_Pending : 
			return (sc_boolean) (stateConfVector[0] == main_region_Armed_r1_RTH_r1_Pending
			);
		case main_region_Armed_r1_RTH_r1_Confirmed : 
			return (sc_boolean) (stateConfVector[0] == main_region_Armed_r1_RTH_r1_Confirmed
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
void SysManagerHSM::shenseq_SequenceImpl() {
	/* shallow enterSequence with history in child r1 */
	/* Handle shallow history entry of r1 */
	switch(historyVector[ 0 ]) {
		case main_region_Armed_r1_Manual : {
			/* 'default' enter sequence for state Manual */
			stateConfVector[0] = main_region_Armed_r1_Manual;
			stateConfVectorPosition = 0;
			historyVector[0] = stateConfVector[0];
			break;
		}
		case main_region_Armed_r1_Follow : {
			/* 'default' enter sequence for state Follow */
			stateConfVector[0] = main_region_Armed_r1_Follow;
			stateConfVectorPosition = 0;
			historyVector[0] = stateConfVector[0];
			break;
		}
		case main_region_Armed_r1_Idle_idle_Pending : {
			/* 'default' enter sequence for state Idle */
			/* 'default' enter sequence for region idle */
			/* Default react sequence for initial entry  */
			/* The reactions of state null. */
			if (! iface.confirmed) { 
				/* 'default' enter sequence for state Pending */
				/* Entry action for state 'Pending'. */
				ifaceInternalSCI_OCB->beepPending();
				ifaceInternalSCI_OCB->ledsArmed(0);
				iface.evConfirmReq_raised = true;
				stateConfVector[0] = main_region_Armed_r1_Idle_idle_Pending;
				stateConfVectorPosition = 0;
			}  else {
				/* 'default' enter sequence for state Confirmed */
				/* Entry action for state 'Confirmed'. */
				ifaceInternalSCI_OCB->beepArmed();
				ifaceInternalSCI_OCB->ledsArmed(1);
				stateConfVector[0] = main_region_Armed_r1_Idle_idle_Confirmed;
				stateConfVectorPosition = 0;
			}
			historyVector[0] = stateConfVector[0];
			break;
		}
		case main_region_Armed_r1_Idle_idle_Confirmed : {
			/* 'default' enter sequence for state Idle */
			/* 'default' enter sequence for region idle */
			/* Default react sequence for initial entry  */
			/* The reactions of state null. */
			if (! iface.confirmed) { 
				/* 'default' enter sequence for state Pending */
				/* Entry action for state 'Pending'. */
				ifaceInternalSCI_OCB->beepPending();
				ifaceInternalSCI_OCB->ledsArmed(0);
				iface.evConfirmReq_raised = true;
				stateConfVector[0] = main_region_Armed_r1_Idle_idle_Pending;
				stateConfVectorPosition = 0;
			}  else {
				/* 'default' enter sequence for state Confirmed */
				/* Entry action for state 'Confirmed'. */
				ifaceInternalSCI_OCB->beepArmed();
				ifaceInternalSCI_OCB->ledsArmed(1);
				stateConfVector[0] = main_region_Armed_r1_Idle_idle_Confirmed;
				stateConfVectorPosition = 0;
			}
			historyVector[0] = stateConfVector[0];
			break;
		}
		case main_region_Armed_r1_RTH_r1_Pending : {
			/* 'default' enter sequence for state RTH */
			/* 'default' enter sequence for region r1 */
			/* Default react sequence for initial entry  */
			/* The reactions of state null. */
			if (! iface.confirmed) { 
				/* 'default' enter sequence for state Pending */
				/* Entry action for state 'Pending'. */
				timer->setTimer(this, &timeEvents[1], 250, false);
				ifaceInternalSCI_OCB->beepPending();
				ifaceInternalSCI_OCB->ledsRth(0);
				stateConfVector[0] = main_region_Armed_r1_RTH_r1_Pending;
				stateConfVectorPosition = 0;
			}  else {
				/* 'default' enter sequence for state Confirmed */
				/* Entry action for state 'Confirmed'. */
				ifaceInternalSCI_OCB->beepRth();
				ifaceInternalSCI_OCB->ledsRth(1);
				stateConfVector[0] = main_region_Armed_r1_RTH_r1_Confirmed;
				stateConfVectorPosition = 0;
			}
			historyVector[0] = stateConfVector[0];
			break;
		}
		case main_region_Armed_r1_RTH_r1_Confirmed : {
			/* 'default' enter sequence for state RTH */
			/* 'default' enter sequence for region r1 */
			/* Default react sequence for initial entry  */
			/* The reactions of state null. */
			if (! iface.confirmed) { 
				/* 'default' enter sequence for state Pending */
				/* Entry action for state 'Pending'. */
				timer->setTimer(this, &timeEvents[1], 250, false);
				ifaceInternalSCI_OCB->beepPending();
				ifaceInternalSCI_OCB->ledsRth(0);
				stateConfVector[0] = main_region_Armed_r1_RTH_r1_Pending;
				stateConfVectorPosition = 0;
			}  else {
				/* 'default' enter sequence for state Confirmed */
				/* Entry action for state 'Confirmed'. */
				ifaceInternalSCI_OCB->beepRth();
				ifaceInternalSCI_OCB->ledsRth(1);
				stateConfVector[0] = main_region_Armed_r1_RTH_r1_Confirmed;
				stateConfVectorPosition = 0;
			}
			historyVector[0] = stateConfVector[0];
			break;
		}
		default: break;
	}
}

/* shallow enterSequence with history in child r1 */
void SysManagerHSM::shenseq_main_region_Disarmed_SequenceImpl() {
	/* shallow enterSequence with history in child r1 */
	/* Handle shallow history entry of r1 */
	switch(historyVector[ 1 ]) {
		case main_region_Disarmed_r1_Pending : {
			/* 'default' enter sequence for state Pending */
			/* Entry action for state 'Pending'. */
			timer->setTimer(this, &timeEvents[4], 1 * 1000, false);
			ifaceInternalSCI_OCB->beepPending();
			ifaceInternalSCI_OCB->ledsDisarmed(0);
			stateConfVector[0] = main_region_Disarmed_r1_Pending;
			stateConfVectorPosition = 0;
			historyVector[1] = stateConfVector[0];
			break;
		}
		case main_region_Disarmed_r1_Confirmed : {
			/* 'default' enter sequence for state Confirmed */
			/* Entry action for state 'Confirmed'. */
			ifaceInternalSCI_OCB->beepDisarmed();
			ifaceInternalSCI_OCB->ledsDisarmed(1);
			stateConfVector[0] = main_region_Disarmed_r1_Confirmed;
			stateConfVectorPosition = 0;
			historyVector[1] = stateConfVector[0];
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
		if (ifaceInternalSCI.mode == 1) { 
			/* 'default' enter sequence for state Idle */
			/* 'default' enter sequence for region idle */
			/* Default react sequence for initial entry  */
			/* The reactions of state null. */
			if (! iface.confirmed) { 
				/* 'default' enter sequence for state Pending */
				/* Entry action for state 'Pending'. */
				ifaceInternalSCI_OCB->beepPending();
				ifaceInternalSCI_OCB->ledsArmed(0);
				iface.evConfirmReq_raised = true;
				stateConfVector[0] = main_region_Armed_r1_Idle_idle_Pending;
				stateConfVectorPosition = 0;
			}  else {
				/* 'default' enter sequence for state Confirmed */
				/* Entry action for state 'Confirmed'. */
				ifaceInternalSCI_OCB->beepArmed();
				ifaceInternalSCI_OCB->ledsArmed(1);
				stateConfVector[0] = main_region_Armed_r1_Idle_idle_Confirmed;
				stateConfVectorPosition = 0;
			}
			historyVector[0] = stateConfVector[0];
		}  else {
			if (ifaceInternalSCI.mode == 2) { 
				/* 'default' enter sequence for state Manual */
				stateConfVector[0] = main_region_Armed_r1_Manual;
				stateConfVectorPosition = 0;
				historyVector[0] = stateConfVector[0];
			}  else {
				if (ifaceInternalSCI.mode == 3) { 
					/* 'default' enter sequence for state Follow */
					stateConfVector[0] = main_region_Armed_r1_Follow;
					stateConfVectorPosition = 0;
					historyVector[0] = stateConfVector[0];
				}  else {
					if (ifaceInternalSCI.mode == 4) { 
						/* 'default' enter sequence for state RTH */
						/* 'default' enter sequence for region r1 */
						/* Default react sequence for initial entry  */
						/* The reactions of state null. */
						if (! iface.confirmed) { 
							/* 'default' enter sequence for state Pending */
							/* Entry action for state 'Pending'. */
							timer->setTimer(this, &timeEvents[1], 250, false);
							ifaceInternalSCI_OCB->beepPending();
							ifaceInternalSCI_OCB->ledsRth(0);
							stateConfVector[0] = main_region_Armed_r1_RTH_r1_Pending;
							stateConfVectorPosition = 0;
						}  else {
							/* 'default' enter sequence for state Confirmed */
							/* Entry action for state 'Confirmed'. */
							ifaceInternalSCI_OCB->beepRth();
							ifaceInternalSCI_OCB->ledsRth(1);
							stateConfVector[0] = main_region_Armed_r1_RTH_r1_Confirmed;
							stateConfVectorPosition = 0;
						}
						historyVector[0] = stateConfVector[0];
					}  else {
						/* 'default' enter sequence for state Pending */
						/* Entry action for state 'Pending'. */
						timer->setTimer(this, &timeEvents[4], 1 * 1000, false);
						ifaceInternalSCI_OCB->beepPending();
						ifaceInternalSCI_OCB->ledsDisarmed(0);
						stateConfVector[0] = main_region_Disarmed_r1_Pending;
						stateConfVectorPosition = 0;
						historyVector[1] = stateConfVector[0];
					}
				}
			}
		}
	}  else {
		if (timeEvents[0]) { 
			/* Default exit sequence for state SelectMode */
			stateConfVector[0] = SysManagerHSM_last_state;
			stateConfVectorPosition = 0;
			/* Exit action for state 'SelectMode'. */
			timer->unsetTimer(this, &timeEvents[0]);
			/* The reactions of state null. */
			if (ifaceInternalSCI.mode == 0) { 
				/* Default react sequence for shallow history entry  */
				/* Enter the region with shallow history */
				if (historyVector[1] != SysManagerHSM_last_state) {
					shenseq_main_region_Disarmed_SequenceImpl();
				} 
			}  else {
				/* Default react sequence for shallow history entry ArmedHistory */
				/* Enter the region with shallow history */
				if (historyVector[0] != SysManagerHSM_last_state) {
					shenseq_SequenceImpl();
				} 
			}
		}  else {
			if (iface.evKeyOkA_raised) { 
				ifaceInternalSCI.tmp = (ifaceInternalSCI.tmp < 4) ? (ifaceInternalSCI.tmp + 1) : 0;
				ifaceInternalSCI_OCB->ledsSelect();
				ifaceInternalSCI_OCB->beepKey();
			} 
		}
	}
}

/* The reactions of state Manual. */
void SysManagerHSM::react_main_region_Armed_r1_Manual() {
	/* The reactions of state Manual. */
	if (iface.evKeyHoldOkB_raised) { 
		/* Default exit sequence for state Armed */
		/* Default exit sequence for region r1 */
		/* Handle exit of all possible states (of r1) at position 0... */
		switch(stateConfVector[ 0 ]) {
			case main_region_Armed_r1_Manual : {
				/* Default exit sequence for state Manual */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_Follow : {
				/* Default exit sequence for state Follow */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_Idle_idle_Pending : {
				/* Default exit sequence for state Pending */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_Idle_idle_Confirmed : {
				/* Default exit sequence for state Confirmed */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_RTH_r1_Pending : {
				/* Default exit sequence for state Pending */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				/* Exit action for state 'Pending'. */
				timer->unsetTimer(this, &timeEvents[1]);
				break;
			}
			case main_region_Armed_r1_RTH_r1_Confirmed : {
				/* Default exit sequence for state Confirmed */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		ifaceInternalSCI.tmp = ifaceInternalSCI.mode;
		/* 'default' enter sequence for state SelectMode */
		/* Entry action for state 'SelectMode'. */
		timer->setTimer(this, &timeEvents[0], ifaceInternalSCI.keyTimeout * 1000, false);
		ifaceInternalSCI_OCB->ledsSelect();
		ifaceInternalSCI_OCB->beepKey();
		stateConfVector[0] = main_region_SelectMode;
		stateConfVectorPosition = 0;
	}  else {
		if (iface.evDisarm_raised) { 
			/* Default exit sequence for state Armed */
			/* Default exit sequence for region r1 */
			/* Handle exit of all possible states (of r1) at position 0... */
			switch(stateConfVector[ 0 ]) {
				case main_region_Armed_r1_Manual : {
					/* Default exit sequence for state Manual */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_Follow : {
					/* Default exit sequence for state Follow */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_Idle_idle_Pending : {
					/* Default exit sequence for state Pending */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_Idle_idle_Confirmed : {
					/* Default exit sequence for state Confirmed */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_RTH_r1_Pending : {
					/* Default exit sequence for state Pending */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					/* Exit action for state 'Pending'. */
					timer->unsetTimer(this, &timeEvents[1]);
					break;
				}
				case main_region_Armed_r1_RTH_r1_Confirmed : {
					/* Default exit sequence for state Confirmed */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				default: break;
			}
			iface.confirmed = false;
			/* 'default' enter sequence for state Disarmed */
			/* 'default' enter sequence for region r1 */
			/* Default react sequence for shallow history entry  */
			/* Enter the region with shallow history */
			if (historyVector[1] != SysManagerHSM_last_state) {
				shenseq_main_region_Disarmed_SequenceImpl();
			} 
		}  else {
			if (iface.evConfirmReq_raised) { 
				/* Default exit sequence for state Armed */
				/* Default exit sequence for region r1 */
				/* Handle exit of all possible states (of r1) at position 0... */
				switch(stateConfVector[ 0 ]) {
					case main_region_Armed_r1_Manual : {
						/* Default exit sequence for state Manual */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						break;
					}
					case main_region_Armed_r1_Follow : {
						/* Default exit sequence for state Follow */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						break;
					}
					case main_region_Armed_r1_Idle_idle_Pending : {
						/* Default exit sequence for state Pending */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						break;
					}
					case main_region_Armed_r1_Idle_idle_Confirmed : {
						/* Default exit sequence for state Confirmed */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						break;
					}
					case main_region_Armed_r1_RTH_r1_Pending : {
						/* Default exit sequence for state Pending */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						/* Exit action for state 'Pending'. */
						timer->unsetTimer(this, &timeEvents[1]);
						break;
					}
					case main_region_Armed_r1_RTH_r1_Confirmed : {
						/* Default exit sequence for state Confirmed */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						break;
					}
					default: break;
				}
				ifaceInternalSCI_OCB->publishRc();
				/* 'default' enter sequence for state Sending */
				/* Entry action for state 'Sending'. */
				timer->setTimer(this, &timeEvents[3], ifaceInternalSCI.ackTimeout * 1000, false);
				stateConfVector[0] = main_region_Sending;
				stateConfVectorPosition = 0;
			} 
		}
	}
}

/* The reactions of state Follow. */
void SysManagerHSM::react_main_region_Armed_r1_Follow() {
	/* The reactions of state Follow. */
	if (iface.evKeyHoldOkB_raised) { 
		/* Default exit sequence for state Armed */
		/* Default exit sequence for region r1 */
		/* Handle exit of all possible states (of r1) at position 0... */
		switch(stateConfVector[ 0 ]) {
			case main_region_Armed_r1_Manual : {
				/* Default exit sequence for state Manual */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_Follow : {
				/* Default exit sequence for state Follow */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_Idle_idle_Pending : {
				/* Default exit sequence for state Pending */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_Idle_idle_Confirmed : {
				/* Default exit sequence for state Confirmed */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_RTH_r1_Pending : {
				/* Default exit sequence for state Pending */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				/* Exit action for state 'Pending'. */
				timer->unsetTimer(this, &timeEvents[1]);
				break;
			}
			case main_region_Armed_r1_RTH_r1_Confirmed : {
				/* Default exit sequence for state Confirmed */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		ifaceInternalSCI.tmp = ifaceInternalSCI.mode;
		/* 'default' enter sequence for state SelectMode */
		/* Entry action for state 'SelectMode'. */
		timer->setTimer(this, &timeEvents[0], ifaceInternalSCI.keyTimeout * 1000, false);
		ifaceInternalSCI_OCB->ledsSelect();
		ifaceInternalSCI_OCB->beepKey();
		stateConfVector[0] = main_region_SelectMode;
		stateConfVectorPosition = 0;
	}  else {
		if (iface.evDisarm_raised) { 
			/* Default exit sequence for state Armed */
			/* Default exit sequence for region r1 */
			/* Handle exit of all possible states (of r1) at position 0... */
			switch(stateConfVector[ 0 ]) {
				case main_region_Armed_r1_Manual : {
					/* Default exit sequence for state Manual */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_Follow : {
					/* Default exit sequence for state Follow */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_Idle_idle_Pending : {
					/* Default exit sequence for state Pending */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_Idle_idle_Confirmed : {
					/* Default exit sequence for state Confirmed */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_RTH_r1_Pending : {
					/* Default exit sequence for state Pending */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					/* Exit action for state 'Pending'. */
					timer->unsetTimer(this, &timeEvents[1]);
					break;
				}
				case main_region_Armed_r1_RTH_r1_Confirmed : {
					/* Default exit sequence for state Confirmed */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				default: break;
			}
			iface.confirmed = false;
			/* 'default' enter sequence for state Disarmed */
			/* 'default' enter sequence for region r1 */
			/* Default react sequence for shallow history entry  */
			/* Enter the region with shallow history */
			if (historyVector[1] != SysManagerHSM_last_state) {
				shenseq_main_region_Disarmed_SequenceImpl();
			} 
		}  else {
			if (iface.evConfirmReq_raised) { 
				/* Default exit sequence for state Armed */
				/* Default exit sequence for region r1 */
				/* Handle exit of all possible states (of r1) at position 0... */
				switch(stateConfVector[ 0 ]) {
					case main_region_Armed_r1_Manual : {
						/* Default exit sequence for state Manual */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						break;
					}
					case main_region_Armed_r1_Follow : {
						/* Default exit sequence for state Follow */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						break;
					}
					case main_region_Armed_r1_Idle_idle_Pending : {
						/* Default exit sequence for state Pending */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						break;
					}
					case main_region_Armed_r1_Idle_idle_Confirmed : {
						/* Default exit sequence for state Confirmed */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						break;
					}
					case main_region_Armed_r1_RTH_r1_Pending : {
						/* Default exit sequence for state Pending */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						/* Exit action for state 'Pending'. */
						timer->unsetTimer(this, &timeEvents[1]);
						break;
					}
					case main_region_Armed_r1_RTH_r1_Confirmed : {
						/* Default exit sequence for state Confirmed */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						break;
					}
					default: break;
				}
				ifaceInternalSCI_OCB->publishProfile();
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
void SysManagerHSM::react_main_region_Armed_r1_Idle_idle_Pending() {
	/* The reactions of state Pending. */
	if (iface.evKeyHoldOkB_raised) { 
		/* Default exit sequence for state Armed */
		/* Default exit sequence for region r1 */
		/* Handle exit of all possible states (of r1) at position 0... */
		switch(stateConfVector[ 0 ]) {
			case main_region_Armed_r1_Manual : {
				/* Default exit sequence for state Manual */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_Follow : {
				/* Default exit sequence for state Follow */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_Idle_idle_Pending : {
				/* Default exit sequence for state Pending */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_Idle_idle_Confirmed : {
				/* Default exit sequence for state Confirmed */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_RTH_r1_Pending : {
				/* Default exit sequence for state Pending */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				/* Exit action for state 'Pending'. */
				timer->unsetTimer(this, &timeEvents[1]);
				break;
			}
			case main_region_Armed_r1_RTH_r1_Confirmed : {
				/* Default exit sequence for state Confirmed */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		ifaceInternalSCI.tmp = ifaceInternalSCI.mode;
		/* 'default' enter sequence for state SelectMode */
		/* Entry action for state 'SelectMode'. */
		timer->setTimer(this, &timeEvents[0], ifaceInternalSCI.keyTimeout * 1000, false);
		ifaceInternalSCI_OCB->ledsSelect();
		ifaceInternalSCI_OCB->beepKey();
		stateConfVector[0] = main_region_SelectMode;
		stateConfVectorPosition = 0;
	}  else {
		if (iface.evDisarm_raised) { 
			/* Default exit sequence for state Armed */
			/* Default exit sequence for region r1 */
			/* Handle exit of all possible states (of r1) at position 0... */
			switch(stateConfVector[ 0 ]) {
				case main_region_Armed_r1_Manual : {
					/* Default exit sequence for state Manual */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_Follow : {
					/* Default exit sequence for state Follow */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_Idle_idle_Pending : {
					/* Default exit sequence for state Pending */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_Idle_idle_Confirmed : {
					/* Default exit sequence for state Confirmed */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_RTH_r1_Pending : {
					/* Default exit sequence for state Pending */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					/* Exit action for state 'Pending'. */
					timer->unsetTimer(this, &timeEvents[1]);
					break;
				}
				case main_region_Armed_r1_RTH_r1_Confirmed : {
					/* Default exit sequence for state Confirmed */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				default: break;
			}
			iface.confirmed = false;
			/* 'default' enter sequence for state Disarmed */
			/* 'default' enter sequence for region r1 */
			/* Default react sequence for shallow history entry  */
			/* Enter the region with shallow history */
			if (historyVector[1] != SysManagerHSM_last_state) {
				shenseq_main_region_Disarmed_SequenceImpl();
			} 
		}  else {
			if (iface.evConfirmReq_raised) { 
				/* Default exit sequence for state Armed */
				/* Default exit sequence for region r1 */
				/* Handle exit of all possible states (of r1) at position 0... */
				switch(stateConfVector[ 0 ]) {
					case main_region_Armed_r1_Manual : {
						/* Default exit sequence for state Manual */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						break;
					}
					case main_region_Armed_r1_Follow : {
						/* Default exit sequence for state Follow */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						break;
					}
					case main_region_Armed_r1_Idle_idle_Pending : {
						/* Default exit sequence for state Pending */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						break;
					}
					case main_region_Armed_r1_Idle_idle_Confirmed : {
						/* Default exit sequence for state Confirmed */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						break;
					}
					case main_region_Armed_r1_RTH_r1_Pending : {
						/* Default exit sequence for state Pending */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						/* Exit action for state 'Pending'. */
						timer->unsetTimer(this, &timeEvents[1]);
						break;
					}
					case main_region_Armed_r1_RTH_r1_Confirmed : {
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
			}
		}
	}
}

/* The reactions of state Confirmed. */
void SysManagerHSM::react_main_region_Armed_r1_Idle_idle_Confirmed() {
	/* The reactions of state Confirmed. */
	if (iface.evKeyHoldOkB_raised) { 
		/* Default exit sequence for state Armed */
		/* Default exit sequence for region r1 */
		/* Handle exit of all possible states (of r1) at position 0... */
		switch(stateConfVector[ 0 ]) {
			case main_region_Armed_r1_Manual : {
				/* Default exit sequence for state Manual */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_Follow : {
				/* Default exit sequence for state Follow */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_Idle_idle_Pending : {
				/* Default exit sequence for state Pending */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_Idle_idle_Confirmed : {
				/* Default exit sequence for state Confirmed */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_RTH_r1_Pending : {
				/* Default exit sequence for state Pending */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				/* Exit action for state 'Pending'. */
				timer->unsetTimer(this, &timeEvents[1]);
				break;
			}
			case main_region_Armed_r1_RTH_r1_Confirmed : {
				/* Default exit sequence for state Confirmed */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		ifaceInternalSCI.tmp = ifaceInternalSCI.mode;
		/* 'default' enter sequence for state SelectMode */
		/* Entry action for state 'SelectMode'. */
		timer->setTimer(this, &timeEvents[0], ifaceInternalSCI.keyTimeout * 1000, false);
		ifaceInternalSCI_OCB->ledsSelect();
		ifaceInternalSCI_OCB->beepKey();
		stateConfVector[0] = main_region_SelectMode;
		stateConfVectorPosition = 0;
	}  else {
		if (iface.evDisarm_raised) { 
			/* Default exit sequence for state Armed */
			/* Default exit sequence for region r1 */
			/* Handle exit of all possible states (of r1) at position 0... */
			switch(stateConfVector[ 0 ]) {
				case main_region_Armed_r1_Manual : {
					/* Default exit sequence for state Manual */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_Follow : {
					/* Default exit sequence for state Follow */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_Idle_idle_Pending : {
					/* Default exit sequence for state Pending */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_Idle_idle_Confirmed : {
					/* Default exit sequence for state Confirmed */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_RTH_r1_Pending : {
					/* Default exit sequence for state Pending */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					/* Exit action for state 'Pending'. */
					timer->unsetTimer(this, &timeEvents[1]);
					break;
				}
				case main_region_Armed_r1_RTH_r1_Confirmed : {
					/* Default exit sequence for state Confirmed */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				default: break;
			}
			iface.confirmed = false;
			/* 'default' enter sequence for state Disarmed */
			/* 'default' enter sequence for region r1 */
			/* Default react sequence for shallow history entry  */
			/* Enter the region with shallow history */
			if (historyVector[1] != SysManagerHSM_last_state) {
				shenseq_main_region_Disarmed_SequenceImpl();
			} 
		}  else {
			if (iface.evConfirmReq_raised) { 
				/* Default exit sequence for state Armed */
				/* Default exit sequence for region r1 */
				/* Handle exit of all possible states (of r1) at position 0... */
				switch(stateConfVector[ 0 ]) {
					case main_region_Armed_r1_Manual : {
						/* Default exit sequence for state Manual */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						break;
					}
					case main_region_Armed_r1_Follow : {
						/* Default exit sequence for state Follow */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						break;
					}
					case main_region_Armed_r1_Idle_idle_Pending : {
						/* Default exit sequence for state Pending */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						break;
					}
					case main_region_Armed_r1_Idle_idle_Confirmed : {
						/* Default exit sequence for state Confirmed */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						break;
					}
					case main_region_Armed_r1_RTH_r1_Pending : {
						/* Default exit sequence for state Pending */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						/* Exit action for state 'Pending'. */
						timer->unsetTimer(this, &timeEvents[1]);
						break;
					}
					case main_region_Armed_r1_RTH_r1_Confirmed : {
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
			}
		}
	}
}

/* The reactions of state Pending. */
void SysManagerHSM::react_main_region_Armed_r1_RTH_r1_Pending() {
	/* The reactions of state Pending. */
	if (iface.evKeyHoldOkB_raised) { 
		/* Default exit sequence for state Armed */
		/* Default exit sequence for region r1 */
		/* Handle exit of all possible states (of r1) at position 0... */
		switch(stateConfVector[ 0 ]) {
			case main_region_Armed_r1_Manual : {
				/* Default exit sequence for state Manual */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_Follow : {
				/* Default exit sequence for state Follow */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_Idle_idle_Pending : {
				/* Default exit sequence for state Pending */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_Idle_idle_Confirmed : {
				/* Default exit sequence for state Confirmed */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_RTH_r1_Pending : {
				/* Default exit sequence for state Pending */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				/* Exit action for state 'Pending'. */
				timer->unsetTimer(this, &timeEvents[1]);
				break;
			}
			case main_region_Armed_r1_RTH_r1_Confirmed : {
				/* Default exit sequence for state Confirmed */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		ifaceInternalSCI.tmp = ifaceInternalSCI.mode;
		/* 'default' enter sequence for state SelectMode */
		/* Entry action for state 'SelectMode'. */
		timer->setTimer(this, &timeEvents[0], ifaceInternalSCI.keyTimeout * 1000, false);
		ifaceInternalSCI_OCB->ledsSelect();
		ifaceInternalSCI_OCB->beepKey();
		stateConfVector[0] = main_region_SelectMode;
		stateConfVectorPosition = 0;
	}  else {
		if (iface.evDisarm_raised) { 
			/* Default exit sequence for state Armed */
			/* Default exit sequence for region r1 */
			/* Handle exit of all possible states (of r1) at position 0... */
			switch(stateConfVector[ 0 ]) {
				case main_region_Armed_r1_Manual : {
					/* Default exit sequence for state Manual */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_Follow : {
					/* Default exit sequence for state Follow */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_Idle_idle_Pending : {
					/* Default exit sequence for state Pending */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_Idle_idle_Confirmed : {
					/* Default exit sequence for state Confirmed */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_RTH_r1_Pending : {
					/* Default exit sequence for state Pending */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					/* Exit action for state 'Pending'. */
					timer->unsetTimer(this, &timeEvents[1]);
					break;
				}
				case main_region_Armed_r1_RTH_r1_Confirmed : {
					/* Default exit sequence for state Confirmed */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				default: break;
			}
			iface.confirmed = false;
			/* 'default' enter sequence for state Disarmed */
			/* 'default' enter sequence for region r1 */
			/* Default react sequence for shallow history entry  */
			/* Enter the region with shallow history */
			if (historyVector[1] != SysManagerHSM_last_state) {
				shenseq_main_region_Disarmed_SequenceImpl();
			} 
		}  else {
			if (timeEvents[1]) { 
				/* Default exit sequence for state Armed */
				/* Default exit sequence for region r1 */
				/* Handle exit of all possible states (of r1) at position 0... */
				switch(stateConfVector[ 0 ]) {
					case main_region_Armed_r1_Manual : {
						/* Default exit sequence for state Manual */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						break;
					}
					case main_region_Armed_r1_Follow : {
						/* Default exit sequence for state Follow */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						break;
					}
					case main_region_Armed_r1_Idle_idle_Pending : {
						/* Default exit sequence for state Pending */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						break;
					}
					case main_region_Armed_r1_Idle_idle_Confirmed : {
						/* Default exit sequence for state Confirmed */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						break;
					}
					case main_region_Armed_r1_RTH_r1_Pending : {
						/* Default exit sequence for state Pending */
						stateConfVector[0] = SysManagerHSM_last_state;
						stateConfVectorPosition = 0;
						/* Exit action for state 'Pending'. */
						timer->unsetTimer(this, &timeEvents[1]);
						break;
					}
					case main_region_Armed_r1_RTH_r1_Confirmed : {
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
			} 
		}
	}
}

/* The reactions of state Confirmed. */
void SysManagerHSM::react_main_region_Armed_r1_RTH_r1_Confirmed() {
	/* The reactions of state Confirmed. */
	if (iface.evKeyHoldOkB_raised) { 
		/* Default exit sequence for state Armed */
		/* Default exit sequence for region r1 */
		/* Handle exit of all possible states (of r1) at position 0... */
		switch(stateConfVector[ 0 ]) {
			case main_region_Armed_r1_Manual : {
				/* Default exit sequence for state Manual */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_Follow : {
				/* Default exit sequence for state Follow */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_Idle_idle_Pending : {
				/* Default exit sequence for state Pending */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_Idle_idle_Confirmed : {
				/* Default exit sequence for state Confirmed */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_RTH_r1_Pending : {
				/* Default exit sequence for state Pending */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				/* Exit action for state 'Pending'. */
				timer->unsetTimer(this, &timeEvents[1]);
				break;
			}
			case main_region_Armed_r1_RTH_r1_Confirmed : {
				/* Default exit sequence for state Confirmed */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		ifaceInternalSCI.tmp = ifaceInternalSCI.mode;
		/* 'default' enter sequence for state SelectMode */
		/* Entry action for state 'SelectMode'. */
		timer->setTimer(this, &timeEvents[0], ifaceInternalSCI.keyTimeout * 1000, false);
		ifaceInternalSCI_OCB->ledsSelect();
		ifaceInternalSCI_OCB->beepKey();
		stateConfVector[0] = main_region_SelectMode;
		stateConfVectorPosition = 0;
	}  else {
		if (iface.evDisarm_raised) { 
			/* Default exit sequence for state Armed */
			/* Default exit sequence for region r1 */
			/* Handle exit of all possible states (of r1) at position 0... */
			switch(stateConfVector[ 0 ]) {
				case main_region_Armed_r1_Manual : {
					/* Default exit sequence for state Manual */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_Follow : {
					/* Default exit sequence for state Follow */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_Idle_idle_Pending : {
					/* Default exit sequence for state Pending */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_Idle_idle_Confirmed : {
					/* Default exit sequence for state Confirmed */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				case main_region_Armed_r1_RTH_r1_Pending : {
					/* Default exit sequence for state Pending */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					/* Exit action for state 'Pending'. */
					timer->unsetTimer(this, &timeEvents[1]);
					break;
				}
				case main_region_Armed_r1_RTH_r1_Confirmed : {
					/* Default exit sequence for state Confirmed */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					break;
				}
				default: break;
			}
			iface.confirmed = false;
			/* 'default' enter sequence for state Disarmed */
			/* 'default' enter sequence for region r1 */
			/* Default react sequence for shallow history entry  */
			/* Enter the region with shallow history */
			if (historyVector[1] != SysManagerHSM_last_state) {
				shenseq_main_region_Disarmed_SequenceImpl();
			} 
		}  else {
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
			if (historyVector[1] != SysManagerHSM_last_state) {
				shenseq_main_region_Disarmed_SequenceImpl();
			} 
		}  else {
			/* Default react sequence for shallow history entry ArmedHistory */
			/* Enter the region with shallow history */
			if (historyVector[0] != SysManagerHSM_last_state) {
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
				if (historyVector[1] != SysManagerHSM_last_state) {
					shenseq_main_region_Disarmed_SequenceImpl();
				} 
			}  else {
				/* Default react sequence for shallow history entry ArmedHistory */
				/* Enter the region with shallow history */
				if (historyVector[0] != SysManagerHSM_last_state) {
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
		/* 'default' enter sequence for state SelectMode */
		/* Entry action for state 'SelectMode'. */
		timer->setTimer(this, &timeEvents[0], ifaceInternalSCI.keyTimeout * 1000, false);
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
				historyVector[1] = stateConfVector[0];
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
		/* 'default' enter sequence for state SelectMode */
		/* Entry action for state 'SelectMode'. */
		timer->setTimer(this, &timeEvents[0], ifaceInternalSCI.keyTimeout * 1000, false);
		ifaceInternalSCI_OCB->ledsSelect();
		ifaceInternalSCI_OCB->beepKey();
		stateConfVector[0] = main_region_SelectMode;
		stateConfVectorPosition = 0;
	}  else {
	}
}


