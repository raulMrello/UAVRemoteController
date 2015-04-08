/*
 * SystemTopics.h
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 *
 *  Topics related to System configuration. This includes topics /sys
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
