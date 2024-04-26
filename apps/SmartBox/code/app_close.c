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
#include "chargebox_comm.h"
#include "ui.h"
#include "extern_ntc_det.h"
#include "uart_send.h"

void close_app_lid_close_deal(u8 chl)
{
    if (ear_info[chl].lid_cnt) {
        ear_info[chl].lid_cnt--;
        app_chargebox_api_send_close_lid(chl);
        if (ear_info[chl].lid_cnt == 0) {
            app_chargebox_api_send_power_close(chl);
            ear_info[chl].charge = 1;
			if (!discharge_sw_check()) {
				vout_pwr_ctrl(chl, 1);
			}
			//物理在线，通信不在线，以及开charge判断为耳机无电状态 强制充电1min，防止耳机LVD退出充电
			app_battery_notice();
			if ((ear_info[chl].online_hw) && (!ear_info[chl].online_comm) && (!discharge_sw_check())) {  
				ear_info[chl].force_charge = CHGBOX_FORCE_CHARGE_TIMES;	
			} 
        }
    }
}

void close_app_shut_down_deal(u8 chl)
{
    if (ear_info[chl].shut_cnt) {
        ear_info[chl].shut_cnt--;
        app_chargebox_api_send_shutdown(chl);
        if (ear_info[chl].shut_cnt == 0) {
#if TCFG_HALL_ENABLE
			vout_inserdet_en(chl, 0);
#else
			if (chl == EAR_L) {
				gpio_set_pull_up(EAR_GROUP_L, EAR_PORT_L, 0);
			} else {
				gpio_set_pull_up(EAR_GROUP_R, EAR_PORT_R, 0);
			}
#endif
        }
    }
}

void close_app_send_power(u8 chl) 
{
    if ((!ear_info[chl].shut_cnt) && \
	    (!ear_info[chl].lid_cnt) && \
		(!ear_info[chl].full_flag) && \
		(!ear_info[chl].force_charge)) {
        app_chargebox_api_send_power_close(chl);
		//物理在线，通信不在线，以及开charge判断为耳机无电状态 强制充电1min，防止耳机LVD退出充电
		if ((ear_info[chl].online_hw) && (!ear_info[chl].online_comm) && (!discharge_sw_check())) {  
			ear_info[chl].force_charge = CHGBOX_FORCE_CHARGE_TIMES;	
		}
    }
}

static void app_close_var_init(void) 
{
    ear_info[EAR_L].shut_cnt = 0;
    ear_info[EAR_R].shut_cnt = 0;
    ear_info[EAR_L].lid_cnt = 0;
    ear_info[EAR_R].lid_cnt = 0;
    ear_info[EAR_L].charge = 0;
    ear_info[EAR_R].charge = 0;
	ear_info[EAR_L].force_charge = 0;
    ear_info[EAR_R].force_charge = 0;
	ear_info[EAR_L].charge_cnt = 0;
	ear_info[EAR_L].charge_times = 0;
	ear_info[EAR_R].charge_cnt = 0;
	ear_info[EAR_R].charge_times = 0;
}

static void app_close_evt_256ms(void) 
{
    close_app_lid_close_deal(EAR_L);
    close_app_lid_close_deal(EAR_R);
    close_app_shut_down_deal(EAR_L);
    close_app_shut_down_deal(EAR_R);
}

static void app_close_evt_512ms(void) 
{
	u8 chl;
    log_putchar('H');
	for (chl = 0; chl < EAR_MAX; chl++) {
        if (ear_info[chl].force_charge) {
            ear_info[chl].force_charge--;
        }
		//耳机低电拔出结束强充
		if(!ear_info[chl].online_comm && (vout_current_mA_get(chl) == 0) && ear_info[chl].force_charge) {
			ear_info[chl].force_charge = 0;
		}
    }
    app_auto_sleep();
}

static void app_close_evt_8s(void) 
{
    close_app_send_power(EAR_L);
    close_app_send_power(EAR_R);
}

