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
	CLOSE_TCP_CONNECTION,
	CHECK_TCP_STATUS,
	SEND_TCP_DATA,
	FINISH_TCP_DATA
};
struct ProtocolAction{
	const char * at_cmd;
	const char * response;
	uint8_t next_stat_ok;
	uint8_t next_stat_err;
};

static ProtocolAction actions[] = {
	{"AT+CWMODE?", 	
	 "+CWMODE:3", 
	 SET_AP_IP,  
	 SET_MODE},
	
	{"AT+CWMODE=3", 
	 "", 		 
	 RESET_LNK,  
	 SET_MODE},
	
	{"AT+RST", 		
	 "", 		 
	 CHECK_MODE, 
	 CHECK_MODE},
	
	{"AT+CIPAP=""192.168.8.10"",""192.168.8.10"",""255.255.255.0""", 		
	 "", 		 
	 SET_AP_NETWORK, 
	 SET_AP_IP},
	
	{"AT+CWSAP=""EAGLE-REMOTE"",""eagleQUAD"",4,3", 		
	 "", 		 
	 SET_MUX, 
	 SET_AP_NETWORK},
	
	{"AT+CIPMUX=1", 		
	 "", 		 
	 START_AP_SERVER, 
	 SET_MUX},
	
	{"AT+CIPSERVER=1,80", 		
	 "", 		 
	 SET_AP_TIMEOUT, 
	 START_AP_SERVER},
	
	{"AT+CIPSTO=7200", 		
	 "", 		 
	 GET_AP_LIST, 
	 SET_AP_TIMEOUT},
	
	{"AT+CWLAP", 		
	 "EAGLE-TELEMETRY", 		 
	 CONNECT_TO_AP, 
	 GET_AP_LIST},
	
	{"AT+CWJAP=""EAGLE-TELEMETRY"",""eagleQUAD""", 		
	 "", 		 
	 CHECK_AP_CONNECTION, 
	 CONNECT_TO_AP},
	
	{"AT+CWJAP?", 		
	 "+CWJAP:""EAGLE-TELEMETRY""", 		 
	 START_TCP_CLIENT, 
	 CHECK_AP_CONNECTION},
	
	{"AT+CIPSTART=4,""TCP"",""192.168.7.10"",80", 		
	 "OK;4,CONNECT;", 		 
	 CHECK_TCP_STATUS, 
	 START_TCP_CLIENT},
	
	{"AT+CIPCLOSE=4", 		
	 "CLOSE", 		 
	 CHECK_AP_CONNECTION, 
	 CHECK_AP_CONNECTION},
	
	{"AT+CIPSTATUS", 		
	 "+CIPSTATUS:4,""TCP"",""192.168.7.10"",80,0", 		 
	 CHECK_TCP_STATUS, 
	 CLOSE_TCP_CONNECTION},
	
	{"AT+CIPSEND=4,", 		
	 ">", 		 
	 FINISH_TCP_DATA, 
	 SEND_TCP_DATA},
	
	{"\r\n\r\n", 		
	 "SEND OK", 		 
	 CHECK_TCP_STATUS, 
	 CLOSE_TCP_CONNECTION},
	
};

//------------------------------------------------------------------------------------
//-- PRIVATE DEFINITIONS -------------------------------------------------------------
//------------------------------------------------------------------------------------

/** Time to get a valid response after sending a commando (default: 5000 ms) */
#define TIME_TO_VALID_RESPONSE		5000

