#ifndef __HW_DEFINES_H
#define __HW_DEFINES_H

#include "regdef_netx4000_arm_com.h" /* used for addresses and structures */

#define __NETX4000 1

#define _HW_CONCAT(a,b) a ## b
#define HW_MSK(bf)           _HW_CONCAT(MSK_NX4000_, bf)
#define HW_SRT(bf)           _HW_CONCAT(SRT_NX4000_, bf)
#define HW_MSK_USED_BITS(bf) _HW_CONCAT(MSK_USED_BITS_NX4000_, bf)
#define HW_DFLT_BF_VAL(bf)   _HW_CONCAT(DFLT_BF_VAL_NX4000_, bf)
#define HW_DFLT_VAL(reg)     _HW_CONCAT(DFLT_VAL_NX4000_, reg)
#define HW_REGADR(reg)       _HW_CONCAT(Adr_NX4000_, reg)
#define HW_AREAADR(area)     _HW_CONCAT(Addr_NX4000_, area)
#define HW_TYPE(area)        _HW_CONCAT(NX4000_, area)

#define HW_BFGET(bf, value) (((value) & HW_MSK(bf)) >> HW_SRT(bf))
#define HW_BFSET(bf, value) ((uint32_t)(value) << HW_SRT(bf))

#ifdef CPU_IS_CA9
  #include "regdef_netx4000_ca9.h"
  #define PERIPHCLK_PER_10NS 3
#else
  #include "regdef_netx4000_cr7.h"
  #define PERIPHCLK_PER_10NS 2
#endif
#include "hal_ptimer.h"
#define SYS_SLEEP_CC(cc) PTimer_Sleep(0,cc)

#define HW_DEV_FREQUENCY 100000000L /* 100 MHz */


typedef struct NX4000_XPEC_IRQ_REGS_Ttag
{
  volatile uint32_t aulIrq_xpec[4];
} NX4000_XPEC_IRQ_REGS_T;

typedef struct NX4000_RANDOM_AREA_Ttag
{
  volatile uint32_t  ulRandom_init;
  volatile uint32_t  ulRandom_random;
} NX4000_RANDOM_AREA_T;


/*** Register areas ***/

/* CORTEX A9 private peripherals */
#ifdef CPU_IS_CA9
#define HW_PTR_L2C(var)                static NX4000_L2C_AREA_T*                  const var   = (NX4000_L2C_AREA_T*) Addr_NX4000_l2c;
#endif

/* CORTEX A9/R7 common private peripherals */
#define HW_PTR_GLOBAL_TIMER(var)       static NX4000_GLOBAL_TIMER_AREA_T*         const var   = (NX4000_GLOBAL_TIMER_AREA_T*) Addr_NX4000_global_timer;
#define HW_PTR_PRIVATE_TIMER(var)      static NX4000_PRIVATE_TIMER_AREA_T*        const var   = (NX4000_PRIVATE_TIMER_AREA_T*) Addr_NX4000_private_timer;
#define HW_PTR_IC_CPU_IF(var)          static NX4000_IC_CPU_INTERFACE_AREA_T*     const var   = (NX4000_IC_CPU_INTERFACE_AREA_T*) Addr_NX4000_ic_cpu_interface;
#define HW_PTR_IC_DIST(var)            static NX4000_IC_DISTRIBUTOR_AREA_T*       const var   = (NX4000_IC_DISTRIBUTOR_AREA_T*) Addr_NX4000_ic_distributor;

/* Common peripherals */
#define HW_PTR_RAP_ECC_CTRL(var)       static NX4000_RAP_ECC_CTRL_AREA_T*         const var[] = \
  {\
    (NX4000_RAP_ECC_CTRL_AREA_T*) Addr_NX4000_RAP_ECC_CTRL0,\
    (NX4000_RAP_ECC_CTRL_AREA_T*) Addr_NX4000_RAP_ECC_CTRL1,\
    (NX4000_RAP_ECC_CTRL_AREA_T*) Addr_NX4000_RAP_ECC_CTRL2 \
  };
#define HW_PTR_RAP_PORT_CTRL(var)      static volatile uint32_t*                  const var   = (volatile uint32_t*)Addr_NX4000_PORTCONTROL;
#define HW_PTR_RAP_GPIO(var)           static NX4000_RAP_GPIO_AREA_T*             const var[] = \
  {\
    (NX4000_RAP_GPIO_AREA_T*) Addr_NX4000_RAP_GPIO0,\
    (NX4000_RAP_GPIO_AREA_T*) Addr_NX4000_RAP_GPIO1,\
    (NX4000_RAP_GPIO_AREA_T*) Addr_NX4000_RAP_GPIO2,\
    (NX4000_RAP_GPIO_AREA_T*) Addr_NX4000_RAP_GPIO3,\
    (NX4000_RAP_GPIO_AREA_T*) Addr_NX4000_RAP_GPIO4 \
  };
#define HW_PTR_SQI(var)                static NX4000_SQI_AREA_T*                  const var[] = \
  {\
    (NX4000_SQI_AREA_T*) Addr_NX4000_SQI0,\
    (NX4000_SQI_AREA_T*) Addr_NX4000_SQI1\
  };
