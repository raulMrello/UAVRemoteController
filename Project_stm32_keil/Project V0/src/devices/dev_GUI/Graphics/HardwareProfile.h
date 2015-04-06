/*********************************************************************
 *
 *	Hardware specific definitions
 *
 *********************************************************************
 * FileName:        HardwareProfile_xxx.h
 * Dependencies:    None
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:        Microchip C32 v1.00 or higher
 *					Microchip C30 v3.01 or higher
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright © 2002-2009 Microchip Technology Inc.  All rights 
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and 
 * distribute: 
 * (i)  the Software when embedded on a Microchip microcontroller or 
 *      digital signal controller product (“Device”) which is 
 *      integrated into Licensee’s product; or
 * (ii) ONLY the Software driver source files ENC28J60.c and 
 *      ENC28J60.h ported to a non-Microchip device used in 
 *      conjunction with a Microchip ethernet controller for the 
 *      sole purpose of interfacing with the ethernet controller. 
 *
 * You should refer to the license agreement accompanying this 
 * Software for additional information regarding your rights and 
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT 
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL 
 * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR 
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF 
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS 
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE 
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER 
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT 
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 *
 * Author               Date		Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Howard Schlunder		10/03/06	Original, copied from Compiler.h
 * Jayanth Murthy       06/25/09    dsPIC & PIC24H support 
 * Pradeep Budagutta	15 Sep 2009 Added PIC24FJ256DA210 Development Board Support
 ********************************************************************/
#ifndef __HARDWARE_PROFILE_H
    #define __HARDWARE_PROFILE_H

/*********************************************************************
* GetSystemClock() returns system clock frequency.
*
* GetPeripheralClock() returns peripheral clock frequency.
*
* GetInstructionClock() returns instruction clock frequency.
*
********************************************************************/

/*********************************************************************
* Macro: #define	GetSystemClock() 
*
* Overview: This macro returns the system clock frequency in Hertz.
*			* value is 8 MHz x 4 PLL for PIC24
*			* value is 8 MHz/2 x 18 PLL for PIC32
*
********************************************************************/
    #if defined(__PIC24F__)
        #define GetSystemClock()    (32000000ul)
    #elif defined(__PIC32MX__)
        #define GetSystemClock()    (72000000ul)
    #elif defined(__dsPIC33F__) || defined(__PIC24H__)
        #define GetSystemClock()    (80000000ul)
    #endif

/*********************************************************************
* Macro: #define	GetPeripheralClock() 
*
* Overview: This macro returns the peripheral clock frequency 
*			used in Hertz.
*			* value for PIC24 is <PRE>(GetSystemClock()/2) </PRE> 
*			* value for PIC32 is <PRE>(GetSystemClock()/(1<<OSCCONbits.PBDIV)) </PRE>
*
********************************************************************/
    #if defined(__PIC24F__) || defined(__PIC24H__) || defined(__dsPIC33F__)
        #define GetPeripheralClock()    (GetSystemClock() / 2)
    #elif defined(__PIC32MX__)
        #define GetPeripheralClock()    (GetSystemClock() / (1 << OSCCONbits.PBDIV))
    #endif

/*********************************************************************
* Macro: #define	GetInstructionClock() 
*
* Overview: This macro returns instruction clock frequency 
*			used in Hertz.
*			* value for PIC24 is <PRE>(GetSystemClock()/2) </PRE> 
*			* value for PIC32 is (GetSystemClock()/PFMWSbits.CHECON) </PRE> 
*
********************************************************************/
    #if defined(__PIC24F__) || defined(__PIC24H__) || defined(__dsPIC33F__)
        #define GetInstructionClock()   (GetSystemClock() / 2)
    #elif defined(__PIC32MX__)
        #define GetInstructionClock()   (GetSystemClock() / PFMWSbits.CHECON)
    #endif

/*********************************************************************
* UART SETTINGS
*********************************************************************/
	#define BAUDRATE2               115200UL
    #define BRG_DIV2                4
    #define BRGH2                   1

