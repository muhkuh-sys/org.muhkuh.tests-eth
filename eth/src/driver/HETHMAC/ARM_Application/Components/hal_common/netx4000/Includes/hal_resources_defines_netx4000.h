#ifndef _HAL_RESOURCES_DEFINES_H
#define _HAL_RESOURCES_DEFINES_H

#include "compiler_spec.h"
#include "hal_armirq.h"
#include "hal_xc.h"
#include "hal_xpic.h"
#include "hal_miimu.h"
#include "hal_phyctrl.h"
#include "regdef_netx4000.h" /* used for addresses and structures */
#include <string.h>

#define _HW_CONCAT(a,b) a ## b

#ifdef COMMON_FUNC_OLD_STYLE
#define HALDEF(name) _HW_CONCAT(NX4000_, name)
#else
#define HALDEF(name) name
#endif

#define HW_MSK(bf)           _HW_CONCAT(MSK_NX4000_, bf)
#define HW_SRT(bf)           _HW_CONCAT(SRT_NX4000_, bf)
#define HW_MSK_USED_BITS(bf) _HW_CONCAT(MSK_USED_BITS_NX4000_, bf)
#define HW_DFLT_BF_VAL(bf)   _HW_CONCAT(DFLT_BF_VAL_NX4000_, bf)
#define HW_DFLT_VAL(reg)     _HW_CONCAT(DFLT_VAL_NX4000_, reg)
#define HW_REGADR(reg)       _HW_CONCAT(Adr_NX4000_, reg)
#define HW_AREAADR(area)     _HW_CONCAT(Addr_NX4000_, area)
#define HW_TYPE(area)        _HW_CONCAT(NX4000_, area)

#ifdef netlist_route
#define Adr_NX4000_spi_xpic3_spi_dr Adr_NX4000_spi_motion_spi_dr
#define Addr_NX4000_spi_xpic3 Addr_NX4000_spi_motion
#endif

#ifndef darm
  #ifdef CPU_IS_CA9
    #include "ca9_regdef_netx4000.h"
    #define PERIPHCLK_PER_10NS 3
  #else
    #include "cr7_regdef_netx4000.h"
    #define PERIPHCLK_PER_10NS 2
  #endif
  #define SLEEP_CC(cc) HALDEF(PTimer_Sleep)(0,cc)
#else
  #define SLEEP_CC(cc) HALDEF(ArmTimer_Sleep)(0,cc)
#endif

#define NX4000_DEV_FREQUENCY 100000000L // 100 Mhz

/* Set definitions */
#define NX_SYSTIME_UC   2

#define NX_XC_TYPE_RPU  HALDEF(XC_TYPE_RPU)
#define NX_XC_TYPE_TPU  HALDEF(XC_TYPE_TPU)
#define NX_XC_TYPE_RPEC HALDEF(XC_TYPE_RPEC)
#define NX_XC_TYPE_TPEC HALDEF(XC_TYPE_TPEC)

#define MSK_NX_XC_TYPE_RPU  MSK_XC_TYPE_RPU
#define MSK_NX_XC_TYPE_TPU  MSK_XC_TYPE_TPU
#define MSK_NX_XC_TYPE_RPEC MSK_XC_TYPE_RPEC
#define MSK_NX_XC_TYPE_TPEC MSK_XC_TYPE_TPEC

#define NX_INT_PHYCTRL_LED_MODE_DIS      HALDEF(PHYCTRL_LED_MODE_MANUAL)
#define NX_INT_PHYCTRL_LED_MODE_STATIC   HALDEF(PHYCTRL_LED_MODE_STATIC)
#define NX_INT_PHYCTRL_LED_MODE_FLASHING HALDEF(PHYCTRL_LED_MODE_FLASHING)
#define NX_INT_PHYCTRL_LED_MODE_COMBINED HALDEF(PHYCTRL_LED_MODE_COMBINED)

#define NX_MIIMU_SEND_PREAMBLE                 HALDEF(MIIMU_SEND_PREAMBLE)
#define NX_MIIMU_MDCFREQ_HIGH                  HALDEF(MIIMU_MDCFREQ_HIGH)
#define NX_MIIMU_RTA_0_BITS                    HALDEF(MIIMU_RTA_0_BITS)
#define NX_DRV_CB12_CONTROL_STATUS_INDICATION  HALDEF(DRV_CB12_CONTROL_STATUS_INDICATION)

typedef struct NX4000_XPEC_IRQ_REGS_Ttag
{
  volatile uint32_t aulIrq_xpec[4];
} NX4000_XPEC_IRQ_REGS_T;


/*** Register areas ***/

#ifndef darm

/* CORTEX A9/R7 common private peripherals */
#define __USE_PRIVATE_TIMER     static NX4000_PRIVATE_TIMER_AREA_T*       const s_ptPTimer             = (NX4000_PRIVATE_TIMER_AREA_T*) Addr_NX4000_private_timer;
#define __USE_IC_CPU_IF         static NX4000_IC_CPU_INTERFACE_AREA_T*    const s_ptGicIf              = (NX4000_IC_CPU_INTERFACE_AREA_T*) Addr_NX4000_ic_cpu_interface;
#define __USE_IC_DIST           static NX4000_IC_DISTRIBUTOR_AREA_T*      const s_ptGicDist            = (NX4000_IC_DISTRIBUTOR_AREA_T*) Addr_NX4000_ic_distributor;

#ifdef CPU_IS_CA9
#define __USE_L2C               static NX4000_L2C_AREA_T*                 const s_ptL2c                = (NX4000_L2C_AREA_T*) Addr_NX4000_l2c;
#endif

  /* Common RAP peripherals */
