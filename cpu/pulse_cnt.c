#include "includes.h"
#include "app_config.h"
#include "pulse_cnt.h"
#include "app_var.h"

#if TCFG_AUTO_WAKEUP_ENABLE

#define PCNT_EN 		BIT(0)
#define PCNT_EDGE		BIT(1) //0:下降沿 1:上升沿
#define PCNT_CLK_SEL	BIT(2) //0:lrc_clk 1:wclk(32K)
#define PCNT_WKUP_EN	BIT(3) 
#define PCNT_WE_PLS		BIT(4) 
#define PCNT_RD_PLS		BIT(5)
#define PCNT_CPND		BIT(6)
#define PCNT_PND		BIT(7)

#define pcnt_cnt g_mux_buf

void set_softoff_wakeup_time_sec(void)
{
	u8 i;
#if TCFG_AUTO_WAKEUP_TIME_SEC
	//精准定时秒数使用
	u32 bsel_cnt_0_31;
	u16 bsel0_31_sec;
	bsel0_31_sec = 0xffffffff / otp_info.lrc_clk;
	if (TCFG_AUTO_WAKEUP_TIME_SEC > bsel0_31_sec) {
		pcnt_cnt[4] = ~(TCFG_AUTO_WAKEUP_TIME_SEC / bsel0_31_sec);
		bsel_cnt_0_31 = ~(TCFG_AUTO_WAKEUP_TIME_SEC % bsel0_31_sec);	 
	} else if (TCFG_AUTO_WAKEUP_TIME_SEC == bsel0_31_sec) {
		pcnt_cnt[4] = ~(0x00);
		bsel_cnt_0_31 = ~(0xffffffff);
	} else if (TCFG_AUTO_WAKEUP_TIME_SEC < bsel0_31_sec) {
		pcnt_cnt[4] = ~(0x00);
		bsel_cnt_0_31 = ~(TCFG_AUTO_WAKEUP_TIME_SEC * otp_info.lrc_clk);
	}
	
	pcnt_cnt[3] = bsel_cnt_0_31>>24;
	pcnt_cnt[2] = bsel_cnt_0_31>>16;
	pcnt_cnt[1] = bsel_cnt_0_31>>8;
	pcnt_cnt[0] = bsel_cnt_0_31;
#else
	//以下数据使用LRC=200K直接计算得出
	//pcnt_cnt = 0xffffffffff-((60*60*24*days)*200000UL)
#if (TCFG_AUTO_WAKEUP_TIME_DAY == WAKEUP_TIME_5_DAYS)
	//432000s 5天 EB FF FF F6 27
	pcnt_cnt[4] = 0xEB;
	pcnt_cnt[3] = 0xFF;
	pcnt_cnt[2] = 0xFF;
	pcnt_cnt[1] = 0xF6;
	pcnt_cnt[0] = 0x27;
#elif (TCFG_AUTO_WAKEUP_TIME_DAY == WAKEUP_TIME_10_DAYS)
	//864000s 10天 D7 FF FF EC 4F
	pcnt_cnt[4] = 0xD7;
	pcnt_cnt[3] = 0xFF;
	pcnt_cnt[2] = 0xFF;
	pcnt_cnt[1] = 0xEC;
	pcnt_cnt[0] = 0x4F;
#elif (TCFG_AUTO_WAKEUP_TIME_DAY == WAKEUP_TIME_15_DAYS)
	//1296000s 15天 C3 FF FF E2 77 
	pcnt_cnt[4] = 0xC3;
	pcnt_cnt[3] = 0xFF;
	pcnt_cnt[2] = 0xFF;
	pcnt_cnt[1] = 0xE2;
	pcnt_cnt[0] = 0x77;
#endif
#endif

	for(i = BSEL_BIT_0_7; i <= BSEL_BIT_32_39; i++) {
		PCNT_DAT = pcnt_cnt[i];
		PCNT_BSEL = i;
		PCNT_CON |= (PCNT_CPND | PCNT_WE_PLS);
	}
}

void sleep_auto_wkup_ctrl(u8 en)
{
	if (en) {
		set_softoff_wakeup_time_sec();
		PCNT_CON |= (PCNT_EN | PCNT_WKUP_EN);
		RC200K_EN(1);	
	} else {
		PCNT_CON &= ~(PCNT_EN | PCNT_WKUP_EN);
		RC200K_EN(0);
	}
}

void sleep_auto_wkup_init(void)
{
	PCNT_CON = (PCNT_EDGE | PCNT_CPND); //使用lrc_clk
	RC200K_KEEP_EN(1);		
}

u8 is_sleep_auto_wkup(void)
{
	if ((PCNT_CON & (PCNT_EN | PCNT_WKUP_EN)) == (PCNT_EN | PCNT_WKUP_EN)) {
		if (PCNT_CON & PCNT_PND) {
			PCNT_CON |= PCNT_CPND;
			return 1;
		}
	}
	return 0;
}

#endif


