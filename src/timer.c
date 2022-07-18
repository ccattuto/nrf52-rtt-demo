#include <common.h>
#include <timer.h>

static volatile uint8_t timer_running;

void timer_wait(uint32_t ms)
{
	timer_running = 1;

	NRF_RTC1->TASKS_CLEAR = 1;
	NRF_RTC1->CC[0] = ms * LFCLOCK_FREQ / 1000;
	NRF_RTC1->TASKS_START = 1;

	while (timer_running)
        __WFE();
}

void RTC1_IRQHandler(void)
{
    if (NRF_RTC1->EVENTS_COMPARE[0]) {
	    timer_running = 0;

	    NRF_RTC1->EVENTS_COMPARE[0] = 0;
        (void) NRF_RTC1->EVENTS_COMPARE[0];

	    NRF_RTC1->TASKS_STOP = 1;
    }
}

void timer_init(void)
{
	NRF_RTC1->TASKS_STOP = 1;
	NRF_RTC1->PRESCALER = 0;
    NRF_RTC1->EVTENSET = (RTC_EVTENSET_COMPARE0_Enabled << RTC_EVTENSET_COMPARE0_Pos);
	NRF_RTC1->INTENSET = (RTC_INTENSET_COMPARE0_Enabled << RTC_INTENSET_COMPARE0_Pos);

	NVIC_SetPriority(RTC1_IRQn, RTC1_IRQ_PRIORITY);
	NVIC_ClearPendingIRQ(RTC1_IRQn);
	NVIC_EnableIRQ(RTC1_IRQn);
}
