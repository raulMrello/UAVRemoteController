/*
 * Memory.cpp
 *
 *  Created on:	09/04/2015
 *  Author: 	raulMrello
 *
 *  Memory module, manages dynamic memory allocation-deallocation, keeping track of the maximum number of allocated
 *  memory.
 */

#include "Memory.h"
using namespace MMF;

//------------------------------------------------------------------------------------
//-- PRIVATE VARIABLES ---------------------------------------------------------------
//------------------------------------------------------------------------------------

/** Max allocated memory ever */
uint32_t Memory::_maxAllocation = 0;

