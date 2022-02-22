#ifndef __HW_DEFINES_H
#define __HW_DEFINES_H

#if defined(__NETX90_MPW)
#error netX90MPW not supported
#endif

#define __NETX90 1

#if !defined(CPU_IS_ARM_APP) && !defined(CPU_IS_ARM)
#define CPU_IS_ARM
#endif

#define _HW_CONCAT(a,b) a ## b
#define HW_MSK(bf)         _HW_CONCAT(MSK_NX90_, bf)
#define HW_SRT(bf)         _HW_CONCAT(SRT_NX90_, bf)
#define HW_DFLT_BF_VAL(bf) _HW_CONCAT(DFLT_BF_VAL_NX90_, bf)
#define HW_DFLT_VAL(reg)   _HW_CONCAT(DFLT_VAL_NX90_, reg)
#define HW_REGADR(reg)     _HW_CONCAT(Adr_NX90_, reg)
#define HW_AREAADR(area)   _HW_CONCAT(Addr_NX90_, area)
#define HW_TYPE(area)      _HW_CONCAT(NX90_, area)

#include "hal_armtimer.h"
#define SYS_SLEEP_CC(cc) ArmTimer_Sleep(0,cc)

#define HW_DEV_FREQUENCY 100000000L // 100 Mhz

typedef struct NX90_XPEC_IRQ_REGS_Ttag
{
  volatile uint32_t aulIrq_xpec[4];
} NX90_XPEC_IRQ_REGS_T;


/*** Register areas ***/

#ifdef CPU_IS_ARM_APP
#include "regdef_netx90_arm_app.h"
/* ARM_APP */
#define HW_PTR_PIO(var)                 static NX90_PIO_AREA_T*                  const var   = (NX90_PIO_AREA_T*) Addr_NX90_pio_app;
#define HW_PTR_SPI(var)                 static NX90_SPI_AREA_T*                  const var[] = \
  {\
    (NX90_SPI_AREA_T*) Addr_NX90_spi0_app,\
    (NX90_SPI_AREA_T*) Addr_NX90_spi1_app,\
    (NX90_SPI_AREA_T*) Addr_NX90_spi2_app,\
    (NX90_SPI_AREA_T*) Addr_NX90_spi_xpic_app\
  };
#define HW_PTR_SQI(var)                 static NX90_SQI_AREA_T*                  const var[] = \
{\
  (NX90_SQI_AREA_T*) Addr_NX90_sqi,\
  (NX90_SQI_AREA_T*) Addr_NX90_sqi0_app,\
  (NX90_SQI_AREA_T*) Addr_NX90_sqi1_app,\
};
#define HW_PTR_BISS(var)                 static NX90_BISS_AREA_T*                const var[] = \
  {\
    (NX90_BISS_AREA_T*) Addr_NX90_biss0_app,\
    (NX90_BISS_AREA_T*) Addr_NX90_biss1_app\
  };
#define HW_PTR_BISS_CTRL(var)            static NX90_BISS_CTRL_AREA_T*           const var[] = \
  {\
    (NX90_BISS_CTRL_AREA_T*) Addr_NX90_biss_ctrl0_app,\
    (NX90_BISS_CTRL_AREA_T*) Addr_NX90_biss_ctrl1_app\
  };
#define HW_PTR_CANCTRL(var)             static NX90_CANCTRL_AREA_T*              const var[] = \
  {\
    (NX90_CANCTRL_AREA_T*) Addr_NX90_can_ctrl0_app,\
    (NX90_CANCTRL_AREA_T*) Addr_NX90_can_ctrl1_app\
  };

