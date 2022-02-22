#ifndef _HAL_RESOURCES_DEFINES_NETX5_H
#define _HAL_RESOURCES_DEFINES_NETX5_H

#include "regdef_netx5.h" // used for structures
#include "dpm_mapping_netx5.h" // used for DPM mapping
#include "common_func_netx5.h" // used for function names 
#include "common_func_netx50.h" // used for function names
#include <string.h> /* used for memcpy() */

// *** ASIC Control area ***
#define __USE_ASICCTRL static NX5_ASIC_CTRL_AREA_T* const s_ptAsicCtrl = (NX5_ASIC_CTRL_AREA_T*)(HOST_DPM_START_ADDR + NX5_DPM_OFFSET_ASIC_CTRL);


// *** MIIMU area ***
typedef int(*NX5_PFN_MIIMU_READ_PHY_REG) ( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuRtaField, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, unsigned short* pusData, void* pvUser );
typedef int(*NX5_PFN_MIIMU_WRITE_PHY_REG)( unsigned int uMiimuPreamble, unsigned int uMiimuMdcFreq, unsigned int uMiimuPhyAddr, unsigned int uMiimuReqAddr, unsigned short usData, void* pvUser );

#define __USE_MIIMU                static NX5_MIIMU_AREA_T* const s_ptMiimu = (NX5_MIIMU_AREA_T*)(HOST_DPM_START_ADDR + NX5_DPM_OFFSET_MIIMU);
#define __USE_MIIMU_FUNC_READ_PHY  static NX5_PFN_MIIMU_READ_PHY_REG  pfnMiimuReadPhyReg  = NX5_MIIMU_ReadPhyReg;
#define __USE_MIIMU_FUNC_WRITE_PHY static NX5_PFN_MIIMU_WRITE_PHY_REG pfnMiimuWritePhyReg = NX5_MIIMU_WritePhyReg;


// *** EXTPHY ***
typedef int(*NX5_PFN_EXTPHY_GETLINKSTATE) ( unsigned int uPortNo, unsigned int* puLink, unsigned int* puMbps, unsigned int* puFullDuplex, void* pvUser );
#define __USE_EXTPHY_FUNC_GETLINKSTATE static NX5_PFN_EXTPHY_GETLINKSTATE pfnExtphyGetLinkState = NX5_EXTPHY_GetLinkState;


// *** SYSTIME area ***
typedef void(*NX5_PFN_SYSTIME_SET_BORDER)  ( unsigned long ulBorder, void* pvUser );
typedef void(*NX5_PFN_SYSTIME_SET_SPEED)   ( unsigned long ulSpeed, void* pvUser );
typedef void(*NX5_PFN_SYSTIME_SET_SYSTIME) ( unsigned long ulSystime_s, unsigned long ulSystime_ns, void* pvUser );
typedef void(*NX5_PFN_SYSTIME_GET_SYSTIME) ( unsigned long* pulSystime_s, unsigned long* pulSystime_ns, void* pvUser );

#define __USE_SYSTIME                  static NX5_SYSTIME_AREA_T* const s_ptSystime = (NX5_SYSTIME_AREA_T*)(HOST_DPM_START_ADDR + NX5_DPM_OFFSET_SYSTIME);
#define __USE_SYSTIME_FUNC_SET_BORDER  static NX5_PFN_SYSTIME_SET_BORDER  pfnSystimeSetBorder   = NX5_SYSTIME_SetBorder;
#define __USE_SYSTIME_FUNC_SET_SPEED   static NX5_PFN_SYSTIME_SET_SPEED   pfnSystimeSetSpeed    = NX5_SYSTIME_SetSpeed;
#define __USE_SYSTIME_FUNC_SET_SYSTIME static NX5_PFN_SYSTIME_SET_SYSTIME pfnSystimeSetSystime  = NX5_SYSTIME_SetSystime;
#define __USE_SYSTIME_FUNC_GET_SYSTIME static NX5_PFN_SYSTIME_GET_SYSTIME pfnSystimeGetSystime  = NX5_SYSTIME_GetSystime;


// *** xPEC Register area ***
#define __USE_XPEC_REGS                                                  \
  static NX5_XPEC_AREA_T* s_ptXpecStart[] =                              \
  {                                                                      \
    (NX5_XPEC_AREA_T*)(HOST_DPM_START_ADDR + NX5_DPM_OFFSET_XPEC0_REG),  \
    (NX5_XPEC_AREA_T*)(HOST_DPM_START_ADDR + NX5_DPM_OFFSET_XPEC1_REG)   \
  };


// *** xPEC PRAM area ***
#define __USE_XPEC_PRAM                                                         \
  static volatile uint32_t* s_pulXpecPramStart[] =                         \
  {                                                                             \
    (volatile uint32_t*)(HOST_DPM_START_ADDR + NX5_DPM_OFFSET_XPEC0_PRAM), \
    (volatile uint32_t*)(HOST_DPM_START_ADDR + NX5_DPM_OFFSET_XPEC1_PRAM)  \
  };


