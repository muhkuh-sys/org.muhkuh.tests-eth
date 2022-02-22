#ifndef __DEFINES_H
#define __DEFINES_H

/*************************************************************/
/*  Definitions                                              */
/*************************************************************/

#define POKE(addr, val) (*(volatile unsigned int *)(addr) = (val))
#define PEEK(addr) (*(volatile unsigned int *)(addr))

#define ARRAY_LEN(x) (sizeof(x)/sizeof(x[0]))

#define NX51_DEV_FREQUENCY 100000000L /* 100 Mhz */


#endif  /* #ifndef __DEFINES_H */
