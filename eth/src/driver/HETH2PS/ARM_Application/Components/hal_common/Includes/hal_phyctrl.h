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

typedef enum PHYCTRL_PRM_Etag
{
  PHYCTRL_PRM_LED_MODE,
  PHYCTRL_PRM_LED_BLINK_INTERVAL_MS,
  PHYCTRL_PRM_LED_OUT_LINE,
  PHYCTRL_PRM_MIIM_PREAMBLE,
  PHYCTRL_PRM_MIIM_MDC_FAST,
} PHYCTRL_PRM_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
int PHYCTRL_Init              ( unsigned int uiPort );
int PHYCTRL_PrmSet            ( unsigned int uiPort, PHYCTRL_PRM_E ePrmID, uint32_t ulPrmVal );
unsigned int PHYCTRL_MiimRead ( unsigned int uiPort, unsigned int uiPhy, unsigned int uiReg );
void PHYCTRL_MiimWrite        ( unsigned int uiPort, unsigned int uiPhy, unsigned int uiReg, unsigned int uiData );
int PHYCTRL_GetLinkState      ( unsigned int uiPort, unsigned int* puiLinkUp, unsigned int* puiSpeed, unsigned int* puiIsFullDuplex);
int PHYCTRL_GetEldStatus      ( unsigned int uiPort, bool* pfBadLink, unsigned int* puiCounter );
int PHYCTRL_ConfigEld         ( unsigned int uiPort, unsigned int uiAdd, unsigned int uiSub, unsigned int uiTreshold );

#endif /* #ifndef __HAL_PHYCTRL_H */
