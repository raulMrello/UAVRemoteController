/*
 * os.h
 *
 *  Created on: 11/3/2015
 *      Author: raulMrello
 *
 *  Cooperative Run-To-Completion kernel with fixed priorities. Designed for extremely low-scale
 *  microcontrollers with small FLASH and RAM footprints.
 *
 *  This is the procedure of use:
 *
 *  1 - Allocates RAM memory for a predefined number of tasks and initializes kernel internals.
 *  	It requires to define an Exception object to capture error conditions.
 *
 *  			Exception e = Exception_ALLOC();
 *  			#define NUM_TASKS	3
 *  			OS_ALLOC(tasklist, NUM_TASKS, &e);
 *  			catch(&e){
 *  				// add error handling code here
 *  			}
 *
 *  OS_ALLOC macro is translated into this code:
 *
 *  	OS_ALLOC(xxxx,N, &e); ========>	static Task* mmf_xxxx[N];
 *  									OS_init(mmf_xxxx, N, &e);
 *
 *  2 - Task initialization. During task initialization Task_initialize, OS_start is invoked
 *
 *  			Task t;
 *  			Task_initialization(&t, ....., &e);
 *  			catch(&e){
 *  				// add error handling code here
 *  			}
 *
 *  2 - Scheduling startup. Scheduling can be executed in a blocked manner (doesn't return) with
 *  	primitive OS_schedule, or it can do a scheduling round along the tasklist and then returns.
 *  	This is done with primitive OS_schedule_once.
 *
 *  	You can do:
 *  						OS_schedule(&e);
 *  						// never reach this point, so you can check exceptions
 *  						catch(&e){
 *  						}
 *
 *		Or you can do:
 *							for(;;){
 *								OS_schedule_once(&e);
 *								catch(&e){
 *									break;
 *								}
 *							}
 *
 *
 *
 */

#ifndef OS_OS_H_
#define OS_OS_H_

#ifdef __cplusplus
 extern "C" {
#endif

//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include <stdint.h>
#include "Task.h"
#include "Exception.h"

//------------------------------------------------------------------------------------
//-- PROTOTYPES ----------------------------------------------------------------------
//------------------------------------------------------------------------------------


/** \fn OS_init
 *  \brief Initializes the kernel with a number of allowed tasks
 *  \param numTasks Max number of tasks handled by the kernel
 *  \param tick_us System tick in microseconds
 *  \param e Exception object
 */
void OS_init(uint8_t numTasks, uint32_t tick_us, ExceptionPtr e);


/** \fn OS_schedule
 *  \brief kernel scheduling mechanism. Evaluates next highest priority task and executes it.
 *  \param e Exception object
 */
void OS_schedule(ExceptionPtr e);

/** \fn OS_schedule_once
 *  \brief kernel scheduling mechanism. Evaluates next highest priority task and executes it. Then returns
 *  \param e Exception object
 */
void OS_schedule_once(ExceptionPtr e);

/** \fn OS_log
 *  \brief Generates a debug log with kernel statistics into a buffer passed as argument.
 *  \param logbuffer String to store kernel's status
 *  \param logbuffersize Max size allowed by the logbuffer
 */
void OS_log(char* logbuffer, int logbuffersize);

/** \fn OS_send_event
 *  \brief Sends an event to a task by its reference or by its name
 *  \param to Destination task reference
 *  \param taskname Destination task name (if parameter to=0)
 *  \param event Combination of event flags
 *  \param e Exception object
 */
void OS_send_event(TaskPtr to, const char * taskname, uint16_t event, ExceptionPtr e);

/** \fn OS_tick
 *  \brief Generates a kernel tick for suspended tasks
 *  \param e Exception object
 */
void OS_tick(ExceptionPtr e);

#ifdef __cplusplus
}
#endif

#endif /* OS_OS_H_ */
