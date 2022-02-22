/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "hal_canctrl.h"
#include "hw_defines.h"

#if defined(__NETX51)
#define HAL_CANCTRL_CNT 1
#endif

#if defined(__NETX4000)
#define HAL_CANCTRL_CNT 1
#endif

#if defined(__NETX90)
#if defined(CPU_IS_ARM_APP)
#define HAL_CANCTRL_CNT 2
#else
#define HAL_CANCTRL_CNT 0
#endif /* defined(CPU_IS_ARM_APP) */
#endif /* defined(__NETX90) */

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
HW_PTR_CANCTRL(s_aptCanCtrl)

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! Enter Reset Mode
* \description
*   Enters the reset mode of the CAN controller and sets it into PeliCAN mode.
* \class
*   Control Service Class
* \params
*   uiInst            [in]    Instance
* \return
*   CANCTRL_RESULT_OKAY
*   CANCTRL_RESULT_INVAL_PARAM                                               */
/*****************************************************************************/
CANCTRL_RESULT CanCtrl_EnterResetMode( unsigned int uiInst )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return CANCTRL_RESULT_INVAL_PARAM;

  s_aptCanCtrl[uiInst]->ulCanctrl_mode |= HW_MSK(canctrl_mode_reset_mode);
  s_aptCanCtrl[uiInst]->ulCanctrl_mode_control = HW_MSK(canctrl_mode_control_mode);

  return CANCTRL_RESULT_OKAY;
}

/*****************************************************************************/
/*! Leave Reset Mode
* \description
*   Leaves the reset mode of the CAN controller and goes to PeliCAN operating mode.
* \class
*   Control Service Class
* \params
*   uiInst            [in]    Instance
* \return
*   CANCTRL_RESULT_OKAY
*   CANCTRL_RESULT_INVAL_PARAM                                               */
/*****************************************************************************/
CANCTRL_RESULT CanCtrl_LeaveResetMode( unsigned int uiInst )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return CANCTRL_RESULT_INVAL_PARAM;

  s_aptCanCtrl[uiInst]->ulCanctrl_mode_control = HW_MSK(canctrl_mode_control_mode);
  s_aptCanCtrl[uiInst]->ulCanctrl_mode &= ~HW_MSK(canctrl_mode_reset_mode);

  return CANCTRL_RESULT_OKAY;
}

/*****************************************************************************/
/*! Set Baud rate
* \description
*   Configures the CAN controller with a specific speed.
* \class
*   Control Service Class
* \params
*   uiInst            [in]    Instance
*   ulBaudrate        [in]    Baud rate in kBit per second
*   uOversampling     [in]    0/1: Over-Sampling (three-times) is disabled/enabled
* \return
*   CANCTRL_RESULT_OKAY
*   CANCTRL_RESULT_INVAL_PARAM
*   CANCTRL_RESULT_WRONG_MODE                                                */
/*****************************************************************************/
CANCTRL_RESULT CanCtrl_SetBaudrate( unsigned int uiInst, uint32_t ulBaudrate, unsigned int uOversampling )
{
  CANCTRL_RESULT eRslt = CANCTRL_RESULT_OKAY;
  uint32_t ulPrescaler = 0;
  uint32_t ulTseg1 = 0;
  uint32_t ulTseg2 = 0;

  if( uiInst >= HAL_CANCTRL_CNT )
    return CANCTRL_RESULT_INVAL_PARAM;

  if( 0 == (s_aptCanCtrl[uiInst]->ulCanctrl_mode & HW_MSK(canctrl_mode_reset_mode)) )
    return CANCTRL_RESULT_WRONG_MODE;

  switch(ulBaudrate)
  {
  case CANCTRL_BAUDRATE_1000K: ulPrescaler = 10; ulTseg1 = 7; break;
  case CANCTRL_BAUDRATE_800K: ulPrescaler = 25; ulTseg1 = 2; break;
  case CANCTRL_BAUDRATE_500K: ulPrescaler = 20; ulTseg1 = 7; break;
  case CANCTRL_BAUDRATE_250K: ulPrescaler = 40; ulTseg1 = 7; break;
  case CANCTRL_BAUDRATE_125K: ulPrescaler = 80; ulTseg1 = 7; break;
  case CANCTRL_BAUDRATE_50K: ulPrescaler = 200; ulTseg1 = 7; break;
  case CANCTRL_BAUDRATE_20K: ulPrescaler = 500; ulTseg1 = 7; break;
  case CANCTRL_BAUDRATE_10K: ulPrescaler = 500; ulTseg1 = 16; ulTseg2 = 1; break;
  default: eRslt = CANCTRL_RESULT_INVAL_PARAM; break;
  }

  if( eRslt == CANCTRL_RESULT_OKAY )
  {
    s_aptCanCtrl[uiInst]->aulCanctrl_bus_timing[0] = ulPrescaler << HW_SRT(canctrl_bus_timing0_prescaler);
    s_aptCanCtrl[uiInst]->aulCanctrl_bus_timing[1] = (ulTseg1 << HW_SRT(canctrl_bus_timing1_tseg1))
                                           |(ulTseg2 << HW_SRT(canctrl_bus_timing1_tseg2))
                                           |((uint32_t)uOversampling << HW_SRT(canctrl_bus_timing1_oversampling));
  }

  return eRslt;
}

