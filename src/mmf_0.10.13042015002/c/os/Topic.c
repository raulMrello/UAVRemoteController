/*
 * Topic.cpp
 *
 *  Created on: 12/3/2015
 *      Author: dptoid
 */

#include "Topic.h"
#include "Task.h"
#include "Memory.h"

//------------------------------------------------------------------------------------
//-- REQUIRED MMF DEPENDENCIES -------------------------------------------------------
//------------------------------------------------------------------------------------

extern void Task_addTopic(TaskPtr task, TopicData *topic, ExceptionPtr e);

//------------------------------------------------------------------------------------
//-- PRIVATE TYPEDEFS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

/** \struct Topic
 *  \brief Topic struct represents the dynamic piece of software in which Observers can subscribe to get
 *  track of its evolution (updates)
 */
typedef struct {
	int id;						///< topic id
	const char * name;			///< topic name id
	void * data;				///< topic data
	int datasize;				///< topic data size
	int count;					///< topic producer/consumer count
	ListPtr observerlist;		///< declaration of observer list
}Topic;

//------------------------------------------------------------------------------------
//-- PRIVATE DEFINES -----------------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef true
#define true (char)1
#endif

#ifndef false
#define false (char)0
#endif

//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

TopicPtr Topic_create(const char* name, ExceptionPtr e){
	Topic *topic = (Topic*)Memory_alloc(sizeof(Topic), e);
	catch(e){
		return 0;
	}
	topic->observerlist = List_create(e);
	catch(e){
		Memory_free(topic, e);
		return 0;
	}
	topic->id = (int)topic;
	if(!name)
		topic->name = "";
	else
		topic->name = name;
	topic->data = 0;
	topic->datasize = 0;
	return (TopicPtr)topic;
}

//------------------------------------------------------------------------------------
void Topic_kill(TopicPtr *ptopic, ExceptionPtr e){
	Topic** pthis = (Topic**)ptopic;
	if(!ptopic || !pthis){
		Exception_throw(e, BAD_ARGUMENT, "Topic_kill ptopic is null");
		return;
	}
	PLATFORM_ENTER_CRITICAL();
	Topic* this;
	this = *pthis;
	if(!this){
		PLATFORM_EXIT_CRITICAL();
		Exception_throw(e, BAD_ARGUMENT, "Topic_kill *ptopic is null");
		return;
	}
	List_kill((ListPtr*)&this->observerlist, e);
	Memory_free(*pthis, e);
	catch(e){
		PLATFORM_EXIT_CRITICAL();
		return;
	}
	*pthis = 0;
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
void Topic_notify(TopicPtr topic, void * data, int datasize, void (*done)(void*), void* publisher, ExceptionPtr e){
	if(!topic){
		Exception_throw(e, BAD_ARGUMENT, "Topic_notify topic is null");
		return;
	}
	Topic *this = (Topic*)topic;
	PLATFORM_ENTER_CRITICAL();
	this->data = data;
	this->datasize = datasize;
	this->count = 0;
	TaskPtr t = (TaskPtr)List_getFirstItem(this->observerlist, e);
	catch(e){
		PLATFORM_EXIT_CRITICAL();
		return;
	}
	for(;;){
		this->count++;
		// inserts into the topic pool
		TopicData td = (TopicData){this->id, this->name, this->data, this->datasize, &this->count, done, publisher};
		Task_addTopic(t, &td, e);
		catch(e){
			break;
		}
		// select next subscriber
		t = (TaskPtr)List_getNextItem(this->observerlist, 0);
		if(!t){
			break;
		}
	}
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
void Topic_attach(TopicPtr topic, void * observer, ExceptionPtr e){
	if(!topic){
		Exception_throw(e, BAD_ARGUMENT, "Topic_attach topic is null");
		return;
	}
	PLATFORM_ENTER_CRITICAL();
	Topic *this = (Topic*)topic;
	Item item = List_searchItem(this->observerlist, observer, 0);
	// if already attached, exit
	if(item){
		PLATFORM_EXIT_CRITICAL();
		Exception_throw(e, MEMORY_ALLOC, "Topic_attach observer exists");
		return;
	}
	// else attach
	List_addItem(this->observerlist, observer, e);
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
void Topic_dettach(TopicPtr topic, void * observer, ExceptionPtr e){
	if(!topic){
		Exception_throw(e, BAD_ARGUMENT, "Topic_dettach topic is null");
		return;
	}
	PLATFORM_ENTER_CRITICAL();
	Topic *this = (Topic*)topic;
	Item item = List_searchItem(this->observerlist, observer, 0);
	// if not attached, exits
	if(!item){
		PLATFORM_EXIT_CRITICAL();
		Exception_throw(e, MEMORY_DEALLOC, "Topic_detach observer doesn't exists");
		return;
	}
	List_removeItem(this->observerlist, observer, e);
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
const char * Topic_getName(TopicPtr topic, ExceptionPtr e){
	if(!topic){
		Exception_throw(e, BAD_ARGUMENT, "Topic_dettach topic is null");
		return "";
	}
	Topic *this = (Topic*)topic;
	return this->name;
}


