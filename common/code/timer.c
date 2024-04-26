#include "includes.h"
#include "app_config.h"
#include "timer.h"
#include "usb_det.h"
#include "key.h"
#include "hall_det.h"
#include "wireless.h"
#include "ui.h"
#include "app_var.h"

data volatile u8 jiffies;
data volatile u8 delay_cnt;
data u8 timer_8s_cnt;
static data u8 timer_cnt;

#define TIMER_8MS_CNT   (8 / TCFG_TIMER_LOOP - 1)
#define TIMER_256MS_CNT (256 / TCFG_TIMER_LOOP - 1)
#define TIMER_512MS_CNT (512 / TCFG_TIMER_LOOP - 1)
#define TIMER_8S_CNT    (16 - 1)

void delay_n2ms(u8 t)
{
	if (EA) {
		delay_cnt = t;
    	while (delay_cnt) {
			_nop_();
		}
	} else {
		while (t--) {
			TMR0_CNT = 0;
			TMR0_CON0 |= TMR_CPND;
			while (!(TMR0_CON0&TMR_PND));
		}
	}
}

void timer0_isr(void) interrupt TIMER0_INT
{
    TMR0_CON0 |= TMR_CPND;
    timer_cnt++;
    jiffies++;
	b_evt_2ms = 1;
    if (delay_cnt) {
        delay_cnt--;
    }

#if TCFG_UI_ENABLE && (TCFG_UI_MODE == UI_SEG_LED)
        ui_scan();
#endif
	
    if ((timer_cnt & TIMER_8MS_CNT) == 0) {
#if TCFG_KEY_ENABLE
        key_scan();
#endif
#if TCFG_HALL_ENABLE
        hall_scan();
#endif
		usb_scan();
    }

    if ((timer_cnt & TIMER_256MS_CNT) == 0) {
        b_evt_256ms = 1;
    }
    if ((timer_cnt & TIMER_512MS_CNT) == 0) {
		b_evt_512ms = 1;
        timer_cnt = 0;
        timer_8s_cnt++;
        if ((timer_8s_cnt % TIMER_8S_CNT) == 0) {
            timer_8s_cnt = 0;
            b_evt_8s = 1;
			if (sys_info.sys_abnormal_cnt < TCFG_SYS_ABNORMAL_CNT_MAX) {
				sys_info.sys_abnormal_cnt = 0;
			}
        }
    }
}

void timer0_init(void) 
{
    TMR0_CNT = 0;
    TMR0_CON1 = 0;
    TMR0_PR = W_CLK / (1000 / TCFG_TIMER_LOOP) - 1;
    TMR0_CON0 = TMR_CPND | TMR_CLK(TMR0_WCLK) | TMR_MODE(TMR_MODE_CNT);
    Timer0_IE(1);
}

