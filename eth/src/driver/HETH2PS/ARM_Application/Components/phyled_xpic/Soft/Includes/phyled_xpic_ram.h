/*********************************************************************/
/* from regdef.v by regdef2h auto-generated C-header file            */
/* please see the regdef.html file for detailed register description */
/*********************************************************************/

#ifndef PHYLED_XPIC_RAM_regdef_h
#define PHYLED_XPIC_RAM_regdef_h

#include <stdint.h>

/* ===================================================================== */

/* Area of PHYLED_DRAM */

/* ===================================================================== */


/* --------------------------------------------------------------------- */
/* Register AREA_DRAM_CONSTANTPOOL */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_AREA_DRAM_CONSTANTPOOL         0x00000000U

/* --------------------------------------------------------------------- */
/* Register AREA_DRAM_GLOBAL_CFG */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_AREA_DRAM_GLOBAL_CFG         0x00000200U

/* --------------------------------------------------------------------- */
/* Register DRAM_POLL_INTERVAL_10NS */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_POLL_INTERVAL_10NS         0x00000200U

#define MSK_DRAM_POLL_INTERVAL_10NS_VAL 0xffffffffU
#define SRT_DRAM_POLL_INTERVAL_10NS_VAL 0

/* all used bits of 'DRAM_POLL_INTERVAL_10NS': */
#define MSK_USED_BITS_DRAM_POLL_INTERVAL_10NS 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register AREA_DRAM_PORT0 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_AREA_DRAM_PORT0         0x00000204U

#define MSK_AREA_DRAM_PORT0_RES1 0x00000003U
#define SRT_AREA_DRAM_PORT0_RES1 0
#define MSK_AREA_DRAM_PORT0_MODE 0x0000000cU
#define SRT_AREA_DRAM_PORT0_MODE 2
#define MSK_AREA_DRAM_PORT0_RES2 0xfffffff0U
#define SRT_AREA_DRAM_PORT0_RES2 4

/* all used bits of 'AREA_DRAM_PORT0': */
#define MSK_USED_BITS_AREA_DRAM_PORT0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_LED_UPDATE_INTERVAL0 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_LED_UPDATE_INTERVAL0         0x00000208U

#define MSK_DRAM_LED_UPDATE_INTERVAL0_VAL 0xffffffffU
#define SRT_DRAM_LED_UPDATE_INTERVAL0_VAL 0

/* all used bits of 'DRAM_LED_UPDATE_INTERVAL0': */
#define MSK_USED_BITS_DRAM_LED_UPDATE_INTERVAL0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_ACT_STRETCH0 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_ACT_STRETCH0         0x0000020CU

#define MSK_DRAM_ACT_STRETCH0_VAL 0xffffffffU
#define SRT_DRAM_ACT_STRETCH0_VAL 0

/* all used bits of 'DRAM_ACT_STRETCH0': */
#define MSK_USED_BITS_DRAM_ACT_STRETCH0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_RX_SR0 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_RX_SR0         0x00000210U

#define MSK_DRAM_RX_SR0_RES 0x00000003U
#define SRT_DRAM_RX_SR0_RES 0
#define MSK_DRAM_RX_SR0_ADR 0xfffffffcU
#define SRT_DRAM_RX_SR0_ADR 2

/* all used bits of 'DRAM_RX_SR0': */
#define MSK_USED_BITS_DRAM_RX_SR0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_RX_SR_BIT0 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_RX_SR_BIT0         0x00000214U

#define MSK_DRAM_RX_SR_BIT0_IDX 0x0000000fU
#define SRT_DRAM_RX_SR_BIT0_IDX 0
#define MSK_DRAM_RX_SR_BIT0_RES 0xfffffff0U
#define SRT_DRAM_RX_SR_BIT0_RES 4

/* all used bits of 'DRAM_RX_SR_BIT0': */
#define MSK_USED_BITS_DRAM_RX_SR_BIT0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_RX_SR_INV0 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_RX_SR_INV0         0x00000218U

