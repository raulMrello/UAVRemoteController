/*
 * KeyDecoder.h
 *
 *  Created on: 20/4/2015
 *      Author: raulMrello
 *
 *  KeyDecoder is an Active module (running as a Task) that will decode keyboard input
 *	combination by interrupts. Each time an interrupt is decode, a topic data structure
 *	(type "/keyb") is updated and published. It can recognise 3 different events: pressed,
 *	repeated and released.
 */

#ifndef SRC_ACTIVE_MODULES_KEYDECODER_H_
#define SRC_ACTIVE_MODULES_KEYDECODER_H_


//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "mbed.h"
#include "rtos.h"
#include "MsgBroker.h"
#include "Topics.h"
#include "KeyDecoderHSM.h"
#include "MbedTimerInterface.h"

//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//-- CLASS ---------------------------------------------------------------------------
//------------------------------------------------------------------------------------

class KeyDecoder : public KeyDecoderHSM, public KeyDecoderHSM::DefaultSCI_OCB{
public:
	
	/** Constructor, destructor, getter and setter */
	KeyDecoder(	osPriority prio, 
				InterruptIn *ii_A_Ok,
				InterruptIn *ii_B_Ok,
				InterruptIn *ii_ARM,
				InterruptIn *ii_LOC,
				InterruptIn *ii_ALT,
				InterruptIn *ii_RTH,
				bool enableRepeatedEvt = false);
	
	virtual ~KeyDecoder();
	Thread *getThread();
	
	/** Input Interrupt callbacks */
	void KeyPressedISRCallback(void);
	void KeyReleasedISRCallback(void);
	
	/** Task */
	static void task(void const *arg){
		KeyDecoder *me = (KeyDecoder*)arg;
		me->run();
	}	

	/** StateMachine callback interface */
	virtual sc_integer readKey();
	virtual void publish(sc_integer key, sc_boolean isHold);
	
private:
	Topic::KeyData_t _keydata;
	Thread *_th;
	InterruptIn *_ii_A_Ok;
	InterruptIn *_ii_B_Ok;
	InterruptIn *_ii_ARM;
	InterruptIn *_ii_LOC;
	InterruptIn *_ii_ALT;
	InterruptIn *_ii_RTH;
	int32_t _signals;
	uint32_t _currentkey;
	uint32_t _timeout;

	void run();
	uint32_t readKeyboard();

	/** Key event enumeration */
	typedef enum {
		KEY_EV_PRESSED 	= (1 << 0),
		KEY_EV_RELEASED = (1 << 1),
		TIMMING_EV 		= (1 << 2)
	}EventEnum;	
	
	MbedTimerInterface	*_titmr;
};


#endif /* SRC_ACTIVE_MODULES_KEYDECODER_H_ */
