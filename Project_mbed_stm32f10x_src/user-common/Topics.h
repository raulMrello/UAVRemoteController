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
	/** Ack code enumeration */
	enum AckCodeEnum {
		ACK_OK = 0,					///< Ack response Ok
		ACK_MISSING_RESPONSE,		///< No response from wifi link
		ACK_WRONG_RESPONSE,			///< Response bad formed from wifi link
		ACK_TIMEOUT					///< Ack timeout without response
	};
	enum AckReqEnum {
		ACK_DISARMED = 0,			///< Ack to disarm mode request
		ACK_MANUAL,					///< Ack to manual mode request
		ACK_FOLLOW,					///< Ack to follow mode request
		ACK_RTH,					///< Ack to rth mode request
		ACK_RC,						///< Ack to RC publish request
		ACK_PROFILE,				///< Ack to PROFILE publish request
		ACK_KAS						///< Ack to KAS publish request
		/* Add more request types */
	};
	
	/** Ack topic data structure */
	struct AckData_t{
		AckCodeEnum ackCode;		///< Code from AckCodeEnum
		AckReqEnum  req;			///< Code from AckReqEnum
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


	//------------------------------------------------------------------------------------
	/** Profile code enumeration */
	enum ProfileCodeEnum {
		PROFILE_NONE = 0,	///< No profile defined
		PROFILE_TRAIL,		///< Profile for trail sport
		PROFILE_SKY,		///< Profile for sky sport
		PROFILE_MBS			//< Profile for mountainboarding
	};	
	/** Profile position enumeration */
	enum ProfilePosEnum {
		POS_FRONT = 0,		
		POS_FRONTRIGHT,		
		POS_RIGHT,			
		POS_RIGHTBACK,
		POS_BACK,
		POS_BACKLEFT,
		POS_LEFT,
		POS_LEFTFRONT
	};	
	
	/** Profile topic data structure */
	struct ProfileData_t{
		ProfileCodeEnum profile;		
		ProfilePosEnum  pos;			
	};


}

#endif  /* SRC_COMMON_TOPICS_H_ */
