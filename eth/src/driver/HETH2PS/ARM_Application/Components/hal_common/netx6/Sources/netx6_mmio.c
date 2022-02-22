/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx6_mmio.h"
#include "hal_resources_defines_netx6.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_ASIC_CTRL
__USE_MMIO_CTRL

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! MMIO Set Configuration
* \description
*   Configures a MMIO pin.
* \class
*   MMIO
* \params
*   uMmioNum          [in]  Selected MMIO
*   tConfig           [in]  Signal Selection
*   uInvertOut        [in]  Invert Output Signal
*   uInvertIn         [in]  Invert Input Signal
* \return
*                                                                            */
/*****************************************************************************/
void NX6_MMIO_SetCfg( unsigned int      uMmioNum,
                      NX6_MMIO_CONFIG_E eConfig,
                      unsigned int    uInvertOut,
                      unsigned int    uInvertIn )
{
  /* read access key, write back access key */
  NX_WRITE32(s_ptAsicCtrl->ulAsic_ctrl_access_key, NX_READ32(s_ptAsicCtrl->ulAsic_ctrl_access_key));

  NX_WRITE32(s_ptMmioCtrl->aulMmio_cfg[uMmioNum],
     ((uint32_t)eConfig    << SRT_NX51_mmio0_cfg_mmio_sel)
    |((uint32_t)uInvertOut << SRT_NX51_mmio0_cfg_mmio_out_inv)
    |((uint32_t)uInvertIn  << SRT_NX51_mmio0_cfg_mmio_in_inv));

}

/*****************************************************************************/
/*! MMIO Get Configuration
* \description
*   Get configuration of a MMIO pin.
* \class
*   MMIO
* \params
*   uMmioNum          [in]  Selected MMIO
*   ptConfig          [out]  Pointer to Signal Selection
*   puInvertOut       [out]  Pointer to Invert Output Signal
*   puInvertIn        [out]  Pointer to Invert Input Signal
* \return
*                                                                            */
/*****************************************************************************/
void NX6_MMIO_GetCfg( unsigned int       uMmioNum,
                      NX6_MMIO_CONFIG_E* peConfig,
                      unsigned int*      puInvertOut,
                      unsigned int*      puInvertIn )
{
  uint32_t ulTemp = NX_READ32(s_ptMmioCtrl->aulMmio_cfg[uMmioNum]);

  *peConfig     = (NX6_MMIO_CONFIG_E)((ulTemp & MSK_NX51_mmio0_cfg_mmio_sel)     >> SRT_NX51_mmio0_cfg_mmio_sel);
  *puInvertOut  = (ulTemp & MSK_NX51_mmio0_cfg_mmio_out_inv) >> SRT_NX51_mmio0_cfg_mmio_out_inv;
  *puInvertIn   = (ulTemp & MSK_NX51_mmio0_cfg_mmio_in_inv)  >> SRT_NX51_mmio0_cfg_mmio_in_inv;
}

/*****************************************************************************/
/*! MMIO Set Mode
* \description
*   Enables/disable output for the selected MMIO. MMIO must be in PIO mode.
* \class
*   PIO
* \params
*   uMmioNum         [in]  Selected MMIO
*   tMode            [in]  NX6_MMIO_MODE_IN  = Set the MMIO to input mode
*                          NX6_MMIO_MODE_OUT = Set the MMIO to output mode
* \return
*                                                                            */
/*****************************************************************************/
void NX6_MMIO_SetMode( unsigned int    uMmioNum,
                       NX6_MMIO_MODE_E eMode )
{
  uint32_t ulVal = NX_READ32(s_ptMmioCtrl->aulMmio_cfg[uMmioNum]);
  if( NX6_MMIO_MODE_OUT == eMode )
    ulVal |= MSK_NX51_mmio0_cfg_pio_oe;
  else
    ulVal &= ~MSK_NX51_mmio0_cfg_pio_oe;
  NX_WRITE32(s_ptMmioCtrl->aulMmio_cfg[uMmioNum], ulVal);
}

/*****************************************************************************/
/*! MMIO Get Input
* \description
*   Read current value of selected MMIO within in_line_status register. MMIO must be in PIO mode.
* \class
*   PIO
* \params
*   uMmioNum         [in]  Selected MMIO
* \return
*   1
*   0                                                                        */
/*****************************************************************************/
unsigned int NX6_MMIO_GetInput( unsigned int uMmioNum )
{
  return ( 0 != (NX_READ32(s_ptMmioCtrl->aulMmio_cfg[uMmioNum]) & MSK_NX51_mmio0_cfg_status_in_ro) ) ? 1 : 0;
}

/*****************************************************************************/
/*! MMIO Get Output
* \description
*   Read current value of selected MMIO within out_line_cfg register. MMIO must be in PIO mode.
* \class
*   PIO
* \params
*   uMmioNum          [in]  Selected MMIO
* \return
*   1
*   0                                                                        */
/*****************************************************************************/
unsigned int NX6_MMIO_GetOutput( unsigned int uMmioNum )
{
  return (0 != (NX_READ32(s_ptMmioCtrl->aulMmio_cfg[uMmioNum]) & MSK_NX51_mmio0_cfg_pio_out) ) ? 1 : 0;
}

/*****************************************************************************/
/*! MMIO Set Output
* \description
*   Set output for the selected MMIO within out_line_cfg register. MMIO must be in PIO mode.
* \class
*   PIO
* \params
*   uMmioNum          [in]  Selected MMIO
*   fEnable           [in]  0 = Disable the Output
*                           1 = Enable the Output
* \return
*                                                                            */
/*****************************************************************************/
void NX6_MMIO_SetOutput( unsigned int uMmioNum,
                         bool         fEnable )
{
  uint32_t ulVal = NX_READ32(s_ptMmioCtrl->aulMmio_cfg[uMmioNum]);
  if( fEnable ) {
    ulVal |= MSK_NX51_mmio0_cfg_pio_out;
  } else {
    ulVal &= ~MSK_NX51_mmio0_cfg_pio_out;
  }
  NX_WRITE32(s_ptMmioCtrl->aulMmio_cfg[uMmioNum], ulVal);
}

/*****************************************************************************/
/*! MMIO Get All Inputs
* \description
*   Read current value of all MMIOs within in_line_status.
* \class
*   PIO
* \params
*   pulLine0         [out]  Bit0: MMIO0, ... Bit31: MMIO31
*   pulLine1         [out]  Bit0: MMIO32, ... Bit17: MMIO49
* \return
*                                                                            */
/*****************************************************************************/
void NX6_MMIO_GetLine( uint32_t* pulLine0,
                       uint32_t* pulLine1 )
{
  *pulLine0 = NX_READ32(s_ptMmioCtrl->aulMmio_in_line_status[0]);
  *pulLine1 = NX_READ32(s_ptMmioCtrl->aulMmio_in_line_status[1]);
}

/*****************************************************************************/
/*! MMIO Set All Inputs
* \description
*   Set output for all MMIOs within out_line_cfg registers.
* \class
*   PIO
* \params
*   ulLine0         [in]  Bit0: MMIO0, ... Bit31: MMIO31
*   ulLine1         [in]  Bit0: MMIO32, ... Bit17: MMIO49
* \return
*                                                                            */
/*****************************************************************************/
void NX6_MMIO_SetLine( uint32_t ulLine0,
                       uint32_t ulLine1 )
{
  NX_WRITE32(s_ptMmioCtrl->aulMmio_in_line_status[0], ulLine0);
  NX_WRITE32(s_ptMmioCtrl->aulMmio_in_line_status[1], ulLine1);
}