/** Time to check tcp socket is open and connected (10sec) */
#define TIME_TO_CHECK_TCP_STAT		10000

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
static void OnTimerUpdateCallback(void const *obj){
	VirtualReceiver *me = (VirtualReceiver*)obj;
	me->notifyUpdate(VirtualReceiver::TIMER_EV_READY);
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
	_tmr = new RtosTimer(OnTimerUpdateCallback, osTimerPeriodic, (void *)this);
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
	
	// starts sending check mode at command
	_rxbuf.count = 0;
	_errcount = 0;
	_timeout = TIME_TO_VALID_RESPONSE;	
	_signals = VR_EV_DATAREADY;
	_mode = INITIALIZING;
	updateStatus(CHECK_MODE);
	
	for(;;){
		// Wait incoming data ... 
		osEvent oe = _th->signal_wait(_signals, _timeout);
		// if timeout...
		if(oe.status == osEventTimeout){
			#warning TODO........
		}
		if(oe.status == osEventSignal && (oe.value.signals & VR_EV_DATAREADY) != 0){	
			_th->signal_clr(VR_EV_DATAREADY);						
			while(_serial->readable()){
				_rxbuf.data[_rxbuf.count++] = (uint8_t)_serial->getc();
			}
			char *pdata;
			int len;
			uint8_t result = processResponse(pdata, &len);
			switch(result){
				case CMD_ACK:{
					_rxbuf.count = 0;
					_errcount = 0;
					updateStatus(actions[_status].next_stat_ok);
					break;
				}
				case CMD_DATA:{
					_rxbuf.count = 0;
					break;
				}
				case CMD_NACK:
				case CMD_ERROR:{
					_rxbuf.count = 0;
					if(++_errcount > 5){
						_errcount = 0;
						updateStatus(actions[_status].next_stat_err);
					}
					else{
						updateStatus(_status);
					}
					break;
				}
				default:{
					break;
				}
			}
			
		}
		
		if(oe.status == osEventSignal && (oe.value.signals & KEY_EV_READY) != 0){		
			_th->signal_clr(KEY_EV_READY);						
			Topic::KeyData_t * keydata = (Topic::KeyData_t *)MsgBroker::getTopicData("/keyb", &e);
			memcpy(&_txbuf.data[1], keydata, sizeof(Topic::KeyData_t));
			MsgBroker::consumed("/keyb", &e);
			_txbuf.data[0] = CMD_SET_KEY;
			_txbuf.count = sizeof(Topic::KeyData_t)+1;
			updateStatus(SEND_TCP_DATA);
			
		}
		
		if(oe.status == osEventSignal && (oe.value.signals & GPS_EV_READY) != 0){	
			_th->signal_clr(GPS_EV_READY);						
			Topic::GpsData_t * gpsdata = (Topic::GpsData_t *)MsgBroker::getTopicData("/gps", &e);
			memcpy(&_txbuf.data[1], gpsdata, sizeof(Topic::GpsData_t));
			MsgBroker::consumed("/gps", &e);
			_txbuf.data[0] = CMD_SET_GPS;
			_txbuf.count = sizeof(Topic::GpsData_t)+1;
			updateStatus(SEND_TCP_DATA);
		}
	}	
}

//------------------------------------------------------------------------------------
int8_t VirtualReceiver::updateStatus(int8_t stat){
	uint8_t next_mode = INITIALIZING;
	_status = stat;
	switch(_status){
		case FINISH_TCP_DATA:
			send();
			// continue below...
		default:{
			if(_status >= CHECK_TCP_STATUS){
				next_mode = READY;
			}
			if(_status == SEND_TCP_DATA){
				char len[4];
				char cmd[32];
				sprintf(len, "%d", _txbuf.count+1);
				strcpy(cmd, actions[_status].at_cmd);
				strcat(cmd, len);
				send(cmd);
			}
			else{
				send(actions[_status].at_cmd);
			}
			break;
		}
	}
	
	// update waiting signals according with operational mode
	if(_mode == INITIALIZING && next_mode == READY){
		_mode = READY;
		_signals = (VR_EV_DATAREADY | GPS_EV_READY | KEY_EV_READY | TIMER_EV_READY);
		_th->signal_clr(GPS_EV_READY | KEY_EV_READY | TIMER_EV_READY);
		_tmr->start(TIME_TO_CHECK_TCP_STAT);
	}
	else if(_mode == READY && next_mode == INITIALIZING){
		_mode = INITIALIZING;
		_signals = VR_EV_DATAREADY;
		_th->signal_clr(GPS_EV_READY | KEY_EV_READY);
		_tmr->stop();
	}
	//returns actual status
	return _status;
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
	// sends 
	for(int i=0;i<_txbuf.count;i++){
		_serial->putc(_txbuf.data[i]);
	}
}

//------------------------------------------------------------------------------------
uint8_t VirtualReceiver::processResponse(char * pdata, int * len){
	uint8_t last = _rxbuf.count-1;
	char * result;
	if(last < 1){
		return CMD_DECODING;
	}
	if(_rxbuf.data[last-1] != '\r' || _rxbuf.data[last] != '\n'){
		return CMD_DECODING;
	}
	// check unhandled conditions
	if(strstr((char*)_rxbuf.data, "ready")){
		return CMD_RESET;
	}	
	result = strstr((char*)_rxbuf.data, "+IPD,");
	if(result){
		char * end = strchr((char*)result, ':');
		pdata = end+1;
		end--;
		int mult = 1;
		*len = 0;
		while(*end != ','){
			*len = mult * (*end - 48);
			mult *= 10;
		}
		return CMD_DATA;
	}	
	if(strlen(actions[_status].response) == 0){
		if(strstr((char*)_rxbuf.data, "OK")){
			return CMD_ACK;			
		}	
	}
	else{
		if(strstr((char*)_rxbuf.data, actions[_status].response)){
			return CMD_ACK;
		}
	}	
	return CMD_NACK;
}
