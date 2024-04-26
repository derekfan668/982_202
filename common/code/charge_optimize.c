#include "includes.h"
#include "app_config.h"
#include "app_var.h"
#include "power_optimize.h"
#include "charge.h"
#include "extern_ntc_det.h"

#if TCFG_CHARGE_OPTIMIZE_ENABLE
u8 get_charge_optimize_end_flag(void)
{
	u8 end_lvl;
#if TCFG_WPR_ENABLE
	if (wpr_status) {
		if (b_evt_wireless_ov) {
			end_lvl = CHARGE_WIRELESS_OV_CURRENT_MA + 8;
		} else {
			end_lvl = CHARGE_WIRELESS_CURRENT_MA + 8;
		}	
	} else {
		end_lvl = CHARGE_CURRENT_MA + 8;
	}
#else 
	end_lvl = CHARGE_CURRENT_MA + 8;
#endif

#if TCFG_EXTERN_NTC_ENABLE
#if (NTC_MODE_SEL == NTC_MODE_JEITA)
	if (ntc.u.charge == NTC_STATUS_CHARGE_CURR) {
		end_lvl = ((NTC_CHARGE_CURRENT_MA+8) < end_lvl) ? (NTC_CHARGE_CURRENT_MA+8) : end_lvl; 
	}
#endif
#endif

	//log_put_u8(power_optimize.charge_lvl);
	//log_put_u8(end_lvl);
	//log_puts("\n");

	if (power_optimize.charge_lvl == end_lvl) {
		return 1;
	} else if (power_optimize.charge_lvl > end_lvl) {
		power_optimize.charge_lvl = end_lvl - 1;
	} 
	return 0;  
}

void power_optimize_for_charge_current_adjust(void)
{
	if (!b_sys_busy_in_charge) {
		power_optimize.charge_lvl = CHARGE_CURRENT_MA;//初始化对应涓流挡位
		return;
	}

	if (b_sys_busy_in_charge) {
		if (sys_info.charge_cc_state) {
			return;
		}
		if (get_charge_optimize_end_flag()) {
			return;
		}
	}

	power_optimize.charge_lvl++;
	if (power_optimize.charge_lvl < 8) {
		CHGI_TRICKLE_EN(1);	
	} else {
		CHGI_TRICKLE_EN(0);
	}
	CHGI_SET(power_optimize.charge_lvl%8);
	//log_put_u8(power_optimize.charge_lvl);
}
#endif