# UAV REMOTE CONTROLLER

This projects is intended to build a remote controller for a UAV. It will sends commands/data to a telemetry receiver in order to control UAV movements and position.
 
## *Operation*

This device has a basic operation:	

1. Read a GPS stream from a GPS module connected to the serial port.
2. Reads input selector commands to set UAV position
3. Sends both GPS+input data to a remote telemetry receiver
4. Receive heartbeat signals from the telemetry receiver and notify with a sound if communication is lost.
 
## *Changelog*

> 23.03.2015-001  

- Project startup with common Wifi+TCP source code
