/*
 * Task.h
 *
 *  Created on: 11/3/2015
 *      Author: raulMrello
 */

#ifndef MMF_OS_TASK_TASK_H_
#define MMF_OS_TASK_TASK_H_

//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include <stdint.h>
#include "Exception.h"
#include "Timer.h"
#include "Topic.h"
#include "Fifo.h"

namespace MMF {


//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** Pointer to Task object */
typedef struct Task_t *  TaskPtr;

/** Type definitions for callback handler  */
typedef void *	TaskHandlerObj;

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
//-- CLASS ---------------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** \class Task
 *  \brief Tasks are pieces of software capable to be executed in a run-to-completion manner, this
 *  implies that they must return to allow other tasks to be executed (they never must block).
 *  Tasks can be STOPPED during initialization, WAITING for an event, READY for execution,
 *  RUNNING during execution and YIELD if allow other tasks to take control of the cpu.
 */
class Task {
public:

	/** \fn Task
	 *  \brief Creates a task, assigns name, priority and creates a topic pool fifo.
	 *  \param Task name
	 *  \param prio Priority
	 *  \param topic_pool_size Number of entries of the topic pool
	 *  \throw Exception
	 */
	Task(const char* name, uint8_t prio, int topic_pool_size) throw (Exception);

	/** \fn ~Task
	 * 	\brief destructor
	 */
	virtual ~Task();

	/** \fn suspend
	 *  \brief Suspends a task during several microseconds
	 *  \param delay_us Microsendos to suspend
	 *  \throw Exception
	 */
	void suspend(uint32_t delay_us) throw (Exception);

	/** \fn resume
	 *  \brief Sets a task ready after a period of suspension
	 *  \param forced  Boolean flag to force a resume while the task is under suspension and
	 *  its timer has not yet finished.
	 */
	void resume(bool forced);

	/** \fn yield
	 *  \brief Sets a task yielded to allow other tasks to take control of the cpu
	 */
	void yield();

	/** \fn wait_or
	 *  \brief Task keeps waiting for any of the events declared
	 *  \param evt Combination of events
	 *  \param delay_us Timeout limit in microseconds for wait operation. If 0 waits forever.
	 *  \throw Exception
	 */
	void wait_or(uint16_t evt, uint32_t delay_us) throw (Exception);

	/** \fn wait_and
	 *  \brief Task keeps waiting for all of the events declared
	 *  \param t Task
	 *  \param evt Combination of events
	 *  \param delay_us Timeout limit in microseconds for wait operation. If 0 waits forever.
	 *  \throw Exception
	 */
	void wait_and(uint16_t evt, uint32_t delay_us) throw (Exception);

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

	/** \fn start
	 *  \brief Starts task (invokes init callback internally)
	 */
	void start();

	/** \fn isYield
	 *  \brief Checks if it is yielded
	 *  \return true or false
	 */
	bool isYield();

	/** \fn isReady
	 *  \brief Checks if it is ready for execution
	 *  \return true or false
	 */
	bool isReady();

	/** \fn setReady
	 *  \brief Sets a task ready, indicating which event has caused it.
	 *  \param evt Event code which causes the state change
	 */
	void setReady(uint32_t evt);

	/** \fn setStop
	 *  \brief Sets a task stopped
	 */
	void setStop();

	/** \fn Task_getPrio
	 *  \brief Get task priority
	 *  \return Priority
	 */
	uint8_t getPrio();

	/** \fn getName
	 *  \brief Get task name
	 *  \return Task name
	 */
	const char * getName();

	/** \fn getTimer
	 *  \brief Get task timer
	 *  \return Task timer
	 */
	Timer* getTimer();

	/** \fn Task_exec
	 *  \brief Executes task exec callback
	 */
	void execCb();

	/** \fn addTopic
	 *  \brief Adds a new topic update to the topic pool
	 *  \param topic Topic data
	 *  \throw Exception
	 */
	void addTopic(TopicData *td) throw (Exception);

	/** \fn popTopic
	 *  \brief Removes a topic from to the topic pool
	 *  \param td Pointer to receive topic data
	 *  \throw Exception
	 */
	void popTopic(TopicData *td) throw (Exception);

protected:
	typedef uint32_t EvtWaitModeEnumFlags;
	typedef uint32_t EventEnumFlags;
	typedef uint32_t StatEnumFlags;

	/** Task interface */
	virtual void init() = 0;
	virtual void onYieldTurn() = 0;
	virtual void onResume() = 0;
	virtual void onEventFlag(uint16_t event) = 0;
	virtual void onTopicUpdate(TopicData * topicdata) = 0;

	/** Event flag structure that manages the way of event flag waiting. */
	typedef struct {
		/** task properties */
		EvtWaitModeEnumFlags mode;		///< Waiting mode
		EventEnumFlags events;			///< Event flag combination
	}EvtFlag;

	const char * _name;			///< Task name
	StatEnumFlags _status;		///< Task status flag
	EventEnumFlags _event;		///< Pending event flags (include topics, suspensions and eventflags)
	Fifo* _topicpool;			///< Topic pool buffer
	EvtFlag _evhandler;			///< Event flag handler structure
	Timer* _tmr;				///< Timer for suspension operations
	uint8_t _prio;				///< Task priority in the range PRIO_CRITICAL .. PRIO_LOW + SUBPRIO_MIN
	bool _isSuspended;			///< Suspension flag
};
}

#endif /* MMF_OS_TASK_TASK_H_ */
