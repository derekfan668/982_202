#ifndef _WAKE_UP_H_
#define _WAKE_UP_H_

#include "typedef.h"

void io_wakeup_en(void);

enum {
    WAKE_FILTER_0US,
    WAKE_FILTER_32US,
    WAKE_FILTER_64US,
    WAKE_FILTER_128US,
    WAKE_FILTER_256US,
    WAKE_FILTER_512US,
    WAKE_FILTER_1MS,
    WAKE_FILTER_2MS,
};

enum {
    WAKE_CH_0,
    WAKE_CH_1,
    WAKE_CH_2,
    WAKE_CH_3,
};

enum {
  RISING_EDGE,
  FAILING_EDGE,
};

//i取值范围为WAKE_PORT_0~WAKE_PORT_3，也就是只支持4路port wkup
#define WAKE_SET_EDGE(i, x)     WKUP_CON &= ~BIT(i+4); WKUP_CON |= (x<<(i+4))
#define WAKE_SET_EN(i)       	WKUP_CON |= BIT(i)
#define WAKE_SET_DIS(i)       	WKUP_CON &= ~BIT(i);

#define WAKE_SET_FILTER(x)      WKUP_FLT &= ~0xf0; WKUP_FLT |= (x<<4)
#define WAKE_FILTER_EN(i)    	WKUP_FLT |= BIT(i)
#define WAKE_FILTER_DIS(i)    	WKUP_FLT &= ~BIT(i);

#define WAKE_CLR_ALL_PND()      WKUP_PND |= 0x0f
#define WAKE_CLR_PND(i)         WKUP_PND |= BIT(i)
#define WAKE_CHECK_ALL_PND()    WKUP_PND & 0xf0
#define WAKE_CHECK_PND(i)       WKUP_PND & BIT(i+4)

#endif