#define HW_PTR_ARM_TIMER(var)           static NX90_ARM_TIMER_AREA_T*            const var   = (NX90_ARM_TIMER_AREA_T*) Addr_NX90_timer_app;
#define HW_PTR_WATCHDOG(var)            static NX90_WATCHDOG_AREA_T*             const var   = (NX90_WATCHDOG_AREA_T*) Addr_NX90_wdg_app;
#define HW_PTR_SYSTIME(var)             static NX90_SYSTIME_AREA_T*              const var[] = \
  {\
    (NX90_SYSTIME_AREA_T*) Addr_NX90_systime_app\
  };
#define HW_PTR_SYSTIME_UC(var)          static NX90_SYSTIME_AREA_T*              const var   = (NX90_SYSTIME_AREA_T*) Addr_NX90_systime_uc_app;
#define HW_PTR_SYSTIMELT(var)           static NX90_INTLOGIC_SYSTIME_LT_AREA_T*  const var[] = \
  {\
    (NX90_INTLOGIC_SYSTIME_LT_AREA_T*) Addr_NX90_systime_lt_app,\
    (NX90_INTLOGIC_SYSTIME_LT_AREA_T*) Addr_NX90_systime_lt_xpic_app\
  };
#define HW_PTR_UART(var)                static NX90_UART_AREA_T*                 const var[] = \
  {\
    (NX90_UART_AREA_T*) Addr_NX90_uart,\
    (NX90_UART_AREA_T*) Addr_NX90_uart_app,\
    (NX90_UART_AREA_T*) Addr_NX90_uart_xpic_app\
  };
#define HW_PTR_I2C(var)                 static NX90_I2C_AREA_T*                  const var[] = \
  {\
    (NX90_I2C_AREA_T*) Addr_NX90_i2c_app,\
    (NX90_I2C_AREA_T*) Addr_NX90_i2c_xpic_app\
  };
#define HW_PTR_GPIO(var)                static NX90_GPIO_APP_AREA_T*             const var[] = \
  {\
    (NX90_GPIO_APP_AREA_T*) Addr_NX90_gpio_app,\
    (NX90_GPIO_APP_AREA_T*) Addr_NX90_gpio_xpic_app\
  };
#define HW_PTR_GPIO_MOTION(var)         static NX90_GPIO_AREA_T*                 const var[] = \
  {\
    (NX90_GPIO_AREA_T*) Addr_NX90_gpio_xpic_app\
  };
#define HW_PTR_XPIC_DEBUG(var)          static NX90_XPIC_DEBUG_AREA_T*           const var[] = \
  {\
    (NX90_XPIC_DEBUG_AREA_T*) Addr_NX90_xpic_app_debug\
  };
#define HW_PTR_XPIC(var)                static NX90_XPIC_AREA_T*                 const var[] = \
  {\
    (NX90_XPIC_AREA_T*) Addr_NX90_xpic_app_regs\
  };
#define HW_PTR_XPIC_DRAM(var)           static uint32_t*                           const var[] = \
  {\
    (uint32_t*) Addr_NX90_xpic_app_dram\
  };
#define HW_PTR_XPIC_PRAM(var)           static uint32_t*                           const var[] = \
  {\
    (uint32_t*) Addr_NX90_xpic_app_pram\
  };
#define HW_PTR_XPIC_VIC(var)            static NX90_XPIC_VIC_AREA_T*             const var[] = \
  {\
    (NX90_XPIC_VIC_AREA_T*) Addr_NX90_vic_xpic_app\
  };
#define HW_PTR_XPIC_WDG(var)            static NX90_XPIC_WDG_AREA_T*             const var[] = \
  {\
    (NX90_XPIC_WDG_AREA_T*) Addr_NX90_wdg_xpic_app\
  };
#define HW_PTR_XPIC_TIMER(var)          static NX90_ARM_TIMER_AREA_T*            const var[] = \
  {\
    (NX90_ARM_TIMER_AREA_T*) Addr_NX90_timer_xpic_app\
  };
