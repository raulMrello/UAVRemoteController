/*
 * Topic.h
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 *
 * 	Topics are data structures capable to be updated by pusblishers and notify on those updates
 * 	to other subscribers objects. For this purpose, each topic has an internal list of subscribers
 * 	(also named observers).
 *
 * 	When you want to create a new Topic, you must implement this interface, declaring a static
 * 	Topic variable and initializating it afterwards. During this initialization, the observer list
 * 	will be erased.
 *
 * 	Subscribers can be attached to topic updates through Topic_attach primitive. Also they can be
 * 	dettached through Topic_dettach.
 *
 * 	On the other hand, publishers can notify topic changes through Topic_notify primitive.
 *
 * 	Example of use:
 *
 * 				Task t_subs;
 * 				Task_initialize (&t_subs, ....);
 *
 * 				Topic* t = MyTopic_initialize(); ///< topic object defined by me.
 * 				Topic_attach(&t, &t_subs, &ex);
 * 				...
 * 				...
 * 				Topic_notify(&t, data, datasize, &ex);
 *
 */

#ifndef SRC_OS_TOPIC_H_
#define SRC_OS_TOPIC_H_

#ifdef __cplusplus
 extern "C" {
#endif

//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "List.h"
#include "Exception.h"

//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** Pointer to Topic object */
typedef struct Topic_t *	TopicPtr;

/** \struct TopicData
 *  \brief Topic data struct is the data part of a Topic
 */
typedef struct {
	int id;							///< topic id
	const char * name;				///< topic name id
	void * data;					///< topic data
	int datasize;					///< topic data size
	int * pcount;					///< pointer to topic producer/consumer count
	void (*done)(void* publisher);	///< callback to notify when topic consumed by all subscribers (count=0)
	void * publisher;				///< publisher object
}TopicData;


//------------------------------------------------------------------------------------
//-- PROTOTYPES ----------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** \fn Topic_create
 *  \brief Function to create and initialize a Topic.
 *  \param name Topic name
 *  \param Exception code
 *  \return Topic reference
 */
TopicPtr Topic_create(const char* name, ExceptionPtr e);

/**
 * \fn Topic_kill
 * \brief Deallocates a Topic. The variable that points to the Topic objects then is set to 0, to
 * indicate that topic is deallocated and now it points to NULL.
 * @param ptopic Variable that points to a topic object
 * @param e Exception handler
 */
void Topic_kill(TopicPtr *ptopic, ExceptionPtr e);

/** \fn Topic_notify
 *  \brief Function to notify the subscribed observers about the update/change of the topic
 *  \param t Topic
 *  \param name Topic name
 *  \param data Topic's data
 *  \param datasize	Topic's data size in bytes
 *  \param done Callback to invoke when topic had been consumed by all subscribers
 *  \param publisher Publisher object who manages done callback
 *  \param Exception code
 */
void Topic_notify(TopicPtr t, void * data, int datasize, void (*done)(void*), void* publisher, ExceptionPtr e);

/** \fn Topic_attach
 *  \brief Static function to attach (subscribe) an observer class to a topic
 *  \param t Topic
 *  \param observer Observer
 *  \param Exception code
 */
void Topic_attach(TopicPtr t, void * observer, ExceptionPtr e);

/** \fn Topic_dettach
 *  \brief Function to dettach (unsubscribe) an observer class from a topic
 *  \param t Topic
 *  \param observer Observer
 *  \param Exception code
 */
void Topic_dettach(TopicPtr t, void * observer, ExceptionPtr e);

/** \fn Topic_getName
 *  \brief Get topic name
 *  \param t Topic
 *  \param Exception code
 *  \return Topic name
 */
const char * Topic_getName(TopicPtr t, ExceptionPtr e);

#ifdef __cplusplus
}
#endif

#endif /* SRC_OS_TOPIC_H_ */
