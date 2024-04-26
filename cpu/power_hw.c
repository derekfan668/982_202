#include "includes.h"
#include "app_common.h"
#include "chargebox.h"
#include "vout_inserdet.h"
#include "pulse_cnt.h"
#include "power_det.h"
#include "uart_key.h"
#include "adc.h"
#include "app_var.h"
#include "wake_up.h"
#include "ui.h"

extern idata volatile u8 g_mux_buf[12];
 
void power_set_soft_poweroff(void)
{
	//关机前关闭模块
	wdt_close();
	adc_uninit();
	BOOST_BIAS_EN(0);
#if TCFG_AUTO_WAKEUP_ENABLE
	//电量不为0且无异常才打开定时唤醒
	if (power_get_vbat_percent() && (sys_info.sys_abnormal_cnt < TCFG_SYS_ABNORMAL_CNT_MAX)) {
		sleep_auto_wkup_ctrl(1);
	}
#endif
#if TCFG_UPDATE_ENABLE
	uart_key_uninit();
#endif
#if TCFG_EXTERN_NTC_ENABLE 
#if (TCFG_EXTERN_NTC_TYPE == TCFG_EXTERN_NTC_100K)
	NTC_PU_EN(0);
#else
	gpio_set_pull_up(NTC_GROUP, NTC_PORT, 0);
#endif
#endif
#if TCFG_UI_ENABLE
	ui_uninit();
#endif
	if (sys_info.sys_abnormal_cnt >= TCFG_SYS_ABNORMAL_CNT_MAX)	{
		vout_inserdet_en(EAR_L, 0);
		vout_inserdet_en(EAR_R, 0);
	}
#ifdef TCFG_NORMALBOX_CASE
	else {
		pmu_wkupx_clear_pending(WKUP_6_VOUT0_DECT);
		pmu_wkupx_clear_pending(WKUP_7_VOUT1_DECT);
		WKUPx_INV_EN(WKUP_6_VOUT0_DECT, ear_info[EAR_L].online_hw);
		WKUPx_INV_EN(WKUP_7_VOUT1_DECT, ear_info[EAR_R].online_hw);
		WKUPx_EN(WKUP_6_VOUT0_DECT, 1);
		WKUPx_EN(WKUP_7_VOUT1_DECT, 1);
	}
#else
#if (!TCFG_HALL_ENABLE)
	else {
		gpio_set_pull_up(EAR_GROUP_R, EAR_PORT_R, 0);
		gpio_set_pull_up(EAR_GROUP_L, EAR_PORT_L, 0);
		VOUT1_INDET_EN(1);
		VOUT0_INDET_EN(1);
		pmu_wkupx_clear_pending(WKUP_6_VOUT0_DECT);
		pmu_wkupx_clear_pending(WKUP_7_VOUT1_DECT);
		WKUPx_INV_EN(WKUP_6_VOUT0_DECT, ear_info[EAR_L].online_hw);
		WKUPx_INV_EN(WKUP_7_VOUT1_DECT, ear_info[EAR_R].online_hw);
		WKUPx_EN(WKUP_6_VOUT0_DECT, 1);
		WKUPx_EN(WKUP_7_VOUT1_DECT, 1);
	}
#endif
#endif

	local_irq_disable();
	//关闭LVD
	LVD_OE_EN(0);
	LVD_EN(0);
	g_mux_buf[6] = P0_DIE;
  	g_mux_buf[7] = P1_DIE;
  	g_mux_buf[8] = P2_DIE;
	g_mux_buf[9] = P0_DIR;
  	g_mux_buf[10] = P1_DIR;
  	g_mux_buf[11] = P2_DIR;
	P0_DIE = 0;
	P1_DIE = 0;
	P0_DIR = 0xff;
	P1_DIR = 0xff;
#if TCFG_HALL_ENABLE || TCFG_KEY_ENABLE
	io_wakeup_en();
#endif
  	g_mux_buf[0] = 0x43;
  	g_mux_buf[1] = 0xd9;
  	g_mux_buf[2] = 0x04;
  	g_mux_buf[3] = 0x22;
  	g_mux_buf[4] = 0x00;
  	g_mux_buf[5] = 0x00;
#ifdef TCFG_USE_FLASH_DEBUG
	P2_DIE &= ~(BIT(4) | BIT(5));
	P2_DIR |=  (BIT(4) | BIT(5));
	((void (code *)())0x2000)();
#else
	P2_DIE = 0;
	P2_DIR = 0xff;
	((void (code *)())0x3f40)();
#endif

	//开机前恢复模块使能
	P0_DIE = g_mux_buf[6];
  	P1_DIE = g_mux_buf[7];
  	P2_DIE = g_mux_buf[8];
	P0_DIR = g_mux_buf[9];
  	P1_DIR = g_mux_buf[10];
  	P2_DIR = g_mux_buf[11];
	LVD_EN(1);
	LVD_OE_EN(1);
#ifdef TCFG_NORMALBOX_CASE
	WKUPx_EN(WKUP_6_VOUT0_DECT, 0);
	WKUPx_EN(WKUP_7_VOUT1_DECT, 0);	
#else
#if (!TCFG_HALL_ENABLE)
	WKUPx_EN(WKUP_6_VOUT0_DECT, 0);
	WKUPx_EN(WKUP_7_VOUT1_DECT, 0);
#endif	
#endif
	local_irq_enable();
	log_puts("\nwkup\n");
#if TCFG_UI_ENABLE
	ui_init();
#endif
	BOOST_BIAS_EN(1);//开机开启boost偏置电压
#if TCFG_UPDATE_ENABLE
	uart_key_init();
#endif
	adc_init();
	wdt_init(WDT_4S);
#if TCFG_AUTO_WAKEUP_ENABLE
	if (is_sleep_auto_wkup()) {
#ifndef TCFG_NORMALBOX_CASE
#if TCFG_HALL_ENABLE
	   	//定时唤醒后,强制切换到开盖模式,重新计算电量百分比
		hall_status = 1;
    	b_evt_hall = 1;
#else
		//定时唤醒后相当于按键升压给耳机充电
		b_evt_key_click = 1;
#endif
#else
		//定时唤醒后相当于按键升压给耳机充电
		b_evt_key_click = 1;
#endif
		power_det_init();
	}
	sleep_auto_wkup_ctrl(0);
#else
	//重新计算电量百分比
	power_det_init();
#endif
#if TCFG_EXTERN_NTC_ENABLE
#if (TCFG_EXTERN_NTC_TYPE == TCFG_EXTERN_NTC_100K)
	NTC_PU_EN(1);
#else
	gpio_set_pull_up(NTC_GROUP, NTC_PORT, 1);
#endif
#endif
	app_auto_mode();
}

