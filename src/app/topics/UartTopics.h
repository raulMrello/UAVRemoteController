/*
 * UartTopics.h
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 *
 *  Topics related to UART peripherals. This includes topics as /nmea and /ubx when they
 *  come from GPS modules or /com topics when they come from the Wifi module.
 */

#ifndef SRC_TOPICS_UARTTOPICS_H_
#define SRC_TOPICS_UARTTOPICS_H_

#include "../../mmf_c/os/mmf.h"

Topic * UartTopic_initialize(const char * name, Exception *e);
//Topic * MyTopic_getRef(void);
//int MyTopic_getId(void);
//const char * MyTopic_getName(void);

#endif /* SRC_TOPICS_UARTTOPICS_H_ */
