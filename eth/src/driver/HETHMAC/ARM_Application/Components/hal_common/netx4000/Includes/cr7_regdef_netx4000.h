/*********************************************************************/
/* from regdef.v by regdef2h auto-generated C-header file            */
/* please see the regdef.html file for detailed register description */
/*********************************************************************/

#ifndef __cr7_regdef_netx4000_h
#define __cr7_regdef_netx4000_h

#include <stdint.h>

/* ===================================================================== */

/* Area of cr7_private_peripherals */

/* ===================================================================== */

#define Addr_NX4000_cr7_private_peripherals 0xFAF00000U

/* ===================================================================== */

/* AREA SCU */
/* Area of scu */

/* ===================================================================== */

#define Addr_NX4000_scu 0xFAF00000U

/* --------------------------------------------------------------------- */
/* Register scu_ctrl */
/* => SCU Control Register */
/*    Writes to this register are enabled when the access bit for the processor is */
/*    set in the SCU Access Control Register (see scu_access_ctrl). */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_scu_ctrl  0x00000000U
#define Adr_NX4000_scu_scu_ctrl  0xFAF00000U
#define Adr_NX4000_scu_ctrl      0xFAF00000U
#define DFLT_VAL_NX4000_scu_ctrl 0x00000002U

#define MSK_NX4000_scu_ctrl_scu_en                          0x00000001U
#define SRT_NX4000_scu_ctrl_scu_en                          0
#define DFLT_VAL_NX4000_scu_ctrl_scu_en                     0x00000000U
#define DFLT_BF_VAL_NX4000_scu_ctrl_scu_en                  0x00000000U
#define MSK_NX4000_scu_ctrl_address_filter_en               0x00000002U
#define SRT_NX4000_scu_ctrl_address_filter_en               1
#define DFLT_VAL_NX4000_scu_ctrl_address_filter_en          0x00000002U
#define DFLT_BF_VAL_NX4000_scu_ctrl_address_filter_en       0x00000001U
#define MSK_NX4000_scu_ctrl_ram_ecc_en                      0x00000004U
#define SRT_NX4000_scu_ctrl_ram_ecc_en                      2
#define DFLT_VAL_NX4000_scu_ctrl_ram_ecc_en                 0x00000000U
#define DFLT_BF_VAL_NX4000_scu_ctrl_ram_ecc_en              0x00000000U
#define MSK_NX4000_scu_ctrl_speculative_linefill_en         0x00000008U
#define SRT_NX4000_scu_ctrl_speculative_linefill_en         3
#define DFLT_VAL_NX4000_scu_ctrl_speculative_linefill_en    0x00000000U
#define DFLT_BF_VAL_NX4000_scu_ctrl_speculative_linefill_en 0x00000000U
#define MSK_NX4000_scu_ctrl_force_dev_port0_en              0x00000010U
#define SRT_NX4000_scu_ctrl_force_dev_port0_en              4
#define DFLT_VAL_NX4000_scu_ctrl_force_dev_port0_en         0x00000000U
#define DFLT_BF_VAL_NX4000_scu_ctrl_force_dev_port0_en      0x00000000U
#define MSK_NX4000_scu_ctrl_scu_standby_en                  0x00000020U
#define SRT_NX4000_scu_ctrl_scu_standby_en                  5
#define DFLT_VAL_NX4000_scu_ctrl_scu_standby_en             0x00000000U
#define DFLT_BF_VAL_NX4000_scu_ctrl_scu_standby_en          0x00000000U
#define MSK_NX4000_scu_ctrl_ic_standby_en                   0x00000040U
#define SRT_NX4000_scu_ctrl_ic_standby_en                   6
#define DFLT_VAL_NX4000_scu_ctrl_ic_standby_en              0x00000000U
#define DFLT_BF_VAL_NX4000_scu_ctrl_ic_standby_en           0x00000000U
#define MSK_NX4000_scu_ctrl_res1                            0x00000f80U
#define SRT_NX4000_scu_ctrl_res1                            7
#define DFLT_VAL_NX4000_scu_ctrl_res1                       0x00000000U
#define DFLT_BF_VAL_NX4000_scu_ctrl_res1                    0x00000000U
#define MSK_NX4000_scu_ctrl_ecc_check_en_m0                 0x00001000U
#define SRT_NX4000_scu_ctrl_ecc_check_en_m0                 12
#define DFLT_VAL_NX4000_scu_ctrl_ecc_check_en_m0            0x00000000U
#define DFLT_BF_VAL_NX4000_scu_ctrl_ecc_check_en_m0         0x00000000U
#define MSK_NX4000_scu_ctrl_ecc_check_en_m1                 0x00002000U
#define SRT_NX4000_scu_ctrl_ecc_check_en_m1                 13
#define DFLT_VAL_NX4000_scu_ctrl_ecc_check_en_m1            0x00000000U
#define DFLT_BF_VAL_NX4000_scu_ctrl_ecc_check_en_m1         0x00000000U
#define MSK_NX4000_scu_ctrl_ecc_check_en_mp                 0x00004000U
#define SRT_NX4000_scu_ctrl_ecc_check_en_mp                 14
#define DFLT_VAL_NX4000_scu_ctrl_ecc_check_en_mp            0x00000000U
#define DFLT_BF_VAL_NX4000_scu_ctrl_ecc_check_en_mp         0x00000000U
#define MSK_NX4000_scu_ctrl_ecc_check_en_acp                0x00008000U
#define SRT_NX4000_scu_ctrl_ecc_check_en_acp                15
#define DFLT_VAL_NX4000_scu_ctrl_ecc_check_en_acp           0x00000000U
#define DFLT_BF_VAL_NX4000_scu_ctrl_ecc_check_en_acp        0x00000000U
#define MSK_NX4000_scu_ctrl_res2                            0xffff0000U
#define SRT_NX4000_scu_ctrl_res2                            16
#define DFLT_VAL_NX4000_scu_ctrl_res2                       0x00000000U
#define DFLT_BF_VAL_NX4000_scu_ctrl_res2                    0x00000000U

/* all used bits of 'NX4000_scu_ctrl': */
#define MSK_USED_BITS_NX4000_scu_ctrl 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register scu_cfg */
/* => SCU Configuration Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_scu_cfg 0x00000004U
#define Adr_NX4000_scu_scu_cfg 0xFAF00004U
#define Adr_NX4000_scu_cfg     0xFAF00004U

#define MSK_NX4000_scu_cfg_number_of_processors 0x00000001U
#define SRT_NX4000_scu_cfg_number_of_processors 0
#define MSK_NX4000_scu_cfg_res0                 0x0000000eU
#define SRT_NX4000_scu_cfg_res0                 1
#define MSK_NX4000_scu_cfg_p0_coherency_mode    0x00000010U
#define SRT_NX4000_scu_cfg_p0_coherency_mode    4
#define MSK_NX4000_scu_cfg_res1                 0x000000e0U
#define SRT_NX4000_scu_cfg_res1                 5
#define MSK_NX4000_scu_cfg_p0_cache_size        0x00000f00U
#define SRT_NX4000_scu_cfg_p0_cache_size        8
#define MSK_NX4000_scu_cfg_res2                 0xfffff000U
#define SRT_NX4000_scu_cfg_res2                 12

/* all used bits of 'NX4000_scu_cfg': */
#define MSK_USED_BITS_NX4000_scu_cfg 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register scu_cpu_power_stat */
/* => SCU Power Status Register */
/*    Specifies the state of the Cortex-R7 processors with reference to power modes. */
/*    Writes to this register are enabled when the access bit for the processor is */
/*    set in the SCU Access Control Register (see scu_access_ctrl). */
/*    Dormant mode and powered-off mode are controlled by an external power */
/*    controller. SCU CPU Status Register bits indicate to the external power */
/*    controller the power domains that can be powered down. */
/*    Before entering any other power mode than Normal, the processor must */
/*    set its status field to signal to the power controller the mode it is about to */
/*    enter. The processor then executes a WFI entry instruction. When in WFI */
/*    state, the PWRCTLOn bus is enabled and signals to the power controller */
/*    what it must do with power domains. */
/*    The SCU CPU Power Status Register bits can also be read by a processor */
/*    exiting low-power mode to determine its state before executing its reset setup. */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_scu_cpu_power_stat  0x00000008U
#define Adr_NX4000_scu_scu_cpu_power_stat  0xFAF00008U
#define Adr_NX4000_scu_cpu_power_stat      0xFAF00008U
#define DFLT_VAL_NX4000_scu_cpu_power_stat 0x00000000U

#define MSK_NX4000_scu_cpu_power_stat_p0_power_stat         0x00000003U
#define SRT_NX4000_scu_cpu_power_stat_p0_power_stat         0
#define DFLT_VAL_NX4000_scu_cpu_power_stat_p0_power_stat    0x00000000U
#define DFLT_BF_VAL_NX4000_scu_cpu_power_stat_p0_power_stat 0x00000000U
#define MSK_NX4000_scu_cpu_power_stat_res                   0xfffffffcU
#define SRT_NX4000_scu_cpu_power_stat_res                   2
#define DFLT_VAL_NX4000_scu_cpu_power_stat_res              0x00000000U
#define DFLT_BF_VAL_NX4000_scu_cpu_power_stat_res           0x00000000U

/* all used bits of 'NX4000_scu_cpu_power_stat': */
#define MSK_USED_BITS_NX4000_scu_cpu_power_stat 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register scu_invalidate_all */
/* => SCU Invalidate All Registers */
/*    Invalidates the SCU tag RAMs on a per Cortex-R7 processor and per way basis. */
/*    Writes to this register are enabled when the access bit for the */
/*    processor is set in the SCU Access Control Register (see scu_access_ctrl). */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_scu_invalidate_all  0x0000000CU
#define Adr_NX4000_scu_scu_invalidate_all  0xFAF0000CU
#define Adr_NX4000_scu_invalidate_all      0xFAF0000CU
#define DFLT_VAL_NX4000_scu_invalidate_all 0x00000000U

#define MSK_NX4000_scu_invalidate_all_p0_ways         0x0000000fU
#define SRT_NX4000_scu_invalidate_all_p0_ways         0
#define DFLT_VAL_NX4000_scu_invalidate_all_p0_ways    0x00000000U
#define DFLT_BF_VAL_NX4000_scu_invalidate_all_p0_ways 0x00000000U
#define MSK_NX4000_scu_invalidate_all_res             0xfffffff0U
#define SRT_NX4000_scu_invalidate_all_res             4
#define DFLT_VAL_NX4000_scu_invalidate_all_res        0x00000000U
#define DFLT_BF_VAL_NX4000_scu_invalidate_all_res     0x00000000U

/* all used bits of 'NX4000_scu_invalidate_all': */
#define MSK_USED_BITS_NX4000_scu_invalidate_all 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register scu_master_filter_start */
/* => Master Filtering Start Address Register */
/*    Provides the start address for use with master port 1. */
/*    netX4000: Address range 0x04000000..0x040fffff (CR7_LLRAM) is accessed via master port 1. */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_scu_master_filter_start 0x00000040U
#define Adr_NX4000_scu_scu_master_filter_start 0xFAF00040U
#define Adr_NX4000_scu_master_filter_start     0xFAF00040U

#define MSK_NX4000_scu_master_filter_start_res     0x000fffffU
#define SRT_NX4000_scu_master_filter_start_res     0
#define MSK_NX4000_scu_master_filter_start_address 0xfff00000U
#define SRT_NX4000_scu_master_filter_start_address 20

/* all used bits of 'NX4000_scu_master_filter_start': */
#define MSK_USED_BITS_NX4000_scu_master_filter_start 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register scu_master_filter_end */
/* => Master Filtering End Address Register */
/*    Provides the end address for use with master port 1 */
/*    This register has an inclusive address as its end address. This means that the topmost megabyte of */
/*    address space of memory can be included in the filtering address range. */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_scu_master_filter_end 0x00000044U
#define Adr_NX4000_scu_scu_master_filter_end 0xFAF00044U
#define Adr_NX4000_scu_master_filter_end     0xFAF00044U

#define MSK_NX4000_scu_master_filter_end_res     0x000fffffU
#define SRT_NX4000_scu_master_filter_end_res     0
#define MSK_NX4000_scu_master_filter_end_address 0xfff00000U
#define SRT_NX4000_scu_master_filter_end_address 20

/* all used bits of 'NX4000_scu_master_filter_end': */
#define MSK_USED_BITS_NX4000_scu_master_filter_end 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register scu_pripheral_filter_start */
/* => Peripherals Filtering Start Address Register */
/*    Provides the filtering start address for the peripheral port. */
/*    netX4000: Address range 0xf0000000..0xffefffff is accessed via the peripheral port. */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_scu_pripheral_filter_start 0x00000048U
#define Adr_NX4000_scu_scu_pripheral_filter_start 0xFAF00048U
#define Adr_NX4000_scu_pripheral_filter_start     0xFAF00048U

#define MSK_NX4000_scu_pripheral_filter_start_res     0x000fffffU
#define SRT_NX4000_scu_pripheral_filter_start_res     0
#define MSK_NX4000_scu_pripheral_filter_start_address 0xfff00000U
#define SRT_NX4000_scu_pripheral_filter_start_address 20

/* all used bits of 'NX4000_scu_pripheral_filter_start': */
#define MSK_USED_BITS_NX4000_scu_pripheral_filter_start 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register scu_pripheral_filter_end */
/* => Peripherals Filtering End Address Register */
/*    Provides the filtering end address for the peripheral port. */
/*    This register has an inclusive address as its end address. This means that the topmost megabyte of */
/*    address space of memory can be included in the filtering address range. */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_scu_pripheral_filter_end 0x0000004CU
#define Adr_NX4000_scu_scu_pripheral_filter_end 0xFAF0004CU
#define Adr_NX4000_scu_pripheral_filter_end     0xFAF0004CU

#define MSK_NX4000_scu_pripheral_filter_end_res     0x000fffffU
#define SRT_NX4000_scu_pripheral_filter_end_res     0
#define MSK_NX4000_scu_pripheral_filter_end_address 0xfff00000U
#define SRT_NX4000_scu_pripheral_filter_end_address 20

/* all used bits of 'NX4000_scu_pripheral_filter_end': */
#define MSK_USED_BITS_NX4000_scu_pripheral_filter_end 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register scu_access_ctrl */
/* => SCU Access Control Register */
/*    Controls access to the following registers: */
/*    - scu_ctrl */
/*    - scu_cpu_power_stat */
/*    - scu_invalidate_all */
/*    - scu_err_bank_entry */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_scu_access_ctrl  0x00000050U
#define Adr_NX4000_scu_scu_access_ctrl  0xFAF00050U
#define Adr_NX4000_scu_access_ctrl      0xFAF00050U
#define DFLT_VAL_NX4000_scu_access_ctrl 0x00000001U

#define MSK_NX4000_scu_access_ctrl_p0_access_en         0x00000001U
#define SRT_NX4000_scu_access_ctrl_p0_access_en         0
#define DFLT_VAL_NX4000_scu_access_ctrl_p0_access_en    0x00000001U
#define DFLT_BF_VAL_NX4000_scu_access_ctrl_p0_access_en 0x00000001U
#define MSK_NX4000_scu_access_ctrl_res                  0xfffffffeU
#define SRT_NX4000_scu_access_ctrl_res                  1
#define DFLT_VAL_NX4000_scu_access_ctrl_res             0x00000000U
#define DFLT_BF_VAL_NX4000_scu_access_ctrl_res          0x00000000U

/* all used bits of 'NX4000_scu_access_ctrl': */
#define MSK_USED_BITS_NX4000_scu_access_ctrl 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register scu_err_bank_entry0 */
/* => SCU Error Bank Entry Registers */
/*    Shows the first SCU error bank entry. */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_scu_err_bank_entry0  0x00000060U
#define Adr_NX4000_scu_scu_err_bank_entry0  0xFAF00060U
#define Adr_NX4000_scu_err_bank_entry0      0xFAF00060U
#define DFLT_VAL_NX4000_scu_err_bank_entry0 0x00000000U

#define MSK_NX4000_scu_err_bank_entry0_status          0x00000003U
#define SRT_NX4000_scu_err_bank_entry0_status          0
#define DFLT_VAL_NX4000_scu_err_bank_entry0_status     0x00000000U
#define DFLT_BF_VAL_NX4000_scu_err_bank_entry0_status  0x00000000U
#define MSK_NX4000_scu_err_bank_entry0_res0            0x0000001cU
#define SRT_NX4000_scu_err_bank_entry0_res0            2
#define DFLT_VAL_NX4000_scu_err_bank_entry0_res0       0x00000000U
#define DFLT_BF_VAL_NX4000_scu_err_bank_entry0_res0    0x00000000U
#define MSK_NX4000_scu_err_bank_entry0_index           0x00003fe0U
#define SRT_NX4000_scu_err_bank_entry0_index           5
#define DFLT_VAL_NX4000_scu_err_bank_entry0_index      0x00000000U
#define DFLT_BF_VAL_NX4000_scu_err_bank_entry0_index   0x00000000U
#define MSK_NX4000_scu_err_bank_entry0_res1            0x0000c000U
#define SRT_NX4000_scu_err_bank_entry0_res1            14
#define DFLT_VAL_NX4000_scu_err_bank_entry0_res1       0x00000000U
#define DFLT_BF_VAL_NX4000_scu_err_bank_entry0_res1    0x00000000U
#define MSK_NX4000_scu_err_bank_entry0_p0_ways         0x000f0000U
#define SRT_NX4000_scu_err_bank_entry0_p0_ways         16
#define DFLT_VAL_NX4000_scu_err_bank_entry0_p0_ways    0x00000000U
#define DFLT_BF_VAL_NX4000_scu_err_bank_entry0_p0_ways 0x00000000U
#define MSK_NX4000_scu_err_bank_entry0_res2            0xfff00000U
#define SRT_NX4000_scu_err_bank_entry0_res2            20
#define DFLT_VAL_NX4000_scu_err_bank_entry0_res2       0x00000000U
#define DFLT_BF_VAL_NX4000_scu_err_bank_entry0_res2    0x00000000U

/* all used bits of 'NX4000_scu_err_bank_entry0': */
#define MSK_USED_BITS_NX4000_scu_err_bank_entry0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register scu_err_bank_entry1 */
/* => See register scu_err_bank_entry0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_scu_err_bank_entry1  0x00000064U
#define Adr_NX4000_scu_scu_err_bank_entry1  0xFAF00064U
#define Adr_NX4000_scu_err_bank_entry1      0xFAF00064U
#define DFLT_VAL_NX4000_scu_err_bank_entry1 0x00000000U

#define MSK_NX4000_scu_err_bank_entry1_status          0x00000003U
#define SRT_NX4000_scu_err_bank_entry1_status          0
#define DFLT_VAL_NX4000_scu_err_bank_entry1_status     0x00000000U
#define DFLT_BF_VAL_NX4000_scu_err_bank_entry1_status  0x00000000U
#define MSK_NX4000_scu_err_bank_entry1_res0            0x0000001cU
#define SRT_NX4000_scu_err_bank_entry1_res0            2
#define DFLT_VAL_NX4000_scu_err_bank_entry1_res0       0x00000000U
#define DFLT_BF_VAL_NX4000_scu_err_bank_entry1_res0    0x00000000U
#define MSK_NX4000_scu_err_bank_entry1_index           0x00003fe0U
#define SRT_NX4000_scu_err_bank_entry1_index           5
#define DFLT_VAL_NX4000_scu_err_bank_entry1_index      0x00000000U
#define DFLT_BF_VAL_NX4000_scu_err_bank_entry1_index   0x00000000U
#define MSK_NX4000_scu_err_bank_entry1_res1            0x0000c000U
#define SRT_NX4000_scu_err_bank_entry1_res1            14
#define DFLT_VAL_NX4000_scu_err_bank_entry1_res1       0x00000000U
#define DFLT_BF_VAL_NX4000_scu_err_bank_entry1_res1    0x00000000U
#define MSK_NX4000_scu_err_bank_entry1_p0_ways         0x000f0000U
#define SRT_NX4000_scu_err_bank_entry1_p0_ways         16
#define DFLT_VAL_NX4000_scu_err_bank_entry1_p0_ways    0x00000000U
#define DFLT_BF_VAL_NX4000_scu_err_bank_entry1_p0_ways 0x00000000U
#define MSK_NX4000_scu_err_bank_entry1_res2            0xfff00000U
#define SRT_NX4000_scu_err_bank_entry1_res2            20
#define DFLT_VAL_NX4000_scu_err_bank_entry1_res2       0x00000000U
#define DFLT_BF_VAL_NX4000_scu_err_bank_entry1_res2    0x00000000U

/* all used bits of 'NX4000_scu_err_bank_entry1': */
#define MSK_USED_BITS_NX4000_scu_err_bank_entry1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register scu_dtr_op */
/* => SCU Debug Tag RAM Operation Register */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_scu_dtr_op  0x00000070U
#define Adr_NX4000_scu_scu_dtr_op  0xFAF00070U
#define Adr_NX4000_scu_dtr_op      0xFAF00070U
#define DFLT_VAL_NX4000_scu_dtr_op 0x00000003U

#define MSK_NX4000_scu_dtr_op_val         0xffffffffU
#define SRT_NX4000_scu_dtr_op_val         0
#define DFLT_VAL_NX4000_scu_dtr_op_val    0x00000003U
#define DFLT_BF_VAL_NX4000_scu_dtr_op_val 0x00000003U

/* all used bits of 'NX4000_scu_dtr_op': */
#define MSK_USED_BITS_NX4000_scu_dtr_op 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register scu_dtr_val */
/* => SCU Debug Tag RAM Value Register */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_scu_dtr_val  0x00000074U
#define Adr_NX4000_scu_scu_dtr_val  0xFAF00074U
#define Adr_NX4000_scu_dtr_val      0xFAF00074U
#define DFLT_VAL_NX4000_scu_dtr_val 0x00000003U

#define MSK_NX4000_scu_dtr_val_val         0xffffffffU
#define SRT_NX4000_scu_dtr_val_val         0
#define DFLT_VAL_NX4000_scu_dtr_val_val    0x00000003U
#define DFLT_BF_VAL_NX4000_scu_dtr_val_val 0x00000003U

/* all used bits of 'NX4000_scu_dtr_val': */
#define MSK_USED_BITS_NX4000_scu_dtr_val 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register scu_dtr_ecc_chunk */
/* => SCU Debug Tag RAM ECC Chunk Register */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_scu_dtr_ecc_chunk  0x00000078U
#define Adr_NX4000_scu_scu_dtr_ecc_chunk  0xFAF00078U
#define Adr_NX4000_scu_dtr_ecc_chunk      0xFAF00078U
#define DFLT_VAL_NX4000_scu_dtr_ecc_chunk 0x00000003U

#define MSK_NX4000_scu_dtr_ecc_chunk_val         0xffffffffU
#define SRT_NX4000_scu_dtr_ecc_chunk_val         0
#define DFLT_VAL_NX4000_scu_dtr_ecc_chunk_val    0x00000003U
#define DFLT_BF_VAL_NX4000_scu_dtr_ecc_chunk_val 0x00000003U

/* all used bits of 'NX4000_scu_dtr_ecc_chunk': */
#define MSK_USED_BITS_NX4000_scu_dtr_ecc_chunk 0xffffffffU


/* ===================================================================== */

/* AREA IC_CPU_INTERFACE */
/* Area of ic_cpu_interface */

/* ===================================================================== */

#define Addr_NX4000_ic_cpu_interface 0xFAF00100U

/* --------------------------------------------------------------------- */
/* Register icci_ctrl */
/* => CPU Interface Control Register (ICCICR) */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icci_ctrl              0x00000000U
#define Adr_NX4000_ic_cpu_interface_icci_ctrl 0xFAF00100U
#define Adr_NX4000_icci_ctrl                  0xFAF00100U
#define DFLT_VAL_NX4000_icci_ctrl             0x00000000U

#define MSK_NX4000_icci_ctrl_enable         0x00000001U
#define SRT_NX4000_icci_ctrl_enable         0
#define DFLT_VAL_NX4000_icci_ctrl_enable    0x00000000U
#define DFLT_BF_VAL_NX4000_icci_ctrl_enable 0x00000000U

/* all used bits of 'NX4000_icci_ctrl': */
#define MSK_USED_BITS_NX4000_icci_ctrl 0x00000001U

