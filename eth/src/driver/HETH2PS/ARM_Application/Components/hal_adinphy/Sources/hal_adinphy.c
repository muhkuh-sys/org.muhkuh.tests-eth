/**
 * \file hal_adinphy.c
 * \brief Functions for external PHY ADIN1100 as used on NXEB 90-SPE board
 *
 * \author Marvin Mager - mmager@hilscher.com
 * \copyright Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
 */

/*lint -save -e534 */

#include "hal_adinphy.h"

#include "hw_defines.h"               /* netX 90 register defines */
#include "hal_asicctrl.h"             /* Set RESET_N output, phy_ctrl0 access */
#include "hw_irqid_arm_com.h"         /* IRQ ID for HIF_PIO interrupt */
#include "hal_armirq.h"               /* HAL to enable/disable interrupts */

#include "adin1100_regdef.h"          /* ADIN1100 register definition */

#ifndef ADIN1100_CHIP_REV
#define ADIN1100_CHIP_REV 2           /* Standard ADIN1100 chip revision is U2 */
#endif

#define BV(x) (1 << x)                /* Bit vector with 1 at bit x */
#define BM(name) (MSK_NX90_##name)    /* netX 90 bit mask */
#define WAIT_100NS(x) for(int i = 0; i < x; i++)    /* Wait for ~100ns */

void(*s_link_state_callback)(void* pvCtx) = NULL;   /* Pointer to a callback function for a
                                                       link state change of any ADIN1100 PHY */

HW_PTR_HIF_IO_CTRL(hif_io_ctrl_reg)   /* Pointer to hif_io_ctrl base register */
HW_PTR_PHY_CTRL(phy_ctrl_reg)         /* Pointer to phy_ctrl base register */

SPE_PHY_T ext_phy[2];                 /* SPE PHY data structure, one per xC port */

int adinphy_initialize(uint8_t xc_port, uint8_t mdio_addr, void* pt_inst)
{
  /* Check parameters for xC port number and MDIO address */
  if(xc_port > 1) {
    return RC_INVALID_PARAM;
  }

  /* Check if correct MDIO address is supplied */
  if(mdio_addr > 31) {
    return RC_INVALID_PARAM;
  }

  /* Check if instance pointer is valid and store it */
  ext_phy[xc_port].pt_instance = (pt_inst != NULL ? pt_inst : NULL);
  ext_phy[xc_port].mdio_addr = mdio_addr;
  ext_phy[xc_port].port = xc_port;

  uint16_t crsm_stat = 0x0;

  /* Release reset and wait for PHY recovery. This will have no effect during runtime, it is used
   * to activate the PHY after io_init() */
  AsicCtrl_SetResetOut(false, true);
  do {
    adinphy_read_reg(ext_phy[xc_port].mdio_addr, AD1100_CRSM_STAT_REG, &crsm_stat);
  }
  while((!(crsm_stat & BV(AD1100_CRSM_SYS_RDY))) || crsm_stat == 0xFFFF);

  /* PHY software reset. This will reset the PHY during runtime if needed */
  adinphy_write_reg(ext_phy[xc_port].mdio_addr, AD1100_CRSM_SFT_RST_REG, BV(AD1100_CRSM_SFT_RST));
  do {
    adinphy_read_reg(ext_phy[xc_port].mdio_addr, AD1100_CRSM_STAT_REG, &crsm_stat);
  }
  while((!(crsm_stat & BV(AD1100_CRSM_SYS_RDY))) || crsm_stat == 0xFFFF);

  /* Put PHY in software power-down mode */
  adinphy_write_reg(ext_phy[xc_port].mdio_addr, AD1100_CRSM_SFT_PD_CNTRL_REG, BV(AD1100_CRSM_SFT_PD));
#if ADIN1100_CHIP_REV == 1
  /* Initialize link/activity LED */
  adinphy_write_reg(ext_phy[xc_port].mdio_addr, AD1100_LED_BLINK_TIME_CNTRL_REG, 0x0A0A);  /* LED blink interval */
  adinphy_write_reg(ext_phy[xc_port].mdio_addr, AD1100_LED_CNTRL_REG, BV(AD1100_LED_EN));  /* Enable PHY status LED */
#else
  /* Initialize LED0 as link/activity LED. LED1 is in high impedance state as default */
  adinphy_write_reg(ext_phy[xc_port].mdio_addr, AD1100_LED0_BLINK_TIME_CNTRL_REG, 0x0A0A); /* LED0 blink interval */
  adinphy_write_reg(ext_phy[xc_port].mdio_addr, AD1100_LED_CNTRL_REG, BV(AD1100_LED0_EN)); /* Enable PHY status LED0 in mode 1 and function 0*/
#endif

  /* Read interrupt status register to clear all latched interrupts */
  uint16_t dummy = 0x0;
  adinphy_read_reg(ext_phy[xc_port].mdio_addr, AD1100_CRSM_IRQ_STATUS_REG, &dummy);
  adinphy_read_reg(ext_phy[xc_port].mdio_addr, AD1100_PHY_SUBSYS_IRQ_STATUS_REG, &dummy);

  ext_phy[xc_port].initialized = true;

  /* Disable inverted link/act LEDs of respective port in PHY LED register */
  AsicCtrl_SetPhyCtrl0(AsicCtrl_GetPhyCtrl0() & ~(
    xc_port ? BM(phy_ctrl0_phy1_led_invert) : BM(phy_ctrl0_phy0_led_invert))
  );
  return RC_OKAY;
}

