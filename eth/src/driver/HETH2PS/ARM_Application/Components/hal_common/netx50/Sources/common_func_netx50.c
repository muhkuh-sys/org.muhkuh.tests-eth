#include <stdarg.h> // uprintf
#include "regdef_netx50.h" // structures
#include "common_func_netx50.h" // function prototypes and defines

#define MSK_NX50_DPM_ARM_IO_MODE0_PIO_MODE    0xffffffffU
#define SRT_NX50_DPM_ARM_IO_MODE0_PIO_MODE    0

#define MSK_NX50_DPM_ARM_IO_MODE1_PIO_MODE    0x001fffffU
#define SRT_NX50_DPM_ARM_IO_MODE1_PIO_MODE    0
#define MSK_NX50_DPM_ARM_IO_MODE1_IN_CONTROL  0xc0000000U
#define SRT_NX50_DPM_ARM_IO_MODE1_IN_CONTROL  30

static NX50_ASIC_CTRL_AREA_T*          s_ptAsicCtrl = (NX50_ASIC_CTRL_AREA_T*) Addr_NX50_asic_ctrl;
static NX50_PIO_AREA_T*                s_ptPio =      (NX50_PIO_AREA_T*) Addr_NX50_pio;
static NX50_GPIO_AREA_T*               s_ptGpio =     (NX50_GPIO_AREA_T*) Addr_NX50_gpio;
static NX50_POINTER_FIFO_AREA_T*       s_ptPFifo =    (NX50_POINTER_FIFO_AREA_T*) Addr_NX50_pointer_fifo;
static NX50_VIC_AREA_T*                s_ptVic =      (NX50_VIC_AREA_T*) Addr_NX50_vic;
static NX50_XPEC_IRQ_REGISTERS_AREA_T* s_ptXpecIrq =  (NX50_XPEC_IRQ_REGISTERS_AREA_T*) Addr_NX50_xpec_irq_registers;
static NX50_MMIO_CTRL_AREA_T*          s_ptMmioCtrl = (NX50_MMIO_CTRL_AREA_T*) Addr_NX50_mmio_ctrl;

static NX50_NETX_CONTROLLED_GLOBAL_REGISTER_BLOCK_1_AREA_T * const s_ptNetXGlobalRegBlock1Area = (NX50_NETX_CONTROLLED_GLOBAL_REGISTER_BLOCK_1_AREA_T*) Addr_NX50_netx_controlled_global_register_block_1;
static NX50_NETX_CONTROLLED_GLOBAL_REGISTER_BLOCK_2_AREA_T * const s_ptNetXGlobalRegBlock2Area = (NX50_NETX_CONTROLLED_GLOBAL_REGISTER_BLOCK_2_AREA_T*) Addr_NX50_netx_controlled_global_register_block_2;

/*
  _   _      _      ____    _____
 | | | |    / \    |  _ \  |_   _|
 | | | |   / _ \   | |_) |   | |
 | |_| |  / ___ \  |  _ <    | |
  \___/  /_/   \_\ |_| \_\   |_|

*/

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
const unsigned int aUartBaseAdr[3] = {
  Addr_NX50_uart0,
  Addr_NX50_uart1,
  Addr_NX50_uart2
};

/*****************************************************************************/
/*! Init serial Port
* \description
*   Initialize serial port.
* \class
*   UART
* \params
*   uiPort              [in]  Number of UART
* \return
*                                                                            */
/*****************************************************************************/
void NX50_UART_InitSerial( unsigned int uiPort )
{
  unsigned int ulUartBaseAdr;

  // get uart base adr
  ulUartBaseAdr = aUartBaseAdr[uiPort];

  // switch gpio 0 - 3 mux to uart
  POKE( Adr_NX50_gpio_cfg0+(uiPort<<4), NX50_GPIO_CFG_EXT_0 );
  POKE( Adr_NX50_gpio_cfg1+(uiPort<<4), NX50_GPIO_CFG_EXT_0 );
  POKE( Adr_NX50_gpio_cfg2+(uiPort<<4), NX50_GPIO_CFG_EXT_0 );
  POKE( Adr_NX50_gpio_cfg3+(uiPort<<4), NX50_GPIO_CFG_EXT_0 );

  // First of all disable everything
  POKE(ulUartBaseAdr+REL_Adr_NX50_uartcr, 0);

  // Set the bit for the second baudrate mode
  POKE(ulUartBaseAdr+REL_Adr_NX50_uartcr_2, MSK_NX50_uartcr_2_Baud_Rate_Mode);

  // Adjust the baudrate register
  POKE(ulUartBaseAdr+REL_Adr_NX50_uartlcr_l, NX50_DEV_BAUDRATE_DIV_LO);
  POKE(ulUartBaseAdr+REL_Adr_NX50_uartlcr_m, NX50_DEV_BAUDRATE_DIV_HI);

  // set UART to 8 bits, 1 stop bit, no parity, fifo enabled
  POKE( ulUartBaseAdr+REL_Adr_NX50_uartlcr_h, (MSK_NX50_uartlcr_h_WLEN | MSK_NX50_uartlcr_h_FEN));
  // Set TX-Driver to enabled
  POKE( ulUartBaseAdr+REL_Adr_NX50_uartdrvout, MSK_NX50_uartdrvout_DRVTX);
  // Finally enable the UART
  POKE( ulUartBaseAdr+REL_Adr_NX50_uartcr, MSK_NX50_uartcr_uartEN);
}

/*****************************************************************************/
/*! Close Serial Port
* \description
*   Closes serial port.
* \class
*   UART
* \params
*   uiPort              [in]  Number of UART
* \return
*                                                                            */
/*****************************************************************************/
void NX50_UART_CloseSerial(unsigned int uiPort)
{
   unsigned int ulUartBaseAdr;
   unsigned int uCnt;

   // get uart base adr
   ulUartBaseAdr = aUartBaseAdr[uiPort];

   while((PEEK(ulUartBaseAdr+REL_Adr_NX50_uartfr) & MSK_NX50_uartfr_BUSY)!=0); /* UART busy ? */
   POKE(ulUartBaseAdr+REL_Adr_NX50_uartcr,0);   /* First of all disable everything */
   POKE(ulUartBaseAdr+REL_Adr_NX50_uartlcr_m,0); /* Adjust the baudrate register */
   POKE(ulUartBaseAdr+REL_Adr_NX50_uartlcr_l,0);
   POKE(ulUartBaseAdr+REL_Adr_NX50_uartlcr_h,0);  /* UART to be 8 bits, 1 stop bit, no parity, fifo enabled */
   POKE(ulUartBaseAdr+REL_Adr_NX50_uartdrvout,0); /* Set TX-Driver to disabled */
   for(uCnt=0;uCnt<5;uCnt++) {
     PEEK(ulUartBaseAdr+REL_Adr_NX50_uartdr); /* Read out the reception register if there are still bytes in it */
   }
   // reset gpio mux to input
   POKE(Adr_NX50_gpio_cfg0+(uiPort<<4), NX50_GPIO_CFG_IN);
   POKE(Adr_NX50_gpio_cfg1+(uiPort<<4), NX50_GPIO_CFG_IN);
   POKE(Adr_NX50_gpio_cfg2+(uiPort<<4), NX50_GPIO_CFG_IN);
   POKE(Adr_NX50_gpio_cfg3+(uiPort<<4), NX50_GPIO_CFG_IN);
}

/*****************************************************************************/
/*! Put Character
* \description
*   Writes a character into UART FIFO.
* \class
*   UART
* \params
*   uiPort              [in]  Number of UART
*   iChar               [in]  Character Content
* \return
*                                                                            */
/*****************************************************************************/
int NX50_UART_PutCharacter(unsigned int uiPort, int iChar)
{
  unsigned int ulUartBaseAdr;

  // get uart base adr
  ulUartBaseAdr = aUartBaseAdr[uiPort];

  // Wait until there is space in the FIFO
  while( (PEEK(ulUartBaseAdr+REL_Adr_NX50_uartfr)&MSK_NX50_uartfr_TXFF)!=0 ) {};
  POKE( ulUartBaseAdr+REL_Adr_NX50_uartdr, iChar);

  return iChar;
}

/*****************************************************************************/
/*! Get Character
* \description
*   Read a character from UART FIFO.
* \class
*   UART
* \params
*   uiPort              [in]  Number of UART
* \return
*   received character
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX50_UART_GetCharacter(unsigned int uiPort)
{
  unsigned int uDat;
  unsigned int ulUartBaseAdr;


  // get uart base adr
  ulUartBaseAdr = aUartBaseAdr[uiPort];

  // We need to wait a defined time for a character
  while((PEEK(ulUartBaseAdr+REL_Adr_NX50_uartfr)&MSK_NX50_uartfr_RXFE)!=0); // Wait infinte for new data in the FIFO

  uDat = PEEK(ulUartBaseAdr+REL_Adr_NX50_uartdr);  // Get the received byte
  if( (uDat&0xFFFFFF00)!=0 ) { // we had a receive error
    POKE(ulUartBaseAdr+REL_Adr_NX50_uartrsr, 0); // Clear the error
    return -1; // return error
  }  else {
    return (int)uDat;
  }
}

/*****************************************************************************/
/*! Uart Peek
* \description
*   -
* \class
*   UART
* \params
*   uiPort              [in]  Number of UART
* \return
*   0
*   1                                                                        */
/*****************************************************************************/
int NX50_UART_Peek(unsigned int uiPort)
{
  unsigned int ulUartBaseAdr;

  // get uart base adr
  ulUartBaseAdr = aUartBaseAdr[uiPort];
  return ( (PEEK(ulUartBaseAdr+REL_Adr_NX50_uartfr)&MSK_NX50_uartfr_RXFE)==0 );
}


/*
 __     __  ___    ____
 \ \   / / |_ _|  / ___|
  \ \ / /   | |  | |
   \ V /    | |  | |___
    \_/    |___|  \____|

*/

