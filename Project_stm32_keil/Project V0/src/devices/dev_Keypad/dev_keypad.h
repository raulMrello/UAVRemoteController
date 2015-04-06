/*************************************************************************//**
 * @file    dev_KeyPad.h
 * @mcu		STM32F1x
 * @brief   Keypad Device Manager
 * @date    11.06.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef __dev_KEYPAD_H__
#define __dev_KEYPAD_H__

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <stdint.h>


/****************************************************************************************************//**
 *                        PROVIDED INTERFACES
 ********************************************************************************************************/
uint8_t dev_KeyPad_Init		(uint8_t priority);
void 	dev_KeyPad_NewKey	(void);
void 	dev_KeyPad_ForgetKey(void);


/****************************************************************************************************//**
 *                        REQUIRED INTERFACES
 *
 * To be implemented in <SYS_Link.c>
 ********************************************************************************************************/
extern void 	(* const dev_KeyPad_Requires_IDriverInit)(void);
extern int8_t  	(* const dev_KeyPad_Requires_IGetKey)(void);
extern void 	(* const dev_KeyPad_Requires_ISendKey)(uint32_t keycode);



#endif