#define MSK_DRAM_RX_SR_INV0_INV 0x00000001U
#define SRT_DRAM_RX_SR_INV0_INV 0
#define MSK_DRAM_RX_SR_INV0_RES 0xfffffffeU
#define SRT_DRAM_RX_SR_INV0_RES 1

/* all used bits of 'DRAM_RX_SR_INV0': */
#define MSK_USED_BITS_DRAM_RX_SR_INV0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_TX_SR0 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_TX_SR0         0x0000021CU

#define MSK_DRAM_TX_SR0_RES 0x00000003U
#define SRT_DRAM_TX_SR0_RES 0
#define MSK_DRAM_TX_SR0_ADR 0xfffffffcU
#define SRT_DRAM_TX_SR0_ADR 2

/* all used bits of 'DRAM_TX_SR0': */
#define MSK_USED_BITS_DRAM_TX_SR0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_TX_SR_BIT0 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_TX_SR_BIT0         0x00000220U

#define MSK_DRAM_TX_SR_BIT0_IDX 0x0000000fU
#define SRT_DRAM_TX_SR_BIT0_IDX 0
#define MSK_DRAM_TX_SR_BIT0_RES 0xfffffff0U
#define SRT_DRAM_TX_SR_BIT0_RES 4

/* all used bits of 'DRAM_TX_SR_BIT0': */
#define MSK_USED_BITS_DRAM_TX_SR_BIT0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_TX_SR_INV0 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_TX_SR_INV0         0x00000224U

#define MSK_DRAM_TX_SR_INV0_INV 0x00000001U
#define SRT_DRAM_TX_SR_INV0_INV 0
#define MSK_DRAM_TX_SR_INV0_RES 0xfffffffeU
#define SRT_DRAM_TX_SR_INV0_RES 1

/* all used bits of 'DRAM_TX_SR_INV0': */
#define MSK_USED_BITS_DRAM_TX_SR_INV0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_PHY_STATUS0 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_PHY_STATUS0         0x00000228U

#define MSK_DRAM_PHY_STATUS0_RES1 0x0000003fU
#define SRT_DRAM_PHY_STATUS0_RES1 0
#define MSK_DRAM_PHY_STATUS0_LNK  0x00000040U
#define SRT_DRAM_PHY_STATUS0_LNK  6
#define MSK_DRAM_PHY_STATUS0_ACT  0x00000080U
#define SRT_DRAM_PHY_STATUS0_ACT  7
#define MSK_DRAM_PHY_STATUS0_RES2 0xffffff00U
#define SRT_DRAM_PHY_STATUS0_RES2 8

/* all used bits of 'DRAM_PHY_STATUS0': */
#define MSK_USED_BITS_DRAM_PHY_STATUS0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_PHY_STATUS_STRETCH0 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_PHY_STATUS_STRETCH0         0x0000022CU

#define MSK_DRAM_PHY_STATUS_STRETCH0_RES1 0x0000003fU
#define SRT_DRAM_PHY_STATUS_STRETCH0_RES1 0
#define MSK_DRAM_PHY_STATUS_STRETCH0_LNK  0x00000040U
#define SRT_DRAM_PHY_STATUS_STRETCH0_LNK  6
#define MSK_DRAM_PHY_STATUS_STRETCH0_ACT  0x00000080U
#define SRT_DRAM_PHY_STATUS_STRETCH0_ACT  7
#define MSK_DRAM_PHY_STATUS_STRETCH0_RES2 0xffffff00U
#define SRT_DRAM_PHY_STATUS_STRETCH0_RES2 8

/* all used bits of 'DRAM_PHY_STATUS_STRETCH0': */
#define MSK_USED_BITS_DRAM_PHY_STATUS_STRETCH0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_LED_UPDATE_TIMER0 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_LED_UPDATE_TIMER0         0x00000230U

