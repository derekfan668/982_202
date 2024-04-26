#include "includes.h"
#include "charge.h"
#include "boost.h"
#include "vout_inserdet.h"
#include "app_config.h"
#include "app_var.h"
#include "adc.h"
#include "chargebox.h"
#include "app_close.h"
#include "app_common.h"
#include "wireless.h"
#include "ui.h"
#include "power_det.h"
#include "extern_ntc_det.h"
#include "inside_ntc_det.h"
#include "ppm.h"
#include "power_optimize.h"

void app_auto_mode(void) 
{   
#if TCFG_HALL_ENABLE
	b_discharge_disable = hall_status;
	if (hall_status) {
		if (ear_info[EAR_L].online_hw) {
			ear_info[EAR_L].online_hw = 0;
			app_close_deal_ear_offline(EAR_L);
		}
		if (ear_info[EAR_R].online_hw) {
			ear_info[EAR_R].online_hw = 0;
			app_close_deal_ear_offline(EAR_R);
		}
		log_puts("\nopen\n");
	} else {
		log_puts("\nclose\n");
	}
	vout_inserdet_en(EAR_L, !hall_status);
	vout_inserdet_en(EAR_R, !hall_status);
#else
	b_discharge_disable = 0;
	vout_inserdet_en(EAR_L, 1);
	vout_inserdet_en(EAR_R, 1);
#endif
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

void usb_in_handle(void) 
{
	log_puts("USB IN\n");

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
}

void vout_insertdet_run(void)
{
	u8 ear_ch;
	u8 online_st;
	for (ear_ch = EAR_L; ear_ch < EAR_MAX; ear_ch++) {
		if (!vout_inserdet_en_get(ear_ch)) {
			continue;
		}
		online_st = ear_info[ear_ch].online_hw;
		
		//���ؼ��
		if (ear_info[ear_ch].charge && (b_sys_busy_in_boost || b_sys_busy_in_vin2vout)) {
			ear_info[ear_ch].det_flt++;
			if (ear_info[ear_ch].det_flt < TCFG_DET_FLT_CNT) {
				if (vout_current_mA_get(ear_ch)) {
					ear_info[ear_ch].det_flt = TCFG_DET_FLT_CNT;
				} 
				ear_info[ear_ch].online_hw = 1;	
			} else {
				ear_info[ear_ch].det_flt = TCFG_DET_FLT_CNT;
				if (vout_current_mA_get(ear_ch)) {
					ear_info[ear_ch].online_hw = 1;
				} else {
					//�л���С�����ɼ�
					vout_current_sense_mode_set(ear_ch, 0);
					delay_n2ms(1);
					//����С�����ɼ�����ж��Ƿ���������		
					if (vout_current_mA_get(ear_ch)) {
						ear_info[ear_ch].online_hw = 1;	
					} else {
						if (get_vout_pwr_ctrl_en(ear_ch)) {
							vout_pwr_ctrl(ear_ch, 0);
						} else {
							//���ݸ��ؼ�����ж��Ƿ���������
							ear_info[ear_ch].online_hw = vout_inserdet_state_get(ear_ch);
						}
					}
					//�л���������ɼ�
					vout_current_sense_mode_set(ear_ch, 1);
					delay_n2ms(1);
				}
			}
		} else {
			ear_info[ear_ch].det_flt = 0;
			ear_info[ear_ch].online_hw = vout_inserdet_state_get(ear_ch);
		} 

		//����״̬�����仯
		if (online_st != ear_info[ear_ch].online_hw) {
			if (ear_info[ear_ch].online_hw) {
				if (ear_ch == EAR_L) {
					app_close_deal_ear_online(EAR_L);
#if TCFG_UI_ENABLE
					ui_run_status(UI_STATUS_EAR0_IN);
#endif
				} else {
					app_close_deal_ear_online(EAR_R);
#if TCFG_UI_ENABLE
					ui_run_status(UI_STATUS_EAR1_IN);
#endif
				}
			} else {
				if (ear_ch == EAR_L) {
					app_close_deal_ear_offline(EAR_L);
#if TCFG_UI_ENABLE
					ui_run_status(UI_STATUS_EAR0_OUT);
#endif
				} else {
					app_close_deal_ear_offline(EAR_R);
#if TCFG_UI_ENABLE
					ui_run_status(UI_STATUS_EAR1_OUT);
#endif
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
	if (b_sys_busy_in_charge && (!sys_info.charge_cc_state) && get_charge_optimize_end_flag()) //���/������ڽ׶β��������
#else
	if (b_sys_busy_in_charge && (!sys_info.charge_cc_state)) //����׶β��������
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
	//����رճ���usb������
	if (sys_info.localcharge && (!b_sys_busy_in_charge)) {
		if (power_get_vbat_percent() <= CHARGE_RECHARGE_PERCENT) {
			log_puts("recharge\n");
			b_charge_localfull = 0;
		} 
	}
}

void discharge_current_limit_det_run(void)
{
	u8 curr_temp[2];
	u8 i;

	for (i = 0; i < EAR_MAX; i++) {
		if (ear_info[i].charge) {
			curr_temp[i] = vout_current_mA_get(i);
			//log_put_u8(curr_temp[i]);
			if ((curr_temp[i] >= TCFG_CURRENT_LIMIT_MA)) {
				log_puts("vbat oc\n");
				b_discharge_current_limit = 1;
				sys_info.sys_abnormal = 1;		
			}
			if ((curr_temp[i] < TCFG_EAR_CHARGE_FULL_CURRENT_MA) && !ear_info[i].full_flag && b_sys_busy_in_boost && ear_info[i].online_hw) {
				ear_info[i].full_cnt++;
				if (ear_info[i].full_cnt >= TCFG_EAR_FULL_DET_CNT) {
					ear_info[i].full_cnt = 0;
					ear_info[i].full_flag = 1;
					if (i) {
						b_evt_ear1_full = 1;
					} else {
						b_evt_ear0_full = 1;	
					}
				}
			} else {
				ear_info[i].full_cnt = 0;
			}		
		}
	}
}

#if TCFG_HANDSHAKE_ENABLE
void handshake_run_det_run(void)
{
	//����lighting�����ٲ��,û����Ӧusb off�ź�,��������������
	if (b_sys_busy_in_usb_on && usb_shake_flag) {
		handshake_start();
		usb_shake_flag = 0;
	}
#if TCFG_WPR_ENABLE
	//���߳������ʱ,���������ֲ���
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
	PMU_IE(0);//�ر�PMU���жϣ���Ҫ������ж�

	//��翪�ؼ��
	charge_ctrl(!charge_sw_check());

	//�ŵ翪�ؼ��
#if TCFG_VIN2VOUT_ENABLE
	//�ȹغ�,˳���ܱ䣡
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

	PMU_IE(1);//����pmu���ж�
}

void app_common_evt_512ms(void)
{
	wdt_clear();

	//�����������
	discharge_current_limit_det_run(); 

	//��ŵ翪�ؼ��
	charge_discharge_sw_det_run();

	//�ճ��������
	if (sys_info.localcharge) {
		charge_cc_check();
	}

	//�ճ��������
	localfull_det_run();

	//�ճ��س���
	recharge_det_run(); 

#if TCFG_CHARGE_OPTIMIZE_ENABLE
	//�������������
	power_optimize_for_charge_current_adjust();
#endif

#if TCFG_LONG_PRESS_RESET_ENABLE && (LONG_PRESS_PINR_IN_SEL == VPWR_IN_DETECT)
	vpwr_long_press_reset_check();
#endif

	//����VBG
	adc_update_vbg();

	//��ص������
	power_det_run();

	//�������ؼ��
	vout_insertdet_run();
	
	//ntc���
#if TCFG_EXTERN_NTC_ENABLE
    ntc_det_run();
#endif
#if TCFG_INSIDE_NTC_ENABLE
	ntc_inside_det_run();
#endif

#if TCFG_HANDSHAKE_ENABLE
	handshake_run_det_run();
#endif

#if TCFG_BOOST_OPTIMIZE_ENABLE
	//�ŵ��������
	power_optimize_for_boost_voltage_adjust();
#endif

	//ϵͳ�쳣�˳����
	sys_abnormal_exit_det_run();
}

#if TCFG_KEY_ENABLE
void app_common_evt_key(void)
{
	if (b_evt_key_click) {
		log_putchar('C');
		b_evt_key_click = 0;
		//������ѹ
		if (ear_info[EAR_L].online_hw && ear_info[EAR_L].full_flag) {
			ear_info[EAR_L].online_hw = 0;
		}
		if (ear_info[EAR_R].online_hw && ear_info[EAR_R].full_flag) {
			ear_info[EAR_R].online_hw = 0;
		}
#if TCFG_UI_ENABLE
		ui_run_status(UI_STATUS_KEY_CLICK);
#endif	 
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

void app_common_event_deal(void) 
{
    app_event_call(b_evt_hall, app_auto_mode);
	app_event_call(b_evt_2ms, app_common_evt_2ms);
	app_event_call(b_evt_8s, discharge_overtime_check);

#if TCFG_WPR_ENABLE
    app_event_call(b_evt_wireless_on, wireless_api_open);
    app_event_call(b_evt_wireless_off, wireless_api_close);
    app_event_call(b_evt_wpr_128ms, wireless_api_128ms_run);
#endif

	app_event_call(b_evt_usb_in, usb_in_handle);
	app_event_call(b_evt_usb_out, usb_out_handle);

#if TCFG_EXTERN_NTC_ENABLE
	//���״̬�仯����
	app_event_call(b_evt_ntc_charge, app_common_ntc_charge_handle);
#endif

#if TCFG_KEY_ENABLE
	app_common_evt_key();
#endif
}


