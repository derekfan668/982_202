#include "includes.h"
#include "boost.h"
#include "app_var.h"
#include "adc.h"
#include "timer.h"
#include "chargebox.h"
#include "uart_send.h"

#define NEW_BOOST_LEN      16
code const u8 new_boost_volt_table[NEW_BOOST_LEN] = {4, 3, 3, 2, 2,	1, 1, 0, 1, 1, 2, 3, 3, 4, 5, 6};

void vout_current_sense_mode_set(u8 ch, u8 mode)
{
	if (ch) {
		VOUT1_CSMODE(mode);//1:大电流模式 0:小电流模式
		if (mode && (!otp_trim.trim_en.u.vout1_curr_h)) {
			VOUT1_CSRES_SET(otp_trim.vout1_high.u.csres);
		} else if ((!mode) && (!otp_trim.trim_en.u.vout1_curr_l)) {
			VOUT1_CSRES_SET(otp_trim.vout1_low.u.csres);
		} else {
			VOUT1_CSRES_SET(3);
		}
	} else {
		VOUT0_CSMODE(mode);//1:大电流模式 0:小电流模式
		if (mode && (!otp_trim.trim_en.u.vout0_curr_h)) {
			VOUT0_CSRES_SET(otp_trim.vout0_high.u.csres);
		} else if ((!mode) && (!otp_trim.trim_en.u.vout0_curr_l)) {
			VOUT0_CSRES_SET(otp_trim.vout0_low.u.csres);
		} else {
			VOUT0_CSRES_SET(3);
		}
	}
}

u8 vbat_discharge_current_mA_get(void)
{
	u16 vbat_curr;
	BOOST_VBATCS_EN(1);
	//VBAT电流不能采集瞬时值,需要一段时间内的平均值
	vbat_curr = adc_get_voltage(AD_ANA_CH_VBAT_ISENSE, 8) * 2 / 5;
	if (vbat_curr > 255) {
		return 255;
	}
	return vbat_curr;	
}

u8 vout_current_mA_get(u8 ch)
{
	u8 vout_curr;
	if (ch) {
		if (VOUT1_CSMODE_GET()) {
			vout_curr = (adc_get_voltage(AD_ANA_CH_VOUT1_ISENSE, 8) / 20);
		} else {
			vout_curr = (adc_get_voltage(AD_ANA_CH_VOUT1_ISENSE, 8) / 200);
		}
	} else {
		if (VOUT0_CSMODE_GET()) {
			vout_curr = (adc_get_voltage(AD_ANA_CH_VOUT0_ISENSE, 8) / 20);
		} else {
			vout_curr = (adc_get_voltage(AD_ANA_CH_VOUT0_ISENSE, 8) / 200);
		}
	}
	if (vout_curr >= 150) {
		return 150;
	}
	return vout_curr;
}

void vout_current_sense_init(void)
{
	VOUT0_CS_EN(1);
	vout_current_sense_mode_set(EAR_L, 1);
	VOUT0_CSVDSKEEP_EN(1);
	VOUT0_CSVDSVOL_SET(3);

	VOUT1_CS_EN(1);
	vout_current_sense_mode_set(EAR_R, 1);
	VOUT1_CSVDSKEEP_EN(1);
	VOUT1_CSVDSVOL_SET(3);	
}

void vout_pwr_ctrl(u8 ch, u8 en)
{
	if (ch) {
		VOUT1_OUTPUT_EN(en);
	} else {
		VOUT0_OUTPUT_EN(en);
	}
}

u8 get_vout_pwr_ctrl_en(u8 ch)
{
	if (ch) {
		if (VOUT1_OUTPUT_EN_GET()) {
			return 1;
		}
	} else {
		if (VOUT0_OUTPUT_EN_GET()) {
			return 1;
		}
	}
	return 0;
}

u8 get_vout_ov_flag(void)
{
	if (VOUT_OV_COMP_GET()) {
		return 1;
	}
	return 0;	
}

u8 get_vout_uv_flag(void)
{
	if (VOUT_UV_COMP_GET()) {
		return 1;
	}
	return 0;	
}

void vout_ov_ctrl(u8 en)
{
	VOUT_OV_DET_EN(en);
	VOUT_OV_COMP_OE(en);
	WKUPx_AUTO_EN(WKUP_0_VOUT_OV, en);
	WKUPx_EN(WKUP_0_VOUT_OV, en);
}

