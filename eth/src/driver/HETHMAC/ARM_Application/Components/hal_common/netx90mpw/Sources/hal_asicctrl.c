/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "hal_asicctrl.h"
#include "hw_defines.h"

#define ASICCTRL_FIREWALL_FIRST s_ptAsicCtrl->ulFirewall_cfg_hifmem_sdram
#define ASICCTRL_FIREWALL_CNT  4

#define SRT_FIREWALL_WR     0
#define SRT_FIREWALL_RD     4
#define SRT_FIREWALL_ABT_EN 8

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
HW_PTR_ASIC_CTRL(s_ptAsicCtrl)
HW_PTR_SAMPLE_AT_PORN_STAT(s_ptSampleAtPornStat)

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/
#ifndef HALDEF
#define HALDEF(name) name
#endif

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

void HALDEF(AsicCtrl_SetIoConfig)(unsigned int uiIdx, uint32_t ulVal)
{
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->asIo_config[uiIdx - 1].ulConfig = ulVal;
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
*   current io_config value
*                                                                            */
/*****************************************************************************/
uint32_t HALDEF(AsicCtrl_GetIoConfig)(unsigned int uiIdx)
{
  return s_ptAsicCtrl->asIo_config[uiIdx - 1].ulConfig;
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
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->ulReset_ctrl           = ulVal;
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
  return s_ptAsicCtrl->ulReset_ctrl;
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
  uint32_t ulVal = s_ptAsicCtrl->ulReset_ctrl;

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

  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->ulReset_ctrl           = ulVal;
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
  return ( (s_ptAsicCtrl->ulReset_ctrl & HW_MSK(reset_ctrl_rst_out_n_in_ro)) >> HW_SRT(reset_ctrl_rst_out_n_in_ro) );
}


/*****************************************************************************/
/*! Set Clock_Enable
* \description
*   Writes a new value to Clock_Enable.
* \class
*   ASIC_CTRL
* \params
*   ulVal  [in]  Value for clock_enable
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_SetClockEnable)(uint32_t ulVal)
{
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->asClock_enable[0].ulEnable = ulVal;
}

/*****************************************************************************/
/*! Set Clock_Enable 1
* \description
*   Writes a new value to Clock_Enable1.
* \class
*   ASIC_CTRL
* \params
*   ulVal  [in]  Value for clock_enable
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_SetClockEnable1)(uint32_t ulVal)
{
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->asClock_enable[1].ulEnable = ulVal;
}

/*****************************************************************************/
/*! Get Clock_Enable
* \description
*   Read the value of Clock_Enable.
* \class
*   ASIC_CTRL
* \params
* \return
* current clock_enable value
*                                                                            */
/*****************************************************************************/
uint32_t HALDEF(AsicCtrl_GetClockEnable)(void)
{
  return s_ptAsicCtrl->asClock_enable[0].ulEnable;
}

/*****************************************************************************/
/*! Set Clock_Enable_Mask
* \description
*   Writes a new value to Clock_Enable Mask.
* \class
*   ASIC_CTRL
* \params
*   ulVal  [in]  Value for clock_enable mask
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_SetClockEnableMask)(uint32_t ulVal)
{
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->asClock_enable[0].ulMask;
}

/*****************************************************************************/
/*! Get Clock_Enable_Mask
* \description
*   Read the value of Clock_Enable Mask.
* \class
*   ASIC_CTRL
* \params
* \return
* current clock_enable value
*                                                                            */
/*****************************************************************************/
uint32_t HALDEF(AsicCtrl_GetClockEnableMask)(void)
{
  return s_ptAsicCtrl->asClock_enable[0].ulMask;
}

/*****************************************************************************/
/*! Get Clock_Enable_Mask 1
* \description
*   Read the value of Clock_Enable Mask 1.
* \class
*   ASIC_CTRL
* \params
* \return
* current clock_enable value
*                                                                            */
/*****************************************************************************/
uint32_t HALDEF(AsicCtrl_GetClockEnableMask1)(void)
{
  return s_ptAsicCtrl->asClock_enable[1].ulMask;
}

/*****************************************************************************/
/*! Set MISC_ASIC_CTRL
* \description
*   Writes a new value to Misc_Asic_Ctrl
* \class
*   ASIC_CTRL
* \params
*   ulVal  [in]  Value for Misc_Asic_Ctrl
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_SetMiscAsicCtrl)( uint32_t ulVal )
{
  s_ptAsicCtrl->ulMisc_asic_ctrl = ulVal;
}

/*****************************************************************************/
/*! Get MISC_ASIC_CTRL
* \description
*   Read the value of Misc_Asic_Ctrl
* \class
*   ASIC_CTRL
* \params
* \return
*   Current Misc_Asic_Ctrl value
*                                                                            */
/*****************************************************************************/
uint32_t HALDEF(AsicCtrl_GetMiscAsicCtrl)( void )
{
  return s_ptAsicCtrl->ulMisc_asic_ctrl;
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
  s_ptAsicCtrl->ulNetx_status = ulVal;
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
  return s_ptAsicCtrl->ulNetx_status;
}

/*****************************************************************************/
/*! Set System Status
* \description
*   Set the value of system status
* \class
*   ASIC_CTRL
* \params
*   ulVal  [in]  Value for System Status
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_SetSystemStatus)( uint32_t ulVal )
{
  s_ptAsicCtrl->ulSystem_status = ulVal;
}

/*****************************************************************************/
/*! Get System Status
* \description
*   Read the value of system status
* \class
*   ASIC_CTRL
* \params
* \return
*   Current System_Status value
*                                                                            */
/*****************************************************************************/
uint32_t HALDEF(AsicCtrl_GetSystemStatus)( void )
{
  return s_ptAsicCtrl->ulSystem_status;
}

/*****************************************************************************/
/*! Set netX Version
* \description
*   Set the value of netX version
* \class
*   ASIC_CTRL
* \params
*   ulVal  [in]  Value for netX Version
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_SetNetxVersion)( uint32_t ulVal )
{
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->ulNetx_version = ulVal;
}

/*****************************************************************************/
/*! Get netX Version
* \description
*   Read the value of netX version
* \class
*   ASIC_CTRL
* \params
* \return
*   Current Netx_Version value
*                                                                            */
/*****************************************************************************/
uint32_t HALDEF(AsicCtrl_GetNetxVersion)( void )
{
  return s_ptAsicCtrl->ulNetx_version;
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
  s_ptAsicCtrl->ulRdy_run_cfg = ((uint32_t)uRdyDrv << HW_SRT(rdy_run_cfg_RDY_DRV))
                               |((uint32_t)uRunDrv << HW_SRT(rdy_run_cfg_RUN_DRV))
                               |((uint32_t)uRdyPol << HW_SRT(rdy_run_cfg_RDY_POL))
                               |((uint32_t)uRunPol << HW_SRT(rdy_run_cfg_RUN_POL))
                               |((uint32_t)uRdy    << HW_SRT(rdy_run_cfg_RDY))
                               |((uint32_t)uRun    << HW_SRT(rdy_run_cfg_RUN));

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
  return s_ptAsicCtrl->ulRdy_run_cfg;
}

/*****************************************************************************/
/*! Set SYSTIME Control
* \description
*   Configures the SYSTIME MUX option for a specific module.
* \class
*   ASIC_CTRL
* \params
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_SetSystimeCtrl)( ASICCTRL_SYSTIME_MUX_E eSystimeMux, unsigned int uSystimeInst )
{
  switch(eSystimeMux)
  {
  case ASICCTRL_SYSTIME_MUX_FETH:
    s_ptAsicCtrl->ulSystime_feth_ctrl = uSystimeInst;
    break;
  case ASICCTRL_SYSTIME_MUX_GPIO:
#if CPU_IS_ARM_APP
    s_ptAsicCtrl->ulSystime_gpio_app_ctrl = uSystimeInst;
#else
    s_ptAsicCtrl->ulSystime_gpio_com_ctrl = uSystimeInst;
#endif
    break;
  default: /* invalid SYSTIME_MUX */
    break;
  } /* end switch */

}

