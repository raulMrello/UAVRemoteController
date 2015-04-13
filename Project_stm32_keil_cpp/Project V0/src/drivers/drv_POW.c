/*
 * drv_POW.c
 *
 *  Created on: 07/04/2015
 *      Author: raulMrello
 *
 */
 
#include "drv_POW.h"

//------------------------------------------------------------------------------------
//-- PRIVATE TYPEDEFS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

/** Pow driver data structure */
typedef struct{
	PowFlags status;
}PowCtrl_t;
	

//------------------------------------------------------------------------------------
//-- PRIVATE MEMBERS -----------------------------------------------------------------
//------------------------------------------------------------------------------------

static PowCtrl_t pow;



//------------------------------------------------------------------------------------
/** \fun restartClocks
 *  \brief Restart clocks after exiting from stop mode
 */
static void restartClocks(void){
	ErrorStatus HSEStartUpStatus;

	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);
 
	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
 
	if(HSEStartUpStatus == SUCCESS){
 
		#ifdef STM32F10X_CL
		/* Enable PLL2 */ 
		RCC_PLL2Cmd(ENABLE);
 
		/* Wait till PLL2 is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET){ }
		#endif
 
		/* Enable PLL */ 
		RCC_PLLCmd(ENABLE);
 
		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){ }
 
		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
 
		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08){ }
	}
}


//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

drv_POW drv_POW_Init(void){
	pow.status |= POW_RUN_MODE;
	return (drv_POW)&pow;
}

//------------------------------------------------------------------------------------
void drv_POW_setMode(drv_POW drv, PowFlags mode){
	PowCtrl_t* pc = (PowCtrl_t*)drv;
	if((pc->status & POW_RUN_MODE) == 0 && (mode & POW_RUN_MODE) != 0){		
		restartClocks();
		pc->status |= POW_RUN_MODE;
	}
	else if((pc->status & POW_RUN_MODE) != 0 && (mode & POW_RUN_MODE) == 0){
		pc->status &= ~POW_RUN_MODE;
		PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFE);
		
		/*** ................ ***/
		/******* STOP MODE ******/
		/*** ................ ***/
		
		restartClocks();
		pc->status |= POW_RUN_MODE;
	}
}

