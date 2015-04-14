# UAVRC_stm32_keil_cpp
This project is the stm32 implementation under KEIL uVision to develope and test final releases in C++.


## Features
This project is structured as follows:

- Active_Modules: user active module files (acts as Tasks) <MODULE>.cpp|h
- User_Libs: user library files lib_<FEATURE>.c|h
- MMF: multithreaded RTC-kernel [see Github project](https://github.com/raulMrello/MMF_C)
- Bsp_Drivers: board specific drivers files drv_<DRIVER>.c|h
- Sysconfig: system configuration files <device>_conf.h, <device>_it.c|h and entry point file main.cpp
- StdPeriph_Drivers: stm32 standard peripheral library files <device>_<periph>.c|h
- CMSIS-CORE: stm32f10x files startup_<device>.s, system_<device>.c|h, core_<cpu>.c|h

## Contents
Important project files are placed on next folders:

- Project V0: Keil project files incluiding (Active_Modules, Topics, User_Libs, Bsp_Drivers, Sysconfig files).
- Libraries: Stm32 and CMSIS files
- Cubemx: Board description and pinout configuration
- Models: UML models explaining UAVRC behaviour.

## Changelog

> 14.04.2015-002 branch: master --------------
 - Added GpsTopic module
 - Finished GpsReader module
 - NEXT STEPS:
	- Define Lib_GPS module content and libgps_ProcessData public primitive.
	- Code KeyReader active module (and also KeyTopic)
	- Code Router active module
	- Code SysManager active module

> 14.04.2015-001 branch: master --------------
 - Designing Active modules (GpsReader)
 - Project configured with MicroLib reduces huge quantity of footprint
 - NEXT STEPS:
	- Keep coding

> 13.04.2015-002 branch: master --------------
 - Redesigned drivers. Unified drv_UART
 - Loaded MMF release 0.10.13042015003
 - NEXT STEPS:
	- Add tasks (LoggerTask in stead Link for serial debug trace)
	- Add code to main

> 13.04.2015-001 branch: master --------------
 - Redefine drivers with callback installation in stead of topics
 - Redefine modules in C++
 - NEXT STEPS:
	- Unify drv_UART1 and drv_UART2 in a common drv_UART.
	- Add tasks (LoggerTask in stead Link for serial debug trace)
	- Add code to main

> 08.04.2015-002 branch: master --------------
 - Updated all topics modules. Added header comments.
 - Modified drv_UART to catch SEND requests.
 - NEXT STEPS:
	- Unify drv_UART1 and drv_UART2 in a common drv_UART.
	- Add tasks (LoggerTask in stead Link for serial debug trace)
	- Add code to main

> 08.04.2015-001 branch: master --------------
 - Removed all unused modules from project and from filesystem.
 - Finished drivers and topics
 - NEXT STEPS:
	- Add tasks (LoggerTask in stead Link for serial debug trace)
	- Add code to main

> 07.04.2015-001 branch: master --------------
 - Done drv_GPIO, drv_UART1, drv_UART2
 - Adapting module drv_TIM3 for PWM output
 - Changed lines PB0 (now TIM3CH3 as pwm) and now PB9 is new input.
 - PENDING:
	- Remove old interfaces, unused drivers, and modules

> 06.04.2015-002 branch: master --------------
 - MMF updated to "0.8.06042015001" version loaded.
 - Modifying drv_GPIO. Must check EXTI assignment to input pins.
 - PENDING:
	- Rest of drivers.
	- Remove old interfaces.

> 05.04.2015-001 branch: master --------------
 - Modified README.md location in linux-eclipse project.
 - Create SysTask to handle lowest priority processes (power management, test cases...).
 KNOWN ISSUES:
 - MMF should be modified to handle timer objects internally.

> 04.04.2015-001 branch: master --------------
 - Added ALT and RTH modes to InputTopics and DataTopics. Also handled in RcTask.
 - Using GpsTask for testing RcTask. Once done, create SysTask to handle whole device management and test cases.
 - Modules GpsTask and CommTasks with default content must be designed.
 KNOWN ISSUES:
 - MMF should be modified to handle task suspensions with predefined timed ticks as other RTOSes.

> 02.04.2015-001 branch: master --------------
 - Using GpsTask for testing RcTask. Once done, create SysTask to handle whole device management and test cases.
 - Modules GpsTask and CommTasks with default content must be designed.

> 01.04.2015-001 branch: master --------------
 - Module RcTask finished (but NOT_TESTED)
 - Sets modules GpsTask and CommTasks to default content and test RcTask execution simulating keyboard events

> 31.03.2015-002 branch: master --------------
 - Defined RC_TOPIC_DATA_T structure for /rc topic updates
 - Task RcTask must be redefined to manage key values according with mode in order to generate /rc topic updates
 - Rest of tasks and topics should be done.

> 31.03.2015-001 branch: master --------------
 - Topics: add getRef to all topics
 - Designing RcTask keycodes
 - PENDING: All tasks.

> 30.03.2015-001 branch: master --------------
 - Adding topics and tasks.
 - InputTopics and UartTopics done.
 - PENDING: DataTopics and all tasks.

> 27.03.2015-001 branch: master --------------
 - Project starts.



