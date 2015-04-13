# UAVRC_windows_eclipse
This project is the windows implementation under Eclipse CDT to develope and test different platform-agnostic modules like: tasks, topics, ...

In this case, SysTask is the lowest priority task and acts as a platform simulator. In fact, it simulates: critical sections, systick timer and more.

## Features
This project includes next modules:

1. RcTask, manages user inputs, like pushbuttons and switch selectors. According with those inputs, this module generates /rc topics to be send to the TelemetryReceiver.
2. GpsTask, reads NMEA/UBX streams from the serial GPS module. It extract useful information and generates /gps topics for the TelemetryReceiver.
3. CommTask, receives /rc and /gps topics from the other modules and sends them to the TelemetryReceiver. Also it receives /ack topics from the TelemetryReceiver

## Contents
- ../src/mmf_c/*: Multithreaded kernel code [see Github project](https://github.com/raulMrello/MMF_C)
- ../src/app/*: Application source code (tasks, topics, ...)

## Changelog

> 06.04.2015-002 branch: master --------------
 - MMF updated to "0.8.06042015001" version.
 - Added SysTask to handle lowest priority tasks (in this case, to simulate systick events)
 - Modified OS_ALLOC invocation to simulate a 10ms systick timer
 - Created project under eclipse windows OK!

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


