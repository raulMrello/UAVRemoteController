/*
 * Fifo.cpp
 *
 *  Created on:	09/04/2015
 *  Author: 	raulMrello
 *
 *  Fifo module, manages fifo memory blocks.
 */

#include "Fifo.h"
#include "Memory.h"
#include "../port/platforms.h"	///< required for critical sections management
using namespace MMF;

//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

Fifo::Fifo(uint16_t size) throw (Exception){
	if(!size){
		throw Exception(Exception::BAD_ARGUMENT, "Fifo::Fifo size is null");
	}
	_mem = (uint8_t*)Memory::alloc(size);
	PLATFORM_ENTER_CRITICAL();
	_free = size;
	_size = size;
	_pread = 0;
	_pwrite = 0;
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
Fifo::~Fifo(){
	Memory::free(_mem);
}

//------------------------------------------------------------------------------------
void Fifo::flush(){
	PLATFORM_ENTER_CRITICAL();
    _pwrite = 0;
    _pread = 0;
    _free = _size;
    PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
void* Fifo::push(void* data, uint16_t *size) throw (Exception){
	uint8_t* pMsg;
	uint16_t i, pushed = 0;
	if(data == 0 || !size || *size == 0){
		throw Exception(Exception::BAD_ARGUMENT, "Fifo::push some parameter is null");
	}
	PLATFORM_ENTER_CRITICAL();
    // check enough free memory
    if(_free == 0){
        *size = 0;
        PLATFORM_EXIT_CRITICAL();
        throw Exception(Exception::MEMORY_ALLOC, "Fifo::push fifo is full");
    }

    // pointer to the insertion position
    pMsg = (uint8_t *)(_mem + _pwrite);

     // write size bytes
    for(i = 0; i < *size; i++){
    	_mem[_pwrite++] = *(uint8_t*)((int)data+i);
        pushed++;
        if(_pwrite >= _size)
        	_pwrite = 0;
        if(--_free == 0){
        	if(pushed < *size){
        		throw Exception(Exception::MEMORY_ALLOC, "Fifo::push fifo has filled at all");
        	}
        	*size = pushed;
        	PLATFORM_EXIT_CRITICAL();
        	return pMsg;
        }
    }
    *size = pushed;
    PLATFORM_EXIT_CRITICAL();
    return pMsg;
}

//------------------------------------------------------------------------------------
void* Fifo::pop(void* data, uint16_t *size) throw (Exception){
	uint8_t* pMsg;
	uint8_t i, poped = 0;
	if(data == 0 || !size || *size == 0){
		throw Exception(Exception::BAD_ARGUMENT, "Fifo::pop some parameter is null");
	}
	PLATFORM_ENTER_CRITICAL();
    // check if fifo has data
    if(_free == _size){
        *size = 0;
        PLATFORM_EXIT_CRITICAL();
        throw Exception(Exception::MEMORY_DEALLOC, "Fifo::pop fifo is empty");
    }

    // pointer to the extraction position
    pMsg = (uint8_t *)(_mem + _pread);

    // extract size bytes
   for(i = 0; i < *size; i++){
	   *(uint8_t*)((int)data+i) = _mem[_pread++];
       poped++;
       if(_pread >= _size)
       	_pread = 0;
       if(++_free == _size){
       	if(poped < *size){
       		throw Exception(Exception::MEMORY_DEALLOC, "Fifo::pop fifo has emptied at all");
       	}
       	*size = poped;
       	PLATFORM_EXIT_CRITICAL();
       	return pMsg;
       }
   }
   *size = poped;
   PLATFORM_EXIT_CRITICAL();
   return pMsg;
}

//------------------------------------------------------------------------------------
void* Fifo::getSectionToEnd(uint16_t *size) throw (Exception){
	uint8_t* pMsg;
	if(!size){
		throw Exception(Exception::BAD_ARGUMENT, "Fifo::getSectionToEnd some parameter is null");
	}
	PLATFORM_ENTER_CRITICAL();
    // check if fifo is empty
    if(_free == _size){
        *size = 0;
        PLATFORM_EXIT_CRITICAL();
        throw Exception(Exception::MEMORY_DEALLOC, "Fifo::getSectionToEnd fifo is empty");
    }

    // pointer to the extraction position
    pMsg = (uint8_t *)(_mem + _pread);
	if(_pread < _pwrite){
		*size = (uint8_t)(_pwrite - _pread);
	}
	else{
		*size = (uint8_t)(_size - _pread);
	}
	PLATFORM_EXIT_CRITICAL();
	return pMsg;
}

//------------------------------------------------------------------------------------
void Fifo::popSection(void* section, uint16_t size) throw (Exception){
	PLATFORM_ENTER_CRITICAL();
	// check section and size are coherent with fifo content
	if(section != (void *)(_mem + _pread) || size > (_size - _pread)){
		PLATFORM_EXIT_CRITICAL();
		throw Exception(Exception::BAD_ARGUMENT, "Fifo_getSectionToEnd section or size are wrong");
	}

	// frees section
	_pread += size;
	if(_pread >= _size){
		_pread -= _size;
	}
	_free += size;
	PLATFORM_EXIT_CRITICAL();
}


