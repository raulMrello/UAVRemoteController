/*
 * UartTopics.c
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 */

#include "UartTopics.h"
#include <string.h>

static Topic nmeatopic, ubxtopic, comtopic;
static Task* nmea_oblist[1], ubx_oblist[1], com_oblist[1];	///< Observerlist can alloc up to 2 different observers

//------------------------------------------------------------------------------------
Topic * UartTopic_initialize(const char * name, Exception *e){
	if(strcmp(name, "/nmea") == 0){
		Topic_initialize(&nmeatopic, name, (void**)nmea_oblist, 1, e);
		catch(e){
			return 0;
		}
		return &nmeatopic;
	}
	else if(strcmp(name, "/ubx") == 0){
		Topic_initialize(&ubxtopic, name, (void**)ubx_oblist, 1, e);
		catch(e){
			return 0;
		}
		return &ubxtopic;
	}
	else if(strcmp(name, "/com") == 0){
		Topic_initialize(&comtopic, name, (void**)com_oblist, 1, e);
		catch(e){
			return 0;
		}
		return &comtopic;
	}
	else {
		Exception_throw(e, BAD_ARGUMENT, "Unknown topic name");
		return 0;
	}
}

////------------------------------------------------------------------------------------
//Topic * MyTopic_getRef(void){
//	return &topic;
//}
//
////------------------------------------------------------------------------------------
//int MyTopic_getId(void){
//	return topic.id;
//}
//
////------------------------------------------------------------------------------------
//const char * MyTopic_getName(void){
//	return topic.name;
//}
