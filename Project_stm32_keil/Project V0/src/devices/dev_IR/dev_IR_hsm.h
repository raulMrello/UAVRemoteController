/*************************************************************************//**
 * @file    dev_IR_hsm.h
 * @mcu		STM32F1x
 * @brief   IR Device State Machine protected header
 * @date    15.06.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef __dev_IR_HSM_H__
#define __dev_IR_HSM_H__

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <stdint.h>
#include "StateFramework.h"
#include "IRunnable.h"
#include "dev_IR.h"

/****************************************************************************************************//**
 *                        	REQUIRED INTERFACES
 ********************************************************************************************************/
#define IDriverInit(baudrate)	dev_IR_Requires_IDriverInit(baudrate)
#define IDriverEnable()			dev_IR_Requires_IDriverEnable()
#define IDriverTurnOn()			dev_IR_Requires_IDriverTurnOn()
#define IDriverTurnOff()		dev_IR_Requires_IDriverTurnOff()
#define IDriverEnableRx()		dev_IR_Requires_IDriverEnableRx()
#define IDriverDisableRx()		dev_IR_Requires_IDriverDisableRx()
#define IGetData()				dev_IR_Requires_IGetData()
#define ISend(ptr,size)			dev_IR_Requires_ISend(ptr,size)					
#define IDataReady(pdata,len)	dev_IR_Requires_IDataReady(pdata,len)		


/****************************************************************************************************//**
 *                        PROTECTED STATE DEFINITIONS
 ********************************************************************************************************/

State IR_State_Initial			(Object* pThis, Event const * e);
State IR_State_Top				(Object* pThis, Event const * e);
State IR_State_WaitingAck		(Object* pThis, Event const * e);
State IR_State_WaitingData		(Object* pThis, Event const * e);

/****************************************************************************************************//**
 *                        PROTECTED TYPES
 ********************************************************************************************************/


/*************************************************************************//**
 * @def		IR_RECV_TIMEOUT_ENABLE
 * @scope	protected
 * @brief   Key to enable watchdog timing during data reception.
 ****************************************************************************/
#define IR_RECV_TIMEOUT_ENABLE	1


/*************************************************************************//**
 * @def		IR_MAX_PDU_LENGTH
 * @def		IR_MAX_PDU_ARGS
 * @scope	protected
 * @brief   Max number of bytes for PDU at application layer. Both for writing
 *			operations IR_MAX_PDU_LENGTH and for read operations IR_MAX_PDU_ARGS
 ****************************************************************************/
#define IR_MAX_PDU_LENGTH		28
#define IR_MAX_PDU_ARGS			27

/*************************************************************************//**
 * @struct	T_IR_PDU
 * @scope	protected
 * @brief   Data estructure for IR PDU streams (decoded) and packed.
 ****************************************************************************/
__packed typedef struct{
    uint8_t   pduType;    						//!< pdu size in bytes
    uint8_t   pduData[IR_MAX_PDU_ARGS];  		//!< pdu data
}T_IR_Pdu;

/*************************************************************************//**
 * @def		IR_FIXED_MAC_LENGTH
 * @def		IR_MAX_MAC_LENGTH
 * @scope	protected
 * @brief   Fixed and Max number of bytes for IR streams at protocol layer
 ****************************************************************************/
#define IR_FIXED_MAC_LENGTH		4
#define IR_MAX_MAC_LENGTH		(IR_FIXED_MAC_LENGTH + IR_MAX_PDU_LENGTH)

/*************************************************************************//**
 * @struct	T_IR_Mac
 * @scope	protected
 * @brief   Data estructure for IR RAW streams coded by Manchester and packed.
 ****************************************************************************/
__packed typedef struct{
    uint8_t   headFlag;    					//!< pdu head flag
    uint8_t   pduSize;    					//!< pdu size in bytes
    uint8_t   pdu[IR_MAX_PDU_LENGTH];  		//!< pdu data
    uint8_t   crc16[2];     				//!< crc16 (MSB->LSB)
}T_IR_Mac;

