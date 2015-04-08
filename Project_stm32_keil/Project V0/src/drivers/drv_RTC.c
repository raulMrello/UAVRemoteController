/*
 * drv_RTC.c
 *
 *  Created on: 07/04/2015
 *      Author: raulMrello
 *
 */
 
#include "drv_RTC.h"

//------------------------------------------------------------------------------------
//-- PRIVATE DEFINITIONS -------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//-- PRIVATE MEMBERS -----------------------------------------------------------------
//------------------------------------------------------------------------------------

static SYS_TOPIC_DATA_T sysTopicData;
static TIME_TOPIC_DATA_T timeTopicData;
static Topic * sysTopic;
static Topic * timeTopic;

static void setTime(int time);
static int getTime(void);
static void endisAlarm(int endis);

//------------------------------------------------------------------------------------
//-- IMPLEMENTATION  -----------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
static void setTime(int time){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
  	PWR_BackupAccessCmd(ENABLE);
	RTC_WaitForLastTask();
	RTC_SetCounter((uint32_t)time);
	RTC_WaitForLastTask();
	PWR_BackupAccessCmd(DISABLE);
}

//------------------------------------------------------------------------------------
static int getTime(void){
	int counter;
	counter = (int)RTC_GetCounter();	
	return (counter);
}

//------------------------------------------------------------------------------------
static void endisAlarm(int endis){
	// Wait until last write operation on RTC registers has finished 
	RTC_WaitForLastTask();
	// Enable the RTC Second 
	RTC_ITConfig(RTC_IT_SEC, ((endis)? ENABLE : DISABLE));
	// Wait until last write operation on RTC registers has finished 
	RTC_WaitForLastTask();

}
//------------------------------------------------------------------------------------
void drv_RTC_Init(Exception *e){
	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 	// Configure one bit for preemption priority
	
	// Enable the RTC Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5){
  		// Enable PWR and BKP clocks 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
		// Allow access to BKP Domain
		PWR_BackupAccessCmd(ENABLE);
		// Reset Backup Domain
		BKP_DeInit();
		// Enable LSE */
		RCC_LSEConfig(RCC_LSE_ON);
		// Wait till LSE is ready 
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){}
		// Select LSE as RTC Clock Source 
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		// Enable RTC Clock 
		RCC_RTCCLKCmd(ENABLE);
		// Wait for RTC registers synchronization 
		RTC_WaitForSynchro();
		// Disable the RTC Second 
		endisAlarm(0);
		// Set RTC prescaler: set RTC period to 1sec
		RTC_SetPrescaler(32767); // RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) 
		// Wait until last write operation on RTC registers has finished 
		RTC_WaitForLastTask();

		/// ----  establece contador ----
		RTC_WaitForLastTask();
		RTC_SetCounter(0);
		RTC_WaitForLastTask();

		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else{
		// Wait for RTC registers synchronization
		RTC_WaitForSynchro();
		// Disable the RTC Second
		endisAlarm(0);
	}
  	/* Clear reset flags */
   	RCC_ClearFlag();
 
	/** Subscribe to SystemTopics update and attach callback function */
	sysTopic = SystemTopic_getRef("/sys", e);
	catch(e){
		return;
	}
	Topic_attach(sysTopic, 0, e);
	catch(e){
		return;
	}
	// sets default value for topic handler 
	sysTopicData.status = 0;
	sysTopicData.queries = SYS_NO_QUERIES;
	sysTopicData.time = 0;
	
	timeTopicData.time = 0;
	timeTopicData.event = TIME_NO_EVENTS;
}

//------------------------------------------------------------------------------------
void drv_RTC_OnTopicUpdate(void * obj, TopicData * td){
	(void)obj;	// param not used
	//topic checking
	if(td->id == (int)sysTopic){
		sysTopicData = *((SYS_TOPIC_DATA_T*)td->data);
		if((sysTopicData.queries & SYS_RTC_SET_TIME) != 0){
			sysTopicData.queries &= ~SYS_RTC_SET_TIME;
			setTime(sysTopicData.time);
		}
		if((sysTopicData.queries & SYS_RTC_ENABLE_EVENTS) != 0 && (sysTopicData.status & SYS_RTC_EVENTS_ENABLED) == 0){
			sysTopicData.queries &= ~SYS_RTC_ENABLE_EVENTS;
			sysTopicData.status |= SYS_RTC_EVENTS_ENABLED;
			endisAlarm(1);
		}
		if((sysTopicData.queries & SYS_RTC_DISABLE_EVENTS) != 0 && (sysTopicData.status & SYS_RTC_EVENTS_ENABLED) != 0){
			sysTopicData.queries &= ~SYS_RTC_DISABLE_EVENTS;
			sysTopicData.status &= ~SYS_RTC_EVENTS_ENABLED;
			endisAlarm(0);
		}
	}
}

//------------------------------------------------------------------------------------
//-- INTERRUPTS  ---------------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
void RTC_IRQHandler(void){
	Exception e;
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET) {
	 	RTC_ClearITPendingBit(RTC_IT_SEC); 	// Clear the RTC Second interrupt 
		RTC_WaitForLastTask(); 				// Wait until last write operation on RTC registers has finished 
		timeTopicData.time = getTime();
		timeTopicData.event = TIME_EVENT_SECOND;
		Topic_notify(timeTopic, &timeTopicData, sizeof(TIME_TOPIC_DATA_T), 0, 0, &e);
	}
}




