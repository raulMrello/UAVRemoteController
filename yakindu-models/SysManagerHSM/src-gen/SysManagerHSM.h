
#ifndef SYSMANAGERHSM_H_
#define SYSMANAGERHSM_H_

#include "sc_types.h"
#include "StatemachineInterface.h"

/*! \file Header of the state machine 'SysManagerHSM'.
*/

class SysManagerHSM : public StatemachineInterface {
	
	public:
		
		SysManagerHSM();
		
		~SysManagerHSM();
		
		//! enumeration of all states 
		typedef enum {
			main_region_SelectMode,
			main_region_Armed,
			main_region_Armed_r1_Manual,
			main_region_Armed_r1_Follow,
			main_region_Armed_r1_RTH,
			main_region_Armed_r1_Idle,
			main_region_Disarmed,
			SysManagerHSM_last_state
		} SysManagerHSMStates;
		
		//! Inner class for default interface scope.
		class DefaultSCI {
			
			public:
				/*! Raises the in event 'ev_hold_ok_b' that is defined in the default interface scope. */ 
				void raise_ev_hold_ok_b();
				
				/*! Raises the in event 'ev_timeout' that is defined in the default interface scope. */ 
				void raise_ev_timeout();
				
				/*! Raises the in event 'ev_push_ok_a' that is defined in the default interface scope. */ 
				void raise_ev_push_ok_a();
				
				/*! Raises the in event 'ev_push_ok_b' that is defined in the default interface scope. */ 
				void raise_ev_push_ok_b();
				
				
			private:
				friend class SysManagerHSM;
				sc_boolean ev_hold_ok_b_raised;
				sc_boolean ev_timeout_raised;
				sc_boolean ev_push_ok_a_raised;
				sc_boolean ev_push_ok_b_raised;
		};
		
		
		/*! Returns an instance of the interface class 'DefaultSCI'. */
		DefaultSCI* getDefaultSCI();
		
		/*! Raises the in event 'ev_hold_ok_b' that is defined in the default interface scope. */ 
		void raise_ev_hold_ok_b();
		
		/*! Raises the in event 'ev_timeout' that is defined in the default interface scope. */ 
		void raise_ev_timeout();
		
		/*! Raises the in event 'ev_push_ok_a' that is defined in the default interface scope. */ 
		void raise_ev_push_ok_a();
		
		/*! Raises the in event 'ev_push_ok_b' that is defined in the default interface scope. */ 
		void raise_ev_push_ok_b();
		
		
		//! Inner class for internal interface scope operation callbacks.
		class InternalSCI_OCB {
			public:
				virtual ~InternalSCI_OCB() = 0;
				
				virtual void init_mode() = 0;
				
				virtual void next_mode() = 0;
				
				virtual void set_mode() = 0;
		};
		
		/*! Set the working instance of the operation callback interface 'InternalSCI_OCB'. */
		void setInternalSCI_OCB(InternalSCI_OCB* operationCallback);
		
		void init();
		
		void enter();
		
		void exit();
		
		void runCycle();
		
		
		/*! Checks if the specified state is active. */
		sc_boolean isActive(SysManagerHSMStates state);
	
	private:
	
		//! Inner class for internal interface scope.
		class InternalSCI {
			
			public:
				/*! Gets the value of the variable 'mode' that is defined in the internal scope. */ 
				sc_integer get_mode();
				
				/*! Sets the value of the variable 'mode' that is defined in the internal scope. */ 
				void set_mode(sc_integer value);
				
				/*! Gets the value of the variable 'tmp' that is defined in the internal scope. */ 
				sc_integer get_tmp();
				
				/*! Sets the value of the variable 'tmp' that is defined in the internal scope. */ 
				void set_tmp(sc_integer value);
				
				
			private:
				friend class SysManagerHSM;
				sc_integer mode;
				sc_integer tmp;
		};
	
		//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
		static const sc_integer maxOrthogonalStates = 1;
		//! dimension of the state configuration vector for history states
		static const sc_integer maxHistoryStates = 1;
		
		
		SysManagerHSMStates stateConfVector[maxOrthogonalStates];
		
		SysManagerHSMStates historyVector[maxHistoryStates];
		sc_ushort stateConfVectorPosition;
		
		DefaultSCI iface;
		InternalSCI ifaceInternalSCI;
		InternalSCI_OCB* ifaceInternalSCI_OCB;
		
		// prototypes of all internal functions
		
		void enact_SequenceImpl();
		void exact_SequenceImpl();
		void shenseq_SequenceImpl();
		void react_main_region_SelectMode();
		void react_main_region_Armed_r1_Manual();
		void react_main_region_Armed_r1_Follow();
		void react_main_region_Armed_r1_RTH();
		void react_main_region_Armed_r1_Idle();
		void react_main_region_Disarmed();
		void clearInEvents();
		void clearOutEvents();
		
};
inline SysManagerHSM::InternalSCI_OCB::~InternalSCI_OCB() {}
#endif /* SYSMANAGERHSM_H_ */
