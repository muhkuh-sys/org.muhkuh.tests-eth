/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx51_vic.h"
#include "hal_resources_defines_netx51.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_VIC

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! Install ISR Vector
* \description
*   Installs the ISR vector.
* \class
*   VIC
* \params
*   ulVector              [in]  ISR Vector Address
* \return
*                                                                            */
/*****************************************************************************/
void NX51_VIC_InstallIsrVector( NX51_pp_func ulVector )
{
  volatile uint32_t* pulIsrAddress;
  volatile uint32_t* pulArm9_irq_vector;

  /* set the address of the ISR */
  pulIsrAddress = (volatile uint32_t*)(ARM9_Vect_IRQ + 0x1c);
  *pulIsrAddress = (uint32_t) ulVector;

  /* set the jump command in the vector table */
  /*
  ** Jump command to set the PC from the IRQ vector;
  ** this is the assembler instruction:
  **
  ** ldr pc, [pc, #8h] ; 34h / will be placed on address 0x18
  */
  pulArm9_irq_vector = (volatile uint32_t*) ARM9_Vect_IRQ;
  *pulArm9_irq_vector = 0xe59ff014; /* arm926ejs irq vector */

}

/*****************************************************************************/
/*! Install FIQ ISR Vector
* \description
*   Installs the FIQ ISR vector.
* \class
*   VIC
* \params
*   ulVector              [in]  FIQ ISR Address
* \return
*                                                                            */
/*****************************************************************************/
void NX51_VIC_InstallFiqIsrVector( NX51_pp_func ulVector )
{
  volatile uint32_t* pulIsrAddress;
  volatile uint32_t* pulArm9_fiq_vector;

  /* set the address of the ISR */
  pulIsrAddress = (volatile uint32_t*)(ARM9_Vect_FIQ + 0x1c);
  *pulIsrAddress = (uint32_t) ulVector;

  /* set the jump command in the vector table */
  /*
  ** Jump command to set the PC from the IRQ vector;
  ** this is the assembler instruction:
  **
  ** ldr pc, [pc, #20h] ; 38h / will be placed on address 0x1c
  */
  pulArm9_fiq_vector = (volatile uint32_t*) ARM9_Vect_FIQ;
  *pulArm9_fiq_vector = 0xe59ff014; /* arm926ejs IRQ vector */

}

/*****************************************************************************/
/*! Install Undef Vector
* \description
*   Installs the Undefined vector.
* \class
*   VIC
* \params
*   ulVector              [in]  Undef Handler Address
* \return
*                                                                            */
/*****************************************************************************/
void NX51_VIC_InstallExceptionHandler( uint32_t ulVectorAdr, NX51_pp_func pfnHandler )
{
  volatile uint32_t* pulVector  = (volatile uint32_t*)ulVectorAdr;
  volatile uint32_t* pulHandler = (volatile uint32_t*)(ulVectorAdr + 0x1c);

  /* set the address of the exception handler */
  *pulHandler = (uint32_t)pfnHandler;

  /* set the jump command in the vector table */
  /*
  ** Jump command to set the PC from the IRQ vector;
  ** this is the assembler instruction:
  **
  ** ldr pc, [pc, #20h]
  */
  *pulVector = 0xe59ff014;
}

/*****************************************************************************/
/*! IRQ Vector Handler
* \description
*   This function reads the interrupt vector to find out where to jump to.
*   Jump to appropriate function and then a new interrupt is possible.
* \class
*   VIC
* \params
* \return
*                                                                            */
/*****************************************************************************/
void INTERRUPT NX51_VIC_IRQVectorHandler_C( void )
{
  NX51_pp_func fncPtr;

  /* read interrupt vector to find out where to jump to */
  fncPtr = (NX51_pp_func)s_ptVic->ulVic_vect_addr;

  /* jump to appropriate function */
  fncPtr();

  /* end of interrupt to IRQ controller, new interrupt possible */
  s_ptVic->ulVic_vect_addr = 0;
}

/*****************************************************************************/
/*! Clear VIC
* \description
*   Clear all VIC vectors.
* \class
*   VIC
* \params
* \return
*                                                                            */
/*****************************************************************************/
void NX51_VIC_ClearVic( void )
{
  /* no FIQs */
  s_ptVic->ulVic_int_select  = 0;
  s_ptVic->ulVic_int_enclear = 0xffffffff;

  /* clear all vectors */
  s_ptVic->aulVic_vect_addr[0] = 0; s_ptVic->aulVic_vect_cntl[0] = 0;
  s_ptVic->aulVic_vect_addr[1] = 0; s_ptVic->aulVic_vect_cntl[1] = 0;
  s_ptVic->aulVic_vect_addr[2] = 0; s_ptVic->aulVic_vect_cntl[2] = 0;
  s_ptVic->aulVic_vect_addr[3] = 0; s_ptVic->aulVic_vect_cntl[3] = 0;
  s_ptVic->aulVic_vect_addr[4] = 0; s_ptVic->aulVic_vect_cntl[4] = 0;
  s_ptVic->aulVic_vect_addr[5] = 0; s_ptVic->aulVic_vect_cntl[5] = 0;
  s_ptVic->aulVic_vect_addr[6] = 0; s_ptVic->aulVic_vect_cntl[6] = 0;
  s_ptVic->aulVic_vect_addr[7] = 0; s_ptVic->aulVic_vect_cntl[7] = 0;
  s_ptVic->aulVic_vect_addr[8] = 0; s_ptVic->aulVic_vect_cntl[8] = 0;
  s_ptVic->aulVic_vect_addr[9] = 0; s_ptVic->aulVic_vect_cntl[9] = 0;
  s_ptVic->aulVic_vect_addr[10] = 0; s_ptVic->aulVic_vect_cntl[10] = 0;
  s_ptVic->aulVic_vect_addr[11] = 0; s_ptVic->aulVic_vect_cntl[11] = 0;
  s_ptVic->aulVic_vect_addr[12] = 0; s_ptVic->aulVic_vect_cntl[12] = 0;
  s_ptVic->aulVic_vect_addr[13] = 0; s_ptVic->aulVic_vect_cntl[13] = 0;
  s_ptVic->aulVic_vect_addr[14] = 0; s_ptVic->aulVic_vect_cntl[14] = 0;
  s_ptVic->aulVic_vect_addr[15] = 0; s_ptVic->aulVic_vect_cntl[15] = 0;
}

