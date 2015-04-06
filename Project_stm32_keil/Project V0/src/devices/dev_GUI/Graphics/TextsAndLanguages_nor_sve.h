/*************************************************************************//**
 * @file    dev_GUI_hsm.h
 * @mcu		STM32F1x
 * @brief   GUI Device State Machine protected header
 *			THIS FILE IS GENERATED AUTOMATICALLY!!!!!!
 * @date    15.06.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef __TEXTSANDLANGUAGES_NOR_SVE_H__
#define __TEXTSANDLANGUAGES_NOR_SVE_H__

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <stdint.h>

/****************************************************************************************************//**
 *                        PUBLIC TYPES
 ********************************************************************************************************/

/*************************************************************************//**
 * @def		MAX_LANGUAGES
 * @scope	public
 * @brief   Max number of languages supported by the device
 ****************************************************************************/
#define MAX_LANGUAGES	3

/*************************************************************************//**
 * @def		L_ESPANOL L_INGLES L_ITALIANO DEFAULT_LANGUAGE
 * @scope	public
 * @brief   Languages id numbers and default selected language for the device
 ****************************************************************************/
#define L_NORUEGO			0
#define L_SUECO				1
#define L_INGLES			2
#define L_END				255
#define DEFAULT_LANGUAGE	L_NORUEGO

/*************************************************************************//**
 * @def		MAX_COUNTRIES
 * @scope	public
 * @brief   Max number of countries supported by the device
 ****************************************************************************/
#define MAX_COUNTRIES	2

/*************************************************************************//**
 * @def		PAIS_ESPANA PAIS_PORTUGAL PAIS_ITALIA USE_LOADED BASE_COUNTRY
 * @scope	public
 * @brief   Country id numbers and default selected country for the device
 ****************************************************************************/
#define PAIS_NORUEGA		0
#define PAIS_SUECIA			1
#define USE_LOADED			2	//!< IMPOTANT!!!! its value is 1 more than the last country

#define BASE_COUNTRY	USE_LOADED

/*************************************************************************//**
 * @var		TEXTS
 * @scope	public
 * @brief   Exported Array with texts in different languages
 ****************************************************************************/
extern const char * const TEXTS[][MAX_LANGUAGES];


/*************************************************************************//**
 * @var		LSTBOX
 * @scope	public
 * @brief   Exported Array with texts for listbox options in different languages
 ****************************************************************************/
extern const char * const LSTBOX[][MAX_LANGUAGES];


#endif


