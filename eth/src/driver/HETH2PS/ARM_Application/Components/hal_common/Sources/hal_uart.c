#include "hal_uart.h"     /* function prototypes and defines */
#include "hw_defines.h"   /* Needed for HW_PTR_UART() */
#include <stdio.h>

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/

HW_PTR_UART(s_aptUart)


/*****************************************************************************/
/*! Init Port
* \description
*   Initialize serial port with custom parameters.
* \class
*   UART
* \params
*   uiPort              [in]  UART instance number
*   ulKbps                [in]  Baud rate in KBit per seconds
*   uDataBits             [in]  Number of data bits (5..8)
*   uParity               [in]  Parity configuration
*                               0: None
*                               1: Odd
*                               2: Even
*                               3: Always 1
*                               4: Always 0
*   uStopBits             [in]  Number of stop bits (1..2)
* \return
*                                                                            */
/*****************************************************************************/
UART_RESULT_E UART_Init( unsigned int uInst,
                         uint32_t     ulBaudrate,
                         unsigned int uDataBits,
                         unsigned int uParity,
                         unsigned int uStopBits )
{
  HW_TYPE(UART_AREA_T)* ptUart;
  uint32_t ulVal;

  if( uInst >= (sizeof(s_aptUart)/sizeof(s_aptUart[0])) )
    return UART_ERR_INVAL_PARAM;

  if( ulBaudrate > 3125000 )
    return UART_ERR_INVAL_PARAM;

  if( ( uDataBits < 5 ) || ( uDataBits > 8 ) )
    return UART_ERR_INVAL_PARAM;

  if( ( uStopBits < 1 ) || ( uStopBits > 2 ) )
    return UART_ERR_INVAL_PARAM;

  ptUart = s_aptUart[uInst];

  /* First of all disable everything */
  ptUart->ulUartcr = 0;

  /* disable flow control */
  ptUart->ulUartrts = 0;

  /* Set the bit for the second baud rate mode */
  ptUart->ulUartcr_2    = HW_MSK(uartcr_2_Baud_Rate_Mode);

  /* Adjust the baud rate register */
  ulVal = (ulBaudrate / 100) << 16;
  ulVal = (ulVal / 62500);
  ptUart->ulUartlcr_l   = ( ulVal & 0x00ff );
  ptUart->ulUartlcr_m   = ( ulVal & 0xff00 ) >> 8;

  /* set UART characteristics */
  ulVal  = HW_MSK(uartlcr_h_FEN);
  ulVal |= (uDataBits - 5) << HW_SRT(uartlcr_h_WLEN);
  if( uParity != 0 )
  {
    ulVal |= HW_MSK(uartlcr_h_PEN);
    if( (uParity & 1) == 0 )
    {
      ulVal |= HW_MSK(uartlcr_h_EPS);
    }
    if( uParity > 2 )
    {
      ptUart->ulUartrts = HW_MSK(uartrts_STICK);
    }
  }
  if( uStopBits == 2 )
  {
    ulVal |= HW_MSK(uartlcr_h_STP2);
  }
  ptUart->ulUartlcr_h = ulVal;

  /* Set TX-Driver to enabled */
  ptUart->ulUartdrvout  = HW_MSK(uartdrvout_DRVTX) | HW_MSK(uartdrvout_DRVRTS);

  /* set default FIFO water marks and enable DMA requests */
  ptUart->ulUartrxiflsel = HW_DFLT_VAL(uartrxiflsel) | HW_MSK(uartrxiflsel_RXDMA);
  ptUart->ulUarttxiflsel = HW_DFLT_VAL(uarttxiflsel) | HW_MSK(uarttxiflsel_TXDMA);

  /* Finally enable the UART */
  ptUart->ulUartcr      = HW_MSK(uartcr_uartEN);

  return UART_OKAY;
}


/*****************************************************************************/
/*! Init serial Port
* \description
*   Initialize serial port.
* \class
*   UART
* \params
*   uiPort              [in]  UART instance number
* \return
*                                                                            */
/*****************************************************************************/
void UART_InitSerial( unsigned int uiPort )
{
  HW_TYPE(UART_AREA_T)* ptUart = s_aptUart[uiPort];

  /* First of all disable everything */
  ptUart->ulUartcr     = 0;

  /* Set the bit for the second baud rate mode */
  ptUart->ulUartcr_2   = HW_MSK(uartcr_2_Baud_Rate_Mode);

  /* Adjust the baud rate register */
  ptUart->ulUartlcr_l  = DEV_BAUDRATE_DIV_LO;
  ptUart->ulUartlcr_m  = DEV_BAUDRATE_DIV_HI;

  /* set UART to 8 bits, 1 stop bit, no parity, FIFO enabled */
  ptUart->ulUartlcr_h  = (HW_MSK(uartlcr_h_WLEN) | HW_MSK(uartlcr_h_FEN));

  /* Set TX-Driver to enabled */
  ptUart->ulUartdrvout = HW_MSK(uartdrvout_DRVTX) | HW_MSK(uartdrvout_DRVRTS);

  /* Finally enable the UART */
  ptUart->ulUartcr     = HW_MSK(uartcr_uartEN);
}