#define MSK_DRAM_LED_UPDATE_TIMER0_VAL 0xffffffffU
#define SRT_DRAM_LED_UPDATE_TIMER0_VAL 0

/* all used bits of 'DRAM_LED_UPDATE_TIMER0': */
#define MSK_USED_BITS_DRAM_LED_UPDATE_TIMER0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_ACT_STRETCH_TIMER0 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_ACT_STRETCH_TIMER0         0x00000234U

#define MSK_DRAM_ACT_STRETCH_TIMER0_VAL 0xffffffffU
#define SRT_DRAM_ACT_STRETCH_TIMER0_VAL 0

/* all used bits of 'DRAM_ACT_STRETCH_TIMER0': */
#define MSK_USED_BITS_DRAM_ACT_STRETCH_TIMER0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_BLINK_MASK0 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_BLINK_MASK0         0x00000238U

#define MSK_DRAM_BLINK_MASK0_RES1 0x0000003fU
#define SRT_DRAM_BLINK_MASK0_RES1 0
#define MSK_DRAM_BLINK_MASK0_LED0 0x00000040U
#define SRT_DRAM_BLINK_MASK0_LED0 6
#define MSK_DRAM_BLINK_MASK0_LED1 0x00000080U
#define SRT_DRAM_BLINK_MASK0_LED1 7
#define MSK_DRAM_BLINK_MASK0_RES2 0xffffff00U
#define SRT_DRAM_BLINK_MASK0_RES2 8

/* all used bits of 'DRAM_BLINK_MASK0': */
#define MSK_USED_BITS_DRAM_BLINK_MASK0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register AREA_DRAM_PORT1 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_AREA_DRAM_PORT1         0x0000023CU

#define MSK_AREA_DRAM_PORT1_RES1 0x00000003U
#define SRT_AREA_DRAM_PORT1_RES1 0
#define MSK_AREA_DRAM_PORT1_MODE 0x0000000cU
#define SRT_AREA_DRAM_PORT1_MODE 2
#define MSK_AREA_DRAM_PORT1_RES2 0xfffffff0U
#define SRT_AREA_DRAM_PORT1_RES2 4

/* all used bits of 'AREA_DRAM_PORT1': */
#define MSK_USED_BITS_AREA_DRAM_PORT1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_LED_UPDATE_INTERVAL1 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_LED_UPDATE_INTERVAL1         0x00000240U

#define MSK_DRAM_LED_UPDATE_INTERVAL1_VAL 0xffffffffU
#define SRT_DRAM_LED_UPDATE_INTERVAL1_VAL 0

/* all used bits of 'DRAM_LED_UPDATE_INTERVAL1': */
#define MSK_USED_BITS_DRAM_LED_UPDATE_INTERVAL1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_ACT_STRETCH1 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_ACT_STRETCH1         0x00000244U

#define MSK_DRAM_ACT_STRETCH1_VAL 0xffffffffU
#define SRT_DRAM_ACT_STRETCH1_VAL 0

/* all used bits of 'DRAM_ACT_STRETCH1': */
#define MSK_USED_BITS_DRAM_ACT_STRETCH1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_RX_SR1 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_RX_SR1         0x00000248U

#define MSK_DRAM_RX_SR1_RES 0x00000003U
#define SRT_DRAM_RX_SR1_RES 0
#define MSK_DRAM_RX_SR1_ADR 0xfffffffcU
#define SRT_DRAM_RX_SR1_ADR 2

/* all used bits of 'DRAM_RX_SR1': */
#define MSK_USED_BITS_DRAM_RX_SR1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_RX_SR_BIT1 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_RX_SR_BIT1         0x0000024CU

#define MSK_DRAM_RX_SR_BIT1_IDX 0x0000000fU
#define SRT_DRAM_RX_SR_BIT1_IDX 0
#define MSK_DRAM_RX_SR_BIT1_RES 0xfffffff0U
#define SRT_DRAM_RX_SR_BIT1_RES 4