/*****************************************************************************/
/*! Set Single Acceptance Filter
* \description
*   Sets a single 32-bit acceptance filter
* \class
*   Control Service Class
* \params
*   uiInst          [in]    Instance
*   ptFilter        [in]    Pointer to acceptance filter configuration
* \return
*   CANCTRL_RESULT_OKAY
*   CANCTRL_RESULT_INVAL_PARAM
*   CANCTRL_RESULT_WRONG_MODE                                                */
/*****************************************************************************/
CANCTRL_RESULT CanCtrl_SetSingleAcceptanceFilter( unsigned int uiInst, CANCTRL_SINGLEFILTER_T* ptFilter )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return CANCTRL_RESULT_INVAL_PARAM;

  if( 0 == (s_aptCanCtrl[uiInst]->ulCanctrl_mode & HW_MSK(canctrl_mode_reset_mode)) )
    return CANCTRL_RESULT_WRONG_MODE;

  s_aptCanCtrl[uiInst]->aulCanctrl_data[3] = ptFilter->ulAcceptanceCode & 0xff;
  s_aptCanCtrl[uiInst]->aulCanctrl_data[2] = (ptFilter->ulAcceptanceCode >> 8) & 0xff;
  s_aptCanCtrl[uiInst]->aulCanctrl_data[1] = (ptFilter->ulAcceptanceCode >> 16) & 0xff;
  s_aptCanCtrl[uiInst]->aulCanctrl_data[0] = (ptFilter->ulAcceptanceCode >> 24) & 0xff;
#if defined(__NETX51)
  /* Note: In netX51/52 there is a failure when writing to Acceptance Mask 0 and 1.
     The write content of Acceptance Mask 0 goes correctly into register Acceptance Mask 0 and additionally abnormal into register Acceptance Mask 1.
     Acceptance Mask 1 is not writable individually.
     Because of this bug the content of Acceptance Mask 0 and Acceptance Mask 1 must be the same and that leads to restricted filter possibilities.
     Furthermore the locations of Acceptance Mask 2/3 changed to Registers Data 5 and Data 6.
  */
  s_aptCanCtrl[uiInst]->aulCanctrl_data[6] = ptFilter->ulAcceptanceMask & 0xff;
  s_aptCanCtrl[uiInst]->aulCanctrl_data[5] = (ptFilter->ulAcceptanceMask >> 8) & 0xff;
  /*s_aptCanCtrl[uiInst]->aulCanctrl_data[4] = (ptFilter->ulAcceptanceMask >> 16) & 0xff; */ /* disabled: bug!!! */
  s_aptCanCtrl[uiInst]->aulCanctrl_data[4] = (ptFilter->ulAcceptanceMask >> 24) & 0xff;
#else
  s_aptCanCtrl[uiInst]->aulCanctrl_data[7] = ptFilter->ulAcceptanceMask & 0xff;
  s_aptCanCtrl[uiInst]->aulCanctrl_data[6] = (ptFilter->ulAcceptanceMask >> 8) & 0xff;
  s_aptCanCtrl[uiInst]->aulCanctrl_data[5] = (ptFilter->ulAcceptanceMask >> 16) & 0xff;
  s_aptCanCtrl[uiInst]->aulCanctrl_data[4] = (ptFilter->ulAcceptanceMask >> 24) & 0xff;
#endif
  s_aptCanCtrl[uiInst]->ulCanctrl_mode |= HW_MSK(canctrl_mode_acceptance_mode);

  return CANCTRL_RESULT_OKAY;
}

/*****************************************************************************/
/*! Get Single Acceptance Filter
* \description
*   Gets the configured single 32-bit acceptance filter
* \class
*   Status Service Class
* \params
*   uiInst          [in]    Instance
*   ptFilter        [out]   Pointer to acceptance filter configuration
* \return
*   CANCTRL_RESULT_OKAY
*   CANCTRL_RESULT_INVAL_PARAM
*   CANCTRL_RESULT_WRONG_MODE                                                */
/*****************************************************************************/
CANCTRL_RESULT CanCtrl_GetSingleAcceptanceFilter( unsigned int uiInst, CANCTRL_SINGLEFILTER_T* ptFilter )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return CANCTRL_RESULT_INVAL_PARAM;

  if( 0 == (s_aptCanCtrl[uiInst]->ulCanctrl_mode & HW_MSK(canctrl_mode_reset_mode)))
    return CANCTRL_RESULT_WRONG_MODE;

  if( 0 == (s_aptCanCtrl[uiInst]->ulCanctrl_mode & HW_MSK(canctrl_mode_acceptance_mode)))
    return CANCTRL_RESULT_WRONG_MODE;

  ptFilter->ulAcceptanceCode = (s_aptCanCtrl[uiInst]->aulCanctrl_data[3] << 0)
                              |(s_aptCanCtrl[uiInst]->aulCanctrl_data[2] << 8)
                              |(s_aptCanCtrl[uiInst]->aulCanctrl_data[1] << 16)
                              |(s_aptCanCtrl[uiInst]->aulCanctrl_data[0] << 24);

  ptFilter->ulAcceptanceMask = (s_aptCanCtrl[uiInst]->aulCanctrl_data[7] << 0)
                              |(s_aptCanCtrl[uiInst]->aulCanctrl_data[6] << 8)
                              |(s_aptCanCtrl[uiInst]->aulCanctrl_data[5] << 16)
                              |(s_aptCanCtrl[uiInst]->aulCanctrl_data[4] << 24);

  return CANCTRL_RESULT_OKAY;
}

