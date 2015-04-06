/*************************************************************************//**
 * @file    dev_GUI_hsm.h
 * @mcu		STM32F1x
 * @brief   GUI Device State Machine protected header
 *			THIS FILE IS GENERATED AUTOMATICALLY!!!!!!
 * @date    15.06.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef __TEXTSANDLANGUAGES_ESP_POR_ITA_H__
#define __TEXTSANDLANGUAGES_ESP_POR_ITA_H__

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
#define MAX_LANGUAGES	4

/*************************************************************************//**
 * @def		L_ESPANOL L_INGLES L_ITALIANO DEFAULT_LANGUAGE
 * @scope	public
 * @brief   Languages id numbers and default selected language for the device
 ****************************************************************************/
#define L_ESPANOL			0
#define L_INGLES			1
#define L_ITALIANO			2
#define L_PORTUGUES			3
#define L_END				255
#define DEFAULT_LANGUAGE	L_ESPANOL

/*************************************************************************//**
 * @def		MAX_COUNTRIES
 * @scope	public
 * @brief   Max number of countries supported by the device
 ****************************************************************************/
#define MAX_COUNTRIES	3

/*************************************************************************//**
 * @def		PAIS_ESPANA PAIS_PORTUGAL PAIS_ITALIA USE_LOADED BASE_COUNTRY
 * @scope	public
 * @brief   Country id numbers and default selected country for the device
 ****************************************************************************/
#define PAIS_ESPANA		0
#define PAIS_ITALIA		1
#define PAIS_PORTUGAL	2
#define USE_LOADED		3	//!< IMPOTANT!!!! its value is 1 more than the last country

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


