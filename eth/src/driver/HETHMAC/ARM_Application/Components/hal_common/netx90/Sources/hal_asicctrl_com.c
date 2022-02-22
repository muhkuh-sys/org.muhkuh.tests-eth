/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "hal_asicctrl.h"
#include "hw_defines.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
HW_PTR_ASIC_CTRL_COM(s_ptAsicCtrlCom)
HW_PTR_ASIC_CTRL(s_ptAsicCtrl_key)

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/
#ifndef HALDEF
#define HALDEF(name) name
#endif

/*****************************************************************************/
/*! Set Lock
* \description
*   Set netx_lock Register.
* \class
*   ASIC_CTRL
* \params
* \return
*                                                                            */
/*****************************************************************************/

void HALDEF(AsicCtrl_SetLock)(uint32_t ulVal)
{
  HW_WR32(s_ptAsicCtrlCom->ulNetx_lock, ulVal);
}

/*****************************************************************************/
/*! Get Lock
* \description
*   Read Lock register.
* \class
*   ASIC_CTRL                                                                */
/*****************************************************************************/
uint32_t HALDEF(AsicCtrl_GetLock)(void)
{
  return HW_RD32(s_ptAsicCtrlCom->ulNetx_lock);
}

/*****************************************************************************/
/*! Set Reset_Ctrl
* \description
*   Writes a new value to Reset_Ctrl.
* \class
*   ASIC_CTRL
* \params
*   ulVal  [in]  Value for reset_ctrl
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_SetResetCtrl)( uint32_t ulVal )
{
  HW_WR32(s_ptAsicCtrl_key->ulAsic_ctrl_access_key, HW_RD32(s_ptAsicCtrl_key->ulAsic_ctrl_access_key));
  HW_WR32(s_ptAsicCtrlCom->ulReset_ctrl, ulVal);
}

/*****************************************************************************/
/*! Get Reset_Ctrl
* \description
*   Read the value of Reset_Ctrl.
* \class
*   ASIC_CTRL
* \params
* \return
*   Current reset_ctrl value
*                                                                            */
/*****************************************************************************/
uint32_t HALDEF(AsicCtrl_GetResetCtrl)( void )
{
  return HW_RD32(s_ptAsicCtrlCom->ulReset_ctrl);
}

/*****************************************************************************/
/*! Set Reset_Out
* \description
*   Sets Reset Out.
* \class
*   ASIC_CTRL
* \params
*   fDrvLow [in]  true/false: set RESET_OUT to low/high
*   fOe     [in]  true: enable OE
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_SetResetOut)( bool fDrvLow, bool fOe )
{
  uint32_t ulVal = HW_RD32(s_ptAsicCtrlCom->ulReset_ctrl);

  if(fDrvLow)
  {
    /* drive low */
    ulVal &= ~HW_MSK(reset_ctrl_RES_REQ_OUT); /* low active */
  }
  else
  {
    /* drive high */
    ulVal |= HW_MSK(reset_ctrl_RES_REQ_OUT); /* low active */
  }

  if(fOe)
  {
    /* enable OE */
    ulVal |= HW_MSK(reset_ctrl_EN_RES_REQ_OUT);
  }
  else
  {
    /* disable OE */
    ulVal &= ~HW_MSK(reset_ctrl_EN_RES_REQ_OUT);
  }

  HW_WR32(s_ptAsicCtrl_key->ulAsic_ctrl_access_key, HW_RD32(s_ptAsicCtrl_key->ulAsic_ctrl_access_key));
  HW_WR32(s_ptAsicCtrlCom->ulReset_ctrl, ulVal);
}

/*****************************************************************************/
/*! Get Reset_Out input value
* \description
*   Read input status Reset Out.
* \class
*   ASIC_CTRL                                                                */
/*****************************************************************************/
unsigned int HALDEF(AsicCtrl_GetResetOutInput)(void)
{
  return ( (HW_RD32(s_ptAsicCtrlCom->ulReset_ctrl) & HW_MSK(reset_ctrl_rst_out_n_in_ro)) >> HW_SRT(reset_ctrl_rst_out_n_in_ro) );
}

