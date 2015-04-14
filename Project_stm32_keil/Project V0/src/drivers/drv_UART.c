/*
 * drv_UART1.c
 *
 *  Created on: 07/04/2015
 *      Author: raulMrello
 *
 */
 
#include "drv_UART.h"

//------------------------------------------------------------------------------------
//-- PRIVATE TYPEDEFS ----------------------------------------------------------------
//------------------------------------------------------------------------------------


/** Transmission buffer data structure */
typedef struct  {
	uint32_t in;       		//!< Next In index
	uint32_t out;   		//!< Next Out index
	uint32_t tx_restart;  	//!< ??
	char *buf;     			//!< Tx buffer definition
	uint16_t bufsize;
}BufTx_t;

/** Reception buffer data structure */
typedef struct  {
	uint32_t in;      	//!< Next In index
	uint32_t out;   	//!< Next Out index
	char *buf;     		//!< Tx buffer definition
	uint16_t bufsize;
}BufRx_t;

/** Uart control data structure */ 
typedef struct {
	BufTx_t 		tbuf;	 		//!< TX buffer 
	BufRx_t 		rbuf;			//!< RX buffer
	OnUartDataReady cbDataReady;	//!< on data ready callback
	OnUartDataSent	cbDataSent;		//!< on data sent callback
	UartHandlerObj	cbHandler;		//!< callbacks handler
}UartCtrl_t;

//------------------------------------------------------------------------------------
//-- PRIVATE DEFINITIONS -------------------------------------------------------------
//------------------------------------------------------------------------------------

#define USART1_TX_PIN		GPIO_Pin_9
#define USART1_TX_PORT		GPIOA
#define USART1_TX_CLOCK		RCC_APB2Periph_GPIOA

#define USART1_RX_PIN		GPIO_Pin_10
#define USART1_RX_PORT		GPIOA

#define USART2_TX_PIN		GPIO_Pin_2
#define USART2_TX_PORT		GPIOA
#define USART2_TX_CLOCK		RCC_APB2Periph_GPIOA

#define USART2_RX_PIN		GPIO_Pin_3
#define USART2_RX_PORT		GPIOA

#define SIO_RBUFLEN(u) ((unsigned short)(u->rbuf.in - u.rbuf.out))
#define SIO_TBUFLEN(u) ((unsigned short)(u->tbuf.in - u.tbuf.out))
	

//------------------------------------------------------------------------------------
//-- PRIVATE MEMBERS -----------------------------------------------------------------
//------------------------------------------------------------------------------------

static UartCtrl_t gpsUart;
static UartCtrl_t lnkUart;

//------------------------------------------------------------------------------------
static void NULL_CALLBACK(UartHandlerObj handler){
	(void)handler;	// param not used
}

//------------------------------------------------------------------------------------
/** \fun sendChar
 *  \brief Sends a character
 *	\param uart Uart controller
 *	\param USARTx Uart peripheral
 *  \param c Character to send
 *  \return Result flag (sent=1, not_sent=0)
 */
static char sendChar(UartCtrl_t *uart, USART_TypeDef * USARTx, char c){  
	if(uart->tbuf.out == 0 && uart->tbuf.in == uart->tbuf.bufsize-1){
		return 0;
	}
	if(uart->tbuf.out > 0 && uart->tbuf.in == (uart->tbuf.out-1)){
		return 0;
	}                                            
	// Add data to the transmit buffer.	
	uart->tbuf.buf [uart->tbuf.in] = c;      
	uart->tbuf.in++;
	// keep index into the range
	if(uart->tbuf.in >= uart->tbuf.bufsize){
		uart->tbuf.in = 0;
	}
	// If transmit interrupt is disabled, enable it and starts transmission
	if (uart->tbuf.tx_restart) {                               
		uart->tbuf.tx_restart = 0;	
		USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
	}  
	return 1;
}

//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

