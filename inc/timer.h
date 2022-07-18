#ifndef __TIMER_H__
#define __TIMER_H__

#define LFCLOCK_FREQ 32768

void timer_init(void);
void timer_wait(uint32_t ms);

#endif/*__TIMER_H__*/
