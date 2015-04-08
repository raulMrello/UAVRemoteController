/*
 * OutputTopics.cpp
 *
 *  Created on: 07/4/2015
 *      Author: raulMrello
 */

#include "OutputTopics.h"
#include <string.h>

//------------------------------------------------------------------------------------
//--  PRIVATE DEFINITIONS  -----------------------------------------------------------
//------------------------------------------------------------------------------------

#define out_OBSIZE		1		///< Observerlist can alloc up to 1 different observers
static Topic outtopic;
static Task* out_oblist[out_OBSIZE];		


//------------------------------------------------------------------------------------
//--  MODULE IMPLEMENTATION  ---------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
Topic * OutputTopic_initialize(const char * name, Exception *e){
	if(strcmp(name, "/out") == 0){
		Topic_initialize(&outtopic, name, (void**)out_oblist, out_OBSIZE, e);
		catch(e){
			return 0;
		}
		return &outtopic;
	}
	else {
		Exception_throw(e, BAD_ARGUMENT, "Unknown topic name");
		return 0;
	}
}

////------------------------------------------------------------------------------------
Topic * OutputTopic_getRef(const char * name, Exception *e){
	if(strcmp(name, "/out") == 0){
		return &outtopic;
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