#define HW_PTR_RAP_I2C(var)            static NX4000_I2C_AREA_T*                  const var[] = \
  {\
    (NX4000_I2C_AREA_T*) Addr_NX4000_RAP_I2C0,\
    (NX4000_I2C_AREA_T*) Addr_NX4000_RAP_I2C1,\
    (NX4000_I2C_AREA_T*) Addr_NX4000_RAP_I2C2,\
    (NX4000_I2C_AREA_T*) Addr_NX4000_RAP_I2C3,\
    (NX4000_I2C_AREA_T*) Addr_NX4000_RAP_I2C4,\
    (NX4000_I2C_AREA_T*) Addr_NX4000_RAP_I2C5 \
  };
#define HW_PTR_RAP_UART(var)            static NX4000_RAP_UART_AREA_T*            const var[] = \
  {\
    (NX4000_RAP_UART_AREA_T*) Addr_NX4000_RAP_UART0,\
    (NX4000_RAP_UART_AREA_T*) Addr_NX4000_RAP_UART1,\
    (NX4000_RAP_UART_AREA_T*) Addr_NX4000_RAP_UART2,\
    (NX4000_RAP_UART_AREA_T*) Addr_NX4000_RAP_UART3\
  };
#define HW_PTR_RAP_I2S(var)            static NX4000_IIS_AREA_T*                  const var[] = \
  {\
    (NX4000_IIS_AREA_T*) Addr_NX4000_IIS0,\
    (NX4000_IIS_AREA_T*) Addr_NX4000_IIS1,\
    (NX4000_IIS_AREA_T*) Addr_NX4000_IIS2\
  };
#define HW_PTR_RAP_I2S_CLKSEL(var)     static NX4000_IISCLK_CTL_AREA_T*           const var   = (NX4000_IISCLK_CTL_AREA_T*) Addr_NX4000_IISCLK_CTL;
#define HW_PTR_RAP_SSP(var)            static NX4000_SSP_AREA_T*                  const var[] = \
  {\
    (NX4000_SSP_AREA_T*) Addr_NX4000_SSP0,\
    (NX4000_SSP_AREA_T*) Addr_NX4000_SSP1\
  };
#define HW_PTR_RAP_DMAC_REG(var)       static NX4000_RAP_DMAC_REG_AREA_T*         const var[] = \
  {\
    (NX4000_RAP_DMAC_REG_AREA_T*) Addr_NX4000_RAP_DMAC0_REG,\
    (NX4000_RAP_DMAC_REG_AREA_T*) Addr_NX4000_RAP_DMAC1_REG\
  };
#define HW_PTR_RAP_DMAC_CHN(var)       static NX4000_RAP_DMAC_CH_AREA_T*          const var[] = \
  {\
    (NX4000_RAP_DMAC_CH_AREA_T*) Addr_NX4000_RAP_DMAC0_CH0,\
    (NX4000_RAP_DMAC_CH_AREA_T*) Addr_NX4000_RAP_DMAC0_CH1,\
    (NX4000_RAP_DMAC_CH_AREA_T*) Addr_NX4000_RAP_DMAC0_CH2,\
    (NX4000_RAP_DMAC_CH_AREA_T*) Addr_NX4000_RAP_DMAC0_CH3,\
    (NX4000_RAP_DMAC_CH_AREA_T*) Addr_NX4000_RAP_DMAC0_CH4,\
    (NX4000_RAP_DMAC_CH_AREA_T*) Addr_NX4000_RAP_DMAC0_CH5,\
    (NX4000_RAP_DMAC_CH_AREA_T*) Addr_NX4000_RAP_DMAC0_CH6,\
    (NX4000_RAP_DMAC_CH_AREA_T*) Addr_NX4000_RAP_DMAC0_CH7,\
    (NX4000_RAP_DMAC_CH_AREA_T*) Addr_NX4000_RAP_DMAC1_CH0,\
    (NX4000_RAP_DMAC_CH_AREA_T*) Addr_NX4000_RAP_DMAC1_CH1,\
    (NX4000_RAP_DMAC_CH_AREA_T*) Addr_NX4000_RAP_DMAC1_CH2,\
    (NX4000_RAP_DMAC_CH_AREA_T*) Addr_NX4000_RAP_DMAC1_CH3,\
    (NX4000_RAP_DMAC_CH_AREA_T*) Addr_NX4000_RAP_DMAC1_CH4,\
    (NX4000_RAP_DMAC_CH_AREA_T*) Addr_NX4000_RAP_DMAC1_CH5,\
    (NX4000_RAP_DMAC_CH_AREA_T*) Addr_NX4000_RAP_DMAC1_CH6,\
    (NX4000_RAP_DMAC_CH_AREA_T*) Addr_NX4000_RAP_DMAC1_CH7\
  };
#define HW_PTR_RAP_SDIO(var)           static NX4000_SDIO_AREA_Ttag*              const var   = (volatile uint32_t* )Addr_NX4000_SDIO;
#define HW_PTR_RAP_SYSCTRL(var)        static NX4000_RAP_SYSCTRL_AREA_T*          const var   = (NX4000_RAP_SYSCTRL_AREA_T*)Addr_NX4000_RAP_SYSCTRL;
#define HW_PTR_RAP_PL353(var)          static NX4000_PL353_AREA_T*                const var   = (NX4000_PL353_AREA_T* )Addr_NX4000_PL353;
#define HW_PTR_RAP_SWITCH_3P(var)      static NX4000_SWITCH_3P_AREA_T*            const var   = (NX4000_SWITCH_3P_AREA_T* )Addr_NX4000_SWITCH_3P;
#define HW_PTR_RAP_GMAC(var)           static NX4000_GMAC_AREA_T*                 const var[] = \
  {\
    (NX4000_GMAC_AREA_T*) Addr_NX4000_GMAC0,\
    (NX4000_GMAC_AREA_T*) Addr_NX4000_GMAC1\
  };