/*****************************************************************************/
/*! Set Dual Acceptance Filter
* \description
*   Sets two 16-bit acceptance filters
* \class
*   Control Service Class
* \params
*   uiInst          [in]    Instance
*   ptFilter        [in]    Pointer to acceptance filter configuration
* \return
*   CANCTRL_RESULT_OKAY
*   CANCTRL_RESULT_INVAL_PARAM
*   CANCTRL_RESULT_WRONG_MODE                                                */
/*****************************************************************************/
CANCTRL_RESULT CanCtrl_SetDualAcceptanceFilter( unsigned int uiInst, CANCTRL_DUALFILTER_T* ptFilter )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return CANCTRL_RESULT_INVAL_PARAM;

  if( 0 == (s_aptCanCtrl[uiInst]->ulCanctrl_mode & HW_MSK(canctrl_mode_reset_mode)) )
    return CANCTRL_RESULT_WRONG_MODE;

  s_aptCanCtrl[uiInst]->aulCanctrl_data[1] = (uint32_t)ptFilter->usAcceptanceCode0 & 0xff;
  s_aptCanCtrl[uiInst]->aulCanctrl_data[0] = ((uint32_t)ptFilter->usAcceptanceCode0 >> 8) & 0xff;
  s_aptCanCtrl[uiInst]->aulCanctrl_data[3] = (uint32_t)ptFilter->usAcceptanceCode1 & 0xff;
  s_aptCanCtrl[uiInst]->aulCanctrl_data[2] = ((uint32_t)ptFilter->usAcceptanceCode1 >> 8) & 0xff;
#if defined(__NETX51)
  /* Note: In netX51/52 there is a failure when writing to Acceptance Mask 0 and 1.
     The write content of Acceptance Mask 0 goes correctly into register Acceptance Mask 0 and additionally abnormal into register Acceptance Mask 1.
     Acceptance Mask 1 is not writable individually.
     Because of this bug the content of Acceptance Mask 0 and Acceptance Mask 1 must be the same and that leads to restricted filter possibilities.
     Furthermore the locations of Acceptance Mask 2/3 changed to Registers Data 5 and Data 6.
  */
  s_aptCanCtrl[uiInst]->aulCanctrl_data[4] = (uint32_t)ptFilter->usAcceptanceMask0 & 0xff;
  /*s_aptCanCtrl[uiInst]->aulCanctrl_data[4] = ((uint32_t)ptFilter->usAcceptanceMask0 >> 8) & 0xff; */ /* disabled: bug!!! */
  s_aptCanCtrl[uiInst]->aulCanctrl_data[6] = (uint32_t)ptFilter->usAcceptanceMask1 & 0xff;
  s_aptCanCtrl[uiInst]->aulCanctrl_data[5] = ((uint32_t)ptFilter->usAcceptanceMask1 >> 8) & 0xff;
#else
  s_aptCanCtrl[uiInst]->aulCanctrl_data[5] = (uint32_t)ptFilter->usAcceptanceMask0 & 0xff;
  s_aptCanCtrl[uiInst]->aulCanctrl_data[4] = ((uint32_t)ptFilter->usAcceptanceMask0 >> 8) & 0xff;
  s_aptCanCtrl[uiInst]->aulCanctrl_data[7] = (uint32_t)ptFilter->usAcceptanceMask1 & 0xff;
  s_aptCanCtrl[uiInst]->aulCanctrl_data[6] = ((uint32_t)ptFilter->usAcceptanceMask1 >> 8) & 0xff;
#endif
  s_aptCanCtrl[uiInst]->ulCanctrl_mode &= ~HW_MSK(canctrl_mode_acceptance_mode);

  return CANCTRL_RESULT_OKAY;
}

/*****************************************************************************/
/*! Get Dual Acceptance Filter
* \description
*   Gets the configured two 16-bit acceptance filters
* \class
*   Status Service Class
* \params
*   uiInst          [in]    Instance
*   ptFilter        [out]   Pointer to acceptance filter configuration
* \return
*   CANCTRL_RESULT_OKAY
*   CANCTRL_RESULT_INVAL_PARAM
*   CANCTRL_RESULT_WRONG_MODE                                                */
/*****************************************************************************/
CANCTRL_RESULT CanCtrl_GetDualAcceptanceFilter( unsigned int uiInst, CANCTRL_DUALFILTER_T* ptFilter )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return CANCTRL_RESULT_INVAL_PARAM;

  if( 0 == (s_aptCanCtrl[uiInst]->ulCanctrl_mode & HW_MSK(canctrl_mode_reset_mode)) )
    return CANCTRL_RESULT_WRONG_MODE;

  if( 0 != (s_aptCanCtrl[uiInst]->ulCanctrl_mode & HW_MSK(canctrl_mode_acceptance_mode)) )
    return CANCTRL_RESULT_WRONG_MODE;

  ptFilter->usAcceptanceCode0 = (uint16_t)(s_aptCanCtrl[uiInst]->aulCanctrl_data[1] << 0)
                               |(uint16_t)(s_aptCanCtrl[uiInst]->aulCanctrl_data[0] << 8);

  ptFilter->usAcceptanceCode1 = (uint16_t)(s_aptCanCtrl[uiInst]->aulCanctrl_data[3] << 0)
                               |(uint16_t)(s_aptCanCtrl[uiInst]->aulCanctrl_data[2] << 8);

  ptFilter->usAcceptanceMask0 = (uint16_t)(s_aptCanCtrl[uiInst]->aulCanctrl_data[5] << 0)
                               |(uint16_t)(s_aptCanCtrl[uiInst]->aulCanctrl_data[4] << 8);

  ptFilter->usAcceptanceMask1 = (uint16_t)(s_aptCanCtrl[uiInst]->aulCanctrl_data[7] << 0)
                               |(uint16_t)(s_aptCanCtrl[uiInst]->aulCanctrl_data[6] << 8);

  return CANCTRL_RESULT_OKAY;
}