/* ################################################################## */
/*********************************************************************
* START OF GRAPHICS RELATED MACROS
********************************************************************/
/* ################################################################## */

/*********************************************************************
* AUTO GENERATED CODE 
********************************************************************/

//Auto Generated Code
//#define GRAPHICS_HARDWARE_PLATFORM GFX_PICTAIL_V3
//#define USE_16BIT_PMP
#define DISPLAY_CONTROLLER CUSTOM_CONTROLLER //OrbisUC1701X
#define DISPLAY_PANEL PANEL128X64ORBIS //TFT_G320240DTSW_69W_TP_E //PANEL128X64ORBIS
#define COLOR_DEPTH 1
//End Auto Generated Code



/*********************************************************************
* END OF AUTO GENERATED CODE 
********************************************************************/

#ifdef _GRAPHICS_H

/*********************************************************************
* DISPLAY SETTINGS 
********************************************************************/

// Error Checking
//	#ifndef GRAPHICS_HARDWARE_PLATFORM
//		#error "Error: GRAPHICS_HARDWARE_PLATFORM is not defined!"
//	#endif

    #ifndef DISPLAY_CONTROLLER
        #error "Error: DISPLAY_CONTROLLER not defined"
    #endif

    #ifndef DISPLAY_PANEL
        #error "Error: DISPLAY_PANEL not defined"
    #endif

	#if (DISPLAY_CONTROLLER == LGDP4531) || (DISPLAY_CONTROLLER == S6D0129)	||		\
		(DISPLAY_PANEL == TFT2N0369_E)	 || (DISPLAY_PANEL == DT032TFT_TS)	||		\
		(DISPLAY_PANEL == DT032TFT)		 ||											\
		(DISPLAY_PANEL == TFT_G240320LTSW_118W_E)	 
			#define DISP_ORIENTATION    90
	#else
            #define DISP_ORIENTATION    0
	#endif
     
    #if (DISPLAY_CONTROLLER == LGDP4531) || (DISPLAY_CONTROLLER == S6D0129)

        #define DISP_HOR_RESOLUTION 240
        #define DISP_VER_RESOLUTION 320

    #elif (DISPLAY_PANEL == TFT_G240320LTSW_118W_E) || (DISPLAY_PANEL == TFT2N0369_E) || \
          (DISPLAY_PANEL == DT032TFT_TS) || (DISPLAY_PANEL == DT032TFT)

        #define DISP_HOR_RESOLUTION 240
        #define DISP_VER_RESOLUTION 320

        #define DISP_DATA_WIDTH 18

        #define DISP_INV_LSHIFT

        #define DISP_HOR_PULSE_WIDTH    25
        #define DISP_HOR_BACK_PORCH     5
        #define DISP_HOR_FRONT_PORCH    10

        #define DISP_VER_PULSE_WIDTH    4
        #define DISP_VER_BACK_PORCH     0
        #define DISP_VER_FRONT_PORCH    2

        #if (DISPLAY_CONTROLLER == MCHP_DA210)
        
            #define GFX_LCD_TYPE                        GFX_LCD_TFT

            #define GFX_DISPLAYENABLE_ENABLE
            #define GFX_HSYNC_ENABLE
            #define GFX_VSYNC_ENABLE
            #define GFX_DISPLAYPOWER_ENABLE

            #define GFX_CLOCK_POLARITY                  GFX_ACTIVE_HIGH
            #define GFX_DISPLAYENABLE_POLARITY          GFX_ACTIVE_HIGH
            #define GFX_HSYNC_POLARITY                  GFX_ACTIVE_LOW
            #define GFX_VSYNC_POLARITY                  GFX_ACTIVE_LOW
            #define GFX_DISPLAYPOWER_POLARITY           GFX_ACTIVE_HIGH
            
        #endif

    #elif (DISPLAY_PANEL == TFT_G320240DTSW_69W_TP_E) || (DISPLAY_PANEL == _35QVW0T)

        #define DISP_HOR_RESOLUTION 240
        #define DISP_VER_RESOLUTION 320

        #define DISP_DATA_WIDTH 18

      //#define DISP_INV_LSHIFT

        #define DISP_HOR_PULSE_WIDTH    25
        #define DISP_HOR_BACK_PORCH     8
        #define DISP_HOR_FRONT_PORCH    8

        #define DISP_VER_PULSE_WIDTH    8
        #define DISP_VER_BACK_PORCH     7
        #define DISP_VER_FRONT_PORCH    5

    #elif (DISPLAY_PANEL == PANEL128X64ORBIS)

        #define DISP_HOR_RESOLUTION 128
        #define DISP_VER_RESOLUTION 64

        #define DISP_DATA_WIDTH 1

      //#define DISP_INV_LSHIFT

        //#define DISP_HOR_PULSE_WIDTH    25
        //#define DISP_HOR_BACK_PORCH     8
        //#define DISP_HOR_FRONT_PORCH    8

        //#define DISP_VER_PULSE_WIDTH    8
        //#define DISP_VER_BACK_PORCH     7
        //#define DISP_VER_FRONT_PORCH    5

    #elif (DISPLAY_PANEL == PH480272T_005_I06Q) || (DISPLAY_PANEL == PH480272T_005_I11Q)

        #define DISP_HOR_RESOLUTION 480
        #define DISP_VER_RESOLUTION 272

        #define DISP_DATA_WIDTH 24

      //#define DISP_INV_LSHIFT

        #define DISP_HOR_PULSE_WIDTH    41
        #define DISP_HOR_BACK_PORCH     2
        #define DISP_HOR_FRONT_PORCH    2

        #define DISP_VER_PULSE_WIDTH    10
        #define DISP_VER_BACK_PORCH     2
        #define DISP_VER_FRONT_PORCH    2

        #if (DISPLAY_CONTROLLER == MCHP_DA210)
        
            #define GFX_LCD_TYPE                        GFX_LCD_TFT

            #define GFX_DISPLAYENABLE_ENABLE
            #define GFX_HSYNC_ENABLE
            #define GFX_VSYNC_ENABLE
            #define GFX_DISPLAYPOWER_ENABLE

            #define GFX_CLOCK_POLARITY                  GFX_ACTIVE_LOW
            #define GFX_DISPLAYENABLE_POLARITY          GFX_ACTIVE_HIGH
            #define GFX_HSYNC_POLARITY                  GFX_ACTIVE_LOW
            #define GFX_VSYNC_POLARITY                  GFX_ACTIVE_LOW
            #define GFX_DISPLAYPOWER_POLARITY           GFX_ACTIVE_HIGH

        #endif

    #else

        #error "Graphics controller is not defined"

    #endif // DISPLAY_CONTROLLER == ABC