/* --------------------------------------------------------------------- */
/* Register icci_prio_msk */
/* => Interrupt Priority Mask Register (ICCPMR) */
/*    Provides an interrupt priority filter. Only interrupts with higher priority than the */
/*    value in this register can be signalled to the processor. */
/*    Note: Higher priority corresponds to a lower Priority field value. */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icci_prio_msk              0x00000004U
#define Adr_NX4000_ic_cpu_interface_icci_prio_msk 0xFAF00104U
#define Adr_NX4000_icci_prio_msk                  0xFAF00104U
#define DFLT_VAL_NX4000_icci_prio_msk             0x00000000U

#define MSK_NX4000_icci_prio_msk_prio         0x000000ffU
#define SRT_NX4000_icci_prio_msk_prio         0
#define DFLT_VAL_NX4000_icci_prio_msk_prio    0x00000000U
#define DFLT_BF_VAL_NX4000_icci_prio_msk_prio 0x00000000U
#define MSK_NX4000_icci_prio_msk_res          0xffffff00U
#define SRT_NX4000_icci_prio_msk_res          8
#define DFLT_VAL_NX4000_icci_prio_msk_res     0x00000000U
#define DFLT_BF_VAL_NX4000_icci_prio_msk_res  0x00000000U

/* all used bits of 'NX4000_icci_prio_msk': */
#define MSK_USED_BITS_NX4000_icci_prio_msk 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icci_bin_point */
/* => Binary Point Register (ICCBPR) */
/*    The register defines the point at which the priority value fields split into two parts, */
/*    the group priority field and the subpriority field. The group priority field is used to */
/*    determine interrupt preemption. */
/*    The minimum binary point value is 3. An attempt to program the binary point field to a value */
/*    less than the minimum value sets the field to the minimum value. On a reset, */
/*    the binary point field is set to the minimum supported value. */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icci_bin_point              0x00000008U
#define Adr_NX4000_ic_cpu_interface_icci_bin_point 0xFAF00108U
#define Adr_NX4000_icci_bin_point                  0xFAF00108U
#define DFLT_VAL_NX4000_icci_bin_point             0x00000003U

#define MSK_NX4000_icci_bin_point_val         0x00000007U
#define SRT_NX4000_icci_bin_point_val         0
#define DFLT_VAL_NX4000_icci_bin_point_val    0x00000003U
#define DFLT_BF_VAL_NX4000_icci_bin_point_val 0x00000003U
#define MSK_NX4000_icci_bin_point_res         0xfffffff8U
#define SRT_NX4000_icci_bin_point_res         3
#define DFLT_VAL_NX4000_icci_bin_point_res    0x00000000U
#define DFLT_BF_VAL_NX4000_icci_bin_point_res 0x00000000U

/* all used bits of 'NX4000_icci_bin_point': */
#define MSK_USED_BITS_NX4000_icci_bin_point 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icci_int_ack */
/* => Interrupt Acknowledge Register (ICCIAR) */
/*    The processor reads this register to obtain the interrupt ID of the signaled interrupt. */
/*    This read acts as an acknowledge for the interrupt. */
/*    A read of the ICCIAR returns the interrupt ID of the highest priority pending interrupt for the CPU interface, */
/*    The read returns a spurious interrupt ID of 1023 if any of the following apply: */
/*    - Signalling of interrupts to the CPU interface is disabled */
/*    - There is no pending interrupt on this CPU interface with sufficient priority for the interface to signal */
/*      it to the processor. */
/*    For every read of a valid Interrupt ID from the ICCIAR, the interrupt service routine on the connected */
/*    processor must perform a matching write to the ICCEOIR, see icc_int_end (ICCEOIR) */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icci_int_ack              0x0000000CU
#define Adr_NX4000_ic_cpu_interface_icci_int_ack 0xFAF0010CU
#define Adr_NX4000_icci_int_ack                  0xFAF0010CU

#define MSK_NX4000_icci_int_ack_intid 0x000003ffU
#define SRT_NX4000_icci_int_ack_intid 0
#define MSK_NX4000_icci_int_ack_res   0xfffffc00U
#define SRT_NX4000_icci_int_ack_res   10

/* all used bits of 'NX4000_icci_int_ack': */
#define MSK_USED_BITS_NX4000_icci_int_ack 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icci_int_end */
/* => End Of Interrupt Register (ICCEOIR) */
/*    A processor writes to this register to inform the CPU interface that it has completed */
/*    its interrupt service routine for the specified interrupt. */
/*    Writing to this register causes the GIC to change the status of the identified interrupt: */
/*    - to inactive, if it was active */
/*    - to pending, if it was active and pending. */
/*    The interrupt service routine on the connected processor must write to the ICCEOIR for every read of a valid */
/*    Interrupt ID from the icc_int_ack Register (ICCIAR). The value */
/*    written to the ICCEOIR must be the interrupt ID read from the ICCIAR. */
/* => Mode: W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icci_int_end              0x00000010U
#define Adr_NX4000_ic_cpu_interface_icci_int_end 0xFAF00110U
#define Adr_NX4000_icci_int_end                  0xFAF00110U
#define DFLT_VAL_NX4000_icci_int_end             0x00000000U

#define MSK_NX4000_icci_int_end_intid         0x000003ffU
#define SRT_NX4000_icci_int_end_intid         0
#define DFLT_VAL_NX4000_icci_int_end_intid    0x00000000U
#define DFLT_BF_VAL_NX4000_icci_int_end_intid 0x00000000U
#define MSK_NX4000_icci_int_end_res           0xfffffc00U
#define SRT_NX4000_icci_int_end_res           10
#define DFLT_VAL_NX4000_icci_int_end_res      0x00000000U
#define DFLT_BF_VAL_NX4000_icci_int_end_res   0x00000000U

/* all used bits of 'NX4000_icci_int_end': */
#define MSK_USED_BITS_NX4000_icci_int_end 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icci_running_prio */
/* => Running Priority Register (ICCRPR) */
/*    Indicates the priority of the highest priority interrupt that is active on the CPU interface. */
/*    If there is no active interrupt on the CPU interface, and the GIC implements 8-bit */
/*    priority fields, a read of this register returns the value 0xFF, corresponding to the */
/*    lowest possible interrupt priority. If the GIC implements priority fields of less than */
/*    8 bits, the read might return the register reset value of 0xFF, or might return a value */
/*    corresponding to the lowest possible interrupt priority. Software cannot determine */
/*    the number of implemented priority bits from a read of this register. */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icci_running_prio              0x00000014U
#define Adr_NX4000_ic_cpu_interface_icci_running_prio 0xFAF00114U
#define Adr_NX4000_icci_running_prio                  0xFAF00114U

#define MSK_NX4000_icci_running_prio_prio 0x000000ffU
#define SRT_NX4000_icci_running_prio_prio 0
#define MSK_NX4000_icci_running_prio_res  0xffffff00U
#define SRT_NX4000_icci_running_prio_res  8

/* all used bits of 'NX4000_icci_running_prio': */
#define MSK_USED_BITS_NX4000_icci_running_prio 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icci_highest_pending */
/* => Highest Pending Interrupt Register (ICCHPIR) */
/*    Indicates the Interrupt ID, and processor ID if appropriate, of the pending interrupt */
/*    with the highest priority on the CPU interface. */
/*    Never returns the Interrupt ID of an interrupt that is Active and Pending. Returns a */
/*    processor ID only for an SGI in a multiprocessor implementation. */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icci_highest_pending              0x00000018U
#define Adr_NX4000_ic_cpu_interface_icci_highest_pending 0xFAF00118U
#define Adr_NX4000_icci_highest_pending                  0xFAF00118U

#define MSK_NX4000_icci_highest_pending_intid 0x000003ffU
#define SRT_NX4000_icci_highest_pending_intid 0
#define MSK_NX4000_icci_highest_pending_res   0xfffffc00U
#define SRT_NX4000_icci_highest_pending_res   10

/* all used bits of 'NX4000_icci_highest_pending': */
#define MSK_USED_BITS_NX4000_icci_highest_pending 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icci_impl_id */
/* => CPU Interface Implementer Identification Register (ICCIIDR) */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icci_impl_id              0x000000FCU
#define Adr_NX4000_ic_cpu_interface_icci_impl_id 0xFAF001FCU
#define Adr_NX4000_icci_impl_id                  0xFAF001FCU

#define MSK_NX4000_icci_impl_id_val 0xffffffffU
#define SRT_NX4000_icci_impl_id_val 0

/* all used bits of 'NX4000_icci_impl_id': */
#define MSK_USED_BITS_NX4000_icci_impl_id 0xffffffffU


/* ===================================================================== */

/* AREA GLOBAL_TIMER */
/* Area of global_timer */

/* ===================================================================== */

#define Addr_NX4000_global_timer 0xFAF00200U

/* --------------------------------------------------------------------- */
/* Register gtimer_count0 */
/* => Global Timer Counter Register */
/*    There are two timer counter registers. They are the lower 32-bit timer counter at offset 0x00 and */
/*    the upper 32-bit timer counter at offset 0x04. */
/*    You must access these registers with 32-bit accesses. You cannot use STRD/LDRD. Any other */
/*    access is UNPREDICTABLE. */
/*    To modify the register proceed as follows: */
/*    1. Clear the timer enable bit in the Global Timer Control Register (gtimer_ctrl). */
/*    2. Write the lower 32-bit timer counter register. */
/*    3. Write the upper 32-bit timer counter register. */
/*    4. Set the timer enable bit. */
/*    To get the value from the Global Timer Counter register proceed as follows: */
/*    1. Read the upper 32-bit timer counter register. */
/*    2. Read the lower 32-bit timer counter register. */
/*    3. Read the upper 32-bit timer counter register again. If the value is different to the 32-bit */
/*       upper value read previously, go back to step 2. Otherwise the 64-bit timer counter value is correct. */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_gtimer_count0          0x00000000U
#define Adr_NX4000_global_timer_gtimer_count0 0xFAF00200U
#define Adr_NX4000_gtimer_count0              0xFAF00200U
#define DFLT_VAL_NX4000_gtimer_count0         0x00000000U

#define MSK_NX4000_gtimer_count0_val         0xffffffffU
#define SRT_NX4000_gtimer_count0_val         0
#define DFLT_VAL_NX4000_gtimer_count0_val    0x00000000U
#define DFLT_BF_VAL_NX4000_gtimer_count0_val 0x00000000U

/* all used bits of 'NX4000_gtimer_count0': */
#define MSK_USED_BITS_NX4000_gtimer_count0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register gtimer_count1 */
/* => See register gtimer_count0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_gtimer_count1          0x00000004U
#define Adr_NX4000_global_timer_gtimer_count1 0xFAF00204U
#define Adr_NX4000_gtimer_count1              0xFAF00204U
#define DFLT_VAL_NX4000_gtimer_count1         0x00000000U

#define MSK_NX4000_gtimer_count1_val         0xffffffffU
#define SRT_NX4000_gtimer_count1_val         0
#define DFLT_VAL_NX4000_gtimer_count1_val    0x00000000U
#define DFLT_BF_VAL_NX4000_gtimer_count1_val 0x00000000U

/* all used bits of 'NX4000_gtimer_count1': */
#define MSK_USED_BITS_NX4000_gtimer_count1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register gtimer_ctrl */
/* => Global Timer Control Register */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_gtimer_ctrl          0x00000008U
#define Adr_NX4000_global_timer_gtimer_ctrl 0xFAF00208U
#define Adr_NX4000_gtimer_ctrl              0xFAF00208U
#define DFLT_VAL_NX4000_gtimer_ctrl         0x00000000U

#define MSK_NX4000_gtimer_ctrl_gtimer_en         0x00000001U
#define SRT_NX4000_gtimer_ctrl_gtimer_en         0
#define DFLT_VAL_NX4000_gtimer_ctrl_gtimer_en    0x00000000U
#define DFLT_BF_VAL_NX4000_gtimer_ctrl_gtimer_en 0x00000000U
#define MSK_NX4000_gtimer_ctrl_comp_en           0x00000002U
#define SRT_NX4000_gtimer_ctrl_comp_en           1
#define DFLT_VAL_NX4000_gtimer_ctrl_comp_en      0x00000000U
#define DFLT_BF_VAL_NX4000_gtimer_ctrl_comp_en   0x00000000U
#define MSK_NX4000_gtimer_ctrl_irq_en            0x00000004U
#define SRT_NX4000_gtimer_ctrl_irq_en            2
#define DFLT_VAL_NX4000_gtimer_ctrl_irq_en       0x00000000U
#define DFLT_BF_VAL_NX4000_gtimer_ctrl_irq_en    0x00000000U
#define MSK_NX4000_gtimer_ctrl_auto_inc          0x00000008U
#define SRT_NX4000_gtimer_ctrl_auto_inc          3
#define DFLT_VAL_NX4000_gtimer_ctrl_auto_inc     0x00000000U
#define DFLT_BF_VAL_NX4000_gtimer_ctrl_auto_inc  0x00000000U
#define MSK_NX4000_gtimer_ctrl_res0              0x000000f0U
#define SRT_NX4000_gtimer_ctrl_res0              4
#define DFLT_VAL_NX4000_gtimer_ctrl_res0         0x00000000U
#define DFLT_BF_VAL_NX4000_gtimer_ctrl_res0      0x00000000U
#define MSK_NX4000_gtimer_ctrl_prescaler         0x0000ff00U
#define SRT_NX4000_gtimer_ctrl_prescaler         8
#define DFLT_VAL_NX4000_gtimer_ctrl_prescaler    0x00000000U
#define DFLT_BF_VAL_NX4000_gtimer_ctrl_prescaler 0x00000000U
#define MSK_NX4000_gtimer_ctrl_res1              0xffff0000U
#define SRT_NX4000_gtimer_ctrl_res1              16
#define DFLT_VAL_NX4000_gtimer_ctrl_res1         0x00000000U
#define DFLT_BF_VAL_NX4000_gtimer_ctrl_res1      0x00000000U

/* all used bits of 'NX4000_gtimer_ctrl': */
#define MSK_USED_BITS_NX4000_gtimer_ctrl 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register gtimer_int */
/* => Global Timer Interrupt Status Register */
/*    The event flag is a sticky bit that is automatically set when the Counter Register reaches the */
/*    Comparator Register value. If the timer interrupt is enabled, Interrupt ID 27 is set as pending in */
/*    the Interrupt Distributor after the event flag is set. The event flag is cleared when written to 1. */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_gtimer_int          0x0000000CU
#define Adr_NX4000_global_timer_gtimer_int 0xFAF0020CU
#define Adr_NX4000_gtimer_int              0xFAF0020CU
#define DFLT_VAL_NX4000_gtimer_int         0x00000000U

#define MSK_NX4000_gtimer_int_stat         0x00000001U
#define SRT_NX4000_gtimer_int_stat         0
#define DFLT_VAL_NX4000_gtimer_int_stat    0x00000000U
#define DFLT_BF_VAL_NX4000_gtimer_int_stat 0x00000000U
#define MSK_NX4000_gtimer_int_res          0xfffffffeU
#define SRT_NX4000_gtimer_int_res          1
#define DFLT_VAL_NX4000_gtimer_int_res     0x00000000U
#define DFLT_BF_VAL_NX4000_gtimer_int_res  0x00000000U

/* all used bits of 'NX4000_gtimer_int': */
#define MSK_USED_BITS_NX4000_gtimer_int 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register gtimer_comp0 */
/* => Global Timer Comparator Value Register */
/*    There are two 32-bit registers, the lower 32-bit comparator value register at offset 0x10 and the */
/*    upper 32-bit comparator value register at offset 0x14. */
/*    You must access these registers with 32-bit accesses. You cannot use STRD/LDRD. There is a */
/*    Comparator Value Register for each Cortex-R7 processor. */
/*    To ensure that updates to this register do not set the Interrupt Status Register proceed as follows: */
/*    1. Clear the Comp Enable bit in the Timer Control Register. */
/*    2. Write the lower 32-bit Comparator Value Register. */
/*    3. Write the upper 32-bit Comparator Value Register. */
/*    4. Set the Comp Enable bit and, if necessary, the IRQ enable bit. */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_gtimer_comp0          0x00000010U
#define Adr_NX4000_global_timer_gtimer_comp0 0xFAF00210U
#define Adr_NX4000_gtimer_comp0              0xFAF00210U
#define DFLT_VAL_NX4000_gtimer_comp0         0x00000000U

#define MSK_NX4000_gtimer_comp0_val         0xffffffffU
#define SRT_NX4000_gtimer_comp0_val         0
#define DFLT_VAL_NX4000_gtimer_comp0_val    0x00000000U
#define DFLT_BF_VAL_NX4000_gtimer_comp0_val 0x00000000U

/* all used bits of 'NX4000_gtimer_comp0': */
#define MSK_USED_BITS_NX4000_gtimer_comp0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register gtimer_comp1 */
/* => See register gtimer_comp0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_gtimer_comp1          0x00000014U
#define Adr_NX4000_global_timer_gtimer_comp1 0xFAF00214U
#define Adr_NX4000_gtimer_comp1              0xFAF00214U
#define DFLT_VAL_NX4000_gtimer_comp1         0x00000000U

#define MSK_NX4000_gtimer_comp1_val         0xffffffffU
#define SRT_NX4000_gtimer_comp1_val         0
#define DFLT_VAL_NX4000_gtimer_comp1_val    0x00000000U
#define DFLT_BF_VAL_NX4000_gtimer_comp1_val 0x00000000U

/* all used bits of 'NX4000_gtimer_comp1': */
#define MSK_USED_BITS_NX4000_gtimer_comp1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register gtimer_auto_inc */
/* => Global Timer Auto-increment Register */
/*    This 32-bit register gives the increment value of the Comparator Register when the */
/*    Auto-increment bit is set in the Timer Control Register. */
/*    If the comp enable and auto-increment bits are set when the global counter reaches the */
/*    Comparator Register value, the comparator is incremented by the auto-increment value, so that */
/*    a new event can be set periodically. */
/*    The global timer is not affected, and continues incrementing. */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_gtimer_auto_inc          0x00000018U
#define Adr_NX4000_global_timer_gtimer_auto_inc 0xFAF00218U
#define Adr_NX4000_gtimer_auto_inc              0xFAF00218U
#define DFLT_VAL_NX4000_gtimer_auto_inc         0x00000000U

#define MSK_NX4000_gtimer_auto_inc_val         0xffffffffU
#define SRT_NX4000_gtimer_auto_inc_val         0
#define DFLT_VAL_NX4000_gtimer_auto_inc_val    0x00000000U
#define DFLT_BF_VAL_NX4000_gtimer_auto_inc_val 0x00000000U

/* all used bits of 'NX4000_gtimer_auto_inc': */
#define MSK_USED_BITS_NX4000_gtimer_auto_inc 0xffffffffU


/* ===================================================================== */

/* AREA PRIVATE_TIMER */
/* Area of private_timer */

/* ===================================================================== */

#define Addr_NX4000_private_timer 0xFAF00600U

/* --------------------------------------------------------------------- */
/* Register ptimer_load */
/* => Private Timer Load Register */
/*    The Timer Load Register contains the value copied to the Timer Counter Register when it */
/*    decrements down to zero with auto-reload mode enabled. Writing to the Timer Load Register */
/*    means that you also write to the Timer Counter Register. */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_ptimer_load           0x00000000U
#define Adr_NX4000_private_timer_ptimer_load 0xFAF00600U
#define Adr_NX4000_ptimer_load               0xFAF00600U
#define DFLT_VAL_NX4000_ptimer_load          0x00000000U

#define MSK_NX4000_ptimer_load_val         0xffffffffU
#define SRT_NX4000_ptimer_load_val         0
#define DFLT_VAL_NX4000_ptimer_load_val    0x00000000U
#define DFLT_BF_VAL_NX4000_ptimer_load_val 0x00000000U

/* all used bits of 'NX4000_ptimer_load': */
#define MSK_USED_BITS_NX4000_ptimer_load 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ptimer_count */
/* => Private Timer Counter Register */
/*    The Timer Counter Register is a decrementing counter. */
/*    The Timer Counter Register decrements if the timer is enabled using the timer enable bit in the */
/*    Timer Control Register. If a Cortex-R7 processor timer is in debug state, the counter only */
/*    decrements when the processor returns to non-debug state. */
/*    When the Timer Counter Register reaches zero and auto-reload mode is enabled, it reloads the */
/*    value in the Timer Load Register and then decrements from that value. If auto-reload mode is */
/*    not enabled, the Timer Counter Register decrements down to zero and stops. */
/*    When the Timer Counter Register reaches zero, the timer interrupt status event flag is set and */
/*    the interrupt ID 29 is set as pending in the Interrupt Distributor, if interrupt generation is enabled */
/*    in the Timer Control Register. */
/*    Writing to the Timer Counter Register or Timer Load Register forces the Timer Counter */
/*    Register to decrement from the newly written value. */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_ptimer_count           0x00000004U
#define Adr_NX4000_private_timer_ptimer_count 0xFAF00604U
#define Adr_NX4000_ptimer_count               0xFAF00604U
#define DFLT_VAL_NX4000_ptimer_count          0x00000000U

#define MSK_NX4000_ptimer_count_val         0xffffffffU
#define SRT_NX4000_ptimer_count_val         0
#define DFLT_VAL_NX4000_ptimer_count_val    0x00000000U
#define DFLT_BF_VAL_NX4000_ptimer_count_val 0x00000000U

/* all used bits of 'NX4000_ptimer_count': */
#define MSK_USED_BITS_NX4000_ptimer_count 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ptimer_ctrl */
/* => Private Timer Control Register */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_ptimer_ctrl           0x00000008U
#define Adr_NX4000_private_timer_ptimer_ctrl 0xFAF00608U
#define Adr_NX4000_ptimer_ctrl               0xFAF00608U
#define DFLT_VAL_NX4000_ptimer_ctrl          0x00000000U

#define MSK_NX4000_ptimer_ctrl_ptimer_en              0x00000001U
#define SRT_NX4000_ptimer_ctrl_ptimer_en              0
#define DFLT_VAL_NX4000_ptimer_ctrl_ptimer_en         0x00000000U
#define DFLT_BF_VAL_NX4000_ptimer_ctrl_ptimer_en      0x00000000U
#define MSK_NX4000_ptimer_ctrl_auto_reload_en         0x00000002U
#define SRT_NX4000_ptimer_ctrl_auto_reload_en         1
#define DFLT_VAL_NX4000_ptimer_ctrl_auto_reload_en    0x00000000U
#define DFLT_BF_VAL_NX4000_ptimer_ctrl_auto_reload_en 0x00000000U
#define MSK_NX4000_ptimer_ctrl_irq_en                 0x00000004U
#define SRT_NX4000_ptimer_ctrl_irq_en                 2
#define DFLT_VAL_NX4000_ptimer_ctrl_irq_en            0x00000000U
#define DFLT_BF_VAL_NX4000_ptimer_ctrl_irq_en         0x00000000U
#define MSK_NX4000_ptimer_ctrl_res0                   0x000000f8U
#define SRT_NX4000_ptimer_ctrl_res0                   3
#define DFLT_VAL_NX4000_ptimer_ctrl_res0              0x00000000U
#define DFLT_BF_VAL_NX4000_ptimer_ctrl_res0           0x00000000U
#define MSK_NX4000_ptimer_ctrl_prescaler              0x0000ff00U
#define SRT_NX4000_ptimer_ctrl_prescaler              8
#define DFLT_VAL_NX4000_ptimer_ctrl_prescaler         0x00000000U
#define DFLT_BF_VAL_NX4000_ptimer_ctrl_prescaler      0x00000000U
#define MSK_NX4000_ptimer_ctrl_res1                   0xffff0000U
#define SRT_NX4000_ptimer_ctrl_res1                   16
#define DFLT_VAL_NX4000_ptimer_ctrl_res1              0x00000000U
#define DFLT_BF_VAL_NX4000_ptimer_ctrl_res1           0x00000000U

/* all used bits of 'NX4000_ptimer_ctrl': */
#define MSK_USED_BITS_NX4000_ptimer_ctrl 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register ptimer_int */
/* => Private Timer Interrupt Status Register */
/*    The event flag is a sticky bit that is automatically set when the Counter Register reaches zero. */
/*    If the timer interrupt is enabled, Interrupt ID 29 is set as pending in the Interrupt Distributor after */
/*    the event flag is set. The event flag is cleared by writing a 1 to bit[0]. */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_ptimer_int           0x0000000CU
#define Adr_NX4000_private_timer_ptimer_int 0xFAF0060CU
#define Adr_NX4000_ptimer_int               0xFAF0060CU
#define DFLT_VAL_NX4000_ptimer_int          0x00000000U

