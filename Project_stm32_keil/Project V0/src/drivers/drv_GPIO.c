/*
 * drv_GPIO.c
 *
 *  Created on: 07/04/2015
 *      Author: raulMrello
 *
 */
 
#include "drv_GPIO.h"


//------------------------------------------------------------------------------------
//-- PRIVATE DEFINITIONS -------------------------------------------------------------
//------------------------------------------------------------------------------------

/** Keyboard */

#define KEY_N_PIN  		GPIO_Pin_1
#define KEY_N_PORT		GPIOB

#define KEY_NE_PIN 		GPIO_Pin_2
#define KEY_NE_PORT		GPIOB

#define KEY_E_PIN  		GPIO_Pin_9
#define KEY_E_PORT		GPIOB

#define KEY_ES_PIN  	GPIO_Pin_10
#define KEY_ES_PORT		GPIOB

#define KEY_S_PIN  		GPIO_Pin_11
#define KEY_S_PORT		GPIOB

#define KEY_SW_PIN  	GPIO_Pin_12
#define KEY_SW_PORT		GPIOB

#define KEY_W_PIN  		GPIO_Pin_13
#define KEY_W_PORT		GPIOB

#define KEY_WN_PIN  	GPIO_Pin_14
#define KEY_WN_PORT		GPIOB

/** Mode selectors */

#define SEL_ARM_PIN  	GPIO_Pin_4
#define SEL_ARM_PORT	GPIOA

#define SEL_LOC_PIN  	GPIO_Pin_5
#define SEL_LOC_PORT	GPIOA

#define SEL_ALT_PIN  	GPIO_Pin_6
#define SEL_ALT_PORT	GPIOA

#define SEL_RTH_PIN  	GPIO_Pin_7
#define SEL_RTH_PORT	GPIOA

/** Outputs */

#define OUT_0_PIN 	 	GPIO_Pin_15
#define OUT_0_PORT		GPIOB

#define OUT_1_PIN 	 	GPIO_Pin_8
#define OUT_1_PORT		GPIOA

/** PWM */

#define PWM_0_PIN 	 	GPIO_Pin_0
#define PWM_0_PORT		GPIOB


//------------------------------------------------------------------------------------
//-- PRIVATE MEMBERS -----------------------------------------------------------------
//------------------------------------------------------------------------------------

static Exception e = Exception_INIT();
static KEY_TOPIC_DATA_T keyTopicData;
static OUT_TOPIC_DATA_T outTopicData;
static Topic * keyTopic;
static Topic * outTopic;

/** \fun configPin
 *  \brief Gonfigures GPIO pin
 */
static void configPin(uint16_t pin,GPIO_TypeDef * port,GPIOMode_TypeDef mode,GPIOSpeed_TypeDef speed);

/** \fun readInputs
 *  \brief Read keyboard and selector pins
 *  \return Bitmask combination of the input pins status
 */
static int readInputs(void);

/** \fun setOutput
 *  \brief Set the output to a logic value
 *  \param id OutputId (0 or 1)
 *  \return value Output logic value (0 or 1)
 */
static void setOutput(int id, int value);


//------------------------------------------------------------------------------------
//-- IMPLEMENTATION  -----------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
static void configPin(uint16_t pin,GPIO_TypeDef * port,GPIOMode_TypeDef mode,GPIOSpeed_TypeDef speed){
	GPIO_InitTypeDef GPIO_InitStructure;
   	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Mode = mode;
	GPIO_InitStructure.GPIO_Speed = speed;
	GPIO_Init(port, &GPIO_InitStructure);
}

