/*************************************************************************//**
 * @file    drv_SPI2.h
 * @mcu		STM32F1x
 * @brief   SPI2 peripheral controller
 * @date    14.06.2010
 * @author  Ra�l M.
 ****************************************************************************/
#ifndef __SPI2_H__
#define __SPI2_H__

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/

typedef enum{
	SPI2_SINGLE_WIRE_TX = 0,
	SPI2_SINGLE_WIRE_RX, 
	SPI2_TWO_WIRES
}SPI2_INIT_TYPE;

/****************************************************************************************************//**
 *                        PROVIDED INTERFACES 
 ********************************************************************************************************/
void drv_SPI2_Init					(uint32_t type);
void drv_SPI2_InitByDefault			(void);
void drv_SPI2_Deinit				(void);
int  drv_SPI2_SendChar 				(int c);
int  drv_SPI2_GetChar 				(int *c);
int  drv_SPI2_SendChar_fullDuplex 	(int c);


/****************************************************************************************************//**
 *                        GPIO CONFIGURATION 
 ********************************************************************************************************/



#endif
