/*
 * SystemTopics.h
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
 *	SystemTopics module includes all kind of topics related to system configuration.
 *	Next lines describe those which are already implemented:
 *
 *	Topic: 	"/sys"
 *	Descr:	Describes system configuration actions to carry out on a specified peripheral.
 *	
 */

#ifndef SRC_TOPICS_SYSTEMTOPICS_H_
#define SRC_TOPICS_SYSTEMTOPICS_H_

#include "../../mmf_c/os/mmf.h"


/** \enum SysRequest
 *  \brief System requested operation flags definitions
 */
typedef enum {
	SYS_NO_QUERIES			= 0,
	SYS_CPU_SET_STOP 		= (1 << 0),
	SYS_CPU_SET_RUN 		= (1 << 1),
	SYS_RTC_SET_TIME 		= (1 << 2),
	SYS_RTC_DISABLE_EVENTS 	= (1 << 3),
	SYS_RTC_ENABLE_EVENTS 	= (1 << 4)
}SysRequest;

/** \enum SysStatus
 *  \brief System status flag definitions
 */
typedef enum {
	SYS_CPU_RUN 			= (1 << 0),
	SYS_RTC_EVENTS_ENABLED 	= (1 << 1)
}SysStatus;

/** \struct SYS_TOPIC_DATA_T
 *  \brief Topic data structure for /sys topics
 */
typedef struct {
	int time;				///< system time in uint32_t format
	SysRequest queries;		///< requested operation flags
	SysStatus status;		///< status flags
}SYS_TOPIC_DATA_T;

Topic * SystemTopic_initialize(const char * name, Exception *e);
Topic * SystemTopic_getRef(const char * name, Exception *e);

//int SystemTopic_getId(void);
//const char * SystemTopic_getName(void);

#endif /* SRC_TOPICS_SYSTEMTOPICS_H_ */
