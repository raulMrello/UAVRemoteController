# UAVRC_mbed_stm32f10x_src
This is the project implementation under keil from mbed ide with mbed sources and rtos-rtx.

## Features
This controller has three main functions:

1. Translate user input commands (pushbuttons) into RC positioning signals for the UAV.
2. Capture user's gps location and transmit to UAV flight controller to allow waypoint navigation with aerial photography.
3. Receive alarms from the UAV, to manage critical situations, (like: inmediate landing, collision avoidance...).

## Contents
- /docs/TaskModel.odp: Sketch of the system firmware. Tasks, devices, and topics.
- /src/mmf_c/*: Multithreaded kernel code [see Github project](https://github.com/raulMrello/MMF_C)
- /src/app/*: Application source code
- /src/port/*: Platform specific source code

## Changelog

> 17.04.2015-002 
	- Release with mbed sources. In parallel project "Mbed_to_Keil" I work to strip most of the driver code.
	- NEXT STEPS:
		- Add  Keyboard task

> 17.04.2015-001
	- Added module MsgBroker (includes List)
	- Compile in: Program Size Code=26828 RO-data=1796 RW-data=368 ZI-data=3628  
	- NEXT STEPS:
		- Add  Keyboard task		

> 16.04.2015-001
	- Created