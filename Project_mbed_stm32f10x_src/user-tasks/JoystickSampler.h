/*
 * JoystickSampler.h
 *
 *  Created on: 20/4/2015
 *      Author: raulMrello
 *
 *  JoystickSampler is an Active module (running as a Task) that will sample joystick inputs
 *	by pooling. Each time a variation is found, then a topic data structure
 *	(type "/joys") is updated and published.
 */

#ifndef SRC_ACTIVE_MODULES_JOYSTICKSAMPLER_H_
#define SRC_ACTIVE_MODULES_JOYSTICKSAMPLER_H_


//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "mbed.h"
#include "rtos.h"
#include "MsgBroker.h"
#include "Topics.h"

//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//-- CLASS ---------------------------------------------------------------------------
//------------------------------------------------------------------------------------

class JoystickSampler{
public:
	
	/** Constructor, destructor, getter and setter */
	JoystickSampler(AnalogIn *joystick_A1, AnalogIn *joystick_A2, AnalogIn *joystick_B1, AnalogIn *joystick_B2);
	
	virtual ~JoystickSampler();
	
	void startScan();
	void scan();
	void stopScan();

private:
	Topic::JoystickData_t _joystickdata;
	RtosTimer *_tmr;
	AnalogIn * _joystick_A1;
	AnalogIn * _joystick_A2;
	AnalogIn * _joystick_B1;
	AnalogIn * _joystick_B2;
	int8_t 	   _throttle_rate;	
	uint8_t    _stat;
};


#endif /* SRC_ACTIVE_MODULES_JOYSTICKSAMPLER_H_ */
