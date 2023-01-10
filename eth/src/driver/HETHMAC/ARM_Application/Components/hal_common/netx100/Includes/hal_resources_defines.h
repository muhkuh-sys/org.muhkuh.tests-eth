#ifndef _HAL_RESOURCES_DEFINES_H
#define _HAL_RESOURCES_DEFINES_H

#include "regdef_netx500.h" /* used for structures */
#include "common_func_netx500.h" /* used for function names */
#include <string.h> /* used for memcpy() */

/* *** ASIC Control area *** */
#define __USE_ASICCTRL static NX500_ASIC_CTRL_AREA_T* const s_ptAsicCtrl = (NX500_ASIC_CTRL_AREA_T*) Addr_NX500_asic_ctrl;


/* *** MIIMU area *** */
typedef int(*NX500_PFN_MIIMU_READ_PHY_REG) ( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuRtaField, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, unsigned short* pusData, void* pvUser );
typedef int(*NX500_PFN_MIIMU_WRITE_PHY_REG)( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, unsigned short usData, void* pvUser );

#define __USE_MIIMU                 static NX500_MIIMU_AREA_T* const s_ptMiimu = (NX500_MIIMU_AREA_T*)Addr_NX500_miimu;
#define __USE_MIIMU_FUNC_READ_PHY   static NX500_PFN_MIIMU_READ_PHY_REG  pfnMiimuReadPhyReg  = NX500_MIIMU_ReadPhyReg;
#define __USE_MIIMU_FUNC_WRITE_PHY  static NX500_PFN_MIIMU_WRITE_PHY_REG pfnMiimuWritePhyReg = NX500_MIIMU_WritePhyReg;


/* *** SYSTIME area *** */
typedef void(*NX500_PFN_SYSTIME_SET_BORDER)  ( unsigned long ulBorder, void* pvUser );
typedef void(*NX500_PFN_SYSTIME_SET_SPEED)   ( unsigned long ulSpeed, void* pvUser );
typedef void(*NX500_PFN_SYSTIME_SET_SYSTIME) ( unsigned long ulSystime_s, unsigned long ulSystime_ns, void* pvUser );
typedef void(*NX500_PFN_SYSTIME_GET_SYSTIME) ( unsigned long* pulSystime_s, unsigned long* pulSystime_ns, void* pvUser );

#define __USE_SYSTIME                  static NX500_SYSTIME_AREA_T* const s_ptSystime = (NX500_SYSTIME_AREA_T*)Addr_NX500_systime;
#define __USE_SYSTIME_FUNC_SET_BORDER  static NX500_PFN_SYSTIME_SET_BORDER  pfnSystimeSetBorder = NX500_SYSTIME_SetBorder;
#define __USE_SYSTIME_FUNC_SET_SPEED   static NX500_PFN_SYSTIME_SET_SPEED   pfnSystimeSetSpeed   = NX500_SYSTIME_SetSpeed;
#define __USE_SYSTIME_FUNC_SET_SYSTIME static NX500_PFN_SYSTIME_SET_SYSTIME pfnSystimeSetSystime = NX500_SYSTIME_SetSystime;
#define __USE_SYSTIME_FUNC_GET_SYSTIME static NX500_PFN_SYSTIME_GET_SYSTIME pfnSystimeGetSystime = NX500_SYSTIME_GetSystime;


/* *** xPEC Register area *** */
#define __USE_XPEC_REGS                        \
  static NX500_XPEC_AREA_T* const s_ptXpecStart[] =  \
  {                                            \
    (NX500_XPEC_AREA_T*) Addr_NX500_xpec0,     \
    (NX500_XPEC_AREA_T*) Addr_NX500_xpec1,     \
    (NX500_XPEC_AREA_T*) Addr_NX500_xpec2,     \
    (NX500_XPEC_AREA_T*) Addr_NX500_xpec3,     \
  };


/* *** xPEC RAM area *** */
#define __USE_XPEC_RAM                                    \
  static volatile unsigned long* s_pulXpecRamStart[] =    \
  {                                                       \
    (volatile unsigned long*) Adr_NX500_xpec0_ram_start,  \
    (volatile unsigned long*) Adr_NX500_xpec1_ram_start,  \
    (volatile unsigned long*) Adr_NX500_xpec2_ram_start,  \
    (volatile unsigned long*) Adr_NX500_xpec3_ram_start,  \
  };

