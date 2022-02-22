/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx51_xpic.h"
#include "hal_resources_defines_netx51.h"

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define NX51_XPIC_SEGDEF_HEADER_MAGIC              0x43495058
#define NX51_XPIC_SEGDEF_HEADER_MSK_VERSION_MAJOR  0xffff0000
#define NX51_XPIC_SEGDEF_HEADER_MSK_VERSION_MINOR  0x0000ffff
#define NX51_XPIC_SEGDEF_HEADER_VERSION_MAJOR      0x00010000
#define NX51_XPIC_SEGDEF_HEADER_VERSION_MINOR      0x00000000

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_XPIC
__USE_XPIC_DEBUG

/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! Load data/code segments
* \description
*   Load DWORD aligned data into memory
*   Can be used for xPIC program or any other areas
* \class
*   XPIC
* \params
*   pulData [in]  Pointer to microcode
* \return
*                                                                            */
/*****************************************************************************/
static void NX51_XPIC_LoadSegments( const uint32_t *pulData )
{
  const uint32_t *pulCur = &pulData[2];
  uint32_t* pulDest;
  uint32_t ulLen;

  /* check header magic */
  if( *pulCur++ != NX51_XPIC_SEGDEF_HEADER_MAGIC ) {
    /* invalid header */
    return;
  }
  /* check major version */
  if( (*pulCur++ & NX51_XPIC_SEGDEF_HEADER_MSK_VERSION_MAJOR) > NX51_XPIC_SEGDEF_HEADER_VERSION_MAJOR ) {
    /* version not supported */
    return;
  }

  pulDest = (uint32_t*)*pulCur++;
  ulLen = *pulCur++;
  while( ulLen ) {
    /* load segment */
    while( ulLen-- ) {
      *pulDest++ = *pulCur++;
    }
    /* read next segment */
    pulDest = (uint32_t*)*pulCur++;
    ulLen = *pulCur++;
  }
}

/*****************************************************************************/
/*! Load xPIC code
* \description
*   Load xPIC code,
*   Initialize trailing loads
* \class
*   XPIC
* \params
*   pulXpicCode [in]  Pointer to microcode
*   pvUser      [in]  Ignored
* \return
*                                                                            */
/*****************************************************************************/
void NX51_XPIC_Load( const uint32_t *pulXpicCode, void* pvUser )
{
  unsigned int programm_size;
  unsigned int tl_size;
  unsigned int programm_start_adr;
  unsigned int  i;
  unsigned int  tl_start;
  uint32_t* dummy;

  if( ( pulXpicCode[0] == 0 ) && ( pulXpicCode[1] == 0 ) ) {
    /* xPIC GNU Tool chain - segment definitions */
    NX51_XPIC_LoadSegments(pulXpicCode);
  } else {
    /* xPICASS generated program and trailing loads */
    programm_size       = pulXpicCode[0] / 4;  /* in Bytes / 4 */
    tl_size             = pulXpicCode[1] / 4;
    programm_start_adr  = pulXpicCode[2];
    tl_start            = programm_size + 2;

    /* program loader */
    for (i = 1; i < programm_size; i++){
      dummy = (uint32_t*) (programm_start_adr + (i - 1)*4);
      *dummy = pulXpicCode[i + 2];
    }

    /* trailing loader */
    for (i = 0; i < tl_size; i = i + 2){
      dummy = (uint32_t*) (pulXpicCode[tl_start + i]);
      *dummy = pulXpicCode[tl_start + i + 1];
    }
  }
}


/*****************************************************************************/
/*! Start xPIC
* \description
*   Start xPIC
* \class
*   XPIC
* \params
*   pvUser [in] Ignored
* \return
*                                                                            */
/*****************************************************************************/
void NX51_XPIC_Start( void* pvUser )
{
  s_ptXpicDebug->ulXpic_hold_pc = 0;
}

/*****************************************************************************/
/*! Stop xPIC
* \description
*   Halt xPIC
* \class
*   XPIC
* \params
*   pvUser [in] Ignored
* \return
*                                                                            */
/*****************************************************************************/
void NX51_XPIC_Stop( void* pvUser )
{
  s_ptXpicDebug->ulXpic_hold_pc = MSK_NX51_xpic_hold_pc_hold;
}

/*****************************************************************************/
/*! Reset xPIC
* \description
*   Hold xPIC, reset xPIC state,
*   clear work registers (both banks), clear user registers
* \class
*   XPIC
* \params
*   pvUser [in] Ignored
* \return
*                                                                            */
/*****************************************************************************/
void NX51_XPIC_Reset( void* pvUser )
{
  uint32_t ulVal;

  /* hold xPIC */
  s_ptXpicDebug->ulXpic_hold_pc = MSK_NX51_xpic_hold_pc_hold;

  /* clear all hold reasons */
  s_ptXpicDebug->ulXpic_break_irq_raw = 0x1f;

  /* hold xPIC and request reset */
  s_ptXpicDebug->ulXpic_hold_pc = MSK_NX51_xpic_hold_pc_hold | MSK_NX51_xpic_hold_pc_reset_xpic;

  /* wait for reset to be finished */
  do
  {
    ulVal = s_ptXpicDebug->ulXpic_break_status;
  } while( (ulVal & MSK_NX51_xpic_break_status_xpic_reset_status) == 0 );

  /* release reset request, engage bank control, select bank 1 */
  s_ptXpicDebug->ulXpic_hold_pc = MSK_NX51_xpic_hold_pc_hold | MSK_NX51_xpic_hold_pc_bank_control | MSK_NX51_xpic_hold_pc_bank_select;

  /* reset xPIC user registers */
  for( ulVal = 0; ulVal < 5; ulVal++ )
  {
    s_ptXpic->aulXpic_usr[ulVal] = 0;
  }

  /* reset xPIC work registers (bank 1) */
  for( ulVal = 0; ulVal < 8; ulVal++ )
  {
    s_ptXpic->aulXpic_r[ulVal] = 0;
  }

  /* select bank 0 */
  s_ptXpicDebug->ulXpic_hold_pc = MSK_NX51_xpic_hold_pc_hold | MSK_NX51_xpic_hold_pc_bank_control;

  /* reset xPIC work registers (bank 0) */
  for( ulVal = 0; ulVal < 8; ulVal++ )
  {
    s_ptXpic->aulXpic_r[ulVal] = 0;
  }

  /* release bank control */
  s_ptXpicDebug->ulXpic_hold_pc = MSK_NX51_xpic_hold_pc_hold;
}
