#include "mbed.h"
#include "rtos.h"
#include "GpsReader.h"
#include "MsgBroker.h"

DigitalOut led(LED1);		//PA_5
InterruptIn key(PA_0);
PwmOut buzzer(PA_1);
Serial gps(PA_2,PA_3);
Serial lnk(PA_9,PA_10);

void task(void const *arg){
}

int main() {
	
	/** Setup peripherals */
	
	gps.baud(9600);
	lnk.baud(9600);
	key.mode(PullUp);
	led.write(0);	
	buzzer.period(0.01f);
	
	/** generate bip-bip start */
	
	for(int i=0;i<2;i++){
		buzzer.pulsewidth(0.005f);
		wait(0.2);
		buzzer.pulsewidth(0);
		wait(0.2);
	}
	
	/** Setup Message Broker and topics */
	MsgBroker::init();
	MsgBroker::installTopic("/gps", sizeof(GpsReader::Data_t));
	
	/** Start scheduling */
	
	osStatus osStat = osKernelInitialize();
	osStat = osKernelStart();
	
	/** Start tasks */
	
	GpsReader *g = new GpsReader(osPriorityAboveNormal, GpsReader::GPS_MODE_UBX, &gps);
	
	/** main is lowest priority task, hence manage low power conditions */
    while(1) {
        /** Low power management */
    }
	
}
