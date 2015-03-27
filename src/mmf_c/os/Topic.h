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

#include "Exception.h"


/** \struct Topic
 *  \brief Topic struct represents the dynamic piece of software in which Observers can subscribe to get
 *  track of its evolution (updates)
 */
typedef struct {
	int id;						///< topic id
	const char * name;			///< topic name id
	void * data;				///< topic data
	int datasize;				///< topic data size
	void ** observerlist;		///< declaration of observer list
	int listsize;				///< max number of observer in list
}Topic;

/** \struct TopicData
 *  \brief Topic data struct is the data part of a Topic
 */
typedef struct {
	int id;						///< topic id
	const char * name;			///< topic name id
	void * data;				///< topic data
	int datasize;				///< topic data size
}TopicData;

/** \struct TopicDataPool
 *  \brief Topic data  pool struct is the pool for storing pending topics
 */
typedef struct {
	int poolsize;				///< pool size (num entries)
	TopicData * topicdata;		///< topic data
}TopicDataPool;

/** \fn Topic_initialize
 *  \brief Function to initialize Topic's internals, like observer list.
 *  \param t Topic
 *  \param name Topic name
 *  \param oblist Observer list pointer
 *  \param listsize List size
 *  \param Exception code
 */
void Topic_initialize(Topic* t, const char* name, void ** oblist, int listsize, Exception *e);

/** \fn Topic_notify
 *  \brief Static function to notify the subscribed observers about the update/change of the topic
 *  \param t Topic
 *  \param name Topic name
 *  \param data Topic's data
 *  \param datasize	Topic's data size in bytes
 *  \param Exception code
 */
void Topic_notify(Topic* t, void * data, int datasize, Exception *e);

/** \fn Topic_attach
 *  \brief Static function to attach (subscribe) an observer class to a topic
 *  \param t Topic
 *  \param observer Observer
 *  \param Exception code
 */
void Topic_attach(Topic* t, void * observer, Exception *e);

/** \fn Topic_dettach
 *  \brief Static function to dettach (unsubscribe) an observer class from a topic
 *  \param t Topic
 *  \param observer Observer
 *  \param Exception code
 */
void Topic_dettach(Topic* t, void * observer, Exception *e);


#endif /* SRC_OS_TOPIC_H_ */
