/*
 * OS.h
 *
 *  Created on: 11/3/2015
 *      Author: raulMrello
 */

#ifndef OS_OS_H_
#define OS_OS_H_

#include <string.h>
#include "../port/platforms.h" ///< platform dependent
#include "Task.h"
#include "Memory.h"
#include "Exception.h"

namespace MMF {

/** \class OS
 *  \brief OS class defines the cooperative run-to-completion kernel. Tasks have fixed priorities
 *  and always is executed the highest priority one. Tasks can be suspended, yielded or waiting for
 *  a flag or a subscribed topic update. They can get activated due to these event sources:
 *   1. After the suspension timeout (if suspended status)
 *   2. After a complete scheduling cycle, all pending tasks are executed (if yielded status)
 *   3. After a topic update (if susbscribed to any kind of topic)
 *   4. [future work] After a flag update (if flagwait status)
 */
class OS {
public:
	//------------------------------------------------------------------------------------
	/** \fn init
	 *  \brief [static] Initializes kernel internals
	 *  \param numTasks Number of allowed tasks
	 *  \param tick_us System time tick in microseconds
	 *  \throw Exception
	 */
	static void init(uint8_t numTasks, uint32_t tick_us) throw (Exception){
		if(!numTasks || !tick_us){
			throw Exception(Exception::BAD_ARGUMENT, "OS::init no tasklist coherence");
		}
		_tasklist = (Task**)Memory::alloc(numTasks * sizeof(TaskPtr));
		for(uint8_t i=0; i<numTasks; i++){
			_tasklist[i] = 0;
		}
		_numTasks = numTasks;
		_tick_us = tick_us;
		_taskcount = 0;
	}

	//------------------------------------------------------------------------------------
	/** \fn addTask
	 *  \brief [static] Adds a task to the kernel.
	 *  \param t Task reference
	 *  \throws exception
	 */
	static void addTask(Task* t)  throw (Exception){
		if(!t){
			throw Exception(Exception::BAD_ARGUMENT, "OS::addTask t is null");
		}
		// if priority assigned, throws an exception
		uint8_t prio = t->getPrio();
		if(prio >= _numTasks){
			throw Exception(Exception::MEMORY_ALLOC, "OS::addTask task prio out of bounds");
		}

		if(_tasklist[prio] != 0){
			throw Exception(Exception::MEMORY_ALLOC, "OS::addTask task prio already assigned");
		}
		// inserts task in list, increase counter and starts it
		_tasklist[prio] = t;
		_taskcount++;
//		_tasklist[prio]->start();
	}

	//------------------------------------------------------------------------------------
	/** \fn kill
	 *  \brief [static] Deallocates a task out of the kernel. On error it throws exception code ERR_TASK_DEALLOC
	 *  \param t Task reference to be deallocated from the kernel
	 *  \throws exception
	 */
	static void killTask(Task* t)  throw (Exception){
		if(!t){
			throw Exception(Exception::BAD_ARGUMENT, "OS::killTask t is null");
		}
		uint8_t prio = t->getPrio();
		// if task doesn't match with requested one, throws exception
		if(_tasklist[prio] != t){
			throw Exception(Exception::MEMORY_ALLOC, "OS::killTask no matching task");
		}
		// get prio level, remove task from list and decrease counter
		_taskcount--;
		_tasklist[prio] = 0;
	}

	//------------------------------------------------------------------------------------
	/** \fn schedule
	 *  \brief [static] kernel scheduling mechanism. Evaluates next highest priority task and executes it.
	 *  \throws exception code ERR_TASK_ALLOC
	 */
	static void schedule() throw (Exception){
		for(int i=0; i < _taskcount; i++){
			_tasklist[i]->start();
		}
		sched(1);
	}

