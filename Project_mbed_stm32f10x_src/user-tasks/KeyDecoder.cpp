/*
 * KeyDecoder.cpp
 *
 *  Created on: 20/04/2015
 *      Author: raulMrello
 */

#include "KeyDecoder.h"

//------------------------------------------------------------------------------------
//-- PRIVATE TYPEDEFS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//-- PRIVATE DEFINITIONS -------------------------------------------------------------
//------------------------------------------------------------------------------------

/** Milliseconds timeout to publish a /key update when a key is pressed */
#define REPEAT_TIMEOUT	500

/** Keyboard codes */
#define KEY_NONE 0u
#define KEY_A_OK	(uint32_t)(1 << 0)
#define KEY_B_OK	(uint32_t)(1 << 1)
#define KEY_ARM		(uint32_t)(1 << 2)
#define KEY_LOC		(uint32_t)(1 << 3)
#define KEY_ALT		(uint32_t)(1 << 4)
#define KEY_RTH		(uint32_t)(1 << 5)

//------------------------------------------------------------------------------------
//-- STATIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

KeyDecoder::KeyDecoder(	osPriority prio, InterruptIn *ii_A_Ok, InterruptIn *ii_B_Ok,
						InterruptIn *ii_ARM, InterruptIn *ii_LOC, InterruptIn *ii_ALT, InterruptIn *ii_RTH, bool enableRepeatedEvt) {

	_enableRepeated = enableRepeatedEvt;
	ii_A_Ok->fall(this, &KeyDecoder::KeyPressedISRCallback);
	ii_A_Ok->rise(this, &KeyDecoder::KeyReleasedISRCallback);
	ii_A_Ok->disable_irq();
	ii_A_Ok->mode(PullUp);
	_ii_A_Ok = ii_A_Ok;

	ii_B_Ok->fall(this, &KeyDecoder::KeyPressedISRCallback);
	ii_B_Ok->rise(this, &KeyDecoder::KeyReleasedISRCallback);
	ii_B_Ok->disable_irq();
	ii_B_Ok->mode(PullUp);
	_ii_B_Ok = ii_B_Ok;
							
	ii_ARM->fall(this, &KeyDecoder::KeyPressedISRCallback);
	ii_ARM->rise(this, &KeyDecoder::KeyReleasedISRCallback);
	ii_ARM->disable_irq();
	ii_ARM->mode(PullUp);
	_ii_ARM = ii_ARM;

	ii_LOC->fall(this, &KeyDecoder::KeyPressedISRCallback);
	ii_LOC->rise(this, &KeyDecoder::KeyReleasedISRCallback);
	ii_LOC->disable_irq();
	ii_LOC->mode(PullUp);
	_ii_LOC = ii_LOC;

	ii_ALT->fall(this, &KeyDecoder::KeyPressedISRCallback);
	ii_ALT->rise(this, &KeyDecoder::KeyReleasedISRCallback);
	ii_ALT->disable_irq();
	ii_ALT->mode(PullUp);
	_ii_ALT = ii_ALT;

	ii_RTH->fall(this, &KeyDecoder::KeyPressedISRCallback);
	ii_RTH->rise(this, &KeyDecoder::KeyReleasedISRCallback);
	ii_RTH->disable_irq();
	ii_RTH->mode(PullUp);			
	_ii_RTH = ii_RTH;
	
	_th = 0;
	_th = new Thread(&KeyDecoder::task, this, prio);
}

//------------------------------------------------------------------------------------
KeyDecoder::~KeyDecoder() {
	_ii_A_Ok->disable_irq();
	_ii_B_Ok->disable_irq();
	_ii_ARM->disable_irq();
	_ii_LOC->disable_irq();
	_ii_ALT->disable_irq();
	_ii_RTH->disable_irq();
}

//------------------------------------------------------------------------------------
Thread * KeyDecoder::getThread() {
	return _th;
}

//------------------------------------------------------------------------------------
void KeyDecoder::KeyPressedISRCallback(void){
	_th->signal_set(KEY_EV_PRESSED);
}

//------------------------------------------------------------------------------------
void KeyDecoder::KeyReleasedISRCallback(void){
	_th->signal_set(KEY_EV_RELEASED);
}

//------------------------------------------------------------------------------------
void KeyDecoder::run(){
	while(_th == 0){
		Thread::wait(100);
	}
	// waits till all keys are released
	do{
		_currentkey = readKeyboard();
	}while(_currentkey != KEY_NONE);
	_lastkey = _currentkey;
	_ii_A_Ok->enable_irq();
	_ii_B_Ok->enable_irq();
	_ii_ARM->enable_irq();
	_ii_LOC->enable_irq();
	_ii_ALT->enable_irq();
	_ii_RTH->enable_irq();
	_timeout = osWaitForever;
	// Starts execution 
	for(;;){
		bool publish = false;
		// Wait input changes ... 
		osEvent oe = _th->signal_wait((KEY_EV_PRESSED |KEY_EV_RELEASED), _timeout);
		// debounce timer
		Thread::wait(50);
		// if key pressed, read keyboard, update topic and enable publishing and repeated events
		if(oe.status == osEventSignal && (oe.value.signals & (KEY_EV_PRESSED|KEY_EV_RELEASED)) != 0){		
			_th->signal_clr(KEY_EV_PRESSED|KEY_EV_RELEASED);
			_currentkey = readKeyboard();
			_keydata.data.keycode = (_currentkey ^ _lastkey);
			publish = true;		
			_timeout = osWaitForever;
			if(_enableRepeated && _currentkey != KEY_NONE){
				_timeout = REPEAT_TIMEOUT;			
			}
		}
		// if repeated event, enables publishing
		if(oe.status == osEventTimeout){
			_currentkey = readKeyboard();
			publish = true;	
			_timeout = REPEAT_TIMEOUT;
			if(!_currentkey){
				_timeout = osWaitForever;
			}
		}
		// if publishing enabled, publish topic update
		if(publish){
			MsgBroker::Exception e = MsgBroker::NO_ERRORS;
			MsgBroker::publish("/keyb", &_keydata, sizeof(Topic::KeyData_t), &e);
			if(e != MsgBroker::NO_ERRORS){
				// TODO: add error handling ...
			}
		}
	}	
}

//------------------------------------------------------------------------------------
//-- PROTECTED/PRIVATE FUNCTIONS -----------------------------------------------------
//------------------------------------------------------------------------------------

uint32_t KeyDecoder::readKeyboard(){
	uint32_t result = KEY_NONE;
	if(_ii_A_Ok->read() == 0)
		result |= KEY_A_OK;
	if(_ii_B_Ok->read() == 0)
		result |= KEY_B_OK;
	if(_ii_ARM->read() == 0)
		result |= KEY_ARM;
	if(_ii_LOC->read() == 0)
		result |= KEY_LOC;
	if(_ii_ALT->read() == 0)
		result |= KEY_ALT;
	if(_ii_RTH->read() == 0)
		result |= KEY_RTH;
	return result;
}