/* all used bits of 'DRAM_RX_SR_BIT1': */
#define MSK_USED_BITS_DRAM_RX_SR_BIT1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_RX_SR_INV1 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_RX_SR_INV1         0x00000250U

#define MSK_DRAM_RX_SR_INV1_INV 0x00000001U
#define SRT_DRAM_RX_SR_INV1_INV 0
#define MSK_DRAM_RX_SR_INV1_RES 0xfffffffeU
#define SRT_DRAM_RX_SR_INV1_RES 1

/* all used bits of 'DRAM_RX_SR_INV1': */
#define MSK_USED_BITS_DRAM_RX_SR_INV1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_TX_SR1 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_TX_SR1         0x00000254U

#define MSK_DRAM_TX_SR1_RES 0x00000003U
#define SRT_DRAM_TX_SR1_RES 0
#define MSK_DRAM_TX_SR1_ADR 0xfffffffcU
#define SRT_DRAM_TX_SR1_ADR 2

/* all used bits of 'DRAM_TX_SR1': */
#define MSK_USED_BITS_DRAM_TX_SR1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_TX_SR_BIT1 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_TX_SR_BIT1         0x00000258U

#define MSK_DRAM_TX_SR_BIT1_IDX 0x0000000fU
#define SRT_DRAM_TX_SR_BIT1_IDX 0
#define MSK_DRAM_TX_SR_BIT1_RES 0xfffffff0U
#define SRT_DRAM_TX_SR_BIT1_RES 4

/* all used bits of 'DRAM_TX_SR_BIT1': */
#define MSK_USED_BITS_DRAM_TX_SR_BIT1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_TX_SR_INV1 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_TX_SR_INV1         0x0000025CU

#define MSK_DRAM_TX_SR_INV1_INV 0x00000001U
#define SRT_DRAM_TX_SR_INV1_INV 0
#define MSK_DRAM_TX_SR_INV1_RES 0xfffffffeU
#define SRT_DRAM_TX_SR_INV1_RES 1

/* all used bits of 'DRAM_TX_SR_INV1': */
#define MSK_USED_BITS_DRAM_TX_SR_INV1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_PHY_STATUS1 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_PHY_STATUS1         0x00000260U

#define MSK_DRAM_PHY_STATUS1_RES1 0x0000003fU
#define SRT_DRAM_PHY_STATUS1_RES1 0
#define MSK_DRAM_PHY_STATUS1_LNK  0x00000040U
#define SRT_DRAM_PHY_STATUS1_LNK  6
#define MSK_DRAM_PHY_STATUS1_ACT  0x00000080U
#define SRT_DRAM_PHY_STATUS1_ACT  7
#define MSK_DRAM_PHY_STATUS1_RES2 0xffffff00U
#define SRT_DRAM_PHY_STATUS1_RES2 8

/* all used bits of 'DRAM_PHY_STATUS1': */
#define MSK_USED_BITS_DRAM_PHY_STATUS1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_PHY_STATUS_STRETCH1 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_PHY_STATUS_STRETCH1         0x00000264U

#define MSK_DRAM_PHY_STATUS_STRETCH1_RES1 0x0000003fU
#define SRT_DRAM_PHY_STATUS_STRETCH1_RES1 0
#define MSK_DRAM_PHY_STATUS_STRETCH1_LNK  0x00000040U
#define SRT_DRAM_PHY_STATUS_STRETCH1_LNK  6
#define MSK_DRAM_PHY_STATUS_STRETCH1_ACT  0x00000080U
#define SRT_DRAM_PHY_STATUS_STRETCH1_ACT  7
#define MSK_DRAM_PHY_STATUS_STRETCH1_RES2 0xffffff00U
#define SRT_DRAM_PHY_STATUS_STRETCH1_RES2 8

/* all used bits of 'DRAM_PHY_STATUS_STRETCH1': */
#define MSK_USED_BITS_DRAM_PHY_STATUS_STRETCH1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_LED_UPDATE_TIMER1 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_LED_UPDATE_TIMER1         0x00000268U

