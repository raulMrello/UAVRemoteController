/*
 * os.cpp
 *
 *  Created on: 11/3/2015
 *      Author: raulMrello
 */

#include "mmf.h"
#include <string.h>	///< required for strcmp

//------------------------------------------------------------------------------------
//-- REQUIRED MMF DEPENDENCIES -------------------------------------------------------
//------------------------------------------------------------------------------------

extern void Task_start(Task* t, Exception *e);
extern char Task_isYield(Task* t, Exception *e);
extern char Task_isReady(Task* t, Exception *e);
extern void Task_setReady(Task* t, int evt, Exception *e);
extern int Task_prio(Task* t, Exception *e);
extern void Task_execCb(Task* t, Exception *e);

//------------------------------------------------------------------------------------
//-- PRIVATE MEMBERS -----------------------------------------------------------------
//------------------------------------------------------------------------------------

Task** _tasklist;				///< List of max number of allocatable tasks
int _numTasks;
int _taskcount;					///< Count of current allocated tasks

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
/** \fn OS_init
 *  \brief Initializes kernel internals
 *  \param tasklist Tasklist array
 *  \param numTasks Number of tasks
 *  \param e Exception
 */
void OS_init(Task ** tasklist, int numTasks, Exception * e){
	int i;
	if(!tasklist || !numTasks){
		Exception_throw(e, BAD_ARGUMENT, "OS_init no tasklist coherence");
		return;
	}
	_tasklist = tasklist;
	_numTasks = numTasks;
	// clears task list and task count
	for(i = 0; i < _numTasks; i++){
		_tasklist[i] = 0;
	}
	_taskcount = 0;
}

//------------------------------------------------------------------------------------
void OS_start(Task* t, Exception *e){
	if(!t){
		Exception_throw(e, BAD_ARGUMENT, "OS_start [t] is null");
		return;
	}
	// if priority assigned, throws an exception
	if(_tasklist[t->prio] !=  0){
		Exception_throw(e, MEMORY_ALLOC, "OS_start task prio already assigned");
		return;
	}
	// inserts task in list, increase counter and starts it
	_tasklist[t->prio] = t;
	_taskcount++;
	Task_start(_tasklist[t->prio], e);
}

//------------------------------------------------------------------------------------
void OS_kill(Task* t, Exception *e){
	if(!t){
		Exception_throw(e, BAD_ARGUMENT, "OS_kill [t] is null");
		return;
	}
	// if task doesn't match with requested one, throws exception
	if(_tasklist[Task_prio(t, e)] != t){
		Exception_throw(e, MEMORY_ALLOC, "OS_kill no matching task");
		return;
	}
	// get prio level, remove task from list and decrease counter
	int prio = Task_prio(t, e);
	_taskcount--;
	_tasklist[prio] = 0;
}

//------------------------------------------------------------------------------------
static void schedule(char forever, Exception *e){
	// scheduler runs forever or not
	Exception_throw(e, NO_EXCEPTIONS, "");
	do{
		int i;
		// search from highest priority to lowest one...
		for(i = 0; i < _numTasks; i++){
			// if task entry is not empty...
			if(_tasklist[i] != 0){
				// and allocated task is ready for execution...
				if(Task_isReady(_tasklist[i], e)){
					// get the event code which causes the task to be ready and passes to exec func.
					Task_execCb(_tasklist[i], e);
					catch(e){
						return;
					}
					break;
				}
				catch(e){
					return;
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
					if(Task_isYield(_tasklist[i], e)){
						Task_setReady(_tasklist[i], EVT_YIELD, e);
					}
					catch(e){
						return;
					}
				}
			}
		}
	}while(forever);
}

//------------------------------------------------------------------------------------
void OS_schedule(Exception *e){
	schedule(1,e);
}

//------------------------------------------------------------------------------------
void OS_schedule_once(Exception *e){
	schedule(0,e);
}

//------------------------------------------------------------------------------------
void OS_log(char* logbuffer, int logbuffersize){
	/**
	 * Example of information
	 *
	 *  ===================================================
	 * | **OS**           NUM_TASKS: 4                     |
	 * |===================================================|
	 * |  TASK_ID   |    LAST_EXEC   |        STAT         |
	 * |============|================|=====================|
	 * |     0      |       -        |        READY        |
	 * |     1      |    resumed     |   RUNNING[TOPIC]    |
	 * |     11     |     TOPIC      |        YIELD        |
	 * |     21     |       -        |   RUNNING[SUSPEND]  |
	 *  ===================================================
	 *
	 * TODO:
	 *
	 * int size;
	 * char kernel_statistics[DEFINE_SIZE];
	 * // ... fill kernel_statistics array ...
	 * int copysize = (size > logbuffersize)? logbuffersize : size;
	 * memcpy(logbuffer, kernel_statistics, copysize);
	 *
	 */
}

//------------------------------------------------------------------------------------
void OS_send_event(Task * t, const char * taskname, uint16_t event, Exception * e){
	if((!t && !taskname) || !event){
		Exception_throw(e, BAD_ARGUMENT, "OS_send_event task or event null");
		return;
	}
	// if no task ref, then search by name
	if(!t){
		int i;
		for(i = 0; i < _numTasks; i++){
			if(strcmp(_tasklist[i]->name, taskname) == 0){
				t = _tasklist[i];
				break;
			}
		}
		// if not found raise exception
		if(i >= _numTasks){
			Exception_throw(e, BAD_ARGUMENT, "OS_send_event task not found");
			return;
		}
	}
	// Adapt to the task event handling format
	int evt = ((((int)event) << 4) | EVT_FLAGS);
	Task_setReady(t, evt, e);
}
