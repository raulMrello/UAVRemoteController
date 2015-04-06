/*************************************************************************//**
 * @file    drv_IR.c
 * @mcu		STM32F1x
 * @brief   IR peripheral controller
 * @date    08.06.2010
 * @author  Ra�l M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include "stm32f10x.h"
#include "drv_IR.h"


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
 * @fun		drv_IR_EN_as_output
 * @brief	Provided interface to initialize driver 
 * @return	n/a
 **********************************************************************************/
void drv_IR_EN_as_output(void){
	drv_GPIO_config_pin(IR_EN_PIN,IR_EN_PORT,GPIO_Mode_Out_OD,GPIO_Speed_2MHz);
}

/****************************************************************************************************//**
 * @fun		drv_IR_EN_On
 * @brief	Provided interface to turn driver on
 * @return	n/a
 **********************************************************************************/
void drv_IR_EN_On(void){
	GPIO_ResetBits(IR_EN_PORT,IR_EN_PIN);
}

/****************************************************************************************************//**
 * @fun		drv_IR_EN_Off
 * @brief	Provided interface to turn driver off
 * @return	n/a
 **********************************************************************************/
void drv_IR_EN_Off(void){
	GPIO_SetBits(IR_EN_PORT,IR_EN_PIN);
}

