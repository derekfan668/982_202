#include "includes.h"
#include "charge.h"
#include "boost.h"
#include "vout_inserdet.h"
#include "app_config.h"
#include "app_var.h"
#include "adc.h"
#include "chargebox.h"
#include "app_close.h"
#include "app_open.h"
#include "app_lowpower.h"
#include "chargebox_comm.h"
#include "app_common.h"
#include "wireless.h"
#include "ui.h"
#include "power_det.h"
#include "extern_ntc_det.h"
#include "inside_ntc_det.h"
#include "ppm.h"
#include "power_optimize.h"
#include "uart_send.h"

void app_auto_mode(void) 
{
    if (hall_status) {
        sys_info.mode = APP_MODE_OPEN;
    } else {
        if (sys_info.bat_low) {
            sys_info.mode = APP_MODE_LOWPOWER;
        } else {
            sys_info.mode = APP_MODE_CLOSE;
        }
    }
}

void app_auto_sleep_reset(void)
{
	sys_info.life_cnt = 0;	
}

void app_auto_sleep(void) 
{
    if (sys_auto_sleep_check() == 0) {
        sys_info.life_cnt++;
        if (sys_info.life_cnt > TCFG_LIFE_CNT_MAX) {
			log_puts("\nauto sleep\n");
            sys_enter_sleep();
            sys_info.life_cnt = 0;
        }
    } else {
        sys_info.life_cnt = 0;
    }
}

//不同模式下耳机上下线事件处理
void app_common_evt_ear_comm_handle(u8 chl, u8 state)
{
	switch (sys_info.mode) {
		case APP_MODE_OPEN:
			auto_exit_cnt = 0;
		break;
		case APP_MODE_CLOSE:
			if (state) {
				app_close_deal_ear_online(chl);	
			} else {
				app_close_deal_ear_offline(chl);
			}
		break;
		case APP_MODE_LOWPOWER:
		break; 
	 }
}

//分发左右耳上下线事件
void app_common_evt_ear_comm(void)
{
	if (b_evt_ear0_com_in) {
    	b_evt_ear0_com_in = 0;
		app_common_evt_ear_comm_handle(EAR_L, 1);
        log_puts("L in\n");
    }
	if (b_evt_ear0_com_out) {
    	b_evt_ear0_com_out = 0;
		app_common_evt_ear_comm_handle(EAR_L, 0);
        log_puts("L out\n");
    }
	if (b_evt_ear1_com_in) {
    	b_evt_ear1_com_in = 0;
		app_common_evt_ear_comm_handle(EAR_R, 1);
        log_puts("R in\n");
    }
	if (b_evt_ear1_com_out) {
    	b_evt_ear1_com_out = 0;
		app_common_evt_ear_comm_handle(EAR_R, 0);
        log_puts("R out\n");
    }	
}
extern bool box_into_boat_mode;
void usb_in_handle(void) 	
{
	log_puts("USB IN\n");
	box_into_boat_mode = 0;
	b_discharge_boat_mode = 0;
#if TCFG_LONG_PRESS_RESET_ENABLE && (LONG_PRESS_PINR_IN_SEL == VPWR_IN_DETECT)
	PINR_LVL_SET_L();
#endif

	b_sys_busy_in_usb_on = 1;
	
#if TCFG_UI_ENABLE
	ui_run_status(UI_STATUS_USB_IN);
#endif

	sys_info.sys_abnormal_cnt = 0;
	sys_info.sys_abnormal = 0;
	b_discharge_vout_ovuv = 0;
	b_discharge_current_limit = 0;

	boost_set_volt(BOOST_VOUT_S_5000_MV);
	boost_lock_volt = 1;

#if TCFG_WPR_ENABLE
	if (wpr_status) {
		//log_puts("\n-wireless charging...-\n");
		charge_set_charge_mA(CHARGE_WIRELESS_CURRENT_MA);
	} else {
		//log_puts("\n-wire charging...-\n");
#if TCFG_HANDSHAKE_ENABLE
		handshake_start();
		handshake_run();		
#endif
		charge_set_charge_mA(CHARGE_CURRENT_MA);
		wpr_disable = 1;
	}
#else
#if TCFG_HANDSHAKE_ENABLE
	handshake_start();
	handshake_run();		
#endif 
	charge_set_charge_mA(CHARGE_CURRENT_MA);
#endif

	b_charge_disable = 0;
	b_charge_localfull = 0;
	app_battery_notice();
	app_battery_notice();
	app_f98_battery_adc_val_notice();
}

