#ifndef __COMPILER_SPEC_H
#define __COMPILER_SPEC_H

#include <stdint.h>   /* ISO C: uint8_t/uint16_t/uint32_t definitions */
#include <stdbool.h>  /* ISO C: "bool" definition */

#ifndef FALSE
#define FALSE false
#endif

#ifndef TRUE
#define TRUE true
#endif

#ifndef NULL
#define NULL (0)
#endif

#ifdef __ARM_COMPILER__
  #ifndef INTERRUPT
  #define INTERRUPT __irq
  #endif
  #ifndef __PACKED_PRE
  #define __PACKED_PRE __packed
  #endif
  #ifndef __PACKED_POST
  #define __PACKED_POST
  #endif
  #define __NOP() __nop
  #define __WFI() __wfi
  #define __WFE() __wfe
  #define __SEV() __sev
  #define __DMB() do{ __schedule_barrier(); __dmb(0xF); __schedule_barrier(); }while(0)
  #define __DSB() do{ __schedule_barrier(); __dsb(0xF); __schedule_barrier(); }while(0)
  #define __ISB() do{ __schedule_barrier(); __isb(0xF); __schedule_barrier(); }while(0)
#else
  #ifndef INTERRUPT
  #define INTERRUPT __attribute__ ((interrupt ("IRQ")))
  #define __EXCEPTION(type) __attribute__ ((interrupt (type)))
  #endif
  #ifndef __PACKED_PRE
  #define __PACKED_PRE
  #endif
  #ifndef __PACKED_POST
  #define __PACKED_POST __attribute__((packed))
  #endif
  #define __NOP() __asm__ __volatile__("NOP")
  #define __WFI() __asm__ __volatile__("WFI")
  #define __WFE() __asm__ __volatile__("WFE")
  #define __SEV() __asm__ __volatile__("SEV")
  #define __DMB() __asm__ __volatile__("DMB")
  #define __DSB() __asm__ __volatile__("DSB")
  #define __ISB() __asm__ __volatile__("ISB")
#endif

/* Byte swap operations */
#ifndef htons
#define htons(val) ((((val) & 0xff) << 8) | ((val) >> 8))
#endif
#ifndef htonl
#define htonl(val) ((((val) & 0xff) << 24) | (((val) & 0xff00) << 8) | (((val) & 0xff0000) >> 8 ) | ((val) >> 24))
#endif
#ifndef htonll
#define htonll(val) ( (((val) & 0xffULL) << 56) | (((val) & 0xff00ULL) << 40) | (((val) & 0xff0000ULL) << 24 ) | (((val) & 0xff000000ULL) << 8) |\
                      (((val) & 0xff00000000ULL) >> 8) | (((val) & 0xff0000000000ULL) >> 24) | (((val) & 0xff000000000000ULL) >> 40 ) | (((val) & 0xff00000000000000ULL) >> 56))
#endif
#ifndef ntohs
#define ntohs(val) htons(val)
#endif
#ifndef ntohl
#define ntohl(val) htonl(val)
#endif
#ifndef ntohll
#define ntohll(val) htonll(val)
#endif

/* number of elements of an array */
#ifndef ARRAY_LEN
#define ARRAY_LEN(x) (sizeof(x)/sizeof(x[0]))
#endif

/* compile time assertion */
#define CASSERT(cond, msg) typedef char msg[(cond) ? 1 : -1];

#endif /* __COMPILER_SPEC_H */
