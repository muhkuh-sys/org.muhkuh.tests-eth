/**
 * \file target_defines.h
 * \brief Definitions of various netX90 boards supported by 2port switch
 *
 * \author Marvin Mager - mmager@hilscher.com
 *
 * \date 17.08.2021 - created
 */

#ifndef TARGETS_NETX90_INCLUDE_TARGET_DEFINES_H_
#define TARGETS_NETX90_INCLUDE_TARGET_DEFINES_H_

/* Select target hardware by defining TARGET_HW */
/* Possible values for TARGET_HW */
#define _NXHX90_JTAG_    0
#define _NXEB90_ETM_     1
#define _NXEB90_SPE_     2

#ifndef TARGET_HW
#define TARGET_HW _NXHX90_JTAG_
#endif

#endif /* TARGETS_NETX90_INCLUDE_TARGET_DEFINES_H_ */
