
#include "stm32f10x.h"
#include "Calendario.h"

/**
*******************************************************************************
- Nombre de Funci�n	:	Calendario_Init
- Descripci�n		:	Inicializaci�n del calendario
- Par�metros		:	No
- Salidas			:	No
- Valor devuelto	:	No
*******************************************************************************/
void Calendario_Init(tCalendarioFecha *Fecha)
{
	os_mut_init(Fecha->Mutex);
}

/**
*******************************************************************************
- Nombre de Funci�n	:	Calendario_FijarFecha
- Descripci�n		:	Configura el d�a en formato (DD/MM/YYYY)
- Par�metros		:
						- Day: D�a de la fecha
						- Month: Mes de la fecha
						- Year: A�o de la fecha
- Salidas			:	No
- Valor devuelto	:	No
*******************************************************************************/
unsigned char Calendario_FijarFecha(tCalendarioFecha *Fecha,
									unsigned char Day,
									unsigned char Month,
									unsigned short Year)
{
	uint32_t DateTimer;
	//char linea[20];
			
	// si el d�a introducido es incorrecto es un error
	if((( Month==4 || Month==6 || Month==9 || Month==11) && Day ==31) \
	|| (Month==2 && Day==31)|| (Month==2 && Day==30)|| \
	  (Month==2 && Day==29 && (Calendario_ComprobarBisiesto(Year)==0)) \
	  || (Month > 12))
	{
		DateTimer=RTC_GetCounter();
	
		while((RTC_GetCounter()-DateTimer)<2)
		{
		}
		return(0);
	}
	// Si el d�a introducido es correcto lo guardamos 
	else
	{
		os_mut_wait(Fecha->Mutex, 0xFFFF);

		// Activar relojes de PWR y BKP
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
		// Permitir acceso al "Backup"
		PWR_BackupAccessCmd(ENABLE);	
		/// Guardamos los valores de la fecha en los registros corresepondientes.
		BKP_WriteBackupRegister(BKP_DR2,Month);
		BKP_WriteBackupRegister(BKP_DR3,Day);
		BKP_WriteBackupRegister(BKP_DR4,Year);
		
		Fecha->Dia = Day; 
		Fecha->Mes = Month;
		Fecha->Anyo = Year;

		// Denegar acceso al "Backup"
		PWR_BackupAccessCmd(DISABLE);

		os_mut_release(Fecha->Mutex);

		return(1);
  }
}

/**
*******************************************************************************
- Nombre de Funci�n	:	Calendario_ComprobarBisiesto
- Descripci�n		:	comprueba si un a�o es bisiesto
- Par�metros		:
						- Year: un a�o
- Salidas			:	No
- Valor devuelto	:
						- 'BISIESTO'
						- 'NO_BISIESTO'
*******************************************************************************/
unsigned char Calendario_ComprobarBisiesto(unsigned short Year)
{
  if((Year%400)==0)
  {
    return BISIESTO;
  }
  else if((Year%100)==0)
  {
    return NO_BISIESTO;
  }
  else if((Year%4)==0)
  {
    return BISIESTO;
  }
  else
  {
    return NO_BISIESTO;
  }
}

