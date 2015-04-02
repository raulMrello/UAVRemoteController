/*
 * DataTopics.c
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 */

#include "DataTopics.h"
#include <string.h>

//------------------------------------------------------------------------------------
//--  PRIVATE DEFINITIONS  -----------------------------------------------------------
//------------------------------------------------------------------------------------

static Topic rctopic;
static Topic gpstopic;
static Task * rc_oblist[1];
static Task * gps_oblist[1];	///< Observerlist can alloc up to 1 different observers


//------------------------------------------------------------------------------------
//--  MODULE IMPLEMENTATION  ---------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
Topic * DataTopic_initialize(const char * name, Exception *e){
	if(strcmp(name, "/rc") == 0){
		Topic_initialize(&rctopic, name, (void**)rc_oblist, 1, e);
		catch(e){
			return 0;
		}
		return &rctopic;
	}
	else if(strcmp(name, "/gps") == 0){
		Topic_initialize(&gpstopic, name, (void**)gps_oblist, 1, e);
		catch(e){
			return 0;
		}
		return &gpstopic;
	}
	else {
		Exception_throw(e, BAD_ARGUMENT, "Unknown topic name");
		return 0;
	}
}

//------------------------------------------------------------------------------------
Topic * DataTopic_getRef(const char * name, Exception *e){
	if(strcmp(name, "/rc") == 0){
		return &rctopic;
	}
	else if(strcmp(name, "/gps") == 0){
		return &gpstopic;
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