#define MSK_DRAM_LED_UPDATE_TIMER1_VAL 0xffffffffU
#define SRT_DRAM_LED_UPDATE_TIMER1_VAL 0

/* all used bits of 'DRAM_LED_UPDATE_TIMER1': */
#define MSK_USED_BITS_DRAM_LED_UPDATE_TIMER1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_ACT_STRETCH_TIMER1 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_ACT_STRETCH_TIMER1         0x0000026CU

#define MSK_DRAM_ACT_STRETCH_TIMER1_VAL 0xffffffffU
#define SRT_DRAM_ACT_STRETCH_TIMER1_VAL 0

/* all used bits of 'DRAM_ACT_STRETCH_TIMER1': */
#define MSK_USED_BITS_DRAM_ACT_STRETCH_TIMER1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register DRAM_BLINK_MASK1 */
/* =>  */
/* => Mode:  */
/* --------------------------------------------------------------------- */

#define REL_Adr_DRAM_BLINK_MASK1         0x00000270U

#define MSK_DRAM_BLINK_MASK1_RES1 0x0000003fU
#define SRT_DRAM_BLINK_MASK1_RES1 0
#define MSK_DRAM_BLINK_MASK1_LED0 0x00000040U
#define SRT_DRAM_BLINK_MASK1_LED0 6
#define MSK_DRAM_BLINK_MASK1_LED1 0x00000080U
#define SRT_DRAM_BLINK_MASK1_LED1 7
#define MSK_DRAM_BLINK_MASK1_RES2 0xffffff00U
#define SRT_DRAM_BLINK_MASK1_RES2 8

/* all used bits of 'DRAM_BLINK_MASK1': */
#define MSK_USED_BITS_DRAM_BLINK_MASK1 0xffffffffU




/*********************************************************************/
/* area structure definitions                                        */
/*********************************************************************/



#endif

#ifndef PHYLED_XPIC_RAM_struct_h
#define PHYLED_XPIC_RAM_struct_h

typedef struct DRAM_CONSTANTPOOL_Ttag
{
  volatile uint32_t aulDataField[128];
} DRAM_CONSTANTPOOL_T;

typedef struct DRAM_GLOBAL_CFG_Ttag
{
  volatile uint32_t ulDRAM_POLL_INTERVAL_10NS;
} DRAM_GLOBAL_CFG_T;

typedef struct DRAM_PORT_Ttag
{
  volatile uint32_t ulDRAM_LED_MODE;
  volatile uint32_t ulDRAM_LED_UPDATE_INTERVAL;
  volatile uint32_t ulDRAM_ACT_STRETCH;
  volatile uint32_t ulDRAM_RX_SR;
  volatile uint32_t ulDRAM_RX_SR_BIT;
  volatile uint32_t ulDRAM_RX_SR_INV;
  volatile uint32_t ulDRAM_TX_SR;
  volatile uint32_t ulDRAM_TX_SR_BIT;
  volatile uint32_t ulDRAM_TX_SR_INV;
  volatile uint32_t ulDRAM_PHY_STATUS;
  volatile uint32_t ulDRAM_PHY_STATUS_STRETCH;
  volatile uint32_t ulDRAM_LED_UPDATE_TIMER;
  volatile uint32_t ulDRAM_ACT_STRETCH_TIMER;
  volatile uint32_t ulDRAM_BLINK_MASK;
} DRAM_PORT_T;

/* combined 3 structures */
typedef struct PHYLED_XPIC_RAM_Ttag {
  DRAM_CONSTANTPOOL_T tDRAM_CONSTANTPOOL;
  DRAM_GLOBAL_CFG_T tDRAM_GLOBAL_CFG;
  DRAM_PORT_T atDRAM_PORT[2];
} PHYLED_XPIC_RAM_T;

#endif /* PHYLED_XPIC_RAM_struct_h */