// *** xPEC DRAM area ***
#define __USE_XPEC_DRAM                                                         \
  static volatile uint32_t* s_pulXpecDramStart[] =                         \
  {                                                                             \
    (volatile uint32_t*)(HOST_DPM_START_ADDR + NX5_DPM_OFFSET_XPEC0_DRAM), \
    (volatile uint32_t*)(HOST_DPM_START_ADDR + NX5_DPM_OFFSET_XPEC1_DRAM)  \
  };


// *** xMAC area ***
#define __USE_XMAC                                                       \
  static NX5_XMAC_AREA_T* s_ptXmacStart[] =                              \
  {                                                                      \
    (NX5_XMAC_AREA_T*)(HOST_DPM_START_ADDR + NX5_DPM_OFFSET_XMAC0),      \
    (NX5_XMAC_AREA_T*)(HOST_DPM_START_ADDR + NX5_DPM_OFFSET_XMAC1)       \
  };


// *** XC loader ***
typedef int(*NX5_PFN_XC_RESET)(unsigned int uPortNo, void* pvUser);
typedef int(*NX5_PFN_XC_LOAD) (unsigned int uPortNo, NX5_XC_TYPE_E eXcType, unsigned long* pulXcPrg, void* pvUser);
typedef int(*NX5_PFN_XC_START)(unsigned int uPortNo, void* pvUser);
typedef int(*NX5_PFN_XC_START_UNITS)(unsigned int uPortNo, unsigned int uUnitVec, void* pvUser);

#define __USE_XC_LOADER_FUNC_RESET       static NX5_PFN_XC_RESET       pfnXcReset      = NX5_XC_Reset;
#define __USE_XC_LOADER_FUNC_LOAD        static NX5_PFN_XC_LOAD        pfnXcLoad       = NX5_XC_Load;
#define __USE_XC_LOADER_FUNC_START       static NX5_PFN_XC_START       pfnXcStart      = NX5_XC_Start;
#define __USE_XC_LOADER_FUNC_START_UNITS static NX5_PFN_XC_START_UNITS pfnXcStartUnits = NX5_XC_StartUnits;


// *** Pointer FIFO area ***
#define __USE_POINTERFIFO static NX5_POINTER_FIFO_AREA_T* const s_ptPFifo = (NX5_POINTER_FIFO_AREA_T*)(HOST_DPM_START_ADDR + NX5_DPM_OFFSET_POINTER_FIFO);


// *** xPEC IRQ area ***
#define __USE_XPEC_IRQS     static NX5_XPEC_IRQ_REGISTERS_AREA_T* s_ptXpecIrqs    = (NX5_XPEC_IRQ_REGISTERS_AREA_T*)(HOST_DPM_START_ADDR + NX5_DPM_OFFSET_XPEC_IRQ_REGISTERS);
#define __USE_XPEC_IRQ_REGS static NX5_XPEC_IRQ_REGISTERS_AREA_T* s_ptXpecIrqRegs = (NX5_XPEC_IRQ_REGISTERS_AREA_T*)(HOST_DPM_START_ADDR + NX5_DPM_OFFSET_XPEC_IRQ_REGISTERS);


// *** INTRAM area ***
#define __USE_INTRAM                                                      \
  static volatile uint32_t* s_pulIntRamStart[] =                          \
  {                                                                       \
    (volatile uint32_t*)(HOST_DPM_START_ADDR + NX5_DPM_OFFSET_INTRAM0),   \
    (volatile uint32_t*)(HOST_DPM_START_ADDR + NX5_DPM_OFFSET_INTRAM1)    \
  };

// *** TRIGGER_SAMPLE area ***
#define __USE_TRIGGER_SAMPLE static NX5_TRIGGER_SAMPLE_UNIT_AREA_T* const s_ptTriggerSample = (NX5_TRIGGER_SAMPLE_UNIT_AREA_T*)(HOST_DPM_START_ADDR + NX5_DPM_OFFSET_TRIGGER_SAMPLE_UNIT);


// *** FMMUSM area ***
#define __USE_FMMUSM static NX5_FMMUSM_AREA_T* const s_ptFmmuSm = (NX5_FMMUSM_AREA_T*)(HOST_DPM_START_ADDR + NX5_DPM_OFFSET_FMMUSM);


// *** BUFMAN area ***
#define __USE_BUFMAN static NX5_BUF_MAN_AREA_T* const s_ptBufMan = (NX5_BUF_MAN_AREA_T*)(HOST_DPM_START_ADDR + NX5_DPM_OFFSET_BUF_MAN);


// *** DPM area ***
#define __USE_DPM static NX5_DPM_AREA_T* const s_ptDpm = (NX5_DPM_AREA_T*)(HOST_DPM_START_ADDR + NX5_DPM_OFFSET_FIX_DPM_CFG_WINDOW);


// *** PIO area ***
#define __USE_PIO static NX5_PIO_AREA_T* const s_ptPio = (NX5_PIO_AREA_T*)(HOST_DPM_START_ADDR + NX5_DPM_OFFSET_PIO);


// *** SPI area ***
#define __USE_SPI static NX5_SPI_AREA_T* const s_ptSpi = (NX5_SPI_AREA_T*)(HOST_DPM_START_ADDR + NX5_DPM_OFFSET_SPI);


