#ifndef __BOARD_H__
#define __BOARD_H__

int board_initialize(void);


/* Stop the CPU.
 * Use a "wait for something" instruction to save some power if this is supported.
 * Default to a dumb endless "NOP" loop.
 * Note that even with the "wait" instruction an encapsulating while loop is
 * needed. After all the CPU might wake up after an event, interrupt or a debugger
 * connection.
 */
void __attribute__ ((noreturn)) board_stop_cpu(void);


/* Reset the ASIC. */
void __attribute__ ((noreturn)) board_reset(void);


#endif  /* __BOARD_H__ */