int adinphy_release(uint8_t xc_port)
{
  /* Check xC port number */
  if(xc_port > 1) {
    return RC_INVALID_PARAM;
  }

  /* Disable software power-down in external PHY */
  adinphy_write_reg(ext_phy[xc_port].mdio_addr, AD1100_CRSM_SFT_PD_CNTRL_REG, 0x0);

  /* Restart and enable Auto-Negotiation */
  adinphy_write_reg(ext_phy[xc_port].mdio_addr, AD1100_AN_CONTROL_REG, BV(AD1100_AN_RESTART) | BV(AD1100_AN_EN));
  return RC_OKAY;
}

void adinphy_setup_irq()
{
  /* Set HIF PIO input sampling mode to continuous */
  /* HIF_DIRQ IRQ input configuration, select low level interrupt */
  uint32_t ulVal = hif_io_ctrl_reg->ulHif_pio_cfg;
  ulVal &= ~(MSK_NX90_hif_pio_cfg_in_ctrl | MSK_NX90_hif_pio_cfg_irq_hif_dirq_cfg);
  ulVal |= (0x01U << SRT_NX90_hif_pio_cfg_in_ctrl);
  hif_io_ctrl_reg->ulHif_pio_cfg = ulVal;

  /* set HIF_DIRQ IRQ */
  hif_io_ctrl_reg->ulHif_pio_irq_arm_mask_set |= (1 << SRT_NX90_hif_pio_irq_arm_mask_set_irq_hif_dirq);
}

