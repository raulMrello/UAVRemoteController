/*
 * InputTopics.cpp
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 */

#include "InputTopics.h"
#include <string.h>
static Topic pushtopic, releasetopic;
static Task* push_oblist[1], release_oblist[1];		///< Observerlist can alloc up to 2 different observers

//------------------------------------------------------------------------------------
Topic * MyTopic_initialize(const char * name, Exception *e){
	if(strcmp(name, "/push") == 0){
		Topic_initialize(&pushtopic, name, (void**)push_oblist, 1, e);
		catch(e){
			return 0;
		}
		return &pushtopic;
	}
	else if(strcmp(name, "/release") == 0){
		Topic_initialize(&releasetopic, name, (void**)release_oblist, 1, e);
		catch(e){
			return 0;
		}
		return &releasetopic;
	}
	else {
		Exception_throw(e, BAD_ARGUMENT, "Unknown topic name");
		return 0;
	}
}

////------------------------------------------------------------------------------------
Topic * MyTopic_getRef(const char * name, Exception *e){
	if(strcmp(name, "/push") == 0){
		return &pushtopic;
	}
	else if(strcmp(name, "/release") == 0){
		return &releasetopic;
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
