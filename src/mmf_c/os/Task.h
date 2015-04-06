/*
 * task.h
 *
 *  Created on: 11/3/2015
 *      Author: raulMrello
 *
 *  Tasks are created statically and they can be initialized, suspended, resumed, yielded and
 *  executed due to a suspension timeout, an event flag change or a topic update. For all these
 *  cases, tasks must install several callback handling functions, which will be invoked on each
 *  execution case.
 *
 *  As this tasks runs in a run-to-completion manner, they NUST NOT block, and hence if they need
 *  to keep running forever, the way of doing is throug the primitive Task_yield.
 *
 *  Once invoked, the task won't be ready for execution again until the next roundtrip scheduling.
 *  That is, after one execution cycle of all the pending tasks (even those with lower priority).
 *
 *  Highest priority value is PRIO_MAX (=0), and they can go up to the number of tasks allowed by
 *  kernel. If you try to install a task with a priority level greater than the number of kernel tasks
 *  an exception MEMORY_ALLOC will be thrown.
 *
 *  Example of use:
 *
 *  		Exception ex = Exception_ALLOC();
 *  		Task t;
 *  		Task_initalize(&t, name, prio, callbacks....., &ex);
 *  		catch(&ex){
 *  			// add your error handling code here.
 *  		}
 *
 *  During that initialization, the kernel will invoke Task Init Callback to initialize its internal
 *  data. Then, the kernel will invoke the necessary event callback, depending on the event conditions.
 *  This is done through OS_schedule or OS_schedule_once.
 */

#ifndef MMF_OS_TASK_TASK_H_
#define MMF_OS_TASK_TASK_H_

#include "Exception.h"
#include "Topic.h"
#include "Timer.h"
#include <stdint.h>

/**
 *  \brief Macros to convert time units
 */
#define SECONDS(v)		(v*1000000)		///< time conversion us -> sec
#define MILLIS(v)		(v*1000)		///< time conversion us -> us
#define MICROS(v)		(v)				///< time macro in usec

/**
 * \def PRIO_MAX
 * \brief Max priority level.
 */
#define PRIO_MAX		(int)0


/**
 *  \enum Event
 *  \brief Event codes supported by Tasks
 */
typedef enum {
	EVT_NONE = 			  0 ,	///< No pending events
	EVT_YIELD = 	(1 << 0),	///< event which sets a yield->ready transition
	EVT_RESUMED = 	(1 << 1),	///< event which sets a suspended->ready transition
	EVT_TOPIC = 	(1 << 2),	///< event due to a topic update
	EVT_FLAGS = 	(1 << 3),	///< event which sets a flagwait->ready transition
	EVT_FLAGMASK =  (0xffffffff & ~(EVT_TOPIC|EVT_RESUMED|EVT_YIELD))
}Event;


/**
 *  \enum Stat
 *  \brief Task status codes
 */
typedef enum {
	RUNNING = 	      0 ,		///< task is executing
	STOPPED = 	(1 << 0),		///< task stopped
	READY = 	(1 << 1),		///< task ready for execution due to some event
	YIELD = 	(1 << 2)		///< task is yielded temporarily
}Stat;

/**
 *  \enum EvtWaitMode
 *  \brief Types of task event flag waiting mode (or, and)
 */
typedef enum {
	WAIT_OR,		///< Wait for any event combination
	WAIT_AND		///< Wait for all event combination
}EvtWaitMode;

/** \struct EvtFlag
 *  \brief Event flag structure that manages the way of event flag waiting.
 */
typedef struct {
	/** task properties */
	EvtWaitMode mode;			///< Waiting mode
	int events;					///< Event flag combination
}EvtFlag;

/**
 *  \def Type definitions for callback declaration
 */
typedef void (*InitCallback)(void* cbhandler);
typedef void (*OnYieldTurnCallback)(void* cbhandler);
typedef void (*OnResumeCallback)(void* cbhandler);
typedef void (*OnEventFlagCallback)(void* cbhandler, int event);
typedef void (*OnTopicUpdateCallback)(void* cbhandler, TopicData * topicdata);

/** \struct Task
 *  \brief Tasks are pieces of software capable to be executed in a run-to-completion manner, this
 *  implies that they must return to allow other tasks to be executed (they never must block).
 *  Tasks can be STOPPED during initialization, WAITING for an event, READY for execution,
 *  RUNNING during execution and YIELD if allow other tasks to take control of the cpu.
 */
