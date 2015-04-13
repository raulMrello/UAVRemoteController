/*
 * Exception.h
 *
 *  Created on:	12/3/2015
 *  Author: 	raulMrello
 *
 *  Exceptions are objects which allow certain kind of error handling. There are several exception
 *  codes that MMF kernel can throw during execution. Nevertheless, users can expand these codes
 *  by declaring inherit objects. During exception rising, a text message can be assigned.
 *
 *  Macro Exception_ALLOC sets a default error code for the exception (=NO_EXCEPTIONS)
 *  On the other hand, macro "catch" allows to check if an exception has been thrown and proceed
 *  accordingly.
 *
 *  Example of use:
 *
 *  			Exception e = Exception_ALLOC();
 *  			... do something ...
 *  			Exception_throw(&e, BAD_ARGUMENT, "Example of error text message");
 *  			... do something more before error checking ...
 *  			catch(&e){
 *  				// add your error handling code here.
 *  			}
 */

#ifndef SRC_OS_EXCEPTION_H_
#define SRC_OS_EXCEPTION_H_

#include "../port/platforms.h" ///< platform dependent

namespace MMF {


//------------------------------------------------------------------------------------
//-- CLASS ---------------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** \class Exception
 *  \brief Exception class gives information about errors during execution
 */
class Exception {
public:

	/** Exception codes */
	typedef enum {
		NO_EXCEPTIONS,		///< No exceptions
		BAD_ARGUMENT,		///< A function argument out of range
		MEMORY_ALLOC,		///< Memory allocation error
		MEMORY_DEALLOC,		///< Memory deallocation error
		UNKNOWN_EXCEPTION	///< Other exceptions
	}ExceptionCode;

	/** \fn Exception
	 *  \brief Default constructor.
	 */
	Exception(ExceptionCode c = NO_EXCEPTIONS, const char * msg = "");

	/** \fn ~Exception
	 *  \brief Default destructor. It will deallocate all resources
	 */
	virtual ~Exception();

	/** \fn getMessage
	 *  \brief Get exception code message
	 *  \return Exception code message
	 */
	const char * getMessage();

	/** \fn getCode
	 *  \brief Get exception code
	 *  \return Exception code
	 */
	int getCode();

private:
 	const char * _msg;				///< Exception message
  	ExceptionCode _code;			///< Exception code
};
}

#endif /* SRC_OS_EXCEPTION_H_ */
