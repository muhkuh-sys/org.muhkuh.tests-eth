#ifndef __HAL_I2C_H
#define __HAL_I2C_H

/*****************************************************************************/
/* Includes                                                                  */
/*****************************************************************************/

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>

/*****************************************************************************/
/* Result codes for functions                                                */
/*****************************************************************************/

typedef enum I2C_RESULT_Etag {
  I2C_OKAY               = 0,    /**< Successful */
  I2C_ERR_INVAL_PARAM    = 1     /**< Invalid parameter */
} I2C_RESULT_E;

/*****************************************************************************/
/* Speed                                                                     */
/*****************************************************************************/

typedef enum {
  I2C_SPEED_50     = 0,    /**< Fast/Standard-mode, 50kbit/s */
  I2C_SPEED_100    = 1,    /**< Fast/Standard-mode, 100kbit/s */
  I2C_SPEED_200    = 2,    /**< Fast/Standard-mode, 200kbit/s */
  I2C_SPEED_400    = 3,    /**< Fast/Standard-mode, 400kbit/s */
  I2C_SPEED_800    = 4,    /**< Fast/Standard-mode, 800kbit/s */
  I2C_SPEED_1200   = 5,    /**< Fast/Standard-mode, 1.2Mbit/s */
  I2C_SPEED_1700   = 6,    /**< High-speed-mode, 1.7Mbit/s */
  I2C_SPEED_3400   = 7     /**< High-speed-mode, 3.4Mbit/s */
} I2C_SPEED_E;

/*****************************************************************************/
/* Commands                                                                  */
/*****************************************************************************/

typedef enum {
  I2C_CMD_START    = 0,    /**< Generate (r)START-condition. */
  I2C_CMD_S_AC     = 1,    /**< Acknowledge-polling: generate up to acpollmax+1 START-sequences (until acknowledged by slave). */
  I2C_CMD_S_AC_T   = 2,    /**< Run S_AC, then transfer tsize+1 bytes from/to master FIFO. Not to be continued. */
  I2C_CMD_S_AC_TC  = 3,    /**< Run S_AC, then transfer tsize+1 bytes from/to master FIFO. To be continued. */
  I2C_CMD_CT       = 4,    /**< Continued transfer not to be continued. */
  I2C_CMD_CTC      = 5,    /**< Continued transfer to be continued. */
  I2C_CMD_STOP     = 6,    /**< Generate STOP-condition. */
  I2C_CMD_IDLE     = 7     /**< Nothing to do, last command finished, break current command. */
} I2C_CMD_E;

/*****************************************************************************/
/* Parameters                                                                */
/*****************************************************************************/

typedef enum I2C_PRM_Etag {
  I2C_MCR        = 0,       /**< Register ulI2c_mcr */
  I2C_SCR        = 1,       /**< Register ulI2c_scr */
  I2C_CMD        = 2,       /**< Register ulI2c_cmd */
  I2C_MDR        = 3,       /**< Register ulI2c_mdr */
  I2C_SDR        = 4,       /**< Register ulI2c_sdr */
  I2C_MFIFO_CR   = 5,       /**< Register ulI2c_mfifo_cr */
  I2C_SFIFO_CR   = 6,       /**< Register ulI2c_sfifo_cr */
  I2C_SR         = 7,       /**< Register ulI2c_sr */
  I2C_IRQMSK     = 8,       /**< Register ulI2c_irqmsk */
  I2C_IRQSR      = 9,       /**< Register ulI2c_irqsr */
  I2C_IRQMSKED   = 10,      /**< Register ulI2c_irqmsked */
  I2C_DMACR      = 11,      /**< Register ulI2c_dmacr */
  I2C_PIO        = 12       /**< Register ulI2c_pio */
} I2C_PRM_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

I2C_RESULT_E        I2C_Init( unsigned int uiInst, I2C_SPEED_E eSpeed );

void                I2C_MStart( unsigned int uiInst, unsigned int uiSlaveAdr, unsigned int uiNumBytes, bool fRead );
int                 I2C_MWr( unsigned int uiInst, const uint8_t* pbData, int iMaxBytes );
int                 I2C_MRd( unsigned int uiInst, uint8_t* pbData, int iMaxBytes );
bool                I2C_MIsBusy( unsigned int uiInst );
void                I2C_MStop( unsigned int uiInst );

int                 I2C_GetStatus( unsigned int uiInst );

I2C_RESULT_E        I2C_PrmGet( unsigned int uiInst, I2C_PRM_E ePrmID, uint32_t* pulPrmVal );
I2C_RESULT_E        I2C_PrmSet( unsigned int uiInst, I2C_PRM_E ePrmID, uint32_t ulPrmVal );

#endif  /* HAL_I2C_H */
