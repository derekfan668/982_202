#include "includes.h"
#include "app_config.h"
#include "app_var.h"
#include "adc.h"
#include "chargebox.h"
#include "app_lowpower.h"
#include "app_common.h"
#include "chargebox_comm.h"
#include "ui.h"
#include "vout_inserdet.h"
#include "extern_ntc_det.h"
#include "uart_send.h"

void lowpower_app_lid_close_deal(u8 chl)
{ 
    if (ear_info[chl].lid_cnt) { 
        ear_info[chl].lid_cnt--; 
        app_chargebox_api_send_close_lid(chl);
        if (ear_info[chl].lid_cnt == 0) { 
            ear_info[chl].shut_cnt = TCFG_SEND_SHUT_DOWN_MAX; 
        } 
    }
}

void lowpower_app_shut_down_deal(u8 chl) 
{
    if (ear_info[chl].shut_cnt) { 
        ear_info[chl].shut_cnt--; 
        app_chargebox_api_send_shutdown(chl); 
        if (ear_info[chl].shut_cnt == 0) { 
			ear_info[chl].charge = 0; 
#if TCFG_HALL_ENABLE
			vout_inserdet_en(chl, 0);
#else
			if (chl == EAR_L) {
				gpio_set_pull_up(EAR_GROUP_L, EAR_PORT_L, 0);
			} else {
				gpio_set_pull_up(EAR_GROUP_R, EAR_PORT_R, 0);
			}
#endif
            b_sys_busy_in_comm = 0; 
        } 
    }
}

static void app_lowpower_evt_256ms(void) 
{
    lowpower_app_lid_close_deal(EAR_L);
    lowpower_app_lid_close_deal(EAR_R);
    lowpower_app_shut_down_deal(EAR_L);
    lowpower_app_shut_down_deal(EAR_R);
}

static void app_lowpower_evt_512ms(void) 
{
    log_putchar('H');
#if TCFG_HALL_ENABLE
	if (hall_status) {
		app_auto_sleep_reset();//¿ª¸Çºó²»ÐÝÃß
		return;
	}
#endif
    app_auto_sleep();
}

void app_lowpower(void) 
{
    log_puts("lowpower\n");

	app_auto_sleep_reset();

	b_discharge_disable = 1;

	vout_inserdet_en(EAR_L, 1);
	vout_inserdet_en(EAR_R, 1);

    ear_info[EAR_L].shut_cnt = 0;
    ear_info[EAR_R].shut_cnt = 0;

    ear_info[EAR_L].lid_cnt = TCFG_SEND_CLOSE_LID_MAX;
    ear_info[EAR_R].lid_cnt = TCFG_SEND_CLOSE_LID_MAX;
    b_sys_busy_in_comm = 1;
    if(hall_status){
		app_hall_notice(1);
	}else{
		app_hall_notice(2);
	}
    while (1) {
        app_common_event_deal();

		app_event_call_2(b_evt_512ms, app_lowpower_evt_512ms, app_common_evt_512ms);

        app_event_call(b_evt_256ms, app_lowpower_evt_256ms);

        if (sys_info.mode != APP_MODE_LOWPOWER) {
            goto __app_exit;
        }
    }
__app_exit:
    log_puts("exit\n");
    b_sys_busy_in_comm = 0;
	ear_info[EAR_L].shut_cnt = 0;
	ear_info[EAR_R].shut_cnt = 0;
	ear_info[EAR_L].lid_cnt	= 0;
	ear_info[EAR_R].lid_cnt	= 0;
	ear_info[EAR_L].full_flag = 0;
	ear_info[EAR_R].full_flag = 0;
	b_discharge_ear_full = 0;
    return;
}

