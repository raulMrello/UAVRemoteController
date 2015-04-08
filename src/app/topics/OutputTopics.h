/*
 * OututTopics.h
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 *
 *  Topics related to GPIO output pins. These topics are named as /out and will be formed
 *  by an output_id and a logic value.
 */

#ifndef SRC_TOPICS_OUTUTTOPICS_H_
#define SRC_TOPICS_OUTUTTOPICS_H_

#include "../../mmf_c/os/mmf.h"


//------------------------------------------------------------------------------------
//--  /out topics definitions  -------------------------------------------------------
//------------------------------------------------------------------------------------

/** \enum OutputStatus
 *  \brief Enumerates different output status 
 */
typedef enum {
	OUT_OFF  = 0,
	OUT_ON 	 = 1
}OutputStatus;


/** \struct OUT_TOPIC_DATA_T
 *  \brief Topic data structure for /out topics
 */
typedef struct {
	int output_id;			///< output id
	OutputStatus value;		///< output logic value
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
