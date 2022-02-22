#ifndef __DEFINES_H
#define __DEFINES_H

#define POKE(addr, val) (*(volatile unsigned int *)(addr) = (val))
#define PEEK(addr) (*(volatile unsigned int *)(addr))

#endif  /* __DEFINES_H */