//------------------------------------------------------------------------------------
static int readInputs(void) {
	int result = 0;
	if (GPIO_ReadInputDataBit(KEY_N_PORT,KEY_N_PIN)==0)
		result |= KEY_N;
	if (GPIO_ReadInputDataBit(KEY_NE_PORT,KEY_NE_PIN)==0)
		result |= KEY_NE;
	if (GPIO_ReadInputDataBit(KEY_E_PORT,KEY_E_PIN)==0)
		result |= KEY_E;
	if (GPIO_ReadInputDataBit(KEY_ES_PORT,KEY_ES_PIN)==0)
		result |= KEY_ES;
	if (GPIO_ReadInputDataBit(KEY_S_PORT,KEY_S_PIN)==0)
		result |= KEY_S;
	if (GPIO_ReadInputDataBit(KEY_SW_PORT,KEY_SW_PIN)==0)
		result |= KEY_SW;
	if (GPIO_ReadInputDataBit(KEY_W_PORT,KEY_W_PIN)==0)
		result |= KEY_W;
	if (GPIO_ReadInputDataBit(KEY_WN_PORT,KEY_WN_PIN)==0)
		result |= KEY_WN;
	if (GPIO_ReadInputDataBit(SEL_LOC_PORT,SEL_LOC_PIN)==0)
		result |= KEY_LOC;
	if (GPIO_ReadInputDataBit(SEL_ARM_PORT,SEL_ARM_PIN)==0)
		result |= KEY_ARM;
	if (GPIO_ReadInputDataBit(SEL_ALT_PORT,SEL_ALT_PIN)==0)
		result |= KEY_ALT;
	if (GPIO_ReadInputDataBit(SEL_RTH_PORT,SEL_RTH_PIN)==0)
		result |= KEY_RTH;
	return result;
}

//------------------------------------------------------------------------------------
static void setOutput(int id, int value){
	if(id == 0){
		if(value == 0)
			GPIO_ResetBits(OUT_0_PORT,OUT_0_PIN);
		else
			GPIO_SetBits(OUT_0_PORT,OUT_0_PIN);
	}
	if(id == 1){
		if(value == 0)
			GPIO_ResetBits(OUT_1_PORT,OUT_1_PIN);
		else
			GPIO_SetBits(OUT_1_PORT,OUT_1_PIN);
	}
}

