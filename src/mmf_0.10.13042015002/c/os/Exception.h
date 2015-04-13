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

#ifdef __cplusplus
 extern "C" {
#endif

//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "../port/platforms.h" ///< platform dependent

//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------

 /** Exception types gives information about errors during execution */
 typedef enum {
 	NO_EXCEPTIONS=0,	///< No exceptions
 	BAD_ARGUMENT,		///< A function argument out of range
 	MEMORY_ALLOC,		///< Memory allocation error
 	MEMORY_DEALLOC,		///< Memory deallocation error
 	UNKNOWN_EXCEPTION	///< Other exceptions
 }ExceptionCode;

 /** Exception data structure */
 typedef struct {
 	const char * msg;				///< Exception message
  	ExceptionCode code;				///< Exception code
 }Exception;
typedef Exception*	ExceptionPtr;	///< Pointer to Exception object


//------------------------------------------------------------------------------------
//-- DEFINES -------------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** \def EXCEPTION_INIT
 *  \brief Set default values for code and msg exception properties
 */
#define Exception_INIT()		{"", NO_EXCEPTIONS}

//------------------------------------------------------------------------------------
//-- PROTOTYPES ----------------------------------------------------------------------
//------------------------------------------------------------------------------------

/**
 *  \fn Exception_throw
 *  \brief Throws a exception
 *	\param e Exception code
 *	\return Exception message
 */
void Exception_throw(ExceptionPtr e, ExceptionCode code, const char * message);

/**
 *  \fn Exception_raised
 *  \brief Checks if exception code is != NO_EXCEPTIONS
 *	\param e Exception
 *	\return true is code is != NO_EXCEPTIONS
 */
char Exception_raised(ExceptionPtr e);

/**
 *  \fn Exception_clear
 *  \brief Clears exception code
 *	\param e Exception
 */
void Exception_clear(ExceptionPtr e);


/**
 *  \def catch
 *  \brief Checks if exception has raised an exception code
 *	\param e Exception
 */
#define catch(e)	if(Exception_raised(e))

#ifdef __cplusplus
}
#endif

#endif /* SRC_OS_EXCEPTION_H_ */
