/*
 * mmf.h
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 *
 *  MMF(_C)	is the ANSI C implementation of the Minimal Multithreaded Framework MMF.
 *  It provides a Run-To-Completion, Cooperative and Fixed-priority-based Kernel, in which
 *  tasks can be executed.
 *
 *  As tasks can not block, they must be driven by different event handling callbacks, which
 *  the kernel will invoke. Tasks do their work and return as soon as possible.
 *
 *  The number of task is not limitied, users can allocate enough space for whatever number
 *  of tasks, through the macro OS_ALLOC.
 *
 *  For example:	OS_ALLOC(mmf, 10); Allocates space for 10 tasks.
 *  				OS_ALLOC(mmf, 1000); Allocates space for 1000 tasks.
 *
 *  The basic procedure for using this framework is extremely simple, and reduced to 4 steps:
 *
 *  	1 - Kernel allocation and initialization (OS_ALLOC)
 *  	2 - [optionally] Topics initialization (if defined a publish/subscribe system)
 *  	3 - Task creation (Task_initialize)
 *  	4 - Kernel scheduling (OS_schedule)
 *
 */

#ifndef MMF_OS_MMF_H_
#define MMF_OS_MMF_H_

//------------------------------------------------------------------------------------
#define __MMF_C_VERSION "1.0"


//------------------------------------------------------------------------------------
/**
 * platforms.h header includes definitions according with the platform to port MMF. In
 * this file must be declared a pair of macros for task suspension and resuming:
 * PLATFORM_TIMER_START, PLATFORM_TIMER_STOP
 *
 * Users must adapt it according with their needs.
 */
#include "../port/platforms.h"

//------------------------------------------------------------------------------------
#include "OS.h"			///< required for OS primitives like: OS_ALLOC, OS_schedule ...
#include "Task.h"		///< required for Task primitives like: Task_initialize ...
#include "Topic.h"		///< required for Topic primitives like: Topic_notify ...
#include "Exception.h"	///< required for Exception primitives like: catch ...


#endif /* MMF_OS_MMF_H_ */