#define HW_PTR_XPIC_INTRAM(var)         static uint32_t*                           const var   = (uint32_t*) Addr_NX90_intram7;
#define HW_PTR_DMAC_CH(var)             static NX90_DMAC_CH_AREA_T*              const var[] = \
  {\
    (NX90_DMAC_CH_AREA_T*) Addr_NX90_dmac_app_ch0,\
    (NX90_DMAC_CH_AREA_T*) Addr_NX90_dmac_app_ch1,\
    (NX90_DMAC_CH_AREA_T*) Addr_NX90_dmac_app_ch2\
  };
#define HW_PTR_DMAC_REG(var)            static NX90_DMAC_REG_AREA_T*             const var   = (NX90_DMAC_REG_AREA_T*) Addr_NX90_dmac_app_reg;
#define HW_PTR_DMAC_MUX(var)            static NX90_DMAC_MUX_AREA_T*             const var   = (NX90_DMAC_MUX_AREA_T*) Addr_NX90_dmac_mux_app;
#define HW_PTR_ENDATM(var)              static NX90_ENDAT_AREA_T*                const var[] = \
  {\
    (NX90_ENDAT_AREA_T*) Addr_NX90_endat0_app,\
    (NX90_ENDAT_AREA_T*) Addr_NX90_endat1_app\
  };
#define HW_PTR_ENDATCTRL(var)           static NX90_ENDAT_CTRL_AREA_T*           const var[] = \
  {\
    (NX90_ENDAT_CTRL_AREA_T*) Addr_NX90_endat_ctrl0_app,\
    (NX90_ENDAT_CTRL_AREA_T*) Addr_NX90_endat_ctrl1_app\
  };
#define HW_PTR_MLED_CTRL(var)           static NX90_MLED_CTRL_APP_AREA_T*        const var   = (MLED_CTRL_APP_AREA_T*) Addr_NX90_mled_ctrl_app;
#define HW_PTR_IFLASH_CFG(var)          static NX90_IFLASH_CFG_AREA_T*           const var[] = \
  {\
    (NX90_IFLASH_CFG_AREA_T*) Addr_NX90_iflash_cfg2\
  };
#define HW_PTR_TRIGGER_IRQ(var)         static NX90_TRIGGER_IRQ_AREA_T*          const var   = (TRIGGER_IRQ_AREA_T*) Addr_NX90_trigger_irq_app;
#define HW_PTR_ECC_CTRL(var)            static NX90_ECC_CTRL_APP_AREA_T*         const var   = (NX90_ECC_CTRL_APP_AREA_T*) Addr_NX90_ecc_ctrl_app;
#define HW_PTR_MCP(var)                 static NX90_HS_IRQ_REG_AREA_T*           const var   = (NX90_HS_IRQ_REG_AREA_T*)Addr_NX90_mcp_app;
#endif


#ifdef CPU_IS_ARM
#include "regdef_netx90_arm_com.h"

/* ARM_COM */
#define HW_PTR_ARM_TIMER(var)           static NX90_ARM_TIMER_AREA_T*            const var   = (NX90_ARM_TIMER_AREA_T*) Addr_NX90_timer_com;
#define HW_PTR_WATCHDOG(var)            static NX90_WATCHDOG_AREA_T*             const var   = (NX90_WATCHDOG_AREA_T*) Addr_NX90_wdg_com;
#define HW_PTR_SYSTIME(var)             static NX90_SYSTIME_AREA_T*              const var[] = \
  {\
    (NX90_SYSTIME_AREA_T*) Addr_NX90_systime_com,\
    (NX90_SYSTIME_AREA_T*) Addr_NX90_systime_uc_com\
  };
#define HW_PTR_SYSTIMELT(var)           static NX90_INTLOGIC_SYSTIME_LT_AREA_T*  const var[] = \
  {\
    (NX90_INTLOGIC_SYSTIME_LT_AREA_T*) Addr_NX90_systime_lt_com,\
    (NX90_INTLOGIC_SYSTIME_LT_AREA_T*) Addr_NX90_systime_lt_xpic_com\
  };