/*********************************************************************
* IOS FOR THE DISPLAY CONTROLLER
*********************************************************************/
	#if (GRAPHICS_HARDWARE_PLATFORM == GFX_PICTAIL_V1)

		// Definitions for reset pin
		#define RST_TRIS_BIT    TRISCbits.TRISC1
        #define RST_LAT_BIT     LATCbits.LATC1

		// Definitions for RS pin
        #define RS_TRIS_BIT TRISBbits.TRISB15
        #define RS_LAT_BIT  LATBbits.LATB15

		// Definitions for CS pin
        #define CS_TRIS_BIT TRISDbits.TRISD8
        #define CS_LAT_BIT  LATDbits.LATD8

		// Definitions for FLASH CS pin
        #define CS_FLASH_LAT_BIT    LATDbits.LATD9
        #define CS_FLASH_TRIS_BIT   TRISDbits.TRISD9

	#elif (GRAPHICS_HARDWARE_PLATFORM == GFX_PICTAIL_V2)
        #if (DISPLAY_CONTROLLER == LGDP4531)
            #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)

				// Definitions for reset pin
                #define RST_TRIS_BIT    TRISAbits.TRISA4
                #define RST_LAT_BIT     LATAbits.LATA4

				// Definitions for RS pin
                #define RS_TRIS_BIT TRISAbits.TRISA1
                #define RS_LAT_BIT  LATAbits.LATA1

				// Definitions for CS pin
                #define CS_TRIS_BIT TRISBbits.TRISB15
                #define CS_LAT_BIT  LATBbits.LATB15

				// Definitions for FLASH CS pin
                #define CS_FLASH_LAT_BIT    LATAbits.LATA8
                #define CS_FLASH_TRIS_BIT   TRISAbits.TRISA8

				// Definitions for POWER ON pin
                #define POWERON_LAT_BIT     LATAbits.LATA10
                #define POWERON_TRIS_BIT    TRISAbits.TRISA10

            #else

				// Definitions for reset pin
                #define RST_TRIS_BIT    TRISCbits.TRISC1
                #define RST_LAT_BIT     LATCbits.LATC1

				// Definitions for RS pin
                #define RS_TRIS_BIT TRISCbits.TRISC2
                #define RS_LAT_BIT  LATCbits.LATC2

				// Definitions for CS pin
                #define CS_TRIS_BIT TRISDbits.TRISD10
                #define CS_LAT_BIT  LATDbits.LATD10

				// Definitions for FLASH CS pin
                #define CS_FLASH_LAT_BIT    LATDbits.LATD1
                #define CS_FLASH_TRIS_BIT   TRISDbits.TRISD1

				// Definitions for POWER ON pin
                #define POWERON_LAT_BIT     LATCbits.LATC3
                #define POWERON_TRIS_BIT    TRISCbits.TRISC3
            #endif
        #elif (DISPLAY_CONTROLLER == SSD1906)

			// Definitions for reset line
            #define RST_TRIS_BIT    TRISCbits.TRISC1
            #define RST_LAT_BIT     LATCbits.LATC1

			// Definitions for RS line
            #define RS_TRIS_BIT TRISCbits.TRISC2
            #define RS_LAT_BIT  LATCbits.LATC2

			// Definitions for CS line
            #define CS_TRIS_BIT TRISDbits.TRISD10
            #define CS_LAT_BIT  LATDbits.LATD10

			// Definitions for A0 line
            #define A0_LAT_BIT  LATDbits.LATD3
            #define A0_TRIS_BIT TRISDbits.TRISD3

			// Definitions for A17 line
            #define A17_LAT_BIT     LATGbits.LATG14
            #define A17_TRIS_BIT    TRISGbits.TRISG14

        #else
            #error GRAPHICS CONTROLLER IS NOT SUPPORTED
        #endif // (DISPLAY_CONTROLLER == ...
    #elif (GRAPHICS_HARDWARE_PLATFORM == GFX_PICTAIL_V3)
        #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)

			// Definitions for reset pin
            #define RST_TRIS_BIT    TRISAbits.TRISA4
            #define RST_LAT_BIT     LATAbits.LATA4

			// Definitions for RS pin
            #define RS_TRIS_BIT TRISAbits.TRISA1
            #define RS_LAT_BIT  LATAbits.LATA1

			// Definitions for CS pin
            #define CS_TRIS_BIT TRISBbits.TRISB15
            #define CS_LAT_BIT  LATBbits.LATB15

        #else

			// Definitions for reset line
            #define RST_TRIS_BIT    TRISCbits.TRISC1
            #define RST_LAT_BIT     LATCbits.LATC1

			// Definitions for RS line
            #define RS_TRIS_BIT TRISCbits.TRISC2
            #define RS_LAT_BIT  LATCbits.LATC2

			// Definitions for CS line
            #define CS_TRIS_BIT TRISDbits.TRISD10
            #define CS_LAT_BIT  LATDbits.LATD10
        #endif
    #endif // (GRAPHICS_HARDWARE_PLATFORM == ...

