#include "defines.h"
#include "common_func_netx51.h"
#include "hal_ethmac.h"
#include "sys.h"
#include "phy.h"
#include "regdef_netx51.h"
#include "uprintf.h"

#define NO_OF_ETH_PORTS 2

#ifdef HAL_ETHMAC_TARGET_NX6
#include "common_func_netx6.h"
#define MIIMU_READ(phy,reg,data)  NX6_MIIMU_ReadPhyReg(NX51_MIIMU_SEND_PREAMBLE, NX51_MIIMU_MDCFREQ_HIGH, phy, reg, data, NULL)
#define MIIMU_WRITE(phy,reg,data) NX6_MIIMU_WritePhyReg(NX51_MIIMU_SEND_PREAMBLE, NX51_MIIMU_MDCFREQ_HIGH, phy, reg, data, NULL)
#else
#define MIIMU_READ(phy,reg,data)  NX51_MIIMU_ReadPhyReg(NX51_MIIMU_SEND_PREAMBLE, NX51_MIIMU_MDCFREQ_HIGH, NX51_MIIMU_RTA_1_BITS, phy, reg, data, NULL)
#define MIIMU_WRITE(phy,reg,data) NX51_MIIMU_WritePhyReg(NX51_MIIMU_SEND_PREAMBLE, NX51_MIIMU_MDCFREQ_HIGH, NX51_MIIMU_RTA_1_BITS, phy, reg, data, NULL)
#endif

/* Select target hardware by defining TARGET_HW */
/* Possible values for TARGET_HW */
#define _NXHX51_ETM_     0
#define _NXHX52_RE_      1
/* Default hardware is NXHX51_ETM */
#ifndef TARGET_HW
#define TARGET_HW _NXHX51_ETM_
#endif

/* print one character to STDOUT */
int sys_ser_putc(int iChar, void* pvStream)
{
  NX51_USBCDC_BufSendPut((unsigned char)iChar);
  return 0;
}

void sys_poll(void)
{
  /* left empty */
}

int sys_ser_flush(void* pvStream)
{
  NX51_USBCDC_BufSendFlush();
  return 0;
}

/* read PHY register */
static unsigned int sys_phy_read(unsigned int uiPhy, unsigned int uiReg)
{
  uint16_t usMiimuData = 0;
  MIIMU_READ(uiPhy, uiReg, &usMiimuData);
  return (unsigned int)usMiimuData;
}

/* write PHY register */
static void sys_phy_write(unsigned int uiPhy, unsigned int uiReg, unsigned int uiValue)
{
  MIIMU_WRITE(uiPhy, uiReg, (uint16_t)uiValue);
}

/* MII Management Interface */
static PHY_OPS_T s_tPhyOps =
{
  sys_phy_write,
  sys_phy_read
};

/* Set Ready/Run LED to a defined color */
void sys_led_rdyrun_set(LED_COLOUR_E eColour)
{
  static const NX51_RDYRUN_LED_E s_aeRdyRunCol[] = { NX51_RDYRUN_LED_OFF, NX51_RDYRUN_LED_RED, NX51_RDYRUN_LED_GREEN };
  NX51_AsicCtrl_SetRdyRunLed(s_aeRdyRunCol[eColour]);
}

void sys_time_get(uint32_t* pulTimeS, uint32_t* pulTimeNs)
{
  NX51_SYSTIME_GetSystime(pulTimeS, pulTimeNs, NULL);
}

/* wait */
void sys_sleep(unsigned int uiTimeMs)
{
  NX51_GPIO_Sleep(1, uiTimeMs * 1000);
}


