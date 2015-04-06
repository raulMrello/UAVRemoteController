/*************************************************************************//**
 * @file    drv_UART1.h
 * @mcu		STM32F1x
 * @brief   USART1 peripheral controller
 * @date    14.06.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <stdio.h>
#include "UART_defines.h"
#include "drv_UART1.h"		// includes <stdint.h>
#include "stm32f10x.h"		// includes <stm32f10x...h>


/****************************************************************************************************//**
 *                        	REQUIRED INTERFACES
 ********************************************************************************************************/
#define IRecvEvent()		drv_UART1_Requires_IRecvEvent()
#define ISentEvent()		drv_UART1_Requires_ISentEvent()

 
/****************************************************************************************************//**
 *                        PRIVATE MEMBERS
 ********************************************************************************************************/
#define USART_DEF							USART1
#define USART_DEF_IRQHandler       			USART1_IRQHandler
#define USART_DEF_Init(baud)   				drv_UART1_Init(baud)
#define USART_DEF_SendMsg					drv_UART1_SendMsg
#define USART_DEF_SendChar      			drv_UART1_SendChar
#define USART_DEF_GetChar      				drv_UART1_GetChar
#define USART_DEF_RegisterDevice(task,flag)	drv_UART1_RegisterDevice(task,flag)
#define UART_DEF_CTRL						uart1_ctrl
#define UART_DEF_CLOCK						RCC_APB2Periph_USART1
#define UART_DEF_CLOCK_FUNC					RCC_APB2PeriphClockCmd
#define USART_DEF_TX_PIN					GPIO_Pin_9
#define USART_DEF_TX_PORT					GPIOA
#define USART_DEF_TX_CLOCK					RCC_APB2Periph_GPIOA
#define USART_DEF_RX_PIN					GPIO_Pin_10
#define USART_DEF_RX_PORT					GPIOA
#define USART_DEF_IRQn						USART1_IRQn
    
// buffer controller for this driver
uart_ctrl_t UART_DEF_CTRL;

//#if UART1_ENABLE_FULLDUPLEX > 0
//static uint8_t enableRx1 = 1;
//#else
//static uint8_t enableRx1 = 0;
//#endif

//buf_rx USART_DEF_RX_BUFF;// = { 0, 0, };
#define SIO_RBUFLEN ((unsigned short)(UART_DEF_CTRL.rbuf.in - UART_DEF_CTRL.rbuf.out))

//buf_tx USART_DEF_TX_BUFF;// = { 0, 0, };
#define SIO_TBUFLEN ((unsigned short)(UART_DEF_CTRL.tbuf.in - UART_DEF_CTRL.tbuf.out))



/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/****************************************************************************************************//**
 * @fun		USART_DEF_Init
 * @brief	Initialize driver assigning a baudrate. If baudrate is 0 it only initialize Rx buffer
 * @param	baudrate	
 * @return	n/a
 **********************************************************************************/
void USART_DEF_Init(uint32_t baudrate) {
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;   
  
  UART_DEF_CTRL.rbuf.in = 0;
  UART_DEF_CTRL.rbuf.out = 0;
   
  if(baudrate == 0){
	return;
  } 

  UART_DEF_CTRL.tbuf.in = 0;                                    // Clear com buffer indexes
  UART_DEF_CTRL.tbuf.out = 0;
  UART_DEF_CTRL.tbuf.tx_restart = 1;

//  #if UART1_ENABLE_FULLDUPLEX > 0
//  enableRx1 = 1;
//  #else
//  enableRx1 = 0;
//  #endif

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
  
  USART_ITConfig(USART_DEF, USART_IT_TXE, DISABLE);
  USART_ITConfig(USART_DEF, USART_IT_RXNE, DISABLE);
     
  /* Enable USART */
  NVIC_InitStructure.NVIC_IRQChannel = USART_DEF_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  USART_Cmd(USART_DEF, ENABLE);

}

/****************************************************************************************************//**
 * @fun		drv_UART1_EnableRx
 * @brief	Disable reception interrupts to avoid ECHO detection
 * @return	n/a
 **********************************************************************************/
void drv_UART1_EnableRx(void) {     
  	USART_ReceiveData(USART_DEF);	// dummy read for flags clearing

	// enables reception ONLY if transmission finished.
	if(UART_DEF_CTRL.tbuf.tx_restart == 1){
		USART_ITConfig(USART_DEF, USART_IT_RXNE, ENABLE);
//  		USART_DEF->CR1 &= ~0x0004;	// Clear RE "Receiver Enable" bit in CR Register
// 		USART_DEF->CR1 |= 0x0004;		// Set RE, init start condition capture
//		enableRx1 = 1;										// enables Rx interrupts
	}
}

/****************************************************************************************************//**
 * @fun		drv_UART1_DisableRx
 * @brief	Disable reception interrupts to avoid ECHO detection
 * @return	n/a
 **********************************************************************************/
void drv_UART1_DisableRx(void) {
	USART_ITConfig(USART_DEF, USART_IT_RXNE, DISABLE);
//	USART_ClearITPendingBit(USART_DEF,USART_IT_RXNE);	// reset Rx pending flag
  	USART_ReceiveData(USART_DEF); 						// dummy read for flags clearing
//	enableRx1 = 0;										// disables Rx interrupts
}

