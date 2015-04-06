/*************************************************************************//**
 * @file    drv_SPI2.c
 * @mcu		STM32F1x
 * @brief   SPI2 peripheral controller
 * @date    14.06.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <stdio.h>
#include "stm32f10x.h"
#include "drv_SPI2.h"
#include "drv_gpio.h"


/****************************************************************************************************//**
 *                        PRIVATE MEMBERS
 ********************************************************************************************************/

void Init_SingleWire_TX	(void);
void Init_SingleWire_RX	(void);
void Init_TwoWires		(void);
int  TxBusy				(void);


/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/****************************************************************************************************//**
 * @fun		drv_SPI2_InitByDefault
 * @brief	Provided interface to setup the driver to its default configuration
 * @return	n/a
 **********************************************************************************/
void drv_SPI2_InitByDefault(void){
	Init_TwoWires();
}
/****************************************************************************************************//**
 * @fun		drv_SPI2_Init
 * @brief	Provided interface to setup the driver to its default configuration
 * @param	type		type of initialization: singleTX, singleRx, twoWires.
 * @return	n/a
 **********************************************************************************/
void drv_SPI2_Init(uint32_t type){
	switch(type){
		case SPI2_SINGLE_WIRE_TX:
			Init_SingleWire_TX();
		case SPI2_SINGLE_WIRE_RX:
			Init_SingleWire_RX();
			break;
		case SPI2_TWO_WIRES:
			Init_TwoWires();
			break;
	}
}

/****************************************************************************************************//**
 * @fun		drv_SPI2_Deinit
 * @brief	Provided interface to unload the driver to its default configuration
 * @return	n/a
 **********************************************************************************/
void drv_SPI2_Deinit(void){
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	SPI_Cmd(SPI2, DISABLE);
}


/****************************************************************************************************//**
 * @fun		drv_SPI2_SendChar
 * @brief	Provided interface to send a character to the physical device
 * @param	c		character to send
 * @return	n/a
 **********************************************************************************/
int drv_SPI2_SendChar (int c) {

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);		// Wait for SPI_MASTER Tx buffer empty 	
	
	SPI_I2S_SendData(SPI2, c);											// Send SPI_MASTER data 

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);		// Wait for SPI_MASTER Tx buffer empty 	

  	return (0);
}

/****************************************************************************************************//**
 * @fun		drv_SPI2_GetChar
 * @brief	Provided interface to read a character from the physical device
 * @param	*c		character read
 * @return	0(n/a)
 **********************************************************************************/
int drv_SPI2_GetChar (int *c) {

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);		// Wait for SPI_MASTER RX buffer not empty 	
	
	*c = SPI_I2S_ReceiveData(SPI2);


	return (0);
}

/****************************************************************************************************//**
 * @fun		drv_SPI2_SendChar_fullDuplex
 * @brief	Provided interface to send a character and read another
 * @param	c		character to send
 * @return	d		character read
 **********************************************************************************/
int drv_SPI2_SendChar_fullDuplex (int c) {

    int kk;

 	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);		// Wait for SPI_MASTER Tx buffer empty 	
	
	SPI_I2S_SendData(SPI2, c);	

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);		// Wait for SPI_MASTER RX buffer not empty 	
	
	kk = SPI_I2S_ReceiveData(SPI2);


	return kk;

}


/****************************************************************************************************//**
 * @fun		Init_SingleWire_RX
 * @brief	Private func. to initialize in single wire RX.
 * @return	n/a
 **********************************************************************************/
void Init_SingleWire_RX(void) {
  
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);


  //  Configure SPI2 pins: CK, WS and SD 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

  // SPI_MASTER configuration 
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Rx;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	//SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);

	SPI_Cmd(SPI2, ENABLE);
}


/****************************************************************************************************//**
 * @fun		Init_SingleWire_TX
 * @brief	Private func. to initialize in single wire TX.
 * @return	n/a
 **********************************************************************************/
void Init_SingleWire_TX(void) {
  
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

  	//Configure SPI2 pins: CK, WS and SD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	*/
	

	SPI_I2S_DeInit(SPI2);

  	//SPI_MASTER configuration 
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;

	//SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);

	SPI_Cmd(SPI2, ENABLE);

}


/****************************************************************************************************//**
 * @fun		Init_TwoWires
 * @brief	Private func. to initialize in two wires.
 * @return	n/a
 **********************************************************************************/
void Init_TwoWires(void) {

	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

  	//Configure SPI2 pins: CK, WS and SD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	SPI_Cmd(SPI2, DISABLE);
	SPI_I2S_DeInit(SPI2);

  // SPI_MASTER configuration 
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	//SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);

	SPI_Cmd(SPI2, ENABLE);
}


/****************************************************************************************************//**
 * @fun		TxBusy
 * @brief	Private func. to read busy flag 
 * @return	n/a
 **********************************************************************************/
int TxBusy(void){
 	return SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE);		// Wait for SPI_MASTER Tx buffer empty 	
}