/*********************************************************************
* IO FOR THE BEEPER
*********************************************************************/
    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
        #define BEEP_TRIS_BIT   TRISBbits.TRISB4
        #define BEEP_LAT_BIT    LATBbits.LATB4
    #else
        #define BEEP_TRIS_BIT   TRISDbits.TRISD0
        #define BEEP_LAT_BIT    LATDbits.LATD0
    #endif

/*********************************************************************
* IOS FOR THE FLASH/EEPROM SPI
*********************************************************************/

    #if ((GRAPHICS_HARDWARE_PLATFORM == GFX_PICTAIL_V2) || (GRAPHICS_HARDWARE_PLATFORM == GFX_PICTAIL_V1))
        #if  defined(__PIC24F__) || defined(__PIC24H__) || defined(__dsPIC33F__)
            #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
                #define EEPROM_CS_TRIS  TRISAbits.TRISA0
                #define EEPROM_CS_LAT   LATAbits.LATA0
            #elif defined(__PIC24FJ256GB110__)
				// This PIM has RD12 rerouted to RG0
                #define EEPROM_CS_TRIS  TRISGbits.TRISG0
                #define EEPROM_CS_LAT   LATGbits.LATG0
            #else
                #define EEPROM_CS_TRIS  TRISDbits.TRISD12
                #define EEPROM_CS_LAT   LATDbits.LATD12
            #endif
        #elif defined(__PIC32MX__)
            #define EEPROM_CS_TRIS  TRISDbits.TRISD12
            #define EEPROM_CS_LAT   LATDbits.LATD12
        #endif
        #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
            #define EEPROM_SCK_TRIS TRISCbits.TRISC2
            #define EEPROM_SDO_TRIS TRISCbits.TRISC0
            #define EEPROM_SDI_TRIS TRISCbits.TRISC1
        #else
            #define EEPROM_SCK_TRIS TRISGbits.TRISG6
            #define EEPROM_SDO_TRIS TRISGbits.TRISG8
            #define EEPROM_SDI_TRIS TRISGbits.TRISG7
        #endif
    #elif (GRAPHICS_HARDWARE_PLATFORM == GFX_PICTAIL_V3)
        #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
            #define SST25_CS_TRIS   TRISAbits.TRISA8
            #define SST25_CS_LAT    LATAbits.LATA8
            #define SST25_SCK_TRIS  TRISCbits.TRISC2
            #define SST25_SDO_TRIS  TRISCbits.TRISC0
            #define SST25_SDI_TRIS  TRISCbits.TRISC1
        #else
            #define SST25_CS_TRIS   TRISDbits.TRISD1
            #define SST25_CS_LAT    LATDbits.LATD1
            #define SST25_SCK_TRIS  TRISGbits.TRISG6
            #define SST25_SDO_TRIS  TRISGbits.TRISG8
            #define SST25_SDI_TRIS  TRISGbits.TRISG7
        #endif
    #elif (GRAPHICS_HARDWARE_PLATFORM == DA210_DEV_BOARD)
        #define SST25_CS_TRIS   TRISAbits.TRISA14
        #define SST25_CS_LAT    LATAbits.LATA14
        #define SST25_SCK_TRIS  TRISDbits.TRISD8
        #define SST25_SDO_TRIS  TRISBbits.TRISB1
        #define SST25_SDI_TRIS  TRISBbits.TRISB0
        #define SST25_SDI_ANS   ANSBbits.ANSB0
        #define SST25_SDO_ANS   ANSBbits.ANSB1
    #endif

