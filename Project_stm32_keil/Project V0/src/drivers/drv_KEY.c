/*************************************************************************//**
 * @file    drv_KEY.c
 * @mcu		STM32F1x
 * @brief   GPIO peripheral controller to manage physical keyboard keystrokes.
 * @date    15.06.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include "drv_KEY.h"		// includes drv_GPIO.h



/****************************************************************************************************//**
 *                        	REQUIRED INTERFACES
 ********************************************************************************************************/
#define IKeyEvent()				drv_KEY_Requires_IKeyEvent()

/****************************************************************************************************//**
 *                        	PRIVATE MEMBERS
 ********************************************************************************************************/
void ExtInterruptConfig(void);


/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/****************************************************************************************************//**
 * @fun		drv_KEYInit
 * @brief	Provided interface to initialize input pins for the keypad
 * @return	n/a
 **********************************************************************************/
void drv_KEY_Init(void){

	//Setup input pins

	drv_KEY_ESC_as_input();		
 	drv_KEY_TOP_as_input();		
 	drv_KEY_CENTER_as_input();		
 	drv_KEY_RIGHT_as_input();	
 	drv_KEY_LEFT_as_input();	
 	drv_KEY_BOTTON_as_input();

	ExtInterruptConfig();
}

/****************************************************************************************************//**
 * @fun		drv_KEYGetKey
 * @brief	Provided interface to read the actual keystroke configuration
 * @return	n/a
 **********************************************************************************/
int8_t drv_KEY_GetKey(void) {
	
	if (drv_KEY_ESC_is_on() == 0)
		return KEY_CANCEL;
	if (drv_KEY_TOP_is_on() == 0)
		return KEY_UP;
	if (drv_KEY_CENTER_is_on() == 0)
		return KEY_ENTER;
	if (drv_KEY_RIGHT_is_on() == 0)
		return KEY_RIGHT;
	if (drv_KEY_LEFT_is_on() == 0)
		return KEY_LEFT;
	if (drv_KEY_BOTTON_is_on() == 0)
		return KEY_DOWN;
	return -1;
}


/****************************************************************************************************//**
 * @fun		EXTI9_5_IRQHandler
 * @brief	keypad interrupt handler
 * @return	n/a
 **********************************************************************************/
void EXTI9_5_IRQHandler(void){
   IKeyEvent();
   EXTI_ClearFlag(EXTI_Line6|EXTI_Line7|EXTI_Line8|EXTI_Line9);
}


/****************************************************************************************************//**
 * @fun		EXTI15_10_IRQHandler
 * @brief	keypad interrupt handler
 * @return	n/a
 **********************************************************************************/
void EXTI15_10_IRQHandler(void){
   IKeyEvent();
   EXTI_ClearFlag(EXTI_Line10|EXTI_Line11);
}


/****************************************************************************************************//**
 * @fun		ExtInterruptConfig
 * @brief	Private func, to configure interrupt pin sources
 * @return	n/a
 **********************************************************************************/
void ExtInterruptConfig(){
  
  	EXTI_InitTypeDef EXTI_InitStructure;
  	NVIC_InitTypeDef NVIC_InitStructure;

 	// EXTI_DeInit();
  
  	//if (Button_Mode == Mode_EXTI)
  
  
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
  
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);		//COL1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);		//COL1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);		//COL1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);		//COL1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);		//COL1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);		//COL1
  
    
 	EXTI_StructInit(&EXTI_InitStructure);
  	EXTI_InitStructure.EXTI_Line = EXTI_Line6|EXTI_Line7|EXTI_Line8|EXTI_Line9|EXTI_Line10|EXTI_Line11;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);


   	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  	NVIC_Init(&NVIC_InitStructure); 

}

