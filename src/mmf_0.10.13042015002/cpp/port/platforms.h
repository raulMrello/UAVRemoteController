/*
 * platforms.h
 *
 *  Created on: 12/3/2015
 *      Author: raulMrello
 *
 *  This header file is the linking point between the MMF_CPP kernel and the platform
 *  porting. Modify this file accordingly with your needs to delegate these public
 *  macros to well defined platform specific methods/macros.
 */

#ifndef SRC_PORT_PLATFORMS_H_
#define SRC_PORT_PLATFORMS_H_


/** \def PLATFORM_TIMER_START
 *  \brief PLATFORM_TIMER_START external function provides the way to start a platform specific
 *  timer. This function must be implemented for each platform porting
 *  \param delay Delay in microseconds
 *  \param timerCb Callback to be called on timer timeout
 *  \param handler Object reference to pass as argument of the timer callback
 */
#define PLATFORM_TIMER_START(delay_us, timer_callback, callback_handler)
//extern void PLATFORM_TIMER_START(int delay, void(*timerCb)(void*), void* handler);


/** \def PLATFORM_TIMER_STOP
 *  \brief PLATFORM_TIMER_STOP external function provides the way to stop a platform specific
 *  timer. This function must be implemented for each platform porting
 *  \param handler Object reference assigned to the specific timer callback
 */
#define PLATFORM_TIMER_STOP(callback_handler)
//extern void PLATFORM_TIMER_STOP(void* handler);

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




#endif /* SRC_PORT_PLATFORMS_H_ */
