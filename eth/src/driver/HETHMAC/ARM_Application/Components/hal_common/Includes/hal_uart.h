#ifndef __HAL_UART_H
#define __HAL_UART_H

#include <stdint.h>
#include <stdbool.h>

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

typedef unsigned long UART_BAUDRATE;
#define UART_BAUDRATE_300      0x0003
#define UART_BAUDRATE_600      0x0006
#define UART_BAUDRATE_1200     0x000c
#define UART_BAUDRATE_2400     0x0018
#define UART_BAUDRATE_4800     0x0030
#define UART_BAUDRATE_9600     0x0060
#define UART_BAUDRATE_19200    0x00c0
#define UART_BAUDRATE_38400    0x0180
#define UART_BAUDRATE_57600    0x0240
#define UART_BAUDRATE_115200   0x0480
#define UART_BAUDRATE_230400   0x0900
#define UART_BAUDRATE_460800   0x1200
#define UART_BAUDRATE_921600   0x2400
#define UART_BAUDRATE_1843200  0x4800
#define UART_BAUDRATE_3125000  0x7a12

#define DEV_BAUDRATE UART_BAUDRATE_115200

// DEV_BAUDRATE is 100 times to small -> multiply with 100 (or divide by DEV_FREQUENCY/100)
#define DEV_BAUDRATE_DIV_LO (((DEV_BAUDRATE*16*65536)/(HW_DEV_FREQUENCY/100))&0xff)*100
#define DEV_BAUDRATE_DIV_HI (((DEV_BAUDRATE*16*65536)/(HW_DEV_FREQUENCY/100))>>8)*100

#define UART_PARITY_NONE  0
#define UART_PARITY_ODD   1
#define UART_PARITY_EVEN  2
#define UART_PARITY_MARK  3 // always 1
#define UART_PARITY_SPACE 4 // always 0

/*****************************************************************************/
/* Result codes for functions                                                */
/*****************************************************************************/

typedef enum UART_RESULT_Etag {
  UART_OKAY               = 0,    /**< Successful */
  UART_ERR_INVAL_PARAM    = 1     /**< Invalid parameter */
} UART_RESULT_E;

/*****************************************************************************/
/* ! UART register                                                          */
/*****************************************************************************/
typedef enum UART_PRM_Etag {
  UART_DR          = 0,
  UART_RSR         = 1,
  UART_LCR_H       = 2,
  UART_LCR_M       = 3,
  UART_LCR_L       = 4,
  UART_CR          = 5,
  UART_FR          = 6,
  UART_IIR         = 7,
  UART_ILPR        = 8,
  UART_RTS         = 9,
  UART_FORERUN     = 19,
  UART_TRAIL       = 11,
  UART_DRVOUT      = 12,
  UART_CR_2        = 13,
  UART_RXIFLSEL    = 14,
  UART_TXIFLSEL    = 15
} UART_PRM_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

#ifndef HALDEC
#define HALDEC(name) name
#endif

UART_RESULT_E         HALDEC(UART_Init)               ( unsigned int uiInst,
                                                        uint32_t     ulBaudrate,
                                                        unsigned int uDataBits,
                                                        unsigned int uParity,
                                                        unsigned int uStopBits );
void                  HALDEC(UART_InitSerial)         ( unsigned int uiInst );
void                  HALDEC(UART_CloseSerial)        ( unsigned int uiInst );
uint32_t              HALDEC(UART_GetIrq)             ( unsigned int uiInst );
void                  HALDEC(UART_ConfirmIrq)         ( unsigned int uiInst, unsigned int ulConfirmMask );
void                  HALDEC(UART_SetIrqMsk)          ( unsigned int uiInst, uint32_t ulIrqMask );
void                  HALDEC(UART_PutCharacter)       ( unsigned int uiInst, const unsigned char bChar );
int                   HALDEC(UART_TryPut)             ( unsigned int uiInst, const unsigned char bChar );
int                   HALDEC(UART_GetCharacter)       ( unsigned int uiInst );
int                   HALDEC(UART_GetCharacterRaw)    ( unsigned int uiInst );
int                   HALDEC(UART_Peek)               ( unsigned int uiInst );
void                  HALDEC(UART_Flush)              ( unsigned int uiInst );
uint32_t              HALDEC(UART_GetRxErrors)        ( unsigned int uiInst );
void                  HALDEC(UART_ClearRxErrors)      ( unsigned int uiInst, uint32_t ulClearMask );
void                  HALDEC(UART_SetCtrlRegister)    ( unsigned int uiInst, uint32_t ulMask );
uint32_t              HALDEC(UART_FlagRegister)       ( unsigned int uiInst );
bool                  HALDEC(UART_IsBusy)             ( unsigned int uiInst );

#endif /* #ifndef __HAL_UART_H */
