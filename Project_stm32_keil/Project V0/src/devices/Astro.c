/*******************************************************************************
* File Name          : Astro.c
* Author             : José Albertoi Villanueva
* Version            : V1.0.0
* Date               : 27/07/2009
* Description        : Implementación cálculos astronómicos
*******************************************************************************/

// Includes
//--------------------------------------------------------
#include "stm32f10x.h"
#include <math.h>
#include <stdlib.h>
#include "Astro.h"

/**
*******************************************************************************
- Nombre de Función	:	Astro_OrtoOcaso
- Descripción		:	
- Parámetros		:
- Salidas			:	
- Valor devuelto	:	
*******************************************************************************/
void Astro_OrtoOcaso(tCalendarioFecha* pFecha, tAstroCoordenada *pLatitud, tAstroCoordenada *pLongitud,
					  tRelojHora *pHoraOrto, tRelojHora *pHoraOcaso)
{		
	double Lon, Lat;
	double JD, J1, J2, S, P, AM, V, L, O_B, Z, DC;
	double AR, ET, C, R, H0, VD, VDOR, DCOR, HORTO;
	double VHORTO, TUORTO, MC, HOC, MOC, HOR, MOR;
	double TUC, HC, VDOC, DCOC, VHOC, TUOC, ACOC;
	double ACOR, GACOC, MACOC, GACOR, MACOR;
	char GGG, A;
	//unsigned short HOR, MOR, HOC, MOC;

	Lon =	pLongitud->Grados +
			(pLongitud->Minutos)/60 +
			(pLongitud->Segundos)/3600;
	
	if (pLongitud->Grados < 0) Lon =	pLongitud->Grados -
										(pLongitud->Minutos)/60 -
										(pLongitud->Segundos)/3600;
	
	Lat =	pLatitud->Grados +
			(pLatitud->Minutos)/60 +
			(pLatitud->Segundos)/3600;	

	if (pLatitud->Grados < 0) Lon =	pLatitud->Grados +
									(pLatitud->Minutos)/60 +
									(pLatitud->Segundos)/3600;
	
	GGG = 1;	
	if (pFecha->Anyo <= 1585) GGG = 0;
	
	JD = -1 * floor(7 * (floor((pFecha->Mes + 9) / 12) + pFecha->Anyo) / 4);
	
	S = 1;	
	if ((pFecha->Mes - 9)<0) S=-1;
	
	A = abs(pFecha->Mes - 9);	
	J1 = floor(pFecha->Anyo + S * floor(A / 7));	
	J1 = -1 * floor((floor(J1 / 100) + 1) * 3 / 4);	
	JD = JD + floor(275 * pFecha->Mes / 9) + pFecha->Dia + (GGG * J1);	
	JD = JD + 1721027 + 2 * GGG + 367 * pFecha->Anyo - 0.5;	
	J2 = JD;		
			  
	P = 4.93204;
	S = 2415020.5;
	ET = 0.016718;

	P = P + (J2-T0)*VP/100;
	AM = M0 + MN*(J2-T0);
	AM = AM - 2*PI*floor(AM/(2 * PI));
	V = AM + 2*ET*sin(AM) + 1.25*ET*ET*sin(2*AM);
		
	if (V<0) V = 2*PI + V;
		
	L = P + V;
	L = L - 2*PI*floor(L/(2*PI));
	Z = (J2-2415020.5)/365.2422;

	O_B = 23.452294-(0.46845*Z+0.00000059*Z*Z)/3600;
	O_B = O_B/RAD;

	DC = asin(sin(O_B)*sin(L));

	AR = acos(cos(L)/cos(DC));
		
	if (L>PI) AR = 2*PI - AR;
		
	O_B = O_B*RAD;		
	L = L*RAD;
	AR = AR * 12 / PI;	
	DC = DC*RAD;
		
	C = 2*ET*F*sin(AM) + 1.25*ET*ET*F*sin(2*AM);		
	R = -MR*F*sin(2*(P+AM))+MR*MR*F*sin(4*(P+AM))/2;		
	ET = C + R;
		
	H0 = acos(-tan(Lat/RAD)*tan(DC/RAD));
	H0 = H0*RAD;
		
	VD = 0.9856*sin(O_B/RAD)*cos(L/RAD)/cos(DC/RAD);

	VDOR = VD*(-H0+180)/360;
	DCOR = DC+VDOR;
	HORTO = -acos(-tan(Lat/RAD)*tan(DCOR/RAD));
	VHORTO = 5/(6*cos(Lat/RAD)*cos(DCOR/RAD)*sin(HORTO));
	HORTO = (HORTO*RAD+VHORTO)/15;
	TUORTO = HORTO+ET/3600-Lon/15+12;		
		
	HOR = floor(TUORTO);	
	MOR = floor((TUORTO - HOR)*60 + 0.5);
		
	if (MOR>59)
	{	
		MOR = MOR - 60;	
		HOR = HOR + 1;	
	}
	
	TUC = 12 + ET/3600 - Lon/15;
		
	HC = floor(TUC);
	MC = floor((TUC - HC)*60 + 0.5);
	if (MC>59) {
		MC = MC - 60;
		HC = HC + 1;
	}
		
		
	VDOC = VD*(H0+180)/360;
	DCOC = DC+VDOC;
	HOC = acos(-tan(Lat/RAD)*tan(DCOC/RAD));
	VHOC =5/(6*cos(Lat/RAD)*cos(DCOC/RAD)*sin(HOC));
	HOC = (HOC*RAD+VHOC)/15;
	TUOC = HOC+ET/3600-Lon/15+12;
	HOC=floor(TUOC);	
	MOC=floor((TUOC - HOC) * 60+0.5);
	if (MOC>59)
	{		
		MOC=MOC-60;
		HOC=HOC+1;
	}

//Para calcular culminación (sin uso)				
//			   HCUL=90-Lat+(DCOR+DCOC)/2
//			   GCUL=floor(HCUL);
//			   MCUL=floor((HCUL - GCUL) * 60+0.5)
//			   if (MCUL>59) {
//				MCUL=MCUL-60;
//				HCUL=HCUL+1;
//				}
			
	ACOC = acos(-sin(DCOC/RAD)/cos(Lat/RAD))*RAD;
	ACOR = 360 - acos(-sin(DCOR/RAD)/cos(Lat/RAD))*RAD;
	GACOC = floor(ACOC);
	MACOC = floor((ACOC - GACOC) * 60+0.5);
	if(MACOC>59)
	{
		MACOC = MACOC - 60;
		GACOC = GACOC + 1;
	}
		
	GACOR = floor(ACOR);
	MACOR = floor((ACOR - GACOR) * 60 + 0.5);
	if(MACOR>59)
	{
		MACOR = MACOR-60;
		GACOR = GACOR+1;
	}

	pHoraOrto->Horas = HOR + pFecha->HorarioVerano + pFecha->GMT;
	pHoraOrto->Minutos = MOR;					
	pHoraOcaso->Horas = HOC + pFecha->HorarioVerano + pFecha->GMT;
	pHoraOcaso->Minutos = MOC;
}
		
