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
#include "drv_TIM2.h"


/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/****************************************************************************************************//**
 * @fun		drv_TIM2_Init
 * @brief	Provided interface to setup the driver to its default configuration
 * @return	n/a
 **********************************************************************************/
void drv_TIM2_Init(){

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	// Enable TIM2 clocks
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	// Configuramos los pines
	GPIO_TIM2_as_timer();

	// Configuracion TIM
	TIM_TimeBaseStructure.TIM_Period = 80;
	TIM_TimeBaseStructure.TIM_Prescaler = 60;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	// Output Compare Toggle Mode configuration: Channel1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Inactive;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);

	// Configuramos la salida del trigger cuando se actualiza el contador
	//TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

	//TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);

	//TIM_Cmd(TIM2, ENABLE);
}


/****************************************************************************************************//**
 * @fun		drv_TIM2_on
 * @brief	Provided interface to start the PWM signal generation
 * @return	n/a
 **********************************************************************************/
void drv_TIM2_on(void){
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);

	drv_TIM2_Counter_on();		// Puede estar habilitado..
}


/****************************************************************************************************//**
 * @fun		drv_TIM2_off
 * @brief	Provided interface to stop the PWM signal generation
 * @return	n/a
 **********************************************************************************/
void drv_TIM2_off(void){
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Inactive;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);

	drv_TIM2_Counter_off();
}

