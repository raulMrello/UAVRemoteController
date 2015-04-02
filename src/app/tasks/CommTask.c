/*
 * CommTask.cpp
 *
 *  Created on: 13/3/2015
 *      Author: raulMrello
 */

#include "CommTask.h"
#include "../topics/InputTopics.h"
#include "../topics/DataTopics.h"


static Exception e = Exception_INIT();

//------------------------------------------------------------------------------------
void CommTask_init(CommTaskPtr t){
	printf("CommTask_init\r\n");
}

//------------------------------------------------------------------------------------
void CommTask_OnYieldTurn(CommTaskPtr t){
	printf("CommTask_OnYieldTurn\r\n");
}


//------------------------------------------------------------------------------------
void CommTask_OnResume(CommTaskPtr t){
	printf("CommTask_OnResume\r\n");

}

//------------------------------------------------------------------------------------
void CommTask_OnEventFlag(CommTaskPtr t, int event){
	printf("CommTask_OnEventFlag\r\n");

}

//*-----------------------------------------------------------------------------------
void CommTask_OnTopicUpdate(CommTaskPtr t, TopicData * td){
	printf("CommTask_OnTopicUpdate\r\n");

}

