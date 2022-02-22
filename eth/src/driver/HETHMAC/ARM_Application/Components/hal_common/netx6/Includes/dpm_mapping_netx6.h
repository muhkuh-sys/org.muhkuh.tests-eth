#ifndef __DPM_MAPPING_NETX_H__
#define __DPM_MAPPING_NETX_H__

/* netX 6 address windows */
/* --- Window 0 (DWORD-access) --- */
#define NX6_DPM_OFFSET_FIX_DPM_CFG_WINDOW   0x00000

/* --- Window 1 (DWORD-access) --- */
/* CS_XPIC_PRAM */
#define NX6_DPM_OFFSET_XPIC_PRAM            0x00100 /* 8192 */
/* CS_RPEC0_PRAM */
#define NX6_DPM_OFFSET_RPEC0_PRAM           0x02100 /* 8192 */
/* CS_TPEC0_RAM */
#define NX6_DPM_OFFSET_TPEC0_PRAM           0x04100 /* 8192 */
/* CS_RPEC1_PRAM */
#define NX6_DPM_OFFSET_RPEC1_PRAM           0x06100 /* 8192 */
/* CS_TPEC1_RAM */
#define NX6_DPM_OFFSET_TPEC1_PRAM           0x08100 /* 8192 */
/* CS_XMAC_RAMS */
#define NX6_DPM_OFFSET_RPU0_PRAM            0x0a100 /* 1024 */
#define NX6_DPM_OFFSET_TPU0_PRAM            0x0a500 /* 1024 */
#define NX6_DPM_OFFSET_RPU1_PRAM            0x0a900 /* 1024 */
#define NX6_DPM_OFFSET_TPU1_PRAM            0x0ad00 /* 1024 */
/* CS_XC_REGS */
#define NX6_DPM_OFFSET_RPEC0_REG            0x0B100 /*  256 */
#define NX6_DPM_OFFSET_TPEC0_REG            0x0B200 /*  256 */
#define NX6_DPM_OFFSET_RPEC1_REG            0x0B300 /*  256 */
#define NX6_DPM_OFFSET_TPEC1_REG            0x0B400 /*  256 */
#define NX6_DPM_OFFSET_XMAC0_REG            0x0B500 /*  512 */
#define NX6_DPM_OFFSET_XMAC1_REG            0x0B700 /*  512 */
#define NX6_DPM_OFFSET_POINTER_FIFO         0x0B900 /*  512 */
#define NX6_DPM_OFFSET_FMMUSM               0x0BB00 /*  512 */
#define NX6_DPM_OFFSET_UNUSED1              0x0BD00 /*  256 */
#define NX6_DPM_OFFSET_TRIGGER_SAMPLE_UNIT  0x0BE00 /*  256 */
#define NX6_DPM_OFFSET_BUF_MAN              0x0BF00 /*   32 */
#define NX6_DPM_OFFSET_XPEC_IRQ_REGISTERS   0x0BF20 /*   16 */
#define NX6_DPM_OFFSET_XC_DEBUG             0x0BF30 /*   16 */
#define NX6_DPM_OFFSET_XC_START_STOP        0x0BF40 /*   16 */
#define NX6_DPM_OFFSET_UNUSED2              0x0BF50 /*  432 */
/* several CS in INTLOGIC / CS_XPIC_REGS */
#define NX6_DPM_OFFSET_DPM                  0x0C100 /*  256 */
#define NX6_DPM_OFFSET_ASIC_CTRL            0x0C200 /*  256 */
#define NX6_DPM_OFFSET_MMIO_CTRL            0x0C300 /*  256 */
#define NX6_DPM_OFFSET_HANDSHAKE_CTRL       0x0C400 /*  256 */
#define NX6_DPM_OFFSET_XPIC_REGS            0x0C500 /*  128 */
#define NX6_DPM_OFFSET_XPIC_DEBUG           0x0C580 /*  128 */
#define NX6_DPM_OFFSET_INT_PHY_CTRL_0       0x0C600 /*   16 */
#define NX6_DPM_OFFSET_INT_PHY_CTRL_1       0x0C610 /*   16 */
#define NX6_DPM_OFFSET_MIIMU                0x0C620 /*   16 */
#define NX6_DPM_OFFSET_PIO                  0x0C630 /*   16 */
#define NX6_DPM_OFFSET_HIF_IO_CTRL          0x0C640 /*   64 */
#define NX6_DPM_OFFSET_CRC                  0x0C680 /*   16 */
#define NX6_DPM_OFFSET_SYSTIME              0x0C690 /*   16 */
#define NX6_DPM_OFFSET_SYSTIME_UC           0x0C6A0 /*   16 */
#define NX6_DPM_OFFSET_WATCHDOG             0x0C6B0 /*   16 */
#define NX6_DPM_OFFSET_SQI                  0x0C6C0 /*   64 */
#define NX6_DPM_OFFSET_UART0                0x0C700 /*   64 */
#define NX6_DPM_OFFSET_UART1                0x0C740 /*   64 */
#define NX6_DPM_OFFSET_UART2                0x0C780 /*   64 */
#define NX6_DPM_OFFSET_I2C0                 0x0C7C0 /*   64 */
#define NX6_DPM_OFFSET_I2C1                 0x0C800 /*   64 */
#define NX6_DPM_OFFSET_UNUSED3              0x0C840 /*  192 */
#define NX6_DPM_OFFSET_USB_DEV              0x0C900 /*  256 */
#define NX6_DPM_OFFSET_CAN_CTRL             0x0CA00 /*  256 */
#define NX6_DPM_OFFSET_GPIO                 0x0CB00 /*  512 */
#define NX6_DPM_OFFSET_UNUSED4              0x0CD00 /* 1024 */
#define NX6_DPM_OFFSET_DMAC                 0x0D100 /*  256 */
#define NX6_DPM_OFFSET_DMAC_CH0             0x0D200 /*   32 */
#define NX6_DPM_OFFSET_DMAC_CH1             0x0D220 /*   32 */
#define NX6_DPM_OFFSET_DMAC_CH2             0x0D240 /*   32 */
#define NX6_DPM_OFFSET_UNUSED5              0x0D260 /* 1696 */
#define NX6_DPM_OFFSET_DMAC_REG             0x0D900 /* 2048 */

