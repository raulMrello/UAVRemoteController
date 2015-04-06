/*************************************************************************//**
 * @file    UART_defines.h
 * @mcu		STM32F1x
 * @brief   UART common structure and buffer definitions
 * @date    11.06.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef __UART_defines_H__
#define __UART_defines_H__


/****************************************************************************************************//**
 * @def		TBUF_SIZE
 * @brief   Trasnmission buffer max size
 ****************************************************************************/
#define TBUF_SIZE   128	     /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/
#if TBUF_SIZE < 2
#error TBUF_SIZE is too small.  It must be larger than 1.
#elif ((TBUF_SIZE & (TBUF_SIZE-1)) != 0)
#error TBUF_SIZE must be a power of 2.
#endif

/****************************************************************************************************//**
 * @def		RBUF_SIZE
 * @brief   Reception buffer max size
 ****************************************************************************/
#define RBUF_SIZE   128      /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/
#if RBUF_SIZE < 2
#error RBUF_SIZE is too small.  It must be larger than 1.
#elif ((RBUF_SIZE & (RBUF_SIZE-1)) != 0)
#error RBUF_SIZE must be a power of 2.
#endif


/****************************************************************************************************//**
 * @struct  buf_tx
 * @brief   Trasnmission buffer structure.
 *			The length of the receive and transmit buffers must be a power of 2.
 *			Each buffer has a next_in and a next_out index.
 *			If next_in = next_out, the buffer is empty.
 *			(next_in - next_out) % buffer_size = the number of characters in the buffer.
 ****************************************************************************/
typedef struct  {
  unsigned int in;       	//!< Next In index
  unsigned int out;   		//!< Next Out index
  unsigned int tx_restart;  //!< ??
  char buf [TBUF_SIZE];     //!< Tx buffer definition
}buf_tx;


/****************************************************************************************************//**
 * @struct  buf_rx
 * @brief   Reception buffer structure
 ****************************************************************************/
typedef struct  {
  unsigned int in;      	//!< Next In index
  unsigned int out;   	    //!< Next Out index
  char buf [RBUF_SIZE];     //!< Tx buffer definition
}buf_rx;


/****************************************************************************************************//**
 * @struct  uart_ctrl_t
 * @brief   Tx/Rx buffer group control structure
 ****************************************************************************/ 
typedef struct {
   buf_tx 		tbuf;	 	//!< TX buffer 
   buf_rx 		rbuf;		//!< RX buffer
}uart_ctrl_t;

#endif
