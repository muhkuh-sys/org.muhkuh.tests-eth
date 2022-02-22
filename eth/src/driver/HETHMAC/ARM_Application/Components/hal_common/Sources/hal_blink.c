/* Includes */
#include "hal_blink.h"
#include "hw_defines.h"

/*  Variables */
HW_PTR_BLINK(s_ptBlink)

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! BLINK Configure Channel
* \description
*   This function configures a BLINK channel.
* \class
*   BLINK
* \params
*   uInst       [in] instance
*   uiChannel   [in] channel
*   bPeriod10ms [in] period 0..255 is 10ms..2560ms
*   ulSeq       [in] blink pattern
* \return
*   0
*  -1                                                                        */
/*****************************************************************************/

#ifndef HALDEF
#define HALDEF(name) name
#endif

int HALDEF(BLINK_ConfigureChannel) (unsigned int uInst, unsigned int uiChannel, unsigned int uiPeriod10ms, unsigned int uiBlinkLen, uint32_t ulSeq)
{
  uint32_t ulVal = (uint32_t)(uiPeriod10ms - 1)
                  |(((uint32_t)(uiBlinkLen) - 1) << 8);

  switch(uiChannel) {
  case 0:
    s_ptBlink->ulBlink_config0 = ulVal;
    s_ptBlink->ulBlink_seq0 = ulSeq;
    break;
  case 1:
    s_ptBlink->ulBlink_config1 = ulVal;
    s_ptBlink->ulBlink_seq1 = ulSeq;
    break;
  case 2:
    s_ptBlink->ulBlink_config2 = ulVal;
    s_ptBlink->ulBlink_seq2 = ulSeq;
    break;
  case 3:
    s_ptBlink->ulBlink_config3 = ulVal;
    s_ptBlink->ulBlink_seq3 = ulSeq;
    break;
  default:
     return -1;
    break;
  }
  return 0;
}

/*****************************************************************************/
/*! BLINK StartStop
* \description
*   This function starts/stops BLINK channels.
* \class 
*   BLINK
* \params
*   uInst      [in] instance
*   fStartStop [in] true: true/false: start/stop
*   fCh0       [in] true: start Channel 0
*   fCh1       [in] true: start Channel 1
*   fCh2       [in] true: start Channel 2
*   fCh3       [in] true: start Channel 3
* \return                                                                    */
/*****************************************************************************/
void HALDEF(BLINK_StartStop) (unsigned int uInst, bool fStartStop, bool fCh0, bool fCh1, bool fCh2, bool fCh3)
{
  uint32_t ulVal = 0;

  if(fStartStop) {
    ulVal =  HW_MSK(blink_enable_en0)
            |HW_MSK(blink_enable_en1)
            |HW_MSK(blink_enable_en2)
            |HW_MSK(blink_enable_en3);
  }

  /* include all channel write masks */
  if(fCh0) ulVal |= HW_MSK(blink_enable_en0_wm);
  if(fCh1) ulVal |= HW_MSK(blink_enable_en1_wm);
  if(fCh2) ulVal |= HW_MSK(blink_enable_en2_wm);
  if(fCh3) ulVal |= HW_MSK(blink_enable_en3_wm);

  s_ptBlink->ulBlink_enable = ulVal;
}

