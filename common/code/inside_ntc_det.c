#include "includes.h"
#include "app_config.h"
#include "app_var.h"
#include "adc.h"
#include "chargebox.h"
#include "app_close.h"

#if TCFG_INSIDE_NTC_ENABLE 

#include "inside_ntc_det.h"
//[-40°C~125°C] -> [1620mV~1000mV], -3.75mV/°C
#define VTEMP_125	 	1000
#define VTEMP_75	 	1190
#define VTEMP_23	 	1383
#define VTEMP_NEG_10 	1507
#define VTEMP_NEG_20 	1545
void ntc_inside_det_run(void)
{
	u16 temp_v;
	u16 voltage_23;

	if (!(otp_trim.new_vtemp>>15)) {
		voltage_23 = otp_trim.new_vtemp * 3 / 16;
	} else if (!otp_trim.trim_en.u.vtemp) {
		if (!otp_trim.vtemp) {
			voltage_23 = VTEMP_23;
		} else {
		 	voltage_23 = otp_trim.vtemp * 3 / 16;
		}
	} else {
		voltage_23 = VTEMP_23;
	}

	temp_v = adc_get_voltage(AD_ANA_CH_VTEMP, 1);
	//log_puts("\n inside ntc v: 0x");
	//log_put_u16(temp_v);
	//内置ntc高于125°C或低于-20°C触发异常
	if ((temp_v < (VTEMP_125 + VTEMP_23 - voltage_23)) || (temp_v > (VTEMP_NEG_20 + VTEMP_23 - voltage_23))) {
		//log_puts("inside ntc err\n");
		b_charge_inside_ntc_err = 1;
		b_discharge_inside_ntc_err = 1;
	} 
	
	//内置ntc小于75°C且大于-10°C解除异常
	if ((temp_v > (VTEMP_75 + VTEMP_23 - voltage_23)) && (temp_v < (VTEMP_NEG_10 + VTEMP_23 - voltage_23))) {
		if (b_charge_inside_ntc_err) {
			//log_puts("inside ntc good 0\n");
			b_charge_inside_ntc_err = 0;
		}

		if (b_discharge_inside_ntc_err) {
			//log_puts("inside ntc good 1\n");
			b_discharge_inside_ntc_err = 0;
#ifndef TCFG_NORMALBOX_CASE
			if (sys_info.mode == APP_MODE_CLOSE) {
				app_close_deal_ear_online(EAR_L);
				app_close_deal_ear_online(EAR_R);
			}
#else
			ear_info[EAR_R].online_hw = 0;
			ear_info[EAR_L].online_hw = 0;
#endif
		}
	}
}

#endif