void usb_out_handle(void) 
{
	log_puts("USB OUT\n");

	boost_lock_volt = 0;
	boost_set_volt(TCFG_BOOST_VOUT_S);

#if TCFG_LONG_PRESS_RESET_ENABLE && (LONG_PRESS_PINR_IN_SEL == VPWR_IN_DETECT)
	PINR_LVL_SET_H();
#endif

	b_sys_busy_in_usb_on = 0;
	
#if TCFG_UI_ENABLE
	ui_run_status(UI_STATUS_USB_OUT);
#endif

#if TCFG_HANDSHAKE_ENABLE
	handshake_stop();
#endif

#if TCFG_WPR_ENABLE
	wpr_disable = 0;
#endif

	b_charge_disable = 1;
	app_battery_notice();
	app_f98_battery_adc_val_notice();
}

#if (!TCFG_HALL_ENABLE)
static u8 online_hw_cur_sts[2] = {0};
static void app_online_comm_status_change(void)
{
    u8 ear_ch = 0;
    for (ear_ch = 0; ear_ch < 2; ear_ch++){
        if(online_hw_cur_sts[ear_ch] != ear_info[ear_ch].online_hw){
            if(online_hw_cur_sts[ear_ch] == 0){
                app_close_deal_ear_online(ear_ch);
            }else{
                app_close_deal_ear_offline(ear_ch);
            }
        }
    online_hw_cur_sts[ear_ch] = ear_info[ear_ch].online_hw;
    }
}
#endif

void vout_insertdet_run(void)
{
	u8 ear_ch;
#if TCFG_UI_ENABLE
	u8 online_st;
#endif
	for (ear_ch = EAR_L; ear_ch < EAR_MAX; ear_ch++) {
		//负载检测无使能
		if ((!vout_inserdet_en_get(ear_ch)) || ear_info[ear_ch].force_charge) {
			continue;
		}
#if TCFG_UI_ENABLE
		online_st = ear_info[ear_ch].online_hw;
#endif
		//负载检测
		if ((vout_current_mA_get(ear_ch) > 5)) {//>5mA物理在线
			ear_info[ear_ch].online_hw = 1;	
		} else {
			vout_pwr_ctrl(ear_ch, 0);
			ear_info[ear_ch].online_hw = vout_inserdet_state_get(ear_ch);
			//耳机处于充电状态，需要恢复
			if (ear_info[ear_ch].charge && (!discharge_sw_check())) {
				vout_pwr_ctrl(ear_ch, 1);
			}
		}
#if (!TCFG_HALL_ENABLE)
		app_online_comm_status_change();
#endif

#if TCFG_UI_ENABLE
		//在线状态发生变化
		if (online_st != ear_info[ear_ch].online_hw) {
			if (ear_info[ear_ch].online_hw) {
				if (ear_ch == EAR_L) {
					ui_run_status(UI_STATUS_EAR0_IN);
				} else {
					ui_run_status(UI_STATUS_EAR1_IN);
				}
			} else {
				if (ear_ch == EAR_L) {
					ui_run_status(UI_STATUS_EAR0_OUT);
				} else {
					ui_run_status(UI_STATUS_EAR1_OUT);
				}
			}
		}

#if (TCFG_UI_MODE == UI_SEG_LED)
#if (TCFG_SEG_LED_MODE == SEG_LED_MODE1)
		//更新耳机电量显示
		if (ear_ch == EAR_L) {
			ui_run_status(UI_STATUS_UPDATE_EAR_L_POWER);
		} else {
			ui_run_status(UI_STATUS_UPDATE_EAR_R_POWER);
		}
#endif
#endif

#endif
		//物理下线，通信随之下线
#if TCFG_BOOST_OPTIMIZE_ENABLE
		if (!ear_info[ear_ch].online_hw && !boost_optimize.boost_optimize_en) 
#else
		if (!ear_info[ear_ch].online_hw)
#endif
		{
			if (ear_info[ear_ch].online_comm) {
				app_chargebox_api_check_online(ear_ch, 0);
			} else {
				//原通信不在线且非强充阶段，发出耳机满电消息后关机(物理不在线不通信)
				if ((sys_info.mode == APP_MODE_CLOSE) && (!ear_info[ear_ch].force_charge)) {
					app_chargebox_api_check_ear_full(ear_ch);
				}		
			}
		}
	}
}

