/*
 * Exception.cpp
 *
 *  Created on: 12/3/2015
 *      Author: dptoid
 */

#include "Exception.h"
using namespace MMF;

//------------------------------------------------------------------------------------
Exception::Exception(ExceptionCode c, const char * msg) {
	_code = c;
	_msg = msg;
}

//------------------------------------------------------------------------------------
Exception::~Exception() {
	// TODO Auto-generated destructor stub
}

//------------------------------------------------------------------------------------
const char * Exception::getMessage(){
	return _msg;
}

//------------------------------------------------------------------------------------
int Exception::getCode(){
	return (int)_code;
}
