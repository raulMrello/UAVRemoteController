/*
 * RcTopics.c
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 */

#include "RcTopics.h"
#include <string.h>

//------------------------------------------------------------------------------------
//--  PRIVATE DEFINITIONS  -----------------------------------------------------------
//------------------------------------------------------------------------------------

#define rc_OBSIZE		1		///< Observerlist can alloc up to 1 different observers
static Topic rctopic;
static Task * rc_oblist[rc_OBSIZE];


//------------------------------------------------------------------------------------
//--  MODULE IMPLEMENTATION  ---------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
Topic * RcTopic_initialize(const char * name, Exception *e){
	if(strcmp(name, "/rc") == 0){
		Topic_initialize(&rctopic, name, (void**)rc_oblist, rc_OBSIZE, e);
		catch(e){
			return 0;
		}
		return &rctopic;
	}
	else {
		Exception_throw(e, BAD_ARGUMENT, "Unknown topic name");
		return 0;
	}
}

//------------------------------------------------------------------------------------
Topic * RcTopic_getRef(const char * name, Exception *e){
	if(strcmp(name, "/rc") == 0){
		return &rctopic;
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
