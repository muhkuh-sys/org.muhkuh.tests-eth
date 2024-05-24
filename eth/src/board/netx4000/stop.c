#include "board.h"

void board_stop_cpu(void)
{
	while(1)
	{
		__asm__("WFE");
	}
}
