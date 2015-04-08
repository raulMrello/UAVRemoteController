/*
 * UartTopics.c
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 */

#include "SystemTopics.h"
#include <string.h>

#define sys_OBSIZE	2		///< Observerlist can alloc up to 2 different observers
static Topic systopic;
static Task* sys_oblist[sys_OBSIZE];	

//------------------------------------------------------------------------------------
Topic * SystemTopic_initialize(const char * name, Exception *e){
	if(strcmp(name, "/sys") == 0){
		Topic_initialize(&systopic, name, (void**)sys_oblist, sys_OBSIZE, e);
		catch(e){
			return 0;
		}
		return &systopic;
	}
	else {
		Exception_throw(e, BAD_ARGUMENT, "Unknown topic name");
		return 0;
	}
}

//------------------------------------------------------------------------------------
Topic * SystemTopic_getRef(const char * name, Exception *e){
	if(strcmp(name, "/sys") == 0){
		return &systopic;
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