#define __USE_RAP_ECC_CTRL      static NX4000_RAP_ECC_CTRL_AREA_T*        const s_aptRapEccCtrl[]      = \
  {\
    (NX4000_RAP_ECC_CTRL_AREA_T*) Addr_NX4000_RAP_ECC_CTRL0,\
    (NX4000_RAP_ECC_CTRL_AREA_T*) Addr_NX4000_RAP_ECC_CTRL1,\
    (NX4000_RAP_ECC_CTRL_AREA_T*) Addr_NX4000_RAP_ECC_CTRL2 \
  };
#define __USE_RAP_PORT_CTRL     static volatile uint32_t*                 const s_paulRapPortCtrl      = (volatile uint32_t*)Addr_NX4000_PORTCONTROL;
#define __USE_RAP_GPIO          static NX4000_RAP_GPIO_AREA_T*            const s_patRapGpio[]         = \
  {\
    (NX4000_RAP_GPIO_AREA_T*) Addr_RAP_GPIO0,\
    (NX4000_RAP_GPIO_AREA_T*) Addr_RAP_GPIO1,\
    (NX4000_RAP_GPIO_AREA_T*) Addr_RAP_GPIO2,\
    (NX4000_RAP_GPIO_AREA_T*) Addr_RAP_GPIO3,\
    (NX4000_RAP_GPIO_AREA_T*) Addr_RAP_GPIO4 \
  };

#define __USE_RAP_SQI           static NX4000_SQI_AREA_T*                 const s_aptRapSqi[]          = \
  {\
    (NX4000_SQI_AREA_T*) Addr_NX4000_SQI0,\
    (NX4000_SQI_AREA_T*) Addr_NX4000_SQI1\
  };

#define __USE_RAP_I2C           static NX4000_I2C_AREA_T*                 const s_aptRapI2c[]          = \
  {\
    (NX4000_I2C_AREA_T*) Addr_NX4000_RAP_I2C0,\
    (NX4000_I2C_AREA_T*) Addr_NX4000_RAP_I2C1,\
    (NX4000_I2C_AREA_T*) Addr_NX4000_RAP_I2C2,\
    (NX4000_I2C_AREA_T*) Addr_NX4000_RAP_I2C3,\
    (NX4000_I2C_AREA_T*) Addr_NX4000_RAP_I2C4,\
    (NX4000_I2C_AREA_T*) Addr_NX4000_RAP_I2C5 \
  };
  
#define __USE_RAP_UART           static NX4000_RAP_UART_AREA_T*           const s_aptRapUart[]          = \
  {\
    (NX4000_RAP_UART_AREA_T*) Addr_NX4000_RAP_UART0,\
    (NX4000_RAP_UART_AREA_T*) Addr_NX4000_RAP_UART1,\
    (NX4000_RAP_UART_AREA_T*) Addr_NX4000_RAP_UART2,\
    (NX4000_RAP_UART_AREA_T*) Addr_NX4000_RAP_UART3\
  };
  
#define __USE_RAP_I2S           static NX4000_IIS_AREA_T*                 const s_aptRapI2s[]          = \
  {\
    (NX4000_IIS_AREA_T*) Addr_NX4000_IIS0,\
    (NX4000_IIS_AREA_T*) Addr_NX4000_IIS1,\
    (NX4000_IIS_AREA_T*) Addr_NX4000_IIS2\
  };

#define __USE_RAP_I2S_CLKSEL    static NX4000_IISCLK_CTL_AREA_T*          const s_aptRapI2sClkSel      = (NX4000_IISCLK_CTL_AREA_T*) Addr_NX4000_IISCLK_CTL;
  
#define __USE_RAP_SSP           static NX4000_SSP_AREA_T*                 const s_aptRapSsp[]           = \
  {\
    (NX4000_SSP_AREA_T*) Addr_NX4000_SSP0,\
    (NX4000_SSP_AREA_T*) Addr_NX4000_SSP1\
  };    
  
#define __USE_RAP_DMAC_REG      static NX4000_RAP_DMAC_REG_AREA_T*        const s_aptRapDmacReg[]           = \
  {\
    (NX4000_RAP_DMAC_REG_AREA_T*) Addr_NX4000_RAP_DMAC0_REG,\
    (NX4000_RAP_DMAC_REG_AREA_T*) Addr_NX4000_RAP_DMAC1_REG\
  };   
  
#define __USE_RAP_DMAC_CHN      static NX4000_RAP_DMAC_CH_AREA_T*        const s_aptRapDmacChn[]           = \
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

#define __USE_RAP_SDIO          static NX4000_SDIO_AREA_Ttag*             const s_ptRapSdio            = (volatile uint32_t* )Addr_NX4000_SDIO;

#define __USE_RAP_SYSCTRL       static NX4000_RAP_SYSCTRL_AREA_T*         const s_ptRapSysCtrl         = (NX4000_RAP_SYSCTRL_AREA_T*)Addr_NX4000_RAP_SYSCTRL;

#define __USE_RAP_PL353         static NX4000_PL353_AREA_T*               const s_ptPl353              = (NX4000_PL353_AREA_T* )Addr_NX4000_PL353;

#define __USE_RAP_SWITCH_3P     static NX4000_SWITCH_3P_AREA_T*           const s_ptRapSwitch3P        = (NX4000_SWITCH_3P_AREA_T* )Addr_NX4000_SWITCH_3P;

#define __USE_RAP_GMAC          static NX4000_GMAC_AREA_T*                const s_aptRapGmac[]         = \
  {\
    (NX4000_GMAC_AREA_T*) Addr_NX4000_GMAC0,\
    (NX4000_GMAC_AREA_T*) Addr_NX4000_GMAC1\
  };

