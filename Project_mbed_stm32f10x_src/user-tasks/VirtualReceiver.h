/*
 * VirtualReceiver.h
 *
 *  Created on: 20/04/2015
 *      Author: raulMrello
 *
 *  VirtualReceiver is an Active module (running as a Task) that will virtualize a 
 *	remote Telemetry Receiver. It will manage Serial communications with a wifi modem
 *	to establish a connection with the remote Telemetry Receiver. Its basic functions are:
 *
 *	1 - Configure and setup communications with the remote peer.
 *	2 - Attach to /gps and /keyb topic updates and pass data to the wifi modem
 *	3 - Convert serial responses into /alarm, /stat topics for other modules
 */

#ifndef SRC_ACTIVE_MODULES_VIRTUALRECEIVER_H_
#define SRC_ACTIVE_MODULES_VIRTUALRECEIVER_H_


//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "mbed.h"
#include "rtos.h"
#include "MsgBroker.h"
#include "Topics.h"
#include "Logger.h"
#include "State.h"
using namespace hsm;

//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//-- CLASS ---------------------------------------------------------------------------
//------------------------------------------------------------------------------------

class VirtualReceiver : public Hsm {
public:
	
	//----------------------
	//------- EVENTS -------
	//----------------------
	
	class NackEvent : public Event{
	public:
		NackEvent(uint32_t sig, Topic::AckData_t* data) : Event(sig){
			nack.ackCode = data->ackCode;
			nack.req = data->req;
		}
		Topic::AckData_t nack;
	}; typedef NackEvent AckEvent;
	
	class DataEvent : public Event{
	public:
		DataEvent(uint32_t sig, void * data, int size) : Event(sig){
			if(!size){
				_data = 0;
				_size = 0;				
			}
			else{
				_data = malloc(size);
				if(_data){
					_size = size;
					memcpy(_data, data, _size);
				}
			}
		}
		~DataEvent(){
			if(_data){
				free(_data);
				_data = 0;
				_size = 0;
			}
		}
		void * _data;
		int _size;
	}; 

	
	//-----------------------------
	//------- STATE MACHINE -------
	//-----------------------------
	
	//-------------------------------------------------------------------------
	
	class StSetup : public State{
	public:
		StSetup(State * parent = (State*)0, void * xif = 0) : State(parent, xif){
			// inserto manejadores de evento
			attach(VirtualReceiver::evNack, this, (State* (State::*)(Event*))&StSetup::onNack);
			attach(VirtualReceiver::evTimeout, this, (State* (State::*)(Event*))&StSetup::onNack);
		}
		
		// Implementaciones entry/exit
		virtual State* entry(){
			TRAN(((VirtualReceiver*)_xif)->stConfig);
		}
		virtual void exit(){
		}				
		// Manejadores de eventos
		State* onNack(Event* e){
			if(((VirtualReceiver*)_xif)->_errcount < VirtualReceiver::ERR_COUNT_LIMIT){
				((VirtualReceiver*)_xif)->_errcount++;
				((VirtualReceiver*)_xif)->updateStatus();
				DONE();
			}
			((VirtualReceiver*)_xif)->notifyError();
			((Hsm*)((VirtualReceiver*)_xif))->raiseEvent(new Event(VirtualReceiver::evError));
			DONE();
		}
	};friend class StSetup;	
	
	//-------------------------------------------------------------------------
	
	class StConfig : public State{
	public:
		StConfig(State * parent = (State*)0, void * xif = 0) : State(parent, xif){
			// inserto manejadores de evento
			attach(VirtualReceiver::evAck, this, (State* (State::*)(Event*))&StConfig::onAck);
		}
		
		// Implementaciones entry/exit
		virtual State* entry(){
			((VirtualReceiver*)_xif)->_errcount = 0;
			((VirtualReceiver*)_xif)->updateStatus(VirtualReceiver::CHECK_MODE);
			DONE();
		}
		
		virtual void exit(){
		}			
		
		// Manejadores de eventos
		State* onAck(Event* e){
			if(((VirtualReceiver*)_xif)->_status < VirtualReceiver::START_TCP_CLIENT){
				((VirtualReceiver*)_xif)->updateStatusOk();
				DONE();				
			}
			TRAN(((VirtualReceiver*)_xif)->stConnectTcp);			
		}		
	};friend class StConfig;	
	
	//-------------------------------------------------------------------------
	
	class StConnectTcp : public State{
	public:
		StConnectTcp(State * parent = (State*)0, void * xif = 0) : State(parent, xif){
			// inserto manejadores de evento
			attach(VirtualReceiver::evAck, this, (State* (State::*)(Event*))&StConnectTcp::onAck);
		}
		
		// Implementaciones entry/exit
		virtual State* entry(){
			((VirtualReceiver*)_xif)->updateStatusOk();
			DONE();
		}
		
