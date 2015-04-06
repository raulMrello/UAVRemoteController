/*************************************************************************//**
 * @file    ComMenu.h
 * @mcu		STM32F1x
 * @brief   Communications Menu with label text and progressbar
 * @date    15.06.2010
 * @author  Raúl M.
 ****************************************************************************/

#ifndef __ComMenu_H
#define __ComMenu_H

/****************************************************************************************************//**
 *                        PUBLIC INTERFACE
 ********************************************************************************************************/
void 	 ComMenu_Create			(void * pThis, XCHAR * mt, uint8_t(*osa)(void*,char**,uint8_t*,uint8_t*,uint8_t**,uint8_t*), 
							 	 uint8_t mp, uint8_t rw, XCHAR *dtt, XCHAR *dbt, XCHAR *dlt, XCHAR *drt, void (*dla)(void *), 
							  	 void (*dra)(void *), StateHandler gts);

void 	 ComMenu_Display		(void * pThis);
uint32_t ComMenu_Callback		(void *pThis, uint32_t objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);


#endif

