/*
 * drv_TIM3.c
 *
 *  Created on: 07/04/2015
 *      Author: raulMrello
 *
 */
 
#include "drv_TIM3.h"

//------------------------------------------------------------------------------------
//-- PRIVATE DEFINITIONS -------------------------------------------------------------
//------------------------------------------------------------------------------------

#define DEFAULT_PREESCALER  36		// APB1 = SYSCLK/2, at 72MHz = 36MHz. Then 36MHz/36 = 1MHz clock
#define DEFAULT_PERIOD		250		// 250 ticks at 1Mhz(1us) = 250us => 4KHz (buzzer resonant freq)

//------------------------------------------------------------------------------------
//-- PRIVATE MEMBERS -----------------------------------------------------------------
//------------------------------------------------------------------------------------

static PWM_TOPIC_DATA_T pwmTopicData;
static Topic * pwmTopic;

/** \fun restart
 *  \brief Restarts the peripheral with a new period/duty configuration
 */
static void restart(void);

/** \fun updateDuty
 *  \brief Updates duty
 */
static void updateDuty(void);

//------------------------------------------------------------------------------------
//-- IMPLEMENTATION  -----------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
static void updateDuty(void){
	TIM_SetCompare3(TIM3, (uint16_t)((pwmTopicData.duty * pwmTopicData.period)/100));
}

//------------------------------------------------------------------------------------
static void restart(void){
  	/* TIM3 disable counter */
 	TIM_Cmd(TIM3, DISABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  		TIM_OCInitStructure;

	// Enable TIM3 clocks
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

//	// Configuramos los pines
//	GPIO_TIM3_as_timer();

	// Configuracion TIM
	TIM_TimeBaseStructure.TIM_Period = pwmTopicData.period;
	TIM_TimeBaseStructure.TIM_Prescaler = DEFAULT_PREESCALER;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
   	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
   	TIM_OCInitStructure.TIM_Pulse = 0;	
 
   	TIM_OC3Init(TIM3, &TIM_OCInitStructure); 
   	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
 	TIM_ARRPreloadConfig(TIM3, ENABLE);
 
   	/* TIM3 enable counter */
   	TIM_Cmd(TIM3, ENABLE);
	updateDuty();
}

//------------------------------------------------------------------------------------
void drv_TIM3_Init(Exception *e){

	/** Subscribe to OutputTopics update and attach callback function */
	pwmTopic = OutputTopic_getRef("/pwm", e);
	catch(e){
		return;
	}
	Topic_attach(pwmTopic, 0, e);
	catch(e){
		return;
	}
	// sets default value for topic handler (duty=0%) and stops PWM generation
	pwmTopicData.period = DEFAULT_PERIOD;
	pwmTopicData.duty = 0;
	pwmTopicData.queries = PWM_NO_QUERIES;
	restart();
}

//------------------------------------------------------------------------------------
void drv_TIM3_OnTopicUpdate(void * obj, TopicData * td){
	(void)obj;	// param not used
	//topic checking
	if(td->id == (int)pwmTopic){
		PWM_TOPIC_DATA_T * topic = (PWM_TOPIC_DATA_T*)td->data;
		if((topic->queries & PWM_SET_PERIOD) != 0){
			pwmTopicData.period = topic->period;
			restart();
		}
		if((topic->queries & PWM_SET_DUTY) != 0){
			pwmTopicData.duty = topic->duty;
			updateDuty();
		}
	}	
}


  
