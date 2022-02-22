/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx51_uart.h" /* function prototypes and defines */
#include "hal_resources_defines_netx51.h"
#include <stdarg.h>      /* uprintf */

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_UART

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
/* DEV_BAUDRATE is 100 times to small -> multiply with 100 (or divide by DEV_FREQUENCY/100) */
#define NX51_DEV_BAUDRATE_DIV_LO(baud) (((baud*16*65536)/(NX51_DEV_FREQUENCY/100)) & 0xff)
#define NX51_DEV_BAUDRATE_DIV_HI(baud) (((baud*16*65536)/(NX51_DEV_FREQUENCY/100))>>8)

/*****************************************************************************/
/*! Initialize serial Port
* \description
*   Initialize serial port.
* \class
*   UART
* \params
*   uUartNr               [in]  Number of UART
*   eBaud                 [in]  Baud rate
* \return
*                                                                            */
/*****************************************************************************/
void NX51_UART_Init( unsigned int         uUartNr,
                     NX51_UART_BAUDRATE_E eBaud )
{
  NX51_UART_AREA_T* ptUart = s_aptUart[uUartNr];

  /* First of all disable everything */
  ptUart->ulUartcr     = 0;

  /* Set the bit for the second baud rate mode */
  ptUart->ulUartcr_2   = MSK_NX51_uartcr_2_Baud_Rate_Mode;

  /* Adjust the baud rate register */
  ptUart->ulUartlcr_l  = NX51_DEV_BAUDRATE_DIV_LO((uint32_t)eBaud);
  ptUart->ulUartlcr_m  = NX51_DEV_BAUDRATE_DIV_HI((uint32_t)eBaud);

  /* set UART to 8 bits, 1 stop bit, no parity, FIFO enabled */
  ptUart->ulUartlcr_h  = (MSK_NX51_uartlcr_h_WLEN | MSK_NX51_uartlcr_h_FEN);
  /* Set TX-Driver to enabled */
  ptUart->ulUartdrvout = MSK_NX51_uartdrvout_DRVTX;
  /* Finally enable the UART */
  ptUart->ulUartcr     = MSK_NX51_uartcr_uartEN;
}

/*****************************************************************************/
/*! Close Serial Port
* \description
*   Closes serial port.
* \class
*   UART
* \params
*   uUartNr              [in]  Number of UART
* \return
*                                                                            */
/*****************************************************************************/
void NX51_UART_CloseSerial( unsigned int uUartNr )
{
  NX51_UART_AREA_T* ptUart = s_aptUart[uUartNr];
  unsigned int uCnt;

  while((ptUart->ulUartfr & MSK_NX51_uartfr_BUSY)!=0); /* UART busy ? */
  ptUart->ulUartcr     = 0; /* First of all disable everything */
  ptUart->ulUartlcr_m  = 0; /* Adjust the baud rate register */
  ptUart->ulUartlcr_l  = 0;
  ptUart->ulUartlcr_h  = 0; /* UART to be 8 bits, 1 stop bit, no parity, FIFO enabled */
  ptUart->ulUartdrvout = 0; /* Set TX-Driver to disabled */
  for(uCnt=0; uCnt < 5; uCnt++)
  {
    ptUart->ulUartdr; /* Read out the reception register if there are still bytes in it */
  }
}

/*****************************************************************************/
/*! Put Character
* \description
*   Writes a character into UART FIFO.
* \class
*   UART
* \params
*   uUartNr               [in]  Number of UART
*   bChar                 [in]  Character Content
* \return
*                                                                            */
/*****************************************************************************/
void NX51_UART_PutCharacter( unsigned int  uUartNr,
                             const uint8_t bChar )
{
  NX51_UART_AREA_T* ptUart = s_aptUart[uUartNr];

  /* Wait until there is space in the FIFO */
  while( (ptUart->ulUartfr & MSK_NX51_uartfr_TXFF) !=0 ) {};
  ptUart->ulUartdr = bChar;
}

/*****************************************************************************/
/*! Get Character
* \description
*   Read a character from UART FIFO.
* \class
*   UART
* \params
*   uUartNr              [in]  Number of UART
* \return
*   Received character
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX51_UART_GetCharacter( unsigned int uUartNr )
{
  uint32_t ulDat;
  NX51_UART_AREA_T* ptUart = s_aptUart[uUartNr];

  /* We need to wait a defined time for a character */
  while( (ptUart->ulUartfr & MSK_NX51_uartfr_RXFE) != 0 ); /* Wait infinite for new data in the FIFO */

  ulDat = ptUart->ulUartdr;  /* Get the received byte */
  if( (ulDat & 0xFFFFFF00) != 0 ) { /* we had a receive error */
    ptUart->ulUartrsr = 0; /* Clear the error */
    return -1; /* return error */
  }  else {
    return (int)ulDat;
  }
}

/*****************************************************************************/
/*! UART Peek
* \description
*   -
* \class
*   UART
* \params
*   uUartNr              [in]  Number of UART
* \return
*   0
*   1                                                                        */
/*****************************************************************************/
int NX51_UART_Peek( unsigned int uUartNr )
{
  NX51_UART_AREA_T* ptUart = s_aptUart[uUartNr];

  return ( (ptUart->ulUartfr & MSK_NX51_uartfr_RXFE) == 0 );
}
