#ifndef __UART3_H__
#define __UART3_H__

#include <RTL.h>
#include "stm32f10x.h"

//definicion funciones
void drv_UART3_Init(int baudrate);
int drv_UART3_SendChar(int c);
int drv_UART3_GetChar(void);
int8_t drv_UART3_RegisterDevice(OS_TID task, uint16_t flag_rx);






#endif