/* --- Window 2 (DWORD access) --- */
/* CS_XPIC_DRAM */
#define NX6_DPM_OFFSET_XPIC_DRAM            0x0E100 /* 8192 */
/* CS_RPEC0_DRAM */
#define NX6_DPM_OFFSET_RPEC0_DRAM           0x10100 /* 8192 */
/* CS_TPEC0_DAM */
#define NX6_DPM_OFFSET_TPEC0_DRAM           0x12100 /* 8192 */
/* CS_RPEC1_DRAM */
#define NX6_DPM_OFFSET_RPEC1_DRAM           0x14100 /* 8192 */
/* CS_TPEC1_DAM */
#define NX6_DPM_OFFSET_TPEC1_DRAM           0x16100 /* 8192 */

/* --- Window 3 (BYTE access) --- */
#define NX6_DPM_OFFSET_INTRAM7              0x18100 /* 65536 */
#define NX6_DPM_OFFSET_INTRAM8              0x28100 /* 65536 */

/* --- Window 4 (BYTE access) --- */
#define NX6_DPM_OFFSET_INTRAM6              0x38100 /* 32512 */

/* --- Fin --- */
#define NX6_DPM_OFFSET_FIN                  0x40000


#define DUMMY_READ_ADDR  (HOST_DPM_START_ADDR + NX6_DPM_OFFSET_INTRAM0)


#endif /* __DPM_MAPPING_NETX_H__ */
