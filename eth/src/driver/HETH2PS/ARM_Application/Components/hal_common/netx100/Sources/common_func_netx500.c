#include <string.h> // memset
#include <stdarg.h> // uprintf
#include "regdef_netx500.h" // structures
#include "common_func_netx500.h" // function prototypes and defines

#define MSK_NX500_DPM_ARM_IO_MODE0_PIO_MODE    0xffffffffU
#define SRT_NX500_DPM_ARM_IO_MODE0_PIO_MODE    0

#define MSK_NX500_DPM_ARM_IO_MODE1_PIO_MODE    0x001fffffU
#define SRT_NX500_DPM_ARM_IO_MODE1_PIO_MODE    0
#define MSK_NX500_DPM_ARM_IO_MODE1_IN_CONTROL  0xc0000000U
#define SRT_NX500_DPM_ARM_IO_MODE1_IN_CONTROL  30


static NX500_ASIC_CTRL_AREA_T*          const s_ptAsicCtrl = (NX500_ASIC_CTRL_AREA_T*) Addr_NX500_asic_ctrl;
static NX500_PIO_AREA_T*                const s_ptPio =      (NX500_PIO_AREA_T*) Addr_NX500_pio;
static NX500_GPIO_AREA_T*               const s_ptGpio =     (NX500_GPIO_AREA_T*) Addr_NX500_gpio;
static NX500_POINTER_FIFO_AREA_T*       const s_ptPFifo =    (NX500_POINTER_FIFO_AREA_T*) Addr_NX500_pointer_fifo;
static NX500_VIC_AREA_T*                const s_ptVic =      (NX500_VIC_AREA_T*) Addr_NX500_vic;
static NX500_XPEC_IRQ_REGISTERS_AREA_T* const s_ptXpecIrq =  (NX500_XPEC_IRQ_REGISTERS_AREA_T*) Addr_NX500_xpec_irq_registers;
static NX500_NETX_CONTROLLED_GLOBAL_REGISTER_BLOCK_1_AREA_T * const s_ptNetXGlobalRegBlock1Area = (NX500_NETX_CONTROLLED_GLOBAL_REGISTER_BLOCK_1_AREA_T*) Addr_NX500_netx_controlled_global_register_block_1;
static NX500_NETX_CONTROLLED_GLOBAL_REGISTER_BLOCK_2_AREA_T * const s_ptNetXGlobalRegBlock2Area = (NX500_NETX_CONTROLLED_GLOBAL_REGISTER_BLOCK_2_AREA_T*) Addr_NX500_netx_controlled_global_register_block_2;

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
  Addr_NX500_uart0,
  Addr_NX500_uart1,
  Addr_NX500_uart2
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
void NX500_UART_InitSerial( unsigned int uiPort )
{
  unsigned int ulUartBaseAdr;

  // get uart base adr
  ulUartBaseAdr = aUartBaseAdr[uiPort];

  // switch gpio 0 - 3 mux to uart
  POKE( Adr_NX500_gpio_cfg0+(uiPort<<4), NX500_GPIO_CFG_EXT_0 );
  POKE( Adr_NX500_gpio_cfg1+(uiPort<<4), NX500_GPIO_CFG_EXT_0 );
  POKE( Adr_NX500_gpio_cfg2+(uiPort<<4), NX500_GPIO_CFG_EXT_0 );
  POKE( Adr_NX500_gpio_cfg3+(uiPort<<4), NX500_GPIO_CFG_EXT_0 );

  // First of all disable everything
  POKE(ulUartBaseAdr+REL_Adr_NX500_uartcr, 0);

  // Set the bit for the second baudrate mode
  POKE(ulUartBaseAdr+REL_Adr_NX500_uartcr_2, MSK_NX500_uartcr_2_Baud_Rate_Mode);

  // Adjust the baudrate register
  POKE(ulUartBaseAdr+REL_Adr_NX500_uartlcr_l, NX500_DEV_BAUDRATE_DIV_LO);
  POKE(ulUartBaseAdr+REL_Adr_NX500_uartlcr_m, NX500_DEV_BAUDRATE_DIV_HI);

  // set UART to 8 bits, 1 stop bit, no parity, fifo enabled
  POKE( ulUartBaseAdr+REL_Adr_NX500_uartlcr_h, (MSK_NX500_uartlcr_h_WLEN | MSK_NX500_uartlcr_h_FEN));
  // Set TX-Driver to enabled
  POKE( ulUartBaseAdr+REL_Adr_NX500_uartdrvout, MSK_NX500_uartdrvout_DRVTX);
  // Finally enable the UART
  POKE( ulUartBaseAdr+REL_Adr_NX500_uartcr, MSK_NX500_uartcr_uartEN);
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
void NX500_UART_CloseSerial(unsigned int uiPort)
{
   unsigned int ulUartBaseAdr;
   unsigned int uCnt;

   // get uart base adr
   ulUartBaseAdr = aUartBaseAdr[uiPort];

   while((PEEK(ulUartBaseAdr+REL_Adr_NX500_uartfr) & MSK_NX500_uartfr_BUSY)!=0); /* UART busy ? */
   POKE(ulUartBaseAdr+REL_Adr_NX500_uartcr,0);   /* First of all disable everything */
   POKE(ulUartBaseAdr+REL_Adr_NX500_uartlcr_m,0); /* Adjust the baudrate register */
   POKE(ulUartBaseAdr+REL_Adr_NX500_uartlcr_l,0);
   POKE(ulUartBaseAdr+REL_Adr_NX500_uartlcr_h,0);  /* UART to be 8 bits, 1 stop bit, no parity, fifo enabled */
   POKE(ulUartBaseAdr+REL_Adr_NX500_uartdrvout,0); /* Set TX-Driver to disabled */
   for(uCnt=0;uCnt<5;uCnt++) {
     PEEK(ulUartBaseAdr+REL_Adr_NX500_uartdr); /* Read out the reception register if there are still bytes in it */
   }
   // reset gpio mux to input
   POKE(Adr_NX500_gpio_cfg0+(uiPort<<4), NX500_GPIO_CFG_IN);
   POKE(Adr_NX500_gpio_cfg1+(uiPort<<4), NX500_GPIO_CFG_IN);
   POKE(Adr_NX500_gpio_cfg2+(uiPort<<4), NX500_GPIO_CFG_IN);
   POKE(Adr_NX500_gpio_cfg3+(uiPort<<4), NX500_GPIO_CFG_IN);
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
int NX500_UART_PutCharacter(unsigned int uiPort, int iChar)
{
  unsigned int ulUartBaseAdr;

  // get uart base adr
  ulUartBaseAdr = aUartBaseAdr[uiPort];

  // Wait until there is space in the FIFO
  while( (PEEK(ulUartBaseAdr+REL_Adr_NX500_uartfr)&MSK_NX500_uartfr_TXFF)!=0 ) {};
  POKE( ulUartBaseAdr+REL_Adr_NX500_uartdr, iChar);

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
int NX500_UART_GetCharacter(unsigned int uiPort)
{
  unsigned int uDat;
  unsigned int ulUartBaseAdr;


  // get uart base adr
  ulUartBaseAdr = aUartBaseAdr[uiPort];

  // We need to wait a defined time for a character
  while((PEEK(ulUartBaseAdr+REL_Adr_NX500_uartfr)&MSK_NX500_uartfr_RXFE)!=0);  /* wait for interrupt */
     // Wait infinte for new data in the FIFO

  uDat = PEEK(ulUartBaseAdr+REL_Adr_NX500_uartdr);  // Get the received byte
  if( (uDat&0xFFFFFF00)!=0 ) { // we had a receive error
    POKE(ulUartBaseAdr+REL_Adr_NX500_uartrsr, 0); // Clear the error
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
*   ulUartNr              [in]  Number of UART
* \return
*   0
*   1                                                                        */
/*****************************************************************************/
int NX500_UART_Peek(unsigned int uiPort)
{
  unsigned int ulUartBaseAdr;

  // get uart base adr
  ulUartBaseAdr = aUartBaseAdr[uiPort];
  return ( (PEEK(ulUartBaseAdr+REL_Adr_NX500_uartfr)&MSK_NX500_uartfr_RXFE)==0 );
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
void NX500_VIC_InstallIsrVector( pp_func ulVector )
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
  pulArm9_irq_vector = (volatile unsigned long*) ARM926EJS_Vect_IRQ;
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
void INTERRUPT NX500_VIC_IRQVectorHandler_C( void )
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
void NX500_VIC_ClearVic( void )
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
void NX500_VIC_ISR_Default( void )
{
  /*
  unsigned int uIrqSource;
  uIrqSource = s_ptVic->ulVic_irq_status;
  */

  /* default handling ... */
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
void NX500_VIC_SetDefaultVector( pp_func default_handler )
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
void NX500_VIC_SetIrqVector( unsigned int vec, unsigned long src, pp_func irq_handler )
{
  /* No action, if vector number out of range */
  if (vec < MAX_IRQ_VECTORS) {
    s_ptVic->aulVic_vect_addr[vec]  = (unsigned long) irq_handler;
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
void NX500_VIC_EnableIrqSources( unsigned long src )
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
void NX500_GPIO_SetupMode( unsigned long ulGpioNum, unsigned long ulMode, unsigned long ulInvert )
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
void NX500_GPIO_SetOutput( unsigned long ulGpioNum, int fEnable )
{
  if( fEnable )
    s_ptGpio->ulGpio_line |= 1UL << ulGpioNum;
  else
    s_ptGpio->ulGpio_line &= ~(1UL << ulGpioNum);
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
unsigned long NX500_GPIO_GetLine( void )
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
void NX500_GPIO_SetLine( unsigned long ulVal )
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
int NX500_GPIO_GetInput( unsigned long ulGpioNum )
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
unsigned long NX500_GPIO_GetIn( void )
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
void NX500_GPIO_IrqEnable( unsigned long ulGpioNum )
{
  s_ptGpio->ulGpio_irq_mask_set |= 1UL << ulGpioNum;
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
int NX500_GPIO_GetIrq( unsigned long ulGpioNum )
{
  return (s_ptGpio->ulGpio_irq & (1UL << ulGpioNum)) ? 1 : 0;
}

/*****************************************************************************/
/*! GPIO Irq Reset
* \description
*   Writes 1 to reset the irq of the selected GPIO.
* \class
*   GPIO
* \params
*   ulGpioNum          [in]   Selected GPIO
* \return
*                                                                            */
/*****************************************************************************/
void NX500_GPIO_IrqReset( unsigned long ulGpioNum )
{
  s_ptGpio->ulGpio_irq = 1UL << ulGpioNum;
}

/*****************************************************************************/
/*! GPIO Sleep
* \description
*   Sleep a while using a GPIO timer.
* \class
*   GPIO
* \params
*   ulGpioNum          [in]   Selected GPIO
*   uiTimeout          [out]  The Time to wait in �s
* \return
*                                                                            */
/*****************************************************************************/
void NX500_GPIO_Sleep( unsigned long ulCounter, unsigned int uiTimeout )
{
  unsigned int uiVal;

  /* Convert counter value from �s to ns */
  uiTimeout = uiTimeout * (NX500_DEV_FREQUENCY/1000000);

  s_ptGpio->aulGpio_counter_ctrl[ulCounter]  = 0;          /* Clear the timer register         */
  s_ptGpio->aulGpio_counter_cnt[ulCounter]   = 0;          /* Clear the current counter value  */
  s_ptGpio->aulGpio_counter_max[ulCounter]   = uiTimeout;  /* Set the counter value            */
  s_ptGpio->aulGpio_counter_ctrl[ulCounter] |= (MSK_NX500_gpio_counter0_ctrl_run | MSK_NX500_gpio_counter0_ctrl_once  ); /* Enable the timer to one shot */

  /* poll timer ctrl for 'run' bit */
  do {
    uiVal  = s_ptGpio->aulGpio_counter_ctrl[ulCounter] & MSK_NX500_gpio_counter0_ctrl_run;
  } while ( uiVal!=0 );
}

/*****************************************************************************/
/*! GPIO Setup Timer
* \description
*   Setup a GPIO timer.
* \class
*   GPIO
* \params
*   ulGpioNum          [in]   Selected GPIO
*   uiTimeout          [out]  The Time to wait in �s
* \return
*                                                                            */
/*****************************************************************************/
void NX500_GPIO_SetupTimer( unsigned long ulCounter, unsigned int uiTimeout )
{
  /* Convert counter value from �s to ns */
  uiTimeout = uiTimeout * (NX500_DEV_FREQUENCY/1000000);

  s_ptGpio->aulGpio_counter_ctrl[ulCounter]  = 0;          /* Clear the timer register         */
  s_ptGpio->aulGpio_counter_cnt[ulCounter]   = 0;          /* Clear the current counter value  */
  s_ptGpio->aulGpio_counter_max[ulCounter]   = uiTimeout;  /* Set the counter value            */
  s_ptGpio->aulGpio_counter_ctrl[ulCounter] |= (MSK_NX500_gpio_counter0_ctrl_run | MSK_NX500_gpio_counter0_ctrl_once  ); /* Enable the timer to one shot */

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
int NX500_GPIO_IsTimerRunning( unsigned long ulCounter )
{
  /* gets timer ctrl 'run' bit */
  if( (s_ptGpio->aulGpio_counter_ctrl[ulCounter] & MSK_NX500_gpio_counter0_ctrl_run) == 0 )
    return 0;
  else
    return 1;
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
void NX500_GPIO_PrepareTimer( unsigned int uCounter, unsigned int uiTimeout, unsigned int uiOnce )
{
  s_ptGpio->aulGpio_counter_ctrl[uCounter]  = 0;          /* Clear the timer register         */
  s_ptGpio->aulGpio_counter_cnt[uCounter]   = 0;          /* Clear the current counter value  */
  s_ptGpio->aulGpio_counter_max[uCounter]   = uiTimeout;  /* Set the counter value            */
  s_ptGpio->aulGpio_counter_ctrl[uCounter]  |= (uiOnce << SRT_NX500_gpio_counter0_ctrl_once);
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
void NX500_GPIO_StartTimer( unsigned int uCounter )
{
  s_ptGpio->aulGpio_counter_ctrl[uCounter] |= MSK_NX500_gpio_counter0_ctrl_run; /* Enable the timer */
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
void NX500_GPIO_ResetTimer( unsigned int uCounter )
{
  s_ptGpio->aulGpio_counter_ctrl[uCounter] = 0;
  s_ptGpio->aulGpio_counter_cnt[uCounter]  = 0;
  s_ptGpio->aulGpio_counter_max[uCounter]  = 0;
  s_ptGpio->ulGpio_irq_mask_rst            = 1UL << (uCounter+16);
  s_ptGpio->ulGpio_irq                     = 1UL << (uCounter+16);
}

/*****************************************************************************/
/*! GPIO Get Timer Interrupt
* \description
*   Read interrupt status of the selected GPIO Timer.
* \class
*   GPIO
* \params
*   uCounter          [in]   Selected Counter
* \return
*   Current counter_irq_masked value                                         */
/*****************************************************************************/
unsigned int NX500_GPIO_GetTimerIrq( unsigned int uCounter )
{
  return (s_ptGpio->ulGpio_irq & (1UL << (uCounter+16))) ? 1 : 0;
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
void NX500_GPIO_EnableTimerIrq( unsigned int uCounter )
{
  s_ptGpio->aulGpio_counter_ctrl[uCounter]  |= MSK_NX500_gpio_counter0_ctrl_irq_en;
  s_ptGpio->ulGpio_irq_mask_set |= 1UL << (uCounter+16);
}

/*****************************************************************************/
/*! GPIO Timer Irq Disable
* \description
*   Disable interrupt request for the selected Counter..
* \class
*   GPIO
* \params
*   uCounter          [in]   Selected Counter
* \return
*                                                                            */
/*****************************************************************************/
void NX500_GPIO_DisableTimerIrq( unsigned int uCounter )
{
  s_ptGpio->aulGpio_counter_ctrl[uCounter]  &= ~MSK_NX500_gpio_counter0_ctrl_irq_en;
  s_ptGpio->ulGpio_irq_mask_rst = 1UL << (uCounter+16);
}

/*****************************************************************************/
/*! GPIO Timer Interrupt Reset
* \description
*   Writes 1 to reset the interrupt of the selected Counter.
* \class
*   GPIO
* \params
*   uCounter          [in]   Selected Counter
* \return
*                                                                            */
/*****************************************************************************/
void NX500_GPIO_ConfirmTimerIrq( unsigned long ulCounter )
{
  s_ptGpio->ulGpio_irq = 1UL << (ulCounter+16);
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
void NX500_PIO_SetMode( unsigned long ulPioNum, unsigned long ulMode )
{
  if(ulPioNum < 32)
  {
    if( NX500_PIO_MODE_OUT == ulMode )
      s_ptPio->ulPio_oe |= 1UL << ulPioNum;
    else
      s_ptPio->ulPio_oe &= ~(1UL << ulPioNum);
  }
  else if(ulPioNum < 64)
  {
    if( NX500_PIO_MODE_OUT == ulMode )
      s_ptNetXGlobalRegBlock2Area->ulIo_reg_drv_en0 |= 1UL << (ulPioNum - 32);
    else
      s_ptNetXGlobalRegBlock2Area->ulIo_reg_drv_en0 &= ~(1UL << (ulPioNum - 32));
  }
  else if(ulPioNum < 85)
  {
    if( NX500_PIO_MODE_OUT == ulMode )
      s_ptNetXGlobalRegBlock2Area->ulIo_reg_drv_en1 |= 1UL << (ulPioNum - 64);
    else
      s_ptNetXGlobalRegBlock2Area->ulIo_reg_drv_en1 &= ~(1UL << (ulPioNum - 64));
  }
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
int NX500_PIO_GetInput( unsigned long ulPioNum )
{
  return ( s_ptPio->ulPio_in & (1UL << ulPioNum)) ? 1 : 0;
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
unsigned long NX500_PIO_GetIn( void )
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
int NX500_PIO_GetOutput( unsigned long ulPioNum )
{
  return ( s_ptPio->ulPio_out & (1UL << ulPioNum)) ? 1 : 0;
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
void NX500_PIO_SetOutput( unsigned long ulPioNum, int fEnable )
{
  if(fEnable) {
    s_ptPio->ulPio_out |= 1UL << ulPioNum;
  } else {
    s_ptPio->ulPio_out &= ~(1UL << ulPioNum);
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
unsigned long NX500_PIO_GetLine( void )
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
void NX500_PIO_SetLine( unsigned long ulVal )
{
  s_ptPio->ulPio_out = ulVal;
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
void NX500_HIF_SetIoMode( unsigned long ulHifModeEnable32to63, unsigned long ulHifModeEnable64to84 )
{
  ulHifModeEnable64to84 &= MSK_NX500_DPM_ARM_IO_MODE1_PIO_MODE;
  s_ptNetXGlobalRegBlock2Area->ulIo_reg_mode1 = ulHifModeEnable64to84 & MSK_NX500_DPM_ARM_IO_MODE1_PIO_MODE ;
  s_ptNetXGlobalRegBlock2Area->ulIo_reg_mode0 = ulHifModeEnable32to63;
}

/*****************************************************************************/
/*! HIF Get Pio Input
* \description
*   Reads the input values of PIO 32..84.
* \class
*   HIF
* \params
*   pulPio32to63 [out]  Bit n: Input value of PIO n+32 (n = 0..31)
*   pulPio64to84 [out]  Bit n: Input value of PIO n+64 (n = 0..20)
* \return
*                                                                            */
/*****************************************************************************/
void NX500_HIF_GetPioIn( unsigned long* pulPio32to63, unsigned long* pulPio64to84 )
{
  unsigned long ulIoMode = s_ptNetXGlobalRegBlock2Area->ulIo_reg_mode1 & MSK_NX500_DPM_ARM_IO_MODE1_PIO_MODE;
  s_ptNetXGlobalRegBlock2Area->ulIo_reg_mode1 = ulIoMode | (1UL << SRT_NX500_DPM_ARM_IO_MODE1_IN_CONTROL);
  *pulPio32to63 = s_ptNetXGlobalRegBlock2Area->ulIo_reg_data0;
  *pulPio64to84 = s_ptNetXGlobalRegBlock2Area->ulIo_reg_mode1;
}

/*****************************************************************************/
/*! HIF Set Pio Outputs
* \description
*   Reads the input values of PIO 32..84.
* \class
*   HIF
* \params
*   pulPio32to63 [out]  Bit n: Input value of PIO n+32 (n = 0..31)
*   pulPio64to84 [out]  Bit n: Input value of PIO n+64 (n = 0..20)
* \return
*                                                                            */
/*****************************************************************************/
void NX500_HIF_SetPioLine( unsigned long ulPio32to63, unsigned long ulPio64to84 )
{
  s_ptNetXGlobalRegBlock2Area->ulIo_reg_data0 = ulPio32to63;
  s_ptNetXGlobalRegBlock2Area->ulIo_reg_mode1 = ulPio64to84;
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
void NX500_RDYRUN_SetRdyRunLed( NX500_RDYRUN_LED_E tMode )
{
  switch( tMode ) {
  case NX500_RDYRUN_LED_OFF:
    s_ptNetXGlobalRegBlock1Area->ulSta_netx = 0x030c0000UL;
    break;
  case NX500_RDYRUN_LED_GREEN:
    s_ptNetXGlobalRegBlock1Area->ulSta_netx = 0x030c0002UL;
    break;
  case NX500_RDYRUN_LED_RED:
    s_ptNetXGlobalRegBlock1Area->ulSta_netx = 0x030c0001UL;
    break;
  case NX500_RDYRUN_LED_INV:
    s_ptNetXGlobalRegBlock1Area->ulSta_netx = ((s_ptNetXGlobalRegBlock1Area->ulSta_netx & 3UL) ^ 3UL) | 0x030c0000UL;
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
/*! Initialize Internal PHY
* \description
*   Initialize internal PHY.
* \class
*   INTPHY
* \params
*   ptPhyCtrlInit          [in]  Pointer of PHY Control Initialize Value
* \return
*                                                                            */
/*****************************************************************************/
void NX500_INTPHY_Init( NX500_PHY_CONTROL_T *ptPhyCtrlInit )
{
  unsigned short usMiimuData;
  unsigned int ulDelayCnt;
  unsigned int uPhyAdr = (ptPhyCtrlInit->bf.phy_address << 1);

#ifdef PHY_SIM_BYPASS
  ptPhyCtrlInit->bf.phy_sim_byp = PHY_SIM_BYPASS;
#else
  // clear bypass
  ptPhyCtrlInit->bf.phy_sim_byp = 0;
#endif

  // read access key, write back access key
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  // write value
  s_ptAsicCtrl->ulPhy_control = ptPhyCtrlInit->val | MSK_NX500_phy_control_phy_reset;

  // delay for 100us -> do 5 MII transfers of 24us each
  ulDelayCnt = 5;
  do
  {
    NX500_MIIMU_ReadPhyReg( NX500_MIIMU_SEND_PREAMBLE,
                            NX500_MIIMU_MDCFREQ_HIGH,
                            NX500_MIIMU_RTA_0_BITS,
                            NX500_INT_PHY1_ADDR,
                            DRV_CB12_CONTROL,
                            &usMiimuData,
                            0
                          );
  } while( ulDelayCnt--!=0 );

  // read access key, write back access key
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;

  // write value
  s_ptAsicCtrl->ulPhy_control = ptPhyCtrlInit->val;

  if(ptPhyCtrlInit->bf.phy_reset)
    return;

  /* wait for PHY 0 ready after reset if PHY is enabled */
  if( (ptPhyCtrlInit->val & MSK_NX500_phy_control_phy0_enable) != 0 )
  {
    do {
      NX500_MIIMU_ReadPhyReg( NX500_MIIMU_SEND_PREAMBLE,
                              NX500_MIIMU_MDCFREQ_HIGH,
                              NX500_MIIMU_RTA_0_BITS,
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
      NX500_INTPHY_CorrectAutoMdix100MBit(uPhyAdr);
    }
  }

  /* wait for PHY 1 ready after reset if PHY is enabled */
  if( (ptPhyCtrlInit->val & MSK_NX500_phy_control_phy1_enable) != 0 )
  {
    do {
      NX500_MIIMU_ReadPhyReg( NX500_MIIMU_SEND_PREAMBLE,
                              NX500_MIIMU_MDCFREQ_HIGH,
                              NX500_MIIMU_RTA_0_BITS,
                              uPhyAdr + 1,
                              DRV_CB12_SILICON_REVISION,
                              &usMiimuData,
                              0
                            );
    } while( (usMiimuData==0) || (usMiimuData==0xffff) );

    // Workaround sequence for correct Auto-MDIX function in forced 100 MBit-Mode
    if(   ptPhyCtrlInit->bf.phy1_automdix==PHY_CTRL_PHY2_AUTOMDIX
        && (   (ptPhyCtrlInit->bf.phy1_mode==PHYCTRL_100BASE_TXFX_HD_NOAUTONEG_CRSTXRX)
             ||(ptPhyCtrlInit->bf.phy1_mode==PHYCTRL_100BASE_TXFX_FD_NOAUTONEG_CRSRX)
           )
      )
    {
      NX500_INTPHY_CorrectAutoMdix100MBit(uPhyAdr + 1);
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
void NX500_INTPHY_CorrectAutoMdix100MBit( unsigned int uMiimuPhyAddr )
{
  NX500_MIIMU_WritePhyReg( NX500_MIIMU_SEND_PREAMBLE, NX500_MIIMU_MDCFREQ_HIGH, uMiimuPhyAddr, 0x14, 0x400, 0 );
  NX500_MIIMU_WritePhyReg( NX500_MIIMU_SEND_PREAMBLE, NX500_MIIMU_MDCFREQ_HIGH, uMiimuPhyAddr, 0x14, 0x0, 0 );
  NX500_MIIMU_WritePhyReg( NX500_MIIMU_SEND_PREAMBLE, NX500_MIIMU_MDCFREQ_HIGH, uMiimuPhyAddr, 0x14, 0x400, 0 );
  NX500_MIIMU_WritePhyReg( NX500_MIIMU_SEND_PREAMBLE, NX500_MIIMU_MDCFREQ_HIGH, uMiimuPhyAddr, 0x17, 0xa1, 0 );
  NX500_MIIMU_WritePhyReg( NX500_MIIMU_SEND_PREAMBLE, NX500_MIIMU_MDCFREQ_HIGH, uMiimuPhyAddr, 0x14, 0x441b, 0 );
  NX500_MIIMU_WritePhyReg( NX500_MIIMU_SEND_PREAMBLE, NX500_MIIMU_MDCFREQ_HIGH, uMiimuPhyAddr, 0x14, 0x0, 0 );
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
void NX500_PFIFO_Reset( void )
{
  unsigned int uCnt;

  // set reset flag of all fifos
  s_ptPFifo->ulPfifo_reset = 0xffffffff;

  // reset pointer fifo borders
  for( uCnt = 0; uCnt < 32; uCnt++ ) {
    s_ptPFifo->aulPfifo_border[uCnt] = ((uCnt+1)* 64)-1 ;
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
int NX500_PFIFO_SetBorders(const unsigned int *auiPFifoDepth)
{
  int iResult;
  unsigned int uiBorder;
  unsigned int uiFifoNum;

  /* set reset bit for all pointer FIFOs */
  s_ptPFifo->ulPfifo_reset = 0xffffffff;

  /* define pointer FIFO borders */
  uiBorder = 0;
  for(uiFifoNum=0; uiFifoNum<32; uiFifoNum++)
  {
    uiBorder += auiPFifoDepth[uiFifoNum];
    s_ptPFifo->aulPfifo_border[uiFifoNum] = uiBorder-1;
  }

  if( uiBorder>2048 ) {
    /* sum of all FIFO elements exceeds the limit */
    iResult = -1;
  } else {
    /* OK! */
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
int NX500_PFIFO_GetBorders(unsigned int *auiPFifoDepth)
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
unsigned long NX500_PFIFO_GetFillLevel( unsigned int uFifoNum )
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
unsigned long NX500_PFIFO_GetFifoResetVector( void )
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
unsigned long NX500_PFIFO_GetFifoFullVector( void )
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
unsigned long NX500_PFIFO_GetFifoEmptyVector( void )
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
unsigned long NX500_PFIFO_GetFifoOverflowVector( void )
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
unsigned long NX500_PFIFO_GetFifoUnderrunVector( void )
{
  return s_ptPFifo->ulPfifo_underrun;
}

/*
   __  __ __  __ _   _
  |  \/  |  \/  | | | |
  | |\/| | |\/| | | | |
  | |  | | |  | | |_| |
  |_|  |_|_|  |_|\___/

*/
#define CYG_MACRO_START do {
#define CYG_MACRO_END   } while (0)

#define MMU_Control_M   0x01
#define MMU_Control_C   0x04
#define MMU_Control_I   0x1000

#ifdef __thumb__
  #error "This module (MMU_Init.c) must be compiled in ARM mode!"
#endif

typedef struct MMU_MAPPING_TABLEtag
{
  unsigned int uiPhysicalAddr;
  unsigned int uiVirtualAddr;
  unsigned int uiSize;        // size in MB
  unsigned int uiCache;
  unsigned int uiBuffer;
  unsigned int uiAccess;
} MMU_MAPPING_TABLE;

/* ARM Translation Table Base Bit Masks */
#define ARM_TRANSLATION_TABLE_MASK               0xFFFFC000

/* ARM Domain Access Control Bit Masks */
#define ARM_ACCESS_TYPE_NO_ACCESS(domain_num)    (0x0 << ((domain_num)*2))
#define ARM_ACCESS_TYPE_CLIENT(domain_num)       (0x1 << ((domain_num)*2))
#define ARM_ACCESS_TYPE_MANAGER(domain_num)      (0x3 << ((domain_num)*2))

struct ARM_MMU_FIRST_LEVEL_FAULT {
    unsigned int id : 2;
    unsigned int sbz : 30;
};
#define ARM_MMU_FIRST_LEVEL_FAULT_ID 0x0

struct ARM_MMU_FIRST_LEVEL_PAGE_TABLE {
    unsigned int id : 2;
    unsigned int imp : 2;
    unsigned int domain : 4;
    unsigned int sbz : 1;
    unsigned int base_address : 23;
};
#define ARM_MMU_FIRST_LEVEL_PAGE_TABLE_ID 0x1

struct ARM_MMU_FIRST_LEVEL_SECTION {
    unsigned int id : 2;
    unsigned int b : 1;
    unsigned int c : 1;
    unsigned int imp : 1;
    unsigned int domain : 4;
    unsigned int sbz0 : 1;
    unsigned int ap : 2;
    unsigned int sbz1 : 8;
    unsigned int base_address : 12;
};
#define ARM_MMU_FIRST_LEVEL_SECTION_ID 0x2

struct ARM_MMU_FIRST_LEVEL_RESERVED {
    int id : 2;
    int sbz : 30;
};
#define ARM_MMU_FIRST_LEVEL_RESERVED_ID 0x3

#define ARM_MMU_FIRST_LEVEL_DESCRIPTOR_ADDRESS(ttb_base, table_index) \
   (unsigned long *)((unsigned long)(ttb_base) + ((table_index) << 2))

#define ARM_FIRST_LEVEL_PAGE_TABLE_SIZE 0x4000

#define ARM_MMU_SECTION(ttb_base, actual_base, virtual_base,              \
                        cacheable, bufferable, perm)                      \
    CYG_MACRO_START                                                       \
        register union ARM_MMU_FIRST_LEVEL_DESCRIPTOR desc;               \
                                                                          \
        desc.word = 0;                                                    \
        desc.section.id = ARM_MMU_FIRST_LEVEL_SECTION_ID;                 \
        desc.section.imp = 1;                                             \
        desc.section.domain = 0;                                          \
        desc.section.c = (cacheable);                                     \
        desc.section.b = (bufferable);                                    \
        desc.section.ap = (perm);                                         \
        desc.section.base_address = (actual_base);                        \
        *ARM_MMU_FIRST_LEVEL_DESCRIPTOR_ADDRESS(ttb_base, (virtual_base)) \
                            = desc.word;                                  \
    CYG_MACRO_END

#define X_ARM_MMU_SECTION(abase,vbase,size,cache,buff,access)      \
    { int i; int j = (abase); int k = (vbase);                     \
      for (i = size; i > 0 ; i--,j++,k++)                          \
      {                                                            \
        ARM_MMU_SECTION(ttb_base, j, k, cache, buff, access);      \
      }                                                            \
    }

union ARM_MMU_FIRST_LEVEL_DESCRIPTOR {
    unsigned long word;
    struct ARM_MMU_FIRST_LEVEL_FAULT fault;
    struct ARM_MMU_FIRST_LEVEL_PAGE_TABLE page_table;
    struct ARM_MMU_FIRST_LEVEL_SECTION section;
    struct ARM_MMU_FIRST_LEVEL_RESERVED reserved;
};

#define ARM_UNCACHEABLE                         0
#define ARM_CACHEABLE                           1
#define ARM_UNBUFFERABLE                        0
#define ARM_BUFFERABLE                          1

#define ARM_ACCESS_PERM_NONE_NONE               0
#define ARM_ACCESS_PERM_RO_NONE                 0
#define ARM_ACCESS_PERM_RO_RO                   0
#define ARM_ACCESS_PERM_RW_NONE                 1
#define ARM_ACCESS_PERM_RW_RO                   2
#define ARM_ACCESS_PERM_RW_RW                   3

static MMU_MAPPING_TABLE s_atMMUMapping[] =
{
  {Adr_NX500_intram0_base, Adr_NX500_intram0_base,  2,  ARM_UNCACHEABLE, ARM_UNBUFFERABLE, ARM_ACCESS_PERM_RW_RW},   // internal SRAM and Registers
  {Addr_NX500_sdram,       Addr_NX500_sdram,      128,  ARM_CACHEABLE,   ARM_BUFFERABLE,   ARM_ACCESS_PERM_RW_RW},   // SDRAM cached
  {Addr_NX500_sdram,       0xA0000000,            128,  ARM_UNCACHEABLE, ARM_UNBUFFERABLE, ARM_ACCESS_PERM_RW_RW},   // SDRAM uncached
  {Addr_NX500_extsram0,    Addr_NX500_extsram0,    64,  ARM_UNCACHEABLE, ARM_UNBUFFERABLE, ARM_ACCESS_PERM_RW_RW},   // flash bank 0 uncached
  {Addr_NX500_extsram1,    Addr_NX500_extsram1,    64,  ARM_UNCACHEABLE, ARM_UNBUFFERABLE, ARM_ACCESS_PERM_RW_RW},   // flash bank 1 uncached
  {Addr_NX500_extsram2,    Addr_NX500_extsram2,    64,  ARM_UNCACHEABLE, ARM_UNBUFFERABLE, ARM_ACCESS_PERM_RW_RW},   // flash bank 2 uncached
  {Addr_NX500_boot_rom,    Addr_NX500_boot_rom,     1,  ARM_UNCACHEABLE, ARM_UNBUFFERABLE, ARM_ACCESS_PERM_RW_RW},   // rom kernel uncached
  {Addr_NX500_backup_ram,  Addr_NX500_backup_ram,   1,  ARM_UNCACHEABLE, ARM_UNBUFFERABLE, ARM_ACCESS_PERM_RW_RW},   // backup RAM uncached
  {0x10000000,             0x10000000,              1,  ARM_UNCACHEABLE, ARM_UNBUFFERABLE, ARM_ACCESS_PERM_RW_RW},   // DTCM
  {0xFFF00000,             0xFFF00000,              1,  ARM_UNCACHEABLE, ARM_UNBUFFERABLE, ARM_ACCESS_PERM_RW_RW},   // mirrored VIC Registers
};


/*****************************************************************************/
/*! Init MMU
* \description
*   Initializes Memory Management Unit and enable Caches.
* \class
*   MMU
* \params
    pulTTBBase [in] Location of translation table base
* \return
*                                                                            */
/*****************************************************************************/
void NX500_MMU_Init( unsigned long* pulTTBBase )
{
  unsigned long ttb_base = (unsigned long)pulTTBBase;
  unsigned long ulTemp;

  /* Disable MMU and Caches first of all */
  asm volatile (                                                                    \
      "mrc    p15, 0, r0, c1, c0, 0;"   /* Get MMU Control Register content */      \
      "bic    r0, r0, #0x1000;"         /* MMU_Control_I*/                          \
      "bic    r0, r0, #0x0005;"         /* MMU_Control_M | MMU_Control_C */         \
      "mcr    p15, 0, r0, c1, c0, 0;"   /* disable MMU, ICache, DCache */           \
      "mov    r0,#0;"                                                               \
      "mcr    p15,0,r0,c7,c7,0;"        /* invalidate  i/d-cache */                 \
      "mcr    p15,0,r0,c8,c7,0;"        /* invalidate TLBs */                       \
      :                                                                             \
      :                                                                             \
      : "r0" /* clobber list */);

  /* Set the TTB register */
  asm volatile ("mcr  p15,0,%0,c2,c0,0" : : "r"(ttb_base) /*:*/);

  /* Set the Domain Access Control Register */
  ulTemp = ARM_ACCESS_TYPE_MANAGER(0)    |
           ARM_ACCESS_TYPE_NO_ACCESS(1)  |
           ARM_ACCESS_TYPE_NO_ACCESS(2)  |
           ARM_ACCESS_TYPE_NO_ACCESS(3)  |
           ARM_ACCESS_TYPE_NO_ACCESS(4)  |
           ARM_ACCESS_TYPE_NO_ACCESS(5)  |
           ARM_ACCESS_TYPE_NO_ACCESS(6)  |
           ARM_ACCESS_TYPE_NO_ACCESS(7)  |
           ARM_ACCESS_TYPE_NO_ACCESS(8)  |
           ARM_ACCESS_TYPE_NO_ACCESS(9)  |
           ARM_ACCESS_TYPE_NO_ACCESS(10) |
           ARM_ACCESS_TYPE_NO_ACCESS(11) |
           ARM_ACCESS_TYPE_NO_ACCESS(12) |
           ARM_ACCESS_TYPE_NO_ACCESS(13) |
           ARM_ACCESS_TYPE_NO_ACCESS(14) |
           ARM_ACCESS_TYPE_NO_ACCESS(15);
  asm volatile ("mcr  p15,0,%0,c3,c0,0" : : "r"(ulTemp) /*:*/);

  /* Clear all TTB entries - ie Set them to Faulting */
  memset(pulTTBBase, 0, ARM_FIRST_LEVEL_PAGE_TABLE_SIZE);

  /* Setup MMU Table */
  for(ulTemp = 0; ulTemp < sizeof(s_atMMUMapping) / sizeof(s_atMMUMapping[0]); ulTemp++)
  {
    X_ARM_MMU_SECTION(s_atMMUMapping[ulTemp].uiPhysicalAddr >> 20,
                      s_atMMUMapping[ulTemp].uiVirtualAddr  >> 20,
                      s_atMMUMapping[ulTemp].uiSize,
                      s_atMMUMapping[ulTemp].uiCache,
                      s_atMMUMapping[ulTemp].uiBuffer,
                      s_atMMUMapping[ulTemp].uiAccess);
  }

  /* Invalidate Caches, Activate Caches */
  asm volatile (                                                                      \
      "mov    r0,#0;"                                                                 \
      "mcr    p15,0,r0,c7,c7,0;"        /* invalidate  i/d-cache */                   \
      "mcr    p15,0,r0,c8,c7,0;"        /* invalidate TLBs */                         \
      "mrc    p15, 0, r1, c1, c0, 0;"   /* Get MMU Control Register content */        \
      "orr    r1, r1, #0x1000;"         /* MMU_Control_I*/                            \
      "orr    r1, r1, #0x0005;"         /* MMU_Control_M | MMU_Control_C */           \
      "ldr    r0, =VirtualStart;"                                                     \
      "cmp    r0, #0;"                  /* make sure no stall on "mov pc,r0" below */ \
      "mcr    p15, 0, r1, c1, c0, 0;"   /* enable MMU, ICache, DCache */              \
      "mov    pc, r0;"                  /* jump to new virtual address */             \
      "nop;"                                                                          \
      ".ltorg;"                                                                       \
      "VirtualStart:"                                                                 \
      :                                                                               \
      :                                                                               \
      : "r0","r1","memory" /* clobber list */);
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

#define NUM_XPECS 4

static const unsigned long XcCode_rpu_reset0[57] = {
  0x000000dc, // program size
  0x00000000, // trailing loads size
  0x00160000, 0x00f80001, 0x915fdb81, 0x00f40041, 0x915fdb82, 0x00dc0000, 0x001fdb83, 0x00f8fffd,
  0x914fe004, 0x00fbfffd, 0x915fdb85, 0x0107fffd, 0x915fdb86, 0x01080001, 0x915fdb87, 0x010bfffd,
  0x915fdb88, 0x01040001, 0x915fdb89, 0x01080001, 0x915fdb8a, 0x00f40001, 0x915fdb8b, 0x00f9fffd,
  0x915fdb8c, 0x00f8fffd, 0x915fdb8d, 0x00f87ffd, 0x915fdb8e, 0x00f83ffd, 0x915fdb8f, 0x00f81ffd,
  0x915fdb90, 0x00f80ffd, 0x915fdb91, 0x00f807fd, 0x915fdb92, 0x00f803fd, 0x915fdb93, 0x00f801fd,
  0x915fdb94, 0x00f800fd, 0x915fdb95, 0x00f8007d, 0x915fdb96, 0x00f8003d, 0x915fdb97, 0x00f8001d,
  0x915fdb98, 0x00f8000d, 0x915fdb99, 0x00f80005, 0x915fdb9a, 0x00dc0000, 0x001fdb9a,
  // trailing loads

};

static const unsigned long XcCode_rpu_reset1[57] = {
  0x000000dc, // program size
  0x00000000, // trailing loads size
  0x00161000, 0x00f80001, 0x915fdb81, 0x00f40041, 0x915fdb82, 0x00dc0000, 0x001fdb83, 0x00f8fffd,
  0x914fe004, 0x00fbfffd, 0x915fdb85, 0x0107fffd, 0x915fdb86, 0x01080001, 0x915fdb87, 0x010bfffd,
  0x915fdb88, 0x01040001, 0x915fdb89, 0x01080001, 0x915fdb8a, 0x00f40001, 0x915fdb8b, 0x00f9fffd,
  0x915fdb8c, 0x00f8fffd, 0x915fdb8d, 0x00f87ffd, 0x915fdb8e, 0x00f83ffd, 0x915fdb8f, 0x00f81ffd,
  0x915fdb90, 0x00f80ffd, 0x915fdb91, 0x00f807fd, 0x915fdb92, 0x00f803fd, 0x915fdb93, 0x00f801fd,
  0x915fdb94, 0x00f800fd, 0x915fdb95, 0x00f8007d, 0x915fdb96, 0x00f8003d, 0x915fdb97, 0x00f8001d,
  0x915fdb98, 0x00f8000d, 0x915fdb99, 0x00f80005, 0x915fdb9a, 0x00dc0000, 0x001fdb9a,
  // trailing loads

};

static const unsigned long XcCode_rpu_reset2[57] = {
  0x000000dc, // program size
  0x00000000, // trailing loads size
  0x00162000, 0x00f80001, 0x915fdb81, 0x00f40041, 0x915fdb82, 0x00dc0000, 0x001fdb83, 0x00f8fffd,
  0x914fe004, 0x00fbfffd, 0x915fdb85, 0x0107fffd, 0x915fdb86, 0x01080001, 0x915fdb87, 0x010bfffd,
  0x915fdb88, 0x01040001, 0x915fdb89, 0x01080001, 0x915fdb8a, 0x00f40001, 0x915fdb8b, 0x00f9fffd,
  0x915fdb8c, 0x00f8fffd, 0x915fdb8d, 0x00f87ffd, 0x915fdb8e, 0x00f83ffd, 0x915fdb8f, 0x00f81ffd,
  0x915fdb90, 0x00f80ffd, 0x915fdb91, 0x00f807fd, 0x915fdb92, 0x00f803fd, 0x915fdb93, 0x00f801fd,
  0x915fdb94, 0x00f800fd, 0x915fdb95, 0x00f8007d, 0x915fdb96, 0x00f8003d, 0x915fdb97, 0x00f8001d,
  0x915fdb98, 0x00f8000d, 0x915fdb99, 0x00f80005, 0x915fdb9a, 0x00dc0000, 0x001fdb9a,
  // trailing loads

};

static const unsigned long XcCode_rpu_reset3[57] = {
  0x000000dc, // program size
  0x00000000, // trailing loads size
  0x00163000, 0x00f80001, 0x915fdb81, 0x00f40041, 0x915fdb82, 0x00dc0000, 0x001fdb83, 0x00f8fffd,
  0x914fe004, 0x00fbfffd, 0x915fdb85, 0x0107fffd, 0x915fdb86, 0x01080001, 0x915fdb87, 0x010bfffd,
  0x915fdb88, 0x01040001, 0x915fdb89, 0x01080001, 0x915fdb8a, 0x00f40001, 0x915fdb8b, 0x00f9fffd,
  0x915fdb8c, 0x00f8fffd, 0x915fdb8d, 0x00f87ffd, 0x915fdb8e, 0x00f83ffd, 0x915fdb8f, 0x00f81ffd,
  0x915fdb90, 0x00f80ffd, 0x915fdb91, 0x00f807fd, 0x915fdb92, 0x00f803fd, 0x915fdb93, 0x00f801fd,
  0x915fdb94, 0x00f800fd, 0x915fdb95, 0x00f8007d, 0x915fdb96, 0x00f8003d, 0x915fdb97, 0x00f8001d,
  0x915fdb98, 0x00f8000d, 0x915fdb99, 0x00f80005, 0x915fdb9a, 0x00dc0000, 0x001fdb9a,
  // trailing loads

};

static const unsigned long XcCode_tpu_reset0[57] = {
  0x000000dc, // program size
  0x00000000, // trailing loads size
  0x00160400, 0x01100001, 0x915fdb81, 0x010c0641, 0x915fdb82, 0x00dc0000, 0x001fdb83, 0x0110fffd,
  0x914fe304, 0x0113fffd, 0x915fdb85, 0x011ffffd, 0x915fdb86, 0x01200001, 0x915fdb87, 0x0123fffd,
  0x915fdb88, 0x011c0001, 0x915fdb89, 0x01200001, 0x915fdb8a, 0x010c0601, 0x915fdb8b, 0x0111fffd,
  0x915fdb8c, 0x0110fffd, 0x915fdb8d, 0x01107ffd, 0x915fdb8e, 0x01103ffd, 0x915fdb8f, 0x01101ffd,
  0x915fdb90, 0x01100ffd, 0x915fdb91, 0x011007fd, 0x915fdb92, 0x011003fd, 0x915fdb93, 0x011001fd,
  0x915fdb94, 0x011000fd, 0x915fdb95, 0x0110007d, 0x915fdb96, 0x0110003d, 0x915fdb97, 0x0110001d,
  0x915fdb98, 0x0110000d, 0x915fdb99, 0x01100005, 0x915fdb9a, 0x00dc0000, 0x001fdb9a,
  // trailing loads

};

static const unsigned long XcCode_tpu_reset1[57] = {
  0x000000dc, // program size
  0x00000000, // trailing loads size
  0x00161400, 0x01100001, 0x915fdb81, 0x010c0641, 0x915fdb82, 0x00dc0000, 0x001fdb83, 0x0110fffd,
  0x914fe304, 0x0113fffd, 0x915fdb85, 0x011ffffd, 0x915fdb86, 0x01200001, 0x915fdb87, 0x0123fffd,
  0x915fdb88, 0x011c0001, 0x915fdb89, 0x01200001, 0x915fdb8a, 0x010c0601, 0x915fdb8b, 0x0111fffd,
  0x915fdb8c, 0x0110fffd, 0x915fdb8d, 0x01107ffd, 0x915fdb8e, 0x01103ffd, 0x915fdb8f, 0x01101ffd,
  0x915fdb90, 0x01100ffd, 0x915fdb91, 0x011007fd, 0x915fdb92, 0x011003fd, 0x915fdb93, 0x011001fd,
  0x915fdb94, 0x011000fd, 0x915fdb95, 0x0110007d, 0x915fdb96, 0x0110003d, 0x915fdb97, 0x0110001d,
  0x915fdb98, 0x0110000d, 0x915fdb99, 0x01100005, 0x915fdb9a, 0x00dc0000, 0x001fdb9a,
  // trailing loads

};

static const unsigned long XcCode_tpu_reset2[57] = {
  0x000000dc, // program size
  0x00000000, // trailing loads size
  0x00162400, 0x01100001, 0x915fdb81, 0x010c0641, 0x915fdb82, 0x00dc0000, 0x001fdb83, 0x0110fffd,
  0x914fe304, 0x0113fffd, 0x915fdb85, 0x011ffffd, 0x915fdb86, 0x01200001, 0x915fdb87, 0x0123fffd,
  0x915fdb88, 0x011c0001, 0x915fdb89, 0x01200001, 0x915fdb8a, 0x010c0601, 0x915fdb8b, 0x0111fffd,
  0x915fdb8c, 0x0110fffd, 0x915fdb8d, 0x01107ffd, 0x915fdb8e, 0x01103ffd, 0x915fdb8f, 0x01101ffd,
  0x915fdb90, 0x01100ffd, 0x915fdb91, 0x011007fd, 0x915fdb92, 0x011003fd, 0x915fdb93, 0x011001fd,
  0x915fdb94, 0x011000fd, 0x915fdb95, 0x0110007d, 0x915fdb96, 0x0110003d, 0x915fdb97, 0x0110001d,
  0x915fdb98, 0x0110000d, 0x915fdb99, 0x01100005, 0x915fdb9a, 0x00dc0000, 0x001fdb9a,
  // trailing loads

};

static const unsigned long XcCode_tpu_reset3[57] = {
  0x000000dc, // program size
  0x00000000, // trailing loads size
  0x00163400, 0x01100001, 0x915fdb81, 0x010c0641, 0x915fdb82, 0x00dc0000, 0x001fdb83, 0x0110fffd,
  0x914fe304, 0x0113fffd, 0x915fdb85, 0x011ffffd, 0x915fdb86, 0x01200001, 0x915fdb87, 0x0123fffd,
  0x915fdb88, 0x011c0001, 0x915fdb89, 0x01200001, 0x915fdb8a, 0x010c0601, 0x915fdb8b, 0x0111fffd,
  0x915fdb8c, 0x0110fffd, 0x915fdb8d, 0x01107ffd, 0x915fdb8e, 0x01103ffd, 0x915fdb8f, 0x01101ffd,
  0x915fdb90, 0x01100ffd, 0x915fdb91, 0x011007fd, 0x915fdb92, 0x011003fd, 0x915fdb93, 0x011001fd,
  0x915fdb94, 0x011000fd, 0x915fdb95, 0x0110007d, 0x915fdb96, 0x0110003d, 0x915fdb97, 0x0110001d,
  0x915fdb98, 0x0110000d, 0x915fdb99, 0x01100005, 0x915fdb9a, 0x00dc0000, 0x001fdb9a,
  // trailing loads

};

static const unsigned long* paulxMacRpuCodes[4]=
{
  XcCode_rpu_reset0,
  XcCode_rpu_reset1,
  XcCode_rpu_reset2,
  XcCode_rpu_reset3
};

static const unsigned long* paulxMacTpuCodes[4]=
{
  XcCode_tpu_reset0,
  XcCode_tpu_reset1,
  XcCode_tpu_reset2,
  XcCode_tpu_reset3
};

/*****************************************************************************/
/*! Reset XC Code
* \description
*   Reset XC port.
* \class
*   XC
* \params
*   uiPort           [in]  XC Port Number
*   pvUser           [in]  User Specific Parameters
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX500_XC_Reset( unsigned int uiPort, void* pvUser )
{
  NX500_XMAC_AREA_T* ptXmac;
  NX500_XPEC_AREA_T* ptXpec;

  /* check instance number */
  if( uiPort>=NUM_XPECS ) return -1;

  ptXmac = s_ptXmacStart[uiPort];
  ptXpec = s_ptXpecStart[uiPort];

  /* stop xMAC/xPEC */
  ptXpec->ulXpu_hold_pc = 1;                                   /* Hold xPEC */
  ptXpec->aulStatcfg[uiPort] = 0x00000088;                    /* Reset IO.OE asap */
  ptXmac->ulXmac_tpu_hold_pc = MSK_NX500_xmac_tpu_hold_pc_tpu_hold;  /* Just set Hold-Bit */
  ptXmac->ulXmac_rpu_hold_pc = MSK_NX500_xmac_rpu_hold_pc_rpu_hold;  /* Just set Hold-Bit */
  ptXpec->aulStatcfg[uiPort] = 0x00000088;                    /* Reset IO.OE asap */

  /* load ratemul reset code */
  if( NX500_XC_Load( uiPort, NX500_XC_TYPE_RPU, (unsigned long*)paulxMacRpuCodes[uiPort], pvUser ) !=0 ) {
    return -1;
  }
  if( NX500_XC_Load( uiPort, NX500_XC_TYPE_TPU, (unsigned long*)paulxMacTpuCodes[uiPort], pvUser ) !=0 ) {
    return -1;
  }
  ptXmac->ulXmac_rpu_pc      = 0; /* Reset PC to 0 */
  ptXmac->ulXmac_tpu_pc      = 0; /* Reset PC to 0 */
  ptXmac->ulXmac_tpu_hold_pc = 0; /* Clear Hold-Bit */
  ptXmac->ulXmac_rpu_hold_pc = 0; /* Clear Hold-Bit */

  /* !!!! ATTENTION: There must be enougth time between starting xMAC and stopping xMAC to execute reset programm */

  /* clear xPEC events and stop DMA */
  ptXpec->aulRam[0] = 0xC0000FFF; /* Use the command wait b000000000000,b111111111111 at Address 0 */
  ptXpec->ulXpec_pc = 0;          /* Reset the Program Counter to 0 on netX100/500 */
  ptXpec->ulXpec_statcfg = 0;     /* Reset Timer operation and DMA */
  ptXpec->ulXpu_hold_pc = 0;      /* Start the Program */
  ptXpec->aulRam[0x7FF] = 0x7F;   /* Link and Stop DMA */
  ptXpec->aulRam[0x7FE] = 0x00;   /* dummy access to let xpec run another cycle */
  ptXpec->ulXpu_hold_pc = 1;      /* Hold the Program Counter */
  ptXpec->ulXpec_pc = 0;          /* Reset the Program Counter to 0 on netX100/500 */

  /* reset all xPEC registers to default values */
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
  ptXpec->ulEc_maska = 0x0000ffff;
  ptXpec->ulEc_maskb = 0x0000ffff;
  ptXpec->aulEc_mask[0] = 0x0000ffff;
  ptXpec->aulEc_mask[1] = 0x0000ffff;
  ptXpec->aulEc_mask[2] = 0x0000ffff;
  ptXpec->aulEc_mask[3] = 0x0000ffff;
  ptXpec->aulEc_mask[4] = 0x0000ffff;
  ptXpec->aulEc_mask[5] = 0x0000ffff;
  ptXpec->aulEc_mask[6] = 0x0000ffff;
  ptXpec->aulEc_mask[7] = 0x0000ffff;
  ptXpec->aulEc_mask[8] = 0x0000ffff;
  ptXpec->aulEc_mask[9] = 0x0000ffff;
  ptXpec->ulTimer4 = 0;
  ptXpec->ulTimer5 = 0;
  ptXpec->ulIrq  = 0xffff0000;   /* confirm all ARM IRQs */
  ptXpec->ulXpec_adc = 0;

  /* reset SR of current port */
  ptXpec->aulXpec_sr[uiPort*4]   = 0;
  ptXpec->aulXpec_sr[uiPort*4+1] = 0;
  ptXpec->aulXpec_sr[uiPort*4+2] = 0;
  ptXpec->aulXpec_sr[uiPort*4+3] = 0;
  ptXpec->aulStatcfg[uiPort]     = 0x00000088;

  /* Hold xMAC */
  ptXmac->ulXmac_tpu_hold_pc = MSK_NX500_xmac_tpu_hold_pc_tpu_hold;
  ptXmac->ulXmac_rpu_hold_pc = MSK_NX500_xmac_rpu_hold_pc_rpu_hold;

  /* reset urx and utx fifos */
  switch( uiPort )
  {
    case 0:
      ptXmac->ulXmac_config_shared0 |= MSK_NX500_xmac_config_shared0_reset_tx_fifo;
      ptXmac->ulXmac_config_shared0 &= ~MSK_NX500_xmac_config_shared0_reset_tx_fifo;
      ptXmac->ulXmac_config_shared0 |= MSK_NX500_xmac_config_shared0_reset_rx_fifo;
      ptXmac->ulXmac_config_shared0 &= ~MSK_NX500_xmac_config_shared0_reset_rx_fifo;
      break;
    case 1:
      ptXmac->ulXmac_config_shared1 |= MSK_NX500_xmac_config_shared1_reset_tx_fifo;
      ptXmac->ulXmac_config_shared1 &= ~MSK_NX500_xmac_config_shared1_reset_tx_fifo;
      ptXmac->ulXmac_config_shared1 |= MSK_NX500_xmac_config_shared1_reset_rx_fifo;
      ptXmac->ulXmac_config_shared1 &= ~MSK_NX500_xmac_config_shared1_reset_rx_fifo;
      break;
    case 2:
      ptXmac->ulXmac_config_shared2 |= MSK_NX500_xmac_config_shared2_reset_tx_fifo;
      ptXmac->ulXmac_config_shared2 &= ~MSK_NX500_xmac_config_shared2_reset_tx_fifo;
      ptXmac->ulXmac_config_shared2 |= MSK_NX500_xmac_config_shared2_reset_rx_fifo;
      ptXmac->ulXmac_config_shared2 &= ~MSK_NX500_xmac_config_shared2_reset_rx_fifo;
      break;
    case 3:
      ptXmac->ulXmac_config_shared3 |= MSK_NX500_xmac_config_shared3_reset_tx_fifo;
      ptXmac->ulXmac_config_shared3 &= ~MSK_NX500_xmac_config_shared3_reset_tx_fifo;
      ptXmac->ulXmac_config_shared3 |= MSK_NX500_xmac_config_shared3_reset_rx_fifo;
      ptXmac->ulXmac_config_shared3 &= ~MSK_NX500_xmac_config_shared3_reset_rx_fifo;
      break;
  }

  /* reset IRQs from ARM side */
  s_ptXpecIrq->aulIrq_xpec[uiPort] = 0x0000FFFF;

  /* reset all xMAC registers to default values */
  ptXmac->ulXmac_rx_hw               = 0;
  ptXmac->ulXmac_rx_hw_count         = 0;
  ptXmac->ulXmac_tx                  = 0;
  ptXmac->ulXmac_tx_hw               = 0;
  ptXmac->ulXmac_tx_hw_count         = 0;
  ptXmac->ulXmac_tx_sent             = 0;
  ptXmac->aulXmac_wr[0]              = 0;
  ptXmac->aulXmac_wr[1]              = 0;
  ptXmac->aulXmac_wr[2]              = 0;
  ptXmac->aulXmac_wr[3]              = 0;
  ptXmac->aulXmac_wr[4]              = 0;
  ptXmac->aulXmac_wr[5]              = 0;
  ptXmac->aulXmac_wr[6]              = 0;
  ptXmac->aulXmac_wr[7]              = 0;
  ptXmac->aulXmac_wr[8]              = 0;
  ptXmac->aulXmac_wr[9]              = 0;
  ptXmac->ulXmac_config_mii          = 0;
  ptXmac->ulXmac_config_nibble_fifo  = 0x00000280;
  ptXmac->ulXmac_rpu_count1          = 0;
  ptXmac->ulXmac_rpu_count2          = 0;
  ptXmac->ulXmac_tpu_count1          = 0;
  ptXmac->ulXmac_tpu_count2          = 0;
  ptXmac->ulXmac_rx_count            = 0;
  ptXmac->ulXmac_tx_count            = 0;
  ptXmac->ulXmac_rpm_mask0           = 0;
  ptXmac->ulXmac_rpm_val0            = 0;
  ptXmac->ulXmac_rpm_mask1           = 0;
  ptXmac->ulXmac_rpm_val1            = 0;
  ptXmac->ulXmac_tpm_mask0           = 0;
  ptXmac->ulXmac_tpm_val0            = 0;
  ptXmac->ulXmac_tpm_mask1           = 0;
  ptXmac->ulXmac_tpm_val1            = 0;
  ptXmac->ulXmac_rx_crc_polynomial_l = 0;
  ptXmac->ulXmac_rx_crc_polynomial_h = 0;
  ptXmac->ulXmac_rx_crc_l            = 0;
  ptXmac->ulXmac_rx_crc_h            = 0;
  ptXmac->ulXmac_rx_crc_cfg          = 0;
  ptXmac->ulXmac_tx_crc_polynomial_l = 0;
  ptXmac->ulXmac_tx_crc_polynomial_h = 0;
  ptXmac->ulXmac_tx_crc_l            = 0;
  ptXmac->ulXmac_tx_crc_h            = 0;
  ptXmac->ulXmac_tx_crc_cfg          = 0;

  /* reset encoder and PWM on ports 2 and 3 */
  switch( uiPort )
  {
    case 2: /* fall through */
    case 3:
      ptXmac->ulXmac_pwm_config = 0;
      ptXmac->ulXmac_pwm_status = 0;
      ptXmac->ulXmac_pwm_tp = 0;
      ptXmac->ulXmac_pwm_tu = 0;
      ptXmac->ulXmac_pwm_tv = 0;
      ptXmac->ulXmac_pwm_tw = 0;
      ptXmac->ulXmac_pwm_td = 0;
      ptXmac->ulXmac_rpwm_tp = 0;
      ptXmac->ulXmac_rpwm_tr = 0;
      ptXmac->ulXmac_pos_config_encoder = 0;
      ptXmac->ulXmac_pos_config_capture = 0;
      ptXmac->ulXmac_pos_command = 0;
      ptXmac->ulXmac_pos_status = 0;
      break;
    default: break;
  }

  return 0;
}

/*****************************************************************************/
/*! Reset XC Units
* \description
*   Reset XC units.
* \class
*   XC
* \params
*   uiPort           [in]  XC Port Number
*   uiUnitVec        [in]  Bit vector defining the units
*                          bit 0: rPU, bit 1: tPU, bit 2: xPEC
*   pvUser           [in]  User Specific Parameters
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX500_XC_ResetUnits( unsigned int uiPort, unsigned int uiUnitVec, void* pvUser )
{
  NX500_XMAC_AREA_T* ptXmac;
  NX500_XPEC_AREA_T* ptXpec;

  /* check instance number */
  if( uiPort>=NUM_XPECS ) return -1;

  ptXmac = s_ptXmacStart[uiPort];
  ptXpec = s_ptXpecStart[uiPort];

  /* stop xMAC/xPEC */
  if( 0 != (uiUnitVec & MSK_NX500_XC_TYPE_XPEC) )
  {
    ptXpec->ulXpu_hold_pc = 1;                                   /* Hold xPEC */
  }

  if( 0 != (uiUnitVec & MSK_NX500_XC_TYPE_XMAC) )
  {
    ptXmac->ulXmac_tpu_hold_pc = MSK_NX500_xmac_tpu_hold_pc_tpu_hold;  /* Just set Hold-Bit */
    ptXmac->ulXmac_rpu_hold_pc = MSK_NX500_xmac_rpu_hold_pc_rpu_hold;  /* Just set Hold-Bit */

    /* load ratemul reset code */
    if( NX500_XC_Load( uiPort, NX500_XC_TYPE_RPU, (unsigned long*)paulxMacRpuCodes[uiPort], pvUser ) !=0 ) {
      return -1;
    }
    if( NX500_XC_Load( uiPort, NX500_XC_TYPE_TPU, (unsigned long*)paulxMacTpuCodes[uiPort], pvUser ) !=0 ) {
      return -1;
    }
    ptXmac->ulXmac_rpu_pc      = 0; /* Reset PC to 0 */
    ptXmac->ulXmac_tpu_pc      = 0; /* Reset PC to 0 */
    ptXmac->ulXmac_tpu_hold_pc = 0; /* Clear Hold-Bit */
    ptXmac->ulXmac_rpu_hold_pc = 0; /* Clear Hold-Bit */

    /* !!!! ATTENTION: There must be enough time between starting xMAC and stopping xMAC to execute reset program */
  }

  if( 0 != (uiUnitVec & MSK_NX500_XC_TYPE_XPEC) )
  {
    /* clear xPEC events and stop DMA */
    ptXpec->aulRam[0] = 0xC0000FFF; /* Use the command wait b000000000000,b111111111111 at Address 0 */
    ptXpec->ulXpec_pc = 0;          /* Reset the Program Counter to 0 on netX100/500 */
    ptXpec->ulXpec_statcfg = 0;     /* Reset Timer operation and DMA */
    ptXpec->ulXpu_hold_pc = 0;      /* Start the Program */
    ptXpec->aulRam[0x7FF] = 0x7F;   /* Link and Stop DMA */
    ptXpec->aulRam[0x7FE] = 0x00;   /* dummy access to let xpec run another cycle */
    ptXpec->ulXpu_hold_pc = 1;      /* Hold the Program Counter */
    ptXpec->ulXpec_pc = 0;          /* Reset the Program Counter to 0 on netX100/500 */

    /* reset all xPEC registers to default values */
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
    ptXpec->ulEc_maska = 0x0000ffff;
    ptXpec->ulEc_maskb = 0x0000ffff;
    ptXpec->aulEc_mask[0] = 0x0000ffff;
    ptXpec->aulEc_mask[1] = 0x0000ffff;
    ptXpec->aulEc_mask[2] = 0x0000ffff;
    ptXpec->aulEc_mask[3] = 0x0000ffff;
    ptXpec->aulEc_mask[4] = 0x0000ffff;
    ptXpec->aulEc_mask[5] = 0x0000ffff;
    ptXpec->aulEc_mask[6] = 0x0000ffff;
    ptXpec->aulEc_mask[7] = 0x0000ffff;
    ptXpec->aulEc_mask[8] = 0x0000ffff;
    ptXpec->aulEc_mask[9] = 0x0000ffff;
    ptXpec->ulTimer4 = 0;
    ptXpec->ulTimer5 = 0;
    ptXpec->ulIrq  = 0xffff0000;   /* confirm all ARM IRQs */
    ptXpec->ulXpec_adc = 0;

    /* reset SR of current port */
    ptXpec->aulXpec_sr[uiPort*4]   = 0;
    ptXpec->aulXpec_sr[uiPort*4+1] = 0;
    ptXpec->aulXpec_sr[uiPort*4+2] = 0;
    ptXpec->aulXpec_sr[uiPort*4+3] = 0;
    ptXpec->aulStatcfg[uiPort]     = 0x00000088;
  }

  if( 0 != (uiUnitVec & MSK_NX500_XC_TYPE_XMAC) )
  {
    /* Hold xMAC */
    ptXmac->ulXmac_tpu_hold_pc = MSK_NX500_xmac_tpu_hold_pc_tpu_hold;
    ptXmac->ulXmac_rpu_hold_pc = MSK_NX500_xmac_rpu_hold_pc_rpu_hold;

    /* reset urx and utx fifos */
    switch( uiPort )
    {
      case 0:
        ptXmac->ulXmac_config_shared0 |= MSK_NX500_xmac_config_shared0_reset_tx_fifo;
        ptXmac->ulXmac_config_shared0 &= ~MSK_NX500_xmac_config_shared0_reset_tx_fifo;
        ptXmac->ulXmac_config_shared0 |= MSK_NX500_xmac_config_shared0_reset_rx_fifo;
        ptXmac->ulXmac_config_shared0 &= ~MSK_NX500_xmac_config_shared0_reset_rx_fifo;
        break;
      case 1:
        ptXmac->ulXmac_config_shared1 |= MSK_NX500_xmac_config_shared1_reset_tx_fifo;
        ptXmac->ulXmac_config_shared1 &= ~MSK_NX500_xmac_config_shared1_reset_tx_fifo;
        ptXmac->ulXmac_config_shared1 |= MSK_NX500_xmac_config_shared1_reset_rx_fifo;
        ptXmac->ulXmac_config_shared1 &= ~MSK_NX500_xmac_config_shared1_reset_rx_fifo;
        break;
      case 2:
        ptXmac->ulXmac_config_shared2 |= MSK_NX500_xmac_config_shared2_reset_tx_fifo;
        ptXmac->ulXmac_config_shared2 &= ~MSK_NX500_xmac_config_shared2_reset_tx_fifo;
        ptXmac->ulXmac_config_shared2 |= MSK_NX500_xmac_config_shared2_reset_rx_fifo;
        ptXmac->ulXmac_config_shared2 &= ~MSK_NX500_xmac_config_shared2_reset_rx_fifo;
        break;
      case 3:
        ptXmac->ulXmac_config_shared3 |= MSK_NX500_xmac_config_shared3_reset_tx_fifo;
        ptXmac->ulXmac_config_shared3 &= ~MSK_NX500_xmac_config_shared3_reset_tx_fifo;
        ptXmac->ulXmac_config_shared3 |= MSK_NX500_xmac_config_shared3_reset_rx_fifo;
        ptXmac->ulXmac_config_shared3 &= ~MSK_NX500_xmac_config_shared3_reset_rx_fifo;
        break;
    }
  }

  if( 0 != (uiUnitVec & MSK_NX500_XC_TYPE_XPEC) )
  {
    /* reset IRQs from ARM side */
    s_ptXpecIrq->aulIrq_xpec[uiPort] = 0x0000FFFF;
  }

  if( 0 != (uiUnitVec & MSK_NX500_XC_TYPE_XMAC) )
  {
    /* reset all xMAC registers to default values */
    ptXmac->ulXmac_rx_hw               = 0;
    ptXmac->ulXmac_rx_hw_count         = 0;
    ptXmac->ulXmac_tx                  = 0;
    ptXmac->ulXmac_tx_hw               = 0;
    ptXmac->ulXmac_tx_hw_count         = 0;
    ptXmac->ulXmac_tx_sent             = 0;
    ptXmac->aulXmac_wr[0]              = 0;
    ptXmac->aulXmac_wr[1]              = 0;
    ptXmac->aulXmac_wr[2]              = 0;
    ptXmac->aulXmac_wr[3]              = 0;
    ptXmac->aulXmac_wr[4]              = 0;
    ptXmac->aulXmac_wr[5]              = 0;
    ptXmac->aulXmac_wr[6]              = 0;
    ptXmac->aulXmac_wr[7]              = 0;
    ptXmac->aulXmac_wr[8]              = 0;
    ptXmac->aulXmac_wr[9]              = 0;
    ptXmac->ulXmac_config_mii          = 0;
    ptXmac->ulXmac_config_nibble_fifo  = 0x00000280;
    ptXmac->ulXmac_rpu_count1          = 0;
    ptXmac->ulXmac_rpu_count2          = 0;
    ptXmac->ulXmac_tpu_count1          = 0;
    ptXmac->ulXmac_tpu_count2          = 0;
    ptXmac->ulXmac_rx_count            = 0;
    ptXmac->ulXmac_tx_count            = 0;
    ptXmac->ulXmac_rpm_mask0           = 0;
    ptXmac->ulXmac_rpm_val0            = 0;
    ptXmac->ulXmac_rpm_mask1           = 0;
    ptXmac->ulXmac_rpm_val1            = 0;
    ptXmac->ulXmac_tpm_mask0           = 0;
    ptXmac->ulXmac_tpm_val0            = 0;
    ptXmac->ulXmac_tpm_mask1           = 0;
    ptXmac->ulXmac_tpm_val1            = 0;
    ptXmac->ulXmac_rx_crc_polynomial_l = 0;
    ptXmac->ulXmac_rx_crc_polynomial_h = 0;
    ptXmac->ulXmac_rx_crc_l            = 0;
    ptXmac->ulXmac_rx_crc_h            = 0;
    ptXmac->ulXmac_rx_crc_cfg          = 0;
    ptXmac->ulXmac_tx_crc_polynomial_l = 0;
    ptXmac->ulXmac_tx_crc_polynomial_h = 0;
    ptXmac->ulXmac_tx_crc_l            = 0;
    ptXmac->ulXmac_tx_crc_h            = 0;
    ptXmac->ulXmac_tx_crc_cfg          = 0;

    /* reset encoder and PWM on ports 2 and 3 */
    switch( uiPort )
    {
      case 2: /* fall through */
      case 3:
        ptXmac->ulXmac_pwm_config = 0;
        ptXmac->ulXmac_pwm_status = 0;
        ptXmac->ulXmac_pwm_tp = 0;
        ptXmac->ulXmac_pwm_tu = 0;
        ptXmac->ulXmac_pwm_tv = 0;
        ptXmac->ulXmac_pwm_tw = 0;
        ptXmac->ulXmac_pwm_td = 0;
        ptXmac->ulXmac_rpwm_tp = 0;
        ptXmac->ulXmac_rpwm_tr = 0;
        ptXmac->ulXmac_pos_config_encoder = 0;
        ptXmac->ulXmac_pos_config_capture = 0;
        ptXmac->ulXmac_pos_command = 0;
        ptXmac->ulXmac_pos_status = 0;
        break;
      default: break;
    }
  }

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
int NX500_XC_Load( unsigned int uiPort, NX500_XC_TYPE_E eXcType, unsigned long* pulXcPrg, void* pvUser )
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
    case NX500_XC_TYPE_XPEC:
      pulRamStart = (volatile unsigned long*) s_ptXpecStart[uiPort];
      pulRamEnd = pulRamStart + (sizeof(NX500_XPEC_AREA_T) / sizeof(unsigned long));
      ulPhysicalStart = (unsigned long) s_ptXpecStart[uiPort];
      break;

    case NX500_XC_TYPE_RPU:
      /* fall through */

    case NX500_XC_TYPE_TPU:
      pulRamStart = (volatile unsigned long*)s_ptXmacStart[uiPort];
      pulRamEnd = pulRamStart + (sizeof(NX500_XMAC_AREA_T)/sizeof(unsigned long));
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
int NX500_XC_Start( unsigned int uiPort, void* pvUser )
{
  NX500_XMAC_AREA_T* ptXmac;
  NX500_XPEC_AREA_T* ptXpec;

  /* check instance number */
  if( uiPort >= NUM_XPECS ) return -1;

  ptXmac = s_ptXmacStart[uiPort];
  ptXpec = s_ptXpecStart[uiPort];

  ptXmac->ulXmac_rpu_pc      = 0;
  ptXmac->ulXmac_tpu_pc      = 0;
  ptXpec->ulXpec_pc          = 0;

  ptXmac->ulXmac_rpu_hold_pc = 0;
  ptXmac->ulXmac_tpu_hold_pc = 0;
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
int NX500_XC_StartUnits(unsigned int uiPort, unsigned int uiUnitVec, void* pvUser)
{
  NX500_XMAC_AREA_T* ptXmac;
  NX500_XPEC_AREA_T* ptXpec;

  /* check instance number */
  if( uiPort >= NUM_XPECS ) return -1;

  ptXmac = s_ptXmacStart[uiPort];
  ptXpec = s_ptXpecStart[uiPort];

  if( 0 != (uiUnitVec & MSK_NX500_XC_TYPE_RPU) )
  {
    ptXmac->ulXmac_rpu_pc      = 0;
    ptXmac->ulXmac_rpu_hold_pc = 0;
  }

  if( 0 != (uiUnitVec & MSK_NX500_XC_TYPE_TPU) )
  {
    ptXmac->ulXmac_tpu_pc      = 0;
    ptXmac->ulXmac_tpu_hold_pc = 0;
  }

  if( 0 != (uiUnitVec & MSK_NX500_XC_TYPE_XPEC) )
  {
    ptXpec->ulXpec_pc          = 0;
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
int NX500_MIIMU_ReadPhyReg( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuRtaField, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, unsigned short* pusData, void* pvUser)
{

  NX500_MIIMU_REG_T tMiiMuReg = {0};

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
int NX500_MIIMU_WritePhyReg( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, unsigned short usData, void* pvUser)
{
  NX500_MIIMU_REG_T tMiiMuReg = {0};

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

//....###.....######..####..######...........######..########.########..##......
//...##.##...##....##..##..##....##.........##....##....##....##.....##.##......
//..##...##..##........##..##...............##..........##....##.....##.##......
//.##.....##..######...##..##...............##..........##....########..##......
//.#########.......##..##..##...............##..........##....##...##...##......
//.##.....##.##....##..##..##....##.........##....##....##....##....##..##......
//.##.....##..######..####..######..#######..######.....##....##.....##.########

/*****************************************************************************/
/*! Set IO_Config
* \description
*   Writes a new value to IO_Config.
* \class
*   ASIC_CTRL
* \params
*   uiIdx  [in] Register index
*   ulVal  [in] Value for io_config
* \return
*                                                                            */
/*****************************************************************************/
void NX500_AsicCtrl_SetIoConfig( unsigned int uiIdx, unsigned long ulVal )
{
  switch(uiIdx)
  {
  case 1:
    s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
    s_ptAsicCtrl->ulIo_config = ulVal;
    break;
  default:
    break;
  }
}

/*****************************************************************************/
/*! Get IO_Config
* \description
*   Read the value of IO_Config.
* \class
*   ASIC_CTRL
* \params
*   uiIdx  [in] Register index
* \return
*   Current io_config value
*                                                                            */
/*****************************************************************************/
unsigned long NX500_AsicCtrl_GetIoConfig( unsigned int uiIdx )
{
  unsigned long ulReturnVal = 0;

  switch(uiIdx)
  {
  case 1:
    ulReturnVal = s_ptAsicCtrl->ulIo_config;
    break;
  default:
    break;
  }
  return ulReturnVal;
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
void NX500_RSTCTRL_SetCfg( unsigned long ulRstCtrlVal )
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
unsigned long NX500_RSTCTRL_GetCfg( void )
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
void NX500_SYSTIME_SetBorder( unsigned long ulBorder, void* pvUser )
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
void NX500_SYSTIME_SetSpeed( unsigned long ulSpeed, void* pvUser )
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
void NX500_SYSTIME_SetSystime( unsigned long ulSystime_s, unsigned long ulSystime_ns, void* pvUser )
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
void NX500_SYSTIME_GetSystime( unsigned long* pulSystime_s, unsigned long* pulSystime_ns, void* pvUser )
{
  *pulSystime_s  = s_ptSystime->ulSystime_s;
  *pulSystime_ns = s_ptSystime->ulSystime_ns;
}
