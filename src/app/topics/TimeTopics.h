/*
 * TimeTopics.h
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 *
 *  Topics related to TIME publishers. This includes topics /time
 */

#ifndef SRC_TOPICS_TIMETOPICS_H_
#define SRC_TOPICS_TIMETOPICS_H_

#include "../../mmf_c/os/mmf.h"


/** \enum TimeEvent
 *  \brief Time event flag definitions
 */
typedef enum {
	TIME_NO_EVENTS		= 0,
	TIME_EVENT_SECOND 	= (1 << 0),
	TIME_EVENT_MINUTE 	= (1 << 1),
	TIME_EVENT_HOUR 	= (1 << 2),
	TIME_EVENT_MIDDAY 	= (1 << 3),
	TIME_EVENT_DAY 		= (1 << 4),
	TIME_EVENT_WEEK 	= (1 << 5),
	TIME_EVENT_MONTH 	= (1 << 6),
	TIME_EVENT_SEASON 	= (1 << 7),
	TIME_EVENT_YEAR 	= (1 << 8)
}TimeEvent;

/** \struct TIME_TOPIC_DATA_T
 *  \brief Topic data structure for /time topics
 */
typedef struct {
	int time;			///< time value uint32_t format
	TimeEvent event;	///< time event
}TIME_TOPIC_DATA_T;


Topic * TimeTopic_initialize(const char * name, Exception *e);
Topic * TimeTopic_getRef(const char * name, Exception *e);

//int MyTopic_getId(void);
//const char * MyTopic_getName(void);

#endif /* SRC_TOPICS_TIMETOPICS_H_ */
