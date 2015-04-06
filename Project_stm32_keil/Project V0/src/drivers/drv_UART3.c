/*----------------------------------------------------------------------------
 * Name:    Usart.c
 * Purpose: USART usage for STM32
 * Version: V1.00
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * Copyright (c) 2005-2007 Keil Software. All rights reserved.
 *----------------------------------------------------------------------------*/


#include "stm32f10x.h"
#include <stdio.h>

#include "drv_UART1.h"
#include "drv_UART2.h"
#include "drv_UART3.h"
#include "drv_UART4.h"

#define USART_DEF							USART3
#define USART_DEF_IRQHandler       			USART3_IRQHandler
#define USART_DEF_Init(baud)   				drv_UART3_Init(baud)
#define USART_DEF_SendChar      			drv_UART3_SendChar
#define USART_DEF_GetChar      				drv_UART3_GetChar
#define USART_DEF_RegisterDevice(task,flag)	drv_UART3_RegisterDevice(task,flag)

#define UART_DEF_CTRL						uart3_ctrl

#define UART_DEF_CLOCK						RCC_APB1Periph_USART3
#define UART_DEF_CLOCK_FUNC					RCC_APB1PeriphClockCmd 
 

#define USART_DEF_TX_PIN					GPIO_Pin_10
#define USART_DEF_TX_PORT					GPIOB
#define USART_DEF_TX_CLOCK					RCC_APB2Periph_GPIOB

#define USART_DEF_RX_PIN					GPIO_Pin_11
#define USART_DEF_RX_PORT					GPIOB

#define USART_DEF_IRQn						USART3_IRQn
    



/*----------------------------------------------------------------------------
  Notes:
  The length of the receive and transmit buffers must be a power of 2.
  Each buffer has a next_in and a next_out index.
  If next_in = next_out, the buffer is empty.
  (next_in - next_out) % buffer_size = the number of characters in the buffer.
 *----------------------------------------------------------------------------*/
#define TBUF_SIZE   128	     /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/
#define RBUF_SIZE   128      /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/

/*----------------------------------------------------------------------------
 *----------------------------------------------------------------------------*/
#if TBUF_SIZE < 2
#error TBUF_SIZE is too small.  It must be larger than 1.
#elif ((TBUF_SIZE & (TBUF_SIZE-1)) != 0)
#error TBUF_SIZE must be a power of 2.
#endif

#if RBUF_SIZE < 2
#error RBUF_SIZE is too small.  It must be larger than 1.
#elif ((RBUF_SIZE & (RBUF_SIZE-1)) != 0)
#error RBUF_SIZE must be a power of 2.
#endif

#include "UART_defines.h"

uart_ctrl_t UART_DEF_CTRL;


//buf_rx USART_DEF_RX_BUFF;// = { 0, 0, };
#define SIO_RBUFLEN ((unsigned short)(UART_DEF_CTRL.rbuf.in - UART_DEF_CTRL.rbuf.out))

//buf_tx USART_DEF_TX_BUFF;// = { 0, 0, };
#define SIO_TBUFLEN ((unsigned short)(UART_DEF_CTRL.tbuf.in - UART_DEF_CTRL.tbuf.out))


/*------------------------------------------------------------------------------
  Link UART with task  
 *------------------------------------------------------------------------------*/
int8_t 
USART_DEF_RegisterDevice(OS_TID task, uint16_t flag_rx) {

 	if (UART_DEF_CTRL.task == 0xFF)
	{
		 UART_DEF_CTRL.task = task;
	 	 UART_DEF_CTRL.flag_rx = flag_rx;
		 return 0;
	}
	else 
	{
	 	return 1;
	}

}

/*------------------------------------------------------------------------------
  buffer_Init
  initialize the buffers
 *------------------------------------------------------------------------------*/
