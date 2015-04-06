
#include "bsp_rc.h"



void BSP_RC_init(void){
	// GPIO initialization
	drv_GPIO_Init();
	
	
}

int WifiLink_write(void * data, int size){
	return -1;
}
int WifiLink_read(void * data, int size){
	return -1;
}
int GpsModule_read(void * data, int size){
	return -1;
}
int ModeSelector_read(void){
	return -1;
}
int Keyboard_read(void){
	return -1;
}
void Output_write(int id, int value){
	return -1;
}
void PwmOutput_start(int id, int duty, int freq){
	return -1;
}
void PwmOutput_stop(int id){
	return -1;
}
