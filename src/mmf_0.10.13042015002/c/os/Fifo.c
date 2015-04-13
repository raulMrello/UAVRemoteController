/*
 * Fifo.c
 *
 *  Created on:	09/04/2015
 *  Author: 	raulMrello
 *
 *  Fifo module, manages fifo memory blocks.
 */

#include "Fifo.h"

//------------------------------------------------------------------------------------
//-- PRIVATE TYPEDEFS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

/** Fifo data structure */
typedef struct {
	uint8_t	*mem;		//!< Pointer to the allocated memory block
	uint16_t size;		//!< fifo memory size
	uint16_t free;		//!< free size
	uint16_t pread;		//!< read pointer
	uint16_t pwrite;	//!< write pointer
}Fifo;

//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

FifoPtr Fifo_create(uint16_t size, ExceptionPtr e){
	Fifo* this;
	PLATFORM_ENTER_CRITICAL();
	this = (Fifo*)Memory_alloc(sizeof(Fifo), e);
	catch(e){
		PLATFORM_EXIT_CRITICAL();
		return 0;
	}
	this->mem = (uint8_t*)Memory_alloc(size, e);
	catch(e){
		Memory_free(this, e);
		PLATFORM_EXIT_CRITICAL();
		return 0;
	}
	this->free = size;
	this->size = size;
	this->pread = 0;
	this->pwrite = 0;
	PLATFORM_EXIT_CRITICAL();
	return (FifoPtr)this;
}

//------------------------------------------------------------------------------------
void Fifo_kill(FifoPtr* pfifo, ExceptionPtr e){
	Fifo** pthis = (Fifo**)pfifo;
	if(!pfifo || !pthis){
		Exception_throw(e, BAD_ARGUMENT, "Fifo_kill pfifo is null");
		return;
	}
	PLATFORM_ENTER_CRITICAL();
	Fifo* this;
	this = *pthis;
	if(!this){
		PLATFORM_EXIT_CRITICAL();
		Exception_throw(e, BAD_ARGUMENT, "Fifo_kill *pfifo is null");
		return;
	}
	Memory_free(this->mem, e);
	Memory_free(this, e);
	*pthis = 0;
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
void Fifo_flush(FifoPtr fifo, ExceptionPtr e){
	Fifo* this = (Fifo*)fifo;
	if(!this){
		Exception_throw(e, BAD_ARGUMENT, "Fifo_flush fifo is null");
		return;
	}
	PLATFORM_ENTER_CRITICAL();
    this->pwrite = 0;
    this->pread = 0;
    this->free = this->size;
    PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
void* Fifo_push(FifoPtr fifo, void* data, uint16_t *size, ExceptionPtr e){
	uint8_t* pMsg;
	uint16_t i, pushed = 0;
	Fifo* this = (Fifo*)fifo;
	if(!this || data == 0 || !size || *size == 0){
		Exception_throw(e, BAD_ARGUMENT, "Fifo_push some parameter is null");
		return 0;
	}
	PLATFORM_ENTER_CRITICAL();
    // check enough free memory
    if(this->free == 0){
        *size = 0;
        PLATFORM_EXIT_CRITICAL();
    	Exception_throw(e, MEMORY_ALLOC, "Fifo_push fifo is full");
		return 0;
    }

    // pointer to the insertion position
    pMsg = (uint8_t *)((uint32_t)this->mem + this->pwrite);

     // write size bytes
    for(i = 0; i < *size; i++){
    	this->mem[this->pwrite++] = *(uint8_t*)((uint32_t)data+i);
        pushed++;
        if(this->pwrite >= this->size)
        	this->pwrite = 0;
        if(--this->free == 0){
        	if(pushed < *size){
        		Exception_throw(e, MEMORY_ALLOC, "Fifo_push fifo has filled at all");
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
void* Fifo_pop(FifoPtr fifo, void* data, uint16_t *size, ExceptionPtr e){
	Fifo* this = (Fifo*)fifo;
	uint8_t* pMsg;
	uint8_t i, poped = 0;
	if(!this || data == 0 || !size || *size == 0){
		Exception_throw(e, BAD_ARGUMENT, "Fifo_pop some parameter is null");
		return 0;
	}
	PLATFORM_ENTER_CRITICAL();
    // check if fifo has data
    if(this->free == this->size){
        *size = 0;
        PLATFORM_EXIT_CRITICAL();
        Exception_throw(e, MEMORY_DEALLOC, "Fifo_pop fifo is empty");
		return 0;
    }

    // pointer to the extraction position
    pMsg = (uint8_t *)((uint32_t)this->mem + this->pread);

    // extract size bytes
   for(i = 0; i < *size; i++){
	   *(uint8_t*)((uint32_t)data+i) = this->mem[this->pread++];
       poped++;
       if(this->pread >= this->size)
       	this->pread = 0;
       if(++this->free == this->size){
       	if(poped < *size){
       		Exception_throw(e, MEMORY_DEALLOC, "Fifo_pop fifo has emptied at all");
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
void* Fifo_getSectionToEnd(FifoPtr fifo, uint16_t *size, ExceptionPtr e){
	Fifo* this = (Fifo*)fifo;
	uint8_t* pMsg;
	if(!this || !size){
		Exception_throw(e, BAD_ARGUMENT, "Fifo_getSectionToEnd some parameter is null");
		return 0;
	}
	PLATFORM_ENTER_CRITICAL();
    // check if fifo is empty
    if(this->free == this->size){
        *size = 0;
        PLATFORM_EXIT_CRITICAL();
        Exception_throw(e, MEMORY_DEALLOC, "Fifo_getSectionToEnd fifo is empty");
		return 0;
    }

    // pointer to the extraction position
    pMsg = (uint8_t *)((uint32_t)this->mem + this->pread);
	if(this->pread < this->pwrite){
		*size = (uint8_t)(this->pwrite - this->pread);
	}
	else{
		*size = (uint8_t)(this->size - this->pread);
	}
	PLATFORM_EXIT_CRITICAL();
	return pMsg;
}

//------------------------------------------------------------------------------------
void Fifo_popSection(FifoPtr fifo, void* section, uint16_t size, ExceptionPtr e){
	Fifo* this = (Fifo*)fifo;
	if(!this){
		Exception_throw(e, BAD_ARGUMENT, "Fifo_getSectionToEnd some parameter is null");
		return;
	}
	PLATFORM_ENTER_CRITICAL();
	// check section and size are coherent with fifo content
	if(section != (void *)((uint32_t)this->mem + this->pread) || size > (this->size - this->pread)){
		PLATFORM_EXIT_CRITICAL();
		Exception_throw(e, BAD_ARGUMENT, "Fifo_getSectionToEnd section or size are wrong");
		return;
	}

	// frees section
	this->pread += size;
	if(this->pread >= this->size){
		this->pread -= this->size;
	}
	this->free += size;
	PLATFORM_EXIT_CRITICAL();
}


