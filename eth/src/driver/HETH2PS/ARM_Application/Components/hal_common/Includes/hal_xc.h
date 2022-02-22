#ifndef __HAL_XC_H
#define __HAL_XC_H

#include <stdint.h>

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

typedef enum {
  XC_TYPE_RPEC = 0,
  XC_TYPE_TPEC,
  XC_TYPE_RPU,
  XC_TYPE_TPU
} XC_TYPE_E;

#define MSK_XC_TYPE_RPEC (1 << XC_TYPE_RPEC)
#define MSK_XC_TYPE_TPEC (1 << XC_TYPE_TPEC)
#define MSK_XC_TYPE_RPU  (1 << XC_TYPE_RPU)
#define MSK_XC_TYPE_TPU  (1 << XC_TYPE_TPU)
#define MSK_XC_TYPE_XPEC (MSK_XC_TYPE_RPEC | MSK_XC_TYPE_TPEC)
#define MSK_XC_TYPE_XMAC (MSK_XC_TYPE_RPU  | MSK_XC_TYPE_TPU)
#define MSK_XC_TYPE_ALL  (MSK_XC_TYPE_XPEC | MSK_XC_TYPE_XMAC)

typedef enum XC_REG_Etag {
  XC_XPEC_REG_R0,
  XC_XPEC_REG_R1,
  XC_XPEC_REG_R2,
  XC_XPEC_REG_R3,
  XC_XPEC_REG_R4,
  XC_XPEC_REG_R5,
  XC_XPEC_REG_R6,
  XC_XPEC_REG_R7,
  XC_XMAC_REG_CONFIG_SBU,
  XC_XMAC_REG_CONFIG_OBU,
  XC_XMAC_REG_OBU_RATE_MUL_ADD,
  XC_XMAC_REG_CONFIG_MII,
  XC_XMAC_REG_RX_HW,
  XC_XMAC_REG_TX_HW,
  XC_XPEC_REG_PC,
  XC_XMAC_REG_RPU_PC,
  XC_XMAC_REG_TPU_PC,
  XC_XPEC_REG_IRQ
} XC_REG_E;

typedef struct XC_PRG_Ttag
{
  const uint32_t* pulRpecPrg;
  const uint32_t* pulTpecPrg;
  const uint32_t* pulRpuPrg;
  const uint32_t* pulTpuPrg;
} XC_PRG_T;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

int         XC_ResetDummy      ( unsigned int uiXcInst, unsigned int uiPort, void* pvUser );
int         XC_Reset           ( unsigned int uiXcInst, unsigned int uiPort, void* pvUser );
int         XC_Load            ( unsigned int uiXcInst, unsigned int uiPort, XC_TYPE_E eXcType, const uint32_t* pulXcPrg, void* pvUser);
int         XC_Start           ( unsigned int uiXcInst, unsigned int uiPort, void* pvUser );
int         XC_StartUnits      ( unsigned int uiXcInst, unsigned int uiPort, unsigned int uUnitVec, void* pvUser );
int         XC_Stop            ( unsigned int uiXcInst, unsigned int uiPort, void* pvUser );
int         XC_GetReg          ( unsigned int uiXcInst, unsigned int uiPort, XC_TYPE_E eXcType, XC_REG_E eReg, uint32_t * pulValue );
int         XC_SetReg          ( unsigned int uiXcInst, unsigned int uiPort, XC_TYPE_E eXcType, XC_REG_E eReg, uint32_t ulValue );
int         XC_WrXpecDramDw    ( unsigned int uiXcInst, unsigned int uiPort, unsigned int uiOffset, uint32_t ulVal );
uint32_t    XC_RdXpecDramDw    ( unsigned int uiXcInst, unsigned int uiPort, unsigned int uiOffset );
int         XC_WrRpecDramDw    ( unsigned int uiXcInst, unsigned int uiPort, unsigned int uiOffset, uint32_t ulVal );
uint32_t    XC_RdRpecDramDw    ( unsigned int uiXcInst, unsigned int uiPort, unsigned int uiOffset );
int         XC_StartStopSet    ( uint32_t ulValue );
int         XC_StartStopGet    ( uint32_t * pulValue );
int         XC_SystimeConfigSet( unsigned int uiXcInst, unsigned int uSystimeInstNo );
int         XC_LoadChannel     ( unsigned int uiXcInst, unsigned int uiPort, XC_PRG_T* ptPrg, void* pvUser );
void        XC_SetFbClk        ( unsigned int uiXcInst, unsigned int uiPort, uint32_t ulFbClkRateMulAdd, uint32_t ulFbClkDiv );


#endif /* #ifndef __HAL_XC_H */