#define __USE_RAP_PCIE          static NX4000_PCIE_AREA_T*                const s_ptPcie              = (NX4000_PCIE_AREA_T* )Addr_NX4000_PCIE;

#define __USE_RAP_USB_HOST      static NX4000_USB_HOST_AREA_T*            const s_ptUsbHost           = (NX4000_USB_HOST_AREA_T*)Addr_NX4000_USB_HOST;

#define __USE_RAP_PVO           static NX4000_PVO_AREA_T*                 const s_ptPvo               = (NX4000_PVO_AREA_T*)Addr_NX4000_PVO;

#define __USE_RAP_DAVEHD        static NX4000_DAVEHD_AREA_T*              const s_ptDaveHd            = (NX4000_DAVEHD_AREA_T*)Addr_NX4000_DAVEHD;

#define __USE_RAP_DDR_CTRL      static NX4000_DDR_CTRL_AREA_T*            const s_ptDdrCtrl           = (NX4000_DDR_CTRL_AREA_T*)Addr_NX4000_DDR_CTRL;

#define __USE_RAP_DDR_PHY       static NX4000_DDR_PHY_AREA_T*             const s_ptDdrPhy            = (NX4000_DDR_PHY_AREA_T*)Addr_NX4000_DDR_PHY;

#endif /* #ifndef darm */

#define __USE_NX2RAP_INTRAM     static uint32_t*                          const s_apulNX2RAPIntRamStart[]     = \
  {\
    (uint32_t*) Addr_NX4000_NX2RAP_intram_rap0,\
    (uint32_t*) Addr_NX4000_NX2RAP_intram_rap1,\
    (uint32_t*) Addr_NX4000_NX2RAP_intram_rap2,\
    (uint32_t*) Addr_NX4000_NX2RAP_intram_rap3 \
  };

//#define __USE_ADC               static NX4000_ADC_CTRL_AREA_T*            const s_ptAdc                = (NX4000_ADC_CTRL_AREA_T*) Addr_NX4000_adc_ctrl;
#define __USE_ADC_CTRL          static NX4000_ADC_CTRL_AREA_T*            const s_aptAdcCtrl[]         = \
  {\
    (NX4000_ADC_CTRL_AREA_T*) Addr_NX4000_adc_ctrl0,\
    (NX4000_ADC_CTRL_AREA_T*) Addr_NX4000_adc_ctrl1\
  };

#define __USE_ADC_CTRL_MOTION   static NX4000_ADC_CTRL_AREA_T*            const s_aptAdcCtrlMotion[]   = \
  {\
    (NX4000_ADC_CTRL_AREA_T*) Addr_NX4000_adc_ctrl0_motion,\
    (NX4000_ADC_CTRL_AREA_T*) Addr_NX4000_adc_ctrl1_motion\
  };

#define __USE_ASIC_CTRL         static NX4000_ASIC_CTRL_AREA_T*           const s_ptAsicCtrl           = (NX4000_ASIC_CTRL_AREA_T*) Addr_NX4000_asic_ctrl;

#define __USE_SAMPLE_AT_PORN_STAT static NX4000_SAMPLE_AT_PORN_STAT_AREA_T* const s_ptSampleAtPornStat = (NX4000_SAMPLE_AT_PORN_STAT_AREA_T*) Addr_NX4000_sample_at_porn_stat;

#define __USE_ECC_CTRL          static NX4000_ECC_CTRL_AREA_T*            const s_ptEccCtrl            = (NX4000_ECC_CTRL_AREA_T*) Addr_NX4000_ecc_ctrl;

#define __USE_ARM_TIMER         static NX4000_ARM_TIMER_AREA_T*           const s_ptArmTimer           = (NX4000_ARM_TIMER_AREA_T*) Addr_NX4000_arm_timer;

#define __USE_ETH               static NX4000_ETH_AREA_T*                 const s_ptEth                = (NX4000_ETH_AREA_T*) Addr_NX4000_eth;

#define __USE_DMAC_MUX          static NX4000_DMAC_MUX_AREA_T*            const s_ptDmacMux            = (NX4000_DMAC_MUX_AREA_T*) Addr_NX4000_dmac_mux;

#define __USE_SYSTIME           static NX4000_SYSTIME_AREA_T*             const s_aptSystime[]         = \
  {\
    (NX4000_SYSTIME_AREA_T*) Addr_NX4000_systime0,\
    (NX4000_SYSTIME_AREA_T*) Addr_NX4000_systime1,\
    (NX4000_SYSTIME_AREA_T*) Addr_NX4000_systime_uc,\
    (NX4000_SYSTIME_AREA_T*) Addr_NX4000_systime_rap\
  };
  
#define __USE_SYSTIME_UC        static NX4000_SYSTIME_AREA_T*             const s_ptSystimeUc          = (NX4000_SYSTIME_AREA_T*) Addr_NX4000_systime_uc;

#define __USE_SYSTIMELT         static NX4000_INTLOGIC_SYSTIME_LT_AREA_T* const s_aptSystimeLt[]       = \
  {\
    (NX4000_INTLOGIC_SYSTIME_LT_AREA_T*) Addr_NX4000_intlogic_systime_lt0,\
    (NX4000_INTLOGIC_SYSTIME_LT_AREA_T*) Addr_NX4000_intlogic_systime_lt1,\
    (NX4000_INTLOGIC_SYSTIME_LT_AREA_T*) Addr_NX4000_intlogic_systime_lt2,\
    (NX4000_INTLOGIC_SYSTIME_LT_AREA_T*) Addr_NX4000_xpic_systime_lt0,\
    (NX4000_INTLOGIC_SYSTIME_LT_AREA_T*) Addr_NX4000_xpic_systime_lt1,\
    (NX4000_INTLOGIC_SYSTIME_LT_AREA_T*) Addr_NX4000_xpic_systime_lt2,\
    (NX4000_INTLOGIC_SYSTIME_LT_AREA_T*) Addr_NX4000_xpic_systime_lt3,\
  };

