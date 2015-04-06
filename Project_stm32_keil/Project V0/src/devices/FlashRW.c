/*******************************************************************************
* Nombre de Archivo		: FlashRW.c
* Autor					: José Alberto Villanueva
* Versión				: V1.0.0
* Fecha					: 03/08/2009
* Descripción			: 
*******************************************************************************/

// Includes
//--------------------------------------------------------
#include "stm32f10x.h"
#include <string.h>
#include "Definiciones.h"
#include "FlashRW.h"


/**
*******************************************************************************
- Nombre de Función	:	FlashRW_Formatear
- Descripción		:	Formatea el espacio de la flash reservado al guardado
						de datos.
- Parámetros		:
- Salidas			:	No
- Valor devuelto	:	No
*******************************************************************************/
void FlashRW_Formatear(void)
{
	FLASH_Unlock();

	FLASH_ErasePage( (u32)(ProgLabAddress) );												 
	FLASH_ErasePage( (u32)(ProgLabAddress + TamPagina) );												 
	FLASH_ErasePage( (u32)(ProgLabAddress + TamPagina + TamPagina) );												 
	FLASH_ErasePage( (u32)(ProgFestAddress) );
	FLASH_ErasePage( (u32)(ProgVacAddress) );
	FLASH_ErasePage( (u32)(ProgTunelAddress) );
	FLASH_ErasePage( (u32)(ManiobrasAddress) );
}

/**
*******************************************************************************
- Nombre de Función	:	FlashRW_ProgLaborables
- Descripción		:	
- Parámetros		:
						- pMem: puntero al origen de datos en memoria
						- pFlash: puntero al destino de los datos en flash
						- Num: número de palabras a escribir
- Salidas			:	No
- Valor devuelto	:	No
*******************************************************************************/
static void FlashRW_EscribirBloque(unsigned int *pMem, unsigned int *pFlash, unsigned int Num)
{
	unsigned int index;
	unsigned int pAuxFlash,
				 *pAuxMem;

	pAuxFlash = (unsigned int)pFlash;
	pAuxMem = pMem;

	for( index = 0; index < Num; index+=4)
	{
		FLASH_ProgramWord( pAuxFlash, (*pAuxMem) );

		pAuxFlash += 4;
		pAuxMem ++;
	}
}

/**
*******************************************************************************
- Nombre de Función	:	FlashRW_ProgLaborables
- Descripción		:	
- Parámetros		:
						- pPrograma: puntero a los programas a leer/escribir
							de la flash
						- Cmd:
								- 'LEER_FLASH': Lee el contenido almacenado
									en flash y lo guarda en el lugar indicado
									por el puntero
								- 'ESCRIBIR_FLASH': escribe en flash todo lo
									que indica el puntero
- Salidas			:	No
- Valor devuelto	:	No
*******************************************************************************/
void FlashRW_ProgLaborables(tPrograma *pPrograma, unsigned char Cmd)
{
	unsigned int	*pFlash,
					*pMem;
	
	pFlash = (unsigned int *)ProgLabAddress;
	pMem = (unsigned int *)pPrograma;

	FLASH_Unlock();

	if(Cmd == ESCRIBIR_FLASH)
	{
		tsk_lock();
			
		FLASH_ErasePage( (u32)(ProgLabAddress) );												 
		FLASH_ErasePage( (u32)(ProgLabAddress + TamPagina) );												 
		FLASH_ErasePage( (u32)(ProgLabAddress + TamPagina + TamPagina) );												 
		FlashRW_EscribirBloque(pMem, pFlash, TamProgLab);
		
		tsk_unlock();		
	}
	else if(Cmd == LEER_FLASH)
	{
		memcpy( pMem, pFlash, TamProgLab);
	}
}


/**
*******************************************************************************
- Nombre de Función	:	FlashRW_ProgFestivos
- Descripción		:	
- Parámetros		:
						- pProgramaFest: puntero a los programas a leer/escribir
							de la flash
						- Cmd:
								- 'LEER_FLASH': Lee el contenido almacenado
									en flash y lo guarda en el lugar indicado
									por el puntero
								- 'ESCRIBIR_FLASH': escribe en flash todo lo
									que indica el puntero
- Salidas			:	No
- Valor devuelto	:	No
*******************************************************************************/
void FlashRW_ProgFestivos(tPrograma *pProgramaFest, unsigned char Cmd)
{
	unsigned int	*pFlashFest,
					*pMemFest;
						
	pFlashFest	= (unsigned int *)ProgFestAddress;
	pMemFest	= (unsigned int *)pProgramaFest;

	if(Cmd == ESCRIBIR_FLASH)
	{	
		tsk_lock();
			
		FLASH_Unlock();
		FLASH_ErasePage( (u32)(ProgFestAddress) );
		FlashRW_EscribirBloque(pMemFest, pFlashFest, TamProgFest);		
		
		tsk_unlock();		
	}
	else if(Cmd == LEER_FLASH)
	{
		memcpy( pMemFest, pFlashFest, TamProgFest);
	}
}

