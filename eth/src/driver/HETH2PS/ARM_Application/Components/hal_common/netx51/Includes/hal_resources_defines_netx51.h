#ifndef __HAL_RESOURCES_DEFINES_H
#define __HAL_RESOURCES_DEFINES_H

#include "regdef_netx51.h" /* used for structures and addresses */
#include "common_func_netx51.h" /* used for function names */
#include <string.h>

#define _HW_CONCAT(a,b) a ## b
#define HW_MSK(bf)         _HW_CONCAT(MSK_NX51_, bf)
#define HW_SRT(bf)         _HW_CONCAT(SRT_NX51_, bf)
#define HW_DFLT_BF_VAL(bf) _HW_CONCAT(DFLT_BF_VAL_NX51_, bf)
#define HW_DFLT_VAL(reg)   _HW_CONCAT(DFLT_VAL_NX51_, reg)
#define HW_REGADR(reg)     _HW_CONCAT(Adr_NX51_, reg)
#define HW_AREAADR(area)   _HW_CONCAT(Addr_NX51_, area)
#define HW_TYPE(area)      _HW_CONCAT(NX51_, area)

#define NX51_DEV_FREQUENCY 100000000L /* 100 MHz */

/* Set definitions */
#define NX_SYSTIME_UC 1

#define NX_XC_TYPE_RPU  NX51_XC_TYPE_RPU
#define NX_XC_TYPE_TPU  NX51_XC_TYPE_TPU
#define NX_XC_TYPE_RPEC NX51_XC_TYPE_RPEC
#define NX_XC_TYPE_TPEC NX51_XC_TYPE_TPEC

#define MSK_NX_XC_TYPE_RPU  MSK_NX51_XC_TYPE_RPU
#define MSK_NX_XC_TYPE_TPU  MSK_NX51_XC_TYPE_TPU
#define MSK_NX_XC_TYPE_XMAC MSK_NX51_XC_TYPE_XMAC
#define MSK_NX_XC_TYPE_RPEC MSK_NX51_XC_TYPE_RPEC
#define MSK_NX_XC_TYPE_TPEC MSK_NX51_XC_TYPE_TPEC
#define MSK_NX_XC_TYPE_XPEC MSK_NX51_XC_TYPE_XPEC
#define MSK_NX_XC_TYPE_ALL  MSK_NX51_XC_TYPE_ALL

#define NX_INT_PHYCTRL_LED_MODE_DIS      NX51_INT_PHYCTRL_LED_MODE_MANUAL
#define NX_INT_PHYCTRL_LED_MODE_STATIC   NX51_INT_PHYCTRL_LED_MODE_STATIC
#define NX_INT_PHYCTRL_LED_MODE_FLASHING NX51_INT_PHYCTRL_LED_MODE_FLASHING
#define NX_INT_PHYCTRL_LED_MODE_COMBINED NX51_INT_PHYCTRL_LED_MODE_COMBINED

#define NX_MIIMU_SEND_PREAMBLE                 NX51_MIIMU_SEND_PREAMBLE
#define NX_MIIMU_MDCFREQ_HIGH                  NX51_MIIMU_MDCFREQ_HIGH
#define NX_MIIMU_RTA_0_BITS                    NX51_MIIMU_RTA_0_BITS
#define NX_DRV_CB12_CONTROL_STATUS_INDICATION  NX51_DRV_CB12_CONTROL_STATUS_INDICATION

