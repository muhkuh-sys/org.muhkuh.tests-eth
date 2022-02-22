/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx6_xpecirq.h"
#include "hal_resources_defines_netx6.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_XPEC_IRQ_REGS

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! XPECIRQ Set Interrupt
* \description
*   This function sets interrupts to xPEC
* \class 
*   XPECIRQ Service Class
* \params
*   uXpecNum	[in] xPEC number
*   ulVal     [in] Value
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
int NX6_XPECIRQ_SetIrq ( unsigned int uXpecNum,
                         uint32_t     ulVal )
{
	if( uXpecNum >= 2 ) return -1;

  NX_WRITE32(s_ptXpecIrqRegs->aulIrq_xpec[uXpecNum], ulVal);
  
  return 0;
}

/*****************************************************************************/
/*! XPECIRQ Get Interrupt
* \description
*   This function gets interrupts from xPEC
* \class 
*   XPECIRQ Service Class
* \params
*   uXpecNum	[in] xPEC number
* \return
*   0 on erroneous
*   IRQ value                                                                */
/*****************************************************************************/
uint32_t NX6_XPECIRQ_GetIrq ( unsigned int uXpecNum )
{
	if( uXpecNum >= 2 ) return 0;

  return NX_READ32(s_ptXpecIrqRegs->aulIrq_xpec[uXpecNum]);
}