void app_common_evt_2ms(void) 
{
#if TCFG_WPR_ENABLE
    wireless_scan();
#endif
}

void localfull_det_run(void)
{
#if TCFG_CHARGE_OPTIMIZE_ENABLE
	if (b_sys_busy_in_charge && (!sys_info.charge_cc_state) && get_charge_optimize_end_flag()) //涓流/缓变调节阶段不检测满电
#else
	if (b_sys_busy_in_charge && (!sys_info.charge_cc_state)) //涓流阶段不检测满电
#endif
	{
		if ((vbat_charge_current_mA_get() <= CHARGE_FULL_CURRENT_MA) && (power_get_vbat_percent() == 100)) {
			sys_info.localfull_cnt++;
			if (sys_info.localfull_cnt >= TCFG_LOCAL_FULL_CNT_MAX) {
				log_puts("local full\n");
				sys_info.localfull_cnt = 0;
				b_charge_localfull = 1;
			}
		} else {
			sys_info.localfull_cnt = 0;
			b_charge_localfull = 0;
		} 
	}
}

void recharge_det_run(void)
{
	//满电关闭充电后usb仍在线
	if (sys_info.localcharge && (!b_sys_busy_in_charge)) {
		if (power_get_vbat_percent() <= CHARGE_RECHARGE_PERCENT) {
			log_puts("recharge\n");
			b_charge_localfull = 0;
		} 
	}
}

void discharge_current_limit_det_run(void)
{
	if (ear_info[EAR_L].charge || ear_info[EAR_R].charge) {
		if ((vout_current_mA_get(EAR_L) >= TCFG_CURRENT_LIMIT_MA) || (vout_current_mA_get(EAR_R) >= TCFG_CURRENT_LIMIT_MA)) {
			log_puts("vbat oc\n");
			b_discharge_current_limit = 1;
			sys_info.sys_abnormal = 1;		
		}		
	}
}

#if TCFG_HANDSHAKE_ENABLE
void handshake_run_det_run(void)
{
	//避免lighting充电快速插拔,没有响应usb off信号,不会继续充电问题
	if (b_sys_busy_in_usb_on && usb_shake_flag) {
		handshake_start();
		usb_shake_flag = 0;
	}
#if TCFG_WPR_ENABLE
	//无线充电在线时,不进行握手操作
	if (!wpr_status) {
	  	handshake_run();
	}
#else
	handshake_run();
#endif
}
#endif

void sys_abnormal_exit_det_run(void)
{	
	if (sys_info.sys_abnormal) {
		if (sys_info.sys_abnormal_cnt < TCFG_SYS_ABNORMAL_CNT_MAX) {
			//vout < 5.5V
			if (adc_get_voltage(AD_ANA_CH_VOUT_DIV_4, 1) <= 1375) {
				b_discharge_vout_ovuv = 0;
			}
			
			if (!(b_discharge_vout_ovuv || b_discharge_current_limit)) {
				sys_info.sys_abnormal = 0;
			}		
		} 
#if TCFG_UI_ENABLE
		else {
			ui_run_status(UI_STATUS_SYS_ABNORMAL);	
		}					
#endif		
	}
}

#if TCFG_LONG_PRESS_RESET_ENABLE && (LONG_PRESS_PINR_IN_SEL == VPWR_IN_DETECT)
void vpwr_long_press_reset_check(void)
{
   	if (VIN_INDET_GET()) {
		PINR_LVL_SET_L();
	} else {
	   	PINR_LVL_SET_H();
	}
}
#endif

