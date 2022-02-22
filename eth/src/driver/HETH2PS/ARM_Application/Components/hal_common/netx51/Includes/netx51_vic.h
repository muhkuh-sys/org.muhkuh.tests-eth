#ifndef __NETX51_VIC_H
#define __NETX51_VIC_H

#include "compiler_spec.h"
#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

typedef void (*NX51_pp_func)(void);

#define MAX_IRQ_VECTORS             16
#define ARM9_Vect_Undefined         0x00000004
#define ARM9_Vect_SWI               0x00000008
#define ARM9_Vect_Prefetch          0x0000000c
#define ARM9_Vect_Abort             0x00000010
#define ARM9_Vect_IRQ               0x00000018
#define ARM9_Vect_FIQ               0x0000001c


/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

void           NX51_VIC_InstallIsrVector( NX51_pp_func ulVector);
void           NX51_VIC_InstallFiqIsrVector( NX51_pp_func ulVector );
void           NX51_VIC_InstallExceptionHandler( uint32_t ulVectAdr, NX51_pp_func pfnHandler );
void           NX51_VIC_ClearVic( void );
void INTERRUPT NX51_VIC_IRQVectorHandler_C( void );
void           NX51_VIC_ISR_Default( void );
void           NX51_VIC_SetDefaultVector( NX51_pp_func default_handler );
void           NX51_VIC_SetIrqVector( unsigned int uVec, uint32_t ulSrc, NX51_pp_func irq_handler );
void           NX51_VIC_EnableIrqSources( uint32_t ulSrc );
void           NX51_VIC_DisableIrqSources( uint32_t ulSrc );
uint32_t       NX51_VIC_GetIrqRaw( void );
uint32_t       NX51_VIC_GetIrqStatus( void );
uint32_t       NX51_VIC_GetFiqStatus( void );
void           NX51_VIC_SetSoftInterrupt( uint32_t ulSrc );
void           NX51_VIC_ClrSoftInterrupt( uint32_t ulSrc );
void           NX51_VIC_SetProtection( uint32_t ulVal );

#endif /* #ifndef __NETX51_VIC_H */
