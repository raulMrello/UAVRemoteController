/*
 * Memory.h
 *
 *  Created on:	09/04/2015
 *  Author: 	raulMrello
 *
 *  Memory module, manages dynamic memory allocation-deallocation, keeping track of the maximum number of allocated
 *  memory.
 */

#ifndef SRC_OS_MEMORY_H_
#define SRC_OS_MEMORY_H_


//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include <stdint.h>
#include <stdlib.h>			///< required for malloc|free
#include "Exception.h"

namespace MMF {

//------------------------------------------------------------------------------------
//-- CLASS ---------------------------------------------------------------------------
//------------------------------------------------------------------------------------

class Memory {
public:
	/**
	 * Allocates a memory block
	 * @param size Block size
	 * @throw Exception
	 * @return Pointer to the allocated block
	 */
	static void* alloc(uint32_t size) throw (Exception){
		void *ptr;
		if(!size){
			throw Exception(Exception::BAD_ARGUMENT, "Memory_alloc size is null");
		}
		ptr = malloc(size);
		if(!ptr){
			throw Exception(Exception::MEMORY_ALLOC, "Memory_alloc out of bounds");
		}
		_maxAllocation += size;
		return ptr;
	}

	/**
	 * Frees an allocated memory block
	 * @param ptr Pointer to the block
	 * @throw Exception
	 */
	static void free(void *ptr) throw (Exception){
		if(!ptr){
			throw Exception(Exception::BAD_ARGUMENT, "Memory_free ptr is null");
		}
		free(ptr);
	}

	/**
	 * Get maximum allocation memory
	 * @return Max allocated size ever
	 */
	static uint32_t getMaxAllocation(void){
		return _maxAllocation;
	}

private:
	/** Max allocated memory ever */
	static uint32_t _maxAllocation;
};
}

#endif	/* SRC_OS_MEMORY_H_ */
