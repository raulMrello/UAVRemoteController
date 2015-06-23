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


struct ProtocolAction{
	const char * at_cmd;
	const char * response;
	uint8_t next_stat_ok;
	uint8_t next_stat_err;
};

ProtocolAction actions[] = {	
	{"", 			//POWER_UP
	 "ready", 
	 VirtualReceiver::CHECK_MODE,  
	 VirtualReceiver::POWER_UP},
	
	 
	{"AT+CWMODE?", 	// CHECK_MODE
	 "+CWMODE:3", 
	 VirtualReceiver::SET_AP_IP,  
	 VirtualReceiver::SET_MODE},
	
	
	{"AT+CWMODE=3", // SET_MODE
	 "", 		 
	 VirtualReceiver::RESET_LNK,  
	 VirtualReceiver::SET_MODE},
	
	
	{"AT+RST", 		//RESET_LINK
	 "ready", 		 
	 VirtualReceiver::CHECK_MODE, 
	 VirtualReceiver::RESET_LNK},
	
	
	{"AT+CIPAP=\"192.168.8.10\",\"192.168.8.10\",\"255.255.255.0\"", 	// SET_AP_IP	
	 "", 		 
	 VirtualReceiver::SET_AP_NETWORK, 
	 VirtualReceiver::SET_AP_IP},
	
	
	{"AT+CWSAP=\"EAGLE-REMOTE\",\"eagleQUAD\",4,3", 	// SET_AP_NETWORK	
	 "", 		 
	 VirtualReceiver::SET_MUX, 
	 VirtualReceiver::SET_AP_NETWORK},
	
	
	{"AT+CIPMUX=1", 	// SET_MUX	
	 "", 		 
	 VirtualReceiver::START_AP_SERVER, 
	 VirtualReceiver::SET_MUX},
	
	
	{"AT+CIPSERVER=1,80", 	// SET_AP_SERVER	
	 "", 		 
	 VirtualReceiver::SET_AP_TIMEOUT, 
	 VirtualReceiver::START_AP_SERVER},
	
	
	{"AT+CIPSTO=7200", 	// SET_AP_TIMEOUT	
	 "", 		 
	 VirtualReceiver::GET_AP_LIST, 
	 VirtualReceiver::SET_AP_TIMEOUT},
	
	
	{"AT+CWLAP", 		// GET_AP_LIST
	 "EAGLE-TELEMETRY", 		 
	 VirtualReceiver::CONNECT_TO_AP, 
	 VirtualReceiver::GET_AP_LIST},
		
	
	{"AT+CWJAP=\"EAGLE-TELEMETRY\",\"eagleQUAD\"", 		// CONNECT_TO_AP
	 "AT+CWJAP=\"EAGLE-TELEMETRY\",\"eagleQUAD\"", 		 
	 VirtualReceiver::START_TCP_CLIENT, 
	 VirtualReceiver::CONNECT_TO_AP},
	
	
	{"AT+CIPSTART=4,\"TCP\",\"192.168.7.10\",80", 	// START_TCP_CLIENT	
	 "4,CONNECT", 		 
	 VirtualReceiver::CHECK_TCP_STATUS, 
	 VirtualReceiver::CLOSE_TCP_CONNECTION},
	
	
	{"AT+CIPCLOSE=4", 		// CLOSE_TCP_CONNECTION
	 "CLOSE", 		 
	 VirtualReceiver::START_TCP_CLIENT, 
	 VirtualReceiver::START_TCP_CLIENT},
	
	
	{"AT+CIPSTATUS", 	// CHECK_TCP_STATUS	
	 "+CIPSTATUS:4,\"TCP\",\"192.168.7.10\",80,0", 		 
	 VirtualReceiver::CHECK_TCP_STATUS, 
	 VirtualReceiver::CLOSE_TCP_CONNECTION},
	
	
	{"AT+CIPSEND=4,", 	// SEND_TCP_DATA	
	 ">", 		 
	 VirtualReceiver::FINISH_TCP_DATA, 
	 VirtualReceiver::SEND_TCP_DATA},
	
	
	{"\r\n\r\n", 		// FINISH_TCP_DATA
	 "SEND OK", 		 
	 VirtualReceiver::CHECK_TCP_STATUS, 
	 VirtualReceiver::CLOSE_TCP_CONNECTION},	
};

//------------------------------------------------------------------------------------
//-- PRIVATE DEFINITIONS -------------------------------------------------------------
//------------------------------------------------------------------------------------

