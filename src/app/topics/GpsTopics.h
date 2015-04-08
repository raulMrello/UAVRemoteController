/*
 * GpsTopics.h
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 *
 *  Topics related to Gps peripherals. This includes topics as /nmea and /ubx.
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
