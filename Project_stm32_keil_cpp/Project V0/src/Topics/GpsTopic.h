/*
 * GpsTopic.h
 *
 *  Created on: 14/04/2015
 *      Author: raulMrello
 *	
 *	GpsTopics are those topics published by a GpsReader. This kind of topics contain information about
 *	positioning (latitude, longitude, altitude, heading). Optionally can contain other information like (signal
 *	strength, number of satellites, etc...)
 */

#ifndef SRC_TOPICS_GPSTOPIC_H_
#define SRC_TOPICS_GPSTOPIC_H_

#include "mmf.h"
using namespace MMF;
class GpsTopic {
public:
	
	/** Gps topic data structure */
	typedef struct {
		float lat;	///< latitude -100º to +100º
		float lng;	///< longitude -12h to +12h
		float alt;	///< altitude -100m to 8000m
		float hea;	///< heading 0º to 360º
	}Data_t;

	/** Constructor */
	GpsTopic (const char * name = "");

	/** static Publish function */
	static void publish(void * data, int datasize,  TopicConsumedCallback done, TopicConsumedHandlerObj publisher) throw (Exception){
		if(datasize != sizeof(Data_t)){
			throw Exception(Exception::BAD_ARGUMENT, "GpsTopic::publish datasize inconsistent");
		}
		_topic->notify(data, datasize, done, publisher);
	}
	
	/** static getRef function */
	static int getRef(){
		return _topic->getId();
	}
	
	/** static getName function */
	static const char * getName(){
		return _topic->getName();
	}
	
	/** static attach function */
	static void attach(TopicSubscribedHandlerObj handler){
		_topic->attach(handler);
	}

private:
	static Topic* _topic;
};

#endif /* SRC_TOPICS_MYTOPIC_H_ */
