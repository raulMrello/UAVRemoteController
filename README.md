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

##### 25.06.2015-001 
- C�digo terminado. Depurando funcionamiento general.
- Reduzco la espera en VirtualReceiver::WaitingData.
- Corrijo error en el driver del Buzzer. Ya funciona!!
- POR HACER:
  - [opcional] La tarea GPS hay que modificarla para que s�lo env�e datos cuando haya alg�n cambio significativo.
  - A veces el socket se queda pillado. Una vez que est� todo terminado, hacer una variante para abrir_socket+enviar+recibir_ack+cerrar.

  
##### 23.06.2015-001 
- C�digo terminado. Depurando funcionamiento general.
- POR HACER:
  - NO PITA EL BUZZER.
  - Hay mucha espera en el estado VirtualReceiver::WaitingData. Ver la forma de que al recibir datos se cancele la espera.
  - La tarea GPS hay que modificarla para que s�lo env�e datos cuando haya alg�n cambio significativo.
  - A veces el socket se queda pillado. Una vez que est� todo terminado, hacer una variante para abrir_socket+enviar+recibir_ack+cerrar.

  
##### 22.06.2015-001 
- VirtualReceiver env�a y recibe datos. Modifico su protocolo de reinicio tras evError.
- Activo todo el c�digo. 
- POR HACER:
  - NO PITA EL BUZZER.
  - Revisar los topics entre los m�dulos haciendo un croquis en models.

  
##### 18.06.2015-001 
- Modificado VirtualReceiver y realizando depuraci�n paso a paso. Hay veces que lo resetea y pasa de _status 1 a 5. Ver por qu�. 
- Modificados los t�picos. Revisarlos todos y hacer un croquis.
- POR HACER:
  - NO PITA EL BUZZER.

  
##### 17.06.2015-002 
- Creo rama virtual_telem para modificar el comportamiento de VirtualTelemetry como una m�quina de estados. 
- M�dulo KeyDecoder, GpsReader, SysManager terminados!!!
- M�dulo VirtualReceiver funciona bien hasta el arranque del socket TCP y env�a el topic /ack la primera vez que establece conexi�n.
- POR HACER:
  - NO PITA EL BUZZER.
  - Modificar VirtualReceiver para que se comporte como una m�quina de estados y pueda gestionar el funcionamiento de forma m�s sencilla (hacer modelo).

  
##### 17.06.2015-001 
- M�dulo KeyDecoder, GpsReader, SysManager terminados!!!
- M�dulo VirtualReceiver funciona bien hasta el arranque del socket TCP y env�a el topic /ack la primera vez que establece conexi�n.
- POR HACER:
  - NO PITA EL BUZZER.
  - Modificar VirtualReceiver para que se comporte como una m�quina de estados y pueda gestionar el funcionamiento de forma m�s sencilla (hacer modelo).

  
##### 16.06.2015-001 
- El cambio de modo en SysManager funciona bien.
- Actualizo el modelo y el c�digo fuente para que s�lo env�e publicaciones (StManual, StFollow) si el flag confirmed es true.
- POR HACER:
  - NO PITA EL BUZZER.
  - Verificar m�dulo GPSReader y VirtualReceiver

  
##### 11.06.2015-003 
- Verificando SysManager (eventos KeyTimeout, ya funcionan seguir probando teclas). 
- POR HACER:
  - NO PITA EL BUZZER.
  - Verificar m�dulo GPSReader y VirtualReceiver

  