#define HW_PTR_RAP_PCIE(var)           static NX4000_PCIE_AREA_T*                 const var   = (NX4000_PCIE_AREA_T* )Addr_NX4000_PCIE;
#define HW_PTR_RAP_USB_FUNC(var)       static NX4000_USB_FUNC_AREA_T*             const var   = (NX4000_USB_FUNC_AREA_T*)Addr_NX4000_USB_FUNC;
#define HW_PTR_RAP_USB_HOST(var)       static NX4000_USB_HOST_AREA_T*             const var   = (NX4000_USB_HOST_AREA_T*)Addr_NX4000_USB_HOST;
#define HW_PTR_RAP_PVO(var)            static NX4000_PVO_AREA_T*                  const var   = (NX4000_PVO_AREA_T*)Addr_NX4000_PVO;
#define HW_PTR_RAP_DAVEHD(var)         static NX4000_DAVEHD_AREA_T*               const var   = (NX4000_DAVEHD_AREA_T*)Addr_NX4000_DAVEHD;
#define HW_PTR_RAP_DDR_CTRL(var)       static NX4000_DDR_CTRL_AREA_T*             const var   = (NX4000_DDR_CTRL_AREA_T*)Addr_NX4000_DDR_CTRL;
#define HW_PTR_RAP_DDR_PHY(var)        static NX4000_DDR_PHY_AREA_T*              const var   = (NX4000_DDR_PHY_AREA_T*)Addr_NX4000_DDR_PHY;
#define HW_PTR_RAP_SIGMA_DELTA(var)    static NX4000_SIGMA_DELTA_AREA_T*          const var   = (NX4000_SIGMA_DELTA_AREA_T*)Addr_NX4000_SIGMA_DELTA;
#define HW_PTR_RAP_FIREWALL(var)       static NX4000_FIREWALL_AREA_T*             const var[] = \
  {\
    (NX4000_FIREWALL_AREA_T*) Addr_NX4000_FIREWALL0,\
    (NX4000_FIREWALL_AREA_T*) Addr_NX4000_FIREWALL1,\
    (NX4000_FIREWALL_AREA_T*) Addr_NX4000_FIREWALL2,\
    (NX4000_FIREWALL_AREA_T*) Addr_NX4000_FIREWALL3,\
    (NX4000_FIREWALL_AREA_T*) Addr_NX4000_FIREWALL4,\
    (NX4000_FIREWALL_AREA_T*) Addr_NX4000_FIREWALL5,\
    (NX4000_FIREWALL_AREA_T*) Addr_NX4000_FIREWALL6,\
    (NX4000_FIREWALL_AREA_T*) Addr_NX4000_FIREWALL7\
  };
#define HW_PTR_RAP_RTC(var)     static NX4000_RTC_AREA_T*          const var   = (NX4000_RTC_AREA_T*)Addr_NX4000_RTC;
#define HW_PTR_RAP_SECURE(var)  static NX4000_SECURE_AREA_T*       const var   = (NX4000_SECURE_AREA_T*)Addr_NX4000_SECURE;
#define HW_PTR_RAP_NOC(var)     static NX4000_NOC_AREA_T*          const var   = (NX4000_NOC_AREA_T*)Addr_NX4000_NOC;
#define HW_PTR_RAP_ICC(var)     static NX4000_ICC_AREA_T*          const var   = (NX4000_ICC_AREA_T*)Addr_NX4000_ICC;

#define HW_PTR_NX2RAP_INTRAM(var)      static uint32_t*                           const var[] = \
  {\
    (uint32_t*) Addr_NX4000_NX2RAP_intram_rap0,\
    (uint32_t*) Addr_NX4000_NX2RAP_intram_rap1,\
    (uint32_t*) Addr_NX4000_NX2RAP_intram_rap2,\
    (uint32_t*) Addr_NX4000_NX2RAP_intram_rap3 \
  };
#define HW_PTR_ADC_CTRL(var)            static NX4000_ADC_CTRL_AREA_T*            const var[] = \
  {\
    (NX4000_ADC_CTRL_AREA_T*) Addr_NX4000_adc_ctrl0,\
    (NX4000_ADC_CTRL_AREA_T*) Addr_NX4000_adc_ctrl1\
  };
#define HW_PTR_ADC_CTRL_MOTION(var)     static NX4000_ADC_CTRL_AREA_T*            const var[] = \
  {\
    (NX4000_ADC_CTRL_AREA_T*) Addr_NX4000_adc_ctrl0_motion,\
    (NX4000_ADC_CTRL_AREA_T*) Addr_NX4000_adc_ctrl1_motion\
  };
