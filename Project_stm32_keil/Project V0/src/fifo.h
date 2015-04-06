//-------------------------------------------------------------------------------
//	 FIFOs
//
//-------------------------------------------------------------------------------

#ifndef __FIFO_H__
#define __FIFO_H__

#include <RTL.h>

typedef struct  {
  unsigned int in;                  // Next In Index
  unsigned int out;					// Next Out Index
  unsigned int size;
  unsigned int data_length;
  char *buffer;			            // Buffer

  OS_TID task;
  uint16_t task_flag;
  
}fifo_t;

void fifo_init(fifo_t * fifo,unsigned int size,char * buffer);
void fifo_configEvtflags(fifo_t * fifo,OS_TID task,uint16_t tsk_flag);
int fifo_insertByte(fifo_t * fifo,char byte);
int fifo_getByte(fifo_t * fifo);
int fifo_getdataLen(fifo_t * fifo);



#endif