/** Time to idle line after receiving serial data (default: 10 ms) */
#define TIME_TO_IDLE_LINE			10

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
	VirtualReceiver *me = static_cast<VirtualReceiver *> (subscriber);
	bool raised = false;
	if(strcmp(topicname, "/nav") == 0){		
		Topic::NavigationData_t * topic = (Topic::NavigationData_t *)MsgBroker::getTopicData("/nav");
		if(topic){
			((Hsm*)me)->raiseEvent(new VirtualReceiver::DataEvent(VirtualReceiver::evSend, topic, sizeof(Topic::NavigationData_t)));
			raised = true;			
		}
		// marca el topic como consumido
		MsgBroker::consumed("/nav");
		if(raised){
			me->getThread()->signal_set(VirtualReceiver::VR_EV_TOPICS);
		}
	}
	
}

//------------------------------------------------------------------------------------
static void OnRxIdleCallback(void const *obj){
	VirtualReceiver *me = (VirtualReceiver*)obj;
	me->getThread()->signal_set(VirtualReceiver::VR_EV_DATAEND);		
}


//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

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
	if(_rxbuf.count == BUFF_SIZE-1){
		_rxbuf.data[_rxbuf.count++] = 0;
		_rxbuf.ovf = true;
		_rxbuf.count = _rxbuf.count & (BUFF_SIZE-1);
	}
	_rxbuf.data[_rxbuf.count++] = (uint8_t)_serial->getc();	
	_th->signal_set(VR_EV_DATAREADY);	
}

//------------------------------------------------------------------------------------
//-- PROTECTED/PRIVATE FUNCTIONS -----------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
void VirtualReceiver::run(){
	while(_th == 0){
		Thread::wait(100);
	}
	// clears buffer for possible undesired receptions during power up
	_rxbuf.count = 0;_rxbuf.ovf = false;
	
	// Attaches to serial peripheral
	_serial->attach(this, &VirtualReceiver::RxISRCallback, (SerialBase::IrqType)RxIrq);
	
	// Enables link device
	_endis->write(VirtualReceiver::DISABLE);
	Thread::wait(250);
	_endis->write(VirtualReceiver::ENABLE);	
	Thread::wait(1000);
		
	// Attaches to topic updates
	MsgBroker::Exception e;
	MsgBroker::attach("/nav", this, OnTopicUpdateCallback, &e);
	
	// start execution context
	_rxtmr = new RtosTimer(OnRxIdleCallback, osTimerOnce, (void *)this);
	_rxbuf.count = 0;_rxbuf.ovf = false;
	_errcount = 0;
	_timeout = osWaitForever;	
	_signals = (VR_EV_DATAREADY | VR_EV_DATAEND);
	_th->signal_clr(_signals);
	this->init();
	for(;;){
		// executes state machine
		this->dispatchEvents();
		// Wait incoming data ... 
		osEvent oe = _th->signal_wait_or(_signals, _timeout);
		// if timeout...
		if(oe.status == osEventTimeout){
			raiseEvent(new Event(evTimeout));
		}
		
		if(oe.status == osEventSignal && (oe.value.signals & TIMER_EV_READY) != 0){	
			_th->signal_clr(TIMER_EV_READY);
			raiseEvent(new Event(evTimeout));
			continue;
		}
		
		if(oe.status == osEventSignal && (oe.value.signals & VR_EV_DATAREADY) != 0){	
			_th->signal_clr(VR_EV_DATAREADY);
			_rxtmr->stop();
			_rxtmr->start(TIME_TO_IDLE_LINE);
		}
		
		if(oe.status == osEventSignal && (oe.value.signals & VR_EV_TOPICS) != 0){		
			_th->signal_clr(VR_EV_TOPICS);		
			continue;
		}
		
		if(oe.status == osEventSignal && (oe.value.signals & VR_EV_DATAEND) != 0){	
			_th->signal_clr(VR_EV_DATAEND);
			char * result;
			int count = (_rxbuf.ovf)? (BUFF_SIZE-1) : _rxbuf.count;
			if(count <= 1){
				raiseEvent(new Event(evNack));
				continue;
			}
			_rxbuf.data[count]=0; // add termination flag to received string
			// check unhandled conditions
			if(strstr((char*)_rxbuf.data, "ready")){
				raiseEvent(new Event(evReset));
				continue;
			}
			else if(strstr((char*)_rxbuf.data, "busy s")){
				// wait completion
				_rxbuf.count = 0;
				continue;
			}			
			result = strstr((char*)_rxbuf.data, "+IPD,");
			if(result){
				char * end = strchr((char*)result, ':');
				_rdata = end+1;
				end--;
				int mult = 1;
				_rdatasize = 0;
				while(*end != ','){
					_rdatasize = mult * (*end - 48);
					mult *= 10;
					end--;
				}
				raiseEvent(new Event(evRecv));
				continue;
			}	
			if(strlen(actions[_status].response) == 0){
				if(strstr((char*)_rxbuf.data, "\r\nOK\r\n")){
					raiseEvent(new Event(evAck));
					continue;		
				}	
			}
			else{
				if(strstr((char*)_rxbuf.data, actions[_status].response)){
					raiseEvent(new Event(evAck));
					continue;
				}
			}	
			raiseEvent(new Event(evNack));
		}
	}	
}