/*********************************************************************
* IOS FOR THE GRAPHICS PICTAIL PARALLEL FLASH MEMORY
*********************************************************************/
#if (GRAPHICS_HARDWARE_PLATFORM == GFX_PICTAIL_V1)
	#define SST39_CS_TRIS    TRISDbits.TRISD9
	#define SST39_CS_LAT     LATDbits.LATD9
	#define SST39_A18_TRIS   TRISBbits.TRISB11
	#define SST39_A18_LAT    LATBbits.LATB11
	#define SST39_A17_TRIS   TRISBbits.TRISB10
	#define SST39_A17_LAT    LATBbits.LATB10
	#define SST39_A16_TRIS   TRISDbits.TRISD3
	#define SST39_A16_LAT    LATDbits.LATD3
#elif (GRAPHICS_HARDWARE_PLATFORM == GFX_PICTAIL_V2)
	#if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__) 
		#define SST39_CS_TRIS    TRISAbits.TRISA8
		#define SST39_CS_LAT     LATAbits.LATA8
		#define SST39_A18_TRIS   TRISAbits.TRISA1
		#define SST39_A18_LAT    LATAbits.LATA1
		#define SST39_A17_TRIS   TRISCbits.TRISC5
		#define SST39_A17_LAT    LATCbits.LATC5
		#define SST39_A16_TRIS   TRISCbits.TRISC4
		#define SST39_A16_LAT    LATCbits.LATC4
	#else
		#define SST39_CS_TRIS    TRISDbits.TRISD1
		#define SST39_CS_LAT     LATDbits.LATD1
		#define SST39_A18_TRIS   TRISCbits.TRISC2
		#define SST39_A18_LAT    LATCbits.LATC2
		#define SST39_A17_TRIS   TRISGbits.TRISG15
		#define SST39_A17_LAT    LATGbits.LATG15
		#define SST39_A16_TRIS   TRISGbits.TRISG14
		#define SST39_A16_LAT    LATGbits.LATG14
	#endif