int main(void)
{
  uint32_t ulPhyControlReg;

  /* lock IRQs during the initialization */
  NX51_IRQFIQ_LOCK

  /* clear all pending IRQs, this is important for 'soft restarts' from within the HITOP debugger */
  NX51_VIC_ClearVic();

  /* initialize SYSTIME unit */
  NX51_SYSTIME_SetBorder(SYSTIME_BORDER - 1, 0);
  NX51_SYSTIME_SetSpeed(SYSTIME_SPEED, 0);

  /* Configure MMIO pin multiplexer */
#if TARGET_HW == _NXHX51_ETM_
  NX51_MMIO_SetCfg(  0, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg(  1, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg(  2, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg(  3, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg(  4, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg(  6, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg(  7, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg(  5, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg(  8, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg(  9, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 10, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 11, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 12, NX51_MMIO_CONFIG_PHY0_LED0,           1, 0 );  /* ETH 0 LINK */
  NX51_MMIO_SetCfg( 13, NX51_MMIO_CONFIG_PHY0_LED1,           1, 0 );  /* ETH 0 ACT  */
  NX51_MMIO_SetCfg( 14, NX51_MMIO_CONFIG_PHY1_LED0,           1, 0 );  /* ETH 1 LINK */
  NX51_MMIO_SetCfg( 15, NX51_MMIO_CONFIG_PHY1_LED1,           1, 0 );  /* ETH 1 ACT  */
  NX51_MMIO_SetCfg( 16, NX51_MMIO_CONFIG_GPIO15,              0, 0 );  /* MMC_INS */
  NX51_MMIO_SetCfg( 17, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 18, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 19, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 20, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 21, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 22, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 23, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 24, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 25, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 26, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 27, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 28, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 29, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 30, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 31, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 32, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 33, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 34, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 35, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 36, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 37, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 38, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 39, NX51_MMIO_CONFIG_INPUT,               0, 0 );
#elif TARGET_HW == _NXHX52_RE_
  /* NXHX52-RE plus NXHX-PHY */
  NX51_MMIO_SetCfg(  0, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg(  1, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg(  2, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg(  3, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg(  4, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg(  5, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg(  6, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg(  7, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg(  8, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg(  9, NX51_MMIO_CONFIG_GPIO9,               0, 0 );  /* GPIO OUT LED */
  NX51_MMIO_SetCfg( 10, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 11, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 12, NX51_MMIO_CONFIG_PIO0,                0, 0 );  /* RTE0 STA0 */
  NX51_MMIO_SetCfg( 13, NX51_MMIO_CONFIG_PIO1,                0, 0 );  /* RTE0 STA1 */
  NX51_MMIO_SetCfg( 14, NX51_MMIO_CONFIG_PIO2,                0, 0 );  /* RTE1 STA0 */
  NX51_MMIO_SetCfg( 15, NX51_MMIO_CONFIG_PIO3,                0, 0 );  /* RTE1 STA1 */
  NX51_MMIO_SetCfg( 16, NX51_MMIO_CONFIG_PHY0_LED0,           1, 0 );  /* ETH0 LINK */
  NX51_MMIO_SetCfg( 17, NX51_MMIO_CONFIG_PHY0_LED1,           1, 0 );  /* ETH0 ACT  */
  NX51_MMIO_SetCfg( 18, NX51_MMIO_CONFIG_PHY1_LED0,           1, 0 );  /* ETH1 LINK */
  NX51_MMIO_SetCfg( 19, NX51_MMIO_CONFIG_PHY1_LED1,           1, 0 );  /* ETH1 ACT  */
  NX51_MMIO_SetCfg( 20, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 21, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 22, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 23, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 24, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* Not Bonded */
  NX51_MMIO_SetCfg( 25, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* Not Bonded */
  NX51_MMIO_SetCfg( 26, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* Not Bonded */
  NX51_MMIO_SetCfg( 27, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* Not Bonded */
  NX51_MMIO_SetCfg( 28, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* Not Bonded */
  NX51_MMIO_SetCfg( 29, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* Not Bonded */
  NX51_MMIO_SetCfg( 30, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* Not Bonded */
  NX51_MMIO_SetCfg( 31, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* Not Bonded */
  NX51_MMIO_SetCfg( 32, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* Not Bonded */
  NX51_MMIO_SetCfg( 33, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* Not Bonded */
  NX51_MMIO_SetCfg( 34, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* Not Bonded */
  NX51_MMIO_SetCfg( 35, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* Not Bonded */
  NX51_MMIO_SetCfg( 36, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* Not Bonded */
  NX51_MMIO_SetCfg( 37, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* Not Bonded */
  NX51_MMIO_SetCfg( 38, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* Not Bonded */
  NX51_MMIO_SetCfg( 39, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* Not Bonded */
#else
#error "Unknown target hardware"
#endif

#if HAL_ETHMAC_TARGET_NX6 && defined(NX6_SERIAL_DPM)
  NX51_MMIO_SetCfg( 40, NX51_MMIO_CONFIG_SPI1_MISO,           0, 0 );  /* SPI 1 MISO */
  NX51_MMIO_SetCfg( 41, NX51_MMIO_CONFIG_SPI1_MOSI,           0, 0 );  /* SPI 1 MOSI */
  NX51_MMIO_SetCfg( 42, NX51_MMIO_CONFIG_SPI1_CS0N,           0, 0 );  /* SPI 1 CS0  */
  NX51_MMIO_SetCfg( 43, NX51_MMIO_CONFIG_SPI1_CLK,            0, 0 );  /* SPI 1 CLK  */
  NX51_MMIO_SetCfg( 44, NX51_MMIO_CONFIG_GPIO31,              0, 0 );  /* GPIO used to get IRQ at rising edge */
#else
  NX51_MMIO_SetCfg( 40, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* prevent parallel DPM pin from being driven */
  NX51_MMIO_SetCfg( 41, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* prevent parallel DPM pin from being driven */
  NX51_MMIO_SetCfg( 42, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* prevent parallel DPM pin from being driven */
  NX51_MMIO_SetCfg( 43, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* prevent parallel DPM pin from being driven */
  NX51_MMIO_SetCfg( 44, NX51_MMIO_CONFIG_INPUT,               0, 0 );  /* prevent parallel DPM pin from being driven */
#endif
  NX51_MMIO_SetCfg( 45, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 46, NX51_MMIO_CONFIG_INPUT,               0, 0 );
  NX51_MMIO_SetCfg( 47, NX51_MMIO_CONFIG_INPUT,               0, 0 );

  /* Configure the GPIO 8..11 as inputs */
  NX51_GPIO_SetupMode (  8, NX51_GPIO_MODE_INPUT_READ, NX51_GPIO_INVERT_NOINVERT );
  NX51_GPIO_SetupMode (  9, NX51_GPIO_MODE_INPUT_READ, NX51_GPIO_INVERT_NOINVERT );
  NX51_GPIO_SetupMode ( 10, NX51_GPIO_MODE_INPUT_READ, NX51_GPIO_INVERT_NOINVERT );
  NX51_GPIO_SetupMode ( 11, NX51_GPIO_MODE_INPUT_READ, NX51_GPIO_INVERT_NOINVERT );

  /* Configure the GPIO 24..27 as outputs in line mode */
  NX51_GPIO_SetLine( 0 ); /* Set outputs off (default) */
  NX51_GPIO_SetupMode ( 24, NX51_GPIO_MODE_OUTPUT_SET_TO_GPIO_LINE, NX51_GPIO_INVERT_INVERT );
  NX51_GPIO_SetupMode ( 25, NX51_GPIO_MODE_OUTPUT_SET_TO_GPIO_LINE, NX51_GPIO_INVERT_INVERT );
  NX51_GPIO_SetupMode ( 26, NX51_GPIO_MODE_OUTPUT_SET_TO_GPIO_LINE, NX51_GPIO_INVERT_INVERT );
  NX51_GPIO_SetupMode ( 27, NX51_GPIO_MODE_OUTPUT_SET_TO_GPIO_LINE, NX51_GPIO_INVERT_INVERT );

#ifdef HAL_ETHMAC_TARGET_NX6

  if( 0 != NX51_ACCESSNX6_SetupParallelDpm( 32, /* Data width */
                                             1, /* EnRdy */
                                            NX6_DPM_RDYPOL_READY_WHEN_HI, /* RDY polarity, only valid of EnRdy==1 */
                                            1 /* INTRAMs as switch buffer required */
                                         )
    ) {
    NX51_AsicCtrl_SetRdyRunLed( NX51_RDYRUN_LED_RED );
    while(1) {}
  }

  /* enable all netX6 clocks */
  NX6_AsicCtrl_SetClockEnable( 0xf3333 );
  if( 0xf3333 != NX6_AsicCtrl_GetClockEnable() ) {
    NX51_AsicCtrl_SetRdyRunLed( NX51_RDYRUN_LED_RED );
    while(1) {}
  }

  /* Configure MMIO pin multiplexer */
  NX6_MMIO_SetCfg(  0, NX6_MMIO_CONFIG_GPIO0,          0, 0 ); /* Rotary Switch "Function" */
  NX6_MMIO_SetCfg(  1, NX6_MMIO_CONFIG_GPIO1,          0, 0 ); /* Rotary Switch "Function" */
  NX6_MMIO_SetCfg(  2, NX6_MMIO_CONFIG_GPIO2,          0, 0 ); /* Rotary Switch "Function" */
  NX6_MMIO_SetCfg(  3, NX6_MMIO_CONFIG_GPIO3,          0, 0 ); /* Rotary Switch "Function" */
  NX6_MMIO_SetCfg(  4, NX6_MMIO_CONFIG_GPIO4,          0, 0 ); /* Rotary Switch "Address X1" */
  NX6_MMIO_SetCfg(  6, NX6_MMIO_CONFIG_GPIO5,          0, 0 ); /* Rotary Switch "Address X1" */
  NX6_MMIO_SetCfg(  5, NX6_MMIO_CONFIG_GPIO6,          0, 0 ); /* Rotary Switch "Address X1" */
  NX6_MMIO_SetCfg(  7, NX6_MMIO_CONFIG_GPIO7,          0, 0 ); /* Rotary Switch "Address X1" */
  NX6_MMIO_SetCfg(  8, NX6_MMIO_CONFIG_GPIO8,          0, 0 ); /* Rotary Switch "Address X10" */
  NX6_MMIO_SetCfg(  9, NX6_MMIO_CONFIG_GPIO9,          0, 0 ); /* Rotary Switch "Address X10" */
  NX6_MMIO_SetCfg( 10, NX6_MMIO_CONFIG_GPIO10,         0, 0 ); /* Rotary Switch "Address X10" */
  NX6_MMIO_SetCfg( 11, NX6_MMIO_CONFIG_GPIO11,         0, 0 ); /* Rotary Switch "Address X10" */
  NX6_MMIO_SetCfg( 12, NX6_MMIO_CONFIG_GPIO12,         0, 0 ); /* COM 0 LED Green */
  NX6_MMIO_SetCfg( 13, NX6_MMIO_CONFIG_GPIO13,         0, 0 ); /* COM 0 LED Red */
  NX6_MMIO_SetCfg( 14, NX6_MMIO_CONFIG_GPIO14,         0, 0 ); /* COM 1 LED Green */
  NX6_MMIO_SetCfg( 15, NX6_MMIO_CONFIG_GPIO15,         0, 0 ); /* COM 1 LED Red */
  NX6_MMIO_SetCfg( 16, NX6_MMIO_CONFIG_PHY0_LED0,      1, 0 ); /* ETH0 LINK */
  NX6_MMIO_SetCfg( 17, NX6_MMIO_CONFIG_PHY0_LED1,      1, 0 ); /* ETH0 ACT  */
  NX6_MMIO_SetCfg( 18, NX6_MMIO_CONFIG_PHY1_LED0,      1, 0 ); /* ETH1 LINK */
  NX6_MMIO_SetCfg( 19, NX6_MMIO_CONFIG_PHY1_LED1,      1, 0 ); /* ETH1 ACT  */
  NX6_MMIO_SetCfg( 20, NX6_MMIO_CONFIG_GPIO20,         0, 0 ); /* Output LED 0 */
  NX6_MMIO_SetCfg( 21, NX6_MMIO_CONFIG_GPIO21,         0, 0 ); /* Output LED 1 */
  NX6_MMIO_SetCfg( 22, NX6_MMIO_CONFIG_XC_TRIGGER0,    0, 0 ); /* SYNC 0 */
  NX6_MMIO_SetCfg( 23, NX6_MMIO_CONFIG_XC_TRIGGER1,    0, 0 ); /* SYNC 1 */

  /* Input DIP switches configuration */
  NX6_GPIO_SetupMode(  0, NX6_GPIO_MODE_INPUT_READ, NX6_GPIO_INVERT_NOINVERT );
  NX6_GPIO_SetupMode(  1, NX6_GPIO_MODE_INPUT_READ, NX6_GPIO_INVERT_NOINVERT );
  NX6_GPIO_SetupMode(  2, NX6_GPIO_MODE_INPUT_READ, NX6_GPIO_INVERT_NOINVERT );
  NX6_GPIO_SetupMode(  3, NX6_GPIO_MODE_INPUT_READ, NX6_GPIO_INVERT_NOINVERT );
  NX6_GPIO_SetupMode(  4, NX6_GPIO_MODE_INPUT_READ, NX6_GPIO_INVERT_NOINVERT );
  NX6_GPIO_SetupMode(  5, NX6_GPIO_MODE_INPUT_READ, NX6_GPIO_INVERT_NOINVERT );
  NX6_GPIO_SetupMode(  6, NX6_GPIO_MODE_INPUT_READ, NX6_GPIO_INVERT_NOINVERT );
  NX6_GPIO_SetupMode(  7, NX6_GPIO_MODE_INPUT_READ, NX6_GPIO_INVERT_NOINVERT );
  NX6_GPIO_SetupMode(  8, NX6_GPIO_MODE_INPUT_READ, NX6_GPIO_INVERT_NOINVERT );
  NX6_GPIO_SetupMode(  9, NX6_GPIO_MODE_INPUT_READ, NX6_GPIO_INVERT_NOINVERT );
  NX6_GPIO_SetupMode( 10, NX6_GPIO_MODE_INPUT_READ, NX6_GPIO_INVERT_NOINVERT );
  NX6_GPIO_SetupMode( 11, NX6_GPIO_MODE_INPUT_READ, NX6_GPIO_INVERT_NOINVERT );

  /* COM LED configuration, clear low-active outputs */
  NX6_GPIO_SetupMode( 12, NX6_GPIO_MODE_OUTPUT_SET_TO_0, NX6_GPIO_INVERT_INVERT );
  NX6_GPIO_SetupMode( 13, NX6_GPIO_MODE_OUTPUT_SET_TO_0, NX6_GPIO_INVERT_INVERT );
  NX6_GPIO_SetupMode( 14, NX6_GPIO_MODE_OUTPUT_SET_TO_0, NX6_GPIO_INVERT_INVERT );
  NX6_GPIO_SetupMode( 15, NX6_GPIO_MODE_OUTPUT_SET_TO_0, NX6_GPIO_INVERT_INVERT );

  /* Output LED configuration (set to gpio_line), clear low-active outputs */
  NX6_GPIO_SetLine( 0x00000000 );
  NX6_GPIO_SetupMode( 20, NX6_GPIO_MODE_OUTPUT_SET_TO_GPIO_LINE, NX6_GPIO_INVERT_INVERT );
  NX6_GPIO_SetupMode( 21, NX6_GPIO_MODE_OUTPUT_SET_TO_GPIO_LINE, NX6_GPIO_INVERT_INVERT );

  /* test input switches and output LEDs */
  /*while (1) { unsigned long ulVal; ulVal = NX6_GPIO_GetIn(); NX6_GPIO_SetLine( (ulVal&0x3)<<20 ); } */

  /* initialize Dual-PHY */
  ulPhyControlReg = 0;
  ulPhyControlReg |= NX51_PHY_CTRL_PHY_ADDR                      << SRT_NX51_phy_control_phy_address;
  ulPhyControlReg |= NX51_PHYCTRL_MODE_ALL_CAPABLE_AUTONEG_AUTOMDIXEN << SRT_NX51_phy_control_phy0_mode;
  ulPhyControlReg |= NX51_PHY_CTRL_PHY_AUTOMDIX                  << SRT_NX51_phy_control_phy0_automdix;
  ulPhyControlReg |= NX51_PHY_CTRL_PHY_ENABLE                    << SRT_NX51_phy_control_phy0_enable;
  ulPhyControlReg |= NX51_PHYCTRL_MODE_ALL_CAPABLE_AUTONEG_AUTOMDIXEN << SRT_NX51_phy_control_phy1_mode;
  ulPhyControlReg |= NX51_PHY_CTRL_PHY_AUTOMDIX                  << SRT_NX51_phy_control_phy1_automdix;
  ulPhyControlReg |= NX51_PHY_CTRL_PHY_ENABLE                    << SRT_NX51_phy_control_phy1_enable;
  NX6_INTPHY_Init( ulPhyControlReg );

  /* reset pointer FIFO borders */
  NX6_PFIFO_Reset();

#elif HAL_ETHMAC_TARGET_NX51
  /* initialize Dual-PHY */
  ulPhyControlReg = 0;
  ulPhyControlReg |= NX51_PHY_CTRL_PHY_ADDR                      << SRT_NX51_phy_control_phy_address;
  ulPhyControlReg |= NX51_PHYCTRL_MODE_ALL_CAPABLE_AUTONEG_AUTOMDIXEN << SRT_NX51_phy_control_phy0_mode;
  ulPhyControlReg |= NX51_PHY_CTRL_PHY_AUTOMDIX                  << SRT_NX51_phy_control_phy0_automdix;
  ulPhyControlReg |= NX51_PHY_CTRL_PHY_ENABLE                    << SRT_NX51_phy_control_phy0_enable;
  ulPhyControlReg |= NX51_PHYCTRL_MODE_ALL_CAPABLE_AUTONEG_AUTOMDIXEN << SRT_NX51_phy_control_phy1_mode;
  ulPhyControlReg |= NX51_PHY_CTRL_PHY_AUTOMDIX                  << SRT_NX51_phy_control_phy1_automdix;
  ulPhyControlReg |= NX51_PHY_CTRL_PHY_ENABLE                    << SRT_NX51_phy_control_phy1_enable;
  NX51_INTPHY_Init( ulPhyControlReg );

  /* reset pointer FIFO borders */
  NX51_PFIFO_Reset();
#endif

  phy_init(0, NX51_PHY_CTRL_PHY_ADDR + 0, PHYID_NETX_INTPHY, &s_tPhyOps);
  phy_init(1, NX51_PHY_CTRL_PHY_ADDR + 1, PHYID_NETX_INTPHY, &s_tPhyOps);

  /* initialize the USB port */
  NX51_USBCDC_Init();
  uprint_init(sys_ser_putc, sys_ser_flush, NULL);

  /* unlock IRQs */
  NX51_IRQFIQ_UNLOCK

  if( main_task(NO_OF_ETH_PORTS) )
  {
    sys_led_rdyrun_set(LED_COLOUR_RED);
  }

  /* do not return */
  while(1);
}
