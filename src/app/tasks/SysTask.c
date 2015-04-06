/*
 * MySubscriber.cpp
 *
 *  Created on: 12/3/2015
 *      Author: dptoid
 */

#include "SysTask.h"

//------------------------------------------------------------------------------------
//-- PRIVATE MEMBERS -----------------------------------------------------------------
//------------------------------------------------------------------------------------


static Exception e = Exception_INIT();

/** Simulates a system tick timer */
static void simulateSysTick(void);

//------------------------------------------------------------------------------------
//-- IMPLEMENTATION ------------------------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
void SysTask_init(SysTaskPtr t){
	printf("SysTask_init\r\n");
}

//------------------------------------------------------------------------------------
void SysTask_OnYieldTurn(SysTaskPtr t){
	printf("SysTask_OnYieldTurn\r\n");
	simulateSysTick();
	Task_yield((Task*)t, &e);		///< IMPORTANT!!! in order to get control later, else it won't be executed
	catch(&e){
		printf("Exception on SysTask_OnYieldTurn: %s\r\n", e.msg);
		Exception_clear(&e);
	}
}

//------------------------------------------------------------------------------------
void SysTask_OnResume(SysTaskPtr t){
	printf("SysTask_OnResume\r\n");

}

//------------------------------------------------------------------------------------
void SysTask_OnEventFlag(SysTaskPtr t, int event){
	printf("SysTask_OnEventFlag\r\n");
}

//------------------------------------------------------------------------------------
void SysTask_OnTopicUpdate(SysTaskPtr t, TopicData * td){
	printf("SysTask_OnTopicUpdate\r\n");
}

//------------------------------------------------------------------------------------
static void simulateSysTick(void){
	OS_tick(&e);
	catch(&e){
		printf("Exception on simulateSysTick: %s\r\n", e.msg);
		Exception_clear(&e);
	}
}