/*****************************************************************************/
/*! Get SampleAtPorn Status
* \description
*   Get SampleAtPorn status.
* \class
*   ASIC_CTRL
* \params
* \return
*   Sample at porn status register content
*                                                                            */
/*****************************************************************************/
uint32_t HALDEF(AsicCtrl_GetSampleAtPornStat)( unsigned int uReg )
{
  return s_ptSampleAtPornStat->aulSample_at_porn_stat_in[uReg];
}

/*****************************************************************************/
/*! Configure FireWall
* \description
*   Configures FireWall.
* \class
*   ASIC_CTRL
* \params
* \return
*   void                                                                     */
/*****************************************************************************/
void HALDEF(AsicCtrl_CfgFirewall)( unsigned int uiFwIdx,
                                   unsigned int uWpVector,
                                   unsigned int uRpVector,
                                   unsigned int uAbortEnVector
                                 )
{
  if( uiFwIdx < ASICCTRL_FIREWALL_CNT )
  {
    volatile uint32_t* pulReg = (&ASICCTRL_FIREWALL_FIRST) + uiFwIdx;
    uint32_t ulVal;

    ulVal = (uWpVector      << SRT_FIREWALL_WR)
           |(uRpVector      << SRT_FIREWALL_RD)
           |(uAbortEnVector << SRT_FIREWALL_ABT_EN);

    /* write */
    s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
    *pulReg = ulVal;
  }
}