//------------------------------------------------------------------------------------
void VirtualReceiver::send(const char * atcmd){
	// waits 500ms between transmissions
	Thread::wait(500);
	if(atcmd){
		int size = strlen(atcmd);
		_rxbuf.count = 0;_rxbuf.ovf = false;
		// sends 
		for(int i=0;i<size;i++){
			_serial->putc(atcmd[i]);
		}
		_serial->putc('\r');
		_serial->putc('\n');
	}
	else{
		_rxbuf.count = 0;_rxbuf.ovf = false;
		// sends 
		char * data = (char*)&_navdata;
		for(int i=0;i<sizeof(Topic::NavigationData_t);i++){
			_serial->putc(data[i]);
		}
	}
	_timeout = ACK_TIMEOUT;
}

//------------------------------------------------------------------------------------
void VirtualReceiver::updateStatusOk(){
	_errcount = 0;
	updateStatus(actions[_status].next_stat_ok);
}

void VirtualReceiver::updateStatus(int8_t stat){
	if(stat != THIS_MODE){
		_status = stat;
		if(_logger){ _logger->sprintf((char*)"VIR: stat=%d # ", _status);}
	}
	switch(_status){
		case FINISH_TCP_DATA:
			send();
			// continue below...
		default:{
			if(_status == SEND_TCP_DATA){
				char len[4];
				char cmd[32];
				sprintf(len, "%d", sizeof(Topic::NavigationData_t)+1);
				strcpy(cmd, actions[_status].at_cmd);
				strcat(cmd, len);
				send(cmd);
			}
			// espera a recibir todas las wifis
			else if((_status == GET_AP_LIST && _errcount > 0)){
				return;
			}
			else{
				send(actions[_status].at_cmd);
			}
			break;
		}
	}
}

//------------------------------------------------------------------------------------
void VirtualReceiver::enableTopics(){
	_signals |= VR_EV_TOPICS;
	_th->signal_clr(VR_EV_TOPICS);
	// notifica sistema enlazado y listo.	
	Topic::AckData_t topic;
	topic.ackCode = Topic::ACK_OK;
	topic.req = Topic::ACK_START;
	MsgBroker::publish("/ack", &topic, sizeof(Topic::AckData_t));
	if(_logger){ _logger->print((char*)"VIR: RDY! # ", 12);}
}

//------------------------------------------------------------------------------------
void VirtualReceiver::disableTopics(){
	_signals &= ~VR_EV_TOPICS;
	_th->signal_clr(VR_EV_TOPICS);
	// notifica sistema desconectado.
	_timeout = osWaitForever;
	Topic::AckData_t topic;
	topic.ackCode = Topic::ACK_OK;
	topic.req = Topic::ACK_FINISH;
	MsgBroker::publish("/ack", &topic, sizeof(Topic::AckData_t));
	if(_logger){ _logger->print((char*)"VIR: DIS! # ", 12);}
}

//------------------------------------------------------------------------------------
void VirtualReceiver::notifyData(){
}

//------------------------------------------------------------------------------------
void VirtualReceiver::notifyError(){
}

//------------------------------------------------------------------------------------
bool VirtualReceiver::getData(){
	if(_navdata.gpsupd || _navdata.rcupd ||_navdata.modeupd){
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------------
void VirtualReceiver::saveData(void * data, int size){
	if(size != sizeof(Topic::NavigationData_t))
		return;
	memcpy(&_navdata, data, size);
	if(_logger){ _logger->print((char*)"VIR: SAVE! # ", 13);}
}

//------------------------------------------------------------------------------------
void VirtualReceiver::sendData(){
	if(_logger){ _logger->print((char*)"VIR: SEND! # ", 13);}
	updateStatus(SEND_TCP_DATA);
}

//------------------------------------------------------------------------------------
void VirtualReceiver::eraseData(){
	_navdata.gpsupd = false;
	_navdata.rcupd = false;
	_navdata.modeupd = false;
}

//------------------------------------------------------------------------------------
void VirtualReceiver::notifyAck(){
	_ackdata.ackCode = Topic::ACK_OK;
	_ackdata.req = _navdata.modedata.req;
	MsgBroker::publish("/ack", &_ackdata, sizeof(Topic::AckData_t));
	if(_logger){ _logger->print((char*)"VIR: ACKD! # ", 13);}
}

//------------------------------------------------------------------------------------
void VirtualReceiver::setTimeout(uint32_t timeout){
	_timeout = timeout;
}

