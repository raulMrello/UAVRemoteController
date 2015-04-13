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

#ifdef __cplusplus
 extern "C" {
#endif

//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include <stdint.h>
#include "Exception.h"
#include "Timer.h"
#include "Topic.h"


//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** Pointer to Task object */
typedef struct Task_t *  TaskPtr;

/** Type definitions for callback handler  */
typedef void *	TaskHandlerObj;

/** Type definitions for callback declaration */
typedef void (*InitCallback)(TaskHandlerObj cbhandler);
typedef void (*OnYieldTurnCallback)(TaskHandlerObj cbhandler);
typedef void (*OnResumeCallback)(TaskHandlerObj cbhandler);
typedef void (*OnEventFlagCallback)(TaskHandlerObj cbhandler, uint16_t event);
typedef void (*OnTopicUpdateCallback)(TaskHandlerObj cbhandler, TopicData * topicdata);


/** Event codes supported by Tasks */
typedef enum {
	EVT_NONE = 			  0 ,	///< No pending events
	EVT_YIELD = 	(1 << 0),	///< event which sets a yield->ready transition
	EVT_RESUMED = 	(1 << 1),	///< event which sets a suspended->ready transition
	EVT_TOPIC = 	(1 << 2),	///< event due to a topic update
	EVT_FLAGS = 	(1 << 3),	///< event which sets a flagwait->ready transition
	EVT_FLAGMASK =  (0xffffffff & ~(EVT_TOPIC|EVT_RESUMED|EVT_YIELD))
}EventEnum;

/** Task status codes */
typedef enum {
	RUNNING = 	      0 ,		///< task is executing
	STOPPED = 	(1 << 0),		///< task stopped
	READY = 	(1 << 1),		///< task ready for execution due to some event
	YIELD = 	(1 << 2)		///< task is yielded temporarily
}StatEnum;

/** Types of task event flag waiting mode (or, and) */
typedef enum {
	WAIT_OR,		///< Wait for any event combination
	WAIT_AND		///< Wait for all event combination
}EvtWaitModeEnum;


//------------------------------------------------------------------------------------
//-- DEFINES -------------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** Max priority level. */
#define PRIO_MAX		(int)0

/** Macros to convert time units */
#define SECONDS(v)		(v*1000000)		///< time conversion us -> sec
#define MILLIS(v)		(v*1000)		///< time conversion us -> us
#define MICROS(v)		(v)				///< time macro in usec


//------------------------------------------------------------------------------------
//-- PROTOTYPES ----------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** \fn Task_create
 *  \brief Creates a task, assigns name, priority and creates a topic pool fifo.
 *  \param t Task
 *  \param Task name
 *  \param prio Priority
 *  \param topic_pool_size Number of entries of the topic pool
 *  \param init Initialization callback
 *  \param onYieldTurn On yield turn callback
 *  \param onResume On resume callback
 *  \param onEventFlag On event flag callback
 *  \param onTopicUpdate On topic update callback
 *  \param cbhandler Callbacks handler object
 *  \param e Exception code
 */
TaskPtr Task_create(	const char* name,
						uint8_t prio,
						int topic_pool_size,
						InitCallback init,
						OnYieldTurnCallback onYieldTurn,
						OnResumeCallback onResume,
						OnEventFlagCallback onEventFlag,
						OnTopicUpdateCallback onTopicUpdate,
						void * cbhandler,
						ExceptionPtr e);

/** \fn Task_suspend
 *  \brief Suspends a task during several microseconds
 *  \param t Task
 *  \param delay_us Microsendos to suspend
 *  \param e Exception code (0: success)
 */
void Task_suspend(TaskPtr t, uint32_t delay_us, ExceptionPtr e);

/** \fn Task_resume
 *  \brief Sets a task ready after a period of suspension
 *  \param t Task
 *  \param forced  Boolean flag to force a resume while the task is under suspension and
 *  its timer has not yet finished.
 *  \param e Exception code (0: success)
 */
void Task_resume(TaskPtr t, uint8_t forced, ExceptionPtr e);

/** \fn Task_yield
 *  \brief Sets a task yielded to allow other tasks to take control of the cpu
 *  \param t Task
 *  \param e Exception code (0: success)
 */
void Task_yield(TaskPtr t, ExceptionPtr e);

/** \fn Task_wait_or
 *  \brief Task keeps waiting for any of the events declared
 *  \param t Task
 *  \param evt Combination of events
 *  \param delay_us Timeout limit in microseconds for wait operation. If 0 waits forever.
 *  \param e Exception code (0: success)
 */
void Task_wait_or(TaskPtr t, uint16_t evt, uint32_t delay_us, ExceptionPtr e);

/** \fn Task_wait_and
 *  \brief Task keeps waiting for all of the events declared
 *  \param t Task
 *  \param evt Combination of events
 *  \param delay_us Timeout limit in microseconds for wait operation. If 0 waits forever.
 *  \param e Exception code (0: success)
 */
void Task_wait_and(TaskPtr t, uint16_t evt, uint32_t delay_us, ExceptionPtr e);

#ifdef __cplusplus
}
#endif

#endif /* MMF_OS_TASK_TASK_H_ */