#define __USE_MMIO_CTRL         static NX4000_MMIO_CTRL_AREA_T*           const s_ptMmioCtrl           = (NX4000_MMIO_CTRL_AREA_T*) Addr_NX4000_mmio_ctrl;

#define __USE_MIIMU             static NX4000_MIIMU_AREA_T*               const s_ptMiimu              = (NX4000_MIIMU_AREA_T*) Addr_NX4000_miimu;

#define __USE_CRC               static NX4000_CRC_AREA_T*                 const s_ptCrc                = (NX4000_CRC_AREA_T*) Addr_NX4000_crc;

#define __USE_WATCHDOG          static NX4000_WATCHDOG_AREA_T*            const s_ptWatchdog           = (NX4000_WATCHDOG_AREA_T*) Addr_NX4000_watchdog;

#define __USE_BUFMAN            static NX4000_BUF_MAN_AREA_T*             const s_aptBufMan[]          = \
  {\
    (NX4000_BUF_MAN_AREA_T*) Addr_NX4000_xc0_buf_man,\
    (NX4000_BUF_MAN_AREA_T*) Addr_NX4000_xc1_buf_man\
  };

#define __USE_BUFMAN_MOTION     static NX4000_BUF_MAN_AREA_T*             const s_aptBufManMotion[]          = \
  {\
    (NX4000_BUF_MAN_AREA_T*) Addr_NX4000_xc0_buf_man_motion,\
    (NX4000_BUF_MAN_AREA_T*) Addr_NX4000_xc1_buf_man_motion\
  };
  
#define __USE_POINTERFIFO       static NX4000_POINTER_FIFO_AREA_T*        const s_aptPFifo[]           = \
  {\
    (NX4000_POINTER_FIFO_AREA_T*) Addr_NX4000_xc0_pointer_fifo,\
    (NX4000_POINTER_FIFO_AREA_T*) Addr_NX4000_xc1_pointer_fifo\
  };

#define __USE_HANDSHAKE_CTRL    static NX4000_HANDSHAKE_CTRL_AREA_T*      const s_aptHsCtrl[]          = \
  {\
    (NX4000_HANDSHAKE_CTRL_AREA_T*) Addr_NX4000_handshake_ctrl0,\
    (NX4000_HANDSHAKE_CTRL_AREA_T*) Addr_NX4000_handshake_ctrl1\
  };


#define __USE_LCD_PL110         static NX4000_LCD_CTRL_AREA_T*						const s_ptLcd = (NX4000_LCD_CTRL_AREA_T*) Addr_NX4000_lcd_ctrl;

#define __USE_CPUPING           static NX4000_HS_IRQ_REG_AREA_T*          const s_aptCpuPing[]  = \
  {\
    (NX4000_HS_IRQ_REG_AREA_T*) Addr_NX4000_cr7_multi_cpu_ping_irq,\
    (NX4000_HS_IRQ_REG_AREA_T*) Addr_NX4000_ca9_multi_cpu_ping_irq0,\
    (NX4000_HS_IRQ_REG_AREA_T*) Addr_NX4000_ca9_multi_cpu_ping_irq1,\
    (NX4000_HS_IRQ_REG_AREA_T*) Addr_NX4000_xpic_multi_cpu_ping_irq0,\
    (NX4000_HS_IRQ_REG_AREA_T*) Addr_NX4000_xpic_multi_cpu_ping_irq1,\
    (NX4000_HS_IRQ_REG_AREA_T*) Addr_NX4000_xpic_multi_cpu_ping_irq2,\
    (NX4000_HS_IRQ_REG_AREA_T*) Addr_NX4000_xpic_multi_cpu_ping_irq3\
  };

#define __USE_DPM               static NX4000_DPM_AREA_T*                 const s_ptDpm                = (NX4000_DPM_AREA_T*) Addr_NX4000_dpm;

#define __USE_IDPM              static NX4000_IDPM_AREA_T*                const s_aptIDpm[]            = \
  {\
    (NX4000_IDPM_AREA_T*) Addr_NX4000_idpm0,\
    (NX4000_IDPM_AREA_T*) Addr_NX4000_idpm1\
  };

#define __USE_ENC               static NX4000_MENC_AREA_T*                const s_ptEnc                = (NX4000_MENC_AREA_T*) Addr_NX4000_menc;

#define __USE_NFIFO             static NX4000_NFIFO_AREA_T*               const s_ptNFifo              = (NX4000_NFIFO_AREA_T*) Addr_NX4000_nfifo;

#define __USE_GPIO              static NX4000_GPIO_AREA_T*                const s_ptGpio               = (NX4000_GPIO_AREA_T*) Addr_NX4000_gpio;

#define __USE_GPIO_MOTION       static NX4000_GPIO_AREA_T*                const s_aptGpioMotion[]      = \
  {\
    (NX4000_GPIO_AREA_T*) Addr_NX4000_gpio_motion0,\
    (NX4000_GPIO_AREA_T*) Addr_NX4000_gpio_motion1,\
    (NX4000_GPIO_AREA_T*) Addr_NX4000_gpio_motion2,\
    (NX4000_GPIO_AREA_T*) Addr_NX4000_gpio_motion3\
  };
  
#define __USE_PIO               static NX4000_PIO_AREA_T*                 const s_ptPio                = (NX4000_PIO_AREA_T*) Addr_NX4000_pio;

#define __USE_MOTION            static NX4000_MOTION_SYSTIME_CONFIG_AREA_T* const s_ptMotion           = (NX4000_MOTION_SYSTIME_CONFIG_AREA_T*)Addr_NX4000_motion_systime_config;

