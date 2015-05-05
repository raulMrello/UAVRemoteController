#include "mbed.h"
#include "rtos.h"
#include "GpsReader.h"
#include "KeyDecoder.h"
#include "SysManager.h"
#include "VirtualReceiver.h"
#include "MsgBroker.h"
#include "Topics.h"

InterruptIn key_S(PB_1);	//<-- CAMBIAR A ENTER_A
InterruptIn key_SW(PB_2);	//<-- CAMBIAR A ENTER_B
InterruptIn key_N(PA_4);  	//<-- CAMBIAR A JOYSTICK_A1 (THROTTLE)
InterruptIn key_NE(PA_5); 	//<-- CAMBIAR A JOYSTICK_A2
InterruptIn key_E(PA_6);  	//<-- CAMBIAR A JOYSTICK_B1
InterruptIn key_ES(PA_7); 	//<-- CAMBIAR A JOYSTICK_B2 
InterruptIn key_ARM(PB_11);
InterruptIn key_LOC(PB_12);
InterruptIn key_ALT(PB_13);
InterruptIn key_RTH(PB_14);
DigitalOut led_arm(PA_8);
DigitalOut led_loc(PB_15);
DigitalOut led_alt(PB_5);
DigitalOut led_rth(PB_6);
PwmOut buzzer(PB_0);
Serial gps(PA_2,PA_3);
Serial lnk(PA_9,PA_10);

InterruptIn key_W(PB_9);	//<-- CAMBIAR A NO_USAR
InterruptIn key_WN(PB_10);	//<-- CAMBIAR A NO_USAR 



int main() {
		
	/** Setup Message Broker and topics */
	
	MsgBroker::init();
	MsgBroker::installTopic("/gps", sizeof(Topic::GpsData_t));
	MsgBroker::installTopic("/keyb", sizeof(Topic::KeyData_t));
	MsgBroker::installTopic("/alarm", sizeof(Topic::AlarmData_t));
	MsgBroker::installTopic("/stat", sizeof(Topic::StatusData_t));
	
	/** Start scheduling */
	
	osStatus osStat = osKernelInitialize();
	osStat = osKernelStart();
	
	/** Start tasks */
	
	KeyDecoder *kd = new KeyDecoder(osPriorityHigh, &key_N, &key_NE, &key_E, &key_ES,
									&key_S, &key_SW, &key_W, &key_WN, &key_ARM, &key_LOC,
									&key_ALT, &key_RTH);
	GpsReader *gr = new GpsReader(osPriorityAboveNormal, GpsReader::GPS_MODE_UBX, &gps);
	SysManager *sm = new SysManager(osPriorityNormal, &led_arm, &led_loc, &led_alt, &led_rth, &buzzer);
	
	/** main is lowest priority task, hence manage low power conditions */
    while(1) {
        /** Low power management */
    }
	
}
