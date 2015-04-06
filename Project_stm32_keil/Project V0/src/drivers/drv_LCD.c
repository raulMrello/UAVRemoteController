/*************************************************************************//**
 * @file    drv_LCD.h
 * @mcu		STM32F1x
 * @brief   LCD peripheral controller
 * @date    08.06.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include "drv_LCD.h"



/****************************************************************************************************//**
 *                        	REQUIRED INTERFACES
 ********************************************************************************************************/

 
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
 * @fun		drv_LCD_...
 * @brief	Provided interface to setup driver 
 * @return	n/a
 **********************************************************************************/
void drv_LCD_RST_as_output(void){
	drv_GPIO_config_pin(GPIO_LCD_RST_PIN,GPIO_LCD_RST_PORT,GPIO_Mode_Out_PP,GPIO_Speed_2MHz);
}

void drv_LCD_RST_Set(void){
	GPIO_ResetBits(GPIO_LCD_RST_PORT,GPIO_LCD_RST_PIN);
}

void drv_LCD_RST_Clear(void){
	GPIO_SetBits(GPIO_LCD_RST_PORT,GPIO_LCD_RST_PIN);
}

void drv_LCD_CMD_as_output(void){
	drv_GPIO_config_pin(GPIO_LCD_CD_PIN,GPIO_LCD_CD_PORT,GPIO_Mode_Out_PP,GPIO_Speed_2MHz);
}

void drv_LCD_CMD_On(void){
	GPIO_ResetBits(GPIO_LCD_CD_PORT,GPIO_LCD_CD_PIN);
}

void drv_LCD_CMD_Off(void){
	GPIO_SetBits(GPIO_LCD_CD_PORT,GPIO_LCD_CD_PIN);
}

void drv_LCD_SEL_as_output(void){
	drv_GPIO_config_pin(GPIO_LCD_SEL_PIN,GPIO_LCD_SEL_PORT,GPIO_Mode_Out_PP,GPIO_Speed_2MHz);
}

void drv_LCD_SEL_Active(void){
	GPIO_ResetBits(GPIO_LCD_SEL_PORT,GPIO_LCD_SEL_PIN);
}

void drv_LCD_SEL_Clear(void){
	GPIO_SetBits(GPIO_LCD_SEL_PORT,GPIO_LCD_SEL_PIN);
}


void drv_LCD_BL_as_output(void){
	drv_GPIO_config_pin(GPIO_LCD_BL_PIN,GPIO_LCD_BL_PORT,GPIO_Mode_Out_PP,GPIO_Speed_2MHz);
}

void drv_LCD_BL_On(void){
	GPIO_SetBits(GPIO_LCD_BL_PORT,GPIO_LCD_BL_PIN);
}

void drv_LCD_BL_Off(void){
	GPIO_ResetBits(GPIO_LCD_BL_PORT,GPIO_LCD_BL_PIN);
}

void drv_LCD_DelayMs(uint16_t d){
	DelayMs(d);
}

void drv_LCD_ClearDevice(void){
	ClearDevice();
}

