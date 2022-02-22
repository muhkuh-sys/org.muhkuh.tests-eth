/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "hal_mmio.h"
#include "hw_defines.h"

#include <string.h> /* definition of NULL */

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
HW_PTR_ASIC_CTRL(s_ptAsicCtrl)
HW_PTR_MMIO_CTRL(s_ptMmioCtrl)

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
*   uiMmio            [in]  Selected MMIO
*   eConfig           [in]  Signal Selection
*   uInvertOut        [in]  Invert Output Signal
*   uInvertIn         [in]  Invert Input Signal
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_SetCfg( unsigned int  uiMmio,
                  MMIO_CONFIG_E eConfig,
                  unsigned int  uiInvertOut,
                  unsigned int  uiInvertIn )
{
  MMIO_SetCfgReg(uiMmio, ((uint32_t)eConfig     << HW_SRT(mmio0_cfg_mmio_sel))
                                |((uint32_t)uiInvertOut << HW_SRT(mmio0_cfg_mmio_out_inv))
                                |((uint32_t)uiInvertIn  << HW_SRT(mmio0_cfg_mmio_in_inv)));
}

/*****************************************************************************/
/*! MMIO Get Configuration
* \description
*   Get configuration of a MMIO pin.
* \class
*   MMIO
* \params
*   uiMmio          [in]  Selected MMIO
*   peConfig        [out]  Pointer to Signal Selection
*   puiInvertOut    [out]  Pointer to Invert Output Signal
*   puiInvertIn     [out]  Pointer to Invert Input Signal
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_GetCfg( unsigned int     uiMmio,
                  MMIO_CONFIG_E*   peConfig,
                  unsigned int*    puiInvertOut,
                  unsigned int*    puiInvertIn )
{
  uint32_t ulTemp = s_ptMmioCtrl->aulMmio_cfg[uiMmio];

  *peConfig     = (MMIO_CONFIG_E)((ulTemp & HW_MSK(mmio0_cfg_mmio_sel)) >> HW_SRT(mmio0_cfg_mmio_sel));
  *puiInvertOut = (ulTemp & HW_MSK(mmio0_cfg_mmio_out_inv)) >> HW_SRT(mmio0_cfg_mmio_out_inv);
  *puiInvertIn  = (ulTemp & HW_MSK(mmio0_cfg_mmio_in_inv))  >> HW_SRT(mmio0_cfg_mmio_in_inv);
}

/*****************************************************************************/
/*! MMIO Set Mode
* \description
*   Enables/disable output for the selected MMIO. MMIO must be in PIO mode.
* \class
*   PIO
* \params
*   uiMmio         [in]  Selected MMIO
*   eMode          [in]  MMIO_MODE_IN  = Set the MMIO to input mode
*                        MMIO_MODE_OUT = Set the MMIO to output mode
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_SetMode( unsigned int uiMmio,
                   MMIO_MODE_E  eMode )
{
  if( MMIO_MODE_OUT == eMode)
    s_ptMmioCtrl->aulMmio_cfg[uiMmio] |= HW_MSK(mmio0_cfg_pio_oe);
  else
    s_ptMmioCtrl->aulMmio_cfg[uiMmio] &= ~HW_MSK(mmio0_cfg_pio_oe);
}

/*****************************************************************************/
/*! MMIO Get Input
* \description
*   Read current value of selected MMIO within in_line_status register.
* \class
*   PIO
* \params
*   uiMmio         [in]  Selected MMIO
* \return
*   Current value of selected MMIO within in_line_status register            */
/*****************************************************************************/
unsigned int MMIO_GetInput( unsigned int uiMmio )
{
  return ( 0 != (s_ptMmioCtrl->aulMmio_cfg[uiMmio] & HW_MSK(mmio0_cfg_status_in_ro)) ) ? 1 : 0;
}

/*****************************************************************************/
/*! MMIO Get Output
* \description
*   Read current value of selected MMIO within out_line_cfg register. MMIO must be in PIO mode.
* \class
*   PIO
* \params
*   uiMmio          [in]  Selected MMIO
* \return
*   Current value of selected MMIO within out_line_cfg register              */
/*****************************************************************************/
unsigned int MMIO_GetOutput( unsigned int uiMmio )
{
  return ( 0 != (s_ptMmioCtrl->aulMmio_cfg[uiMmio] & HW_MSK(mmio0_cfg_pio_out)) ) ? 1 : 0;
}