/*****************************************************************************/
/*! Enter Listen Mode
* \description
*   Enters the listen mode of the CAN controller.
* \class
*   Control Service Class
* \params
*   uiInst          [in]    Instance
* \return
*   CANCTRL_RESULT_OKAY
*   CANCTRL_RESULT_INVAL_PARAM
*   CANCTRL_RESULT_WRONG_MODE                                                */
/*****************************************************************************/
CANCTRL_RESULT CanCtrl_EnterListenMode( unsigned int uiInst )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return CANCTRL_RESULT_INVAL_PARAM;

  if( 0 == (s_aptCanCtrl[uiInst]->ulCanctrl_mode & HW_MSK(canctrl_mode_reset_mode)) )
    return CANCTRL_RESULT_WRONG_MODE;

  s_aptCanCtrl[uiInst]->ulCanctrl_mode |= HW_MSK(canctrl_mode_listen_mode);

  return CANCTRL_RESULT_OKAY;
}

/*****************************************************************************/
/*! Leave Listen Mode
* \description
*   Leaves the listen mode of the CAN controller and goes to operating mode.
* \class
*   Control Service Class
* \params
*   uiInst            [in]    Instance
* \return
*   CANCTRL_RESULT_OKAY
*   CANCTRL_RESULT_WRONG_MODE
*   CANCTRL_RESULT_INVAL_PARAM                                               */
/*****************************************************************************/
CANCTRL_RESULT CanCtrl_LeaveListenMode( unsigned int uiInst )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return CANCTRL_RESULT_INVAL_PARAM;

  if( 0 == (s_aptCanCtrl[uiInst]->ulCanctrl_mode & HW_MSK(canctrl_mode_reset_mode)) )
    return CANCTRL_RESULT_WRONG_MODE;

  s_aptCanCtrl[uiInst]->ulCanctrl_mode &= ~HW_MSK(canctrl_mode_listen_mode);

  return CANCTRL_RESULT_OKAY;
}

/*****************************************************************************/
/*! Enter Self-Test Mode
* \description
*   Enters the Self-Test mode of the CAN controller.
* \class
*   Control Service Class
* \params
*   uiInst          [in]    Instance
* \return
*   CANCTRL_RESULT_OKAY
*   CANCTRL_RESULT_INVAL_PARAM
*   CANCTRL_RESULT_WRONG_MODE                                                */
/*****************************************************************************/
CANCTRL_RESULT CanCtrl_EnterSelftestMode( unsigned int uiInst )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return CANCTRL_RESULT_INVAL_PARAM;

  if( 0 == (s_aptCanCtrl[uiInst]->ulCanctrl_mode & HW_MSK(canctrl_mode_reset_mode)) )
    return CANCTRL_RESULT_WRONG_MODE;

  s_aptCanCtrl[uiInst]->ulCanctrl_mode |= HW_MSK(canctrl_mode_selftest);

  return CANCTRL_RESULT_OKAY;
}


/*****************************************************************************/
/*! Leave Self-Test Mode
* \description
*   Leaves the Self-Test mode of the CAN controller and goes to operating mode.
* \class
*   Control Service Class
* \params
*   uiInst          [in]    Instance
* \return
*   CANCTRL_RESULT_OKAY
*   CANCTRL_RESULT_INVAL_PARAM
*   CANCTRL_RESULT_WRONG_MODE                                                */
/*****************************************************************************/
CANCTRL_RESULT CanCtrl_LeaveSelftestMode( unsigned int uiInst )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return CANCTRL_RESULT_INVAL_PARAM;

  if( 0 == (s_aptCanCtrl[uiInst]->ulCanctrl_mode & HW_MSK(canctrl_mode_reset_mode)) )
    return CANCTRL_RESULT_WRONG_MODE;

  s_aptCanCtrl[uiInst]->ulCanctrl_mode &= ~HW_MSK(canctrl_mode_selftest);

  return CANCTRL_RESULT_OKAY;
}

