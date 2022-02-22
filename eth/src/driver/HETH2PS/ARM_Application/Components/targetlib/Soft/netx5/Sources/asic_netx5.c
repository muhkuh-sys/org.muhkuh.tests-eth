#include "asic_netx5.h"
#include "phy.h"
#include "common_func_netx5.h"
#include "common_func_netx50.h"
#include "dpm_func_netx50.h"
#include "regdef_netx5.h"
#include "asic.h"

#include <stdlib.h>

#define MIIMU_READ(phy,reg,data)  NX5_MIIMU_ReadPhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, NX5_MIIMU_RTA_0_BITS, phy, reg, data, NULL)
#define MIIMU_WRITE(phy,reg,data) NX5_MIIMU_WritePhyReg(NX5_MIIMU_SEND_PREAMBLE, NX5_MIIMU_MDCFREQ_HIGH, phy, reg, data, NULL)

void netx5_init(void)
{
  int iResult;

  /* setup netx50 extension bus to 8 bit including reset of netX5 */
  iResult = NX50_DPM_SetupExtbus( NX50_DPM_CFG_08_NORDY, 1, 1000000 );
  if( iResult != 0 ) {
    NX50_RDYRUN_SetRdyRunLed( NX50_RDYRUN_LED_RED );
    while(1) {}
  }

  /* initialize netX5 DPM */
  iResult = NX5_DPM_InitDpm( 16384, NX5_DPM_SIZE_16, 0, 1 );
  if( iResult != 0 ) {
    NX50_RDYRUN_SetRdyRunLed( NX50_RDYRUN_LED_RED );
    while(1);
  }

  /* setup netx50 extension bus to 16 bit without reset of netX5 */
  iResult = NX50_DPM_SetupExtbus( NX50_DPM_CFG_16_RDY, 0, 1000000 );
  if( iResult != 0 ) {
    NX50_RDYRUN_SetRdyRunLed( NX50_RDYRUN_LED_RED );
    while(1) {}
  }

  /* check netx5 DPM accessible */
  iResult = NX5_DPM_ChkDpm();
  if( iResult!=0 ) {
    NX50_RDYRUN_SetRdyRunLed( NX50_RDYRUN_LED_RED );
    while(1);
  }

  /* Configure Window 5 (INTRAM) as byte accessible */
  NX5_DPM_SetupByteAccess( 5, 1 );

  /* Attention: We need to disable byte access on window 2 and 3 as this will break loading a XC program */
  NX5_DPM_SetupByteAccess(2, 0);
  NX5_DPM_SetupByteAccess(3, 0);

  // wait until DPM is unlocked
  NX5_DPM_WaitUnlocked();

  /* configure netX5 interrupts (enable Com0, Msync0, Com1, Msync1 and DPM Error) */
  NX5_DPM_SetupIrq( ( MSK_NX5_dpm_irq_irq_mask_msync0
                    | MSK_NX5_dpm_irq_irq_mask_msync1
                    | MSK_NX5_dpm_irq_irq_mask_com0
                    | MSK_NX5_dpm_irq_irq_mask_com1
                    | MSK_NX5_dpm_irq_irq_mask_dpm_err
                    ), 0, 0 );

  netx5_systime_init(0, 0);

  /* configure low-active COM0 and switch it off */
  NX5_PIO_SetMode( 0, NX5_PIO_MODE_OUT ); NX5_PIO_SetOutput( 0, 1 );
  NX5_PIO_SetMode( 1, NX5_PIO_MODE_OUT ); NX5_PIO_SetOutput( 1, 1 );

  /* configure low-active COM1 and switch it off */
  NX5_PIO_SetMode( 2, NX5_PIO_MODE_OUT ); NX5_PIO_SetOutput( 2, 1 );
  NX5_PIO_SetMode( 3, NX5_PIO_MODE_OUT ); NX5_PIO_SetOutput( 3, 1 );

  /* --- Input DIP switches --- */
  NX5_PIO_SetMode( 4, NX5_PIO_MODE_IN );
  NX5_PIO_SetMode( 5, NX5_PIO_MODE_IN );
  NX5_PIO_SetMode( 6, NX5_PIO_MODE_IN );
  NX5_PIO_SetMode( 7, NX5_PIO_MODE_IN );

  /* --- Input Address switches S200 --- */
  NX5_PIO_SetMode( 8, NX5_PIO_MODE_IN );
  NX5_PIO_SetMode( 9, NX5_PIO_MODE_IN );
  NX5_PIO_SetMode( 10, NX5_PIO_MODE_IN );
  NX5_PIO_SetMode( 11, NX5_PIO_MODE_IN );

  /* --- Input Address switches S201 --- */
  NX5_PIO_SetMode( 12, NX5_PIO_MODE_IN );
  NX5_PIO_SetMode( 13, NX5_PIO_MODE_IN );
  NX5_PIO_SetMode( 14, NX5_PIO_MODE_IN );
  NX5_PIO_SetMode( 15, NX5_PIO_MODE_IN );

  /* --- Output LEDs configuration, clear low-active outputs --- */
  NX5_DPM_SetPioOut31_28( 0 ); /* DPM PIOs 31..28 connected to low-active LEDs */
  NX5_DPM_SetPioOe31_28( 0xf );  /* DPM PIOs 31..28 used as output */

  /* reset XC units of netX5 */
  iResult = NX5_XC_Reset( 0, NULL );
  if( iResult != 0 ) {
    NX50_RDYRUN_SetRdyRunLed( NX50_RDYRUN_LED_RED );
    while(1);
  }
  iResult = NX5_XC_Reset( 1, NULL );
  if( iResult != 0 ) {
    NX50_RDYRUN_SetRdyRunLed( NX50_RDYRUN_LED_RED );
    while(1);
  }

  /* reset pointer FIFO unit of netX5 */
  NX5_PFIFO_Reset();

  /* configure netx50 DPM IRQ */
  NX50_DPM_ConfigDpmIrq( (1<<31) /* global enable */
                        |(1<<27) /* pio47 */
                       );
}