		virtual void exit(){
		}			
		
		// Manejadores de eventos
		State* onAck(Event* e){
			TRAN(((VirtualReceiver*)_xif)->stConnected);			
		}
		
	};friend class StConnectTcp;	
	
	//-------------------------------------------------------------------------	
	
	class StDisconnectTcp : public State{
	public:
		StDisconnectTcp(State * parent = (State*)0, void * xif = 0) : State(parent, xif){
			// inserto manejadores de evento
			attach(VirtualReceiver::evAck, this, (State* (State::*)(Event*))&StDisconnectTcp::onAck);
		}
		
		// Implementaciones entry/exit
		virtual State* entry(){
			((VirtualReceiver*)_xif)->updateStatus(VirtualReceiver::CLOSE_TCP_CONNECTION);
			DONE();
		}
		
		virtual void exit(){
		}			
		
		// Manejadores de eventos
		State* onAck(Event* e){
			TRAN(((VirtualReceiver*)_xif)->stConnectTcp);			
		}
		
	};friend class StDisconnectTcp;	
	
	//-------------------------------------------------------------------------	
	
	class StConnected : public State{
	public:
		StConnected(State * parent = (State*)0, void * xif = 0) : State(parent, xif){
			// inserto manejadores de evento
			attach(VirtualReceiver::evSend, this, (State* (State::*)(Event*))&StConnected::onSend);
			attach(VirtualReceiver::evRecv, this, (State* (State::*)(Event*))&StConnected::onRecv);
			attach(VirtualReceiver::evError, this, (State* (State::*)(Event*))&StConnected::onError);
		}
		
		// Implementaciones entry/exit
		virtual State* entry(){
			((VirtualReceiver*)_xif)->enableTopics();
			TRAN(((VirtualReceiver*)_xif)->stWaitingData);
		}
		
		virtual void exit(){
		}			
		
		// Manejadores de eventos
		State* onSend(Event* e){
			VirtualReceiver::DataEvent * de = (VirtualReceiver::DataEvent *)e;
			((VirtualReceiver*)_xif)->saveData(de->_data, de->_size);			
			DONE();
		}
		State* onRecv(Event* e){
			((VirtualReceiver*)_xif)->notifyData();			
			DONE();
		}
		State* onError(Event* e){
			((VirtualReceiver*)_xif)->_errcount = 0;
			((VirtualReceiver*)_xif)->disableTopics();
			TRAN(((VirtualReceiver*)_xif)->stDisconnectTcp);			
		}
		
	};friend class StConnected;	
	
	//-------------------------------------------------------------------------	
	
	class StWaitingData : public State{
	public:
		StWaitingData(State * parent = (State*)0, void * xif = 0) : State(parent, xif){
			// inserto manejadores de evento
			attach(VirtualReceiver::evTimeout, this, (State* (State::*)(Event*))&StWaitingData::onTimeout);
			attach(VirtualReceiver::evSend, this, (State* (State::*)(Event*))&StWaitingData::onSend);
		}
		
		// Implementaciones entry/exit
		virtual State* entry(){
			((VirtualReceiver*)_xif)->_timeout = VirtualReceiver::TCP_TIMEOUT;
			if(((VirtualReceiver*)_xif)->getData()){
				((VirtualReceiver*)_xif)->sendData();	
				TRAN(((VirtualReceiver*)_xif)->stProcessing);
			}
			DONE();
		}
		
		virtual void exit(){
		}			
		
		// Manejadores de eventos
		State* onSend(Event* e){
			VirtualReceiver::DataEvent * de = (VirtualReceiver::DataEvent *)e;
			((VirtualReceiver*)_xif)->saveData(de->_data, de->_size);			
			((VirtualReceiver*)_xif)->sendData();	
			TRAN(((VirtualReceiver*)_xif)->stProcessing);
		}
		
		// Manejadores de eventos
		State* onTimeout(Event* e){
			if(((VirtualReceiver*)_xif)->getData()){
				((VirtualReceiver*)_xif)->sendData();	
				TRAN(((VirtualReceiver*)_xif)->stProcessing);
			}			
			// si no hay datos, conmuta al chequeo de conexión
			TRAN(((VirtualReceiver*)_xif)->stCheckConnection);
		}
		
	};friend class StWaitingData;	
	
	//-------------------------------------------------------------------------	
	
	class StCheckConnection : public State{
	public:
		StCheckConnection(State * parent = (State*)0, void * xif = 0) : State(parent, xif){
			// inserto manejadores de evento
			attach(VirtualReceiver::evAck, this, (State* (State::*)(Event*))&StCheckConnection::onAck);
			attach(VirtualReceiver::evNack, this, (State* (State::*)(Event*))&StCheckConnection::onNack);
			attach(VirtualReceiver::evTimeout, this, (State* (State::*)(Event*))&StCheckConnection::onNack);
		}
		
