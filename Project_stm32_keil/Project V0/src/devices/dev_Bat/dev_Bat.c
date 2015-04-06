/*************************************************************************//**
 * @file    dev_Bat.h
 * @mcu		STM32F1x
 * @brief   Bat Device Manager
 * @date    11.06.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <RTL.h>
#include "dev_Bat_hsm.h"

 
/****************************************************************************************************//**
 *                        PRIVATE MEMBERS
 ********************************************************************************************************/

static T_DEV_Bat This;
static uint8_t ConvertToRange_0_4(uint32_t value);

/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/****************************************************************************************************//**
 * @fun		dev_Bat_Init
 * @brief	Provided interface used by the initial system loading to carry out Bat Device initialization
 * @return	
 **********************************************************************************/
uint8_t dev_Bat_Init(void){
	This.rawValue = 0;
	This.batLevel = BAT_LEVEL_MAX;
	This.numScans = 0;
	This.accValue = 0xFFFFFFFF; // forced max level.
	IDriverInit();
	return 0;
}

/****************************************************************************************************//**
 * @fun		dev_Bat_StartScan
 * @brief	Provided interface to start delayed scan process
 * @return	
 **********************************************************************************/
void dev_Bat_StartScan(void){
	// driver start scanning
	IDriverScan();
}

/****************************************************************************************************//**
 * @fun		dev_Bat_CancelScan
 * @brief	Provided interface to stop a scan process in progress
 * @return	
 **********************************************************************************/
void dev_Bat_CancelScan(void){
	IDriverCancel();
	This.numScans = 0;
	This.accValue = 0;
	// if cancel due to Stop mode, then force LO_Level to allow user operations after
	//wakeup.
	if(This.batLevel == BAT_LEVEL_MIN){
		This.batLevel = BAT_LEVEL_LO;	
	}
}

/****************************************************************************************************//**
 * @fun		dev_Bat_NewValue
 * @brief	Provided interface used by the ADC driver to notify key events
 * @return	
 **********************************************************************************/
void dev_Bat_NewValue(void){
	This.rawValue = IGetValue();
	This.accValue += This.rawValue;
	if(++This.numScans >= 4){
		This.numScans = 0;
		This.batLevel = ConvertToRange_0_4(This.accValue);
		This.accValue = 0;
	}

}

/****************************************************************************************************//**
 * @fun		dev_Bat_GetLevel
 * @brief	Provided interface used by tha application to read scanned data
 * @return	
 **********************************************************************************/
uint8_t dev_Bat_GetLevel(void){		
	return(This.batLevel);
}


/****************************************************************************************************//**
 * @fun		ConvertToRange_0_4
 * @brief	Converts acc raw ADC data into 5 levels (0..4)
 * @return	Battery level
 **********************************************************************************/
static uint8_t ConvertToRange_0_4(uint32_t value){
	// div value by 4 and check cross levels
	value = value >> 2;
	if(value > BAT_CROSS_MAX_HI){
		return BAT_LEVEL_MAX;
	}
	if(value > BAT_CROSS_HI_MED){
		return BAT_LEVEL_HI;
	}
	if(value > BAT_CROSS_MED_LO){
		return BAT_LEVEL_MED;
	}
	if(value > BAT_CROSS_LO_MIN){
		return BAT_LEVEL_LO;
	}
	return BAT_LEVEL_MIN;
}

