/*
 * TimeTopics.c
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 */

#include "TimeTopics.h"
#include <string.h>


#define time_OBSIZE		1		///< Observerlist can alloc up to 2 different observers
static Topic timetopic;
static Task* time_oblist[time_OBSIZE];

//------------------------------------------------------------------------------------
Topic * TimeTopic_initialize(const char * name, Exception *e){
	if(strcmp(name, "/time") == 0){
		Topic_initialize(&timetopic, name, (void**)time_oblist, time_OBSIZE, e);
		catch(e){
			return 0;
		}
		return &timetopic;
	}
	else {
		Exception_throw(e, BAD_ARGUMENT, "Unknown topic name");
		return 0;
	}
}

//------------------------------------------------------------------------------------
Topic * TimeTopic_getRef(const char * name, Exception *e){
	if(strcmp(name, "/time") == 0){
		return &timetopic;
	}
	else {
		Exception_throw(e, BAD_ARGUMENT, "Unknown topic name");
		return 0;
	}
}

////------------------------------------------------------------------------------------
//int MyTopic_getId(void){
//	return topic.id;
//}
//
////------------------------------------------------------------------------------------
//const char * MyTopic_getName(void){
//	return topic.name;
//}