#define HW_PTR_SQI(var)                 static NX90_SQI_AREA_T*                  const var[] = \
{\
  (NX90_SQI_AREA_T*) Addr_NX90_sqi,\
};
#define HW_PTR_UART(var)                static NX90_UART_AREA_T*                 const var[] = \
  {\
    (NX90_UART_AREA_T*) Addr_NX90_uart\
  };
#define HW_PTR_I2C(var)                 static NX90_I2C_AREA_T*                  const var[] = \
  {\
    (NX90_I2C_AREA_T*) Addr_NX90_i2c0_com,\
    (NX90_I2C_AREA_T*) Addr_NX90_i2c1_com\
  };
#define HW_PTR_GPIO(var)                static NX90_GPIO_AREA_T*                 const var[] = \
  {\
    (NX90_GPIO_AREA_T*) Addr_NX90_gpio_com\
  };
#define HW_PTR_DPM(var)                 static NX90_DPM_AREA_T*                  const var[] = \
  {\
    (NX90_DPM_AREA_T*) Addr_NX90_dpm0_com,\
    (NX90_DPM_AREA_T*) Addr_NX90_dpm1_com\
  };
#define HW_PTR_IDPM(var)                static NX90_IDPM_AREA_T*                 const var[] = \
  {\
    (NX90_IDPM_AREA_T*) Addr_NX90_idpm_com\
  };
#define HW_PTR_HANDSHAKE_CTRL(var)      static NX90_HANDSHAKE_CTRL_AREA_T*       const var[] = \
  {\
    (NX90_HANDSHAKE_CTRL_AREA_T*) Addr_NX90_handshake_ctrl_com\
  };
#define HW_PTR_XPIC_DEBUG(var)          static NX90_XPIC_DEBUG_AREA_T*           const var[] = \
  {\
    (NX90_XPIC_DEBUG_AREA_T*) Addr_NX90_xpic_com_debug\
  };
#define HW_PTR_XPIC(var)                static NX90_XPIC_AREA_T*                 const var[] = \
  {\
    (NX90_XPIC_AREA_T*) Addr_NX90_xpic_com_regs\
  };
#define HW_PTR_XPIC_DRAM(var)           static uint32_t*                           const var[] = \
  {\
    (uint32_t*) Addr_NX90_xpic_com_dram\
  };
#define HW_PTR_XPIC_PRAM(var)           static uint32_t*                           const var[] = \
  {\
    (uint32_t*) Addr_NX90_xpic_com_pram\
  };
#define HW_PTR_XPIC_VIC(var)            static NX90_XPIC_VIC_AREA_T*             const var[] = \
  {\
    (NX90_XPIC_VIC_AREA_T*) Addr_NX90_vic_xpic_com\
  };
#define HW_PTR_XPIC_WDG(var)            static NX90_XPIC_WDG_AREA_T*             const var[] = \
  {\
    (NX90_XPIC_WDG_AREA_T*) Addr_NX90_wdg_xpic_com\
  };
#define HW_PTR_XPIC_TIMER(var)          static NX90_ARM_TIMER_AREA_T*            const var[] = \
  {\
    (NX90_ARM_TIMER_AREA_T*) Addr_NX90_timer_xpic_com\
  };
#define HW_PTR_XPIC_INTRAM(var)         static uint32_t*                           const var   = (uint32_t*) Addr_NX90_intram5;

#define HW_PTR_TRIGGER_SAMPLE(var)      static NX90_TRIGGER_SAMPLE_UNIT_AREA_T*  const var[] = \
  {\
    (NX90_TRIGGER_SAMPLE_UNIT_AREA_T*) Addr_NX90_xc0_trigger_sample_unit\
  };
