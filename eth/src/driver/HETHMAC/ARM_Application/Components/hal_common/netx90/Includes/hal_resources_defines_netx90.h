#ifndef _HAL_RESOURCES_DEFINES_H
#define _HAL_RESOURCES_DEFINES_H

#if defined(__NETX90_MPW)
#include "regdef_netx90mpw.h" /* used for addresses and structures */
#ifndef __NETX90
#define __NETX90
#endif
#else
#include "regdef_netx90.h" /* used for addresses and structures */
#endif

/* used for function names */
#include "hal_armirq.h"
#include "hal_extphy.h"
#include "hal_xc.h"
#include "hal_xpic.h"
#include "hal_phyctrl.h"

#include <string.h>


#define _HW_CONCAT(a,b) a ## b
#define HW_MSK(bf)         _HW_CONCAT(MSK_NX90_, bf)
#define HW_SRT(bf)         _HW_CONCAT(SRT_NX90_, bf)
#define HW_DFLT_BF_VAL(bf) _HW_CONCAT(DFLT_BF_VAL_NX90_, bf)
#define HW_DFLT_VAL(reg)   _HW_CONCAT(DFLT_VAL_NX90_, reg)
#define HW_REGADR(reg)     _HW_CONCAT(Adr_NX90_, reg)
#define HW_AREAADR(area)   _HW_CONCAT(Addr_NX90_, area)
#define HW_TYPE(area)      _HW_CONCAT(NX90_, area)

#define SLEEP_CC(cc) PTimer_Sleep(0,cc)

#define NX_DEV_FREQUENCY 100000000L // 100 Mhz

/* Set definitions */
#define NX_SYSTIME_UC   1

#define NX_XC_TYPE_RPU  XC_TYPE_RPU
#define NX_XC_TYPE_TPU  XC_TYPE_TPU
#define NX_XC_TYPE_RPEC XC_TYPE_RPEC
#define NX_XC_TYPE_TPEC XC_TYPE_TPEC

#define MSK_NX_XC_TYPE_RPU  MSK_XC_TYPE_RPU
#define MSK_NX_XC_TYPE_TPU  MSK_XC_TYPE_TPU
#define MSK_NX_XC_TYPE_RPEC MSK_XC_TYPE_RPEC
#define MSK_NX_XC_TYPE_TPEC MSK_XC_TYPE_TPEC

#define NX_INT_PHYCTRL_LED_MODE_DIS      PHYCTRL_LED_MODE_MANUAL
#define NX_INT_PHYCTRL_LED_MODE_STATIC   PHYCTRL_LED_MODE_STATIC
#define NX_INT_PHYCTRL_LED_MODE_FLASHING PHYCTRL_LED_MODE_FLASHING
#define NX_INT_PHYCTRL_LED_MODE_COMBINED PHYCTRL_LED_MODE_COMBINED

#define NX_MIIMU_SEND_PREAMBLE                 MIIMU_SEND_PREAMBLE
#define NX_MIIMU_MDCFREQ_HIGH                  MIIMU_MDCFREQ_HIGH
#define NX_MIIMU_RTA_0_BITS                    MIIMU_RTA_0_BITS
#define NX_DRV_CB12_CONTROL_STATUS_INDICATION  DRV_CB12_CONTROL_STATUS_INDICATION

typedef struct NX90_XPEC_IRQ_REGS_Ttag
{
  volatile uint32_t aulIrq_xpec[2];
} NX90_XPEC_IRQ_REGS_T;


/*** Register areas ***/

#if defined(CPU_IS_ARM_APP)

/* ARM_APP resources */

#define __USE_XPIC_DRAM         static uint32_t*                          const s_apulXpicDram[]       = \
  {\
    (uint32_t*) Addr_NX90_xpic_app_dram\
  };

#define __USE_XPIC_INTRAM       static uint32_t*                          const s_pulIntRamXpicStart   = (uint32_t*) Addr_NX90_intram7_mirror_sram;

#else

/* ARM_COM resources */

#define __USE_XPIC_TIMER        static NX90_XPIC_TIMER_AREA_T*          const s_aptXpicTimer[]       = \
  {\
    (NX90_XPIC_TIMER_AREA_T*) Addr_NX90_timer_xpic_com\
  };

#define __USE_XPIC_WDG        static NX90_XPIC_WDG_AREA_T*          const s_aptXpicWdg[]       = \
  {\
    (NX90_XPIC_WDG_AREA_T*) Addr_NX90_wdg_xpic_com\
  };

