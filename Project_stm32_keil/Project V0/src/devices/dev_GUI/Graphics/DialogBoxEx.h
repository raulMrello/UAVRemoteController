/*************************************************************************//**
 * @file    DialogBoxEx.c
 * @mcu		STM32F1x
 * @brief   Dialogbox implementation
 * @date    28.09.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef	__FORM_DIALOGBOXEX_H__
#define	__FORM_DIALOGBOXEX_H__

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <stdint.h>
#include "Graphics.h"

/****************************************************************************************************//**
 *                        PUBLIC INTERFACE
 ********************************************************************************************************/
void DialogBoxEx_create(void *pThis, uint8_t type, XCHAR *text_w, XCHAR *text, void (*ptr_fun_left)(void *), 
								void (*ptr_fun_right)(void *), void (*ptr_fun_up)(void *), void (*ptr_fun_down)(void *), 
								XCHAR *leftText, XCHAR *rightText, StateHandler gts, void (*ptr_fun_esc)(void *));

uint32_t DialogBoxEx_callback(void *pThis, uint32_t objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);

#endif

