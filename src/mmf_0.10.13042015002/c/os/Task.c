/*
 * task.cpp
 *
 *  Created on: 11/3/2015
 *      Author: raulMrello
 */

#include "Task.h"
#include "Timer.h"
#include "Fifo.h"
#include "../port/platforms.h" 

#ifndef true
#define true (char)1
#endif

#ifndef false
#define false (char)0
#endif

//------------------------------------------------------------------------------------
//-- REQUIRED MMF DEPENDENCIES -------------------------------------------------------
//------------------------------------------------------------------------------------

/** Required local members from other mmf modules */
extern void OS_start(TaskPtr t, ExceptionPtr e);

//------------------------------------------------------------------------------------
//-- PRIVATE TYPEDEFS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

/** Event flag structure that manages the way of event flag waiting. */
typedef struct {
	/** task properties */
	EvtWaitModeEnum mode;		///< Waiting mode
	uint16_t events;			///< Event flag combination
}EvtFlag;

/** Tasks are pieces of software capable to be executed in a run-to-completion manner, this
 *  implies that they must return to allow other tasks to be executed (they never must block).
 *  Tasks can be STOPPED during initialization, WAITING for an event, READY for execution,
 *  RUNNING during execution and YIELD if allow other tasks to take control of the cpu. */
typedef struct {
	/** task properties */
	const char * name;			///< Task name
	uint8_t prio;				///< Task priority in the range PRIO_CRITICAL .. PRIO_LOW + SUBPRIO_MIN
	StatEnum status;			///< Task status flag
	uint8_t isSuspended;		///< Suspension flag
	uint32_t event;				///< Pending event flags (include topics, suspensions and eventflags)
	EvtFlag evhandler;			///< Event flag handler structure
	FifoPtr topicpool;			///< Topic pool buffer
	TimerPtr tmr;				///< Timer for suspension operations

	/** task interface */
	TaskHandlerObj cbhandler;				///< inherit object
	InitCallback init;						///< initialization callback
	OnYieldTurnCallback onYieldTurn;		///< execution callback on yield turn (NO_EVENTS)
	OnResumeCallback onResume;				///< execution callback on yield turn (NO_EVENTS)
	OnEventFlagCallback onEventFlag;		///< execution callback on event flag
	OnTopicUpdateCallback onTopicUpdate;	///< execution callback on topic update
}Task;

//------------------------------------------------------------------------------------
//-- PRIVATE PROTOTYPES --------------------------------------------------------------
//------------------------------------------------------------------------------------

/** \fn Task_start
 *  \brief Starts task (invokes init callback internally)
 *  \param t Task
 *  \param e Exception code (0: success)
 */
void Task_start(TaskPtr t, ExceptionPtr e);

/** \fn Task_isYield
 *  \brief Checks if it is yielded
 *  \param t Task
 *  \param e Exception code (0: success)
 *  \return true or false
 */
char Task_isYield(TaskPtr t, ExceptionPtr e);

/** \fn Task_isReady
 *  \brief Checks if it is ready for execution
 *  \param t Task
 *  \param e Exception code (0: success)
 *  \return true or false
 */
char Task_isReady(TaskPtr t, ExceptionPtr e);

/** \fn Task_setReady
 *  \brief Sets a task ready, indicating which event has caused it.
 *  \param task Task
 *  \param evt Event code which causes the state change
 *  \param e Exception code (0: success)
 */
void Task_setReady(TaskPtr task, uint32_t evt, ExceptionPtr e);

/** \fn Task_setStop
 *  \brief Sets a task stopped
 *  \param t Task
 *  \param e Exception code (0: success)
 */
void Task_setStop(TaskPtr t, ExceptionPtr e);

/** \fn Task_getPrio
 *  \brief Get task priority
 *  \param t Task
 *  \param e Exception code (0: success)
 *  \return Priority
 */
uint8_t Task_getPrio(TaskPtr t, ExceptionPtr e);

