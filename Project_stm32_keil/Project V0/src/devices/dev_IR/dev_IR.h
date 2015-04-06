/*************************************************************************//**
 * @file    dev_IR.h
 * @mcu		STM32F1x
 * @brief   IR Device Manager
 * @date    11.06.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef __dev_IR_H__
#define __dev_IR_H__

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <stdint.h>


/****************************************************************************************************//**
 *                        PROVIDED INTERFACES
 ********************************************************************************************************/
uint8_t	dev_IR_Init	   				(uint8_t priority);
uint8_t dev_IR_Send	  				(uint8_t *pdu, uint8_t len);
uint8_t dev_IR_Read					(uint8_t *pdu, uint8_t len);
uint8_t dev_IR_IsWaitingReq 		(void);
uint8_t dev_IR_Cancel  				(void);
uint8_t dev_IR_Power				(void);
uint8_t dev_IR_Unpower				(void);
void 	dev_IR_NewRecvData 			(void);
void 	dev_IR_NewSentData 			(void);
void 	dev_IR_RecvTimeoutOverflow	(void);

/****************************************************************************************************//**
 *                        REQUIRED INTERFACES
 *
 * To be implemented in <SYS_Link.c>
 ********************************************************************************************************/
extern void 	(* const dev_IR_Requires_IDriverInit)(uint32_t baudrate);
extern void 	(* const dev_IR_Requires_IDriverEnableRx)(void);
extern void 	(* const dev_IR_Requires_IDriverDisableRx)(void);
extern void 	(* const dev_IR_Requires_IDriverEnable)(void);
extern void 	(* const dev_IR_Requires_IDriverTurnOn)(void);
extern void 	(* const dev_IR_Requires_IDriverTurnOff)(void);
extern int 		(* const dev_IR_Requires_IGetData)(void);
extern uint8_t	(* const dev_IR_Requires_ISend)(uint8_t *ptr, uint8_t size);
extern void 	(* const dev_IR_Requires_IDataReady)(uint8_t *pdata, uint16_t size);


/****************************************************************************************************//**
 *                        PUBLIC TYPES 
 ********************************************************************************************************/   

/*************************************************************************//**
 * @def		IR_OS_TMR
 * @scope	public
 * @brief   Timer code value for os_tmr_call processing
 ****************************************************************************/
#define IR_OS_TMR	1


#endif

