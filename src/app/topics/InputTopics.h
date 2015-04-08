/*
 * InputTopics.h
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
 *	InputTopics module includes all kind of topics related to input pins (ADC, digital in, DAC, ...).
 *	Next lines describe those which are already implemented:
 *
 *	Topic: 	"/key"
 *	Descr:	Describes keyboard actions (pressed/released) as an uint32 bitmask variable
 *	
 */

#ifndef SRC_TOPICS_INPUTTOPICS_H_
#define SRC_TOPICS_INPUTTOPICS_H_

#include "../../mmf_c/os/mmf.h"


//------------------------------------------------------------------------------------
//--  /key topics definitions  -------------------------------------------------------
//------------------------------------------------------------------------------------

/** \enum KeyStatus
 *  \brief Enumerates different keys status from pushbuttons
 */
typedef enum {
	KEY_NONE = 0,
	KEY_N 	 = (int)(1 << 0),
	KEY_NE 	 = (int)(1 << 1),
	KEY_E 	 = (int)(1 << 2),
	KEY_ES 	 = (int)(1 << 3),
	KEY_S 	 = (int)(1 << 4),
	KEY_SW 	 = (int)(1 << 5),
	KEY_W 	 = (int)(1 << 6),
	KEY_WN 	 = (int)(1 << 7),
	KEY_ARM  = (int)(1 << 8),
	KEY_LOC  = (int)(1 << 9),
	KEY_ALT  = (int)(1 << 10),
	KEY_RTH  = (int)(1 << 11),
}KeyStatus;


/** \enum KeyEvent
 *  \brief Key events
 */
typedef enum {
	KEY_RELEASED = 0,	///< key has been released
	KEY_PRESSED  = 1	///< key has been pressed
}KeyEvent;

/** \struct KEY_TOPIC_DATA_T
 *  \brief Topic data structure for /key topics
 */
typedef struct {
	int keys;		///< key combination PRESSED/RELEASED
}KEY_TOPIC_DATA_T;

/**
 *  \fn InputTopic_initialize
 *  \brief Initializes an Input topic
 *  \param name Topic's name to initialize
 *	\param e Exception code
 *	\return Topic reference
 */
Topic * InputTopic_initialize(const char * name, Exception *e);

/**
 *  \fn InputTopic_getRef
 *  \brief Get topic reference pointer
 *  \param name Topic's name to initialize
 *	\param e Exception code
 *	\return Topic reference
 */
Topic * InputTopic_getRef(const char * name, Exception *e);

#endif /* SRC_TOPICS_INPUTTOPICS_H_ */