/** \fn Task_getName
 *  \brief Get task name
 *  \param t Task
 *  \param e Exception code (0: success)
 *  \return Task name
 */
const char * Task_getName(TaskPtr t, ExceptionPtr e);

/** \fn Task_getTimer
 *  \brief Get task timer
 *  \param t Task
 *  \param e Exception code (0: success)
 *  \return Task timer
 */
TimerPtr Task_getTimer(TaskPtr t, ExceptionPtr e);

/** \fn Task_exec
 *  \brief Executes task exec callback
 *  \param t Task
 *  \param evt Event
 *  \param e Exception code (0: success)
 */
void Task_execCb(TaskPtr t, ExceptionPtr e);

/** \fn Task_addTopic
 *  \brief Adds a new topic update to the topic pool
 *  \param t Task
 *  \param topic Topic data
 *  \param e Exception code (0: success)
 */
void Task_addTopic(TaskPtr t, TopicData *topic, ExceptionPtr e);

/** \fn Task_addTopic
 *  \brief Adds a new topic update to the topic pool
 *  \param t Task
 *  \param td Topic data extracted from topic fifo pool
 *  \param e Exception code (0: success)
 */
void Task_popTopic(TaskPtr t, TopicData *td, ExceptionPtr e);


//------------------------------------------------------------------------------------
//-- PRIVATE FUNCTIONS ---------------------------------------------------------------
//------------------------------------------------------------------------------------

static void timertask_callback(TaskHandlerObj handler){
	if(!handler) 
		return;
	Task* t = (Task*)handler;
	Task_resume((TaskPtr)t, false, 0);
}