drv_UART drv_UART_Init(	uint8_t channel, uint16_t tbufsize, uint16_t rbufsize,
						OnUartDataReady cbDataReady, OnUartDataSent cbDataSent, UartHandlerObj handler){
						
	UartCtrl_t *uart;
	USART_TypeDef * USARTx;
	uint32_t USART_TX_CLOCK;
	uint32_t RCC_APBxPeriph_USART;
	uint16_t USART_TX_PIN;
	GPIO_TypeDef * USART_TX_PORT;
	uint16_t USART_RX_PIN;
	GPIO_TypeDef * USART_RX_PORT;
	uint8_t USARTx_IRQn;
						
	/** param checking */
	if(channel != 1 && channel != 2)
		return 0;
	
	/** buffer and topic initialization */
	if(channel == 1){
		uart = &gpsUart;			
		USARTx = USART1;
		USARTx_IRQn = USART1_IRQn;
		USART_TX_CLOCK = USART1_TX_CLOCK;
		RCC_APBxPeriph_USART = RCC_APB2Periph_USART1;
		USART_TX_PIN = USART1_TX_PIN;
		USART_TX_PORT = USART1_TX_PORT;
		USART_RX_PIN = USART1_RX_PIN;
		USART_RX_PORT = USART1_RX_PORT;
	}
	else{
		uart = &lnkUart;	
		USARTx = USART2;
		USARTx_IRQn = USART2_IRQn;
		USART_TX_CLOCK = USART2_TX_CLOCK;
		RCC_APBxPeriph_USART = RCC_APB1Periph_USART2;
		USART_TX_PIN = USART2_TX_PIN;
		USART_TX_PORT = USART2_TX_PORT;
		USART_RX_PIN = USART2_RX_PIN;
		USART_RX_PORT = USART2_RX_PORT;
	}
	  
	uart->cbDataReady = (cbDataReady)? cbDataReady : NULL_CALLBACK;
	uart->cbDataSent = (cbDataSent)? cbDataSent : NULL_CALLBACK;
	uart->cbHandler = handler;
	uart->rbuf.buf = 0;
	uart->rbuf.bufsize = rbufsize;
	uart->rbuf.in = 0;
	uart->rbuf.out = 0;
	
	uart->tbuf.buf = 0;
	uart->tbuf.bufsize = tbufsize;
   	uart->tbuf.in = 0;                                    
	uart->tbuf.out = 0;
	uart->tbuf.tx_restart = 1;
	if(tbufsize){
		uart->tbuf.buf = (char*)malloc(tbufsize);
		if(uart->tbuf.buf == 0)
			return 0;
	}
	if(rbufsize){
		uart->rbuf.buf = (char*)malloc(rbufsize);
		if(uart->rbuf.buf == 0){
			free(uart->tbuf.buf);
			return 0;
		}
	}
	
	/** peripheral initialization */
	
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;   

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd(USART_TX_CLOCK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APBxPeriph_USART, ENABLE); 

	/* Configure USART Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = USART_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_TX_PORT, &GPIO_InitStructure);
    
	/* Configure USART Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = USART_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART_RX_PORT, &GPIO_InitStructure);

	/* USART configuration */
	USART_Init(USARTx, &USART_InitStructure);
	USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
	USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE);
     
	/* Enable USART */
	NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USARTx, ENABLE);
	
	return (drv_UART)uart;
}

//------------------------------------------------------------------------------------
void drv_UART_endisRx(drv_UART drv, uint8_t endis){
	USART_TypeDef * USARTx;
	UartCtrl_t *uart = (UartCtrl_t*)drv;
	
	
	/** buffer and topic initialization */
	if(uart == &gpsUart){
		USARTx = USART1;
	}
	else{
		USARTx = USART2;
	}
	if(endis){
		USART_ReceiveData(USARTx);	// dummy read for flags clearing
		USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
		USART_Cmd(USARTx, ENABLE);
	}
	else{
		USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE);
		USART_ReceiveData(USARTx); 						// dummy read for flags clearing
		USART_Cmd(USARTx, ENABLE);
	}		
}

