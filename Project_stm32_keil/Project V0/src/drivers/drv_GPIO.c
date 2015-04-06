
#include "drv_GPIO.h"


/****************************************************************************************************//**
 *                        	PRIVATE MEMBERS
 ********************************************************************************************************/
void ExtInterruptConfig(void);
void drv_GPIO_config_pin(uint16_t pin,GPIO_TypeDef * port,GPIOMode_TypeDef mode,GPIOSpeed_TypeDef speed);

/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/


/**
 * @fun		drv_GPIO_config_pin
 * @brief	Gonfigures GPIO pin
 **/
void drv_GPIO_config_pin(uint16_t pin,GPIO_TypeDef * port,GPIOMode_TypeDef mode,GPIOSpeed_TypeDef speed){

	GPIO_InitTypeDef GPIO_InitStructure;

   	if (port == GPIOA){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
   	} else if (port == GPIOB){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
   	} else if (port == GPIOC){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
   	} else if (port == GPIOD){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 
   	} else if (port == GPIOE){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); 
   	} else if (port == GPIOF){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE); 
   	} else if (port == GPIOG){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE); 
   	}

   	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Mode = mode;
	GPIO_InitStructure.GPIO_Speed = speed;
	GPIO_Init(port, &GPIO_InitStructure);

}

/**
 * @fun		drv_GPIO_Init
 * @brief	Initializes GPIO pins
 * @return	n/a
 **/
void drv_GPIO_Init(void){

	drv_GPIO_config_pin(KEY_N_PIN,  KEY_N_PORT,  GPIO_Mode_IPU, GPIO_Speed_10MHz);
	drv_GPIO_config_pin(KEY_NE_PIN, KEY_NE_PORT, GPIO_Mode_IPU, GPIO_Speed_10MHz);
	drv_GPIO_config_pin(KEY_E_PIN,  KEY_E_PORT,  GPIO_Mode_IPU, GPIO_Speed_10MHz);
	drv_GPIO_config_pin(KEY_ES_PIN, KEY_ES_PORT, GPIO_Mode_IPU, GPIO_Speed_10MHz);
	drv_GPIO_config_pin(KEY_S_PIN,  KEY_S_PORT,  GPIO_Mode_IPU, GPIO_Speed_10MHz);
	drv_GPIO_config_pin(KEY_SW_PIN, KEY_SW_PORT, GPIO_Mode_IPU, GPIO_Speed_10MHz);
	drv_GPIO_config_pin(KEY_W_PIN,  KEY_W_PORT,  GPIO_Mode_IPU, GPIO_Speed_10MHz);
	drv_GPIO_config_pin(KEY_WN_PIN, KEY_WN_PORT, GPIO_Mode_IPU, GPIO_Speed_10MHz);

	drv_GPIO_config_pin(SEL_ARM_PIN, SEL_ARM_PORT, GPIO_Mode_IPU, GPIO_Speed_10MHz);
	drv_GPIO_config_pin(SEL_LOC_PIN, SEL_LOC_PORT, GPIO_Mode_IPU, GPIO_Speed_10MHz);
	drv_GPIO_config_pin(SEL_ALT_PIN, SEL_ALT_PORT, GPIO_Mode_IPU, GPIO_Speed_10MHz);
	drv_GPIO_config_pin(SEL_RTH_PIN, SEL_RTH_PORT, GPIO_Mode_IPU, GPIO_Speed_10MHz);
	
	drv_GPIO_config_pin(OUT_0_PIN, OUT_0_PORT, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
	drv_GPIO_config_pin(OUT_0_PIN, OUT_1_PORT, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);

	ExtInterruptConfig();
}

/****************************************************************************************************//**
 * @fun		drv_GPIO_readKeyboard
 * @brief	Read keyboard pins
 * @return	n/a
 **********************************************************************************/
int drv_GPIO_readKeyboard(void) {
	
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

/**
 * @fun		drv_GPIO_readSelectors
 * @brief	Read selector pins
 * @return	n/a
 **/
int drv_GPIO_readSelectors(void) {
	
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


/**
 * @fun		drv_GPIO_setOutput
 * @brief	Set output value
 * @return	n/a
 **/
void drv_GPIO_setOutput(uint8_t id, uint8_t value){
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
  
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);		
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);		
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource2);		
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);		
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);		
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);		

*** TENGO QUE VER COMO ASIGNAR LAS EXTI A LOS PULSADORES	
    
 	EXTI_StructInit(&EXTI_InitStructure);
  	EXTI_InitStructure.EXTI_Line = EXTI_Line0|EXTI_Line1|EXTI_Line2|EXTI_Line10|EXTI_Line11|EXTI_Line12|EXTI_Line13|EXTI_Line14;
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