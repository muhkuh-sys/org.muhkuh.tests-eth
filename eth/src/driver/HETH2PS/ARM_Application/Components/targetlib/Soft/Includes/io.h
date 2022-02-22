#ifndef __IO_H
#define __IO_H

#include <stdint.h>

/****************************************************************************************
*  Functions
****************************************************************************************/

void io_init(void);

void io_output_init(void);
unsigned int io_output_cnt(void);
void io_output_set(uint32_t ulOutput);

void io_input_init(void);
unsigned int io_input_cnt(void);
uint32_t io_input_get(void);

uint32_t io_address_get(void);

void io_apptrg_init(void);
void io_apptrg_set(unsigned int uiIdx, unsigned int uiVal);

void io_serial_init(unsigned int uiPort);

#endif /* __IO_H */