/*****************************************************************************/
/*! Send Message
* \description
*   Sends a message on CAN bus.
* \class
*   Transmission Service Class
* \params
*   uiInst           [in]   Instance
*   ptMessage        [in]   Pointer to message
*   fSingleShot      [in]   Send message in single-shot (true) or repeat if no acknowledge was received (false)
*   fSelfRx          [in]   Send message and receive it simultaneously
* \return
*   CANCTRL_RESULT_OKAY
*   CANCTRL_RESULT_INVAL_PARAM
*   CANCTRL_RESULT_TX_BUFFER_NOT_FREE
*   CANCTRL_RESULT_WRONG_MODE                                                */
/*****************************************************************************/
CANCTRL_RESULT CanCtrl_SendMessage( unsigned int uiInst, CANCTRL_MESSAGE_T* ptMessage, bool fSingleShot, bool fSelfRx )
{
  uint32_t  ulCmd;
  unsigned int uLength, uIdx, uDataPtr;

  if( uiInst >= HAL_CANCTRL_CNT )
    return CANCTRL_RESULT_INVAL_PARAM;

  if( 0 != (s_aptCanCtrl[uiInst]->ulCanctrl_mode & HW_MSK(canctrl_mode_reset_mode)) )
    return CANCTRL_RESULT_WRONG_MODE;

  if( 0 == (s_aptCanCtrl[uiInst]->ulCanctrl_status & HW_MSK(canctrl_status_tx_buf_status)) )
    return CANCTRL_RESULT_TX_BUFFER_NOT_FREE;

  /* build message */
  s_aptCanCtrl[uiInst]->aulCanctrl_data[0] = ((uint32_t)ptMessage->uFrameFormat << 7)
                                            |((uint32_t)ptMessage->uRtr << 6)
                                            |(uint32_t)ptMessage->uDlc;

  if( 0 == ptMessage->uFrameFormat )
  {
    /* Standard ID */
    s_aptCanCtrl[uiInst]->aulCanctrl_data[1] = (ptMessage->ulIdentifier >> 3) & 0xff;
    s_aptCanCtrl[uiInst]->aulCanctrl_data[2] = (ptMessage->ulIdentifier << 5) & 0xff;
    uDataPtr = 3;
  }
  else
  {
    /* Extended ID */
    s_aptCanCtrl[uiInst]->aulCanctrl_data[1] = (ptMessage->ulIdentifier >> 21) & 0xff;
    s_aptCanCtrl[uiInst]->aulCanctrl_data[2] = (ptMessage->ulIdentifier >> 13) & 0xff;
    s_aptCanCtrl[uiInst]->aulCanctrl_data[3] = (ptMessage->ulIdentifier >> 5)  & 0xff;
    s_aptCanCtrl[uiInst]->aulCanctrl_data[4] = (ptMessage->ulIdentifier << 3)  & 0xff;
    uDataPtr = 5;
  }

  if( 0 == ptMessage->uRtr )
  {
    /* Data frame */
    uLength = ptMessage->uDlc;
    if( uLength > 8 ) uLength = 8;

    /* put transmit data to CAN controller */
    for( uIdx = 0; uIdx < uLength; ++uIdx ) {
      s_aptCanCtrl[uiInst]->aulCanctrl_data[uDataPtr + uIdx] = ptMessage->abData[uIdx];
    }
  }

  /* send message */
  if( !fSelfRx )
  {
    ulCmd = HW_MSK(canctrl_command_tx_request);

    /* Setting the command bits tx_request and abort_tx simultaneously results in sending the transmit message once */
    if( fSingleShot ) {
      ulCmd |= HW_MSK(canctrl_command_abort_tx);
    }
  }
  else {
    ulCmd = HW_MSK(canctrl_command_self_rx_request);
  }

  s_aptCanCtrl[uiInst]->ulCanctrl_command = ulCmd;

  return CANCTRL_RESULT_OKAY;
}

/*****************************************************************************/
/*! Abort Message
* \description
*   Aborts a currently pending message.
* \class
*   Transmission Service Class
* \params
*   uiInst            [in]    Instance
* \return
*   CANCTRL_RESULT_OKAY
*   CANCTRL_RESULT_INVAL_PARAM
*   CANCTRL_RESULT_WRONG_MODE                                                */
/*****************************************************************************/
CANCTRL_RESULT CanCtrl_AbortMessage( unsigned int uiInst )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return CANCTRL_RESULT_INVAL_PARAM;

  if( 0 != (s_aptCanCtrl[uiInst]->ulCanctrl_mode & HW_MSK(canctrl_mode_reset_mode)) )
    return CANCTRL_RESULT_WRONG_MODE;

  s_aptCanCtrl[uiInst]->ulCanctrl_command = HW_MSK(canctrl_command_abort_tx);

  return CANCTRL_RESULT_OKAY;
}

/*****************************************************************************/
/*! Receive Message
* \description
*   Receives the first pending message from the RX FIFO.
* \class
*   Reception Service Class
* \params
*   uiInst        [in]    Instance
*   ptMessage     [out]   Pointer to received message
* \return
*   CANCTRL_RESULT_OKAY
*   CANCTRL_RESULT_INVAL_PARAM
*   CANCTRL_RESULT_NO_MESSAGE_AVAILABLE
*   CANCTRL_RESULT_MESSAGE_DROPPED
*   CANCTRL_RESULT_WRONG_MODE                                                */
/*****************************************************************************/
CANCTRL_RESULT CanCtrl_ReceiveMessage( unsigned int uiInst, CANCTRL_MESSAGE_T* ptMessage )
{
  unsigned int uDataPtr, uLength, uIdx;

  if( uiInst >= HAL_CANCTRL_CNT )
    return CANCTRL_RESULT_INVAL_PARAM;

  /* check that we are operational */
  if( 0 != (s_aptCanCtrl[uiInst]->ulCanctrl_mode & HW_MSK(canctrl_mode_reset_mode)) )
    return CANCTRL_RESULT_WRONG_MODE;

  /* check that message available */
  if( 0 == (s_aptCanCtrl[uiInst]->ulCanctrl_status & HW_MSK(canctrl_status_rx_buf_status)) )
    return CANCTRL_RESULT_NO_MESSAGE_AVAILABLE;

  /* check for overrun */
  if( 0 != (s_aptCanCtrl[uiInst]->ulCanctrl_status & HW_MSK(canctrl_status_overrun)) ) {
    /* clear overrun condition */
    s_aptCanCtrl[uiInst]->ulCanctrl_command = HW_MSK(canctrl_command_clr_overrun)
                                    |HW_MSK(canctrl_command_release_rx_buf);
    return CANCTRL_RESULT_MESSAGE_DROPPED;
  }

  /* get message type */
  ptMessage->uFrameFormat = (s_aptCanCtrl[uiInst]->aulCanctrl_data[0] >> 7) & 0x1;

  /* get RTR bit */
  ptMessage->uRtr = (s_aptCanCtrl[uiInst]->aulCanctrl_data[0] >> 6) & 0x1;
  if( 0 == ptMessage->uFrameFormat ) {
    /* Standard ID */
    ptMessage->ulIdentifier = ( (s_aptCanCtrl[uiInst]->aulCanctrl_data[1] << 24)
                             | ((s_aptCanCtrl[uiInst]->aulCanctrl_data[2] & 0xef) << 16)
                              ) >> 21;
    uDataPtr = 3;
  }
  else {
    /* Extended ID */
    ptMessage->ulIdentifier = ( (s_aptCanCtrl[uiInst]->aulCanctrl_data[1] << 24)
                               |(s_aptCanCtrl[uiInst]->aulCanctrl_data[2] << 16)
                               |(s_aptCanCtrl[uiInst]->aulCanctrl_data[3] << 8)
                               |(s_aptCanCtrl[uiInst]->aulCanctrl_data[4] << 0)
                              ) >> 3;
    uDataPtr = 5;
  }

  /* get DLC */
  ptMessage->uDlc = (unsigned int)(s_aptCanCtrl[uiInst]->aulCanctrl_data[0] & 0xf);

  if( 0 == ptMessage->uRtr )
  {
    /* Data frame */
    uLength = ptMessage->uDlc;
    if( uLength > 8 ) uLength = 8;

    /* get data bytes */
    for( uIdx = 0; uIdx < uLength; ++uIdx )
      ptMessage->abData[uIdx] = (uint8_t)s_aptCanCtrl[uiInst]->aulCanctrl_data[uDataPtr + uIdx];
  }

  /* release buffer */
  s_aptCanCtrl[uiInst]->ulCanctrl_command = HW_MSK(canctrl_command_release_rx_buf);

  return CANCTRL_RESULT_OKAY;
}

