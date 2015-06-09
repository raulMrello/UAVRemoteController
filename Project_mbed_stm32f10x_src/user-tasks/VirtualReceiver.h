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

//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//-- CLASS ---------------------------------------------------------------------------
//------------------------------------------------------------------------------------

class VirtualReceiver{
public:
	
	
	/** Constructor, destructor, getter and setter */
	VirtualReceiver(osPriority prio, RawSerial *serial, DigitalOut *endis);
	virtual ~VirtualReceiver();
	Thread *getThread();
	
	/** Serial Interrupt callbacks */
	void RxISRCallback(void);
	void IdleISRCallback(void);
	void TxISRCallback(void);
		
	/** Topic updates callbacks */
	void notifyUpdate(uint32_t event);

	/** Topic updates event enumeration */
	typedef enum {
		GPS_EV_READY 	= (1 << 0),
		KEY_EV_READY 	= (1 << 1),
		TIMER_EV_READY	= (1 << 2),
		VR_EV_DATAREADY = (1 << 3),
		VR_EV_DATAEND	= (1 << 4),
		VR_EV_DATASENT  = (1 << 5)
	}EventEnum;	
		
	/** Task */
	static void task(void const *arg){
		VirtualReceiver *me = (VirtualReceiver*)arg;
		me->run();
	}	

private:

	enum ControlFlag {DISABLE=0, ENABLE=1};

	/** Reception buffer max size */
	static const uint8_t BUFF_SIZE = 128;

	/** Reception buffer structure */
	struct DataBuffer{
		uint8_t count;
		uint8_t data[BUFF_SIZE];	
		bool ovf;
	};

	/** Operational mode flags */
	static const uint8_t INITIALIZING = 0x01;
	static const uint8_t READY		  = 0x02;

	/** Data protocol command types */
	static const uint8_t CMD_SET_GPS 	= 0x01;
	static const uint8_t CMD_SET_KEY 	= 0x02;
	static const uint8_t CMD_DECODING	= 0x03;
	static const uint8_t CMD_ERROR		= 0x04;
	static const uint8_t CMD_READY		= 0x05;
	static const uint8_t CMD_RESET		= 0x06;
	static const uint8_t CMD_ACK		= 0x07;	
	static const uint8_t CMD_NACK		= 0x08;	
	static const uint8_t CMD_DATA		= 0x09;	
		
	
	/** Private variables */
	DataBuffer _rxbuf;
	DataBuffer _txbuf;
	RawSerial *_serial;
	DigitalOut *_endis;
	Thread *_th;
	uint32_t _timeout;
	Topic::AckData_t _ackdata;

	int8_t _status;
	uint8_t _mode;
	int8_t _errcount;
	int32_t _signals;
	RtosTimer * _tcptmr;
	RtosTimer * _rxtmr;
	
	void run();
	int8_t updateStatus(int8_t stat);
	void send();
	void send(const char * atcmd);
	uint8_t processResponse(char * pdata, int * len);

};


#endif /* SRC_ACTIVE_MODULES_VIRTUALRECEIVER_H_ */
