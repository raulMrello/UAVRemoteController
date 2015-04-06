/*************************************************************************//**
 * @file    drv_TIM2.c
 * @mcu		STM32F1x
 * @brief   TIM2 peripheral controller
 * @date    14.06.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include "drv_TIM3.h"

static uint8_t duty = 255;

/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/****************************************************************************************************//**
 * @fun		ReloadAndRun
 * @brief	Loads a new duty value and restart timer
 * @return	n/a
 **********************************************************************************/
static void ReloadAndRun(uint8_t value){
	static uint8_t lastLoadedValue = (uint8_t)-1;

    if(value == lastLoadedValue)
   		return;

	lastLoadedValue = value;
	TIM_SetCompare3(TIM3, value*10);
 
}

/****************************************************************************************************//**
 * @fun		drv_TIM3_Init
 * @brief	Provided interface to setup the driver to its default configuration
 * @return	n/a
 **********************************************************************************/
void drv_TIM3_Init(){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  		TIM_OCInitStructure;

	// Enable TIM3 clocks
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	// Configuramos los pines
	GPIO_TIM3_as_timer();

	// Configuracion TIM
	TIM_TimeBaseStructure.TIM_Period = 999;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
   	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   	TIM_OCInitStructure.TIM_Pulse = 0;	
 
   	TIM_OC3Init(TIM3, &TIM_OCInitStructure); 
   	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
 	TIM_ARRPreloadConfig(TIM3, ENABLE);
 
   	/* TIM3 enable counter */
   	TIM_Cmd(TIM3, ENABLE);

	duty = 0;
}


/****************************************************************************************************//**
 * @fun		drv_TIM3_SetDuty
 * @brief	Provided interface to set a new duty %
 * @param	val		new duty value. Range 0..99
 * @return	n/a
 **********************************************************************************/
void drv_TIM3_SetDuty(uint8_t val){
	if(val > 100)
		val = 100;
	duty = val;
}


/****************************************************************************************************//**
 * @fun		drv_TIM3_on
 * @brief	Provided interface to start the PWM signal generation
 * @return	n/a
 **********************************************************************************/
void drv_TIM3_on(void){
	ReloadAndRun(duty);
}


/****************************************************************************************************//**
 * @fun		drv_TIM3_off
 * @brief	Provided interface to stop the PWM signal generation
 * @return	n/a
 **********************************************************************************/
void drv_TIM3_off(void){
	ReloadAndRun(0);
}

  