#define HW_PTR_ASIC_CTRL(var)           static NX4000_ASIC_CTRL_AREA_T*           const var   = (NX4000_ASIC_CTRL_AREA_T*) Addr_NX4000_asic_ctrl;
#define HW_PTR_SAMPLE_AT_PORN_STAT(var) static NX4000_SAMPLE_AT_PORN_STAT_AREA_T* const var   = (NX4000_SAMPLE_AT_PORN_STAT_AREA_T*) Addr_NX4000_sample_at_porn_stat;
#define HW_PTR_ECC_CTRL(var)            static NX4000_ECC_CTRL_AREA_T*            const var   = (NX4000_ECC_CTRL_AREA_T*) Addr_NX4000_ecc_ctrl;
#define HW_PTR_ARM_TIMER(var)           static NX4000_ARM_TIMER_AREA_T*           const var   = (NX4000_ARM_TIMER_AREA_T*) Addr_NX4000_arm_timer;
#define HW_PTR_ETH(var)                 static NX4000_ETH_AREA_T*                 const var   = (NX4000_ETH_AREA_T*) Addr_NX4000_eth;
#define HW_PTR_DMAC_MUX(var)            static NX4000_DMAC_MUX_AREA_T*            const var   = (NX4000_DMAC_MUX_AREA_T*) Addr_NX4000_dmac_mux;
#define HW_PTR_SYSTIME(var)             static NX4000_SYSTIME_AREA_T*             const var[] = \
  {\
    (NX4000_SYSTIME_AREA_T*) Addr_NX4000_systime0,\
    (NX4000_SYSTIME_AREA_T*) Addr_NX4000_systime1,\
    (NX4000_SYSTIME_AREA_T*) Addr_NX4000_systime_uc,\
  };
#define HW_PTR_SYSTIME_UC(var)         static NX4000_SYSTIME_AREA_T*              const var   = (NX4000_SYSTIME_AREA_T*) Addr_NX4000_systime_uc;
#define HW_PTR_SYSTIMELT(var)          static NX4000_INTLOGIC_SYSTIME_LT_AREA_T*  const var[] = \
  {\
    (NX4000_INTLOGIC_SYSTIME_LT_AREA_T*) Addr_NX4000_intlogic_systime_lt0,\
    (NX4000_INTLOGIC_SYSTIME_LT_AREA_T*) Addr_NX4000_intlogic_systime_lt1,\
    (NX4000_INTLOGIC_SYSTIME_LT_AREA_T*) Addr_NX4000_intlogic_systime_lt2,\
    (NX4000_INTLOGIC_SYSTIME_LT_AREA_T*) Addr_NX4000_xpic_systime_lt0,\
    (NX4000_INTLOGIC_SYSTIME_LT_AREA_T*) Addr_NX4000_xpic_systime_lt1,\
    (NX4000_INTLOGIC_SYSTIME_LT_AREA_T*) Addr_NX4000_xpic_systime_lt2,\
    (NX4000_INTLOGIC_SYSTIME_LT_AREA_T*) Addr_NX4000_xpic_systime_lt3,\
  };
#define HW_PTR_MMIO_CTRL(var)          static NX4000_MMIO_CTRL_AREA_T*            const var   = (NX4000_MMIO_CTRL_AREA_T*) Addr_NX4000_mmio_ctrl;
#define HW_PTR_MIIMU(var)              static NX4000_MIIMU_AREA_T*                const var   = (NX4000_MIIMU_AREA_T*) Addr_NX4000_miimu;
#define HW_PTR_CRC(var)                static NX4000_CRC_AREA_T*                  const var   = (NX4000_CRC_AREA_T*) Addr_NX4000_crc;
#define HW_PTR_WATCHDOG(var)           static NX4000_WATCHDOG_AREA_T*             const var   = (NX4000_WATCHDOG_AREA_T*) Addr_NX4000_watchdog;
#define HW_PTR_BUFMAN(var)             static NX4000_BUF_MAN_AREA_T*              const var[] = \
  {\
    (NX4000_BUF_MAN_AREA_T*) Addr_NX4000_xc0_buf_man,\
    (NX4000_BUF_MAN_AREA_T*) Addr_NX4000_xc1_buf_man\
  };
#define HW_PTR_BUFMAN_MOTION(var)      static NX4000_BUF_MAN_AREA_T*              const var[] = \
  {\
    (NX4000_BUF_MAN_AREA_T*) Addr_NX4000_xc0_buf_man_motion,\
    (NX4000_BUF_MAN_AREA_T*) Addr_NX4000_xc1_buf_man_motion\
  };
#define HW_PTR_POINTERFIFO(var)        static NX4000_POINTER_FIFO_AREA_T*         const var[] = \
  {\
    (NX4000_POINTER_FIFO_AREA_T*) Addr_NX4000_xc0_pointer_fifo,\
    (NX4000_POINTER_FIFO_AREA_T*) Addr_NX4000_xc1_pointer_fifo\
  };
#define HW_PTR_HANDSHAKE_CTRL(var)     static NX4000_HANDSHAKE_CTRL_AREA_T*       const var[] = \
  {\
    (NX4000_HANDSHAKE_CTRL_AREA_T*) Addr_NX4000_handshake_ctrl0,\
    (NX4000_HANDSHAKE_CTRL_AREA_T*) Addr_NX4000_handshake_ctrl1\
  };
#define HW_PTR_LCD_PL110(var)          static NX4000_LCD_CTRL_AREA_T*             const var   = (NX4000_LCD_CTRL_AREA_T*) Addr_NX4000_lcd_ctrl;
#ifdef CPU_IS_CA9
#define HW_PTR_MCP(var)                static NX4000_HS_IRQ_REG_AREA_T*           const var   = (NX4000_HS_IRQ_REG_AREA_T*)Addr_NX4000_ca9_multi_cpu_ping_irq0;
#else
#define HW_PTR_MCP(var)                static NX4000_HS_IRQ_REG_AREA_T*           const var   = (NX4000_HS_IRQ_REG_AREA_T*)Addr_NX4000_cr7_multi_cpu_ping_irq;
#endif
#define HW_PTR_CPUPING(var)            static NX4000_HS_IRQ_REG_AREA_T*           const var[] = \
  {\
    (NX4000_HS_IRQ_REG_AREA_T*) Addr_NX4000_cr7_multi_cpu_ping_irq,\
    (NX4000_HS_IRQ_REG_AREA_T*) Addr_NX4000_ca9_multi_cpu_ping_irq0,\
    (NX4000_HS_IRQ_REG_AREA_T*) Addr_NX4000_ca9_multi_cpu_ping_irq1,\
    (NX4000_HS_IRQ_REG_AREA_T*) Addr_NX4000_xpic_multi_cpu_ping_irq0,\
    (NX4000_HS_IRQ_REG_AREA_T*) Addr_NX4000_xpic_multi_cpu_ping_irq1,\
    (NX4000_HS_IRQ_REG_AREA_T*) Addr_NX4000_xpic_multi_cpu_ping_irq2,\
    (NX4000_HS_IRQ_REG_AREA_T*) Addr_NX4000_xpic_multi_cpu_ping_irq3\
  };
