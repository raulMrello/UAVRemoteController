/*
 * mmf.h
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 */

#ifndef SRC_OS_MMF_H_
#define SRC_OS_MMF_H_


//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "../port/platforms.h"	///< required for porting
#include "OS.h"					///< required for OS primitives like: OS_ALLOC, OS_schedule ...
#include "Task.h"				///< required for Task primitives like: Task_initialize ...
#include "Topic.h"				///< required for Topic primitives like: Topic_notify ...
#include "Exception.h"			///< required for Exception primitives like: catch ...
#include "Timer.h"				///< required for Timming primitives like: Timer_start ...
#include "Fifo.h"				///< required for Fifo management
#include "Memory.h"				///< required for Memory management
#include "List.h"				///< required for List management

//------------------------------------------------------------------------------------
//-- DEFINES -------------------------------------------------------------------------
//------------------------------------------------------------------------------------

#define __MMF_CPP_VERSION "0.10.13042015002"


#endif /* SRC_OS_MMF_H_ */