#define MSK_NX4000_ptimer_int_stat         0x00000001U
#define SRT_NX4000_ptimer_int_stat         0
#define DFLT_VAL_NX4000_ptimer_int_stat    0x00000000U
#define DFLT_BF_VAL_NX4000_ptimer_int_stat 0x00000000U
#define MSK_NX4000_ptimer_int_res          0xfffffffeU
#define SRT_NX4000_ptimer_int_res          1
#define DFLT_VAL_NX4000_ptimer_int_res     0x00000000U
#define DFLT_BF_VAL_NX4000_ptimer_int_res  0x00000000U

/* all used bits of 'NX4000_ptimer_int': */
#define MSK_USED_BITS_NX4000_ptimer_int 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register wdg_load */
/* => Watchdog Load Register */
/*    The Watchdog Load Register contains the value copied to the Watchdog Counter Register when */
/*    it decrements down to zero with auto-reload mode enabled, in Timer mode. Writing to the */
/*    Watchdog Load Register means that you also write to the Watchdog Counter Register. */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_wdg_load           0x00000020U
#define Adr_NX4000_private_timer_wdg_load 0xFAF00620U
#define Adr_NX4000_wdg_load               0xFAF00620U
#define DFLT_VAL_NX4000_wdg_load          0x00000000U

#define MSK_NX4000_wdg_load_val         0xffffffffU
#define SRT_NX4000_wdg_load_val         0
#define DFLT_VAL_NX4000_wdg_load_val    0x00000000U
#define DFLT_BF_VAL_NX4000_wdg_load_val 0x00000000U

/* all used bits of 'NX4000_wdg_load': */
#define MSK_USED_BITS_NX4000_wdg_load 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register wdg_count */
/* => Watchdog Counter Register */
/*    The Watchdog Counter Register is a decrementing counter. */
/*    It decrements if the Watchdog is enabled using the Watchdog enable bit in the Watchdog Control */
/*    Register. If the Cortex-R7 processor associated with the Watchdog is in debug state, the counter */
/*    does not decrement until the processor returns to non-debug state. */
/*    When the Watchdog Counter Register reaches zero and auto-reload mode is enabled, and in */
/*    timer mode, it reloads the value in the Watchdog Load Register and then decrements from that */
/*    value. If auto-reload mode is not enabled or the watchdog is not in timer mode, the Watchdog */
/*    Counter Register decrements down to zero and stops. */
/*    When in watchdog mode the only way to update the Watchdog Counter Register is to write to */
/*    the Watchdog Load Register. When in timer mode the Watchdog Counter Register is write */
/*    accessible. */
/*    The behavior of the watchdog when the Watchdog Counter Register reaches zero depends on its */
/*    current mode: */
/*    - Timer mode: When the Watchdog Counter Register reaches zero, the watchdog interrupt status */
/*      event flag is set. If interrupt generation is enabled in the Watchdog Control */
/*      Register, interrupt ID 30 is set as pending in the Interrupt Distributor. */
/*    - Watchdog mode */
/*      If a software failure prevents the Watchdog Counter Register from being */
/*      refreshed: */
/*      - The Watchdog Counter Register reaches zero. */
/*      - The Watchdog reset status flag is set. */
/*      - The associated WDRESETREQ reset request output is asserted. */
/*      WDRESETREQ is valid for one PERIPHCLK cycle. */
/*      The external reset source is then responsible for resetting all or part of the */
/*      Cortex-R7 MPCore processor design. */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_wdg_count           0x00000024U
#define Adr_NX4000_private_timer_wdg_count 0xFAF00624U
#define Adr_NX4000_wdg_count               0xFAF00624U
#define DFLT_VAL_NX4000_wdg_count          0x00000000U

#define MSK_NX4000_wdg_count_val         0xffffffffU
#define SRT_NX4000_wdg_count_val         0
#define DFLT_VAL_NX4000_wdg_count_val    0x00000000U
#define DFLT_BF_VAL_NX4000_wdg_count_val 0x00000000U

/* all used bits of 'NX4000_wdg_count': */
#define MSK_USED_BITS_NX4000_wdg_count 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register wdg_ctrl */
/* => Watchdog Control Register */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_wdg_ctrl           0x00000028U
#define Adr_NX4000_private_timer_wdg_ctrl 0xFAF00628U
#define Adr_NX4000_wdg_ctrl               0xFAF00628U
#define DFLT_VAL_NX4000_wdg_ctrl          0x00000000U

#define MSK_NX4000_wdg_ctrl_wdg_en                 0x00000001U
#define SRT_NX4000_wdg_ctrl_wdg_en                 0
#define DFLT_VAL_NX4000_wdg_ctrl_wdg_en            0x00000000U
#define DFLT_BF_VAL_NX4000_wdg_ctrl_wdg_en         0x00000000U
#define MSK_NX4000_wdg_ctrl_auto_reload_en         0x00000002U
#define SRT_NX4000_wdg_ctrl_auto_reload_en         1
#define DFLT_VAL_NX4000_wdg_ctrl_auto_reload_en    0x00000000U
#define DFLT_BF_VAL_NX4000_wdg_ctrl_auto_reload_en 0x00000000U
#define MSK_NX4000_wdg_ctrl_irq_en                 0x00000004U
#define SRT_NX4000_wdg_ctrl_irq_en                 2
#define DFLT_VAL_NX4000_wdg_ctrl_irq_en            0x00000000U
#define DFLT_BF_VAL_NX4000_wdg_ctrl_irq_en         0x00000000U
#define MSK_NX4000_wdg_ctrl_wdg_mode_en            0x00000008U
#define SRT_NX4000_wdg_ctrl_wdg_mode_en            3
#define DFLT_VAL_NX4000_wdg_ctrl_wdg_mode_en       0x00000000U
#define DFLT_BF_VAL_NX4000_wdg_ctrl_wdg_mode_en    0x00000000U
#define MSK_NX4000_wdg_ctrl_res0                   0x000000f0U
#define SRT_NX4000_wdg_ctrl_res0                   4
#define DFLT_VAL_NX4000_wdg_ctrl_res0              0x00000000U
#define DFLT_BF_VAL_NX4000_wdg_ctrl_res0           0x00000000U
#define MSK_NX4000_wdg_ctrl_prescaler              0x0000ff00U
#define SRT_NX4000_wdg_ctrl_prescaler              8
#define DFLT_VAL_NX4000_wdg_ctrl_prescaler         0x00000000U
#define DFLT_BF_VAL_NX4000_wdg_ctrl_prescaler      0x00000000U
#define MSK_NX4000_wdg_ctrl_res1                   0xffff0000U
#define SRT_NX4000_wdg_ctrl_res1                   16
#define DFLT_VAL_NX4000_wdg_ctrl_res1              0x00000000U
#define DFLT_BF_VAL_NX4000_wdg_ctrl_res1           0x00000000U

/* all used bits of 'NX4000_wdg_ctrl': */
#define MSK_USED_BITS_NX4000_wdg_ctrl 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register wdg_int */
/* => Watchdog Interrupt Status Register */
/*    The event flag is a sticky bit that is automatically set when the Counter Register reaches zero in */
/*    timer mode. If the watchdog interrupt is enabled, Interrupt ID 30 is set as pending in the */
/*    Interrupt Distributor after the event flag is set. The event flag is cleared when written with a */
/*    value of 1. Trying to write a zero to the event flag or a one when it is not set has no effect. */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_wdg_int           0x0000002CU
#define Adr_NX4000_private_timer_wdg_int 0xFAF0062CU
#define Adr_NX4000_wdg_int               0xFAF0062CU
#define DFLT_VAL_NX4000_wdg_int          0x00000000U

#define MSK_NX4000_wdg_int_stat         0x00000001U
#define SRT_NX4000_wdg_int_stat         0
#define DFLT_VAL_NX4000_wdg_int_stat    0x00000000U
#define DFLT_BF_VAL_NX4000_wdg_int_stat 0x00000000U
#define MSK_NX4000_wdg_int_res          0xfffffffeU
#define SRT_NX4000_wdg_int_res          1
#define DFLT_VAL_NX4000_wdg_int_res     0x00000000U
#define DFLT_BF_VAL_NX4000_wdg_int_res  0x00000000U

/* all used bits of 'NX4000_wdg_int': */
#define MSK_USED_BITS_NX4000_wdg_int 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register wdg_reset */
/* => Watchdog Interrupt Status Register */
/*    In watchdog mode, the reset flag is a sticky bit that is automatically set when the Counter */
/*    Register reaches zero and a reset request is sent accordingly. */
/*    For lock-step or split/lock in safe mode, the reset must not be applied immediately to the entire */
/*    Cortex-R7 MPCore. This is because when starting again after reset, the sticky flag is set in one */
/*    processor, but not the other, which raises COMPFAULT. Therefore, if one processor has its */
/*    watchdog flag set, the other processor must reach the same state, that is, also having its */
/*    watchdog flag set. */
/*    The reset flag is cleared when written with a value of 1. Trying to write a zero to the reset flag */
/*    or a one when it is not set has no effect. This flag is not reset by normal Cortex-R7 processor */
/*    resets but has its own reset line, nWDRESET. nWDRESET must not be asserted when the */
/*    processor reset assertion is the result of a watchdog reset request with WDRESETREQ. This */
/*    distinction enables software to differentiate between a normal boot sequence, reset flag is zero, */
/*    and one caused by a previous watchdog time-out, reset flag set to one. */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_wdg_reset           0x00000030U
#define Adr_NX4000_private_timer_wdg_reset 0xFAF00630U
#define Adr_NX4000_wdg_reset               0xFAF00630U
#define DFLT_VAL_NX4000_wdg_reset          0x00000000U

#define MSK_NX4000_wdg_reset_stat         0x00000001U
#define SRT_NX4000_wdg_reset_stat         0
#define DFLT_VAL_NX4000_wdg_reset_stat    0x00000000U
#define DFLT_BF_VAL_NX4000_wdg_reset_stat 0x00000000U
#define MSK_NX4000_wdg_reset_res          0xfffffffeU
#define SRT_NX4000_wdg_reset_res          1
#define DFLT_VAL_NX4000_wdg_reset_res     0x00000000U
#define DFLT_BF_VAL_NX4000_wdg_reset_res  0x00000000U

/* all used bits of 'NX4000_wdg_reset': */
#define MSK_USED_BITS_NX4000_wdg_reset 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register wdg_disable */
/* => Watchdog Disable Register */
/*    Use the Watchdog Disable Register to switch from watchdog to timer mode. The software must */
/*    write 0x12345678 then 0x87654321 successively to the Watchdog Disable Register so that the */
/*    watchdog mode bit in the Watchdog Control Register is set to zero. */
/*    If one of the values written to the Watchdog Disable Register is incorrect or if any other write */
/*    occurs in between the two word writes, the watchdog remains in its current state. To reactivate */
/*    the Watchdog, the software must write 1 to the watchdog mode bit of the Watchdog Control Register. */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_wdg_disable           0x00000034U
#define Adr_NX4000_private_timer_wdg_disable 0xFAF00634U
#define Adr_NX4000_wdg_disable               0xFAF00634U
#define DFLT_VAL_NX4000_wdg_disable          0x00000000U

#define MSK_NX4000_wdg_disable_val         0xffffffffU
#define SRT_NX4000_wdg_disable_val         0
#define DFLT_VAL_NX4000_wdg_disable_val    0x00000000U
#define DFLT_BF_VAL_NX4000_wdg_disable_val 0x00000000U

/* all used bits of 'NX4000_wdg_disable': */
#define MSK_USED_BITS_NX4000_wdg_disable 0xffffffffU


/* ===================================================================== */

/* AREA IC_DISTRIBUTOR */
/* Area of ic_distributor */

/* ===================================================================== */

#define Addr_NX4000_ic_distributor 0xFAF01000U

/* --------------------------------------------------------------------- */
/* Register icd_ctrl */
/* => Distributor Control Register (ICDDCR) */
/*    Controls whether the distributor responds to external stimulus changes that occur on SPI and PPI signals. */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_ctrl            0x00000000U
#define Adr_NX4000_ic_distributor_icd_ctrl 0xFAF01000U
#define Adr_NX4000_icd_ctrl                0xFAF01000U
#define DFLT_VAL_NX4000_icd_ctrl           0x00000000U

#define MSK_NX4000_icd_ctrl_icd_en         0x00000001U
#define SRT_NX4000_icd_ctrl_icd_en         0
#define DFLT_VAL_NX4000_icd_ctrl_icd_en    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_ctrl_icd_en 0x00000000U
#define MSK_NX4000_icd_ctrl_res            0xfffffffeU
#define SRT_NX4000_icd_ctrl_res            1
#define DFLT_VAL_NX4000_icd_ctrl_res       0x00000000U
#define DFLT_BF_VAL_NX4000_icd_ctrl_res    0x00000000U

/* all used bits of 'NX4000_icd_ctrl': */
#define MSK_USED_BITS_NX4000_icd_ctrl 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_ic_type */
/* => Interrupt Controller Type Register (ICDICTR) */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_ic_type            0x00000004U
#define Adr_NX4000_ic_distributor_icd_ic_type 0xFAF01004U
#define Adr_NX4000_icd_ic_type                0xFAF01004U

#define MSK_NX4000_icd_ic_type_it_lines_number 0x0000001fU
#define SRT_NX4000_icd_ic_type_it_lines_number 0
#define MSK_NX4000_icd_ic_type_cpu_number      0x000000e0U
#define SRT_NX4000_icd_ic_type_cpu_number      5
#define MSK_NX4000_icd_ic_type_res             0xffffff00U
#define SRT_NX4000_icd_ic_type_res             8

/* all used bits of 'NX4000_icd_ic_type': */
#define MSK_USED_BITS_NX4000_icd_ic_type 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_impl_id */
/* => Distributor Implementer Identification Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_impl_id            0x00000008U
#define Adr_NX4000_ic_distributor_icd_impl_id 0xFAF01008U
#define Adr_NX4000_icd_impl_id                0xFAF01008U

#define MSK_NX4000_icd_impl_id_val 0xffffffffU
#define SRT_NX4000_icd_impl_id_val 0

/* all used bits of 'NX4000_icd_impl_id': */
#define MSK_USED_BITS_NX4000_icd_impl_id 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_set_en0 */
/* => Interrupt Set-Enable Registers (ICDISERn) */
/*    The ICDISERs provide a Set-enable bit for each interrupt supported by the GIC. */
/*    Writing 1 to a Set-enable bit enables forwarding of the corresponding interrupt to */
/*    the CPU interfaces. Reading a bit identifies whether the interrupt is enabled. */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_set_en0            0x00000100U
#define Adr_NX4000_ic_distributor_icd_set_en0 0xFAF01100U
#define Adr_NX4000_icd_set_en0                0xFAF01100U
#define DFLT_VAL_NX4000_icd_set_en0           0x00000000U

#define MSK_NX4000_icd_set_en0_val         0xffffffffU
#define SRT_NX4000_icd_set_en0_val         0
#define DFLT_VAL_NX4000_icd_set_en0_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_set_en0_val 0x00000000U

/* all used bits of 'NX4000_icd_set_en0': */
#define MSK_USED_BITS_NX4000_icd_set_en0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_set_en1 */
/* => See register icd_set_en0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_set_en1            0x00000104U
#define Adr_NX4000_ic_distributor_icd_set_en1 0xFAF01104U
#define Adr_NX4000_icd_set_en1                0xFAF01104U
#define DFLT_VAL_NX4000_icd_set_en1           0x00000000U

#define MSK_NX4000_icd_set_en1_val         0xffffffffU
#define SRT_NX4000_icd_set_en1_val         0
#define DFLT_VAL_NX4000_icd_set_en1_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_set_en1_val 0x00000000U

/* all used bits of 'NX4000_icd_set_en1': */
#define MSK_USED_BITS_NX4000_icd_set_en1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_set_en2 */
/* => See register icd_set_en0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_set_en2            0x00000108U
#define Adr_NX4000_ic_distributor_icd_set_en2 0xFAF01108U
#define Adr_NX4000_icd_set_en2                0xFAF01108U
#define DFLT_VAL_NX4000_icd_set_en2           0x00000000U

#define MSK_NX4000_icd_set_en2_val         0xffffffffU
#define SRT_NX4000_icd_set_en2_val         0
#define DFLT_VAL_NX4000_icd_set_en2_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_set_en2_val 0x00000000U

/* all used bits of 'NX4000_icd_set_en2': */
#define MSK_USED_BITS_NX4000_icd_set_en2 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_set_en3 */
/* => See register icd_set_en0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_set_en3            0x0000010CU
#define Adr_NX4000_ic_distributor_icd_set_en3 0xFAF0110CU
#define Adr_NX4000_icd_set_en3                0xFAF0110CU
#define DFLT_VAL_NX4000_icd_set_en3           0x00000000U

#define MSK_NX4000_icd_set_en3_val         0xffffffffU
#define SRT_NX4000_icd_set_en3_val         0
#define DFLT_VAL_NX4000_icd_set_en3_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_set_en3_val 0x00000000U

/* all used bits of 'NX4000_icd_set_en3': */
#define MSK_USED_BITS_NX4000_icd_set_en3 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_set_en4 */
/* => See register icd_set_en0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_set_en4            0x00000110U
#define Adr_NX4000_ic_distributor_icd_set_en4 0xFAF01110U
#define Adr_NX4000_icd_set_en4                0xFAF01110U
#define DFLT_VAL_NX4000_icd_set_en4           0x00000000U

#define MSK_NX4000_icd_set_en4_val         0xffffffffU
#define SRT_NX4000_icd_set_en4_val         0
#define DFLT_VAL_NX4000_icd_set_en4_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_set_en4_val 0x00000000U

/* all used bits of 'NX4000_icd_set_en4': */
#define MSK_USED_BITS_NX4000_icd_set_en4 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_set_en5 */
/* => See register icd_set_en0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_set_en5            0x00000114U
#define Adr_NX4000_ic_distributor_icd_set_en5 0xFAF01114U
#define Adr_NX4000_icd_set_en5                0xFAF01114U
#define DFLT_VAL_NX4000_icd_set_en5           0x00000000U

#define MSK_NX4000_icd_set_en5_val         0xffffffffU
#define SRT_NX4000_icd_set_en5_val         0
#define DFLT_VAL_NX4000_icd_set_en5_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_set_en5_val 0x00000000U

/* all used bits of 'NX4000_icd_set_en5': */
#define MSK_USED_BITS_NX4000_icd_set_en5 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_set_en6 */
/* => See register icd_set_en0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_set_en6            0x00000118U
#define Adr_NX4000_ic_distributor_icd_set_en6 0xFAF01118U
#define Adr_NX4000_icd_set_en6                0xFAF01118U
#define DFLT_VAL_NX4000_icd_set_en6           0x00000000U

#define MSK_NX4000_icd_set_en6_val         0xffffffffU
#define SRT_NX4000_icd_set_en6_val         0
#define DFLT_VAL_NX4000_icd_set_en6_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_set_en6_val 0x00000000U

/* all used bits of 'NX4000_icd_set_en6': */
#define MSK_USED_BITS_NX4000_icd_set_en6 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_set_en7 */
/* => See register icd_set_en0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_set_en7            0x0000011CU
#define Adr_NX4000_ic_distributor_icd_set_en7 0xFAF0111CU
#define Adr_NX4000_icd_set_en7                0xFAF0111CU
#define DFLT_VAL_NX4000_icd_set_en7           0x00000000U

#define MSK_NX4000_icd_set_en7_val         0xffffffffU
#define SRT_NX4000_icd_set_en7_val         0
#define DFLT_VAL_NX4000_icd_set_en7_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_set_en7_val 0x00000000U

/* all used bits of 'NX4000_icd_set_en7': */
#define MSK_USED_BITS_NX4000_icd_set_en7 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_clr_en0 */
/* => Interrupt Clear-Enable Registers (ICDICERn) */
/*    The ICDICERs provide a Clear-enable bit for each interrupt supported by the GIC. */
/*    Writing 1 to a Clear-enable bit disables forwarding of the corresponding interrupt to */
/*    the CPU interfaces. Reading a bit identifies whether the interrupt is enabled. */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_clr_en0            0x00000180U
#define Adr_NX4000_ic_distributor_icd_clr_en0 0xFAF01180U
#define Adr_NX4000_icd_clr_en0                0xFAF01180U
#define DFLT_VAL_NX4000_icd_clr_en0           0x00000000U

#define MSK_NX4000_icd_clr_en0_val         0xffffffffU
#define SRT_NX4000_icd_clr_en0_val         0
#define DFLT_VAL_NX4000_icd_clr_en0_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_clr_en0_val 0x00000000U

/* all used bits of 'NX4000_icd_clr_en0': */
#define MSK_USED_BITS_NX4000_icd_clr_en0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_clr_en1 */
/* => See register icd_clr_en0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_clr_en1            0x00000184U
#define Adr_NX4000_ic_distributor_icd_clr_en1 0xFAF01184U
#define Adr_NX4000_icd_clr_en1                0xFAF01184U
#define DFLT_VAL_NX4000_icd_clr_en1           0x00000000U

#define MSK_NX4000_icd_clr_en1_val         0xffffffffU
#define SRT_NX4000_icd_clr_en1_val         0
#define DFLT_VAL_NX4000_icd_clr_en1_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_clr_en1_val 0x00000000U

/* all used bits of 'NX4000_icd_clr_en1': */
#define MSK_USED_BITS_NX4000_icd_clr_en1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_clr_en2 */
/* => See register icd_clr_en0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_clr_en2            0x00000188U
#define Adr_NX4000_ic_distributor_icd_clr_en2 0xFAF01188U
#define Adr_NX4000_icd_clr_en2                0xFAF01188U
#define DFLT_VAL_NX4000_icd_clr_en2           0x00000000U

#define MSK_NX4000_icd_clr_en2_val         0xffffffffU
#define SRT_NX4000_icd_clr_en2_val         0
#define DFLT_VAL_NX4000_icd_clr_en2_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_clr_en2_val 0x00000000U

/* all used bits of 'NX4000_icd_clr_en2': */
#define MSK_USED_BITS_NX4000_icd_clr_en2 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_clr_en3 */
/* => See register icd_clr_en0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_clr_en3            0x0000018CU
#define Adr_NX4000_ic_distributor_icd_clr_en3 0xFAF0118CU
#define Adr_NX4000_icd_clr_en3                0xFAF0118CU
#define DFLT_VAL_NX4000_icd_clr_en3           0x00000000U

#define MSK_NX4000_icd_clr_en3_val         0xffffffffU
#define SRT_NX4000_icd_clr_en3_val         0
#define DFLT_VAL_NX4000_icd_clr_en3_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_clr_en3_val 0x00000000U

/* all used bits of 'NX4000_icd_clr_en3': */
#define MSK_USED_BITS_NX4000_icd_clr_en3 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_clr_en4 */
/* => See register icd_clr_en0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_clr_en4            0x00000190U
#define Adr_NX4000_ic_distributor_icd_clr_en4 0xFAF01190U
#define Adr_NX4000_icd_clr_en4                0xFAF01190U
#define DFLT_VAL_NX4000_icd_clr_en4           0x00000000U

#define MSK_NX4000_icd_clr_en4_val         0xffffffffU
#define SRT_NX4000_icd_clr_en4_val         0
#define DFLT_VAL_NX4000_icd_clr_en4_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_clr_en4_val 0x00000000U

/* all used bits of 'NX4000_icd_clr_en4': */
#define MSK_USED_BITS_NX4000_icd_clr_en4 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_clr_en5 */
/* => See register icd_clr_en0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_clr_en5            0x00000194U
#define Adr_NX4000_ic_distributor_icd_clr_en5 0xFAF01194U
#define Adr_NX4000_icd_clr_en5                0xFAF01194U
#define DFLT_VAL_NX4000_icd_clr_en5           0x00000000U

#define MSK_NX4000_icd_clr_en5_val         0xffffffffU
#define SRT_NX4000_icd_clr_en5_val         0
#define DFLT_VAL_NX4000_icd_clr_en5_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_clr_en5_val 0x00000000U

/* all used bits of 'NX4000_icd_clr_en5': */
#define MSK_USED_BITS_NX4000_icd_clr_en5 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_clr_en6 */
/* => See register icd_clr_en0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_clr_en6            0x00000198U
#define Adr_NX4000_ic_distributor_icd_clr_en6 0xFAF01198U
#define Adr_NX4000_icd_clr_en6                0xFAF01198U
#define DFLT_VAL_NX4000_icd_clr_en6           0x00000000U

#define MSK_NX4000_icd_clr_en6_val         0xffffffffU
#define SRT_NX4000_icd_clr_en6_val         0
#define DFLT_VAL_NX4000_icd_clr_en6_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_clr_en6_val 0x00000000U

/* all used bits of 'NX4000_icd_clr_en6': */
#define MSK_USED_BITS_NX4000_icd_clr_en6 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_clr_en7 */
/* => See register icd_clr_en0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_clr_en7            0x0000019CU
#define Adr_NX4000_ic_distributor_icd_clr_en7 0xFAF0119CU
#define Adr_NX4000_icd_clr_en7                0xFAF0119CU
#define DFLT_VAL_NX4000_icd_clr_en7           0x00000000U

#define MSK_NX4000_icd_clr_en7_val         0xffffffffU
#define SRT_NX4000_icd_clr_en7_val         0
#define DFLT_VAL_NX4000_icd_clr_en7_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_clr_en7_val 0x00000000U

/* all used bits of 'NX4000_icd_clr_en7': */
#define MSK_USED_BITS_NX4000_icd_clr_en7 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_set_pending0 */
/* => Interrupt Set-Pending Registers (ICDISPRn) */
/*    The ICDISPRs provide a Set-pending bit for each interrupt supported by the GIC. */
/*    Writing 1 to a Set-pending bit sets the status of the corresponding peripheral */
/*    interrupt to pending. Reading a bit identifies whether the interrupt is pending. */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_set_pending0            0x00000200U
#define Adr_NX4000_ic_distributor_icd_set_pending0 0xFAF01200U
#define Adr_NX4000_icd_set_pending0                0xFAF01200U
#define DFLT_VAL_NX4000_icd_set_pending0           0x00000000U

