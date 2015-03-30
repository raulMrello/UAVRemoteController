/*
 * InputTopics.h
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 *
 *  Topics related to GPIO input pins in which pushbuttons are connected to those pins.
 *  These topics can be /push and /release and will add a keycode parameter in a char format.
 */

#ifndef SRC_TOPICS_INPUTTOPICS_H_
#define SRC_TOPICS_INPUTTOPICS_H_

#include "../../mmf_c/os/mmf.h"

Topic * InputTopic_initialize(const char * name, Exception *e);
//Topic * InputTopic_getRef(void);
//int InputTopic_getId(void);
//const char * InputTopic_getName(void);

#endif /* SRC_TOPICS_INPUTTOPICS_H_ */