##### 11.06.2015-002 
- Modificado y verificado m�dulo [State](https://github.com/raulMrello/Cpp_HierarchicalStateMachines)
- Verificando SysManager (primer evento HoldB). NO PITA EL BUZZER.
- POR HACER:
  - Habilitar SysManager y ver el funcionamiento de LedFlasher, Beeper y la m�quina de estados.
  - Verificar m�dulo GPSReader y VirtualReceiver

  
##### 10.06.2015-001 (rama state)
- Modifico KeyDecoder (TERMINADO)
- Verificando SysManager (primer evento HoldB). NO PITA EL BUZZER.
- POR HACER:
  - Habilitar SysManager y ver el funcionamiento de LedFlasher, Beeper y la m�quina de estados.
  - Verificar m�dulo GPSReader y VirtualReceiver

  
##### 09.06.2015-002 (rama state)
- Modifico JoystickSampler para que se ejecute como un RtosTimer
- Modifico KeyDecoder para que implemente la clase JoystickSampler
- A�ado nuevos topics y modifico algunos de los ya creados
- Modifico SysManager y KeyDecoder para que la m�quina de estados quede perfectamente definida.
- Modifico el modelo .EAP
- POR HACER:
  - Habilitar �nicamente KeyDecoder y ver la m�quina de estados y generaci�n de topics.
  - Habilitar SysManager y ver el funcionamiento de LedFlasher, Beeper y la m�quina de estados.
  - Verificar m�dulo GPSReader y VirtualReceiver

  
##### 09.06.2015-001 
- Terminado SysManager (DEPURAR KeyDecoder y luego SysManager)
- POR HACER:
  - Rehacer m�quinas de estado pendientes y validar
  - Verificar m�dulo GPSReader
  - Definir m�quina de control de VirtualReceiver o verificar su funcionamiento general.

  
##### 08.06.2015-001 
- Terminada la validaci�n de las m�quinas de estados.
- Actualizo el modelo incluyendo TemplateHSM.
- Actualizo KeyDecoder
- Actualizando SysManager (state StManual)
- POR HACER:
  - Rehacer m�quinas de estado pendientes y validar
  - Verificar m�dulo GPSReader
  - Definir m�quina de control de VirtualReceiver o verificar su funcionamiento general.

  
##### 05.06.2015-001 
- Rehaciendo la gesti�n interna de los estados. Depurar los �ltimos cambios.
- Modifico main para probar el funcionamiento de las m�quinas de estados.
- POR HACER:
  - Rehacer todas las tareas con las nuevas m�quinas de estados.
  - Verificar m�dulo GPSReader
  - Definir m�quina de control de VirtualReceiver o verificar su funcionamiento general.

  
##### 03.06.2015-001 
- Creo el directorio /hsm-api con el c�digo de las nuevas m�quinas de estados. EN DEPURACION!!
	- DBG: Los estados se vuelven locos. Repasar la gesti�n de _state, _target y dem�s para que no se piren.
- Creo el m�dulo TemplateHsm que es una m�quina sencilla de 2 estados para probarlo.
- Modifico main para probar el funcionamiento de las m�quinas de estados.
- POR HACER:
  - Rehacer todas las tareas con las nuevas m�quinas de estados.
  - Verificar m�dulo GPSReader
  - Definir m�quina de control de VirtualReceiver o verificar su funcionamiento general.

  
##### 02.06.2015-001 
- Actualizo modelo SysManagerHSM PERO encuentro fallos de concepto en la herramienta.
- POR HACER:
  - Voy a rehacer el c�digo que utilizaba en el MandoAstro para utilizar m�quinas de estados con mbed.
  - Revisar y actualizar la m�quina de estados de KeyDecoder.
  - Verificar m�dulo GPSReader
  - Definir m�quina de control de VirtualReceiver o verificar su funcionamiento general.

  
##### 01.06.2015-001 
- Actualizo modelo SysManagerHSM (estado Follow)
- POR HACER:
  - Finalizar la m�quina de estados de SysManager.
  - Revisar y actualizar la m�quina de estados de KeyDecoder.
  - Verificar m�dulo GPSReader
  - Definir m�quina de control de VirtualReceiver o verificar su funcionamiento general.

  
##### 27.05.2015-001 
- Actualizo modelo SysManagerHSM
- Compruebo funcionamiento de los botones de los joysticks y corrijo bug en KeyDecoder. 
- POR HACER:
  - Finalizar la m�quina de estados de SysManager.
  - Revisar y actualizar la m�quina de estados de KeyDecoder.
  - Verificar m�dulo GPSReader
  - Definir m�quina de control de VirtualReceiver o verificar su funcionamiento general.

  
##### 26.05.2015-001 
- Actualizo modelo SysManagerHSM
- Actualizo comportamiento general. 
- Desarrollando Idle como subm�quina. No permite depuraci�n con lo que tengo que estudiar la opci�n de crear otro proyecto y asignarlo a Idle para poder depurarlo por si mismo.


##### 23.05.2015-001 
- Actualizo modelo SysManagerHSM
- Actualizo comportamiento general. 
- Desarrollando Idle como subm�quina. No permite depuraci�n con lo que tengo que estudiar la opci�n de crear otro proyecto y asignarlo a Idle para poder depurarlo por si mismo.


##### 22.05.2015-003 
- Actualizo modelo SysManagerHSM
- No funciona la propagaci�n de eventos de publicaci�n en modo Armed.


##### 22.05.2015-002 
- Elimino carpeta /yakindu-models


##### 22.05.2015-001 
- A�ado carpeta /models con los diferentes proyectos de yakindu con la generaci�n de las HSM
- A�ado c�digo fuente de la HSM de System Manager para integrarla cuando la necesite.
- A�ado c�digo fuente de la HSM del teclado (KeyDecoderHSM). PROBADO OK!!
- Modifico el topic KeyData_t a�adiendo la propiedad [bool isHold] para saber si la tecla est� en pulsaci�n larga.
- Modificado topic AlarmData_t. Cambia la estructura a�adiendo un c�digo de alarma, adem�s de los 8 bytes de datos adjuntos.
- Modifico KeyDecoder integrando la m�quina de estados KeyDecoderHSM
- Modifico JoystickSampler. Acepta alarma FORCE_DISARM. REVISAR !!!!!!
NEXT STEPS:
- Definir el funcionamiento de SysManagerHSM e integrarlo en SysManager
- Definir los modos de fallo de conexi�n TCP en VirtualReceiver.
- Probar lectura de tramas GPS
OPTIONAL:
- Volver a retomar el tema IDLE_serial ahora que los eventos se controlan correctamente. Si funcionara bien, eliminar _rxtmr en la gesti�n de datos entrantes y actualizar la rama mbed.

		
##### 20.05.2015-001
- Modificado topic AlarmData_t. Cambia la estructura a�adiendo un c�digo de alarma, adem�s de los 8 bytes de datos adjuntos.
- Modificado JoystickSampler. Acepta alarma FORCE_DISARM. Probar �ltimos cambios.	
- Modificado KeyDecoder. Env�a mensaje de alarma ALARM_FORCE_DISARM.
NEXT STEPS:
- Definir los modos de fallo de conexi�n TCP en VirtualReceiver.
- Probar lectura de tramas GPS
- Dise�ar SysManager y finalizar c�digo RC1
OPTIONAL:
- Volver a retomar el tema IDLE_serial ahora que los eventos se controlan correctamente. Si funcionara bien, eliminar _rxtmr en la gesti�n de datos entrantes y actualizar la rama mbed.

		
##### 19.05.2015-001
- Modificado JoystickSampler. Probar �ltimos cambios.	
NEXT STEPS:
- Habilitar comunicaci�n entre KeyDecoder y JoystickSampler para detectar un DISARM mediante la pulsaci�n de ambos joystick_OK al mismo tiempo.
- Definir los modos de fallo de conexi�n TCP en VirtualReceiver.
- Probar lectura de tramas GPS
- Dise�ar SysManager y finalizar c�digo RC1
OPTIONAL:
- Volver a retomar el tema IDLE_serial ahora que los eventos se controlan correctamente. Si funcionara bien, eliminar _rxtmr en la gesti�n de datos entrantes y actualizar la rama mbed.

		
##### 14.05.2015-001
- Habilito evento de control para chequear la conexi�n cada 10s.
- Cambio la gesti�n de CWLAP para no reenviar la petici�n hasta recibir un OK. Despu�s chequeo que existe la wifi y si no se encuentra, volver a intentar.
- VirtualReceiver:
- Modificar las respuestas del enlace wifi a�adiendo la solicitud y la respuesta con 0d0d0a0d0a antes del OK.		
NEXT STEPS:
- Definir qu� ocurre si dejo de recibir fallos de conexi�n TCP en VirtualReceiver.
- Testear el m�dulo JoystickSampler y modficarlo de acuerdo a c�mo deba comportarse.
- Volver a retomar el tema IDLE_serial ahora que los eventos se controlan correctamente. Si funcionara bien, eliminar _rxtmr en la gesti�n de datos entrantes y actualizar la rama mbed.

		
##### 13.05.2015-001
- Verificando funcionamiento del enlace wifi.
- VirtualReceiver:
  - Modificar las respuestas del enlace wifi a�adiendo la solicitud y la respuesta con 0d0d0a0d0a antes del OK.
  - A�adir gesti�n de la conexi�n temporizada. Cambiar nombre de _tmr por _tcptmr, habilitar evento y controlarlo.
  - Cambiar la gesti�n de CWLAP y no reenviar la petici�n hasta recibir un OK. Despu�s buscar la wifi y si no se encuentra, volver a intentar. Ir mirando el buffer de forma peri�dica para buscar la red.
  - Verificado funcionamiento del puerto serie, aunque no funciona la gesti�n de la interrupci�n IDLE para detectar el fin de trama. Actualmente lo hago con un RtosTimer.
NEXT STEPS:
- Volver a retomar el tema IDLE_serial ahora que los eventos se controlan correctamente. Si funcionara bien, eliminar _rxtmr en la gesti�n de datos entrantes y actualizar la rama mbed.

		
##### 12.05.2015-001
- Verificando funcionamiento de VirtualReceiver.
- Verificado funcionamiento del puerto serie, aunque no funciona la gesti�n de la interrupci�n IDLE para detectar el fin de trama. Actualmente lo hago con un RtosTimer.
- A�adido pull-request relativo a la gesti�n de eventos (OR) con Thread::signal_wait_or.
- Finalizado VirtualReceiver aunque falta depurar en detalle su funcionamiento.
NEXT STEPS:
- Cuidado con el descubrimiento de las redes wifi cercanas que puede desbordar mi buffer de recepci�n. Pensar c�mo gestionarlo.
- Probar m�dulos pendientes.

		
> 11.05.2015-001
	- Modificando main para ir probando cada m�dulo de forma independiente. Actualmente probando el GPS (no cambia el baudrate).
	- Finalizado VirtualReceiver aunque falta depurar en detalle su funcionamiento.
	NEXT STEPS:
		- Programar tarea de test en rama testing
		
> 08.05.2015-002
	- Modificando VirtualReceiver. Definiendo las ProtocolActions para simplificar la gesti�n de las comunicaciones.
	NEXT STEPS:
		- Programar tarea de test en rama testing
		
> 08.05.2015-001
	- Actualizo /docs a�adiendo comandos AT del ESP-01
	- Actualizo el ppt de resumen de montaje del RemoteController
	NEXT STEPS:
		- Programar ESP-01 en modo AT y verificar la comunicaci�n bidireccional
		- Definir los comandos del VirtualReceiver y finalizarlo
		- Programar tarea de test en rama testing
		
	
> 07.05.2015-001
	- Actualizo /docs a�adiendo software del ESP-01
	- A�ado librer�as GPS_UBLOX y GPS_NMEA.
	- Finalizo desarrollo de GpsReader.	
	- Finalizo desarrollo de JoystickSampler
	NEXT STEPS:
		- Programar ESP-01 en modo AT y verificar la comunicaci�n bidireccional
		- Definir los comandos del VirtualReceiver y finalizarlo
		- Programar tarea de test en rama testing
		
	
> 05.05.2015-001
	- Actualizo /docs a�adiendo pptx con la asignaci�n de pines.
	- A�ado comentarios en main.cpp con la nueva asignaci�n de pines
	NEXT STEPS:
		- A�adir m�dulo de lectura de joysticks (JoystickDecoder)
	
> 04.05.2015-003
	- Actualizo /docs a�adiendo documentaci�n de dise�o y montaje
	
> 04.05.2015-002 
	- Importo librer�a mbed de la rama [all_my_changes](https://github.com/raulMrello/mbed/tree/all_my_changes) utilizando el porting al F103C8T6.
	
> 04.05.2015-001
	- A�ado directorio para cmsis.h para solventar error en compilaci�n.
	
> 30.04.2015-001
	- A�ado directorio /docs/ublox con informaci�n sobre la configuraci�n del GPS y c�digo fuente de ejemplo.
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