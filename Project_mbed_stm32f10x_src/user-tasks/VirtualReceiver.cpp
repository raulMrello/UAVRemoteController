/*
 * VirtualReceiver.cpp
 *
 *  Created on: 20/04/2015
 *      Author: raulMrello
 */

#include "VirtualReceiver.h"

//------------------------------------------------------------------------------------
//-- PRIVATE TYPEDEFS ----------------------------------------------------------------
//------------------------------------------------------------------------------------


enum Status{
	CHECK_MODE = 0,
	SET_MODE,
	RESET_LNK,
	SET_AP_IP,
	SET_AP_NETWORK,
	SET_MUX,
	START_AP_SERVER,
	SET_AP_TIMEOUT,
	GET_AP_LIST,
	CONNECT_TO_AP,
	CHECK_AP_CONNECTION,
	START_TCP_CLIENT,
	CHECK_TCP_STATUS,
	SEND_TCP_DATA,
	CLOSE_TCP_CONNECTION,
};
struct ProtocolAction{
	uint8_t stat;
	const char * at_cmd;
	const char * response;
	uint8_t next_stat_ok;
	uint8_t next_stat_err;
};

static ProtocolAction actions[] = {
	{CHECK_MODE,
	 "AT+CWMODE?", 	
	 "+CWMODE:3", 
	 SET_AP_IP,  
	 SET_MODE},
	
	{SET_MODE, 	 
	 "AT+CWMODE=3", 
	 "OK", 		 
	 RESET_LNK,  
	 SET_MODE},
	
	{RESET_LNK,  
	 "AT+RST", 		
	 "OK", 		 
	 CHECK_MODE, 
	 CHECK_MODE},
	
	{SET_AP_IP,  
	 "AT+CIPAP=""192.168.8.10"",""192.168.8.10"",""255.255.255.0""", 		
	 "OK", 		 
	 SET_AP_NETWORK, 
	 SET_AP_IP},
	
	{SET_AP_NETWORK,  
	 "AT+CWSAP=""EAGLE-REMOTE"",""eagleQUAD"",4,3", 		
	 "OK", 		 
	 SET_MUX, 
	 SET_AP_NETWORK},
	
	{SET_MUX,  
	 "AT+CIPMUX=1", 		
	 "OK", 		 
	 START_AP_SERVER, 
	 SET_MUX},
	
	{START_AP_SERVER,  
	 "AT+CIPSERVER=1,80", 		
	 "OK", 		 
	 SET_AP_TIMEOUT, 
	 START_AP_SERVER},
	
	,
	,
	GET_AP_LIST,
	CONNECT_TO_AP,
	CHECK_AP_CONNECTION,
	START_TCP_CLIENT,
	CHECK_TCP_STATUS,
	SEND_TCP_DATA,
	CLOSE_TCP_CONNECTION,
};

static const char * ATcmd[] = {	
	"AT+CWMODE?",
	"AT+CWMODE=3",
	"AT+RST",
	"AT+CIPAP=""192.168.8.10"",""192.168.8.10"",""255.255.255.0""",
	"AT+CWSAP=""EAGLE-REMOTE"",""eagleQUAD"",4,3",
	"AT+CIPMUX=1",
	"AT+CIPSERVER=1,80",
	"AT+CIPSTO=7200",
	"AT+CWLAP",
	"AT+CWJAP=""EAGLE-TELEMETRY"",""eagleQUAD""",
	"AT+CWJAP?",
	"AT+CIPSTART=4,""TCP"",""192.168.7.10"",80"	
	"AT+CIPSTATUS",
	"AT+CIPSEND=4,"
	"AT+CIPCLOSE=4"
};

static const char * response[] = {	
	"+CWMODE:3",
	"ERROR",
	"+CWJAP:",
	"4,CONNECT",
	"0,CONNECT",
	"+CWLAP:",
	"+CIPSTATUS:",
	"busy",
	"SEND OK",
	"+IPD,"	
	"4,CLOSED",
	"0,CLOSED"
	"AT+CIPCLOSE=4",
	"ready",
	"OK"
};

//------------------------------------------------------------------------------------
//-- PRIVATE DEFINITIONS -------------------------------------------------------------
//------------------------------------------------------------------------------------

/** Time to get a valid response after sending a commando (default: 5000 ms) */
# define TIME_TO_VALID_RESPONSE		5000

//------------------------------------------------------------------------------------
//-- STATIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------


static uint8_t getCRC(uint8_t * buffer, uint8_t size){
	uint8_t result  = 0;
	for(uint8_t i=0 ; i<size; i++){
		result = result ^ buffer[i];
	}
	return result;
}