void adinphy_irq_handler()
{
  /* Check if interrupt came from HIF_DIRQ (MMIO17) pin */
  if(hif_io_ctrl_reg->ulHif_pio_irq_arm_masked & MSK_NX90_hif_pio_irq_arm_masked_irq_hif_dirq)
  {
    /* Check interrupt registers of both PHYs */
    for(uint8_t port = 0; port < 2; port++)
    {
      /* When the PHY is not initialized, skip it (needed when only 1 external PHY is used) */
      if(!ext_phy[port].initialized) {
        continue;
      }

      /* Read PHY system IRQ status register via MDIO */
      uint16_t phy_irq_status = 0x0;
      adinphy_read_reg(ext_phy[port].mdio_addr, AD1100_CRSM_IRQ_STATUS_REG, &phy_irq_status);

      /* Check if PHY was reset during runtime */
      /* Note: this should never happen, it was only added for external PHY test boards that have
       *       their own reset circuit */
      if(phy_irq_status & BV(AD1100_CRSM_HRD_REST_IRQ_LH)) {
        adinphy_initialize(port, ext_phy[port].mdio_addr, ext_phy[port].pt_instance);
        adinphy_release(port);
      }

      /* Read PHY interrupt status register via MDIO */
      uint16_t phy_subsys_irq_status = 0x0;
      adinphy_read_reg(ext_phy[port].mdio_addr, AD1100_PHY_SUBSYS_IRQ_STATUS_REG, &phy_subsys_irq_status);

      /* Check if interrupt was asserted due to link status change */
      if(phy_subsys_irq_status & BV(AD1100_LINK_STAT_CHNG_LH))
      {
        /* Call the external link state change callback, if its set */
        if(s_link_state_callback != NULL)
        {
          /* The link state change callback will be called with the external PHY's data structure
           * of type SPE_PHY_T as parameter */
          s_link_state_callback((void*)&ext_phy[port]);
        }
      }

      /* Check if interrupt was asserted due to auto-negotiation status change */
      /* Note: auto-negotiation status change interrupt is disabled by default. See ADIN1100
       * datasheet for more information. */
      if(phy_subsys_irq_status & BV(AD1100_AN_STAT_CHNG_IRQ_LH))
      {
        /* Nothing to do right now, when AN changes, the link status changes as well */
      }
    }
    /* Clear HIF_DIRQ interrupt in raw register */
    hif_io_ctrl_reg->ulHif_pio_irq_raw = MSK_NX90_hif_pio_irq_arm_masked_irq_hif_dirq;
  }
}

void adinphy_register_link_state_callback(void(*link_state_callback)(void* pvCtx))
{
  s_link_state_callback = link_state_callback;
}

int adinphy_get_linkstate(uint8_t xc_port, unsigned int* link, unsigned int* speed, unsigned int* duplex)
{
  /* Check xC port number */
  if(xc_port > 1) {
    return RC_INVALID_PARAM;
  }

  /* Check if external PHY on given port is initialized */
  if(!ext_phy[xc_port].initialized) {
    return RC_CONFIG_ERR;
  }

  /* Read AN status register twice to clear the latching mechanism of the link status bit */
  uint16_t an_status = 0;
  adinphy_read_reg(ext_phy[xc_port].mdio_addr, AD1100_AN_STATUS_REG, &an_status);
  adinphy_read_reg(ext_phy[xc_port].mdio_addr, AD1100_AN_STATUS_REG, &an_status);
  *link = (unsigned int)(an_status & BV(AD1100_AN_LINK_STATUS) ? 1 : 0);

  /* Always full duplex as well as 10MBit/s for this PHY */
  *duplex = 1;
  *speed = 10;
  return RC_OKAY;
}

unsigned int adinphy_read_cl45(unsigned int uiPhy, unsigned int uiReg)
{
  uint16_t data = 0x0;
  if(uiPhy > 31) {
      /* Invalid MDIO address supplied */
      return 0x0;
  }
  if(uiReg == 2 || uiReg == 3)
  {
    /* Append correct subdevice 0x1E to retrieve PHY ID during initialization. This
     * adds support for MDIO clause 22 register addresses 2 and 3 */
    uiReg |= 0x1E0000;
  }
  else if(uiReg < 0x010000) {
    /* Other MDIO clause 22 registers will be ignored */
    return 0x0;
  }
  adinphy_read_reg(uiPhy, uiReg, &data);
  return (unsigned int) data;
}

void adinphy_write_cl45(unsigned int uiPhy, unsigned int uiReg, unsigned int uiValue)
{
  if(uiPhy > 31) {
    /* Invalid MDIO address supplied */
    return;
  }
  if(uiReg < 0x010000) {
    /* This PHY does not have clause 45 registers below 0x01 MMD address */
    return;
  }
  adinphy_write_reg(uiPhy, uiReg, (uint16_t) uiValue);
}

