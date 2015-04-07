/*
 * drv_UART1.c
 *
 *  Created on: 07/04/2015
 *      Author: raulMrello
 *
 */
 
#include "drv_UART1.h"

//------------------------------------------------------------------------------------
//-- PRIVATE DEFINITIONS -------------------------------------------------------------
//------------------------------------------------------------------------------------

#define USART1_TX_PIN		GPIO_Pin_9
#define USART1_TX_PORT		GPIOA
#define USART1_TX_CLOCK		RCC_APB2Periph_GPIOA

#define USART1_RX_PIN		GPIO_Pin_10
#define USART1_RX_PORT		GPIOA

#define USART1_IRQn			USART1_IRQn

#define SIO_RBUFLEN ((unsigned short)(gpsUart.rbuf.in - gpsUart.rbuf.out))
#define SIO_TBUFLEN ((unsigned short)(gpsUart.tbuf.in - gpsUart.tbuf.out))

//------------------------------------------------------------------------------------
//-- PRIVATE MEMBERS -----------------------------------------------------------------
//------------------------------------------------------------------------------------

static Exception e = Exception_INIT();
static GPS_TOPIC_DATA_T gpsTopicData;
static UART_TOPIC_DATA_T uartTopicData;
static Topic * gpsTopic;
static Topic * uartTopic;
static uart_ctrl_t gpsUart;

/** \fun enableRx
 *  \brief Enables data reception
 */
static void enableRx(void);

/** \fun disableRx
 *  \brief Disables data reception
 */
static void disableRx(void);

/** \fun sendChar
 *  \brief Sends a character
 *  \param c Character to send
 *  \param e Exception handler
 */
static void sendChar(char c, Exception *e);

/** \fun sendMsg
 *  \brief Sends a message. If buffer overflows, then sends what it can.
 *  \param ptr Buffer to send
 *  \param size Number of bytes to send
 *  \return Number of bytes sent
 */
static int sendMsg(uint8_t *ptr, int size);

/** \fun getChar
 *  \brief Gets a character
 *  \param c Pointer to the receptor of the character
 *  \param e Exception handler
 */
static void getChar (char * c, Exception *e);


//------------------------------------------------------------------------------------
//-- IMPLEMENTATION  -----------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
static void enableRx(void) {     
  	USART_ReceiveData(USART1);	// dummy read for flags clearing
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}

//------------------------------------------------------------------------------------
static void disableRx(void) {
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
  	USART_ReceiveData(USART1); 						// dummy read for flags clearing
	USART_Cmd(USART1, ENABLE);
}

//------------------------------------------------------------------------------------
void sendChar(char c, Exception *e) {  
	if(gpsUart.tbuf.out == 0 && gpsUart.tbuf.in == TBUF_SIZE-1){
		Exception_throw(e, MEMORY_ALLOC, "drv_UART1::sendChar buffer overflow");
		return;
	}
	if(gpsUart.tbuf.out > 0 && gpsUart.tbuf.in == (gpsUart.tbuf.out-1)){
		Exception_throw(e, MEMORY_ALLOC, "drv_UART1::sendChar buffer overflow");
		return;
	}                                            
	// Add data to the transmit buffer.	
	gpsUart.tbuf.buf [gpsUart.tbuf.in] = c;      
	gpsUart.tbuf.in++;
	// keep index into the range
	if(gpsUart.tbuf.in >= TBUF_SIZE){
		gpsUart.tbuf.in = 0;
	}
	// If transmit interrupt is disabled, enable it and starts transmission
	if (gpsUart.tbuf.tx_restart) {                               
		gpsUart.tbuf.tx_restart = 0;	
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	}  
}

//------------------------------------------------------------------------------------
static int sendMsg(uint8_t *ptr, int size){
	uint8_t count = 0; 
	Exception e = Exception_INIT();
	for(count = 0; count < size; count++){
		sendChar(*(ptr+count), &e);
		catch(&e){
			return count;
		}
	}
	return size;
}