#define __USE_XPIC_VIC        static NX90_XPIC_VIC_AREA_T*          const s_aptXpicVic[]       = \
  {\
    (NX90_XPIC_VIC_AREA_T*) Addr_NX90_vic_xpic_com\
  };

#define __USE_XPIC              static NX90_XPIC_AREA_T*                const s_aptXpic[]            = \
  {\
    (NX90_XPIC_AREA_T*) Addr_NX90_xpic_com_regs\
  };

#define __USE_XPIC_DEBUG        static NX90_XPIC_DEBUG_AREA_T*          const s_aptXpicDebug[]       = \
  {\
    (NX90_XPIC_DEBUG_AREA_T*) Addr_NX90_xpic_com_debug\
  };

#define __USE_XPIC_DRAM         static uint32_t*                          const s_apulXpicDram[]       = \
  {\
    (uint32_t*) Addr_NX90_xpic_com_dram\
  };

#define __USE_XPIC_INTRAM       static uint32_t*                          const s_pulIntRamXpicStart   = (uint32_t*) Addr_NX90_intram5_mirror_sram;

#define __USE_ASIC_CTRL         static NX90_ASIC_CTRL_AREA_T*           const s_ptAsicCtrl           = (NX90_ASIC_CTRL_AREA_T*) Addr_NX90_asic_ctrl;

#define __USE_SYSTIME           static NX90_SYSTIME_AREA_T*             const s_aptSystime[]         = \
  {\
    (NX90_SYSTIME_AREA_T*) Addr_NX90_systime_com,\
    (NX90_SYSTIME_AREA_T*) Addr_NX90_systime_uc_com\
  };

#define __USE_SYSTIMELT         static NX90_INTLOGIC_SYSTIME_LT_AREA_T* const s_aptSystimeLt[]       = \
  {\
    (NX90_INTLOGIC_SYSTIME_LT_AREA_T*) Addr_NX90_systime_lt_com,\
    (NX90_INTLOGIC_SYSTIME_LT_AREA_T*) Addr_NX90_systime_lt_xpic_com,\
  };
  
#define __USE_MIIMU             static NX90_MIIMU_AREA_T*               const s_ptMiimu              = (NX90_MIIMU_AREA_T*) Addr_NX90_miimu;

#define __USE_BUFMAN            static NX90_BUF_MAN_AREA_T*             const s_aptBufMan[]          = \
  {\
    (NX90_BUF_MAN_AREA_T*) Addr_NX90_xc0_buf_man\
  };

#define __USE_POINTERFIFO       static NX90_POINTER_FIFO_AREA_T*        const s_aptPFifo[]           = \
  {\
    (NX90_POINTER_FIFO_AREA_T*) Addr_NX90_xc0_pointer_fifo\
  };

#define __USE_TRIGGER_SAMPLE    static NX90_TRIGGER_SAMPLE_UNIT_AREA_T* const s_aptTriggerSample[]   = \
  {\
    (NX90_TRIGGER_SAMPLE_UNIT_AREA_T*) Addr_NX90_xc0_trigger_sample_unit\
  };
  
#define __USE_XC_EXTERNAL_CFG   static NX90_XC_EXTERNAL_CONFIG_AREA_T*  const s_ptXcExtCfg           = (NX90_XC_EXTERNAL_CONFIG_AREA_T*)Addr_NX90_xc_external_config;

#define __USE_FMMUSM            static NX90_FMMUSM_AREA_T*              const s_aptFmmuSm[]          = \
  {\
    (NX90_FMMUSM_AREA_T*) Addr_NX90_xc0_fmmusm\
  };

#define __USE_XPEC_IRQ_REGS     static NX90_XPEC_IRQ_REGS_T*            const s_ptXpecIrqRegs        = (NX90_XPEC_IRQ_REGS_T*) Addr_NX90_xc_xpec_irq_registers;

#define __USE_XC_XPEC_IRQ_REGS  static NX90_XPEC_IRQ_REGISTERS_AREA_T*  const s_ptXcXpecIrqRegs      = (NX90_XPEC_IRQ_REGISTERS_AREA_T*) Addr_NX90_xc_xpec_irq_registers;

#define __USE_PHY_CTRL          static NX90_PHY_CTRL_AREA_T*            const s_aptPhyCtrl[]         = \
  {\
    (NX90_PHY_CTRL_AREA_T*) Addr_NX90_xc0_phy_ctrl0,\
    (NX90_PHY_CTRL_AREA_T*) Addr_NX90_xc0_phy_ctrl1\
  };

