#include "includes.h"
#include "boost.h"
#include "vout_inserdet.h"
#include "app_config.h"
#include "app_var.h"
#include "adc.h"
#include "timer.h"
#include "chargebox.h"
#include "app_close.h"
#include "app_common.h"
#include "ui.h"
#include "extern_ntc_det.h"

static void app_close_evt_512ms(void) 
{
    log_putchar('H');
#if TCFG_HALL_ENABLE
	if (hall_status) {
		app_auto_sleep_reset();
		return;
	}
#endif 
	app_auto_sleep();
}

void app_close_deal_ear_online(u8 chl)
{
    app_auto_sleep_reset();
	if (chl) {
		log_puts("R in\n");
		WKUPx_EN(WKUP_7_VOUT1_DECT, 0);	
	} else {
		log_puts("L in\n");
		WKUPx_EN(WKUP_6_VOUT0_DECT, 0);	
	}
	b_discharge_ear_full = 0;
	ear_info[chl].full_flag	= 0;
	ear_info[chl].charge = 1;
	b_discharge_ear_offline_hw = 0;
	vout_pwr_ctrl(chl, 1);
}

void app_close_deal_ear_offline(u8 chl)
{
    app_auto_sleep_reset();
	ear_info[chl].full_flag	= 1;
	ear_info[chl].charge = 0;
	ear_info[chl].charge_cnt = 0;
	ear_info[chl].charge_times = 0;
	if (!ear_info[chl].online_hw && !ear_info[!chl].online_hw) {
		b_discharge_ear_offline_hw = 1;
	}
	if (ear_info[!chl].full_flag || !ear_info[!chl].online_hw) {
		b_discharge_ear_full = 1;	
	}
    vout_pwr_ctrl(chl, 0);
	if (chl) {
		log_puts("R out\n");
		pmu_wkupx_clear_pending(WKUP_7_VOUT1_DECT);
		WKUPx_INV_EN(WKUP_7_VOUT1_DECT, 0);
		WKUPx_EN(WKUP_7_VOUT1_DECT, 1);
	} else {
		log_puts("L out\n");
		pmu_wkupx_clear_pending(WKUP_6_VOUT0_DECT);
		WKUPx_INV_EN(WKUP_6_VOUT0_DECT, 0);
		WKUPx_EN(WKUP_6_VOUT0_DECT, 1);
	}
}

void app_close_deal_ear_full(u8 chl) 
{
	app_auto_sleep_reset();
	vout_pwr_ctrl(chl, 0);
	ear_info[chl].charge = 0;
	
	//左右耳满电，关boost和vin2vout
	if (ear_info[!chl].full_flag || !ear_info[!chl].online_hw) {
		b_discharge_ear_full = 1;	
	} 	
}

#if TCFG_EXTERN_NTC_ENABLE
void app_close_ntc_discharge_handle(void)
{
	switch (ntc.u.discharge) {
		case NTC_STATUS_DISCHARGE_GOOD:
			log_puts("ntc reg 1\n");
			b_discharge_outside_ntc_err = 0;
			ear_info[EAR_R].online_hw = 0;
			ear_info[EAR_L].online_hw = 0;
			break;
		case NTC_STATUS_DISCHARGE_STOP:
			log_puts("ntc err 1\n");
			b_discharge_outside_ntc_err = 1;
			break;
	}
}
#endif

void app_close(void) 
{
	app_auto_sleep_reset();

	b_discharge_ear_offline_hw = 1;	

    while (1) {
        app_common_event_deal();

        app_event_call_2(b_evt_512ms, app_close_evt_512ms, app_common_evt_512ms);

#if TCFG_EXTERN_NTC_ENABLE
		//ntc放电状态变化处理
		app_event_call(b_evt_ntc_discharge, app_close_ntc_discharge_handle);
#endif

		if (b_evt_ear0_full) {
			b_evt_ear0_full = 0;
			log_puts("L full\n");
			app_close_deal_ear_full(EAR_L);
		}
		if (b_evt_ear1_full) {
			b_evt_ear1_full = 0;
			log_puts("R full\n");
			app_close_deal_ear_full(EAR_R);
		}
	}
}