/*****************************************************************************/
/*! Install Isr Vector
* \description
*   Installs the ISR vector.
* \class
*   VIC
* \params
*   ulVector              [in]  Isr Vector Address
* \return
*                                                                            */
/*****************************************************************************/
void NX50_VIC_InstallIsrVector( pp_func ulVector )
{
  volatile unsigned long* pulIsrAddress;
  volatile unsigned long* pulArm9_irq_vector;

  /* set the address of the isr */
  pulIsrAddress = (volatile unsigned long*) 0x00000020;
  *pulIsrAddress = (unsigned long) ulVector;

  /* set the jump command in the vector table */
  /*
  ** Jump command to set the pc from the irq vector;
  ** this is the assembler instruction:
  **
  ** ldr pc, [pc, #8h] ; 20h / will be placed on address 0x18
  */
  pulArm9_irq_vector = (volatile unsigned long*) ARM9_Vect_IRQ;
  *pulArm9_irq_vector = 0xe59ff000; /* arm926ejs irq vector */

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
void INTERRUPT NX50_VIC_IRQVectorHandler_C( void )
{
  pp_func fncPtr;

  /* read interrupt vector to find out where to jump to */
  fncPtr = (pp_func)s_ptVic->ulVic_vect_addr;

  /* jump to appropiate function */
  fncPtr();

  /* end of interrupt to irq controller, new interrupt possible */
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
void NX50_VIC_ClearVic( void )
{
  /* no firqs */
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
/*! Default ISR
* \description
*   Set Default ISR.
* \class
*   VIC
* \params
* \return
*                                                                            */
/*****************************************************************************/
void NX50_VIC_ISR_Default( void )
{
  s_ptVic->ulVic_irq_status;

  // default handling ...
}

/*****************************************************************************/
/*! Set Default Vector
* \description
*   This function sets the default vector address.
* \class
*   VIC
* \params
*   default_handler  [in]   Default Vector Address
* \return
*                                                                            */
/*****************************************************************************/
void NX50_VIC_SetDefaultVector( pp_func default_handler )
{
  s_ptVic->ulVic_def_vect_addr = (unsigned long) default_handler;
}

/*****************************************************************************/
/*! Set IRQ Vector
* \description
*   Set Interrupt vector.
* \class
*   VIC
* \params
*   vec          [in]   Vector Number
*   src          [in]   Source
*   irq_handler  [in]   Irq Handler
* \return
*                                                                            */
/*****************************************************************************/
void NX50_VIC_SetIrqVector( unsigned int vec, unsigned long src, pp_func irq_handler )
{
  /* No action, if vector number out of range */
  if ( vec < MAX_IRQ_VECTORS )
  {
    s_ptVic->aulVic_vect_addr[vec]  = (unsigned long)irq_handler;
    s_ptVic->aulVic_vect_cntl[vec]  = src;
  }
}

/*****************************************************************************/
/*! Enable IRQ SRC
* \description
*   Enable Interrupt source.
* \class
*   VIC
* \params
*   src          [in]   Source
* \return
*                                                                            */
/*****************************************************************************/
void NX50_VIC_EnableIrqSources( unsigned long src )
{
  s_ptVic->ulVic_int_enable = src;
}


/*
   ____   ____    ___    ___
  / ___| |  _ \  |_ _|  / _ \
 | |  _  | |_) |  | |  | | | |
 | |_| | |  __/   | |  | |_| |
  \____| |_|     |___|  \___/

*/

/*****************************************************************************/
/*! GPIO Setup Mode
* \description
*   Configure mode for the selected GPIO.
* \class
*   GPIO
* \params
*   ulGpioNum          [in]   Selected GPIO
*   ulMode             [in]   GPIO_MODE_OUTPUT      = Set the selected GPIO to Output mode
*                             GPIO_MODE_INPUT_READ  = Set the selected GPIO to Input mode
*   ulInvert           [in]   GPIO_NOINVERT         = Does not invert the selected GPIO
*                             GPIO_INVERT           = Inverts the selected GPIO
* \return
*                                                                            */
/*****************************************************************************/
void NX50_GPIO_SetupMode( unsigned long ulGpioNum, unsigned long ulMode, unsigned long ulInvert )
{
  s_ptGpio->aulGpio_cfg[ulGpioNum] = (unsigned long) (ulMode | ulInvert);
}

/*****************************************************************************/
/*! GPIO Set Output
* \description
*   Enables/disable output for the selected GPIO.
* \class
*   GPIO
* \params
*   ulGpioNum          [in]   Selected GPIO
*   fEnable            [in]   0 = Disable the Output
*                             1 = Enable the Output
* \return
*                                                                            */
/*****************************************************************************/
void NX50_GPIO_SetOutput( unsigned long ulGpioNum, int fEnable )
{
  if( fEnable )
    s_ptGpio->ulGpio_line |= 1U << ulGpioNum;
  else
    s_ptGpio->ulGpio_line &= ~(1U << ulGpioNum);
}

/*****************************************************************************/
/*! GPIO Get Line
* \description
*  Read the GPIO line register.
* \class
*   GPIO
* \params
* \return
*   Gpio_line value                                                          */
/*****************************************************************************/
unsigned long NX50_GPIO_GetLine( void )
{
  return ( s_ptGpio->ulGpio_line );
}

/*****************************************************************************/
/*! GPIO Set Line
* \description
*   Set GPIO line.
* \class
*   GPIO
* \params
*   ulVal          [in]   Value of GPIO Line
* \return
*                                                                            */
/*****************************************************************************/
void NX50_GPIO_SetLine( unsigned long ulVal )
{
  s_ptGpio->ulGpio_line = ulVal;
}

/*****************************************************************************/
/*! GPIO Get Input
* \description
*   Read selected GPIO.
* \class
*   GPIO
* \params
*   ulGpioNum          [in]   Selected GPIO
* \return
*                                                                            */
/*****************************************************************************/
int NX50_GPIO_GetInput( unsigned long ulGpioNum )
{
  return ( s_ptGpio->ulGpio_in & (1<<ulGpioNum) ) ? 1 : 0;
}

/*****************************************************************************/
/*! GPIO Get In Register
* \description
*  Read the GPIO IN register.
* \class
*   GPIO
* \params
* \return
*   Gpio_in value                                                            */
/*****************************************************************************/
unsigned long NX50_GPIO_GetIn( void )
{
  return ( s_ptGpio->ulGpio_in );
}

/*****************************************************************************/
/*! GPIO Irq Enable
* \description
*   Enable interrupt request for the selected GPIO.
* \class
*   GPIO
* \params
*   ulGpioNum          [in]   Selected GPIO
* \return
*                                                                            */
/*****************************************************************************/
void NX50_GPIO_IrqEnable( unsigned long ulGpioNum )
{
  s_ptGpio->ulGpio_irq_mask_set |= 1 << ulGpioNum;
}

/*****************************************************************************/
/*! GPIO Get Irq
* \description
*   Read interrupt status of the selected GPIO.
* \class
*   GPIO
* \params
*   ulGpioNum          [in]   Selected GPIO
* \return
*  0
*  1                                                                         */
/*****************************************************************************/
int NX50_GPIO_GetIrq( unsigned long ulGpioNum )
{
  return (s_ptGpio->ulGpio_irq_masked & (1 << ulGpioNum)) ? 1 : 0;
}

/*****************************************************************************/
/*! GPIO Irq Reset
* \description
*   Replaces the write to Adr_NX50_gpio_cnt_irq_raw and Adr_NX50_gpio_gpio_irq_raw
*   for interrupt confirmation. \n
*   This must be called by everyone who wants to confirm an interrupt generated by the GPIO module.
* \class
*   GPIO
* \params
*   ulConfirm  [in] Adr_NX50_gpio_cnt_irq_raw / Adr_NX50_gpio_gpio_irq_raw depending on the IRQ to confirm
*   ulMask     [in] Interrupt mask to confirm
* \return
*                                                                            */
/*****************************************************************************/
void NX50_GPIO_ConfirmIrq( unsigned long ulConfirm, unsigned long ulMask )
{
  IRQSTATE      irq;
  unsigned int  uiIdx;

  /* Make these variables static so they can be placed in fast memory by linker.
     Sharing / Race conditions are no problem as we are only using them inside
     IRQ locked code */
  static unsigned long aulTimerValues[5] = {0};
  static unsigned long aulTimerCtrl[5]   = {0};
  static unsigned long ulSystimeNs;
  unsigned long        ulSoftIntMask     = 0;
  unsigned long        ulTimerIrqMask    = 0;

  irq = IRQ_LockSave();

  if( Adr_NX50_gpio_cnt_irq_raw == ulConfirm )
  {
    /* On Timer confirmation we need to clear software interrupts on VIC */
    unsigned long ulSoftIntConfirm = (((ulMask & MSK_GPIO_COUNTER0_2) >> SRT_NX50_cnt_irq_raw_cnt0) << SRT_NX50_vic_softint_timer0) |
                                     (((ulMask & MSK_GPIO_COUNTER3_4) >> SRT_NX50_cnt_irq_raw_cnt3) << SRT_NX50_vic_softint_timer3) |
                                     (((ulMask & MSK_NX50_cnt_irq_raw_sys_time) >> SRT_NX50_cnt_irq_raw_sys_time) << SRT_NX50_vic_softint_systime_ns);

    POKE( Adr_NX50_vic_vic_softint_clear, ulSoftIntConfirm );
  }

  /* Store all current timers */
  for( uiIdx = 0; uiIdx < sizeof(s_ptGpio->aulGpio_counter_cnt) / sizeof(s_ptGpio->aulGpio_counter_cnt[0]); ++uiIdx)
  {
    aulTimerValues[uiIdx] = s_ptGpio->aulGpio_counter_cnt[uiIdx];
    aulTimerCtrl[uiIdx]   = s_ptGpio->aulGpio_counter_ctrl[uiIdx];
  }

  /* Save Systime value */
  PEEK(Adr_NX50_systime_systime_s);
  ulSystimeNs = PEEK(Adr_NX50_systime_systime_ns);

  POKE( ulConfirm, ulMask );

  ulTimerIrqMask = s_ptGpio->ulCnt_irq_mask_set;

  /* Verify system against gpio */
  if( ulTimerIrqMask  & MSK_NX50_cnt_irq_masked_sys_time )
  {
    unsigned long ulNewSystimeNs;
    unsigned long ulSystimeCmp   = s_ptGpio->ulGpio_systime_cmp;

    PEEK(Adr_NX50_systime_systime_s);
    ulNewSystimeNs = PEEK(Adr_NX50_systime_systime_ns);

    if( ulNewSystimeNs > ulSystimeNs )
    {
      if( (ulSystimeNs < ulSystimeCmp) && (ulNewSystimeNs > ulSystimeCmp) )
        ulSoftIntMask |= MSK_NX50_vic_softint_systime_ns;
    }
    else
    {
      if( (ulSystimeNs < ulSystimeCmp) || (ulNewSystimeNs > ulSystimeCmp) )
        ulSoftIntMask |= MSK_NX50_vic_softint_systime_ns;
    }
  }

  /* Check for timers that may have caused an IRQ during our IRQ acknowledge */
  for( uiIdx = 0; uiIdx < sizeof(s_ptGpio->aulGpio_counter_cnt) / sizeof(s_ptGpio->aulGpio_counter_cnt[0]); ++uiIdx )
  {
    /* Timer0-2 are continues from Bit1-3, 3-4 are on Bits 29/30 */
    unsigned long ulTempIrqMsk = (uiIdx < 3) ?
                                 MSK_NX50_vic_softint_timer0 << uiIdx :
                                 MSK_NX50_vic_softint_timer3 << (uiIdx - 3);

    /* Only process timers that are supposed to generate an IRQ and were running before */
    if( (ulTimerIrqMask & (1 << uiIdx))                            &&
        (aulTimerCtrl[uiIdx] & MSK_NX50_gpio_counter0_ctrl_irq_en) &&
        (aulTimerCtrl[uiIdx] & MSK_NX50_gpio_counter0_ctrl_run) )
    {
      if( 0 == (aulTimerCtrl[uiIdx] & MSK_NX50_gpio_counter0_ctrl_once) )
      {
        /* This is a cyclic timer, so we just need to check if the new value is
           smaller than the old one and generate an IRQ if neccessary */
        if( s_ptGpio->aulGpio_counter_cnt[uiIdx] < aulTimerValues[uiIdx] )
          ulSoftIntMask |= ulTempIrqMsk;

      } else
      {
        /* Single shot timer */
        if( 0 == (s_ptGpio->aulGpio_counter_ctrl[uiIdx] & MSK_NX50_gpio_counter0_ctrl_run) )
        {
          /* Timer was running at start of Confirmation and has stopped now so this timer has expired */
          ulSoftIntMask |= ulTempIrqMsk;
        }
      }
    }
  }

  /* Generate software interrupt for all counters that expired. This may result
     in "normal" AND software IRQ being set. Make sure the interrupt handlers clears both.
     Software interrupts will be cleared when calling netX50HandleTimerIrq */
  POKE( Adr_NX50_vic_vic_softint, ulSoftIntMask );

  IRQ_LockRestore(irq);
}

/*****************************************************************************/
/*! GPIO Sleep
* \description
*   Sleep a while using a GPIO timer.
* \class
*   GPIO
* \params
*   ulGpioNum          [in]   Selected GPIO
*   uiTimeout          [out]  The Time to wait in µs
* \return
*                                                                            */
/*****************************************************************************/
void NX50_GPIO_Sleep( unsigned long ulCounter, unsigned int uiTimeout )
{
  unsigned int uiVal;

  /* Convert counter value from µs to ns */
  uiTimeout = uiTimeout * (NX50_DEV_FREQUENCY/1000000);

  s_ptGpio->aulGpio_counter_ctrl[ulCounter]  = 0;          /* Clear the timer register         */
  s_ptGpio->aulGpio_counter_cnt[ulCounter]   = 0;          /* Clear the current counter value  */
  s_ptGpio->aulGpio_counter_max[ulCounter]   = uiTimeout;  /* Set the counter value            */
  s_ptGpio->aulGpio_counter_ctrl[ulCounter] |= (MSK_NX50_gpio_counter0_ctrl_run | MSK_NX50_gpio_counter0_ctrl_once  ); /* Enable the timer to one shot */

  /* poll timer ctrl for 'run' bit */
  do {
    uiVal  = s_ptGpio->aulGpio_counter_ctrl[ulCounter] & MSK_NX50_gpio_counter0_ctrl_run;
  } while ( uiVal!=0 );
}

/*****************************************************************************/
/*! GPIO Reset Timer
* \description
*   Reset a GPIO timer.
* \class
*   GPIO
* \params
*   uCounter          [in]  Selected Counter
* \return
*                                                                            */
/*****************************************************************************/
void NX50_GPIO_ResetTimer( unsigned int uCounter )
{
  s_ptGpio->aulGpio_counter_ctrl[uCounter] = 0;
  s_ptGpio->aulGpio_counter_cnt[uCounter]  = 0;
  s_ptGpio->aulGpio_counter_max[uCounter]  = 0;
  s_ptGpio->ulCnt_irq_mask_rst             = 1 << uCounter;
  s_ptGpio->ulCnt_irq_raw                  = 1 << uCounter;
}

/*****************************************************************************/
/*! GPIO Prepare Timer
* \description
*   Prepares a GPIO timer.
* \class
*   GPIO
* \params
*   ulGpioNum          [in]  Selected GPIO
*   uiTimeout          [in]  Timeout value [10ns]
*   uiOnce             [in]  1/0: Single run, Continuous run
* \return
*                                                                            */
/*****************************************************************************/
void NX50_GPIO_PrepareTimer( unsigned int uCounter, unsigned int uiTimeout, unsigned int uiOnce )
{
  s_ptGpio->aulGpio_counter_ctrl[uCounter]  = 0;          /* Clear the timer register         */
  s_ptGpio->aulGpio_counter_cnt[uCounter]   = 0;          /* Clear the current counter value  */
  s_ptGpio->aulGpio_counter_max[uCounter]   = uiTimeout;  /* Set the counter value            */
  s_ptGpio->aulGpio_counter_ctrl[uCounter]  |= (uiOnce << SRT_NX50_gpio_counter0_ctrl_once);
}

/*****************************************************************************/
/*! GPIO Start Timer
* \description
*   Starts a GPIO timer.
* \class
*   GPIO
* \params
*   ulGpioNum          [in]   Selected GPIO
* \return
*                                                                            */
/*****************************************************************************/
void NX50_GPIO_StartTimer( unsigned int uCounter )
{
  s_ptGpio->aulGpio_counter_ctrl[uCounter] |= MSK_NX50_gpio_counter0_ctrl_run; /* Enable the timer */
}

/*****************************************************************************/
/*! GPIO IsTimerRunning
* \description
*   Checks for GPIO timer expired.
* \class
*   GPIO
* \params
* \return
*    0
*    1                                                                       */
/*****************************************************************************/
int NX50_GPIO_IsTimerRunning( unsigned long ulCounter )
{
  /* gets timer ctrl 'run' bit */
  if( (s_ptGpio->aulGpio_counter_ctrl[ulCounter] & MSK_NX50_gpio_counter0_ctrl_run) == 0 )
    return 0;
  else
    return 1;
}

/*****************************************************************************/
/*! GPIO Timer Interrupt Enable
* \description
*   Enable interrupt request for the selected Counter.
* \class
*   GPIO
* \params
*   uCounter          [in]   Selected Counter
* \return
*                                                                            */
/*****************************************************************************/
void NX50_GPIO_EnableTimerIrq( unsigned int uCounter )
{
  s_ptGpio->aulGpio_counter_ctrl[uCounter]  |= MSK_NX50_gpio_counter0_ctrl_irq_en;
  s_ptGpio->ulCnt_irq_mask_set |= 1 << (uCounter);
}

/*
  ____    ___    ___
 |  _ \  |_ _|  / _ \
 | |_) |  | |  | | | |
 |  __/   | |  | |_| |
 |_|     |___|  \___/

*/

/*****************************************************************************/
/*! PIO Set Mode
* \description
*   Enables/disable output for the selected PIO.
* \class
*   PIO
* \params
*   ulPioNum          [in]  Selected PIO
*   ulMode            [in]  PIO_MODE_IN  = Set the PIO to input mode
*                           PIO_MODE_OUT = Set the PIO to output mode
* \return
*                                                                            */
/*****************************************************************************/
void NX50_PIO_SetMode( unsigned long ulPioNum, unsigned long ulMode )
{
  if( NX50_PIO_MODE_OUT == ulMode )
    s_ptPio->ulPio_oe |= 1U << ulPioNum;
  else
    s_ptPio->ulPio_oe &= ~(1U << ulPioNum);
}

/*****************************************************************************/
/*! PIO Get Input
* \description
*   Read current value of selected PIO within PIO in register.
* \class
*   PIO
* \params
*   ulPioNum          [in]  Selected PIO
* \return
*    0
*    1                                                                       */
/*****************************************************************************/
int NX50_PIO_GetInput( unsigned long ulPioNum )
{
  return ( s_ptPio->ulPio_in & (1 << ulPioNum)) ? 1 : 0;
}

/*****************************************************************************/
/*! PIO Get In Register
* \description
*  Read the PIO IN register.
* \class
*   PIO
* \params
* \return
*   Pio_in value                                                            */
/*****************************************************************************/
unsigned long NX50_PIO_GetIn( void )
{
  return ( s_ptPio->ulPio_in );
}

/*****************************************************************************/
/*! PIO Get Output
* \description
*   Read current value of selected PIO within PIO out register.
* \class
*   PIO
* \params
*   ulPioNum          [in]  Selected PIO
* \return
*    0
*    1                                                                       */
/*****************************************************************************/
int NX50_PIO_GetOutput( unsigned long ulPioNum )
{
  return ( s_ptPio->ulPio_out & (1 << ulPioNum)) ? 1 : 0;
}

/*****************************************************************************/
/*! PIO Set Output
* \description
*   Set output for the selected PIO.
* \class
*   PIO
* \params
*   ulPioNum          [in]  Selected PIO
*   fEnable           [in]  0 = Disable the Output
*                           1 = Enable the Output
* \return
*                                                                            */
/*****************************************************************************/
void NX50_PIO_SetOutput( unsigned long ulPioNum, int fEnable )
{
  if(fEnable) {
    s_ptPio->ulPio_out |= 1U << ulPioNum;
  } else {
    s_ptPio->ulPio_out &= ~(1U << ulPioNum);
  }
}

/*****************************************************************************/
/*! PIO Get Line
* \description
*  Read the PIO out register.
* \class
*   PIO
* \params
* \return
*   Pio_line value                                                          */
/*****************************************************************************/
unsigned long NX50_PIO_GetLine( void )
{
  return ( s_ptPio->ulPio_out );
}

/*****************************************************************************/
/*! PIO Set Line
* \description
*   Set PIO line.
* \class
*   PIO
* \params
*   ulVal          [in]   Value of PIO Line
* \return
*                                                                            */
/*****************************************************************************/
void NX50_PIO_SetLine( unsigned long ulVal )
{
  s_ptPio->ulPio_out = ulVal;
}

void NX50_HIF_SetHifMode( unsigned int uiHifMode )
{
  unsigned long ulVal = s_ptNetXGlobalRegBlock2Area->ulIf_conf1;
  ulVal &= ~(7UL << 28);
  ulVal |= uiHifMode << 28;
  s_ptNetXGlobalRegBlock2Area->ulIf_conf1 = ulVal;
}


/*****************************************************************************/
/*! HIF Set IO Mode
* \description
*   Selects IO or HIF functionality for each HIF_PIO Pin (PIO32..PIO84).
* \class
*   HIF
* \params
*   ulHifModeEnable32to63 [in]  Bit n 0/1: PIO Pin n+32 is PIO/HIF mode (n = 0..31)
*   ulHifModeEnable64to84 [in]  Bit n 0/1: PIO Pin n+64 is PIO/HIF mode (n = 0..20)
* \return
*                                                                            */
/*****************************************************************************/
void NX50_HIF_SetIoMode( unsigned long ulHifModeEnable32to63, unsigned long ulHifModeEnable64to84 )
{
  /* enable PIO input sampling at 100MHz */
  ulHifModeEnable64to84 &= MSK_NX50_DPM_ARM_IO_MODE1_PIO_MODE;
  ulHifModeEnable64to84 |= (1UL << SRT_NX50_DPM_ARM_IO_MODE1_IN_CONTROL);
  s_ptNetXGlobalRegBlock2Area->ulIo_reg_mode1 = ulHifModeEnable64to84;
  s_ptNetXGlobalRegBlock2Area->ulIo_reg_mode0 = ulHifModeEnable32to63;
}


/*****************************************************************************/
/*! HIF Set PIO Output Enable
* \description
*   Configures if the HIF PIOs of the selected line are inputs or outputs.
* \class
*   HIF
* \params
*   uiLine       [in]  Line index: 0/1 PIOs 32..63/64..84
*   ulHifPioLine [in]  Bit mask to set PIO outputs enables of the selected line
* \return
*                                                                            */
/*****************************************************************************/
void NX50_HIF_SetPioOe( unsigned int uiLine, unsigned long ulHifPioLine )
{
  if( uiLine == 0 )
  {
    s_ptNetXGlobalRegBlock2Area->ulIo_reg_drv_en0 = ulHifPioLine;
  }
  else
  {
    s_ptNetXGlobalRegBlock2Area->ulIo_reg_drv_en1 = ulHifPioLine;
  }
}


/*****************************************************************************/
/*! HIF Set PIO Output Value
* \description
*   Configures the HIF PIOs Outputs driving high or low level.
* \class
*   HIF
* \params
*   uiLine       [in]  Line index: 0/1 PIOs 32..63/64..84
*   ulHifPioLine [in]  Bit mask to set PIO outputs of the selected line
* \return
*                                                                            */
/*****************************************************************************/
void NX50_HIF_SetPioOut( unsigned int uiLine, unsigned long ulHifPioLine )
{
  if( uiLine == 0 )
  {
    s_ptNetXGlobalRegBlock2Area->ulIo_reg_data0 = ~ulHifPioLine;
  }
  else
  {
    s_ptNetXGlobalRegBlock2Area->ulIo_reg_data1 = ~ulHifPioLine;
  }
}

/*****************************************************************************/
/*! HIF Get PIO Input Value
* \description
*   Gets the current state of the HIF PIOs Inputs.
* \class
*   HIF
* \params
*   uiLine       [in]  Line index: 0/1 PIOs 32..63/64..84
*   ulHifPioLine [in]  Bit mask to set PIO outputs of the selected line
* \return
*                                                                            */
/*****************************************************************************/
unsigned long NX50_HIF_GetPioIn( unsigned int uiLine )
{
  unsigned long ulData;

  if( uiLine == 0 )
  {
    ulData = s_ptNetXGlobalRegBlock2Area->ulIo_reg_data0;
  }
  else
  {
    ulData = s_ptNetXGlobalRegBlock2Area->ulIo_reg_data1;
  }

  return ulData;
}

/*
  __  __   __  __   ___    ___
 |  \/  | |  \/  | |_ _|  / _ \
 | |\/| | | |\/| |  | |  | | | |
 | |  | | | |  | |  | |  | |_| |
 |_|  |_| |_|  |_| |___|  \___/

*/

/*****************************************************************************/
/*! MMIO Set Configuration
* \description
*   Configures a MMIO pin.
* \class
*   MMIO
* \params
*   uMmioNum          [in]  Selected MMIO
*   bFunction         [in]  Signal Selection
*   fInvertOut        [in]  Invert Output Signal
*   fInvertIn         [in]  Invert Input Signal
* \return
*                                                                            */
/*****************************************************************************/
void NX50_MMIO_SetCfg( unsigned int uMmioNum, unsigned char bFunction, int fInvertOut, int fInvertIn )
{
  // read access key, write back access key
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;

  s_ptMmioCtrl->aulMmio_cfg[uMmioNum] =  bFunction << SRT_NX50_mmio0_cfg_mmio0_sel
                                        |(fInvertOut ? MSK_NX50_mmio0_cfg_mmio0_out_inv : 0)
                                        |(fInvertIn  ? MSK_NX50_mmio0_cfg_mmio0_in_inv  : 0);

}

/*
  ____        _           ____
 |  _ \    __| |  _   _  |  _ \   _   _   _ __
 | |_) |  / _` | | | | | | |_) | | | | | | '_ \
 |  _ <  | (_| | | |_| | |  _ <  | |_| | | | | |
 |_| \_\  \__,_|  \__, | |_| \_\  \__,_| |_| |_|
                  |___/
*/

/*****************************************************************************/
/*! Set RDYRUN Led
* \description
*   Set RDY/RUN led depending of mode.
* \class
*   RDYRUN
* \params
*   tMode          [in]  Ready/Run Mode (off, green, red, inv) to set
* \return
*                                                                            */
/*****************************************************************************/
void NX50_RDYRUN_SetRdyRunLed( NX50_RDYRUN_LED_E tMode )
{
  switch( tMode ) {
  case NX50_RDYRUN_LED_OFF:
    s_ptNetXGlobalRegBlock1Area->ulSta_netx = 0x03000000;
    break;
  case NX50_RDYRUN_LED_GREEN:
    s_ptNetXGlobalRegBlock1Area->ulSta_netx = 0x03000001;
    break;
  case NX50_RDYRUN_LED_RED:
    s_ptNetXGlobalRegBlock1Area->ulSta_netx = 0x03000002;
    break;
  case NX50_RDYRUN_LED_INV:
    s_ptNetXGlobalRegBlock1Area->ulSta_netx = ((s_ptNetXGlobalRegBlock1Area->ulSta_netx & 3) ^ 3) | 0x03000000;
    break;
  }
}

/*
  ___           _                                   _           ____    _   _  __   __
 |_ _|  _ __   | |_    ___   _ __   _ __     __ _  | |         |  _ \  | | | | \ \ / /
  | |  | '_ \  | __|  / _ \ | '__| | '_ \   / _` | | |         | |_) | | |_| |  \ V /
  | |  | | | | | |_  |  __/ | |    | | | | | (_| | | |         |  __/  |  _  |   | |
 |___| |_| |_|  \__|  \___| |_|    |_| |_|  \__,_| |_|  _____  |_|     |_| |_|   |_|
                                                       |_____|

*/
/*****************************************************************************/
/*! Init Internal PHY
* \description
*   Initialize internal PHY.
* \class
*   INTPHY
* \params
*   ptPhyCtrlInit          [in]  Pointer of PHY Control Init Value
* \return
*                                                                            */
/*****************************************************************************/
void NX50_INTPHY_Init( NX50_PHY_CONTROL_T *ptPhyCtrlInit )
{
  unsigned short usMiimuData;
  unsigned int ulDelayCnt;
  unsigned int uPhyAdr = (ptPhyCtrlInit->bf.phy_address << 1);

  // clear bypass
  ptPhyCtrlInit->bf.phy_sim_byp = 0;

  // read access key, write back access key
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  // write value
  s_ptAsicCtrl->ulPhy_control = ptPhyCtrlInit->val | MSK_NX50_phy_control_phy_reset;

  // delay for 100us -> do 5 MII transfers of 24us each
  ulDelayCnt = 5;
  do
  {
    NX50_MIIMU_ReadPhyReg( NX50_MIIMU_SEND_PREAMBLE,
                           NX50_MIIMU_MDCFREQ_HIGH,
                           NX50_MIIMU_RTA_0_BITS,
                           NX50_INT_PHY1_ADDR,
                           DRV_CB12_CONTROL,
                           &usMiimuData,
                           0
                          );
  } while( ulDelayCnt--!=0 );

  // read access key, write back access key
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;

  // write value
  s_ptAsicCtrl->ulPhy_control = ptPhyCtrlInit->val;


  // nothing left to do if PHYs are in reset
  if( ptPhyCtrlInit->bf.phy_reset == 1 )
    return;

  // wait for PHY1 ready after reset if PHY is enabled
  if( ptPhyCtrlInit->bf.phy0_enable == PHY_CTRL_PHY1_ENABLE )
  {
    // wait until MIIMU access to PHY 1 successfully
    do {
      NX50_MIIMU_ReadPhyReg( NX50_MIIMU_SEND_PREAMBLE,
                           NX50_MIIMU_MDCFREQ_HIGH,
                           NX50_MIIMU_RTA_0_BITS,
                           uPhyAdr,
                           DRV_CB12_SILICON_REVISION,
                           &usMiimuData,
                           0
                          );
    } while( (usMiimuData==0) || (usMiimuData==0xffff) );

    // Workaround sequence for correct Auto-MDIX function in forced 100 MBit-Mode
    if(   ptPhyCtrlInit->bf.phy0_automdix==PHY_CTRL_PHY1_AUTOMDIX
        && (  (ptPhyCtrlInit->bf.phy0_mode==PHYCTRL_100BASE_TXFX_HD_NOAUTONEG_CRSTXRX)
            ||(ptPhyCtrlInit->bf.phy0_mode==PHYCTRL_100BASE_TXFX_FD_NOAUTONEG_CRSRX)
           )
      )
    {
      NX50_INTPHY_CorrectAutoMdix100MBit(uPhyAdr);
    }
  }

  // wait for PHY2 ready after reset if PHY is enabled
  if( ptPhyCtrlInit->bf.phy1_enable == PHY_CTRL_PHY2_ENABLE )
  {
    // wait until MIIMU access to PHY 2 successfully
    do {
      NX50_MIIMU_ReadPhyReg( NX50_MIIMU_SEND_PREAMBLE,
                           NX50_MIIMU_MDCFREQ_HIGH,
                           NX50_MIIMU_RTA_0_BITS,
                           uPhyAdr + 1,
                           DRV_CB12_SILICON_REVISION,
                           &usMiimuData,
                           0
                          );
    } while( (usMiimuData==0) || (usMiimuData==0xffff) );

    if(   ptPhyCtrlInit->bf.phy1_automdix==PHY_CTRL_PHY2_AUTOMDIX
        && (   (ptPhyCtrlInit->bf.phy1_mode==PHYCTRL_100BASE_TXFX_HD_NOAUTONEG_CRSTXRX)
             ||(ptPhyCtrlInit->bf.phy1_mode==PHYCTRL_100BASE_TXFX_FD_NOAUTONEG_CRSRX)
           )
      )
    {
      NX50_INTPHY_CorrectAutoMdix100MBit(uPhyAdr + 1);
    }
  }
}

/*****************************************************************************/
/*! Correct AutoMDIX for 100 MBit forced modes
* \description
*   This function is a workaround to enable the AutoMDIX function in forced 100 MBit mode (HD or FD).
*   This sequence is necessary because after forcing the internal PHY into 100 MBit mode the requested AutoMDIX function does not work correctly.
*   Note: This function may only be called when requesting forced 100 MBit mode in conjunction with AutoMDIX functionality.
* \class
*   INTPHY
* \params
*   uMiimuPhyAddr          [in] MIIMU PHY Address
* \return
*                                                                            */
/*****************************************************************************/
void NX50_INTPHY_CorrectAutoMdix100MBit( unsigned int uMiimuPhyAddr )
{
  NX50_MIIMU_WritePhyReg( NX50_MIIMU_SEND_PREAMBLE, NX50_MIIMU_MDCFREQ_HIGH, uMiimuPhyAddr, 0x14, 0x400, 0 );
  NX50_MIIMU_WritePhyReg( NX50_MIIMU_SEND_PREAMBLE, NX50_MIIMU_MDCFREQ_HIGH, uMiimuPhyAddr, 0x14, 0x0, 0 );
  NX50_MIIMU_WritePhyReg( NX50_MIIMU_SEND_PREAMBLE, NX50_MIIMU_MDCFREQ_HIGH, uMiimuPhyAddr, 0x14, 0x400, 0 );
  NX50_MIIMU_WritePhyReg( NX50_MIIMU_SEND_PREAMBLE, NX50_MIIMU_MDCFREQ_HIGH, uMiimuPhyAddr, 0x17, 0xa1, 0 );
  NX50_MIIMU_WritePhyReg( NX50_MIIMU_SEND_PREAMBLE, NX50_MIIMU_MDCFREQ_HIGH, uMiimuPhyAddr, 0x14, 0x441b, 0 );
  NX50_MIIMU_WritePhyReg( NX50_MIIMU_SEND_PREAMBLE, NX50_MIIMU_MDCFREQ_HIGH, uMiimuPhyAddr, 0x14, 0x0, 0 );
}

/*
  ____            _           _                           _____   _    __
 |  _ \    ___   (_)  _ __   | |_    ___   _ __          |  ___| (_)  / _|   ___
 | |_) |  / _ \  | | | '_ \  | __|  / _ \ | '__|         | |_    | | | |_   / _ \
 |  __/  | (_) | | | | | | | | |_  |  __/ | |            |  _|   | | |  _| | (_) |
 |_|      \___/  |_| |_| |_|  \__|  \___| |_|     _____  |_|     |_| |_|    \___/
                                                 |_____|
*/

/*****************************************************************************/
/*! Reset Pointer Fifo
* \description
*   Resets pointer fifo to default values.
* \class
*   PFIFO
* \params
* \return
*                                                                            */
/*****************************************************************************/
void NX50_PFIFO_Reset( void )
{
  unsigned int uCnt;

  // set reset flag of all fifos
  s_ptPFifo->ulPfifo_reset = 0xffffffff;

  /* reset pointer fifo borders to first 16 fifos of size 64 and last 16 fifos of size 0 */
  /* Note: 8 Fifos must have contain exact 512 entries */
  for( uCnt = 0; uCnt < 16; uCnt++ ) {
    s_ptPFifo->aulPfifo_border[uCnt] = ((uCnt+1)* 64)-1 ;
  }
  for( uCnt = 16; uCnt < 32; uCnt++ ) {
    s_ptPFifo->aulPfifo_border[uCnt] = ((15+1)* 64)-1 ;
  }

  // clear reset flag of all fifos
  s_ptPFifo->ulPfifo_reset = 0;
}

/*****************************************************************************/
/*! Set Pointer FIFO Borders
* \description
*   Set pointer FIFO borders to given values.
* \class
*   PFIFO
* \params
*   auiPFifoDepth  [in] Array of 32 Elements containing the depth of each FIFO
* \return
*                                                                            */
/*****************************************************************************/
int NX50_PFIFO_SetBorders(const unsigned int *auiPFifoDepth)
{
  int iResult;
  unsigned int uiBorder;
  unsigned int uiFifoNum;

  /* set reset bit for all pointer FIFOs */
  s_ptPFifo->ulPfifo_reset = 0xffffffff;

  /* define pointer FIFO borders */
  uiBorder = 0;
  for(uiFifoNum=0; uiFifoNum < 32; uiFifoNum++)
  {
    uiBorder += auiPFifoDepth[uiFifoNum];
    s_ptPFifo->aulPfifo_border[uiFifoNum] = uiBorder - 1;
  }

  if( uiBorder > 2048 ) {
    /* sum of all FIFO elements exceeds the limit */
    iResult = -1;
  } else {
    /* ok! */
    iResult = 0;

    /* clear reset bit for all pointer FIFOs */
    s_ptPFifo->ulPfifo_reset = 0x00000000;
  }

  return iResult;
}


/*****************************************************************************/
/*! Get Pointer FIFO Borders
* \description
*   Get pointer FIFO depth.
* \class
*   PFIFO
* \params
*   auiPFifoDepth  [out] Array of 32 Elements containing the depth of each FIFO
* \return
*                                                                            */
/*****************************************************************************/
int NX50_PFIFO_GetBorders(unsigned int *auiPFifoDepth)
{
  int iResult;
  unsigned int uiBorder, uiBorderPrev;
  unsigned int uiFifoNum;

  /* read pointer FIFO borders */
  uiBorderPrev = 0;
  for(uiFifoNum = 0; uiFifoNum < 32; uiFifoNum++)
  {
    uiBorder = s_ptPFifo->aulPfifo_border[uiFifoNum] + 1;
    auiPFifoDepth[uiFifoNum] = uiBorder - uiBorderPrev;
    uiBorderPrev = uiBorder;
  }

  if( uiBorder > 2048 ) {
    /* sum of all FIFO elements exceeds the limit */
    iResult = -1;
  } else {
    /* OK! */
    iResult = 0;
  }

  return iResult;
}

/*****************************************************************************/
/*! Get Fifo Fill Level
* \description
*   Returns the fill level of the fifo
* \class
*   PFIFO
* \params
* \return
*                                                                            */
/*****************************************************************************/
unsigned long NX50_PFIFO_GetFillLevel( unsigned int uFifoNum )
{
  // set reset flag of all fifos
  if( uFifoNum<32 )
    return s_ptPFifo->aulPfifo_fill_level[uFifoNum];
  else
    return 0xffffffffUL;
}

/*****************************************************************************/
/*! Get FIFO Reset Vector
* \description
*   Retrieves the reset vector of pointer fifo.
* \class
*   PFIFO
* \params
* \return
    reset vector
                                                                             */
/*****************************************************************************/
unsigned long NX50_PFIFO_GetFifoResetVector( void )
{
  return s_ptPFifo->ulPfifo_reset;
}

/*****************************************************************************/
/*! Get FIFO Full Vector
* \description
*   Retrieves the full vector of pointer fifo.
* \class
*   PFIFO
* \params
* \return
    full vector
                                                                             */
/*****************************************************************************/
unsigned long NX50_PFIFO_GetFifoFullVector( void )
{
  return s_ptPFifo->ulPfifo_full;
}

/*****************************************************************************/
/*! Get FIFO Empty Vector
* \description
*   Retrieves the empty vector of pointer fifo.
* \class
*   PFIFO
* \params
* \return
    empty vector
                                                                             */
/*****************************************************************************/
unsigned long NX50_PFIFO_GetFifoEmptyVector( void )
{
  return s_ptPFifo->ulPfifo_empty;
}

/*****************************************************************************/
/*! Get FIFO Overflow Vector
* \description
*   Retrieves the overflow vector of pointer fifo.
* \class
*   PFIFO
* \params
* \return
    overflow vector
                                                                             */
/*****************************************************************************/
unsigned long NX50_PFIFO_GetFifoOverflowVector( void )
{
  return s_ptPFifo->ulPfifo_overflow;
}

/*****************************************************************************/
/*! Get FIFO Underrun Vector
* \description
*   Retrieves the underrun vector of pointer fifo.
* \class
*   PFIFO
* \params
* \return
    underrun vector
                                                                             */
/*****************************************************************************/
unsigned long NX50_PFIFO_GetFifoUnderrunVector( void )
{
  return s_ptPFifo->ulPfifo_underrun;
}

//
// ######  #    #  #    #   ####   #####  #   ####   #    #   ####
// #       #    #  ##   #  #    #    #    #  #    #  ##   #  #
// #####   #    #  # #  #  #         #    #  #    #  # #  #   ####
// #       #    #  #  # #  #         #    #  #    #  #  # #       #
// #       #    #  #   ##  #    #    #    #  #    #  #   ##  #    #
// #        ####   #    #   ####     #    #   ####   #    #   ####
//
//                                                        #     #     #     #
// #    #   ####   ######  #####       #####   #   #      #     #    # #    #
// #    #  #       #       #    #      #    #   # #       #     #   #   #   #
// #    #   ####   #####   #    #      #####     #        #######  #     #  #
// #    #       #  #       #    #      #    #    #        #     #  #######  #
// #    #  #    #  #       #    #      #    #    #        #     #  #     #  #
//  ####    ####   ######  #####       #####     #        #     #  #     #  #######
//

/*****************************************************************************/
#include "hal_resources_defines.h"
__USE_XPEC_REGS
__USE_XMAC
__USE_MIIMU
__USE_SYSTIME
/*****************************************************************************/

/*
 __  __   ____           _                           _
 \ \/ /  / ___|         | |       ___     __ _    __| |   ___   _ __
  \  /  | |             | |      / _ \   / _` |  / _` |  / _ \ | '__|
  /  \  | |___          | |___  | (_) | | (_| | | (_| | |  __/ | |
 /_/\_\  \____|  _____  |_____|  \___/   \__,_|  \__,_|  \___| |_|
                |_____|

*/

#define NUM_XPECS 2

static const unsigned long XcCode_rpu_reset0[57] = {
  0x000000dc, // program size
  0x00000000, // trailing loads size
  0x1c060000, 0x00fc0001, 0x995fdc01, 0x00f80041, 0x995fdc02, 0x00e00000, 0x001fdc03, 0x00fcfffd,
  0x994fe084, 0x00fffffd, 0x995fdc05, 0x010bfffd, 0x995fdc06, 0x010c0001, 0x995fdc07, 0x010ffffd,
  0x995fdc08, 0x01080001, 0x995fdc09, 0x010c0001, 0x995fdc0a, 0x00f80001, 0x995fdc0b, 0x00fdfffd,
  0x995fdc0c, 0x00fcfffd, 0x995fdc0d, 0x00fc7ffd, 0x995fdc0e, 0x00fc3ffd, 0x995fdc0f, 0x00fc1ffd,
  0x995fdc10, 0x00fc0ffd, 0x995fdc11, 0x00fc07fd, 0x995fdc12, 0x00fc03fd, 0x995fdc13, 0x00fc01fd,
  0x995fdc14, 0x00fc00fd, 0x995fdc15, 0x00fc007d, 0x995fdc16, 0x00fc003d, 0x995fdc17, 0x00fc001d,
  0x995fdc18, 0x00fc000d, 0x995fdc19, 0x00fc0005, 0x995fdc1a, 0x00e00000, 0x001fdc1a,
  // trailing loads

};

static const unsigned long XcCode_rpu_reset1[57] = {
  0x000000dc, // program size
  0x00000000, // trailing loads size
  0x1c061000, 0x00fc0001, 0x995fdc01, 0x00f80041, 0x995fdc02, 0x00e00000, 0x001fdc03, 0x00fcfffd,
  0x994fe084, 0x00fffffd, 0x995fdc05, 0x010bfffd, 0x995fdc06, 0x010c0001, 0x995fdc07, 0x010ffffd,
  0x995fdc08, 0x01080001, 0x995fdc09, 0x010c0001, 0x995fdc0a, 0x00f80001, 0x995fdc0b, 0x00fdfffd,
  0x995fdc0c, 0x00fcfffd, 0x995fdc0d, 0x00fc7ffd, 0x995fdc0e, 0x00fc3ffd, 0x995fdc0f, 0x00fc1ffd,
  0x995fdc10, 0x00fc0ffd, 0x995fdc11, 0x00fc07fd, 0x995fdc12, 0x00fc03fd, 0x995fdc13, 0x00fc01fd,
  0x995fdc14, 0x00fc00fd, 0x995fdc15, 0x00fc007d, 0x995fdc16, 0x00fc003d, 0x995fdc17, 0x00fc001d,
  0x995fdc18, 0x00fc000d, 0x995fdc19, 0x00fc0005, 0x995fdc1a, 0x00e00000, 0x001fdc1a,
  // trailing loads

};

static const unsigned long XcCode_tpu_reset0[57] = {
  0x000000dc, // program size
  0x00000000, // trailing loads size
  0x1c060400, 0x01140001, 0x995fdc01, 0x01100641, 0x995fdc02, 0x00e00000, 0x001fdc03, 0x0114fffd,
  0x994fe384, 0x0117fffd, 0x995fdc05, 0x0123fffd, 0x995fdc06, 0x01240001, 0x995fdc07, 0x0127fffd,
  0x995fdc08, 0x01200001, 0x995fdc09, 0x01240001, 0x995fdc0a, 0x01100601, 0x995fdc0b, 0x0115fffd,
  0x995fdc0c, 0x0114fffd, 0x995fdc0d, 0x01147ffd, 0x995fdc0e, 0x01143ffd, 0x995fdc0f, 0x01141ffd,
  0x995fdc10, 0x01140ffd, 0x995fdc11, 0x011407fd, 0x995fdc12, 0x011403fd, 0x995fdc13, 0x011401fd,
  0x995fdc14, 0x011400fd, 0x995fdc15, 0x0114007d, 0x995fdc16, 0x0114003d, 0x995fdc17, 0x0114001d,
  0x995fdc18, 0x0114000d, 0x995fdc19, 0x01140005, 0x995fdc1a, 0x00e00000, 0x001fdc1a,
  // trailing loads

};

static const unsigned long XcCode_tpu_reset1[57] = {
  0x000000dc, // program size
  0x00000000, // trailing loads size
  0x1c061400, 0x01140001, 0x995fdc01, 0x01100641, 0x995fdc02, 0x00e00000, 0x001fdc03, 0x0114fffd,
  0x994fe384, 0x0117fffd, 0x995fdc05, 0x0123fffd, 0x995fdc06, 0x01240001, 0x995fdc07, 0x0127fffd,
  0x995fdc08, 0x01200001, 0x995fdc09, 0x01240001, 0x995fdc0a, 0x01100601, 0x995fdc0b, 0x0115fffd,
  0x995fdc0c, 0x0114fffd, 0x995fdc0d, 0x01147ffd, 0x995fdc0e, 0x01143ffd, 0x995fdc0f, 0x01141ffd,
  0x995fdc10, 0x01140ffd, 0x995fdc11, 0x011407fd, 0x995fdc12, 0x011403fd, 0x995fdc13, 0x011401fd,
  0x995fdc14, 0x011400fd, 0x995fdc15, 0x0114007d, 0x995fdc16, 0x0114003d, 0x995fdc17, 0x0114001d,
  0x995fdc18, 0x0114000d, 0x995fdc19, 0x01140005, 0x995fdc1a, 0x00e00000, 0x001fdc1a,
  // trailing loads

};

static const unsigned long* paulxMacRpuCodes[2]=
{
  XcCode_rpu_reset0,
  XcCode_rpu_reset1
};

static const unsigned long* paulxMacTpuCodes[2]=
{
  XcCode_tpu_reset0,
  XcCode_tpu_reset1
};

/*****************************************************************************/
/*! Reset XC Code
* \description
*   Reset XC port.
* \class
*   XC
* \params
*   uiPort           [in]  XC Port Number
*   pvUser           [in]  User Specific Parameter
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX50_XC_Reset( unsigned int uiPort, void* pvUser )
{
  NX50_XMAC_AREA_T* ptXmac;
  NX50_XPEC_AREA_T* ptXpec;
  unsigned int uIdx;

  /* check instance number */
  if( uiPort>=NUM_XPECS ) return -1;

  ptXmac = s_ptXmacStart[uiPort];
  ptXpec = s_ptXpecStart[uiPort];

  ptXpec->ulXpu_hold_pc = 1;                                         /* Hold the Program Counter */
  ptXpec->aulStatcfg[uiPort] = 0x08500000;                          /* Reset IO.OE asap */
  ptXmac->ulXmac_tpu_hold_pc = MSK_NX50_xmac_tpu_hold_pc_tpu_hold;   /* Just set Hold-Bit */
  ptXmac->ulXmac_rpu_hold_pc = MSK_NX50_xmac_rpu_hold_pc_rpu_hold;   /* Just set Hold-Bit */
  ptXpec->aulStatcfg[uiPort] = 0x08500000;                          /* Reset IO.OE asap */

  /* load ratemul reset code */
  if( NX50_XC_Load( uiPort, NX50_XC_TYPE_RPU, (unsigned long*)paulxMacRpuCodes[uiPort], pvUser)!=0 ) {
    return -1;
  }
  if( NX50_XC_Load( uiPort, NX50_XC_TYPE_TPU, (unsigned long*)paulxMacTpuCodes[uiPort], pvUser)!=0 ) {
    return -1;
  }
  ptXmac->ulXmac_rpu_pc      = 0; /* Reset PC to 0 */
  ptXmac->ulXmac_tpu_pc      = 0; /* Reset PC to 0 */
  ptXmac->ulXmac_tpu_hold_pc = 0; /* Clear Hold-Bit */
  ptXmac->ulXmac_rpu_hold_pc = 0; /* Clear Hold-Bit */

  /* !!!! ATTENTION: There must be enougth time between starting xMAC and stopping xMAC to execute reset programm */

  /* reset xPEC ALU */
  for( uIdx = 0; uIdx < NX50_XPEC_DWORD_RAMSIZE; uIdx++ )
    ptXpec->aulPram[uIdx] = 0xC0FFF000; /* Use the command wait b111111111111,b000000000000 at all addresses */
  ptXpec->ulXpec_pc = 0x7ff; /* Reset the Program Counter to 0x7ff */
  ptXpec->ulXpec_statcfg = 0; /* Reset Timer operation and DMA */
  ptXpec->aulDram[0x7FF] = 0x7F; /* Link and Stop DMA */
  for(uIdx = 0; uIdx < 10; uIdx++)
    ptXpec->ulXpu_hold_pc = 0; /* let the XC run for at least 10 cycles */
  ptXpec->ulXpu_hold_pc = 1; /* Hold the Program Counter */
  ptXpec->ulXpec_pc = 0x7ff; /* Reset the Program Counter to 0x7ff */

  /* reset all xPEC registers */
  ptXpec->aulXpec_r[0] = 0;
  ptXpec->aulXpec_r[1] = 0;
  ptXpec->aulXpec_r[2] = 0;
  ptXpec->aulXpec_r[3] = 0;
  ptXpec->aulXpec_r[4] = 0;
  ptXpec->aulXpec_r[5] = 0;
  ptXpec->aulXpec_r[6] = 0;
  ptXpec->aulXpec_r[7] = 0;
  ptXpec->ulRange01 = 0;
  ptXpec->ulRange23 = 0;
  ptXpec->ulRange45 = 0;
  ptXpec->ulRange67 = 0;
  ptXpec->aulTimer[0] = 0;
  ptXpec->aulTimer[1] = 0;
  ptXpec->aulTimer[2] = 0;
  ptXpec->aulTimer[3] = 0;
  ptXpec->ulUrx_count = 0;
  ptXpec->ulUtx_count = 0;
  ptXpec->ulXpec_statcfg = 0;
  ptXpec->ulTimer4    = 0;
  ptXpec->ulTimer5    = 0;

  /* Reset the event controller if the XC was stopped on a wait.
     This will program an IRQ event to branch to address 0.
     The command at address 0 is an endless loop */
  ptXpec->aulEc_mask[0] = 0x1000E0E0;
  ptXpec->aulEc_mask[1] = 0x1000E0E0;
  ptXpec->aulEc_mask[2] = 0x1000E0E0;
  ptXpec->aulEc_mask[3] = 0x1000E0E0;
  ptXpec->aulEc_mask[4] = 0x1000E0E0;
  ptXpec->aulEc_mask[5] = 0x1000E0E0;
  ptXpec->aulEc_mask[6] = 0x1000E0E0;
  ptXpec->aulEc_mask[7] = 0x1000E0E0;
  ptXpec->aulEc_mask[8] = 0x1000E0E0;
  ptXpec->aulEc_mask[9] = 0x1000E0E0;
  ptXpec->ulEc_maska    = 0x1000E0E0;
  ptXpec->ulEc_maskb    = 0x1000E0E0;
  ptXpec->aulPram[0]    = 0xbe000000;
  ptXpec->ulXpu_hold_pc = 0;

  /* Signal IRQ to XPEC */
  s_ptXpecIrq->aulIrq_xpec[uiPort] = 0x00010000;

  /* let the XC run for at least 10 cycles */
  for( uIdx = 0; uIdx < 10; uIdx++ )
    ptXpec->ulXpu_hold_pc = 0;

  ptXpec->ulXpu_hold_pc = 1; /* Hold the Program Counter */
  ptXpec->ulXpec_pc = 0x7ff; /* Reset the Program Counter to 0x7ff */
  ptXpec->ulIrq = 0xFFFF0000; /* Clear XPEC side IRQ request lines */

  /* reset events */
  ptXpec->aulEc_mask[0] = 0x0000FFFF;
  ptXpec->aulEc_mask[1] = 0x0000FFFF;
  ptXpec->aulEc_mask[2] = 0x0000FFFF;
  ptXpec->aulEc_mask[3] = 0x0000FFFF;
  ptXpec->aulEc_mask[4] = 0x0000FFFF;
  ptXpec->aulEc_mask[5] = 0x0000FFFF;
  ptXpec->aulEc_mask[6] = 0x0000FFFF;
  ptXpec->aulEc_mask[7] = 0x0000FFFF;
  ptXpec->aulEc_mask[8] = 0x0000FFFF;
  ptXpec->aulEc_mask[9] = 0x0000FFFF;
  ptXpec->ulEc_maska = 0x0000FFFF;
  ptXpec->ulEc_maskb = 0x0000FFFF;
  ptXpec->ulXpec_adc = 0;

  /* Reset SR0-3 for XC0, SR4-7 for XC1 */
  for( uIdx = 4 * uiPort; uIdx < 4 * uiPort + 4; ++uIdx )
    ptXpec->aulXpec_sr[uIdx] = 0;
  /* Reset SR8-11 for XC0, SR12-15 for XC1 */
  for( uIdx = 8 + 4 * uiPort; uIdx < 12 + 4 * uiPort; ++uIdx )
    ptXpec->aulXpec_sr[uIdx] = 0;

  /* hold xMAC */
  ptXmac->ulXmac_tpu_hold_pc = MSK_NX50_xmac_tpu_hold_pc_tpu_hold;
  ptXmac->ulXmac_rpu_hold_pc = MSK_NX50_xmac_rpu_hold_pc_rpu_hold;

  /* reset IRQs from ARM side */
  s_ptXpecIrq->aulIrq_xpec[uiPort] = 0x0000FFFF;

  /* reset urx and utx fifos */
  switch( uiPort )
  {
    case 0:
      ptXmac->ulXmac_config_shared0 |=MSK_NX50_xmac_config_shared0_reset_tx_fifo;
      ptXmac->ulXmac_config_shared0 &= ~MSK_NX50_xmac_config_shared0_reset_tx_fifo;
      ptXmac->ulXmac_config_shared0 |=MSK_NX50_xmac_config_shared0_reset_rx_fifo;
      ptXmac->ulXmac_config_shared0 &= ~MSK_NX50_xmac_config_shared0_reset_rx_fifo;
      break;
    case 1:
      ptXmac->ulXmac_config_shared1 |=MSK_NX50_xmac_config_shared1_reset_tx_fifo;
      ptXmac->ulXmac_config_shared1 &= ~MSK_NX50_xmac_config_shared1_reset_tx_fifo;
      ptXmac->ulXmac_config_shared1 |=MSK_NX50_xmac_config_shared1_reset_rx_fifo;
      ptXmac->ulXmac_config_shared1 &= ~MSK_NX50_xmac_config_shared1_reset_rx_fifo;
      break;
  }

  /* reset all xMAC registers to default values */
  ptXmac->ulXmac_rx_hw                = 0;
  ptXmac->ulXmac_rx_hw_count          = 0;
  ptXmac->ulXmac_tx                   = 0;
  ptXmac->ulXmac_tx_hw                = 0;
  ptXmac->ulXmac_tx_hw_count          = 0;
  ptXmac->ulXmac_tx_sent              = 0;
  ptXmac->aulXmac_wr[0]               = 0;
  ptXmac->aulXmac_wr[1]               = 0;
  ptXmac->aulXmac_wr[2]               = 0;
  ptXmac->aulXmac_wr[3]               = 0;
  ptXmac->aulXmac_wr[4]               = 0;
  ptXmac->aulXmac_wr[5]               = 0;
  ptXmac->aulXmac_wr[6]               = 0;
  ptXmac->aulXmac_wr[7]               = 0;
  ptXmac->aulXmac_wr[8]               = 0;
  ptXmac->aulXmac_wr[9]               = 0;
  ptXmac->ulXmac_config_mii           = 0;
  ptXmac->ulXmac_config_nibble_fifo   = 0x00000600;
  ptXmac->ulXmac_rpu_count1           = 0;
  ptXmac->ulXmac_rpu_count2           = 0;
  ptXmac->ulXmac_tpu_count1           = 0;
  ptXmac->ulXmac_tpu_count2           = 0;
  ptXmac->ulXmac_rx_count             = 0;
  ptXmac->ulXmac_tx_count             = 0;
  ptXmac->ulXmac_rpm_mask0            = 0;
  ptXmac->ulXmac_rpm_val0             = 0;
  ptXmac->ulXmac_rpm_mask1            = 0;
  ptXmac->ulXmac_rpm_val1             = 0;
  ptXmac->ulXmac_tpm_mask0            = 0;
  ptXmac->ulXmac_tpm_val0             = 0;
  ptXmac->ulXmac_tpm_mask1            = 0;
  ptXmac->ulXmac_tpm_val1             = 0;
  ptXmac->ulXmac_rx_tx_nof_bits       = 0x00000088;
  ptXmac->ulXmac_rx_crc_polynomial_l  = 0;
  ptXmac->ulXmac_rx_crc_polynomial_h  = 0;
  ptXmac->ulXmac_rx_crc_l             = 0;
  ptXmac->ulXmac_rx_crc_h             = 0;
  ptXmac->ulXmac_rx_crc_cfg           = 0;
  ptXmac->ulXmac_tx_crc_polynomial_l  = 0;
  ptXmac->ulXmac_tx_crc_polynomial_h  = 0;
  ptXmac->ulXmac_tx_crc_l             = 0;
  ptXmac->ulXmac_tx_crc_h             = 0;
  ptXmac->ulXmac_tx_crc_cfg           = 0;
  ptXmac->ulXmac_rpu_pc               = 0;
  ptXmac->ulXmac_tpu_pc               = 0;

  return 0;
}

/*****************************************************************************/
/*! Load XC Code
* \description
*   Load XC port.
* \class
*   XC
* \params
*   uiPort               [in]  XC Port Number
*   eXcType              [in]  XC Entity To Download To (RPU, TPU, xPEC)
*   pulXcPrg             [in]  Pointer To Microcode
*   pvUser               [in]  User specific parameter
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX50_XC_Load( unsigned int uiPort, NX50_XC_TYPE_E eXcType, unsigned long* pulXcPrg, void* pvUser )
{
  volatile unsigned long *pulDst, *pulDstCnt;
  volatile unsigned long *pulRamStart, *pulRamEnd;
  unsigned long ulPhysicalStart;
  unsigned long *pulSrcStart, *pulSrcCnt, *pulSrcEnd;
  unsigned int uiElements;

  /* check instance number */
  if( uiPort>=NUM_XPECS ) return -1;

  /* get the start and end address of the ram area, get the physical address */
  switch( eXcType )
  {
    case NX50_XC_TYPE_XPEC:
      pulRamStart = (volatile unsigned long*) s_ptXpecStart[uiPort];
      pulRamEnd = pulRamStart + (sizeof(NX50_XPEC_AREA_T) / sizeof(unsigned long));
      ulPhysicalStart = (unsigned long) s_ptXpecStart[uiPort];
      break;

    case NX50_XC_TYPE_RPU:
      /* fall through */

    case NX50_XC_TYPE_TPU:
      pulRamStart = (volatile unsigned long*)s_ptXmacStart[uiPort];
      pulRamEnd = pulRamStart + (sizeof(NX50_XMAC_AREA_T)/sizeof(unsigned long));
      ulPhysicalStart = (unsigned long) s_ptXmacStart[uiPort];
      break;

    default: return -1; /* unknown unit type */
  }

  /* get the number of code elements */
  uiElements = pulXcPrg[0] / sizeof(unsigned long) - 1;

  /* get the pointer in the xc area */
  /* ram_virtual_start + code_physical_start - ram_physical_start */
  pulDst = pulRamStart + (pulXcPrg[2] - ulPhysicalStart) / sizeof(unsigned long);

  /* the code must fit into the ram area */
  if( (pulDst<pulRamStart) || ((pulDst+uiElements)>pulRamEnd) ) {
    /* the code exceeds the xpec ram! */
    return -1;
  }

  /* get source start and end pointer */
  pulSrcStart = pulXcPrg + 3;
  pulSrcEnd = pulSrcStart + uiElements;

  /* copy the code to xc ram */
  pulSrcCnt = pulSrcStart;
  pulDstCnt = pulDst;
  while( pulSrcCnt<pulSrcEnd ) {
    *pulDstCnt = *pulSrcCnt;
    pulDstCnt++;
    pulSrcCnt++;
  }

  /* compare the code */
  pulSrcCnt = pulSrcStart;
  pulDstCnt = pulDst;
  while( pulSrcCnt<pulSrcEnd ) {
    if( *pulDstCnt != *pulSrcCnt ) return -1;
    pulDstCnt++;
    pulSrcCnt++;
  }

  /* get the number of trailing loads */
  uiElements = pulXcPrg[1] / sizeof(unsigned long);

  /* get source start and end pointer */
  pulSrcCnt = pulXcPrg + 2 + pulXcPrg[0] / sizeof(unsigned long);
  pulSrcEnd = pulSrcCnt + uiElements;

  /* write all trailing loads */
  while( pulSrcCnt<pulSrcEnd ) {
    /* get the destination address ( ram_virtual_start + data_physical_start - ram_physical_start) */
    pulDst = pulRamStart + (*pulSrcCnt - ulPhysicalStart) / sizeof(unsigned long);
    pulSrcCnt++;

    /* check the destination address */
    if( (pulDst<pulRamStart) || (pulDst>=pulRamEnd) ) return -1;

    /* write the data */
    *pulDst = *pulSrcCnt;
    pulSrcCnt++;
  }

  return 0;
}

/*****************************************************************************/
/*! Start XC Port
* \description
*   Start XC port.
* \class
*   XC
* \params
*   uiPort               [in]  XC Port Number
*   pvUser               [in]  User specific parameter
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX50_XC_Start( unsigned int uiPort, void* pvUser )
{
  NX50_XMAC_AREA_T* ptXmac;
  NX50_XPEC_AREA_T* ptXpec;

  /* check instance number */
  if( uiPort>=NUM_XPECS ) return -1;

  ptXmac = s_ptXmacStart[uiPort];
  ptXpec = s_ptXpecStart[uiPort];

  /* reset pc of units */
  ptXmac->ulXmac_rpu_pc = 0;
  ptXmac->ulXmac_tpu_pc = 0;
  ptXpec->ulXpec_pc     = 0x7ff;

  /* start units */
  ptXmac->ulXmac_tpu_hold_pc = 0;
  ptXmac->ulXmac_rpu_hold_pc = 0;
  ptXpec->ulXpu_hold_pc      = 0;

  return 0;
}

