/*************************************************************************//**
 * @file    FlexMenu.h
 * @mcu		STM32F1x
 * @brief   Complex Menu with listbox and dialog box to confirm option selection
 * @date    15.06.2010
 * @author  Raúl M.
 ****************************************************************************/

#ifndef __FlexMenu_H
#define __FlexMenu_H

/****************************************************************************************************//**
 *                        PUBLIC INTERFACE
 ********************************************************************************************************/
void FlexMenu_Create(	void * pThis, XCHAR * mt, XCHAR *lbo, XCHAR *ftr, void(*osa)(void*,char*), XCHAR *db, XCHAR *dlo, 
						 XCHAR *dro, void(*ola)(void*), void(*ora)(void*), StateHandler gts,	StateHandler * tts);

uint32_t FlexMenu_Callback(void *pThis, uint32_t objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);


#endif