/*lint -restore */

/*
 * Writes to MDIO interface via software using MIIMU_SW register
 *
 * Uses bit-banging to write a complete MDIO frame to the given MII management unit's SW register.
 * Does not disable global interrupts, this has to be done prior to calling this function.
 *
 * \param [in] mdio_sw_reg    Pointer on MIIMU_SW register
 * \param [in] mdio_frame     Complete MDIO frame with preamble
 */
void adinphy_write_mdio(volatile uint32_t* mdio_sw_reg, uint64_t mdio_frame)
{
  uint8_t cur_pos = 64;
  uint32_t miimu_enable_mask = HW_MSK(eth_miimu_sw_enable) | HW_MSK(eth_miimu_sw_mdoe);

  while(cur_pos != 0)
  {
    /* Compute current bit value and multiply with MDO (data line) enable bit mask */
    uint8_t current_bit = ((mdio_frame >> (cur_pos - 1)) & 1);
    *mdio_sw_reg = miimu_enable_mask | (current_bit * HW_MSK(eth_miimu_sw_mdo));

    /* Computing MDO bit takes ~400ns, setting the clock takes ~200ns, so wait here */
    WAIT_100NS(4);

    /* Toggle MDC (clock line) */
    *mdio_sw_reg |= HW_MSK(eth_miimu_sw_mdc);

    cur_pos--;
  };

  /* Set interface to idle */
  WAIT_100NS(2);
  *mdio_sw_reg = HW_MSK(eth_miimu_sw_enable);
}

/*
 * Reads from MDIO interface via software using MIIMU_SW register
 *
 * Uses bit-banging to write a complete MDIO frame to the given MII management unit's SW register.
 * During the TA and DATA field, the MIIMU_SW will be read to retrieve the data on the MDIO bus.
 * Does not disable global interrupts, this has to be done prior to calling this function.
 *
 * \param [in]  mdio_sw_reg   Pointer on MIIMU_SW register
 * \param [in]  mdio_frame    Complete MDIO frame with preamble
 * \param [out] data          Data received from MDIO
 */
void adinphy_read_mdio(volatile uint32_t* mdio_sw_reg, uint64_t mdio_frame, uint16_t* data)
{
  uint8_t cur_pos = 64;
  uint32_t miimu_enable_mask = HW_MSK(eth_miimu_sw_enable) | HW_MSK(eth_miimu_sw_mdoe);
  *data = 0x0;

  /* Write the preamble and addresses on MDIO first (without TA) */
  while(cur_pos != 18)
  {
    /* Compute current bit value and multiply with MDO (data line) enable bit mask */
    uint8_t current_bit = ((mdio_frame >> (cur_pos - 1)) & 1);
    *mdio_sw_reg = miimu_enable_mask | (current_bit * HW_MSK(eth_miimu_sw_mdo));

    /* Setting MDO bit takes ~400ns, setting the clock takes ~200ns, so wait here */
    WAIT_100NS(4);

    /* Toggle MDC (clock line) */
    *mdio_sw_reg |= HW_MSK(eth_miimu_sw_mdc);

    cur_pos--;
  };

  /* Deactivate MDO and MDOE for turnaround (two cycles), give control to slave */
  WAIT_100NS(1);    // Wait 100ns to catch up
  while(cur_pos != 16) {
    /* Clear clock bit */
    *mdio_sw_reg = HW_MSK(eth_miimu_sw_enable);
    WAIT_100NS(2);

    /* Toggle MDC (clock line) */
    *mdio_sw_reg |= HW_MSK(eth_miimu_sw_mdc);
    WAIT_100NS(2);

    cur_pos--;
  }

  /* Read data from MDIO for the next 16 samples */
  while (cur_pos != 0) {
    /* Clear clock bit */
    *mdio_sw_reg = HW_MSK(eth_miimu_sw_enable);

    /* Read data from MDIO register */
    *data |= (*mdio_sw_reg & HW_MSK(eth_miimu_sw_mdi_ro) ? 1 : 0) << (cur_pos - 1);

    /* Toggle MDC (clock line) */
    *mdio_sw_reg |= HW_MSK(eth_miimu_sw_mdc);
    WAIT_100NS(4);

    cur_pos--;
  };

  /* Set interface to idle */
  *mdio_sw_reg = HW_MSK(eth_miimu_sw_enable);
}