/*****************************************************************************/
/*! Enable FireWall interrupts
* \description
*   Enable FireWall interrupts.
* \class
*   ASIC_CTRL
* \params
* \return
*   void                                                                     */
/*****************************************************************************/
void HALDEF(AsicCtrl_EnFirewallIrq)( unsigned int uiFwIdx,
                                     unsigned int uEnVector
                                   )
{
  if( uiFwIdx < ASICCTRL_FIREWALL_CNT )
  {
    s_ptAsicCtrl->ulAsic_ctrl_irq_mask_set = uEnVector << (uiFwIdx * 2);
  }
}

/*****************************************************************************/
/*! Disable FireWall interrupts
* \description
*   Disable FireWall interrupts.
* \class
*   ASIC_CTRL
* \params
* \return
*   void                                                                     */
/*****************************************************************************/
void HALDEF(AsicCtrl_DisFirewallIrq)( unsigned int uiFwIdx,
                                      unsigned int uDisVector
                                    )
{
  if( uiFwIdx < ASICCTRL_FIREWALL_CNT )
  {
    s_ptAsicCtrl->ulAsic_ctrl_irq_mask_reset = uDisVector << (uiFwIdx * 2);
  }
}

/*****************************************************************************/
/*! Get FireWall Interrupts
* \description
*   Get FireWall interrupts.
* \class
*   ASIC_CTRL
* \params
* \return
*   unsigned int                                                             */
/*****************************************************************************/
unsigned int HALDEF(AsicCtrl_GetFirewallIrq)( unsigned int uiFwIdx )
{
  unsigned int uiFirewallIrq = 0;

  if( uiFwIdx < ASICCTRL_FIREWALL_CNT )
  {
    uint32_t ulStatus = s_ptAsicCtrl->ulAsic_ctrl_irq_masked;
    uiFirewallIrq = 0x3 & (ulStatus >> (uiFwIdx * 2));
  }

  return uiFirewallIrq;
}

/*****************************************************************************/
/*! Get FireWall raw interrupts
* \description
*   Get FireWall raw interrupts.
* \class
*   ASIC_CTRL
* \params
* \return
*   unsigned int                                                             */
/*****************************************************************************/
unsigned int HALDEF(AsicCtrl_GetFirewallIrqRaw)( unsigned int uiFwIdx )
{
  unsigned int uiFirewallIrqRaw = 0;

  if( uiFwIdx < ASICCTRL_FIREWALL_CNT )
  {
    uint32_t ulStatus = s_ptAsicCtrl->ulAsic_ctrl_irq_raw;
    uiFirewallIrqRaw = 0x3 & (ulStatus >> (uiFwIdx * 2));
  }

  return uiFirewallIrqRaw;
}

