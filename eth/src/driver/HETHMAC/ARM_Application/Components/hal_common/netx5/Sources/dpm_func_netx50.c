#include "regdef_netx50.h" // structures
#include "dpm_mapping_netx5.h"
#include "common_func_netx50.h" // function prototypes and defines
#include "tcm_define.h"

static NX50_ASIC_CTRL_AREA_T*                               s_ptAsicCtrl                = (NX50_ASIC_CTRL_AREA_T*) Addr_NX50_asic_ctrl;
#ifdef NX5_SERIAL_DPM
static NX50_SPI_AREA_T*                                     s_ptSpi                     = (NX50_SPI_AREA_T*) Addr_NX50_spi1;
#endif
static NX50_NETX_CONTROLLED_GLOBAL_REGISTER_BLOCK_1_AREA_T* s_ptNetXGlobalRegBlock1Area = (NX50_NETX_CONTROLLED_GLOBAL_REGISTER_BLOCK_1_AREA_T*) Addr_NX50_netx_controlled_global_register_block_1;
static NX50_NETX_CONTROLLED_GLOBAL_REGISTER_BLOCK_2_AREA_T* s_ptNetXGlobalRegBlock2Area = (NX50_NETX_CONTROLLED_GLOBAL_REGISTER_BLOCK_2_AREA_T*) Addr_NX50_netx_controlled_global_register_block_2;


/*
  ____    ____    __  __ 
 |  _ \  |  _ \  |  \/  |
 | | | | | |_) | | |\/| |
 | |_| | |  __/  | |  | |
 |____/  |_|     |_|  |_|

*/
                         

/*****************************************************************************/
/*! Setup Extension Bus
* \description
*   Configure the Extension Bus.
* \class 
*   DPM 
* \params
*   tSize          [in]  DPM Size
*   iReset         [in]  Reset Signal, if the reset is triggered on netX5\
*                        settings in tSize are ignored and Extension Bus is\
*                        initialized to netX5 default values.
*   uResetDelay    [in]  Reset Delay
* \return
*   0 on success
*   -1 on errorneous                                                         */
/*****************************************************************************/
int NX50_DPM_SetupExtbus( NX50_DPM_CFG_E tSize, int iReset, unsigned int uResetDelay )
{
  unsigned long ulExtBusCs0Cfg = 0;
  unsigned int uCnt;
  unsigned long ulTmp;
  int iRslt = 0;

#define SRT_tcson 23
#define SRT_trdon 20
#define SRT_twron 17
#define SRT_trdwroff 12
#define SRT_trdwrcyc 7

  switch( tSize ) {
    case NX50_DPM_CFG_08_RDY:
      ulExtBusCs0Cfg = 0x21 |
          (1<<SRT_tcson)    |
          (1<<SRT_trdon)    |
          (1<<SRT_twron)    |
          (8<<SRT_trdwroff) |
          (10<<SRT_trdwrcyc);
      break;
    case NX50_DPM_CFG_08_NORDY:
      ulExtBusCs0Cfg = 0x21 |
          (1<<SRT_tcson)    |
          (1<<SRT_trdon)    |
          (1<<SRT_twron)    |
          (17<<SRT_trdwroff) |
          (18<<SRT_trdwrcyc);
      break;
    case NX50_DPM_CFG_16_RDY:
      ulExtBusCs0Cfg = 0x23 |
          (1<<SRT_tcson)    |
          (1<<SRT_trdon)    |
          (1<<SRT_twron)    |
          (8<<SRT_trdwroff) |
          (10<<SRT_trdwrcyc);
      break;
    case NX50_DPM_CFG_16_NORDY:
      ulExtBusCs0Cfg = 0x03 |
          (1<<SRT_tcson)    |
          (1<<SRT_trdon)    |
          (1<<SRT_twron)    |
          (17<<SRT_trdwroff) |
          (18<<SRT_trdwrcyc);
      break;
    default:
      iRslt = -1; // invalid DPM size
      break;
  }

  if( iRslt != 0 )
  {
    return iRslt;
  }

  // enable dpm/pio mode
  ulTmp = s_ptAsicCtrl->ulIo_config & ~MSK_NX50_io_config_if_select_n;
  s_ptAsicCtrl->ulAsic_ctrl_access_key = s_ptAsicCtrl->ulAsic_ctrl_access_key;
  s_ptAsicCtrl->ulIo_config = ulTmp;

  s_ptNetXGlobalRegBlock2Area->ulExp_bus_reg         = ulExtBusCs0Cfg;
  s_ptNetXGlobalRegBlock2Area->ulIf_conf1            = 0x10247912;
  s_ptNetXGlobalRegBlock2Area->ulIf_conf2            = 0x01800000;
  s_ptNetXGlobalRegBlock2Area->ulIo_reg_mode0        = 0x37ff7ee7;
  s_ptNetXGlobalRegBlock2Area->ulIo_reg_mode1        = 0x400e7e67;
  s_ptNetXGlobalRegBlock2Area->ulIo_reg_drv_en0      = 0;
  s_ptNetXGlobalRegBlock2Area->ulIo_reg_drv_en1      = 0;
  
  // reset netX5 if requested
  if( iReset ) {
    // use slower timings to allow write cycles without busy
    s_ptNetXGlobalRegBlock2Area->ulExp_bus_reg       = 0x00a5cf81;

    /* save old value */
    ulTmp = s_ptAsicCtrl->ulReset_ctrl;
    
    // pull up reset out of netX50 and generate some delay
    s_ptAsicCtrl->ulReset_ctrl |= (MSK_NX50_reset_ctrl_RES_REQ_OUT | MSK_NX50_reset_ctrl_EN_RES_REQ_OUT);
    for(uCnt = 0; uCnt < uResetDelay; uCnt++);
  
    // pull down reset out of netX50 and generate some delay
    s_ptAsicCtrl->ulReset_ctrl &= ~(MSK_NX50_reset_ctrl_RES_REQ_OUT | MSK_NX50_reset_ctrl_EN_RES_REQ_OUT);
    for(uCnt = 0; uCnt < uResetDelay; uCnt++);

    /* restore old value */
    s_ptAsicCtrl->ulReset_ctrl = ulTmp;
  }
  return 0;
}