/*****************************************************************************/
/*! Set netX Status
* \description
*   Set the value of netX status
* \class
*   ASIC_CTRL
* \params
*   ulVal  [in]  Value for netX Status
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_SetNetxStatus)( uint32_t ulVal )
{
  HW_WR32(s_ptAsicCtrlCom->ulNetx_status, ulVal);
}

/*****************************************************************************/
/*! Get netX Status
* \description
*   Read the value of netX status
* \class
*   ASIC_CTRL
* \params
* \return
*   Current Netx_Status value
*                                                                            */
/*****************************************************************************/
uint32_t HALDEF(AsicCtrl_GetNetxStatus)( void )
{
  return HW_RD32(s_ptAsicCtrlCom->ulNetx_status);
}

/*****************************************************************************/
/*! Set RDYRUN Configuration
* \description
*   Set RDY/RUN configuration.
* \class
*   ASIC_CTRL
* \params
*   uRdyDrv          [in]  RDY driver enable
*   uRunDrv          [in]  RUN driver enable
*   uRdyPol          [in]  RDY output polarity
*   uRunPol          [in]  RUN output polarity
*   uRdy             [in]  RDY output
*   uRun             [in]  RUN output
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_SetRdyRunCfg)( unsigned int uRdyDrv,
                                    unsigned int uRunDrv,
                                    unsigned int uRdyPol,
                                    unsigned int uRunPol,
                                    unsigned int uRdy,
                                    unsigned int uRun
                                  )
{
  HW_WR32(s_ptAsicCtrlCom->ulRdy_run_cfg,
      ((uint32_t)uRdyDrv << HW_SRT(rdy_run_cfg_RDY_DRV))
     |((uint32_t)uRunDrv << HW_SRT(rdy_run_cfg_RUN_DRV))
     |((uint32_t)uRdyPol << HW_SRT(rdy_run_cfg_RDY_POL))
     |((uint32_t)uRunPol << HW_SRT(rdy_run_cfg_RUN_POL))
     |((uint32_t)uRdy    << HW_SRT(rdy_run_cfg_RDY))
     |((uint32_t)uRun    << HW_SRT(rdy_run_cfg_RUN)));

}

/*****************************************************************************/
/*! Get RDYRUN Configuration
* \description
*   Get RDY/RUN configuration.
* \class
*   ASIC_CTRL
* \params
* \return
*   Current RDY/RUN configuration value
*                                                                            */
/*****************************************************************************/
uint32_t HALDEF(AsicCtrl_GetRdyRunCfg)( void )
{
  return HW_RD32(s_ptAsicCtrlCom->ulRdy_run_cfg);
}

/*****************************************************************************/
/*! Set ONLY_PORN
* \description
*   Sets ONLY_PORN register..
* \class
*   ASIC_CTRL
* \params
*   ulSetMsk         [in]  set mask
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_SetOnlyPorn)( uint32_t ulVal )
{
  HW_WR32(s_ptAsicCtrl_key->ulAsic_ctrl_access_key, HW_RD32(s_ptAsicCtrl_key->ulAsic_ctrl_access_key));
  HW_WR32(s_ptAsicCtrlCom->ulOnly_porn, ulVal);
}

/*****************************************************************************/
/*! Get ONLY_PORN
* \description
*   Read ONLY_PORN register.
* \class
*   ASIC_CTRL                                                                */
/*****************************************************************************/
uint32_t HALDEF(AsicCtrl_GetOnlyPorn)(void)
{
  return HW_RD32(s_ptAsicCtrlCom->ulOnly_porn);
}

