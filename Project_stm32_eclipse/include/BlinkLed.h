//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#ifndef BLINKLED_H_
#define BLINKLED_H_

#include "stm32f10x.h"

// ----- LED definitions ------------------------------------------------------

// Adjust these definitions for your own board.

// Olimex STM32-H103 definitions (the GREEN led, C12, active low)
// (SEGGER J-Link device name: STM32F103RB).

// Port numbers: 0=A, 1=B, 2=C, 3=D, 4=E, 5=F, 6=G, ...
#define BLINK_PORT_NUMBER               (0)
#define BLINK_PIN_NUMBER                (0)
#define BLINK_ACTIVE_LOW                (0)

#define BLINK_GPIOx(_N)                 ((GPIO_TypeDef *)(GPIOA_BASE + (GPIOB_BASE-GPIOA_BASE)*(_N)))
#define BLINK_PIN_MASK(_N)              (1 << (_N))
#define BLINK_RCC_MASKx(_N)             (RCC_APB2Periph_GPIOA << (_N))
// ----------------------------------------------------------------------------

extern void blink_led_init(void);

// ----------------------------------------------------------------------------

inline void blink_led_on(void);

inline void blink_led_off(void);

// ----------------------------------------------------------------------------

inline void __attribute__((always_inline)) blink_led_on(void) {
	#if (BLINK_ACTIVE_LOW)
	GPIO_ResetBits(BLINK_GPIOx(BLINK_PORT_NUMBER), BLINK_PIN_MASK(BLINK_PIN_NUMBER));
	#else
	GPIO_SetBits(BLINK_GPIOx(BLINK_PORT_NUMBER), BLINK_PIN_MASK(BLINK_PIN_NUMBER));
	#endif
}

inline void __attribute__((always_inline)) blink_led_off(void) {
	#if (BLINK_ACTIVE_LOW)
	GPIO_SetBits(BLINK_GPIOx(BLINK_PORT_NUMBER), BLINK_PIN_MASK(BLINK_PIN_NUMBER));
	#else
	GPIO_ResetBits(BLINK_GPIOx(BLINK_PORT_NUMBER), BLINK_PIN_MASK(BLINK_PIN_NUMBER));
	#endif
}

// ----------------------------------------------------------------------------
#include "../../src/mmf_c/os/mmf.h"

/** \def BlinkLedTaskPtr
 *  \brief Definition to abstract the callback handler into void* type
 */
typedef void* BlinkLedTaskPtr;

/** \fn BlinkLedTask_init
 *  \brief BlinkLedTask initialization callback
 *  \param t BlinkLedTask task object
 */
void BlinkLedTask_init(BlinkLedTaskPtr t);

/** \fn BlinkLedTask_OnYieldTurn
 *  \brief BlinkLedTask on yield turn events callback
 *  \param t BlinkLedTask task object
 */
void BlinkLedTask_OnYieldTurn(BlinkLedTaskPtr t);

/** \fn BlinkLedTask_OnResume
 *  \brief BlinkLedTask on resume events callback
 *  \param t BlinkLedTask task object
 */
void BlinkLedTask_OnResume(BlinkLedTaskPtr t);

/** \fn BlinkLedTask_OnEventFlag
 *  \brief BlinkLedTask on event flags callback
 *  \param t BlinkLedTask task object
 *  \param event Event raised
 */
void BlinkLedTask_OnEventFlag(BlinkLedTaskPtr t, int event);

/** \fn BlinkLedTask_OnTopicUpdate
 *  \brief BlinkLedTask on topic update events callback
 *  \param t BlinkLedTask task object
 *  \param td Topic data
 */
void BlinkLedTask_OnTopicUpdate(BlinkLedTaskPtr t, TopicData * td);

#endif // BLINKLED_H_