/*****************************************************************************/
/*! Configure DPM Irq
* \description
*   Configure DPM Interrupt.
* \class 
*   DPM 
* \params
*   ulDpmIrqMsk          [in]  DPM Irq Mask
* \return
*                                                                            */
/*****************************************************************************/
void NX50_DPM_ConfigDpmIrq( unsigned long ulDpmIrqMsk )
{
  /* load mask to DPM interrupt enable 0 */
  s_ptNetXGlobalRegBlock1Area->aulDpmas_int_en[0] = ulDpmIrqMsk;
}

/*****************************************************************************/
/*! Get DPM Irq
* \description
*   Read value of DPM Interrupt.
* \class 
*   DPM 
* \params
*   pulIrq          [out]  DPM Irq Value
* \return
*                                                                            */
/*****************************************************************************/
void NX50_DPM_GetDpmIrq( unsigned long* pulIrq )
{
  /* get hif irq register */
  *pulIrq = s_ptNetXGlobalRegBlock1Area->ulIrq_reg_netx;
}

/*****************************************************************************/
/*! Acknowledge the DPM Irq
* \description
*   Acknowledge DPM interrupt.
* \class 
*   DPM 
* \params
*   ulVal          [in]  Acknowledge Value
* \return
*                                                                            */
/*****************************************************************************/
void NX50_DPM_AckDpmIrq( unsigned long ulVal )
{
  /* acknowledge HIF irq */
  s_ptNetXGlobalRegBlock1Area->ulIrq_reg_netx = ulVal;
}

/*
  ____    ____   ___      __                    ____    ____    __  __
 / ___|  |  _ \ |_ _|    / _|   ___    _ __    |  _ \  |  _ \  |  \/  |
 \___ \  | |_) | | |    | |_   / _ \  | '__|   | | | | | |_) | | |\/| |
  ___) | |  __/  | |    |  _| | (_) | | |      | |_| | |  __/  | |  | |
 |____/  |_|    |___|   |_|    \___/  |_|      |____/  |_|     |_|  |_|
 
*/

#include "regdef_netx5.h"

#define MSK_CHIP_SELECT    1

#ifdef NX5_SERIAL_DPM

