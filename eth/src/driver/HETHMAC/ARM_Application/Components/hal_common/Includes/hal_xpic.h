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

#ifndef HALDEC
#define HALDEC(name) name
#endif

void     HALDEC(XPIC_Reset)( unsigned int uiXpicNo, void* pvUser );
void     HALDEC(XPIC_Load)( unsigned int uiXpicNo, const uint32_t *pulXpicPrg, void* pvUser );
void     HALDEC(XPIC_Start)( unsigned int uiXpicNo, void* pvUser );
void     HALDEC(XPIC_StartEx)( unsigned int uiXpicNo, bool fSinglestep, bool fMonitor, bool fIntDis, bool fMisalignHold, void* pvUser);
void     HALDEC(XPIC_Stop)( unsigned int uiXpicNo, void* pvUser );
void     HALDEC(XPIC_SetBankSelect)( unsigned int uiXpicNo, bool fSel, bool fFiqBank);
uint32_t HALDEC(XPIC_GetLastPc)( unsigned int uiXpicNo );
uint32_t HALDEC(XPIC_GetIrqStatus)( unsigned int uiXpicNo );
uint32_t HALDEC(XPIC_GetIrqReturnPc)( unsigned int uiXpicNo );
uint32_t HALDEC(XPIC_GetFiqReturnPc)( unsigned int uiXpicNo );
uint32_t HALDEC(XPIC_GetBreakStatus)( unsigned int uiXpicNo );
uint32_t HALDEC(XPIC_GetBreakIrqRaw)( unsigned int uiXpicNo );
void     HALDEC(XPIC_ConfirmBreakIrq)( unsigned int uiXpicNo, uint32_t ulIrq );
uint32_t HALDEC(XPIC_GetBreakIrq)( unsigned int uiXpicNo, bool fOwn );
void     HALDEC(XPIC_EnableBreakIrq)( unsigned int uiXpicNo, uint32_t ulIrq, bool fOwn );
void     HALDEC(XPIC_DisableBreakIrq)( unsigned int uiXpicNo, uint32_t ulIrq, bool fOwn );
void     HALDEC(XPIC_SetupWatchPoint)( unsigned int uiXpicNo, unsigned int uWpNo, XPIC_WATCHPOINT_T* ptWp );
uint32_t HALDEC(XPIC_GetReg)( unsigned int uiXpicNo, XPIC_REG_E eReg );
void     HALDEC(XPIC_SetReg)( unsigned int uiXpicNo, XPIC_REG_E eReg, uint32_t ulValue );
void     HALDEC(XPIC_WrDramDw)( unsigned int uiXpicNo, unsigned int uiOffset, uint32_t ulValue );
uint32_t HALDEC(XPIC_RdDramDw)( unsigned int uiXpicNo, unsigned int uiOffset );
void     HALDEC(XPIC_WrPramDw)( unsigned int uiXpicNo, unsigned int uiOffset, uint32_t ulValue );
uint32_t HALDEC(XPIC_RdPramDw)( unsigned int uiXpicNo, unsigned int uiOffset );


#endif /* #ifndef __HAL_XPIC_H */