/*****************************************************************************/
/*! MMIO Set Output
* \description
*   Set output for the selected MMIO within out_line_cfg register. MMIO must be in PIO mode.
* \class
*   PIO
* \params
*   uiMmio          [in]  Selected MMIO
*   uiEnable        [in]  0 = Disable the Output
*                         1 = Enable the Output
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_SetOutput( unsigned int uiMmio,
                     unsigned int uiEnable )
{
  if( uiEnable == 0 ) {
    s_ptMmioCtrl->aulMmio_cfg[uiMmio] &= ~HW_MSK(mmio0_cfg_pio_out);
  } else {
    s_ptMmioCtrl->aulMmio_cfg[uiMmio] |= HW_MSK(mmio0_cfg_pio_out);
  }
}

/*****************************************************************************/
/*! MMIO Set config register
* \description
*   Directly set the specific mmio config register with unlocking mechanism.
* \class
*   PIO
* \params
*   uiMmio         [in]  Selected MMIO
*   ulSetCfg       [in]  Selected config
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_SetCfgReg ( unsigned int uiMmio, uint32_t ulSetCfg )
{
  /* read access key, write back access key */
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;

  s_ptMmioCtrl->aulMmio_cfg[uiMmio] = ulSetCfg;
}

/*****************************************************************************/
/*! MMIO Set raw config register
* \description
*   Directly set the specific mmio config register without unlocking mechanism.
* \class
*   PIO
* \params
*   uiMmio         [in]  Selected MMIO
*   ulSetCfg       [in]  Selected config
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_SetCfgRaw ( unsigned int uiMmio, uint32_t ulSetCfg )
{
  s_ptMmioCtrl->aulMmio_cfg[uiMmio] = ulSetCfg;
}

/*****************************************************************************/
/*! MMIO Get config register
* \description
*   Directly get the specific mmio config register
* \class
*   PIO
* \params
*   uiMmio         [in]  Selected MMIO
*   pulGetCfg       [out] Selected config
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_GetCfgReg ( unsigned int uiMmio, uint32_t* pulGetCfg )
{
  *pulGetCfg = s_ptMmioCtrl->aulMmio_cfg[uiMmio];
}

#if defined(__NETX4000)
/*****************************************************************************/
/*! MMIO Write Pio out line set
* \description
*   -
* \class
*   PIO
* \params
*   ulLine0         [out]  Bit0: MMIO0, ... Bit31: MMIO31
*   ulLine1         [out]  Bit0: MMIO32, ... Bit17: MMIO63
*   ulLine2         [out]  Bit0: MMIO64, ... Bit31: MMIO95
*   ulLine3         [out]  Bit0: MMIO96, ... Bit9: MMIO105
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_PioOutLineCfgSet ( uint32_t ulLine0, uint32_t ulLine1, uint32_t ulLine2, uint32_t ulLine3 )
{
  s_ptMmioCtrl->aulMmio_pio_out_line_set_cfg[0] = ulLine0;
  s_ptMmioCtrl->aulMmio_pio_out_line_set_cfg[1] = ulLine1;
  s_ptMmioCtrl->aulMmio_pio_out_line_set_cfg[2] = ulLine2;
  s_ptMmioCtrl->aulMmio_pio_out_line_set_cfg[3] = ulLine3;
}

/*****************************************************************************/
/*! MMIO Write Pio out line reset
* \description
*   -
* \class
*   PIO
* \params
*   ulLine0         [out]  Bit0: MMIO0, ... Bit31: MMIO31
*   ulLine1         [out]  Bit0: MMIO32, ... Bit17: MMIO63
*   ulLine2         [out]  Bit0: MMIO64, ... Bit31: MMIO95
*   ulLine3         [out]  Bit0: MMIO96, ... Bit9: MMIO105
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_PioOutLineCfgRst ( uint32_t ulLine0, uint32_t ulLine1, uint32_t ulLine2, uint32_t ulLine3 )
{
  s_ptMmioCtrl->aulMmio_pio_out_line_reset_cfg[0] = ulLine0;
  s_ptMmioCtrl->aulMmio_pio_out_line_reset_cfg[1] = ulLine1;
  s_ptMmioCtrl->aulMmio_pio_out_line_reset_cfg[2] = ulLine2;
  s_ptMmioCtrl->aulMmio_pio_out_line_reset_cfg[3] = ulLine3;
}

/*****************************************************************************/
/*! MMIO Write Pio oe line set
* \description
*   -
* \class
*   PIO
* \params
*   ulLine0         [out]  Bit0: MMIO0, ... Bit31: MMIO31
*   ulLine1         [out]  Bit0: MMIO32, ... Bit17: MMIO63
*   ulLine2         [out]  Bit0: MMIO64, ... Bit31: MMIO95
*   ulLine3         [out]  Bit0: MMIO96, ... Bit9: MMIO105
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_PioOeLineCfgSet ( uint32_t ulLine0, uint32_t ulLine1, uint32_t ulLine2, uint32_t ulLine3 )
{
  s_ptMmioCtrl->aulMmio_pio_oe_line_set_cfg[0] = ulLine0;
  s_ptMmioCtrl->aulMmio_pio_oe_line_set_cfg[1] = ulLine1;
  s_ptMmioCtrl->aulMmio_pio_oe_line_set_cfg[2] = ulLine2;
  s_ptMmioCtrl->aulMmio_pio_oe_line_set_cfg[3] = ulLine3;
}

/*****************************************************************************/
/*! MMIO Write Pio oe line reset
* \description
*   -
* \class
*   PIO
* \params
*   ulLine0         [out]  Bit0: MMIO0,  ... Bit31: MMIO31
*   ulLine1         [out]  Bit0: MMIO32, ... Bit17: MMIO63
*   ulLine2         [out]  Bit0: MMIO64, ... Bit31: MMIO95
*   ulLine3         [out]  Bit0: MMIO96, ... Bit9: MMIO105
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_PioOeLineCfgRst ( uint32_t ulLine0, uint32_t ulLine1, uint32_t ulLine2, uint32_t ulLine3 )
{
  s_ptMmioCtrl->aulMmio_pio_oe_line_reset_cfg[0] = ulLine0;
  s_ptMmioCtrl->aulMmio_pio_oe_line_reset_cfg[1] = ulLine1;
  s_ptMmioCtrl->aulMmio_pio_oe_line_reset_cfg[2] = ulLine2;
  s_ptMmioCtrl->aulMmio_pio_oe_line_reset_cfg[3] = ulLine3;
}


/*****************************************************************************/
/*! MMIO Set All OE config
* \description
*   Set output enable for all MMIOs within out_line_cfg registers.
* \class
*   PIO
* \params
*   ulLine0         [in]  Bit0: MMIO0, ... Bit31: MMIO31
*   ulLine1         [in]  Bit0: MMIO32, ... Bit31: MMIO63
*   ulLine2         [in]  Bit0: MMIO64, ... Bit31: MMIO95
*   ulLine3         [in]  Bit0: MMIO96, ... Bit9: MMIO105
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_SetOeLine( uint32_t ulLine0,
                     uint32_t ulLine1,
                     uint32_t ulLine2,
                     uint32_t ulLine3 )
{
  s_ptMmioCtrl->aulMmio_pio_oe_line_cfg[0] = ulLine0;
  s_ptMmioCtrl->aulMmio_pio_oe_line_cfg[1] = ulLine1;
  s_ptMmioCtrl->aulMmio_pio_oe_line_cfg[2] = ulLine2;
  s_ptMmioCtrl->aulMmio_pio_oe_line_cfg[3] = ulLine3;
}

/*****************************************************************************/
/*! MMIO Get All OE config
* \description
*   Get output enable for all MMIOs within out_line_cfg registers.
* \class
*   PIO
* \params
*   ulLine0         [out]  Bit0: MMIO0,  ... Bit31: MMIO31
*   ulLine1         [out]  Bit0: MMIO32, ... Bit31: MMIO63
*   ulLine2         [out]  Bit0: MMIO64, ... Bit31: MMIO95
*   ulLine3         [out]  Bit0: MMIO96, ... Bit9: MMIO105
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_GetOeLine( uint32_t* pulLine0,
                     uint32_t* pulLine1,
                     uint32_t* pulLine2,
                     uint32_t* pulLine3 )
{
  *pulLine0 = s_ptMmioCtrl->aulMmio_pio_oe_line_cfg[0];
  *pulLine1 = s_ptMmioCtrl->aulMmio_pio_oe_line_cfg[1];
  if( NULL != pulLine2 ) {
    *pulLine2 = s_ptMmioCtrl->aulMmio_pio_oe_line_cfg[2];
  }
  if( NULL != pulLine3 ) {
    *pulLine3 = s_ptMmioCtrl->aulMmio_pio_oe_line_cfg[3];
  }
}

/*****************************************************************************/
/*! MMIO Set All out line config
* \description
*   Set out line config for all MMIOs within out_line_cfg registers.
* \class
*   PIO
* \params
*   ulLine0         [in]  Bit0: MMIO0,  ... Bit31: MMIO31
*   ulLine1         [in]  Bit0: MMIO32, ... Bit17: MMIO63
*   ulLine2         [in]  Bit0: MMIO64, ... Bit31: MMIO95
*   ulLine3         [in]  Bit0: MMIO96, ... Bit9: MMIO105
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_SetOutLineCfg( uint32_t ulLine0,
                         uint32_t ulLine1,
                         uint32_t ulLine2,
                         uint32_t ulLine3 )
{
  s_ptMmioCtrl->aulMmio_pio_out_line_cfg[0] = ulLine0;
  s_ptMmioCtrl->aulMmio_pio_out_line_cfg[1] = ulLine1;
  s_ptMmioCtrl->aulMmio_pio_out_line_cfg[2] = ulLine2;
  s_ptMmioCtrl->aulMmio_pio_out_line_cfg[3] = ulLine3;
}

/*****************************************************************************/
/*! MMIO Get All out line config
* \description
*   Get out line config for all MMIOs within out_line_cfg registers.
* \class
*   PIO
* \params
*   ulLine0         [out]  Bit0: MMIO0, ... Bit31: MMIO31
*   ulLine1         [out]  Bit0: MMIO32, ... Bit17: MMIO63
*   ulLine2         [out]  Bit0: MMIO64, ... Bit31: MMIO95
*   ulLine3         [out]  Bit0: MMIO96, ... Bit9: MMIO105
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_GetOutLineCfg( uint32_t* pulLine0,
                         uint32_t* pulLine1,
                         uint32_t* pulLine2,
                         uint32_t* pulLine3 )
{
  if( NULL != pulLine0 )
  {
    *pulLine0 = s_ptMmioCtrl->aulMmio_pio_out_line_cfg[0];
  }
  if( NULL != pulLine1 )
  {
    *pulLine1 = s_ptMmioCtrl->aulMmio_pio_out_line_cfg[1];
  }
  if( NULL != pulLine2 )
  {
    *pulLine2 = s_ptMmioCtrl->aulMmio_pio_out_line_cfg[2];
  }
  if( NULL != pulLine3 )
  {
    *pulLine3 = s_ptMmioCtrl->aulMmio_pio_out_line_cfg[3];
  }
}

/*****************************************************************************/
/*! MMIO Get All in line status lines
* \description
*   Get in line status for all MMIOs within out_line_cfg registers.
* \class
*   PIO
* \params
*   ulLine0         [out]  Bit0: MMIO0, ... Bit31: MMIO31
*   ulLine1         [out]  Bit0: MMIO32, ... Bit17: MMIO63
*   ulLine2         [out]  Bit0: MMIO64, ... Bit31: MMIO95
*   ulLine3         [out]  Bit0: MMIO96, ... Bit9: MMIO105
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_GetInLineStatus( uint32_t* pulLine0,
                           uint32_t* pulLine1,
                           uint32_t* pulLine2,
                           uint32_t* pulLine3 )
{
  if( NULL != pulLine0 )
  {
    *pulLine0 = s_ptMmioCtrl->aulMmio_in_line_status[0];
  }
  if( NULL != pulLine1 )
  {
    *pulLine1 = s_ptMmioCtrl->aulMmio_in_line_status[1];
  }
  if( NULL != pulLine2 )
  {
    *pulLine2 = s_ptMmioCtrl->aulMmio_in_line_status[2];
  }
  if( NULL != pulLine3 )
  {
    *pulLine3 = s_ptMmioCtrl->aulMmio_in_line_status[3];
  }
}

/*****************************************************************************/
/*! MMIO Get input line status
* \description
*   Get in line status.
* \class
*   PIO
* \params
*   uiLine  [in]  Line register number to read
* \return
*                                                                            */
/*****************************************************************************/
uint32_t MMIO_GetInLine( unsigned int uiLine )
{
  if( uiLine > 3 )
  {
    return 0;
  }
  return s_ptMmioCtrl->aulMmio_in_line_status[uiLine];
}
#endif

