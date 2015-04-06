/*************************************************************************//**
 * @file    drv_KEY.h
 * @mcu		STM32F1x
 * @brief   GPIO peripheral controller to manage physical keyboard keystrokes.
 * @date    15.06.2010
 * @author  Raúl M.
 ****************************************************************************/

#ifndef __DRV_KEY_H
#define	__DRV_KEY_H

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include "drv_GPIO.h"
#include "stm32f10x.h"		    	// STM32F10x Library Definitions     

/****************************************************************************************************//**
 *                        PROVIDED INTERFACES
 ********************************************************************************************************/
void 	drv_KEY_Init  (void);
int8_t 	drv_KEY_GetKey(void);

/****************************************************************************************************//**
 *                        REQUIRED INTERFACES
 *
 * To be implemented in <SYS_Link.c>
 ********************************************************************************************************/
extern void (* const drv_KEY_Requires_IKeyEvent)(void);


/****************************************************************************************************//**
 *                        GPIO CONFIGURATION 
 ********************************************************************************************************/
#define SW_N_PIN  		GPIO_Pin_0
#define SW_N_PORT		GPIOB

#define SW_NE_PIN  		GPIO_Pin_1
#define SW_NE_PORT		GPIOB

#define SW_E_PIN  		GPIO_Pin_2
#define SW_E_PORT		GPIOB

#define SW_ES_PIN  		GPIO_Pin_10
#define SW_ES_PORT		GPIOB

#define SW_S_PIN  		GPIO_Pin_11
#define SW_S_PORT		GPIOB

#define SW_SW_PIN  		GPIO_Pin_12
#define SW_SW_PORT		GPIOB

#define SW_W_PIN  		GPIO_Pin_13
#define SW_W_PORT		GPIOB

#define SW_WN_PIN  		GPIO_Pin_14
#define SW_WN_PORT		GPIOB

/** Mode selectors */

#define SW_ARM_PIN  	GPIO_Pin_4
#define SW_ARM_PORT		GPIOA

#define SW_LOC_PIN  	GPIO_Pin_5
#define SW_LOC_PORT		GPIOA

#define SW_ALT_PIN  	GPIO_Pin_6
#define SW_ALT_PORT		GPIOA

#define SW_RTH_PIN  	GPIO_Pin_7
#define SW_RTH_PORT		GPIOA



#define drv_KEY_ESC_as_input()			drv_GPIO_config_pin(SW_ESC_PIN,SW_ESC_PORT,GPIO_Mode_IPU,GPIO_Speed_10MHz)
#define drv_KEY_TOP_as_input()			drv_GPIO_config_pin(SW_TOP_PIN,SW_TOP_PORT,GPIO_Mode_IPU,GPIO_Speed_10MHz)
#define drv_KEY_CENTER_as_input()		drv_GPIO_config_pin(SW_CENTER_PIN,SW_CENTER_PORT,GPIO_Mode_IPU,GPIO_Speed_10MHz)
#define drv_KEY_RIGHT_as_input()		drv_GPIO_config_pin(SW_RIGHT_PIN,SW_RIGHT_PORT,GPIO_Mode_IPU,GPIO_Speed_10MHz)
#define drv_KEY_LEFT_as_input()			drv_GPIO_config_pin(SW_LEFT_PIN,SW_LEFT_PORT,GPIO_Mode_IPU,GPIO_Speed_10MHz)
#define drv_KEY_BOTTON_as_input()		drv_GPIO_config_pin(SW_BOTTON_PIN,SW_BOTTON_PORT,GPIO_Mode_IPU,GPIO_Speed_10MHz)

