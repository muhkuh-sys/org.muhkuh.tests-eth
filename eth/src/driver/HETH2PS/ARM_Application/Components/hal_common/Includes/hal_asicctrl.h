#ifndef __HAL_ASICCTRL_H
#define __HAL_ASICCTRL_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
  ASICCTRL_SYSTIME_MUX_FETH   = 0,
  ASICCTRL_SYSTIME_MUX_GPIO   = 1,
  ASICCTRL_SYSTIME_MUX_GMAC   = 2,
  ASICCTRL_SYSTIME_MUX_MOTION = 3
} ASICCTRL_SYSTIME_MUX_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

void     AsicCtrl_SetIoConfig          ( unsigned int uIdx, uint32_t ulVal );
uint32_t AsicCtrl_GetIoConfig          ( unsigned int uIdx );

void     AsicCtrl_SetResetCtrl         ( uint32_t ulVal );
uint32_t AsicCtrl_GetResetCtrl         ( void );

void     AsicCtrl_SetClockEnable       ( uint32_t ulVal );
void     AsicCtrl_SetClockEnable1      ( uint32_t ulVal );
uint32_t AsicCtrl_GetClockEnable       ( void );
uint32_t AsicCtrl_GetClockEnable1      ( void );
void     AsicCtrl_SetClockEnableMask   ( uint32_t ulVal );
void     AsicCtrl_SetClockEnableMask1  ( uint32_t ulVal );
uint32_t AsicCtrl_GetClockEnableMask   ( void );
uint32_t AsicCtrl_GetClockEnableMask1  ( void );
void     AsicCtrl_SetMiscAsicCtrl      ( uint32_t ulVal );
uint32_t AsicCtrl_GetMiscAsicCtrl      ( void );
void     AsicCtrl_SetAdcClk            ( uint32_t ulAdcClkDiv );
void     AsicCtrl_SetFbClk             ( unsigned int uXc,
                                                unsigned int uXpec,
                                                uint32_t ulFbClkRateMulAdd,
                                                uint32_t ulFbClkDiv );
void     AsicCtrl_SetNetxStatus        ( uint32_t ulVal );
uint32_t AsicCtrl_GetNetxStatus        ( void );
void     AsicCtrl_SetRomWdg            ( uint32_t ulVal );
void     AsicCtrl_SetSystemStatus      ( uint32_t ulVal );
uint32_t AsicCtrl_GetSystemStatus      ( void );
void     AsicCtrl_SetNetxVersion       ( uint32_t ulVal );
uint32_t AsicCtrl_GetNetxVersion       ( void );
void     AsicCtrl_SetRdyRunCfg         ( unsigned int uRdyDrv,
                                         unsigned int uRunDrv,
                                         unsigned int uRdyPol,
                                         unsigned int uRunPol,
                                         unsigned int uRdy,
                                         unsigned int uRun );
uint32_t AsicCtrl_GetRdyRunCfg         ( void );
uint32_t AsicCtrl_GetDebugCtrlStatus   ( void );
void     AsicCtrl_SetDebugCtrl         ( uint32_t ulSetMsk );
void     AsicCtrl_ResetDebugCtrl       ( uint32_t ulResetMsk );
void     AsicCtrl_SetNetxMasterStat0   ( uint32_t ulVal );
void     AsicCtrl_GetNetxMasterStat    ( uint32_t* pulStat0, uint32_t* pulStat1 );
void     AsicCtrl_SetSystimeCtrl       ( ASICCTRL_SYSTIME_MUX_E eSystimeMux, unsigned int uSystimeInst );
uint32_t AsicCtrl_GetSampleAtPornStat  ( unsigned int uReg );

void AsicCtrl_CfgFirewall        ( unsigned int uiFwIdx,
                                   unsigned int uWpVector,
                                   unsigned int uRpVector,
                                   unsigned int uAbortEnVector
                                 );
void AsicCtrl_EnFirewallIrq( unsigned int uiFwIdx, unsigned int uEnVector );
void AsicCtrl_DisFirewallIrq( unsigned int uiFwIdx, unsigned int uDisVector );
unsigned int AsicCtrl_GetFirewallIrq( unsigned int uiFwIdx );
unsigned int AsicCtrl_GetFirewallIrqRaw( unsigned int uiFwIdx );
void AsicCtrl_ConfirmFirewallIrq( unsigned int uiFwIdx,  unsigned int uConfirmVector );

uint32_t AsicCtrl_GetPhyCtrl0(void);
void AsicCtrl_SetPhyCtrl0(uint32_t ulVal);

void AsicCtrl_IrqMskSet(uint32_t ulIrq);
void AsicCtrl_IrqMskClr(uint32_t ulIrq);
uint32_t AsicCtrl_GetIrq(void);
uint32_t AsicCtrl_GetIrqRaw(void);
void AsicCtrl_IrqReset(uint32_t ulIrq);
void AsicCtrl_SetResetOut( bool fDrvLow, bool fOe );
unsigned int AsicCtrl_GetResetOutInput(void);
void AsicCtrl_SetOnlyPorn( uint32_t ulVal );
void AsicCtrl_SetOnlyPornRom( uint32_t ulVal );
uint32_t AsicCtrl_GetOnlyPorn (void);
uint32_t AsicCtrl_GetOnlyPornRom (void);

void AsicCtrl_GetClk2RcLen(uint32_t* pulMin, uint32_t* pulMax );
void AsicCtrl_ResetClk2RcLen(void);

#endif /* #ifndef __HAL_ASICCTRL_H */
