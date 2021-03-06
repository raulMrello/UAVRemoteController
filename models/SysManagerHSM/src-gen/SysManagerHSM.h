
#ifndef SYSMANAGERHSM_H_
#define SYSMANAGERHSM_H_

#include "sc_types.h"
#include "StatemachineInterface.h"
#include "TimedStatemachineInterface.h"

/*! \file Header of the state machine 'SysManagerHSM'.
*/

class SysManagerHSM : public TimedStatemachineInterface, public StatemachineInterface {
	
	public:
		
		SysManagerHSM();
		
		~SysManagerHSM();
		
		//! enumeration of all states 
		typedef enum {
			main_region_SelectMode,
			main_region_Armed,
			main_region_Error,
			main_region_Sending,
			main_region_Disarmed,
			main_region_Disarmed_r1_Pending,
			main_region_Disarmed_r1_Confirmed,
			r1_RTH,
			r1_RTH_r1_Pending,
			r1_RTH_r1_Confirmed,
			r1_Follow,
			r1_Follow_r1_Pending,
			r1_Follow_r1_Confirmed,
			r1_Idle,
			r1_Idle_r1_Pending,
			r1_Idle_r1_Confirmed,
			r1_Manual,
			r1_Manual_r1_Confirmed,
			r1_Manual_r1_Pending,
			r1_Select,
			SysManagerHSM_last_state
		} SysManagerHSMStates;
		
		//! Inner class for default interface scope.
		class DefaultSCI {
			
			public:
				/*! Raises the in event 'evKeyHoldOkB' that is defined in the default interface scope. */ 
				void raise_evKeyHoldOkB();
				
				/*! Raises the in event 'evKeyOkA' that is defined in the default interface scope. */ 
				void raise_evKeyOkA();
				
				/*! Raises the in event 'evKeyOkB' that is defined in the default interface scope. */ 
				void raise_evKeyOkB();
				
				/*! Raises the in event 'evDisarm' that is defined in the default interface scope. */ 
				void raise_evDisarm();
				
				/*! Raises the in event 'evAck' that is defined in the default interface scope. */ 
				void raise_evAck();
				
				/*! Raises the in event 'evNack' that is defined in the default interface scope. */ 
				void raise_evNack();
				
				/*! Raises the in event 'evConfirmReq' that is defined in the default interface scope. */ 
				void raise_evConfirmReq();
				
				/*! Raises the in event 'evDisarmed' that is defined in the default interface scope. */ 
				void raise_evDisarmed();
				
				/*! Raises the in event 'evJoysHold' that is defined in the default interface scope. */ 
				void raise_evJoysHold();
				
				/*! Raises the in event 'evJoysMove' that is defined in the default interface scope. */ 
				void raise_evJoysMove();
				
				/*! Gets the value of the variable 'confirmed' that is defined in the default interface scope. */ 
				sc_boolean get_confirmed();
				
				/*! Sets the value of the variable 'confirmed' that is defined in the default interface scope. */ 
				void set_confirmed(sc_boolean value);
				
				
			private:
				friend class SysManagerHSM;
				sc_boolean evKeyHoldOkB_raised;
				sc_boolean evKeyOkA_raised;
				sc_boolean evKeyOkB_raised;
				sc_boolean evDisarm_raised;
				sc_boolean evAck_raised;
				sc_boolean evNack_raised;
				sc_boolean evConfirmReq_raised;
				sc_boolean evDisarmed_raised;
				sc_boolean evJoysHold_raised;
				sc_boolean evJoysMove_raised;
				sc_boolean confirmed;
		};
		
		
		/*! Returns an instance of the interface class 'DefaultSCI'. */
		DefaultSCI* getDefaultSCI();
		
		/*! Raises the in event 'evKeyHoldOkB' that is defined in the default interface scope. */ 
		void raise_evKeyHoldOkB();
		
		/*! Raises the in event 'evKeyOkA' that is defined in the default interface scope. */ 
		void raise_evKeyOkA();
		
		/*! Raises the in event 'evKeyOkB' that is defined in the default interface scope. */ 
		void raise_evKeyOkB();
		
		/*! Raises the in event 'evDisarm' that is defined in the default interface scope. */ 
		void raise_evDisarm();
		
		/*! Raises the in event 'evAck' that is defined in the default interface scope. */ 
		void raise_evAck();
		
		/*! Raises the in event 'evNack' that is defined in the default interface scope. */ 
		void raise_evNack();
		
		/*! Raises the in event 'evConfirmReq' that is defined in the default interface scope. */ 
		void raise_evConfirmReq();
		
		/*! Raises the in event 'evDisarmed' that is defined in the default interface scope. */ 
		void raise_evDisarmed();
		
		/*! Raises the in event 'evJoysHold' that is defined in the default interface scope. */ 
		void raise_evJoysHold();
		
		/*! Raises the in event 'evJoysMove' that is defined in the default interface scope. */ 
		void raise_evJoysMove();
		
		/*! Gets the value of the variable 'confirmed' that is defined in the default interface scope. */ 
		sc_boolean get_confirmed();
		
		/*! Sets the value of the variable 'confirmed' that is defined in the default interface scope. */ 
		void set_confirmed(sc_boolean value);
		
		
		//! Inner class for internal interface scope operation callbacks.
		class InternalSCI_OCB {
			public:
				virtual ~InternalSCI_OCB() = 0;
				
				virtual void beepKey() = 0;
				
				virtual void beepSelect() = 0;
				
				virtual void beepAck() = 0;
				
				virtual void beepError() = 0;
				
				virtual void beepDisarmed() = 0;
				
