/*
 * UartTopics.h
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 *
 *  Topics are data structures used in publisher/subscriber architectures. These structures use to have
 *	various common members: 
 *		- status: is a bitmask variable showing the current state of the topic
 *		- queries: is a bitmask variable indicating which params of the topic have been updated
 *		- params: several variables according with the topic type
 *	Also, topics has a unique name string "/name". Using that string, an agent can get topic
 *	references, publish updates, attach to those updates ...
 *
 *	UartTopics module includes all kind of topics related to uart ports (config, send).
 *	Next lines describe those which are already implemented:
 *
 *	Topic: 	"/uart1"
 *	Descr:	Describes uart common actions (configure, send) to carry out on peripheral id: 1.
 *
 *	Topic: 	"/uart2"
 *	Descr:	Describes uart common actions (configure, send) to carry out on peripheral id: 2.
 *	
 */

#ifndef SRC_TOPICS_UARTTOPICS_H_
#define SRC_TOPICS_UARTTOPICS_H_

#include "../../mmf_c/os/mmf.h"



/** \enum UartRequest
 *  \brief Uart requested operation flags definitions
 */
typedef enum {
	UART_NO_QUERIES			= 0,
	UART_SET_NOFLOWCTL 		= (1 << 0),
	UART_SET_RTS_CTS 		= (1 << 1),
	UART_SET_8BITS 			= (1 << 2),
	UART_SET_9BITS 			= (1 << 3),
	UART_SET_10BITS 		= (1 << 4),
	UART_SET_NO_PARITY 		= (1 << 5),
	UART_SET_STOP 			= (1 << 6),
	UART_SET_2STOP 			= (1 << 7),
	UART_SET_SPEED 			= (1 << 8),
	UART_DISABLE_RX 		= (1 << 9),
	UART_ENABLE_RX 			= (1 << 10),
	UART_SEND				= (1 << 11),
	UART_ACK 				= (1 << 12),
	UART_NACK	 			= (1 << 13)
}UartRequest;

/** \enum UartStatus
 *  \brief Uart status flag definitions
 */
typedef enum {
	UART_NOFLOWCTL 			= (1 << 0),
	UART_RTS_CTS 			= (1 << 1),
	UART_8BITS 				= (1 << 2),
	UART_9BITS 				= (1 << 3),
	UART_10BITS 			= (1 << 4),
	UART_NO_PARITY 			= (1 << 5),
	UART_1STOP 				= (1 << 6),
	UART_2STOP 				= (1 << 7),
	UART_RX_ENABLED 		= (1 << 8),
	UART_ERR_BUFFER_FULL	= (1 << 9)
	
}UartStatus;

/** \struct UART_TOPIC_DATA_T
 *  \brief Topic data structure for /uart topics
 */
typedef struct {
	int baudrate;				///< speed
	char * data;				///< data buffer
	int datasize;				///< data buffer size
	UartRequest queries;		///< requested configuration flags
	UartStatus status;			///< status flags
}UART_TOPIC_DATA_T;


Topic * UartTopic_initialize(const char * name, Exception *e);
Topic * UartTopic_getRef(const char * name, Exception *e);

//int MyTopic_getId(void);
//const char * MyTopic_getName(void);

#endif /* SRC_TOPICS_UARTTOPICS_H_ */
