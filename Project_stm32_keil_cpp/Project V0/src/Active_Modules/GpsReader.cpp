/*
 * GpsReader.cpp
 *
 *  Created on: 14/04/2015
 *      Author: raulMrello
 */

#include "GpsReader.h"

//------------------------------------------------------------------------------------
//-- PRIVATE TYPEDEFS ----------------------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
//-- PRIVATE DEFINITIONS -------------------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
//-- STATIC MEMBERS ------------------------------------------------------------------
//------------------------------------------------------------------------------------

static void UartDataReadyISRCallback(UartHandlerObj handler){
	GpsReader * me = (GpsReader*)handler;
	MMF::OS::sendEvent((Task*)me, "", (uint16_t)GpsReader::GPS_EV_DATAREADY);
}

//------------------------------------------------------------------------------------
static void UartDataSentISRCallback(UartHandlerObj handler){
	GpsReader * me = (GpsReader*)handler;
	MMF::OS::sendEvent((Task*)me, "", (uint16_t)GpsReader::GPS_EV_DATASENT);
}

//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
GpsReader::~GpsReader() {
	// TODO Auto-generated destructor stub
}

//------------------------------------------------------------------------------------
void GpsReader::init(){
	// setup uart1 peripheral
	_uart = drv_UART_Init(1, 64, 64, UartDataReadyISRCallback, UartDataSentISRCallback, this);
}

//------------------------------------------------------------------------------------
void GpsReader::onYieldTurn(){
	// enable uart rx interrupts
	drv_UART_endisRx(_uart, 1);
	// wait for uart rx event during 5 seconds
	wait_and(GPS_EV_DATAREADY, 5000000);
}


//------------------------------------------------------------------------------------
void GpsReader::onResume(){
	// timeout occur due to some cause. Reinit uart peripheral 
	// setup uart1 peripheral
	_uart = drv_UART_Init(1, 64, 64, UartDataSentISRCallback, UartDataSentISRCallback, this);
	// yields to restart interrupts and wait events
	yield();
}

//------------------------------------------------------------------------------------
void GpsReader::onEventFlag(uint16_t event){
	// on data received from uart...
	if((event & GPS_EV_DATAREADY) != 0){		
		// stops suspension timer and sets waiting mode again
		resume(1);	
		wait_and(GPS_EV_DATAREADY, 5000000);		
		// while received data, pass to gps NMEA/UBX processor
		char data;
		bool ready = false;
		while(drv_UART_read(_uart, &data, 1) > 0){			
			#warning TODO gps data processor 
			/* >>> add code here <<<
			 * ready = libgps_ProcessData(&_gpsdata, data);
			 */			
			// if data ready, publish topic update
			if(ready){
				ready = false;
				GpsTopic::publish(&_gpsdata, sizeof(GpsTopic::Data_t),  0, 0);
			}
		}
	}
}

//------------------------------------------------------------------------------------
void GpsReader::onTopicUpdate(TopicData * td){
	/** Never should reach this point */
	throw Exception(Exception::UNKNOWN_EXCEPTION, "GpsReader::onTopicUpdate never should occur");
}

//------------------------------------------------------------------------------------
//-- PROTECTED/PRIVATE FUNCTIONS -----------------------------------------------------
//------------------------------------------------------------------------------------