#if defined(__NETX90)
/*****************************************************************************/
/*! MMIO Write Pio out line set
* \description
*   -
* \class
*   PIO
* \params
*   ulLine0         [out]  Bit0: MMIO0, ... Bit31: MMIO31
*   ulLine1         [out]  Bit0: MMIO32, ... Bit17: MMIO63
*   ulLine2         [out]  Bit0: MMIO64, ... Bit31: MMIO95
*   ulLine3         [out]  Bit0: MMIO96, ... Bit9: MMIO105
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_PioOutLineCfgSet ( uint32_t ulLine0, uint32_t ulLine1, uint32_t ulLine2, uint32_t ulLine3 )
{
  s_ptMmioCtrl->ulMmio_pio_out_line_set_cfg0 = ulLine0;
}

/*****************************************************************************/
/*! MMIO Write Pio out line reset
* \description
*   -
* \class
*   PIO
* \params
*   ulLine0         [out]  Bit0: MMIO0, ... Bit31: MMIO31
*   ulLine1         [out]  Bit0: MMIO32, ... Bit17: MMIO63
*   ulLine2         [out]  Bit0: MMIO64, ... Bit31: MMIO95
*   ulLine3         [out]  Bit0: MMIO96, ... Bit9: MMIO105
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_PioOutLineCfgRst ( uint32_t ulLine0, uint32_t ulLine1, uint32_t ulLine2, uint32_t ulLine3 )
{
  s_ptMmioCtrl->ulMmio_pio_out_line_reset_cfg0 = ulLine0;
}

/*****************************************************************************/
/*! MMIO Write Pio oe line set
* \description
*   -
* \class
*   PIO
* \params
*   ulLine0         [out]  Bit0: MMIO0, ... Bit31: MMIO31
*   ulLine1         [out]  Bit0: MMIO32, ... Bit17: MMIO63
*   ulLine2         [out]  Bit0: MMIO64, ... Bit31: MMIO95
*   ulLine3         [out]  Bit0: MMIO96, ... Bit9: MMIO105
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_PioOeLineCfgSet ( uint32_t ulLine0, uint32_t ulLine1, uint32_t ulLine2, uint32_t ulLine3 )
{
  s_ptMmioCtrl->ulMmio_pio_oe_line_set_cfg0 = ulLine0;
}

/*****************************************************************************/
/*! MMIO Write Pio oe line reset
* \description
*   -
* \class
*   PIO
* \params
*   ulLine0         [out]  Bit0: MMIO0, ... Bit31: MMIO31
*   ulLine1         [out]  Bit0: MMIO32, ... Bit17: MMIO63
*   ulLine2         [out]  Bit0: MMIO64, ... Bit31: MMIO95
*   ulLine3         [out]  Bit0: MMIO96, ... Bit9: MMIO105
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_PioOeLineCfgRst ( uint32_t ulLine0, uint32_t ulLine1, uint32_t ulLine2, uint32_t ulLine3 )
{
  s_ptMmioCtrl->ulMmio_pio_oe_line_reset_cfg0 = ulLine0;
}


/*****************************************************************************/
/*! MMIO Set All OE config
* \description
*   Set output enable for all MMIOs within out_line_cfg registers.
* \class
*   PIO
* \params
*   ulLine0         [in]  Bit0: MMIO0, ... Bit31: MMIO31
*   ulLine1         [in]  Bit0: MMIO32, ... Bit31: MMIO63
*   ulLine2         [in]  Bit0: MMIO64, ... Bit31: MMIO95
*   ulLine3         [in]  Bit0: MMIO96, ... Bit9: MMIO105
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_SetOeLine( uint32_t ulLine0,
                     uint32_t ulLine1,
                     uint32_t ulLine2,
                     uint32_t ulLine3 )
{
  s_ptMmioCtrl->ulMmio_pio_oe_line_cfg0 = ulLine0;
}

/*****************************************************************************/
/*! MMIO Get All OE config
* \description
*   Get output enable for all MMIOs within out_line_cfg registers.
* \class
*   PIO
* \params
*   ulLine0         [out]  Bit0: MMIO0, ... Bit31: MMIO31
*   ulLine1         [out]  Bit0: MMIO32, ... Bit31: MMIO63
*   ulLine2         [out]  Bit0: MMIO64, ... Bit31: MMIO95
*   ulLine3         [out]  Bit0: MMIO96, ... Bit9: MMIO105
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_GetOeLine( uint32_t* pulLine0,
                     uint32_t* pulLine1,
                     uint32_t* pulLine2,
                     uint32_t* pulLine3 )
{
  *pulLine0 = s_ptMmioCtrl->ulMmio_pio_oe_line_cfg0;
}

/*****************************************************************************/
/*! MMIO Set All out line config
* \description
*   Set out line config for all MMIOs within out_line_cfg registers.
* \class
*   PIO
* \params
*   ulLine0         [in]  Bit0: MMIO0, ... Bit31: MMIO31
*   ulLine1         [in]  Bit0: MMIO32, ... Bit17: MMIO63
*   ulLine2         [in]  Bit0: MMIO64, ... Bit31: MMIO95
*   ulLine3         [in]  Bit0: MMIO96, ... Bit9: MMIO105
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_SetOutLineCfg( uint32_t ulLine0,
                         uint32_t ulLine1,
                         uint32_t ulLine2,
                         uint32_t ulLine3 )
{
  s_ptMmioCtrl->ulMmio_pio_out_line_cfg0 = ulLine0;
}

/*****************************************************************************/
/*! MMIO Get All out line config
* \description
*   Get out line config for all MMIOs within out_line_cfg registers.
* \class
*   PIO
* \params
*   ulLine0         [out]  Bit0: MMIO0, ... Bit31: MMIO31
*   ulLine1         [out]  Bit0: MMIO32, ... Bit17: MMIO63
*   ulLine2         [out]  Bit0: MMIO64, ... Bit31: MMIO95
*   ulLine3         [out]  Bit0: MMIO96, ... Bit9: MMIO105
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_GetOutLineCfg( uint32_t* pulLine0,
                         uint32_t* pulLine1,
                         uint32_t* pulLine2,
                         uint32_t* pulLine3 )
{
  *pulLine0 = s_ptMmioCtrl->ulMmio_pio_out_line_cfg0;
}

/*****************************************************************************/
/*! MMIO Get All in line status lines
* \description
*   Get in line status for all MMIOs within out_line_cfg registers.
* \class
*   PIO
* \params
*   ulLine0         [out]  Bit0: MMIO0, ... Bit31: MMIO31
*   ulLine1         [out]  Bit0: MMIO32, ... Bit17: MMIO63
*   ulLine2         [out]  Bit0: MMIO64, ... Bit31: MMIO95
*   ulLine3         [out]  Bit0: MMIO96, ... Bit9: MMIO105
* \return
*                                                                            */
/*****************************************************************************/
void MMIO_GetInLineStatus( uint32_t* pulLine0,
                           uint32_t* pulLine1,
                           uint32_t* pulLine2,
                           uint32_t* pulLine3 )
{
  *pulLine0 = s_ptMmioCtrl->ulMmio_in_line_status0;
}


/*****************************************************************************/
/*! MMIO Get input line status
* \description
*   Get in line status.
* \class
*   PIO
* \params
*   uiLine  [in]  Line register number to read
* \return
*                                                                            */
/*****************************************************************************/
uint32_t MMIO_GetInLine( unsigned int uiLine )
{
  if( uiLine != 0 )
  {
    return 0;
  }
  return s_ptMmioCtrl->ulMmio_in_line_status0;
}
#endif
