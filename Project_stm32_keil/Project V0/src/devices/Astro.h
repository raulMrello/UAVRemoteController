/*******************************************************************************
* File Name          : Astro.h
* Author             : José Albertoi Villanueva
* Version            : V1.0.0
* Date               : 27/07/2009
* Description        : Archivo de cabecera para Astro.c
*******************************************************************************/
#ifndef ASTRO_H
#define ASTRO_H

#include "Calendario.h"
#include "Reloj.h"

#define PI		(3.14159265)
#define RAD		(180/PI)
#define VP		(8.22E-5)
#define M0		(2.12344)
#define MN		(1.72019E-2)
#define T0		(2444000.5)
#define MR		(0.04301)
#define F		(13750.987)

/** Estructura para manejar coordenadas de posición*/
typedef struct
{
	short	Grados;
	char	Minutos;
	char	Segundos;
} tAstroCoordenada;

//Prototipos de funciones
//----------------------------------------------------------------------------------
void Astro_OrtoOcaso(tCalendarioFecha* pFecha, tAstroCoordenada *pLatitud, tAstroCoordenada *pLongitud,
					  tRelojHora *pHoraOrto, tRelojHora *pHoraOcaso);
//----------------------------------------------------------------------------------

#endif //ASTRO_H
