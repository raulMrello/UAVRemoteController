/*************************************************************************//**
 * @file    dev_LCD.h
 * @mcu		STM32F1x
 * @brief   LCD and Backlight device manager
 * @date    14.06.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef __dev_LCD_H__
#define __dev_LCD_H__

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <stdint.h>

/****************************************************************************************************//**
 *                        PROVIDED INTERFACES 
 ********************************************************************************************************/
void 	dev_LCD_Init			(uint8_t blight);
void	dev_LCD_UpdateMem		(void);
void	dev_LCD_Standby			(void);
void 	dev_LCD_Weakup			(void);
void 	dev_LCD_DrawMem			(void);
void 	dev_LCD_Off				(void);
void 	dev_LCD_On				(void);
void 	dev_LCD_PermitBacklight	(void);
void 	dev_LCD_ForbidBacklight	(void);
void 	dev_LCD_TurnOn			(uint8_t blight);
void 	dev_LCD_TurnOff			(void);
uint8_t dev_LCD_GetContrast		(void);
void 	dev_LCD_SetContrast		(uint8_t contrast);

/****************************************************************************************************//**
 *                        REQUIRED INTERFACES
 *
 * To be implemented in <SYS_Link.c>
 ********************************************************************************************************/
extern void (* const dev_LCD_Requires_IDriverInit)(void);
extern void (* const dev_LCD_Requires_IDriverDeinit)(void);
extern int 	(* const dev_LCD_Requires_IWriteData)(int c);
extern void (* const dev_LCD_Requires_ILightInit)(void);
extern void (* const dev_LCD_Requires_ILightAt)(uint8_t val);
extern void (* const dev_LCD_Requires_ILightOff)(void);
extern void (* const dev_LCD_Requires_IRstAsOutput)(void);
extern void (* const dev_LCD_Requires_IRstSet)(void);
extern void (* const dev_LCD_Requires_IRstClear)(void);
extern void (* const dev_LCD_Requires_ICmdAsOutput)(void);
extern void (* const dev_LCD_Requires_ICmdOn)(void);
extern void (* const dev_LCD_Requires_ICmdOff)(void);
extern void (* const dev_LCD_Requires_ISelAsOutput)(void);
extern void (* const dev_LCD_Requires_ISelActive)(void);
extern void (* const dev_LCD_Requires_ISelClear)(void);
extern void (* const dev_LCD_Requires_IBlAsOutput)(void);
extern void (* const dev_LCD_Requires_IBlOn)(void);
extern void (* const dev_LCD_Requires_IBlOff)(void);
extern void (* const dev_LCD_Requires_IDelayMs)(uint16_t delay);
extern void (* const dev_LCD_Requires_IClearDevice)(void);

/****************************************************************************************************//**
 *                        PUBLIC TYPES 
 ********************************************************************************************************/
extern unsigned char LCD_MenMirror[8][128];  // Copia de la memoria 1024Byte
extern unsigned char LCD_FlagUpdate;

#endif