#define HW_PTR_DPM(var)                static NX4000_DPM_AREA_T*                  const var[] = \
  {\
    (NX4000_DPM_AREA_T*) Addr_NX4000_dpm\
  };
#define HW_PTR_IDPM(var)               static NX4000_IDPM_AREA_T*                 const var[] = \
  {\
    (NX4000_IDPM_AREA_T*) Addr_NX4000_idpm0,\
    (NX4000_IDPM_AREA_T*) Addr_NX4000_idpm1\
  };
#define HW_PTR_ENC(var)                static NX4000_MENC_AREA_T*                 const var   = (NX4000_MENC_AREA_T*) Addr_NX4000_menc;
#define HW_PTR_NFIFO(var)              static NX4000_NFIFO_AREA_T*                const var   = (NX4000_NFIFO_AREA_T*) Addr_NX4000_nfifo;
#define HW_PTR_GPIO(var)               static NX4000_GPIO_AREA_T*                 const var[] = \
  {\
    (NX4000_GPIO_AREA_T*) Addr_NX4000_gpio,\
    (NX4000_GPIO_AREA_T*) Addr_NX4000_gpio_motion0,\
    (NX4000_GPIO_AREA_T*) Addr_NX4000_gpio_motion1,\
    (NX4000_GPIO_AREA_T*) Addr_NX4000_gpio_motion2,\
    (NX4000_GPIO_AREA_T*) Addr_NX4000_gpio_motion3\
  };
#define HW_PTR_PIO(var)                static NX4000_PIO_AREA_T*                  const var   = (NX4000_PIO_AREA_T*) Addr_NX4000_pio;
#define HW_PTR_MOTION(var)             static NX4000_MOTION_SYSTIME_CONFIG_AREA_T* const var   = (NX4000_MOTION_SYSTIME_CONFIG_AREA_T*)Addr_NX4000_motion_systime_config;
#define HW_PTR_PWM(var)                static NX4000_MPWM_AREA_T*                 const var   = (NX4000_MPWM_AREA_T*) Addr_NX4000_mpwm;
#define HW_PTR_TRIGGER_SAMPLE(var)     static NX4000_TRIGGER_SAMPLE_UNIT_AREA_T*  const var[] = \
  {\
    (NX4000_TRIGGER_SAMPLE_UNIT_AREA_T*) Addr_NX4000_xc0_trigger_sample_unit,\
    (NX4000_TRIGGER_SAMPLE_UNIT_AREA_T*) Addr_NX4000_xc1_trigger_sample_unit\
  };
#define HW_PTR_XC_EXTERNAL_CFG(var)    static NX4000_XC_EXTERNAL_CONFIG_AREA_T*   const var   = (NX4000_XC_EXTERNAL_CONFIG_AREA_T*)Addr_NX4000_xc_external_config;
#define HW_PTR_FMMUSM(var)             static NX4000_FMMUSM_AREA_T*               const var[] = \
  {\
    (NX4000_FMMUSM_AREA_T*) Addr_NX4000_xc0_fmmusm,\
    (NX4000_FMMUSM_AREA_T*) Addr_NX4000_xc1_fmmusm\
  };
#define HW_PTR_SR_MOTION(var)          static NX4000_SR_AREA_T*                   const var[] = \
  {\
    (NX4000_SR_AREA_T*) Addr_NX4000_xc0_sr_motion,\
    (NX4000_SR_AREA_T*) Addr_NX4000_xc1_sr_motion\
  };
#define HW_PTR_XPEC_IRQ_REGS(var)      static NX4000_XPEC_IRQ_REGS_T*             const var   = (NX4000_XPEC_IRQ_REGS_T*) Addr_NX4000_xc_xpec_irq_registers;
#define HW_PTR_XC_XPEC_IRQ_REGS(var)   static NX4000_XPEC_IRQ_REGISTERS_AREA_T*   const var   = (NX4000_XPEC_IRQ_REGISTERS_AREA_T*) Addr_NX4000_xc_xpec_irq_registers;
#define HW_PTR_XC_START_STOP(var)      static NX4000_XC_START_STOP_AREA_T*        const var   = (NX4000_XC_START_STOP_AREA_T*) Addr_NX4000_xc_start_stop;
#define HW_PTR_XC_SYSTIME_CONFIG(var)  static NX4000_XC_SYSTIME_CONFIG_AREA_T*    const var   = (NX4000_XC_SYSTIME_CONFIG_AREA_T*) Addr_NX4000_xc_systime_config;
#define HW_PTR_XPIC_TIMER(var)         static NX4000_XPIC_TIMER_AREA_T*           const var[] = \
  {\
    (NX4000_XPIC_TIMER_AREA_T*) Addr_NX4000_xpic_timer0,\
    (NX4000_XPIC_TIMER_AREA_T*) Addr_NX4000_xpic_timer1,\
    (NX4000_XPIC_TIMER_AREA_T*) Addr_NX4000_xpic_timer2,\
    (NX4000_XPIC_TIMER_AREA_T*) Addr_NX4000_xpic_timer3\
  };
