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

extern void Task_start(TaskPtr t, ExceptionPtr e);
extern char Task_isYield(TaskPtr t, ExceptionPtr e);
extern char Task_isReady(TaskPtr t, ExceptionPtr e);
extern void Task_setReady(TaskPtr t, uint32_t evt, ExceptionPtr e);
extern void Task_execCb(TaskPtr t, ExceptionPtr e);
extern uint8_t Task_getPrio(TaskPtr t, ExceptionPtr e);
extern const char * Task_getName(TaskPtr t, ExceptionPtr e);
extern TimerPtr Task_getTimer(TaskPtr t, ExceptionPtr e);

//------------------------------------------------------------------------------------
//-- PRIVATE VARIABLES ---------------------------------------------------------------
//------------------------------------------------------------------------------------

static TaskPtr* _tasklist;			///< List of max number of allocatable tasks
static uint8_t _taskcount;			///< Count of current allocated tasks
static uint8_t _numTasks;			///< Count of current allocated tasks
static uint32_t _tick_us;			///< System tick in microseconds

//------------------------------------------------------------------------------------
//-- PRIVATE PROTOTYPES --------------------------------------------------------------
//------------------------------------------------------------------------------------

/** \fn OS_start
 *  \brief Starts a new task. Insert the task into the task list according with its priority.
 *  It can throws exceptions on null pointers or tasklist allocation violations
 *  \param t Task reference to be allocated in the kernel
 *  \param prio Task priority
 *  \param e Exception object
 */
void OS_start(TaskPtr t, ExceptionPtr e);

/** \fn OS_kill
 *  \brief Deallocates a task out of the kernel.
 *  \param t Task reference to be deallocated from the kernel
 *  \param e Exception object
 */
void OS_kill(TaskPtr t, ExceptionPtr e);

//------------------------------------------------------------------------------------
//-- PRIVATE FUNCTIONS ---------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
/** \fn OS_getTimeTicks
 *  \brief Get systicks from a microsecond value
 *  \param microseconds Microseconds value
 *  \return Number of systicks
 */
int OS_getTimeTicks(int microseconds){
	 return (int)(microseconds/_tick_us);
 }

//------------------------------------------------------------------------------------
void OS_start(TaskPtr t, ExceptionPtr e){
	if(!t){
		Exception_throw(e, BAD_ARGUMENT, "OS_start [t] is null");
		return;
	}
	// if priority assigned, throws an exception
	uint8_t prio = Task_getPrio(t, e);
	catch(e){
		return;
	}
	if(prio >= _numTasks){
		Exception_throw(e, MEMORY_ALLOC, "OS_start task prio out of bounds");
		return;
	}

	if(_tasklist[prio] != 0){
		Exception_throw(e, MEMORY_ALLOC, "OS_start task prio already assigned");
		return;
	}
	// inserts task in list, increase counter and starts it
	_tasklist[prio] = t;
	_taskcount++;
	Task_start(_tasklist[prio], e);
}

//------------------------------------------------------------------------------------
void OS_kill(TaskPtr t, ExceptionPtr e){
	if(!t){
		Exception_throw(e, BAD_ARGUMENT, "OS_kill [t] is null");
		return;
	}
	int prio = Task_getPrio(t, e);
	// if task doesn't match with requested one, throws exception
	if(_tasklist[prio] != t){
		Exception_throw(e, MEMORY_ALLOC, "OS_kill no matching task");
		return;
	}
	// get prio level, remove task from list and decrease counter
	_taskcount--;
	_tasklist[prio] = 0;
}
//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

void OS_init(uint8_t numTasks, uint32_t tick_us, ExceptionPtr e){
	int i;
	if(!numTasks || !tick_us){
		Exception_throw(e, BAD_ARGUMENT, "OS_init no tasklist coherence");
		return;
	}
	_tasklist = (TaskPtr*)Memory_alloc(numTasks * sizeof(TaskPtr), e);
	catch(e){
		return;
	}
	for(i=0; i<numTasks; i++){
		_tasklist[i] = 0;
	}
	_numTasks = numTasks;
	_tick_us = tick_us;
	_taskcount = 0;
}

//------------------------------------------------------------------------------------
static void schedule(char forever, ExceptionPtr e){
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
void OS_schedule(ExceptionPtr e){
	schedule(1,e);
}

//------------------------------------------------------------------------------------
void OS_schedule_once(ExceptionPtr e){
	schedule(0,e);
}

//------------------------------------------------------------------------------------
void OS_send_event(TaskPtr t, const char * taskname, uint16_t event, ExceptionPtr e){
	if((!t && !taskname) || !event){
		Exception_throw(e, BAD_ARGUMENT, "OS_send_event task or event null");
		return;
	}
	// if no task ref, then search by name
	if(!t){
		int i;
		for(i = 0; i < _numTasks; i++){
			const char * name = Task_getName(_tasklist[i], e);
			catch(e){
				return;
			}
			if(strcmp(name, taskname) == 0){
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

//------------------------------------------------------------------------------------
void OS_tick(ExceptionPtr e){
	int i;
	// executes a tick on each task.
	for(i = 0; i < _numTasks; i++){
		TimerPtr tmr = Task_getTimer(_tasklist[i], e);
		catch(e){
			continue;
		}
		Timer_tick(tmr, e);
		catch(e){
			return;
		}
	}
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
