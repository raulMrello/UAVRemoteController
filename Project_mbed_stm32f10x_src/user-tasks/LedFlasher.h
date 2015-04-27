/*
 * LedFlasher.h
 *
 *  Created on: 21/4/2015
 *      Author: raulMrello
 *
 *	LedFlasher is a periodic module (RtosTimerTask) that generates a led flashing shot 
 *	on a DigitalOut pin. It can manage repeated short-long flashing shots.
 */

#ifndef SRC_ACTIVE_MODULES_LEDFLASHER_H_
#define SRC_ACTIVE_MODULES_LEDFLASHER_H_


//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "mbed.h"
#include "rtos.h"

//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------
//-- CLASS ---------------------------------------------------------------------------
//------------------------------------------------------------------------------------

class LedFlasher{
public:
	
	/** Shot mode enumeration */
	enum ShotModeEnum{
		ON_FOREVER,					///< Always lighting
		SLOW_FLASHING,				///< Slow (1sec,1sec) flashing
		SINGLE_FAST_FLASHING,		///< Rapid (500ms, 1s) flashing
		DUAL_FAST_FLASHING,			///< Rapid (500ms, 500ms) dual-shot flashing
		CONTINUOUS_FAST_FLASHING	///< Rapid continuous flashing 500ms
	};
	
	/** Shot status enumeration */
	enum ShotStatusEnum{
		STOPPED,		///< Switched off and stopped
		STAT_FLASH,		///< Switched on
		STAT_OFF,		///< Switched off
		STAT_WAIT		///< Waiting next cycle
	};
	
	/** Shot time enumeration */
	enum ShotTimeEnum{
		TIME_DISABLED = 0,
		TIME_FAST = 500,		///< 500ms
		TIME_SLOW = 1000,		///< 1s
		TIME_WAIT = 1500		///< 1.5
	};
	
	struct LedChannel{
		DigitalOut *led;
		ShotModeEnum mode;
		ShotTimeEnum on_time;
		ShotTimeEnum off_time; 
		ShotTimeEnum wait_time;
		ShotStatusEnum status;
		RtosTimer * tmr;
		uint8_t shots;
	};	
	/** Constructor, destructor, getter and setter */
	LedFlasher(uint8_t led_channels);	
	virtual ~LedFlasher();
	
	int8_t addLedChannel(DigitalOut *led);
	void ledStart(int8_t channel, ShotModeEnum mode);
	void ledStop(int8_t channel);
	uint8_t _channels;
	LedChannel *_led_list;
};


#endif /* SRC_ACTIVE_MODULES_LEDFLASHER_H_ */
