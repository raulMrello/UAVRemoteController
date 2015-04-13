/*
 * Task.cpp
 *
 *  Created on: 11/3/2015
 *      Author: raulMrello
 */

#include "Task.h"
#include "Fifo.h"
#include "OS.h"
using namespace MMF;

//------------------------------------------------------------------------------------
//-- PRIVATE FUNCTIONS ---------------------------------------------------------------
//------------------------------------------------------------------------------------

static void timertask_callback(TaskHandlerObj handler){
	if(!handler)
		return;
	Task* t = (Task*)handler;
	t->resume(false);
}

//------------------------------------------------------------------------------------
void Task::start(){
	PLATFORM_ENTER_CRITICAL();
	_status = READY;
	init();
	// if task keeps READY (not suspended nor waiting), then allows execution
	if(_status == READY){
		_event |= EVT_YIELD;
	}
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
bool Task::isYield(){
	return ((_status == YIELD)? true : false);
}

//------------------------------------------------------------------------------------
bool Task::isReady(){
	return ((_status == READY)? true : false);
}

//------------------------------------------------------------------------------------
void Task::setReady(uint32_t evt){
	PLATFORM_ENTER_CRITICAL();
	_event |= evt;
	_status = READY;
	if(_isSuspended){
		resume(true);
	}
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
void Task::setStop(){
	PLATFORM_ENTER_CRITICAL();
	_status = STOPPED;
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
void Task::execCb(){
	_status &= ~READY;
	if((_event & EVT_RESUMED)==EVT_RESUMED){
		_event &= ~EVT_RESUMED;
		onResume();
	}
	if((_event & EVT_TOPIC)==EVT_TOPIC){
		_event &= ~EVT_TOPIC;
		// processes all pending topics until topic fifo pool is empty
		for(;;){
			TopicData td;
			try {
				popTopic(&td);
			}
			catch(Exception &e){
				// discard exception (no more items in fifo, then exists)
				break;
			}
			onTopicUpdate(&td);
			// decrease topic count and if reaches 0, then notifies to publisher, that
			// topic has been completely processed by all the subscribers.
			*(td.pcount) -= 1;
			if(*(td.pcount) <= 0 && td.done){
				td.done(td.publisher);
			}
		}
	}
	if((_event & EVT_FLAGS)==EVT_FLAGS){
		PLATFORM_ENTER_CRITICAL();
		int event = (_event >> 4);
		// checks if waiting_or or waiting_and
		if(_evhandler.mode == WAIT_OR && (_evhandler.events & event)!=0){
			_evhandler.events &= ~event;
			_event &= ~EVT_FLAGMASK;
			PLATFORM_EXIT_CRITICAL();
			onEventFlag(event);
		}
		else if(_evhandler.mode == WAIT_AND && (_evhandler.events & event)==_evhandler.events){
			_evhandler.events &= ~event;
			_event &= ~EVT_FLAGMASK;
			PLATFORM_EXIT_CRITICAL();
			onEventFlag(event);
		}
		else{
			PLATFORM_EXIT_CRITICAL();
		}
	}
	if(_event == EVT_YIELD){
		_event &= ~EVT_YIELD;
		onYieldTurn();
	}
}

//------------------------------------------------------------------------------------
uint8_t Task::getPrio(){
	return _prio;
}

//------------------------------------------------------------------------------------
const char * Task::getName(){
	return _name;
}

//------------------------------------------------------------------------------------
Timer* Task::getTimer(){
	return _tmr;
}

//------------------------------------------------------------------------------------
void Task::addTopic(TopicData * topic) throw (Exception){
	if(!topic){
		throw Exception(Exception::BAD_ARGUMENT, "Task::addTopic topic null");
	}
	PLATFORM_ENTER_CRITICAL();
	uint16_t topicsize = sizeof(TopicData);
	try{
		_topicpool->push(topic, &topicsize);
	}
	catch(Exception &e){
		PLATFORM_EXIT_CRITICAL();
		throw e;
	}
	PLATFORM_EXIT_CRITICAL();
	setReady(EVT_TOPIC);
}

//------------------------------------------------------------------------------------
void Task::popTopic(TopicData* topic) throw (Exception){
	if(!topic){
		throw Exception(Exception::BAD_ARGUMENT, "Task::popTopic topic null");
	}
	PLATFORM_ENTER_CRITICAL();
	uint16_t topicsize = sizeof(TopicData);
	try{
		_topicpool->pop(topic, &topicsize);
	}
	catch(Exception &e){
		PLATFORM_EXIT_CRITICAL();
		throw e;
	}
	PLATFORM_EXIT_CRITICAL();
}


//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

Task::Task(const char* name, uint8_t prio, int topic_pool_size) throw (Exception){
	// create timer task
	_tmr = new Timer();
	// create topic pool if required
	_topicpool = 0;
	if(topic_pool_size)
		_topicpool = new Fifo(topic_pool_size);
	// initializes internal data
	_status = STOPPED;
	_isSuspended = false;
	_event = EVT_NONE;
	_name = name;
	if(!name)
		_name = "";
	_evhandler.mode = WAIT_AND;
	_evhandler.events = 0;
	_prio = prio;
	OS::addTask(this);
}
//------------------------------------------------------------------------------------
Task::~Task() {
	delete _tmr;
	delete _topicpool;
}

//------------------------------------------------------------------------------------
void Task::suspend(uint32_t delay_us) throw (Exception){
	PLATFORM_ENTER_CRITICAL();
	_isSuspended = true;
	try{
		_tmr->start(delay_us, 0, timertask_callback, this);
		PLATFORM_EXIT_CRITICAL();
	}
	catch(Exception &e){
		PLATFORM_EXIT_CRITICAL();
		throw e;
	}
}

//------------------------------------------------------------------------------------
void Task::resume(bool forced){
	PLATFORM_ENTER_CRITICAL();
	// if resume forced, stops running timer, and do not apply READY state.
	if(forced){
		_isSuspended = false;
		_tmr->stop();
		PLATFORM_EXIT_CRITICAL();
		return;
	}
	_isSuspended = false;
	_status = READY;
	_event &= ~EVT_YIELD;
	_event |= EVT_RESUMED;
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
void Task::yield(){
	_status = YIELD;
}

//------------------------------------------------------------------------------------
void Task::wait_or(uint16_t evt, uint32_t delay_us) throw (Exception){
	PLATFORM_ENTER_CRITICAL();
	_evhandler.mode = WAIT_OR;
	_evhandler.events = evt;
	if(delay_us > 0){
		try{
			suspend(delay_us);
		}
		catch(Exception &e){
			PLATFORM_EXIT_CRITICAL();
			throw e;
		}
	}
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
void Task::wait_and(uint16_t evt, uint32_t delay_us) throw (Exception){
	PLATFORM_ENTER_CRITICAL();
	_evhandler.mode = WAIT_AND;
	_evhandler.events = evt;
	if(delay_us > 0){
		try{
			suspend(delay_us);
		}
		catch(Exception &e){
			PLATFORM_EXIT_CRITICAL();
			throw e;
		}
	}
	PLATFORM_EXIT_CRITICAL();
}

