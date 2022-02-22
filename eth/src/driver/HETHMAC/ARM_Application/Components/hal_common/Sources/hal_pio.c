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
*   uPioNum           [in]  Selected PIO
*   eMode             [in]  PIO_MODE_IN  = Set the PIO to input mode
*                           PIO_MODE_OUT = Set the PIO to output mode
* \return
*                                                                            */
/*****************************************************************************/
#ifndef HALDEF
#define HALDEF(name) name
#endif

void HALDEF(PIO_SetMode)( unsigned int uPioNum, PIO_MODE_E eMode )
{
  if( PIO_MODE_OUT == eMode )
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
int HALDEF(PIO_GetInput)( unsigned int uPioNum )
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
uint32_t HALDEF(PIO_GetIn)( void )
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
int HALDEF(PIO_GetOutput)( unsigned int uPioNum )
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
*   fEnable           [in]  false/true = Disable/Enable the Output
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(PIO_SetOutput)( unsigned int uPioNum, bool fEnable )
{
  if( fEnable ) {
    s_ptPio->ulPio_out |= (uint32_t)1 << uPioNum;
  } else {
    s_ptPio->ulPio_out &= ~((uint32_t)1 << uPioNum);
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
uint32_t HALDEF(PIO_GetLine)( void )
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
void HALDEF(PIO_SetLine)( uint32_t ulVal )
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
void HALDEF(PIO_SetOe)( uint32_t ulOe )
{
  s_ptPio->ulPio_oe = ulOe;
}
