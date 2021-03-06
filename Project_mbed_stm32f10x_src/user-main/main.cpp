#include "mbed.h"
#include "rtos.h"
#include "GpsReader.h"
#include "KeyDecoder.h"
#include "SysManager.h"
#include "VirtualReceiver.h"
#include "MsgBroker.h"
#include "Logger.h"
#include "Topics.h"

InterruptIn joys_A_Ok(PB_1);	
InterruptIn joys_B_Ok(PB_2);	
AnalogIn joys_A1(PA_4);  		
AnalogIn joys_A2(PA_5); 		
AnalogIn joys_B1(PA_6);  		
AnalogIn joys_B2(PA_7); 		
InterruptIn key_ARM(PB_11);
InterruptIn key_LOC(PB_12);
InterruptIn key_ALT(PB_13);
InterruptIn key_RTH(PB_14);
DigitalOut led_arm(PA_8);
DigitalOut led_loc(PB_15);
DigitalOut led_alt(PB_5);
DigitalOut led_rth(PB_6);
PwmOut buzzer(PB_0);
RawSerial gps(PA_2,PA_3);
RawSerial lnk(PA_9,PA_10);
DigitalOut lnk_rst(PB_4); // 0-enables esp8266, 1-reset

Mutex broker_mutex;
Logger *logger;
char logger_pool[512];

	
int main() {
	
	/** Prepare logging capabilites*/
	logger = new Logger(logger_pool, 512);
		
	/** Setup Message Broker and topics */
	
	MsgBroker::init();
	MsgBroker::installTopic("/gps", sizeof(Topic::GpsData_t));
	MsgBroker::installTopic("/keyb", sizeof(Topic::KeyData_t));
	MsgBroker::installTopic("/joys", sizeof(Topic::JoystickData_t));
	MsgBroker::installTopic("/ack", sizeof(Topic::AckData_t));
	MsgBroker::installTopic("/nav", sizeof(Topic::NavigationData_t));
	
	/** Start scheduling */
	
	osStatus osStat = osKernelInitialize();
	osStat = osKernelStart();
	
	/** Start tasks */
	
	KeyDecoder *kd = new KeyDecoder(osPriorityHigh, &joys_A_Ok, &joys_B_Ok, &key_ARM, &key_LOC, &key_ALT, &key_RTH, &joys_A1, &joys_A2, &joys_B1, &joys_B2, logger);
	SysManager *sm = new SysManager(osPriorityNormal, &led_arm, &led_loc, &led_alt, &led_rth, &buzzer, logger);
	GpsReader *gr = new GpsReader(osPriorityAboveNormal, GpsReader::GPS_MODE_UBX, &gps, logger);
	VirtualReceiver *vr = new VirtualReceiver(osPriorityAboveNormal, &lnk, &lnk_rst, logger);
		
	/** main is lowest priority task, hence manage low power conditions */
    while(1) {
        /** Low power management */
    }
	
}
