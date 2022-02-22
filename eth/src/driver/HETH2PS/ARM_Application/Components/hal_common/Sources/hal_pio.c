/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "hal_pio.h"
#include "hw_defines.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
HW_PTR_PIO(s_ptPio)

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
*   uiPioNum           [in]  Selected PIO
*   eMode             [in]  PIO_MODE_IN  = Set the PIO to input mode
*                           PIO_MODE_OUT = Set the PIO to output mode
* \return
*                                                                            */
/*****************************************************************************/
void PIO_SetMode( unsigned int uiPioNum, PIO_MODE_E eMode )
{
  if( PIO_MODE_OUT == eMode )
    s_ptPio->ulPio_oe |= (uint32_t)1 << uiPioNum;
  else
    s_ptPio->ulPio_oe &= ~((uint32_t)1 << uiPioNum);
}

/*****************************************************************************/
/*! PIO Get Input
* \description
*   Read current value of selected PIO within PIO in register.
* \class
*   PIO
* \params
*   uiPioNum          [in]  Selected PIO
* \return
*   Current pio_in_level of selected PIO                                     */
/*****************************************************************************/
int PIO_GetInput( unsigned int uiPioNum )
{
  return ( s_ptPio->ulPio_in & ((uint32_t)1 << uiPioNum)) ? 1 : 0;
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
uint32_t PIO_GetIn( void )
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
*   uiPioNum          [in]  Selected PIO
* \return
*   Current pio_out_level of selected PIO                                    */
/*****************************************************************************/
int PIO_GetOutput( unsigned int uiPioNum )
{
  return ( s_ptPio->ulPio_out & ((uint32_t)1 << uiPioNum)) ? 1 : 0;
}

/*****************************************************************************/
/*! PIO Set Output
* \description
*   Set output for the selected PIO.
* \class
*   PIO
* \params
*   uiPioNum           [in]  Selected PIO
*   fEnable           [in]  false/true = Disable/Enable the Output
* \return
*                                                                            */
/*****************************************************************************/
void PIO_SetOutput( unsigned int uiPioNum, bool fEnable )
{
  if( fEnable ) {
    s_ptPio->ulPio_out |= (uint32_t)1 << uiPioNum;
  } else {
    s_ptPio->ulPio_out &= ~((uint32_t)1 << uiPioNum);
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
uint32_t PIO_GetLine( void )
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
void PIO_SetLine( uint32_t ulVal )
{
  s_ptPio->ulPio_out = ulVal;
}

/*****************************************************************************/
/*! PIO Set OE vector
* \description
*   Set output enable vector.
* \class
*   PIO
* \params
*   ulOe             [in]  OE vector
* \return
*                                                                            */
/*****************************************************************************/
void PIO_SetOe( uint32_t ulOe )
{
  s_ptPio->ulPio_oe = ulOe;
}
