#include "includes.h"
#include "app_config.h"
#include "app_var.h"
#include "adc.h"
#include "chargebox.h"
#include "app_open.h"
#include "chargebox_comm.h"
#include "app_common.h"
#include "extern_ntc_det.h"
#include "ppm.h"
#include "ui.h"
#include "vout_inserdet.h"
#include "uart_send.h"

data volatile u8 goto_pair_cnt;//配对功能按键时间计数

#define COMM_LIFE_MAX   (60*2)//一分钟超时
data volatile u8 auto_exit_cnt;//退出开盖通信模式计数

void open_app_pair_continue(void) 
{
    if (sys_info.pair_status == 2) {
        app_chargebox_api_send_pair();
        if (sys_info.pair_succ) {
            sys_info.pair_status = 0;
#if TCFG_UI_ENABLE
			ui_run_status(UI_STATUS_PAIR_STOP);
#endif
        }
    }
}

void app_open_auto_exit(void) 
{
    if ((auto_exit_cnt++) > COMM_LIFE_MAX) {
        auto_exit_cnt = 0;
        if (sys_info.bat_low) {
            sys_info.mode = APP_MODE_LOWPOWER;
        } else {
            // sys_info.mode = APP_MODE_CLOSE;
        }
    }
}

void app_open_evt_256ms(void) 
{
    app_chargebox_api_send_power_open(EAR_L);
    app_chargebox_api_send_power_open(EAR_R);
}

void app_open_evt_512ms(void) 
{
    log_putchar('H');
    open_app_pair_continue();
    app_open_auto_exit();
}

void app_open(void) 
{
    log_puts("open\n");

	app_auto_sleep_reset();

	b_discharge_disable = 1;

	vout_inserdet_en(EAR_L, 1);
	vout_inserdet_en(EAR_R, 1);
	
    sys_info.pair_succ = 0;
    goto_pair_cnt = 0;
    auto_exit_cnt = 0;
#if OPEN_CLOSE_HALL_NOTIGY
	if(hall_status){
 		app_hall_notice(1);
	}
#endif
    while (1) {
        app_common_event_deal();

        app_event_call(b_evt_256ms, app_open_evt_256ms);

		app_event_call_2(b_evt_512ms, app_open_evt_512ms, app_common_evt_512ms);																																								  

        if (sys_info.mode != APP_MODE_OPEN) {
            goto __app_exit;
        }
    }
__app_exit:
#if TCFG_UI_ENABLE
	//开盖超时未配对完成
	 if (sys_info.pair_status) {
	 	sys_info.pair_status = 0;
	 	ui_run_status(UI_STATUS_ALL_OFF);
	 }
#else
	 sys_info.pair_status = 0;
#endif
	ear_info[EAR_L].full_flag = 0;
	ear_info[EAR_R].full_flag = 0;
	b_discharge_ear_full = 0;
    log_puts("exit\n");
}