/****************************************************************************************************//**
 * @fun		USART_DEF_SendChar
 * @brief	Sends a character
 * @param	c 	character to send	
 * @retval	-1	error
 * @retval  0	succesfull
 **********************************************************************************/
int USART_DEF_SendChar (char c) {  
  if(UART_DEF_CTRL.tbuf.out == 0 && UART_DEF_CTRL.tbuf.in == TBUF_SIZE-1){
    return (-1);
  }
  if(UART_DEF_CTRL.tbuf.out > 0 && UART_DEF_CTRL.tbuf.in == (UART_DEF_CTRL.tbuf.out-1)){
    return (-1);
  }
                                                  
  UART_DEF_CTRL.tbuf.buf [UART_DEF_CTRL.tbuf.in] = c;           // Add data to the transmit buffer.
  UART_DEF_CTRL.tbuf.in++;
  // keep index into the range
  if(UART_DEF_CTRL.tbuf.in >= TBUF_SIZE){
  	UART_DEF_CTRL.tbuf.in = 0;
  }

  if (UART_DEF_CTRL.tbuf.tx_restart) {                               // If transmit interrupt is disabled, enable it
    UART_DEF_CTRL.tbuf.tx_restart = 0;
	
//	#if UART1_ENABLE_FULLDUPLEX > 0
//	enableRx1 = 1;
//	#else
//	enableRx1 = 0;
//	#endif

	//USART_ITConfig(USART_DEF, (USART_IT_LBD | USART_IT_ERR | USART_IT_ORE | USART_IT_NE | USART_IT_FE | USART_IT_PE), ENABLE);
//	USART_ITConfig(USART_DEF, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART_DEF, USART_IT_TXE, ENABLE);
  }

  return (0);
}



/****************************************************************************************************//**
 * @fun		USART_DEF_SendMsg
 * @brief	Sends a buffer
 * @param	ptr		Buffer address
 * @param	size	Size in bytes to be sent
 * @return	Bytes sent
 **********************************************************************************/
 uint8_t USART_DEF_SendMsg(uint8_t *ptr,uint8_t size){

	  uint8_t count = 0;
	  
	  for (count=0;count<size;count++)
	  {
	  		if (USART_DEF_SendChar(*(ptr+count))!=0) {
			   	return count;
			}
	  }

	  return size;
}




/****************************************************************************************************//**
 * @fun		USART_DEF_GetChar
 * @brief	Read the next byte in the buffer
 * @return	Byte read
 **********************************************************************************/
int USART_DEF_GetChar (void) {
  	int data;
	
	if (SIO_RBUFLEN == 0){
    	return (-1);
	}

	data = UART_DEF_CTRL.rbuf.buf [UART_DEF_CTRL.rbuf.out];
	UART_DEF_CTRL.rbuf.out++;
	if(UART_DEF_CTRL.rbuf.out >= RBUF_SIZE){
		UART_DEF_CTRL.rbuf.out = 0;
	}
  	return (data);
}



/****************************************************************************************************//**
 * @fun		USART_DEF_IRQHandler
 * @brief	Interrupt handler. On byte reception, interface <IRecvEvent> is invoked
 * @return	
 **********************************************************************************/
volatile uint16_t idle_data_irq;
void USART_DEF_IRQHandler (void) {
	 
  	volatile unsigned int IIR;
  	uart_ctrl_t *ptr;
  	uint16_t input_data;

  	ptr = &UART_DEF_CTRL;

  	if (USART_GetITStatus(USART_DEF,USART_IT_RXNE)!=RESET){ 
  		// process data if Rx enabled
 		if(USART_GetFlagStatus(USART_DEF,USART_FLAG_FE)!=RESET){
  	  		volatile uint32_t i;
			USART_DEF->CR1 &= ~0x0004;	// Clear RE "Receiver Enable" bit in CR Register
			USART_DEF_Init(0);
			USART_DEF->CR1 |= 0x0004;		// Set RE, init start condition capture	
  		}
  		// read and hence clear pending flags  USART_ClearITPendingBit(USART_DEF,USART_IT_RXNE);
  		input_data = (USART_ReceiveData(USART_DEF)& 0x1FF); 
		if ((ptr->rbuf.in - ptr->rbuf.out) == 0) {
   			ptr->rbuf.buf [ptr->rbuf.in] = input_data;
   			ptr->rbuf.in++;
			if(ptr->rbuf.in >= RBUF_SIZE){
				ptr->rbuf.in = 0;
			}
		}
		IRecvEvent(); 			
	}
	
   if (USART_GetITStatus(USART_DEF,USART_IT_TXE)!=RESET){
	 USART_ClearITPendingBit(USART_DEF,USART_IT_TXE);
     if (ptr->tbuf.in != ptr->tbuf.out) {
        
		USART_SendData(USART_DEF,ptr->tbuf.buf[ptr->tbuf.out]);
		ptr->tbuf.out++;
		if(ptr->tbuf.out >= TBUF_SIZE){
			ptr->tbuf.out = 0;
		}
        ptr->tbuf.tx_restart = 0;
    
	 }
     else {
        ptr->tbuf.tx_restart = 1;
	   	USART_ITConfig(USART_DEF, USART_IT_TXE, DISABLE);
		ISentEvent();										//notify end of stream transmission
     }
   }
   	// Prueba
	idle_data_irq = USART_ReceiveData(USART_DEF); 

}