#endif


/*********************************************************************
* IOS FOR THE TOUCH SCREEN
*********************************************************************/

	// ADC channel constants
    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
        #define ADC_TEMP        4
        #define ADC_POT         0
        #define ADC_POT_TRIS    TRISAbits.TRISA0
        #define ADC_POT_PCFG    AD1PCFGLbits.PCFG0
    #elif defined(__PIC32MX__)
        #define ADC_TEMP    ADC_CH0_POS_SAMPLEA_AN4
        #define ADC_POT     ADC_CH0_POS_SAMPLEA_AN5
    #else
        #define ADC_TEMP    4
        #define ADC_POT     5
    #endif

    #if (GRAPHICS_HARDWARE_PLATFORM == GFX_PICTAIL_V1)
        #ifdef __PIC32MX__
            #define ADC_XPOS    ADC_CH0_POS_SAMPLEA_AN13
            #define ADC_YPOS    ADC_CH0_POS_SAMPLEA_AN12
        #else
            #define ADC_XPOS    13
            #define ADC_YPOS    12
        #endif

		// Y port definitions
        #define ADPCFG_XPOS AD1PCFGbits.PCFG13
        #define LAT_XPOS    LATBbits.LATB13
        #define LAT_XNEG    LATBbits.LATB11
        #define TRIS_XPOS   TRISBbits.TRISB13
        #define TRIS_XNEG   TRISBbits.TRISB11

		// X port definitions
        #define ADPCFG_YPOS AD1PCFGbits.PCFG12
        #define LAT_YPOS    LATBbits.LATB12
        #define LAT_YNEG    LATBbits.LATB10
        #define TRIS_YPOS   TRISBbits.TRISB12
        #define TRIS_YNEG   TRISBbits.TRISB10

    #elif (GRAPHICS_HARDWARE_PLATFORM == GFX_PICTAIL_V2)
        #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
            #define ADC_XPOS    5
            #define ADC_YPOS    4
        #elif defined(__PIC32MX__)
            #define ADC_XPOS    ADC_CH0_POS_SAMPLEA_AN11
            #define ADC_YPOS    ADC_CH0_POS_SAMPLEA_AN10
        #else
            #define ADC_XPOS    11
            #define ADC_YPOS    10
        #endif
        #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)

			// Y port definitions
            #define ADPCFG_XPOS AD1PCFGLbits.PCFG5
            #define LAT_XPOS    LATBbits.LATB3
            #define LAT_XNEG    LATCbits.LATC9
            #define TRIS_XPOS   TRISBbits.TRISB3
            #define TRIS_XNEG   TRISCbits.TRISC9

			// X port definitions
            #define ADPCFG_YPOS AD1PCFGLbits.PCFG4
            #define LAT_YPOS    LATBbits.LATB2
            #define LAT_YNEG    LATCbits.LATC8
            #define TRIS_YPOS   TRISBbits.TRISB2
            #define TRIS_YNEG   TRISCbits.TRISC8

        #else

			// Y port definitions
            #define ADPCFG_XPOS AD1PCFGbits.PCFG11
            #define LAT_XPOS    LATBbits.LATB11
            #define LAT_XNEG    LATGbits.LATG13
            #define TRIS_XPOS   TRISBbits.TRISB11
            #define TRIS_XNEG   TRISGbits.TRISG13

			// X port definitions
            #define ADPCFG_YPOS AD1PCFGbits.PCFG10
            #define LAT_YPOS    LATBbits.LATB10
            #define LAT_YNEG    LATGbits.LATG12
            #define TRIS_YPOS   TRISBbits.TRISB10
            #define TRIS_YNEG   TRISGbits.TRISG12
        #endif
    #elif (GRAPHICS_HARDWARE_PLATFORM == GFX_PICTAIL_V3)
        #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
            #define ADC_XPOS    5
            #define ADC_YPOS    4
        #elif defined(__PIC32MX__)
            #define ADC_XPOS    ADC_CH0_POS_SAMPLEA_AN11
            #define ADC_YPOS    ADC_CH0_POS_SAMPLEA_AN10
        #else
            #define ADC_XPOS    11
            #define ADC_YPOS    10
        #endif
        #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)

			// Y port definitions
            #define ADPCFG_XPOS AD1PCFGLbits.PCFG5
            #define LAT_XPOS    LATBbits.LATB3
            #define LAT_XNEG    LATCbits.LATC9
            #define TRIS_XPOS   TRISBbits.TRISB3
            #define TRIS_XNEG   TRISCbits.TRISC9

			// X port definitions
            #define ADPCFG_YPOS AD1PCFGLbits.PCFG4
            #define LAT_YPOS    LATBbits.LATB2
            #define LAT_YNEG    LATCbits.LATC8
            #define TRIS_YPOS   TRISBbits.TRISB2
            #define TRIS_YNEG   TRISCbits.TRISC8

        #else

			// Y port definitions
            #define ADPCFG_XPOS AD1PCFGbits.PCFG11
            #define LAT_XPOS    LATBbits.LATB11
            #define TRIS_XPOS   TRISBbits.TRISB11

            #if defined(__32MX460F512L__) && !defined (PIC32_USB_STK)
                #define LAT_XNEG    LATGbits.LATG15
                #define TRIS_XNEG   TRISGbits.TRISG15
            #else
                #define LAT_XNEG    LATDbits.LATD9
                #define TRIS_XNEG   TRISDbits.TRISD9
            #endif

			// X port definitions
            #define ADPCFG_YPOS AD1PCFGbits.PCFG10
            #define LAT_YPOS    LATBbits.LATB10
            #define LAT_YNEG    LATDbits.LATD8
            #define TRIS_YPOS   TRISBbits.TRISB10
            #define TRIS_YNEG   TRISDbits.TRISD8
        #endif
    #elif (GRAPHICS_HARDWARE_PLATFORM == DA210_DEV_BOARD)
        #if defined(__PIC24FJ256DA210__)
            #define ADC_XPOS    16
            #define ADC_YPOS    18

			// Y port definitions
            #define ADPCFG_XPOS ANSCbits.ANSC4
            #define LAT_XPOS    LATCbits.LATC4
            #define TRIS_XPOS   TRISCbits.TRISC4
            #define LAT_XNEG    LATAbits.LATA2
            #define TRIS_XNEG   TRISAbits.TRISA2
         
			// X port definitions
            #define ADPCFG_YPOS ANSGbits.ANSG7
            #define LAT_YPOS    LATGbits.LATG7
            #define TRIS_YPOS   TRISGbits.TRISG7
        	#define LAT_YNEG    LATAbits.LATA1
            #define TRIS_YNEG   TRISAbits.TRISA1
        #endif
    #endif

