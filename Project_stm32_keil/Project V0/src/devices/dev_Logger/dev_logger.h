//------------------------------------------------------------------------------
//
//			Device Logger
//		    
//
//
//
//------------------------------------------------------------------------------

#ifndef __dev_LOGGER_H__
#define __dev_LOGGER_H__

#include "stdint.h"



#define MAX_SIZE_LOGGER_MSG 			80
#define MAX_SIZE_LOGGER_TASKID		 	10


#define LOG_NORMAL						0
#define LOG_EVENT1						1
#define LOG_EVENT2						2
#define LOG_WARNING1					3
#define LOG_WARNING2					4
#define LOG_ERROR						5


//Modos de funcionamiento
#define LOG_DISABLED					1


typedef struct {
	uint8_t task_id[MAX_SIZE_LOGGER_TASKID+1];
	uint8_t msg_type;
	uint8_t msg[MAX_SIZE_LOGGER_MSG+1];
} logger_msg_t;



//public functions
void dev_logger_init(void);
int dev_logger_sendMsg(uint8_t *task_id,uint8_t msg_type,uint8_t *msg);



#endif
