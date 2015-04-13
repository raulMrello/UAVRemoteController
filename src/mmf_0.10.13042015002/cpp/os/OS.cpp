/*
 * OS.cpp
 *
 *  Created on: 11/3/2015
 *      Author: raulMrello
 */


#include "OS.h"
using namespace MMF;

//------------------------------------------------------------------------------------
//-- PRIVATE VARIABLES ---------------------------------------------------------------
//------------------------------------------------------------------------------------

Task**  OS::_tasklist = 0;			///< List of max number of allocatable tasks
uint8_t OS::_taskcount = 0;			///< Count of current allocated tasks
uint8_t OS::_numTasks = 0;			///< Count of current allocated tasks
uint32_t OS::_tick_us = 0;			///< System tick in microseconds