#define HW_PTR_XC_EXTERNAL_CFG(var)     static NX90_XC_EXTERNAL_CONFIG_AREA_T*   const var   = (NX90_XC_EXTERNAL_CONFIG_AREA_T*)Addr_NX90_xc_external_config;
#define HW_PTR_FMMUSM(var)              static NX90_FMMUSM_AREA_T*               const var[] = \
  {\
    (NX90_FMMUSM_AREA_T*) Addr_NX90_xc0_fmmusm\
  };
#define HW_PTR_SR_MOTION(var)           static NX90_SR_AREA_T*                   const var[] = \
  {\
    (NX90_SR_AREA_T*) Addr_NX90_xc0_sr_xpic_com\
  };
#define HW_PTR_XPEC_IRQ_REGS(var)       static NX90_XPEC_IRQ_REGS_T*             const var   = (NX90_XPEC_IRQ_REGS_T*) Addr_NX90_xc_xpec_irq_registers;
#define HW_PTR_XC_XPEC_IRQ_REGS(var)    static NX90_XPEC_IRQ_REGISTERS_AREA_T*   const var   = (NX90_XPEC_IRQ_REGISTERS_AREA_T*) Addr_NX90_xc_xpec_irq_registers;
#define HW_PTR_XC_START_STOP(var)       static NX90_XC_START_STOP_AREA_T*        const var   = (NX90_XC_START_STOP_AREA_T*) Addr_NX90_xc_start_stop;
#define HW_PTR_XC_SYSTIME_CONFIG(var)   static NX90_XC_SYSTIME_CONFIG_AREA_T*    const var   = (NX90_XC_SYSTIME_CONFIG_AREA_T*) Addr_NX90_xc_systime_config;

/* only single connection to INTPHYs or external PHYs */
#define HW_PTR_PHY_CTRL(var)            static NX90_PHY_CTRL_AREA_T*             const var[] = \
  {\
    (NX90_PHY_CTRL_AREA_T*) Addr_NX90_xc0_phy_ctrl0,\
    (NX90_PHY_CTRL_AREA_T*) Addr_NX90_xc0_phy_ctrl1\
  };
#define HW_PTR_RPEC_REGS(var)           static NX90_XPEC_AREA_T*                 const var[] = \
  {\
    (NX90_XPEC_AREA_T*) Addr_NX90_xc0_rpec0_regs,\
    (NX90_XPEC_AREA_T*) Addr_NX90_xc0_rpec1_regs\
  };
#define HW_PTR_TPEC_REGS(var)           static NX90_XPEC_AREA_T*                 const var[] = \
  {\
    (NX90_XPEC_AREA_T*) Addr_NX90_xc0_tpec0_regs,\
    (NX90_XPEC_AREA_T*) Addr_NX90_xc0_tpec1_regs\
  };
#define HW_PTR_RPEC_PRAM_START(var)     static uint32_t*                           const var[] = \
  {\
    (uint32_t*) Adr_NX90_xc0_rpec0_pram_ram_start,\
    (uint32_t*) Adr_NX90_xc0_rpec1_pram_ram_start\
  };
#define HW_PTR_RPEC_PRAM_END(var)       static uint32_t*                           const var[] = \
  {\
    (uint32_t*) Adr_NX90_xc0_rpec0_pram_ram_end,\
    (uint32_t*) Adr_NX90_xc0_rpec1_pram_ram_end\
  };
#define HW_PTR_TPEC_PRAM_START(var)     static uint32_t*                           const var[] = \
  {\
    (uint32_t*) Adr_NX90_xc0_tpec0_pram_ram_start,\
    (uint32_t*) Adr_NX90_xc0_tpec1_pram_ram_start\
  };
#define HW_PTR_TPEC_PRAM_END(var)       static uint32_t*                           const var[] = \
  {\
    (uint32_t*) Adr_NX90_xc0_tpec0_pram_ram_end,\
    (uint32_t*) Adr_NX90_xc0_tpec1_pram_ram_end\
  };