//------------------------------------------------------------------------------------
void drv_GPIO_Init(Exception *e){

	/** Port clocks enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/** Keyboard selection pins */
	configPin(KEY_N_PIN,  KEY_N_PORT,  GPIO_Mode_IPU, GPIO_Speed_10MHz);
	configPin(KEY_NE_PIN, KEY_NE_PORT, GPIO_Mode_IPU, GPIO_Speed_10MHz);
	configPin(KEY_E_PIN,  KEY_E_PORT,  GPIO_Mode_IPU, GPIO_Speed_10MHz);
	configPin(KEY_ES_PIN, KEY_ES_PORT, GPIO_Mode_IPU, GPIO_Speed_10MHz);
	configPin(KEY_S_PIN,  KEY_S_PORT,  GPIO_Mode_IPU, GPIO_Speed_10MHz);
	configPin(KEY_SW_PIN, KEY_SW_PORT, GPIO_Mode_IPU, GPIO_Speed_10MHz);
	configPin(KEY_W_PIN,  KEY_W_PORT,  GPIO_Mode_IPU, GPIO_Speed_10MHz);
	configPin(KEY_WN_PIN, KEY_WN_PORT, GPIO_Mode_IPU, GPIO_Speed_10MHz);

	/** Mode Selector pins */
	configPin(SEL_ARM_PIN, SEL_ARM_PORT, GPIO_Mode_IPU, GPIO_Speed_10MHz);
	configPin(SEL_LOC_PIN, SEL_LOC_PORT, GPIO_Mode_IPU, GPIO_Speed_10MHz);
	configPin(SEL_ALT_PIN, SEL_ALT_PORT, GPIO_Mode_IPU, GPIO_Speed_10MHz);
	configPin(SEL_RTH_PIN, SEL_RTH_PORT, GPIO_Mode_IPU, GPIO_Speed_10MHz);
	
	/** Output pins */
	configPin(OUT_0_PIN, OUT_0_PORT, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
	GPIO_ResetBits(OUT_0_PORT,OUT_0_PIN);
	configPin(OUT_1_PIN, OUT_1_PORT, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
	GPIO_ResetBits(OUT_1_PORT,OUT_1_PIN);

	/** PWM pins */
	configPin(PWM_0_PIN, PWM_X_PORT, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
	configPin(PWM_1_PIN, PWM_X_PORT, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
	configPin(PWM_2_PIN, PWM_X_PORT, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
	configPin(PWM_3_PIN, PWM_X_PORT, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);

	/** EXTI configuration */
  	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource2);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);

	EXTI_StructInit(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0|EXTI_Line1|EXTI_Line2|EXTI_Line4|EXTI_Line5|EXTI_Line6|EXTI_Line7|EXTI_Line10|EXTI_Line11|EXTI_Line12|EXTI_Line13|EXTI_Line14;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/** NVIC configuration */
  	NVIC_InitTypeDef NVIC_InitStructure;

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

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/** Subscribe to OutputTopics update and attach callback function */
	outTopic = OutputTopic_getRef("/out", e);
	catch(e){
		return;
	}
	Topic_attach(outTopic, 0, e);
	catch(e){
		return;
	}
	// sets default value for topic handler
	outTopicData = (OUT_TOPIC_DATA_T){0, 0};
}

//------------------------------------------------------------------------------------
void drv_GPIO_OnTopicUpdate(void * obj, TopicData * td){
	(void)obj;	// param not used
	//topic checking
	if(td->id == (int)outTopic){
		outTopicData = *((OUT_TOPIC_DATA_T*)td->data);
		setOutput(outTopicData.output_id, outTopicData.value);
	}	
}

//------------------------------------------------------------------------------------
//-- INTERRUPTS  ---------------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
void EXTI1_IRQHandler(void){
   keyTopicData.keys = readInputs();
   EXTI_ClearFlag(EXTI_Line1);
   Topic_notify(outTopic, &keyTopicData, sizeof(KEY_TOPIC_DATA_T), 0, 0, &e);
}

//------------------------------------------------------------------------------------
void EXTI2_IRQHandler(void){
   keyTopicData.keys = readInputs();
   EXTI_ClearFlag(EXTI_Line2);
   Topic_notify(outTopic, &keyTopicData, sizeof(KEY_TOPIC_DATA_T), 0, 0, &e);
}

//------------------------------------------------------------------------------------
void EXTI3_IRQHandler(void){
   keyTopicData.keys = readInputs();
   EXTI_ClearFlag(EXTI_Line3);
   Topic_notify(outTopic, &keyTopicData, sizeof(KEY_TOPIC_DATA_T), 0, 0, &e);
}

//------------------------------------------------------------------------------------
void EXTI4_IRQHandler(void){
   keyTopicData.keys = readInputs();
   EXTI_ClearFlag(EXTI_Line4);
   Topic_notify(outTopic, &keyTopicData, sizeof(KEY_TOPIC_DATA_T), 0, 0, &e);
}

//------------------------------------------------------------------------------------
void EXTI9_5_IRQHandler(void){
   keyTopicData.keys = readInputs();
   EXTI_ClearFlag(EXTI_Line5|EXTI_Line6|EXTI_Line7|EXTI_Line8|EXTI_Line9);
   Topic_notify(outTopic, &keyTopicData, sizeof(KEY_TOPIC_DATA_T), 0, 0, &e);
}


//------------------------------------------------------------------------------------
void EXTI15_10_IRQHandler(void){
   keyTopicData.keys = readInputs();
   EXTI_ClearFlag(EXTI_Line10|EXTI_Line11|EXTI_Line12|EXTI_Line13|EXTI_Line14);
   Topic_notify(outTopic, &keyTopicData, sizeof(KEY_TOPIC_DATA_T), 0, 0, &e);
}