typedef struct {
	/** task properties */
	char * name;				///< Task name
	int prio;					///< Task priority in the range PRIO_CRITICAL .. PRIO_LOW + SUBPRIO_MIN
	Stat status;				///< Task status flag
	char isSuspended;			///< Suspension flag
	int event;					///< Pending event flags
	EvtFlag evhandler;			///< Event flag handler structure
	TopicDataPool topicpool;	///< Topic pool buffer
	Timer tmr;					///< Timer for suspension operations
	/** task interface */
	void * cbhandler;						///< inherit object
	InitCallback init;						///< initialization callback
	OnYieldTurnCallback onYieldTurn;		///< execution callback on yield turn (NO_EVENTS)
	OnResumeCallback onResume;				///< execution callback on yield turn (NO_EVENTS)
	OnEventFlagCallback onEventFlag;		///< execution callback on event flag
	OnTopicUpdateCallback onTopicUpdate;	///< execution callback on topic update
}Task;


/** \def Task_ALLOC_TOPIC_POOL
 *  \brief Allocates memory for the topic pool of the task. A topic pool is necessary if the task is subscribed to
 *  several topics and it can receive multiple updates before its execution turn. 'max_topics_on_pool' limits the number of
 *  unattended topics before an exception raising.
 *  \param max_topics_on_pool Max number of unattended topics on pool
 */
#define Task_ALLOC_TOPIC_POOL(taskname, max_topics_on_pool) 		static TopicData mmf_topicpool_##taskname[max_topics_on_pool]

/** \fn Task_initialize
 *  \brief Default constructor. Task name can be assigned during object creation.
 *  \param t Task
 *  \param Task name
 *  \param prio Priority
 *  \param topic_pool Topic pool pointer
 *  \param topic_pool_size Number of entries of the topic pool
 *  \param init Initialization callback
 *  \param onYieldTurn On yield turn callback
 *  \param onResume On resume callback
 *  \param onEventFlag On event flag callback
 *  \param onTopicUpdate On topic update callback
 *  \param cbhandler Callbacks handler object
 *  \param e Exception code
 */
void Task_initialize(	Task* t,
						char* name,
						int prio,
						TopicData * topic_pool,
						int topic_pool_size,
						InitCallback init,
						OnYieldTurnCallback onYieldTurn,
						OnResumeCallback onResume,
						OnEventFlagCallback onEventFlag,
						OnTopicUpdateCallback onTopicUpdate,
						void * cbhandler,
						Exception *e);

/** \fn Task_suspend
 *  \brief Suspends a task during several microseconds
 *  \param t Task
 *  \param delay_us Microsendos to suspend
 *  \param e Exception code (0: success)
 */
void Task_suspend(Task* t, int delay_us, Exception *e);

/** \fn Task_resume
 *  \brief Sets a task ready after a period of suspension
 *  \param t Task
 *  \param forced  Boolean flag to force a resume while the task is under suspension and
 *  its timer has not yet finished.
 *  \param e Exception code (0: success)
 */
void Task_resume(Task* t, char forced, Exception *e);

/** \fn Task_yield
 *  \brief Sets a task yielded to allow other tasks to take control of the cpu
 *  \param t Task
 *  \param e Exception code (0: success)
 */
void Task_yield(Task* t, Exception *e);

/** \fn Task_wait_or
 *  \brief Task keeps waiting for any of the events declared
 *  \param t Task
 *  \param evt Combination of events
 *  \param delay_us Timeout limit in microseconds for wait operation. If 0 waits forever.
 *  \param e Exception code (0: success)
 */
void Task_wait_or(Task* t, uint16_t evt, uint32_t delay_us, Exception *e);

/** \fn Task_wait_and
 *  \brief Task keeps waiting for all of the events declared
 *  \param t Task
 *  \param evt Combination of events
 *  \param delay_us Timeout limit in microseconds for wait operation. If 0 waits forever.
 *  \param e Exception code (0: success)
 */
void Task_wait_and(Task* t, uint16_t evt, uint32_t delay_us, Exception *e);


#endif /* MMF_OS_TASK_TASK_H_ */
