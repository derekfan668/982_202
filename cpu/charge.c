#include "includes.h"
#include "charge.h"
#include "power_det.h"
#include "app_var.h"
#include "adc.h"
#include "extern_ntc_det.h"
#include "app_config.h"
#include "power_optimize.h"

#if TCFG_CHARGE_OPTIMIZE_ENABLE
void charge_set_charge_mA(u8 current_lvl)
{
	if (get_charge_optimize_end_flag()) {
		CHGI_SET(current_lvl);
	}
}
#else
void charge_set_charge_mA(u8 current_lvl)
{
	u8 t_current_lvl;
	t_current_lvl = current_lvl;
#if TCFG_WPR_ENABLE
	if (wpr_status && b_evt_wireless_ov) {
		t_current_lvl = CHARGE_WIRELESS_OV_CURRENT_MA;
	}
#endif
#if TCFG_EXTERN_NTC_ENABLE
#if (NTC_MODE_SEL == NTC_MODE_JEITA)
	if (ntc.u.charge == NTC_STATUS_CHARGE_CURR) {
		t_current_lvl = (NTC_CHARGE_CURRENT_MA < t_current_lvl) ? NTC_CHARGE_CURRENT_MA : t_current_lvl; 
	}
#endif
#endif
	t_current_lvl = (t_current_lvl < current_lvl) ? t_current_lvl : current_lvl;
	CHGI_SET(t_current_lvl);
	//log_put_u8(t_current_lvl);
}
#endif

u16 vbat_charge_current_mA_get(void)
{
	return (adc_get_voltage(AD_ANA_CH_PROGI, 1) / 5);
}

void charge_set_charge_volt(u8 full_volt)
{
	u8 trim_val, offset;
#if (CHARGE_CONSTANT_VOLTAGE <= CHGV_S_4338_MV)
	CHG_HV_MODE(0);
	if (!otp_trim.trim_en.u.vbat_4p2) {
		trim_val = otp_trim.pmu1.u.vbat_4p2;
	} else {
		trim_val = CHGV_S_4199_MV;
	}
	//log_put_u8(trim_val);
	//log_puts(":vbat420_trim\n");
	if (full_volt >= CHGV_S_4199_MV) {
		offset = full_volt - CHGV_S_4199_MV;
		full_volt = trim_val + offset;
		if (full_volt > 0xf) {
			full_volt = 0xf;
		}	
	} else {
		offset = CHGV_S_4199_MV - full_volt;
		if (trim_val >= offset) {
			full_volt = trim_val - offset;	
		} else {
			full_volt = 0;
		}
	}
	//log_put_u8(full_volt);
	//log_puts(":vbat420_set\n");
	CHGV_SET(full_volt);
#else
	CHG_HV_MODE(1);
	if (!otp_trim.trim_en.u.vbat_4p35) {
		trim_val = otp_trim.pmu1.u.vbat_4p35;
	} else {
		trim_val = CHGV_S_4354_MV - CHGV_S_4237_MV;
	}
	//log_put_u8(trim_val);
	//log_puts(":vbat435_trim\n");
	if (full_volt >= CHGV_S_4354_MV) {
		offset = full_volt - CHGV_S_4354_MV;
		full_volt = trim_val + offset;
		if (full_volt > 0xf) {
			full_volt = 0xf;
		}	
	} else {
		offset = CHGV_S_4354_MV - full_volt;
		if (trim_val >= offset) {
			full_volt = trim_val - offset;	
		} else {
			full_volt = 0;
		}
	}
	//log_put_u8(full_volt);
	//log_puts(":vbat435_set\n");
	CHGV_SET(full_volt);
#endif
}

void charge_ctrl(u8 en)
{
	if (b_sys_busy_in_charge == en) {
		return;
	}

	if (en) {
		if (power_get_vbat_voltage() < CHARGE_TC_2_CC_VOLT_MV) {
			//log_puts("\nenter trickle charge\n");
			sys_info.charge_cc_state = 1;
#if (TCFG_CHARGE_OPTIMIZE_ENABLE == 0)
			CHGI_TRICKLE_EN(1);
#endif
		}
#if TCFG_CHARGE_OPTIMIZE_ENABLE
		CHGI_TRICKLE_EN(1);//默认开启charge，使能涓流
#endif
		CHARGE_EN(3);
		b_sys_busy_in_charge = 1;
		log_puts("charge start\n");
	} else {
		CHARGE_EN(0);
		CHGI_TRICKLE_EN(0);
		b_sys_busy_in_charge = 0;
		log_puts("charge close\n");
	}
}

void charge_cc_check(void)
{
	if (!sys_info.charge_cc_state) {
		return;
	}
	if (power_get_vbat_voltage() > CHARGE_TC_2_CC_VOLT_MV) {
		//log_puts("\nexit trickle charge\n");
		sys_info.charge_cc_state = 0;
#if TCFG_CHARGE_OPTIMIZE_ENABLE
		//此处不关闭涓流使能，由power_optimize充电电流缓变模块关闭
#else 
		CHGI_TRICKLE_EN(0);
#endif
	}
}

void charge_init(void)
{
	CHARGE_EN(0);
	CHGI_SET(CHARGE_CURRENT_MA);
	charge_set_charge_volt(CHARGE_CONSTANT_VOLTAGE);
}