/*** Register areas ***/
#define __USE_ASIC_CTRL         static NX51_ASIC_CTRL_AREA_T*           const s_ptAsicCtrl           = (NX51_ASIC_CTRL_AREA_T*) Addr_NX51_asic_ctrl;
#define __USE_MMIO_CTRL         static NX51_MMIO_CTRL_AREA_T*           const s_ptMmioCtrl           = (NX51_MMIO_CTRL_AREA_T*) Addr_NX51_mmio_ctrl;
#define __USE_MIIMU             static NX51_MIIMU_AREA_T*               const s_ptMiimu              = (NX51_MIIMU_AREA_T*)Addr_NX51_miimu;
#define __USE_POINTERFIFO       static NX51_POINTER_FIFO_AREA_T*        const s_ptPFifo              = (NX51_POINTER_FIFO_AREA_T*) Addr_NX51_pointer_fifo;
#define __USE_NFIFO             static NX51_NFIFO_AREA_T*               const s_ptNFifo              = (NX51_NFIFO_AREA_T*) Addr_NX51_nfifo;
#define __USE_TRIGGER_SAMPLE    static NX51_TRIGGER_SAMPLE_UNIT_AREA_T* const s_ptTriggerSample      = (NX51_TRIGGER_SAMPLE_UNIT_AREA_T*) Addr_NX51_trigger_sample_unit;
#define __USE_FMMUSM            static NX51_FMMUSM_AREA_T*              const s_ptFmmuSm             = (NX51_FMMUSM_AREA_T*) Addr_NX51_fmmusm;
#define __USE_BUFMAN            static NX51_BUF_MAN_AREA_T*             const s_ptBufMan             = (NX51_BUF_MAN_AREA_T*) Addr_NX51_buf_man;
#define __USE_XPIC              static NX51_XPIC_AREA_T*                const s_ptXpic               = (NX51_XPIC_AREA_T*) Addr_NX51_xpic_regs;
#define __USE_XPIC_DEBUG        static NX51_XPIC_DEBUG_AREA_T*          const s_ptXpicDebug          = (NX51_XPIC_DEBUG_AREA_T*) Addr_NX51_xpic_debug;
#define __USE_XPIC_DRAM         static volatile uint32_t*               const s_pulXpicDram          = (volatile uint32_t*) Addr_NX51_xpic_dram;
#define __USE_ETH               static NX51_ETH_AREA_T*                 const s_ptEth                = (NX51_ETH_AREA_T*) Addr_NX51_eth;
#define __USE_ARM_TIMER         static NX51_ARM_TIMER_AREA_T*           const s_ptArmTimer           = (NX51_ARM_TIMER_AREA_T*) Addr_NX51_arm_timer;
#define __USE_XPIC_TIMER        static NX51_XPIC_TIMER_AREA_T*          const s_ptXpicTimer          = (NX51_XPIC_TIMER_AREA_T*) Addr_NX51_xpic_timer;
#define __USE_SYSTIME           static NX51_SYSTIME_AREA_T*             const s_ptSystime            = (NX51_SYSTIME_AREA_T*) Addr_NX51_systime;
#define __USE_SYSTIME_UC        static NX51_SYSTIME_AREA_T*             const s_ptSystimeUc          = (NX51_SYSTIME_AREA_T*) Addr_NX51_systime_uc;
#define __USE_SYSTIMES          static NX51_SYSTIME_AREA_T*             const s_aptSystime[]         = \
  {\
    (NX51_SYSTIME_AREA_T*) Addr_NX51_systime,\
    (NX51_SYSTIME_AREA_T*) Addr_NX51_systime_uc\
  };
