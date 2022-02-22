/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx6_pio.h"
#include "hal_resources_defines_netx6.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_PIO

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! PIO Set Mode
* \description
*   Enables/disable output for the selected PIO.
* \class
*   PIO
* \params
*   uPioNum          [in]  Selected PIO
*   eMode            [in]  PIO_MODE_IN  = Set the PIO to input mode
*                          PIO_MODE_OUT = Set the PIO to output mode
* \return
*                                                                            */
/*****************************************************************************/
void NX6_PIO_SetMode( unsigned int uPioNum,
                      NX6_PIO_MODE_E eMode )
{
  uint32_t ulVal = NX_READ32(s_ptPio->ulPio_oe);
  if( NX6_PIO_MODE_OUT == eMode )
    ulVal |= (uint32_t)1 << uPioNum;
  else
    ulVal &= ~((uint32_t)1 << uPioNum);
  NX_WRITE32(s_ptPio->ulPio_oe, ulVal);
}

/*****************************************************************************/
/*! PIO Get Input
* \description
*   Read current value of selected PIO within PIO in register.
* \class
*   PIO
* \params
*   uPioNum          [in]  Selected PIO
* \return
*    Current pio_in_level of selected PIO                                    */
/*****************************************************************************/
unsigned int NX6_PIO_GetInput( unsigned int uPioNum )
{
  return ( NX_READ32(s_ptPio->ulPio_in) & ((uint32_t)1 << uPioNum)) ? 1 : 0;
}

/*****************************************************************************/
/*! PIO Get In Register
* \description
*  Read the PIO IN register.
* \class
*   PIO
* \params
* \return
*   Current pio_in value                                                     */
/*****************************************************************************/
uint32_t NX6_PIO_GetIn( void )
{
  return NX_READ32(s_ptPio->ulPio_in);
}

/*****************************************************************************/
/*! PIO Get Output
* \description
*   Read current value of selected PIO within PIO out register.
* \class
*   PIO
* \params
*   uPioNum          [in]  Selected PIO
* \return
*   Current pio_out_level of selected PIO                                   */
/*****************************************************************************/
unsigned int NX6_PIO_GetOutput( unsigned int uPioNum )
{
  return (NX_READ32(s_ptPio->ulPio_out) & ((uint32_t)1 << uPioNum)) ? 1 : 0;
}

/*****************************************************************************/
/*! PIO Set Output
* \description
*   Set output for the selected PIO.
* \class
*   PIO
* \params
*   uPioNum          [in]  Selected PIO
*   uEnable          [in]  0 = Disable the Output
*                          1 = Enable the Output
* \return
*                                                                            */
/*****************************************************************************/
void NX6_PIO_SetOutput( unsigned int uPioNum,
                        unsigned int uEnable )
{
  uint32_t ulVal = NX_READ32(s_ptPio->ulPio_out);
  if( uEnable == 1 ) {
    ulVal |= (uint32_t)1 << uPioNum;
  } else {
    ulVal &= ~((uint32_t)1 << uPioNum);
  }
  NX_WRITE32(s_ptPio->ulPio_out, ulVal);
}

/*****************************************************************************/
/*! PIO Get Line
* \description
*  Read the PIO out register.
* \class
*   PIO
* \params
* \return
*   Current pio_line value                                                   */
/*****************************************************************************/
uint32_t NX6_PIO_GetLine( void )
{
  return NX_READ32(s_ptPio->ulPio_out);
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
void NX6_PIO_SetLine( uint32_t ulVal )
{
  NX_WRITE32(s_ptPio->ulPio_out, ulVal);
}