/*****************************************************************************/
/*! Get RX Message Count
* \description
*   Returns the number of pending messages in the RX FIFO.
* \class
*   Status Service Class
* \params
*   uiInst          [in]    Instance
* \return
*   Current number of messages in receive FIFO                               */
/*****************************************************************************/
uint32_t CanCtrl_GetRxMessageCnt( unsigned int uiInst )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return 0;

  return s_aptCanCtrl[uiInst]->ulCanctrl_rx_message_cnt;
}

/*****************************************************************************/
/*! Get Mode
* \description
*   Returns the mode of the CAN controller.
* \class
*   Status Service Class
* \params
*   uiInst          [in]    Instance
* \return
*   Current mode of CAN controller                                           */
/*****************************************************************************/
uint32_t CanCtrl_GetMode( unsigned int uiInst )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return 0;

  return s_aptCanCtrl[uiInst]->ulCanctrl_mode;
}

/*****************************************************************************/
/*! Get Status
* \description
*   Returns the status of the CAN controller.
* \class
*   Status Service Class
* \params
*   uiInst          [in]    Instance
* \return
*   Current status of CAN controller                                         */
/*****************************************************************************/
uint32_t CanCtrl_GetStatus( unsigned int uiInst )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return 0;

  return s_aptCanCtrl[uiInst]->ulCanctrl_status;
}

/*****************************************************************************/
/*! Get and Clear IRQ Status
* \description
*   Returns and clears the interrupt status of the CAN controller.
*   The status of the receive interrupt bit will not be cleared.
* \class
*   Status Service Class
* \params
*   uiInst          [in]    Instance
* \return
*   Current interrupt status                                                 */
/*****************************************************************************/
uint32_t CanCtrl_GetAndClearIrqStatus( unsigned int uiInst )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return 0;

  return s_aptCanCtrl[uiInst]->ulCanctrl_irq;
}

/*****************************************************************************/
/*! Set IRQ Mask
* \description
*   Sets the IRQ enable mask.
* \class
*   Control Service Class
* \params
*   uiInst           [in]   Instance
*   ulIrqMask        [in]   Interrupt enable mask
* \return
*   CANCTRL_RESULT_OKAY
*   CANCTRL_RESULT_INVAL_PARAM                                               */
/*****************************************************************************/
CANCTRL_RESULT CanCtrl_SetIrqMask( unsigned int uiInst, uint32_t ulIrqMask )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return CANCTRL_RESULT_INVAL_PARAM;

  s_aptCanCtrl[uiInst]->ulCanctrl_irq_en = ulIrqMask;

  return CANCTRL_RESULT_OKAY;
}

/*****************************************************************************/
/*! Get IRQ Mask
* \description
*   Returns the currently set IRQ enable mask.
* \class
*   Status Service Class
* \params
*   uiInst            [in]    Instance
* \return
*   Current interrupt mask                                                   */
/*****************************************************************************/
uint32_t CanCtrl_GetIrqMask( unsigned int uiInst )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return 0;

  return s_aptCanCtrl[uiInst]->ulCanctrl_irq_en;
}

/*****************************************************************************/
/*! Get and Clear Arbitration Lost Status
* \description
*   Returns and clears the status of the last latched lost
*   bus arbitration.
* \class
*   Status Service Class
* \params
*   uiInst          [in]    Instance
* \return
*   Current status of the last latched arbitration lost                      */
/*****************************************************************************/
uint32_t CanCtrl_GetAndClearArblostStatus( unsigned int uiInst )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return 0;

  return s_aptCanCtrl[uiInst]->ulCanctrl_arb_lost_capture;
}

/*****************************************************************************/
/*! Get and Clear Error Code
* \description
*   Returns and clears the error code of the last latched bus error.
* \class
*   Status Service Class
* \params
*   uiInst          [in]    Instance
* \return
*   Current error code of the last latched bus error                         */
/*****************************************************************************/
uint32_t CanCtrl_GetAndClearErrorCode( unsigned int uiInst )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return 0;

  return s_aptCanCtrl[uiInst]->ulCanctrl_err_code_capture;
}

