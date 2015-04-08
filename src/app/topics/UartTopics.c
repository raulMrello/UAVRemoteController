/*
 * UartTopics.c
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 */

#include "UartTopics.h"
#include <string.h>

#define uart1_OBSIZE		1		///< Observerlist can alloc up to 1 different observers
static Topic uart1topic;
static Task* uart1_oblist[uart1_OBSIZE];

#define uart2_OBSIZE		1		///< Observerlist can alloc up to 1 different observers
static Topic uart2topic;
static Task* uart2_oblist[uart2_OBSIZE];

//------------------------------------------------------------------------------------
Topic * UartTopic_initialize(const char * name, Exception *e){
	if(strcmp(name, "/uart1") == 0){
		Topic_initialize(&uart1topic, name, (void**)uart1_oblist, uart1_OBSIZE, e);
		catch(e){
			return 0;
		}
		return &uart1topic;
	}
	else if(strcmp(name, "/uart2") == 0){
		Topic_initialize(&uart2topic, name, (void**)uart2_oblist, uart2_OBSIZE, e);
		catch(e){
			return 0;
		}
		return &uart2topic;
	}
	else {
		Exception_throw(e, BAD_ARGUMENT, "Unknown topic name");
		return 0;
	}
}

//------------------------------------------------------------------------------------
Topic * UartTopic_getRef(const char * name, Exception *e){
	if(strcmp(name, "/uart1") == 0){
		return &uart1topic;
	}
	else if(strcmp(name, "/uart2") == 0){
		return &uart2topic;
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
