/*
 * RcTopics.h
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 *
 *  Topics are data structures used in publisher/subscriber architectures. These structures use to have
 *	various common members: 
 *		- status: is a bitmask variable showing the current state of the topic
 *		- queries: is a bitmask variable indicating which params of the topic have been updated
 *		- params: several variables according with the topic type
 *	Also, topics has a unique name string "/name". Using that string, an agent can get topic
 *	references, publish updates, attach to those updates ...
 *
 *	RcTopics module includes all kind of topics related to UAV remote controller.
 *	Next lines describe those which are already implemented:
 *
 *	Topic: 	"/rc"
 *	Descr:	Describes rc channel data (throttle, pitch, roll,...) provided by a rc device
 *	
 */

#ifndef SRC_TOPICS_RCTOPICS_H_
#define SRC_TOPICS_RCTOPICS_H_

#include "../../mmf_c/os/mmf.h"


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
	RC_MODE_LOC 			 = (int)(1 << 0),	  	///< Location mode for auto-follow aerial photography
	RC_MODE_MANUAL_DISARMED  = (int)(1 << 1),	  	///< Manual mode, UAV disarmed
	RC_MODE_MANUAL_ARMED 	 = (int)(1 << 2),	  	///< Manual mode, UAV armed and ready for flight
	RC_MODE_MANUAL_ARMED_ALT = (int)(1 << 3),	  	///< Manual mode, UAV armed with Altitude Hold mode
	RC_MODE_MANUAL_ARMED_RTH = (int)(1 << 4)	  	///< Manual mode, UAV armed with Return To Home mode
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


/**
 *  \fn RcTopic_initialize
 *  \brief Initializes a topic relative to DataTopic types
 *  \param name Topic's name to initialize
 *	\param e Exception code
 *	\return Topic reference
 */
Topic * RcTopic_initialize(const char * name, Exception *e);

/**
 *  \fn RcTopic_getRef
 *  \brief Get topic reference pointer
 *  \param name Topic's name to initialize
 *	\param e Exception code
 *	\return Topic reference
 */
Topic * RcTopic_getRef(const char * name, Exception *e);

#endif /* SRC_TOPICS_RCTOPICS_H_ */
