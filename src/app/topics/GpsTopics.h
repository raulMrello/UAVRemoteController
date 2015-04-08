/*
 * GpsTopics.h
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
 *	GpsTopics module includes all kind of topics related to gps data.
 *	Next lines describe those which are already implemented:
 *
 *	Topic: 	"/nmea"
 *	Descr:	Describes gps topic data (lat, lng ...) provided by a gps module in NMEA format.
 *
 *	Topic: 	"/ubx"
 *	Descr:	Describes gps topic data (lat, lng ...) provided by a gps module in UBX format.
 *	
 */

#ifndef SRC_TOPICS_GPSTOPICS_H_
#define SRC_TOPICS_GPSTOPICS_H_

#include "../../mmf_c/os/mmf.h"

/** \struct GPS_TOPIC_DATA_T
 *  \brief Topic data structure for /nmea and /ubx topics
 */
typedef struct {
	float latitude;
	float longitude;
	float altitude;
	float heading;
}GPS_TOPIC_DATA_T;


Topic * GpsTopic_initialize(const char * name, Exception *e);
Topic * GpsTopic_getRef(const char * name, Exception *e);

//int GpsTopic_getId(void);
//const char * GpsTopic_getName(void);

#endif /* SRC_TOPICS_GPSTOPICS_H_ */
