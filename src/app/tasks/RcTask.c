/*
 * RcTask.c
 *
 *  Created on: 13/3/2015
 *      Author: raulMrello
 */

#include "RcTask.h"
#include "../topics/InputTopics.h"
#include "../topics/DataTopics.h"

/** Pushbutton abstract definitions */
#define KEY_NONE	(int)0
#define KEY_N		(int)(1 << 0)
#define KEY_NE		(int)(1 << 1)
#define KEY_E		(int)(1 << 2)
#define KEY_ES		(int)(1 << 3)
#define KEY_S		(int)(1 << 4)
#define KEY_SW		(int)(1 << 5)
#define KEY_W		(int)(1 << 6)
#define KEY_WN		(int)(1 << 7)
#define KEY_MODE	(int)(1 << 8)


static int key;
static Exception e = Exception_INIT();
static Topic * pushTopic;
static Topic * releaseTopic;
static Topic * rcTopic;

//------------------------------------------------------------------------------------
static void publishUpdate(RcTaskPtr t, Exception * e);

//------------------------------------------------------------------------------------
void RcTask_init(RcTaskPtr t){
	/** Get topic reference for /push and attach to it */
	pushTopic = InputTopic_getRef("/push", &e);
	catch(&e){
		printf("Exception on RcTask_init e=%s\r\n", e.msg);
		Exception_clear(&e);
	}
	Topic_attach(pushTopic, t, &e);
	catch(&e){
		printf("Exception on RcTask_init e=%s\r\n", e.msg);
		Exception_clear(&e);
	}
	/** Get topic reference for /release and attach to it */
	releaseTopic = InputTopic_getRef("/release", &e);
	catch(&e){
		printf("Exception on RcTask_init e=%s\r\n", e.msg);
		Exception_clear(&e);
	}
	Topic_attach(releaseTopic, t, &e);
	catch(&e){
		printf("Exception on RcTask_init e=%s\r\n", e.msg);
		Exception_clear(&e);
	}
	/** Get topic reference for /rc for future publishing */
	rcTopic =  DataTopic_getRef("/rc", &e);
	catch(&e){
		printf("Exception on RcTask_init e=%s\r\n", e.msg);
		Exception_clear(&e);
	}
	key = KEY_NONE;
}

//------------------------------------------------------------------------------------
void RcTask_OnYieldTurn(RcTaskPtr t){
}


//------------------------------------------------------------------------------------
void RcTask_OnResume(RcTaskPtr t){
	// publish /rc update
	publishUpdate(t, &e);
	catch(&e){
		printf("Exception on RcTask_OnResume e=%s\r\n", e.msg);
		Exception_clear(&e);
	}
}

//------------------------------------------------------------------------------------
void RcTask_OnEventFlag(RcTaskPtr t, int event){
}

//------------------------------------------------------------------------------------
void RcTask_OnTopicUpdate(RcTaskPtr t, TopicData * td){
	if(td->id == (int)pushTopic){
		// get key event
		key |= (int)td->data;
	}
	if(td->id == (int)releaseTopic){
		// release keys event
		key &= ~(int)td->data;
	}
	// publish /rc update
	publishUpdate(t, &e);
	catch(&e){
		printf("Exception on RcTask_OnTopicUpdate e=%s\r\n", e.msg);
		Exception_clear(&e);
	}
}

//------------------------------------------------------------------------------------
/**
 * \brief Publish an /rc topic update. If keys already pressed, enables task suspension.
 * Else, force task resume.
 */
static void publishUpdate(RcTaskPtr t, Exception * e){
	static int lastKey = KEY_NONE;
	// publish /rc update if key changes respect last notified value
	if(key != lastKey){
		lastKey = key;
		Topic_notify(rcTopic, (void*)key, sizeof(int), 0, 0, e);
		catch(e){
			return;

		}
	}
	// if keys pressed, keep waiting for a repeated event in 500ms
	if(key){
		Task_suspend(t, 500000, e);
		catch(e){
			return;

		}
	}
	// else, disable in-course task suspension
	else{
		Task_resume(t, 1, e);
		catch(e){
			return;
		}
	}
}

