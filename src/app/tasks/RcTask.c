/*
 * RcTask.c
 *
 *  Created on: 13/3/2015
 *      Author: raulMrello
 */

#include "RcTask.h"
#include "../topics/InputTopics.h"
#include "../topics/DataTopics.h"

//------------------------------------------------------------------------------------
//--  PRIVATE DEFINITIONS  -----------------------------------------------------------
//------------------------------------------------------------------------------------

static KEY_TOPIC_DATA_T inp;
static RC_TOPIC_DATA_T rc;
static Exception e = Exception_INIT();
static Topic * keyTopic;
static Topic * rcTopic;

static void updateManualControls(void);


//------------------------------------------------------------------------------------
//--  MODULE IMPLEMENTATION  ---------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
void RcTask_init(RcTaskPtr t){
	/** Get topic reference for /key and attach to it */
	keyTopic = InputTopic_getRef("/key", &e);
	catch(&e){
		printf("Exception on RcTask_init e=%s\r\n", e.msg);
		Exception_clear(&e);
	}
	Topic_attach(keyTopic, t, &e);
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
	// sets default value for topic handlers
	inp = (KEY_TOPIC_DATA_T){KEY_NONE};
	rc = (RC_TOPIC_DATA_T){0,0,0,0,0,0};
}

//------------------------------------------------------------------------------------
void RcTask_OnYieldTurn(RcTaskPtr t){
}


//------------------------------------------------------------------------------------
void RcTask_OnResume(RcTaskPtr t){
	// updates manual controls
	rc.mode = RC_MODE_MANUAL_ARMED;
	rc.loc_keys = 0;
	updateManualControls();
	// publishes topic update
	Topic_notify(rcTopic, (void*)&rc, sizeof(RC_TOPIC_DATA_T), 0, 0, &e);
	catch(&e){
		printf("Exception on RcTask_OnResume e=%s\r\n", e.msg);
		Exception_clear(&e);
	}
	Task_suspend(t, 1000000, &e);
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
	// topic checking
	if(td->id == (int)keyTopic){
		char enableSuspension = 0;
		// finalizes any previous suspension in course
		Task_resume(t, 1, &e);
		catch(&e){
			printf("Exception on RcTask_OnResume e=%s\r\n", e.msg);
			Exception_clear(&e);
		}
		// get new key event
		inp.keys = (int)td->data;
		// if mode LOC then saves new key events and publish /rc topic
		if((inp.keys & KEY_LOC) != KEY_RELEASED){
			rc.mode = RC_MODE_LOC;
			rc.loc_keys = (inp.keys & (KEY_N|KEY_NE|KEY_E|KEY_ES|KEY_S|KEY_SW|KEY_W|KEY_WN));
		}
		// if mode MANUAL: then check if ARMED then enables task suspension and publish /rc topic
		else if((inp.keys & KEY_ARM) != KEY_RELEASED){
			// checks priority modes RTH (then) ALT (then) ARMED
			if((inp.keys & KEY_RTH) != KEY_RELEASED){
				rc.mode = RC_MODE_MANUAL_ARMED_RTH;
			}
			else if((inp.keys & KEY_ALT) != KEY_RELEASED){
				rc.mode = RC_MODE_MANUAL_ARMED_ALT;
			}
			else{
				rc.mode = RC_MODE_MANUAL_ARMED;
			}
			rc.loc_keys = 0;
			updateManualControls();
			enableSuspension = 1;
		}
		// if mode MANUAL & DISARMED, idem
		else{
			rc.mode = RC_MODE_MANUAL_DISARMED;
			rc.loc_keys = 0;
			rc.throttle = 0;
			rc.pitch = 0;
			rc.roll = 0;
			rc.yaw = 0;
		}

		// publishes topic update
		Topic_notify(rcTopic, (void*)&rc, sizeof(RC_TOPIC_DATA_T), 0, 0, &e);
		catch(&e){
			printf("Exception on RcTask_OnTopicUpdate e=%s\r\n", e.msg);
			Exception_clear(&e);
		}

		// if must suspend to reactivate in a period of time, then do it
		if(enableSuspension){
			Task_suspend(t, 1000000, &e);
			catch(&e){
				printf("Exception on RcTask_OnTopicUpdate e=%s\r\n", e.msg);
				Exception_clear(&e);
			}
		}
	}
}

//------------------------------------------------------------------------------------
/**
 * \brief Updates manual controls (throttle, roll, pitch, yaw) according with their last
 * state and the new one, keeping them into their ranges.
 */
static void updateManualControls(void){
	// throttle will inc/dec with keys N/S in the range 1000/2000
	if((inp.keys & KEY_S) != KEY_RELEASED){
		rc.throttle = (rc.throttle < 1000)? 1000 : rc.throttle;
		rc.throttle -= (rc.throttle >= 1100)? 100 : 0;
	}
	if((inp.keys & KEY_N) != KEY_RELEASED){
		rc.throttle = (rc.throttle < 1000)? 1000 : rc.throttle;
		rc.throttle += (rc.throttle <= 1900)? 100 : 0;
	}

	// pitch will inc/dec with keys NE/WN in the range 1000/2000
	if((inp.keys & KEY_WN) != KEY_RELEASED){
		rc.pitch = (rc.pitch < 1000)? 1000 : rc.pitch;
		rc.pitch -= (rc.pitch >= 1100)? 100 : 0;
	}
	if((inp.keys & KEY_NE) != KEY_RELEASED){
		rc.pitch = (rc.pitch < 1000)? 1000 : rc.pitch;
		rc.pitch += (rc.pitch <= 1900)? 100 : 0;
	}

	// roll will inc/dec with keys E/W in the range 1000/2000
	if((inp.keys & KEY_W) != KEY_RELEASED){
		rc.roll = (rc.roll < 1000)? 1000 : rc.roll;
		rc.roll -= (rc.roll >= 1100)? 100 : 0;
	}
	if((inp.keys & KEY_E) != KEY_RELEASED){
		rc.roll = (rc.roll < 1000)? 1000 : rc.roll;
		rc.roll += (rc.roll <= 1900)? 100 : 0;
	}

	// yaw will inc/dec with keys ES/SW in the range 1000/2000
	if((inp.keys & KEY_SW) != KEY_RELEASED){
		rc.yaw = (rc.yaw < 1000)? 1000 : rc.yaw;
		rc.yaw -= (rc.yaw >= 1100)? 100 : 0;
	}
	if((inp.keys & KEY_ES) != KEY_RELEASED){
		rc.yaw = (rc.yaw < 1000)? 1000 : rc.yaw;
		rc.yaw += (rc.yaw <= 1900)? 100 : 0;
	}
}

