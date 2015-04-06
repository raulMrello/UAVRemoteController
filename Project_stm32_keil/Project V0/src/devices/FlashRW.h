/*******************************************************************************
* Nombre de Archivo		: FlashRW.h
* Autor					: Jos� Alberto Villanueva
* Versi�n				: V1.0.0
* Fecha					: 03/08/2009
* Descripci�n			: 
*******************************************************************************/
#ifndef _FLASHRW_H
#define _FLASHRW_H

#include "Programas.h"
#include "Tunel.h"
#include "Maniobras.h"

/** Direcci�n de inicio de los datos de programas
laborales de los esdoni en flash*/
#define ProgLabAddress			0x807C000
/** Direcci�n de inicio de los datos de programas
laborales de los esdoni en flash*/
#define ProgFestAddress			0x807D800
/** Direcci�n de inicio de los datos de programas
laborales de los esdoni en flash*/
#define ProgVacAddress			0x807E000
/** Direcci�n de inicio de los datos de programas
laborales de los esdoni en flash*/
#define ProgTunelAddress		0x807E800
/** Direcci�n de inicio de los datos de programas
laborales de los esdoni en flash*/
#define ManiobrasAddress		0x807F000

/** Tama�o de los programas laborables*/
#define TamPagina		(2048)
/** Tama�o de los programas laborables*/
#define TamProgLab		(5760)
/** Tama�o de los programas festivo*/
#define TamProgFest		(960)
/** Tama�o de los programas vacaciones*/
#define TamProgVac		(960)
/** Tama�o de los programas de tune�*/
#define TamProgTunel	(352)
/** Tama�o de las maniobras*/
#define TamManiobras	(564)

/** Identificadores de comando*/
enum ID_CMD
{
	LEER_FLASH = 0,
	ESCRIBIR_FLASH,
};

//Prototipos de funciones
//---------------------------------------------------
void FlashRW_Formatear(void);
void FlashRW_ProgLaborables(tPrograma *pPrograma, unsigned char Cmd);
void FlashRW_ProgFestivos(tPrograma *pProgramaFest, unsigned char Cmd);
void FlashRW_ProgVacaciones(tPrograma *pProgramaVac, unsigned char Cmd);
void FlashRW_ProgTunel(tProgramaTunel *pPrograma, unsigned char Cmd);
void FlashRW_Maniobras(tManiobrasInstancias *pManiobras, unsigned char Cmd);
//---------------------------------------------------

#endif  //_FLASHRW_H