#define __USE_CANCTRL           static NX51_CANCTRL_AREA_T*             const s_ptCanCtrl            = (NX51_CANCTRL_AREA_T*) Addr_NX51_canctrl;
#define __USE_DMAC_REG          static NX51_DMAC_REG_AREA_T*            const s_ptDmacReg            = (NX51_DMAC_REG_AREA_T*) Addr_NX51_dmac_reg;
#define __USE_DPM               static NX51_DPM_AREA_T*                 const s_ptDpm                = (NX51_DPM_AREA_T*) Addr_NX51_dpm;
#define __USE_HANDSHAKE_CTRL    static NX51_HANDSHAKE_CTRL_AREA_T*      const s_ptHsCtrl             = (NX51_HANDSHAKE_CTRL_AREA_T*) Addr_NX51_handshake_ctrl;
#define __USE_HIF_IO_CTRL       static NX51_HIF_IO_CTRL_AREA_T*         const s_ptHifIoCtrl          = (NX51_HIF_IO_CTRL_AREA_T*) Addr_NX51_hif_io_ctrl;
#define __USE_HIF_ASYNCMEM_CTRL static NX51_EXT_ASYNCMEM_CTRL_AREA_T*   const s_ptHifAsyncMemCtrl    = (NX51_EXT_ASYNCMEM_CTRL_AREA_T*) Addr_NX51_hif_asyncmem_ctrl;
#define __USE_GPIO              static NX51_GPIO_AREA_T*                const s_ptGpio               = (NX51_GPIO_AREA_T*) Addr_NX51_gpio;
#define __USE_PIO               static NX51_PIO_AREA_T*                 const s_ptPio                = (NX51_PIO_AREA_T*) Addr_NX51_pio;
#define __USE_USB_DEV_CTRL      static NX51_USB_DEV_CTRL_AREA_T*        const s_ptUsbDevCtrl         = (NX51_USB_DEV_CTRL_AREA_T*) Addr_NX51_usb_dev_ctrl;
#define __USE_USB_DEV_ENUM_RAM  static uint32_t*                        const s_pulUsbDevEnumRam     = (uint32_t*) Addr_NX51_usb_dev_enum_ram;
#define __USE_USB_DEV_FIFO_CTRL static NX51_USB_DEV_FIFO_CTRL_AREA_T*   const s_ptUsbDevFifoCtrl     = (NX51_USB_DEV_FIFO_CTRL_AREA_T*) Addr_NX51_usb_dev_fifo_ctrl;
#define __USE_USB_DEV_FIFO      static NX51_USB_DEV_FIFO_AREA_T*        const s_ptUsbDevFifo         = (NX51_USB_DEV_FIFO_AREA_T*) Addr_NX51_usb_dev_fifo;
#define __USE_VIC               static NX51_VIC_AREA_T*                 const s_ptVic                = (NX51_VIC_AREA_T*) Addr_NX51_vic;
#define __USE_XPEC_IRQ_REGS     static NX51_XPEC_IRQ_REGISTERS_AREA_T*  const s_ptXpecIrqRegs        = (NX51_XPEC_IRQ_REGISTERS_AREA_T*) Addr_NX51_xpec_irq_registers;
#define __USE_WATCHDOG          static NX51_WATCHDOG_AREA_T*            const s_ptWatchdog           = (NX51_WATCHDOG_AREA_T*) Addr_NX51_watchdog;
#define __USE_SQI               static NX51_SQI_AREA_T*                 const s_ptSqi                = (NX51_SQI_AREA_T*) Addr_NX51_sqi;
#define __USE_SPI_MOTION        static NX51_SPI_AREA_T*                 const s_ptSpiMotion          = (NX51_SPI_AREA_T*) Addr_NX51_spi_motion;
#define __USE_CRC               static NX51_CRC_AREA_T*                 const s_ptCrc                = (NX51_CRC_AREA_T*) Addr_NX51_crc;
#define __USE_UART              static NX51_UART_AREA_T*                const s_aptUart[]            = \
  {\
    (NX51_UART_AREA_T*) Addr_NX51_uart0,\
    (NX51_UART_AREA_T*) Addr_NX51_uart1,\
    (NX51_UART_AREA_T*) Addr_NX51_uart2,\
  };
#define __USE_I2C               static NX51_I2C_AREA_T*                 const s_aptI2C[]            = \
  {\
    (NX51_I2C_AREA_T*) Addr_NX51_i2c0,\
    (NX51_I2C_AREA_T*) Addr_NX51_i2c1,\
  };
#define __USE_DMAC_CH           static NX51_DMAC_CH_AREA_T*             const s_aptDmacCh[]          = \
  {\
    (NX51_DMAC_CH_AREA_T*) Addr_NX51_dmac_ch0,\
    (NX51_DMAC_CH_AREA_T*) Addr_NX51_dmac_ch1,\
    (NX51_DMAC_CH_AREA_T*) Addr_NX51_dmac_ch2,\
  };
#define __USE_INT_PHY_CTRL      static NX51_INT_PHY_CTRL_AREA_T*        const s_aptIntPhyCtrl[]       = \
  {\
    (NX51_INT_PHY_CTRL_AREA_T*) Addr_NX51_int_phy_ctrl0,\
    (NX51_INT_PHY_CTRL_AREA_T*) Addr_NX51_int_phy_ctrl1,\
  };
#define __USE_XC_START_STOP     static NX51_XC_START_STOP_AREA_T*       const s_ptXcStartStop         = (NX51_XC_START_STOP_AREA_T*) Addr_NX51_xc_start_stop;
#define __USE_XC_DEBUG          static NX51_XC_DEBUG_AREA_T*            const s_ptXcDebug             = (NX51_XC_DEBUG_AREA_T*) Addr_NX51_xc_debug;
#define __USE_RPEC_REGS         static NX51_XPEC_AREA_T*                const s_aptRpecRegArea[]      = \
  {\
    (NX51_XPEC_AREA_T*) Addr_NX51_rpec0_regs,\
    (NX51_XPEC_AREA_T*) Addr_NX51_rpec1_regs,\
  };
#define __USE_TPEC_REGS         static NX51_XPEC_AREA_T*                const s_aptTpecRegArea[]      = \
  {\
    (NX51_XPEC_AREA_T*) Addr_NX51_tpec0_regs,\
    (NX51_XPEC_AREA_T*) Addr_NX51_tpec1_regs,\
  };
