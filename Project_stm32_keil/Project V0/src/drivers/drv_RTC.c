/*************************************************************************//**
 * @file    drv_RTC.h
 * @mcu		STM32F1x
 * @brief   RTC peripheral controller
 * @date    08.06.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include "stm32f10x.h"
#include "drv_RTC.h"


/****************************************************************************************************//**
 *                        	REQUIRED INTERFACES
 ********************************************************************************************************/
#define ITimeEvent()		drv_RTC_Requires_ITimeEvent()

 
/****************************************************************************************************//**
 *                        PRIVATE MEMBERS
 ********************************************************************************************************/




/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/****************************************************************************************************//**
 * @fun		drv_RTC_Init
 * @brief	Provided interface to initialize driver enabling interrupts
 * @return	n/a
 **********************************************************************************/
void drv_RTC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 	// Configure one bit for preemption priority
	
	// Enable the RTC Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
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
		// Enable the RTC Second 
		drv_RTC_Enable();
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
	else
	{
		// Wait for RTC registers synchronization
		RTC_WaitForSynchro();
		// Enable the RTC Second
		drv_RTC_Enable();
	}
  	/* Clear reset flags */
   	RCC_ClearFlag();
 
}

/****************************************************************************************************//**
 * @fun		drv_RTC_GetTime
 * @brief	Provided interface to get the current time value
 * @return	time value as uint32
 **********************************************************************************/
uint32_t drv_RTC_GetTime(void){
	uint32_t counter;
	counter = RTC_GetCounter();	
	return (counter);
}

/****************************************************************************************************//**
 * @fun		drv_RTC_SetTime
 * @brief	Provided interface to set the current time value
 * @param	counter		New time value
 * @return	n/a
 **********************************************************************************/
void drv_RTC_SetTime(uint32_t counter){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
  	PWR_BackupAccessCmd(ENABLE);
	RTC_WaitForLastTask();
	RTC_SetCounter(counter);
	RTC_WaitForLastTask();
	PWR_BackupAccessCmd(DISABLE);
}

/****************************************************************************************************//**
 * @fun		RTC_IRQHandler
 * @brief	RTC interrupt handler it invokes to required interface ITimeEvent to notify time updating
 * @return	n/a
 **********************************************************************************/
void RTC_IRQHandler(void){
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{
	 	RTC_ClearITPendingBit(RTC_IT_SEC); 	// Clear the RTC Second interrupt 
		RTC_WaitForLastTask(); 			// Wait until last write operation on RTC registers has finished 
		ITimeEvent();
	}
}
/****************************************************************************************************//**
 * @fun		drv_RTC_Enable
 * @brief	Enable interrupts
 * @return	n/a
 **********************************************************************************/
void drv_RTC_Enable(void){
	// Wait until last write operation on RTC registers has finished 
	RTC_WaitForLastTask();
	// Enable the RTC Second 
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	// Wait until last write operation on RTC registers has finished 
	RTC_WaitForLastTask();

}

/****************************************************************************************************//**
 * @fun		drv_RTC_Disable
 * @brief	Disable interrupts
 * @return	n/a
 **********************************************************************************/
void drv_RTC_Disable(void){
	// Wait until last write operation on RTC registers has finished 
	RTC_WaitForLastTask();
	// Disable the RTC Second 
	RTC_ITConfig(RTC_IT_SEC, DISABLE);
	// Wait until last write operation on RTC registers has finished 
	RTC_WaitForLastTask();
}


