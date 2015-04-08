/*
 * UartTopics.c
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 */

#include "UartTopics.h"
#include <string.h>

#define uart_OBSIZE		2		///< Observerlist can alloc up to 2 different observers
static Topic uarttopic;
static Task* uart_oblist[uart_OBSIZE];

//------------------------------------------------------------------------------------
Topic * UartTopic_initialize(const char * name, Exception *e){
	if(strcmp(name, "/uart") == 0){
		Topic_initialize(&uarttopic, name, (void**)uart_oblist, uart_OBSIZE, e);
		catch(e){
			return 0;
		}
		return &uarttopic;
	}
	else {
		Exception_throw(e, BAD_ARGUMENT, "Unknown topic name");
		return 0;
	}
}

//------------------------------------------------------------------------------------
Topic * UartTopic_getRef(const char * name, Exception *e){
	if(strcmp(name, "/uart") == 0){
		return &uarttopic;
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