		// Implementaciones entry/exit
		virtual State* entry(){
			((VirtualReceiver*)_xif)->_errcount = 0;
			((VirtualReceiver*)_xif)->updateStatus(VirtualReceiver::CHECK_TCP_STATUS);
			DONE();
		}
		
		virtual void exit(){
		}			
		
		// Manejadores de eventos
		State* onAck(Event* e){
			TRAN(((VirtualReceiver*)_xif)->stWaitingData);			
		}
		State* onNack(Event* e){
			if(((VirtualReceiver*)_xif)->_errcount < VirtualReceiver::ERR_COUNT_LIMIT){
				((VirtualReceiver*)_xif)->_errcount++;
				((VirtualReceiver*)_xif)->updateStatus();
				DONE();
			}
			((VirtualReceiver*)_xif)->notifyError();
			((Hsm*)((VirtualReceiver*)_xif))->raiseEvent(new Event(VirtualReceiver::evError));
			DONE();
		}
		
	};friend class StCheckConnection;	
	
	//-------------------------------------------------------------------------	
	
	class StProcessing : public State{
	public:
		StProcessing(State * parent = (State*)0, void * xif = 0) : State(parent, xif){
			// inserto manejadores de evento
			attach(VirtualReceiver::evAck, this, (State* (State::*)(Event*))&StProcessing::onAck);
			attach(VirtualReceiver::evNack, this, (State* (State::*)(Event*))&StProcessing::onNack);
			attach(VirtualReceiver::evTimeout, this, (State* (State::*)(Event*))&StProcessing::onNack);
		}
		
		// Implementaciones entry/exit
		virtual State* entry(){
			((VirtualReceiver*)_xif)->_errcount = 0;
			((VirtualReceiver*)_xif)->setTimeout();
			DONE();
		}
		
		virtual void exit(){
		}			
		
		// Manejadores de eventos
		State* onAck(Event* e){
			// if data sent, then finish
			if(((VirtualReceiver*)_xif)->_status == VirtualReceiver::SEND_TCP_DATA){
				((VirtualReceiver*)_xif)->updateStatusOk();
				DONE();
			}
			// si los datos se han finalizado, notificar envío
			((VirtualReceiver*)_xif)->eraseData();
			((VirtualReceiver*)_xif)->notifyAck();
			TRAN(((VirtualReceiver*)_xif)->stWaitingData);
		}
		State* onNack(Event* e){
			if(((VirtualReceiver*)_xif)->_errcount < VirtualReceiver::ERR_COUNT_LIMIT){
				((VirtualReceiver*)_xif)->_errcount++;
				((VirtualReceiver*)_xif)->updateStatus();
				DONE();
			}
			((Hsm*)((VirtualReceiver*)_xif))->raiseEvent(new Event(VirtualReceiver::evError));
			DONE();
		}
		
	};friend class StProcessing;	
	
	//-------------------------------------------------------------------------
	
	// Implementaciones entry/exit
	virtual State* entry(){		
		TRAN(stSetup);
	}
	
	virtual void exit(){		
	}	
		
	// Manejadores de eventos
	State* onError(Event* e){
		// Reset link device
		_endis->write(VirtualReceiver::DISABLE);
		Thread::wait(250);
		_endis->write(VirtualReceiver::ENABLE);	
		Thread::wait(1000);
		_rxbuf.count = 0;_rxbuf.ovf = false;
		_errcount = 0;
		TRAN(stSetup);		
	}
	State* onReset(Event* e){
		TRAN(stSetup);		
	}
	
	// Estados
	StSetup *stSetup;
	StConfig *stConfig;
	StConnectTcp *stConnectTcp;
	StDisconnectTcp *stDisconnectTcp;
	StConnected *stConnected;
	StWaitingData *stWaitingData;
	StCheckConnection *stCheckConnection;
	StProcessing *stProcessing;
	
	// Eventos de la máquina de estados
	static const uint32_t evAck 	= (USER_SIG << 0);
	static const uint32_t evNack 	= (USER_SIG << 1);
	static const uint32_t evTimeout = (USER_SIG << 2);
	static const uint32_t evSend    = (USER_SIG << 3);
	static const uint32_t evRecv    = (USER_SIG << 4);
	static const uint32_t evError 	= (USER_SIG << 5);
	static const uint32_t evReset 	= (USER_SIG << 6);