#define HW_PTR_XPEC_DRAM(var)           static uint32_t*                           const var[] = \
  {\
    (uint32_t*) Adr_NX90_xc0_tpec0_dram_ram_start,\
    (uint32_t*) Adr_NX90_xc0_tpec1_dram_ram_start\
  };
#define HW_PTR_RPEC_DRAM(var)           static uint32_t*                           const var[] = \
  {\
    (uint32_t*) Adr_NX90_xc0_rpec0_dram_ram_start,\
    (uint32_t*) Adr_NX90_xc0_rpec1_dram_ram_start\
  };
#define HW_PTR_XMAC(var)                static NX90_XMAC_AREA_T*                 const var[] = \
  {\
    (NX90_XMAC_AREA_T*) Addr_NX90_xc0_xmac0_regs,\
    (NX90_XMAC_AREA_T*) Addr_NX90_xc0_xmac1_regs\
  };
#define HW_PTR_RPU_PRAM_START(var)      static uint32_t*                           const var[] = \
  {\
    (uint32_t*)  Adr_NX90_xc0_rpu0_ram_xmac_ram_start,\
    (uint32_t*)  Adr_NX90_xc0_rpu1_ram_xmac_ram_start\
  };
#define HW_PTR_TPU_PRAM_START(var)      static uint32_t*                           const var[] = \
  {\
    (uint32_t*)  Adr_NX90_xc0_tpu0_ram_xmac_ram_start,\
    (uint32_t*)  Adr_NX90_xc0_tpu1_ram_xmac_ram_start\
  };
#define HW_PTR_RPU_PRAM_END(var)        static uint32_t*                           const var[] = \
  {\
    (uint32_t*)  Adr_NX90_xc0_rpu0_ram_xmac_ram_end,\
    (uint32_t*)  Adr_NX90_xc0_rpu1_ram_xmac_ram_end\
  };
#define HW_PTR_TPU_PRAM_END(var)        static uint32_t*                           const var[] = \
  {\
    (uint32_t*)  Adr_NX90_xc0_tpu0_ram_xmac_ram_end,\
    (uint32_t*)  Adr_NX90_xc0_tpu1_ram_xmac_ram_end\
  };
#define HW_PTR_INTRAM(var)              static uint32_t*                           const var[] = \
  {\
    (uint32_t*) Addr_NX90_intram3,\
    (uint32_t*) Addr_NX90_intram4\
  };
#define HW_PTR_XC_STATCFG_SHARED(var)   static NX90_XC_STATCFG_SHARED_AREA_T*    const var   = (NX90_XC_STATCFG_SHARED_AREA_T*) Addr_NX90_xc0_statbits_shared_xpic_com;
#define HW_PTR_BUFMAN(var)              static NX90_BUF_MAN_AREA_T*              const var[] = \
  {\
    (NX90_BUF_MAN_AREA_T*) Addr_NX90_xc0_buf_man\
  };
#define HW_PTR_BUFMAN_MOTION(var)       static NX90_BUF_MAN_AREA_T*              const var[] = \
  {\
    (NX90_BUF_MAN_AREA_T*) Addr_NX90_xc0_buf_man_xpic_com\
  };
#define HW_PTR_POINTERFIFO(var)         static NX90_POINTER_FIFO_AREA_T*         const var[] = \
  {\
    (NX90_POINTER_FIFO_AREA_T*) Addr_NX90_xc0_pointer_fifo\
  };
#define HW_PTR_MIIMU(var)               static NX90_MIIMU_AREA_T*                const var   = (NX90_MIIMU_AREA_T*) Addr_NX90_miimu;
#define HW_PTR_LVDS2MII(var)            static NX90_LVDS2MII_AREA_T *            const var[] = \
    {\
      (NX90_LVDS2MII_AREA_T*) Addr_NX90_lvds2mii0_com,\
      (NX90_LVDS2MII_AREA_T*) Addr_NX90_lvds2mii1_com\
    };
