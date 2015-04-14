/*
 * drv_TIM.c
 *
 *  Created on: 07/04/2015
 *      Author: raulMrello
 *
 */
 
#include "drv_TIM.h"

//------------------------------------------------------------------------------------
//-- PRIVATE TYPEDEFS ----------------------------------------------------------------
//------------------------------------------------------------------------------------


/** TIM data structure */
typedef struct  {
	TimFlags status; 	 	//!< tim status
	uint32_t period;    	//!< Next In index
	uint16_t duty;   		//!< Next Out index
}TimCtrl_t;

//------------------------------------------------------------------------------------
//-- PRIVATE DEFINITIONS -------------------------------------------------------------
//------------------------------------------------------------------------------------

#define DEFAULT_PREESCALER  36		// APB1 = SYSCLK/2, at 72MHz = 36MHz. Then 36MHz/36 = 1MHz clock
#define DEFAULT_PERIOD		250		// 250 ticks at 1Mhz(1us) = 250us => 4KHz (buzzer resonant freq)

//------------------------------------------------------------------------------------
//-- PRIVATE MEMBERS -----------------------------------------------------------------
//------------------------------------------------------------------------------------

static TimCtrl_t pwm;

/** \fun updateDuty
 *  \brief Updates duty
 */
static void updateDuty(TimCtrl_t * tc){
	TIM_SetCompare3(TIM3, (uint16_t)((tc->duty * tc->period)/100));
}

//------------------------------------------------------------------------------------
/** \fun restart
 *  \brief Restarts the peripheral with a new period/duty configuration
 */
static void restart(TimCtrl_t * tc){
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
	TIM_TimeBaseStructure.TIM_Period = tc->period;
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
	updateDuty(tc);
}

//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

drv_TIM drv_TIM_Init(uint8_t channel, uint32_t period, uint16_t duty){
	// by default only TIM3 as PWM generator is developed
	if(channel != 3)
		return 0;
	// sets default value for topic handler (duty=0%) and stops PWM generation
	pwm.period = DEFAULT_PERIOD;
	pwm.duty = 0;
	pwm.status = 0;
	restart(&pwm);
	return (drv_TIM)&pwm;
}

//------------------------------------------------------------------------------------
void drv_TIM_update(drv_TIM drv, uint16_t duty){
	TimCtrl_t * tc = (TimCtrl_t*)drv;
	tc->duty = duty;
	if(!duty)
		tc->status &= ~TIM_TIMMING;
	else
		tc->status |= TIM_TIMMING;
	
	restart((TimCtrl_t*)drv);
}

//------------------------------------------------------------------------------------
TimFlags drv_TIM_getStatus(drv_TIM drv){
	TimCtrl_t * tc = (TimCtrl_t*)drv;
	return tc->status;	
}