#define __USE_PWM               static NX4000_MPWM_AREA_T*                const s_ptPwm                = (NX4000_MPWM_AREA_T*) Addr_mpwm;

#define __USE_TRIGGER_SAMPLE    static NX4000_TRIGGER_SAMPLE_UNIT_AREA_T* const s_aptTriggerSample[]   = \
  {\
    (NX4000_TRIGGER_SAMPLE_UNIT_AREA_T*) Addr_NX4000_xc0_trigger_sample_unit,\
    (NX4000_TRIGGER_SAMPLE_UNIT_AREA_T*) Addr_NX4000_xc1_trigger_sample_unit\
  };
  
#define __USE_XC_EXTERNAL_CFG   static NX4000_XC_EXTERNAL_CONFIG_AREA_T*  const s_ptXcExtCfg           = (NX4000_XC_EXTERNAL_CONFIG_AREA_T*)Addr_NX4000_xc_external_config;

#define __USE_FMMUSM            static NX4000_FMMUSM_AREA_T*              const s_aptFmmuSm[]          = \
  {\
    (NX4000_FMMUSM_AREA_T*) Addr_NX4000_xc0_fmmusm,\
    (NX4000_FMMUSM_AREA_T*) Addr_NX4000_xc1_fmmusm\
  };

#define __USE_SR_MOTION         static NX4000_SR_AREA_T*                  const s_aptSrMotion[]        = \
  {\
    (NX4000_SR_AREA_T*) Addr_NX4000_xc0_sr_motion,\
    (NX4000_SR_AREA_T*) Addr_NX4000_xc1_sr_motion\
  };

#define __USE_XPEC_IRQ_REGS     static NX4000_XPEC_IRQ_REGS_T*            const s_ptXpecIrqRegs        = (NX4000_XPEC_IRQ_REGS_T*) Addr_NX4000_xc_xpec_irq_registers;

#define __USE_XC_XPEC_IRQ_REGS  static NX4000_XPEC_IRQ_REGISTERS_AREA_T*  const s_ptXcXpecIrqRegs      = (NX4000_XPEC_IRQ_REGISTERS_AREA_T*) Addr_NX4000_xc_xpec_irq_registers;

#define __USE_XC_START_STOP     static NX4000_XC_START_STOP_AREA_T*       const s_ptXcStartStop        = (NX4000_XC_START_STOP_AREA_T*) Addr_NX4000_xc_start_stop;

#define __USE_XC_SYSTIME_CONFIG static NX4000_XC_SYSTIME_CONFIG_AREA_T*   const s_ptXcSystimeConfig    = (NX4000_XC_SYSTIME_CONFIG_AREA_T*) Addr_NX4000_xc_systime_config;

#define __USE_XPIC_TIMER        static NX4000_XPIC_TIMER_AREA_T*          const s_aptXpicTimer[]       = \
  {\
    (NX4000_XPIC_TIMER_AREA_T*) Addr_NX4000_xpic_timer0,\
    (NX4000_XPIC_TIMER_AREA_T*) Addr_NX4000_xpic_timer1,\
    (NX4000_XPIC_TIMER_AREA_T*) Addr_NX4000_xpic_timer2,\
    (NX4000_XPIC_TIMER_AREA_T*) Addr_NX4000_xpic_timer3\
  };

#define __USE_XPIC_WDG        static NX4000_XPIC_WDG_AREA_T*          const s_aptXpicWdg[]       = \
  {\
    (NX4000_XPIC_WDG_AREA_T*) Addr_NX4000_xpic_wdg0,\
    (NX4000_XPIC_WDG_AREA_T*) Addr_NX4000_xpic_wdg1,\
    (NX4000_XPIC_WDG_AREA_T*) Addr_NX4000_xpic_wdg2,\
    (NX4000_XPIC_WDG_AREA_T*) Addr_NX4000_xpic_wdg3\
  };

#define __USE_XPIC_VIC        static NX4000_XPIC_VIC_AREA_T*          const s_aptXpicVic[]       = \
  {\
    (NX4000_XPIC_VIC_AREA_T*) Addr_NX4000_xpic_vic0,\
    (NX4000_XPIC_VIC_AREA_T*) Addr_NX4000_xpic_vic1,\
    (NX4000_XPIC_VIC_AREA_T*) Addr_NX4000_xpic_vic2,\
    (NX4000_XPIC_VIC_AREA_T*) Addr_NX4000_xpic_vic3\
  };

#define __USE_XPIC              static NX4000_XPIC_AREA_T*                const s_aptXpic[]            = \
  {\
    (NX4000_XPIC_AREA_T*) Addr_NX4000_xpic0_regs,\
    (NX4000_XPIC_AREA_T*) Addr_NX4000_xpic1_regs,\
    (NX4000_XPIC_AREA_T*) Addr_NX4000_xpic2_regs,\
    (NX4000_XPIC_AREA_T*) Addr_NX4000_xpic3_regs\
  };

#define __USE_XPIC_DEBUG        static NX4000_XPIC_DEBUG_AREA_T*          const s_aptXpicDebug[]       = \
  {\
    (NX4000_XPIC_DEBUG_AREA_T*) Addr_NX4000_xpic0_debug,\
    (NX4000_XPIC_DEBUG_AREA_T*) Addr_NX4000_xpic1_debug,\
    (NX4000_XPIC_DEBUG_AREA_T*) Addr_NX4000_xpic2_debug,\
    (NX4000_XPIC_DEBUG_AREA_T*) Addr_NX4000_xpic3_debug\
  };