#define HW_PTR_DMAC_CH(var)             static NX90_DMAC_CH_AREA_T*              const var[] = \
  {\
    (NX90_DMAC_CH_AREA_T*) Addr_NX90_dmac_ch0,\
    (NX90_DMAC_CH_AREA_T*) Addr_NX90_dmac_ch1,\
    (NX90_DMAC_CH_AREA_T*) Addr_NX90_dmac_ch2\
  };
#define HW_PTR_DMAC_REG(var)            static NX90_DMAC_REG_AREA_T*             const var   = (NX90_DMAC_REG_AREA_T*) Addr_NX90_dmac_reg;
#define HW_PTR_CRC(var)                 static NX90_CRC_AREA_T*                  const var   = (NX90_CRC_AREA_T*) Addr_NX90_crc_com;
#define HW_PTR_BLINK(var)               static NX90_BLINK_AREA_T*                const var   = (NX90_BLINK_AREA_T*) Addr_NX90_blink_com;
#define HW_PTR_MLED_CTRL(var)           static NX90_MLED_CTRL_AREA_T*            const var   = (NX90_MLED_CTRL_AREA_T*) Addr_NX90_mled_ctrl_com;
#define HW_PTR_IFLASH_CFG(var)          static NX90_IFLASH_CFG_AREA_T*           const var[] = \
  {\
    (NX90_IFLASH_CFG_AREA_T*) Addr_NX90_iflash_cfg0_com,\
    (NX90_IFLASH_CFG_AREA_T*) Addr_NX90_iflash_cfg1_com,\
    (NX90_IFLASH_CFG_AREA_T*) Addr_NX90_iflash_cfg2\
  };
#define HW_PTR_IFLASH_GLOBAL_TIMINGS(var) static NX90_IFLASH_GLOBAL_TIMINGS_AREA_T* const var   = (NX90_IFLASH_GLOBAL_TIMINGS_AREA_T*) Addr_NX90_flash_global_timings_com;
#define HW_PTR_INT_PHY_CFG(var)           static NX90_INT_PHY_CFG_AREA_T*           const var   = (NX90_INT_PHY_CFG_AREA_T*) Addr_NX90_int_phy_cfg_com;
#define HW_PTR_ECC_CTRL(var)              static NX90_ECC_CTRL_COM_AREA_T*          const var   = (NX90_ECC_CTRL_COM_AREA_T*) Addr_NX90_ecc_ctrl_com;
#define HW_PTR_ASIC_CTRL_COM(var)         static NX90_ASIC_CTRL_COM_AREA_T*         const var   = (NX90_ASIC_CTRL_COM_AREA_T*) Addr_NX90_asic_ctrl_com;
#define HW_PTR_MCP(var)                   static NX90_HS_IRQ_REG_AREA_T*            const var   = (NX90_HS_IRQ_REG_AREA_T*)Addr_NX90_mcp_com;

#endif /* CPU_IS_ARM */


/* COMMON */
#define HW_PTR_CM4_SCS(var)             static NX90_CM4_SCS_AREA_T*              const var   = (NX90_CM4_SCS_AREA_T*)Addr_NX90_cm4_scs;
#define HW_PTR_CPUPING(var)             static NX90_HS_IRQ_REG_AREA_T*           const var[] = \
  {\
    (NX90_HS_IRQ_REG_AREA_T*) Addr_NX90_mcp_com,\
    (NX90_HS_IRQ_REG_AREA_T*) Addr_NX90_mcp_app,\
    (NX90_HS_IRQ_REG_AREA_T*) Addr_NX90_mcp_xpic_com,\
    (NX90_HS_IRQ_REG_AREA_T*) Addr_NX90_mcp_xpic_app\
  };