				virtual void beepArmed() = 0;
				
				virtual void beepPending() = 0;
				
				virtual void beepManual() = 0;
				
				virtual void beepFollow() = 0;
				
				virtual void beepRth() = 0;
				
				virtual void ledsDisarmed(sc_integer ack) = 0;
				
				virtual void ledsSelect() = 0;
				
				virtual void ledsArmed(sc_integer ack) = 0;
				
				virtual void ledsError() = 0;
				
				virtual void ledsManual(sc_integer ack) = 0;
				
				virtual void ledsFollow(sc_integer ack) = 0;
				
				virtual void ledsRth(sc_integer ack) = 0;
				
				virtual void publishMode() = 0;
				
				virtual void publishRc() = 0;
				
				virtual void publishProfile() = 0;
				
				virtual void publishKAT() = 0;
				
				virtual void getAction() = 0;
		};
		
		/*! Set the working instance of the operation callback interface 'InternalSCI_OCB'. */
		void setInternalSCI_OCB(InternalSCI_OCB* operationCallback);
		
		void init();
		
		void enter();
		
		void exit();
		
		void runCycle();
		
		void setTimer(TimerInterface* timer);
		
		TimerInterface* getTimer();
		
		void raiseTimeEvent(sc_eventid event);
		
		/*! Checks if the specified state is active. */
		sc_boolean isActive(SysManagerHSMStates state);
	
	private:
	
		//! Inner class for internal interface scope.
		class InternalSCI {
			
			public:
				/*! Raises the in event 'evInit' that is defined in the internal scope. */ 
				void raise_evInit();
				
				/*! Checks if the out event 'evInit' that is defined in the internal scope has been raised. */ 
				sc_boolean isRaised_evInit();
				
				/*! Gets the value of the variable 'touch' that is defined in the internal scope. */ 
				sc_boolean get_touch();
				
				/*! Sets the value of the variable 'touch' that is defined in the internal scope. */ 
				void set_touch(sc_boolean value);
				
				/*! Gets the value of the variable 'sent' that is defined in the internal scope. */ 
				sc_boolean get_sent();
				
				/*! Sets the value of the variable 'sent' that is defined in the internal scope. */ 
				void set_sent(sc_boolean value);
				
				/*! Gets the value of the variable 'mode' that is defined in the internal scope. */ 
				sc_integer get_mode();
				
				/*! Sets the value of the variable 'mode' that is defined in the internal scope. */ 
				void set_mode(sc_integer value);
				
				/*! Gets the value of the variable 'tmp' that is defined in the internal scope. */ 
				sc_integer get_tmp();
				
				/*! Sets the value of the variable 'tmp' that is defined in the internal scope. */ 
				void set_tmp(sc_integer value);
				
				/*! Gets the value of the variable 'keyTimeout' that is defined in the internal scope. */ 
				sc_integer get_keyTimeout();
				
				/*! Sets the value of the variable 'keyTimeout' that is defined in the internal scope. */ 
				void set_keyTimeout(sc_integer value);
				
				/*! Gets the value of the variable 'ackTimeout' that is defined in the internal scope. */ 
				sc_integer get_ackTimeout();
				
				/*! Sets the value of the variable 'ackTimeout' that is defined in the internal scope. */ 
				void set_ackTimeout(sc_integer value);
				
				/*! Gets the value of the variable 'keepAliveTimeout' that is defined in the internal scope. */ 
				sc_integer get_keepAliveTimeout();
				
				/*! Sets the value of the variable 'keepAliveTimeout' that is defined in the internal scope. */ 
				void set_keepAliveTimeout(sc_integer value);
				
				
			private:
				friend class SysManagerHSM;
				sc_boolean evInit_raised;
				sc_boolean touch;
				sc_boolean sent;
				sc_integer mode;
				sc_integer tmp;
				sc_integer keyTimeout;
				sc_integer ackTimeout;
				sc_integer keepAliveTimeout;
		};
	
		//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
		static const sc_integer maxOrthogonalStates = 2;
		//! dimension of the state configuration vector for history states
		static const sc_integer maxHistoryStates = 2;
		
		TimerInterface* timer;
		sc_boolean timeEvents[14];
		
		SysManagerHSMStates stateConfVector[maxOrthogonalStates];
		
		SysManagerHSMStates historyVector[maxHistoryStates];
		sc_ushort stateConfVectorPosition;
		
		DefaultSCI iface;
		InternalSCI ifaceInternalSCI;
		InternalSCI_OCB* ifaceInternalSCI_OCB;
		
		// prototypes of all internal functions
		
		void enact_SequenceImpl();
		void exact_SequenceImpl();
		void shenseq_main_region_Disarmed_SequenceImpl();
		void shenseq_SequenceImpl();
		void react_main_region_SelectMode();
		void react_main_region_Armed();
		void react_main_region_Error();
		void react_main_region_Sending();
		void react_main_region_Disarmed_r1_Pending();
		void react_main_region_Disarmed_r1_Confirmed();
		void react_r1_RTH_r1_Pending();
		void react_r1_RTH_r1_Confirmed();
		void react_r1_Follow_r1_Pending();
		void react_r1_Follow_r1_Confirmed();
		void react_r1_Idle_r1_Pending();
		void react_r1_Idle_r1_Confirmed();
		void react_r1_Manual_r1_Confirmed();
		void react_r1_Manual_r1_Pending();
		void react_r1_Select();
		void clearInEvents();
		void clearOutEvents();
		
};
inline SysManagerHSM::InternalSCI_OCB::~InternalSCI_OCB() {}
#endif /* SYSMANAGERHSM_H_ */