void vout_uv_ctrl(u8 en)
{
	VOUT_UV_DET_EN(en);
	VOUT_UV_COMP_OE(en);
	WKUPx_AUTO_EN(WKUP_1_VOUT_UV, en);
	WKUPx_EN(WKUP_1_VOUT_UV, en);
}

void vout_ov_uv_init(void)
{
	VOUT_OV_VOL_SET(VOUT_OV_VOLT);
	VOUT_UV_VOL_SET(VOUT_UV_VOLT);	
}

//过压欠压标志位获取
u8 get_vout_ov_uv_loop(void)
{
	if (get_vout_ov_flag() || get_vout_uv_flag()) {
		return 1;	
	} else {
		return 0;		
	}
	
}

void vin2vout(u8 en) 
{
	u16 vin_volt;
	u16 vout_volt;
	u16 diff;

	if (b_sys_busy_in_vin2vout == en) {
		return;
	}

	vout_pwr_ctrl(EAR_L, 0);
	vout_pwr_ctrl(EAR_R, 0);
	if (en)	{
		sys_info.sys_abnormal = 1;
		b_discharge_vout_ovuv = 1;
		vin_volt = adc_get_voltage(AD_ANA_CH_VIN_DIV_4, 1);
		if (vin_volt <= 1375) {
			delay_n2ms(25);
			vin_volt = adc_get_voltage(AD_ANA_CH_VIN_DIV_4, 1);
			if (vin_volt <= 1375) {
				VIN2VOUT_SSEN(1);
				delay_n2ms(2);
				vout_volt = adc_get_voltage(AD_ANA_CH_VOUT_DIV_4, 1);
				diff = (vin_volt >= vout_volt) ? (vin_volt - vout_volt) : (vout_volt - vin_volt);
				if (diff <= (vin_volt / 10)) {
					VIN2VOUT_EN(1);
				} else {
					//log_puts("vin2vout failed 0\n");
					goto _vin2vout_failed;//不满足(vin-vout)/vin在10%以内	
				}
				vout_ov_ctrl(1);
				vout_uv_ctrl(1);
				vout_pwr_ctrl(EAR_L, ear_info[EAR_L].charge);
				vout_pwr_ctrl(EAR_R, ear_info[EAR_R].charge);
				if (get_vout_ov_uv_loop()) {
					//log_puts("vin2vout failed 3\n");
					vout_pwr_ctrl(EAR_L, 0);
					vout_pwr_ctrl(EAR_R, 0);
					goto _vin2vout_failed;//vout ov或者uv
				}
				b_sys_busy_in_vin2vout = 1;
				sys_info.sys_abnormal = 0;
				b_discharge_vout_ovuv = 0;
				log_puts("vin2vout start\n");
				app_battery_notice();
			} else {
				//log_puts("vin2vout failed 1\n");
				goto _vin2vout_failed;//vin不满足连续50mS低于5.5V条件
			}	
		} else {
			//log_puts("vin2vout failed 2\n");
			goto _vin2vout_failed;//vin大于5.5V，不符合vin2vout打开条件
		}

	} else {
_vin2vout_failed:
		vout_uv_ctrl(0);
		vout_ov_ctrl(0);
		b_sys_busy_in_vin2vout = 0;
		log_puts("vin2vout close\n");
		VIN2VOUT_SSEN(0);
		VIN2VOUT_EN(0);
		app_battery_notice();
	}
}

