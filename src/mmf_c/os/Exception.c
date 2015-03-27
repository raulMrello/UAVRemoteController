/*
 * Exception.cpp
 *
 *  Created on: 12/3/2015
 *      Author: dptoid
 */

#include "Exception.h"

//------------------------------------------------------------------------------------
void Exception_throw(Exception * e, ExceptionCode code, const char * message){
	if(e){
		e->code = code;
		e->msg = message;
	}
}

//------------------------------------------------------------------------------------
char Exception_raised(Exception * e){
	if(e){
		if(e->code != NO_EXCEPTIONS)
			return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------------
void Exception_clear(Exception * e){
	e->code = NO_EXCEPTIONS;
	e->msg = "";
}