/*****************************************************************************/
/*! Set Error Warning Limit
* \description
*   Sets the warning limit for error counters.
* \class
*   Control Service Class
* \params
*   uiInst         [in]   Instance
*   ulLimit        [in]   Error warning limit
* \return
*   CANCTRL_RESULT_OKAY
*   CANCTRL_RESULT_INVAL_PARAM
*   CANCTRL_RESULT_WRONG_MODE                                                */
/*****************************************************************************/
CANCTRL_RESULT CanCtrl_SetWarningLimit( unsigned int uiInst, uint32_t ulLimit )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return CANCTRL_RESULT_INVAL_PARAM;

  if( 0 == (s_aptCanCtrl[uiInst]->ulCanctrl_mode & HW_MSK(canctrl_mode_reset_mode)) )
    return CANCTRL_RESULT_WRONG_MODE;

  s_aptCanCtrl[uiInst]->ulCanctrl_err_warning_limit = ulLimit;

  return CANCTRL_RESULT_OKAY;
}

/*****************************************************************************/
/*! Get Error Warning Limit
* \description
*   Returns the warning limit for error counters.
* \class
*   Status Service Class
* \params
*   uiInst            [in]    Instance
* \return
*   Current error warning limit                                              */
/*****************************************************************************/
uint32_t CanCtrl_GetWarningLimit( unsigned int uiInst )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return 0;

  return s_aptCanCtrl[uiInst]->ulCanctrl_err_warning_limit;
}

/*****************************************************************************/
/*! Set RX Error Counter
* \description
*   Sets the value of the RX error counter.
* \class
*   Control Service Class
* \params
*   uiInst       [in]   Instance
*   ulCnt        [in]   Error counter value
* \return
*   CANCTRL_RESULT_OKAY
*   CANCTRL_RESULT_INVAL_PARAM
*   CANCTRL_RESULT_WRONG_MODE                                                */
/*****************************************************************************/
CANCTRL_RESULT CanCtrl_SetRxErrorCnt( unsigned int uiInst, uint32_t ulCnt )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return CANCTRL_RESULT_INVAL_PARAM;

  if( 0 == (s_aptCanCtrl[uiInst]->ulCanctrl_mode & HW_MSK(canctrl_mode_reset_mode)) )
    return CANCTRL_RESULT_WRONG_MODE;

  s_aptCanCtrl[uiInst]->ulCanctrl_rx_error_cnt = ulCnt;

  return CANCTRL_RESULT_OKAY;
}

/*****************************************************************************/
/*! Get RX Error Counter
* \description
*   Returns the value of the RX error counter.
* \class
*   Status Service Class
* \params
*   uiInst         [in]    Instance
* \return
*   Current receive error counter                                            */
/*****************************************************************************/
uint32_t CanCtrl_GetRxErrorCnt( unsigned int uiInst )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return 0;

  return s_aptCanCtrl[uiInst]->ulCanctrl_rx_error_cnt;
}

/*****************************************************************************/
/*! Set TX Error Counter
* \description
*   Sets the value of the TX error counter.
* \class
*   Control Service Class
* \params
*   uiInst       [in]   Instance
*   ulCnt        [in]   Error counter value
* \return
*   CANCTRL_RESULT_OKAY
*   CANCTRL_RESULT_INVAL_PARAM
*   CANCTRL_RESULT_WRONG_MODE                                                */
/*****************************************************************************/
CANCTRL_RESULT CanCtrl_SetTxErrorCnt( unsigned int uiInst, uint32_t ulCnt )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return CANCTRL_RESULT_INVAL_PARAM;

  if( 0 == (s_aptCanCtrl[uiInst]->ulCanctrl_mode & HW_MSK(canctrl_mode_reset_mode)) )
    return CANCTRL_RESULT_WRONG_MODE;

  s_aptCanCtrl[uiInst]->ulCanctrl_tx_error_cnt = ulCnt;

  return CANCTRL_RESULT_OKAY;
}

/*****************************************************************************/
/*! Get TX Error Counter
* \description
*   Gets the value of the TX error counter.
* \class
*   Status Service Class
* \params
*   uiInst          [in]    Instance
* \return
*   Current transmit error counter                                           */
/*****************************************************************************/
uint32_t CanCtrl_GetTxErrorCnt( unsigned int uiInst )
{
  if( uiInst >= HAL_CANCTRL_CNT )
    return 0;

  return s_aptCanCtrl[uiInst]->ulCanctrl_tx_error_cnt;
}

