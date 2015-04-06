//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include "diag/Trace.h"

/** required includes for SysTick handling events */
//#include "Timer.h"
#include "cmsis_device.h"
#include "cortexm/ExceptionHandlers.h"

#include "BlinkLed.h"
#include "../../src/mmf_c/os/mmf.h"
static Exception e = Exception_INIT();
// ----------------------------------------------------------------------------
//
// Standalone STM32F1 led blink sample (trace via NONE).
//
// In debug configurations, demonstrate how to print a greeting message
// on the trace device. In release configurations the message is
// simply discarded.
//
// Then demonstrates how to blink a led with 1Hz, using a
// continuous loop and SysTick delays.
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the NONE output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//
// The external clock frequency is specified as a preprocessor definition
// passed to the compiler via a command line option (see the 'C/C++ General' ->
// 'Paths and Symbols' -> the 'Symbols' tab, if you want to change it).
// The value selected during project creation was HSE_VALUE=8000000.
//
// Note: The default clock settings take the user defined HSE_VALUE and try
// to reach the maximum possible system clock. For the default 8MHz input
// the result is guaranteed, but for other values it might not be possible,
// so please adjust the PLL settings in system/src/cmsis/system_stm32f10x.c
//

// ----- Timing definitions -------------------------------------------------

// Keep the LED on for 2/3 of a second.
#define BLINK_ON_TICKS  (TIMER_FREQUENCY_HZ * 2 / 3)
#define BLINK_OFF_TICKS (TIMER_FREQUENCY_HZ - BLINK_ON_TICKS)

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int main(int argc, char* argv[]) {
	// Send a greeting to the trace device (skipped on Release).
	trace_puts("Hello ARM World!");

	// At this stage the system clock should have already been configured
	// at high speed.
	trace_printf("System clock: %uHz\n", SystemCoreClock);


	// 1 - Kernel allocation and initialization.
	//     This creates a static task list in the form: 	static Task* mmf_os_tasklist[2];
	//     And invokes to init member in the form:			OS_init(mmf_os_tasklist, 2, &e);
	//     Exception handling should be checked afterwards.

	OS_ALLOC(os_tasklist, 1, 10000, &e);
	catch(&e){
		Exception_clear(&e);
		return -1;
	}

	// 2- [optional] Topics initialization, it can be done in tasks implementation. Nevertheless is done
	//	  here for clarification to ensure that observerlist is not accidentally cleared.


	// 3 - Tasks creation, init callback is invoked internally.
	//     Exception checking should be done afterwards with "catch" macro
	//	   It allocates memory to get up to 5 topic updates pending. The aray is named: mmf_topicpool_TASKNAME

	Task ledtask;
	Task_initialize(&ledtask,
					"ledtask",
					PRIO_MAX,
					0,
					0,
					BlinkLedTask_init,
					BlinkLedTask_OnYieldTurn,
					BlinkLedTask_OnResume,
					BlinkLedTask_OnEventFlag,
					BlinkLedTask_OnTopicUpdate,
					&ledtask,
					&e);
	catch(&e){
		Exception_clear(&e);
		return -1;
	}

	// 4 - Kernel scheduling. Runs forever, else call OS_schedule_once
	//	   Exception handling should be done if scheduling finish
	//	   Systick timer is set to 10ms periodic interrupt

	SysTick_Config (SystemCoreClock / 100);
	OS_schedule(&e);
	catch(&e){
		Exception_clear(&e);
	}
	// Infinite loop, never return.
}


// ----- SysTick_Handler() ----------------------------------------------------
void SysTick_Handler (void) {
	OS_tick(&e);
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