void app_close_deal_ear_online(u8 chl)
{
    app_auto_sleep_reset();
	vout_inserdet_en(chl, 1);
	ear_info[chl].online_hw = vout_inserdet_state_get(chl);
    ear_info[chl].charge = 0;
    ear_info[chl].full_flag = 0;
    ear_info[chl].full_cnt = 0;
    ear_info[chl].shut_cnt = 0;
    ear_info[chl].lid_cnt = TCFG_SEND_CLOSE_LID_MAX;
	b_discharge_ear_full = 0;
}

void app_close_deal_ear_offline(u8 chl)
{
    app_auto_sleep_reset();
   	ear_info[chl].shut_cnt = 0;
    ear_info[chl].lid_cnt = 0;
    ear_info[chl].power = 0xff;
    ear_info[chl].full_flag = 0;
    ear_info[chl].full_cnt = 0;
	ear_info[chl].charge = 0;
    ear_info[chl].force_charge = 0;
	b_discharge_ear_full = 0;
    vout_pwr_ctrl(chl, 0);
	app_battery_notice();
}

void app_close_deal_ear_full(u8 chl) 
{
	app_auto_sleep_reset();
	vout_pwr_ctrl(chl, 0);
	ear_info[chl].shut_cnt = TCFG_SEND_SHUT_DOWN_MAX;
	ear_info[chl].lid_cnt = 0;
	ear_info[chl].charge = 0;
	ear_info[chl].force_charge = 0;
	
	//左右耳满电，关boost和vin2vout
	if (ear_info[chl].full_flag && ear_info[!chl].full_flag) {
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
			app_close_deal_ear_online(EAR_L);
			app_close_deal_ear_online(EAR_R);
			break;
		case NTC_STATUS_DISCHARGE_STOP:
			log_puts("ntc err 1\n");
			b_discharge_outside_ntc_err = 1;
			break;
	}
}
#endif

void discharge_overtime_check(void)
{
	u8 ear_ch;
	for (ear_ch = 0; ear_ch < EAR_MAX; ear_ch++) {
		if (ear_info[ear_ch].charge) {
			ear_info[ear_ch].charge_cnt++;
			if (ear_info[ear_ch].charge_cnt == 225) {//225*8s=30mins
				ear_info[ear_ch].charge_cnt = 0;
				ear_info[ear_ch].charge_times++;
				if (ear_info[ear_ch].charge_times >= TCFG_DISCHARGE_OVERTIME) {//TCFG_DISCHARGE_OVERTIME*30mins
					ear_info[ear_ch].charge_times = 0;
					ear_info[ear_ch].full_cnt = 0;
					ear_info[ear_ch].full_flag = 1;
					if (ear_ch) {
						b_evt_ear1_full = 1;
					} else {
						b_evt_ear0_full = 1;	
					}	
				}	
			}
		} else {
			ear_info[ear_ch].charge_cnt = 0;
			ear_info[ear_ch].charge_times = 0;
		}
	}
}

void app_close(void) 
{
    log_puts("close\n");

	app_auto_sleep_reset();

	b_discharge_disable = 0;

    //8s的事件复位
    timer_8s_cnt = 0;
    b_evt_8s = 0;

    app_close_var_init();

	//盒盖初始化
	app_close_deal_ear_online(EAR_L);
	app_close_deal_ear_online(EAR_R);	
#if OPEN_CLOSE_HALL_NOTIGY	
	if(hall_status){

	}else{
		app_hall_notice(2);
	}	
#endif
    while (1) {
        app_common_event_deal();

        app_event_call(b_evt_256ms, app_close_evt_256ms);

        app_event_call_2(b_evt_512ms, app_close_evt_512ms, app_common_evt_512ms);

        app_event_call_2(b_evt_8s, app_close_evt_8s, discharge_overtime_check);

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

        if (sys_info.mode != APP_MODE_CLOSE) {
            goto __app_exit;
        }
    }

__app_exit:
    log_puts("exit\n");
    app_close_var_init();
#if TCFG_BOOST_OPTIMIZE_ENABLE
	boost_set_volt(TCFG_BOOST_VOUT_S);
#endif
}

