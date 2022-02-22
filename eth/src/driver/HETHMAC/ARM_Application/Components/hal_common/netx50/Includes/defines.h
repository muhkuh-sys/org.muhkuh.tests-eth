/*
************************************************************
*   Macros Definitions
************************************************************
*/
#define INTERRUPT __attribute__ ((interrupt ("IRQ")))

#define POKE(addr, val) (*(volatile unsigned int *)(addr) = (val))
#define PEEK(addr) (*(volatile unsigned int *)(addr))