void netx5_systime_init(uint32_t ulBorder, uint32_t ulSpeed)
{
  NX5_SYSTIME_SetBorder(ulBorder, NULL);
  NX5_SYSTIME_SetSpeed(ulSpeed, NULL);
  NX5_SYSTIME_SetSystime(0, 0, NULL);
}

/* read PHY register */
unsigned int netx5_phy_read(unsigned int uiPhy, unsigned int uiReg)
{
  uint16_t usMiimuData = 0;

  MIIMU_READ(uiPhy, uiReg, &usMiimuData);

  return (unsigned int)usMiimuData;
}

/* write PHY register */
void netx5_phy_write(unsigned int uiPhy, unsigned int uiReg, unsigned int uiValue)
{
  MIIMU_WRITE(uiPhy, uiReg, (uint16_t)uiValue);
}

void netx5_phy_init(void)
{
  /* Reset external PHYs: enable phy_clk_out, OE=1, do not derive PHY clock out from clock */
  NX5_EXTPHY_EnablePhyClk(1, 0);

  NX5_EXTPHY_SetPhyReset(1);
  sleep(1);
  NX5_EXTPHY_SetPhyReset(0);
  sleep(2);

  /* initialize external PHYs to AutoNeg all capable mode */
  if( 0 != NX5_EXTPHY_Init(NX5_EXT_PHY1_ADDR, PHY_MODE_AUTONEG_AUTOMDIXEN) ) {
    NX50_RDYRUN_SetRdyRunLed( NX50_RDYRUN_LED_RED );
    while(1);
  }

  if( 0 != NX5_EXTPHY_Init(NX5_EXT_PHY2_ADDR, PHY_MODE_AUTONEG_AUTOMDIXEN) ) {
    NX50_RDYRUN_SetRdyRunLed( NX50_RDYRUN_LED_RED ); // ERROR: check failed
    while(1);
  }

  phy_add(0, NX5_EXT_PHY1_ADDR, netx5_phy_write, netx5_phy_read);
  phy_add(1, NX5_EXT_PHY2_ADDR, netx5_phy_write, netx5_phy_read);
}
