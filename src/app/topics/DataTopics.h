/*
 * DataTopics.h
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 *
 *  Topics related to tasks IPC. This includes topics as /rc and /gps.
 */

#ifndef SRC_TOPICS_DATATOPICS_H_
#define SRC_TOPICS_DATATOPICS_H_

#include "../../mmf_c/os/mmf.h"


//------------------------------------------------------------------------------------
//--  /rc topics definitions  --------------------------------------------------------
//------------------------------------------------------------------------------------

/** \enum RcLocPosition
 *  \brief Enumerates different positions where UAV is located relative to target for
 *  auto-follow mode in aerial photography
 */
typedef enum {
	RC_LOC_FRONT,
	RC_LOC_FRONT_LEFT,
	RC_LOC_FRONT_RIGTH,
	RC_LOC_LEFT,
	RC_LOC_RIGHT,
	RC_LOC_LEFT_BACK,
	RC_LOC_RIGHT_BACK,
	RC_LOC_BACK
}RcLocPosition;


/** \enum RcMode
 *  \brief Enumerates different modes in which UAV can be setup
 */
typedef enum {
	RC_MODE_LOC 			= (int)(1 << 0),  ///< Location mode for auto-follow aerial photography
	RC_MODE_MANUAL_DISARMED = (int)(1 << 1),  ///< Manual mode, UAV disarmed
	RC_MODE_MANUAL_ARMED 	= (int)(1 << 2)	  ///< Manual mode, UAV armed and ready for flight
}RcMode;


/** \struct RC_TOPIC_DATA_T
 *  \brief Topic data structure for /rc topics
 */
typedef struct {
	int loc_keys;	///< key configuration for local positioning (N|NE|E|ES|S|SW|W|WN)
	int throttle;	///< throttle value (range 1000, 2000) = (1ms, 2ms) = (0m/s, ?m/s)
	int roll;		///< roll value (range 1000, 2000) = (1ms, 2ms)
	int pitch;		///< pitch value (range 1000, 2000) = (1ms, 2ms)
	int yaw;		///< yaw value (range 1000, 2000) = (1ms, 2ms)
	RcMode mode;	///< mode value
}RC_TOPIC_DATA_T;


//------------------------------------------------------------------------------------
//--  /gps topics definitions  -------------------------------------------------------
//------------------------------------------------------------------------------------

/** \struct RC_GPS_DATA_T
 *  \brief GPS data structure for /gps topics
 */
typedef struct {
	float latitude;		///< Latitude value (range -65º +65º)
	float longitude;	///< Longitude value (range -180º, 180º)
	int altitude;		///< Altitude value (range 0m, 10000m)
	int heading;		///< Heading value (range 0º, 360º)
}GPS_TOPIC_DATA_T;

//------------------------------------------------------------------------------------
//--  COMMON DEFINITIONS  ------------------------------------------------------------
//------------------------------------------------------------------------------------

/**
 *  \fn DataTopic_initialize
 *  \brief Initializes a topic relative to DataTopic types
 *  \param name Topic's name to initialize
 *	\param e Exception code
 *	\return Topic reference
 */
Topic * DataTopic_initialize(const char * name, Exception *e);

/**
 *  \fn InputTopic_getRef
 *  \brief Get topic reference pointer
 *  \param name Topic's name to initialize
 *	\param e Exception code
 *	\return Topic reference
 */
Topic * DataTopic_getRef(const char * name, Exception *e);

#endif /* SRC_TOPICS_DATATOPICS_H_ */