/*****************************************************************************/
/*! Start XC Units
* \description
*   Start XC Units.
* \class
*   XC
* \params
*   uiPort               [in]  XC Port Number
*   uiUnitVec            [in]  Bit vector defining the units
*                              bit 0: rPU, bit 1: tPU, bit 2: xPEC
*   pvUser               [in]  User specific parameter
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX50_XC_StartUnits(unsigned int uiPort, unsigned int uiUnitVec, void* pvUser)
{
  NX50_XMAC_AREA_T* ptXmac;
  NX50_XPEC_AREA_T* ptXpec;

  /* check instance number */
  if( uiPort >= NUM_XPECS ) return -1;

  ptXmac = s_ptXmacStart[uiPort];
  ptXpec = s_ptXpecStart[uiPort];

  if( 0 != (uiUnitVec & MSK_NX50_XC_TYPE_RPU) )
  {
    ptXmac->ulXmac_rpu_hold_pc = 0;
  }

  if( 0 != (uiUnitVec & MSK_NX50_XC_TYPE_TPU) )
  {
    ptXmac->ulXmac_tpu_hold_pc = 0;
  }

  if( 0 != (uiUnitVec & MSK_NX50_XC_TYPE_XPEC) )
  {
    ptXpec->ulXpu_hold_pc      = 0;
  }

  return 0;
}