/*****************************************************************************/
/*! Confirm FireWall interrupts
* \description
*   Confirms FireWall interrupts.
* \class
*   ASIC_CTRL
* \params
* \return
*   unsigned int                                                             */
/*****************************************************************************/
void HALDEF(AsicCtrl_ConfirmFirewallIrq)( unsigned int uiFwIdx,
                                          unsigned int uConfirmVector
                                        )
{
  if( uiFwIdx < ASICCTRL_FIREWALL_CNT )
  {
    s_ptAsicCtrl->ulAsic_ctrl_irq_raw = uConfirmVector << (uiFwIdx * 2);
  }
}


/*****************************************************************************/
/*! Set Clock_Enable_Mask 1
* \description
*   Writes a new value to Clock_Enable Mask.
* \class
*   ASIC_CTRL
* \params
*   ulVal  [in]  Value for clock_enable
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_SetClockEnableMask1)(uint32_t ulVal)
{
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->asClock_enable[1].ulMask;
}

/*****************************************************************************/
/*! Get Clock_Enable 1
* \description
*   Read the value of Clock_Enable 1.
* \class
*   ASIC_CTRL
* \params
* \return
* current clock_enable value
*                                                                            */
/*****************************************************************************/
uint32_t HALDEF(AsicCtrl_GetClockEnable1)(void)
{
  return s_ptAsicCtrl->asClock_enable[1].ulEnable;
}
/*****************************************************************************/
/*! Get PHY_CTRL0
* \description
*   Get PHY Control 0 register
* \class
*   ASIC_CTRL
* \params
* \return
*   PHY Control configuration
*                                                                            */
/*****************************************************************************/
uint32_t HALDEF(AsicCtrl_GetPhyCtrl0)( void )
{
  return s_ptAsicCtrl->ulPhy_ctrl0;
}

/*****************************************************************************/
/*! Set PHY_CTRL0
* \description
*   Sets PHY Control 0 configuration.
* \class
*   ASIC_CTRL
* \params
*   ulSetMsk         [in]  set mask
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_SetPhyCtrl0)( uint32_t ulVal )
{
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->ulPhy_ctrl0 = ulVal;
}

/*****************************************************************************/
/*! BOD Fail IRQ Enable / Disable
* \description
*   Enable / disable interrupt request BOD fail.
* \class
*   ASIC_CTRL
* \params
*   fEn          [in]  true / false: enable/disable
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_IrqMskSet)(uint32_t ulIrq)
{
  s_ptAsicCtrl->ulAsic_ctrl_irq_mask_set = ulIrq;
}

/*****************************************************************************/
/*! BOD Fail IRQ Enable / Disable
* \description
*   Enable / disable interrupt request BOD fail.
* \class
*   ASIC_CTRL
* \params
*   fEn          [in]  true / false: enable/disable
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_IrqMskClr)(uint32_t ulIrq)
{
  s_ptAsicCtrl->ulAsic_ctrl_irq_mask_reset = ulIrq;
}

/*****************************************************************************/
/*! BOD Fail Get IRQ
* \description
*   Read masked interrupt status of BOD fail IRQ.
* \class
*   ASIC_CTRL                                                                */
/*****************************************************************************/
uint32_t HALDEF(AsicCtrl_GetIrq)(void)
{
  return s_ptAsicCtrl->ulAsic_ctrl_irq_masked;
}

/*****************************************************************************/
/*! BOD Fail Get Raw IRQ
* \description
*   Read interrupt status of BOD fail IRQ.
* \class
*   ASIC_CTRL                                                                */
/*****************************************************************************/
uint32_t HALDEF(AsicCtrl_GetIrqRaw)(void)
{
  return s_ptAsicCtrl->ulAsic_ctrl_irq_raw;
}

/*****************************************************************************/
/*! GPIO IRQ Reset
* \description
*   Reset the BOD fail IRQ.
* \class
*   ASIC_CTRL
* \params
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_IrqReset)(uint32_t ulIrq)
{
  s_ptAsicCtrl->ulAsic_ctrl_irq_raw = ulIrq;
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
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->ulOnly_porn = ulVal;
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
  return s_ptAsicCtrl->ulOnly_porn;
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
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->ulOnly_porn_rom = ulVal;
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
  return s_ptAsicCtrl->ulOnly_porn_rom;
}
