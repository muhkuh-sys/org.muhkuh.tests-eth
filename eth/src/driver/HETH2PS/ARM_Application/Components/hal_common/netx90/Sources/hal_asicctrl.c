/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "hal_asicctrl.h"
#include "hw_defines.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
HW_PTR_ASIC_CTRL(s_ptAsicCtrl)
HW_PTR_SAMPLE_AT_PORN_STAT(s_ptSampleAtPornStat)

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

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
void AsicCtrl_SetIoConfig(unsigned int uiIdx, uint32_t ulVal)
{
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->asIo_config[uiIdx - 1].ulConfig = ulVal | 0xffff0000U;
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
uint32_t AsicCtrl_GetIoConfig(unsigned int uiIdx)
{
  return s_ptAsicCtrl->asIo_config[uiIdx - 1].ulConfig;
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
void AsicCtrl_SetClockEnable(uint32_t ulVal)
{
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->asClock_enable[0].ulEnable = ulVal | 0xffff0000;
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
void AsicCtrl_SetClockEnable1(uint32_t ulVal)
{
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->asClock_enable[1].ulEnable = ulVal | 0xffff0000;
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
uint32_t AsicCtrl_GetClockEnable(void)
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
void AsicCtrl_SetClockEnableMask(uint32_t ulVal)
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
uint32_t AsicCtrl_GetClockEnableMask(void)
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
uint32_t AsicCtrl_GetClockEnableMask1(void)
{
  return s_ptAsicCtrl->asClock_enable[1].ulMask;
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
void AsicCtrl_SetSystemStatus( uint32_t ulVal )
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
uint32_t AsicCtrl_GetSystemStatus( void )
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
void AsicCtrl_SetNetxVersion( uint32_t ulVal )
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
uint32_t AsicCtrl_GetNetxVersion( void )
{
  return s_ptAsicCtrl->ulNetx_version;
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
void AsicCtrl_SetSystimeCtrl( ASICCTRL_SYSTIME_MUX_E eSystimeMux, unsigned int uSystimeInst )
{
  switch(eSystimeMux)
  {
  case ASICCTRL_SYSTIME_MUX_FETH:
    s_ptAsicCtrl->ulSystime_eth_system_ctrl = uSystimeInst;
    break;
  case ASICCTRL_SYSTIME_MUX_GPIO:
#if defined(CPU_IS_ARM_APP)
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
uint32_t AsicCtrl_GetSampleAtPornStat( unsigned int uReg )
{
  return s_ptSampleAtPornStat->aulSample_at_porn_stat_in[uReg];
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
void AsicCtrl_SetClockEnableMask1(uint32_t ulVal)
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
uint32_t AsicCtrl_GetClockEnable1(void)
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
uint32_t AsicCtrl_GetPhyCtrl0( void )
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
void AsicCtrl_SetPhyCtrl0( uint32_t ulVal )
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
void AsicCtrl_IrqMskSet(uint32_t ulIrq)
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
void AsicCtrl_IrqMskClr(uint32_t ulIrq)
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
uint32_t AsicCtrl_GetIrq(void)
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
uint32_t AsicCtrl_GetIrqRaw(void)
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
void AsicCtrl_IrqReset(uint32_t ulIrq)
{
  s_ptAsicCtrl->ulAsic_ctrl_irq_raw = ulIrq;
}

/*****************************************************************************/
/*! Get Measured CLK2 RC Length
* \description
*   Get measured minimum/maximum RC-OSC clock period in cycles of system clock (10ns)
* \class
*   ASIC_CTRL
* \params
* \return
*                                                                            */
/*****************************************************************************/
void AsicCtrl_GetClk2RcLen(uint32_t* pulMin, uint32_t* pulMax )
{
  *pulMin = s_ptAsicCtrl->ulAsic_ctrl_clk2rc_length_min;
  *pulMax = s_ptAsicCtrl->ulAsic_ctrl_clk2rc_length_max;
}

/*****************************************************************************/
/*! Reset Measured CLK2 RC Length
* \description
*   Reset measured minimum/maximum RC-OSC clock period registers.
*   Set clk2rc_length_min register to 0x3ff to find a new minimum value
*   *   Set clk2rc_length_max register to 0 to find a new maximum value.
* \class
*   ASIC_CTRL
* \params
* \return
*                                                                            */
/*****************************************************************************/
void AsicCtrl_ResetClk2RcLen(void)
{
  s_ptAsicCtrl->ulAsic_ctrl_clk2rc_length_min = 0x3ff;
  s_ptAsicCtrl->ulAsic_ctrl_clk2rc_length_max = 0;
}