/**
*******************************************************************************
- Nombre de Función	:	FlashRW_ProgFestivos
- Descripción		:	
- Parámetros		:
						- pProgramaVac: puntero a los programas a leer/escribir
							de la flash
						- Cmd:
								- 'LEER_FLASH': Lee el contenido almacenado
									en flash y lo guarda en el lugar indicado
									por el puntero
								- 'ESCRIBIR_FLASH': escribe en flash todo lo
									que indica el puntero
- Salidas			:	No
- Valor devuelto	:	No
*******************************************************************************/
void FlashRW_ProgVacaciones(tPrograma *pProgramaVac, unsigned char Cmd)
{
	unsigned int	*pFlashVac,
					*pMemVac;
	
	pFlashVac	= (unsigned int *)ProgVacAddress;
	pMemVac		= (unsigned int *)pProgramaVac;

	if(Cmd == ESCRIBIR_FLASH)
	{	
		tsk_lock();
			
		FLASH_Unlock();
		FLASH_ErasePage( (u32)(ProgVacAddress) );
		FlashRW_EscribirBloque(pMemVac, pFlashVac, TamProgVac);		
		
		tsk_unlock();		
	}
	else if(Cmd == LEER_FLASH)
	{
		memcpy( pMemVac, pFlashVac, TamProgVac);
	}
}

/**
*******************************************************************************
- Nombre de Función	:	FlashRW_ProgTunel
- Descripción		:	
- Parámetros		:
						- pPrograma: puntero a los programas a leer/escribir
							de la flash
						- Cmd:
								- 'LEER_FLASH': Lee el contenido almacenado
									en flash y lo guarda en el lugar indicado
									por el puntero
								- 'ESCRIBIR_FLASH': escribe en flash todo lo
									que indica el puntero
- Salidas			:	No
- Valor devuelto	:	No
*******************************************************************************/
void FlashRW_ProgTunel(tProgramaTunel *pPrograma, unsigned char Cmd)
{
	unsigned int	*pFlashTunel,
					*pMemTunel;
	
	pFlashTunel	= (unsigned int *)ProgTunelAddress;
	pMemTunel	= (unsigned int *)pPrograma;

	if(Cmd == ESCRIBIR_FLASH)
	{	
		tsk_lock();
			
		FLASH_Unlock();
		FLASH_ErasePage( (u32)(ProgTunelAddress) );
		FlashRW_EscribirBloque(pMemTunel, pFlashTunel, TamProgTunel);		
		
		tsk_unlock();		
	}
	else if(Cmd == LEER_FLASH)
	{
		memcpy( pMemTunel, pFlashTunel, TamProgTunel);
	}
}

/**
*******************************************************************************
- Nombre de Función	:	FlashRW_Maniobras
- Descripción		:	
- Parámetros		:
						- pManiobra: puntero a las maniobras a leer/escribir
							de la flash
						- Cmd:
								- 'LEER_FLASH': Lee el contenido almacenado
									en flash y lo guarda en el lugar indicado
									por el puntero
								- 'ESCRIBIR_FLASH': escribe en flash todo lo
									que indica el puntero
- Salidas			:	No
- Valor devuelto	:	No
*******************************************************************************/
void FlashRW_Maniobras(tManiobrasInstancias *pManiobras, unsigned char Cmd)
{
	unsigned int	*pFlashMan,
					*pMemMan;
	
	pFlashMan	= (unsigned int *)ManiobrasAddress;
	pMemMan		= (unsigned int *)pManiobras;

	if(Cmd == ESCRIBIR_FLASH)
	{	
		tsk_lock();
			
		FLASH_Unlock();
		FLASH_ErasePage( (u32)(ManiobrasAddress) );
		FlashRW_EscribirBloque(pMemMan, pFlashMan, TamManiobras/4);		
		
		tsk_unlock();		
	}
	else if(Cmd == LEER_FLASH)
	{
		memcpy( pMemMan, pFlashMan, TamProgVac);
	}
}