/*********************************************************************
* IOS FOR THE SIDE BUTTONS
*********************************************************************/
    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
        #define BTN_S3  _RA9
        #define BTN_S4  0
        #define BTN_S5  0
        #define BTN_S6  0
    #elif defined(__PIC24FJ256DA210__)
        #define BTN_S1  _RG8
        #define BTN_S2  _RE9
        #define BTN_S3  _RB5
	#elif defined (__32MX460F512L__) && defined (PIC32_USB_STK) 
		#if defined (USE_16BIT_PMP)
			// cannot use PIC32 Starter kit buttons when 16 bit PMP is enabled.
			// PMP is using the IO pins 
	        #define BTN_S3  0 
			#define BTN_S4  0
			#define BTN_S5  0
	        #define BTN_S6  0
		#else
	        #define BTN_S3  _RD13
			#define BTN_S4  _RD7
			#define BTN_S5  _RD6
			#define BTN_S6  0
		#endif 
	#else
        #define BTN_S3  _RD6
        #define BTN_S4  _RD13
        #define BTN_S5  _RA7
        #define BTN_S6  _RD7
    #endif

/*********************************************************************
* IOS FOR THE TCON 
*********************************************************************/
#if (GRAPHICS_HARDWARE_PLATFORM == DA210_DEV_BOARD)
	#if (DISPLAY_PANEL == TFT_G240320LTSW_118W_E)
		#define CS_PORT     _RA0
		#define CS_TRIS     _TRISA0
		#define CS_DIG()
	    
		#define SCL_PORT    _RD8
		#define SCL_TRIS    _TRISD8
		#define SCL_DIG()
	    
		#define SDO_PORT    _RB1
		#define SDO_TRIS    _TRISB1
		#define SDO_DIG()   _ANSB1 = 0;
	    
		#define DC_PORT     _RB0
		#define DC_TRIS     _TRISB0
		#define DC_DIG()    _ANSB0 = 0;

	#endif // #if (DISPLAY_PANEL == TFT_G240320LTSW_118W_E)
