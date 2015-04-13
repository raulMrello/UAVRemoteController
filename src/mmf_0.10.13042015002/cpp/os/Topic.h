/*
 * Topic.h
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 */

#ifndef SRC_OS_TOPIC_H_
#define SRC_OS_TOPIC_H_

//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include <stddef.h>
#include "../port/platforms.h" ///< platform dependent
#include "List.h"
#include "Exception.h"

namespace MMF {

//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------

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

/** Type definitions for callback handler  */
typedef void *	TopicConsumedHandlerObj;

/** Type definitions for callback handler  */
typedef void *	TopicSubscribedHandlerObj;

/** Type definitions for callback declaration  */
typedef void (*TopicConsumedCallback)(TopicConsumedHandlerObj cbhandler);

//------------------------------------------------------------------------------------
//-- CLASS ---------------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** \class Topic
 *  \brief Topic class represents the dynamic piece of software in which Observers can subscribe to get
 *  track of its evolution (updates)
 */
class Topic {
public:
	/** \fn Topic
	 *  \brief Constructor
	 */
	Topic(const char * name = "");

	/** \fn ~Topic
	 *  \brief Destructor
	 */
	virtual ~Topic();

	/** \fn notify
	 *  \brief function to notify the subscribed observers about the update/change of the topic
	 *  \param data Topic's data
	 *  \param datasize	Topic's data size in bytes
	 */
	void notify(void * data, int datasize,  TopicConsumedCallback done, TopicConsumedHandlerObj publisher) throw (Exception);

	/** \fn attach
	 *  \brief function to attach (subscribe) an observer class to a topic
	 *  \param o Observer
	 */
	void attach(TopicSubscribedHandlerObj o) throw (Exception);

	/** \fn dettach
	 *  \brief function to dettach (unsubscribe) an observer class from a topic
	 *  \param o Observer
	 */
	void dettach(TopicSubscribedHandlerObj o) throw (Exception);


	/** \fn getName
	 *  \brief Get topic name
	 *  \return name
	 */
	const char * getName();

	/** \fn getId
	 *  \brief Get topic id reference
	 *  \return id reference
	 */
	int getId();


private:
	int _id;					///< topic id
	const char * _name;			///< topic name id
	void * _data;				///< topic data
	int _datasize;				///< topic data size
	int _count;					///< topic producer/consumer count
	List* _observerlist;		///< declaration of observer list
};
}

#endif /* SRC_OS_TOPIC_H_ */