void boost_ctrl(u8 en) 
{
    u8 boost_gradual_volt = 0;//渐变的电压档位为最低一档
	if (b_sys_busy_in_boost == en) {
		return;
	}	

	vout_pwr_ctrl(EAR_L, 0);
	vout_pwr_ctrl(EAR_R, 0);
	if (en) {
	    boost_start_unlock = 1;//boost启动流程不锁升压
		boost_set_volt(boost_gradual_volt);//刷新boost的档位，从最低档位开始一档一档往上升
		BOOST_CLK_EN();
		BOOST_EN(1);
		//按照一档一档的升压方式
		if(boost_lock_volt) {   //判断是否锁存boost升压
			for(boost_gradual_volt = 1; boost_gradual_volt <= BOOST_VOUT_S_5000_MV; boost_gradual_volt++) {
				boost_set_volt(boost_gradual_volt);
				delay_n2ms(3);//延时6ms，防止在升压时候环路不稳定检测过压欠压问题
			}
		} else {
		   	for(boost_gradual_volt = 1; boost_gradual_volt <= TCFG_BOOST_VOUT_S; boost_gradual_volt++) {
				boost_set_volt(boost_gradual_volt);
				delay_n2ms(3);//延时6ms，防止在升压时候环路不稳定检测过压欠压问题
			} 
		}
		boost_start_unlock = 0;//boost启动流程之后锁升压
		vout_ov_ctrl(1);
		vout_uv_ctrl(1);
		vout_pwr_ctrl(EAR_L, ear_info[EAR_L].charge);
		vout_pwr_ctrl(EAR_R, ear_info[EAR_R].charge);
		if (get_vout_ov_uv_loop()) {
			b_discharge_vout_ovuv = 1;
			sys_info.sys_abnormal = 1;
			//log_puts("boost failed\n");
			vout_pwr_ctrl(EAR_L, 0);
			vout_pwr_ctrl(EAR_R, 0);
			goto _boost_failed;	
		}
		b_sys_busy_in_boost = 1;	
		log_puts("boost start\n");
		app_battery_notice();
	} else {
_boost_failed:
		vout_uv_ctrl(0);
		vout_ov_ctrl(0);
		BOOST_EN(0);
		BOOST_CLK_DIS();
		b_sys_busy_in_boost = 0;
		log_puts("boost close\n");	
		app_battery_notice();
	}
}

void boost_set_volt(u8 boost_volt)
{
	u8 trim_val, offset;

	if (boost_lock_volt && (!boost_start_unlock)) {
		return;
	}
	if(!(otp_trim.new_boost.u.boost_position == 0x3)) {
		trim_val = otp_trim.new_boost.u.new_boost_lev;
	} else if (!otp_trim.trim_en.u.boost) {
		trim_val = otp_trim.pmu0.u.boost_lev;
	} else {
		trim_val = BOOST_VOUT_S_5000_MV;
	}
	//log_put_u8(trim_val);
	//log_puts(":boost trim!\n");
	if (boost_volt >= BOOST_VOUT_S_5000_MV) {
		offset = boost_volt - BOOST_VOUT_S_5000_MV;
		boost_volt = trim_val + offset;
		if (boost_volt > 0xf) {
			boost_volt = 0xf;
		}	
	} else {
		offset = BOOST_VOUT_S_5000_MV - boost_volt;
		if (trim_val >= offset) {
			boost_volt = trim_val - offset;	
		} else {
			boost_volt = 0;
		}
	}
	if(otp_trim.new_boost.u.boost_position == 0x3) {
		if(!otp_trim.trim_en.u.mvbg){
		    //log_put_u8(otp_trim.pmu0.u.mvbg_lev);
			if(otp_trim.pmu0.u.mvbg_lev <= 0x07) {
				boost_volt = boost_volt + new_boost_volt_table[otp_trim.pmu0.u.mvbg_lev];
				if (boost_volt > 0xf) {
					boost_volt = 0xf;
				}		
			} else {
			    if(boost_volt > new_boost_volt_table[otp_trim.pmu0.u.mvbg_lev]) {
			    	boost_volt = boost_volt - new_boost_volt_table[otp_trim.pmu0.u.mvbg_lev];
				} else {
					boost_volt = 0;	
				} 
			}		
		}				
	}
	//log_put_u8(boost_volt);
	//log_puts(":boost set!\n");
	BOOST_VOUT_SET(boost_volt);
}

void boost_init(void)
{
	//BOOST_VS_EN(1);		
	//BOOST_VS_SET(1);		
	//BOOST_ZCD_EN(1);	
	//BOOST_ZCD_HSEL(1);	
	//BOOST_ZCD_LSEL(0);
	PMU_CON_SET(PSFR_ANA_CON2, 0xff, (0x01 << 4) | (0x01 << 3) | (0x01 << 1) | 0x01);
	//RC32M_EN(1);			
	//RC32M_NTRIM_SET(0x4);	
	//RC32M_PTRIM_SET(0x8);
	PMU_CON_SET(PSFR_ANA_CON10, 0xff, (0x8 << 4) | (0x4 << 1) | 0x01);

	vout_current_sense_init();
	vout_ov_uv_init();
	boost_start_unlock = 0;//boost启动流程锁存标志位
}


