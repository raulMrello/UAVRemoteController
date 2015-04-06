#ifndef __DRV_GPIO_H
#define	__DRV_GPIO_H

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include "stm32f10x.h"		    	// STM32F10x Library Definitions     
#include <stdint.h>

/****************************************************************************************************//**
 *                        PUBLIC DEFINITIONS
 ********************************************************************************************************/
#define PIN_On(port,pin)    	port->ODR |=  (1 << pin)
#define PIN_Toggle(port,pin) 	port->ODR ^=  (1 << pin)
#define PIN_Off(port,pin)     	port->ODR &= ~(1 << pin)
#define PIN_Is_On(port,pin)  	(port->ODR & (1 << pin))

//Configuration functions
void drv_GPIO_Init(void);
int  drv_GPIO_readKeyboard(void);
int  drv_GPIO_readSelectors(void);
void drv_GPIO_setOutput(uint8_t id, uint8_t value);

/****************************************************************************************************//**
 *                        GPIO CONFIGURATION 
 ********************************************************************************************************/

/** Keyboard */

#define KEY_N_PIN  		GPIO_Pin_0
#define KEY_N_PORT		GPIOB

#define KEY_NE_PIN 		GPIO_Pin_1
#define KEY_NE_PORT		GPIOB

#define KEY_E_PIN  		GPIO_Pin_2
#define KEY_E_PORT		GPIOB

#define KEY_ES_PIN  	GPIO_Pin_10
#define KEY_ES_PORT		GPIOB

#define KEY_S_PIN  		GPIO_Pin_11
#define KEY_S_PORT		GPIOB

#define KEY_SW_PIN  	GPIO_Pin_12
#define KEY_SW_PORT		GPIOB

#define KEY_W_PIN  		GPIO_Pin_13
#define KEY_W_PORT		GPIOB

#define KEY_WN_PIN  	GPIO_Pin_14
#define KEY_WN_PORT		GPIOB

/** Mode selectors */

#define SEL_ARM_PIN  	GPIO_Pin_4
#define SEL_ARM_PORT	GPIOA

#define SEL_LOC_PIN  	GPIO_Pin_5
#define SEL_LOC_PORT	GPIOA

#define SEL_ALT_PIN  	GPIO_Pin_6
#define SEL_ALT_PORT	GPIOA

#define SEL_RTH_PIN  	GPIO_Pin_7
#define SEL_RTH_PORT	GPIOA

/** Outputs */

#define OUT_0_PIN 	 	GPIO_Pin_15
#define OUT_0_PORT		GPIOB

#define OUT_1_PIN 	 	GPIO_Pin_8
#define OUT_1_PORT		GPIOA


/* Así se utilizan las líneas

#define drv_KEY_ESC_as_input()			drv_GPIO_config_pin(SW_ESC_PIN,SW_ESC_PORT,GPIO_Mode_IPU,GPIO_Speed_10MHz)
#define drv_KEY_ESC_is_on()				(GPIO_ReadInputDataBit(SW_ESC_PORT,SW_ESC_PIN)!=0)
#define drv_KEY_COL1_state()			GPIO_ReadInputDataBit(KEY_COL1_PORT,KEY_COL1_PIN)
#define drv_KEY_ROW1_as_output()		drv_GPIO_config_pin(KEY_ROW1_PIN,KEY_ROW1_PORT,GPIO_Mode_Out_PP,GPIO_Speed_2MHz)
#define drv_KEY_ROW1_Off()				GPIO_ResetBits(KEY_ROW1_PORT,KEY_ROW1_PIN)
#define drv_KEY_ROW1_On()				GPIO_SetBits(KEY_ROW1_PORT,KEY_ROW1_PIN)

*/

#endif

