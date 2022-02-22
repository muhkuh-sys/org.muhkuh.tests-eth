#ifndef _HAL_RESOURCES_DEFINES_H
#define _HAL_RESOURCES_DEFINES_H

#include "regdef_netx50.h" /* used for structures */
#include "common_func_netx50.h" /* used for function names */
#include <string.h> /* used for memcpy() */

/*** ASIC Control area ***/
#define __USE_ASICCTRL static NX50_ASIC_CTRL_AREA_T* s_ptAsicCtrl = (NX50_ASIC_CTRL_AREA_T*) Addr_NX50_asic_ctrl;


/*** MIIMU area ***/
typedef int(*NX50_PFN_MIIMU_READ_PHY_REG) ( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuRtaField, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, unsigned short* pusData, void* pvUser );
typedef int(*NX50_PFN_MIIMU_WRITE_PHY_REG)( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, unsigned short usData, void* pvUser );

#define __USE_MIIMU                static NX50_MIIMU_AREA_T* s_ptMiimu = (NX50_MIIMU_AREA_T*)Addr_NX50_miimu;
#define __USE_MIIMU_FUNC_READ_PHY  static NX50_PFN_MIIMU_READ_PHY_REG  pfnMiimuReadPhyReg  = NX50_MIIMU_ReadPhyReg;
#define __USE_MIIMU_FUNC_WRITE_PHY static NX50_PFN_MIIMU_WRITE_PHY_REG pfnMiimuWritePhyReg = NX50_MIIMU_WritePhyReg;


/*** SYSTIME area ***/
typedef void(*NX50_PFN_SYSTIME_SET_BORDER)  ( unsigned long ulBorder, void* pvUser );
typedef void(*NX50_PFN_SYSTIME_SET_SPEED)   ( unsigned long ulSpeed, void* pvUser );
typedef void(*NX50_PFN_SYSTIME_SET_SYSTIME) ( unsigned long ulSystime_s, unsigned long ulSystime_ns, void* pvUser );
typedef void(*NX50_PFN_SYSTIME_GET_SYSTIME) ( unsigned long* pulSystime_s, unsigned long* pulSystime_ns, void* pvUser );

#define __USE_SYSTIME                  static NX50_SYSTIME_AREA_T* s_ptSystime = (NX50_SYSTIME_AREA_T*) Addr_NX50_systime;
#define __USE_SYSTIME_FUNC_SET_BORDER  static NX50_PFN_SYSTIME_SET_BORDER  pfnSystimeSetBorder   = NX50_SYSTIME_SetBorder;
#define __USE_SYSTIME_FUNC_SET_SPEED   static NX50_PFN_SYSTIME_SET_SPEED   pfnSystimeSetSpeed    = NX50_SYSTIME_SetSpeed;
#define __USE_SYSTIME_FUNC_SET_SYSTIME static NX50_PFN_SYSTIME_SET_SYSTIME pfnSystimeSetSystime  = NX50_SYSTIME_SetSystime;
#define __USE_SYSTIME_FUNC_GET_SYSTIME static NX50_PFN_SYSTIME_GET_SYSTIME pfnSystimeGetSystime  = NX50_SYSTIME_GetSystime;


/*** xPEC Register area ***/
#define __USE_XPEC_REGS                      \
  static NX50_XPEC_AREA_T* s_ptXpecStart[] = \
  {                                          \
    (NX50_XPEC_AREA_T*) Addr_NX50_xpec0,     \
    (NX50_XPEC_AREA_T*) Addr_NX50_xpec1,     \
  };


/*** xPEC PRAM area ***/
#define __USE_XPEC_PRAM                                  \
  static volatile unsigned long* s_pulXpecPramStart[] =  \
  {                                                      \
    (volatile unsigned long*) Adr_NX50_xpec0_pram_start, \
    (volatile unsigned long*) Adr_NX50_xpec1_pram_start, \
  };


/*** xPEC DRAM area ***/
#define __USE_XPEC_DRAM                                  \
  static volatile unsigned long* s_pulXpecDramStart[] =  \
  {                                                      \
    (volatile unsigned long*) Adr_NX50_xpec0_dram_start, \
    (volatile unsigned long*) Adr_NX50_xpec1_dram_start, \
  };


