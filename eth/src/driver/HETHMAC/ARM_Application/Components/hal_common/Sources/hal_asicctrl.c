/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "hal_asicctrl.h"
#include "hw_defines.h"

#define ASICCTRL_FIREWALL_FIRST s_ptAsicCtrl->aulFirewall_cfg_netx_ram[0]
#define ASICCTRL_FIREWALL_CNT 10

#define SRT_FIREWALL_WR     0
#define SRT_FIREWALL_RD     4
#define SRT_FIREWALL_ABT_EN 8

typedef struct ASICCTRL_FIREWALLS_Ttag
{
  volatile uint32_t aulFirewall_cfg[ASICCTRL_FIREWALL_CNT];
} ASICCTRL_FIREWALLS_T;

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
HW_PTR_ASIC_CTRL(s_ptAsicCtrl)
HW_PTR_SAMPLE_AT_PORN_STAT(s_ptSampleAtPornStat)
HW_PTR_MOTION(s_ptMotion)

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
  switch(uiIdx)
  {
  case 1:
    s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
    s_ptAsicCtrl->ulIo_config = ulVal;
    break;
  case 2:
    s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
    s_ptAsicCtrl->ulIo_config2 = ulVal;
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
*   current io_config value
*                                                                            */
/*****************************************************************************/
uint32_t HALDEF(AsicCtrl_GetIoConfig)(unsigned int uiIdx)
{
  uint32_t ulReturnVal = 0;

  switch(uiIdx)
  {
  case 1:
    ulReturnVal = s_ptAsicCtrl->ulIo_config;
    break;
  case 2:
    ulReturnVal = s_ptAsicCtrl->ulIo_config2;
    break;
  default:
    break;
  }
  return ulReturnVal;
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
    ulVal &= ~HW_MSK(reset_ctrl_rst_out_n_out); /* standard PIO function */
  }
  else
  {
    /* drive high */
    ulVal |= HW_MSK(reset_ctrl_rst_out_n_out); /* standard PIO function */
  }

  if(fOe)
  {
    /* enable OE */
    ulVal |= HW_MSK(reset_ctrl_rst_out_n_oe);
  }
  else
  {
    /* disable OE */
    ulVal &= ~HW_MSK(reset_ctrl_rst_out_n_oe);
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
  s_ptAsicCtrl->ulClock_enable = ulVal;
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
  return s_ptAsicCtrl->ulClock_enable;
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
  s_ptAsicCtrl->ulClock_enable_mask = ulVal;
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
  return s_ptAsicCtrl->ulClock_enable_mask;
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
  case ASICCTRL_SYSTIME_MUX_MOTION:
    s_ptMotion->ulMotion_systime_config = uSystimeInst;
    break;
  default:
  {
    unsigned int uSrt = (unsigned int)eSystimeMux << 1;
    uint32_t ulSystimeCtrl = s_ptAsicCtrl->ulSystime_ctrl;

    /* set bit field to desired value */
    ulSystimeCtrl &= ~(3UL << uSrt);
    ulSystimeCtrl |= (uint32_t)uSystimeInst << uSrt;

    /* write back value */
    s_ptAsicCtrl->ulSystime_ctrl = ulSystimeCtrl;
  }
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
    ASICCTRL_FIREWALLS_T* ptFw = (ASICCTRL_FIREWALLS_T*)&ASICCTRL_FIREWALL_FIRST;
    uint32_t ulVal;

    ulVal = (uWpVector      << SRT_FIREWALL_WR)
           |(uRpVector      << SRT_FIREWALL_RD)
           |(uAbortEnVector << SRT_FIREWALL_ABT_EN);

    /* write */
    s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
    ptFw->aulFirewall_cfg[uiFwIdx] = ulVal;
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
    volatile uint32_t* pulReg = (&ASICCTRL_FIREWALL_FIRST) + uiFwIdx;
    uint32_t ulVal = *pulReg;

    /* clear flags (do not clear status) */
    ulVal &= ~( HW_MSK(firewall_cfg_netx_ram0_stat_cr7)
               |HW_MSK(firewall_cfg_netx_ram0_stat_ca9)
               |HW_MSK(firewall_cfg_netx_ram0_stat_coresight)
               |HW_MSK(firewall_cfg_netx_ram0_stat_rest)
              );
    /* set interrupt enable flags */
    ulVal |= (uEnVector << HW_SRT(firewall_cfg_netx_ram0_irq_en_cr7));

    /* write back */
    s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
    *pulReg = ulVal;
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
    volatile uint32_t* pulReg = (&ASICCTRL_FIREWALL_FIRST) + uiFwIdx;
    uint32_t ulVal = *pulReg;

    /* clear flags (do not clear status) */
    ulVal &= ~( HW_MSK(firewall_cfg_netx_ram0_stat_cr7)
               |HW_MSK(firewall_cfg_netx_ram0_stat_ca9)
               |HW_MSK(firewall_cfg_netx_ram0_stat_coresight)
               |HW_MSK(firewall_cfg_netx_ram0_stat_rest)
              );
    /* clear interrupt enable flags */
    ulVal &= ~(uDisVector << HW_SRT(firewall_cfg_netx_ram0_irq_en_cr7));

    /* write back */
    s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
    *pulReg = ulVal;
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
  unsigned int uFirewallIrq = 0;

  if( uiFwIdx < ASICCTRL_FIREWALL_CNT )
  {
    volatile uint32_t* pulReg = (&ASICCTRL_FIREWALL_FIRST) + uiFwIdx;
    uint32_t ulVal  = *pulReg;
    uint32_t ulStatus = (ulVal >> HW_SRT(firewall_cfg_netx_ram0_stat_cr7)) & 0xf;
    uint32_t ulIrqEn  = (ulVal >> HW_SRT(firewall_cfg_netx_ram0_irq_en_cr7)) & 0xf;

    uFirewallIrq = ulStatus & ulIrqEn;
  }

  return uFirewallIrq;
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
  unsigned int uFirewallIrqRaw = 0;

  if( uiFwIdx < ASICCTRL_FIREWALL_CNT )
  {
    /* get value */
    volatile uint32_t* pulReg = (&ASICCTRL_FIREWALL_FIRST) + uiFwIdx;
    uint32_t ulVal = *pulReg;

    uFirewallIrqRaw = (ulVal >> HW_SRT(firewall_cfg_netx_ram0_stat_cr7)) & 0xf;
  }

  return uFirewallIrqRaw;
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
    /* get value */
    volatile uint32_t* pulReg = (&ASICCTRL_FIREWALL_FIRST) + uiFwIdx;
    uint32_t ulVal = *pulReg;

    /* do not clear status */
    ulVal &= ~( HW_MSK(firewall_cfg_netx_ram0_stat_cr7)
               |HW_MSK(firewall_cfg_netx_ram0_stat_ca9)
               |HW_MSK(firewall_cfg_netx_ram0_stat_coresight)
               |HW_MSK(firewall_cfg_netx_ram0_stat_rest)
              );

    /* clear status */
    ulVal |= (uConfirmVector << HW_SRT(firewall_cfg_netx_ram0_stat_cr7));

    /* write back */
    s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
    *pulReg = ulVal;
  }
}