/*
  __  __   ___   ___   __  __   _   _
 |  \/  | |_ _| |_ _| |  \/  | | | | |
 | |\/| |  | |   | |  | |\/| | | | | |
 | |  | |  | |   | |  | |  | | | |_| |
 |_|  |_| |___| |___| |_|  |_|  \___/

*/

/*****************************************************************************/
/*! Read PHY Register
* \description
*   Reads PHY register over MDIO.
* \class
*   MIIMU
* \params
*   uMiimuPreamble              [in]  Miimu Preamble
*   uMiimuMdcFreq               [in]  Miimu Mdc Frequence
*   uMiimuRtaField              [in]  Miimu Rta Field
*   uMiimuPhyAddr               [in]  Miimu PHY Address
*   uMiimuReqAddr               [in]  Miimu Register Address
*   pusData                     [out] Miimu Data
*   pvUser                      [in]  User specific parameter
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX50_MIIMU_ReadPhyReg( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuRtaField, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, unsigned short* pusData, void* pvUser)
{

  NX50_MIIMU_REG_T tMiiMuReg = {0};

  if( (uMiimuPhyAddr>31) || (uMiimuReqAddr>31) ) return -1; // invalid phy register number
  else {
    tMiiMuReg.bf.miimu_snrdy      = 1;
    tMiiMuReg.bf.phy_nres         = 1;
    tMiiMuReg.bf.miimu_preamble   = uMiimuPreamble;
    tMiiMuReg.bf.miimu_mdc_period = uMiimuMdcFreq;
    tMiiMuReg.bf.miimu_rta        = uMiimuRtaField;
    tMiiMuReg.bf.miimu_regaddr    = uMiimuReqAddr;
    tMiiMuReg.bf.miimu_phyaddr    = uMiimuPhyAddr;

    // write command to PHY
    s_ptMiimu->ulMiimu_reg = tMiiMuReg.val;

    // wait until ready
    do {
      tMiiMuReg.val = s_ptMiimu->ulMiimu_reg;
    } while( tMiiMuReg.bf.miimu_snrdy );

    *pusData = (unsigned short) tMiiMuReg.bf.miimu_data;
  }

  return 0;
}

/*****************************************************************************/
/*! Write PHY Register
* \description
*   Write PHY register over MDIO.
* \class
*   MIIMU
* \params
*   uMiimuPreamble              [in]  Miimu Preamble
*   uMiimuMdcFreq               [in]  Miimu Mdc Frequence
*   uMiimuPhyAddr               [in]  Miimu PHY Address
*   uMiimuReqAddr               [in]  Miimu Register Address
*   usData                      [in]  Miimu Data
*   pvUser                      [in]  User specific parameter
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX50_MIIMU_WritePhyReg( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, unsigned short usData, void* pvUser)
{
  NX50_MIIMU_REG_T tMiiMuReg = {0};

  if( (uMiimuPhyAddr>31) || (uMiimuReqAddr>31) ) return -1; // invalid phy register number
  else {
    tMiiMuReg.bf.miimu_snrdy        = 1;
    tMiiMuReg.bf.phy_nres           = 1;
    tMiiMuReg.bf.miimu_opmode       = 1;
    tMiiMuReg.bf.miimu_preamble     = uMiimuPreamble;
    tMiiMuReg.bf.miimu_mdc_period   = uMiimuMdcFreq;
    tMiiMuReg.bf.miimu_regaddr      = uMiimuReqAddr;
    tMiiMuReg.bf.miimu_phyaddr      = uMiimuPhyAddr;
    tMiiMuReg.bf.miimu_data         = usData;

    // write command to PHY
    s_ptMiimu->ulMiimu_reg = tMiiMuReg.val;

    // wait until ready
    do {
      tMiiMuReg.val = s_ptMiimu->ulMiimu_reg;
    } while( tMiiMuReg.bf.miimu_snrdy );
  }

  return 0;
}



/*
  ____                        _              ____   _            _
 |  _ \    ___   ___    ___  | |_           / ___| | |_   _ __  | |
 | |_) |  / _ \ / __|  / _ \ | __|         | |     | __| | '__| | |
 |  _ <  |  __/ \__ \ |  __/ | |_          | |___  | |_  | |    | |
 |_| \_\  \___| |___/  \___|  \__|  _____   \____|  \__| |_|    |_|
                                   |_____|

*/