#define __USE_XPEC_DRAM                                   \
  static volatile unsigned long* const s_pulXpecDramStart[] =   \
  {                                                       \
    (volatile unsigned long*) Adr_NX500_xpec0_ram_start,  \
    (volatile unsigned long*) Adr_NX500_xpec1_ram_start,  \
    (volatile unsigned long*) Adr_NX500_xpec2_ram_start,  \
    (volatile unsigned long*) Adr_NX500_xpec3_ram_start,  \
  };


/* *** xMAC area *** */
#define __USE_XMAC                              \
  static const NX500_XMAC_AREA_T* const s_ptXmacStart[] =   \
  {                                             \
    (NX500_XMAC_AREA_T*) Addr_NX500_xmac0,      \
    (NX500_XMAC_AREA_T*) Addr_NX500_xmac1,      \
    (NX500_XMAC_AREA_T*) Addr_NX500_xmac2,      \
    (NX500_XMAC_AREA_T*) Addr_NX500_xmac3       \
  };


/* *** XC loader *** */
typedef int(*NX500_PFN_NX500_XC_RESET)( unsigned int uPortNo, void* pvUser );
typedef int(*NX500_PFN_NX500_XC_RESET_UNITS)( unsigned int uPortNo, unsigned int uUnitVec, void* pvUser );
typedef int(*NX500_PFN_NX500_XC_LOAD) ( unsigned int uPortNo, NX500_XC_TYPE_E eXcType, unsigned long* pulXcPrg, void* pvUser );
typedef int(*NX500_PFN_NX500_XC_START)( unsigned int uPortNo, void* pvUser );
typedef int(*NX500_PFN_NX500_XC_START_UNITS)( unsigned int uPortNo, unsigned int uUnitVec, void* pvUser );

#define __USE_XC_LOADER_FUNC_RESET       static const NX500_PFN_NX500_XC_RESET       pfnXcReset      = NX500_XC_Reset;
#define __USE_XC_LOADER_FUNC_RESET_UNITS static NX500_PFN_NX500_XC_RESET_UNITS pfnXcResetUnits = NX500_XC_ResetUnits;
#define __USE_XC_LOADER_FUNC_LOAD        static const NX500_PFN_NX500_XC_LOAD        pfnXcLoad       = NX500_XC_Load;
#define __USE_XC_LOADER_FUNC_START       static const NX500_PFN_NX500_XC_START       pfnXcStart      = NX500_XC_Start;
#define __USE_XC_LOADER_FUNC_START_UNITS static NX500_PFN_NX500_XC_START_UNITS pfnXcStartUnits = NX500_XC_StartUnits;


/* *** Pointer FIFO area *** */
#define __USE_POINTERFIFO static NX500_POINTER_FIFO_AREA_T* const s_ptPFifo = (NX500_POINTER_FIFO_AREA_T*) Addr_NX500_pointer_fifo;


/* *** xPEC IRQ area *** */
#define  __USE_XPEC_IRQS     static NX500_XPEC_IRQ_REGISTERS_AREA_T* const s_ptXpecIrqs = (NX500_XPEC_IRQ_REGISTERS_AREA_T*) Addr_NX500_xpec_irq_registers;
#define  __USE_XPEC_IRQ_REGS static NX500_XPEC_IRQ_REGISTERS_AREA_T* const s_ptXpecIrqRegs = (NX500_XPEC_IRQ_REGISTERS_AREA_T*) Addr_NX500_xpec_irq_registers;


/* *** INTRAM area *** */
#define __USE_INTRAM                                      \
  static volatile  unsigned long* const s_pulIntRamStart[4] =   \
  {                                                       \
    (volatile unsigned long*) Addr_NX500_sram0_ahbls0,    \
    (volatile unsigned long*) Addr_NX500_sram1_ahbls1,    \
    (volatile unsigned long*) Addr_NX500_sram2_ahbls2,    \
    (volatile unsigned long*) Addr_NX500_sram3_ahbls3,    \
  };

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

#define lock_irqfiq_save(x)    NX500_lock_irqfiq_save(x)
#define lock_irqfiq_restore(x) NX500_lock_irqfiq_restore(x)


#endif /* _HAL_RESOURCES_DEFINES_H */