/*****************************************************************************/
/*! Set ROM WatchDog
* \description
*   Write the ROM Watchdog
* \class
*   ASIC_CTRL
* \params
*   ulVal  [in]  Value to set
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_SetRomWdg)( uint32_t ulVal )
{
  s_ptAsicCtrl->ulRom_wdg = ulVal;
}

/*****************************************************************************/
/*! Set AdcClk
* \description
*   Configure rate multiplier for ADC clock.
*   ulAdcClkDiv = 200/f - 1
*   ulAdcClkDiv=9: f=20 MHz (default)
*   ulAdcClkDiv=4: f=40 MHz
* \class
*   ASIC_CTRL
* \params
*   ulAdcClkDiv  [in]  Value for Rate multiplier
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_SetAdcClk)(uint32_t ulAdcClkDiv)
{
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->ulAdcclk_div = ulAdcClkDiv;
}

/*****************************************************************************/
/*! Get DEBUG_CTRL Status
* \description
*   Get Debug control status
* \class
*   ASIC_CTRL
* \params
* \return
*   Current Debug Control configuration
*                                                                            */
/*****************************************************************************/
uint32_t HALDEF(AsicCtrl_GetDebugCtrlStatus)( void )
{
  return s_ptAsicCtrl->ulDebug_ctrl_status;
}

/*****************************************************************************/
/*! Set DEBUG_CTRL
* \description
*   Sets Debug Control configuration.
* \class
*   ASIC_CTRL
* \params
*   ulSetMsk         [in]  set mask
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_SetDebugCtrl)( uint32_t ulSetMsk )
{
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->ulDebug_ctrl_set = ulSetMsk;
}

/*****************************************************************************/
/*! Reset DEBUG_CTRL
* \description
*   Resets Debug Control configuration.
* \class
*   ASIC_CTRL
* \params
*   ulSetMsk         [in]  reset mask
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_ResetDebugCtrl)( uint32_t ulResetMsk )
{
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->ulDebug_ctrl_reset = ulResetMsk;
}

/*****************************************************************************/
/*! Set netX Master Stat0
* \description
*   Sets netX Master Status 0 registers.
* \class
*   ASIC_CTRL
* \params
*   ulSetMsk         [in]  value to set
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_SetNetxMasterStat0)( uint32_t ulVal )
{
  s_ptAsicCtrl->aulNetx_master_stat[0] = ulVal;
}

/*****************************************************************************/
/*! Get netX Master Stat0
* \description
*   Sets netX Master Status registers.
* \class
*   ASIC_CTRL
* \params
*   pulStat0         [in]  Buffer to stat0
*   pulStat1         [in]  Buffer to stat1
* \return
*                                                                            */
/*****************************************************************************/
void HALDEF(AsicCtrl_GetNetxMasterStat)( uint32_t* pulStat0, uint32_t* pulStat1 )
{
  *pulStat0 = s_ptAsicCtrl->aulNetx_master_stat[0];
  *pulStat1 = s_ptAsicCtrl->aulNetx_master_stat[1];
}