#define __USE_INT_PHY_CFG       static NX90_INT_PHY_CFG_AREA_T*         const s_ptIntPhyCfg           = (NX90_INT_PHY_CFG_AREA_T*)Addr_NX90_int_phy_cfg_com;

#define __USE_RPEC_REGS         static NX90_XPEC_AREA_T*                const s_aptRpecRegArea[]      = \
  {\
    (NX90_XPEC_AREA_T*) Addr_NX90_xc0_rpec0_regs,\
    (NX90_XPEC_AREA_T*) Addr_NX90_xc0_rpec1_regs\
  };

#define __USE_TPEC_REGS         static NX90_XPEC_AREA_T*                const s_aptTpecRegArea[]      = \
  {\
    (NX90_XPEC_AREA_T*) Addr_NX90_xc0_tpec0_regs,\
    (NX90_XPEC_AREA_T*) Addr_NX90_xc0_tpec1_regs\
  };

#define __USE_RPEC_PRAM_START   static uint32_t*                          const s_apulRpecPramArea[]    = \
  {\
    (uint32_t*) Adr_NX90_xc0_rpec0_pram_ram_start,\
    (uint32_t*) Adr_NX90_xc0_rpec1_pram_ram_start\
  };

#define __USE_RPEC_PRAM_END     static uint32_t*                          const s_apulRpecPramAreaEnd[] = \
  {\
    (uint32_t*) Adr_NX90_xc0_rpec0_pram_ram_end,\
    (uint32_t*) Adr_NX90_xc0_rpec1_pram_ram_end\
  };

#define __USE_TPEC_PRAM_START   static uint32_t*                          const s_apulTpecPramArea[]    = \
  {\
    (uint32_t*) Adr_NX90_xc0_tpec0_pram_ram_start,\
    (uint32_t*) Adr_NX90_xc0_tpec1_pram_ram_start\
  };

#define __USE_TPEC_PRAM_END     static uint32_t*                          const s_apulTpecPramAreaEnd[] = \
  {\
    (uint32_t*) Adr_NX90_xc0_tpec0_pram_ram_end,\
    (uint32_t*) Adr_NX90_xc0_tpec1_pram_ram_end\
  };

#define __USE_XPEC_DRAM         static uint32_t*                          const s_apulXpecDramArea[]    = \
  {\
    (uint32_t*) Adr_NX90_xc0_tpec0_dram_ram_start,\
    (uint32_t*) Adr_NX90_xc0_tpec1_dram_ram_start\
  };
  
#define __USE_RPEC_DRAM         static uint32_t*                          const s_apulRpecDramArea[]    = \
  {\
    (uint32_t*) Adr_NX90_xc0_rpec0_dram_ram_start,\
    (uint32_t*) Adr_NX90_xc0_rpec1_dram_ram_start\
  };
  
#define __USE_XMAC              static NX90_XMAC_AREA_T*                const s_aptXmacArea[]         = \
  {\
    (NX90_XMAC_AREA_T*) Addr_NX90_xc0_xmac0_regs,\
    (NX90_XMAC_AREA_T*) Addr_NX90_xc0_xmac1_regs\
  };
  
#define __USE_RPU_PRAM_START    static uint32_t*                          const s_aptRpuPramArea[]        = \
  {\
    (uint32_t*)  Adr_NX90_xc0_rpu0_ram_xmac_ram_start,\
    (uint32_t*)  Adr_NX90_xc0_rpu1_ram_xmac_ram_start\
  };

#define __USE_TPU_PRAM_START    static uint32_t*                          const s_aptTpuPramArea[]        = \
  {\
    (uint32_t*)  Adr_NX90_xc0_tpu0_ram_xmac_ram_start,\
    (uint32_t*)  Adr_NX90_xc0_tpu1_ram_xmac_ram_start\
  };

#define __USE_RPU_PRAM_END      static uint32_t*                          const s_aptRpuPramAreaEnd[]     = \
  {\
    (uint32_t*)  Adr_NX90_xc0_rpu0_ram_xmac_ram_end,\
    (uint32_t*)  Adr_NX90_xc0_rpu1_ram_xmac_ram_end\
  };

#define __USE_TPU_PRAM_END      static uint32_t*                          const s_aptTpuPramAreaEnd[]     = \
  {\
    (uint32_t*)  Adr_NX90_xc0_tpu0_ram_xmac_ram_end,\
    (uint32_t*)  Adr_NX90_xc0_tpu1_ram_xmac_ram_end\
  };

#define __USE_INTRAM            static uint32_t*                          const s_apulIntRamStart[]     = \
  {\
    (uint32_t*) Addr_NX90_intram3_mirror_ocp,\
    (uint32_t*) Addr_NX90_intram4_mirror_ocp,\
  };

