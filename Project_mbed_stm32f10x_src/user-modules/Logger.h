/*
 * Logger.h
 *
 *  Created on: 15/4/2015
 *      Author: raulMrello
 *
 *  Logger is a passive module that will print log messages to a 
 *	Serial link or a data memory pool.
 */

#ifndef SRC_PASSIVE_MODULES_LOGGER_H_
#define SRC_PASSIVE_MODULES_LOGGER_H_


//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "mbed.h"
#include "rtos.h"

//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//-- CLASS ---------------------------------------------------------------------------
//------------------------------------------------------------------------------------

class Logger{
public:
		
	/** Constructor, destructor, getter and setter */
	Logger(RawSerial *serial);
	Logger(char *pool, int size);
	virtual ~Logger();
	/** Interface */
	void print(char *text, int len);
	void sprintf(char *text, int value);
	

private:
	RawSerial *_serial;
	char * _pool;
	int _poolsize;
	int _poolpos;
	Mutex _mutex;
};


#endif /* SRC_PASSIVE_MODULES_LOGGER_H_ */
