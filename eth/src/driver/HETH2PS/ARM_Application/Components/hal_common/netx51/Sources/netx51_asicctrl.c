/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx51_asicctrl.h"
#include "hal_resources_defines_netx51.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_ASIC_CTRL

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
*   ulVal  [in]  Value for io_config
* \return
*                                                                            */
/*****************************************************************************/
void NX51_AsicCtrl_SetIoConfig( uint32_t ulVal )
{
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->ulIo_config            = ulVal;
}

/*****************************************************************************/
/*! Get IO_Config
* \description
*   Read the value of IO_Config.
* \class
*   ASIC_CTRL
* \params
* \return
*   Current io_config value
*                                                                            */
/*****************************************************************************/
uint32_t NX51_AsicCtrl_GetIoConfig( void )
{
  return s_ptAsicCtrl->ulIo_config;
}

/*****************************************************************************/
/*! Set IO_Config2
* \description
*   Writes a new value to IO_Config2.
* \class
*   ASIC_CTRL
* \params
*   ulVal  [in]  Value for io_config2
* \return
*                                                                            */
/*****************************************************************************/
void NX51_AsicCtrl_SetIoConfig2( uint32_t ulVal )
{
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->ulIo_config2           = ulVal;
}

/*****************************************************************************/
/*! Get IO_Config2
* \description
*   Read the value of IO_Config2.
* \class
*   ASIC_CTRL
* \params
* \return
*   Current io_config2 value
*                                                                            */
/*****************************************************************************/
uint32_t NX51_AsicCtrl_GetIoConfig2( void )
{
  return s_ptAsicCtrl->ulIo_config2;
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
void NX51_AsicCtrl_SetClockEnable( uint32_t ulVal )
{
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->ulClock_enable         = ulVal;
}

/*****************************************************************************/
/*! Get Clock_Enable
* \description
*   Read the value of Clock_Enable.
* \class
*   ASIC_CTRL
* \params
* \return
*   Current clock_enable value
*                                                                            */
/*****************************************************************************/
uint32_t NX51_AsicCtrl_GetClockEnable( void )
{
  return s_ptAsicCtrl->ulClock_enable;
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
void NX51_AsicCtrl_SetResetCtrl( uint32_t ulVal )
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
uint32_t NX51_AsicCtrl_GetResetCtrl( void )
{
  return s_ptAsicCtrl->ulReset_ctrl;
}
