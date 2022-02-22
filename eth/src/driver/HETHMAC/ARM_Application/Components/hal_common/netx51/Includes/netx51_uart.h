#ifndef __NETX51_UART_H
#define __NETX51_UART_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

/*****************************************************************************/
/* ! UART Baud rate                                                          */
/*****************************************************************************/
typedef enum NX51_UART_BAUDRATE_Etag {
  NX51_UART_BAUDRATE_300     =     3,
  NX51_UART_BAUDRATE_600     =     6,
  NX51_UART_BAUDRATE_1200    =    12,
  NX51_UART_BAUDRATE_2400    =    24,
  NX51_UART_BAUDRATE_4800    =    48,
  NX51_UART_BAUDRATE_9600    =    96,
  NX51_UART_BAUDRATE_19200   =   192,
  NX51_UART_BAUDRATE_38400   =   384,
  NX51_UART_BAUDRATE_57600   =   576,
  NX51_UART_BAUDRATE_115200  =  1152,
  NX51_UART_BAUDRATE_230400  =  2304,
  NX51_UART_BAUDRATE_460800  =  4608,
  NX51_UART_BAUDRATE_921600  =  9216,
  NX51_UART_BAUDRATE_1843200 = 18432,
  NX51_UART_BAUDRATE_3125000 = 31250,
} NX51_UART_BAUDRATE_E;

#ifndef NX51_UART_BAUDRATE_DEAFAULT
#define NX51_UART_BAUDRATE_DEAFAULT NX51_UART_BAUDRATE_115200
#endif

#define NX51_UART_InitSerial(inst) NX51_UART_Init(inst, NX51_UART_BAUDRATE_DEAFAULT)

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX51_UART_Init( unsigned int uUartNr, NX51_UART_BAUDRATE_E eBaud );
void NX51_UART_CloseSerial( unsigned int uUartNr );
void NX51_UART_PutCharacter( unsigned int uUartNr, const uint8_t bChar );
int  NX51_UART_GetCharacter( unsigned int uUartNr );
int  NX51_UART_Peek( unsigned int uUartNr );


#endif /* #ifndef __NETX51_UART_H */
