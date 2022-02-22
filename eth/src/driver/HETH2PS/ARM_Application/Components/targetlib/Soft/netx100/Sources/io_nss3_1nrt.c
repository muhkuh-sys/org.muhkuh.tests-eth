#include "io.h"
#include "common_func_netx500.h"
#include "usb_globals.h"
#include "usb_requests_cdc.h"
#include "asic.h"
#include "phy.h"
#include "serial.h"
#include <stdbool.h>
#include <stdio.h>

static bool s_fHasUsbPullDown;


void io_output_init(void)
{
  /* Configure PIOs 5..4 as output (Dual-LED) and set them off */
  NX500_PIO_SetOutput(4, 1);
  NX500_PIO_SetOutput(5, 1);
  NX500_PIO_SetOutput(6, 1);
  NX500_PIO_SetOutput(7, 1);
  NX500_PIO_SetMode(4, NX500_PIO_MODE_OUT);
  NX500_PIO_SetMode(5, NX500_PIO_MODE_OUT);
  NX500_PIO_SetMode(6, NX500_PIO_MODE_OUT);
  NX500_PIO_SetMode(7, NX500_PIO_MODE_OUT);
}

unsigned int io_output_cnt(void)
{
  return 0;
}

void io_output_set(uint32_t ulOutput)
{
}

void io_input_init(void)
{
}

unsigned int io_input_cnt(void)
{
  return 0;
}

uint32_t io_input_get(void)
{
  return 0;
}

uint32_t io_address_get(void)
{
  return 0;
}

void io_init(void)
{
  s_fHasUsbPullDown = true;

  /* disable output enables of PIOs 0..3,8..14,16,17,26..30 to use PU/PD configuration of PHY */
  NX500_PIO_SetMode( 0, NX500_PIO_MODE_IN);
  NX500_PIO_SetMode( 1, NX500_PIO_MODE_IN);
  NX500_PIO_SetMode( 2, NX500_PIO_MODE_IN);
  NX500_PIO_SetMode( 3, NX500_PIO_MODE_IN);
  NX500_PIO_SetMode( 8, NX500_PIO_MODE_IN);
  NX500_PIO_SetMode( 9, NX500_PIO_MODE_IN);
  NX500_PIO_SetMode(10, NX500_PIO_MODE_IN);
  NX500_PIO_SetMode(11, NX500_PIO_MODE_IN);
  NX500_PIO_SetMode(12, NX500_PIO_MODE_IN);
  NX500_PIO_SetMode(13, NX500_PIO_MODE_IN);
  NX500_PIO_SetMode(14, NX500_PIO_MODE_IN);
  NX500_PIO_SetMode(16, NX500_PIO_MODE_IN);
  NX500_PIO_SetMode(17, NX500_PIO_MODE_IN);
  NX500_PIO_SetMode(26, NX500_PIO_MODE_IN);
  NX500_PIO_SetMode(27, NX500_PIO_MODE_IN);
  NX500_PIO_SetMode(28, NX500_PIO_MODE_IN);
  NX500_PIO_SetMode(29, NX500_PIO_MODE_IN);
  NX500_PIO_SetMode(30, NX500_PIO_MODE_IN);

  /* set io_config to enable MII2 */
  NX500_AsicCtrl_SetIoConfig( 1,  MSK_NX500_io_config_sel_mii2     /* for MII */
                                 |MSK_NX500_io_config_sel_mii23    /* for MDIO */
                                 |MSK_NX500_io_config_sel_led_mii2 /* for Link Signal */
                            );

  /* detect old revision of NS-S3 */
  NX500_GPIO_SetupMode(13, NX500_GPIO_MODE_INPUT_READ, NX500_GPIO_MODE_NOINVERT);
  if( 0 == NX500_GPIO_GetInput(13) )
  {
    /* HW REV <= 3 */
    s_fHasUsbPullDown = false;
  }
}

static int io_usb_poll(unsigned int uiPort)
{
  if( uiPort != 0 ) return EOF;
  return (usb_pollConnection() == USB_CDC_ConnectionState_Connected) ? 0 : EOF;
}