	// Constantes
	static const uint8_t POWER_UP 				= 0;	///< status flags
	static const uint8_t CHECK_MODE 			= 1;
	static const uint8_t SET_MODE 				= 2;
	static const uint8_t RESET_LNK 				= 3;
	static const uint8_t SET_AP_IP 				= 4;
	static const uint8_t SET_AP_NETWORK 		= 5;
	static const uint8_t SET_MUX 				= 6;	
	static const uint8_t START_AP_SERVER 		= 7;
	static const uint8_t SET_AP_TIMEOUT 		= 8;
	static const uint8_t GET_AP_LIST 			= 9;
	static const uint8_t CONNECT_TO_AP 			= 10;
	static const uint8_t START_TCP_CLIENT 		= 11;
	static const uint8_t CLOSE_TCP_CONNECTION 	= 12;
	static const uint8_t CHECK_TCP_STATUS 		= 13;
	static const uint8_t SEND_TCP_DATA 			= 14;	
	static const uint8_t FINISH_TCP_DATA 		= 15;
	static const uint8_t THIS_MODE 				= 16;
	
	static const uint8_t CMD_ACK				= 0x00;	///< operation result flags
	static const uint8_t CMD_NACK				= 0xfe;	
	static const uint8_t CMD_ERROR				= 0xff;	

	static const uint8_t ERR_COUNT_LIMIT		= 5;	
	
	static const uint32_t ACK_TIMEOUT 			= 5000;	///< serial tx-rx timeout
	static const uint32_t TCP_TIMEOUT 			= 5000;	///< tcp timeout

protected:
	// variables
	int _errcount;
	int8_t _status;
	char *_rdata;
	int _rdatasize;

	// interfaces
	bool getData();
	void saveData(void * data, int size);
	void updateStatus(int8_t stat = THIS_MODE);
	void updateStatusOk();
	void setTimeout(uint32_t timeout = ACK_TIMEOUT);
	void sendData();
	void send(const char * atcmd = 0);
	void eraseData();
	void notifyAck();
	void notifyError();
	void notifyData();
	void enableTopics();
	void disableTopics();

		
	//-------------------------------------
	//------- VIRTUALRECEIVER CLASS -------
	//-------------------------------------
	
public:

	/** Constructor, destructor, getter and setter */
	VirtualReceiver(osPriority prio, RawSerial *serial, DigitalOut *endis, Logger * logger = 0) : Hsm(){				
		// creo estados
		stSetup = new StSetup(this, this);
		stConfig = new StConfig(stSetup, this);
		stConnectTcp = new StConnectTcp(stSetup, this);
		stDisconnectTcp = new StDisconnectTcp(stSetup, this);
		stConnected = new StConnected(this, this);
		stWaitingData = new StWaitingData(stConnected, this);
		stCheckConnection = new StCheckConnection(stConnected, this);
		stProcessing = new StProcessing(stConnected, this);
		// Inserto estados
		attachState(stSetup);
		attachState(stConfig);
		attachState(stConnectTcp);
		attachState(stDisconnectTcp);
		attachState(stConnected);
		attachState(stWaitingData);
		attachState(stProcessing);
		// Inserto manejadores de evento
		attach(VirtualReceiver::evError, this, (State* (State::*)(Event*))&VirtualReceiver::onError);
		attach(VirtualReceiver::evReset, this, (State* (State::*)(Event*))&VirtualReceiver::onReset);
		
		_serial = serial;
		_serial->baud(115200);
		_endis = endis;
		_endis->write(VirtualReceiver::DISABLE);
		_status = 0;
		_logger = logger;
		_th = 0;
		_th = new Thread(&VirtualReceiver::task, this, prio);
	}
	
	virtual ~VirtualReceiver();
	Thread *getThread();
	
	/** Serial Interrupt callbacks */
	void RxISRCallback(void);
	void IdleISRCallback(void);
	void TxISRCallback(void);
		
	/** Topic updates event enumeration */
	typedef enum {
		VR_EV_TOPICS 	= (1 << 0),
		TIMER_EV_READY	= (1 << 1),
		VR_EV_DATAREADY = (1 << 2),
		VR_EV_DATAEND	= (1 << 3)
	}EventEnum;	
		
	/** Task */
	static void task(void const *arg){
		VirtualReceiver *me = (VirtualReceiver*)arg;
		me->run();
	}	

private:

	enum ControlFlag {DISABLE=0, ENABLE=1};

	/** Reception buffer max size */
	static const uint16_t BUFF_SIZE = 512;

	/** Reception buffer structure */
	struct DataBuffer{
		uint16_t count;
		uint8_t data[BUFF_SIZE];	
		bool ovf;
	};
	
	/** Private variables */
	DataBuffer _rxbuf;
	RawSerial *_serial;
	DigitalOut *_endis;
	Logger * _logger;
	Thread *_th;
	uint32_t _timeout;
	Topic::NavigationData_t _navdata;
	Topic::AckData_t _ackdata;
	int32_t _signals;
	RtosTimer * _rxtmr;
	
	void run();


};


#endif /* SRC_ACTIVE_MODULES_VIRTUALRECEIVER_H_ */