#define HW_PTR_ASIC_CTRL(var)           static NX90_ASIC_CTRL_AREA_T*            const var   = (NX90_ASIC_CTRL_AREA_T*) Addr_NX90_asic_ctrl;
#define HW_PTR_ECC_CTRL_SHD(var)        static NX90_ECC_CTRL_AREA_T*             const var   = (NX90_ECC_CTRL_AREA_T*) Addr_NX90_ecc_ctrl;
#define HW_PTR_SAMPLE_AT_PORN_STAT(var) static NX90_SAMPLE_AT_PORN_STAT_AREA_T*  const var   = (NX90_SAMPLE_AT_PORN_STAT_AREA_T*) Addr_NX90_sample_at_porn_stat;
#define HW_PTR_ETH(var)                 static NX90_ETH_AREA_T*                  const var   = (NX90_ETH_AREA_T*) Addr_NX90_eth;
#define HW_PTR_MMIO_CTRL(var)           static NX90_MMIO_CTRL_AREA_T*            const var   = (NX90_MMIO_CTRL_AREA_T*) Addr_NX90_mmio_ctrl;
#define HW_PTR_NFIFO(var)               static NX90_NFIFO_AREA_T*                const var   = (NX90_NFIFO_AREA_T*) Addr_NX90_nfifo;
#define HW_PTR_GLOBAL_BUFMAN(var)       static NX90_GLOBAL_BUF_MAN_AREA_T*       const var   = (NX90_GLOBAL_BUF_MAN_AREA_T*) Addr_NX90_global_buf_man;
#define HW_PTR_HIF_IO_CTRL(var)         static NX90_HIF_IO_CTRL_AREA_T*          const var   = (NX90_HIF_IO_CTRL_AREA_T*) Addr_NX90_hif_io_ctrl;
#define HW_PTR_ASYNCMEM_CTRL(var)       static NX90_EXT_ASYNCMEM_CTRL_AREA_T*    const var[] =\
{\
  (NX90_EXT_ASYNCMEM_CTRL_AREA_T*) Addr_NX90_hif_asyncmem_ctrl\
};
#define HW_PTR_HASH(var)                static NX90_HASH_AREA_T*                 const var   = (NX90_HASH_AREA_T*) Addr_NX90_hash;
#define HW_PTR_AES(var)                 static NX90_AES_AREA_T*                  const var   = (NX90_AES_AREA_T*) Addr_NX90_aes;
#define HW_PTR_RANDOM(var)              static NX90_RANDOM_AREA_T*               const var   = (NX90_RANDOM_AREA_T*) Addr_NX90_random;
#define HW_PTR_MTGY(var)                static NX90_MTGY_AREA_T*                 const var   = (NX90_MTGY_AREA_T*) Addr_NX90_mtgy;
#define HW_PTR_PADCTRL(var)             static volatile uint32_t*                  const var   = (volatile uint32_t*) Addr_NX90_pad_ctrl;
#define HW_PTR_ADC_SEQ(var)             static NX90_ADC_SEQ_AREA_T*              const var   = (NX90_ADC_SEQ_AREA_T*) Addr_NX90_adc_seq;


/*** Access Macros ***/
#ifndef HW_WR32
#define HW_WR32(var,val) (var) = (val)
#endif

#ifndef HW_RD32
#define HW_RD32(var)      (var)
#endif

#ifndef HW_WR16
#define HW_WR16(var,val) (var) = (val)
#endif

#ifndef HW_RD16
#define HW_RD16(var)      (var)
#endif

#ifndef HW_WR8
#define HW_WR8(var,val)  (var) = (val)
#endif

#ifndef HW_RD8
#define HW_RD8(var)       (var)
#endif

/* Copy data from netX to Host */
#ifndef HW_RDMEM
#define HW_RDMEM(dst, src, len) memcpy(dst, src, len)
#endif

/* Copy data from Host to netX */
#ifndef HW_WRMEM
#define HW_WRMEM(dst, src, len) memcpy(dst, src, len)
#endif

#endif /* __HW_DEFINES_H */