//------------------------------------------------------------------------------------
static bool checkCRC(uint8_t * buffer, uint8_t crc, uint8_t size){
	if(getCRC(buffer, size) == crc)
		return true;
	return false;
}

//------------------------------------------------------------------------------------
static void OnTopicUpdateCallback(void *subscriber, const char * topicname){
	VirtualReceiver *me = (VirtualReceiver*)subscriber;
	if(strcmp(topicname, "/keyb") == 0){
		me->notifyUpdate(VirtualReceiver::KEY_EV_READY);
		return;
	}
	if(strcmp(topicname, "/gps") == 0){
		me->notifyUpdate(VirtualReceiver::GPS_EV_READY);
		return;
	}
}


//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

VirtualReceiver::VirtualReceiver(osPriority prio, Serial *serial, DigitalOut *endis) {
	_serial = serial;
	_serial->baud(115200);
	_endis = endis;
	_endis->write(DISABLE);
	_status = 0;
	_th = 0;
	_th = new Thread(&VirtualReceiver::task, this, prio);
}

//------------------------------------------------------------------------------------
VirtualReceiver::~VirtualReceiver() {
	// TODO Auto-generated destructor stub
}

//------------------------------------------------------------------------------------
Thread * VirtualReceiver::getThread() {
	return _th;
}

//------------------------------------------------------------------------------------
void VirtualReceiver::RxISRCallback(void){
	_th->signal_set(VR_EV_DATAREADY);
}

//------------------------------------------------------------------------------------
void VirtualReceiver::TxISRCallback(void){
	_th->signal_set(VR_EV_DATASENT);
}

//------------------------------------------------------------------------------------
void VirtualReceiver::notifyUpdate(uint32_t event){
	_th->signal_set(event);
}

//------------------------------------------------------------------------------------
//-- PROTECTED/PRIVATE FUNCTIONS -----------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
void VirtualReceiver::run(){
	while(_th == 0){
		Thread::wait(100);
	}
	// Attaches to serial peripheral
	_serial->attach(this, &VirtualReceiver::RxISRCallback, (SerialBase::IrqType)RxIrq);
//	_serial->attach(this, &VirtualReceiver::TxISRCallback, (SerialBase::IrqType)TxIrq);
	
	// Attaches to topic updates
	MsgBroker::Exception e;
	MsgBroker::attach("/gps", this, OnTopicUpdateCallback, &e);
	MsgBroker::attach("/keyb", this, OnTopicUpdateCallback, &e);

	// Enables link device
	_endis->write(ENABLE);
	Thread::wait(500);
	
	// setup waiting events...
	_protostat = STAT_WAIT_COMMAND;	
	_errcount = 0;
	
	// starts sending check mode at command
	_rxbuf.count = 0;
	_status = CHECK_MODE;
	_timeout = TIME_TO_VALID_RESPONSE;
	send(ATcmd[_status]);
	for(;;){
		// Wait incoming data ... 
		osEvent oe = _th->signal_wait((VR_EV_DATAREADY | KEY_EV_READY | GPS_EV_READY), _timeout);
		// if timeout...
		if(oe.status == osEventTimeout){
			#warning TODO........
		}
		if(oe.status == osEventSignal && (oe.value.signals & VR_EV_DATAREADY) != 0){	
			_th->signal_clr(VR_EV_DATAREADY);						
			while(_serial->readable()){
				_rxbuf.data[_rxbuf.count++] = (uint8_t)_serial->getc();
				processResponse();
			}
		}
		
//		if(oe.status == osEventTimeout){
//			// if waiting response, active alarm ALARM_MISSING_RESPONSE
//			if(_protostat >= STAT_WAIT_RESPONSE){
//				if(_protostat == STAT_WAIT_RESPONSE){
//					_alarmdata.alarm[0] = (uint8_t)Topic::ALARM_MISSING_RESPONSE;
//				}
//				else{
//					_alarmdata.alarm[0] = (uint8_t)Topic::ALARM_WRONG_RESPONSE;
//				}
//				MsgBroker::publish("/alarm", &_alarmdata, sizeof(Topic::AlarmData_t), &e);
//				_timeout = osWaitForever;
//				_protostat = STAT_WAIT_COMMAND;
//			}			
//		}
//		if(oe.status == osEventSignal && (oe.value.signals & VR_EV_DATAREADY) != 0){	
//			_th->signal_clr(VR_EV_DATAREADY);						
//			while(_serial->readable()){
//				uint8_t cmd;
//				// reads data and executes protocol state machine
//				cmd = decodePdu((uint8_t)_serial->getc());
//				switch(cmd){
//					case CMD_ERROR:{
//						// nothing done, if error persists, EV_TIMEOUT will raise later
//						break;
//					}
//					case CMD_READY:{
//						// if NACK received, transmission wan't understood, raise alarm and continue
//						if((_rxpdu.data[0] & CMD_NACK) != 0){
//						}
//						// publish /stat topic
//						else{
//							memcpy(&_statdata, &_rxpdu.data[1], sizeof(Topic::StatusData_t));
//							MsgBroker::publish("/stat", &_alarmdata, sizeof(Topic::AlarmData_t), &e);
//							_timeout = osWaitForever;
//							_protostat = STAT_WAIT_COMMAND;
//						}
//						break;
//					}
//					default: /*CMD_DECODING*/{
//						// nothing done, decoding reception stream
//						break;
//					}
//				}
//			}
//		}
		if(oe.status == osEventSignal && (oe.value.signals & KEY_EV_READY) != 0){		
			_th->signal_clr(KEY_EV_READY);						
			Topic::KeyData_t * keydata = (Topic::KeyData_t *)MsgBroker::getTopicData("/keyb", &e);
			memcpy(&_txpdu.data[1], keydata, sizeof(Topic::KeyData_t));
			MsgBroker::consumed("/keyb", &e);
			_txpdu.size = sizeof(Topic::KeyData_t)+1;
			_txpdu.data[0] = CMD_SET_KEY;
			send();		
			_timeout = TIME_TO_VALID_RESPONSE;
			_protostat = STAT_WAIT_RESPONSE;
		}
		if(oe.status == osEventSignal && (oe.value.signals & GPS_EV_READY) != 0){	
			_th->signal_clr(GPS_EV_READY);						
			Topic::GpsData_t * gpsdata = (Topic::GpsData_t *)MsgBroker::getTopicData("/gps", &e);
			memcpy(&_txpdu.data[1], gpsdata, sizeof(Topic::GpsData_t));
			MsgBroker::consumed("/gps", &e);
			_txpdu.size = sizeof(Topic::GpsData_t)+1;
			_txpdu.data[0] = CMD_SET_GPS;
			send();
			_timeout = TIME_TO_VALID_RESPONSE;
			_protostat = STAT_WAIT_RESPONSE;
		}
	}	
}

