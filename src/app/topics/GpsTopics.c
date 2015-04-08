/*
 * GpsTopics.c
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 */

#include "GpsTopics.h"
#include <string.h>

#define nmea_OBSIZE		2		///< Observerlist can alloc up to 2 different observers
static Topic nmeatopic;
static Task* nmea_oblist[nmea_OBSIZE];

#define ubx_OBSIZE		2		///< Observerlist can alloc up to 2 different observers
static Topic ubxtopic;
static Task* ubx_oblist[ubx_OBSIZE];

//------------------------------------------------------------------------------------
Topic * GpsTopic_initialize(const char * name, Exception *e){
	if(strcmp(name, "/nmea") == 0){
		Topic_initialize(&nmeatopic, name, (void**)nmea_oblist, nmea_OBSIZE, e);
		catch(e){
			return 0;
		}
		return &nmeatopic;
	}
	else if(strcmp(name, "/ubx") == 0){
		Topic_initialize(&ubxtopic, name, (void**)ubx_oblist, ubx_OBSIZE, e);
		catch(e){
			return 0;
		}
		return &ubxtopic;
	}
	else {
		Exception_throw(e, BAD_ARGUMENT, "Unknown topic name");
		return 0;
	}
}

//------------------------------------------------------------------------------------
Topic * GpsTopic_getRef(const char * name, Exception *e){
	if(strcmp(name, "/nmea") == 0){
		return &nmeatopic;
	}
	else if(strcmp(name, "/ubx") == 0){
		return &ubxtopic;
	}
	else {
		Exception_throw(e, BAD_ARGUMENT, "Unknown topic name");
		return 0;
	}
}

////------------------------------------------------------------------------------------
//int GpsTopic_getId(void){
//	return topic.id;
//}
//
////------------------------------------------------------------------------------------
//const char * GpsTopic_getName(void){
//	return topic.name;
//}