// *** ZPU area ***
#define __USE_ZPU static NX5_ZPU_AREA_T* const s_ptZpu = (NX5_ZPU_AREA_T*)(HOST_DPM_START_ADDR + NX5_DPM_OFFSET_ZPU);

/*
 ____                         _   _          _     ____    ____    __  __
|  _ \   __ _   _ __   __ _  | | | |   ___  | |   |  _ \  |  _ \  |  \/  |
| |_) | / _` | | '__| / _` | | | | |  / _ \ | |   | | | | | |_) | | |\/| |
|  __/ | (_| | | |   | (_| | | | | | |  __/ | |   | |_| | |  __/  | |  | |
|_|     \__,_| |_|    \__,_| |_| |_|  \___| |_|   |____/  |_|     |_|  |_|

*/

#ifdef NX5_PARALLEL_DPM

/***************************************/
/* 32 bit access in full address range */
/***************************************/
#define NX_WRITE32_XPEC(var, val)         NX50_ParDPM_Write32_XPEC_FullRange((unsigned long)&(var), val)
#define NX_READ32_XPEC(var)               NX50_ParDPM_Read32_XPEC_FullRange((unsigned long)&(var))
#define NX_WRITE32(var, val)              (var) = (val)
#define NX_READ32(var)                    (var)

/***************************************/
/* 16 bit access in full address range */
/***************************************/
#define NX_WRITE16_XPEC(var, val)         NX50_ParDPM_Write16_XPEC_FullRange((unsigned long)&(var), val)
#define NX_READ16_XPEC(var)               NX50_ParDPM_Read16_XPEC_FullRange((unsigned long)&(var))
#define NX_WRITE16(var, val)              (var) = (val)
#define NX_READ16(var)                    (var)

/***************************************/
/* 8 bit access in full address range  */
/***************************************/
#define NX_WRITE8_XPEC(var, val)          NX50_ParDPM_Write8_XPEC_FullRange((unsigned long)&(var), val)
#define NX_READ8_XPEC(var)                NX50_ParDPM_Read8_XPEC_FullRange((unsigned long)&(var))
#define NX_WRITE8(var, val)               (var) = (val)
#define NX_READ8(var)                     (var)

/***************************************/
/* 8 bit access in small address range
 * shall not be used for parallel DPM  */
/***************************************/
#define NX_WRITE8_SR                      NX_WRITE8
#define NX_READ8_SR                       NX_READ8

/* Copy data from netX to Host */
#define NX_READMEM(dst, src, len) memcpy(dst, src, len)
/* Copy data from Host to netX */
#define NX_WRITEMEM(dst, src, len) memcpy(dst, src, len)

#endif  // NX5_PARALLEL_DPM


/*
 ____                  _           _     ____    ____    __  __
/ ___|    ___   _ __  (_)   __ _  | |   |  _ \  |  _ \  |  \/  |
\___ \   / _ \ | '__| | |  / _` | | |   | | | | | |_) | | |\/| |
 ___) | |  __/ | |    | | | (_| | | |   | |_| | |  __/  | |  | |
|____/   \___| |_|    |_|  \__,_| |_|   |____/  |_|     |_|  |_|

*/

#ifdef NX5_SERIAL_DPM

/***************************************/
/* 32 bit access in full address range */
/***************************************/
#define NX_WRITE32_XPEC(var, val)     (NX50_SerDPM_Write32_XPEC_FullRange((unsigned long)&(var), val))
#define NX_READ32_XPEC(var)           (NX50_SerDPM_Read32_XPEC_FullRange((unsigned long)&(var)))
#define NX_WRITE32(var, val)          (NX50_SerDPM_Write32_nonXPEC_FullRange((unsigned long)&(var), val))
#define NX_READ32(var)                (NX50_SerDPM_Read32_nonXPEC_FullRange((unsigned long)&(var)))

/***************************************/
/* 8 bit access in full address range  */
/***************************************/
#define NX_WRITE8_XPEC(var, val)      (NX50_SerDPM_Write8_XPEC_FullRange((unsigned long)&(var), val))
#define NX_READ8_XPEC(var)            (NX50_SerDPM_Read8_XPEC_FullRange((unsigned long)&(var)))
#define NX_WRITE8(var, val)           (NX50_SerDPM_Write8_nonXPEC_FullRange((unsigned long)&(var), val))
#define NX_READ8(var)                 (NX50_SerDPM_Read8_nonXPEC_FullRange((unsigned long)&(var)))

/***************************************/
/* 8 bit access in small address range */
/***************************************/
#define NX_WRITE8_SR(var, val)        (NX50_SerDPM_Write8_SmallRange((unsigned long)&(var), val))
#define NX_READ8_SR(var)              (NX50_SerDPM_Read8_SmallRange((unsigned long)&(var)))

#endif  // NX5_SERIAL_DPM

#define lock_irqfiq_save(x)    NX50_lock_irqfiq_save(x)
#define lock_irqfiq_restore(x) NX50_lock_irqfiq_restore(x)

#endif /* _HAL_RESOURCES_DEFINES_NETX5_H */
