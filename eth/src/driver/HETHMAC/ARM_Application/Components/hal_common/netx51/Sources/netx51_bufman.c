/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx51_bufman.h"
#include "hal_resources_defines_netx51.h"

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_BUFMAN

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! BUFMAN Request Write Buffer
* \description
*   This function requests a new write buffer.
*   Last write buffer is released automatically).
* \class
*   BUFMAN Service Class
* \params
*   uChNum  [in] channel number
* \return
*   0..2 - Buffer number
*   -1 invalid parameter                                                     */
/*****************************************************************************/
int NX51_BUFMAN_ReqWriteBuf ( unsigned int uChNum )
{
  uint32_t ulVal;

  if (uChNum >= NX51_BUFMAN_CH_MAX_NUM) return -1;

  /* request buffer */
  s_ptBufMan->ulBuf_man = ((uint32_t)0x01<<SRT_NX51_buf_man_req_type) | ((uint32_t)uChNum<<SRT_NX51_buf_man_buf_nr);

  /* dummy reads (buf_man needs 2 CC to evaluate correct address) */
  ulVal=s_ptBufMan->ulBuf_man;
  ulVal=s_ptBufMan->ulBuf_man;
  ulVal=s_ptBufMan->ulBuf_man;

  return (int) ((ulVal&MSK_NX51_buf_man_buf_nr)>>SRT_NX51_buf_man_buf_nr);
}

/*****************************************************************************/
/*! BUFMAN Release Write Buffer
* \description
*   This function releases the current write buffer and requests a new one automatically.
* \class
*   BUFMAN Service Class
* \params
*   uChNum  [in] channel number
* \return
*   0..2 buffer number
*   -1 invalid parameter                                                     */
/*****************************************************************************/
int NX51_BUFMAN_RelWriteBuf ( unsigned int uChNum )
{
  if (uChNum >= NX51_BUFMAN_CH_MAX_NUM) return -1;

  /* request buffer */
  s_ptBufMan->ulBuf_man = ((uint32_t)0x02<<SRT_NX51_buf_man_req_type) | ((uint32_t)uChNum<<SRT_NX51_buf_man_buf_nr);

  return 0;
}

/*****************************************************************************/
/*! BUFMAN Get actual read Buffer
* \description
*   This function requests the actual read buffer.
* \class
*   BUFMAN Service Class
* \params
*   uChNum  [in] channel number
* \return
*   0..2 buffer number
*   -1 invalid parameter                                                     */
/*****************************************************************************/
int NX51_BUFMAN_ReqReadBuf ( unsigned int uChNum )
{
  uint32_t ulVal;

  if (uChNum >= NX51_BUFMAN_CH_MAX_NUM) return -1;

  /* request buffer */
  s_ptBufMan->ulBuf_man = ((uint32_t)0x00<<SRT_NX51_buf_man_req_type) | ((uint32_t)uChNum<<SRT_NX51_buf_man_buf_nr);

  /* dummy reads (buf_man needs 2 CC to evaluate correct address) */
  ulVal=s_ptBufMan->ulBuf_man;
  ulVal=s_ptBufMan->ulBuf_man;
  ulVal=s_ptBufMan->ulBuf_man;

  return (int) ((ulVal&MSK_NX51_buf_man_buf_nr)>>SRT_NX51_buf_man_buf_nr);
}

/*****************************************************************************/
/*! BUFMAN Reset Channel
* \description
*   This function resets a buffer manager channel.
* \class
*   BUFMAN Service Class
* \params
*   uChNum  [in] channel number
* \return
*    0 Ok
*   -1 invalid parameter                                                     */
/*****************************************************************************/
int NX51_BUFMAN_ResetChannel ( unsigned int uChNum )
{
  if (uChNum >= NX51_BUFMAN_CH_MAX_NUM) return -1;

  /* reset channel */
  s_ptBufMan->ulBuf_man = (MSK_NX51_buf_man_reset) | ((uint32_t)uChNum<<SRT_NX51_buf_man_buf_nr);

  return 0;
}