#define drv_KEY_ESC_is_on()				(GPIO_ReadInputDataBit(SW_ESC_PORT,SW_ESC_PIN)!=0)
#define drv_KEY_TOP_is_on()				(GPIO_ReadInputDataBit(SW_TOP_PORT,SW_TOP_PIN)!=0)
#define drv_KEY_CENTER_is_on()			(GPIO_ReadInputDataBit(SW_CENTER_PORT,SW_CENTER_PIN)!=0)
#define drv_KEY_RIGHT_is_on()			(GPIO_ReadInputDataBit(SW_RIGHT_PORT,SW_RIGHT_PIN)!=0)
#define drv_KEY_LEFT_is_on()			(GPIO_ReadInputDataBit(SW_LEFT_PORT,SW_LEFT_PIN)!=0)
#define drv_KEY_BOTTON_is_on()			(GPIO_ReadInputDataBit(SW_BOTTON_PORT,SW_BOTTON_PIN)!=0)

#define KEY_COL4_PIN					GPIO_Pin_1		  
#define KEY_COL4_PORT					GPIOC
										   	
#define KEY_COL3_PIN					GPIO_Pin_8		  
#define KEY_COL3_PORT					GPIOG

#define KEY_COL2_PIN					GPIO_Pin_3		  
#define KEY_COL2_PORT					GPIOC

#define KEY_COL1_PIN					GPIO_Pin_4		  
#define KEY_COL1_PORT					GPIOC

												
#define KEY_POWER_PIN					GPIO_Pin_13		  //FUN1
#define KEY_POWER_PORT					GPIOC

#define KEY_FUN4_PIN					GPIO_Pin_5		  
#define KEY_FUN4_PORT					GPIOC


#define drv_KEY_COL1_as_input()			drv_GPIO_config_pin(KEY_COL1_PIN, KEY_COL1_PORT, GPIO_Mode_IPU,GPIO_Speed_2MHz) 
#define drv_KEY_COL1_state()			GPIO_ReadInputDataBit(KEY_COL1_PORT,KEY_COL1_PIN)

#define drv_KEY_COL2_as_input()			drv_GPIO_config_pin(KEY_COL2_PIN, KEY_COL2_PORT, GPIO_Mode_IPU,GPIO_Speed_2MHz) 
#define drv_KEY_COL2_state()			GPIO_ReadInputDataBit(KEY_COL2_PORT,KEY_COL2_PIN)

#define drv_KEY_COL3_as_input()			drv_GPIO_config_pin(KEY_COL3_PIN, KEY_COL3_PORT, GPIO_Mode_IPU,GPIO_Speed_2MHz) 
#define drv_KEY_COL3_state()			GPIO_ReadInputDataBit(KEY_COL3_PORT,KEY_COL3_PIN)

#define drv_KEY_COL4_as_input()			drv_GPIO_config_pin(KEY_COL4_PIN, KEY_COL4_PORT, GPIO_Mode_IPU,GPIO_Speed_2MHz) 
#define drv_KEY_COL4_state()			GPIO_ReadInputDataBit(KEY_COL4_PORT,KEY_COL4_PIN)

#define drv_KEY_POWER_as_input()		drv_GPIO_config_pin(KEY_POWER_PIN, KEY_POWER_PORT, GPIO_Mode_IPU,GPIO_Speed_2MHz) 
#define drv_KEY_POWER_state()			GPIO_ReadInputDataBit(KEY_POWER_PORT,KEY_POWER_PIN)

#define drv_KEY_ENTER_as_input()		drv_GPIO_config_pin(KEY_ENTER_PIN, KEY_ENTER_PORT, GPIO_Mode_IPU,GPIO_Speed_2MHz) 
#define drv_KEY_ENTER_state()			GPIO_ReadInputDataBit(KEY_ENTER_PORT,KEY_ENTER_PIN)

#define drv_KEY_FUN4_as_input()			drv_GPIO_config_pin(KEY_FUN4_PIN, KEY_FUN4_PORT, GPIO_Mode_IPU,GPIO_Speed_2MHz) 
#define drv_KEY_FUN4_state()			GPIO_ReadInputDataBit(KEY_FUN4_PORT,KEY_FUN4_PIN)


