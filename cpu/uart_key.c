#include "includes.h"
#include "timer.h"
#include "uart_key.h"
#include "power.h"
#include "app_config.h"

#if TCFG_UPDATE_ENABLE

#define TCFG_UPDATE_GROUP					1
#define TCFG_UPDATE_PORT					7

//用RCH时钟32M(16M~48M),div16,2M(1M~3M)
#define CAP_CLK          	HRC_CLK_DEF
#define TMR_DIV           	16UL
#define CNT_500US           (CAP_CLK / (TMR_DIV * 1000 * 2))	//1000
#define CNT_250US           (CAP_CLK / (TMR_DIV * 1000 * 2 * 2))//500
#define CNT_VALUE           CNT_500US      //理论上一个周期的计数时间
#define CNT_THRESHOLD       CNT_250US      //百分之五十的误差值
#define CYCLE_MAX			(CNT_VALUE + CNT_THRESHOLD)//1500
#define CYCLE_MIN			(CNT_VALUE - CNT_THRESHOLD)//500

static u16 data last_prd;
static volatile u16 data uart_rx_key;

void timer1_isr(void) interrupt TIMER1_INT using 1
{
	u16 cycle, cur_prd;
	TMR1_CNTH = 0;
	TMR1_CNTL = 0;
	TMR1_CON0 |= TMR_CPND;
	cur_prd = (TMR1_PRH << 8) | TMR1_PRL;

	if ((TMR1_CON0 & BIT(0)) == 0) {
		TMR1_CON0 |= BIT(0);//捕获到上升沿切换成下降沿

		if (last_prd == 0) {
			last_prd = cur_prd;
			return;
		}

		cycle = cur_prd + last_prd;
		if ((cycle > CYCLE_MAX) || (cycle < CYCLE_MIN)) {
			last_prd = 0;
			return;
		}

		if (last_prd > cur_prd) {
			if ((last_prd > (3 * cur_prd)) && (last_prd < (5 * cur_prd))) {
            	//正占空比4:1-----逻辑1
            	uart_rx_key = (uart_rx_key << 1) | 1;
			} else {
				last_prd = 0;
				return;
			}
		} else {
		    if (((last_prd * 3) < cur_prd) && ((last_prd * 5) > cur_prd)) {
				//正占空比1:4-----逻辑0
				uart_rx_key = (uart_rx_key << 1);
			} else {
				last_prd = 0;
				return;
			}
		}

		if (0x68af == uart_rx_key) {
		    CPU_RESET();
            return;
        }
	} else {
		TMR1_CON0 &= ~BIT(0);//捕获到下降沿切换为上升沿
	}
	last_prd = cur_prd;
	return;
}


void uart_key_init(void)
{
	gpio_set_pull_up(TCFG_UPDATE_GROUP, TCFG_UPDATE_PORT, 0);
	gpio_set_pull_down(TCFG_UPDATE_GROUP, TCFG_UPDATE_PORT, 0);
	gpio_set_die(TCFG_UPDATE_GROUP, TCFG_UPDATE_PORT, 1);
	gpio_set_dir(TCFG_UPDATE_GROUP, TCFG_UPDATE_PORT, 1);
	TMR1_CAP_IOS_INPUT_CH1();
	IN_CH1_SEL(TCFG_UPDATE_GROUP * 8 + TCFG_UPDATE_PORT);
	TMR1_CON0 = TMR_CPND;
	TMR1_CNTH = 0;
	TMR1_CNTL = 0;
	TMR1_CON0 = TMR_DIV0(TMR_DIV_16) | TMR_CLK(TMR1_RCH_CLK) | TMR_MODE(TMR_MODE_RISE_CAP);
	Timer1_IE(1);
	Timer1_IP(1);
}

void uart_key_uninit(void)
{
	TMR1_CON0 = TMR_CPND;
	Timer1_IE(0);
	TMR1_CAP_IOS_P04();
	IN_CH1_SEL(0);
	gpio_set_pull_up(TCFG_UPDATE_GROUP, TCFG_UPDATE_PORT, 0);
	gpio_set_pull_down(TCFG_UPDATE_GROUP, TCFG_UPDATE_PORT, 0);
	gpio_set_die(TCFG_UPDATE_GROUP, TCFG_UPDATE_PORT, 0);
	gpio_set_dir(TCFG_UPDATE_GROUP, TCFG_UPDATE_PORT, 1);
}

#endif
