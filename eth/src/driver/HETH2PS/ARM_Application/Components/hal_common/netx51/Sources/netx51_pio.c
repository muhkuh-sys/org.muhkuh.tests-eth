/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx51_pio.h"
#include "hal_resources_defines_netx51.h"

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
*   uPioNum           [in]  Selected PIO
*   eMode             [in]  PIO_MODE_IN  = Set the PIO to input mode
*                           PIO_MODE_OUT = Set the PIO to output mode
* \return
*                                                                            */
/*****************************************************************************/
void NX51_PIO_SetMode( unsigned int    uPioNum,
                       NX51_PIO_MODE_E eMode )
{
  if( NX51_PIO_MODE_OUT == eMode )
    s_ptPio->ulPio_oe |= (uint32_t)1 << uPioNum;
  else
    s_ptPio->ulPio_oe &= ~((uint32_t)1 << uPioNum);
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
*   Current pio_in_level of selected PIO                                     */
/*****************************************************************************/
unsigned int NX51_PIO_GetInput( unsigned int uPioNum )
{
  return ( s_ptPio->ulPio_in & ((uint32_t)1 << uPioNum)) ? 1 : 0;
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
uint32_t NX51_PIO_GetIn( void )
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
*   uPioNum          [in]  Selected PIO
* \return
*   Current pio_out_level of selected PIO                                    */
/*****************************************************************************/
unsigned int NX51_PIO_GetOutput( unsigned int uPioNum )
{
  return ( s_ptPio->ulPio_out & ((uint32_t)1 << uPioNum)) ? 1 : 0;
}

/*****************************************************************************/
/*! PIO Set Output
* \description
*   Set output for the selected PIO.
* \class
*   PIO
* \params
*   uPioNum           [in]  Selected PIO
*   uEnable           [in]  0 = Disable the Output
*                           1 = Enable the Output
* \return
*                                                                            */
/*****************************************************************************/
void NX51_PIO_SetOutput( unsigned int uPioNum,
                         unsigned int uEnable )
{
  if( uEnable == 0 ) {
    s_ptPio->ulPio_out &= ~((uint32_t)1 << uPioNum);
  } else {
    s_ptPio->ulPio_out |= (uint32_t)1 << uPioNum;
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
*   Current pio_line value                                                   */
/*****************************************************************************/
uint32_t NX51_PIO_GetLine( void )
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
void NX51_PIO_SetLine( uint32_t ulVal )
{
  s_ptPio->ulPio_out = ulVal;
}
