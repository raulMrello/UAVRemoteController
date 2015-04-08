/*
 * OututTopics.h
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
 *	OutputTopics module includes all kind of topics related to output pins (ADC, digital out, PWM, ...).
 *	Next lines describe those which are already implemented:
 *
 *	Topic: 	"/out"
 *	Descr:	Describes digital output actions (On,Off) to carry out on a specified output pin.
 *
 *	Topic: 	"/pwm"
 *	Descr:	Describes pwm output actions (start, stop, setDuty, setPeriod) to carry out on a specified output pin.
 *	
 */

#ifndef SRC_TOPICS_OUTUTTOPICS_H_
#define SRC_TOPICS_OUTUTTOPICS_H_

#include "../../mmf_c/os/mmf.h"


//------------------------------------------------------------------------------------
//--  /out topics definitions  -------------------------------------------------------
//------------------------------------------------------------------------------------

/** \enum OutRequest
 *  \brief Out requested operation flags definitions
 */
typedef enum {
	OUT_NO_QUERIES	= 0,
	OUT_SET_OFF 	= (1 << 0),
	OUT_SET_ON 		= (1 << 1)
}OutRequest;

/** \enum OutputStatus
 *  \brief Enumerates different output status 
 */
typedef enum {
	OUT_OFF  = 0,
	OUT_ON 	 = (1 << 0)
}OutStatus;

/** \struct OUT_TOPIC_DATA_T
 *  \brief Topic data structure for /out topics
 */
typedef struct {
	int output_id;			///< output id
	OutRequest queries;		///< requested operation flags
	OutStatus status;		///< output logic value
}OUT_TOPIC_DATA_T;

//------------------------------------------------------------------------------------
//--  /pwm topics definitions  -------------------------------------------------------
//------------------------------------------------------------------------------------

/** \enum PwmRequest
 *  \brief Pwm requested operation flags definitions
 */
typedef enum {
	PWM_NO_QUERIES		= 0,
	PWM_SET_PERIOD 		= (1 << 0),
	PWM_SET_DUTY 		= (1 << 1)
}PwmRequest;

/** \struct PWM_TOPIC_DATA_T
 *  \brief Topic data structure for /pwm topics
 */
typedef struct {
	int period;			///< period in milliseconds
	int duty;			///< duty (range 0%..100%)
	PwmRequest queries;	///< requested operation flags
}PWM_TOPIC_DATA_T;

//------------------------------------------------------------------------------------
//--  COMMON DEFINITIONS  ------------------------------------------------------------
//------------------------------------------------------------------------------------


/**
 *  \fn OutputTopic_initialize
 *  \brief Initializes an Output topic
 *  \param name Topic's name to initialize
 *	\param e Exception code
 *	\return Topic reference
 */
Topic * OutputTopic_initialize(const char * name, Exception *e);

/**
 *  \fn OutputTopic_getRef
 *  \brief Get topic reference pointer
 *  \param name Topic's name to initialize
 *	\param e Exception code
 *	\return Topic reference
 */
Topic * OutputTopic_getRef(const char * name, Exception *e);

#endif /* SRC_TOPICS_OUTUTTOPICS_H_ */