#define MSK_NX4000_icd_set_pending0_val         0xffffffffU
#define SRT_NX4000_icd_set_pending0_val         0
#define DFLT_VAL_NX4000_icd_set_pending0_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_set_pending0_val 0x00000000U

/* all used bits of 'NX4000_icd_set_pending0': */
#define MSK_USED_BITS_NX4000_icd_set_pending0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_set_pending1 */
/* => See register icd_set_pending0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_set_pending1            0x00000204U
#define Adr_NX4000_ic_distributor_icd_set_pending1 0xFAF01204U
#define Adr_NX4000_icd_set_pending1                0xFAF01204U
#define DFLT_VAL_NX4000_icd_set_pending1           0x00000000U

#define MSK_NX4000_icd_set_pending1_val         0xffffffffU
#define SRT_NX4000_icd_set_pending1_val         0
#define DFLT_VAL_NX4000_icd_set_pending1_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_set_pending1_val 0x00000000U

/* all used bits of 'NX4000_icd_set_pending1': */
#define MSK_USED_BITS_NX4000_icd_set_pending1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_set_pending2 */
/* => See register icd_set_pending0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_set_pending2            0x00000208U
#define Adr_NX4000_ic_distributor_icd_set_pending2 0xFAF01208U
#define Adr_NX4000_icd_set_pending2                0xFAF01208U
#define DFLT_VAL_NX4000_icd_set_pending2           0x00000000U

#define MSK_NX4000_icd_set_pending2_val         0xffffffffU
#define SRT_NX4000_icd_set_pending2_val         0
#define DFLT_VAL_NX4000_icd_set_pending2_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_set_pending2_val 0x00000000U

/* all used bits of 'NX4000_icd_set_pending2': */
#define MSK_USED_BITS_NX4000_icd_set_pending2 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_set_pending3 */
/* => See register icd_set_pending0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_set_pending3            0x0000020CU
#define Adr_NX4000_ic_distributor_icd_set_pending3 0xFAF0120CU
#define Adr_NX4000_icd_set_pending3                0xFAF0120CU
#define DFLT_VAL_NX4000_icd_set_pending3           0x00000000U

#define MSK_NX4000_icd_set_pending3_val         0xffffffffU
#define SRT_NX4000_icd_set_pending3_val         0
#define DFLT_VAL_NX4000_icd_set_pending3_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_set_pending3_val 0x00000000U

/* all used bits of 'NX4000_icd_set_pending3': */
#define MSK_USED_BITS_NX4000_icd_set_pending3 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_set_pending4 */
/* => See register icd_set_pending0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_set_pending4            0x00000210U
#define Adr_NX4000_ic_distributor_icd_set_pending4 0xFAF01210U
#define Adr_NX4000_icd_set_pending4                0xFAF01210U
#define DFLT_VAL_NX4000_icd_set_pending4           0x00000000U

#define MSK_NX4000_icd_set_pending4_val         0xffffffffU
#define SRT_NX4000_icd_set_pending4_val         0
#define DFLT_VAL_NX4000_icd_set_pending4_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_set_pending4_val 0x00000000U

/* all used bits of 'NX4000_icd_set_pending4': */
#define MSK_USED_BITS_NX4000_icd_set_pending4 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_set_pending5 */
/* => See register icd_set_pending0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_set_pending5            0x00000214U
#define Adr_NX4000_ic_distributor_icd_set_pending5 0xFAF01214U
#define Adr_NX4000_icd_set_pending5                0xFAF01214U
#define DFLT_VAL_NX4000_icd_set_pending5           0x00000000U

#define MSK_NX4000_icd_set_pending5_val         0xffffffffU
#define SRT_NX4000_icd_set_pending5_val         0
#define DFLT_VAL_NX4000_icd_set_pending5_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_set_pending5_val 0x00000000U

/* all used bits of 'NX4000_icd_set_pending5': */
#define MSK_USED_BITS_NX4000_icd_set_pending5 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_set_pending6 */
/* => See register icd_set_pending0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_set_pending6            0x00000218U
#define Adr_NX4000_ic_distributor_icd_set_pending6 0xFAF01218U
#define Adr_NX4000_icd_set_pending6                0xFAF01218U
#define DFLT_VAL_NX4000_icd_set_pending6           0x00000000U

#define MSK_NX4000_icd_set_pending6_val         0xffffffffU
#define SRT_NX4000_icd_set_pending6_val         0
#define DFLT_VAL_NX4000_icd_set_pending6_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_set_pending6_val 0x00000000U

/* all used bits of 'NX4000_icd_set_pending6': */
#define MSK_USED_BITS_NX4000_icd_set_pending6 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_set_pending7 */
/* => See register icd_set_pending0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_set_pending7            0x0000021CU
#define Adr_NX4000_ic_distributor_icd_set_pending7 0xFAF0121CU
#define Adr_NX4000_icd_set_pending7                0xFAF0121CU
#define DFLT_VAL_NX4000_icd_set_pending7           0x00000000U

#define MSK_NX4000_icd_set_pending7_val         0xffffffffU
#define SRT_NX4000_icd_set_pending7_val         0
#define DFLT_VAL_NX4000_icd_set_pending7_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_set_pending7_val 0x00000000U

/* all used bits of 'NX4000_icd_set_pending7': */
#define MSK_USED_BITS_NX4000_icd_set_pending7 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_clr_pending0 */
/* => Interrupt Clear-Pending Registers (ICDICPRn) */
/*    The ICDICPRs provide a Clear-pending bit for each interrupt supported by the GIC. */
/*    Writing 1 to a Clear-pending bit clears the pending status of the corresponding */
/*    peripheral interrupt. Reading a bit identifies whether the interrupt is pending. */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_clr_pending0            0x00000280U
#define Adr_NX4000_ic_distributor_icd_clr_pending0 0xFAF01280U
#define Adr_NX4000_icd_clr_pending0                0xFAF01280U
#define DFLT_VAL_NX4000_icd_clr_pending0           0x00000000U

#define MSK_NX4000_icd_clr_pending0_val         0xffffffffU
#define SRT_NX4000_icd_clr_pending0_val         0
#define DFLT_VAL_NX4000_icd_clr_pending0_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_clr_pending0_val 0x00000000U

/* all used bits of 'NX4000_icd_clr_pending0': */
#define MSK_USED_BITS_NX4000_icd_clr_pending0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_clr_pending1 */
/* => See register icd_clr_pending0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_clr_pending1            0x00000284U
#define Adr_NX4000_ic_distributor_icd_clr_pending1 0xFAF01284U
#define Adr_NX4000_icd_clr_pending1                0xFAF01284U
#define DFLT_VAL_NX4000_icd_clr_pending1           0x00000000U

#define MSK_NX4000_icd_clr_pending1_val         0xffffffffU
#define SRT_NX4000_icd_clr_pending1_val         0
#define DFLT_VAL_NX4000_icd_clr_pending1_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_clr_pending1_val 0x00000000U

/* all used bits of 'NX4000_icd_clr_pending1': */
#define MSK_USED_BITS_NX4000_icd_clr_pending1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_clr_pending2 */
/* => See register icd_clr_pending0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_clr_pending2            0x00000288U
#define Adr_NX4000_ic_distributor_icd_clr_pending2 0xFAF01288U
#define Adr_NX4000_icd_clr_pending2                0xFAF01288U
#define DFLT_VAL_NX4000_icd_clr_pending2           0x00000000U

#define MSK_NX4000_icd_clr_pending2_val         0xffffffffU
#define SRT_NX4000_icd_clr_pending2_val         0
#define DFLT_VAL_NX4000_icd_clr_pending2_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_clr_pending2_val 0x00000000U

/* all used bits of 'NX4000_icd_clr_pending2': */
#define MSK_USED_BITS_NX4000_icd_clr_pending2 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_clr_pending3 */
/* => See register icd_clr_pending0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_clr_pending3            0x0000028CU
#define Adr_NX4000_ic_distributor_icd_clr_pending3 0xFAF0128CU
#define Adr_NX4000_icd_clr_pending3                0xFAF0128CU
#define DFLT_VAL_NX4000_icd_clr_pending3           0x00000000U

#define MSK_NX4000_icd_clr_pending3_val         0xffffffffU
#define SRT_NX4000_icd_clr_pending3_val         0
#define DFLT_VAL_NX4000_icd_clr_pending3_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_clr_pending3_val 0x00000000U

/* all used bits of 'NX4000_icd_clr_pending3': */
#define MSK_USED_BITS_NX4000_icd_clr_pending3 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_clr_pending4 */
/* => See register icd_clr_pending0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_clr_pending4            0x00000290U
#define Adr_NX4000_ic_distributor_icd_clr_pending4 0xFAF01290U
#define Adr_NX4000_icd_clr_pending4                0xFAF01290U
#define DFLT_VAL_NX4000_icd_clr_pending4           0x00000000U

#define MSK_NX4000_icd_clr_pending4_val         0xffffffffU
#define SRT_NX4000_icd_clr_pending4_val         0
#define DFLT_VAL_NX4000_icd_clr_pending4_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_clr_pending4_val 0x00000000U

/* all used bits of 'NX4000_icd_clr_pending4': */
#define MSK_USED_BITS_NX4000_icd_clr_pending4 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_clr_pending5 */
/* => See register icd_clr_pending0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_clr_pending5            0x00000294U
#define Adr_NX4000_ic_distributor_icd_clr_pending5 0xFAF01294U
#define Adr_NX4000_icd_clr_pending5                0xFAF01294U
#define DFLT_VAL_NX4000_icd_clr_pending5           0x00000000U

#define MSK_NX4000_icd_clr_pending5_val         0xffffffffU
#define SRT_NX4000_icd_clr_pending5_val         0
#define DFLT_VAL_NX4000_icd_clr_pending5_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_clr_pending5_val 0x00000000U

/* all used bits of 'NX4000_icd_clr_pending5': */
#define MSK_USED_BITS_NX4000_icd_clr_pending5 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_clr_pending6 */
/* => See register icd_clr_pending0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_clr_pending6            0x00000298U
#define Adr_NX4000_ic_distributor_icd_clr_pending6 0xFAF01298U
#define Adr_NX4000_icd_clr_pending6                0xFAF01298U
#define DFLT_VAL_NX4000_icd_clr_pending6           0x00000000U

#define MSK_NX4000_icd_clr_pending6_val         0xffffffffU
#define SRT_NX4000_icd_clr_pending6_val         0
#define DFLT_VAL_NX4000_icd_clr_pending6_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_clr_pending6_val 0x00000000U

/* all used bits of 'NX4000_icd_clr_pending6': */
#define MSK_USED_BITS_NX4000_icd_clr_pending6 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_clr_pending7 */
/* => See register icd_clr_pending0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_clr_pending7            0x0000029CU
#define Adr_NX4000_ic_distributor_icd_clr_pending7 0xFAF0129CU
#define Adr_NX4000_icd_clr_pending7                0xFAF0129CU
#define DFLT_VAL_NX4000_icd_clr_pending7           0x00000000U

#define MSK_NX4000_icd_clr_pending7_val         0xffffffffU
#define SRT_NX4000_icd_clr_pending7_val         0
#define DFLT_VAL_NX4000_icd_clr_pending7_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_clr_pending7_val 0x00000000U

/* all used bits of 'NX4000_icd_clr_pending7': */
#define MSK_USED_BITS_NX4000_icd_clr_pending7 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_active0 */
/* => Active Bit Registers (ICDABRn) */
/*    The ICDABRs provide an Active bit for each interrupt supported by the GIC. */
/*    Reading an Active bit identifies whether the corresponding interrupt is active. */
/*    Note: The bit reads as one if the status of the interrupt is active or active and pending. Read */
/*    the ICDSPR or ICDCPR to find the pending status of the interrupt. */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_active0            0x00000300U
#define Adr_NX4000_ic_distributor_icd_active0 0xFAF01300U
#define Adr_NX4000_icd_active0                0xFAF01300U

#define MSK_NX4000_icd_active0_val 0xffffffffU
#define SRT_NX4000_icd_active0_val 0

/* all used bits of 'NX4000_icd_active0': */
#define MSK_USED_BITS_NX4000_icd_active0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_active1 */
/* => See register icd_active0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_active1            0x00000304U
#define Adr_NX4000_ic_distributor_icd_active1 0xFAF01304U
#define Adr_NX4000_icd_active1                0xFAF01304U
#define DFLT_VAL_NX4000_icd_active1           0x00000000U

#define MSK_NX4000_icd_active1_val         0xffffffffU
#define SRT_NX4000_icd_active1_val         0
#define DFLT_VAL_NX4000_icd_active1_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_active1_val 0x00000000U

/* all used bits of 'NX4000_icd_active1': */
#define MSK_USED_BITS_NX4000_icd_active1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_active2 */
/* => See register icd_active0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_active2            0x00000308U
#define Adr_NX4000_ic_distributor_icd_active2 0xFAF01308U
#define Adr_NX4000_icd_active2                0xFAF01308U
#define DFLT_VAL_NX4000_icd_active2           0x00000000U

#define MSK_NX4000_icd_active2_val         0xffffffffU
#define SRT_NX4000_icd_active2_val         0
#define DFLT_VAL_NX4000_icd_active2_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_active2_val 0x00000000U

/* all used bits of 'NX4000_icd_active2': */
#define MSK_USED_BITS_NX4000_icd_active2 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_active3 */
/* => See register icd_active0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_active3            0x0000030CU
#define Adr_NX4000_ic_distributor_icd_active3 0xFAF0130CU
#define Adr_NX4000_icd_active3                0xFAF0130CU
#define DFLT_VAL_NX4000_icd_active3           0x00000000U

#define MSK_NX4000_icd_active3_val         0xffffffffU
#define SRT_NX4000_icd_active3_val         0
#define DFLT_VAL_NX4000_icd_active3_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_active3_val 0x00000000U

/* all used bits of 'NX4000_icd_active3': */
#define MSK_USED_BITS_NX4000_icd_active3 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_active4 */
/* => See register icd_active0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_active4            0x00000310U
#define Adr_NX4000_ic_distributor_icd_active4 0xFAF01310U
#define Adr_NX4000_icd_active4                0xFAF01310U
#define DFLT_VAL_NX4000_icd_active4           0x00000000U

#define MSK_NX4000_icd_active4_val         0xffffffffU
#define SRT_NX4000_icd_active4_val         0
#define DFLT_VAL_NX4000_icd_active4_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_active4_val 0x00000000U

/* all used bits of 'NX4000_icd_active4': */
#define MSK_USED_BITS_NX4000_icd_active4 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_active5 */
/* => See register icd_active0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_active5            0x00000314U
#define Adr_NX4000_ic_distributor_icd_active5 0xFAF01314U
#define Adr_NX4000_icd_active5                0xFAF01314U
#define DFLT_VAL_NX4000_icd_active5           0x00000000U

#define MSK_NX4000_icd_active5_val         0xffffffffU
#define SRT_NX4000_icd_active5_val         0
#define DFLT_VAL_NX4000_icd_active5_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_active5_val 0x00000000U

/* all used bits of 'NX4000_icd_active5': */
#define MSK_USED_BITS_NX4000_icd_active5 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_active6 */
/* => See register icd_active0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_active6            0x00000318U
#define Adr_NX4000_ic_distributor_icd_active6 0xFAF01318U
#define Adr_NX4000_icd_active6                0xFAF01318U
#define DFLT_VAL_NX4000_icd_active6           0x00000000U

#define MSK_NX4000_icd_active6_val         0xffffffffU
#define SRT_NX4000_icd_active6_val         0
#define DFLT_VAL_NX4000_icd_active6_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_active6_val 0x00000000U

/* all used bits of 'NX4000_icd_active6': */
#define MSK_USED_BITS_NX4000_icd_active6 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_active7 */
/* => See register icd_active0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_active7            0x0000031CU
#define Adr_NX4000_ic_distributor_icd_active7 0xFAF0131CU
#define Adr_NX4000_icd_active7                0xFAF0131CU
#define DFLT_VAL_NX4000_icd_active7           0x00000000U

#define MSK_NX4000_icd_active7_val         0xffffffffU
#define SRT_NX4000_icd_active7_val         0
#define DFLT_VAL_NX4000_icd_active7_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_active7_val 0x00000000U

/* all used bits of 'NX4000_icd_active7': */
#define MSK_USED_BITS_NX4000_icd_active7 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio0 */
/* => Interrupt Priority Registers (ICDIPRn) */
/*    The ICDIPRs provide an 8-bit Priority field for each interrupt supported by the GIC. */
/*    This field stores the priority of the corresponding interrupt. */
/*    These registers are byte accessible. */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio0            0x00000400U
#define Adr_NX4000_ic_distributor_icd_prio0 0xFAF01400U
#define Adr_NX4000_icd_prio0                0xFAF01400U
#define DFLT_VAL_NX4000_icd_prio0           0x00000000U

#define MSK_NX4000_icd_prio0_val         0xffffffffU
#define SRT_NX4000_icd_prio0_val         0
#define DFLT_VAL_NX4000_icd_prio0_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio0_val 0x00000000U

/* all used bits of 'NX4000_icd_prio0': */
#define MSK_USED_BITS_NX4000_icd_prio0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio1 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio1            0x00000404U
#define Adr_NX4000_ic_distributor_icd_prio1 0xFAF01404U
#define Adr_NX4000_icd_prio1                0xFAF01404U
#define DFLT_VAL_NX4000_icd_prio1           0x00000000U

#define MSK_NX4000_icd_prio1_val         0xffffffffU
#define SRT_NX4000_icd_prio1_val         0
#define DFLT_VAL_NX4000_icd_prio1_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio1_val 0x00000000U

/* all used bits of 'NX4000_icd_prio1': */
#define MSK_USED_BITS_NX4000_icd_prio1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio2 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio2            0x00000408U
#define Adr_NX4000_ic_distributor_icd_prio2 0xFAF01408U
#define Adr_NX4000_icd_prio2                0xFAF01408U
#define DFLT_VAL_NX4000_icd_prio2           0x00000000U

#define MSK_NX4000_icd_prio2_val         0xffffffffU
#define SRT_NX4000_icd_prio2_val         0
#define DFLT_VAL_NX4000_icd_prio2_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio2_val 0x00000000U

/* all used bits of 'NX4000_icd_prio2': */
#define MSK_USED_BITS_NX4000_icd_prio2 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio3 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio3            0x0000040CU
#define Adr_NX4000_ic_distributor_icd_prio3 0xFAF0140CU
#define Adr_NX4000_icd_prio3                0xFAF0140CU
#define DFLT_VAL_NX4000_icd_prio3           0x00000000U

#define MSK_NX4000_icd_prio3_val         0xffffffffU
#define SRT_NX4000_icd_prio3_val         0
#define DFLT_VAL_NX4000_icd_prio3_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio3_val 0x00000000U

/* all used bits of 'NX4000_icd_prio3': */
#define MSK_USED_BITS_NX4000_icd_prio3 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio4 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio4            0x00000410U
#define Adr_NX4000_ic_distributor_icd_prio4 0xFAF01410U
#define Adr_NX4000_icd_prio4                0xFAF01410U
#define DFLT_VAL_NX4000_icd_prio4           0x00000000U

#define MSK_NX4000_icd_prio4_val         0xffffffffU
#define SRT_NX4000_icd_prio4_val         0
#define DFLT_VAL_NX4000_icd_prio4_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio4_val 0x00000000U

/* all used bits of 'NX4000_icd_prio4': */
#define MSK_USED_BITS_NX4000_icd_prio4 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio5 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio5            0x00000414U
#define Adr_NX4000_ic_distributor_icd_prio5 0xFAF01414U
#define Adr_NX4000_icd_prio5                0xFAF01414U
#define DFLT_VAL_NX4000_icd_prio5           0x00000000U

#define MSK_NX4000_icd_prio5_val         0xffffffffU
#define SRT_NX4000_icd_prio5_val         0
#define DFLT_VAL_NX4000_icd_prio5_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio5_val 0x00000000U

/* all used bits of 'NX4000_icd_prio5': */
#define MSK_USED_BITS_NX4000_icd_prio5 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio6 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio6            0x00000418U
#define Adr_NX4000_ic_distributor_icd_prio6 0xFAF01418U
#define Adr_NX4000_icd_prio6                0xFAF01418U
#define DFLT_VAL_NX4000_icd_prio6           0x00000000U

#define MSK_NX4000_icd_prio6_val         0xffffffffU
#define SRT_NX4000_icd_prio6_val         0
#define DFLT_VAL_NX4000_icd_prio6_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio6_val 0x00000000U

/* all used bits of 'NX4000_icd_prio6': */
#define MSK_USED_BITS_NX4000_icd_prio6 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio7 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio7            0x0000041CU
#define Adr_NX4000_ic_distributor_icd_prio7 0xFAF0141CU
#define Adr_NX4000_icd_prio7                0xFAF0141CU
#define DFLT_VAL_NX4000_icd_prio7           0x00000000U

#define MSK_NX4000_icd_prio7_val         0xffffffffU
#define SRT_NX4000_icd_prio7_val         0
#define DFLT_VAL_NX4000_icd_prio7_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio7_val 0x00000000U

/* all used bits of 'NX4000_icd_prio7': */
#define MSK_USED_BITS_NX4000_icd_prio7 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio8 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio8            0x00000420U
#define Adr_NX4000_ic_distributor_icd_prio8 0xFAF01420U
#define Adr_NX4000_icd_prio8                0xFAF01420U
#define DFLT_VAL_NX4000_icd_prio8           0x00000000U

#define MSK_NX4000_icd_prio8_val         0xffffffffU
#define SRT_NX4000_icd_prio8_val         0
#define DFLT_VAL_NX4000_icd_prio8_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio8_val 0x00000000U

/* all used bits of 'NX4000_icd_prio8': */
#define MSK_USED_BITS_NX4000_icd_prio8 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio9 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio9            0x00000424U
#define Adr_NX4000_ic_distributor_icd_prio9 0xFAF01424U
#define Adr_NX4000_icd_prio9                0xFAF01424U
#define DFLT_VAL_NX4000_icd_prio9           0x00000000U

#define MSK_NX4000_icd_prio9_val         0xffffffffU
#define SRT_NX4000_icd_prio9_val         0
#define DFLT_VAL_NX4000_icd_prio9_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio9_val 0x00000000U