//------------------------------------------------------------------------------------
void VirtualReceiver::send(const char * atcmd){
	int size = strlen(atcmd);
	// sends 
	for(int i=0;i<size;i++){
		_serial->putc(atcmd[i]);
	}
	_serial->putc('\r');
	_serial->putc('\n');
}

//------------------------------------------------------------------------------------
void VirtualReceiver::send(){
	// build tx pdu: head, crc
	_txpdu.head = HEAD_FLAG;
	_txpdu.data[_txpdu.size] = getCRC(&_txpdu.size, _txpdu.size + 1);
	// sends 
	for(int i=0;i<_txpdu.size+3;i++){
		_serial->putc(((char *)(&_txpdu))[i]);
	}
}

//------------------------------------------------------------------------------------
uint8_t VirtualReceiver::decodePdu(uint8_t data){
	static uint8_t datasize = 0;
	switch((int)_protostat){
		case STAT_WAIT_RESPONSE:{
			if(data != HEAD_FLAG){
				return CMD_ERROR;
			}
			_rxpdu.head = data;
			_protostat = STAT_RECV_HEAD;
			return CMD_DECODING;
		}
		case STAT_RECV_HEAD:{
			if(data >= MAX_SIZE){
				return CMD_ERROR;
			}
			_rxpdu.size = data;
			datasize = 0;
			_protostat = STAT_RECV_SIZE;
			return CMD_DECODING;
		}
		case STAT_RECV_SIZE:{
			_rxpdu.data[datasize++] = data;
			if(datasize >= _rxpdu.size){
				_protostat = STAT_RECV_DATA;
			}
			return CMD_DECODING;
		}
		case STAT_RECV_DATA:{
			_rxpdu.data[datasize] = data;
			if(checkCRC(&_rxpdu.size, _rxpdu.data[datasize], datasize+1)){
				return CMD_READY;
			}
			return CMD_ERROR;
		}
	}
	return CMD_ERROR;
}

//------------------------------------------------------------------------------------
uint8_t VirtualReceiver::processResponse(){
	uint8_t last = _rxbuf.count-1;
	if(last < 1){
		return CMD_DECODING;
	}
	if(_rxbuf.data[last-1] != '\r' || _rxbuf.data[last] != '\n'){
		return CMD_DECODING;
	}
	
}
