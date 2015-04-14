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
//-- PRIVATE MEMBERS -----------------------------------------------------------------
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
	_uart = drv_UART_Init(1, 64, 64, UartDataSentISRCallback, UartDataSentISRCallback, this);
}

//------------------------------------------------------------------------------------
void GpsReader::onYieldTurn(){
	

	switch(counter){
		// in this case checks task suspension
		case 1:{
			try{
				suspend(20000);
			}
			catch(Exception &e){
				//printf("Exception on GpsReader_OnYieldTurn: %s\r\n", e.msg);
			}
			return;
		}
		// in this case sends a topic update
		case 2:{
			try{
				MyTopic::publish(&counter, sizeof(int), topicDone, this);
			}
			catch(Exception &e){
				e.getMessage();
			}
			break;
		}
		// in this case sends event 1
		case 3:{
			try{
				OS::sendEvent(0, "subscriber", 1);
			}
			catch(Exception &e){
				e.getMessage();
			}
			break;
		}
		// in this case sends event 2
		case 4:{
			try{
				OS::sendEvent(0, "subscriber", 2);
			}
			catch(Exception &e){
				e.getMessage();
			}
			break;
		}
		// in this case sends event 8
		case 5:{
			try{
				OS::sendEvent(0, "subscriber", 8);
			}
			catch(Exception &e){
				e.getMessage();
			}
			break;
		}
		// in this case sends event 16
		case 6:{
			try{
				OS::sendEvent(0, "subscriber", 16);
			}
			catch(Exception &e){
				e.getMessage();
			}
			break;
		}
		// in this case sends event 1 and post topic update
		case 7:{
			try{
				OS::sendEvent(0, "subscriber", 1);
				MyTopic::publish(&counter, sizeof(int), topicDone, this);
			}
			catch(Exception &e){
				e.getMessage();
			}
			break;
		}
		// in this case sends an event to an unknown task
		case 8:{
			try{
				OS::sendEvent(0, "subscriBER", 1);
			}
			catch(Exception &e){
				e.getMessage();
			}
			break;
		}
		// in this case colapses susbscriber topic pool
		case 9:{
			int j;
			for(j=0;j<10;j++){
				try{
					MyTopic::publish(&counter, sizeof(int), topicDone, this);
				}
				catch(Exception &e){
					e.getMessage();
				}
			}
			break;
		}
	}

	// keeps task running
	yield();	///< IMPORTANT!!! in order to get control later, else it won't be executed
}


//------------------------------------------------------------------------------------
void GpsReader::onResume(){
	// keeps task running
	yield();	///< IMPORTANT!!! in order to get control later, else it won't be executed
}

//------------------------------------------------------------------------------------
void GpsReader::onEventFlag(uint16_t event){

}

//------------------------------------------------------------------------------------
void GpsReader::onTopicUpdate(TopicData * td){
}