/*** xMAC area ***/
#define __USE_XMAC                           \
  static NX50_XMAC_AREA_T* s_ptXmacStart[] = \
  {                                          \
    (NX50_XMAC_AREA_T*) Addr_NX50_xmac0,     \
    (NX50_XMAC_AREA_T*) Addr_NX50_xmac1,     \
  };


/*** XC loader ***/
typedef int(*NX50_PFN_XC_RESET)( unsigned int uPortNo, void* pvUser );
typedef int(*NX50_PFN_XC_LOAD) ( unsigned int uPortNo, NX50_XC_TYPE_E eXcType, unsigned long* pulXcPrg, void* pvUser );
typedef int(*NX50_PFN_XC_START)( unsigned int uPortNo, void* pvUser );
  
#define __USE_XC_LOADER_FUNC_RESET static NX50_PFN_XC_RESET pfnXcReset = NX50_XC_Reset;
#define __USE_XC_LOADER_FUNC_LOAD  static NX50_PFN_XC_LOAD  pfnXcLoad  = NX50_XC_Load;
#define __USE_XC_LOADER_FUNC_START static NX50_PFN_XC_START pfnXcStart = NX50_XC_Start;


/*** Pointer FIFO area ***/
#define __USE_POINTERFIFO static NX50_POINTER_FIFO_AREA_T*  s_ptPFifo = (NX50_POINTER_FIFO_AREA_T*) Addr_NX50_pointer_fifo;


/*** xPEC IRQ area ***/
#define __USE_XPEC_IRQS     static NX50_XPEC_IRQ_REGISTERS_AREA_T* s_ptXpecIrqs    = (NX50_XPEC_IRQ_REGISTERS_AREA_T*) Addr_NX50_xpec_irq_registers;
#define __USE_XPEC_IRQ_REGS static NX50_XPEC_IRQ_REGISTERS_AREA_T* s_ptXpecIrqRegs = (NX50_XPEC_IRQ_REGISTERS_AREA_T*) Addr_NX50_xpec_irq_registers;


/*** INTRAM area ***/
#define __USE_INTRAM                                              \
  static volatile  unsigned long* s_pulIntRamStart[] =            \
  {                                                               \
    (volatile unsigned long*) Adr_NX50_sram0_ahbls0_intram0_base, \
    (volatile unsigned long*) Adr_NX50_sram1_ahbls1_intram1_base, \
  };


/*** TRIGGER_SAMPLE area ***/
#define __USE_TRIGGER_SAMPLE static NX50_TRIGGER_SAMPLE_UNIT_AREA_T* s_ptTriggerSample = (NX50_TRIGGER_SAMPLE_UNIT_AREA_T*) Addr_NX50_trigger_sample_unit;


/*** FMMUSM area ***/
#define __USE_FMMUSM static NX50_FMMUSM_AREA_T* s_ptFmmuSm = (NX50_FMMUSM_AREA_T*) Addr_NX50_fmmusm;


/*** BUFMAN area ***/
#define __USE_BUFMAN static NX50_BUF_MAN_AREA_T s_ptBufMan = (NX50_BUF_MAN_AREA_T*) Addr_NX50_buf_man;



/*** Access Macros ***/
#define NX_WRITE32(var,val) (var) = (val)
#define NX_READ32(var)      (var)
#define NX_WRITE16(var,val) (var) = (val)
#define NX_READ16(var)      (var)
#define NX_WRITE8(var,val)  (var) = (val)
#define NX_READ8(var)       (var)
/* Copy data from netX to Host */
#define NX_READMEM(dst, src, len) memcpy(dst, src, len)
/* Copy data from Host to netX */
#define NX_WRITEMEM(dst, src, len) memcpy(dst, src, len)

#define lock_irqfiq_save(x)    NX50_lock_irqfiq_save(x)
#define lock_irqfiq_restore(x) NX50_lock_irqfiq_restore(x)

#endif /* _HAL_RESOURCES_DEFINES_H */
