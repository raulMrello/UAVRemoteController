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

#define GPS_TIMEOUT		2000	///< If GPS doesn't emit data in 2sec, then error

//------------------------------------------------------------------------------------
//-- STATIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

GpsReader::GpsReader(osPriority prio, GpsReader::ModeEnum mode, RawSerial *serial) {
	_mode = (uint32_t)mode;
	_serial = serial;
	_serial->baud(38400);
	ublox.Init();
	nmea.Init();
	_th = 0;
	_th = new Thread(&GpsReader::task, this, prio);
}

//------------------------------------------------------------------------------------
GpsReader::~GpsReader() {
	// TODO Auto-generated destructor stub
}

//------------------------------------------------------------------------------------
Thread * GpsReader::getThread() {
	return _th;
}

//------------------------------------------------------------------------------------
void GpsReader::RxISRCallback(void){
	_th->signal_set(GPS_EV_DATAREADY);
}

//------------------------------------------------------------------------------------
void GpsReader::TxISRCallback(void){
	_th->signal_set(GPS_EV_DATASENT);
}

//------------------------------------------------------------------------------------
void GpsReader::run(){
	while(_th == 0){
		Thread::wait(100);
	}
	// Attaches to installed serial peripheral
	_serial->attach(this, &GpsReader::RxISRCallback, (SerialBase::IrqType)RxIrq);
	_serial->attach(this, &GpsReader::TxISRCallback, (SerialBase::IrqType)TxIrq);
	
	// Starts execution 
	for(;;){
		// Wait incoming data forever ... 
		osEvent oe = _th->signal_wait(GPS_EV_DATAREADY, GPS_TIMEOUT);
		// on timeout notify gps error
		if(oe.status == osEventTimeout){
			MsgBroker::Exception e = MsgBroker::NO_ERRORS;
			_gpsdata.timeout = 1;
			MsgBroker::publish("/gps", &_gpsdata, sizeof(Topic::GpsData_t), &e);
			if(e != MsgBroker::NO_ERRORS){
				// TODO: add error handling ...
			}
			continue;
		}
		_th->signal_clr(GPS_EV_DATAREADY);
		while(_serial->readable()){
			uint8_t data;
			// reads data and pass to gps processor
			data = (uint8_t) _serial->getc();
			if(ublox.Read(data) == GPS_UBLOX::OK){
				MsgBroker::Exception e = MsgBroker::NO_ERRORS;
				_gpsdata.time = (float)ublox.Time;
				_gpsdata.lat = (float)ublox.Lattitude;
				_gpsdata.lng = (float)ublox.Longitude;
				_gpsdata.alt = (float)ublox.Altitude;
				_gpsdata.gcourse = (float)ublox.Ground_Course;
				_gpsdata.gspeed = (float)ublox.Ground_Speed;
				_gpsdata.speed3d = (float)ublox.Speed_3d;
				_gpsdata.sats = (float)ublox.NumSats;
				_gpsdata.fix = (float)ublox.Fix;
				_gpsdata.timeout = 0;
				MsgBroker::publish("/gps", &_gpsdata, sizeof(Topic::GpsData_t), &e);
				if(e != MsgBroker::NO_ERRORS){
					// TODO: add error handling ...
				}
				continue;
			}
			if(nmea.Read(data) == GPS_NMEA::OK){
				MsgBroker::Exception e = MsgBroker::NO_ERRORS;
				_gpsdata.time = (float)nmea.Time;
				_gpsdata.lat = (float)nmea.Lattitude;
				_gpsdata.lng = (float)nmea.Longitude;
				_gpsdata.alt = (float)nmea.Altitude;
				_gpsdata.gcourse = (float)nmea.Ground_Course;
				_gpsdata.gspeed = (float)nmea.Ground_Speed;
				_gpsdata.speed3d = (float)nmea.Speed_3d;
				_gpsdata.sats = (float)nmea.NumSats;
				_gpsdata.fix = (float)nmea.Fix;
				_gpsdata.timeout = 0;
				MsgBroker::publish("/gps", &_gpsdata, sizeof(Topic::GpsData_t), &e);
				if(e != MsgBroker::NO_ERRORS){
					// TODO: add error handling ...
				}
			}
		}
	}	
}

//------------------------------------------------------------------------------------
//-- PROTECTED/PRIVATE FUNCTIONS -----------------------------------------------------
//------------------------------------------------------------------------------------


