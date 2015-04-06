/*************************************************************************//**
 * @file    drv_ADC.c
 * @mcu		STM32F1x
 * @brief   ADC peripheral controller to read battery voltage status  
 * @date    15.06.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include "drv_ADC.h"


/****************************************************************************************************//**
 *                        	REQUIRED INTERFACES
 ********************************************************************************************************/
#define IDataEvent()		drv_ADC_Requires_IDataEvent()
 
/****************************************************************************************************//**
 *                        PRIVATE MEMBERS
 ********************************************************************************************************/


static volatile uint16_t ADC1ConvertedValue;

/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/****************************************************************************************************//**
 * @fun		drv_ADC_Init
 * @brief	Provided interface to initialize driver as single shot on ADC_Channel9
 * @return	n/a
 **********************************************************************************/
void drv_ADC_Init(void){
	
	ADC_InitTypeDef ADC_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;				// NVIC

	// ADCCLK = PCLK2/8 - 9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 

	// Enable ADC1 clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	// Config port B (INA9)
	//drv_GPIO_IN_FDX1_as_analog();
	//drv_GPIO_IN_FDX2_as_analog();

	// ADC1 configuration
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	// ADC1 regular channels configuration 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9,  1, ADC_SampleTime_239Cycles5);
	
	// Disable ADC end of conversion interrupts
	ADC_ITConfig (ADC1,ADC_IT_EOC,DISABLE); 

	// Enable ADC1
	ADC_Cmd(ADC1, ENABLE);

	// Enable ADC1 reset calibaration register
	ADC_ResetCalibration(ADC1);
	// Check the end of ADC1 reset calibration register
	while(ADC_GetResetCalibrationStatus(ADC1));

	// Start ADC1 calibaration
	ADC_StartCalibration(ADC1);
	// Check the end of ADC1 calibration 
	while(ADC_GetCalibrationStatus(ADC1));

	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Disable ADC1
	ADC_Cmd(ADC1, DISABLE);
}

/****************************************************************************************************//**
 * @fun		drv_ADC_Scan
 * @brief	Provided interface to start a single shot measure
 * @return	n/a
 **********************************************************************************/
void drv_ADC_Scan(void){
	uint8_t i;

	// Enable ADC1 (ADON = 1) 1º time
	ADC_Cmd(ADC1, ENABLE);

	// blocking delay tstab
	for(i=0;i<100;i++);

	// Clear ADC1 EOC pending interrupt bit
	ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);

	// Enable ADC end of conversion interrupts
	ADC_ITConfig (ADC1,ADC_IT_EOC,ENABLE); 

	// Enable ADC1 (ADON = 1) 2º time (start single scan)
	ADC_Cmd(ADC1, ENABLE);
}

/****************************************************************************************************//**
 * @fun		drv_ADC_Cancel
 * @brief	Provided interface to stop a single shot measure
 * @return	n/a
 **********************************************************************************/
void drv_ADC_Cancel(void){
	// Disable ADC1
	ADC_Cmd(ADC1, DISABLE);
	// Disable ADC end of conversion interrupts
	ADC_ITConfig (ADC1,ADC_IT_EOC,DISABLE); 
}

/****************************************************************************************************//**
 * @fun		drv_ADC_GetValue
 * @brief	Provided interface to get last data
 * @return	n/a
 **********************************************************************************/
uint16_t drv_ADC_GetValue(void){	
	return(ADC1ConvertedValue);
}



/****************************************************************************************************//**
 * @fun		ADC1_2_IRQHandler
 * @brief	ADC1 interrupt handler
 * @return	n/a
 **********************************************************************************/
void ADC1_2_IRQHandler(void)
{
	// Clear ADC1 EOC pending interrupt bit
	ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
	// stores sampled conversion value into static variable
	ADC1ConvertedValue = ADC_GetConversionValue(ADC1);
	//Notify EOC
	IDataEvent();	
}
