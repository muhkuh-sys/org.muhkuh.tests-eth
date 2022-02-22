/**
 * \file sdram_test.c
 * \brief External SD-RAM memory test
 *
 * \author Marvin Mager - mmager@hilscher.com
 *
 * \date 01.09.2021 - created
 */

#include "sdram_test.h"
#include "hw_defines.h"         /* register addresses */
#include "hal_irq.h"
#include "targetlib.h"
#include "uprintf.h"

HW_PTR_HIF_IO_CTRL(hif_io_ctrl_reg);  /**< \brief Pointer to hif_io_ctrl base register */

/**
 * \brief Initializes external SD-RAM of type IS45S16400J-7BLA2
 *
 * Timings are taken from COMPVERIFY-74. This function will not configure the PAD_CTRL
 * registers, this has to be done prior to initialization.
 *
 * FIXME: Initialization via software does not work! Use hw-config instead.
 */
void sdram_init(void)
{
#if 0
  /* Select HIF MI address width A0...A11 */
  hif_io_ctrl_reg->ulHif_io_cfg &= ~(MSK_NX90_hif_io_cfg_sel_hif_a_width);
  hif_io_ctrl_reg->ulHif_io_cfg |= 0b0001;

  /* Global HIF IO memory interface configuration in 16bit mode */
  hif_io_ctrl_reg->ulHif_io_cfg &= ~(MSK_NX90_hif_io_cfg_hif_mi_cfg);
  hif_io_ctrl_reg->ulHif_io_cfg |= 0b01;

  /* Enable HIF IOs for SDRAM memory interface configuration */
  hif_io_ctrl_reg->ulHif_io_cfg |= MSK_NX90_hif_io_cfg_en_hif_sdram_mi;

  /* Configure sdram_timing_register */
  /* 000. .... .... .... .... .... .... .... : ...
   * ...0 .... .... .... .... .... .... .... : bypass_neg_delay
   * .... 0... .... .... .... .... .... .... : ...
   * .... .000 .... .... .... .... .... .... : data_sample_phase -> 1.25ns
   * .... .... 0... .... .... .... .... .... : ...
   * .... .... .011 .... .... .... .... .... : mem_sdclk_phase   -> 5ns
   * .... .... .... 00.. .... .... .... .... : ...
   * .... .... .... ..00 .... .... .... .... : t_REFI            -> 3.9us
   * .... .... .... .... 0011 .... .... .... : t_RFC             -> 19 clocks
   * .... .... .... .... .... 0... .... .... : ...
   * .... .... .... .... .... .010 .... .... : t_RAS             -> 5 clocks
   * .... .... .... .... .... .... 01.. .... : t_RP              -> 2 clocks
   * .... .... .... .... .... .... ..01 .... : t_WR              -> 2 clocks
   * .... .... .... .... .... .... .... 00.. : ...
   * .... .... .... .... .... .... .... ..01 : t_RCD             -> 2 clocks
   */
  uint32_t* sdram_timing_ctrl = (uint32_t*)Adr_NX90_sdram_timing_ctrl;
  //*sdram_timing_ctrl = 0x00303251; /* Values from compverify */
  *sdram_timing_ctrl = 0x01403251; /* Values used in netX studio template */

  /* Configure sdram_mode_register */
  /* 0000 0000 0000 0000 0000 00.. .... .... : ...
   * .... .... .... .... .... ..0. .... .... : Write burst mode -> Programmed burst length
   * .... .... .... .... .... ...0 0... .... : Operation mode   -> Standard operation
   * .... .... .... .... .... ....  010 .... : Latency mode     -> 2
   * .... .... .... .... .... .... .... 0... : Burst type       -> Sequential
   * .... .... .... .... .... .... .... .011 : Burst length     -> 8
   */
  uint32_t* sdram_mr = (uint32_t*)Adr_NX90_sdram_mr;
  *sdram_mr = 0x00000022; /* Values used in netX studio template */
  //*sdram_mr = 0x00000023; /* Values from compverify */

  /* Configure sdram_general_ctrl register */
  /* 0... .... .... .... .... .... .... .... : refresh_status
   * .0.. .... .... .... .... .... .... .... : sdram_ready
   * ..00 00.. .... .... .... .... .... .... : ...
   * .... ..00 .... .... .... .... .... .... : refresh mode -> fixed interval
   * .... .... 0000 .... .... .... .... .... : ...
   * .... .... .... 0... .... .... .... .... : ctrl_en
   * .... .... .... .1.. .... .... .... .... : extclk_en
   * .... .... .... ..0. .... .... .... .... : sdram_pwdn
   * .... .... .... ...1 .... .... .... .... : dbus16       -> 16 bit databus width
   * .... .... .... .... 0000 0... .... .... : ...
   * .... .... .... .... ....  000 .... .... : columns      -> 256 columns
   * .... .... .... .... .... .... 00.. .... : ...
   * .... .... .... .... .... .... ..01 .... : rows         -> 4k rows
   * .... .... .... .... .... .... .... 00.. : ...
   * .... .... .... .... .... .... .... ..01 : banks        -> 4 banks
   */
  uint32_t* sdram_general_ctrl = (uint32_t*)Adr_NX90_sdram_general_ctrl;
  *sdram_general_ctrl = 0x000D0011;

  /* Enable the memory controller with ctrl_en bit */
  *sdram_general_ctrl |= MSK_NX90_sdram_general_ctrl_ctrl_en;

  /* Wait until sdram_ready is set */
  while(!(*sdram_general_ctrl & MSK_NX90_sdram_general_ctrl_sdram_ready));
#endif
}

/**
 * \brief Test procedure for on-board SD-RAM
 *
 * Does only work when a hw-config is flashed onto the board first that enables the SD-RAM.
 * When the SD-RAM controller is not initialized, the program will halt in this function.
 */
void sdram_test(void)
{
  /* Check if SD-RAM is initialized with HW-config */
  uint32_t* sdram_general_ctrl = (uint32_t*)Adr_NX90_sdram_general_ctrl;
  if(!(*sdram_general_ctrl & MSK_NX90_sdram_general_ctrl_sdram_ready))
  {
    IRQ_Disable();
    uprint_init(serial_send, NULL, 0);
    uprintf("ERROR: SD-RAM controller not initialized! HW config missing?\n");
    while(1)
    {
      led_com_set(0, LED_COLOUR_RED);
      led_com_set(1, LED_COLOUR_OFF);
      sleep(250);
      led_com_set(0, LED_COLOUR_OFF);
      led_com_set(1, LED_COLOUR_RED);
      sleep(250);
    };
  }
}
