/*----------------------------------------------------------------------------
 * Name:    Retarget.c
 * Purpose: 'Retarget' layer for target-dependent low level functions
 * Version: V1.0
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * Copyright (c) 2005-2007 Keil Software. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <rt_misc.h>


//#include "stm32f10x_usart.h"

#include "drv_UART1.h"
#include "drv_UART2.h"
#include "drv_UART3.h"

//define default output USART/UART
#define UART1
 

#pragma import(__use_no_semihosting_swi)


/*----------------------------------------------------------------------------*
  external functions
 *----------------------------------------------------------------------------*/


#ifdef UART1   		//UART 1 Enabled
	#define retarget_SendChar(b)		drv_UART1_SendChar(b)
	#define retarget_GetChar()		    drv_UART1_GetChar()
#elif defined UART2 //UART 2 Enabled
	#define retarget_SendChar(b)		drv_UART2_SendChar(b)
	#define retarget_GetChar()		    drv_UART2_GetChar()
#elif defined UART3 //UART 3 Enabled
	#define retarget_SendChar(b)		drv_UART3_SendChar(b)
	#define retarget_GetChar()		    drv_UART3_GetChar()
#else
	#error "USART/UART not defined"
#endif

	
	
struct __FILE {
  int handle;                 // Add whatever you need here 
};


extern FILE __stdout;
extern FILE __stdin;


/*----------------------------------------------------------------------------
  fputc
 *----------------------------------------------------------------------------*/
int fputc(int ch, FILE *f) {
  
  return  retarget_SendChar(ch);
}

/*----------------------------------------------------------------------------
  fgetc
 *----------------------------------------------------------------------------*/
int fgetc(FILE *f) {
  int ch;

  do 
  {  
	ch = retarget_GetChar ();
  }	
  while (ch == -1);
  
  return (ch);
}

/*----------------------------------------------------------------------------
  _ttywrch
 *----------------------------------------------------------------------------*/
void _ttywrch(int ch) {
 retarget_SendChar(ch);
}

/*----------------------------------------------------------------------------
  ferror
 *----------------------------------------------------------------------------*/
/*
int ferror(FILE *f) {
                              // Your implementation of ferror
  return EOF;
}
*/


