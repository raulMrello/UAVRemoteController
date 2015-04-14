/*
 * drv_RTC.c
 *
 *  Created on: 07/04/2015
 *      Author: raulMrello
 *
 */
 
#include "drv_RTC.h"

//------------------------------------------------------------------------------------
//-- PRIVATE TYPEDEFS ----------------------------------------------------------------
//------------------------------------------------------------------------------------


/** Rtc data structure */
typedef struct  {
	uint32_t time;       		//!< time
	OnRtcTimeEvent cbTimeEvent; //!< time event callback
	RtcHandlerObj cbHandler;	//!< callback handler
	RtcFlags status;			//!< Rtc status flags
}RtcCtrl_t;


//------------------------------------------------------------------------------------
//-- PRIVATE MEMBERS -----------------------------------------------------------------
//------------------------------------------------------------------------------------

static RtcCtrl_t rtc;

//------------------------------------------------------------------------------------
static void NULL_CALLBACK(RtcHandlerObj handler){
	(void)handler;	// param not used
}


//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS  ---------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
drv_RTC drv_RTC_Init(OnRtcTimeEvent cbTimeEvent, RtcHandlerObj handler){
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
		drv_RTC_endisEvents((drv_RTC)&rtc, 0);
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
		drv_RTC_endisEvents((drv_RTC)&rtc, 0);
	}
  	/* Clear reset flags */
   	RCC_ClearFlag();
 
	/** Initializes driver members */
	rtc.status = 0;
	rtc.time = 0;
	rtc.cbHandler = handler;
	rtc.cbTimeEvent = (cbTimeEvent)? cbTimeEvent : NULL_CALLBACK;
	return (drv_RTC)&rtc;
}

//------------------------------------------------------------------------------------
void drv_RTC_endisEvents(drv_RTC drv, uint8_t flag){
	RtcCtrl_t* rc = (RtcCtrl_t *)drv;
	rc->status = 0;
	rc->status |= (flag & RTC_EVENTS_ENABLED);
	// Wait until last write operation on RTC registers has finished 
	RTC_WaitForLastTask();
	// Enable the RTC Second 
	RTC_ITConfig(RTC_IT_SEC, (((flag & RTC_EVENTS_ENABLED)!=0)? ENABLE : DISABLE));
	// Wait until last write operation on RTC registers has finished 
	RTC_WaitForLastTask();
}

//------------------------------------------------------------------------------------
uint32_t drv_RTC_getTime(drv_RTC drv){
	RtcCtrl_t* rc = (RtcCtrl_t *)drv;
	return rc->time;
}

//------------------------------------------------------------------------------------
void drv_RTC_setTime(drv_RTC drv, uint32_t time){
	RtcCtrl_t* rc = (RtcCtrl_t*)drv;
	rc->time = time;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
  	PWR_BackupAccessCmd(ENABLE);
	RTC_WaitForLastTask();
	RTC_SetCounter(rc->time);
	RTC_WaitForLastTask();
	PWR_BackupAccessCmd(DISABLE);
}


//------------------------------------------------------------------------------------
//-- INTERRUPTS  ---------------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
void RTC_IRQHandler(void){
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET) {
	 	RTC_ClearITPendingBit(RTC_IT_SEC); 	// Clear the RTC Second interrupt 
		RTC_WaitForLastTask(); 				// Wait until last write operation on RTC registers has finished 
		rtc.time = RTC_GetCounter();
		rtc.cbTimeEvent(rtc.cbHandler);
	}
}