#define HW_PTR_XPIC_WDG(var)         static NX4000_XPIC_WDG_AREA_T*               const var[] = \
  {\
    (NX4000_XPIC_WDG_AREA_T*) Addr_NX4000_xpic_wdg0,\
    (NX4000_XPIC_WDG_AREA_T*) Addr_NX4000_xpic_wdg1,\
    (NX4000_XPIC_WDG_AREA_T*) Addr_NX4000_xpic_wdg2,\
    (NX4000_XPIC_WDG_AREA_T*) Addr_NX4000_xpic_wdg3\
  };
#define HW_PTR_XPIC_VIC(var)         static NX4000_XPIC_VIC_AREA_T*               const var[] = \
  {\
    (NX4000_XPIC_VIC_AREA_T*) Addr_NX4000_xpic_vic0,\
    (NX4000_XPIC_VIC_AREA_T*) Addr_NX4000_xpic_vic1,\
    (NX4000_XPIC_VIC_AREA_T*) Addr_NX4000_xpic_vic2,\
    (NX4000_XPIC_VIC_AREA_T*) Addr_NX4000_xpic_vic3\
  };
#define HW_PTR_XPIC(var)               static NX4000_XPIC_AREA_T*                 const var[] = \
  {\
    (NX4000_XPIC_AREA_T*) Addr_NX4000_xpic0_regs,\
    (NX4000_XPIC_AREA_T*) Addr_NX4000_xpic1_regs,\
    (NX4000_XPIC_AREA_T*) Addr_NX4000_xpic2_regs,\
    (NX4000_XPIC_AREA_T*) Addr_NX4000_xpic3_regs\
  };
#define HW_PTR_XPIC_DEBUG(var)         static NX4000_XPIC_DEBUG_AREA_T*           const var[] = \
  {\
    (NX4000_XPIC_DEBUG_AREA_T*) Addr_NX4000_xpic0_debug,\
    (NX4000_XPIC_DEBUG_AREA_T*) Addr_NX4000_xpic1_debug,\
    (NX4000_XPIC_DEBUG_AREA_T*) Addr_NX4000_xpic2_debug,\
    (NX4000_XPIC_DEBUG_AREA_T*) Addr_NX4000_xpic3_debug\
  };
#define HW_PTR_XPIC_DRAM(var)         static uint32_t*                            const var[] = \
  {\
    (uint32_t*) Addr_NX4000_xpic0_dram,\
    (uint32_t*) Addr_NX4000_xpic1_dram,\
    (uint32_t*) Addr_NX4000_xpic2_dram,\
    (uint32_t*) Addr_NX4000_xpic3_dram\
  };
#define HW_PTR_XPIC_PRAM(var)         static uint32_t*                            const var[] = \
  {\
    (uint32_t*) Addr_NX4000_xpic0_pram,\
    (uint32_t*) Addr_NX4000_xpic1_pram,\
    (uint32_t*) Addr_NX4000_xpic2_pram,\
    (uint32_t*) Addr_NX4000_xpic3_pram\
  };
#define HW_PTR_DMAC_CH(var)            static NX4000_DMAC_CH_AREA_T*              const var[] = \
  {\
    (NX4000_DMAC_CH_AREA_T*) Addr_NX4000_dmac_ch0,\
    (NX4000_DMAC_CH_AREA_T*) Addr_NX4000_dmac_ch1,\
    (NX4000_DMAC_CH_AREA_T*) Addr_NX4000_dmac_ch2,\
    (NX4000_DMAC_CH_AREA_T*) Addr_NX4000_dmac_ch3\
  };
#define HW_PTR_PHY_CTRL(var)           static NX4000_PHY_CTRL_AREA_T*             const var[] = \
  {\
    (NX4000_PHY_CTRL_AREA_T*) Addr_NX4000_xc0_phy_ctrl0,\
    (NX4000_PHY_CTRL_AREA_T*) Addr_NX4000_xc0_phy_ctrl1,\
    (NX4000_PHY_CTRL_AREA_T*) Addr_NX4000_xc1_phy_ctrl0,\
    (NX4000_PHY_CTRL_AREA_T*) Addr_NX4000_xc1_phy_ctrl1\
  };
#define HW_PTR_RPEC_REGS(var)          static NX4000_XPEC_AREA_T*                 const var[] = \
  {\
    (NX4000_XPEC_AREA_T*) Addr_NX4000_xc0_rpec0_regs,\
    (NX4000_XPEC_AREA_T*) Addr_NX4000_xc0_rpec1_regs,\
    (NX4000_XPEC_AREA_T*) Addr_NX4000_xc1_rpec0_regs,\
    (NX4000_XPEC_AREA_T*) Addr_NX4000_xc1_rpec1_regs\
  };
#define HW_PTR_TPEC_REGS(var)          static NX4000_XPEC_AREA_T*                 const var[] = \
  {\
    (NX4000_XPEC_AREA_T*) Addr_NX4000_xc0_tpec0_regs,\
    (NX4000_XPEC_AREA_T*) Addr_NX4000_xc0_tpec1_regs,\
    (NX4000_XPEC_AREA_T*) Addr_NX4000_xc1_tpec0_regs,\
    (NX4000_XPEC_AREA_T*) Addr_NX4000_xc1_tpec1_regs\
  };
