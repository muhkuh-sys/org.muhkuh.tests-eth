#ifndef __DPM_MAPPING_NETX_H__
#define __DPM_MAPPING_NETX_H__
/* NX5 DPM Mode */

/* bilsen: DPM Mode should be set outside this file, e.g. in the project's preprocessor definitions */
#if !defined(NX5_SERIAL_DPM) && !defined(NX5_PARALLEL_DPM)
#define NX5_PARALLEL_DPM   /* NX5_PARALLEL_DPM or NX5_SERIAL_DPM */
#endif

#ifdef NX5_PARALLEL_DPM
/* start address of DPM within address area of host processor */
#define HOST_DPM_START_ADDR              0x30000000
#endif

#ifdef NX5_SERIAL_DPM
#define HOST_DPM_START_ADDR              0
#endif

#include "dpm_func_netx50.h"

/* netX 5 address windows */
/* Window 0 */
#define NX5_DPM_OFFSET_FIX_DPM_CFG_WINDOW   0x00000
/* Window 1 */
#define NX5_DPM_OFFSET_ASIC_CTRL            0x00100
#define NX5_DPM_OFFSET_ABORT                0x00200
#define NX5_DPM_OFFSET_ZPU                  0x00240
#define NX5_DPM_OFFSET_PIO                  0x00400
#define NX5_DPM_OFFSET_MIIMU                0x00500
#define NX5_DPM_OFFSET_SPI                  0x00600
#define NX5_DPM_OFFSET_SYSTIME              0x00700
#define NX5_DPM_OFFSET_DPM                  0x00800
#define NX5_DPM_OFFSET_XPEC0_REG            0x00900
/* Window 2 */
#define NX5_DPM_OFFSET_XPEC0_PRAM           0x01000
#define NX5_DPM_OFFSET_XPEC0_DRAM           0x03000
#define NX5_DPM_OFFSET_XPEC1_REG            0x05000
/* Window 3 */
#define NX5_DPM_OFFSET_XPEC1_PRAM           0x06000
#define NX5_DPM_OFFSET_XPEC1_DRAM           0x08000
/* Window 4 */
#define NX5_DPM_OFFSET_XMAC0                0x0A000
#define NX5_DPM_OFFSET_XMAC1                0x0B000
#define NX5_DPM_OFFSET_POINTER_FIFO         0x0E000
#define NX5_DPM_OFFSET_XPEC_IRQ_REGISTERS   0x0E400
#define NX5_DPM_OFFSET_XC_MEM_PROT          0x0E800
#define NX5_DPM_OFFSET_XC_DEBUG             0x0EC00
#define NX5_DPM_OFFSET_TRIGGER_SAMPLE_UNIT  0x0F000
#define NX5_DPM_OFFSET_FMMUSM               0x0F400
#define NX5_DPM_OFFSET_BUF_MAN              0x0F600
/* Window 5 */
#define NX5_DPM_OFFSET_INTRAM0              0x10000
#define NX5_DPM_OFFSET_INTRAM1              0x18000


#define DUMMY_READ_ADDR  (HOST_DPM_START_ADDR + NX5_DPM_OFFSET_INTRAM0)

#endif /* __DPM_MAPPING_NETX_H__ */
