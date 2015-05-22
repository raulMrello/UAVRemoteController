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
	/** Gps topic data structure */
	struct GpsData_t{
		long time;		///< GPS Millisecond Time of Week
		long lat;     	///< Geographic coordinates
		long lng;
		long alt;
		long gspeed;	///< Ground speed
		long speed3d;   ///< Speed (3-D)
		long gcourse;	///< Ground Course
		uint8_t sats;   ///< Number of visible satelites
		uint8_t fix;    ///< 1:GPS FIX   0:No FIX (normal logic)
		uint8_t timeout;///< Timeout error flag
	};


	//------------------------------------------------------------------------------------
	/** Alarm error code enumeration */
	enum AlarmCodeEnum {
		ALARM_MISSING_RESPONSE,		///< No response from wifi link
		ALARM_WRONG_RESPONSE,			///< Response bad formed from wifi link
		ALARM_FORCE_DISARM				///< Force a disarm action
	};
	
	/** Alarm topic data structure */
	struct AlarmData_t{
		AlarmCodeEnum code;
		uint8_t data[8];
	};

	


	//------------------------------------------------------------------------------------
	/** Status topic data structure */
	struct StatusData_t{
		uint8_t mode;
		GpsData_t location;
	};
	
	/** Status mode enumeration */
	enum StatusCodeEnum {
		MODE_DISARMED 	= 0,
		MODE_LOC 		= (1 << 0),
		MODE_MANUAL		= (1 << 1),
		MODE_ALT		= (1 << 2),
		MODE_RTH		= (1 << 3)
	};

	//------------------------------------------------------------------------------------
	/** Keyboard topic data structure */
	struct KeyData_t{
		bool isHold;
		union {
			uint32_t keycode;
			struct {
				unsigned key_A_Ok    :1;
				unsigned key_B_Ok    :1;
				unsigned key_ARM     :1;
				unsigned key_LOC     :1;
				unsigned key_ALT     :1;
				unsigned key_RTH     :1;
				unsigned keys_UNUSED :27;
			}keys;
		}data;	
	};

	
	//------------------------------------------------------------------------------------
	/** Joystick topic data structure */
	struct JoystickData_t{
		int8_t yaw;			///< value 0=left, 50=center, 100=right
		int8_t throttle;	///< value 0=dynamic_down, 50, 100=dynamic_up
		int8_t roll;		///< value 0=left, 50=center, 100=right
		int8_t pitch;		///< value 0=down, 50=middle, 100=up
	};


}

#endif  /* SRC_COMMON_TOPICS_H_ */