#define HW_PTR_RPEC_PRAM_START(var)    static uint32_t*                           const var[] = \
  {\
    (uint32_t*) Adr_NX4000_xc0_rpec0_pram_ram_start,\
    (uint32_t*) Adr_NX4000_xc0_rpec1_pram_ram_start,\
    (uint32_t*) Adr_NX4000_xc1_rpec0_pram_ram_start,\
    (uint32_t*) Adr_NX4000_xc1_rpec1_pram_ram_start\
  };
#define HW_PTR_RPEC_PRAM_END(var)      static uint32_t*                           const var[] = \
  {\
    (uint32_t*) Adr_NX4000_xc0_rpec0_pram_ram_end,\
    (uint32_t*) Adr_NX4000_xc0_rpec1_pram_ram_end,\
    (uint32_t*) Adr_NX4000_xc1_rpec0_pram_ram_end,\
    (uint32_t*) Adr_NX4000_xc1_rpec1_pram_ram_end\
  };
#define HW_PTR_TPEC_PRAM_START(var)    static uint32_t*                           const var[] = \
  {\
    (uint32_t*) Adr_NX4000_xc0_tpec0_pram_ram_start,\
    (uint32_t*) Adr_NX4000_xc0_tpec1_pram_ram_start,\
    (uint32_t*) Adr_NX4000_xc1_tpec0_pram_ram_start,\
    (uint32_t*) Adr_NX4000_xc1_tpec1_pram_ram_start\
  };
#define HW_PTR_TPEC_PRAM_END(var)      static uint32_t*                           const var[] = \
  {\
    (uint32_t*) Adr_NX4000_xc0_tpec0_pram_ram_end,\
    (uint32_t*) Adr_NX4000_xc0_tpec1_pram_ram_end,\
    (uint32_t*) Adr_NX4000_xc1_tpec0_pram_ram_end,\
    (uint32_t*) Adr_NX4000_xc1_tpec1_pram_ram_end\
  };
#define HW_PTR_XPEC_DRAM(var)          static uint32_t*                           const var[] = \
  {\
    (uint32_t*) Adr_NX4000_xc0_tpec0_dram_ram_start,\
    (uint32_t*) Adr_NX4000_xc0_tpec1_dram_ram_start,\
    (uint32_t*) Adr_NX4000_xc1_tpec0_dram_ram_start,\
    (uint32_t*) Adr_NX4000_xc1_tpec1_dram_ram_start\
  };
#define HW_PTR_RPEC_DRAM(var)          static uint32_t*                           const var[] = \
  {\
    (uint32_t*) Adr_NX4000_xc0_rpec0_dram_ram_start,\
    (uint32_t*) Adr_NX4000_xc0_rpec1_dram_ram_start,\
    (uint32_t*) Adr_NX4000_xc1_rpec0_dram_ram_start,\
    (uint32_t*) Adr_NX4000_xc1_rpec1_dram_ram_start\
  };
#define HW_PTR_XMAC(var)               static NX4000_XMAC_AREA_T*                 const var[] = \
  {\
    (NX4000_XMAC_AREA_T*) Addr_NX4000_xc0_xmac0_regs,\
    (NX4000_XMAC_AREA_T*) Addr_NX4000_xc0_xmac1_regs,\
    (NX4000_XMAC_AREA_T*) Addr_NX4000_xc1_xmac0_regs,\
    (NX4000_XMAC_AREA_T*) Addr_NX4000_xc1_xmac1_regs\
  };
#define HW_PTR_RPU_PRAM_START(var)     static uint32_t*                           const var[] = \
  {\
    (uint32_t*)  Adr_NX4000_xc0_rpu0_ram_xmac_ram_start,\
    (uint32_t*)  Adr_NX4000_xc0_rpu1_ram_xmac_ram_start,\
    (uint32_t*)  Adr_NX4000_xc1_rpu0_ram_xmac_ram_start,\
    (uint32_t*)  Adr_NX4000_xc1_rpu1_ram_xmac_ram_start\
  };
#define HW_PTR_TPU_PRAM_START(var)     static uint32_t*                           const var[] = \
  {\
    (uint32_t*)  Adr_NX4000_xc0_tpu0_ram_xmac_ram_start,\
    (uint32_t*)  Adr_NX4000_xc0_tpu1_ram_xmac_ram_start,\
    (uint32_t*)  Adr_NX4000_xc1_tpu0_ram_xmac_ram_start,\
    (uint32_t*)  Adr_NX4000_xc1_tpu1_ram_xmac_ram_start\
  };
#define HW_PTR_RPU_PRAM_END(var)       static uint32_t*                           const var[] = \
  {\
    (uint32_t*)  Adr_NX4000_xc0_rpu0_ram_xmac_ram_end,\
    (uint32_t*)  Adr_NX4000_xc0_rpu1_ram_xmac_ram_end,\
    (uint32_t*)  Adr_NX4000_xc1_rpu0_ram_xmac_ram_end,\
    (uint32_t*)  Adr_NX4000_xc1_rpu1_ram_xmac_ram_end\
  };
#define HW_PTR_TPU_PRAM_END(var)       static uint32_t*                           const var[] = \
  {\
    (uint32_t*)  Adr_NX4000_xc0_tpu0_ram_xmac_ram_end,\
    (uint32_t*)  Adr_NX4000_xc0_tpu1_ram_xmac_ram_end,\
    (uint32_t*)  Adr_NX4000_xc1_tpu0_ram_xmac_ram_end,\
    (uint32_t*)  Adr_NX4000_xc1_tpu1_ram_xmac_ram_end\
  };

