/*
 * GpsTopic.cpp
 *
 *  Created on: 14/04/2015
 *      Author: raulMrello
 */

#include "GpsTopic.h"

Topic* GpsTopic::_topic = 0;

GpsTopic::GpsTopic (const char * name){
	_topic = new Topic(name);
}