/*****************************************************************************/
/*! VIC Default ISR
* \description
*   Set Default ISR.
* \class
*   VIC
* \params
* \return
*                                                                            */
/*****************************************************************************/
void NX51_VIC_ISR_Default( void )
{
  s_ptVic->ulVic_irq_status;

  /* default handling ... */
}

/*****************************************************************************/
/*! VIC Set Default Vector
* \description
*   This function sets the default vector address.
* \class
*   VIC
* \params
*   default_handler  [in]   Default Vector Address
* \return
*                                                                            */
/*****************************************************************************/
void NX51_VIC_SetDefaultVector( NX51_pp_func default_handler )
{
  s_ptVic->ulVic_def_vect_addr = (uint32_t) default_handler;
}

/*****************************************************************************/
/*! VIC Set IRQ Vector
* \description
*   Set Interrupt vector.
* \class
*   VIC
* \params
*   uVec          [in]   Vector Number
*   ulSrc          [in]   Source
*   irq_handler  [in]   Irq Handler
* \return
*                                                                            */
/*****************************************************************************/
void NX51_VIC_SetIrqVector( unsigned int uVec,
                            uint32_t     ulSrc,
                            NX51_pp_func irq_handler )
{
  /* No action, if vector number out of range */
  if ( uVec<MAX_IRQ_VECTORS ) {
    s_ptVic->aulVic_vect_addr[uVec] = (uint32_t) irq_handler;
    s_ptVic->aulVic_vect_cntl[uVec] = ulSrc;
  }
}

/*****************************************************************************/
/*! Enable interrupt sources
* \description
*   Enable Interrupt source.
* \class
*   VIC
* \params
*   ulSrc          [in]   Source
* \return
*                                                                            */
/*****************************************************************************/
void NX51_VIC_EnableIrqSources( uint32_t ulSrc )
{
  s_ptVic->ulVic_int_enable = ulSrc;
}

/*****************************************************************************/
/*! Disable interrupt sources
* \description
*   Clear Interrupt Enable source.
* \class
*   VIC
* \params
*   ulSrc          [in]   Source
* \return
*                                                                            */
/*****************************************************************************/
void NX51_VIC_DisableIrqSources( uint32_t ulSrc )
{
  s_ptVic->ulVic_int_enclear = ulSrc;
}


/*****************************************************************************/
/*! Get raw interrupt
* \description
*  Read the RAW interrupt register.
* \class
*   VIC
* \params
* \return
*   ulVic_int_raw_status value                                               */
/*****************************************************************************/
uint32_t NX51_VIC_GetIrqRaw( void )
{
  return ( s_ptVic->ulVic_raw_intr );
}

/*****************************************************************************/
/*! Get irq status
* \description
*  Read the IRQ status register.
* \class
*   VIC
* \params
* \return
*   ulVic_int_irq_status value                                               */
/*****************************************************************************/
uint32_t NX51_VIC_GetIrqStatus( void )
{
  return ( s_ptVic->ulVic_irq_status );
}

/*****************************************************************************/
/*! Get fiq status
* \description
*  Read the FIQ status register.
* \class
*   VIC
* \params
* \return
*   ulVic_int_fiq_status value                                               */
/*****************************************************************************/
uint32_t NX51_VIC_GetFiqStatus( void )
{
  return ( s_ptVic->ulVic_fiq_status );
}

/*****************************************************************************/
/*! Set SW interrupt
* \description
*   Set software Interrupt.
* \class
*   VIC
* \params
*   ulSrc          [in]   Source
* \return
*                                                                            */
/*****************************************************************************/
void NX51_VIC_SetSoftInterrupt( uint32_t ulSrc )
{
  s_ptVic->ulVic_softint = ulSrc;
}

/*****************************************************************************/
/*! Clear SW interrupt
* \description
*   Clear software Interrupt.
* \class
*   VIC
* \params
*   ulSrc          [in]   Source
* \return
*                                                                            */
/*****************************************************************************/
void NX51_VIC_ClrSoftInterrupt( uint32_t ulSrc )
{
  s_ptVic->ulVic_softint_clear = ulSrc;
}

/*****************************************************************************/
/*! Set Protection
* \description
*   Enables/disables protected register access.
* \class
*   VIC
* \params
*   ulVal          [in]   1/0: Protection enabled/disabled
* \return
*                                                                            */
/*****************************************************************************/
void NX51_VIC_SetProtection( uint32_t ulVal )
{
  s_ptVic->ulVic_protection = ulVal;
}