/*****************************************************************************/
/*! Set ONLY_PORN_ROM
* \description
*   Sets ONLY_PORN_ROM register..
* \class
*   ASIC_CTRL
* \params
*   ulSetMsk         [in]  set mask
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_SetOnlyPornRom)( uint32_t ulVal )
{
  HW_WR32(s_ptAsicCtrl_key->ulAsic_ctrl_access_key, HW_RD32(s_ptAsicCtrl_key->ulAsic_ctrl_access_key));
  HW_WR32(s_ptAsicCtrlCom->ulOnly_porn_rom, ulVal);
}

/*****************************************************************************/
/*! Get ONLY_PORN_ROM
* \description
*   Read ONLY_PORN_ROM register.
* \class
*   ASIC_CTRL                                                                */
/*****************************************************************************/
uint32_t HALDEF(AsicCtrl_GetOnlyPornRom)(void)
{
  return HW_RD32(s_ptAsicCtrlCom->ulOnly_porn_rom);
}

/*****************************************************************************/
/*! Set SDRAM_SPLIT
* \description
*   Sets SDRAM_SPLIT register.
* \class
*   ASIC_CTRL
* \params
*   ulVal         [in]  set value
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_SetSdramSplit)( uint32_t ulVal )
{
#ifndef __NETX90_MPW
  HW_WR32(s_ptAsicCtrlCom->ulSplit_sdram, ulVal);
#endif
}

/*****************************************************************************/
/*! Get SDRAM_SPLIT
* \description
*   Read SDRAM_SPLIT register.
* \class
*   ASIC_CTRL                                                                */
/*****************************************************************************/
uint32_t HALDEF(AsicCtrl_GetSdramSplit)(void)
{
#ifndef __NETX90_MPW
  return HW_RD32(s_ptAsicCtrlCom->ulSplit_sdram);
#endif
}

/*****************************************************************************/
/*! Set System Calibration
* \description
*   Sets System Calibration register.
* \class
*   ASIC_CTRL
* \params
*   uiRegNum      [in]  Register number
*   ulVal         [in]  set value
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_SetSystemCalib)( unsigned int uiRegNum, uint32_t ulVal )
{
#ifndef __NETX90_MPW
  switch( uiRegNum )
  {
    case 0:
    /* fall through */
    case 1:
    /* fall through */
    case 2:
    /* fall through */
    case 3: HW_WR32(s_ptAsicCtrlCom->aulSystem_calibration[uiRegNum], ulVal);
            break;

    case 4: HW_WR32(s_ptAsicCtrlCom->ulSystem_calibration4, ulVal);
            break;

    case 5: HW_WR32(s_ptAsicCtrlCom->ulSystem_calibration5, ulVal);
            break;

    default: break;
  }
#endif
}

/*****************************************************************************/
/*! Get System Calibration
* \description
*   Read System Calibration register.
* \class
*   ASIC_CTRL
* \params
*   uiRegNum      [in]  Register number                                      */
/*****************************************************************************/
uint32_t HALDEF(AsicCtrl_GetSystemCalib)( unsigned int uiRegNum )
{
  uint32_t ulRet = 0;
#ifndef __NETX90_MPW
  switch( uiRegNum )
  {
    case 0:
    /* fall through */
    case 1:
    /* fall through */
    case 2:
    /* fall through */
    case 3: ulRet =  HW_RD32(s_ptAsicCtrlCom->aulSystem_calibration[uiRegNum]); break;
    case 4: ulRet =  HW_RD32(s_ptAsicCtrlCom->ulSystem_calibration4); break;
    case 5: ulRet =  HW_RD32(s_ptAsicCtrlCom->ulSystem_calibration5); break;

    default: break;
  }
#endif
  return ulRet;
}

/*****************************************************************************/
/*! Set System Control
* \description
*   Sets System Control register.
* \class
*   ASIC_CTRL
* \params
*   ulVal         [in]  set value
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_SetSystemCtrl)( uint32_t ulVal )
{
#ifndef __NETX90_MPW
  HW_WR32(s_ptAsicCtrlCom->ulSystem_ctrl, ulVal);
#endif
}

/*****************************************************************************/
/*! Get System Control
* \description
*   Read System Control register.
* \class
*   ASIC_CTRL                                                                */
/*****************************************************************************/
uint32_t HALDEF(AsicCtrl_GetSystemCtrl)( void)
{
#ifndef __NETX90_MPW
  return HW_RD32(s_ptAsicCtrlCom->ulSystem_ctrl);
#endif
}