/* all used bits of 'NX4000_icd_prio9': */
#define MSK_USED_BITS_NX4000_icd_prio9 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio10 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio10            0x00000428U
#define Adr_NX4000_ic_distributor_icd_prio10 0xFAF01428U
#define Adr_NX4000_icd_prio10                0xFAF01428U
#define DFLT_VAL_NX4000_icd_prio10           0x00000000U

#define MSK_NX4000_icd_prio10_val         0xffffffffU
#define SRT_NX4000_icd_prio10_val         0
#define DFLT_VAL_NX4000_icd_prio10_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio10_val 0x00000000U

/* all used bits of 'NX4000_icd_prio10': */
#define MSK_USED_BITS_NX4000_icd_prio10 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio11 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio11            0x0000042CU
#define Adr_NX4000_ic_distributor_icd_prio11 0xFAF0142CU
#define Adr_NX4000_icd_prio11                0xFAF0142CU
#define DFLT_VAL_NX4000_icd_prio11           0x00000000U

#define MSK_NX4000_icd_prio11_val         0xffffffffU
#define SRT_NX4000_icd_prio11_val         0
#define DFLT_VAL_NX4000_icd_prio11_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio11_val 0x00000000U

/* all used bits of 'NX4000_icd_prio11': */
#define MSK_USED_BITS_NX4000_icd_prio11 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio12 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio12            0x00000430U
#define Adr_NX4000_ic_distributor_icd_prio12 0xFAF01430U
#define Adr_NX4000_icd_prio12                0xFAF01430U
#define DFLT_VAL_NX4000_icd_prio12           0x00000000U

#define MSK_NX4000_icd_prio12_val         0xffffffffU
#define SRT_NX4000_icd_prio12_val         0
#define DFLT_VAL_NX4000_icd_prio12_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio12_val 0x00000000U

/* all used bits of 'NX4000_icd_prio12': */
#define MSK_USED_BITS_NX4000_icd_prio12 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio13 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio13            0x00000434U
#define Adr_NX4000_ic_distributor_icd_prio13 0xFAF01434U
#define Adr_NX4000_icd_prio13                0xFAF01434U
#define DFLT_VAL_NX4000_icd_prio13           0x00000000U

#define MSK_NX4000_icd_prio13_val         0xffffffffU
#define SRT_NX4000_icd_prio13_val         0
#define DFLT_VAL_NX4000_icd_prio13_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio13_val 0x00000000U

/* all used bits of 'NX4000_icd_prio13': */
#define MSK_USED_BITS_NX4000_icd_prio13 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio14 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio14            0x00000438U
#define Adr_NX4000_ic_distributor_icd_prio14 0xFAF01438U
#define Adr_NX4000_icd_prio14                0xFAF01438U
#define DFLT_VAL_NX4000_icd_prio14           0x00000000U

#define MSK_NX4000_icd_prio14_val         0xffffffffU
#define SRT_NX4000_icd_prio14_val         0
#define DFLT_VAL_NX4000_icd_prio14_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio14_val 0x00000000U

/* all used bits of 'NX4000_icd_prio14': */
#define MSK_USED_BITS_NX4000_icd_prio14 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio15 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio15            0x0000043CU
#define Adr_NX4000_ic_distributor_icd_prio15 0xFAF0143CU
#define Adr_NX4000_icd_prio15                0xFAF0143CU
#define DFLT_VAL_NX4000_icd_prio15           0x00000000U

#define MSK_NX4000_icd_prio15_val         0xffffffffU
#define SRT_NX4000_icd_prio15_val         0
#define DFLT_VAL_NX4000_icd_prio15_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio15_val 0x00000000U

/* all used bits of 'NX4000_icd_prio15': */
#define MSK_USED_BITS_NX4000_icd_prio15 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio16 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio16            0x00000440U
#define Adr_NX4000_ic_distributor_icd_prio16 0xFAF01440U
#define Adr_NX4000_icd_prio16                0xFAF01440U
#define DFLT_VAL_NX4000_icd_prio16           0x00000000U

#define MSK_NX4000_icd_prio16_val         0xffffffffU
#define SRT_NX4000_icd_prio16_val         0
#define DFLT_VAL_NX4000_icd_prio16_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio16_val 0x00000000U

/* all used bits of 'NX4000_icd_prio16': */
#define MSK_USED_BITS_NX4000_icd_prio16 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio17 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio17            0x00000444U
#define Adr_NX4000_ic_distributor_icd_prio17 0xFAF01444U
#define Adr_NX4000_icd_prio17                0xFAF01444U
#define DFLT_VAL_NX4000_icd_prio17           0x00000000U

#define MSK_NX4000_icd_prio17_val         0xffffffffU
#define SRT_NX4000_icd_prio17_val         0
#define DFLT_VAL_NX4000_icd_prio17_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio17_val 0x00000000U

/* all used bits of 'NX4000_icd_prio17': */
#define MSK_USED_BITS_NX4000_icd_prio17 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio18 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio18            0x00000448U
#define Adr_NX4000_ic_distributor_icd_prio18 0xFAF01448U
#define Adr_NX4000_icd_prio18                0xFAF01448U
#define DFLT_VAL_NX4000_icd_prio18           0x00000000U

#define MSK_NX4000_icd_prio18_val         0xffffffffU
#define SRT_NX4000_icd_prio18_val         0
#define DFLT_VAL_NX4000_icd_prio18_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio18_val 0x00000000U

/* all used bits of 'NX4000_icd_prio18': */
#define MSK_USED_BITS_NX4000_icd_prio18 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio19 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio19            0x0000044CU
#define Adr_NX4000_ic_distributor_icd_prio19 0xFAF0144CU
#define Adr_NX4000_icd_prio19                0xFAF0144CU
#define DFLT_VAL_NX4000_icd_prio19           0x00000000U

#define MSK_NX4000_icd_prio19_val         0xffffffffU
#define SRT_NX4000_icd_prio19_val         0
#define DFLT_VAL_NX4000_icd_prio19_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio19_val 0x00000000U

/* all used bits of 'NX4000_icd_prio19': */
#define MSK_USED_BITS_NX4000_icd_prio19 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio20 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio20            0x00000450U
#define Adr_NX4000_ic_distributor_icd_prio20 0xFAF01450U
#define Adr_NX4000_icd_prio20                0xFAF01450U
#define DFLT_VAL_NX4000_icd_prio20           0x00000000U

#define MSK_NX4000_icd_prio20_val         0xffffffffU
#define SRT_NX4000_icd_prio20_val         0
#define DFLT_VAL_NX4000_icd_prio20_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio20_val 0x00000000U

/* all used bits of 'NX4000_icd_prio20': */
#define MSK_USED_BITS_NX4000_icd_prio20 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio21 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio21            0x00000454U
#define Adr_NX4000_ic_distributor_icd_prio21 0xFAF01454U
#define Adr_NX4000_icd_prio21                0xFAF01454U
#define DFLT_VAL_NX4000_icd_prio21           0x00000000U

#define MSK_NX4000_icd_prio21_val         0xffffffffU
#define SRT_NX4000_icd_prio21_val         0
#define DFLT_VAL_NX4000_icd_prio21_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio21_val 0x00000000U

/* all used bits of 'NX4000_icd_prio21': */
#define MSK_USED_BITS_NX4000_icd_prio21 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio22 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio22            0x00000458U
#define Adr_NX4000_ic_distributor_icd_prio22 0xFAF01458U
#define Adr_NX4000_icd_prio22                0xFAF01458U
#define DFLT_VAL_NX4000_icd_prio22           0x00000000U

#define MSK_NX4000_icd_prio22_val         0xffffffffU
#define SRT_NX4000_icd_prio22_val         0
#define DFLT_VAL_NX4000_icd_prio22_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio22_val 0x00000000U

/* all used bits of 'NX4000_icd_prio22': */
#define MSK_USED_BITS_NX4000_icd_prio22 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio23 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio23            0x0000045CU
#define Adr_NX4000_ic_distributor_icd_prio23 0xFAF0145CU
#define Adr_NX4000_icd_prio23                0xFAF0145CU
#define DFLT_VAL_NX4000_icd_prio23           0x00000000U

#define MSK_NX4000_icd_prio23_val         0xffffffffU
#define SRT_NX4000_icd_prio23_val         0
#define DFLT_VAL_NX4000_icd_prio23_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio23_val 0x00000000U

/* all used bits of 'NX4000_icd_prio23': */
#define MSK_USED_BITS_NX4000_icd_prio23 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio24 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio24            0x00000460U
#define Adr_NX4000_ic_distributor_icd_prio24 0xFAF01460U
#define Adr_NX4000_icd_prio24                0xFAF01460U
#define DFLT_VAL_NX4000_icd_prio24           0x00000000U

#define MSK_NX4000_icd_prio24_val         0xffffffffU
#define SRT_NX4000_icd_prio24_val         0
#define DFLT_VAL_NX4000_icd_prio24_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio24_val 0x00000000U

/* all used bits of 'NX4000_icd_prio24': */
#define MSK_USED_BITS_NX4000_icd_prio24 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio25 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio25            0x00000464U
#define Adr_NX4000_ic_distributor_icd_prio25 0xFAF01464U
#define Adr_NX4000_icd_prio25                0xFAF01464U
#define DFLT_VAL_NX4000_icd_prio25           0x00000000U

#define MSK_NX4000_icd_prio25_val         0xffffffffU
#define SRT_NX4000_icd_prio25_val         0
#define DFLT_VAL_NX4000_icd_prio25_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio25_val 0x00000000U

/* all used bits of 'NX4000_icd_prio25': */
#define MSK_USED_BITS_NX4000_icd_prio25 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio26 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio26            0x00000468U
#define Adr_NX4000_ic_distributor_icd_prio26 0xFAF01468U
#define Adr_NX4000_icd_prio26                0xFAF01468U
#define DFLT_VAL_NX4000_icd_prio26           0x00000000U

#define MSK_NX4000_icd_prio26_val         0xffffffffU
#define SRT_NX4000_icd_prio26_val         0
#define DFLT_VAL_NX4000_icd_prio26_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio26_val 0x00000000U

/* all used bits of 'NX4000_icd_prio26': */
#define MSK_USED_BITS_NX4000_icd_prio26 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio27 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio27            0x0000046CU
#define Adr_NX4000_ic_distributor_icd_prio27 0xFAF0146CU
#define Adr_NX4000_icd_prio27                0xFAF0146CU
#define DFLT_VAL_NX4000_icd_prio27           0x00000000U

#define MSK_NX4000_icd_prio27_val         0xffffffffU
#define SRT_NX4000_icd_prio27_val         0
#define DFLT_VAL_NX4000_icd_prio27_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio27_val 0x00000000U

/* all used bits of 'NX4000_icd_prio27': */
#define MSK_USED_BITS_NX4000_icd_prio27 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio28 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio28            0x00000470U
#define Adr_NX4000_ic_distributor_icd_prio28 0xFAF01470U
#define Adr_NX4000_icd_prio28                0xFAF01470U
#define DFLT_VAL_NX4000_icd_prio28           0x00000000U

#define MSK_NX4000_icd_prio28_val         0xffffffffU
#define SRT_NX4000_icd_prio28_val         0
#define DFLT_VAL_NX4000_icd_prio28_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio28_val 0x00000000U

/* all used bits of 'NX4000_icd_prio28': */
#define MSK_USED_BITS_NX4000_icd_prio28 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio29 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio29            0x00000474U
#define Adr_NX4000_ic_distributor_icd_prio29 0xFAF01474U
#define Adr_NX4000_icd_prio29                0xFAF01474U
#define DFLT_VAL_NX4000_icd_prio29           0x00000000U

#define MSK_NX4000_icd_prio29_val         0xffffffffU
#define SRT_NX4000_icd_prio29_val         0
#define DFLT_VAL_NX4000_icd_prio29_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio29_val 0x00000000U

/* all used bits of 'NX4000_icd_prio29': */
#define MSK_USED_BITS_NX4000_icd_prio29 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio30 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio30            0x00000478U
#define Adr_NX4000_ic_distributor_icd_prio30 0xFAF01478U
#define Adr_NX4000_icd_prio30                0xFAF01478U
#define DFLT_VAL_NX4000_icd_prio30           0x00000000U

#define MSK_NX4000_icd_prio30_val         0xffffffffU
#define SRT_NX4000_icd_prio30_val         0
#define DFLT_VAL_NX4000_icd_prio30_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio30_val 0x00000000U

/* all used bits of 'NX4000_icd_prio30': */
#define MSK_USED_BITS_NX4000_icd_prio30 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio31 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio31            0x0000047CU
#define Adr_NX4000_ic_distributor_icd_prio31 0xFAF0147CU
#define Adr_NX4000_icd_prio31                0xFAF0147CU
#define DFLT_VAL_NX4000_icd_prio31           0x00000000U

#define MSK_NX4000_icd_prio31_val         0xffffffffU
#define SRT_NX4000_icd_prio31_val         0
#define DFLT_VAL_NX4000_icd_prio31_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio31_val 0x00000000U

/* all used bits of 'NX4000_icd_prio31': */
#define MSK_USED_BITS_NX4000_icd_prio31 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio32 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio32            0x00000480U
#define Adr_NX4000_ic_distributor_icd_prio32 0xFAF01480U
#define Adr_NX4000_icd_prio32                0xFAF01480U
#define DFLT_VAL_NX4000_icd_prio32           0x00000000U

#define MSK_NX4000_icd_prio32_val         0xffffffffU
#define SRT_NX4000_icd_prio32_val         0
#define DFLT_VAL_NX4000_icd_prio32_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio32_val 0x00000000U

/* all used bits of 'NX4000_icd_prio32': */
#define MSK_USED_BITS_NX4000_icd_prio32 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio33 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio33            0x00000484U
#define Adr_NX4000_ic_distributor_icd_prio33 0xFAF01484U
#define Adr_NX4000_icd_prio33                0xFAF01484U
#define DFLT_VAL_NX4000_icd_prio33           0x00000000U

#define MSK_NX4000_icd_prio33_val         0xffffffffU
#define SRT_NX4000_icd_prio33_val         0
#define DFLT_VAL_NX4000_icd_prio33_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio33_val 0x00000000U

/* all used bits of 'NX4000_icd_prio33': */
#define MSK_USED_BITS_NX4000_icd_prio33 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio34 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio34            0x00000488U
#define Adr_NX4000_ic_distributor_icd_prio34 0xFAF01488U
#define Adr_NX4000_icd_prio34                0xFAF01488U
#define DFLT_VAL_NX4000_icd_prio34           0x00000000U

#define MSK_NX4000_icd_prio34_val         0xffffffffU
#define SRT_NX4000_icd_prio34_val         0
#define DFLT_VAL_NX4000_icd_prio34_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio34_val 0x00000000U

/* all used bits of 'NX4000_icd_prio34': */
#define MSK_USED_BITS_NX4000_icd_prio34 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio35 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio35            0x0000048CU
#define Adr_NX4000_ic_distributor_icd_prio35 0xFAF0148CU
#define Adr_NX4000_icd_prio35                0xFAF0148CU
#define DFLT_VAL_NX4000_icd_prio35           0x00000000U

#define MSK_NX4000_icd_prio35_val         0xffffffffU
#define SRT_NX4000_icd_prio35_val         0
#define DFLT_VAL_NX4000_icd_prio35_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio35_val 0x00000000U

/* all used bits of 'NX4000_icd_prio35': */
#define MSK_USED_BITS_NX4000_icd_prio35 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio36 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio36            0x00000490U
#define Adr_NX4000_ic_distributor_icd_prio36 0xFAF01490U
#define Adr_NX4000_icd_prio36                0xFAF01490U
#define DFLT_VAL_NX4000_icd_prio36           0x00000000U

#define MSK_NX4000_icd_prio36_val         0xffffffffU
#define SRT_NX4000_icd_prio36_val         0
#define DFLT_VAL_NX4000_icd_prio36_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio36_val 0x00000000U

/* all used bits of 'NX4000_icd_prio36': */
#define MSK_USED_BITS_NX4000_icd_prio36 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio37 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio37            0x00000494U
#define Adr_NX4000_ic_distributor_icd_prio37 0xFAF01494U
#define Adr_NX4000_icd_prio37                0xFAF01494U
#define DFLT_VAL_NX4000_icd_prio37           0x00000000U

#define MSK_NX4000_icd_prio37_val         0xffffffffU
#define SRT_NX4000_icd_prio37_val         0
#define DFLT_VAL_NX4000_icd_prio37_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio37_val 0x00000000U

/* all used bits of 'NX4000_icd_prio37': */
#define MSK_USED_BITS_NX4000_icd_prio37 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio38 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio38            0x00000498U
#define Adr_NX4000_ic_distributor_icd_prio38 0xFAF01498U
#define Adr_NX4000_icd_prio38                0xFAF01498U
#define DFLT_VAL_NX4000_icd_prio38           0x00000000U

#define MSK_NX4000_icd_prio38_val         0xffffffffU
#define SRT_NX4000_icd_prio38_val         0
#define DFLT_VAL_NX4000_icd_prio38_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio38_val 0x00000000U

/* all used bits of 'NX4000_icd_prio38': */
#define MSK_USED_BITS_NX4000_icd_prio38 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio39 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio39            0x0000049CU
#define Adr_NX4000_ic_distributor_icd_prio39 0xFAF0149CU
#define Adr_NX4000_icd_prio39                0xFAF0149CU
#define DFLT_VAL_NX4000_icd_prio39           0x00000000U

#define MSK_NX4000_icd_prio39_val         0xffffffffU
#define SRT_NX4000_icd_prio39_val         0
#define DFLT_VAL_NX4000_icd_prio39_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio39_val 0x00000000U

/* all used bits of 'NX4000_icd_prio39': */
#define MSK_USED_BITS_NX4000_icd_prio39 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio40 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio40            0x000004A0U
#define Adr_NX4000_ic_distributor_icd_prio40 0xFAF014A0U
#define Adr_NX4000_icd_prio40                0xFAF014A0U
#define DFLT_VAL_NX4000_icd_prio40           0x00000000U

#define MSK_NX4000_icd_prio40_val         0xffffffffU
#define SRT_NX4000_icd_prio40_val         0
#define DFLT_VAL_NX4000_icd_prio40_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio40_val 0x00000000U

/* all used bits of 'NX4000_icd_prio40': */
#define MSK_USED_BITS_NX4000_icd_prio40 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio41 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio41            0x000004A4U
#define Adr_NX4000_ic_distributor_icd_prio41 0xFAF014A4U
#define Adr_NX4000_icd_prio41                0xFAF014A4U
#define DFLT_VAL_NX4000_icd_prio41           0x00000000U

#define MSK_NX4000_icd_prio41_val         0xffffffffU
#define SRT_NX4000_icd_prio41_val         0
#define DFLT_VAL_NX4000_icd_prio41_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio41_val 0x00000000U

/* all used bits of 'NX4000_icd_prio41': */
#define MSK_USED_BITS_NX4000_icd_prio41 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio42 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio42            0x000004A8U
#define Adr_NX4000_ic_distributor_icd_prio42 0xFAF014A8U
#define Adr_NX4000_icd_prio42                0xFAF014A8U
#define DFLT_VAL_NX4000_icd_prio42           0x00000000U

#define MSK_NX4000_icd_prio42_val         0xffffffffU
#define SRT_NX4000_icd_prio42_val         0
#define DFLT_VAL_NX4000_icd_prio42_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio42_val 0x00000000U

/* all used bits of 'NX4000_icd_prio42': */
#define MSK_USED_BITS_NX4000_icd_prio42 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio43 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio43            0x000004ACU
#define Adr_NX4000_ic_distributor_icd_prio43 0xFAF014ACU
#define Adr_NX4000_icd_prio43                0xFAF014ACU
#define DFLT_VAL_NX4000_icd_prio43           0x00000000U

#define MSK_NX4000_icd_prio43_val         0xffffffffU
#define SRT_NX4000_icd_prio43_val         0
#define DFLT_VAL_NX4000_icd_prio43_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio43_val 0x00000000U

/* all used bits of 'NX4000_icd_prio43': */
#define MSK_USED_BITS_NX4000_icd_prio43 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio44 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio44            0x000004B0U
#define Adr_NX4000_ic_distributor_icd_prio44 0xFAF014B0U
#define Adr_NX4000_icd_prio44                0xFAF014B0U
#define DFLT_VAL_NX4000_icd_prio44           0x00000000U

#define MSK_NX4000_icd_prio44_val         0xffffffffU
#define SRT_NX4000_icd_prio44_val         0
#define DFLT_VAL_NX4000_icd_prio44_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio44_val 0x00000000U

/* all used bits of 'NX4000_icd_prio44': */
#define MSK_USED_BITS_NX4000_icd_prio44 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio45 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio45            0x000004B4U
#define Adr_NX4000_ic_distributor_icd_prio45 0xFAF014B4U
#define Adr_NX4000_icd_prio45                0xFAF014B4U
#define DFLT_VAL_NX4000_icd_prio45           0x00000000U

#define MSK_NX4000_icd_prio45_val         0xffffffffU
#define SRT_NX4000_icd_prio45_val         0
#define DFLT_VAL_NX4000_icd_prio45_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio45_val 0x00000000U

/* all used bits of 'NX4000_icd_prio45': */
#define MSK_USED_BITS_NX4000_icd_prio45 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio46 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio46            0x000004B8U
#define Adr_NX4000_ic_distributor_icd_prio46 0xFAF014B8U
#define Adr_NX4000_icd_prio46                0xFAF014B8U
#define DFLT_VAL_NX4000_icd_prio46           0x00000000U

#define MSK_NX4000_icd_prio46_val         0xffffffffU
#define SRT_NX4000_icd_prio46_val         0
#define DFLT_VAL_NX4000_icd_prio46_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio46_val 0x00000000U

/* all used bits of 'NX4000_icd_prio46': */
#define MSK_USED_BITS_NX4000_icd_prio46 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio47 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio47            0x000004BCU
#define Adr_NX4000_ic_distributor_icd_prio47 0xFAF014BCU
#define Adr_NX4000_icd_prio47                0xFAF014BCU
#define DFLT_VAL_NX4000_icd_prio47           0x00000000U

#define MSK_NX4000_icd_prio47_val         0xffffffffU
#define SRT_NX4000_icd_prio47_val         0
#define DFLT_VAL_NX4000_icd_prio47_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio47_val 0x00000000U

/* all used bits of 'NX4000_icd_prio47': */
#define MSK_USED_BITS_NX4000_icd_prio47 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio48 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio48            0x000004C0U
#define Adr_NX4000_ic_distributor_icd_prio48 0xFAF014C0U
#define Adr_NX4000_icd_prio48                0xFAF014C0U
#define DFLT_VAL_NX4000_icd_prio48           0x00000000U

#define MSK_NX4000_icd_prio48_val         0xffffffffU
#define SRT_NX4000_icd_prio48_val         0
#define DFLT_VAL_NX4000_icd_prio48_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio48_val 0x00000000U

/* all used bits of 'NX4000_icd_prio48': */
#define MSK_USED_BITS_NX4000_icd_prio48 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio49 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio49            0x000004C4U
#define Adr_NX4000_ic_distributor_icd_prio49 0xFAF014C4U
#define Adr_NX4000_icd_prio49                0xFAF014C4U
#define DFLT_VAL_NX4000_icd_prio49           0x00000000U

#define MSK_NX4000_icd_prio49_val         0xffffffffU
#define SRT_NX4000_icd_prio49_val         0
#define DFLT_VAL_NX4000_icd_prio49_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio49_val 0x00000000U

/* all used bits of 'NX4000_icd_prio49': */
#define MSK_USED_BITS_NX4000_icd_prio49 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio50 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio50            0x000004C8U
#define Adr_NX4000_ic_distributor_icd_prio50 0xFAF014C8U
#define Adr_NX4000_icd_prio50                0xFAF014C8U
#define DFLT_VAL_NX4000_icd_prio50           0x00000000U

#define MSK_NX4000_icd_prio50_val         0xffffffffU
#define SRT_NX4000_icd_prio50_val         0
#define DFLT_VAL_NX4000_icd_prio50_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio50_val 0x00000000U

/* all used bits of 'NX4000_icd_prio50': */
#define MSK_USED_BITS_NX4000_icd_prio50 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio51 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio51            0x000004CCU
#define Adr_NX4000_ic_distributor_icd_prio51 0xFAF014CCU
#define Adr_NX4000_icd_prio51                0xFAF014CCU
#define DFLT_VAL_NX4000_icd_prio51           0x00000000U

