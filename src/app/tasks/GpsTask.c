/*
 * GpsTask.cpp
 *
 *  Created on: 13/3/2015
 *      Author: raulMrello
 */

#include "GpsTask.h"
#include "../topics/InputTopics.h"
#include "../topics/DataTopics.h"

//------------------------------------------------------------------------------------
//--  PRIVATE DEFINITIONS  -----------------------------------------------------------
//------------------------------------------------------------------------------------

static Exception e = Exception_INIT();

static void testCases(GpsTaskPtr t);

//------------------------------------------------------------------------------------
//--  MODULE IMPLEMENTATION  ---------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
void GpsTask_init(GpsTaskPtr t){
	printf("GpsTask_init\r\n");
}

//------------------------------------------------------------------------------------
void GpsTask_OnYieldTurn(GpsTaskPtr t){
	printf("GpsTask_OnYieldTurn\r\n");
	testCases(t);
}


//------------------------------------------------------------------------------------
void GpsTask_OnResume(GpsTaskPtr t){
	printf("GpsTask_OnResume\r\n");

}

//------------------------------------------------------------------------------------
void GpsTask_OnEventFlag(GpsTaskPtr t, int event){
	printf("GpsTask_OnEventFlag\r\n");

}

//------------------------------------------------------------------------------------
void GpsTask_OnTopicUpdate(GpsTaskPtr t, TopicData * td){
	printf("GpsTask_OnTopicUpdate\r\n");

}

//------------------------------------------------------------------------------------
static void testCases(GpsTaskPtr t){
	static int count  =0;
	Topic * topic = InputTopic_getRef("/key", &e);
	catch(&e){
		printf("Exception on testCases e=%s\r\n", e.msg);
		Exception_clear(&e);
	}
	switch(count){
		// TC001 - No action if NO_LOC and/or NO_ARMED
		case 0:{
			Topic_notify(topic, (void *)KEY_N, sizeof(int), 0, 0, &e);
			catch(&e){
				printf("Exception on testCases e=%s\r\n", e.msg);
				Exception_clear(&e);
			}
			break;
		}
		// TC002 - Set LOC mode
		case 1:{
			Topic_notify(topic, (void *)(KEY_LOC), sizeof(int), 0, 0, &e);
			catch(&e){
				printf("Exception on testCases e=%s\r\n", e.msg);
				Exception_clear(&e);
			}
			break;
		}
		// TC003 - Change LOC positioning
		case 2:{
			Topic_notify(topic, (void *)(KEY_N|KEY_LOC), sizeof(int), 0, 0, &e);
			catch(&e){
				printf("Exception on testCases e=%s\r\n", e.msg);
				Exception_clear(&e);
			}
			break;
		}
		// TC004 - Disable actual mode
		case 3:{
			Topic_notify(topic, (void *)(KEY_NONE), sizeof(int), 0, 0, &e);
			catch(&e){
				printf("Exception on testCases e=%s\r\n", e.msg);
				Exception_clear(&e);
			}
			break;
		}
		// TC005 - Don't allow actions on disabled state
		case 4:{
			Topic_notify(topic, (void *)(KEY_N), sizeof(int), 0, 0, &e);
			catch(&e){
				printf("Exception on testCases e=%s\r\n", e.msg);
				Exception_clear(&e);
			}
			break;
		}
		// TC006 - Set ARMED mode
		case 5:{
			Topic_notify(topic, (void *)(KEY_ARM), sizeof(int), 0, 0, &e);
			catch(&e){
				printf("Exception on testCases e=%s\r\n", e.msg);
				Exception_clear(&e);
			}
			break;
		}
		// TC007 - Increase throttle
		case 6:
		case 7:
		case 8:
		case 9:{
			Topic_notify(topic, (void *)(KEY_N), sizeof(int), 0, 0, &e);
			catch(&e){
				printf("Exception on testCases e=%s\r\n", e.msg);
				Exception_clear(&e);
			}
			break;
		}
	}
	count++;
	Task_yield(t, &e);
	catch(&e){
		printf("Exception on testCases e=%s\r\n", e.msg);
		Exception_clear(&e);
	}
}
