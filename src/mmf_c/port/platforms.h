/*
 * Timer.h
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 */

#ifndef SRC_PORT_TIMER_H_
#define SRC_PORT_TIMER_H_


/** \def PLATFORM_ENTER_CRITICAL
 *  \brief PLATFORM_ENTER_CRITICAL disables all interrupts to enter into a critical section
 */
#define PLATFORM_ENTER_CRITICAL()
//extern void PLATFORM_ENTER_CRITICAL(void);

/** \def PLATFORM_EXIT_CRITICAL
 *  \brief PLATFORM_EXIT_CRITICAL re-enables all interrupts to exit from a critical section
 */
#define PLATFORM_EXIT_CRITICAL()
//extern void PLATFORM_EXIT_CRITICAL(void);


#if defined(PLATFORM_KL05Z)
#include "common.h"
#include <stdint.h>
#endif

#if defined(PLATFORM_GENERIC)
#include <stdio.h>	///< required for printf functions
#endif


#endif /* SRC_PORT_TIMER_H_ */
