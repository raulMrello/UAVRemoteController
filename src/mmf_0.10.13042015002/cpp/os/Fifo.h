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


//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include <stdint.h>
#include "Exception.h"

namespace MMF {

//------------------------------------------------------------------------------------
//-- CLASS ---------------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** \class Fifo
 *  \brief Fifo management class
 */
class Fifo {
public:

	/** \fn Fifo
	 *  \brief Default constructor. Sets a fifo size
	 *  \param size Fifo size in bytes
	 *  \throw Exception if size is NULL or no more HEAP space available
	 */
	Fifo(uint16_t size) throw (Exception);

	/** \fn ~Fifo
	 *  \brief Default destructor. It will deallocate all resources
	 */
	virtual ~Fifo();

	/**
	 * Clears fifo content
	 */
	void flush();

	/**
	 * Inserts data into the fifo
	 * @param data Data buffer to add
	 * @param size Pointer to the data buffer size. It is updated with the number of inserted bytes
	 * @throw Exception
	 * @return Pointer to the insertion point or NULL if fails.
	 */
	void* push(void* data, uint16_t *size) throw (Exception);

	/**
	 * Extracts data from the fifo
	 * @param data Data buffer to receive extracted bytes
	 * @param size Pointer to the size to be extracted. It is updated with the number of extracted bytes
	 * @throw Exception
	 * @return Pointer to the extraction point or NULL if fails.
	 */
	void* pop(void* data, uint16_t *size) throw (Exception);

	/**
	 * Gets a pointer to the actual "read" pointer of the Fifo, and stores in "size" parameter the
	 * number of bytes to the last Fifo memory address. Do not extracts any data!!!
	 * @param size Pointer to datasize. Receive the section size from the "read" pointer to the Fifo last address.
	 * @throw Exception
	 * @return Pointer to the data section
	 */
	void* getSectionToEnd(uint16_t *size) throw (Exception);

	/**
	 * Frees a data section from the Fifo. From the "section" position to the end.
	 * @param section Section address to free
	 * @param size Section size. Must be the same as (end_of_fifo - section)
	 * @throw Exception
	 */
	void popSection(void* section, uint16_t size) throw (Exception);

private:
	uint8_t	*_mem;		//!< Pointer to the allocated memory block
	uint16_t _size;		//!< fifo memory size
	uint16_t _free;		//!< free size
	uint16_t _pread;	//!< read pointer
	uint16_t _pwrite;	//!< write pointer
};
}


#endif	/* __SRC_OS_FIFO_H */