//------------------------------------------------------------------------------------
int drv_UART_write(drv_UART drv, char *buffer, uint16_t size, OnUartDataSent cbDataSent, UartHandlerObj handler){
	USART_TypeDef * USARTx;
	UartCtrl_t *uart = (UartCtrl_t*)drv;
	
	
	/** buffer and topic initialization */
	if(uart == &gpsUart){
		USARTx = USART1;
	}
	else{
		USARTx = USART2;
	}
	  
	uart->cbDataSent = (cbDataSent)? cbDataSent : NULL_CALLBACK;
	uart->cbHandler = handler;
	int count;
	for(count = 0; count < size; count++){
		if(!sendChar(uart, USARTx, buffer[count])){
			return count;
		}		
	}
	return count;
}

//------------------------------------------------------------------------------------
int drv_UART_read(drv_UART drv, char *buffer, uint16_t size){
	UartCtrl_t *uart = (UartCtrl_t*)drv;
	

	int count = 0;
	while (count < size){
		if ((uart->rbuf.in - uart->rbuf.out) == 0){
			break;
		}
		buffer[count++] = uart->rbuf.buf [uart->rbuf.out];
		uart->rbuf.out++;
		if(uart->rbuf.out >= uart->rbuf.bufsize){
			uart->rbuf.out = 0;
		}
	}
	return count;
}

//------------------------------------------------------------------------------------
//-- INTERRUPTS  ---------------------------------------------------------------------
//------------------------------------------------------------------------------------

static void USARTx_IRQHandler(UartCtrl_t *uart, USART_TypeDef * USARTx){
  	volatile unsigned int IIR;
  	uint16_t input_data;

  	if (USART_GetITStatus(USARTx,USART_IT_RXNE)!=RESET){ 
  		// process data if Rx enabled
 		if(USART_GetFlagStatus(USARTx,USART_FLAG_FE)!=RESET){
  	  		volatile uint32_t i;
			USART1->CR1 &= ~0x0004;	// Clear RE "Receiver Enable" bit in CR Register
			uart->rbuf.in = 0;	// clear buffers
			uart->rbuf.out = 0;
			USARTx->CR1 |= 0x0004;	// Set RE, init start condition capture	
  		}
  		// read and hence clear pending flags  USART_ClearITPendingBit(USART1,USART_IT_RXNE);
  		input_data = (USART_ReceiveData(USARTx)& 0x1FF); 
		if ((uart->rbuf.in - uart->rbuf.out) == 0) {
   			uart->rbuf.buf [uart->rbuf.in] = input_data;
   			uart->rbuf.in++;
			if(uart->rbuf.in >= uart->rbuf.bufsize){
				uart->rbuf.in = 0;
			}
		}
		/** [optional] add code here to notify new data received */ 
		// uart->cbDataReady(uart->cbHandler);	
	}
	
	if (USART_GetITStatus(USARTx,USART_IT_TXE)!=RESET){
		USART_ClearITPendingBit(USARTx,USART_IT_TXE);
		if (uart->tbuf.in != uart->tbuf.out) {
        
			USART_SendData(USARTx, uart->tbuf.buf[uart->tbuf.out]);
			uart->tbuf.out++;
			if(uart->tbuf.out >= uart->tbuf.bufsize){
				uart->tbuf.out = 0;
			}
			uart->tbuf.tx_restart = 0;    
		}
		else {
			uart->tbuf.tx_restart = 1;
			USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
			/** [optional] add code here to notify end of transmission */
			uart->cbDataSent(uart->cbHandler);
		}
	}
	
	if(USART_GetITStatus(USARTx,USART_IT_IDLE)!=RESET){
		USART_ClearITPendingBit(USARTx,USART_IT_IDLE);
		/** [optional] add code here to notify end of stream reception */  
		uart->cbDataReady(uart->cbHandler);
   }
   	// dummy read for flags clearing
	input_data = USART_ReceiveData(USARTx); 
}

//------------------------------------------------------------------------------------
void USART1_IRQHandler (void) {	
	USARTx_IRQHandler(&gpsUart, USART1);	
}

//------------------------------------------------------------------------------------
void USART2_IRQHandler (void) {	 
	USARTx_IRQHandler(&lnkUart, USART2);	
}
