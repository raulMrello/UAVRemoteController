/*************************************************************************//**
 * @file    drv_TIM2.h
 * @mcu		STM32F1x
 * @brief   TIM2 peripheral controller
 * @date    14.06.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef __TIM2_H__
#define __TIM2_H__

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include "stm32f10x.h"		            // STM32F10x Library Definitions     
#include "drv_GPIO.h"

/****************************************************************************************************//**
 *                        PROVIDED INTERFACES 
 ********************************************************************************************************/
void drv_TIM2_Init	(void);
void drv_TIM2_on	(void);
void drv_TIM2_off	(void);


/****************************************************************************************************//**
 *                        PUBLIC TYPES 
 ********************************************************************************************************/
   
#define TIM2_COUNT_MODE_ON  		0	// deshabilitamos la salida a "0", pero no el contador
#define TIM2_COUNT_MODE_OFF			1	// deshabilitamos la salida a "0" y el contador General

#define drv_TIM2_Counter_on()  		TIM_Cmd(TIM2, ENABLE)
#define drv_TIM2_Counter_off() 		TIM_Cmd(TIM2, DISABLE)


/****************************************************************************************************//**
 *                        GPIO CONFIGURATION 
 ********************************************************************************************************/
#define GPIO_TIM2_PIN  				GPIO_Pin_1
#define GPIO_TIM2_PORT				GPIOA
#define GPIO_TIM2_as_timer()		drv_GPIO_config_pin(GPIO_TIM2_PIN,GPIO_TIM2_PORT,GPIO_Mode_AF_PP,GPIO_Speed_50MHz)


#endif

