# UAVRC_mbed_stm32f10x_src
Implementaci�n en keil uv5 utilizando las fuentes de mbed y mbed-rtos

## Features
Las funciones principales del equipo son:

1. Convertir las pulsaciones del teclado en comandos RC para el receptor de telemetr�a
2. Obtener la localizaci�n gps y enviarla al receptor de telemetr�a
3. Recibir el estado del dron, as� como indicadores de alarma

## Contents
- ../docs/ Bocetos y dem�s
- /user-modues/ Software accesorio (MsgBroker, ...)
- /user-common/ Software que depende de la aplicaci�n (Topics, ...)
- /user-tasks/ Tareas 
- /mbed-src/ C�digo fuente del repositorio mbed-src
- /mbed-rtos/ C�digo fuente del repositorio mbed-rtos

## Changelog

> 30.04.2015-001 
	- *A�ado directorio /docs/ublox con informaci�n sobre la configuraci�n del GPS y c�digo fuente de ejemplo.*
	- Modificado m�dulo LedFlasher para que pueda controlar diversos canales led en paralelo.
	- Modificado m�dulo SysConfig para controlar 4 leds
	- Se a�aden nuevas salidas digitales para los 4 leds
	- Test y montaje del dispositivo (hardware):
		- Teclado:...........TEST(x) MONTADO( )
		- Puerto serie GPS:..TEST( ) MONTADO( )
		- Puerto serie Wifi:.TEST( ) MONTADO( )
		- Leds:..............TEST( ) MONTADO( )
		- Zumbador:..........TEST( ) MONTADO( )
	- NEXT STEPS:
		- Probar y montar el hardware
		- Configurar los GPS con la herramiento de UBLOX
		- Adaptar la librer�a gps con las tramas deseadas
		- Definir el funcionamiento b�sico y los casos de error
	- MBED ISSUES:
		- **Thread[.cpp|.h]**: added function *signal_clr* to clear consumed flags.
		- **pinmap.c**: modified *pin_mode* to setup pullups properly.

> 27.04.2015-003 
	- Modificado m�dulo LedFlasher para que pueda controlar diversos canales led en paralelo.
	- Modificado m�dulo SysConfig para controlar 4 leds
	- Se a�aden nuevas salidas digitales para los 4 leds
	- Test y montaje del dispositivo (hardware):
		- Teclado:...........TEST(x) MONTADO( )
		- Puerto serie GPS:..TEST( ) MONTADO( )
		- Puerto serie Wifi:.TEST( ) MONTADO( )
		- Leds:..............TEST( ) MONTADO( )
		- Zumbador:..........TEST( ) MONTADO( )
	- NEXT STEPS:
		- Probar y montar el hardware
		- Configurar los GPS con la herramiento de UBLOX
		- Adaptar la librer�a gps con las tramas deseadas
		- Definir el funcionamiento b�sico y los casos de error
	- MBED ISSUES:
		- **Thread[.cpp|.h]**: added function *signal_clr* to clear consumed flags.
		- **pinmap.c**: modified *pin_mode* to setup pullups properly.

> 27.04.2015-001 
	- Saved project in uv5 format
	- Testing modules:
		- Keyboard (x)
		- Serial GPS ( )
		- Serial LNK ( )
		- Output led ( )
		- Output buzzer ( )
	- NEXT STEPS:
		- Finish startup code
		- Write a functional spec
		- Test and configure GPS with UBLOX tool
		- Adapt lib_gps according with the NMEA/UBX reception
		- Define error conditions and refactor application tasks (/user-tasks/*)
		- Mount final assembly (keyboards, selectors, buzzer, led, battery, wifimodule)
	- MBED ISSUES:
		- **Thread[.cpp|.h]**: added function *signal_clr* to clear consumed flags.
		- **pinmap.c**: modified *pin_mode* to setup pullups properly.

> 24.04.2015-001 
	- Improving Tasks execution
	- Checking KeyDecoder
	- Solved gpio problems
	- NEXT STEPS:
		- Finish startup code
		- Write a functional spec
		- Test and configure GPS with UBLOX tool
		- Adapt lib_gps according with the NMEA/UBX reception
		- Define error conditions and refactor application tasks (/user-tasks/*)
		- Mount final assembly (keyboards, selectors, buzzer, led, battery, wifimodule)
	- MBED ISSUES:
		- **Thread[.cpp|.h]**: added function *signal_clr* to clear consumed flags.
		- **pinmap.c**: modified *pin_mode* to setup pullups properly.

> 23.04.2015-001 
	- Removed unused projects (eclipse, linux, ....)
	- Debugging code with stm32 platform (PROBLEMS reading InterruptIn pins).
	- Seems that gpio_api is not setting pullups properly... check it!!!
	- NEXT STEPS:
		- Write a functional spec
		- Test and configure GPS with UBLOX tool
		- Adapt lib_gps according with the NMEA/UBX reception
		- Define error conditions and refactor application tasks (/user-tasks/*)
		- Mount final assembly (keyboards, selectors, buzzer, led, battery, wifimodule)

> 21.04.2015-001 
	- Added VirtualReceiver, LedFlasher
	- Main core functions added!!
	- NEXT STEPS:
		- Write a functional spec
		- Test and configure GPS with UBLOX tool
		- Adapt lib_gps according with the NMEA/UBX reception
		- Define error conditions and refactor application tasks (/user-tasks/*)
		- Mount final assembly (keyboards, selectors, buzzer, led, battery, wifimodule)

> 20.04.2015-001 
	- Designing VirtualReceiver
	- Added KeyDecoder, SysManager and BeepGenerator.
	- NEXT STEPS:
		- Finish VirtualReceiver

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