/*****************************************************************************/
/*! Set Parameter
* \description
*   This function sets parameter.
*   CAN Controller must be in Reset mode.
* \class
*   Control Service Class
* \params
*   uiInst           [in]  Instance
*   ePrmID           [in]  Parameter ID
*   ulPrmVal         [in]  New parameter value
* \return
*   CANCTRL_RESULT_OKAY
*   CANCTRL_ERR_INVAL_PARAM                                                  */
/*****************************************************************************/
CANCTRL_RESULT CanCtrl_SetPrm( unsigned int uiInst, CANCTRL_PRM_E ePrmID, uint32_t ulPrmVal )
{
  CANCTRL_RESULT eRslt = CANCTRL_RESULT_OKAY;

  if( uiInst >= HAL_CANCTRL_CNT )
    return CANCTRL_RESULT_INVAL_PARAM;

  if( 0 == (s_aptCanCtrl[uiInst]->ulCanctrl_mode & HW_MSK(canctrl_mode_reset_mode)) )
    return CANCTRL_RESULT_WRONG_MODE;

  switch( ePrmID )
  {
    case CANCTRL_PRM_IRQEN:             s_aptCanCtrl[uiInst]->ulCanctrl_irq_en = ulPrmVal; break;
    case CANCTRL_PRM_BT0:               s_aptCanCtrl[uiInst]->aulCanctrl_bus_timing[0] = ulPrmVal; break;
    case CANCTRL_PRM_BT1:               s_aptCanCtrl[uiInst]->aulCanctrl_bus_timing[1] = ulPrmVal; break;
    case CANCTRL_PRM_ERR_WARNING_LIMIT: s_aptCanCtrl[uiInst]->ulCanctrl_err_warning_limit = ulPrmVal; break;
    case CANCTRL_PRM_RXERR_CNT:         s_aptCanCtrl[uiInst]->ulCanctrl_rx_error_cnt = ulPrmVal; break;
    case CANCTRL_PRM_TXERR_CNT:         s_aptCanCtrl[uiInst]->ulCanctrl_tx_error_cnt = ulPrmVal; break;
    case CANCTRL_PRM_ACCEPTANCE_CODE0:  s_aptCanCtrl[uiInst]->aulCanctrl_data[0] = ulPrmVal; break;
    case CANCTRL_PRM_ACCEPTANCE_CODE1:  s_aptCanCtrl[uiInst]->aulCanctrl_data[1] = ulPrmVal; break;
    case CANCTRL_PRM_ACCEPTANCE_CODE2:  s_aptCanCtrl[uiInst]->aulCanctrl_data[2] = ulPrmVal; break;
    case CANCTRL_PRM_ACCEPTANCE_CODE3:  s_aptCanCtrl[uiInst]->aulCanctrl_data[3] = ulPrmVal; break;
    case CANCTRL_PRM_ACCEPTANCE_MASK0:  s_aptCanCtrl[uiInst]->aulCanctrl_data[4] = ulPrmVal; break;
    case CANCTRL_PRM_ACCEPTANCE_MASK1:  s_aptCanCtrl[uiInst]->aulCanctrl_data[5] = ulPrmVal; break;
    case CANCTRL_PRM_ACCEPTANCE_MASK2:  s_aptCanCtrl[uiInst]->aulCanctrl_data[6] = ulPrmVal; break;
    case CANCTRL_PRM_ACCEPTANCE_MASK3:  s_aptCanCtrl[uiInst]->aulCanctrl_data[7] = ulPrmVal; break;
    /* Default */
    default:
      eRslt = CANCTRL_RESULT_INVAL_PARAM;
    break;
  } /* end switch */

  return eRslt;
}

/*****************************************************************************/
/*! Get Parameter
* \description
*   This function reads parameter.
*   CAN Controller must be in Reset mode.
* \class
*   Status Service Class
* \params
*   uiInst           [in]  Instance
*   ePrmID           [in]  Parameter ID
*   pulPrmVal        [in]  Pointer to parameter value
* \return
*   CANCTRL_RESULT_OKAY
*   CANCTRL_ERR_INVAL_PARAM                                           */
/*****************************************************************************/
CANCTRL_RESULT CanCtrl_GetPrm( unsigned int uiInst, CANCTRL_PRM_E ePrmID, uint32_t* pulPrmVal )
{
  CANCTRL_RESULT eRslt = CANCTRL_RESULT_OKAY;

  if( uiInst >= HAL_CANCTRL_CNT )
    return CANCTRL_RESULT_INVAL_PARAM;

  if( 0 == (s_aptCanCtrl[uiInst]->ulCanctrl_mode & HW_MSK(canctrl_mode_reset_mode)) )
    return CANCTRL_RESULT_WRONG_MODE;

  switch( ePrmID )
  {
    case CANCTRL_PRM_IRQEN:             *pulPrmVal = s_aptCanCtrl[uiInst]->ulCanctrl_irq_en; break;
    case CANCTRL_PRM_BT0:               *pulPrmVal = s_aptCanCtrl[uiInst]->aulCanctrl_bus_timing[0]; break;
    case CANCTRL_PRM_BT1:               *pulPrmVal = s_aptCanCtrl[uiInst]->aulCanctrl_bus_timing[1]; break;
    case CANCTRL_PRM_ERR_WARNING_LIMIT: *pulPrmVal = s_aptCanCtrl[uiInst]->ulCanctrl_err_warning_limit; break;
    case CANCTRL_PRM_RXERR_CNT:         *pulPrmVal = s_aptCanCtrl[uiInst]->ulCanctrl_rx_error_cnt; break;
    case CANCTRL_PRM_TXERR_CNT:         *pulPrmVal = s_aptCanCtrl[uiInst]->ulCanctrl_tx_error_cnt; break;
    case CANCTRL_PRM_ACCEPTANCE_CODE0:  *pulPrmVal = s_aptCanCtrl[uiInst]->aulCanctrl_data[0]; break;
    case CANCTRL_PRM_ACCEPTANCE_CODE1:  *pulPrmVal = s_aptCanCtrl[uiInst]->aulCanctrl_data[1]; break;
    case CANCTRL_PRM_ACCEPTANCE_CODE2:  *pulPrmVal = s_aptCanCtrl[uiInst]->aulCanctrl_data[2]; break;
    case CANCTRL_PRM_ACCEPTANCE_CODE3:  *pulPrmVal = s_aptCanCtrl[uiInst]->aulCanctrl_data[3]; break;
    case CANCTRL_PRM_ACCEPTANCE_MASK0:  *pulPrmVal = s_aptCanCtrl[uiInst]->aulCanctrl_data[4]; break;
    case CANCTRL_PRM_ACCEPTANCE_MASK1:  *pulPrmVal = s_aptCanCtrl[uiInst]->aulCanctrl_data[5]; break;
    case CANCTRL_PRM_ACCEPTANCE_MASK2:  *pulPrmVal = s_aptCanCtrl[uiInst]->aulCanctrl_data[6]; break;
    case CANCTRL_PRM_ACCEPTANCE_MASK3:  *pulPrmVal = s_aptCanCtrl[uiInst]->aulCanctrl_data[7]; break;
    default: eRslt = CANCTRL_RESULT_INVAL_PARAM; break;
  }

  return eRslt;
}