#define __USE_RPEC_PRAM_START   static volatile uint32_t*               const s_apulRpecPramArea[]    = \
  {\
    (volatile uint32_t*) Adr_NX51_rpec0_pram_ram_start,\
    (volatile uint32_t*) Adr_NX51_rpec1_pram_ram_start,\
  };
#define __USE_RPEC_PRAM_END     static volatile uint32_t*               const s_apulRpecPramAreaEnd[] = \
  {\
    (volatile uint32_t*) Adr_NX51_rpec0_pram_ram_end,\
    (volatile uint32_t*) Adr_NX51_rpec1_pram_ram_end,\
  };
#define __USE_TPEC_PRAM_START   static volatile uint32_t*               const s_apulTpecPramArea[]    = \
  {\
    (volatile uint32_t*) Adr_NX51_tpec0_pram_ram_start,\
    (volatile uint32_t*) Adr_NX51_tpec1_pram_ram_start,\
  };
#define __USE_TPEC_PRAM_END     static volatile uint32_t*               const s_apulTpecPramAreaEnd[] = \
  {\
    (volatile uint32_t*) Adr_NX51_tpec0_pram_ram_end,\
    (volatile uint32_t*) Adr_NX51_tpec1_pram_ram_end,\
  };
#define __USE_XPEC_DRAM         static volatile uint32_t*               const s_apulXpecDramArea[]    = \
  {\
    (volatile uint32_t*) Adr_NX51_tpec0_dram_ram_start,\
    (volatile uint32_t*) Adr_NX51_tpec1_dram_ram_start,\
  };
#define __USE_XMAC              static NX51_XMAC_AREA_T*                const s_aptXmacArea[]         = \
  {\
    (NX51_XMAC_AREA_T*) Addr_NX51_xmac0_regs,\
    (NX51_XMAC_AREA_T*) Addr_NX51_xmac1_regs,\
  };
#define __USE_RPU_PRAM_START    static volatile uint32_t*               const s_aptRpuPramArea[]      = \
  {\
    (volatile uint32_t*)  Adr_NX51_rpu0_ram_xmac_ram_start,\
    (volatile uint32_t*)  Adr_NX51_rpu1_ram_xmac_ram_start,\
  };
#define __USE_TPU_PRAM_START    static volatile uint32_t*               const s_aptTpuPramArea[]      = \
  {\
    (volatile uint32_t*)  Adr_NX51_tpu0_ram_xmac_ram_start,\
    (volatile uint32_t*)  Adr_NX51_tpu1_ram_xmac_ram_start,\
  };
#define __USE_RPU_PRAM_END      static volatile uint32_t*               const s_aptRpuPramAreaEnd[]   = \
  {\
    (volatile uint32_t*)  Adr_NX51_rpu0_ram_xmac_ram_end,\
    (volatile uint32_t*)  Adr_NX51_rpu1_ram_xmac_ram_end,\
  };
#define __USE_TPU_PRAM_END      static volatile uint32_t*               const s_aptTpuPramAreaEnd[]   = \
  {\
    (volatile uint32_t*)  Adr_NX51_tpu0_ram_xmac_ram_end,\
    (volatile uint32_t*)  Adr_NX51_tpu1_ram_xmac_ram_end,\
  };
#define __USE_INTRAM            static volatile uint32_t*               const s_apulIntRamStart[]     = \
  {\
    (volatile uint32_t*) Adr_NX51_intram7_intram7_base,\
    (volatile uint32_t*) Adr_NX51_intram8_intram8_base,\
  };

#define __USE_XPIC_INTRAM       static volatile uint32_t*               const s_pulIntRamXpicStart    = (volatile uint32_t*) Adr_NX51_intram6_intram6_base;

/*** Pointer to common functions ***/

/*** MIIMU ***/
typedef int(*NX51_PFN_MIIMU_READ_PHY_REG) ( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuRtaField, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, uint16_t* pusData, void* pvUser );
typedef int(*NX51_PFN_MIIMU_WRITE_PHY_REG)( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, uint16_t usData, void* pvUser );

#define __USE_MIIMU_FUNC_READ_PHY  static NX51_PFN_MIIMU_READ_PHY_REG  pfnMiimuReadPhyReg  = NX51_MIIMU_ReadPhyReg;
#define __USE_MIIMU_FUNC_WRITE_PHY static NX51_PFN_MIIMU_WRITE_PHY_REG pfnMiimuWritePhyReg = NX51_MIIMU_WritePhyReg;