#define __USE_XPIC_DRAM         static uint32_t*                          const s_apulXpicDram[]       = \
  {\
    (uint32_t*) Addr_NX4000_xpic0_dram,\
    (uint32_t*) Addr_NX4000_xpic1_dram,\
    (uint32_t*) Addr_NX4000_xpic2_dram,\
    (uint32_t*) Addr_NX4000_xpic3_dram\
  };

#define __USE_DMAC_CH           static NX4000_DMAC_CH_AREA_T*             const s_aptDmacCh[]          = \
  {\
    (NX4000_DMAC_CH_AREA_T*) Addr_NX4000_dmac_ch0,\
    (NX4000_DMAC_CH_AREA_T*) Addr_NX4000_dmac_ch1,\
    (NX4000_DMAC_CH_AREA_T*) Addr_NX4000_dmac_ch2,\
    (NX4000_DMAC_CH_AREA_T*) Addr_NX4000_dmac_ch3\
  };
  
#define __USE_PHY_CTRL      static NX4000_PHY_CTRL_AREA_T*        const s_aptPhyCtrl[]       = \
  {\
    (NX4000_PHY_CTRL_AREA_T*) Addr_NX4000_xc0_phy_ctrl0,\
    (NX4000_PHY_CTRL_AREA_T*) Addr_NX4000_xc0_phy_ctrl1,\
    (NX4000_PHY_CTRL_AREA_T*) Addr_NX4000_xc1_phy_ctrl0,\
    (NX4000_PHY_CTRL_AREA_T*) Addr_NX4000_xc1_phy_ctrl1\
  };  

#define __USE_RPEC_REGS         static NX4000_XPEC_AREA_T*                const s_aptRpecRegArea[]      = \
  {\
    (NX4000_XPEC_AREA_T*) Addr_NX4000_xc0_rpec0_regs,\
    (NX4000_XPEC_AREA_T*) Addr_NX4000_xc0_rpec1_regs,\
    (NX4000_XPEC_AREA_T*) Addr_NX4000_xc1_rpec0_regs,\
    (NX4000_XPEC_AREA_T*) Addr_NX4000_xc1_rpec1_regs\
  };

#define __USE_TPEC_REGS         static NX4000_XPEC_AREA_T*                const s_aptTpecRegArea[]      = \
  {\
    (NX4000_XPEC_AREA_T*) Addr_NX4000_xc0_tpec0_regs,\
    (NX4000_XPEC_AREA_T*) Addr_NX4000_xc0_tpec1_regs,\
    (NX4000_XPEC_AREA_T*) Addr_NX4000_xc1_tpec0_regs,\
    (NX4000_XPEC_AREA_T*) Addr_NX4000_xc1_tpec1_regs\
  };

#define __USE_RPEC_PRAM_START   static uint32_t*                          const s_apulRpecPramArea[]    = \
  {\
    (uint32_t*) Adr_NX4000_xc0_rpec0_pram_ram_start,\
    (uint32_t*) Adr_NX4000_xc0_rpec1_pram_ram_start,\
    (uint32_t*) Adr_NX4000_xc1_rpec0_pram_ram_start,\
    (uint32_t*) Adr_NX4000_xc1_rpec1_pram_ram_start\
  };

#define __USE_RPEC_PRAM_END     static uint32_t*                          const s_apulRpecPramAreaEnd[] = \
  {\
    (uint32_t*) Adr_NX4000_xc0_rpec0_pram_ram_end,\
    (uint32_t*) Adr_NX4000_xc0_rpec1_pram_ram_end,\
    (uint32_t*) Adr_NX4000_xc1_rpec0_pram_ram_end,\
    (uint32_t*) Adr_NX4000_xc1_rpec1_pram_ram_end\
  };

#define __USE_TPEC_PRAM_START   static uint32_t*                          const s_apulTpecPramArea[]    = \
  {\
    (uint32_t*) Adr_NX4000_xc0_tpec0_pram_ram_start,\
    (uint32_t*) Adr_NX4000_xc0_tpec1_pram_ram_start,\
    (uint32_t*) Adr_NX4000_xc1_tpec0_pram_ram_start,\
    (uint32_t*) Adr_NX4000_xc1_tpec1_pram_ram_start\
  };

#define __USE_TPEC_PRAM_END     static uint32_t*                          const s_apulTpecPramAreaEnd[] = \
  {\
    (uint32_t*) Adr_NX4000_xc0_tpec0_pram_ram_end,\
    (uint32_t*) Adr_NX4000_xc0_tpec1_pram_ram_end,\
    (uint32_t*) Adr_NX4000_xc1_tpec0_pram_ram_end,\
    (uint32_t*) Adr_NX4000_xc1_tpec1_pram_ram_end\
  };

#define __USE_XPEC_DRAM         static uint32_t*                          const s_apulXpecDramArea[]    = \
  {\
    (uint32_t*) Adr_NX4000_xc0_tpec0_dram_ram_start,\
    (uint32_t*) Adr_NX4000_xc0_tpec1_dram_ram_start,\
    (uint32_t*) Adr_NX4000_xc1_tpec0_dram_ram_start,\
    (uint32_t*) Adr_NX4000_xc1_tpec1_dram_ram_start\
  };
  
#define __USE_RPEC_DRAM         static uint32_t*                          const s_apulRpecDramArea[]    = \
  {\
    (uint32_t*) Adr_NX4000_xc0_rpec0_dram_ram_start,\
    (uint32_t*) Adr_NX4000_xc0_rpec1_dram_ram_start,\
    (uint32_t*) Adr_NX4000_xc1_rpec0_dram_ram_start,\
    (uint32_t*) Adr_NX4000_xc1_rpec1_dram_ram_start\
  };
  