#define __USE_CANCTRL           static NX90_CANCTRL_AREA_T*             const s_ptCanCtrl            = (NX90_CANCTRL_AREA_T*) Addr_NX90_canctrl;
 
#define __USE_XC_STATCFG_SHARED static NX90_XC_STATCFG_SHARED_AREA_T*   const s_ptXcStatCfgShared    = (NX90_XC_STATCFG_SHARED_AREA_T*) Addr_NX90_xc_statcfg_shared;

#define __USE_GLOBAL_BUFMAN     static NX90_GLOBAL_BUF_MAN_AREA_T*      const s_ptGlobalBufMan       = (NX90_GLOBAL_BUF_MAN_AREA_T*) Addr_NX90_global_buf_man;

#endif

/*** XC loader ***/
typedef int(*PFN_XC_RESET)( unsigned int uXcNo, unsigned int uXcPortNo, void* pvUser);
typedef int(*PFN_XC_LOAD) ( unsigned int uXcNo, unsigned int uXcPortNo, XC_TYPE_E eXcType, const uint32_t* pulXcPrg, void* pvUser );
typedef int(*PFN_XC_START)( unsigned int uXcNo, unsigned int uXcPortNo, void* pvUser);
typedef int(*PFN_XC_START_UNITS)( unsigned int uXcNo, unsigned int uPortNo, unsigned int uUnitVec, void* pvUser );

#ifdef DISABLE_XC_RESET
#define __USE_XC_LOADER_FUNC_RESET        static const PFN_XC_RESET       pfnXcReset      = XC_ResetDummy;
#else
#define __USE_XC_LOADER_FUNC_RESET        static const PFN_XC_RESET       pfnXcReset      = XC_Reset;
#endif
#define __USE_XC_LOADER_FUNC_LOAD         static const PFN_XC_LOAD        pfnXcLoad       = XC_Load;
#define __USE_XC_LOADER_FUNC_START        static const PFN_XC_START       pfnXcStart      = XC_Start;
#define __USE_XC_LOADER_FUNC_START_UNITS  static const PFN_XC_START_UNITS pfnXcStartUnits = XC_StartUnits;
#define __USE_XC_LOADER_FUNC_START_UNITS2 static const PFN_XC_START_UNITS pfnXcStartUnits = XC_StartUnits2;

/*** xPIC loader ***/
typedef void(*PFN_XPIC_RESET)(unsigned int uInstNo, void* pvUser);
typedef void(*PFN_XPIC_LOAD) (unsigned int uInstNo, const uint32_t* pulXpicPrg, void* pvUser);
typedef void(*PFN_XPIC_START)(unsigned int uInstNo, void* pvUser);

#define __USE_XPIC_LOADER_FUNC_RESET static PFN_XPIC_RESET pfnXpicReset = XPIC_Reset;
#define __USE_XPIC_LOADER_FUNC_LOAD  static PFN_XPIC_LOAD  pfnXpicLoad  = XPIC_Load;
#define __USE_XPIC_LOADER_FUNC_START static PFN_XPIC_START pfnXpicStart = XPIC_Start;

/*** External PHY ***/
typedef int(*PFN_EXTPHY_GETLINKSTATE) (unsigned int uiPhyCtrlInst, unsigned int uPhy, unsigned int* puLink, unsigned int* puMbps, unsigned int* puFullDuplex, void* pvUser);
#define __USE_EXTPHY_FUNC_GETLINKSTATE static PFN_EXTPHY_GETLINKSTATE pfnExtphyGetLinkState = EXTPHY_GetLinkState;

/*** Access Macros ***/
#ifndef NX_WRITE32
#define NX_WRITE32(var,val) (var) = (val)
#endif

#ifndef NX_READ32
#define NX_READ32(var)      (var)
#endif

#ifndef NX_WRITE16
#define NX_WRITE16(var,val) (var) = (val)
#endif

#ifndef NX_READ16
#define NX_READ16(var)      (var)
#endif

#ifndef NX_WRITE8
#define NX_WRITE8(var,val)  (var) = (val)
#endif

#ifndef NX_READ8
#define NX_READ8(var)       (var)
#endif

/* Copy data from netX to Host */
#ifndef NX_READMEM
#define NX_READMEM(dst, src, len) memcpy(dst, src, len)
#endif

/* Copy data from Host to netX */
#ifndef NX_WRITEMEM
#define NX_WRITEMEM(dst, src, len) memcpy(dst, src, len)
#endif
  
#endif
