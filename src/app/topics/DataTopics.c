/*
 * MyTopic.cpp
 *
 *  Created on: 12/3/2015
 *      Author: dptoid
 */

#include "MyTopic.h"

static Topic topic;
static Task* observerlist[2];				///< Observerlist can alloc up to 2 different observers
static Exception e = Exception_INIT();		///< Module's exception handling

//------------------------------------------------------------------------------------
Topic * MyTopic_initialize(const char * name){
	Topic_initialize(&topic, name, (void**)observerlist, 2, &e);
	catch(&e){
		Exception_clear(&e);
		return 0;
	}
	return &topic;
}

//------------------------------------------------------------------------------------
Topic * MyTopic_getRef(void){
	return &topic;
}

//------------------------------------------------------------------------------------
int MyTopic_getId(void){
	return topic.id;
}

//------------------------------------------------------------------------------------
const char * MyTopic_getName(void){
	return topic.name;
}