/*****************************************************************************/
/*! Close Serial Port
* \description
*   Closes serial port.
* \class
*   UART
* \params
*   uiPort              [in]  UART instance number
* \return
*                                                                            */
/*****************************************************************************/
void UART_CloseSerial(unsigned int uiPort)
{
  HW_TYPE(UART_AREA_T)* ptUart = s_aptUart[uiPort];

  while((ptUart->ulUartfr & HW_MSK(uartfr_BUSY)) != 0); /* UART busy ? */
  ptUart->ulUartcr     = 0; /* First of all disable everything */
  ptUart->ulUartlcr_m  = 0; /* Adjust the baud rate register */
  ptUart->ulUartlcr_l  = 0;
  ptUart->ulUartlcr_h  = 0; /* UART to be 8 bits, 1 stop bit, no parity, FIFO enabled */
  ptUart->ulUartdrvout = 0; /* Set TX-Driver to disabled */
}

/*****************************************************************************/
/*! Get interrupt(s)
* \description
*   Closes serial port.
* \class
*   UART
* \params
*   uiPort          [in]  UART instance number
* \return
*   IRQ vector
*                                                                            */
/*****************************************************************************/
uint32_t UART_GetIrq( unsigned int uiPort )
{
  return s_aptUart[uiPort]->ulUartiir;
}

/*****************************************************************************/
/*! Confirm interrupt(s)
* \description
*   Closes serial port.
* \class
*   UART
* \params
*   uiPort              [in]  UART instance number
*   ulConfirmMask       [in]  Mask to confirm interrupt(s)
* \return
*                                                                            */
/*****************************************************************************/
void UART_ConfirmIrq( unsigned int uiPort, unsigned int ulConfirmMask )
{
  HW_TYPE(UART_AREA_T)* ptUart = s_aptUart[uiPort];

  ptUart->ulUartiir = ulConfirmMask;
}

/*****************************************************************************/
/*! Set IRQ Mask
* \description
*   Enables/disables IRQs.
* \class
*   UART
* \params
*   uiPort          [in]  UART instance number
*   ulIrqMask       [in]  IRQ mask
* \return
*                                                                            */
/*****************************************************************************/
void UART_SetIrqMsk(unsigned int uiPort, uint32_t ulIrqMask)
{
  uint32_t ulVal;
  ulVal = s_aptUart[uiPort]->ulUartcr & ~(HW_MSK(uartcr_MSIE)
                                         |HW_MSK(uartcr_RIE)
                                         |HW_MSK(uartcr_TIE)
                                         |HW_MSK(uartcr_RTIE));
  s_aptUart[uiPort]->ulUartcr = ulVal | (ulIrqMask << HW_SRT(uartcr_MSIE));
}

/*****************************************************************************/
/*! Put Character
* \description
*   Writes a character into UART FIFO.
*   Blocks until character is buffered for transmission.
* \class
*   UART
* \params
*   uiPort              [in]  UART instance number
*   iChar               [in]  Integer representation of character to be sent
* \return
*                                                                            */
/*****************************************************************************/
int UART_PutCharacter(unsigned int uiPort, int iChar)
{
  HW_TYPE(UART_AREA_T)* ptUart = s_aptUart[uiPort];

  /* Wait until there is space in the FIFO */
  while( (ptUart->ulUartfr & HW_MSK(uartfr_TXFF)) !=0 ) {};
  ptUart->ulUartdr = (uint32_t)iChar;
  return iChar;
}

/*****************************************************************************/
/*! Put Character
* \description
*   Writes a character into UART FIFO if there is enough space (non-blocking).
* \class
*   UART
* \params
*   uiPort              [in]  UART instance number
*   bChar               [in]  Character Content
* \return
*    0 on success
*    -1 else                                                                 */
/*****************************************************************************/
int UART_TryPut(unsigned int uiPort, int iChar)
{
  HW_TYPE(UART_AREA_T)* ptUart = s_aptUart[uiPort];

  /* Check there is space in the FIFO */
  if( (ptUart->ulUartfr & HW_MSK(uartfr_TXFF)) !=0 )
    return EOF;

  ptUart->ulUartdr = (uint32_t)iChar;
  return iChar;
}

/*****************************************************************************/
/*! Get Character
* \description
*   Read a character from UART FIFO.
* \class
*   UART
* \params
*   uiPort              [in]  UART instance number
* \return
*   received character
*   -1 on errors                                                             */
/*****************************************************************************/
int UART_GetCharacter(unsigned int uiPort)
{
  uint32_t ulData;
  HW_TYPE(UART_AREA_T)* ptUart = s_aptUart[uiPort];

  /* We need to wait a defined time for a character */
  while( (ptUart->ulUartfr & HW_MSK(uartfr_RXFE)) != 0 );

  ulData = ptUart->ulUartdr;  /* Get the received byte */

  if( (ulData & 0xFFFFFF00) != 0 )
  {
    /* we had a receive error */
    ptUart->ulUartrsr = 0; /* Clear the error */
    return EOF; /* return error */
  }

  return 0xFF & ulData;
}

/*****************************************************************************/
/*! UART Peek
* \description
*   Returns the inverted RXFE flag (receive FIFO empty).
* \class
*   UART
* \params
*   uiPort              [in]  UART instance number
* \return
*   0
*   1                                                                        */
/*****************************************************************************/
int UART_Peek(unsigned int uiPort)
{
  HW_TYPE(UART_AREA_T)* ptUart = s_aptUart[uiPort];

  int iData = ptUart->ulUartfr & HW_MSK(uartfr_RXFE);

  return ( iData == 0 );
}

/*****************************************************************************/
/*! UART Is busy
* \description
*   Returns true if the UART instance is currently transmitting data.
* \class
*   UART
* \params
*   uiPort              [in]  UART instance number
* \return
*   true: UART is busy
*   false: UART is idle                                                      */
/*****************************************************************************/
bool UART_IsBusy( unsigned int uiPort )
{
  return ( 0 != (s_aptUart[uiPort]->ulUartfr & HW_MSK(uartfr_BUSY)) );
}
