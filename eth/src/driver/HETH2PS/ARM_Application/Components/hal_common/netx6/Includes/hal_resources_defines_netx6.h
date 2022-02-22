#ifndef __HAL_RESOURCES_DEFINES_NETX6_H
#define __HAL_RESOURCES_DEFINES_NETX6_H

#include "compiler_spec.h"     /* compiler specific definitions */
#include "dpm_mapping_netx6.h" /* used for DPM mapping */
#include "regdef_netx51.h"     /* used for structures */
#include "common_func_netx51.h"
#include "common_func_netx6.h" /* used for function names */
#include "netx51_access_nx6.h"
#include <string.h>

#define _HW_CONCAT(a,b) a ## b
#define HW_MSK(bf)         _HW_CONCAT(MSK_NX51_, bf)
#define HW_SRT(bf)         _HW_CONCAT(SRT_NX51_, bf)
#define HW_DFLT_BF_VAL(bf) _HW_CONCAT(DFLT_BF_VAL_NX51_, bf)
#define HW_DFLT_VAL(reg)   _HW_CONCAT(DFLT_VAL_NX51_, reg)
#define HW_REGADR(reg)     _HW_CONCAT(Adr_NX51_, reg)
#define HW_AREAADR(area)   _HW_CONCAT(Addr_NX51_, area)
#define HW_TYPE(area)      _HW_CONCAT(NX51_, area)

#define NX6_DEV_FREQUENCY 100000000L /* 100 MHz */

/* Set definitions */
#define NX_SYSTIME_UC 1

#define NX_XC_TYPE_RPU  NX6_XC_TYPE_RPU
#define NX_XC_TYPE_TPU  NX6_XC_TYPE_TPU
#define NX_XC_TYPE_RPEC NX6_XC_TYPE_RPEC
#define NX_XC_TYPE_TPEC NX6_XC_TYPE_TPEC

#define MSK_NX_XC_TYPE_RPU  MSK_NX6_XC_TYPE_RPU
#define MSK_NX_XC_TYPE_TPU  MSK_NX6_XC_TYPE_TPU
#define MSK_NX_XC_TYPE_XMAC MSK_NX6_XC_TYPE_XMAC
#define MSK_NX_XC_TYPE_RPEC MSK_NX6_XC_TYPE_RPEC
#define MSK_NX_XC_TYPE_TPEC MSK_NX6_XC_TYPE_TPEC
#define MSK_NX_XC_TYPE_XPEC MSK_NX6_XC_TYPE_XPEC
#define MSK_NX_XC_TYPE_ALL  MSK_NX6_XC_TYPE_ALL

#define NX_INT_PHYCTRL_LED_MODE_DIS      NX6_INT_PHYCTRL_LED_MODE_MANUAL
#define NX_INT_PHYCTRL_LED_MODE_STATIC   NX6_INT_PHYCTRL_LED_MODE_STATIC
#define NX_INT_PHYCTRL_LED_MODE_FLASHING NX6_INT_PHYCTRL_LED_MODE_FLASHING
#define NX_INT_PHYCTRL_LED_MODE_COMBINED NX6_INT_PHYCTRL_LED_MODE_COMBINED

#define NX_MIIMU_SEND_PREAMBLE                 NX6_MIIMU_SEND_PREAMBLE
#define NX_MIIMU_MDCFREQ_HIGH                  NX6_MIIMU_MDCFREQ_HIGH
#define NX_MIIMU_RTA_0_BITS                    NX6_MIIMU_RTA_0_BITS
#define NX_DRV_CB12_CONTROL_STATUS_INDICATION  NX6_DRV_CB12_CONTROL_STATUS_INDICATION


/*** set default DPM mode ***/
#if !defined(NX6_SERIAL_DPM) && !defined(NX6_PARALLEL_DPM)
#define NX6_PARALLEL_DPM
#endif

/*** Access Macros ***/
#ifdef NX6_PARALLEL_DPM

