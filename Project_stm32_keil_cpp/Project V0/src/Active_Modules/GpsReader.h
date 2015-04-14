/*
 * GpsReader.h
 *
 *  Created on: 15/4/2015
 *      Author: raulMrello
 *
 *  GpsReader is an Active module (running as a Task) that will read NMEA/UBX data
 *	streams from a serial GPS module. Each time data is received and analysed, a topic
 *	data structure (type "/gps") is updated and published.
 */

#ifndef SRC_ACTIVE_MODULES_GPSREADER_H_
#define SRC_ACTIVE_MODULES_GPSREADER_H_


//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include <stdint.h>
#include "drv_UART.h"
#include "mmf.h"
using namespace MMF;

//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
//-- CLASS ---------------------------------------------------------------------------
//------------------------------------------------------------------------------------

class GpsReader : public Task {
public:
	/** Gps mode reading enumeration */
	typedef enum {
		GPS_MODE_NMEA = (1 << 0),
		GPS_MODE_UBX  = (1 << 1)
	}ModeEnum;
	
		/** Gps event enumeration */
	typedef enum {
		GPS_EV_DATAREADY = (1 << 0),
		GPS_EV_DATASENT  = (1 << 1)
	}EventEnum;

	/** Gps common flags */
	typedef uint32_t GpsReaderFlags;

	GpsReader(const char * name, uint8_t prio, GpsReaderFlags mode) : Task(name, prio, 0) {
		_mode = mode;
	}
	virtual ~GpsReader();
private:
	GpsReaderFlags _mode;
	drv_UART _uart;
	/** Task interface */
	void init();
	void onYieldTurn();
	void onResume();
	void onEventFlag(uint16_t event);
	void onTopicUpdate(TopicData * topicdata);
};


#endif /* SRC_ACTIVE_MODULES_GPSREADER_H_ */
