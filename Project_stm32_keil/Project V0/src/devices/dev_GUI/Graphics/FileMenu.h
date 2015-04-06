/*************************************************************************//**
 * @file    FileMenu.h
 * @mcu		STM32F1x
 * @brief   File saving Menu with label text and dialogbox
 * @date    15.06.2010
 * @author  Raúl M.
 ****************************************************************************/

#ifndef __FileMenu_H
#define __FileMenu_H

/****************************************************************************************************//**
 *                        PUBLIC INTERFACE
 ********************************************************************************************************/
void 	 FileMenu_Create	(void * pThis, XCHAR * mt, char * filename, uint8_t **pData, uint16_t *pSize, XCHAR * path, uint8_t ovrWrt, StateHandler gts);
void 	 FileMenu_Display	(void * pThis);
uint32_t FileMenu_Callback	(void *pThis, uint32_t objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);


#endif

