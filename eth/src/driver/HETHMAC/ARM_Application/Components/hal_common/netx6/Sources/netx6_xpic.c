/*****************************************************************************/
/*  Includes                                                                 */
/*****************************************************************************/
#include "netx6_xpic.h"
#include "hal_resources_defines_netx6.h"

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define NX6_XPIC_SEGDEF_HEADER_MAGIC              0x43495058
#define NX6_XPIC_SEGDEF_HEADER_MSK_VERSION_MAJOR  0xffff0000
#define NX6_XPIC_SEGDEF_HEADER_MSK_VERSION_MINOR  0x0000ffff
#define NX6_XPIC_SEGDEF_HEADER_VERSION_MAJOR      0x00010000
#define NX6_XPIC_SEGDEF_HEADER_VERSION_MINOR      0x00000000

#define NX6_XPIC_DWORD_RAMSIZE     0x0800
#define NX6_XPIC_DWORD_REGAREASIZE (REL_Adr_NX51_xpic_zero-REL_Adr_NX51_xpic_r0)

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/
__USE_XPIC
__USE_XPIC_DEBUG
__USE_XPIC_PRAM
__USE_XPIC_DRAM

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
static void NX6_XPIC_LoadSegments( const uint32_t *pulData )
{
  const uint32_t *pulCur = &pulData[2];
  uint32_t* pulDest;
  uint32_t ulLen;

  /* check header magic */
  if( *pulCur++ != NX6_XPIC_SEGDEF_HEADER_MAGIC ) {
    /* invalid header */
    return;
  }
  /* check major version */
  if( (*pulCur++ & NX6_XPIC_SEGDEF_HEADER_MSK_VERSION_MAJOR) > NX6_XPIC_SEGDEF_HEADER_VERSION_MAJOR ) {
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
*   xpic_code [in]  Pointer to microcode
*   pvUser    [in]  User specific parameter
* \return
*                                                                            */
/*****************************************************************************/
void NX6_XPIC_Load( const uint32_t *xpic_code,
                    void           *pvUser )
{
  unsigned int programm_size;
  unsigned int tl_size;
  uint32_t programm_start_adr;
  unsigned int  i;
  unsigned int  tl_start;
  volatile uint32_t* pulDst;
  uint32_t ulAddress, ulOffset;

  if( ( xpic_code[0] == 0 ) && ( xpic_code[1] == 0 ) )
  {
    /* xPIC GNU Tool chain - segment definitions */
    NX6_XPIC_LoadSegments(xpic_code);
  }
  else
  {
    /* xPICASS generated program and trailing loads */
    programm_size       = xpic_code[0] / 4;  /* in Bytes / 4 */
    tl_size             = xpic_code[1] / 4;
    programm_start_adr  = (uint32_t)s_pulXpicPram;
    tl_start            = programm_size + 2;

    /* program loader */
    for (i = 1; i < programm_size; i++)
    {
      pulDst = (uint32_t*) (programm_start_adr + (i - 1)*4);
      NX_WRITE32(*pulDst, xpic_code[i + 2]);
    }

    /* trailing loader */
    for (i = 0; i < tl_size; i = i + 2)
    {
      ulAddress = xpic_code[tl_start + i];

      if( ulAddress >= Adr_NX51_xpic_dram_xpic_ram_start && (ulOffset=ulAddress-Adr_NX51_xpic_dram_xpic_ram_start)<NX6_XPIC_DWORD_RAMSIZE )
      {
        pulDst = s_pulXpicDram + ulOffset/sizeof(uint32_t);
      }
      else if( ulAddress >= Addr_NX51_xpic_regs && (ulOffset=ulAddress-Addr_NX51_xpic_regs)<NX6_XPIC_DWORD_REGAREASIZE )
      {
        pulDst = (uint32_t*)s_ptXpic + ulOffset/sizeof(uint32_t);
      }
      else
      {
        /* address is outside all ranges, cancel! */
        return;
      }

      NX_WRITE32(*pulDst, xpic_code[tl_start + i + 1]);
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
*   pvUser [in] User specific parameter
* \return
*                                                                            */
/*****************************************************************************/
void NX6_XPIC_Start( void* pvUser )
{
  NX_WRITE32(s_ptXpicDebug->ulXpic_hold_pc, 0);
}

/*****************************************************************************/
/*! Stop xPIC
* \description
*   Halt xPIC
* \class 
*   XPIC
* \params
*   pvUser [in] User specific parameter
* \return
*                                                                            */
/*****************************************************************************/
void NX6_XPIC_Stop( void* pvUser )
{
  NX_WRITE32(s_ptXpicDebug->ulXpic_hold_pc, MSK_NX51_xpic_hold_pc_hold);
}

/*****************************************************************************/
/*! Reset xPIC
* \description
*   Hold xPIC, reset xPIC state, 
*   clear work registers (both banks), clear user registers
* \class 
*   XPIC
* \params
*   pvUser [in] User specific parameter
* \return
*                                                                            */
/*****************************************************************************/
void NX6_XPIC_Reset( void* pvUser )
{
  uint32_t ulVal;
  
  /* hold xPIC */
  NX_WRITE32(s_ptXpicDebug->ulXpic_hold_pc, MSK_NX51_xpic_hold_pc_hold);
    
  /* clear all hold reasons */
  NX_WRITE32(s_ptXpicDebug->ulXpic_break_irq_raw, 0x1f);

  /* hold xPIC and request reset */
  NX_WRITE32(s_ptXpicDebug->ulXpic_hold_pc, MSK_NX51_xpic_hold_pc_hold | MSK_NX51_xpic_hold_pc_reset_xpic);
  
  /* wait for reset to be finished */
  do
  {
    ulVal = NX_READ32(s_ptXpicDebug->ulXpic_break_status);
  } while( (ulVal & MSK_NX51_xpic_break_status_xpic_reset_status) == 0 );
  
  /* release reset request, engage bank control, select bank 1 */
  NX_WRITE32(s_ptXpicDebug->ulXpic_hold_pc, MSK_NX51_xpic_hold_pc_hold | MSK_NX51_xpic_hold_pc_bank_control | MSK_NX51_xpic_hold_pc_bank_select);
  
  /* reset xPIC user registers */
  for( ulVal = 0; ulVal < 5; ulVal++ )
  {
    NX_WRITE32(s_ptXpic->aulXpic_usr[ulVal], 0);
  }
  
  /* reset xPIC work registers (bank 1) */
  for( ulVal = 0; ulVal < 8; ulVal++ )
  {
    NX_WRITE32(s_ptXpic->aulXpic_r[ulVal], 0);
  }
  
  /* select bank 0 */
  NX_WRITE32(s_ptXpicDebug->ulXpic_hold_pc, MSK_NX51_xpic_hold_pc_hold | MSK_NX51_xpic_hold_pc_bank_control);
  
  /* reset xPIC work registers (bank 0) */
  for( ulVal = 0; ulVal < 8; ulVal++ )
  {
    NX_WRITE32(s_ptXpic->aulXpic_r[ulVal], 0);
  }
  
  /* release bank control */
  NX_WRITE32(s_ptXpicDebug->ulXpic_hold_pc, MSK_NX51_xpic_hold_pc_hold);
}
