/*
 * InputTopics.cpp
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 */

#include "InputTopics.h"
#include <string.h>

//------------------------------------------------------------------------------------
//--  PRIVATE DEFINITIONS  -----------------------------------------------------------
//------------------------------------------------------------------------------------

static Topic keytopic;
static Task* key_oblist[1];		///< Observerlist can alloc up to 2 different observers


//------------------------------------------------------------------------------------
//--  MODULE IMPLEMENTATION  ---------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
Topic * MyTopic_initialize(const char * name, Exception *e){
	if(strcmp(name, "/key") == 0){
		Topic_initialize(&keytopic, name, (void**)key_oblist, 1, e);
		catch(e){
			return 0;
		}
		return &keytopic;
	}
	else {
		Exception_throw(e, BAD_ARGUMENT, "Unknown topic name");
		return 0;
	}
}

////------------------------------------------------------------------------------------
Topic * MyTopic_getRef(const char * name, Exception *e){
	if(strcmp(name, "/key") == 0){
		return &keytopic;
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
