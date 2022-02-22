#ifndef __HAL_GIC_H
#define __HAL_GIC_H

#include <stdint.h>
#include <stdbool.h>

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

/* Function pointer */
typedef void (*GIC_ISR_FN)(uint32_t ulParameter);

/* Result codes for functions */
typedef enum
{
  GIC_OKAY = 0,           /* Successful */
  GIC_ERR_INVAL_PARAM = 1 /* Invalid parameter */
} GIC_RESULT_E;

/* Interrupt controller interface registers */
#define GIC_CONTROLLER_PRIORITY_MASK_SUPPORT_128_LEVELS                                              0xFE /* Bit[0] = 0b0 -> 128 supported levels */
#define GIC_CONTROLLER_PRIORITY_MASK_SUPPORT_64_LEVELS                                               0xFC /* Bit[1:0] = 0b00 -> 64 supported levels */
#define GIC_CONTROLLER_PRIORITY_MASK_SUPPORT_32_LEVELS                                               0xF8 /* Bit[2:0] = 0b000 -> 32 supported levels */
#define GIC_CONTROLLER_PRIORITY_MASK_SUPPORT_16_LEVELS                                               0xF0 /* Bit[3:0] = 0b0000 -> 16 supported levels */
#define GIC_CONTROLLER_PRIORITY_MASK_HIGHEST_PRIORITY                                                0x00 /* Highest priority */
#define GIC_CONTROLLER_PRIORITY_MASK_LOWEST_PRIORITY                                                 0x80 /* Lowest priority */

/* Interrupt distributor interface registers */
#define GIC_DISTRIBUTOR_INT_SET_ENABLE_INTERRUPT                                                      0x1 /* Set enable interrupt */
#define GIC_DISTRIBUTOR_INT_CLEAR_ENABLE_INTERRUPT                                                    0x0 /* Clear enable interrupt */

#define GIC_DISTRIBUTOR_INT_PRIORITY_LOWEST_CONFIG                                                   0x00 /* Lower configuration border */
#define GIC_DISTRIBUTOR_INT_PRIORITY_HIGHEST_CONFIG                                                  0xFF /* Upper configuration border */

/* Generic values */
#define GIC_CPU_INTERRUPT_ID_LOWER_BORDER                                                                       31 /* Interrupt IDs 00..31 are reserved - SPI 00..31 */
#define GIC_CPU_INTERRUPT_ID_UPPER_BORDER                                                                      511 /* Interrupt IDs 32..511 can be used - PPI 00..479 */
#define GIC_TOTAL_NUMBER_OF_INTERRUPTS                                                                         511 /* 000...511 */
#define GIC_NO_INTERRUPT_IS_PENDING                                                                          0x3ff /* No interrupt is pending */
#define GIC_SET_BIT                                                                                            0x1 /* Set one bit */
#define GIC_ENABLE                                                                                             0x1 /* Generic enable bit */
#define GIC_DISABLE                                                                                            0x0 /* Generic disable bit */

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

#ifndef HALDEC
#define HALDEC(name) name
#endif

void                HALDEC(Gic_CpuiInit)                          (void);
void                HALDEC(Gic_CpuiReset)                         (void);
GIC_RESULT_E        HALDEC(Gic_CpuiSetPriorityMask)               (uint32_t ulPriorityMsk);
uint32_t            HALDEC(Gic_CpuiHighestPendingIrq)             (void);

void                HALDEC(Gic_DistInit)                          (void);
void                HALDEC(Gic_DistReset)                         (void);
GIC_RESULT_E        HALDEC(Gic_DistSetEnable)                     (unsigned int uIrqNo, bool fEnable);
GIC_RESULT_E        HALDEC(Gic_DistSetPending)                    (unsigned int uIrqNo, bool fEnable);
GIC_RESULT_E        HALDEC(Gic_DistSetPriority)                   (unsigned int uIrqNo, uint32_t ulPriority);
GIC_RESULT_E        HALDEC(Gic_DistSetProcessorTarget)            (unsigned int uIrqNo, uint32_t ulTargetCoreNum);
GIC_RESULT_E        HALDEC(Gic_DistCfg)                           (unsigned int uIrqNo, bool fEdge);
GIC_RESULT_E        HALDEC(Gic_DistSetIsr)                        (unsigned int uIrqNo, uint32_t ulIsrAdr);
bool                HALDEC(Gic_DistIsPending)                     (unsigned int uIrqNo);
bool                HALDEC(Gic_DistIsActive)                      (unsigned int uIrqNo);

GIC_RESULT_E        HALDEC(Gic_DefaultIrqHandler)                 (void);

#endif /* #ifndef __HAL_GIC_H */
