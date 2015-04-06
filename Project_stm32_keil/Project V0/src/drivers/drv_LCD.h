/*************************************************************************//**
 * @file    drv_LCD.h
 * @mcu		STM32F1x
 * @brief   LCD physical chip driver 
 * @date    15.06.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef __DRV_LCD_H
#define	__DRV_LCD_H

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include "stm32f10x.h"		    	// STM32F10x Library Definitions     
#include "drv_GPIO.h"
#include "OrbisUC1701x.h"

/****************************************************************************************************//**
 *                        GPIO CONFIGURATION 
 ********************************************************************************************************/
#define GPIO_LCD_RST_PIN			GPIO_Pin_12	
#define GPIO_LCD_RST_PORT			GPIOB

#define GPIO_LCD_CD_PIN				GPIO_Pin_8	
#define GPIO_LCD_CD_PORT			GPIOA

#define GPIO_LCD_SEL_PIN			GPIO_Pin_14	
#define GPIO_LCD_SEL_PORT			GPIOB

#define GPIO_LCD_BL_PIN				GPIO_Pin_0
#define GPIO_LCD_BL_PORT			GPIOB

/****************************************************************************************************//**
 *                        PROVIDED INTERFACES
 ********************************************************************************************************/
void drv_LCD_RST_as_output(void);
void drv_LCD_RST_Set(void);
void drv_LCD_RST_Clear(void);

void drv_LCD_CMD_as_output(void);
void drv_LCD_CMD_On(void);
void drv_LCD_CMD_Off(void);		

void drv_LCD_SEL_as_output(void);
void drv_LCD_SEL_Active(void);	
void drv_LCD_SEL_Clear(void);	

void drv_LCD_BL_as_output(void);
void drv_LCD_BL_On(void);		
void drv_LCD_BL_Off(void);		

void drv_LCD_DelayMs(uint16_t d);
void drv_LCD_ClearDevice(void);	

#endif