int adinphy_read_reg(uint8_t phy_addr, uint32_t reg_addr_combined, uint16_t* data)
{
  if(phy_addr > 31) {
    return RC_INVALID_PARAM;
  }

  /* Disable HIF_PIO interrupts in NVIC */
  ArmIrq_SetActive(IRQID_ARM_HIF_PIO, false);

  /* Use software MDIO register of xC unit 1 */
  volatile uint32_t* miimu_sw_reg;
  miimu_sw_reg = (volatile uint32_t*) &(phy_ctrl_reg[1]->ulInt_phy_ctrl_miimu_sw);

  /* Allocate space for complete MDIO frame with preamble */
  uint64_t mdio_frame = 0xFFFFFFFF00000000;

  /* ADDRESS frame with OP code 0b00 and start delimiter 0b00 */
  mdio_frame |= phy_addr << 23;                       // PHY address
  mdio_frame |= (reg_addr_combined & 0x1F0000U) << 2; // MMD address from given combined address
  mdio_frame |= 0x02U << 16;                          // Turnaround field
  mdio_frame |= (reg_addr_combined & 0xFFFF);         // Address in data field
  adinphy_write_mdio(miimu_sw_reg, mdio_frame);

  /* READ frame with OP code 0b11 and data */
  mdio_frame &= 0xFFFFFFFFCFFC0000;                   // Clear previous data, TA and OP code field
  mdio_frame |= 0x03U << 28;                          // Set OP code to READ instruction
  adinphy_read_mdio(miimu_sw_reg, mdio_frame, data);

  /* Enable HIF_PIO interrupts again */
  ArmIrq_SetActive(IRQID_ARM_HIF_PIO, true);
  return RC_OKAY;
}

int adinphy_write_reg(uint8_t phy_addr, uint32_t reg_addr_combined, uint16_t data)
{
  if(phy_addr > 31) {
    return RC_INVALID_PARAM;
  }

  /* Disable HIF_PIO interrupts in NVIC */
  ArmIrq_SetActive(IRQID_ARM_HIF_PIO, false);

  /* Use software MDIO register of xC unit 1 */
  volatile uint32_t* miimu_sw_reg;
  miimu_sw_reg = (volatile uint32_t*) &(phy_ctrl_reg[1]->ulInt_phy_ctrl_miimu_sw);

  /* Allocate space for complete MDIO frame with preamble */
  uint64_t mdio_frame = 0xFFFFFFFF00000000;

  /* ADDRESS frame with OP code 0b00 and start delimiter 0b00 */
  mdio_frame |= phy_addr << 23;                       // PHY address
  mdio_frame |= (reg_addr_combined & 0x1F0000U) << 2; // MMD address from given combined address
  mdio_frame |= 0x02U << 16;                          // Turnaround field
  mdio_frame |= (reg_addr_combined & 0xFFFF);         // Address in data field
  adinphy_write_mdio(miimu_sw_reg, mdio_frame);

  /* WRITE frame with OP code 0b01 and data */
  mdio_frame &= 0xFFFFFFFFFFFF0000;                   // Clear previous data field
  mdio_frame |= 0x01U << 28;                          // Set OP code to WRITE instruction
  mdio_frame |= data;                                 // Write data in data field
  adinphy_write_mdio(miimu_sw_reg, mdio_frame);

  /* Enable HIF_PIO interrupts again */
  ArmIrq_SetActive(IRQID_ARM_HIF_PIO, true);
  return RC_OKAY;
}