/*** SYSTIME ***/
typedef void(*NX51_PFN_SYSTIME_SET_BORDER)  ( uint32_t ulBorder, void* pvUser );
typedef void(*NX51_PFN_SYSTIME_SET_SPEED)   ( uint32_t ulSpeed, void* pvUser );
typedef void(*NX51_PFN_SYSTIME_SET_SYSTIME) ( uint32_t ulSystime_s, uint32_t ulSystime_ns, void* pvUser );
typedef void(*NX51_PFN_SYSTIME_GET_SYSTIME) ( uint32_t* pulSystime_s, uint32_t* pulSystime_ns, void* pvUser );

#define __USE_SYSTIME_FUNC_SET_BORDER  static NX51_PFN_SYSTIME_SET_BORDER  pfnSystimeSetBorder   = NX51_SYSTIME_SetBorder;
#define __USE_SYSTIME_FUNC_SET_SPEED   static NX51_PFN_SYSTIME_SET_SPEED   pfnSystimeSetSpeed    = NX51_SYSTIME_SetSpeed;
#define __USE_SYSTIME_FUNC_SET_SYSTIME static NX51_PFN_SYSTIME_SET_SYSTIME pfnSystimeSetSystime  = NX51_SYSTIME_SetSystime;
#define __USE_SYSTIME_FUNC_GET_SYSTIME static NX51_PFN_SYSTIME_GET_SYSTIME pfnSystimeGetSystime  = NX51_SYSTIME_GetSystime;

/*** XC loader ***/
typedef int(*NX51_PFN_XC_RESET)( unsigned int uPortNo, void* pvUser );
typedef int(*NX51_PFN_XC_LOAD) ( unsigned int uPortNo, NX51_XC_TYPE_E eXcType, const uint32_t* pulXcPrg, void* pvUser );
typedef int(*NX51_PFN_XC_START)( unsigned int uPortNo, void* pvUser );
typedef int(*NX51_PFN_XC_START_UNITS)( unsigned int uPortNo, unsigned int uUnitVec, void* pvUser );

#define __USE_XC_LOADER_FUNC_RESET       static NX51_PFN_XC_RESET       pfnXcReset      = NX51_XC_Reset;
#define __USE_XC_LOADER_FUNC_LOAD        static NX51_PFN_XC_LOAD        pfnXcLoad       = NX51_XC_Load;
#define __USE_XC_LOADER_FUNC_START       static NX51_PFN_XC_START       pfnXcStart      = NX51_XC_Start;
#define __USE_XC_LOADER_FUNC_START_UNITS static NX51_PFN_XC_START_UNITS pfnXcStartUnits = NX51_XC_StartUnits;

/*** xPIC loader ***/
typedef void(*NX51_PFN_XPIC_RESET)(void* pvUser);
typedef void(*NX51_PFN_XPIC_LOAD) (const uint32_t* pulXpicPrg, void* pvUser);
typedef void(*NX51_PFN_XPIC_START)(void* pvUser);

#define __USE_XPIC_LOADER_FUNC_RESET static NX51_PFN_XPIC_RESET pfnXpicReset = NX51_XPIC_Reset;
#define __USE_XPIC_LOADER_FUNC_LOAD  static NX51_PFN_XPIC_LOAD  pfnXpicLoad  = NX51_XPIC_Load;
#define __USE_XPIC_LOADER_FUNC_START static NX51_PFN_XPIC_START pfnXpicStart = NX51_XPIC_Start;


/*** Access Macros ***/
#define NX_WRITE32(var,val) (var) = (val)
#define NX_READ32(var)      (var)
#define NX_WRITE16(var,val) (var) = (val)
#define NX_READ16(var)      (var)
#define NX_WRITE8(var,val)  (var) = (val)
#define NX_READ8(var)       (var)
/* Copy data from netX to Host */
#define NX_READMEM(dst, src, len) memcpy(dst, src, len)
/* Copy data from Host to netX */
#define NX_WRITEMEM(dst, src, len) memcpy(dst, src, len)

/*** IRQ lock/unlock ***/
#define lock_irqfiq_save(x)    x = IRQ_LockSave()
#define lock_irqfiq_restore(x) IRQ_LockRestore(x)

#endif /* __HAL_RESOURCES_DEFINES_H */