/*****************************************************************************/
/*! RESET_CTRL Set Config
* \description
*   Configures the Reset Control value.
* \class
*   RESET_CTRL
* \params
*   ulRstCtrlVal      [in]  Reset Control Mask
* \return
*                                                                            */
/*****************************************************************************/
void NX50_RSTCTRL_SetCfg( unsigned long ulRstCtrlVal )
{
  // read access key, write back access key
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;

  // write config
  s_ptAsicCtrl->ulReset_ctrl = ulRstCtrlVal;
}



/*****************************************************************************/
/*! RESET_CTRL Get Config
* \description
*   return current value of Reset Control Register.
* \class
*   RESET_CTRL
* \params
* \return
*  current Reset Control Mask                                                */
/*****************************************************************************/
unsigned long NX50_RSTCTRL_GetCfg( void )
{
  // return current config
  return s_ptAsicCtrl->ulReset_ctrl;
}

/*
  ____                  _     _
 / ___|   _   _   ___  | |_  (_)  _ __ ___     ___
 \___ \  | | | | / __| | __| | | | '_ ` _ \   / _ \
  ___) | | |_| | \__ \ | |_  | | | | | | | | |  __/
 |____/   \__, | |___/  \__| |_| |_| |_| |_|  \___|
          |___/

*/

