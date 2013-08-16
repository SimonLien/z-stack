/**************************************************************************************************
  Filename:       hal_uart.h
  Revised:        $Date: 2009-03-09 05:27:36 -0700 (Mon, 09 Mar 2009) $
  Revision:       $Revision: 19340 $

  Description:    This file contains the interface to the UART Service.

  Copyright 2005-2009 Texas Instruments Incorporated. All rights reserved.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com. 
**************************************************************************************************/

#ifndef HAL_UART_H
#define HAL_UART_H

#ifdef __cplusplus
extern "C"
{
#endif

/***************************************************************************************************
 *                                             INCLUDES
 ***************************************************************************************************/
#include "hal_board.h"
#include "ZComDef.h"

/***************************************************************************************************
 *                                              MACROS
 ***************************************************************************************************/

/***************************************************************************************************
 *                                            CONSTANTS
 ***************************************************************************************************/
/* UART Ports */

/* Serial Port Baudrate Settings Have to match with baudrate table  
   Note that when using interrupt service based UART configuration (as opposed to DMA)
   higher baudrate such as 115200bps may have problem when radio is operational at the same time.*/

/* Frame Format constant */
/* BaudRate */
#define HAL_UART_BR_9600          0x00
#define HAL_UART_BR_19200         0x01
#define HAL_UART_BR_38400         0x02
#define HAL_UART_BR_57600         0x03
#define HAL_UART_BR_115200        0x04
/* Stop Bits */
#define HAL_UART_ONE_STOP_BIT     0x00
#define HAL_UART_TWO_STOP_BITS    0x01
/* Parity settings */
#define HAL_UART_NO_PARITY        0x00
#define HAL_UART_EVEN_PARITY      0x01
#define HAL_UART_ODD_PARITY       0x02
/* Character Size */
#define HAL_UART_8_BITS_PER_CHAR  0x00
#define HAL_UART_9_BITS_PER_CHAR  0x01
/* Flow control */
#define HAL_UART_FLOW_OFF         0x00
#define HAL_UART_FLOW_ON          0x01
/* Ports */
#define HAL_UART_PORT_0           0x00
#define HAL_UART_PORT_1           0x01
#define HAL_UART_PORT_MAX         0x02
/* UART Status */
#define  HAL_UART_SUCCESS         0x00
#define  HAL_UART_UNCONFIGURED    0x01
#define  HAL_UART_NOT_SUPPORTED   0x02
#define  HAL_UART_MEM_FAIL        0x03
#define  HAL_UART_BAUDRATE_ERROR  0x04
/* UART Events */
#define HAL_UART_RX_FULL          0x01
#define HAL_UART_RX_ABOUT_FULL    0x02
#define HAL_UART_RX_TIMEOUT       0x04
#define HAL_UART_TX_FULL          0x08
#define HAL_UART_TX_EMPTY         0x10

/***************************************************************************************************
 *                                             TYPEDEFS
 ***************************************************************************************************/
typedef void (*halUARTCBack_t) (uint8 port, uint8 event);

typedef struct
{
  // The head or tail is updated by the Tx or Rx ISR respectively, when not polled.
  volatile uint16 bufferHead;
  volatile uint16 bufferTail;
  uint16 maxBufSize;
  uint8 *pBuffer;
} halUARTBufControl_t;

typedef struct
{
  bool                configured;
  uint8               baudRate;
  bool                flowControl;
  uint16              flowControlThreshold;
  uint8               idleTimeout;
  halUARTBufControl_t rx;
  halUARTBufControl_t tx;
  bool                intEnable;
  uint32              rxChRvdTime;
  halUARTCBack_t      callBackFunc;
}halUARTCfg_t;

typedef union
{
  bool paramCTS;
  bool paramRTS;
  bool paramDSR;
  bool paramDTR;
  bool paramCD;
  bool paramRI;
  uint16 baudRate;
  bool flowControl;
  bool flushControl;
}halUARTIoctl_t;

/***************************************************************************************************
 *                                           GENERAL VARIABLES
 ***************************************************************************************************/
extern  byte zclZigbeeUARTRecv_TaskID;

/***************************************************************************************************
 *                                           Local VARIABLES
 ***************************************************************************************************/
/* The UART function _ zcluartmsg_Init parameter */
#define SERIAL_APP_PORT  0
#define zcluartmsg_BAUD  HAL_UART_BR_115200
#define zcluartmsg_THRESH  64
#define zcluartmsg_RX_SZ  128
#define zcluartmsg_TX_SZ  128
#define zcluartmsg_IDLE  6

/***************************************************************************************************
 *                                            FUNCTIONS - API
 ***************************************************************************************************/
/*
 *  Initialize UART at the startup
 */
extern void HalUARTInit ( void );

/*
 * Open a port based on the configuration
 */
extern uint8 HalUARTOpen ( uint8 port, halUARTCfg_t *config );

/*
 * Close a port
 */
extern void HalUARTClose ( uint8 port );

/*
 * Read a buffer from the UART
 */
extern uint16 HalUARTRead ( uint8 port, uint8 *pBuffer, uint16 length );

/*
 * Write a buff to the uart
 */
extern uint16 HalUARTWrite ( uint8 port, uint8 *pBuffer, uint16 length );

/*
 * Write a buffer to the UART
 */
extern uint8 HalUARTIoctl ( uint8 port, uint8 cmd, halUARTIoctl_t *pIoctl );

/*
 * This to support polling
 */
extern void HalUARTPoll( void );

/*
 * Return the number of bytes in the Rx buffer
 */
extern uint16 Hal_UART_RxBufLen ( uint8 port );

/*
 * Return the number of bytes in the Tx buffer
 */
extern uint16 Hal_UART_TxBufLen ( uint8 port );

/*
 * This enable/disable flow control
 */
extern void Hal_UART_FlowControlSet ( uint8 port, bool status );

/*
 * Initialize hardware for UART
 */
extern uint8 HalUART_HW_Init(uint8 port);

/*
 * Abort UART when entering sleep mode
 */
extern void HalUARTSuspend(void);

/*
 * Resume UART after wakeup from sleep
 */
extern void HalUARTResume(void);

/***************************************************************************************************
***************************************************************************************************/
/*
 * Initilization the UART to receive command
 */
extern void zcluartmsg_Init( byte task_id );


#ifdef __cplusplus
}
#endif

#endif