#define __USE_XMAC              static NX4000_XMAC_AREA_T*                const s_aptXmacArea[]         = \
  {\
    (NX4000_XMAC_AREA_T*) Addr_NX4000_xc0_xmac0_regs,\
    (NX4000_XMAC_AREA_T*) Addr_NX4000_xc0_xmac1_regs,\
    (NX4000_XMAC_AREA_T*) Addr_NX4000_xc1_xmac0_regs,\
    (NX4000_XMAC_AREA_T*) Addr_NX4000_xc1_xmac1_regs\
  };
  
#define __USE_RPU_PRAM_START    static uint32_t*                          const s_aptRpuPramArea[]        = \
  {\
    (uint32_t*)  Adr_NX4000_xc0_rpu0_ram_xmac_ram_start,\
    (uint32_t*)  Adr_NX4000_xc0_rpu1_ram_xmac_ram_start,\
    (uint32_t*)  Adr_NX4000_xc1_rpu0_ram_xmac_ram_start,\
    (uint32_t*)  Adr_NX4000_xc1_rpu1_ram_xmac_ram_start\
  };

#define __USE_TPU_PRAM_START    static uint32_t*                          const s_aptTpuPramArea[]        = \
  {\
    (uint32_t*)  Adr_NX4000_xc0_tpu0_ram_xmac_ram_start,\
    (uint32_t*)  Adr_NX4000_xc0_tpu1_ram_xmac_ram_start,\
    (uint32_t*)  Adr_NX4000_xc1_tpu0_ram_xmac_ram_start,\
    (uint32_t*)  Adr_NX4000_xc1_tpu1_ram_xmac_ram_start\
  };

#define __USE_RPU_PRAM_END      static uint32_t*                          const s_aptRpuPramAreaEnd[]     = \
  {\
    (uint32_t*)  Adr_NX4000_xc0_rpu0_ram_xmac_ram_end,\
    (uint32_t*)  Adr_NX4000_xc0_rpu1_ram_xmac_ram_end,\
    (uint32_t*)  Adr_NX4000_xc1_rpu0_ram_xmac_ram_end,\
    (uint32_t*)  Adr_NX4000_xc1_rpu1_ram_xmac_ram_end\
  };

#define __USE_TPU_PRAM_END      static uint32_t*                          const s_aptTpuPramAreaEnd[]     = \
  {\
    (uint32_t*)  Adr_NX4000_xc0_tpu0_ram_xmac_ram_end,\
    (uint32_t*)  Adr_NX4000_xc0_tpu1_ram_xmac_ram_end,\
    (uint32_t*)  Adr_NX4000_xc1_tpu0_ram_xmac_ram_end,\
    (uint32_t*)  Adr_NX4000_xc1_tpu1_ram_xmac_ram_end\
  };

#ifdef netlist_route
#define __USE_UART              static NX4000_UART_AREA_T*                const s_aptUart[]            = \
  {\
    (NX4000_UART_AREA_T*) Addr_NX4000_uart0,\
    (NX4000_UART_AREA_T*) Addr_NX4000_uart1,\
    (NX4000_UART_AREA_T*) Addr_NX4000_uart2\
  };
#else
#define __USE_UART              static NX4000_UART_AREA_T*                const s_aptUart[]            = \
  {\
    (NX4000_UART_AREA_T*) Addr_NX4000_uart0,\
    (NX4000_UART_AREA_T*) Addr_NX4000_uart1,\
    (NX4000_UART_AREA_T*) Addr_NX4000_uart2,\
    (NX4000_UART_AREA_T*) Addr_NX4000_uart_xpic3\
  };
#endif
  
#define __USE_I2C               static NX4000_I2C_AREA_T*                 const s_aptI2c[]            = \
  {\
    (NX4000_I2C_AREA_T*) Addr_NX4000_i2c0,\
    (NX4000_I2C_AREA_T*) Addr_NX4000_i2c1,\
    (NX4000_I2C_AREA_T*) Addr_NX4000_i2c2\
  };  
  
#define __USE_INTRAM            static uint32_t*                          const s_apulIntRamStart[]     = \
  {\
    (uint32_t*) Addr_NX4000_intram0,\
    (uint32_t*) Addr_NX4000_intram1,\
    (uint32_t*) Addr_NX4000_intram2,\
    (uint32_t*) Addr_NX4000_intram3 \
  };

#define __USE_SPI            static NX4000_SPI_AREA_T*                    const s_aptSpi[]             = \
  {\
    (NX4000_SPI_AREA_T*) Addr_NX4000_spi,\
    (NX4000_SPI_AREA_T*) Addr_NX4000_spi_xpic3\
  };
     
#define __USE_XPIC_INTRAM       static uint32_t*                          const s_pulIntRamXpicStart   = (uint32_t*) Addr_NX4000_intram4;

#define __USE_CANCTRL           static NX4000_CANCTRL_AREA_T*             const s_ptCanCtrl            = (NX4000_CANCTRL_AREA_T*) Addr_NX4000_canctrl;
 
#define __USE_DMAC_REG          static NX4000_DMAC_REG_AREA_T*            const s_ptDmacReg            = (NX4000_DMAC_REG_AREA_T*) Addr_NX4000_dmac_reg;

#define __USE_USB_DEV_CTRL      static NX4000_USB_DEV_CTRL_AREA_T*        const s_ptUsbDevCtrl         = (NX4000_USB_DEV_CTRL_AREA_T*) Addr_NX4000_usb_dev_ctrl;

#define __USE_USB_DEV_ENUM_RAM  static uint32_t*                          const s_pulUsbDevEnumRam     = (uint32_t*) Addr_NX4000_usb_dev_enum_ram;

