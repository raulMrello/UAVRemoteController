# UAVRemoteController
The UAV Remote Controller is a remote controller for an UAV quadcopter, which includes a keypad for UAV positioning and a GPS module for
user reference location.

## Features
This controller has three main functions:

1. Translate user input commands (pushbuttons) into RC positioning signals for the UAV.
2. Capture user's gps location and transmit to UAV flight controller to allow waypoint navigation with aerial photography.
3. Receive alarms from the UAV, to manage critical situations, (like: inmediate landing, collision avoidance...).

## Contents
- /docs/TaskModel.odp: Sketch of the system firmware. Tasks, devices, and topics.
- /src/mmf_c/*: Multithreaded kernel code [see Github project](https://github.com/raulMrello/MMF_C)
- /src/app/*: Application source code (tasks, topics, etc...)
- /Project_linux_eclipse: development and test project under eclipse linux.
- /Project_windows_eclipse: development and test project under eclipse windows.
- /Project_stm32_eclipse: development stm32 project under eclipse (gnuarm).
- /Project_stm32_keil: development stm32 project under keil (uvision).

## Changelog

> 14.04.2015-004 branch: master --------------
 - See README.md in project "stm32_keil_cpp"
 
> 13.04.2015-001 branch: master --------------
 - Added project "stm32_keil_cpp"
 - See README.md in project "stm32_keil_cpp"
 
> 08.04.2015-002 branch: master --------------
 - See README.md in project "stm32_keil"
 
> 07.04.2015-001 branch: master --------------
 - See README.md in project "stm32_keil"

> 06.04.2015-003 branch: master --------------
 - Added "driver" link folder to stm32_eclipse from stm32_keil
 - Building Project_stm32_keil from old sources.

> 06.04.2015-002 branch: master --------------
 - New project structure (linux, windows, stm32_gnuarm, stm32_keil)
 - Building Project_stm32_keil from old sources.

> 06.04.2015-001 branch: master --------------
 - MMF updated to "0.8.06042015001" version.
 - Added SysTask to handle lowest priority tasks (in this case, to simulate systick events)
 - Modified OS_ALLOC invocation to simulate a 10ms systick timer

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