#define HOST_DPM_START_ADDR  0x60000000 /* start address of DPM within address area of host processor */
#define NX_WRITE32(var,val) (var) = (val)
#define NX_READ32(var)      (var)
#define NX_WRITE16(var,val) (var) = (val)
#define NX_READ16(var)      (var)
#define NX_WRITE8(var,val)  (var) = (val)
#define NX_READ8(var)       (var)
#define NX_READMEM( dst, src, len )  memcpy( dst, src, len )
#define NX_WRITEMEM( dst, src, len ) memcpy( dst, src, len )

#endif

#ifdef NX6_SERIAL_DPM

#define HOST_DPM_START_ADDR  0 /* start address of DPM within address area of host processor */
#define NX_WRITE32(var,val) NX51_ACCESSNX6_SerialDpmWrite( 4, (uint32_t)&(var), val )
#define NX_READ32(var)      NX51_ACCESSNX6_SerialDpmRead( 4, (uint32_t) &(var) )
#define NX_WRITE16(var,val) NX51_ACCESSNX6_SerialDpmWrite( 2, (uint32_t)&(var), val )
#define NX_READ16(var)      NX51_ACCESSNX6_SerialDpmRead( 2, (uint32_t) &(var) )
#define NX_WRITE8(var,val)  NX51_ACCESSNX6_SerialDpmWrite( 1, (uint32_t)&(var), val )
#define NX_READ8(var)       NX51_ACCESSNX6_SerialDpmRead( 1, (uint32_t) &(var) )
#define NX_READMEM( dst, src, len )   NX51_ACCESSNX6_SerialDpmMemRead( dst, src, len )
#define NX_WRITEMEM( dst, src, len )  NX51_ACCESSNX6_SerialDpmMemWrite( dst, src, len )

#endif

/*** Register areas ***/
#define __USE_ASIC_CTRL         static NX51_ASIC_CTRL_AREA_T*           const s_ptAsicCtrl           = (NX51_ASIC_CTRL_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_ASIC_CTRL);
#define __USE_MMIO_CTRL         static NX51_MMIO_CTRL_AREA_T*           const s_ptMmioCtrl           = (NX51_MMIO_CTRL_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_MMIO_CTRL);
#define __USE_MIIMU             static NX51_MIIMU_AREA_T*               const s_ptMiimu              = (NX51_MIIMU_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_MIIMU);
#define __USE_POINTERFIFO       static NX51_POINTER_FIFO_AREA_T*        const s_ptPFifo              = (NX51_POINTER_FIFO_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_POINTER_FIFO);
#define __USE_TRIGGER_SAMPLE    static NX51_TRIGGER_SAMPLE_UNIT_AREA_T* const s_ptTriggerSample      = (NX51_TRIGGER_SAMPLE_UNIT_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_TRIGGER_SAMPLE_UNIT);
#define __USE_FMMUSM            static NX51_FMMUSM_AREA_T*              const s_ptFmmuSm             = (NX51_FMMUSM_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_FMMUSM);
#define __USE_BUFMAN            static NX51_BUF_MAN_AREA_T*             const s_ptBufMan             = (NX51_BUF_MAN_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_BUF_MAN);
#define __USE_XPIC              static NX51_XPIC_AREA_T*                const s_ptXpic               = (NX51_XPIC_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_XPIC_REGS);
#define __USE_XPIC_DEBUG        static NX51_XPIC_DEBUG_AREA_T*          const s_ptXpicDebug          = (NX51_XPIC_DEBUG_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_XPIC_DEBUG);
#define __USE_XPIC_PRAM         static volatile uint32_t*               const s_pulXpicPram          = (volatile uint32_t*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_XPIC_PRAM);
#define __USE_XPIC_DRAM         static volatile uint32_t*               const s_pulXpicDram          = (volatile uint32_t*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_XPIC_DRAM);
#define __USE_SYSTIME           static NX51_SYSTIME_AREA_T*             const s_ptSystime            = (NX51_SYSTIME_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_SYSTIME);
#define __USE_SYSTIME_UC        static NX51_SYSTIME_AREA_T*             const s_ptSystimeUc          = (NX51_SYSTIME_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_SYSTIME_UC);
#define __USE_SYSTIMES          static NX51_SYSTIME_AREA_T*             const s_aptSystime[]         = \
  {\
    (NX51_SYSTIME_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_SYSTIME),\
    (NX51_SYSTIME_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_SYSTIME_UC)\
  };
