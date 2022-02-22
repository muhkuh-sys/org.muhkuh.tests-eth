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
#ifndef HALDEC
#define HALDEC(name) name
#endif

void     HALDEC(AsicCtrl_SetIoConfig)          ( unsigned int uIdx, uint32_t ulVal );
uint32_t HALDEC(AsicCtrl_GetIoConfig)          ( unsigned int uIdx );

void     HALDEC(AsicCtrl_SetResetCtrl)         ( uint32_t ulVal );
uint32_t HALDEC(AsicCtrl_GetResetCtrl)         ( void );

void     HALDEC(AsicCtrl_SetClockEnable)       ( uint32_t ulVal );
void     HALDEC(AsicCtrl_SetClockEnable1)      ( uint32_t ulVal );
uint32_t HALDEC(AsicCtrl_GetClockEnable)       ( void );
uint32_t HALDEC(AsicCtrl_GetClockEnable1)      ( void );
void     HALDEC(AsicCtrl_SetClockEnableMask)   ( uint32_t ulVal );
void     HALDEC(AsicCtrl_SetClockEnableMask1)  ( uint32_t ulVal );
uint32_t HALDEC(AsicCtrl_GetClockEnableMask)   ( void );
uint32_t HALDEC(AsicCtrl_GetClockEnableMask1)  ( void );
void     HALDEC(AsicCtrl_SetMiscAsicCtrl)      ( uint32_t ulVal );
uint32_t HALDEC(AsicCtrl_GetMiscAsicCtrl)      ( void );
void     HALDEC(AsicCtrl_SetAdcClk)            ( uint32_t ulAdcClkDiv );
void     HALDEC(AsicCtrl_SetFbClk)             ( unsigned int uXc,
                                                unsigned int uXpec,
                                                uint32_t ulFbClkRateMulAdd,
                                                uint32_t ulFbClkDiv );
void     HALDEC(AsicCtrl_SetNetxStatus)        ( uint32_t ulVal );
uint32_t HALDEC(AsicCtrl_GetNetxStatus)        ( void );
void     HALDEC(AsicCtrl_SetRomWdg)            ( uint32_t ulVal );
void     HALDEC(AsicCtrl_SetSystemStatus)      ( uint32_t ulVal );
uint32_t HALDEC(AsicCtrl_GetSystemStatus)      ( void );
void     HALDEC(AsicCtrl_SetNetxVersion)       ( uint32_t ulVal );
uint32_t HALDEC(AsicCtrl_GetNetxVersion)       ( void );
void     HALDEC(AsicCtrl_SetRdyRunCfg)         ( unsigned int uRdyDrv,
                                                unsigned int uRunDrv,
                                                unsigned int uRdyPol,
                                                unsigned int uRunPol,
                                                unsigned int uRdy,
                                                unsigned int uRun );
uint32_t HALDEC(AsicCtrl_GetRdyRunCfg)         ( void );
uint32_t HALDEC(AsicCtrl_GetDebugCtrlStatus)   ( void );
void     HALDEC(AsicCtrl_SetDebugCtrl)         ( uint32_t ulSetMsk );
void     HALDEC(AsicCtrl_ResetDebugCtrl)       ( uint32_t ulResetMsk );
void     HALDEC(AsicCtrl_SetNetxMasterStat0)   ( uint32_t ulVal );
void     HALDEC(AsicCtrl_GetNetxMasterStat)    ( uint32_t* pulStat0, uint32_t* pulStat1 );
void     HALDEC(AsicCtrl_SetSystimeCtrl)       ( ASICCTRL_SYSTIME_MUX_E eSystimeMux, unsigned int uSystimeInst );
uint32_t HALDEC(AsicCtrl_GetSampleAtPornStat)  ( unsigned int uReg );

void HALDEC(AsicCtrl_CfgFirewall)( unsigned int uiFwIdx,
                                   unsigned int uWpVector,
                                   unsigned int uRpVector,
                                   unsigned int uAbortEnVector
                                 );
void HALDEC(AsicCtrl_EnFirewallIrq)( unsigned int uiFwIdx, unsigned int uEnVector );
void HALDEC(AsicCtrl_DisFirewallIrq)( unsigned int uiFwIdx, unsigned int uDisVector );
unsigned int HALDEC(AsicCtrl_GetFirewallIrq)( unsigned int uiFwIdx );
unsigned int HALDEC(AsicCtrl_GetFirewallIrqRaw)( unsigned int uiFwIdx );
void HALDEC(AsicCtrl_ConfirmFirewallIrq)( unsigned int uiFwIdx,  unsigned int uConfirmVector );

uint32_t HALDEC(AsicCtrl_GetPhyCtrl0)(void);
void HALDEC(AsicCtrl_SetPhyCtrl0)(uint32_t ulVal);

void HALDEC(AsicCtrl_IrqMskSet)(uint32_t ulIrq);
void HALDEC(AsicCtrl_IrqMskClr)(uint32_t ulIrq);
uint32_t HALDEC(AsicCtrl_GetIrq)(void);
uint32_t HALDEC(AsicCtrl_GetIrqRaw)(void);
void HALDEC(AsicCtrl_IrqReset)(uint32_t ulIrq);
void HALDEC(AsicCtrl_SetResetOut)( bool fDrvLow, bool fOe );
unsigned int HALDEC(AsicCtrl_GetResetOutInput)(void);
void HALDEC(AsicCtrl_SetOnlyPorn)( uint32_t ulVal );
void HALDEC(AsicCtrl_SetOnlyPornRom)( uint32_t ulVal );
uint32_t HALDEC(AsicCtrl_GetOnlyPorn) (void);
uint32_t HALDEC(AsicCtrl_GetOnlyPornRom) (void);

void HALDEC(AsicCtrl_GetClk2RcLen)(uint32_t* pulMin, uint32_t* pulMax );
void HALDEC(AsicCtrl_ResetClk2RcLen)(void);

#endif /* #ifndef __HAL_ASICCTRL_H */
