
#ifndef KEYDECODERHSM_H_
#define KEYDECODERHSM_H_

#include "sc_types.h"
#include "StatemachineInterface.h"
#include "TimedStatemachineInterface.h"

/*! \file Header of the state machine 'KeyDecoderHSM'.
*/

class KeyDecoderHSM : public TimedStatemachineInterface, public StatemachineInterface {
	
	public:
		
		KeyDecoderHSM();
		
		~KeyDecoderHSM();
		
		//! enumeration of all states 
		typedef enum {
			main_region_Inactive,
			main_region_Active,
			main_region_Active_r1_Pressed,
			main_region_Active_r1_Hold,
			KeyDecoderHSM_last_state
		} KeyDecoderHSMStates;
		
		//! Inner class for default interface scope.
		class DefaultSCI {
			
			public:
				/*! Raises the in event 'evPressed' that is defined in the default interface scope. */ 
				void raise_evPressed();
				
				/*! Raises the in event 'evReleased' that is defined in the default interface scope. */ 
				void raise_evReleased();
				
				
			private:
				friend class KeyDecoderHSM;
				sc_boolean evPressed_raised;
				sc_boolean evReleased_raised;
		};
		
		//! Inner class for default interface scope operation callbacks.
		class DefaultSCI_OCB {
			public:
				virtual ~DefaultSCI_OCB() = 0;
				
				virtual sc_integer readKey() = 0;
				
				virtual void publish(sc_integer key, sc_boolean isHold) = 0;
		};
		
		/*! Set the working instance of the operation callback interface 'DefaultSCI_OCB'. */
		void setDefaultSCI_OCB(DefaultSCI_OCB* operationCallback);
		
		/*! Returns an instance of the interface class 'DefaultSCI'. */
		DefaultSCI* getDefaultSCI();
		
		/*! Raises the in event 'evPressed' that is defined in the default interface scope. */ 
		void raise_evPressed();
		
		/*! Raises the in event 'evReleased' that is defined in the default interface scope. */ 
		void raise_evReleased();
		
		
		void init();
		
		void enter();
		
		void exit();
		
		void runCycle();
		
		void setTimer(TimerInterface* timer);
		
		TimerInterface* getTimer();
		
		void raiseTimeEvent(sc_eventid event);
		
		/*! Checks if the specified state is active. */
		sc_boolean isActive(KeyDecoderHSMStates state);
	
	private:
	
	
		//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
		static const sc_integer maxOrthogonalStates = 1;
		
		TimerInterface* timer;
		sc_boolean timeEvents[1];
		
		KeyDecoderHSMStates stateConfVector[maxOrthogonalStates];
		
		sc_ushort stateConfVectorPosition;
		
		DefaultSCI iface;
		DefaultSCI_OCB* iface_OCB;
		
		// prototypes of all internal functions
		
		void enact_SequenceImpl();
		void exact_SequenceImpl();
		void react_main_region_Inactive();
		void react_main_region_Active_r1_Pressed();
		void react_main_region_Active_r1_Hold();
		void clearInEvents();
		void clearOutEvents();
		
};
inline KeyDecoderHSM::DefaultSCI_OCB::~DefaultSCI_OCB() {}
#endif /* KEYDECODERHSM_H_ */