#define KEY_ROW1_PIN					GPIO_Pin_11		  //pin 14 port D
#define KEY_ROW1_PORT					GPIOF

#define KEY_ROW2_PIN					GPIO_Pin_10		  //pin 14 port D
#define KEY_ROW2_PORT					GPIOF

#define KEY_ROW3_PIN					GPIO_Pin_9		  //pin 14 port D
#define KEY_ROW3_PORT					GPIOF

#define KEY_ROW4_PIN					GPIO_Pin_8		  //pin 14 port D
#define KEY_ROW4_PORT					GPIOF

#define drv_KEY_ROW1_as_output()		drv_GPIO_config_pin(KEY_ROW1_PIN,KEY_ROW1_PORT,GPIO_Mode_Out_PP,GPIO_Speed_2MHz)
#define drv_KEY_ROW1_Off()				GPIO_ResetBits(KEY_ROW1_PORT,KEY_ROW1_PIN)
#define drv_KEY_ROW1_On()				GPIO_SetBits(KEY_ROW1_PORT,KEY_ROW1_PIN)

#define drv_KEY_ROW2_as_output()		drv_GPIO_config_pin(KEY_ROW2_PIN,KEY_ROW2_PORT,GPIO_Mode_Out_PP,GPIO_Speed_2MHz)
#define drv_KEY_ROW2_Off()				GPIO_ResetBits(KEY_ROW2_PORT,KEY_ROW2_PIN)
#define drv_KEY_ROW2_On()				GPIO_SetBits(KEY_ROW2_PORT,KEY_ROW2_PIN)

#define drv_KEY_ROW3_as_output()		drv_GPIO_config_pin(KEY_ROW3_PIN,KEY_ROW3_PORT,GPIO_Mode_Out_PP,GPIO_Speed_2MHz)
#define drv_KEY_ROW3_Off()				GPIO_ResetBits(KEY_ROW3_PORT,KEY_ROW3_PIN)
#define drv_KEY_ROW3_On()				GPIO_SetBits(KEY_ROW3_PORT,KEY_ROW3_PIN)
										
#define drv_KEY_ROW4_as_output()		drv_GPIO_config_pin(KEY_ROW4_PIN,KEY_ROW4_PORT,GPIO_Mode_Out_PP,GPIO_Speed_2MHz)
#define drv_KEY_ROW4_Off()				GPIO_ResetBits(KEY_ROW4_PORT,KEY_ROW4_PIN)
#define drv_KEY_ROW4_On()				GPIO_SetBits(KEY_ROW4_PORT,KEY_ROW4_PIN)


#define KEY_LED1_PIN					GPIO_Pin_2		  //pin 14 port D
#define KEY_LED1_PORT					GPIOC

#define KEY_LED2_PIN					GPIO_Pin_7		  //pin 14 port D
#define KEY_LED2_PORT					GPIOG

#define drv_KEY_LED1_as_output()		drv_GPIO_config_pin(KEY_LED1_PIN,KEY_LED1_PORT,GPIO_Mode_Out_PP,GPIO_Speed_2MHz)
#define drv_KEY_LED1_Off()				GPIO_ResetBits(KEY_LED1_PORT,KEY_LED1_PIN)
#define drv_KEY_LED1_On()				GPIO_SetBits(KEY_LED1_PORT,KEY_LED1_PIN)

#define drv_KEY_LED2_as_output()		drv_GPIO_config_pin(KEY_LED2_PIN,KEY_LED2_PORT,GPIO_Mode_Out_PP,GPIO_Speed_2MHz)
#define drv_KEY_LED2_On()				GPIO_ResetBits(KEY_LED2_PORT,KEY_LED2_PIN)
#define drv_KEY_LED2_Off()				GPIO_SetBits(KEY_LED2_PORT,KEY_LED2_PIN)



#endif

