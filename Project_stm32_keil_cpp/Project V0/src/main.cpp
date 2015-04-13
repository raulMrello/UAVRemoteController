/**
  ******************************************************************************
  * @file    Project/Template/main.c 
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    06/19/2009
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "core_cm3.h"


/** \def 	SYSTICK_PERIOD_MILLISECONDS
  * @brief  Convert a milliseconds period into systick ticks, according with selected system frequency.
  */
#define SYSTICK_PERIOD_MILLISECONDS(ms)		(int)(((SystemFrequency/8)*1000000*(ms))/1000)


int main(void){
	// Setup STM32 system (clock, PLL and Flash configuration)
  	SystemInit();		
	// Setup SysTick interrupts
	SysTick_Config(SYSTICK_PERIOD_MILLISECONDS(10));
	// Infinite loop
  	for(;;); 		
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */

void SysTick_Handler(void){	
//	OS_tick(&e);
//	catch(&e){
//		while(1){ }
//	}
}



