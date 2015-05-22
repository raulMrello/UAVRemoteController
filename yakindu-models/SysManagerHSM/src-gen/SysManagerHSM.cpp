
#include "SysManagerHSM.h"
#include <string.h>
/*! \file Implementation of the state machine 'SysManagerHSM'
*/

SysManagerHSM::SysManagerHSM() {
	
	ifaceInternalSCI_OCB = null;
	
	for (int i = 0; i < maxHistoryStates; ++i)
		historyVector[i] = SysManagerHSM_last_state;
	
	stateConfVectorPosition = 0;
	
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
	ifaceInternalSCI.mode = 0;
	ifaceInternalSCI.tmp = 0;

}

void SysManagerHSM::enter()
{
	/* Default enter sequence for statechart SysManagerHSM */
	enact_SequenceImpl();
	/* 'default' enter sequence for region main region */
	/* Default react sequence for initial entry  */
	/* 'default' enter sequence for state Disarmed */
	stateConfVector[0] = main_region_Disarmed;
	stateConfVectorPosition = 0;
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
		case main_region_Armed_r1_RTH : {
			/* Default exit sequence for state RTH */
			stateConfVector[0] = SysManagerHSM_last_state;
			stateConfVectorPosition = 0;
			break;
		}
		case main_region_Armed_r1_Idle : {
			/* Default exit sequence for state Idle */
			stateConfVector[0] = SysManagerHSM_last_state;
			stateConfVectorPosition = 0;
			break;
		}
		case main_region_Disarmed : {
			/* Default exit sequence for state Disarmed */
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
		case main_region_Armed_r1_RTH : {
			react_main_region_Armed_r1_RTH();
			break;
		}
		case main_region_Armed_r1_Idle : {
			react_main_region_Armed_r1_Idle();
			break;
		}
		case main_region_Disarmed : {
			react_main_region_Disarmed();
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents();
}

void SysManagerHSM::clearInEvents() {
	iface.ev_hold_ok_b_raised = false;
	iface.ev_timeout_raised = false;
	iface.ev_push_ok_a_raised = false;
	iface.ev_push_ok_b_raised = false;
}

void SysManagerHSM::clearOutEvents() {
}


sc_boolean SysManagerHSM::isActive(SysManagerHSMStates state) {
	switch (state) {
		case main_region_SelectMode : 
			return (sc_boolean) (stateConfVector[0] == main_region_SelectMode
			);
		case main_region_Armed : 
			return (sc_boolean) (stateConfVector[0] >= main_region_Armed
				&& stateConfVector[0] <= main_region_Armed_r1_Idle);
		case main_region_Armed_r1_Manual : 
			return (sc_boolean) (stateConfVector[0] == main_region_Armed_r1_Manual
			);
		case main_region_Armed_r1_Follow : 
			return (sc_boolean) (stateConfVector[0] == main_region_Armed_r1_Follow
			);
		case main_region_Armed_r1_RTH : 
			return (sc_boolean) (stateConfVector[0] == main_region_Armed_r1_RTH
			);
		case main_region_Armed_r1_Idle : 
			return (sc_boolean) (stateConfVector[0] == main_region_Armed_r1_Idle
			);
		case main_region_Disarmed : 
			return (sc_boolean) (stateConfVector[0] == main_region_Disarmed
			);
		default: return false;
	}
}

SysManagerHSM::DefaultSCI* SysManagerHSM::getDefaultSCI() {
	return &iface;
}

void SysManagerHSM::DefaultSCI::raise_ev_hold_ok_b() {
	ev_hold_ok_b_raised = true;
}

void SysManagerHSM::raise_ev_hold_ok_b() {
	iface.raise_ev_hold_ok_b();
}

void SysManagerHSM::DefaultSCI::raise_ev_timeout() {
	ev_timeout_raised = true;
}

void SysManagerHSM::raise_ev_timeout() {
	iface.raise_ev_timeout();
}

void SysManagerHSM::DefaultSCI::raise_ev_push_ok_a() {
	ev_push_ok_a_raised = true;
}

void SysManagerHSM::raise_ev_push_ok_a() {
	iface.raise_ev_push_ok_a();
}

void SysManagerHSM::DefaultSCI::raise_ev_push_ok_b() {
	ev_push_ok_b_raised = true;
}

void SysManagerHSM::raise_ev_push_ok_b() {
	iface.raise_ev_push_ok_b();
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
		case main_region_Armed_r1_RTH : {
			/* 'default' enter sequence for state RTH */
			stateConfVector[0] = main_region_Armed_r1_RTH;
			stateConfVectorPosition = 0;
			historyVector[0] = stateConfVector[0];
			break;
		}
		case main_region_Armed_r1_Idle : {
			/* 'default' enter sequence for state Idle */
			stateConfVector[0] = main_region_Armed_r1_Idle;
			stateConfVectorPosition = 0;
			historyVector[0] = stateConfVector[0];
			break;
		}
		default: break;
	}
}

/* The reactions of state SelectMode. */
void SysManagerHSM::react_main_region_SelectMode() {
	/* The reactions of state SelectMode. */
	if (iface.ev_push_ok_a_raised) { 
		/* Default exit sequence for state SelectMode */
		stateConfVector[0] = SysManagerHSM_last_state;
		stateConfVectorPosition = 0;
		ifaceInternalSCI.tmp = (ifaceInternalSCI.tmp < 4) ? (ifaceInternalSCI.tmp + 1) : 0;
		/* 'default' enter sequence for state SelectMode */
		/* Entry action for state 'SelectMode'. */
		ifaceInternalSCI_OCB->init_mode();
		stateConfVector[0] = main_region_SelectMode;
		stateConfVectorPosition = 0;
	}  else {
		if (iface.ev_push_ok_b_raised) { 
			/* Default exit sequence for state SelectMode */
			stateConfVector[0] = SysManagerHSM_last_state;
			stateConfVectorPosition = 0;
			ifaceInternalSCI.mode = ifaceInternalSCI.tmp;
			/* The reactions of state null. */
			if (ifaceInternalSCI.tmp == 1) { 
				/* 'default' enter sequence for state Idle */
				stateConfVector[0] = main_region_Armed_r1_Idle;
				stateConfVectorPosition = 0;
				historyVector[0] = stateConfVector[0];
			}  else {
				if (ifaceInternalSCI.tmp == 2) { 
					/* 'default' enter sequence for state Manual */
					stateConfVector[0] = main_region_Armed_r1_Manual;
					stateConfVectorPosition = 0;
					historyVector[0] = stateConfVector[0];
				}  else {
					if (ifaceInternalSCI.tmp == 3) { 
						/* 'default' enter sequence for state Follow */
						stateConfVector[0] = main_region_Armed_r1_Follow;
						stateConfVectorPosition = 0;
						historyVector[0] = stateConfVector[0];
					}  else {
						if (ifaceInternalSCI.tmp == 4) { 
							/* 'default' enter sequence for state RTH */
							stateConfVector[0] = main_region_Armed_r1_RTH;
							stateConfVectorPosition = 0;
							historyVector[0] = stateConfVector[0];
						}  else {
							/* 'default' enter sequence for state SelectMode */
							/* Entry action for state 'SelectMode'. */
							ifaceInternalSCI_OCB->init_mode();
							stateConfVector[0] = main_region_SelectMode;
							stateConfVectorPosition = 0;
						}
					}
				}
			}
		}  else {
			if ((iface.ev_timeout_raised) && ifaceInternalSCI.mode > 0) { 
				/* Default exit sequence for state SelectMode */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				/* Default react sequence for shallow history entry  */
				/* Enter the region with shallow history */
				if (historyVector[0] != SysManagerHSM_last_state) {
					shenseq_SequenceImpl();
				} 
			}  else {
				if ((iface.ev_timeout_raised) && ifaceInternalSCI.mode == 0) { 
					/* Default exit sequence for state SelectMode */
					stateConfVector[0] = SysManagerHSM_last_state;
					stateConfVectorPosition = 0;
					/* 'default' enter sequence for state Disarmed */
					stateConfVector[0] = main_region_Disarmed;
					stateConfVectorPosition = 0;
				} 
			}
		}
	}
}

/* The reactions of state Manual. */
void SysManagerHSM::react_main_region_Armed_r1_Manual() {
	/* The reactions of state Manual. */
	if (iface.ev_hold_ok_b_raised) { 
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
			case main_region_Armed_r1_RTH : {
				/* Default exit sequence for state RTH */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_Idle : {
				/* Default exit sequence for state Idle */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		ifaceInternalSCI.tmp = ifaceInternalSCI.mode;
		/* 'default' enter sequence for state SelectMode */
		/* Entry action for state 'SelectMode'. */
		ifaceInternalSCI_OCB->init_mode();
		stateConfVector[0] = main_region_SelectMode;
		stateConfVectorPosition = 0;
	}  else {
	}
}

/* The reactions of state Follow. */
void SysManagerHSM::react_main_region_Armed_r1_Follow() {
	/* The reactions of state Follow. */
	if (iface.ev_hold_ok_b_raised) { 
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
			case main_region_Armed_r1_RTH : {
				/* Default exit sequence for state RTH */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_Idle : {
				/* Default exit sequence for state Idle */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		ifaceInternalSCI.tmp = ifaceInternalSCI.mode;
		/* 'default' enter sequence for state SelectMode */
		/* Entry action for state 'SelectMode'. */
		ifaceInternalSCI_OCB->init_mode();
		stateConfVector[0] = main_region_SelectMode;
		stateConfVectorPosition = 0;
	}  else {
	}
}

/* The reactions of state RTH. */
void SysManagerHSM::react_main_region_Armed_r1_RTH() {
	/* The reactions of state RTH. */
	if (iface.ev_hold_ok_b_raised) { 
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
			case main_region_Armed_r1_RTH : {
				/* Default exit sequence for state RTH */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_Idle : {
				/* Default exit sequence for state Idle */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		ifaceInternalSCI.tmp = ifaceInternalSCI.mode;
		/* 'default' enter sequence for state SelectMode */
		/* Entry action for state 'SelectMode'. */
		ifaceInternalSCI_OCB->init_mode();
		stateConfVector[0] = main_region_SelectMode;
		stateConfVectorPosition = 0;
	}  else {
	}
}

/* The reactions of state Idle. */
void SysManagerHSM::react_main_region_Armed_r1_Idle() {
	/* The reactions of state Idle. */
	if (iface.ev_hold_ok_b_raised) { 
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
			case main_region_Armed_r1_RTH : {
				/* Default exit sequence for state RTH */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			case main_region_Armed_r1_Idle : {
				/* Default exit sequence for state Idle */
				stateConfVector[0] = SysManagerHSM_last_state;
				stateConfVectorPosition = 0;
				break;
			}
			default: break;
		}
		ifaceInternalSCI.tmp = ifaceInternalSCI.mode;
		/* 'default' enter sequence for state SelectMode */
		/* Entry action for state 'SelectMode'. */
		ifaceInternalSCI_OCB->init_mode();
		stateConfVector[0] = main_region_SelectMode;
		stateConfVectorPosition = 0;
	}  else {
	}
}

/* The reactions of state Disarmed. */
void SysManagerHSM::react_main_region_Disarmed() {
	/* The reactions of state Disarmed. */
	if (iface.ev_hold_ok_b_raised) { 
		/* Default exit sequence for state Disarmed */
		stateConfVector[0] = SysManagerHSM_last_state;
		stateConfVectorPosition = 0;
		ifaceInternalSCI.tmp = ifaceInternalSCI.mode;
		/* 'default' enter sequence for state SelectMode */
		/* Entry action for state 'SelectMode'. */
		ifaceInternalSCI_OCB->init_mode();
		stateConfVector[0] = main_region_SelectMode;
		stateConfVectorPosition = 0;
	} 
}


