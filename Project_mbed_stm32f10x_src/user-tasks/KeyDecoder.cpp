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
#define REPEAT_TIMEOUT	200

//------------------------------------------------------------------------------------
//-- STATIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

KeyDecoder::KeyDecoder(	osPriority prio, InterruptIn *ii_N, InterruptIn *ii_NE,
						InterruptIn *ii_E, InterruptIn *ii_ES, InterruptIn *ii_S,
						InterruptIn *ii_SW, InterruptIn *ii_W, InterruptIn *ii_WN,
						InterruptIn *ii_ARM, InterruptIn *ii_LOC, InterruptIn *ii_ALT,
						InterruptIn *ii_RTH) {
	_th = new Thread(&KeyDecoder::task, this, prio);
							
	ii_N->fall(this, &KeyDecoder::KeyPressedISRCallback);
	ii_N->rise(this, &KeyDecoder::KeyReleasedISRCallback);
	ii_N->disable_irq();
	_ii_N = ii_N;

	ii_NE->fall(this, &KeyDecoder::KeyPressedISRCallback);
	ii_NE->rise(this, &KeyDecoder::KeyReleasedISRCallback);
	ii_NE->disable_irq();
	_ii_NE = ii_NE;
							
	ii_E->fall(this, &KeyDecoder::KeyPressedISRCallback);
	ii_E->rise(this, &KeyDecoder::KeyReleasedISRCallback);
	ii_E->disable_irq();
	_ii_E = ii_E;

	ii_ES->fall(this, &KeyDecoder::KeyPressedISRCallback);
	ii_ES->rise(this, &KeyDecoder::KeyReleasedISRCallback);
	ii_ES->disable_irq();
	_ii_ES = ii_ES;

	ii_S->fall(this, &KeyDecoder::KeyPressedISRCallback);
	ii_S->rise(this, &KeyDecoder::KeyReleasedISRCallback);
	ii_S->disable_irq();
	_ii_S = ii_S;

	ii_SW->fall(this, &KeyDecoder::KeyPressedISRCallback);
	ii_SW->rise(this, &KeyDecoder::KeyReleasedISRCallback);
	ii_SW->disable_irq();
	_ii_SW = ii_SW;

	ii_W->fall(this, &KeyDecoder::KeyPressedISRCallback);
	ii_W->rise(this, &KeyDecoder::KeyReleasedISRCallback);
	ii_W->disable_irq();
	_ii_W = ii_W;

	ii_WN->fall(this, &KeyDecoder::KeyPressedISRCallback);
	ii_WN->rise(this, &KeyDecoder::KeyReleasedISRCallback);
	ii_WN->disable_irq();
	_ii_WN = ii_WN;

	ii_ARM->fall(this, &KeyDecoder::KeyPressedISRCallback);
	ii_ARM->rise(this, &KeyDecoder::KeyReleasedISRCallback);
	ii_ARM->disable_irq();
	_ii_ARM = ii_ARM;

	ii_LOC->fall(this, &KeyDecoder::KeyPressedISRCallback);
	ii_LOC->rise(this, &KeyDecoder::KeyReleasedISRCallback);
	ii_LOC->disable_irq();
	_ii_LOC = ii_LOC;

	ii_ALT->fall(this, &KeyDecoder::KeyPressedISRCallback);
	ii_ALT->rise(this, &KeyDecoder::KeyReleasedISRCallback);
	ii_ALT->disable_irq();
	_ii_ALT = ii_ALT;

	ii_RTH->fall(this, &KeyDecoder::KeyPressedISRCallback);
	ii_RTH->rise(this, &KeyDecoder::KeyReleasedISRCallback);
	ii_RTH->disable_irq();
	_ii_RTH = ii_RTH;

}

//------------------------------------------------------------------------------------
KeyDecoder::~KeyDecoder() {
	_ii_N->disable_irq();
	_ii_NE->disable_irq();
	_ii_E->disable_irq();
	_ii_ES->disable_irq();
	_ii_S->disable_irq();
	_ii_SW->disable_irq();
	_ii_W->disable_irq();
	_ii_WN->disable_irq();
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
	// waits till all keys are released
	do{
		_currentkey = readKeyboard();
	}while(_currentkey != 0);
	_lastkey = _currentkey;
	
	// Enables input interrupts
	_ii_N->enable_irq();
	_ii_NE->enable_irq();
	_ii_E->enable_irq();
	_ii_ES->enable_irq();
	_ii_S->enable_irq();
	_ii_SW->enable_irq();
	_ii_W->enable_irq();
	_ii_WN->enable_irq();
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
		// if key pressed, read keyboard, update topic and enable publishing and repeated events
		if(oe.status == osEventSignal && (oe.value.signals & KEY_EV_PRESSED) != 0){		
			_currentkey = readKeyboard();
			_keydata.data.keycode = (_currentkey ^ _lastkey);
			publish = true;			
			_timeout = REPEAT_TIMEOUT;
		}
		// if key released and no keys pressed, disables repeated events
		if(oe.status == osEventSignal && (oe.value.signals & KEY_EV_RELEASED) != 0){
			_currentkey = readKeyboard();
			if(!_currentkey){
				_timeout = osWaitForever;
			}
		}
		// if repeated event, enables publishing
		if(oe.status == osEventTimeout){
			publish = true;	
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
	uint32_t result = 0;
	if(_ii_N->read())
		result |= (1 << 0);
	if(_ii_NE->read())
		result |= (1 << 1);
	if(_ii_E->read())
		result |= (1 << 2);
	if(_ii_ES->read())
		result |= (1 << 3);
	if(_ii_S->read())
		result |= (1 << 4);
	if(_ii_SW->read())
		result |= (1 << 5);
	if(_ii_W->read())
		result |= (1 << 6);
	if(_ii_WN->read())
		result |= (1 << 7);
	if(_ii_ARM->read())
		result |= (1 << 8);
	if(_ii_LOC->read())
		result |= (1 << 9);
	if(_ii_ALT->read())
		result |= (1 << 10);
	if(_ii_RTH->read())
		result |= (1 << 11);
	return result;
}