/*****************************************************************************/
/*! Set Systime Border
* \description
*   Set the systime border.
* \class
*   SYSTIME
* \params
*   ulBorder              [in]  Systime Border Value
*   pvUser                [in]  User specific parameter
* \return
*                                                                            */
/*****************************************************************************/
void NX50_SYSTIME_SetBorder( unsigned long ulBorder, void* pvUser )
{
  s_ptSystime->ulSystime_border = ulBorder;
}

/*****************************************************************************/
/*! Set Systime Speed
* \description
*   Set the systime speed.
* \class
*   SYSTIME
* \params
*   ulSpeed               [in]  Systime Speed Value
*   pvUser                [in]  User specific parameter
* \return
*                                                                            */
/*****************************************************************************/
void NX50_SYSTIME_SetSpeed( unsigned long ulSpeed, void* pvUser )
{
  s_ptSystime->ulSystime_count_value = ulSpeed;
}

/*****************************************************************************/
/*! Set Systime
* \description
*   Sets the systime.
* \class
*   SYSTIME
* \params
*   ulSystime_s           [in]  Systime Value in Seconds
*   ulSystime_ns          [in]  Systime Value in Nanoseconds
*   pvUser                [in]  User specific parameter
* \return
*                                                                            */
/*****************************************************************************/
void NX50_SYSTIME_SetSystime( unsigned long ulSystime_s, unsigned long ulSystime_ns, void* pvUser )
{
  s_ptSystime->ulSystime_s = ulSystime_s;
  s_ptSystime->ulSystime_ns = ulSystime_ns;
}

/*****************************************************************************/
/*! Get Systime
* \description
*   Read the systime.
* \class
*   SYSTIME
* \params
*   pulSystime_s          [in]  Pointer of Systime Value in Seconds
*   pulSystime_ns         [in]  Pointer of Systime Value in Nanoseconds
*   pvUser                [in]  User specific parameter
* \return
*                                                                            */
/*****************************************************************************/
void NX50_SYSTIME_GetSystime( unsigned long* pulSystime_s, unsigned long* pulSystime_ns, void* pvUser )
{
  *pulSystime_s  = s_ptSystime->ulSystime_s;
  *pulSystime_ns = s_ptSystime->ulSystime_ns;
}
