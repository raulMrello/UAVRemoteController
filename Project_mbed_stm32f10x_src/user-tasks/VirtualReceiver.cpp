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

//------------------------------------------------------------------------------------
//-- PRIVATE DEFINITIONS -------------------------------------------------------------
//------------------------------------------------------------------------------------

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

VirtualReceiver::VirtualReceiver(osPriority prio, Serial *serial) {
	_th = new Thread(&VirtualReceiver::task, this, prio);
	_serial = serial;
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
void VirtualReceiver::run(){
	// Attaches to installed serial peripheral
	_serial->attach(this, &VirtualReceiver::RxISRCallback, (SerialBase::IrqType)RxIrq);
	_serial->attach(this, &VirtualReceiver::TxISRCallback, (SerialBase::IrqType)TxIrq);
	
	// Attaches to topic updates
	MsgBroker::attach("/gps", this, OnTopicUpdateCallback, &e);
	MsgBroker::attach("/keyb", this, OnTopicUpdateCallback, &e);

	// starts waiting events...
	_protostat = STAT_WAITING;	
	_errcount = 0;
	_timeout = osWaitForever;
	for(;;){
		// Wait incoming data ... 
		osEvent oe = _th->signal_wait((VR_EV_DATAREADY | KEY_EV_READY | GPS_EV_READY), _timeout);
		// if timeout...
		if(oe.status == osEventTimeout){
			
		}
		if(oe.status == osEventSignal && (oe.value.signals & VR_EV_DATAREADY) != 0){		
					
		}
		if(oe.status == osEventSignal && (oe.value.signals & KEY_EV_READY) != 0){		
					
		}
		if(oe.status == osEventSignal && (oe.value.signals & GPS_EV_READY) != 0){		
			_txpdu.head = HEAD_FLAG;
			_txpdu.size = sizeof()
		}
	}	
}

//------------------------------------------------------------------------------------
bool VirtualReceiver::send(){
	// restart state machine, error counter and resp timeout
	_protostat = STAT_WAITING;	
	_errcount = 0;
	_timeout = 5000;
	// build tx pdu: head, crc
	_txpdu.head = HEAD_FLAG;
	_txpdu.data[_txpdu.size] = getCRC(&_txpdu.size, _txpdu.size + 1);
	// sends 
	while(_errcount < 5){
		for(int i=0;i<_txpdu.size+3;i++){
			_serial->putc(((char *)(&_txpdu))[i]);
		}
		for(;;){
			// Wait incoming data ... 
			osEvent oe = _th->signal_wait(VR_EV_DATAREADY, _timeout);
			// if timeout, then resends last command and increase error count
			if(oe.status == osEventTimeout){
				_errcount++;
				break; //resend
			}
			if(oe.status == osEventSignal && (oe.value.signals & VR_EV_DATAREADY) != 0){		
				bool rxerror = false;
				while(_serial->readable()){
					uint8_t data;
					// reads data and executes protocol state machine
					data = (uint8_t) _serial->getc();
					switch(decodePdu(data)){
						case CMD_ALARM_DATA:{
							MsgBroker::Exception e = MsgBroker::NO_ERRORS;
							MsgBroker::publish("/alarm", &_alarmdata, sizeof(AlarmData_t), &e);
							if(e != MsgBroker::NO_ERRORS){
								// TODO: add error handling ...
							}
							return true;
						}
						case CMD_STATUS_DATA:{
							MsgBroker::Exception e = MsgBroker::NO_ERRORS;
							MsgBroker::publish("/stat", &_statdata, sizeof(StatusData_t), &e);
							if(e != MsgBroker::NO_ERRORS){
								// TODO: add error handling ...
							}
							return true;
						}
						case CMD_RESP_ACK:{
							return true;
						}
						case CMD_ERROR:
						case CMD_RESP_NACK: {
							rxerror = true;
							break;
						}
						case CMD_DECODING: {
							break;
						}
					}
				}
				if(rxerror){
					break;//resend
				}
			}
		}	
	}
	return false;
}

//------------------------------------------------------------------------------------
//-- PROTECTED/PRIVATE FUNCTIONS -----------------------------------------------------
//------------------------------------------------------------------------------------

uint8_t VirtualReceiver::decodePdu(uint8_t data){
	static uint8_t datasize = 0;
	switch((int)_protostat){
		case STAT_WAITING:{
			if(data != HEAD_FLAG){
				return CMD_ERROR;
			}
			_rxpdu.head = data;
			_protostat = STAT_RECV_HEAD;
			return CMD_DECODING;
		}
		case STAT_RECV_HEAD:{
			if(data >= MAX_SIZE){
				_protostat = STAT_WAITING;
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
				return CMD_DECODING;
			}
			return CMD_DECODING;
		}
		case STAT_RECV_DATA:{
			_rxpdu.data[datasize] = data;
			_protostat = STAT_WAITING;
			if(checkCRC(&_rxpdu.size, _rxpdu.data[datasize], datasize+1)){
				return _rxpdu.data[0];
			}
			return CMD_ERROR;
		}
	}
}

