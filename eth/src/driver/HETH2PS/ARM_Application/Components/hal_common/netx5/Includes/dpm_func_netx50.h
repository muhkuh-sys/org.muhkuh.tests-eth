#ifndef __DPM_FUNC_NETX50_H
#define __DPM_FUNC_NETX50_H

#include "defines.h" // for PEEK, POKE, INTERRUPT
#include "regdef_netx50.h"

/*
  ____    ____    __  __
 |  _ \  |  _ \  |  \/  |
 | | | | | |_) | | |\/| |
 | |_| | |  __/  | |  | |
 |____/  |_|     |_|  |_|

*/

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
typedef enum {
  NX50_DPM_CFG_08_RDY,      /* 8 bit DPM with ready     */
  NX50_DPM_CFG_16_RDY,      /* 16 bit DPM with ready    */
  NX50_DPM_CFG_08_NORDY,     /* 8 bit DPM without ready  */
  NX50_DPM_CFG_16_NORDY,    /* 16 bit DPM without ready */
} NX50_DPM_CFG_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
int  NX50_DPM_SetupExtbus( NX50_DPM_CFG_E tSize, int iReset, unsigned int uResetDelay );
void NX50_DPM_ConfigDpmIrq( unsigned long ulDpmIrqMsk );
void NX50_DPM_GetDpmIrq( unsigned long* pulIrq );
void NX50_DPM_AckDpmIrq( unsigned long pulVal );


/*
  ____    ____   ___      __                    ____    ____    __  __
 / ___|  |  _ \ |_ _|    / _|   ___    _ __    |  _ \  |  _ \  |  \/  |
 \___ \  | |_) | | |    | |_   / _ \  | '__|   | | | | | |_) | | |\/| |
  ___) | |  __/  | |    |  _| | (_) | | |      | |_| | |  __/  | |  | |
 |____/  |_|    |___|   |_|    \___/  |_|      |____/  |_|     |_|  |_|

*/

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
unsigned long NX50_SerDPM_SetupSPI(unsigned long  ulSckMulAdd, int iReset, unsigned int uResetDelay);
unsigned char NX50_SerDPM_Read8_SmallRange(unsigned long ulDpmAddr);
void NX50_SerDPM_Write8_SmallRange(unsigned long ulDpmAddr, unsigned char bData);
unsigned long NX50_SerDPM_Read32_XPEC_FullRange(unsigned long ulDpmAddr);
unsigned long NX50_SerDPM_Read32_nonXPEC_FullRange(unsigned long ulDpmAddr);
void NX50_SerDPM_Write32_XPEC_FullRange(unsigned long ulDpmAddr, unsigned long ulData);
void NX50_SerDPM_Write32_nonXPEC_FullRange(unsigned long ulDpmAddr, unsigned long ulData);
unsigned char NX50_SerDPM_Read8_XPEC_FullRange(unsigned long ulDpmAddr);
unsigned char NX50_SerDPM_Read8_nonXPEC_FullRange(unsigned long ulDpmAddr);
void NX50_SerDPM_Write8_XPEC_FullRange(unsigned long ulDpmAddr, unsigned char bData);
void NX50_SerDPM_Write8_nonXPEC_FullRange(unsigned long ulDpmAddr, unsigned char bData);


/* Parallel DPM */

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
void NX50_ParDPM_Write32_XPEC_FullRange(unsigned long ulDpmAddr, unsigned long ulData);
unsigned long NX50_ParDPM_Read32_XPEC_FullRange(unsigned long ulDpmAddr);
void NX50_ParDPM_Write16_XPEC_FullRange(unsigned long ulDpmAddr, unsigned short usData);
unsigned short NX50_ParDPM_Read16_XPEC_FullRange(unsigned long ulDpmAddr);
void NX50_ParDPM_Write8_XPEC_FullRange(unsigned long ulDpmAddr, unsigned char bData);
unsigned char NX50_ParDPM_Read8_XPEC_FullRange(unsigned long ulDpmAddr);



/*****************************************************************************/
/* Debug, statistics                                                         */
/*****************************************************************************/

#ifdef __DEBUG_DPM__

typedef struct DPM_ERRCNT_Ttag {
  unsigned long ulRdErrCnt;     // read error
  unsigned long ulWrErrCnt;     // write error
  unsigned long ulRdyToErrCnt;  // ready timeout
  unsigned long ulBusErrCnt;    // bus conflict
} DPM_ERRCNT_T;
extern DPM_ERRCNT_T g_tDpmReadErr;
extern DPM_ERRCNT_T g_tDpmWriteErr;

#endif

#ifdef __DPM_STATS__

typedef struct DPM_STAT_Ttag {
  unsigned long ulCnt;
  unsigned long ulMin;
  unsigned long ulAvg;
  unsigned long ulMax;
} DPM_STAT_T;

typedef struct DPM_STATS_Ttag {
  DPM_STAT_T tRd32;
  DPM_STAT_T tWr32;
  DPM_STAT_T tRd16;
  DPM_STAT_T tWr16;
  DPM_STAT_T tRd8;
  DPM_STAT_T tWr8;
} DPM_STATS_T;

extern DPM_STATS_T g_tDpmStat;

#endif

#endif // #define __DPM_FUNC_NETX50_H