#define MSK_NX4000_icd_prio51_val         0xffffffffU
#define SRT_NX4000_icd_prio51_val         0
#define DFLT_VAL_NX4000_icd_prio51_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio51_val 0x00000000U

/* all used bits of 'NX4000_icd_prio51': */
#define MSK_USED_BITS_NX4000_icd_prio51 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio52 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio52            0x000004D0U
#define Adr_NX4000_ic_distributor_icd_prio52 0xFAF014D0U
#define Adr_NX4000_icd_prio52                0xFAF014D0U
#define DFLT_VAL_NX4000_icd_prio52           0x00000000U

#define MSK_NX4000_icd_prio52_val         0xffffffffU
#define SRT_NX4000_icd_prio52_val         0
#define DFLT_VAL_NX4000_icd_prio52_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio52_val 0x00000000U

/* all used bits of 'NX4000_icd_prio52': */
#define MSK_USED_BITS_NX4000_icd_prio52 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio53 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio53            0x000004D4U
#define Adr_NX4000_ic_distributor_icd_prio53 0xFAF014D4U
#define Adr_NX4000_icd_prio53                0xFAF014D4U
#define DFLT_VAL_NX4000_icd_prio53           0x00000000U

#define MSK_NX4000_icd_prio53_val         0xffffffffU
#define SRT_NX4000_icd_prio53_val         0
#define DFLT_VAL_NX4000_icd_prio53_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio53_val 0x00000000U

/* all used bits of 'NX4000_icd_prio53': */
#define MSK_USED_BITS_NX4000_icd_prio53 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio54 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio54            0x000004D8U
#define Adr_NX4000_ic_distributor_icd_prio54 0xFAF014D8U
#define Adr_NX4000_icd_prio54                0xFAF014D8U
#define DFLT_VAL_NX4000_icd_prio54           0x00000000U

#define MSK_NX4000_icd_prio54_val         0xffffffffU
#define SRT_NX4000_icd_prio54_val         0
#define DFLT_VAL_NX4000_icd_prio54_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio54_val 0x00000000U

/* all used bits of 'NX4000_icd_prio54': */
#define MSK_USED_BITS_NX4000_icd_prio54 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio55 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio55            0x000004DCU
#define Adr_NX4000_ic_distributor_icd_prio55 0xFAF014DCU
#define Adr_NX4000_icd_prio55                0xFAF014DCU
#define DFLT_VAL_NX4000_icd_prio55           0x00000000U

#define MSK_NX4000_icd_prio55_val         0xffffffffU
#define SRT_NX4000_icd_prio55_val         0
#define DFLT_VAL_NX4000_icd_prio55_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio55_val 0x00000000U

/* all used bits of 'NX4000_icd_prio55': */
#define MSK_USED_BITS_NX4000_icd_prio55 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio56 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio56            0x000004E0U
#define Adr_NX4000_ic_distributor_icd_prio56 0xFAF014E0U
#define Adr_NX4000_icd_prio56                0xFAF014E0U
#define DFLT_VAL_NX4000_icd_prio56           0x00000000U

#define MSK_NX4000_icd_prio56_val         0xffffffffU
#define SRT_NX4000_icd_prio56_val         0
#define DFLT_VAL_NX4000_icd_prio56_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio56_val 0x00000000U

/* all used bits of 'NX4000_icd_prio56': */
#define MSK_USED_BITS_NX4000_icd_prio56 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio57 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio57            0x000004E4U
#define Adr_NX4000_ic_distributor_icd_prio57 0xFAF014E4U
#define Adr_NX4000_icd_prio57                0xFAF014E4U
#define DFLT_VAL_NX4000_icd_prio57           0x00000000U

#define MSK_NX4000_icd_prio57_val         0xffffffffU
#define SRT_NX4000_icd_prio57_val         0
#define DFLT_VAL_NX4000_icd_prio57_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio57_val 0x00000000U

/* all used bits of 'NX4000_icd_prio57': */
#define MSK_USED_BITS_NX4000_icd_prio57 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio58 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio58            0x000004E8U
#define Adr_NX4000_ic_distributor_icd_prio58 0xFAF014E8U
#define Adr_NX4000_icd_prio58                0xFAF014E8U
#define DFLT_VAL_NX4000_icd_prio58           0x00000000U

#define MSK_NX4000_icd_prio58_val         0xffffffffU
#define SRT_NX4000_icd_prio58_val         0
#define DFLT_VAL_NX4000_icd_prio58_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio58_val 0x00000000U

/* all used bits of 'NX4000_icd_prio58': */
#define MSK_USED_BITS_NX4000_icd_prio58 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio59 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio59            0x000004ECU
#define Adr_NX4000_ic_distributor_icd_prio59 0xFAF014ECU
#define Adr_NX4000_icd_prio59                0xFAF014ECU
#define DFLT_VAL_NX4000_icd_prio59           0x00000000U

#define MSK_NX4000_icd_prio59_val         0xffffffffU
#define SRT_NX4000_icd_prio59_val         0
#define DFLT_VAL_NX4000_icd_prio59_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio59_val 0x00000000U

/* all used bits of 'NX4000_icd_prio59': */
#define MSK_USED_BITS_NX4000_icd_prio59 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio60 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio60            0x000004F0U
#define Adr_NX4000_ic_distributor_icd_prio60 0xFAF014F0U
#define Adr_NX4000_icd_prio60                0xFAF014F0U
#define DFLT_VAL_NX4000_icd_prio60           0x00000000U

#define MSK_NX4000_icd_prio60_val         0xffffffffU
#define SRT_NX4000_icd_prio60_val         0
#define DFLT_VAL_NX4000_icd_prio60_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio60_val 0x00000000U

/* all used bits of 'NX4000_icd_prio60': */
#define MSK_USED_BITS_NX4000_icd_prio60 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio61 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio61            0x000004F4U
#define Adr_NX4000_ic_distributor_icd_prio61 0xFAF014F4U
#define Adr_NX4000_icd_prio61                0xFAF014F4U
#define DFLT_VAL_NX4000_icd_prio61           0x00000000U

#define MSK_NX4000_icd_prio61_val         0xffffffffU
#define SRT_NX4000_icd_prio61_val         0
#define DFLT_VAL_NX4000_icd_prio61_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio61_val 0x00000000U

/* all used bits of 'NX4000_icd_prio61': */
#define MSK_USED_BITS_NX4000_icd_prio61 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio62 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio62            0x000004F8U
#define Adr_NX4000_ic_distributor_icd_prio62 0xFAF014F8U
#define Adr_NX4000_icd_prio62                0xFAF014F8U
#define DFLT_VAL_NX4000_icd_prio62           0x00000000U

#define MSK_NX4000_icd_prio62_val         0xffffffffU
#define SRT_NX4000_icd_prio62_val         0
#define DFLT_VAL_NX4000_icd_prio62_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio62_val 0x00000000U

/* all used bits of 'NX4000_icd_prio62': */
#define MSK_USED_BITS_NX4000_icd_prio62 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_prio63 */
/* => See register icd_prio0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_prio63            0x000004FCU
#define Adr_NX4000_ic_distributor_icd_prio63 0xFAF014FCU
#define Adr_NX4000_icd_prio63                0xFAF014FCU
#define DFLT_VAL_NX4000_icd_prio63           0x00000000U

#define MSK_NX4000_icd_prio63_val         0xffffffffU
#define SRT_NX4000_icd_prio63_val         0
#define DFLT_VAL_NX4000_icd_prio63_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_prio63_val 0x00000000U

/* all used bits of 'NX4000_icd_prio63': */
#define MSK_USED_BITS_NX4000_icd_prio63 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg0 */
/* => Interrupt Processor Targets Registers (ICDIPTRn) */
/*    The ICDIPTRs provide an 8-bit CPU targets field for each interrupt supported by */
/*    the GIC. This field stores the list of processors that the interrupt is sent to if it is asserted. */
/*    These registers are byte accessible. */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg0            0x00000800U
#define Adr_NX4000_ic_distributor_icd_processor_trg0 0xFAF01800U
#define Adr_NX4000_icd_processor_trg0                0xFAF01800U
#define DFLT_VAL_NX4000_icd_processor_trg0           0x00000000U

#define MSK_NX4000_icd_processor_trg0_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg0_val         0
#define DFLT_VAL_NX4000_icd_processor_trg0_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg0_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg0': */
#define MSK_USED_BITS_NX4000_icd_processor_trg0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg1 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg1            0x00000804U
#define Adr_NX4000_ic_distributor_icd_processor_trg1 0xFAF01804U
#define Adr_NX4000_icd_processor_trg1                0xFAF01804U
#define DFLT_VAL_NX4000_icd_processor_trg1           0x00000000U

#define MSK_NX4000_icd_processor_trg1_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg1_val         0
#define DFLT_VAL_NX4000_icd_processor_trg1_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg1_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg1': */
#define MSK_USED_BITS_NX4000_icd_processor_trg1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg2 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg2            0x00000808U
#define Adr_NX4000_ic_distributor_icd_processor_trg2 0xFAF01808U
#define Adr_NX4000_icd_processor_trg2                0xFAF01808U
#define DFLT_VAL_NX4000_icd_processor_trg2           0x00000000U

#define MSK_NX4000_icd_processor_trg2_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg2_val         0
#define DFLT_VAL_NX4000_icd_processor_trg2_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg2_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg2': */
#define MSK_USED_BITS_NX4000_icd_processor_trg2 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg3 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg3            0x0000080CU
#define Adr_NX4000_ic_distributor_icd_processor_trg3 0xFAF0180CU
#define Adr_NX4000_icd_processor_trg3                0xFAF0180CU
#define DFLT_VAL_NX4000_icd_processor_trg3           0x00000000U

#define MSK_NX4000_icd_processor_trg3_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg3_val         0
#define DFLT_VAL_NX4000_icd_processor_trg3_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg3_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg3': */
#define MSK_USED_BITS_NX4000_icd_processor_trg3 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg4 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg4            0x00000810U
#define Adr_NX4000_ic_distributor_icd_processor_trg4 0xFAF01810U
#define Adr_NX4000_icd_processor_trg4                0xFAF01810U
#define DFLT_VAL_NX4000_icd_processor_trg4           0x00000000U

#define MSK_NX4000_icd_processor_trg4_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg4_val         0
#define DFLT_VAL_NX4000_icd_processor_trg4_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg4_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg4': */
#define MSK_USED_BITS_NX4000_icd_processor_trg4 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg5 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg5            0x00000814U
#define Adr_NX4000_ic_distributor_icd_processor_trg5 0xFAF01814U
#define Adr_NX4000_icd_processor_trg5                0xFAF01814U
#define DFLT_VAL_NX4000_icd_processor_trg5           0x00000000U

#define MSK_NX4000_icd_processor_trg5_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg5_val         0
#define DFLT_VAL_NX4000_icd_processor_trg5_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg5_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg5': */
#define MSK_USED_BITS_NX4000_icd_processor_trg5 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg6 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg6            0x00000818U
#define Adr_NX4000_ic_distributor_icd_processor_trg6 0xFAF01818U
#define Adr_NX4000_icd_processor_trg6                0xFAF01818U
#define DFLT_VAL_NX4000_icd_processor_trg6           0x00000000U

#define MSK_NX4000_icd_processor_trg6_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg6_val         0
#define DFLT_VAL_NX4000_icd_processor_trg6_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg6_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg6': */
#define MSK_USED_BITS_NX4000_icd_processor_trg6 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg7 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg7            0x0000081CU
#define Adr_NX4000_ic_distributor_icd_processor_trg7 0xFAF0181CU
#define Adr_NX4000_icd_processor_trg7                0xFAF0181CU
#define DFLT_VAL_NX4000_icd_processor_trg7           0x00000000U

#define MSK_NX4000_icd_processor_trg7_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg7_val         0
#define DFLT_VAL_NX4000_icd_processor_trg7_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg7_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg7': */
#define MSK_USED_BITS_NX4000_icd_processor_trg7 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg8 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg8            0x00000820U
#define Adr_NX4000_ic_distributor_icd_processor_trg8 0xFAF01820U
#define Adr_NX4000_icd_processor_trg8                0xFAF01820U
#define DFLT_VAL_NX4000_icd_processor_trg8           0x00000000U

#define MSK_NX4000_icd_processor_trg8_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg8_val         0
#define DFLT_VAL_NX4000_icd_processor_trg8_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg8_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg8': */
#define MSK_USED_BITS_NX4000_icd_processor_trg8 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg9 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg9            0x00000824U
#define Adr_NX4000_ic_distributor_icd_processor_trg9 0xFAF01824U
#define Adr_NX4000_icd_processor_trg9                0xFAF01824U
#define DFLT_VAL_NX4000_icd_processor_trg9           0x00000000U

#define MSK_NX4000_icd_processor_trg9_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg9_val         0
#define DFLT_VAL_NX4000_icd_processor_trg9_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg9_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg9': */
#define MSK_USED_BITS_NX4000_icd_processor_trg9 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg10 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg10            0x00000828U
#define Adr_NX4000_ic_distributor_icd_processor_trg10 0xFAF01828U
#define Adr_NX4000_icd_processor_trg10                0xFAF01828U
#define DFLT_VAL_NX4000_icd_processor_trg10           0x00000000U

#define MSK_NX4000_icd_processor_trg10_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg10_val         0
#define DFLT_VAL_NX4000_icd_processor_trg10_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg10_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg10': */
#define MSK_USED_BITS_NX4000_icd_processor_trg10 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg11 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg11            0x0000082CU
#define Adr_NX4000_ic_distributor_icd_processor_trg11 0xFAF0182CU
#define Adr_NX4000_icd_processor_trg11                0xFAF0182CU
#define DFLT_VAL_NX4000_icd_processor_trg11           0x00000000U

#define MSK_NX4000_icd_processor_trg11_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg11_val         0
#define DFLT_VAL_NX4000_icd_processor_trg11_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg11_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg11': */
#define MSK_USED_BITS_NX4000_icd_processor_trg11 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg12 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg12            0x00000830U
#define Adr_NX4000_ic_distributor_icd_processor_trg12 0xFAF01830U
#define Adr_NX4000_icd_processor_trg12                0xFAF01830U
#define DFLT_VAL_NX4000_icd_processor_trg12           0x00000000U

#define MSK_NX4000_icd_processor_trg12_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg12_val         0
#define DFLT_VAL_NX4000_icd_processor_trg12_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg12_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg12': */
#define MSK_USED_BITS_NX4000_icd_processor_trg12 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg13 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg13            0x00000834U
#define Adr_NX4000_ic_distributor_icd_processor_trg13 0xFAF01834U
#define Adr_NX4000_icd_processor_trg13                0xFAF01834U
#define DFLT_VAL_NX4000_icd_processor_trg13           0x00000000U

#define MSK_NX4000_icd_processor_trg13_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg13_val         0
#define DFLT_VAL_NX4000_icd_processor_trg13_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg13_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg13': */
#define MSK_USED_BITS_NX4000_icd_processor_trg13 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg14 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg14            0x00000838U
#define Adr_NX4000_ic_distributor_icd_processor_trg14 0xFAF01838U
#define Adr_NX4000_icd_processor_trg14                0xFAF01838U
#define DFLT_VAL_NX4000_icd_processor_trg14           0x00000000U

#define MSK_NX4000_icd_processor_trg14_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg14_val         0
#define DFLT_VAL_NX4000_icd_processor_trg14_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg14_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg14': */
#define MSK_USED_BITS_NX4000_icd_processor_trg14 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg15 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg15            0x0000083CU
#define Adr_NX4000_ic_distributor_icd_processor_trg15 0xFAF0183CU
#define Adr_NX4000_icd_processor_trg15                0xFAF0183CU
#define DFLT_VAL_NX4000_icd_processor_trg15           0x00000000U

#define MSK_NX4000_icd_processor_trg15_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg15_val         0
#define DFLT_VAL_NX4000_icd_processor_trg15_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg15_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg15': */
#define MSK_USED_BITS_NX4000_icd_processor_trg15 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg16 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg16            0x00000840U
#define Adr_NX4000_ic_distributor_icd_processor_trg16 0xFAF01840U
#define Adr_NX4000_icd_processor_trg16                0xFAF01840U
#define DFLT_VAL_NX4000_icd_processor_trg16           0x00000000U

#define MSK_NX4000_icd_processor_trg16_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg16_val         0
#define DFLT_VAL_NX4000_icd_processor_trg16_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg16_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg16': */
#define MSK_USED_BITS_NX4000_icd_processor_trg16 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg17 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg17            0x00000844U
#define Adr_NX4000_ic_distributor_icd_processor_trg17 0xFAF01844U
#define Adr_NX4000_icd_processor_trg17                0xFAF01844U
#define DFLT_VAL_NX4000_icd_processor_trg17           0x00000000U

#define MSK_NX4000_icd_processor_trg17_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg17_val         0
#define DFLT_VAL_NX4000_icd_processor_trg17_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg17_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg17': */
#define MSK_USED_BITS_NX4000_icd_processor_trg17 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg18 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg18            0x00000848U
#define Adr_NX4000_ic_distributor_icd_processor_trg18 0xFAF01848U
#define Adr_NX4000_icd_processor_trg18                0xFAF01848U
#define DFLT_VAL_NX4000_icd_processor_trg18           0x00000000U

#define MSK_NX4000_icd_processor_trg18_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg18_val         0
#define DFLT_VAL_NX4000_icd_processor_trg18_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg18_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg18': */
#define MSK_USED_BITS_NX4000_icd_processor_trg18 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg19 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg19            0x0000084CU
#define Adr_NX4000_ic_distributor_icd_processor_trg19 0xFAF0184CU
#define Adr_NX4000_icd_processor_trg19                0xFAF0184CU
#define DFLT_VAL_NX4000_icd_processor_trg19           0x00000000U

#define MSK_NX4000_icd_processor_trg19_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg19_val         0
#define DFLT_VAL_NX4000_icd_processor_trg19_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg19_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg19': */
#define MSK_USED_BITS_NX4000_icd_processor_trg19 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg20 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg20            0x00000850U
#define Adr_NX4000_ic_distributor_icd_processor_trg20 0xFAF01850U
#define Adr_NX4000_icd_processor_trg20                0xFAF01850U
#define DFLT_VAL_NX4000_icd_processor_trg20           0x00000000U

#define MSK_NX4000_icd_processor_trg20_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg20_val         0
#define DFLT_VAL_NX4000_icd_processor_trg20_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg20_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg20': */
#define MSK_USED_BITS_NX4000_icd_processor_trg20 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg21 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg21            0x00000854U
#define Adr_NX4000_ic_distributor_icd_processor_trg21 0xFAF01854U
#define Adr_NX4000_icd_processor_trg21                0xFAF01854U
#define DFLT_VAL_NX4000_icd_processor_trg21           0x00000000U

#define MSK_NX4000_icd_processor_trg21_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg21_val         0
#define DFLT_VAL_NX4000_icd_processor_trg21_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg21_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg21': */
#define MSK_USED_BITS_NX4000_icd_processor_trg21 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg22 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg22            0x00000858U
#define Adr_NX4000_ic_distributor_icd_processor_trg22 0xFAF01858U
#define Adr_NX4000_icd_processor_trg22                0xFAF01858U
#define DFLT_VAL_NX4000_icd_processor_trg22           0x00000000U

#define MSK_NX4000_icd_processor_trg22_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg22_val         0
#define DFLT_VAL_NX4000_icd_processor_trg22_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg22_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg22': */
#define MSK_USED_BITS_NX4000_icd_processor_trg22 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg23 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg23            0x0000085CU
#define Adr_NX4000_ic_distributor_icd_processor_trg23 0xFAF0185CU
#define Adr_NX4000_icd_processor_trg23                0xFAF0185CU
#define DFLT_VAL_NX4000_icd_processor_trg23           0x00000000U

#define MSK_NX4000_icd_processor_trg23_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg23_val         0
#define DFLT_VAL_NX4000_icd_processor_trg23_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg23_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg23': */
#define MSK_USED_BITS_NX4000_icd_processor_trg23 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg24 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg24            0x00000860U
#define Adr_NX4000_ic_distributor_icd_processor_trg24 0xFAF01860U
#define Adr_NX4000_icd_processor_trg24                0xFAF01860U
#define DFLT_VAL_NX4000_icd_processor_trg24           0x00000000U

#define MSK_NX4000_icd_processor_trg24_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg24_val         0
#define DFLT_VAL_NX4000_icd_processor_trg24_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg24_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg24': */
#define MSK_USED_BITS_NX4000_icd_processor_trg24 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg25 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg25            0x00000864U
#define Adr_NX4000_ic_distributor_icd_processor_trg25 0xFAF01864U
#define Adr_NX4000_icd_processor_trg25                0xFAF01864U
#define DFLT_VAL_NX4000_icd_processor_trg25           0x00000000U

#define MSK_NX4000_icd_processor_trg25_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg25_val         0
#define DFLT_VAL_NX4000_icd_processor_trg25_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg25_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg25': */
#define MSK_USED_BITS_NX4000_icd_processor_trg25 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg26 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg26            0x00000868U
#define Adr_NX4000_ic_distributor_icd_processor_trg26 0xFAF01868U
#define Adr_NX4000_icd_processor_trg26                0xFAF01868U
#define DFLT_VAL_NX4000_icd_processor_trg26           0x00000000U

#define MSK_NX4000_icd_processor_trg26_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg26_val         0
#define DFLT_VAL_NX4000_icd_processor_trg26_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg26_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg26': */
#define MSK_USED_BITS_NX4000_icd_processor_trg26 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg27 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg27            0x0000086CU
#define Adr_NX4000_ic_distributor_icd_processor_trg27 0xFAF0186CU
#define Adr_NX4000_icd_processor_trg27                0xFAF0186CU
#define DFLT_VAL_NX4000_icd_processor_trg27           0x00000000U

#define MSK_NX4000_icd_processor_trg27_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg27_val         0
#define DFLT_VAL_NX4000_icd_processor_trg27_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg27_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg27': */
#define MSK_USED_BITS_NX4000_icd_processor_trg27 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg28 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg28            0x00000870U
#define Adr_NX4000_ic_distributor_icd_processor_trg28 0xFAF01870U
#define Adr_NX4000_icd_processor_trg28                0xFAF01870U
#define DFLT_VAL_NX4000_icd_processor_trg28           0x00000000U

#define MSK_NX4000_icd_processor_trg28_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg28_val         0
#define DFLT_VAL_NX4000_icd_processor_trg28_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg28_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg28': */
#define MSK_USED_BITS_NX4000_icd_processor_trg28 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg29 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg29            0x00000874U
#define Adr_NX4000_ic_distributor_icd_processor_trg29 0xFAF01874U
#define Adr_NX4000_icd_processor_trg29                0xFAF01874U
#define DFLT_VAL_NX4000_icd_processor_trg29           0x00000000U

#define MSK_NX4000_icd_processor_trg29_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg29_val         0
#define DFLT_VAL_NX4000_icd_processor_trg29_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg29_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg29': */
#define MSK_USED_BITS_NX4000_icd_processor_trg29 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg30 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg30            0x00000878U
#define Adr_NX4000_ic_distributor_icd_processor_trg30 0xFAF01878U
#define Adr_NX4000_icd_processor_trg30                0xFAF01878U
#define DFLT_VAL_NX4000_icd_processor_trg30           0x00000000U

#define MSK_NX4000_icd_processor_trg30_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg30_val         0
#define DFLT_VAL_NX4000_icd_processor_trg30_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg30_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg30': */
#define MSK_USED_BITS_NX4000_icd_processor_trg30 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg31 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg31            0x0000087CU
#define Adr_NX4000_ic_distributor_icd_processor_trg31 0xFAF0187CU
#define Adr_NX4000_icd_processor_trg31                0xFAF0187CU
#define DFLT_VAL_NX4000_icd_processor_trg31           0x00000000U

#define MSK_NX4000_icd_processor_trg31_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg31_val         0
#define DFLT_VAL_NX4000_icd_processor_trg31_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg31_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg31': */
#define MSK_USED_BITS_NX4000_icd_processor_trg31 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg32 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg32            0x00000880U
#define Adr_NX4000_ic_distributor_icd_processor_trg32 0xFAF01880U
#define Adr_NX4000_icd_processor_trg32                0xFAF01880U
#define DFLT_VAL_NX4000_icd_processor_trg32           0x00000000U

