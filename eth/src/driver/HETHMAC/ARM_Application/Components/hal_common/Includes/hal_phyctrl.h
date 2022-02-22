#ifndef __HAL_PHYCTRL_H
#define __HAL_PHYCTRL_H

#include <stdint.h>   /* ISO C99: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C99: "bool" definition */

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

/*****************************************************************************/
/*! PHY LED Flashing Mode                                                    */
/*****************************************************************************/
typedef enum PHYCTRL_LED_MODE_Etag {
  PHYCTRL_LED_MODE_MANUAL    = 0,
  PHYCTRL_LED_MODE_STATIC    = 1,
  PHYCTRL_LED_MODE_FLASHING  = 2,
  PHYCTRL_LED_MODE_COMBINED  = 3
} PHYCTRL_LED_MODE_E;


/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
#ifndef HALDEC
#define HALDEC(name) name
#endif

uint32_t HALDEC(PHYCTRL_GetLedStatus)     ( unsigned int uXcInst, unsigned int uPhyNum );
uint32_t HALDEC(PHYCTRL_GetLinkState)     ( unsigned int uXcInst, unsigned int uPhyNum, unsigned int* puiLinkUp, unsigned int* puiSpeed, unsigned int* puiIsFullDuplex);
int      HALDEC(PHYCTRL_ConfigLedBlink)   ( unsigned int uXcInst, unsigned int uPhyNum, PHYCTRL_LED_MODE_E eBlinkMode, uint32_t ulBlinkInterval );
int      HALDEC(PHYCTRL_ManualLedSet)     ( unsigned int uXcInst, unsigned int uPhyNum, bool fLed0, bool fLed1 );
int      HALDEC(PHYCTRL_GetEldStatus)     ( unsigned int uXcInst, unsigned int uPhyNum, bool* pfBadLink, unsigned short* pusCounter );
int      HALDEC(PHYCTRL_ConfigEld)        ( unsigned int uXcInst, unsigned int uPhyNum, unsigned char  bAdd, unsigned char  bSub, unsigned short usTreshold );

#endif /* #ifndef __HAL_PHYCTRL_H */
