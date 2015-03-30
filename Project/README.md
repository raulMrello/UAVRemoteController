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
- /src/app/*: Application source code
- /src/port/*: Platform specific source code

## Changelog

> 30.03.2015-001 branch: master --------------
 - Adding topics and tasks.
 - InputTopics and UartTopics done.
 - PENDING: DataTopics and all tasks.

> 27.03.2015-001 branch: master --------------
 - Project starts.