#define MSK_NX4000_icd_processor_trg32_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg32_val         0
#define DFLT_VAL_NX4000_icd_processor_trg32_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg32_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg32': */
#define MSK_USED_BITS_NX4000_icd_processor_trg32 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg33 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg33            0x00000884U
#define Adr_NX4000_ic_distributor_icd_processor_trg33 0xFAF01884U
#define Adr_NX4000_icd_processor_trg33                0xFAF01884U
#define DFLT_VAL_NX4000_icd_processor_trg33           0x00000000U

#define MSK_NX4000_icd_processor_trg33_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg33_val         0
#define DFLT_VAL_NX4000_icd_processor_trg33_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg33_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg33': */
#define MSK_USED_BITS_NX4000_icd_processor_trg33 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg34 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg34            0x00000888U
#define Adr_NX4000_ic_distributor_icd_processor_trg34 0xFAF01888U
#define Adr_NX4000_icd_processor_trg34                0xFAF01888U
#define DFLT_VAL_NX4000_icd_processor_trg34           0x00000000U

#define MSK_NX4000_icd_processor_trg34_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg34_val         0
#define DFLT_VAL_NX4000_icd_processor_trg34_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg34_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg34': */
#define MSK_USED_BITS_NX4000_icd_processor_trg34 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg35 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg35            0x0000088CU
#define Adr_NX4000_ic_distributor_icd_processor_trg35 0xFAF0188CU
#define Adr_NX4000_icd_processor_trg35                0xFAF0188CU
#define DFLT_VAL_NX4000_icd_processor_trg35           0x00000000U

#define MSK_NX4000_icd_processor_trg35_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg35_val         0
#define DFLT_VAL_NX4000_icd_processor_trg35_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg35_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg35': */
#define MSK_USED_BITS_NX4000_icd_processor_trg35 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg36 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg36            0x00000890U
#define Adr_NX4000_ic_distributor_icd_processor_trg36 0xFAF01890U
#define Adr_NX4000_icd_processor_trg36                0xFAF01890U
#define DFLT_VAL_NX4000_icd_processor_trg36           0x00000000U

#define MSK_NX4000_icd_processor_trg36_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg36_val         0
#define DFLT_VAL_NX4000_icd_processor_trg36_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg36_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg36': */
#define MSK_USED_BITS_NX4000_icd_processor_trg36 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg37 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg37            0x00000894U
#define Adr_NX4000_ic_distributor_icd_processor_trg37 0xFAF01894U
#define Adr_NX4000_icd_processor_trg37                0xFAF01894U
#define DFLT_VAL_NX4000_icd_processor_trg37           0x00000000U

#define MSK_NX4000_icd_processor_trg37_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg37_val         0
#define DFLT_VAL_NX4000_icd_processor_trg37_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg37_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg37': */
#define MSK_USED_BITS_NX4000_icd_processor_trg37 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg38 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg38            0x00000898U
#define Adr_NX4000_ic_distributor_icd_processor_trg38 0xFAF01898U
#define Adr_NX4000_icd_processor_trg38                0xFAF01898U
#define DFLT_VAL_NX4000_icd_processor_trg38           0x00000000U

#define MSK_NX4000_icd_processor_trg38_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg38_val         0
#define DFLT_VAL_NX4000_icd_processor_trg38_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg38_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg38': */
#define MSK_USED_BITS_NX4000_icd_processor_trg38 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg39 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg39            0x0000089CU
#define Adr_NX4000_ic_distributor_icd_processor_trg39 0xFAF0189CU
#define Adr_NX4000_icd_processor_trg39                0xFAF0189CU
#define DFLT_VAL_NX4000_icd_processor_trg39           0x00000000U

#define MSK_NX4000_icd_processor_trg39_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg39_val         0
#define DFLT_VAL_NX4000_icd_processor_trg39_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg39_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg39': */
#define MSK_USED_BITS_NX4000_icd_processor_trg39 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg40 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg40            0x000008A0U
#define Adr_NX4000_ic_distributor_icd_processor_trg40 0xFAF018A0U
#define Adr_NX4000_icd_processor_trg40                0xFAF018A0U
#define DFLT_VAL_NX4000_icd_processor_trg40           0x00000000U

#define MSK_NX4000_icd_processor_trg40_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg40_val         0
#define DFLT_VAL_NX4000_icd_processor_trg40_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg40_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg40': */
#define MSK_USED_BITS_NX4000_icd_processor_trg40 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg41 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg41            0x000008A4U
#define Adr_NX4000_ic_distributor_icd_processor_trg41 0xFAF018A4U
#define Adr_NX4000_icd_processor_trg41                0xFAF018A4U
#define DFLT_VAL_NX4000_icd_processor_trg41           0x00000000U

#define MSK_NX4000_icd_processor_trg41_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg41_val         0
#define DFLT_VAL_NX4000_icd_processor_trg41_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg41_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg41': */
#define MSK_USED_BITS_NX4000_icd_processor_trg41 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg42 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg42            0x000008A8U
#define Adr_NX4000_ic_distributor_icd_processor_trg42 0xFAF018A8U
#define Adr_NX4000_icd_processor_trg42                0xFAF018A8U
#define DFLT_VAL_NX4000_icd_processor_trg42           0x00000000U

#define MSK_NX4000_icd_processor_trg42_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg42_val         0
#define DFLT_VAL_NX4000_icd_processor_trg42_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg42_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg42': */
#define MSK_USED_BITS_NX4000_icd_processor_trg42 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg43 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg43            0x000008ACU
#define Adr_NX4000_ic_distributor_icd_processor_trg43 0xFAF018ACU
#define Adr_NX4000_icd_processor_trg43                0xFAF018ACU
#define DFLT_VAL_NX4000_icd_processor_trg43           0x00000000U

#define MSK_NX4000_icd_processor_trg43_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg43_val         0
#define DFLT_VAL_NX4000_icd_processor_trg43_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg43_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg43': */
#define MSK_USED_BITS_NX4000_icd_processor_trg43 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg44 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg44            0x000008B0U
#define Adr_NX4000_ic_distributor_icd_processor_trg44 0xFAF018B0U
#define Adr_NX4000_icd_processor_trg44                0xFAF018B0U
#define DFLT_VAL_NX4000_icd_processor_trg44           0x00000000U

#define MSK_NX4000_icd_processor_trg44_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg44_val         0
#define DFLT_VAL_NX4000_icd_processor_trg44_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg44_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg44': */
#define MSK_USED_BITS_NX4000_icd_processor_trg44 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg45 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg45            0x000008B4U
#define Adr_NX4000_ic_distributor_icd_processor_trg45 0xFAF018B4U
#define Adr_NX4000_icd_processor_trg45                0xFAF018B4U
#define DFLT_VAL_NX4000_icd_processor_trg45           0x00000000U

#define MSK_NX4000_icd_processor_trg45_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg45_val         0
#define DFLT_VAL_NX4000_icd_processor_trg45_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg45_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg45': */
#define MSK_USED_BITS_NX4000_icd_processor_trg45 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg46 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg46            0x000008B8U
#define Adr_NX4000_ic_distributor_icd_processor_trg46 0xFAF018B8U
#define Adr_NX4000_icd_processor_trg46                0xFAF018B8U
#define DFLT_VAL_NX4000_icd_processor_trg46           0x00000000U

#define MSK_NX4000_icd_processor_trg46_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg46_val         0
#define DFLT_VAL_NX4000_icd_processor_trg46_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg46_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg46': */
#define MSK_USED_BITS_NX4000_icd_processor_trg46 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg47 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg47            0x000008BCU
#define Adr_NX4000_ic_distributor_icd_processor_trg47 0xFAF018BCU
#define Adr_NX4000_icd_processor_trg47                0xFAF018BCU
#define DFLT_VAL_NX4000_icd_processor_trg47           0x00000000U

#define MSK_NX4000_icd_processor_trg47_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg47_val         0
#define DFLT_VAL_NX4000_icd_processor_trg47_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg47_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg47': */
#define MSK_USED_BITS_NX4000_icd_processor_trg47 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg48 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg48            0x000008C0U
#define Adr_NX4000_ic_distributor_icd_processor_trg48 0xFAF018C0U
#define Adr_NX4000_icd_processor_trg48                0xFAF018C0U
#define DFLT_VAL_NX4000_icd_processor_trg48           0x00000000U

#define MSK_NX4000_icd_processor_trg48_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg48_val         0
#define DFLT_VAL_NX4000_icd_processor_trg48_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg48_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg48': */
#define MSK_USED_BITS_NX4000_icd_processor_trg48 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg49 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg49            0x000008C4U
#define Adr_NX4000_ic_distributor_icd_processor_trg49 0xFAF018C4U
#define Adr_NX4000_icd_processor_trg49                0xFAF018C4U
#define DFLT_VAL_NX4000_icd_processor_trg49           0x00000000U

#define MSK_NX4000_icd_processor_trg49_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg49_val         0
#define DFLT_VAL_NX4000_icd_processor_trg49_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg49_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg49': */
#define MSK_USED_BITS_NX4000_icd_processor_trg49 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg50 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg50            0x000008C8U
#define Adr_NX4000_ic_distributor_icd_processor_trg50 0xFAF018C8U
#define Adr_NX4000_icd_processor_trg50                0xFAF018C8U
#define DFLT_VAL_NX4000_icd_processor_trg50           0x00000000U

#define MSK_NX4000_icd_processor_trg50_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg50_val         0
#define DFLT_VAL_NX4000_icd_processor_trg50_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg50_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg50': */
#define MSK_USED_BITS_NX4000_icd_processor_trg50 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg51 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg51            0x000008CCU
#define Adr_NX4000_ic_distributor_icd_processor_trg51 0xFAF018CCU
#define Adr_NX4000_icd_processor_trg51                0xFAF018CCU
#define DFLT_VAL_NX4000_icd_processor_trg51           0x00000000U

#define MSK_NX4000_icd_processor_trg51_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg51_val         0
#define DFLT_VAL_NX4000_icd_processor_trg51_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg51_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg51': */
#define MSK_USED_BITS_NX4000_icd_processor_trg51 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg52 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg52            0x000008D0U
#define Adr_NX4000_ic_distributor_icd_processor_trg52 0xFAF018D0U
#define Adr_NX4000_icd_processor_trg52                0xFAF018D0U
#define DFLT_VAL_NX4000_icd_processor_trg52           0x00000000U

#define MSK_NX4000_icd_processor_trg52_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg52_val         0
#define DFLT_VAL_NX4000_icd_processor_trg52_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg52_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg52': */
#define MSK_USED_BITS_NX4000_icd_processor_trg52 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg53 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg53            0x000008D4U
#define Adr_NX4000_ic_distributor_icd_processor_trg53 0xFAF018D4U
#define Adr_NX4000_icd_processor_trg53                0xFAF018D4U
#define DFLT_VAL_NX4000_icd_processor_trg53           0x00000000U

#define MSK_NX4000_icd_processor_trg53_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg53_val         0
#define DFLT_VAL_NX4000_icd_processor_trg53_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg53_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg53': */
#define MSK_USED_BITS_NX4000_icd_processor_trg53 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg54 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg54            0x000008D8U
#define Adr_NX4000_ic_distributor_icd_processor_trg54 0xFAF018D8U
#define Adr_NX4000_icd_processor_trg54                0xFAF018D8U
#define DFLT_VAL_NX4000_icd_processor_trg54           0x00000000U

#define MSK_NX4000_icd_processor_trg54_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg54_val         0
#define DFLT_VAL_NX4000_icd_processor_trg54_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg54_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg54': */
#define MSK_USED_BITS_NX4000_icd_processor_trg54 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg55 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg55            0x000008DCU
#define Adr_NX4000_ic_distributor_icd_processor_trg55 0xFAF018DCU
#define Adr_NX4000_icd_processor_trg55                0xFAF018DCU
#define DFLT_VAL_NX4000_icd_processor_trg55           0x00000000U

#define MSK_NX4000_icd_processor_trg55_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg55_val         0
#define DFLT_VAL_NX4000_icd_processor_trg55_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg55_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg55': */
#define MSK_USED_BITS_NX4000_icd_processor_trg55 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg56 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg56            0x000008E0U
#define Adr_NX4000_ic_distributor_icd_processor_trg56 0xFAF018E0U
#define Adr_NX4000_icd_processor_trg56                0xFAF018E0U
#define DFLT_VAL_NX4000_icd_processor_trg56           0x00000000U

#define MSK_NX4000_icd_processor_trg56_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg56_val         0
#define DFLT_VAL_NX4000_icd_processor_trg56_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg56_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg56': */
#define MSK_USED_BITS_NX4000_icd_processor_trg56 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg57 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg57            0x000008E4U
#define Adr_NX4000_ic_distributor_icd_processor_trg57 0xFAF018E4U
#define Adr_NX4000_icd_processor_trg57                0xFAF018E4U
#define DFLT_VAL_NX4000_icd_processor_trg57           0x00000000U

#define MSK_NX4000_icd_processor_trg57_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg57_val         0
#define DFLT_VAL_NX4000_icd_processor_trg57_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg57_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg57': */
#define MSK_USED_BITS_NX4000_icd_processor_trg57 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg58 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg58            0x000008E8U
#define Adr_NX4000_ic_distributor_icd_processor_trg58 0xFAF018E8U
#define Adr_NX4000_icd_processor_trg58                0xFAF018E8U
#define DFLT_VAL_NX4000_icd_processor_trg58           0x00000000U

#define MSK_NX4000_icd_processor_trg58_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg58_val         0
#define DFLT_VAL_NX4000_icd_processor_trg58_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg58_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg58': */
#define MSK_USED_BITS_NX4000_icd_processor_trg58 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg59 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg59            0x000008ECU
#define Adr_NX4000_ic_distributor_icd_processor_trg59 0xFAF018ECU
#define Adr_NX4000_icd_processor_trg59                0xFAF018ECU
#define DFLT_VAL_NX4000_icd_processor_trg59           0x00000000U

#define MSK_NX4000_icd_processor_trg59_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg59_val         0
#define DFLT_VAL_NX4000_icd_processor_trg59_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg59_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg59': */
#define MSK_USED_BITS_NX4000_icd_processor_trg59 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg60 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg60            0x000008F0U
#define Adr_NX4000_ic_distributor_icd_processor_trg60 0xFAF018F0U
#define Adr_NX4000_icd_processor_trg60                0xFAF018F0U
#define DFLT_VAL_NX4000_icd_processor_trg60           0x00000000U

#define MSK_NX4000_icd_processor_trg60_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg60_val         0
#define DFLT_VAL_NX4000_icd_processor_trg60_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg60_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg60': */
#define MSK_USED_BITS_NX4000_icd_processor_trg60 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg61 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg61            0x000008F4U
#define Adr_NX4000_ic_distributor_icd_processor_trg61 0xFAF018F4U
#define Adr_NX4000_icd_processor_trg61                0xFAF018F4U
#define DFLT_VAL_NX4000_icd_processor_trg61           0x00000000U

#define MSK_NX4000_icd_processor_trg61_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg61_val         0
#define DFLT_VAL_NX4000_icd_processor_trg61_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg61_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg61': */
#define MSK_USED_BITS_NX4000_icd_processor_trg61 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg62 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg62            0x000008F8U
#define Adr_NX4000_ic_distributor_icd_processor_trg62 0xFAF018F8U
#define Adr_NX4000_icd_processor_trg62                0xFAF018F8U
#define DFLT_VAL_NX4000_icd_processor_trg62           0x00000000U

#define MSK_NX4000_icd_processor_trg62_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg62_val         0
#define DFLT_VAL_NX4000_icd_processor_trg62_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg62_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg62': */
#define MSK_USED_BITS_NX4000_icd_processor_trg62 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_processor_trg63 */
/* => See register icd_processor_trg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_processor_trg63            0x000008FCU
#define Adr_NX4000_ic_distributor_icd_processor_trg63 0xFAF018FCU
#define Adr_NX4000_icd_processor_trg63                0xFAF018FCU
#define DFLT_VAL_NX4000_icd_processor_trg63           0x00000000U

#define MSK_NX4000_icd_processor_trg63_val         0xffffffffU
#define SRT_NX4000_icd_processor_trg63_val         0
#define DFLT_VAL_NX4000_icd_processor_trg63_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_processor_trg63_val 0x00000000U

/* all used bits of 'NX4000_icd_processor_trg63': */
#define MSK_USED_BITS_NX4000_icd_processor_trg63 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_cfg0 */
/* => Interrupt Configuration Registers (ICDICFRn) */
/*    The ICDICFRs provide a 2-bit Int_config field for each interrupt supported by the GIC */
/*    Bit 0 is reserved, for bit 1 the encoding is: */
/*      0: Corresponding interrupt is level-sensitive */
/*      1: Corresponding interrupt is edge-triggered */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_cfg0            0x00000C00U
#define Adr_NX4000_ic_distributor_icd_cfg0 0xFAF01C00U
#define Adr_NX4000_icd_cfg0                0xFAF01C00U
#define DFLT_VAL_NX4000_icd_cfg0           0x00000000U

#define MSK_NX4000_icd_cfg0_val         0xffffffffU
#define SRT_NX4000_icd_cfg0_val         0
#define DFLT_VAL_NX4000_icd_cfg0_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_cfg0_val 0x00000000U

/* all used bits of 'NX4000_icd_cfg0': */
#define MSK_USED_BITS_NX4000_icd_cfg0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_cfg1 */
/* => See register icd_cfg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_cfg1            0x00000C04U
#define Adr_NX4000_ic_distributor_icd_cfg1 0xFAF01C04U
#define Adr_NX4000_icd_cfg1                0xFAF01C04U
#define DFLT_VAL_NX4000_icd_cfg1           0x00000000U

#define MSK_NX4000_icd_cfg1_val         0xffffffffU
#define SRT_NX4000_icd_cfg1_val         0
#define DFLT_VAL_NX4000_icd_cfg1_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_cfg1_val 0x00000000U

/* all used bits of 'NX4000_icd_cfg1': */
#define MSK_USED_BITS_NX4000_icd_cfg1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_cfg2 */
/* => See register icd_cfg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_cfg2            0x00000C08U
#define Adr_NX4000_ic_distributor_icd_cfg2 0xFAF01C08U
#define Adr_NX4000_icd_cfg2                0xFAF01C08U
#define DFLT_VAL_NX4000_icd_cfg2           0x00000000U

#define MSK_NX4000_icd_cfg2_val         0xffffffffU
#define SRT_NX4000_icd_cfg2_val         0
#define DFLT_VAL_NX4000_icd_cfg2_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_cfg2_val 0x00000000U

/* all used bits of 'NX4000_icd_cfg2': */
#define MSK_USED_BITS_NX4000_icd_cfg2 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_cfg3 */
/* => See register icd_cfg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_cfg3            0x00000C0CU
#define Adr_NX4000_ic_distributor_icd_cfg3 0xFAF01C0CU
#define Adr_NX4000_icd_cfg3                0xFAF01C0CU
#define DFLT_VAL_NX4000_icd_cfg3           0x00000000U

#define MSK_NX4000_icd_cfg3_val         0xffffffffU
#define SRT_NX4000_icd_cfg3_val         0
#define DFLT_VAL_NX4000_icd_cfg3_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_cfg3_val 0x00000000U

/* all used bits of 'NX4000_icd_cfg3': */
#define MSK_USED_BITS_NX4000_icd_cfg3 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_cfg4 */
/* => See register icd_cfg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_cfg4            0x00000C10U
#define Adr_NX4000_ic_distributor_icd_cfg4 0xFAF01C10U
#define Adr_NX4000_icd_cfg4                0xFAF01C10U
#define DFLT_VAL_NX4000_icd_cfg4           0x00000000U

#define MSK_NX4000_icd_cfg4_val         0xffffffffU
#define SRT_NX4000_icd_cfg4_val         0
#define DFLT_VAL_NX4000_icd_cfg4_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_cfg4_val 0x00000000U

/* all used bits of 'NX4000_icd_cfg4': */
#define MSK_USED_BITS_NX4000_icd_cfg4 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_cfg5 */
/* => See register icd_cfg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_cfg5            0x00000C14U
#define Adr_NX4000_ic_distributor_icd_cfg5 0xFAF01C14U
#define Adr_NX4000_icd_cfg5                0xFAF01C14U
#define DFLT_VAL_NX4000_icd_cfg5           0x00000000U

#define MSK_NX4000_icd_cfg5_val         0xffffffffU
#define SRT_NX4000_icd_cfg5_val         0
#define DFLT_VAL_NX4000_icd_cfg5_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_cfg5_val 0x00000000U

/* all used bits of 'NX4000_icd_cfg5': */
#define MSK_USED_BITS_NX4000_icd_cfg5 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_cfg6 */
/* => See register icd_cfg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_cfg6            0x00000C18U
#define Adr_NX4000_ic_distributor_icd_cfg6 0xFAF01C18U
#define Adr_NX4000_icd_cfg6                0xFAF01C18U
#define DFLT_VAL_NX4000_icd_cfg6           0x00000000U

#define MSK_NX4000_icd_cfg6_val         0xffffffffU
#define SRT_NX4000_icd_cfg6_val         0
#define DFLT_VAL_NX4000_icd_cfg6_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_cfg6_val 0x00000000U

/* all used bits of 'NX4000_icd_cfg6': */
#define MSK_USED_BITS_NX4000_icd_cfg6 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_cfg7 */
/* => See register icd_cfg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_cfg7            0x00000C1CU
#define Adr_NX4000_ic_distributor_icd_cfg7 0xFAF01C1CU
#define Adr_NX4000_icd_cfg7                0xFAF01C1CU
#define DFLT_VAL_NX4000_icd_cfg7           0x00000000U

#define MSK_NX4000_icd_cfg7_val         0xffffffffU
#define SRT_NX4000_icd_cfg7_val         0
#define DFLT_VAL_NX4000_icd_cfg7_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_cfg7_val 0x00000000U

/* all used bits of 'NX4000_icd_cfg7': */
#define MSK_USED_BITS_NX4000_icd_cfg7 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_cfg8 */
/* => See register icd_cfg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_cfg8            0x00000C20U
#define Adr_NX4000_ic_distributor_icd_cfg8 0xFAF01C20U
#define Adr_NX4000_icd_cfg8                0xFAF01C20U
#define DFLT_VAL_NX4000_icd_cfg8           0x00000000U

#define MSK_NX4000_icd_cfg8_val         0xffffffffU
#define SRT_NX4000_icd_cfg8_val         0
#define DFLT_VAL_NX4000_icd_cfg8_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_cfg8_val 0x00000000U

/* all used bits of 'NX4000_icd_cfg8': */
#define MSK_USED_BITS_NX4000_icd_cfg8 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_cfg9 */
/* => See register icd_cfg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_cfg9            0x00000C24U
#define Adr_NX4000_ic_distributor_icd_cfg9 0xFAF01C24U
#define Adr_NX4000_icd_cfg9                0xFAF01C24U
#define DFLT_VAL_NX4000_icd_cfg9           0x00000000U

#define MSK_NX4000_icd_cfg9_val         0xffffffffU
#define SRT_NX4000_icd_cfg9_val         0
#define DFLT_VAL_NX4000_icd_cfg9_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_cfg9_val 0x00000000U

/* all used bits of 'NX4000_icd_cfg9': */
#define MSK_USED_BITS_NX4000_icd_cfg9 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_cfg10 */
/* => See register icd_cfg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_cfg10            0x00000C28U
#define Adr_NX4000_ic_distributor_icd_cfg10 0xFAF01C28U
#define Adr_NX4000_icd_cfg10                0xFAF01C28U
#define DFLT_VAL_NX4000_icd_cfg10           0x00000000U

#define MSK_NX4000_icd_cfg10_val         0xffffffffU
#define SRT_NX4000_icd_cfg10_val         0
#define DFLT_VAL_NX4000_icd_cfg10_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_cfg10_val 0x00000000U

/* all used bits of 'NX4000_icd_cfg10': */
#define MSK_USED_BITS_NX4000_icd_cfg10 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_cfg11 */
/* => See register icd_cfg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_cfg11            0x00000C2CU
#define Adr_NX4000_ic_distributor_icd_cfg11 0xFAF01C2CU
#define Adr_NX4000_icd_cfg11                0xFAF01C2CU
#define DFLT_VAL_NX4000_icd_cfg11           0x00000000U

#define MSK_NX4000_icd_cfg11_val         0xffffffffU
#define SRT_NX4000_icd_cfg11_val         0
#define DFLT_VAL_NX4000_icd_cfg11_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_cfg11_val 0x00000000U

/* all used bits of 'NX4000_icd_cfg11': */
#define MSK_USED_BITS_NX4000_icd_cfg11 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_cfg12 */
/* => See register icd_cfg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_cfg12            0x00000C30U
#define Adr_NX4000_ic_distributor_icd_cfg12 0xFAF01C30U
#define Adr_NX4000_icd_cfg12                0xFAF01C30U
#define DFLT_VAL_NX4000_icd_cfg12           0x00000000U

#define MSK_NX4000_icd_cfg12_val         0xffffffffU
#define SRT_NX4000_icd_cfg12_val         0
#define DFLT_VAL_NX4000_icd_cfg12_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_cfg12_val 0x00000000U

/* all used bits of 'NX4000_icd_cfg12': */
#define MSK_USED_BITS_NX4000_icd_cfg12 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_cfg13 */
/* => See register icd_cfg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_cfg13            0x00000C34U
#define Adr_NX4000_ic_distributor_icd_cfg13 0xFAF01C34U
#define Adr_NX4000_icd_cfg13                0xFAF01C34U
#define DFLT_VAL_NX4000_icd_cfg13           0x00000000U

#define MSK_NX4000_icd_cfg13_val         0xffffffffU
#define SRT_NX4000_icd_cfg13_val         0
#define DFLT_VAL_NX4000_icd_cfg13_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_cfg13_val 0x00000000U

/* all used bits of 'NX4000_icd_cfg13': */
#define MSK_USED_BITS_NX4000_icd_cfg13 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_cfg14 */
/* => See register icd_cfg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_cfg14            0x00000C38U
#define Adr_NX4000_ic_distributor_icd_cfg14 0xFAF01C38U
#define Adr_NX4000_icd_cfg14                0xFAF01C38U
#define DFLT_VAL_NX4000_icd_cfg14           0x00000000U

#define MSK_NX4000_icd_cfg14_val         0xffffffffU
#define SRT_NX4000_icd_cfg14_val         0
#define DFLT_VAL_NX4000_icd_cfg14_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_cfg14_val 0x00000000U

/* all used bits of 'NX4000_icd_cfg14': */
#define MSK_USED_BITS_NX4000_icd_cfg14 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_cfg15 */
/* => See register icd_cfg0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_cfg15            0x00000C3CU
#define Adr_NX4000_ic_distributor_icd_cfg15 0xFAF01C3CU
#define Adr_NX4000_icd_cfg15                0xFAF01C3CU
#define DFLT_VAL_NX4000_icd_cfg15           0x00000000U

#define MSK_NX4000_icd_cfg15_val         0xffffffffU
#define SRT_NX4000_icd_cfg15_val         0
#define DFLT_VAL_NX4000_icd_cfg15_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_cfg15_val 0x00000000U

/* all used bits of 'NX4000_icd_cfg15': */
#define MSK_USED_BITS_NX4000_icd_cfg15 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_ppi_stat */
/* => PPI Status Register */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_ppi_stat            0x00000D00U
#define Adr_NX4000_ic_distributor_icd_ppi_stat 0xFAF01D00U
#define Adr_NX4000_icd_ppi_stat                0xFAF01D00U

