/*
 * Topic.cpp
 *
 *  Created on: 12/3/2015
 *      Author: dptoid
 */

#include "Topic.h"
#include "Task.h"
using namespace MMF;

//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------


Topic::Topic(const char * name){
	_name = name;
	_count = 0;						///< topic producer/consumer count
	_data = 0;
	_datasize = 0;
	_observerlist = new List();		///< declaration of observer list
	_id = (int)this;
}

//------------------------------------------------------------------------------------
Topic::~Topic(){
	delete _observerlist;
}

//------------------------------------------------------------------------------------
void Topic::notify(void * data, int datasize,  TopicConsumedCallback done, TopicConsumedHandlerObj publisher) throw (Exception){
	PLATFORM_ENTER_CRITICAL();
	_data = data;
	_datasize = datasize;
	_count = 0;
	Task* t = (Task*)_observerlist->getFirstItem();
	if(!t){
		PLATFORM_EXIT_CRITICAL();
		return;
	}
	for(;;){
		_count++;
		// inserts into the topic pool
		TopicData td;
		td.id = _id;
		td.name = _name;
		td.datasize = _datasize;
		td.pcount = &_count;
		td.done = done;
		td.publisher = publisher;

		try{
			t->addTopic(&td);
		}
		catch(Exception &e){
			PLATFORM_EXIT_CRITICAL();
			throw e;
		}
		// select next subscriber
		t = (Task*)_observerlist->getNextItem();
		if(!t){
			break;
		}
	}
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
void Topic::attach(TopicSubscribedHandlerObj observer) throw (Exception){
	Item item = _observerlist->searchItem(observer);
	PLATFORM_ENTER_CRITICAL();
	// if already attached, exit
	if(item){
		PLATFORM_EXIT_CRITICAL();
		throw Exception(Exception::MEMORY_ALLOC, "Topic::attach observer exists");
	}
	// else attach
	try{
		_observerlist->addItem(observer);
		PLATFORM_EXIT_CRITICAL();
	}
	catch(Exception &e){
		PLATFORM_EXIT_CRITICAL();
		throw e;
	}
}

//------------------------------------------------------------------------------------
void Topic::dettach(TopicSubscribedHandlerObj observer) throw (Exception){
	Item item = _observerlist->searchItem(observer);
	PLATFORM_ENTER_CRITICAL();
	// if not attached, exits
	if(!item){
		PLATFORM_EXIT_CRITICAL();
		throw Exception(Exception::MEMORY_DEALLOC, "Topic::detach observer doesn't exists");
	}
	// else dettach
	try{
		_observerlist->removeItem(observer);
		PLATFORM_EXIT_CRITICAL();
	}
	catch(Exception &e){
		PLATFORM_EXIT_CRITICAL();
		throw e;
	}
}

//------------------------------------------------------------------------------------
const char * Topic::getName(){
	return _name;
}

//------------------------------------------------------------------------------------
int Topic::getId(){
	return _id;
}
