#ifndef _PULSE_CNT_H_
#define _PULSE_CNT_H_

#include "typedef.h"

enum {
	BSEL_BIT_0_7,   //BIT(0)~BIT(7)	
	BSEL_BIT_8_15,  //BIT(8)~BIT(15)
	BSEL_BIT_16_23,
	BSEL_BIT_24_31,
	BSEL_BIT_32_39,
};

extern idata volatile u8 g_mux_buf[];//全局数据buf

void sleep_auto_wkup_ctrl(u8 en);
void sleep_auto_wkup_init(void);
u8 is_sleep_auto_wkup(void);

#endif  /*  _PULSE_CNT_H_ */


