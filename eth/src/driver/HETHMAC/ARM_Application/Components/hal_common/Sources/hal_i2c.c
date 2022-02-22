/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/

#include "hal_i2c.h"
#include "hw_defines.h"

#define MSK_I2C_MFIFO_ERR (HW_MSK(i2c_sr_mfifo_err_undr)|HW_MSK(i2c_sr_mfifo_err_ovfl))

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/

HW_PTR_I2C(s_aptI2c)

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! I2C Initialize
* \description
*   This function resets and configures an I2C instance.
* \class
*   I2C
* \params
*   uInst           [in]  Instance number
*   eSpeed          [in]  Clock speed
* \return
*   I2C_OKAY
*   I2C_ERR_INVAL_PARAM                                               */
/*****************************************************************************/
#ifndef HALDEF
#define HALDEF(name) name
#endif

I2C_RESULT_E HALDEF(I2C_Init)( unsigned int uInst, I2C_SPEED_E eSpeed )
{
  /* Disable the unit */
  s_aptI2c[uInst]->ulI2c_mcr = HW_MSK(i2c_mcr_rst_i2c);
  s_aptI2c[uInst]->ulI2c_mcr = 0;

  /* Disable slave mode */
  s_aptI2c[uInst]->ulI2c_scr = 0;

  /* Clear the master FIFO */
  s_aptI2c[uInst]->ulI2c_mfifo_cr = HW_MSK(i2c_mfifo_cr_mfifo_clr);
  s_aptI2c[uInst]->ulI2c_mfifo_cr = 0;

  /* Clear the slave FIFO */
  s_aptI2c[uInst]->ulI2c_sfifo_cr = HW_MSK(i2c_sfifo_cr_sfifo_clr);
  s_aptI2c[uInst]->ulI2c_sfifo_cr = 0;

  /* Disable IRQs */
  s_aptI2c[uInst]->ulI2c_irqmsk = 0;

  /* Enable DMA requests */
  s_aptI2c[uInst]->ulI2c_dmacr = HW_MSK(i2c_dmacr_mdmab_en)
                                |HW_MSK(i2c_dmacr_mdmas_en)
                                |HW_MSK(i2c_dmacr_sdmab_en)
                                |HW_MSK(i2c_dmacr_sdmas_en);

  /* Enable I2C core */
  s_aptI2c[uInst]->ulI2c_mcr = ((uint32_t)eSpeed << HW_SRT(i2c_mcr_mode))
                              |HW_MSK(i2c_mcr_en_i2c)
                              |HW_MSK(i2c_mcr_en_timeout);

  return I2C_OKAY;
}

/*****************************************************************************/
/*! I2C Master Start
* \description
*   This function generates a START condition on the bus
*   and optionally prepares a transfer.
* \class
*   I2C
* \params
*   uInst           [in]  Instance number
*   uSlaveAdr       [in]  Slave address
*   uiNumBytes      [in]  Length in bytes of the transfer excluding
*                         the first byte (slave address)
*   fRead           [in]  false/true: write/read transfer                    */
/*****************************************************************************/
void HALDEF(I2C_MStart)( unsigned int uInst, unsigned int uSlaveAdr, unsigned int uiNumBytes, bool fRead )
{
  uint32_t ulVal, ulSadr;

  /* check if slave address must be set */
  ulVal  = s_aptI2c[uInst]->ulI2c_mcr;
  ulSadr = uSlaveAdr << HW_SRT(i2c_mcr_sadr);
  if( ulSadr != (ulVal & HW_MSK(i2c_mcr_sadr)) )
  {
    /* set slave address */
    ulVal &= ~HW_MSK(i2c_mcr_sadr);
    ulVal |= ulSadr;
    s_aptI2c[uInst]->ulI2c_mcr = ulVal;
  }

  ulVal = fRead ? HW_MSK(i2c_cmd_nwr) : 0;

  if( 0 == uiNumBytes )
  {
    /* send slave address byte only */
    ulVal |= I2C_CMD_S_AC << HW_SRT(i2c_cmd_cmd);
  }
  else
  {
    /* FIFO transfer */
    ulVal |= I2C_CMD_S_AC_T << HW_SRT(i2c_cmd_cmd);
    ulVal |= (uiNumBytes - 1) << HW_SRT(i2c_cmd_tsize);
  }

  s_aptI2c[uInst]->ulI2c_cmd = ulVal;
}

