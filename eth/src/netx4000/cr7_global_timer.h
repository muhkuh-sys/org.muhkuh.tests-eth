#ifndef __CR7_PRIVATE_TIMER_H__
#define __CR7_PRIVATE_TIMER_H__

/* The global timer seems to run on half of the peripheral clock. This is 5ns in the simulation. */
#define NX4000_GLOBAL_TIMER_TICK_NS 5

#define CR7_GLOBAL_TIMER_US_TO_TICKS(ulDelayUs)  (((unsigned long long)ulDelayUs)*200ULL)
#define CR7_GLOBAL_TIMER_MS_TO_TICKS(ulDelayMs)  (((unsigned long long)ulDelayMs)*200000ULL)


typedef struct CR7_TIMER_HANDLE_STRUCT
{
	unsigned long long ullStart;
	unsigned long long ullDuration;
} CR7_TIMER_HANDLE_T;


void cr7_global_timer_initialize(void);

int cr7_global_timer_elapsed(CR7_TIMER_HANDLE_T *ptHandle);
unsigned long long cr7_global_timer_get_ticks(void);

void cr7_global_timer_start_ticks(CR7_TIMER_HANDLE_T *ptHandle, unsigned long ulDelayTicks);
void cr7_global_timer_start_us(CR7_TIMER_HANDLE_T *ptHandle, unsigned long ulDelayUs);
void cr7_global_timer_start_ms(CR7_TIMER_HANDLE_T *ptHandle, unsigned long ulDelayMs);

void cr7_global_timer_delay_ticks(unsigned long ulDelayTicks);
void cr7_global_timer_delay_us(unsigned long ulDelayUs);
void cr7_global_timer_delay_ms(unsigned long ulDelayMs);


#endif  /* __CR7_PRIVATE_TIMER_H__ */

