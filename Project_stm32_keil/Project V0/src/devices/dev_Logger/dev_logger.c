
//------------------------------------------------------------------------------
//
//			Device Logger
//		    
//
//
//
//------------------------------------------------------------------------------

#include <rtl.h>
#include <stdio.h>
#include <string.h>

#include "dev_logger.h"

#include "app_defines.h"
#include "drv_UART1.h"

#define	MAX_NUMBER_MSG_LOGGER			20

#define drv_LOGGER_SendMsg(ptr,size)	drv_UART1_SendMsg((uint8_t*)ptr,size)


OS_MBX mbx_logger;
_declare_box(mbx_logger_mpool,sizeof(logger_msg_t),MAX_NUMBER_MSG_LOGGER);
os_mbx_declare (mbx_logger,MAX_NUMBER_MSG_LOGGER);




uint8_t logger_msg[]="dev_logger_init";


int dev_logger_sendMsg(uint8_t *task_id,uint8_t msg_type,uint8_t *msg){

	logger_msg_t * ptr_msg;

   
	ptr_msg = _calloc_box(mbx_logger_mpool);

	if (ptr_msg!= NULL) {

		memset(ptr_msg,sizeof(ptr_msg),0);
		strncpy ((char *)ptr_msg->task_id,(char *)task_id,MAX_SIZE_LOGGER_TASKID);
		ptr_msg->msg_type = msg_type;
		strncpy ((char *)ptr_msg->msg,(char *)msg,MAX_SIZE_LOGGER_MSG);
		
	 	if (os_mbx_send(mbx_logger,(void*)ptr_msg,DELAY_100MS)==OS_R_OK)
			return 0;
	}

	return -1;

}

char logger_on_msg[]="\r\nLogger On\r\n";

__task
void logger_task(void) {
    
	logger_msg_t * ptr_msg;
	uint8_t	msg[sizeof(logger_msg_t)+10];
	
	drv_LOGGER_SendMsg(logger_on_msg,strlen(logger_on_msg));
			    
    for(;;) {

	   	os_mbx_wait(mbx_logger,(void **)&ptr_msg,0xFFFF);
		memset(msg,sizeof(msg),0);
		sprintf((char *)msg,"%s:[%c] %s\r\n",ptr_msg->task_id,ptr_msg->msg_type+48,ptr_msg->msg);
		_free_box(mbx_logger_mpool,ptr_msg);
		drv_LOGGER_SendMsg(msg,strlen((char *)msg));
    }
}


void dev_logger_init(void){

	_init_box(mbx_logger_mpool,sizeof(mbx_logger_mpool),sizeof(logger_msg_t)); 
  	os_mbx_init(mbx_logger,sizeof(mbx_logger));

	os_tsk_create (logger_task,100);

}