#define MSK_NX4000_icd_ppi_stat_res0   0x000007ffU
#define SRT_NX4000_icd_ppi_stat_res0   0
#define MSK_NX4000_icd_ppi_stat_gtimer 0x00000800U
#define SRT_NX4000_icd_ppi_stat_gtimer 11
#define MSK_NX4000_icd_ppi_stat_nfiq   0x00001000U
#define SRT_NX4000_icd_ppi_stat_nfiq   12
#define MSK_NX4000_icd_ppi_stat_ptimer 0x00002000U
#define SRT_NX4000_icd_ppi_stat_ptimer 13
#define MSK_NX4000_icd_ppi_stat_wdg    0x00004000U
#define SRT_NX4000_icd_ppi_stat_wdg    14
#define MSK_NX4000_icd_ppi_stat_nirq   0x00008000U
#define SRT_NX4000_icd_ppi_stat_nirq   15
#define MSK_NX4000_icd_ppi_stat_res1   0xffff0000U
#define SRT_NX4000_icd_ppi_stat_res1   16

/* all used bits of 'NX4000_icd_ppi_stat': */
#define MSK_USED_BITS_NX4000_icd_ppi_stat 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_spi_stat0 */
/* => SPI Status Registers */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_spi_stat0            0x00000D04U
#define Adr_NX4000_ic_distributor_icd_spi_stat0 0xFAF01D04U
#define Adr_NX4000_icd_spi_stat0                0xFAF01D04U

#define MSK_NX4000_icd_spi_stat0_val 0xffffffffU
#define SRT_NX4000_icd_spi_stat0_val 0

/* all used bits of 'NX4000_icd_spi_stat0': */
#define MSK_USED_BITS_NX4000_icd_spi_stat0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_spi_stat1 */
/* => See register icd_spi_stat0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_spi_stat1            0x00000D08U
#define Adr_NX4000_ic_distributor_icd_spi_stat1 0xFAF01D08U
#define Adr_NX4000_icd_spi_stat1                0xFAF01D08U
#define DFLT_VAL_NX4000_icd_spi_stat1           0x00000000U

#define MSK_NX4000_icd_spi_stat1_val         0xffffffffU
#define SRT_NX4000_icd_spi_stat1_val         0
#define DFLT_VAL_NX4000_icd_spi_stat1_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_spi_stat1_val 0x00000000U

/* all used bits of 'NX4000_icd_spi_stat1': */
#define MSK_USED_BITS_NX4000_icd_spi_stat1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_spi_stat2 */
/* => See register icd_spi_stat0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_spi_stat2            0x00000D0CU
#define Adr_NX4000_ic_distributor_icd_spi_stat2 0xFAF01D0CU
#define Adr_NX4000_icd_spi_stat2                0xFAF01D0CU
#define DFLT_VAL_NX4000_icd_spi_stat2           0x00000000U

#define MSK_NX4000_icd_spi_stat2_val         0xffffffffU
#define SRT_NX4000_icd_spi_stat2_val         0
#define DFLT_VAL_NX4000_icd_spi_stat2_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_spi_stat2_val 0x00000000U

/* all used bits of 'NX4000_icd_spi_stat2': */
#define MSK_USED_BITS_NX4000_icd_spi_stat2 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_spi_stat3 */
/* => See register icd_spi_stat0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_spi_stat3            0x00000D10U
#define Adr_NX4000_ic_distributor_icd_spi_stat3 0xFAF01D10U
#define Adr_NX4000_icd_spi_stat3                0xFAF01D10U
#define DFLT_VAL_NX4000_icd_spi_stat3           0x00000000U

#define MSK_NX4000_icd_spi_stat3_val         0xffffffffU
#define SRT_NX4000_icd_spi_stat3_val         0
#define DFLT_VAL_NX4000_icd_spi_stat3_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_spi_stat3_val 0x00000000U

/* all used bits of 'NX4000_icd_spi_stat3': */
#define MSK_USED_BITS_NX4000_icd_spi_stat3 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_spi_stat4 */
/* => See register icd_spi_stat0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_spi_stat4            0x00000D14U
#define Adr_NX4000_ic_distributor_icd_spi_stat4 0xFAF01D14U
#define Adr_NX4000_icd_spi_stat4                0xFAF01D14U
#define DFLT_VAL_NX4000_icd_spi_stat4           0x00000000U

#define MSK_NX4000_icd_spi_stat4_val         0xffffffffU
#define SRT_NX4000_icd_spi_stat4_val         0
#define DFLT_VAL_NX4000_icd_spi_stat4_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_spi_stat4_val 0x00000000U

/* all used bits of 'NX4000_icd_spi_stat4': */
#define MSK_USED_BITS_NX4000_icd_spi_stat4 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_spi_stat5 */
/* => See register icd_spi_stat0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_spi_stat5            0x00000D18U
#define Adr_NX4000_ic_distributor_icd_spi_stat5 0xFAF01D18U
#define Adr_NX4000_icd_spi_stat5                0xFAF01D18U
#define DFLT_VAL_NX4000_icd_spi_stat5           0x00000000U

#define MSK_NX4000_icd_spi_stat5_val         0xffffffffU
#define SRT_NX4000_icd_spi_stat5_val         0
#define DFLT_VAL_NX4000_icd_spi_stat5_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_spi_stat5_val 0x00000000U

/* all used bits of 'NX4000_icd_spi_stat5': */
#define MSK_USED_BITS_NX4000_icd_spi_stat5 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_spi_stat6 */
/* => See register icd_spi_stat0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_spi_stat6            0x00000D1CU
#define Adr_NX4000_ic_distributor_icd_spi_stat6 0xFAF01D1CU
#define Adr_NX4000_icd_spi_stat6                0xFAF01D1CU
#define DFLT_VAL_NX4000_icd_spi_stat6           0x00000000U

#define MSK_NX4000_icd_spi_stat6_val         0xffffffffU
#define SRT_NX4000_icd_spi_stat6_val         0
#define DFLT_VAL_NX4000_icd_spi_stat6_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_spi_stat6_val 0x00000000U

/* all used bits of 'NX4000_icd_spi_stat6': */
#define MSK_USED_BITS_NX4000_icd_spi_stat6 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_soft_gen */
/* => Software Generated Interrupt Register (ICDSGIR) */
/*    Controls the generation of SGIs. */
/* => Mode: W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_soft_gen            0x00000F00U
#define Adr_NX4000_ic_distributor_icd_soft_gen 0xFAF01F00U
#define Adr_NX4000_icd_soft_gen                0xFAF01F00U
#define DFLT_VAL_NX4000_icd_soft_gen           0x00000000U

#define MSK_NX4000_icd_soft_gen_sgintid                 0x0000000fU
#define SRT_NX4000_icd_soft_gen_sgintid                 0
#define DFLT_VAL_NX4000_icd_soft_gen_sgintid            0x00000000U
#define DFLT_BF_VAL_NX4000_icd_soft_gen_sgintid         0x00000000U
#define MSK_NX4000_icd_soft_gen_res0                    0x0000fff0U
#define SRT_NX4000_icd_soft_gen_res0                    4
#define DFLT_VAL_NX4000_icd_soft_gen_res0               0x00000000U
#define DFLT_BF_VAL_NX4000_icd_soft_gen_res0            0x00000000U
#define MSK_NX4000_icd_soft_gen_cpu_trg_list            0x00ff0000U
#define SRT_NX4000_icd_soft_gen_cpu_trg_list            16
#define DFLT_VAL_NX4000_icd_soft_gen_cpu_trg_list       0x00000000U
#define DFLT_BF_VAL_NX4000_icd_soft_gen_cpu_trg_list    0x00000000U
#define MSK_NX4000_icd_soft_gen_trg_list_filter         0x03000000U
#define SRT_NX4000_icd_soft_gen_trg_list_filter         24
#define DFLT_VAL_NX4000_icd_soft_gen_trg_list_filter    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_soft_gen_trg_list_filter 0x00000000U
#define MSK_NX4000_icd_soft_gen_res                     0xfc000000U
#define SRT_NX4000_icd_soft_gen_res                     26
#define DFLT_VAL_NX4000_icd_soft_gen_res                0x00000000U
#define DFLT_BF_VAL_NX4000_icd_soft_gen_res             0x00000000U

/* all used bits of 'NX4000_icd_soft_gen': */
#define MSK_USED_BITS_NX4000_icd_soft_gen 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_peripheral_id0 */
/* => Peripheral Identification Registers */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_peripheral_id0            0x00000FD0U
#define Adr_NX4000_ic_distributor_icd_peripheral_id0 0xFAF01FD0U
#define Adr_NX4000_icd_peripheral_id0                0xFAF01FD0U

#define MSK_NX4000_icd_peripheral_id0_val 0xffffffffU
#define SRT_NX4000_icd_peripheral_id0_val 0

/* all used bits of 'NX4000_icd_peripheral_id0': */
#define MSK_USED_BITS_NX4000_icd_peripheral_id0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_peripheral_id1 */
/* => See register icd_peripheral_id0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_peripheral_id1            0x00000FD4U
#define Adr_NX4000_ic_distributor_icd_peripheral_id1 0xFAF01FD4U
#define Adr_NX4000_icd_peripheral_id1                0xFAF01FD4U
#define DFLT_VAL_NX4000_icd_peripheral_id1           0x00000000U

#define MSK_NX4000_icd_peripheral_id1_val         0xffffffffU
#define SRT_NX4000_icd_peripheral_id1_val         0
#define DFLT_VAL_NX4000_icd_peripheral_id1_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_peripheral_id1_val 0x00000000U

/* all used bits of 'NX4000_icd_peripheral_id1': */
#define MSK_USED_BITS_NX4000_icd_peripheral_id1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_peripheral_id2 */
/* => See register icd_peripheral_id0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_peripheral_id2            0x00000FD8U
#define Adr_NX4000_ic_distributor_icd_peripheral_id2 0xFAF01FD8U
#define Adr_NX4000_icd_peripheral_id2                0xFAF01FD8U
#define DFLT_VAL_NX4000_icd_peripheral_id2           0x00000000U

#define MSK_NX4000_icd_peripheral_id2_val         0xffffffffU
#define SRT_NX4000_icd_peripheral_id2_val         0
#define DFLT_VAL_NX4000_icd_peripheral_id2_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_peripheral_id2_val 0x00000000U

/* all used bits of 'NX4000_icd_peripheral_id2': */
#define MSK_USED_BITS_NX4000_icd_peripheral_id2 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_peripheral_id3 */
/* => See register icd_peripheral_id0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_peripheral_id3            0x00000FDCU
#define Adr_NX4000_ic_distributor_icd_peripheral_id3 0xFAF01FDCU
#define Adr_NX4000_icd_peripheral_id3                0xFAF01FDCU
#define DFLT_VAL_NX4000_icd_peripheral_id3           0x00000000U

#define MSK_NX4000_icd_peripheral_id3_val         0xffffffffU
#define SRT_NX4000_icd_peripheral_id3_val         0
#define DFLT_VAL_NX4000_icd_peripheral_id3_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_peripheral_id3_val 0x00000000U

/* all used bits of 'NX4000_icd_peripheral_id3': */
#define MSK_USED_BITS_NX4000_icd_peripheral_id3 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_peripheral_id4 */
/* => See register icd_peripheral_id0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_peripheral_id4            0x00000FE0U
#define Adr_NX4000_ic_distributor_icd_peripheral_id4 0xFAF01FE0U
#define Adr_NX4000_icd_peripheral_id4                0xFAF01FE0U
#define DFLT_VAL_NX4000_icd_peripheral_id4           0x00000000U

#define MSK_NX4000_icd_peripheral_id4_val         0xffffffffU
#define SRT_NX4000_icd_peripheral_id4_val         0
#define DFLT_VAL_NX4000_icd_peripheral_id4_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_peripheral_id4_val 0x00000000U

/* all used bits of 'NX4000_icd_peripheral_id4': */
#define MSK_USED_BITS_NX4000_icd_peripheral_id4 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_peripheral_id5 */
/* => See register icd_peripheral_id0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_peripheral_id5            0x00000FE4U
#define Adr_NX4000_ic_distributor_icd_peripheral_id5 0xFAF01FE4U
#define Adr_NX4000_icd_peripheral_id5                0xFAF01FE4U
#define DFLT_VAL_NX4000_icd_peripheral_id5           0x00000000U

#define MSK_NX4000_icd_peripheral_id5_val         0xffffffffU
#define SRT_NX4000_icd_peripheral_id5_val         0
#define DFLT_VAL_NX4000_icd_peripheral_id5_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_peripheral_id5_val 0x00000000U

/* all used bits of 'NX4000_icd_peripheral_id5': */
#define MSK_USED_BITS_NX4000_icd_peripheral_id5 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_peripheral_id6 */
/* => See register icd_peripheral_id0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_peripheral_id6            0x00000FE8U
#define Adr_NX4000_ic_distributor_icd_peripheral_id6 0xFAF01FE8U
#define Adr_NX4000_icd_peripheral_id6                0xFAF01FE8U
#define DFLT_VAL_NX4000_icd_peripheral_id6           0x00000000U

#define MSK_NX4000_icd_peripheral_id6_val         0xffffffffU
#define SRT_NX4000_icd_peripheral_id6_val         0
#define DFLT_VAL_NX4000_icd_peripheral_id6_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_peripheral_id6_val 0x00000000U

/* all used bits of 'NX4000_icd_peripheral_id6': */
#define MSK_USED_BITS_NX4000_icd_peripheral_id6 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_peripheral_id7 */
/* => See register icd_peripheral_id0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_peripheral_id7            0x00000FECU
#define Adr_NX4000_ic_distributor_icd_peripheral_id7 0xFAF01FECU
#define Adr_NX4000_icd_peripheral_id7                0xFAF01FECU
#define DFLT_VAL_NX4000_icd_peripheral_id7           0x00000000U

#define MSK_NX4000_icd_peripheral_id7_val         0xffffffffU
#define SRT_NX4000_icd_peripheral_id7_val         0
#define DFLT_VAL_NX4000_icd_peripheral_id7_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_peripheral_id7_val 0x00000000U

/* all used bits of 'NX4000_icd_peripheral_id7': */
#define MSK_USED_BITS_NX4000_icd_peripheral_id7 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_component_id0 */
/* => Component Identification Registers */
/* => Mode: R */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_component_id0            0x00000FD0U
#define Adr_NX4000_ic_distributor_icd_component_id0 0xFAF01FD0U
#define Adr_NX4000_icd_component_id0                0xFAF01FD0U

#define MSK_NX4000_icd_component_id0_val 0xffffffffU
#define SRT_NX4000_icd_component_id0_val 0

/* all used bits of 'NX4000_icd_component_id0': */
#define MSK_USED_BITS_NX4000_icd_component_id0 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_component_id1 */
/* => See register icd_component_id0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_component_id1            0x00000FD4U
#define Adr_NX4000_ic_distributor_icd_component_id1 0xFAF01FD4U
#define Adr_NX4000_icd_component_id1                0xFAF01FD4U
#define DFLT_VAL_NX4000_icd_component_id1           0x00000000U

#define MSK_NX4000_icd_component_id1_val         0xffffffffU
#define SRT_NX4000_icd_component_id1_val         0
#define DFLT_VAL_NX4000_icd_component_id1_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_component_id1_val 0x00000000U

/* all used bits of 'NX4000_icd_component_id1': */
#define MSK_USED_BITS_NX4000_icd_component_id1 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_component_id2 */
/* => See register icd_component_id0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_component_id2            0x00000FD8U
#define Adr_NX4000_ic_distributor_icd_component_id2 0xFAF01FD8U
#define Adr_NX4000_icd_component_id2                0xFAF01FD8U
#define DFLT_VAL_NX4000_icd_component_id2           0x00000000U

#define MSK_NX4000_icd_component_id2_val         0xffffffffU
#define SRT_NX4000_icd_component_id2_val         0
#define DFLT_VAL_NX4000_icd_component_id2_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_component_id2_val 0x00000000U

/* all used bits of 'NX4000_icd_component_id2': */
#define MSK_USED_BITS_NX4000_icd_component_id2 0xffffffffU

/* --------------------------------------------------------------------- */
/* Register icd_component_id3 */
/* => See register icd_component_id0 */
/* => Mode: R/W */
/* --------------------------------------------------------------------- */

#define REL_Adr_NX4000_icd_component_id3            0x00000FDCU
#define Adr_NX4000_ic_distributor_icd_component_id3 0xFAF01FDCU
#define Adr_NX4000_icd_component_id3                0xFAF01FDCU
#define DFLT_VAL_NX4000_icd_component_id3           0x00000000U

#define MSK_NX4000_icd_component_id3_val         0xffffffffU
#define SRT_NX4000_icd_component_id3_val         0
#define DFLT_VAL_NX4000_icd_component_id3_val    0x00000000U
#define DFLT_BF_VAL_NX4000_icd_component_id3_val 0x00000000U

/* all used bits of 'NX4000_icd_component_id3': */
#define MSK_USED_BITS_NX4000_icd_component_id3 0xffffffffU




/*********************************************************************/
/* area structure definitions                                        */
/*********************************************************************/


typedef struct NX4000_SCU_AREA_Ttag
{
  volatile uint32_t  ulScu_ctrl;
  volatile uint32_t  ulScu_cfg;
  volatile uint32_t  ulScu_cpu_power_stat;
  volatile uint32_t  ulScu_invalidate_all;
  volatile uint32_t aulReserved0[12];
  volatile uint32_t aulScu_master_filter[2];
  volatile uint32_t aulScu_pripheral_filter[2];
  volatile uint32_t  ulScu_access_ctrl;
  volatile uint32_t aulReserved1[3];
  volatile uint32_t aulScu_err_bank_entry[2];
  volatile uint32_t aulReserved2[2];
  volatile uint32_t  ulScu_dtr_op;
  volatile uint32_t  ulScu_dtr_val;
  volatile uint32_t  ulScu_dtr_ecc_chunk;
} NX4000_SCU_AREA_T;

typedef struct NX4000_IC_CPU_INTERFACE_AREA_Ttag
{
  volatile uint32_t  ulIcci_ctrl;
  volatile uint32_t  ulIcci_prio_msk;
  volatile uint32_t  ulIcci_bin_point;
  volatile uint32_t  ulIcci_int_ack;
  volatile uint32_t  ulIcci_int_end;
  volatile uint32_t  ulIcci_running_prio;
  volatile uint32_t  ulIcci_highest_pending;
  volatile uint32_t aulReserved0[56];
  volatile uint32_t  ulIcci_impl_id;
} NX4000_IC_CPU_INTERFACE_AREA_T;

typedef struct NX4000_GLOBAL_TIMER_AREA_Ttag
{
  volatile uint32_t aulGtimer_count[2];
  volatile uint32_t  ulGtimer_ctrl;
  volatile uint32_t  ulGtimer_int;
  volatile uint32_t aulGtimer_comp[2];
  volatile uint32_t  ulGtimer_auto_inc;
} NX4000_GLOBAL_TIMER_AREA_T;

typedef struct NX4000_PRIVATE_TIMER_AREA_Ttag
{
  volatile uint32_t  ulPtimer_load;
  volatile uint32_t  ulPtimer_count;
  volatile uint32_t  ulPtimer_ctrl;
  volatile uint32_t  ulPtimer_int;
  volatile uint32_t aulReserved0[4];
  volatile uint32_t  ulWdg_load;
  volatile uint32_t  ulWdg_count;
  volatile uint32_t  ulWdg_ctrl;
  volatile uint32_t  ulWdg_int;
  volatile uint32_t  ulWdg_reset;
  volatile uint32_t  ulWdg_disable;
} NX4000_PRIVATE_TIMER_AREA_T;

typedef struct NX4000_IC_DISTRIBUTOR_AREA_Ttag
{
  volatile uint32_t  ulIcd_ctrl;
  volatile uint32_t  ulIcd_ic_type;
  volatile uint32_t  ulIcd_impl_id;
  volatile uint32_t aulReserved0[61];
  volatile uint32_t aulIcd_set_en[8];
  volatile uint32_t aulReserved1[24];
  volatile uint32_t aulIcd_clr_en[8];
  volatile uint32_t aulReserved2[24];
  volatile uint32_t aulIcd_set_pending[8];
  volatile uint32_t aulReserved3[24];
  volatile uint32_t aulIcd_clr_pending[8];
  volatile uint32_t aulReserved4[24];
  volatile uint32_t aulIcd_active[8];
  volatile uint32_t aulReserved5[56];
  volatile uint32_t aulIcd_prio[64];
  volatile uint32_t aulReserved6[192];
  volatile uint32_t aulIcd_processor_trg[64];
  volatile uint32_t aulReserved7[192];
  volatile uint32_t aulIcd_cfg[16];
  volatile uint32_t aulReserved8[48];
  volatile uint32_t  ulIcd_ppi_stat;
  volatile uint32_t aulIcd_spi_stat[7];
  volatile uint32_t aulReserved9[120];
  volatile uint32_t  ulIcd_soft_gen;
  volatile uint32_t aulReserved10[51];
  volatile uint32_t aulIcd_component_id[4];
  volatile uint32_t  ulIcd_peripheral_id4;
  volatile uint32_t  ulIcd_peripheral_id5;
  volatile uint32_t  ulIcd_peripheral_id6;
  volatile uint32_t  ulIcd_peripheral_id7;
} NX4000_IC_DISTRIBUTOR_AREA_T;


#endif
