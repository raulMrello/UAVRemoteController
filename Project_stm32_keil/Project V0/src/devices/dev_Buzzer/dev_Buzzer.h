/*************************************************************************//**
 * @file    dev_Buzzer.h
 * @mcu		STM32F1x
 * @brief   Buzzer device manager
 * @date    14.06.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef __dev_BUZZER_H__
#define __dev_BUZZER_H__

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <stdint.h>

/****************************************************************************************************//**
 *                        PROVIDED INTERFACES 
 ********************************************************************************************************/
uint8_t dev_Buzzer_Init			(uint8_t priority);
uint8_t dev_Buzzer_BeepShort	(void);
uint8_t dev_Buzzer_BeepLong		(void);
uint8_t dev_Buzzer_BeepConfirm	(void);
#define BEEP_SHORT()			dev_Buzzer_BeepShort()
#define BEEP_LONG()				dev_Buzzer_BeepLong()


/****************************************************************************************************//**
 *                        REQUIRED INTERFACES
 *
 * To be implemented in <SYS_Link.c>
 ********************************************************************************************************/
extern void (* const dev_Buzzer_Requires_IDriverInit)(void);
extern void (* const dev_Buzzer_Requires_IStart)(void);
extern void (* const dev_Buzzer_Requires_IStop)(void);


#endif