#define __USE_CANCTRL           static NX51_CANCTRL_AREA_T*             const s_ptCanCtrl            = (NX51_CANCTRL_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_CAN_CTRL);
#define __USE_DMAC_REG          static NX51_DMAC_REG_AREA_T*            const s_ptDmacReg            = (NX51_DMAC_REG_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_DMAC_REG);
#define __USE_DPM               static NX51_DPM_AREA_T*                 const s_ptDpm                = (NX51_DPM_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_FIX_DPM_CFG_WINDOW);
#define __USE_HIF_IO_CTRL       static NX51_HIF_IO_CTRL_AREA_T*         const s_ptHifIoCtrl          = (NX51_HIF_IO_CTRL_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_HIF_IO_CTRL);
#define __USE_HANDSHAKE_CTRL    static NX51_HANDSHAKE_CTRL_AREA_T*      const s_ptHsCtrl             = (NX51_HANDSHAKE_CTRL_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_HANDSHAKE_CTRL);
#define __USE_GPIO              static NX51_GPIO_AREA_T*                const s_ptGpio               = (NX51_GPIO_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_GPIO);
#define __USE_PIO               static NX51_PIO_AREA_T*                 const s_ptPio                = (NX51_PIO_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_PIO);
#define __USE_XPEC_IRQ_REGS     static NX51_XPEC_IRQ_REGISTERS_AREA_T*  const s_ptXpecIrqRegs        = (NX51_XPEC_IRQ_REGISTERS_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_XPEC_IRQ_REGISTERS);
#define __USE_WATCHDOG          static NX51_WATCHDOG_AREA_T*            const s_ptWatchdog           = (NX51_WATCHDOG_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_WATCHDOG);
#define __USE_SQI               static NX51_SQI_AREA_T*                 const s_ptSqi                = (NX51_SQI_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_SQI);
#define __USE_CRC               static NX51_CRC_AREA_T*                 const s_ptCrc                = (NX51_CRC_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_CRC);
#define __USE_UART              static NX51_UART_AREA_T*                const s_aptUart[]            = \
  {\
    (NX51_UART_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_UART0),\
    (NX51_UART_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_UART1),\
    (NX51_UART_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_UART2),\
  };
#define __USE_I2C               static NX51_I2C_AREA_T*                 const s_aptI2C[]             = \
  {\
    (NX51_I2C_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_I2C0),\
    (NX51_I2C_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_I2C1),\
  };
#define __USE_DMAC_CH           static NX51_DMAC_CH_AREA_T*             const s_aptDmacCh[]          = \
  {\
    (NX51_DMAC_CH_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_DMAC_CH0),\
    (NX51_DMAC_CH_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_DMAC_CH1),\
    (NX51_DMAC_CH_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_DMAC_CH2),\
  };
#define __USE_INT_PHY_CTRL      static NX51_INT_PHY_CTRL_AREA_T*        const s_aptIntPhyCtrl[]       = \
  {\
    (NX51_INT_PHY_CTRL_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_INT_PHY_CTRL_0),\
    (NX51_INT_PHY_CTRL_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_INT_PHY_CTRL_1),\
  };
#define __USE_XC_START_STOP     static NX51_XC_START_STOP_AREA_T*       const s_ptXcStartStop         = (NX51_XC_START_STOP_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_XC_START_STOP);
#define __USE_XC_DEBUG          static NX51_XC_DEBUG_AREA_T*            const s_ptXcDebug             = (NX51_XC_DEBUG_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_XC_DEBUG);
#define __USE_RPEC_REGS         static NX51_XPEC_AREA_T*                const s_aptRpecRegArea[]      = \
  {\
    (NX51_XPEC_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_RPEC0_REG),\
    (NX51_XPEC_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_RPEC1_REG),\
  };