void charge_discharge_sw_det_run(void)
{
	PMU_IE(0);//关闭PMU的中断，不要动这个中断

	//充电开关检测
	charge_ctrl(!charge_sw_check());

	//放电开关检测
#if TCFG_VIN2VOUT_ENABLE
	//先关后开,顺序不能变！
	if (sys_info.localcharge) {
		boost_ctrl(0);
		vin2vout(!discharge_sw_check());
	} else {
		vin2vout(0);
		boost_ctrl(!discharge_sw_check());
	}
#else 
	boost_ctrl(!discharge_sw_check());
#endif

	WKUPx_EN(WKUP_3_VPWR_OV, 1);

	if (sys_info.sys_abnormal) {
		if (sys_info.sys_abnormal_cnt < TCFG_SYS_ABNORMAL_CNT_MAX) {
			b_discharge_current_limit = 0;
			sys_info.sys_abnormal_cnt++;
		}
		log_puts("SYS ERR\n");
	}

	PMU_IE(1);//开启pmu的中断
}

void key_tws_pair_det(void);
void app_common_evt_512ms(void)
{
	wdt_clear();

	//耳机过流检测
	discharge_current_limit_det_run(); 

	//充放电开关检测
	charge_discharge_sw_det_run();

	//舱充电涓流检测
	if (sys_info.localcharge) {
		charge_cc_check();
	}

	//舱充电满电检测
	localfull_det_run();

	//舱充电回充检测
	recharge_det_run(); 

#if TCFG_CHARGE_OPTIMIZE_ENABLE
	//充电电流缓变调节
	power_optimize_for_charge_current_adjust();
#endif

#if TCFG_LONG_PRESS_RESET_ENABLE && (LONG_PRESS_PINR_IN_SEL == VPWR_IN_DETECT)
	vpwr_long_press_reset_check();
#endif

	//更新VBG
	adc_update_vbg();

	//电池电量检测
	power_det_run();

	//耳机负载检测
	vout_insertdet_run();
	
	//外挂ntc检测
#if TCFG_EXTERN_NTC_ENABLE
    ntc_det_run();
#endif
	//内置NTC检测
#if TCFG_INSIDE_NTC_ENABLE
	ntc_inside_det_run();
#endif

#if TCFG_HANDSHAKE_ENABLE
	handshake_run_det_run();
#endif

#if TCFG_BOOST_OPTIMIZE_ENABLE
	//放电跟随充调节
	power_optimize_for_boost_voltage_adjust();
#endif

	//系统异常退出检查
	sys_abnormal_exit_det_run();

	key_tws_pair_det();
}

#if TCFG_KEY_ENABLE
extern void app_close_deal_ear_full(u8 chl);
void app_common_evt_key(void)
{
	if(b_evt_key_triple){
		log_putchar('s');
		b_evt_pair = 1;
		b_evt_key_triple = 0;

#if TCFG_UI_ENABLE
		ui_run_status(UI_STATTUS_SWITCH_MAC);
#endif
	}

	if (b_evt_key_long) {
		log_putchar('L');
		b_evt_key_long = 0;
		switch (sys_info.mode) {
			case APP_MODE_OPEN:
				auto_exit_cnt = 0;
    			if (sys_info.pair_status == 0) {
       				sys_info.pair_status = 1;
        			goto_pair_cnt = 0;
#if TCFG_UI_ENABLE
					ui_run_status(UI_STATUS_PAIR_PRE);
#endif
				}
				break;
			case APP_MODE_CLOSE:	
				break;
			case APP_MODE_LOWPOWER:
				break; 
	 	}
	}

	if (b_evt_key_hold) {
		log_putchar('D');
		b_evt_key_hold = 0;
		switch (sys_info.mode) {
			case APP_MODE_OPEN:
				auto_exit_cnt = 0;
    			if (sys_info.pair_status == 1) {
        			goto_pair_cnt++;
        			if (goto_pair_cnt >= KEY_PAIR_CNT) {
            			//log_put_u8(sys_info.pair_status);
            			sys_info.pair_status = 2;
            			sys_info.pair_succ = 0;
#if TCFG_UI_ENABLE
						ui_run_status(UI_STATUS_PAIRING);
#endif
					}
    			}
				break;
			case APP_MODE_CLOSE:	
				break;
			case APP_MODE_LOWPOWER:
				break; 
	 	}
	}

	if (b_evt_key_up) {
		log_putchar('U');
		b_evt_key_up = 0;
		switch (sys_info.mode) {
			case APP_MODE_OPEN:
				//log_put_u8(sys_info.pair_status);
    			auto_exit_cnt = 0;
#if TCFG_UI_ENABLE
				if (goto_pair_cnt < KEY_PAIR_CNT) {
					ui_run_status(UI_STATUS_PAIR_STOP);
				}
#endif
    			if (sys_info.pair_status != 2) {
        			sys_info.pair_status = 0;
    			}
				break;
			case APP_MODE_CLOSE:	
				break;
			case APP_MODE_LOWPOWER:
				break; 
	 	}
	}
}
#endif