/*************************************************************************//**
 * @struct	E_IR_Mac_Addr
 * @scope	protected
 * @brief   Fixed positions for each kind of data in the IR_MAC stream
 ****************************************************************************/
typedef enum {
    IR_MAC_ADDR_HEADFLAG   = 0,		//!< head flag is at 1st byte
    IR_MAC_ADDR_PDULENGTH,			//!< pdu size is at 2nd byte
    IR_MAC_ADDR_PDUDATA				//!< pdu data is at 3rd byte
}E_IR_Mac_Addr;

/*************************************************************************//**
 * @def		IR_PREAMBLE_LENGTH
 * @scope	protected
 * @brief   Size in bytes of the preamble IR stream
 ****************************************************************************/
#define IR_PREAMBLE_LENGTH		8

/*************************************************************************//**
 * @def		IR_PREAMBLE_LENGTH_ERROR
 * @scope	protected
 * @brief   Size in bytes of the preamble IR stream error checking
 ****************************************************************************/
#define IR_PREAMBLE_LENGTH_ERROR	16

/*************************************************************************//**
 * @def		IR_MAX_MAC_LENGTH
 * @scope	protected
 * @brief   Max number of bytes for IR streams at protocol layer
 ****************************************************************************/
#define IR_MAX_PHY_LENGTH	(IR_PREAMBLE_LENGTH + (2*IR_MAX_MAC_LENGTH))

/*************************************************************************//**
 * @struct	T_IR_RAW_Stream
 * @scope	protected
 * @brief   Data estructure for IR RAW streams at physical layer
 ****************************************************************************/
__packed typedef struct{
    uint8_t   preamble[IR_PREAMBLE_LENGTH]; //!< preamble manchester coded
    uint8_t   data[(2*IR_MAX_MAC_LENGTH)];	//!< data manchester coded
}T_IR_Phy;

/*************************************************************************//**
 * @def		IR_PREAMBLE_FLAG
 * @scope	protected
 * @brief   Preamble flag to identify the beginning of a receving coded stream
 ****************************************************************************/
#define IR_PREAMBLE_FLAG		0x55

/*************************************************************************//**
 * @def		IR_MANCHESTER_FLAG
 * @scope	protected
 * @brief   Manchester flag to code an uncoded pdu.
 ****************************************************************************/
#define IR_MANCHESTER_FLAG		0x55

/*************************************************************************//**
 * @def		IR_HEADFLAG
 * @scope	protected
 * @brief   Head flag decoded
 ****************************************************************************/
#define IR_HEADFLAG				0xF1

/*************************************************************************//**
 * @def		IR_HEADFLAG_MSB
 * @def		IR_HEADFLAG_LSB
 * @scope	protected
 * @brief   Head flag coded as two bytes
 ****************************************************************************/
#define IR_HEADFLAG_MSB			0xAA
#define IR_HEADFLAG_LSB			0x56

/*************************************************************************//**
 * @def		IR_PDUTYPE_RW_MASK
 * @def		IR_PDUTYPE_READ_OP
 * @def		IR_PDUTYPE_WR_MASK
 * @scope	protected
 * @brief   Definitions aplicable to <pduType> byte. This masks set/get the
 *			type of pdu operation: Write or Read, depending on the bit status
 ****************************************************************************/
#define IR_PDUTYPE_RW_MASK         0x80
#define IR_PDUTYPE_READ_OP         0x80
#define IR_PDUTYPE_WR_MASK         0x7F

/*************************************************************************//**
 * @enum	E_IR_PDU_TYPES
 * @scope	protected
 * @brief   PDU types accepted by this device
 ****************************************************************************/
