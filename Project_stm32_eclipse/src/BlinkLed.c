//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#include "BlinkLed.h"

// ----------------------------------------------------------------------------

void
blink_led_init()
{
  // Enable GPIO Peripheral clock
  RCC_APB2PeriphClockCmd(BLINK_RCC_MASKx(BLINK_PORT_NUMBER), ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;

  // Configure pin in output push/pull mode
  GPIO_InitStructure.GPIO_Pin = BLINK_PIN_MASK(BLINK_PIN_NUMBER);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(BLINK_GPIOx(BLINK_PORT_NUMBER), &GPIO_InitStructure);

  // Start with led turned off
  blink_led_off();
}

// ----------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//-- PRIVATE MEMBERS -----------------------------------------------------------------
//------------------------------------------------------------------------------------

static char led = 0;
static Exception e = Exception_INIT();

//------------------------------------------------------------------------------------
//-- IMPLEMENTATION ------------------------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
void BlinkLedTask_init(BlinkLedTaskPtr t){
	led = 0;
	blink_led_init();
	Task_suspend(t, 1000000, &e);
	catch(&e){
		Exception_clear(&e);
	}
}

//------------------------------------------------------------------------------------
void BlinkLedTask_OnYieldTurn(BlinkLedTaskPtr t){
}

//------------------------------------------------------------------------------------
void BlinkLedTask_OnResume(BlinkLedTaskPtr t){
	if(led){
		blink_led_off();
		led = 0;
	}
	else{
		blink_led_on();
		led = 1;
	}
	Task_suspend(t, 1000000, &e);
	catch(&e){
		Exception_clear(&e);
	}
}

//------------------------------------------------------------------------------------
void BlinkLedTask_OnEventFlag(BlinkLedTaskPtr t, int event){
}

//------------------------------------------------------------------------------------
void BlinkLedTask_OnTopicUpdate(BlinkLedTaskPtr t, TopicData * td){
}