#define __USE_TPEC_REGS         static NX51_XPEC_AREA_T*                const s_aptTpecRegArea[]      = \
  {\
    (NX51_XPEC_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_TPEC0_REG),\
    (NX51_XPEC_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_TPEC1_REG),\
  };
#define __USE_RPEC_PRAM_START   static volatile uint32_t*          const s_apulRpecPramArea[]    = \
  {\
    (volatile uint32_t*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_RPEC0_PRAM),\
    (volatile uint32_t*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_RPEC1_PRAM),\
  };
#define __USE_RPEC_PRAM_END     static volatile uint32_t*          const s_apulRpecPramAreaEnd[] = \
  {\
    (volatile uint32_t*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_RPEC0_PRAM + REL_Adr_NX51_ram_end),\
    (volatile uint32_t*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_RPEC1_PRAM + REL_Adr_NX51_ram_end),\
  };
#define __USE_TPEC_PRAM_START   static volatile uint32_t*          const s_apulTpecPramArea[]    = \
  {\
    (volatile uint32_t*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_TPEC0_PRAM),\
    (volatile uint32_t*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_TPEC1_PRAM),\
  };
#define __USE_TPEC_PRAM_END     static volatile uint32_t*          const s_apulTpecPramAreaEnd[] = \
  {\
    (volatile uint32_t*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_TPEC0_PRAM + REL_Adr_NX51_ram_end),\
    (volatile uint32_t*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_TPEC1_PRAM + REL_Adr_NX51_ram_end),\
  };
#define __USE_XPEC_DRAM         static volatile uint32_t*          const s_apulXpecDramArea[]    = \
  {\
    (volatile uint32_t*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_TPEC0_DRAM),\
    (volatile uint32_t*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_TPEC1_DRAM),\
  };
#define __USE_XMAC              static NX51_XMAC_AREA_T*                const s_aptXmacArea[]         = \
  {\
    (NX51_XMAC_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_XMAC0_REG),\
    (NX51_XMAC_AREA_T*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_XMAC1_REG),\
  };
#define __USE_RPU_PRAM_START    static volatile uint32_t*          const s_aptRpuPramArea[]      = \
  {\
    (volatile uint32_t*)  (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_RPU0_PRAM),\
    (volatile uint32_t*)  (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_RPU1_PRAM),\
  };
#define __USE_TPU_PRAM_START    static volatile uint32_t*          const s_aptTpuPramArea[]      = \
  {\
    (volatile uint32_t*)  (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_TPU0_PRAM),\
    (volatile uint32_t*)  (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_TPU1_PRAM),\
  };
#define __USE_RPU_PRAM_END      static volatile uint32_t*          const s_aptRpuPramAreaEnd[]   = \
  {\
  (volatile uint32_t*)  (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_RPU0_PRAM + REL_Adr_NX51_xmac_ram_end),\
  (volatile uint32_t*)  (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_RPU1_PRAM + REL_Adr_NX51_xmac_ram_end),\
  };
#define __USE_TPU_PRAM_END      static volatile uint32_t*          const s_aptTpuPramAreaEnd[]   = \
  {\
  (volatile uint32_t*)  (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_TPU0_PRAM + REL_Adr_NX51_xmac_ram_end),\
  (volatile uint32_t*)  (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_TPU1_PRAM + REL_Adr_NX51_xmac_ram_end),\
  };
#define __USE_INTRAM            static volatile uint32_t*          const s_apulIntRamStart[]     = \
  {\
    (volatile uint32_t*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_INTRAM7),\
    (volatile uint32_t*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_INTRAM8),\
  };

#define __USE_XPIC_INTRAM       static volatile uint32_t*          const s_pulIntRamXpicStart    = (volatile uint32_t*) (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_INTRAM6);


/*** Pointer to common functions ***/

/*** MIIMU  ***/
typedef int(*NX6_PFN_MIIMU_READ_PHY_REG) ( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuRtaField, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, uint16_t* pusData, void* pvUser );
typedef int(*NX6_PFN_MIIMU_WRITE_PHY_REG)( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, uint16_t usData, void* pvUser );
#define __USE_MIIMU_FUNC_READ_PHY  static NX6_PFN_MIIMU_READ_PHY_REG  pfnMiimuReadPhyReg  = NX6_MIIMU_ReadPhyReg;
#define __USE_MIIMU_FUNC_WRITE_PHY static NX6_PFN_MIIMU_WRITE_PHY_REG pfnMiimuWritePhyReg = NX6_MIIMU_WritePhyReg;