#define HW_PTR_UART(var)               static NX4000_UART_AREA_T*                 const var[] = \
  {\
    (NX4000_UART_AREA_T*) Addr_NX4000_uart0,\
    (NX4000_UART_AREA_T*) Addr_NX4000_uart1,\
    (NX4000_UART_AREA_T*) Addr_NX4000_uart2,\
    (NX4000_UART_AREA_T*) Addr_NX4000_uart_xpic3\
  };

#define HW_PTR_I2C(var)                static NX4000_I2C_AREA_T*                  const var[] = \
  {\
    (NX4000_I2C_AREA_T*) Addr_NX4000_i2c0,\
    (NX4000_I2C_AREA_T*) Addr_NX4000_i2c1,\
    (NX4000_I2C_AREA_T*) Addr_NX4000_i2c2\
  };
#define HW_PTR_INTRAM(var)             static uint32_t*                           const var[] = \
  {\
    (uint32_t*) Addr_NX4000_intram0,\
    (uint32_t*) Addr_NX4000_intram1,\
    (uint32_t*) Addr_NX4000_intram2,\
    (uint32_t*) Addr_NX4000_intram3 \
  };
#define HW_PTR_SPI(var)                static NX4000_SPI_AREA_T*                  const var[] = \
  {\
    (NX4000_SPI_AREA_T*) Addr_NX4000_spi,\
    (NX4000_SPI_AREA_T*) Addr_NX4000_spi_xpic3\
  };
#define HW_PTR_XPIC_INTRAM(var)        static uint32_t*                           const var   = (uint32_t*) Addr_NX4000_intram4;
#define HW_PTR_CANCTRL(var)            static NX4000_CANCTRL_AREA_T*              const var[] = \
  {\
    (NX4000_CANCTRL_AREA_T*) Addr_NX4000_canctrl \
  };
#define HW_PTR_DMAC_REG(var)           static NX4000_DMAC_REG_AREA_T*             const var   = (NX4000_DMAC_REG_AREA_T*) Addr_NX4000_dmac_reg;
#define HW_PTR_USB_DEV_CTRL(var)       static NX4000_USB_DEV_CTRL_AREA_T*         const var   = (NX4000_USB_DEV_CTRL_AREA_T*) Addr_NX4000_usb_dev_ctrl;
#define HW_PTR_USB_DEV_ENUM_RAM(var)   static uint32_t*                           const var   = (uint32_t*) Addr_NX4000_usb_dev_enum_ram;
#define HW_PTR_USB_DEV_FIFO_CTRL(var)  static NX4000_USB_DEV_FIFO_CTRL_AREA_T*    const var   = (NX4000_USB_DEV_FIFO_CTRL_AREA_T*) Addr_NX4000_usb_dev_fifo_ctrl;
#define HW_PTR_USB_DEV_FIFO(var)       static NX4000_USB_DEV_FIFO_AREA_T*         const var   = (NX4000_USB_DEV_FIFO_AREA_T*) Addr_NX4000_usb_dev_fifo;
#define HW_PTR_XC_STATCFG_SHARED(var)  static NX4000_XC_STATCFG_SHARED_AREA_T*    const var   = (NX4000_XC_STATCFG_SHARED_AREA_T*) Addr_NX4000_xc_statcfg_shared;
#define HW_PTR_LVDS2MII(var)           static NX4000_LVDS2MII_AREA_T *            const var[] = \
    {\
      (NX4000_LVDS2MII_AREA_T*) Addr_NX4000_lvds2mii0,\
      (NX4000_LVDS2MII_AREA_T*) Addr_NX4000_lvds2mii1\
    };
#define HW_PTR_GLOBAL_BUFMAN(var)      static NX4000_GLOBAL_BUF_MAN_AREA_T*       const var   = (NX4000_GLOBAL_BUF_MAN_AREA_T*) Addr_NX4000_global_buf_man;
#define HW_PTR_HIF_IO_CTRL(var)        static NX4000_HIF_IO_CTRL_AREA_T*          const var   = (NX4000_HIF_IO_CTRL_AREA_T*) Addr_NX4000_hif_io_ctrl;
#define HW_PTR_ASYNCMEM_CTRL(var)      static NX4000_EXT_ASYNCMEM_CTRL_AREA_T*    const var[] =\
{\
  (NX4000_EXT_ASYNCMEM_CTRL_AREA_T*) Addr_NX4000_ext_asyncmem_ctrl,\
  (NX4000_EXT_ASYNCMEM_CTRL_AREA_T*) Addr_NX4000_hif_asyncmem_ctrl\
};
#define HW_PTR_HASH(var)                   static NX4000_CRYPT_AREA_T*                const var   = (NX4000_CRYPT_AREA_T*) Addr_NX4000_crypt;
#define HW_PTR_RANDOM(var)                 static NX4000_RANDOM_AREA_T*               const var   = (NX4000_RANDOM_AREA_T*) Adr_NX4000_crypt_random_init;
#define HW_PTR_SIGMA_DELTA_TRIGGER(var)    static NX4000_SIGMA_DELTA_TRIGGER_AREA_T*  const var   = (NX4000_SIGMA_DELTA_TRIGGER_AREA_T*) Addr_NX4000_sigma_delta_trigger;

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