#endif // #if (GRAPHICS_HARDWARE_PLATFORM == DA210_DEV_BOARD)

#endif //ifdef _GRAPHICS_H


/*********************************************************************
* IOS FOR THE UART
*********************************************************************/
    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
        #define TX_TRIS TRISCbits.TRISC1
        #define RX_TRIS TRISCbits.TRISC0
	#elif defined(__PIC24FJ256DA210__)    
        #define TX_TRIS TRISFbits.TRISF3
        #define RX_TRIS TRISCbits.TRISC14
    #else
        #define TX_TRIS TRISFbits.TRISF5
        #define RX_TRIS TRISFbits.TRISF4
    #endif

/*********************************************************************
* RTCC DEFAULT INITIALIZATION (these are values to initialize the RTCC
*********************************************************************/
    #define RTCC_DEFAULT_DAY        18      // 18
    #define RTCC_DEFAULT_MONTH      11      // November
    #define RTCC_DEFAULT_YEAR       9       // 2009
    #define RTCC_DEFAULT_WEEKDAY    05      // Friday
    #define RTCC_DEFAULT_HOUR       10      // 10:10:01
    #define RTCC_DEFAULT_MINUTE     10
    #define RTCC_DEFAULT_SECOND     01

#endif // __HARDWARE_PROFILE_H