/*****************************************************************************/
/*! I2C Master Write
* \description
*   Passes the data to be transmitted to the master FIFO.
* \class
*   I2C
* \params
*   uInst           [in]  Instance number
*   pbData          [in]  Data pointer
*   iMaxBytes       [in]  Length in bytes of the data available at pbData
* \return
*   Number of bytes successfully buffered for transmission or -1 on errors   */
/*****************************************************************************/
int HALDEF(I2C_MWr)( unsigned int uInst, const uint8_t* pbData, int iMaxBytes )
{
  int iCnt;
  uint32_t ulVal;

  ulVal = s_aptI2c[uInst]->ulI2c_sr;

  if( 0 != (ulVal & MSK_I2C_MFIFO_ERR) )
  {
    s_aptI2c[uInst]->ulI2c_mfifo_cr = HW_MSK(i2c_mfifo_cr_mfifo_clr);
    s_aptI2c[uInst]->ulI2c_mfifo_cr = 0;
    return -1;
  }

  /* limit number of bytes to FIFO capacity */
  ulVal = (ulVal & HW_MSK(i2c_sr_mfifo_level)) >> HW_SRT(i2c_sr_mfifo_level);
  ulVal = 16 - ulVal;
  if( (uint32_t)iMaxBytes > ulVal ) iMaxBytes = (int)ulVal;

  for(iCnt = 0; iCnt < iMaxBytes; ++iCnt)
  {
    s_aptI2c[uInst]->ulI2c_mdr = pbData[iCnt];
  }

  return iMaxBytes;
}

/*****************************************************************************/
/*! I2C Master Read
* \description
*   Receives the data sent by the slave from the master FIFO.
* \class
*   I2C
* \params
*   uInst           [in]  Instance number
*   pbData          [in]  Data pointer
*   iMaxBytes       [in]  Length in bytes of the buffer available at pbData
* \return
*   Number of bytes successfully buffered for transmission or -1 on errors   */
/*****************************************************************************/
int HALDEF(I2C_MRd)( unsigned int uInst, uint8_t* pbData, int iMaxBytes )
{
  int iCnt;
  uint32_t ulVal;

  ulVal = s_aptI2c[uInst]->ulI2c_sr;

  if( 0 != (ulVal & MSK_I2C_MFIFO_ERR) )
  {
    s_aptI2c[uInst]->ulI2c_mfifo_cr = HW_MSK(i2c_mfifo_cr_mfifo_clr);
    s_aptI2c[uInst]->ulI2c_mfifo_cr = 0;
    return -1;
  }

  /* limit number of bytes to FIFO fill level */
  ulVal = (ulVal & HW_MSK(i2c_sr_mfifo_level)) >> HW_SRT(i2c_sr_mfifo_level);
  if( (uint32_t)iMaxBytes > ulVal ) iMaxBytes = (int)ulVal;

  for(iCnt = 0; iCnt < iMaxBytes; ++iCnt)
  {
    ulVal = s_aptI2c[uInst]->ulI2c_mdr;
    pbData[iCnt] = (uint8_t)ulVal;
  }

  return iMaxBytes;
}

/*****************************************************************************/
/*! I2C Master Is Busy
* \description
*   Returns true if a master transfer is in progress.
* \class
*   I2C
* \params
*   uInst           [in]  Instance number
* \return
*   false/true: Master is idle/busy   */
/*****************************************************************************/
bool HALDEF(I2C_MIsBusy)( unsigned int uInst )
{
  uint32_t ulVal = s_aptI2c[uInst]->ulI2c_cmd;
  ulVal &= HW_MSK(i2c_cmd_cmd);

  return ( ulVal != (I2C_CMD_IDLE << HW_SRT(i2c_cmd_cmd)) );
}

/*****************************************************************************/
/*! I2C Get Status
* \description
*   Returns a status code containing the most significant error.
* \class
*   I2C
* \params
*   uInst           [in]  Instance number
* \return
*    0: No errors
*   -1: Master FIFO error
*   -3: ACK not received
*   -5: Bus master lost arbitration
*   -6: Clock stretching timeout  */
/*****************************************************************************/
int HALDEF(I2C_GetStatus)( unsigned int uInst )
{
  uint32_t ulVal = s_aptI2c[uInst]->ulI2c_sr;
  int iRet = 0;

  if( 0 != (ulVal & MSK_I2C_MFIFO_ERR) )
  {
    iRet = -1;
  }

  if( 0 != (ulVal & HW_MSK(i2c_sr_timeout)) )
  {
    iRet = -6;
  }

  if( 0 == (ulVal & HW_MSK(i2c_sr_bus_master)) )
  {
    iRet = -5;
  }

  if( 0 == (ulVal & HW_MSK(i2c_sr_last_ac)) )
  {
    iRet = -3;
  }

  if( iRet )
  {
    s_aptI2c[uInst]->ulI2c_mfifo_cr = HW_MSK(i2c_mfifo_cr_mfifo_clr);
    s_aptI2c[uInst]->ulI2c_mfifo_cr = 0;
  }

  return iRet;
}

/*****************************************************************************/
/*! I2C Master Stop
* \description
*   Generates a STOP condition on the bus.
* \class
*   I2C
* \params
*   uInst           [in]  Instance number                                    */
/*****************************************************************************/
void HALDEF(I2C_MStop)( unsigned int uInst )
{
  s_aptI2c[uInst]->ulI2c_cmd = (I2C_CMD_STOP << HW_SRT(i2c_cmd_cmd));
}

