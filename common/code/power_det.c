#include "includes.h"
#include "app_config.h"
#include "app_var.h"
#include "power_det.h"
#include "adc.h"
#include "app_common.h"
#include "ui.h"

//实时电池电压,计算百分比,500mS扫描一次
static data __power_det power_det;
static idata u16 cur_bat_volt;

#define POWER_DETECT_PERIOD             (10 * 2) //检测周期30S

#define POWER_LVL_MAX   10

#define POWERINIT_FILTER_CNT            3  //连续相同电压的滤波数次	  <=15

#define POWERINIT_TIMEOUT_CNT			12 //超时之后直接滤波数次	 12 * 150ms  1.8s	 <=15

#if (VBAT_TYPE_SEL == VBAT_TYPE_4200)
code const u8 voltage_table[POWER_LVL_MAX] = {VBAT420_LEV_10, VBAT420_LEV_20, VBAT420_LEV_30, VBAT420_LEV_40, VBAT420_LEV_50,
                                              VBAT420_LEV_60, VBAT420_LEV_70, VBAT420_LEV_80, VBAT420_LEV_90, VBAT420_LEV_100};
#else
code const u8 voltage_table[POWER_LVL_MAX] = {VBAT435_LEV_10, VBAT435_LEV_20, VBAT435_LEV_30, VBAT435_LEV_40, VBAT435_LEV_50,
                                              VBAT435_LEV_60, VBAT435_LEV_70, VBAT435_LEV_80, VBAT435_LEV_90, VBAT435_LEV_100};
#endif

//获取电压
u16 power_det_voltage(void)
{
	cur_bat_volt = adc_get_voltage(AD_ANA_CH_VBAT_DIV_4, 8) * 4;
	return cur_bat_volt;
}
u8 low_power_flag=0;
//��������ٷֱ�
u8 power_calc_percent(void)
{
	u8 calc_volt, i, tmp0, tmp1;
	if (cur_bat_volt <= 3500) {
		low_power_flag=1;
	}else{
		low_power_flag=0;
	}
	if (cur_bat_volt <= POWER_BOOT_LEV) {
		return 0;
	} else if (cur_bat_volt >= POWER_TOP_LEV) {
		return 100;
	} else {
		calc_volt = (cur_bat_volt - POWER_BOOT_LEV) / 10;
		for (i = 0; i < POWER_LVL_MAX; i++) {
			if (calc_volt < voltage_table[i]) {
				if (i == 0) {
					tmp0 = calc_volt;
					tmp1 = voltage_table[0];
				} else {
					tmp0 = calc_volt - voltage_table[i-1];
					tmp1 = voltage_table[i] - voltage_table[i-1];
				}
				calc_volt = (tmp0 * 10) / tmp1 + i*10;
				break;
			}
		}
	}
	return calc_volt;
}

//扫描更新百分比信息
void power_det_scan(void)
{
	u8 cur_percent;
	power_det_voltage();
	cur_percent = power_calc_percent();
	if ((power_det.percent > cur_percent) && (!b_sys_busy_in_charge)) {
		power_det.percent--;
		if ((power_det.percent == 0) && (sys_info.bat_low == 0)) {
    		sys_info.bat_low = 1;
    		app_auto_mode();//如果当前是开盖则等待超时进入低电模式
		}
	} else if ((power_det.percent < cur_percent) && b_sys_busy_in_charge) {
		power_det.percent++;
		if (sys_info.bat_low) {
    		sys_info.bat_low = 0;
    		app_auto_mode();
		}
	}
}

void power_info_dump(void)
{
	log_puts("\nvbat volt: 0x");
	log_put_u16(cur_bat_volt);
	log_puts("\nvbat lvl: 0x");
	log_put_u8(power_det.percent);
	log_puts("\n");

	//更新充电舱电量显示
#if TCFG_UI_ENABLE && (TCFG_UI_MODE == UI_SEG_LED)
	ui_run_status(UI_STATUS_UPDATE_VBAT_PERCENT);
#endif
}

void power_det_run(void)
{	
	power_det.counter++;
	if(!sys_info.localcharge){
		if (power_det.counter >= POWER_DETECT_PERIOD) {
			power_det.counter = 0;
			power_det_scan();
			power_info_dump();
// #ifndef TCFG_NORMALBOX_CASE
// #if (!TCFG_HALL_ENABLE)
			//使用无hall通信仓的时候，30s更新一次电量信息
			app_battery_notice();
			delay_n2ms(10);
			app_battery_notice();
// #endif
// #endif
		}
	} else {
		if ((power_det.counter % 4) == 0)
		{
			power_det.counter = 0;
			power_det_scan();
			power_info_dump();
			app_f98_battery_adc_val_notice();
			delay_n2ms(10);
			app_battery_notice();
		}
	}
}
//电压初始化的时候防止物理抖动导致的电压不准确问题。
void power_on_filter_init(void)
{
	u16 cur_power = 0,last_power = 0;
	while (1) {
	    last_power = cur_power;
		power_det_voltage();
		cur_power = cur_bat_volt;
		if(cur_power != last_power) {
			power_det.same_volt_cnt = 0;
		} else {
		    power_det.same_volt_cnt++; 
		}
		if(power_det.same_volt_cnt >= POWERINIT_FILTER_CNT) {
			power_det.same_volt_cnt = 0; 
			break;	
		}
		power_det.filter_time_cnt++;
		if(power_det.filter_time_cnt >= POWERINIT_TIMEOUT_CNT) {
		    power_det.filter_time_cnt = 0;
			break;
		}
		delay_n2ms(75);
	}	
}

void power_det_init(void)  
{
	power_det.counter = 0;
	power_on_filter_init();
	power_det.percent = power_calc_percent();
	power_info_dump();
	if (power_det.percent == 0) {
		sys_info.bat_low = 1;
	} else {
		sys_info.bat_low = 0;
	}
}

u16 power_get_vbat_voltage(void)
{
    if (cur_bat_volt > POWER_TOP_LEV) {
        return POWER_TOP_LEV;
    } else {
        return cur_bat_volt;
    }
}

u8 power_get_vbat_percent(void)
{
    return power_det.percent;
}