#if TCFG_EXTERN_NTC_ENABLE
void app_common_ntc_charge_handle(void)
{
	switch (ntc.u.charge) {
		case NTC_STATUS_CHARGE_GOOD:
			log_puts("ntc reg 0\n");
#if (NTC_MODE_SEL == NTC_MODE_JEITA)
#if TCFG_WPR_ENABLE
			if (wpr_status) {
				charge_set_charge_mA(CHARGE_WIRELESS_CURRENT_MA);
			} else {
				charge_set_charge_mA(CHARGE_CURRENT_MA);
			}
#else 
			charge_set_charge_mA(CHARGE_CURRENT_MA);
#endif
			charge_set_charge_volt(CHARGE_CONSTANT_VOLTAGE);
#endif
			b_charge_outside_ntc_err = 0;	
			break;
		case NTC_STATUS_CHARGE_STOP:
			log_puts("ntc err 0\n");
			b_charge_outside_ntc_err = 1;	
			break;
#if (NTC_MODE_SEL == NTC_MODE_JEITA)
		case NTC_STATUS_CHARGE_CURR:
			b_charge_outside_ntc_err = 0;
			charge_set_charge_mA(NTC_CHARGE_CURRENT_MA);
			break;
		case NTC_STATUS_CHARGE_VOLT:
			b_charge_outside_ntc_err = 0;
			charge_set_charge_volt(NTC_CHARGE_CONSTANT_VOLTAGE);
			break;
#endif
	}
}
#endif

void app_common_event_deal(void) 
{
    app_event_call(b_evt_hall, app_auto_mode);
	app_event_call(b_evt_2ms, app_common_evt_2ms);

#if TCFG_WPR_ENABLE
    app_event_call(b_evt_wireless_on, wireless_api_open);
    app_event_call(b_evt_wireless_off, wireless_api_close);
    app_event_call(b_evt_wpr_128ms, wireless_api_128ms_run);
#endif

	app_event_call(b_evt_usb_in, usb_in_handle);
	app_event_call(b_evt_usb_out, usb_out_handle);

#if TCFG_EXTERN_NTC_ENABLE
	//充电状态变化处理
	app_event_call(b_evt_ntc_charge, app_common_ntc_charge_handle);
#endif

	//耳机通信上下线处理
	app_common_evt_ear_comm();

#if TCFG_KEY_ENABLE
	app_common_evt_key();
#endif
	uart1_rx_packet_handle();
}

//三级按键配对处理函数 --2.28
extern void app_close_deal_ear_full(u8 chl);
void key_tws_pair_det(void)
{
	u8 chl = 0;
	if(b_evt_pair) {
		if(box_into_boat_mode){
        	return;
		}
#if TCFG_HALL_ENABLE
		if(!hall_status) {
			sys_info.key_evt_pair_cnt++;
			app_chargebox_api_send_power_close(0);
			app_chargebox_api_send_power_close(1);
			if(sys_info.key_evt_pair_cnt > 5) {
				b_evt_pair = 0;
				app_chargebox_api_send_pair();
				chargebox_api_send_tws_mac();
				sys_info.key_evt_pair_cnt = 0;
			}
		} else {
			b_evt_pair = 0;
			app_chargebox_api_send_pair();
			chargebox_api_send_tws_mac();
			sys_info.key_evt_pair_cnt = 0;
		}
#endif

		for(chl = 0; chl < 2; chl++) {
			if (ear_info[chl].charge == 0) {
				app_close_deal_ear_full(chl);
#if TCFG_HALL_ENABLE
				vout_inserdet_en(chl, 0);
#endif
			}
		}
	}
}