/*** SYSTIME  ***/
typedef void(*NX6_PFN_SYSTIME_SET_BORDER)  ( uint32_t ulBorder, void* pvUser );
typedef void(*NX6_PFN_SYSTIME_SET_SPEED)   ( uint32_t ulSpeed, void* pvUser );
typedef void(*NX6_PFN_SYSTIME_SET_SYSTIME) ( uint32_t ulSystime_s, uint32_t ulSystime_ns, void* pvUser );
typedef void(*NX6_PFN_SYSTIME_GET_SYSTIME) ( uint32_t* pulSystime_s, uint32_t* pulSystime_ns, void* pvUser );
#define __USE_SYSTIME_FUNC_SET_BORDER  static NX6_PFN_SYSTIME_SET_BORDER  pfnSystimeSetBorder   = NX6_SYSTIME_SetBorder;
#define __USE_SYSTIME_FUNC_SET_SPEED   static NX6_PFN_SYSTIME_SET_SPEED   pfnSystimeSetSpeed    = NX6_SYSTIME_SetSpeed;
#define __USE_SYSTIME_FUNC_SET_SYSTIME static NX6_PFN_SYSTIME_SET_SYSTIME pfnSystimeSetSystime  = NX6_SYSTIME_SetSystime;
#define __USE_SYSTIME_FUNC_GET_SYSTIME static NX6_PFN_SYSTIME_GET_SYSTIME pfnSystimeGetSystime  = NX6_SYSTIME_GetSystime;

/*** XC loader ***/
typedef int(*NX6_PFN_XC_RESET)      (unsigned int uiPort, void* pvUser);
typedef int(*NX6_PFN_XC_LOAD)       (unsigned int uiPort, NX6_XC_TYPE_E eXcType, const uint32_t* pulXcPrg, void* pvUser);
typedef int(*NX6_PFN_XC_START)      (unsigned int uiPort, void* pvUser);
typedef int(*NX6_PFN_XC_START_UNITS)(unsigned int uiPort, unsigned int uiUnitVec, void* pvUser);

#define __USE_XC_LOADER_FUNC_RESET       static NX6_PFN_XC_RESET       pfnXcReset      = NX6_XC_Reset;
#define __USE_XC_LOADER_FUNC_LOAD        static NX6_PFN_XC_LOAD        pfnXcLoad       = NX6_XC_Load;
#define __USE_XC_LOADER_FUNC_START       static NX6_PFN_XC_START       pfnXcStart      = NX6_XC_Start;
#define __USE_XC_LOADER_FUNC_START_UNITS static NX6_PFN_XC_START_UNITS pfnXcStartUnits = NX6_XC_StartUnits;

/*** xPIC loader ***/
typedef void(*NX6_PFN_XPIC_RESET)(void* pvUser);
typedef void(*NX6_PFN_XPIC_LOAD) (const uint32_t* pulXpicPrg, void* pvUser);
typedef void(*NX6_PFN_XPIC_START)(void* pvUser);

#define __USE_XPIC_LOADER_FUNC_RESET static NX6_PFN_XPIC_RESET pfnXpicReset = NX6_XPIC_Reset;
#define __USE_XPIC_LOADER_FUNC_LOAD  static NX6_PFN_XPIC_LOAD  pfnXpicLoad  = NX6_XPIC_Load;
#define __USE_XPIC_LOADER_FUNC_START static NX6_PFN_XPIC_START pfnXpicStart = NX6_XPIC_Start;

/*** IRQ lock/unlock ***/
#define lock_irqfiq_save(x)    x = IRQ_LockSave()
#define lock_irqfiq_restore(x) IRQ_LockRestore(x)

#endif /* __HAL_RESOURCES_DEFINES_NETX6_H */
