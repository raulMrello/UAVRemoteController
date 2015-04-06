

#include "stdlib.h"
#include "string.h"
#include "stdint.h"

#include <rtl.h>

#include "fifo.h"


#define FIFO_BUFLEN ((unsigned short)(fifo->in - fifo->out))

void 
fifo_init(fifo_t * fifo,unsigned int size,char * buffer){

	if (fifo==NULL)
		return;
	
	// Clear buffer indexes and set size
	fifo->in = 0;                                    
		fifo->out = 0;
	fifo->size = size;
	//link with buffer memory
	fifo->buffer = buffer;
	fifo->data_length = 0;
	memset(fifo->buffer,fifo->size,0);
	
	fifo->task = 0;
	fifo->task_flag = 0;
	
}


void 
fifo_configEvtflags(fifo_t * fifo,OS_TID task,uint16_t tsk_flag){

	if (fifo==NULL)
		return;

		fifo->task = task;
		fifo->task_flag = tsk_flag;
}

int 
fifo_getdataLen(fifo_t * fifo){

   return FIFO_BUFLEN;

}
int 
fifo_insertByte(fifo_t * fifo,char byte){

	if (fifo==NULL)
		return 0;

   	fifo->data_length = FIFO_BUFLEN;

	if (fifo->data_length >= fifo->size)
    	return 0;
                                                  
  	fifo->buffer[fifo->in & (fifo->size - 1)] = byte;           // Add data to the transmit buffer.
  	fifo->in++;
	fifo->data_length++;

	if (fifo->task_flag!=0)
		isr_evt_set(fifo->task_flag,fifo->task);

	return 1;
}


int 
fifo_getByte(fifo_t * fifo){

   	if (fifo==NULL)
		return (-1);

	fifo->data_length = FIFO_BUFLEN;

	if (fifo->data_length == 0)
		return (-1);
	
	fifo->data_length--;

	return (uint8_t)(fifo->buffer[(fifo->out++) & (fifo->size - 1)]);

}