void io_serial_init(unsigned int uiPort)
{
  if( uiPort != 0 ) return;

  /* initialize the USB port */
  if(s_fHasUsbPullDown)
  {
    /* pull down USB_D+ (Bus Reset) */
    NX500_GPIO_SetupMode(12, NX500_GPIO_MODE_OUTPUT_SET_TO_0, NX500_GPIO_MODE_NOINVERT);
    sleep(10);
  }

  /* initialize the USB port */
  usb_init();

  if(s_fHasUsbPullDown)
  {
    /* pull up USB_D+ (Release Bus Reset) */
    NX500_GPIO_SetupMode(12, NX500_GPIO_MODE_OUTPUT_SET_TO_1, NX500_GPIO_MODE_NOINVERT);
  }

  serial_add(0, 0, io_usb_poll, usb_cdc_buf_rec_peek, usb_cdc_buf_rec_get, usb_cdc_buf_send_put, usb_cdc_buf_send_flush);
}

void extphy_write(unsigned int uiPhyAddr, unsigned int uiReg, unsigned int uiVal)
{
  NX500_MIIMU_WritePhyReg(NX500_MIIMU_SEND_PREAMBLE, NX500_MIIMU_MDCFREQ_HIGH, uiPhyAddr, uiReg, (unsigned short)uiVal, 0);
}

unsigned int extphy_read(unsigned int uiPhyAddr, unsigned int uiReg)
{
  unsigned short usVal = 0xffff;
  NX500_MIIMU_ReadPhyReg( NX500_MIIMU_SEND_PREAMBLE, NX500_MIIMU_MDCFREQ_HIGH, NX500_MIIMU_RTA_0_BITS, uiPhyAddr, uiReg, &usVal, 0 );
  return (unsigned int)usVal;
}

/* reset and setup PHY */
void extphy_init(void)
{
  unsigned int uiPhyAddr = 1;
  unsigned int uiPhyRegVal;
  uint32_t ulExpPhyId = PHYID_MICREL_KSZ8041;
  uint32_t ulPhyId;

  /* Reset external PHY */
  NX500_RSTCTRL_SetCfg(  MSK_NX500_reset_ctrl_RES_REQ_OUT
                        |MSK_NX500_reset_ctrl_EN_RES_REQ_OUT
                      );

  /* make a dummy MDIO read to cure MDIO first access bug (used to generate Reset Delay) */
  uiPhyRegVal = extphy_read(uiPhyAddr, 0);

  /* clear reset out pin */
  NX500_RSTCTRL_SetCfg( MSK_NX500_reset_ctrl_EN_RES_REQ_OUT );

  /* read PHY ID (Register 3) to check that PHY is accessible via MDIO */
  do {
    uiPhyRegVal = extphy_read(uiPhyAddr, DRV_CB12_PHY_ID_1);
    ulPhyId = (uint32_t)uiPhyRegVal << 16;
    uiPhyRegVal = extphy_read(uiPhyAddr, DRV_CB12_PHY_ID_2);
    ulPhyId |= uiPhyRegVal & 0xfff0;
  } while( ulPhyId != ulExpPhyId );

  /* set AutoNeg advertisement to all capable */
  uiPhyRegVal = extphy_read(uiPhyAddr, DRV_CB12_AUTO_NEG_ADVERTISEMENT);
  uiPhyRegVal |= (0xf << 5);
  extphy_write(uiPhyAddr, DRV_CB12_AUTO_NEG_ADVERTISEMENT, uiPhyRegVal);


  /* configure LED mode */
  uiPhyRegVal = extphy_read(uiPhyAddr, 30);
  uiPhyRegVal = (uiPhyRegVal & 0x3fff)
               |(0x1 << 14);
  extphy_write(uiPhyAddr, 30, uiPhyRegVal);

  /* enable and restart AutoNeg */
  uiPhyRegVal = (1 << 12) | (1 << 9);
  extphy_write(uiPhyAddr, DRV_CB12_CONTROL, uiPhyRegVal);

  phy_add(2, uiPhyAddr, extphy_write, extphy_read);
}