/**
*******************************************************************************
- Nombre de Funci�n	:	Calendario_AumentarDia
- Descripci�n		:	Aumenta un dia a la fecha que se le pasa como par�metro
- Par�metros		:
						- Fecha: puntero a fecha
- Salidas			:	No
- Valor devuelto	:	No
*******************************************************************************/
void Calendario_AumentarDia(tCalendarioFecha *Fecha)
{
//	contador_ZZZ++;
	// leemos la fecha de los Registros de Backup
	Fecha->Dia = BKP_ReadBackupRegister(BKP_DR3);
	Fecha->Mes = BKP_ReadBackupRegister(BKP_DR2);
	Fecha->Anyo = BKP_ReadBackupRegister(BKP_DR4);
	
	
	// Revisar los meses de 31 d�as
	if(Fecha->Mes == 1 || Fecha->Mes == 3 || \
	Fecha->Mes == 5 || Fecha->Mes == 7 ||\
	 Fecha->Mes == 8 || Fecha->Mes == 10 \
	   || Fecha->Mes == 12)
	{
		// Si no se ha llegado al �ltimo d�a del mes (31) lo aumentamos
		if(Fecha->Dia < 31)
		{
		  	Fecha->Dia++;	// aumentar d�a
		}
		// Si se ha llegado al �ltimo d�a del mes (el 31)
		else
		{
			// Aumentar el mes siempre y cuando no se trate de diciembre (12)
			if(Fecha->Mes != 12)
			{
				Fecha->Mes++;	// aumentar mes
				Fecha->Dia = 1;
			}
			// Se estamos en el �ltimo d�a del mes (31) y el mes es diciembre (12)
			// es que estamos en el fin de a�o
			else
			{
				Fecha->Mes = 1;
				Fecha->Dia = 1;
				Fecha->Anyo++;		// aumentar a�o
			}
		}
	}
	//Revisar los meses de 30 d�as
	else if(Fecha->Mes == 4 || Fecha->Mes == 6 \
	        || Fecha->Mes == 9 ||Fecha->Mes == 11)
	{
		// Si no se ha llegado al �ltimo d�a del mes (30)
		if(Fecha->Dia < 30)
		{
			Fecha->Dia++;		// aumentar d�a	
		}
		// Si se ha llegado al �ltimo d�a del mes (30)
		else
		{
			Fecha->Mes++;	// Aumentar mes		
			Fecha->Dia = 1;
		}
	}
	//Revisar Febrero
	else if(Fecha->Mes == 2)
	{
		// Si no se ha llegado al �ltimo d�a del mes (28)
		if(Fecha->Dia < 28)
		{
		  	Fecha->Dia++;		// aumentar d�a
		}
		// comprobamos si hemos llegado al d�a 28
		else if(Fecha->Dia == 28)
		{
		  // Comprobar si el a�o es bisiesto
		  if(Calendario_ComprobarBisiesto(Fecha->Anyo))
		  {
		    Fecha->Dia++;		// Aumentamos el d�a para que sea 29
		  }
		  else	// Si el a�o no es bisiesto
		  {
		    Fecha->Mes++;	// Cambiar a marzo.
		    Fecha->Dia = 1;
		  }
		}
		// Cuando ya estamos en un a�o bisiesto y toca aumentar el d�a, si 
		// estamos a 29 pasamos a marzo.
		else if(Fecha->Dia == 29)
		{
		  Fecha->Mes++;
		  Fecha->Dia = 1;
		}
	}
	// Activar relojes de PWR y BKP
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	// Permitir acceso al "Backup"
	PWR_BackupAccessCmd(ENABLE);
	/// Guardamos los valores de la fecha en los registros corresepondientes.
	BKP_WriteBackupRegister(BKP_DR2,Fecha->Mes);
	BKP_WriteBackupRegister(BKP_DR3,Fecha->Dia);
	BKP_WriteBackupRegister(BKP_DR4,Fecha->Anyo);
//	DIA =	Fecha->Dia; 
//	MES =  	Fecha->Mes;
//	ANY = 	Fecha->Anyo;
	// Denegar acceso al "Backup"
	PWR_BackupAccessCmd(DISABLE);
	// Indicar que la fecha ha cambiado y que hay que cambiar la visualizaci�n
//	Flag_CambiaFecha=1;		
}

/**
*******************************************************************************
- Nombre de Funci�n	:	Calendario_DisminuirDia
- Descripci�n		:	Resta un dia a la fecha que se le pasa como par�metro
- Par�metros		:
						- Fecha: puntero a fecha
- Salidas			:	No
- Valor devuelto	:	No
*******************************************************************************/
void Calendario_DisminuirDia(tCalendarioFecha *Fecha)
{
	//Decrementamos un d�a
	Fecha->Dia--;	

	//Si el d�a es '0' -> hay que cambiar de mes
	if(Fecha->Dia == 0)
	{
		Fecha->Mes--;
		//Si el mes es '0' -> el d�a anterior es el 31 de diciembre
		if(Fecha->Mes == 0)
		{
			Fecha->Dia = 31;
			Fecha->Mes = 12;
			Fecha->Anyo--;
			return;
		}

		switch(Fecha->Mes)
		{
			case 1: case 3: case 5: case 7:
			case 8: case 10: case 12:
				Fecha->Dia = 31;
				break;
			case 4: case 6: case 9: case 11:
				Fecha->Dia = 30;
				break;
			case 2:
				if(Calendario_ComprobarBisiesto(Fecha->Anyo))
					Fecha->Dia = 29;
				else
					Fecha->Dia = 28;
				break;
		}

	}	
}