/*****************************************************************************/
/*! Initializes SPI module for access to serial DPM
* \description
*   Initializes the SPI1 module to be used for accessing netX5 via
*   serial DPM. Slave is connected on chip-select 0, SPI mode is 0.
* \class 
*   SPI_DPM 
* \params
*   ulSckMulAdd      [in]  Rate multiplier add value for SPI speed \n\
*                          ulSckMulAdd = spi_clk [MHz] * 4096 / 100
*   iReset           [in]  Reset Signal
*   uResetDelay      [in]  Reset Delay
* \return
*   0 on success
*   -1 on erroneous                                                          */
/*****************************************************************************/
unsigned long NX50_SerDPM_SetupSPI(unsigned long  ulSckMulAdd, int iReset, unsigned int uResetDelay)
{
  unsigned int  uCnt;

  /* speed > 50 MHz is not supported */
  if (ulSckMulAdd > 0x800)
    return -1;
  
  /* confuigure hif irq pin */
  s_ptNetXGlobalRegBlock2Area->ulIf_conf1            = 0x00000100;  /* set irq_mode to push pull output */
  s_ptNetXGlobalRegBlock2Area->ulIf_conf2            = 0x00000000;  /* irq polarity = 0 */
  s_ptNetXGlobalRegBlock2Area->ulIo_reg_mode1        = 0x40000000;
  s_ptNetXGlobalRegBlock2Area->ulIo_reg_drv_en0      = 0;
  s_ptNetXGlobalRegBlock2Area->ulIo_reg_drv_en1      = 0;
  
  /* reset netX5 if requested */
  if( iReset ) 
  {
    /* pull up reset out of netX50 and generate some delay */
    s_ptAsicCtrl->ulReset_ctrl |= (MSK_NX50_reset_ctrl_RES_REQ_OUT | MSK_NX50_reset_ctrl_EN_RES_REQ_OUT);
    for(uCnt = 0; uCnt < uResetDelay; uCnt++);
  
    /* pull down reset out of netX50 and generate some delay */
    s_ptAsicCtrl->ulReset_ctrl &= ~(MSK_NX50_reset_ctrl_RES_REQ_OUT | MSK_NX50_reset_ctrl_EN_RES_REQ_OUT);
    for(uCnt = 0; uCnt < uResetDelay; uCnt++);
  }


  /* configure spi interface */
  s_ptSpi->aulSpi_cr[0] = (ulSckMulAdd << SRT_NX50_spi_cr0_sck_muladd) | (7 << SRT_NX50_spi_cr0_datasize);

  s_ptSpi->aulSpi_cr[1] = MSK_NX50_spi_cr1_rx_fifo_clr |
                          (8 << SRT_NX50_spi_cr1_rx_fifo_wm) |
                          MSK_NX50_spi_cr1_tx_fifo_clr |
                          (8 << SRT_NX50_spi_cr1_tx_fifo_wm) |
                          MSK_NX50_spi_cr1_fss_static |
                          (0 << SRT_NX50_spi_cr1_fss) |
                          MSK_NX50_spi_cr1_SSE;
                        
  s_ptSpi->aulSpi_cr[1] = (8 << SRT_NX50_spi_cr1_rx_fifo_wm) |
                          (8 << SRT_NX50_spi_cr1_tx_fifo_wm) |
                          MSK_NX50_spi_cr1_fss_static |
                          (0 << SRT_NX50_spi_cr1_fss) |
                          MSK_NX50_spi_cr1_SSE;

  /* initialize netx5 DPM by doing two dummy accesses */
  /* assert chip select */
  s_ptSpi->aulSpi_cr[1] |= (MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
  
  s_ptSpi->ulSpi_dr = 0x00;             /* send address */
  s_ptSpi->ulSpi_dr = 0x00;
  s_ptSpi->ulSpi_dr = 0x80 + 1;                            /* read one byte */
  s_ptSpi->ulSpi_dr = 0x00;                                /* read transfer */

  /* wait for transfer to be complete */
  while  (s_ptSpi->ulSpi_sr & MSK_NX50_spi_sr_BSY);        

  /* deassert chip select */
  s_ptSpi->aulSpi_cr[1] &= ~(MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);

  /* read dummy data */
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;

  /* assert chip select */
  s_ptSpi->aulSpi_cr[1] |= (MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
  
  s_ptSpi->ulSpi_dr = 0x00;             /* send address */
  s_ptSpi->ulSpi_dr = 0x00;
  s_ptSpi->ulSpi_dr = 0x80 + 1;                            /* read one byte */
  s_ptSpi->ulSpi_dr = 0x00;                                /* read transfer */

  /* wait for transfer to be complete */
  while  (s_ptSpi->ulSpi_sr & MSK_NX50_spi_sr_BSY);        

  /* deassert chip select */
  s_ptSpi->aulSpi_cr[1] &= ~(MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);

  /* read dummy data */
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;

  return 0;
}


/*****************************************************************************/
/*! Read 1 Byte (Small Address Range)
* \description
*   Reads one byte from netX5 serial DPM in small address range mode.
* \class 
*   SPI_DPM 
* \params
*   ulDpmAddr      [in]  DPM address to read from
* \return
*   Read value                                                               */
/*****************************************************************************/
unsigned char NX50_SerDPM_Read8_SmallRange(unsigned long ulDpmAddr)
{   
  /* Read Byte from netX5 DPM in SPI mode, low range adressing (2 address header bytes) */
  unsigned char  abData[4];
  unsigned long  ulStatus;
  volatile int iIrq;

  /* lock irq */
  NX50_lock_irqfiq_save(iIrq);

  /* read as long as dpm_status is not correct */
  do
  {
    /*** read data byte ***/

    /* assert chip select */
    s_ptSpi->aulSpi_cr[1] |= (MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
    
    s_ptSpi->ulSpi_dr = (ulDpmAddr >> 8) & 0xff;             /* send address */
    s_ptSpi->ulSpi_dr = ulDpmAddr & 0xff;
    s_ptSpi->ulSpi_dr = 0x80 + 4;                            /* read whole dword */
    s_ptSpi->ulSpi_dr = 0x00;                                /* read transfer */
    s_ptSpi->ulSpi_dr = 0x00;                                /* read transfer */
    s_ptSpi->ulSpi_dr = 0x00;                                /* read transfer */
    s_ptSpi->ulSpi_dr = 0x00;                                /* read transfer */

    /* wait for transfer to be complete */
    while  (s_ptSpi->ulSpi_sr & MSK_NX50_spi_sr_BSY);        
  
    /* deassert chip select */
    s_ptSpi->aulSpi_cr[1] &= ~(MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
  
    /* read dummy data */
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
  
    /* read data byte */
    abData[0] = s_ptSpi->ulSpi_dr;
    abData[1] = s_ptSpi->ulSpi_dr;
    abData[2] = s_ptSpi->ulSpi_dr;
    abData[3] = s_ptSpi->ulSpi_dr;
    
    /*** check if access was correct ***/
  
    /* assert chip select */
    s_ptSpi->aulSpi_cr[1] |= (MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
    
    s_ptSpi->ulSpi_dr = (REL_Adr_NX5_dpm_status_err_auto_reset >> 8) & 0xff;   /* send address */
    s_ptSpi->ulSpi_dr = REL_Adr_NX5_dpm_status_err_auto_reset & 0xff;
    s_ptSpi->ulSpi_dr = 0x80 + 1;           /* read one byte */
    s_ptSpi->ulSpi_dr = 0x00;               /* read transfer */

    /* wait for transfer to be complete */
    while  (s_ptSpi->ulSpi_sr & MSK_NX50_spi_sr_BSY);        
  
    /* deassert chip select */
    s_ptSpi->aulSpi_cr[1] &= ~(MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
  
    /* read dummy data */
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
  
    /* read data byte */
    ulStatus = (s_ptSpi->ulSpi_dr & 0xff);
    ulStatus |= (s_ptSpi->ulSpi_dr & 0xff) << 8;
    ulStatus |= (s_ptSpi->ulSpi_dr & 0xff) << 16;
    ulStatus |= (s_ptSpi->ulSpi_dr & 0xff) << 24;
    
  } while (ulStatus != 0);

  /* unlock irq */
  NX50_lock_irqfiq_restore(iIrq);
  
  return abData[ulDpmAddr & 0x3];
}


/*****************************************************************************/
/*! Write 1 Byte (Small Address Range)
* \description
*   Writes one byte to netX5 serial DPM in small address range mode.
* \class 
*   SPI_DPM 
* \params
*   ulDpmAddr      [in]  DPM address to read from
*   bData          [in]  Data to write
* \return
*                                                                            */
/*****************************************************************************/
void NX50_SerDPM_Write8_SmallRange(unsigned long ulDpmAddr, unsigned char bData)
{
  /* Write Byte to netX5 DPM in SPI mode, low range adressing (2 address header bytes) */
  unsigned long  ulStatus;
  volatile int iIrq;

  /* lock irq */
  NX50_lock_irqfiq_save(iIrq);

  do
  {
    /*** write data byte ***/

    /* assert chip select */
    s_ptSpi->aulSpi_cr[1] |= (MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
    
    s_ptSpi->ulSpi_dr = (ulDpmAddr >> 8) & 0xff;             /* send address */
    s_ptSpi->ulSpi_dr = ulDpmAddr & 0xff;
    s_ptSpi->ulSpi_dr = 1;                                   /* write one byte */
    s_ptSpi->ulSpi_dr = bData;                               /* write transfer */

    /* wait for transfer to be complete */
    while  (s_ptSpi->ulSpi_sr & MSK_NX50_spi_sr_BSY);        
  
    /* deassert chip select */
    s_ptSpi->aulSpi_cr[1] &= ~(MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
  
    /* read dummy data */
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;

    /*** check if access was correct ***/
  
    /* assert chip select */
    s_ptSpi->aulSpi_cr[1] |= (MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
    
    s_ptSpi->ulSpi_dr = (REL_Adr_NX5_dpm_status_err_auto_reset >> 8) & 0xff;   /* send address */
    s_ptSpi->ulSpi_dr = REL_Adr_NX5_dpm_status_err_auto_reset & 0xff;
    s_ptSpi->ulSpi_dr = 0x80 + 1;           /* read one byte */
    s_ptSpi->ulSpi_dr = 0x00;               /* read transfer */

    /* wait for transfer to be complete */
    while  (s_ptSpi->ulSpi_sr & MSK_NX50_spi_sr_BSY);        
  
    /* deassert chip select */
    s_ptSpi->aulSpi_cr[1] &= ~(MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
  
    /* read dummy data */
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
  
    /* read data byte */
    ulStatus = (s_ptSpi->ulSpi_dr & 0xff);

  } while (ulStatus != 0);

  /* unlock irq */
  NX50_lock_irqfiq_restore(iIrq);  
}



/*****************************************************************************/
/*! Read 1 DWord (Full Address Range) from xPEC DRAM
* \description
*   Reads one dword from netX5 serial DPM in full address range mode.
*   This function shall be used for all accesses to the xPEC DRAM area.
* \class 
*   SPI_DPM 
* \params
*   ulDpmAddr      [in]  DPM address to read from
* \return
*   Read value                                                               */
/*****************************************************************************/
LINK_TO_TCM_ATTRIBUTE
unsigned long NX50_SerDPM_Read32_XPEC_FullRange(unsigned long ulDpmAddr)
{
  unsigned long  ulData;
  unsigned long  ulStatus;
  volatile int iIrq;

  /* lock irq */
  NX50_lock_irqfiq_save(iIrq);
  
  /* read as long as dpm_status is not correct */
  do
  {
    /*** read data byte ***/

    /* assert chip select */
    s_ptSpi->aulSpi_cr[1] |= (MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
    
    s_ptSpi->ulSpi_dr = (ulDpmAddr >> 16) & 0xff;            /* send address */
    s_ptSpi->ulSpi_dr = (ulDpmAddr >> 8) & 0xff;        
    s_ptSpi->ulSpi_dr = ulDpmAddr & 0xff;
    s_ptSpi->ulSpi_dr = 0x80 + 4;                            /* read one dword */
    s_ptSpi->ulSpi_dr = 0x00;                                /* read transfer */
    s_ptSpi->ulSpi_dr = 0x00;                                /* read transfer */
    s_ptSpi->ulSpi_dr = 0x00;                                /* read transfer */
    s_ptSpi->ulSpi_dr = 0x00;                                /* read transfer */

    /* wait for transfer to be complete */
    while  (s_ptSpi->ulSpi_sr & MSK_NX50_spi_sr_BSY);        
  
    /* deassert chip select */
    s_ptSpi->aulSpi_cr[1] &= ~(MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
  
    /* read dummy data */
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
  
    /* read data byte */
    ulData = (s_ptSpi->ulSpi_dr & 0xff);
    ulData |= (s_ptSpi->ulSpi_dr & 0xff) << 8;
    ulData |= (s_ptSpi->ulSpi_dr & 0xff) << 16;
    ulData |= (s_ptSpi->ulSpi_dr & 0xff) << 24;
    
    
    /*** check if access was correct ***/
  
    /* assert chip select */
    s_ptSpi->aulSpi_cr[1] |= (MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
    
    s_ptSpi->ulSpi_dr = (REL_Adr_NX5_dpm_status_err_auto_reset >> 16) & 0xff;  /* send address */
    s_ptSpi->ulSpi_dr = (REL_Adr_NX5_dpm_status_err_auto_reset >> 8) & 0xff; 
    s_ptSpi->ulSpi_dr = REL_Adr_NX5_dpm_status_err_auto_reset & 0xff;
    s_ptSpi->ulSpi_dr = 0x80 + 1;           /* read one byte */
    s_ptSpi->ulSpi_dr = 0x00;               /* read transfer */

    /* wait for transfer to be complete */
    while  (s_ptSpi->ulSpi_sr & MSK_NX50_spi_sr_BSY);        
  
    /* deassert chip select */
    s_ptSpi->aulSpi_cr[1] &= ~(MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
  
    /* read dummy data */
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
  
    /* read data byte */
    ulStatus = (s_ptSpi->ulSpi_dr & 0xff);

  } while (ulStatus != 0);

  /* unlock irq */
  NX50_lock_irqfiq_restore(iIrq);
  
  return ulData;
}


/*****************************************************************************/
/*! Read 1 DWord (Full Address Range) other than xPEC DRAM
* \description
*   Reads one dword from netX5 serial DPM in full address range mode.
*   This function shall be used for all accesses to other areas than the xPEC DRAM area.
* \class 
*   SPI_DPM 
* \params0x
*,    ulDpmAddr      [in]  DPM address to read from
* \return
*   Read value                                                               */
/*****************************************************************************/
LINK_TO_TCM_ATTRIBUTE
unsigned long NX50_SerDPM_Read32_nonXPEC_FullRange(unsigned long ulDpmAddr)
{
  unsigned long  ulData;
  volatile int   iIrq;

  /* lock irq */
  NX50_lock_irqfiq_save(iIrq);
 
  /*** read data byte ***/

  /* assert chip select */
  s_ptSpi->aulSpi_cr[1] |= (MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
  
  s_ptSpi->ulSpi_dr = (ulDpmAddr >> 16) & 0xff;            /* send address */
  s_ptSpi->ulSpi_dr = (ulDpmAddr >> 8) & 0xff;        
  s_ptSpi->ulSpi_dr = ulDpmAddr & 0xff;
  s_ptSpi->ulSpi_dr = 0x80 + 4;                            /* read one dword */
  s_ptSpi->ulSpi_dr = 0x00;                                /* read transfer */
  s_ptSpi->ulSpi_dr = 0x00;                                /* read transfer */
  s_ptSpi->ulSpi_dr = 0x00;                                /* read transfer */
  s_ptSpi->ulSpi_dr = 0x00;                                /* read transfer */

  /* wait for transfer to be complete */
  while  (s_ptSpi->ulSpi_sr & MSK_NX50_spi_sr_BSY);        

  /* deassert chip select */
  s_ptSpi->aulSpi_cr[1] &= ~(MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);

  /* read dummy data */
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;

  /* read data byte */
  ulData = (s_ptSpi->ulSpi_dr & 0xff);
  ulData |= (s_ptSpi->ulSpi_dr & 0xff) << 8;
  ulData |= (s_ptSpi->ulSpi_dr & 0xff) << 16;
  ulData |= (s_ptSpi->ulSpi_dr & 0xff) << 24;
  
  /* unlock irq */
  NX50_lock_irqfiq_restore(iIrq);
  
  return ulData;
}



/*****************************************************************************/
/*! Write 1 DWord (Full Address Range) to xPEC DRAM
* \description
*   Writes one dword to netX5 serial DPM in full address range mode.
*   This function shall be used for all accesses to the xPEC DRAM area.
* \class 
*   SPI_DPM 
* \params
*   ulDpmAddr      [in]  DPM address to read from
*   ulData         [in]  Data to write
* \return
*                                                                            */
/*****************************************************************************/
LINK_TO_TCM_ATTRIBUTE
void NX50_SerDPM_Write32_XPEC_FullRange(unsigned long ulDpmAddr, unsigned long ulData)
{
  unsigned long  ulStatus;
  volatile int iIrq;

  /* lock irq */
  NX50_lock_irqfiq_save(iIrq);

  /*** write data byte ***/

  /* assert chip select */
  s_ptSpi->aulSpi_cr[1] |= (MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
  
  s_ptSpi->ulSpi_dr = (ulDpmAddr >> 16) & 0xff;            /* send address */
  s_ptSpi->ulSpi_dr = (ulDpmAddr >> 8) & 0xff;        
  s_ptSpi->ulSpi_dr = ulDpmAddr & 0xff;
  s_ptSpi->ulSpi_dr = 4;                                   /* write one dword */
  s_ptSpi->ulSpi_dr = ulData & 0xff;                       /* write transfer */
  s_ptSpi->ulSpi_dr = (ulData >> 8) & 0xff;               
  s_ptSpi->ulSpi_dr = (ulData >> 16) & 0xff;               
  s_ptSpi->ulSpi_dr = (ulData >> 24) & 0xff;               

  /* wait for transfer to be complete */
  while  (s_ptSpi->ulSpi_sr & MSK_NX50_spi_sr_BSY);        

  /* deassert chip select */
  s_ptSpi->aulSpi_cr[1] &= ~(MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);

  /* read dummy data */
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;

  do
  {
    /*** read dummy byte ***/
    
    /* assert chip select */
    s_ptSpi->aulSpi_cr[1] |= (MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
    
    s_ptSpi->ulSpi_dr = (DUMMY_READ_ADDR >> 16) & 0xff;            /* send address */
    s_ptSpi->ulSpi_dr = (DUMMY_READ_ADDR >> 8) & 0xff;        
    s_ptSpi->ulSpi_dr = DUMMY_READ_ADDR & 0xff;
    s_ptSpi->ulSpi_dr = 0x80 + 1;                            /* read one dword */
    s_ptSpi->ulSpi_dr = 0x00;                                /* read transfer */

    /* wait for transfer to be complete */
    while  (s_ptSpi->ulSpi_sr & MSK_NX50_spi_sr_BSY);        
  
    /* deassert chip select */
    s_ptSpi->aulSpi_cr[1] &= ~(MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
  
    /* read dummy data */
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
  
    /*** check if access was correct ***/
  
    /* assert chip select */
    s_ptSpi->aulSpi_cr[1] |= (MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
    
    s_ptSpi->ulSpi_dr = (REL_Adr_NX5_dpm_status_err_auto_reset >> 16) & 0xff;  /* send address */
    s_ptSpi->ulSpi_dr = (REL_Adr_NX5_dpm_status_err_auto_reset >> 8) & 0xff;   /* send address */
    s_ptSpi->ulSpi_dr = REL_Adr_NX5_dpm_status_err_auto_reset & 0xff;
    s_ptSpi->ulSpi_dr = 0x80 + 1;           /* read one byte */
    s_ptSpi->ulSpi_dr = 0x00;               /* read transfer */

    /* wait for transfer to be complete */
    while  (s_ptSpi->ulSpi_sr & MSK_NX50_spi_sr_BSY);        
  
    /* deassert chip select */
    s_ptSpi->aulSpi_cr[1] &= ~(MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
  
    /* read dummy data */
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
  
    /* read data byte */
    ulStatus = (s_ptSpi->ulSpi_dr & 0xff);

  } while (ulStatus != 0);

  /* unlock irq */
  NX50_lock_irqfiq_restore(iIrq);
}


/*****************************************************************************/
/*! Write 1 DWord (Full Address Range) other than xPEC DRAM
* \description
*   Writes one dword to netX5 serial DPM in full address range mode.
*   This function shall be used for all accesses to other areas than the xPEC DRAM area.
* \class 
*   SPI_DPM 
* \params
*   ulDpmAddr      [in]  DPM address to read from
*   ulData         [in]  Data to write
* \return
*                                                                            */
/*****************************************************************************/
LINK_TO_TCM_ATTRIBUTE
void NX50_SerDPM_Write32_nonXPEC_FullRange(unsigned long ulDpmAddr, unsigned long ulData)
{
  volatile int iIrq;

  /* lock irq */
  NX50_lock_irqfiq_save(iIrq);

  /*** write data byte ***/

  /* assert chip select */
  s_ptSpi->aulSpi_cr[1] |= (MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
  
  s_ptSpi->ulSpi_dr = (ulDpmAddr >> 16) & 0xff;            /* send address */
  s_ptSpi->ulSpi_dr = (ulDpmAddr >> 8) & 0xff;        
  s_ptSpi->ulSpi_dr = ulDpmAddr & 0xff;
  s_ptSpi->ulSpi_dr = 4;                                   /* write four byte */
  s_ptSpi->ulSpi_dr = ulData & 0xff;                       /* write transfer */
  s_ptSpi->ulSpi_dr = (ulData >> 8) & 0xff;               
  s_ptSpi->ulSpi_dr = (ulData >> 16) & 0xff;               
  s_ptSpi->ulSpi_dr = (ulData >> 24) & 0xff;               

  /* wait for transfer to be complete */
  while  (s_ptSpi->ulSpi_sr & MSK_NX50_spi_sr_BSY);        

  /* deassert chip select */
  s_ptSpi->aulSpi_cr[1] &= ~(MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);

  /* read dummy data */
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
    
  /* unlock irq */
  NX50_lock_irqfiq_restore(iIrq);
}



/*****************************************************************************/
/*! Read 1 Byte (Full Address Range) from xPEC DRAM
* \description
*   Reads one byte from netX5 serial DPM in full address range mode.
*   This function shall be used for all accesses to the xPEC DRAM area.
* \class 
*   SPI_DPM 
* \params
*   ulDpmAddr      [in]  DPM address to read from
* \return
*   Read value                                                               */
/*****************************************************************************/
LINK_TO_TCM_ATTRIBUTE
unsigned char NX50_SerDPM_Read8_XPEC_FullRange(unsigned long ulDpmAddr)
{
  /* Read Byte from netX5 DPM in SPI mode, low range adressing (2 address header bytes) */
  unsigned long  ulData;
  unsigned long  ulStatus;
  volatile int iIrq;

  /* lock irq */
  NX50_lock_irqfiq_save(iIrq);

  /* read as long as dpm_status is not correct */
  do
  {
    /*** read data byte ***/

    /* assert chip select */
    s_ptSpi->aulSpi_cr[1] |= (MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
    
    s_ptSpi->ulSpi_dr = (ulDpmAddr >> 16) & 0xff;            /* send address */
    s_ptSpi->ulSpi_dr = (ulDpmAddr >> 8) & 0xff;        
    s_ptSpi->ulSpi_dr = ulDpmAddr & 0xff;
    s_ptSpi->ulSpi_dr = 0x80 + 1;                            /* read one byte */
    s_ptSpi->ulSpi_dr = 0x00;                                /* read transfer */

    /* wait for transfer to be complete */
    while  (s_ptSpi->ulSpi_sr & MSK_NX50_spi_sr_BSY);        
  
    /* deassert chip select */
    s_ptSpi->aulSpi_cr[1] &= ~(MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
  
    /* read dummy data */
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
  
    /* read data byte */
    ulData = (s_ptSpi->ulSpi_dr & 0xff);    
    
    
    /*** check if access was correct ***/
  
    /* assert chip select */
    s_ptSpi->aulSpi_cr[1] |= (MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
    
    s_ptSpi->ulSpi_dr = (REL_Adr_NX5_dpm_status_err_auto_reset >> 16) & 0xff;  /* send address */
    s_ptSpi->ulSpi_dr = (REL_Adr_NX5_dpm_status_err_auto_reset >> 8) & 0xff; 
    s_ptSpi->ulSpi_dr = REL_Adr_NX5_dpm_status_err_auto_reset & 0xff;
    s_ptSpi->ulSpi_dr = 0x80 + 1;           /* read one byte */
    s_ptSpi->ulSpi_dr = 0x00;               /* read transfer */

    /* wait for transfer to be complete */
    while  (s_ptSpi->ulSpi_sr & MSK_NX50_spi_sr_BSY);        
  
    /* deassert chip select */
    s_ptSpi->aulSpi_cr[1] &= ~(MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
  
    /* read dummy data */
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
  
    /* read data byte */
    ulStatus = (s_ptSpi->ulSpi_dr & 0xff);

  } while (ulStatus != 0);
     

  /* unlock irq */
  NX50_lock_irqfiq_restore(iIrq);
  
  return ulData;
}


/*****************************************************************************/
/*! Read 1 Byte (Full Address Range) other than xPEC DRAM
* \description
*   Reads one byte from netX5 serial DPM in full address range mode.
*   This function shall be used for all accesses to other areas than the xPEC DRAM area.
* \class 
*   SPI_DPM 
* \params
*   ulDpmAddr      [in]  DPM address to read from
* \return
*   Read value                                                               */
/*****************************************************************************/
LINK_TO_TCM_ATTRIBUTE
unsigned char NX50_SerDPM_Read8_nonXPEC_FullRange(unsigned long ulDpmAddr)
{
  unsigned long  abData[4];
  volatile int iIrq;

  /* lock irq */
  NX50_lock_irqfiq_save(iIrq);

  /*** read data byte ***/

  /* assert chip select */
  s_ptSpi->aulSpi_cr[1] |= (MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
  
  s_ptSpi->ulSpi_dr = (ulDpmAddr >> 16) & 0xff;            /* send address */
  s_ptSpi->ulSpi_dr = (ulDpmAddr >> 8) & 0xff;        
  s_ptSpi->ulSpi_dr = ulDpmAddr & 0xff;
  s_ptSpi->ulSpi_dr = 0x80 + 4;                            /* read whole dword */
  s_ptSpi->ulSpi_dr = 0x00;                                /* read transfer */
  s_ptSpi->ulSpi_dr = 0x00;                                /* read transfer */
  s_ptSpi->ulSpi_dr = 0x00;                                /* read transfer */
  s_ptSpi->ulSpi_dr = 0x00;                                /* read transfer */

  /* wait for transfer to be complete */
  while  (s_ptSpi->ulSpi_sr & MSK_NX50_spi_sr_BSY);        

  /* deassert chip select */
  s_ptSpi->aulSpi_cr[1] &= ~(MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);

  /* read dummy data */
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;

  /* read data byte */
  abData[0] = (s_ptSpi->ulSpi_dr & 0xff);    
  abData[1] = (s_ptSpi->ulSpi_dr & 0xff);    
  abData[2] = (s_ptSpi->ulSpi_dr & 0xff);    
  abData[3] = (s_ptSpi->ulSpi_dr & 0xff);    
    
  /* unlock irq */
  NX50_lock_irqfiq_restore(iIrq);
  
  return abData[ulDpmAddr & 0x3];
}



/*****************************************************************************/
/*! Write 1 Byte (Full Address Range) from xPEC DRAM
* \description
*   Writes one byte to netX5 serial DPM in full address range mode.
*   This function shall be used for all accesses to the xPEC DRAM area.
* \class 
*   SPI_DPM 
* \params
*   ulDpmAddr      [in]  DPM address to read from
*   ulData         [in]  Data to write
* \return
*                                                                            */
/*****************************************************************************/
LINK_TO_TCM_ATTRIBUTE
void NX50_SerDPM_Write8_XPEC_FullRange(unsigned long ulDpmAddr, unsigned char bData)
{
  unsigned long  ulStatus;
  volatile int   iIrq;

  /* lock irq */
  NX50_lock_irqfiq_save(iIrq);

  /*** write data byte ***/

  /* assert chip select */
  s_ptSpi->aulSpi_cr[1] |= (MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
  
  s_ptSpi->ulSpi_dr = (ulDpmAddr >> 16) & 0xff;            /* send address */
  s_ptSpi->ulSpi_dr = (ulDpmAddr >> 8) & 0xff;        
  s_ptSpi->ulSpi_dr = ulDpmAddr & 0xff;
  s_ptSpi->ulSpi_dr = 1;                                   /* write one byte */
  s_ptSpi->ulSpi_dr = bData;                               /* write transfer */

  /* wait for transfer to be complete */
  while  (s_ptSpi->ulSpi_sr & MSK_NX50_spi_sr_BSY);        

  /* deassert chip select */
  s_ptSpi->aulSpi_cr[1] &= ~(MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);

  /* read dummy data */
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;

  do
  {
    /*** read dummy byte ***/
    
    /* assert chip select */
    s_ptSpi->aulSpi_cr[1] |= (MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
    
    s_ptSpi->ulSpi_dr = (DUMMY_READ_ADDR >> 16) & 0xff;            /* send address */
    s_ptSpi->ulSpi_dr = (DUMMY_READ_ADDR >> 8) & 0xff;        
    s_ptSpi->ulSpi_dr = DUMMY_READ_ADDR & 0xff;
    s_ptSpi->ulSpi_dr = 0x80 + 1;                            /* read one dword */
    s_ptSpi->ulSpi_dr = 0x00;                                /* read transfer */

    /* wait for transfer to be complete */
    while  (s_ptSpi->ulSpi_sr & MSK_NX50_spi_sr_BSY);        
  
    /* deassert chip select */
    s_ptSpi->aulSpi_cr[1] &= ~(MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
  
    /* read dummy data */
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;

    /*** check if access was correct ***/
  
    /* assert chip select */
    s_ptSpi->aulSpi_cr[1] |= (MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
    
    s_ptSpi->ulSpi_dr = (REL_Adr_NX5_dpm_status_err_auto_reset >> 16) & 0xff;  /* send address */
    s_ptSpi->ulSpi_dr = (REL_Adr_NX5_dpm_status_err_auto_reset >> 8) & 0xff;   /* send address */
    s_ptSpi->ulSpi_dr = REL_Adr_NX5_dpm_status_err_auto_reset & 0xff;
    s_ptSpi->ulSpi_dr = 0x80 + 1;           /* read one byte */
    s_ptSpi->ulSpi_dr = 0x00;               /* read transfer */

    /* wait for transfer to be complete */
    while  (s_ptSpi->ulSpi_sr & MSK_NX50_spi_sr_BSY);        
  
    /* deassert chip select */
    s_ptSpi->aulSpi_cr[1] &= ~(MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
  
    /* read dummy data */
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
    s_ptSpi->ulSpi_dr;
  
    /* read data byte */
    ulStatus = (s_ptSpi->ulSpi_dr & 0xff);
    
  } while (ulStatus != 0);

  /* unlock irq */
  NX50_lock_irqfiq_restore(iIrq);
}


/*****************************************************************************/
/*! Write 1 Byte (Full Address Range) other than xPEC DRAM
* \description
*   Writes one byte to netX5 serial DPM in full address range mode.
*   This function shall be used for all accesses to other areas than the xPEC DRAM area.
* \class 
*   SPI_DPM 
* \params
*   ulDpmAddr      [in]  DPM address to read from
*   ulData         [in]  Data to write
* \return
*                                                                            */
/*****************************************************************************/
LINK_TO_TCM_ATTRIBUTE
void NX50_SerDPM_Write8_nonXPEC_FullRange(unsigned long ulDpmAddr, unsigned char bData)
{
  volatile int   iIrq;

  /* lock irq */
  NX50_lock_irqfiq_save(iIrq);

  /*** write data byte ***/

  /* assert chip select */
  s_ptSpi->aulSpi_cr[1] |= (MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);
  
  s_ptSpi->ulSpi_dr = (ulDpmAddr >> 16) & 0xff;            /* send address */
  s_ptSpi->ulSpi_dr = (ulDpmAddr >> 8) & 0xff;        
  s_ptSpi->ulSpi_dr = ulDpmAddr & 0xff;
  s_ptSpi->ulSpi_dr = 1;                                   /* write one byte */
  s_ptSpi->ulSpi_dr = bData;                               /* write transfer */

  /* wait for transfer to be complete */
  while  (s_ptSpi->ulSpi_sr & MSK_NX50_spi_sr_BSY);        

  /* deassert chip select */
  s_ptSpi->aulSpi_cr[1] &= ~(MSK_CHIP_SELECT << SRT_NX50_spi_cr1_fss);

  /* read dummy data */
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;
  s_ptSpi->ulSpi_dr;

  /* unlock irq */
  NX50_lock_irqfiq_restore(iIrq);
}

#endif /* def NX5_SERIAL_DPM */



#ifdef NX5_PARALLEL_DPM

/*****************************************************************************/
/*! Write 1 DWord (Full Address Range) to xPEC DRAM
* \description
*   Writes one dword to netX5 parallel DPM in full address range mode.
*   This function shall be used for all accesses to the xPEC DRAM area.
* \class 
*   DPM 
* \params
*   ulDpmAddr      [in]  DPM address to read from
*   ulData         [in]  Data to write
* \return
*                                                                            */
/*****************************************************************************/
LINK_TO_TCM_ATTRIBUTE
void NX50_ParDPM_Write32_XPEC_FullRange(unsigned long ulDpmAddr, unsigned long ulData)
{
  volatile int iIrq;
  unsigned long ulStatus;
  
  /* lock irq */
  NX50_lock_irqfiq_save(iIrq);
  
 /* write value */
 (*(volatile unsigned long *)ulDpmAddr) = ulData;
    
  do {
    /* dummy read */
    (*(volatile unsigned char *) DUMMY_READ_ADDR);
    
    /* read DPM status */
    ulStatus = (*(volatile unsigned char *)(HOST_DPM_START_ADDR + REL_Adr_NX5_dpm_status_err_auto_reset));

    /* retry if failed */
  } while( ulStatus != 0 );
  
  /* unlock irq */
  NX50_lock_irqfiq_restore(iIrq);
}
    

/*****************************************************************************/
/*! Read 1 DWord (Full Address Range) from xPEC DRAM
* \description
*   Reads one dword from netX5 parallel DPM in full address range mode.
*   This function shall be used for all accesses to the xPEC DRAM area.
* \class 
*   DPM 
* \params
*   ulDpmAddr      [in]  DPM address to read from
* \return
*   Read value                                                               */
/*****************************************************************************/
LINK_TO_TCM_ATTRIBUTE
unsigned long NX50_ParDPM_Read32_XPEC_FullRange(unsigned long ulDpmAddr)
{
  int iIrq;
  unsigned long ulStatus, ulData;
  
  /* lock irq */
  NX50_lock_irqfiq_save(iIrq);

  do {
    /* read access */
    ulData = (*(volatile unsigned long *)(ulDpmAddr));
  
    /* read DPM status */
    ulStatus = (*(volatile unsigned char *)(HOST_DPM_START_ADDR + REL_Adr_NX5_dpm_status_err_auto_reset));
    
    /* retry if failed */
  } while( ulStatus != 0 );
  
  /* unlock irq */
  /*lint -e{522} yes, this function has side effects */
  NX50_lock_irqfiq_restore(iIrq);
  
  return ulData;
}

/*****************************************************************************/
/*! Write 1 Word (Full Address Range) to xPEC DRAM
* \description
*   Writes one word to netX5 parallel DPM in full address range mode.
*   This function shall be used for all accesses to the xPEC DRAM area.
* \class 
*   DPM 
* \params
*   ulDpmAddr      [in]  DPM address to read from
*   usData         [in]  Data to write
* \return
*                                                                            */
/*****************************************************************************/
LINK_TO_TCM_ATTRIBUTE
void NX50_ParDPM_Write16_XPEC_FullRange(unsigned long ulDpmAddr, unsigned short usData)
{
  volatile int iIrq;
  unsigned long ulStatus;
  
  /* lock irq */
  NX50_lock_irqfiq_save(iIrq);
  
  /* write value */
  (*(volatile unsigned short *)ulDpmAddr) = usData;
    
  do {    
    /* dummy read */
    (*(volatile unsigned char *) DUMMY_READ_ADDR);
   
    /* read DPM status */
    ulStatus = (*(volatile unsigned char *)(HOST_DPM_START_ADDR + REL_Adr_NX5_dpm_status_err_auto_reset));
    
    /* retry if failed */
  } while( ulStatus != 0 );
  
  /* unlock irq */
  NX50_lock_irqfiq_restore(iIrq);
  
}

/*****************************************************************************/
/*! Read 1 Word (Full Address Range) from xPEC DRAM
* \description
*   Reads one word from netX5 parallel DPM in full address range mode.
*   This function shall be used for all accesses to the xPEC DRAM area.
* \class 
*   DPM 
* \params
*   ulDpmAddr      [in]  DPM address to read from
* \return
*   Read value                                                               */
/*****************************************************************************/
LINK_TO_TCM_ATTRIBUTE
unsigned short NX50_ParDPM_Read16_XPEC_FullRange(unsigned long ulDpmAddr)
{
  volatile int iIrq;
  unsigned long ulStatus;
  unsigned short usData;
  
  /* lock irq */
  NX50_lock_irqfiq_save(iIrq);
  
  do {
    /* read access */
    usData = (*(volatile unsigned short *)(ulDpmAddr));
  
    /* read DPM status */
    ulStatus = (*(volatile unsigned char *)(HOST_DPM_START_ADDR + REL_Adr_NX5_dpm_status_err_auto_reset));

    /* retry if failed */
  } while( ulStatus != 0 );
  
  /* unlock irq */
  NX50_lock_irqfiq_restore(iIrq);
  
  return usData;
}

/*****************************************************************************/
/*! Write 1 Byte (Full Address Range) to xPEC DRAM
* \description
*   Writes one byte to netX5 parallel DPM in full address range mode.
*   This function shall be used for all accesses to the xPEC DRAM area.
* \class 
*   DPM 
* \params
*   ulDpmAddr      [in]  DPM address to read from
*   ulData         [in]  Data to write
* \return
*                                                                            */
/*****************************************************************************/
LINK_TO_TCM_ATTRIBUTE
void NX50_ParDPM_Write8_XPEC_FullRange(unsigned long ulDpmAddr, unsigned char bData)
{
  volatile int iIrq;
  unsigned long ulStatus;
  
  /* lock irq */
  NX50_lock_irqfiq_save(iIrq);
  
  /* write value */
  (*(volatile unsigned char *)ulDpmAddr) = bData;
    
  /* loop until success */
  do {
    /* dummy read */
    (*(volatile unsigned char *) DUMMY_READ_ADDR);
    
    /* read DPM status */
    ulStatus = (*(volatile unsigned char *)(HOST_DPM_START_ADDR + REL_Adr_NX5_dpm_status_err_auto_reset));
    
    /* retry if failed */
  } while( ulStatus != 0 );
  
  /* unlock irq */
  NX50_lock_irqfiq_restore(iIrq);
}

/*****************************************************************************/
/*! Read 1 Byte (Full Address Range) from xPEC DRAM
* \description
*   Reads one byte from netX5 parallel DPM in full address range mode.
*   This function shall be used for all accesses to the xPEC DRAM area.
* \class 
*   DPM 
* \params
*   ulDpmAddr      [in]  DPM address to read from
* \return
*   Read value                                                               */
/*****************************************************************************/
LINK_TO_TCM_ATTRIBUTE
unsigned char NX50_ParDPM_Read8_XPEC_FullRange(unsigned long ulDpmAddr)
{
  volatile int iIrq;
  unsigned long ulStatus, bData;
  
  /* lock irq */
  NX50_lock_irqfiq_save(iIrq);
  
  do {
    /* read access */
    bData = (*(volatile unsigned char *)(ulDpmAddr));
  
    /* read DPM status */
    ulStatus = (*(volatile unsigned char *)(HOST_DPM_START_ADDR + REL_Adr_NX5_dpm_status_err_auto_reset));
    
    /* retry if failed */
  } while( ulStatus != 0 );
  
  /* unlock irq */
  NX50_lock_irqfiq_restore(iIrq);
  
  return bData;
}

#endif /* def NX5_PARALLEL_DPM */
