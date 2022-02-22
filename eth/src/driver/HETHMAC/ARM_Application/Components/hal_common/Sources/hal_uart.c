#include "hal_uart.h"                    /* function prototypes and defines */
#include "hw_defines.h"   /* Needed for HW_PTR_UART(s_aptUart) */
#include <stdarg.h>                           /* uprintf */

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
*   uiInst              [in]  UART instance number
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
#ifndef HALDEF
#define HALDEF(name) name
#endif

UART_RESULT_E HALDEF(UART_Init)( unsigned int uInst,
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
*   uiInst              [in]  Number of UART
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(UART_InitSerial)( unsigned int uiInst )
{
  HW_TYPE(UART_AREA_T)* ptUart = s_aptUart[uiInst];
    
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
*   uiInst              [in]  Number of UART
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(UART_CloseSerial)(unsigned int uiInst)
{
  HW_TYPE(UART_AREA_T)* ptUart = s_aptUart[uiInst];
   
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
*   uiInst          [in]  UART instance number
* \return
*   IRQ vector
*                                                                            */
/*****************************************************************************/
uint32_t HALDEF(UART_GetIrq)( unsigned int uiInst )
{
  return s_aptUart[uiInst]->ulUartiir;
}

/*****************************************************************************/
/*! Confirm interrupt(s)
* \description
*   Closes serial port.
* \class 
*   UART 
* \params
*   uiInst              [in]  Number of UART
*   ulConfirmMask         [in]  Mask to confirm interrupt(s)
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(UART_ConfirmIrq) ( unsigned int uiInst, unsigned int ulConfirmMask )
{
  HW_TYPE(UART_AREA_T)* ptUart = s_aptUart[uiInst];
  
  ptUart->ulUartiir = ulConfirmMask;
}

/*****************************************************************************/
/*! Set IRQ Mask
* \description
*   Enables/disables IRQs.
* \class
*   UART
* \params
*   uiInst          [in]  UART instance number
*   ulIrqMask       [in]  IRQ mask
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(UART_SetIrqMsk)(unsigned int uiInst, uint32_t ulIrqMask)
{
  uint32_t ulVal;
  ulVal = s_aptUart[uiInst]->ulUartcr & ~(HW_MSK(uartcr_MSIE)
                                         |HW_MSK(uartcr_RIE)
                                         |HW_MSK(uartcr_TIE)
                                         |HW_MSK(uartcr_RTIE));
  s_aptUart[uiInst]->ulUartcr = ulVal | (ulIrqMask << HW_SRT(uartcr_MSIE));
}

/*****************************************************************************/
/*! Put Character
* \description
*   Writes a character into uart fifo.
* \class 
*   UART 
* \params
*   uiInst              [in]  Number of UART
*   bChar                 [in]  Charcter Content
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(UART_PutCharacter)(unsigned int uiInst, const unsigned char bChar)
{
  HW_TYPE(UART_AREA_T)* ptUart = s_aptUart[uiInst];
          
  /* Wait until there is space in the FIFO */
  while( (ptUart->ulUartfr & HW_MSK(uartfr_TXFF)) !=0 ) {};
  ptUart->ulUartdr = bChar;
}

/*****************************************************************************/
/*! Put Character
* \description
*   Writes a character into UART FIFO if there is enough space.
* \class
*   UART
* \params
*   uiInst              [in]  Number of UART
*   bChar               [in]  Charcter Content
* \return
*    0 on success
*    -1 else                                                                 */
/*****************************************************************************/
int HALDEF(UART_TryPut)(unsigned int uiInst, const unsigned char bChar)
{
  HW_TYPE(UART_AREA_T)* ptUart = s_aptUart[uiInst];

  /* Check there is space in the FIFO */
  if( (ptUart->ulUartfr & HW_MSK(uartfr_TXFF)) !=0 )
    return -1;

  ptUart->ulUartdr = bChar;
  return 0;
}

/*****************************************************************************/
/*! Get Character
* \description
*   Read a character from uart fifo.
* \class 
*   UART 
* \params
*   uiInst              [in]  Number of UART
* \return
*   reveived character
*   -1 on erroneous                                                          */
/*****************************************************************************/
int HALDEF(UART_GetCharacter)(unsigned int uiInst)
{
  uint32_t ulData;
  HW_TYPE(UART_AREA_T)* ptUart = s_aptUart[uiInst];
                 
  /* We need to wait a defined time for a character */
  while( (ptUart->ulUartfr & HW_MSK(uartfr_RXFE)) != 0 );
                 
  ulData = ptUart->ulUartdr;  /* Get the received byte */
  
  if( (ulData & 0xFFFFFF00) != 0 )
  { 
    /* we had a receive error */
    ptUart->ulUartrsr = 0; /* Clear the error */
    return -1; /* return error */
  }  
  else
  {
    return 0xFF & ulData;
  }       
}

/*****************************************************************************/
/*! Get Character without checking for errors
* \description
*   Read a character from uart fifo.
* \class 
*   UART 
* \params
*   uiInst              [in]  Number of UART
* \return
*   reveived character
*   -1 on erroneous                                                          */
/*****************************************************************************/
int HALDEF(UART_GetCharacterRaw)(unsigned int uiInst)
{
  int iData;
  HW_TYPE(UART_AREA_T)* ptUart = s_aptUart[uiInst];
                 
  /* We need to wait a defined time for a character */
  while( (ptUart->ulUartfr & HW_MSK(uartfr_RXFE)) != 0 );
                 
  iData = (int) ptUart->ulUartdr;  /* Get the received byte */
  
  return ( iData );
}

/*****************************************************************************/
/*! UART Peek
* \description
*   Returns the inverted RXFE flag (receive FIFO empty).
* \class 
*   UART 
* \params
*   uiInst              [in]  Number of UART
* \return
*   0
*   1                                                                        */
/*****************************************************************************/
int HALDEF(UART_Peek)(unsigned int uiInst)
{
  HW_TYPE(UART_AREA_T)* ptUart = s_aptUart[uiInst];
  
  int iData = ptUart->ulUartfr & HW_MSK(uartfr_RXFE);
  
  return ( iData == 0 );
}

/*****************************************************************************/
/*! UART Flush
* \description
*   Send pending data
* \class
*   UART
* \params
*   uiInst              [in]  Number of UART                                 */
/*****************************************************************************/
void HALDEF(UART_Flush)( unsigned int uiInst )
{
}

/*****************************************************************************/
/*! UART get receive errors
* \description
*   Returns all receive errros flags
* \class 
*   UART 
* \params
*   uiInst              [in]  Number of UART
* \return
*   uartrsr register                                                         */
/*****************************************************************************/
uint32_t HALDEF(UART_GetRxErrors) ( unsigned int uiInst )
{
  HW_TYPE(UART_AREA_T)* ptUart = s_aptUart[uiInst];
  
  uint32_t ulData = ptUart->ulUartrsr;
  
  return ( ulData );
    
}

/*****************************************************************************/
/*! UART Set control register
* \description
*   Set the control register
* \class 
*   UART 
* \params
*   ulMask              [in]  New value
* \return
*   uartrsr register                                                         */
/*****************************************************************************/
void HALDEF(UART_SetCtrlRegister) ( unsigned int uiInst, uint32_t ulMask )
{
  HW_TYPE(UART_AREA_T)* ptUart = s_aptUart[uiInst];
  
  ptUart->ulUartrts = ulMask;
}

/*****************************************************************************/
/*! UART clear receive errors
* \description
*   Clears all selected receive errros flags
* \class 
*   UART 
* \params
*   uiInst              [in]  Number of UART
*   ulClearMask           [in]  Clear mask
* \return
*   uartrsr register                                                         */
/*****************************************************************************/
void HALDEF(UART_ClearRxErrors) ( unsigned int uiInst, uint32_t ulClearMask )
{
  HW_TYPE(UART_AREA_T)* ptUart = s_aptUart[uiInst];
  
  ptUart->ulUartrsr = ulClearMask;
}

/*****************************************************************************/
/*! UART Flag register
* \description
*   Returns the complete flag register.
* \class 
*   UART 
* \params
*   uiInst              [in]  Number of UART
* \return
*   0
*   1                                                                        */
/*****************************************************************************/
uint32_t HALDEF(UART_FlagRegister) ( unsigned int uiInst )
{
  HW_TYPE(UART_AREA_T)* ptUart = s_aptUart[uiInst];
  
  uint32_t ulData = ptUart->ulUartfr;
  
  return ( ulData );
}

/*****************************************************************************/
/*! UART Is busy
* \description
*   Returns true if the UART instance is currently transmitting data.
* \class
*   UART
* \params
*   uiInst              [in]  UART instance number
* \return
*   true: UART is busy
*   false: UART is idle                                                      */
/*****************************************************************************/
bool HALDEF(UART_IsBusy)( unsigned int uiInst )
{
  return ( 0 != (s_aptUart[uiInst]->ulUartfr & HW_MSK(uartfr_BUSY)) );
}
