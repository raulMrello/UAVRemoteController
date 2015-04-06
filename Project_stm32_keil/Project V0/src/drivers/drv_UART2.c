/****************************************************************************************************//**
 * @file    drv_UART2.c
 * @mcu		STM32F1x
 * @brief   USART2 peripheral controller
 * @date    11.06.2010
 * @author  Raúl M.
 ********************************************************************************************************/

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <stdio.h>
#include "UART_defines.h"
#include "drv_UART2.h"		// includes <stdint.h>
#include "stm32f10x.h"		// includes <stm32f10x...h>


/****************************************************************************************************//**
 *                        	REQUIRED INTERFACES
 ********************************************************************************************************/
#define IRecvEvent()		drv_UART2_Requires_IRecvEvent()
#define ISentEvent()		drv_UART2_Requires_ISentEvent()

 
/****************************************************************************************************//**
 *                        PRIVATE MEMBERS
 ********************************************************************************************************/
#define USART_DEF							USART2
#define USART_DEF_IRQHandler       			USART2_IRQHandler
#define USART_DEF_Init(baud)   				drv_UART2_Init(baud)
#define USART_DEF_SendChar      			drv_UART2_SendChar
#define USART_DEF_SendMsg					drv_UART2_SendMsg
#define USART_DEF_GetChar      				drv_UART2_GetChar
#define USART_DEF_RegisterDevice(task,flag)	drv_UART2_RegisterDevice(task,flag)
#define UART_DEF_CTRL						uart2_ctrl
#define UART_DEF_CLOCK						RCC_APB1Periph_USART2
#define USART_DEF_TX_PIN					GPIO_Pin_2
#define USART_DEF_TX_PORT					GPIOA
#define USART_DEF_TX_CLOCK					RCC_APB2Periph_GPIOA
#define USART_DEF_RX_PIN					GPIO_Pin_3
#define USART_DEF_RX_PORT					GPIOA
#define USART_DEF_IRQn						USART2_IRQn
    
// buffer controller for this driver
uart_ctrl_t UART_DEF_CTRL;	

#if UART2_ENABLE_FULLDUPLEX > 0
static uint8_t enableRx2 = 1;
#else
static uint8_t enableRx2 = 0;
#endif

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

  #if UART2_ENABLE_FULLDUPLEX > 0
  enableRx2 = 1;
  #else
  enableRx2 = 0;
  #endif

  USART_InitStructure.USART_BaudRate = baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//USART_HardwareFlowControl_RTS_CTS;//;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(USART_DEF_TX_CLOCK, ENABLE);
  RCC_APB1PeriphClockCmd(UART_DEF_CLOCK, ENABLE); 
 
  //RCC_APB2PeriphClockCmd
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
  

  //GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
   
  // Configure USART CTS as alternate function push-pull
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  // Configure USART RTS as alternate function push-pull 
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //GPIO_Init(GPIOD, &GPIO_InitStructure);

  #if HARDWARE_RELEASE == HW_REV2  
  // Configure USART Tx as alternate function push-pull 
  GPIO_InitStructure.GPIO_Pin = USART_DEF_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(USART_DEF_TX_PORT, &GPIO_InitStructure);  
  #endif
    
  // Configure USART Rx as input floating 
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

  USART_Cmd(USART_DEF, DISABLE);

}

/****************************************************************************************************//**
 * @fun		drv_UART2_EnableRx
 * @brief	Disable reception interrupts to avoid ECHO detection
 * @return	n/a
 **********************************************************************************/
void drv_UART2_EnableRx(void) {
	enableRx2 = 1;
	USART_ClearITPendingBit(USART_DEF,USART_IT_RXNE);	// reset Rx pending flag
  	USART_ReceiveData(USART_DEF); 						// dummy read
	USART_Cmd(USART_DEF, ENABLE);
}

/****************************************************************************************************//**
 * @fun		drv_UART2_DisableRx
 * @brief	Disable reception interrupts to avoid ECHO detection
 * @return	n/a
 **********************************************************************************/
void drv_UART2_DisableRx(void) {
	USART_ClearITPendingBit(USART_DEF,USART_IT_RXNE);	// reset Rx pending flag
  	USART_ReceiveData(USART_DEF); 						// dummy read
	USART_Cmd(USART_DEF, DISABLE);
	enableRx2 = 0;
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
	
	#if UART2_ENABLE_FULLDUPLEX > 0
	enableRx2 = 1;
	#else
	enableRx2 = 0;
	#endif
	USART_ITConfig(USART_DEF, USART_IT_RXNE, ENABLE);
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
void USART_DEF_IRQHandler (void) {
  
  	volatile unsigned int IIR;
  	uart_ctrl_t *ptr;
  	uint16_t input_data;

  	ptr = &UART_DEF_CTRL;

  	if (USART_GetITStatus(USART_DEF,USART_IT_RXNE)!=RESET){ 
  		// process data if Rx enabled
  		if(enableRx2){
  	  		if(USART_GetFlagStatus(USART_DEF,USART_FLAG_FE)!=RESET){
	  	  		volatile uint32_t i;
				USART_DEF->CR1 &= ~0x0004;	// Clear RE "Receiver Enable" bit in CR Register
				USART_DEF_Init(0);
				USART_DEF->CR1 |= 0x0004;		// Set RE, init start condition capture	
	  		}
   			if(1) {
		  		USART_ClearITPendingBit(USART_DEF,USART_IT_RXNE);
		  		input_data = (USART_ReceiveData(USART_DEF)& 0x1FF); 
				// Inserta siempre que pueda...
				if(!(ptr->rbuf.in == (ptr->rbuf.out-1)) &&  !(ptr->rbuf.in == RBUF_SIZE-1 && ptr->rbuf.out == 0)){
				//if ((ptr->rbuf.in - ptr->rbuf.out) == 0) {
	       			ptr->rbuf.buf [ptr->rbuf.in] = input_data;
	       			ptr->rbuf.in++;
					if(ptr->rbuf.in >= RBUF_SIZE){
						ptr->rbuf.in = 0;
					}
	   			}
				IRecvEvent(); 			
			}
  		}
		else{
	  		USART_ClearITPendingBit(USART_DEF,USART_IT_RXNE);
  			input_data = (USART_ReceiveData(USART_DEF)& 0x1FF); 
		}   	  
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
}

