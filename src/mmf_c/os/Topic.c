/*
 * Topic.cpp
 *
 *  Created on: 12/3/2015
 *      Author: dptoid
 */

#include "Topic.h"
#include "Task.h"

//------------------------------------------------------------------------------------
//-- REQUIRED MMF DEPENDENCIES -------------------------------------------------------
//------------------------------------------------------------------------------------

extern void Task_setReady(Task* t, int evt, Exception *e);
extern void Task_addTopic(Task*t, int id, const char* name, void *data, int datasize, Exception *e);

//------------------------------------------------------------------------------------
//-- PRIVATE MEMBERS -----------------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef true
#define true (char)1
#endif

#ifndef false
#define false (char)0
#endif

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
void Topic_initialize(Topic* topic, const char * name, void ** oblist, int listsize, Exception *e){
	int i;
	if(!topic){
		Exception_throw(e, BAD_ARGUMENT, "Topic_initialize topic is null");
		return;
	}
	topic->id = (int)topic;
	if(!name)
		topic->name = "";
	else
		topic->name = name;
	topic->observerlist = oblist;
	topic->listsize = listsize;
	for(i = 0; i < listsize; i++){
		topic->observerlist[i] = (void*)0;
	}
	topic->data = 0;
	topic->datasize = 0;
}

//------------------------------------------------------------------------------------
void Topic_notify(Topic* topic, void * data, int datasize, Exception *e){
	int i;
	if(!topic){
		Exception_throw(e, BAD_ARGUMENT, "Topic_notify topic is null");
		return;
	}
	topic->data = data;
	topic->datasize = datasize;
	for(i = 0; i < topic->listsize; i++){
		if(topic->observerlist[i]){
			Task* t = (Task*)topic->observerlist[i];
			// inserts into the topic pool
			Task_addTopic(t, topic->id, topic->name, topic->data, topic->datasize, e);
			catch(e){
				return;
			}
		}
	}
}

//------------------------------------------------------------------------------------
void Topic_attach(Topic* topic, void * observer, Exception *e){
	int i;
	if(!topic){
		Exception_throw(e, BAD_ARGUMENT, "Topic_attach topic is null");
		return;
	}
	// if already attached, exit
	for(i = 0; i < topic->listsize; i++){
		if(topic->observerlist[i] == observer){
			return;
		}
	}
	// else attach
	for(i = 0; i < topic->listsize; i++){
		if(topic->observerlist[i] == 0){
			topic->observerlist[i] = observer;
			return;
		}
	}
	// if list full, throw exception
	Exception_throw(e, MEMORY_ALLOC, "Topic_attach list is full");
}

//------------------------------------------------------------------------------------
void Topic_dettach(Topic* topic, void * observer, Exception *e){
	int i;
	if(!topic){
		Exception_throw(e, BAD_ARGUMENT, "Topic_dettach topic is null");
		return;
	}
	for(i = 0; i < topic->listsize; i++){
		if(topic->observerlist[i] == observer){
			topic->observerlist[i] = 0;
			return;
		}
	}
	// if not in list, throw exception
	Exception_throw(e, MEMORY_DEALLOC, "Topic_dettach topic not in list");
}