#define __USE_USB_DEV_FIFO_CTRL static NX4000_USB_DEV_FIFO_CTRL_AREA_T*   const s_ptUsbDevFifoCtrl     = (NX4000_USB_DEV_FIFO_CTRL_AREA_T*) Addr_NX4000_usb_dev_fifo_ctrl;

#define __USE_USB_DEV_FIFO      static NX4000_USB_DEV_FIFO_AREA_T*        const s_ptUsbDevFifo         = (NX4000_USB_DEV_FIFO_AREA_T*) Addr_NX4000_usb_dev_fifo;

#define __USE_XC_STATCFG_SHARED static NX4000_XC_STATCFG_SHARED_AREA_T*   const s_ptXcStatCfgShared    = (NX4000_XC_STATCFG_SHARED_AREA_T*) Addr_NX4000_xc_statcfg_shared;

#define __USE_LVDS2MII          static NX4000_LVDS2MII_AREA_T*            const s_aptLvds2Mii[]        = \
    {\
      (NX4000_LVDS2MII_AREA_T*) Addr_NX4000_lvds2mii0,\
      (NX4000_LVDS2MII_AREA_T*) Addr_NX4000_lvds2mii1\
    };

#define __USE_GLOBAL_BUFMAN     static NX4000_GLOBAL_BUF_MAN_AREA_T*      const s_ptGlobalBufMan       = (NX4000_GLOBAL_BUF_MAN_AREA_T*) Addr_NX4000_global_buf_man;

/*** MIIMU ***/
typedef int(*NX4000_PFN_MIIMU_READ_PHY_REG) ( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuRtaField, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, uint16_t* pusData, void* pvUser );
typedef int(*NX4000_PFN_MIIMU_WRITE_PHY_REG)( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, uint16_t usData, void* pvUser );

#define __USE_MIIMU_FUNC_READ_PHY  static NX4000_PFN_MIIMU_READ_PHY_REG  pfnMiimuReadPhyReg  = HALDEF(MIIMU_ReadPhyReg);
#define __USE_MIIMU_FUNC_WRITE_PHY static NX4000_PFN_MIIMU_WRITE_PHY_REG pfnMiimuWritePhyReg = HALDEF(MIIMU_WritePhyReg);

/*** XC loader ***/
typedef int(*NX4000_PFN_XC_RESET)( unsigned int uXcNo, unsigned int uXcPortNo, void* pvUser);
typedef int(*NX4000_PFN_XC_LOAD) ( unsigned int uXcNo, unsigned int uXcPortNo, HALDEF(XC_TYPE_E) eXcType, const uint32_t* pulXcPrg, void* pvUser );
typedef int(*NX4000_PFN_XC_START)( unsigned int uXcNo, unsigned int uXcPortNo, void* pvUser);
typedef int(*NX4000_PFN_XC_START_UNITS)( unsigned int uXcNo, unsigned int uPortNo, unsigned int uUnitVec, void* pvUser );

#ifdef DISABLE_XC_RESET
#define __USE_XC_LOADER_FUNC_RESET        static NX4000_PFN_XC_RESET       pfnXcReset      = HALDEF(XC_ResetDummy);
#else
#define __USE_XC_LOADER_FUNC_RESET        static NX4000_PFN_XC_RESET       pfnXcReset      = HALDEF(XC_Reset);
#endif
#define __USE_XC_LOADER_FUNC_LOAD         static NX4000_PFN_XC_LOAD        pfnXcLoad       = HALDEF(XC_Load);
#define __USE_XC_LOADER_FUNC_START        static NX4000_PFN_XC_START       pfnXcStart      = HALDEF(XC_Start);
#define __USE_XC_LOADER_FUNC_START_UNITS  static NX4000_PFN_XC_START_UNITS pfnXcStartUnits = HALDEF(XC_StartUnits);
#define __USE_XC_LOADER_FUNC_START_UNITS2 static NX4000_PFN_XC_START_UNITS pfnXcStartUnits = HALDEF(XC_StartUnits2);

/*** xPIC loader ***/
typedef void(*NX4000_PFN_XPIC_RESET)(unsigned int uInstNo, void* pvUser);
typedef void(*NX4000_PFN_XPIC_LOAD) (unsigned int uInstNo, const uint32_t* pulXpicPrg, void* pvUser);
typedef void(*NX4000_PFN_XPIC_START)(unsigned int uInstNo, void* pvUser);

#define __USE_XPIC_LOADER_FUNC_RESET static NX4000_PFN_XPIC_RESET pfnXpicReset = HALDEF(XPIC_Reset);
#define __USE_XPIC_LOADER_FUNC_LOAD  static NX4000_PFN_XPIC_LOAD  pfnXpicLoad  = HALDEF(XPIC_Load);
#define __USE_XPIC_LOADER_FUNC_START static NX4000_PFN_XPIC_START pfnXpicStart = HALDEF(XPIC_Start);

#define __USE_HIF_IO_CTRL       static NX4000_HIF_IO_CTRL_AREA_T*         const s_ptHifIoCtrl          = (NX4000_HIF_IO_CTRL_AREA_T*) Addr_NX4000_hif_io_ctrl;

#define __USE_ASYNCMEM_CTRL static NX4000_EXT_ASYNCMEM_CTRL_AREA_T*   const s_aptAsyncMemCtrl[] =\
{\
  (NX4000_EXT_ASYNCMEM_CTRL_AREA_T*) Addr_NX4000_ext_asyncmem_ctrl,\
  (NX4000_EXT_ASYNCMEM_CTRL_AREA_T*) Addr_NX4000_hif_asyncmem_ctrl\
};

#define __USE_CRYPT      static NX4000_CRYPT_AREA_T*         const s_ptCrypt          = (NX4000_CRYPT_AREA_T*) Addr_NX4000_crypt;

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