/**
*******************************************************************************
- Nombre de Funci�n	:	Calendario_AumentarMes
- Descripci�n		:	A�ade un mes a la fecha que se le pasa como par�metro
- Par�metros		:
						- Fecha: puntero a fecha
- Salidas			:	No
- Valor devuelto	:	No
*******************************************************************************/
void Calendario_AumentarMes(tCalendarioFecha *Fecha)
{
	//u8 Resultado;

	// leemos la fecha de los Registros de Backup
	Fecha->Dia = BKP_ReadBackupRegister(BKP_DR3);
	Fecha->Mes = BKP_ReadBackupRegister(BKP_DR2);
	Fecha->Anyo = BKP_ReadBackupRegister(BKP_DR4);
	Fecha->Mes++;

	if (Fecha->Mes > 12)
	{
		Fecha->Mes = 1;
		Fecha->Anyo++;
	}

//	Resultado = Calendario_FijarFecha(Fecha, Fecha->Dia,Fecha->Mes,Fecha->Anyo);
//
//	while (Resultado == NO_OK)
//	{
//		Fecha->Mes++;;
//		Resultado = Calendario_FijarFecha(Fecha, Fecha->Dia,Fecha->Mes,Fecha->Anyo);	
//	}

	while (!Calendario_FijarFecha(Fecha, Fecha->Dia,Fecha->Mes,Fecha->Anyo))
	{
		Fecha->Mes++;;
	}


}

/**
*******************************************************************************
- Nombre de Funci�n	:	Calendario_AumentarAnyo
- Descripci�n		:	A�ade un a�o a la fecha que se le pasa como par�metro
- Par�metros		:
						- Fecha: puntero a fecha
- Salidas			:	No
- Valor devuelto	:	No
*******************************************************************************/
void Calendario_AumentarAnyo(tCalendarioFecha *Fecha)
{
	//u8 dia_semana;

	// leemos la fecha de los Registros de Backup
	Fecha->Dia = BKP_ReadBackupRegister(BKP_DR3);
	Fecha->Mes = BKP_ReadBackupRegister(BKP_DR2);
	Fecha->Anyo = BKP_ReadBackupRegister(BKP_DR4);
	Fecha->Anyo++;


	Calendario_FijarFecha(Fecha, Fecha->Dia,Fecha->Mes,Fecha->Anyo);	
}

/**
*******************************************************************************
- Nombre de Funci�n	:	Calendario_FechaValida
- Descripci�n		:	Comprueba si una fecha es v�lida
- Par�metros		:
						- Fecha: puntero a fecha
- Salidas			:	No
- Valor devuelto	:
						- '0' fecha no es v�lida.
    					- '1' fecha v�lida.	
*******************************************************************************/
u8 Calendario_FechaValida(tCalendarioFecha *Fecha)
{
    u8 diasmes[]={31,28,31,30,31,30,31,31,30,31,30,31};
    if(Fecha->Mes<1 || Fecha->Mes>12)
        return 0;
    if(Fecha->Dia<1)
        return 0;
    if(Fecha->Mes != 2){
        if(Fecha->Dia > diasmes[Fecha->Mes-1])
            return 0;
    }
    else{
        if(Calendario_ComprobarBisiesto(Fecha->Anyo)){
            if(Fecha->Dia > 29)
                return 0;
        }
        else{
            if(Fecha->Dia > 28)
                return 0;
        }
    }
    return 1;
}


/**
*******************************************************************************
- Nombre de Funci�n	:	Calendario_DiaSemana
- Descripci�n		:	C�lcula el d�a de la semana para una fecha dada
- Par�metros		:	
						- Fecha: puntero a una fecha
- Salidas			:	No
- Valor devuelto	:
						- '100' si la fecha no es v�lida.
						- '0' si es lunes.
						- '1' si es martes.
						- '2' si es miercoles.
						- '3' si es jueves.
						- '4' si es viernes. 
						- '5' si es s�bado.
						- '6' si es domingo.	
*******************************************************************************/
unsigned char Calendario_DiaSemana(tCalendarioFecha *Fecha)
{
	
	u8 tb_nma[] = {0,3,3,6,1,4,6,2,5,0,3,5}; /* para a�o no bisiesto */
	u8 tb_nmb[] = {6,2,3,6,1,4,6,2,5,0,3,5}; /* para a�o bisiesto */
	u16 manyo;
	u8 *tb;

	if(!Calendario_FechaValida(Fecha))
		return 100;
	if( Calendario_ComprobarBisiesto(Fecha->Anyo) )
		tb = (u8 *)&tb_nmb[0];
	else
		tb = (u8 *)&tb_nma[0];
	
	manyo = Fecha->Anyo;
	manyo += Fecha->Anyo / 4;
	manyo -= Fecha->Anyo / 100;
	manyo += Fecha->Anyo / 400;
	manyo += Fecha->Dia;
	manyo += tb[Fecha->Mes - 1];
	manyo -= 1;
	manyo %= 7;

	if(manyo == 0)
		manyo = 6;
	else
		manyo--;

	return (u8)manyo;
}

