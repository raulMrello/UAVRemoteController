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
	KEY_LOC  = (int)(1 << 8),
	KEY_ARM  = (int)(1 << 9),
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
