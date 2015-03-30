/*
 * MyTopic.h
 *
 *  Created on: 12/3/2015
 *      Author: dptoid
 */

#ifndef SRC_TOPICS_MYTOPIC_H_
#define SRC_TOPICS_MYTOPIC_H_

#include "../../mmf_c/os/mmf.h"

Topic * MyTopic_initialize(const char * name);
Topic * MyTopic_getRef(void);
int MyTopic_getId(void);
const char * MyTopic_getName(void);

#endif /* SRC_TOPICS_MYTOPIC_H_ */