/**
*******************************************************************************
- Nombre de Funci�n	:	Calendario_ObtenerFecha
- Descripci�n		:	Obtiene la fecha almacenada en los registros de backup
- Par�metros		:
						- Fecha: puntero a fecha
- Salidas			:	Fecha almacenada en los registros de backup
- Valor devuelto	:	No
*******************************************************************************/
void Calendario_ObtenerFecha(tCalendarioFecha *Fecha)
{
	// leemos la fecha de los Registros de Backup
	Fecha->Dia = BKP_ReadBackupRegister(BKP_DR3);
	if(Fecha->Dia < 1) Fecha->Dia = 1;
	Fecha->Mes = BKP_ReadBackupRegister(BKP_DR2);
	if(Fecha->Mes < 1) Fecha->Mes = 1;
	Fecha->Anyo = BKP_ReadBackupRegister(BKP_DR4);
	if(Fecha->Anyo < 2000) Fecha->Anyo = 2000;
}

/**
*******************************************************************************
- Nombre de Funci�n	:	Calendario_ObtenerTipoDia
- Descripci�n		:	Obtine el tipo de d�a para una fecha dada
- Par�metros		:
						- Fecha: puntero a una fecha
- Salidas			:	No
- Valor devuelto	:
						- 'LABORABLE': se trata de un d�a laborable (valor 0)
						- 'FESTIVO': d�a festivo (valor 1)
						- 'VACACIONES': d�a de vacaciones (valor 2)
*******************************************************************************/
unsigned char Calendario_ObtenerTipoDia(tDiasFestivos *Festivos, 
										tDiasVacaciones *Vacaciones, 
										tCalendarioFecha *Fecha)
{
	unsigned char i;

	//Comprobar si es domingo
	if(Calendario_DiaSemana(Fecha) == 6)
	{
		return(FESTIVO);
	}

	//Comprobar si esta en la lista de festivos
	for(i=0; i<MAX_DIAS_FESTIVOS; i++)
	{
		if( (Fecha->Dia == Festivos->Dia[i].Dia) && 
			(Fecha->Mes == Festivos->Dia[i].Mes) &&
			(Fecha->Anyo == Festivos->Dia[i].Anyo) )
			return(FESTIVO);
	}

	//Comprobar si esta en la lista de vacaciones
	for(i=0; i<MAX_DIAS_VACACIONES; i++)
	{
		if( (Fecha->Dia == Vacaciones->Dia[i].Dia) && 
			(Fecha->Mes == Vacaciones->Dia[i].Mes) &&
			(Fecha->Anyo == Vacaciones->Dia[i].Anyo) )
			return(VACACIONES);
	}

	return(LABORABLE);
}

/**
*******************************************************************************
- Nombre de Funci�n	:	Calendario_RegistrarFestivo
- Descripci�n		:	A�ade un d�a festivo
- Par�metros		:
						- Festivos: puntero a lista de dias festivo
						- Fecha: puntero a la fecha a �adir
- Salidas			:	No
- Valor devuelto	:	No
*******************************************************************************/
void Calendario_RegistrarFestivo(tDiasFestivos *Festivos, tCalendarioFecha *Fecha)
{
	if(Festivos->NumDias == sizeof(Festivos->Dia))
		return;

	Festivos->Dia[Festivos->NumDias++] = *Fecha;
}

/**
*******************************************************************************
- Nombre de Funci�n	:	Calendario_RegistrarVacional
- Descripci�n		:	A�ade un d�a festivo
- Par�metros		:
						- Festivos: puntero a lista de dias festivo
						- Fecha: puntero a la fecha a �adir
- Salidas			:	No
- Valor devuelto	:	No
*******************************************************************************/
void Calendario_RegistrarVacional(tDiasVacaciones *Vacaciones, tCalendarioFecha *Fecha)
{
	if(Vacaciones->NumDias == sizeof(Vacaciones->Dia))
		return;

	Vacaciones->Dia[Vacaciones->NumDias++] = *Fecha;
}