//------------------------------------------------------------------------------------
void Task_start(TaskPtr task, ExceptionPtr e){
	if(!task){ 
		Exception_throw(e, BAD_ARGUMENT, "Task_start task is null");
		return; 
	}
	Task *this = (Task*)task;
	PLATFORM_ENTER_CRITICAL();
	this->status = READY;
	this->init(this->cbhandler);
	// if task keeps READY (not suspended nor waiting), then allows execution
	if(this->status == READY){
		this->event |= EVT_YIELD;
	}
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
char Task_isYield(TaskPtr task, ExceptionPtr e){
	if(!task){ 
		Exception_throw(e, BAD_ARGUMENT, "Task_isYield task is null");
		return false; 
	}
	Task *this = (Task*)task;
	return ((this->status == YIELD)? true : false);
}

//------------------------------------------------------------------------------------
char Task_isReady(TaskPtr task, ExceptionPtr e){
	if(!task){ 
		Exception_throw(e, BAD_ARGUMENT, "Task_isReady task is null");
		return false; 
	}
	Task *this = (Task*)task;
	return ((this->status == READY)? true : false);
}

//------------------------------------------------------------------------------------
void Task_setReady(TaskPtr task, uint32_t evt, ExceptionPtr e){
	if(!task){ 
		Exception_throw(e, BAD_ARGUMENT, "Task_setReady task is null");
		return; 
	}
	Task *this = (Task*)task;
	PLATFORM_ENTER_CRITICAL();
	this->event |= evt;
	this->status = READY;
	if(this->isSuspended){
		Task_resume((TaskPtr)this, true, e);
	}
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
void Task_setStop(TaskPtr task, ExceptionPtr e){
	if(!task){ 
		Exception_throw(e, BAD_ARGUMENT, "Task_setStop task is null");
		return; 
	}
	Task *this = (Task*)task;
	PLATFORM_ENTER_CRITICAL();
	this->status = STOPPED;
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
void Task_execCb(TaskPtr task, ExceptionPtr e){
	if(!task){ 
		Exception_throw(e, BAD_ARGUMENT, "Task_execCb task is null");
		return; 
	}
	Task *this = (Task*)task;
	this->status &= ~READY;
	if((this->event & EVT_RESUMED)==EVT_RESUMED && this->onResume){
		this->event &= ~EVT_RESUMED;
		this->onResume(this->cbhandler);
	}
	if((this->event & EVT_TOPIC)==EVT_TOPIC && this->onTopicUpdate){
		this->event &= ~EVT_TOPIC;
		// processes all pending topics until topic fifo pool is empty
		for(;;){
			TopicData td;
			Task_popTopic((TaskPtr)this, &td, e);
			catch(e){
				Exception_clear(e);
				break;
			}
			this->onTopicUpdate(this->cbhandler, &td);
			// decrease topic count and if reaches 0, then notifies to publisher, that
			// topic has been completely processed by all the subscribers.
			*(td.pcount) -= 1;
			if(*(td.pcount) <= 0 && td.done){
				td.done(td.publisher);
			}
		}
	}
	if((this->event & EVT_FLAGS)==EVT_FLAGS && this->onEventFlag){
		PLATFORM_ENTER_CRITICAL();
		int event = (this->event >> 4);
		// checks if waiting_or or waiting_and
		if(this->evhandler.mode == WAIT_OR && (this->evhandler.events & event)!=0){
			this->evhandler.events &= ~event;
			this->event &= ~EVT_FLAGMASK;
			PLATFORM_EXIT_CRITICAL();
			this->onEventFlag(this->cbhandler, event);
		}
		else if(this->evhandler.mode == WAIT_AND && (this->evhandler.events & event)==this->evhandler.events){
			this->evhandler.events &= ~event;
			this->event &= ~EVT_FLAGMASK;
			PLATFORM_EXIT_CRITICAL();
			this->onEventFlag(this->cbhandler, event);
		}
		else{
			PLATFORM_EXIT_CRITICAL();
		}
	}
	if(this->event == EVT_YIELD && this->onYieldTurn){
		this->event &= ~EVT_YIELD;
		this->onYieldTurn(this->cbhandler);
	}
}

//------------------------------------------------------------------------------------
uint8_t Task_getPrio(TaskPtr task, ExceptionPtr e){
	if(!task){ 
		Exception_throw(e, BAD_ARGUMENT, "Task_getPrio task is null");
		return UINT8_MAX;
	}
	Task *this = (Task*)task;
	return this->prio;
}

//------------------------------------------------------------------------------------
const char * Task_getName(TaskPtr task, ExceptionPtr e){
	if(!task){
		Exception_throw(e, BAD_ARGUMENT, "Task_getName task is null");
		return "";
	}
	Task *this = (Task*)task;
	return this->name;
}

//------------------------------------------------------------------------------------
TimerPtr Task_getTimer(TaskPtr task, ExceptionPtr e){
	if(!task){
		Exception_throw(e, BAD_ARGUMENT, "Task_getTimer task is null");
		return 0;
	}
	Task *this = (Task*)task;
	return this->tmr;
}

//------------------------------------------------------------------------------------
void Task_addTopic(TaskPtr task, TopicData * topic, ExceptionPtr e){
	if(!task || !topic){
		Exception_throw(e, BAD_ARGUMENT, "Task_addTopic task or topic null");
		return;
	}
	Task *this = (Task*)task;
	PLATFORM_ENTER_CRITICAL();
	uint16_t topicsize = sizeof(TopicData);
	Fifo_push(this->topicpool, topic, &topicsize, e);
	catch(e){
		PLATFORM_EXIT_CRITICAL();
		return;
	}
	PLATFORM_EXIT_CRITICAL();
	Task_setReady((TaskPtr)this, EVT_TOPIC, e);
}

//------------------------------------------------------------------------------------
void Task_popTopic(TaskPtr task, TopicData* td, ExceptionPtr e){
	if(!task || !td){
		Exception_throw(e, BAD_ARGUMENT, "Task_addTopic task or topic null");
		return;
	}
	Task *this = (Task*)task;
	PLATFORM_ENTER_CRITICAL();
	uint16_t topicsize = sizeof(TopicData);
	Fifo_pop(this->topicpool, td, &topicsize, e);
	PLATFORM_EXIT_CRITICAL();
}


//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

TaskPtr Task_create(	const char* name, uint8_t prio, int topic_pool_size, InitCallback init,
						OnYieldTurnCallback onYieldTurn, OnResumeCallback onResume,
						OnEventFlagCallback onEventFlag, OnTopicUpdateCallback onTopicUpdate,
						TaskHandlerObj cbhandler, ExceptionPtr e){
	Task *this = (Task*)Memory_alloc(sizeof(Task), e);
	catch(e){
		return 0;
	}
	// create timer task
	this->tmr = Timer_create(e);
	catch(e){
		Memory_free(this, e);
		return 0;
	}
	// create topic pool if required
	this->topicpool = 0;
	if(topic_pool_size){
		this->topicpool = Fifo_create(topic_pool_size, e);
		catch(e){
			Timer_kill((TimerPtr*)&this->tmr, e);
			Memory_free(this, e);
			return 0;
		}
	}
	// initializes internal data
	this->status = STOPPED;
	this->isSuspended = false;
	this->event = EVT_NONE;
	this->name = name;
	if(!name)
		this->name = "";
	this->evhandler = (EvtFlag){WAIT_AND, 0};
	this->prio = prio;
	this->init = init;
	this->onYieldTurn = onYieldTurn;
	this->onResume = onResume;
	this->onEventFlag = onEventFlag;
	this->onTopicUpdate = onTopicUpdate;
	this->cbhandler = (cbhandler)? cbhandler : this;
	OS_start((TaskPtr)this, e);
	return (TaskPtr)this;
}

//------------------------------------------------------------------------------------
void Task_suspend(TaskPtr task, uint32_t delay_us, ExceptionPtr e){
	if(!task){
		Exception_throw(e, BAD_ARGUMENT, "Task_suspend task is null");
		return;
	}
	Task *this = (Task*)task;
	PLATFORM_ENTER_CRITICAL();
	this->isSuspended = true;
	Timer_start(this->tmr, delay_us, 0, timertask_callback, this, e);
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
void Task_resume(TaskPtr task, uint8_t forced, ExceptionPtr e){
	if(!task){
		Exception_throw(e, BAD_ARGUMENT, "Task_resume task is null");
		return;
	}
	Task *this = (Task*)task;
	PLATFORM_ENTER_CRITICAL();
	// if resume forced, stops running timer, and do not apply READY state.
	if(forced){
		this->isSuspended = false;
		Timer_stop(this->tmr, e);
		PLATFORM_EXIT_CRITICAL();
		return;
	}
	this->isSuspended = false;
	this->status = READY;
	this->event &= ~EVT_YIELD;
	this->event |= EVT_RESUMED;
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
void Task_yield(TaskPtr task, ExceptionPtr e){
	if(!task){
		Exception_throw(e, BAD_ARGUMENT, "Task_yield task is null");
		return;
	}
	Task *this = (Task*)task;
	PLATFORM_ENTER_CRITICAL();
	this->status = YIELD;
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
void Task_wait_or(TaskPtr task, uint16_t evt, uint32_t delay_us, ExceptionPtr e){
	if(!task){
		Exception_throw(e, BAD_ARGUMENT, "Task_wait_or task null");
		return;
	}
	Task *this = (Task*)task;
	PLATFORM_ENTER_CRITICAL();
	this->evhandler.mode = WAIT_OR;
	this->evhandler.events = evt;
	if(delay_us > 0){
		Task_suspend((TaskPtr)this, delay_us, e);
	}
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
void Task_wait_and(TaskPtr task, uint16_t evt, uint32_t delay_us, ExceptionPtr e){
	if(!task){
		Exception_throw(e, BAD_ARGUMENT, "Task_wait_and task null");
		return;
	}
	Task *this = (Task*)task;
	PLATFORM_ENTER_CRITICAL();
	this->evhandler.mode = WAIT_AND;
	this->evhandler.events = evt;
	if(delay_us > 0){
		Task_suspend((TaskPtr)this, delay_us, e);
	}
	PLATFORM_EXIT_CRITICAL();
}


