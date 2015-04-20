/* 
 * Topics.h
 *
 *  Created on: 20/4/2015
 *      Author: raulMrello
 *
 *	Topics data structure description file
 */

#ifndef SRC_COMMON_TOPICS_H_
#define SRC_COMMON_TOPICS_H_

namespace Topic {	
//------------------------------------------------------------------------------------
/** Alarm topic data structure */
struct AlarmData_t{
	uint8_t alarm[8];
};


//------------------------------------------------------------------------------------
/** Status topic data structure */
struct StatusData_t{
	uint8_t status[8];
};


//------------------------------------------------------------------------------------
/** Gps topic data structure */
struct GpsData_t{
	float lat;	///< latitude -100º to +100º
	float lng;	///< longitude -12h to +12h
	float alt;	///< altitude -100m to 8000m
	float hea;	///< heading 0º to 360º
};


//------------------------------------------------------------------------------------
/** Keyboard topic data structure */
struct KeyData_t{
	union {
		uint32_t keycode;
		struct {
			unsigned key_N	:1; //bit 0
			unsigned key_NE	:1;
			unsigned key_E	:1;
			unsigned key_ES	:1;
			unsigned key_S	:1;
			unsigned key_SW	:1;
			unsigned key_W	:1;
			unsigned key_WN	:1;
			unsigned key_ARM:1;
			unsigned key_LOC:1;
			unsigned key_ALT:1;
			unsigned key_RTH:1;
			unsigned keys_UNUSED:21;
		}keys;
	}data;	
};


}

#endif  /* SRC_COMMON_TOPICS_H_ */
