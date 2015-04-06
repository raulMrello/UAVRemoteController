#include <string.h>
#include "drv_TIM4.h"

#include "drv_gpio.h"


void drv_TIM4_Init(){

	//TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;				// NVIC

	// Enable TIM4 clocks
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	drv_GPIO_HDX2_as_timer();

 	// Enable the TIM4 global Interrupt
 	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;

	TIM_ICInit(TIM4, &TIM_ICInitStructure);

	// Enable the CC1 Interrupt Request
	//TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);
	
	// Enable the TIM4 global Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


}



void drv_TIM4_CaptureOn(void){
/*
	//memset(BufferCapture,0,sizeof(BufferCapture)/sizeof(uint16_t));
	TIM_SetCounter  ( TIM4,0x0000);
	TIM_Cmd(TIM4, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);
  */
}


void drv_TIM4_CaptureOff(void){
/*	TIM_Cmd(TIM4, DISABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC1, DISABLE);
*/
}



void TIM4_IRQHandler(void)
{ 
/*	
	if(TIM_GetITStatus(TIM4, TIM_IT_CC1) == SET) 
	{
		// Clear TIM4 Capture compare interrupt pending bit
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
		//DataCapture =  TIM_GetCapture1(TIM4);
		
		Decode_HDX(TIM_GetCapture1(TIM4));
		
	}
	*/

}