void USART_DEF_Init(int baudrate) {
   
   
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;


  UART_DEF_CTRL.task = 0xFF;

  UART_DEF_CTRL.tbuf.in = 0;                                    // Clear com buffer indexes
  UART_DEF_CTRL.tbuf.out = 0;
  UART_DEF_CTRL.tbuf.tx_restart = 1;

  UART_DEF_CTRL.rbuf.in = 0;
  UART_DEF_CTRL.rbuf.out = 0;


  USART_InitStructure.USART_BaudRate = baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;


//---------------
 
  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(USART_DEF_TX_CLOCK, ENABLE);
  UART_DEF_CLOCK_FUNC(UART_DEF_CLOCK, ENABLE); 
 
  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = USART_DEF_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(USART_DEF_TX_PORT, &GPIO_InitStructure);
    
  /* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = USART_DEF_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(USART_DEF_TX_PORT, &GPIO_InitStructure);

  /* USART configuration */
  USART_Init(USART_DEF, &USART_InitStructure);
  
  USART_ITConfig(USART_DEF, USART_IT_TXE, ENABLE);
  USART_ITConfig(USART_DEF, USART_IT_RXNE, ENABLE);
     
  /* Enable USART */
  NVIC_InitStructure.NVIC_IRQChannel = USART_DEF_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  USART_Cmd(USART_DEF, ENABLE);

}

/*------------------------------------------------------------------------------
  SenChar
  transmit a character
 *------------------------------------------------------------------------------*/
int USART_DEF_SendChar (int c) {
  
  //buf_tx *p = &USART_DEF_TX_BUFF;
                                                    // If the buffer is full, return an error value
  if (SIO_TBUFLEN >= TBUF_SIZE)
    return (-1);
                                                  
  UART_DEF_CTRL.tbuf.buf [UART_DEF_CTRL.tbuf.in & (TBUF_SIZE - 1)] = c;           // Add data to the transmit buffer.
  UART_DEF_CTRL.tbuf.in++;

  if (UART_DEF_CTRL.tbuf.tx_restart) {                               // If transmit interrupt is disabled, enable it
    UART_DEF_CTRL.tbuf.tx_restart = 0;
	USART_ITConfig(USART_DEF, USART_IT_TXE, ENABLE);
	//USART_DEF->CR1 |= USART_FLAG_TXE;		      // enable TX interrupt
  }

  return (0);
}

/*------------------------------------------------------------------------------
  GetKey
  receive a character
 *------------------------------------------------------------------------------*/
int USART_DEF_GetChar (void) {
  
  //buf_rx *p = &USART_DEF_RX_BUFF;
	 
  if (SIO_RBUFLEN == 0)
    return (-1);

  return (UART_DEF_CTRL.rbuf.buf [(UART_DEF_CTRL.rbuf.out++) & (RBUF_SIZE - 1)]);
}




/*----------------------------------------------------------------------------
  USART_DEF_IRQHandler
  Handles USART1 global interrupt request.
*----------------------------------------------------------------------------*/
void USART_DEF_IRQHandler (void) {
  
  volatile unsigned int IIR;
  uart_ctrl_t *ptr;
  uint16_t input_data;

  ptr = &UART_DEF_CTRL;

  if (USART_GetITStatus(USART_DEF,USART_IT_RXNE)!=RESET){  

	  USART_ClearITPendingBit(USART_DEF,USART_IT_RXNE);
  	  input_data = (USART_ReceiveData(USART_DEF)& 0x1FF); 
   	  drv_UART2_SendChar(input_data);

   	   if (((ptr->rbuf.in - ptr->rbuf.out) & ~(RBUF_SIZE-1)) == 0) {
        	ptr->rbuf.buf [ptr->rbuf.in & (RBUF_SIZE-1)] = input_data;
        	ptr->rbuf.in++;
    	}
   }

   if (USART_GetITStatus(USART_DEF,USART_IT_TXE)!=RESET){
	 
	 USART_ClearITPendingBit(USART_DEF,USART_IT_TXE);
     if (ptr->tbuf.in != ptr->tbuf.out) {
        
		USART_SendData(USART_DEF,(ptr->tbuf.buf [ptr->tbuf.out & (TBUF_SIZE-1)] & 0x1FF));
		ptr->tbuf.out++;
        ptr->tbuf.tx_restart = 0;
    
	 }
     else {
        ptr->tbuf.tx_restart = 1;
		//USART_DEF->CR1 &= ~USART_FLAG_TXE;		      // disable TX interrupt if nothing to send
	   	USART_ITConfig(USART_DEF, USART_IT_TXE, DISABLE);
     }
   }
}
