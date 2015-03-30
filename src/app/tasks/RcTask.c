/*
 * RcTask.c
 *
 *  Created on: 13/3/2015
 *      Author: raulMrello
 */

#include "RcTask.h"
#include "../topics/InputTopics.h"
#include "../topics/DataTopics.h"

#define KEY_L_UP		(int)(1 << 0)
#define KEY_L_RIGHT		(int)(1 << 0)
#define KEY_L_DOWN		(int)(1 << 0)
#define KEY_L_LEFT		(int)(1 << 0)

static int key;
static Exception e = Exception_INIT();

//------------------------------------------------------------------------------------
void RcTask_init(RcTaskPtr t){
	Topic_attach(InputTopic_getRef("/push", &e), t, &e);
	catch(&e){
		printf("Exception on RcTask_init e=%s\r\n", e.msg);
		Exception_clear(&e);
	}
	Topic_attach(InputTopic_getRef("/release", &e), t, &e);
	catch(&e){
		printf("Exception on RcTask_init e=%s\r\n", e.msg);
		Exception_clear(&e);
	}
	key = KEY_NONE;
}

//------------------------------------------------------------------------------------
void RcTask_OnYieldTurn(RcTaskPtr t){
	printf("Publisher_OnYieldTurn\r\n");
	counter++;

	switch(counter){
		// in this case sends a topic update
		case 1:{
			Topic_notify(MyTopic_getRef(), &counter, sizeof(int), &e);
			catch(&e){
				printf("Exception on Publisher_OnYieldTurn: %s\r\n", e.msg);
				Exception_clear(&e);
			}
			break;
		}
		// in this case sends event 1
		case 2:{
			OS_send_event(0, "subscriber", 1, &e);
			catch(&e){
				printf("Exception on Publisher_OnYieldTurn: %s\r\n", e.msg);
				Exception_clear(&e);
			}
			break;
		}
		// in this case sends event 2
		case 3:{
			OS_send_event(0, "subscriber", 2, &e);
			catch(&e){
				printf("Exception on Publisher_OnYieldTurn: %s\r\n", e.msg);
				Exception_clear(&e);
			}
			break;
		}
		// in this case sends event 8
		case 4:{
			OS_send_event(0, "subscriber", 8, &e);
			catch(&e){
				printf("Exception on Publisher_OnYieldTurn: %s\r\n", e.msg);
				Exception_clear(&e);
			}
			break;
		}
		// in this case sends event 16
		case 5:{
			OS_send_event(0, "subscriber", 16, &e);
			catch(&e){
				printf("Exception on Publisher_OnYieldTurn: %s\r\n", e.msg);
				Exception_clear(&e);
			}
			break;
		}
		// in this case sends event 1 and post topic update
		case 6:{
			OS_send_event(0, "subscriber", 1, &e);
			catch(&e){
				printf("Exception on Publisher_OnYieldTurn: %s\r\n", e.msg);
				Exception_clear(&e);
			}
			Topic_notify(MyTopic_getRef(), &counter, sizeof(int), &e);
			catch(&e){
				printf("Exception on Publisher_OnYieldTurn: %s\r\n", e.msg);
				Exception_clear(&e);
			}
			break;
		}
		// in this case sends an event to an unknown task
		case 7:{
			OS_send_event(0, "subscriBER", 1, &e);
			catch(&e){
				printf("Exception on Publisher_OnYieldTurn: %s\r\n", e.msg);
				Exception_clear(&e);
			}
			break;
		}
		// in this case colapses susbscriber topic pool
		case 8:{
			int j;
			for(j=0;j<10;j++){
				Topic_notify(MyTopic_getRef(), &counter, sizeof(int), &e);
				catch(&e){
					printf("Exception on Publisher_OnYieldTurn: %s\r\n", e.msg);
					Exception_clear(&e);
					break;
				}
			}
			break;
		}
	}

	// keeps task running
	Task_yield((Task*)t, &e);	///< IMPORTANT!!! in order to get control later, else it won't be executed
	catch(&e){
		printf("Exception on Publisher_OnYieldTurn: %s\r\n", e.msg);
		Exception_clear(&e);
	}
}


//------------------------------------------------------------------------------------
void RcTask_OnResume(RcTaskPtr t){
	printf("Publisher_OnResume\r\n");

}

//------------------------------------------------------------------------------------
void RcTask_OnEventFlag(RcTaskPtr t, int event){
	printf("Publisher_OnEventFlag\r\n");

}

//------------------------------------------------------------------------------------
void RcTask_OnTopicUpdate(RcTaskPtr t, TopicData * td){
	printf("Publisher_OnTopicUpdate\r\n");

}