typedef enum {
	IR_PDU_ZONE_WR = 0,			//!< zone pdu write request
	IR_PDU_PROG_WR = 1,			//!< program pdu write request
	IR_PDU_SETT_WR = 2,			//!< settings pdu write request
	IR_PDU_CLOCK_WR = 3,		//!< clock pdu write request
	IR_PDU_SEAS_WR = 4,			//!< seasons pdu write request
	IR_PDU_VACS_WR = 9,			//!< seasons pdu write request
	IR_PDU_ZONE_RD = (IR_PDUTYPE_READ_OP + IR_PDU_ZONE_WR),		//!< zone pdu read request
	IR_PDU_PROG_RD = (IR_PDUTYPE_READ_OP + IR_PDU_PROG_WR),		//!< zone pdu read request
	IR_PDU_SETT_RD = (IR_PDUTYPE_READ_OP + IR_PDU_SETT_WR),		//!< zone pdu read request
	IR_PDU_CLOCK_RD = (IR_PDUTYPE_READ_OP + IR_PDU_CLOCK_WR),	//!< zone pdu read request
	IR_PDU_SEAS_RD = (IR_PDUTYPE_READ_OP + IR_PDU_SEAS_WR),		//!< zone pdu read request
	IR_PDU_VACS_RD = (IR_PDUTYPE_READ_OP + IR_PDU_VACS_WR) 		//!< zone pdu read request
}E_IR_PDU_TYPES;

/*************************************************************************//**
 * @def		IR_RECV_EVT_TIMEOUT
 * @scope	protected
 * @brief   Returns the number of OS_TICK for a delay of 50 milliseconds while
 *			waiting for a data received response
 ****************************************************************************/
#define IR_RECV_EVT_TIMEOUT		GetOSTicks(50)

/*************************************************************************//**
 * @def		IR_MBX_TIMEOUT
 * @scope	protected
 * @brief   Returns the number of OS_TICK for a delay of 1000 milliseconds while
 *			waiting for a mailbox message
 ****************************************************************************/
#define IR_MBX_TIMEOUT		GetOSTicks(1000)

/*************************************************************************//**
 * @def		IR_RECV_TIMEOUT
 * @scope	protected
 * @brief   Returns the number of OS_TICK for a delay of 50 milliseconds while
 *			waiting for a valid data received response
 ****************************************************************************/
#define IR_RECV_TIMEOUT		GetOSTicks(50)

/*************************************************************************//**
 * @enum	E_IR_EVENTS
 * @scope	protected
 * @brief   Event signals accepted by the state machine of this device
 ****************************************************************************/
typedef enum {
	EV_SEND = USER_SIG,		//!< event through Send interface
	EV_READ,				//!< event through Read interface
	EV_DATARECV,			//!< event due to EVF_DATARECV through NewData(0) interface
	EV_DATASENT,			//!< event due to EVF_DATASENT through NewData(1) interface
	EV_CANCELLED,			//!< event due to EVF_CANCELLED through Cancel interface
	EV_RECVTOUTOVF			//!< event due to EVF_RECVTOUTOVF through WatchdogOverflow
}E_IR_EVENTS;

/*************************************************************************//**
 * @enum	E_IR_FLAGS
 * @scope	protected
 * @brief   EventFlags masks accepted by this device during os_evt_wait_or
 ****************************************************************************/
typedef enum {
	EVF_DATARECV  	= 0x0001,	//!< event through NewData(0) interface
	EVF_DATASENT  	= 0x0002,	//!< event through NewData(1) interface
	EVF_CANCELLED 	= 0x0004,	//!< event through Cancel interface
	EVF_RECVTOUTOVF   = 0x0008,	//!< event through WatchdogOverflow
	EVF_IR_ALLFLASG = (EVF_DATARECV | EVF_DATASENT | EVF_CANCELLED | EVF_RECVTOUTOVF)
}E_IR_FLAGS;


/*************************************************************************//**
 * @struct	T_IR_Message
 * @scope	protected
 * @brief   This structure is the message type accepted by the object. As can
 *			be seen, its content can vary according with the public interface
 *			invoked from an external component. This implies than both SendMsg
 *			and ReadMsg can be thrown to this object and inserted into the mem-
 *			pool as a block of size = sizeof(T_IR_Message).
 ****************************************************************************/
