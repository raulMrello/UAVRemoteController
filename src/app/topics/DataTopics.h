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

Topic * DataTopic_initialize(const char * name, Exception *e);
//Topic * MyTopic_getRef(void);
//int MyTopic_getId(void);
//const char * MyTopic_getName(void);

#endif /* SRC_TOPICS_DATATOPICS_H_ */