/*****************************************************************************/
/*! I2C Get Parameter
* \description
*   This function reads parameter.
* \class 
*   I2C
* \params
*   ePrmID           [in]  Parameter ID
*   pulPrmVal        [in]  Pointer to parameter value
* \return
*   I2C_OKAY                                                             
*   I2C_ERR_INVAL_PARAM                                               */
/*****************************************************************************/
I2C_RESULT_E HALDEF(I2C_PrmGet)( unsigned int uI2cInst, I2C_PRM_E ePrmID, uint32_t* pulPrmVal )
{

  /* Get the selected parameter */
  switch( ePrmID )
  { 
    case I2C_MCR:        *pulPrmVal = s_aptI2c[uI2cInst]->ulI2c_mcr;         break;        
    case I2C_SCR:        *pulPrmVal = s_aptI2c[uI2cInst]->ulI2c_scr;         break;  
    case I2C_CMD:        *pulPrmVal = s_aptI2c[uI2cInst]->ulI2c_cmd;         break; 
    case I2C_MDR:        *pulPrmVal = s_aptI2c[uI2cInst]->ulI2c_mdr;         break; 
    case I2C_SDR:        *pulPrmVal = s_aptI2c[uI2cInst]->ulI2c_sdr;         break; 
    case I2C_MFIFO_CR:   *pulPrmVal = s_aptI2c[uI2cInst]->ulI2c_mfifo_cr;    break; 
    case I2C_SFIFO_CR:   *pulPrmVal = s_aptI2c[uI2cInst]->ulI2c_sfifo_cr;    break; 
    case I2C_SR:         *pulPrmVal = s_aptI2c[uI2cInst]->ulI2c_sr;          break; 
    case I2C_IRQMSK:     *pulPrmVal = s_aptI2c[uI2cInst]->ulI2c_irqmsk;      break; 
    case I2C_IRQSR:      *pulPrmVal = s_aptI2c[uI2cInst]->ulI2c_irqsr;       break; 
    case I2C_IRQMSKED:   *pulPrmVal = s_aptI2c[uI2cInst]->ulI2c_irqmsked;    break; 
    case I2C_DMACR:      *pulPrmVal = s_aptI2c[uI2cInst]->ulI2c_dmacr;       break; 
    case I2C_PIO:        *pulPrmVal = s_aptI2c[uI2cInst]->ulI2c_pio;         break; 

    /* Default */
    default:
      return I2C_ERR_INVAL_PARAM;
    break;
  } /* end switch */

  return I2C_OKAY;
}

/*****************************************************************************/
/*! I2C Set Parameter
* \description
*   This function reads parameter.
* \class 
*   I2C
* \params
*   ePrmID           [in]  Parameter ID
*   pulPrmVal        [in]  New parameter value
* \return
*   I2C_OKAY                                                             
*   I2C_ERR_INVAL_PARAM                                               */
/*****************************************************************************/
I2C_RESULT_E HALDEF(I2C_PrmSet)( unsigned int uI2cInst, I2C_PRM_E ePrmID, uint32_t ulPrmVal )
{

  /* Set the selected parameter */
  switch( ePrmID )
  { 
    case I2C_MCR:        s_aptI2c[uI2cInst]->ulI2c_mcr = ulPrmVal;         break;        
    case I2C_SCR:        s_aptI2c[uI2cInst]->ulI2c_scr = ulPrmVal;         break;  
    case I2C_CMD:        s_aptI2c[uI2cInst]->ulI2c_cmd = ulPrmVal;         break; 
    case I2C_MDR:        s_aptI2c[uI2cInst]->ulI2c_mdr = ulPrmVal;         break; 
    case I2C_SDR:        s_aptI2c[uI2cInst]->ulI2c_sdr = ulPrmVal;         break; 
    case I2C_MFIFO_CR:   s_aptI2c[uI2cInst]->ulI2c_mfifo_cr = ulPrmVal;    break; 
    case I2C_SFIFO_CR:   s_aptI2c[uI2cInst]->ulI2c_sfifo_cr = ulPrmVal;    break; 
    case I2C_SR:         s_aptI2c[uI2cInst]->ulI2c_sr = ulPrmVal;          break; 
    case I2C_IRQMSK:     s_aptI2c[uI2cInst]->ulI2c_irqmsk = ulPrmVal;      break; 
    case I2C_IRQSR:      s_aptI2c[uI2cInst]->ulI2c_irqsr = ulPrmVal;       break; 
    case I2C_IRQMSKED:   s_aptI2c[uI2cInst]->ulI2c_irqmsked = ulPrmVal;    break; 
    case I2C_DMACR:      s_aptI2c[uI2cInst]->ulI2c_dmacr = ulPrmVal;       break; 
    case I2C_PIO:        s_aptI2c[uI2cInst]->ulI2c_pio = ulPrmVal;         break; 

    /* Default */
    default:
      return I2C_ERR_INVAL_PARAM;
    break;
  } /* end switch */

  return I2C_OKAY;

}
