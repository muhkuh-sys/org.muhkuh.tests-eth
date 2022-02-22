#ifndef __HAL_XPIC_H
#define __HAL_XPIC_H

#include <stdint.h>
#include <stdbool.h>


typedef struct XPIC_WATCHPOINT_Ttag
{
  uint32_t  ulAddrValue;
  uint32_t  ulAddrDcMsk;
  uint32_t  ulDataValue;
  uint32_t  ulDataDcMsk;
  uint32_t  ulCtrlValue;
  uint32_t  ulCtrlDcMsk;
} XPIC_WATCHPOINT_T;

typedef enum
{
  XPIC_REG_R0   = 0,
  XPIC_REG_R1   = 1,
  XPIC_REG_R2   = 2,
  XPIC_REG_R3   = 3,
  XPIC_REG_R4   = 4,
  XPIC_REG_R5   = 5,
  XPIC_REG_R6   = 6,
  XPIC_REG_R7   = 7,
  XPIC_REG_USR0 = 8,
  XPIC_REG_USR1 = 9,
  XPIC_REG_USR2 = 10,
  XPIC_REG_USR3 = 11,
  XPIC_REG_USR4 = 12,
  XPIC_REG_PC   = 13,
  XPIC_REG_STAT = 14,
  XPIC_REG_ZERO = 15
} XPIC_REG_E;


/*****************************************************************************/
/* Functions                                                                 */
/*****************************************************************************/

void     XPIC_Reset( unsigned int uiXpic, void* pvUser );
void     XPIC_Load( unsigned int uiXpic, const uint32_t *pulXpicPrg, void* pvUser );
void     XPIC_Start( unsigned int uiXpic, void* pvUser );
void     XPIC_StartEx( unsigned int uiXpic, bool fSinglestep, bool fMonitor, bool fIntDis, bool fMisalignHold, void* pvUser);
void     XPIC_Stop( unsigned int uiXpic, void* pvUser );
void     XPIC_SetBankSelect( unsigned int uiXpic, bool fSel, bool fFiqBank);
uint32_t XPIC_GetLastPc( unsigned int uiXpic );
uint32_t XPIC_GetIrqStatus( unsigned int uiXpic );
uint32_t XPIC_GetIrqReturnPc( unsigned int uiXpic );
uint32_t XPIC_GetFiqReturnPc( unsigned int uiXpic );
uint32_t XPIC_GetBreakStatus( unsigned int uiXpic );
uint32_t XPIC_GetBreakIrqRaw( unsigned int uiXpic );
void     XPIC_ConfirmBreakIrq( unsigned int uiXpic, uint32_t ulIrq );
uint32_t XPIC_GetBreakIrq( unsigned int uiXpic, bool fOwn );
void     XPIC_EnableBreakIrq( unsigned int uiXpic, uint32_t ulIrq, bool fOwn );
void     XPIC_DisableBreakIrq( unsigned int uiXpic, uint32_t ulIrq, bool fOwn );
void     XPIC_SetupWatchPoint( unsigned int uiXpic, unsigned int uiWpNo, XPIC_WATCHPOINT_T* ptWp );
uint32_t XPIC_GetReg( unsigned int uiXpic, XPIC_REG_E eReg );
void     XPIC_SetReg( unsigned int uiXpic, XPIC_REG_E eReg, uint32_t ulValue );
void     XPIC_WrDramDw( unsigned int uiXpic, unsigned int uiOffset, uint32_t ulValue );
uint32_t XPIC_RdDramDw( unsigned int uiXpic, unsigned int uiOffset );
void     XPIC_WrPramDw( unsigned int uiXpic, unsigned int uiOffset, uint32_t ulValue );
uint32_t XPIC_RdPramDw( unsigned int uiXpic, unsigned int uiOffset );


#endif /* #ifndef __HAL_XPIC_H */