	//------------------------------------------------------------------------------------
	/** \fn scheduleOnce
	 *  \brief [static] kernel scheduling mechanism. Evaluates next highest priority task and executes it. Then returns
	 *  \throws exception code ERR_TASK_ALLOC
	 */
	static void scheduleOnce() throw (Exception){
		static bool firstTime = true;
		if(firstTime){
			firstTime = false;
			for(int i=0; i < _taskcount; i++){
				_tasklist[i]->start();
			}
		}
		sched(0);
	}

	//------------------------------------------------------------------------------------
	/** \fn getTracelog
	 *  \brief Generates a debug log with kernel statistics into a buffer passed as argument.
	 *  \param buffer String to store kernel's status
	 *  \param buffersize Max size allowed by the logbuffer
	 *  \throw Exception
	 */
	static void getTracelog(char* buffer, int buffersize) throw (Exception){

	}

	//------------------------------------------------------------------------------------
	/** \fn OS_sendEvent
	 *  \brief Sends an event to a task by its reference or by its name
	 *  \param to Destination task reference
	 *  \param taskname Destination task name (if parameter to=0)
	 *  \param event Combination of event flags
	 *  \throw Exception
	 */
	static void sendEvent(Task* to, const char * taskname, uint16_t event) throw (Exception){
		if((!to && !taskname) || !event){
			throw Exception(Exception::BAD_ARGUMENT, "OS::sendEvent task or event null");
		}
		// if no task ref, then search by name
		if(!to){
			for(int i = 0; i < _numTasks; i++){
				const char * name = _tasklist[i]->getName();
				if(strcmp(name, taskname) == 0){
					to = _tasklist[i];
					break;
				}
			}
			if(!to){
				throw Exception(Exception::BAD_ARGUMENT, "OS::sendEvent task not found");
			}
		}
		// Adapt to the task event handling format
		int evt = ((((int)event) << 4) | Task::EVT_FLAGS);
		to->setReady(evt);
		return;

	}


	//------------------------------------------------------------------------------------
	/** \fn OS_tick
	 *  \brief Generates a kernel tick for suspended tasks
	 */
	static void tick(){
		// executes a tick on each task.
		for(int i = 0; i < _numTasks; i++){
			Timer* tmr = _tasklist[i]->getTimer();
			tmr->tick();
		}
	}

	//------------------------------------------------------------------------------------
	/** \fn getTimeTicks
	 *  \brief Get systicks from a microsecond value
	 *  \param microseconds Microseconds value
	 *  \return Number of systicks
	 */
	static int getTimeTicks(int microseconds){
		 return (int)(microseconds/_tick_us);
	 }

private:
	static Task**  _tasklist;			///< List of max number of allocatable tasks
	static uint8_t _taskcount;			///< Count of current allocated tasks
	static uint8_t _numTasks;			///< Count of current allocated tasks
	static uint32_t _tick_us;			///< System tick in microseconds


	//------------------------------------------------------------------------------------
	/** \fn scheduleOnce
	 *  \brief [static] kernel scheduling mechanism. Evaluates next highest priority task and executes it. Then returns
	 *  \throws exception code ERR_TASK_ALLOC
	 */
	static void sched(bool forever) throw (Exception){
		do{
			int i;
			// search from highest priority to lowest one...
			for(i = 0; i < _numTasks; i++){
				// if task entry is not empty...
				if(_tasklist[i] != 0){
					// and allocated task is ready for execution...
					if(_tasklist[i]->isReady()){
						// get the event code which causes the task to be ready and passes to exec func.
						_tasklist[i]->execCb();
						break;
					}
				}
			}
			// after a complete cycle of scheduling (no more executable task pending)
			// yielded tasks must be set to READY again
			if(i >= _numTasks){
				for(i = 0; i < _numTasks; i++){
					// ensure check only allocated tasks
					if(_tasklist[i] != 0){
						// ensure set ready only those which are yielded
						if(_tasklist[i]->isYield()){
							_tasklist[i]->setReady(Task::EVT_YIELD);
						}
					}
				}
			}
		}while(forever);
	}

};
}

#endif /* OS_OS_H_ */
