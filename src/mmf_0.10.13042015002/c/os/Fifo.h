/*
 * Fifo.h
 *
 *  Created on:	09/04/2015
 *  Author: 	raulMrello
 *
 *  Fifo module, manages fifo memory blocks.
 */

#ifndef __SRC_OS_FIFO_H
#define __SRC_OS_FIFO_H

#ifdef __cplusplus
 extern "C" {
#endif


//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include <stdint.h>
#include "Memory.h"
#include "Exception.h"
#include "../port/platforms.h"	///< required for critical sections management

//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** Pointer to Fifo object */
typedef struct Fifo_t*  FifoPtr;


//------------------------------------------------------------------------------------
//-- PROTOTYPES ----------------------------------------------------------------------
//------------------------------------------------------------------------------------

 /**
  * Creates a fifo with a predefined size
  * @param size Fifo size
  * @param e Exception handler
  * @return Pointer to the allocated object
  */
 FifoPtr Fifo_create(uint16_t size, ExceptionPtr e);

/**
 * Deallocates a fifo. The variable that points to the Fifo object then is set to 0, to
 * indicate that Fifo is deallocated and now it points to NULL.
 * @param pfifo	Variable that points to a fifo object
 * @param e Exception handler
 */
void Fifo_kill(FifoPtr* pfifo, ExceptionPtr e);

/**
 * Clears fifo content
 * @param fifo Fifo object
 * @param e Exception handler
 */
void Fifo_flush(FifoPtr fifo, ExceptionPtr e);

/**
 * Inserts data into the fifo
 * @param fifo Fifo object
 * @param data Data buffer to add
 * @param size Pointer to the data buffer size. It is updated with the number of inserted bytes
 * @param e Exception handler
 * @return Pointer to the insertion point or NULL if fails.
 */
void* Fifo_push(FifoPtr fifo, void* data, uint16_t *size, ExceptionPtr e);

/**
 * Extracts data from the fifo
 * @param fifo Fifo object
 * @param data Data buffer to receive extracted bytes
 * @param size Pointer to the size to be extracted. It is updated with the number of extracted bytes
 * @param e Exception handler
 * @return Pointer to the extraction point or NULL if fails.
 */
void* Fifo_pop(FifoPtr fifo, void* data, uint16_t *size, ExceptionPtr e);

/**
 * Gets a pointer to the actual "read" pointer of the Fifo, and stores in "size" parameter the
 * number of bytes to the last Fifo memory address. Do not extracts any data!!!
 * @param fifo fifo object
 * @param size Pointer to datasize. Receive the section size from the "read" pointer to the Fifo last address.
 * @param e Exception handler
 * @return Pointer to the data section
 */
void* Fifo_getSectionToEnd(FifoPtr fifo, uint16_t *size, ExceptionPtr e);

/**
 * Frees a data section from the Fifo. From the "section" position to the end.
 * @param fifo fifo object
 * @param section Section address to free
 * @param size Section size. Must be the same as (end_of_fifo - section)
 * @param e Exception handler
 */
void Fifo_popSection(FifoPtr fifo, void* section, uint16_t size, ExceptionPtr e);

#ifdef __cplusplus
}
#endif


#endif	/* __SRC_OS_FIFO_H */