#if TCFG_LONG_PRESS_RESET_ENABLE
void long_press_reset_init(void)
{
	PINR_IN_SEL_SET(LONG_PRESS_PINR_IN_SEL);
	PINR_TSEL(LONG_PRESS_TIME);
#if (LONG_PRESS_PINR_LEVEL == LOW_VAIL)
	PINR_LVL_SET_L();
#else
	PINR_LVL_SET_H();
#endif
	if (LONG_PRESS_RESET_IO != 0xFF) {
		IN_CH0_SEL(LONG_PRESS_RESET_IO);
	}
	PINR_ENABLE(); 
}
#endif

#if TCFG_DEBUG_ENABLE
void reset_source_dump(void)
{
	log_puts("\nF98 RST_SRC : 0x");
	log_put_u8(RST_SRC);
	log_putchar('\n');

    if (IS_POWER_ON_RESET()) {
        log_puts("POWER_ON\n");
    }

    if (IS_LVD_RESET()) {
        log_puts("LVD\n");
    }
    
    if (IS_MCLR_RESET()) {
        log_puts("MCLR\n");
    }
    
    if (IS_PIN_RESET()) {
        log_puts("PIN\n");
    }
	
    if (IS_WDT_RESET()) {
        log_puts("WDT\n");
    }
    
    if (IS_SOFT_RESET()) {
        log_puts("SOFT\n");
    }

	if (GET_POR_FLAG()) {
		CLR_POR_FLAG();
	}
}
#endif




