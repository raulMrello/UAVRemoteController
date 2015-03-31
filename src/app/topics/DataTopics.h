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

/**
 * \struct RC_TOPIC_DATA_T
 * \brief Topic data structure for /rc topics
 */
typedef struct {
	int loc_keys;	///< key configuration for local positioning (N|NE|E|ES|S|SW|W|WN)
	int throttle;	///< throttle value (range -1000, +1000) = (-100.0%, 100.0%)
	int roll;		///< roll value (range -1000, +1000) = (-100.0%, 100.0%)
	int pitch;		///< pitch value (range -1000, +1000) = (-100.0%, 100.0%)
	int yaw;		///< yaw value (range -1000, +1000) = (-100.0%, 100.0%)
	int mode;		///< flight mode value (range 0,1,2)
}RC_TOPIC_DATA_T;

Topic * DataTopic_initialize(const char * name, Exception *e);
//Topic * MyTopic_getRef(void);
//int MyTopic_getId(void);
//const char * MyTopic_getName(void);

#endif /* SRC_TOPICS_DATATOPICS_H_ */