//------------------------------------------------------------------------------------
static void getChar (char * c, Exception *e) {
 	int data;
	if (SIO_RBUFLEN == 0){
    	Exception_throw(e, MEMORY_ALLOC, "drv_UART1::getChar buffer is empty");
		return;
	}
	data = gpsUart.rbuf.buf [gpsUart.rbuf.out];
	gpsUart.rbuf.out++;
	if(gpsUart.rbuf.out >= RBUF_SIZE){
		gpsUart.rbuf.out = 0;
	}
  	*c = (char)data;
}

//------------------------------------------------------------------------------------
void drv_UART1_Init(Exception *e){
	
	/** buffer and topic initialization */
	  
	gpsUart.rbuf.in = 0;
	gpsUart.rbuf.out = 0;
   	gpsUart.tbuf.in = 0;                                    
	gpsUart.tbuf.out = 0;
	gpsUart.tbuf.tx_restart = 1;
	
	/** Subscribe to UartTopics update and attach callback function */
	uartTopic = UartTopic_getRef("/uart", e);
	catch(e){
		return;
	}
	Topic_attach(uartTopic, 0, e);
	catch(e){
		return;
	}
	// sets default value for topic handler
	gpsTopicData = (GPS_TOPIC_DATA_T){0, 0, 0, 0};

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
	RCC_APB2PeriphClockCmd(USART1_TX_CLOCK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 

	/* Configure USART Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART1_TX_PORT, &GPIO_InitStructure);
    
	/* Configure USART Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = USART1_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART1_TX_PORT, &GPIO_InitStructure);

	/* USART configuration */
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
     
	/* Enable USART */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART1, ENABLE);
}

//------------------------------------------------------------------------------------
void drv_UART1_OnTopicUpdate(void * obj, TopicData * td){
	(void)obj;	// param not used
	//topic checking
	if(td->id == (int)uartTopic){
		uartTopicData = *((UART_TOPIC_DATA_T*)td->data);
		#warning TODO extract data and proceed for example with send(...)
		// TODO
	}	
}

//------------------------------------------------------------------------------------
//-- INTERRUPTS  ---------------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
void USART1_IRQHandler (void) {	 
  	volatile unsigned int IIR;
  	uart_ctrl_t *ptr;
  	uint16_t input_data;
  	ptr = &gpsUart;

  	if (USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET){ 
  		// process data if Rx enabled
 		if(USART_GetFlagStatus(USART1,USART_FLAG_FE)!=RESET){
  	  		volatile uint32_t i;
			USART1->CR1 &= ~0x0004;	// Clear RE "Receiver Enable" bit in CR Register
			gpsUart.rbuf.in = 0;	// clear buffers
			gpsUart.rbuf.out = 0;
			USART1->CR1 |= 0x0004;	// Set RE, init start condition capture	
  		}
  		// read and hence clear pending flags  USART_ClearITPendingBit(USART1,USART_IT_RXNE);
  		input_data = (USART_ReceiveData(USART1)& 0x1FF); 
		if ((ptr->rbuf.in - ptr->rbuf.out) == 0) {
   			ptr->rbuf.buf [ptr->rbuf.in] = input_data;
   			ptr->rbuf.in++;
			if(ptr->rbuf.in >= RBUF_SIZE){
				ptr->rbuf.in = 0;
			}
		}
		/** [optional] add code here to notify new data received */ 
		#warning TODO add code to notify new recv data		
	}
	
	if (USART_GetITStatus(USART1,USART_IT_TXE)!=RESET){
		USART_ClearITPendingBit(USART1,USART_IT_TXE);
		if (ptr->tbuf.in != ptr->tbuf.out) {
        
			USART_SendData(USART1,ptr->tbuf.buf[ptr->tbuf.out]);
			ptr->tbuf.out++;
			if(ptr->tbuf.out >= TBUF_SIZE){
				ptr->tbuf.out = 0;
			}
			ptr->tbuf.tx_restart = 0;    
		}
		else {
			ptr->tbuf.tx_restart = 1;
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
			/** [optional] add code here to notify end of transmission */
		}
	}
	
	if(USART_GetITStatus(USART1,USART_IT_IDLE)!=RESET){
		USART_ClearITPendingBit(USART1,USART_IT_IDLE);
		/** [optional] add code here to notify end of stream reception */  
		#warning TODO add code to notify new recv stream
   }
   	// dummy read for flags clearing
	input_data = USART_ReceiveData(USART1); 
}