typedef struct  {
    Event 	super;          	//!< derives from Event
	union {
		struct  {				//!< event structure for dev_IR_Send interface
			uint8_t	 len;
			uint8_t  pdu[IR_MAX_PDU_LENGTH];
		} SendMsg;
		struct  {				//!< event structure for dev_IR_Read interface
			uint8_t	 len;
			uint8_t  pdu[IR_MAX_PDU_LENGTH];
		} ReadMsg;			
	}msgContent;
}T_IR_Message;

/*************************************************************************//**
 * @struct	T_IR_Manchester
 * @scope	protected
 * @brief   This structure is the manchester coder/decoder to manage its non-
 *			blocking operations.
 ****************************************************************************/
typedef struct{
    uint8_t   status;     //!< status during manchester decoding operations
    uint8_t   msbByte;    //!< Byte MSB
    uint8_t   lsbByte;    //!< Byte LSB
    uint8_t   result;     //!< resulting decoded byte operation
}T_IR_Manchester;

/*************************************************************************//**
 * @enum	E_IR_MODES
 * @scope	protected
 * @brief   This structure is the status mode during reception operations
 ****************************************************************************/
typedef enum {
    modeDISCONNECTED = 0,
    modeBINDING,
    modeACCEPTED,
    modeRECEIVING,
    modeENDING,    
    modeWRITING, 
    modeACKING
}E_IR_MODES;

/*************************************************************************//**
 * @enum	E_IR_ERRORS
 * @scope	protected
 * @brief   Error codes for IR reception operation
 ****************************************************************************/
typedef enum {
	ERR_IR_OK = 0,
	ERR_IR_PENDING,
	ERR_IR_PREAMBLE,
    ERR_IR_HEADFLAG,
	ERR_IR_PDULENGTH,
	ERR_IR_CHECKSUM,
	ERR_IR_UNKNOWN
} E_IR_ERRORS;

/*************************************************************************//**
 * @struct	T_DEV_IR
 * @scope	protected
 * @brief   This structure is the internal device structure with all the static
 *			variable required for functionality.
 ****************************************************************************/
typedef struct{
	IRunnable 		runnable;	//!< Interface to execute a thread with a mailhox
	uint32_t		uid;		//!< Component UID
	uint32_t		timeout;	//!< timeout for os_mbx_wait or os_evt_wait_or
	T_IR_Message *	msg;		//!< pointer to the message in process
	OS_ID			recvTimeout;//!< timeout to save unprocessable reception data
	E_WAITFOR 		waitfor;	//!< waiting method: mbx or evt
	T_IR_Manchester	manchester;	//!< manchester decoder manager
	uint16_t		crc16;		//!< CRC16 temporal result
	E_IR_MODES		smMode;		//!< reception operation status mode
	uint8_t			nrx;		//!< number of received bytes during read
	uint8_t		 *	rxBuff;		//!< reception buffer with decoded data
	uint8_t			ntx;		//!< number of bytes to send 
	uint8_t		 *	txBuff;		//!< transmission buffer to code data into
}T_DEV_IR;


/****************************************************************************************************//**
 *                        PROTECTED FUNTIONS
 ********************************************************************************************************/
uint8_t   	IsPduTypeValid		(uint8_t pduType);				//!< checks the pduType to be valid
void 		InitReceptionProcess(T_DEV_IR* me);
uint16_t  	CodeToManchester	(uint8_t * rawStream, uint8_t * pdu, uint16_t size);
E_IR_ERRORS	DecodeManchester	(T_IR_Manchester *This, uint8_t data);
E_IR_ERRORS ProcessByteReceived	(T_DEV_IR *me, uint8_t data);
//void		IR_WatchdogFeed		(void);
//void		IR_WatchdogStart	(void);
void		IR_ResetRecvTimeout	(void);
void		IR_StartRecvTimeout	(void);

#endif

