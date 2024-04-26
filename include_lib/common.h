#ifndef __COMMON_H__
#define __COMMON_H__

#include "typedef.h"

extern data volatile u8 jiffies;
extern data volatile u8 delay_cnt;
extern data volatile u8 irq_lock_cnt;

#define time_after(a,b)					((char)(b) - (char)(a) < 0)
#define time_before(a,b)				time_after(b,a)

void delay(u8 t);
void delay_n2ms(u8 t);
void my_memset(u8 idata *, u8, u8);
void my_memcpy(u8 idata *, u8 *, u8);

#define local_irq_disable() \
	EA = 0; \
	irq_lock_cnt++
	

#define local_irq_enable() \
	irq_lock_cnt--; \
	if (irq_lock_cnt == 0) { \
		EA = 1; \
	}
    

#endif


