#include "includes.h"
#include "app_config.h"
#include "app_var.h"
#include "power_optimize.h"
#include "boost.h"
#include "chargebox_hw.h"
#include "app_close.h"
#include "vout_inserdet.h"
#include "adc.h"

#if TCFG_BOOST_OPTIMIZE_ENABLE

__boost_optimize boost_optimize;

#if (TCFG_BOOST_OPTIMIZE_MODE == TCFG_MODE_FOLLOW_MIN) 

//停止跟随充的耳机电流(一般设置比耳机恒流电流小15mA左右)
#define TCFG_STOP_ADJUST_CURR		(TCFG_EAR_CHARGE_CURRENT_MA - 15) 
//启动跟随充的耳机电流(一般设置比耳机恒流电流小10mA左右)
#define TCFG_START_ADJUST_CURR		(TCFG_EAR_CHARGE_CURRENT_MA - 10) 
#define TCFG_START_FLT_TIMES		5  // <=7，TCFG_START_FLT_TIMES*512mS
#define TCFG_STOP_FLT_TIMES			7  // <=7，TCFG_STOP_FLT_TIMES*512mS
#define TCFG_ADJUST_ADD_FLT_TIMES	3  // <=7，TCFG_ADJUST_ADD_FLT_TIMES*512mS
#define TCFG_ADJUST_SUB_FLT_TIMES	2  // <=7，TCFG_ADJUST_SUB_FLT_TIMES*512mS
#define TCFG_LOW_CH_FLT_TIMES		7  // <=15，TCFG_LOW_CH_FLT_TIMES*512mS
#define TCFG_CH_FULL_FLT_TIMES		7  // <=15，TCFG_CH_FULL_FLT_TIMES*512mS
#define TCFG_LOW_CH_COMP_CURR		3  //3mA
#define TCFG_UPDATE_ADJUST_TIMES	60 // TCFG_UPDATE_ADJUST_TIMES*512mS
#define adjust_ear_now_curr			optimize_buf[0]	//耳机当前充电电流
#define adjust_ear_last_curr		optimize_buf[1]	//耳机上一次充电电流
#define curr_diff    				optimize_buf[2] //左右耳充电电流差值

void power_optimize_for_boost_voltage_adjust(void)
{
	u8 curr_temp[2];
	u8 adjust_ch;
	u8 curr_change;
	u16 vbat_volt;
	u8 min_lvl, temp0, temp1;

   	//没有在升压状态或者升压电压被锁存了，不开启跟随充
	if (!b_sys_busy_in_boost || boost_lock_volt) {
		boost_optimize.boost_optimize_en = 0;
		boost_optimize.init_state = 0;		
		power_optimize.boost_lvl = TCFG_BOOST_VOUT_S;
		return;
	}

	curr_temp[EAR_L] = vout_current_mA_get(EAR_L);
	//log_put_u8(curr_temp[EAR_L]);	
	curr_temp[EAR_R] = vout_current_mA_get(EAR_R);
	//log_put_u8(curr_temp[EAR_R]);

	if (!boost_optimize.boost_optimize_en) {
		//有一只耳机处于恒流状态即可开启跟随充
		if ((curr_temp[EAR_L] >= TCFG_START_ADJUST_CURR) || (curr_temp[EAR_R] >= TCFG_START_ADJUST_CURR)) {
			boost_optimize.boost_optimize_en = 1;
		} 
	} 
	
	//跟随充退出检查，boost挡位在最高档持续时间超过TCFG_STOP_FLT_TIMES，且左右耳电流小于TCFG_STOP_ADJUST_CURR
	//注意这里如果两只耳机都非恒流状态时会退出跟随充，等有只耳机进入恒流之后再开启跟随充
	if (power_optimize.boost_lvl == TCFG_BOOST_VOUT_S) {
		if ((curr_temp[EAR_L] <= TCFG_STOP_ADJUST_CURR) && (curr_temp[EAR_R] <= TCFG_STOP_ADJUST_CURR)) {
			boost_optimize.optimize_stop_flt++;
			if (boost_optimize.optimize_stop_flt >= TCFG_STOP_FLT_TIMES) {
				boost_optimize.boost_optimize_en = 0;	
			}
		} else {
			boost_optimize.optimize_stop_flt = 0;
		}
	} else {
		boost_optimize.optimize_stop_flt = 0;
	} 	

	//跟随充使能检查
	if (!boost_optimize.boost_optimize_en) {
		boost_optimize.init_state = 0;		
		power_optimize.boost_lvl = TCFG_BOOST_VOUT_S;
		return;
	}
	
	//更新最低挡位，必须>=(vbat电压+100mV)
	vbat_volt = adc_get_voltage(AD_ANA_CH_VBAT_DIV_4, 8) * 4;
	if (vbat_volt <= 3700) {
		min_lvl = BOOST_VOUT_S_3800_MV;	
	} else {
		temp0 = (vbat_volt-3700)/100;
		temp1 = (vbat_volt-3700)%100;
		if (temp1) {
			min_lvl = BOOST_VOUT_S_3800_MV+temp0+1;
		} else {
			min_lvl = BOOST_VOUT_S_3800_MV+temp0;
		}
	}
	//log_put_u8(min_lvl); 

	if (b_sys_busy_in_boost) {
		if (!boost_optimize.init_state) {
			//初始化跟随充相关参数
			boost_optimize.init_state = 1;
			boost_optimize.add_flt = TCFG_ADJUST_ADD_FLT_TIMES;
			boost_optimize.sub_flt = TCFG_ADJUST_SUB_FLT_TIMES;
			boost_optimize.optimize_start_flt = TCFG_START_FLT_TIMES;
			boost_optimize.vout_curr[EAR_L] = curr_temp[EAR_L];
			boost_optimize.vout_curr[EAR_R] = curr_temp[EAR_R];
			boost_optimize.low_ch = EAR_MAX;	
		}  
	}

	if ((ear_info[EAR_L].online_hw && ear_info[EAR_R].online_hw) || (curr_temp[EAR_L] && curr_temp[EAR_R])) {
		//双耳在仓低电量耳机筛查
		if (boost_optimize.low_ch != EAR_MAX) {
			//已筛查出
			if (curr_temp[!boost_optimize.low_ch] > (curr_temp[boost_optimize.low_ch] + TCFG_LOW_CH_COMP_CURR)) {
				//之前被判定为大电量的耳机电流比小电量的耳机电流大5mA
				boost_optimize.low_ch_flt--;
				if (!boost_optimize.low_ch_flt) {
					//log_puts("low ch change 0\n");
					power_optimize.boost_lvl = TCFG_BOOST_VOUT_S;
					boost_optimize.low_ch = EAR_MAX;
					goto _adjust_boost;
				}
				log_putchar('\n');
				return;	
			} else if (boost_optimize.max_ch_curr && (curr_temp[!boost_optimize.low_ch] > (boost_optimize.max_ch_curr * 2))) {
				//之前被判定为大电量的耳机出现涓流转恒流，需要重新判定小电量耳机
				//若是满电被唤醒起来充电则不需要重新判定耳机
				//log_puts("low ch change 1\n");
				boost_optimize.max_ch_curr = curr_temp[!boost_optimize.low_ch];
				power_optimize.boost_lvl = TCFG_BOOST_VOUT_S;
				boost_optimize.low_ch = EAR_MAX;
				goto _adjust_boost;	
			} else {
				boost_optimize.low_ch_flt = TCFG_LOW_CH_FLT_TIMES;
				//更新以小电量耳机为调整
				adjust_ch = boost_optimize.low_ch;
				boost_optimize.max_ch_curr = curr_temp[!boost_optimize.low_ch];
				//大电量耳机不在充电但是物理在线，则让其关机，待调高boost挡位后再唤醒
				if (!curr_temp[!boost_optimize.low_ch] && !ear_info[!boost_optimize.low_ch].full_flag) {
					boost_optimize.full_ch_flt++;
    				if (boost_optimize.full_ch_flt >= TCFG_CH_FULL_FLT_TIMES) {
						boost_optimize.full_ch_flt = 0;
    					ear_info[!boost_optimize.low_ch].full_flag = 1;
						if (!boost_optimize.low_ch) {
							b_evt_ear1_full = 1;
						} else {
							b_evt_ear0_full = 1;
						}               
    				}
				} else {
					boost_optimize.full_ch_flt = 0;
				}	
			}
		} else {
			//未筛查出
			boost_optimize.low_ch_flt = TCFG_LOW_CH_FLT_TIMES;
			curr_diff = (curr_temp[EAR_L] >= curr_temp[EAR_R]) ? (curr_temp[EAR_L] - curr_temp[EAR_R]) : (curr_temp[EAR_R] - curr_temp[EAR_L]);
			if (curr_diff >= TCFG_LOW_CH_COMP_CURR) {
				//同boost挡位下，双耳电流差距超过TCFG_LOW_CH_COMP_CURR 
				boost_optimize.optimize_start_flt--;
				if (!boost_optimize.optimize_start_flt) {
					//找到低电量的耳机，开启跟随充
					boost_optimize.optimize_start_flt = TCFG_START_FLT_TIMES;
					boost_optimize.low_ch = (curr_temp[EAR_L] >= curr_temp[EAR_R]) ? EAR_L : EAR_R;
					power_optimize.boost_lvl = TCFG_BOOST_VOUT_S;
					//log_puts("low ch:");
					//log_put_u8(boost_optimize.low_ch);
					//log_putchar('\n');
					goto _adjust_boost;
				}
				log_putchar('\n');
				return;
			} else {
				boost_optimize.optimize_start_flt = TCFG_START_FLT_TIMES;
				if (((curr_temp[EAR_L] <= TCFG_STOP_ADJUST_CURR) && (curr_temp[EAR_R] <= TCFG_STOP_ADJUST_CURR)) || !power_optimize.boost_lvl) {
					//boost调至最低挡位仍未能筛查出低电量耳机，则默认左耳低电，开启跟随充
					//双耳电流都小于TCFG_STOP_ADJUST_CURR，则默认左耳低电，开启跟随充
					//log_puts("normal L\n");
					if (curr_temp[EAR_L]) {
						boost_optimize.low_ch = EAR_L;
					} else {
						boost_optimize.low_ch = EAR_R;	
					}
					power_optimize.boost_lvl = TCFG_BOOST_VOUT_S;
					goto _adjust_boost;
				} else {
					//同boost挡位下，双耳电流差距不大，继续往下调boost挡位
					power_optimize.boost_lvl--;
					goto _adjust_boost;
				} 
			}
		}
		log_putchar('\n'); 
	} else {
		//单耳在仓，通信时不充电
		if (curr_temp[EAR_L] || ear_info[EAR_L].online_hw) {
			adjust_ch = EAR_L;
		} else {
			adjust_ch = EAR_R;
		}
	}
	
	adjust_ear_now_curr = curr_temp[adjust_ch];
	adjust_ear_last_curr = boost_optimize.vout_curr[adjust_ch];

	//log_put_u8(power_optimize.boost_lvl);
	//log_put_u8(adjust_ear_now_curr);
	//log_put_u8(adjust_ear_last_curr);
	//log_put_u8(adjust_ch);
    //log_putchar('\n');

	//更新跟随的电流值
	if (power_optimize.boost_lvl == TCFG_BOOST_VOUT_S) {
		boost_optimize.adjust_time = TCFG_UPDATE_ADJUST_TIMES;
	} else {
		boost_optimize.adjust_time++;
	}
	if (boost_optimize.adjust_time == TCFG_UPDATE_ADJUST_TIMES) {
		boost_optimize.adjust_time = 0;
		if (power_optimize.boost_lvl != TCFG_BOOST_VOUT_S) {
			boost_set_volt(TCFG_BOOST_VOUT_S);
			delay_n2ms(2);
		}
		curr_change = vout_current_mA_get(EAR_L);
		if (curr_change) {
			boost_optimize.vout_curr[EAR_L]	= curr_change;
		}
		curr_change = vout_current_mA_get(EAR_R);
		if (curr_change) {
			boost_optimize.vout_curr[EAR_R]	= curr_change;
		}
		if (power_optimize.boost_lvl != TCFG_BOOST_VOUT_S) {
			boost_set_volt(power_optimize.boost_lvl);
		}
	}

	if (adjust_ear_now_curr < adjust_ear_last_curr) {
		//电流减小，boost挡位提高
		boost_optimize.sub_flt = TCFG_ADJUST_SUB_FLT_TIMES;
		boost_optimize.add_flt--;
		if (!boost_optimize.add_flt) {
			boost_optimize.add_flt = TCFG_ADJUST_ADD_FLT_TIMES;
			if (power_optimize.boost_lvl < TCFG_BOOST_VOUT_S) {
				power_optimize.boost_lvl++;
				//双耳在仓充电调高挡位，需要把休眠的大电量耳机唤醒起来充电
				if (boost_optimize.low_ch != EAR_MAX) {
					if (ear_info[!boost_optimize.low_ch].full_flag) {
						//log_puts("wkup ear");
						vout_pwr_ctrl(!boost_optimize.low_ch, 1);
						delay_n2ms(25);
						vout_pwr_ctrl(!boost_optimize.low_ch, 0);
						app_close_deal_ear_online(!boost_optimize.low_ch);
						ear_info[!boost_optimize.low_ch].force_charge = 0;	
					}
				}
				//防止调整耳机由于降低挡位导致退出充电
				if (!curr_temp[adjust_ch]) {
					//log_puts("wkup ear !");
					boost_set_volt(power_optimize.boost_lvl);
					vout_inserdet_en(adjust_ch, 0);
					vout_pwr_ctrl(adjust_ch, 0);
					delay_n2ms(150);
					vout_inserdet_en(adjust_ch, 1);
					vout_pwr_ctrl(adjust_ch, 1);
				}
			}  
		} else {
			return;
		}
	} else {
		//电流稳定，boost可以继续往下调
		boost_optimize.sub_flt--;
		boost_optimize.add_flt = TCFG_ADJUST_ADD_FLT_TIMES;
		if (!boost_optimize.sub_flt) {
			boost_optimize.sub_flt = TCFG_ADJUST_SUB_FLT_TIMES;
			if (adjust_ear_now_curr > TCFG_STOP_ADJUST_CURR) {
				if (power_optimize.boost_lvl > min_lvl) {
					power_optimize.boost_lvl--;
				} else {
					return;
				}
			}	
		} else {
			return;
		}	
	}

_adjust_boost:
	//更新挡位
	if (power_optimize.boost_lvl > TCFG_BOOST_VOUT_S) {
		power_optimize.boost_lvl = TCFG_BOOST_VOUT_S;
	}
	boost_set_volt(power_optimize.boost_lvl); 
}

#elif (TCFG_BOOST_OPTIMIZE_MODE == TCFG_MODE_FOLLOW_MAX)

//停止跟随充的耳机电流(一般设置比耳机恒流电流小15mA左右)
#define TCFG_STOP_ADJUST_CURR		(TCFG_EAR_CHARGE_CURRENT_MA - 15) 
//启动跟随充的耳机电流(一般设置比耳机恒流电流小10mA左右)
#define TCFG_START_ADJUST_CURR		(TCFG_EAR_CHARGE_CURRENT_MA - 10) 
#define TCFG_START_FLT_TIMES		5  // <=7，TCFG_START_FLT_TIMES*512mS
#define TCFG_STOP_FLT_TIMES			7  // <=7，TCFG_STOP_FLT_TIMES*512mS
#define TCFG_ADJUST_ADD_FLT_TIMES	3  // <=7，TCFG_ADJUST_ADD_FLT_TIMES*512mS
#define TCFG_ADJUST_SUB_FLT_TIMES	2  // <=7，TCFG_ADJUST_SUB_FLT_TIMES*512mS
#define TCFG_CH_FULL_FLT_TIMES		7  // <=15，TCFG_CH_FULL_FLT_TIMES*512mS
#define TCFG_LOW_CH_COMP_CURR		3  //3mA
#define TCFG_UPDATE_ADJUST_TIMES	60 // TCFG_UPDATE_ADJUST_TIMES*512mS
#define adjust_ear_now_curr			optimize_buf[0]	//耳机当前充电电流
#define adjust_ear_last_curr		optimize_buf[1]	//耳机上一次充电电流
#define curr_diff    				optimize_buf[2] //左右耳充电电流差值

void power_optimize_for_boost_voltage_adjust(void)
{
	u8 curr_temp[2];
	u8 adjust_ch;
	u8 curr_change;
	u16 vbat_volt;
	u8 min_lvl, temp0, temp1;

   	//没有在升压状态或者升压电压被锁存了，不开启跟随充
	if (!b_sys_busy_in_boost || boost_lock_volt) {
		boost_optimize.boost_optimize_en = 0;
		boost_optimize.init_state = 0;		
		power_optimize.boost_lvl = TCFG_BOOST_VOUT_S;
		return;
	}

	curr_temp[EAR_L] = vout_current_mA_get(EAR_L);
	//log_put_u8(curr_temp[EAR_L]);	
	curr_temp[EAR_R] = vout_current_mA_get(EAR_R);
	//log_put_u8(curr_temp[EAR_R]);

	if (!boost_optimize.boost_optimize_en) {
		//充电状态的耳机需都处于恒流状态才可开启跟随充
		if (ear_info[EAR_L].charge && (curr_temp[EAR_L] < TCFG_START_ADJUST_CURR)) {
			boost_optimize.boost_optimize_en = 0;
			return;
		} 
		if (ear_info[EAR_R].charge && (curr_temp[EAR_R] < TCFG_START_ADJUST_CURR)) {
			boost_optimize.boost_optimize_en = 0;
			return;
		}
		if (ear_info[EAR_L].charge || ear_info[EAR_R].charge) {
			boost_optimize.boost_optimize_en = 1;
		} 
	} 
	
	//跟随充退出检查，boost挡位在最高档持续时间超过TCFG_STOP_FLT_TIMES，且左右耳电流小于TCFG_STOP_ADJUST_CURR
	//注意这里如果两只耳机都非恒流状态时会退出跟随充，等有只耳机进入恒流之后再开启跟随充
	if (power_optimize.boost_lvl == TCFG_BOOST_VOUT_S) {
		if ((curr_temp[EAR_L] <= TCFG_STOP_ADJUST_CURR) && (curr_temp[EAR_R] <= TCFG_STOP_ADJUST_CURR)) {
			boost_optimize.optimize_stop_flt++;
			if (boost_optimize.optimize_stop_flt >= TCFG_STOP_FLT_TIMES) {
				boost_optimize.boost_optimize_en = 0;	
			}
		} else {
			boost_optimize.optimize_stop_flt = 0;
		}
	} else {
		boost_optimize.optimize_stop_flt = 0;
	} 	

	//跟随充使能检查
	if (!boost_optimize.boost_optimize_en) {
		boost_optimize.init_state = 0;		
		power_optimize.boost_lvl = TCFG_BOOST_VOUT_S;
		return;
	}
	
	//更新最低挡位，必须>=(vbat电压+100mV)
	vbat_volt = adc_get_voltage(AD_ANA_CH_VBAT_DIV_4, 8) * 4;
	if (vbat_volt <= 3700) {
		min_lvl = BOOST_VOUT_S_3800_MV;	
	} else {
		temp0 = (vbat_volt-3700)/100;
		temp1 = (vbat_volt-3700)%100;
		if (temp1) {
			min_lvl = BOOST_VOUT_S_3800_MV+temp0+1;
		} else {
			min_lvl = BOOST_VOUT_S_3800_MV+temp0;
		}
	}
	//log_put_u8(min_lvl);  

	if (b_sys_busy_in_boost) {
		if (!boost_optimize.init_state) {
			//初始化跟随充相关参数
			boost_optimize.init_state = 1;
			boost_optimize.add_flt = TCFG_ADJUST_ADD_FLT_TIMES;
			boost_optimize.sub_flt = TCFG_ADJUST_SUB_FLT_TIMES;
			boost_optimize.optimize_start_flt = TCFG_START_FLT_TIMES;
			boost_optimize.vout_curr[EAR_L] = curr_temp[EAR_L];
			boost_optimize.vout_curr[EAR_R] = curr_temp[EAR_R];
			boost_optimize.low_ch = EAR_MAX;	
		}  
	}

	if ((ear_info[EAR_L].online_hw && ear_info[EAR_R].online_hw) || (curr_temp[EAR_L] && curr_temp[EAR_R])) {
		//双耳在仓低电量耳机筛查
		if (boost_optimize.low_ch != EAR_MAX) {
			//已筛查出
			if (boost_optimize.vout_curr[!boost_optimize.low_ch] > (boost_optimize.vout_curr[boost_optimize.low_ch] + TCFG_LOW_CH_COMP_CURR)) {
				//之前被判定为大电量的耳机电流比小电量的耳机电流大
				//log_puts("low ch change 0\n");
				power_optimize.boost_lvl = TCFG_BOOST_VOUT_S;
				boost_optimize.low_ch = EAR_MAX;
				goto _adjust_boost;
			} else {
				//高电量耳机判满
				if (!curr_temp[!boost_optimize.low_ch] && !ear_info[!boost_optimize.low_ch].full_flag && (power_optimize.boost_lvl == TCFG_BOOST_VOUT_S)) {
					boost_optimize.full_ch_flt++;
    				if (boost_optimize.full_ch_flt >= TCFG_CH_FULL_FLT_TIMES) {
						boost_optimize.full_ch_flt = 0;
    					ear_info[!boost_optimize.low_ch].full_flag = 1;
						if (!boost_optimize.low_ch) {
							b_evt_ear1_full = 1;
						} else {
							b_evt_ear0_full = 1;
						}               
    				}
				} else {
					boost_optimize.full_ch_flt = 0;
				}

				if (ear_info[!boost_optimize.low_ch].full_flag) {
					//高电量耳机已经满电，开始跟随低电量耳机
					adjust_ch = boost_optimize.low_ch;
				} else {
					//更新以大电量耳机为调整
					adjust_ch = !boost_optimize.low_ch;
				}
			}
		} else {
			//未筛查出
			curr_diff = (curr_temp[EAR_L] >= curr_temp[EAR_R]) ? (curr_temp[EAR_L] - curr_temp[EAR_R]) : (curr_temp[EAR_R] - curr_temp[EAR_L]);
			if (curr_diff >= TCFG_LOW_CH_COMP_CURR) {
				//同boost挡位下，双耳电流差距超过TCFG_LOW_CH_COMP_CURR 
				boost_optimize.optimize_start_flt--;
				if (!boost_optimize.optimize_start_flt) {
					//找到低电量的耳机，开启跟随充
					boost_optimize.optimize_start_flt = TCFG_START_FLT_TIMES;
					boost_optimize.low_ch = (curr_temp[EAR_L] >= curr_temp[EAR_R]) ? EAR_L : EAR_R;
					power_optimize.boost_lvl = TCFG_BOOST_VOUT_S;
					//log_puts("low ch:");
					//log_put_u8(boost_optimize.low_ch);
					//log_putchar('\n');

					//防止高电量耳机电流被拉为0
					if (!curr_temp[!boost_optimize.low_ch]) {
						vout_inserdet_en(adjust_ch, 0);
						vout_pwr_ctrl(adjust_ch, 0);
						delay_n2ms(150);
						vout_inserdet_en(adjust_ch, 1);
						vout_pwr_ctrl(adjust_ch, 1);
					}
					goto _adjust_boost;
				}
				log_putchar('\n');
				return;
			} else {
				boost_optimize.optimize_start_flt = TCFG_START_FLT_TIMES;
				if (((curr_temp[EAR_L] <= TCFG_STOP_ADJUST_CURR) && (curr_temp[EAR_R] <= TCFG_STOP_ADJUST_CURR)) || !power_optimize.boost_lvl) {
					//boost调至最低挡位仍未能筛查出低电量耳机，则默认左耳低电，开启跟随充
					//双耳电流都小于TCFG_STOP_ADJUST_CURR，则默认左耳低电，开启跟随充
					//log_puts("normal L\n");
					if (curr_temp[EAR_L]) {
						boost_optimize.low_ch = EAR_L;
					} else {
						boost_optimize.low_ch = EAR_R;	
					}
					power_optimize.boost_lvl = TCFG_BOOST_VOUT_S;
					goto _adjust_boost;
				} else {
					//同boost挡位下，双耳电流差距不大，继续往下调boost挡位
					power_optimize.boost_lvl--;
					goto _adjust_boost;
				} 
			}
		}
		log_putchar('\n'); 
	} else {
		//单耳在仓，通信时不充电
		if (curr_temp[EAR_L] || ear_info[EAR_L].online_hw) {
			adjust_ch = EAR_L;
		} else {
			adjust_ch = EAR_R;
		}
	}
	
	adjust_ear_now_curr = curr_temp[adjust_ch];
	adjust_ear_last_curr = boost_optimize.vout_curr[adjust_ch];

	//log_put_u8(power_optimize.boost_lvl);
	//log_put_u8(adjust_ear_now_curr);
	//log_put_u8(adjust_ear_last_curr);
	//log_put_u8(adjust_ch);
    //log_putchar('\n');

	//更新跟随的电流值
	if (power_optimize.boost_lvl == TCFG_BOOST_VOUT_S) {
		boost_optimize.adjust_time = TCFG_UPDATE_ADJUST_TIMES;
	} else {
		boost_optimize.adjust_time++;
	}
	if (boost_optimize.adjust_time == TCFG_UPDATE_ADJUST_TIMES) {
		boost_optimize.adjust_time = 0;
		if (power_optimize.boost_lvl != TCFG_BOOST_VOUT_S) {
			boost_set_volt(TCFG_BOOST_VOUT_S);
			delay_n2ms(2);
		}
		curr_change = vout_current_mA_get(EAR_L);
		if (curr_change) {
			boost_optimize.vout_curr[EAR_L]	= curr_change;
		}
		curr_change = vout_current_mA_get(EAR_R);
		if (curr_change) {
			boost_optimize.vout_curr[EAR_R]	= curr_change;
		}
		if (power_optimize.boost_lvl != TCFG_BOOST_VOUT_S) {
			boost_set_volt(power_optimize.boost_lvl);
		}
	}

	if (adjust_ear_now_curr < adjust_ear_last_curr) {
		//电流减小，boost挡位提高
		boost_optimize.sub_flt = TCFG_ADJUST_SUB_FLT_TIMES;
		boost_optimize.add_flt--;
		if (!boost_optimize.add_flt) {
			boost_optimize.add_flt = TCFG_ADJUST_ADD_FLT_TIMES;
			if (power_optimize.boost_lvl < TCFG_BOOST_VOUT_S) {
				power_optimize.boost_lvl++;
				//双耳在仓充电调高挡位，没充电的小电量耳机唤醒起来充电
				if (boost_optimize.low_ch != EAR_MAX) {
					if (!curr_temp[!adjust_ch] && !ear_info[!adjust_ch].full_flag) {
						//log_puts("wkup ear 1\n");
						boost_set_volt(power_optimize.boost_lvl);
						vout_inserdet_en(!adjust_ch, 0);
						vout_pwr_ctrl(!adjust_ch, 0);
						delay_n2ms(150);
						vout_inserdet_en(!adjust_ch, 1);
						vout_pwr_ctrl(!adjust_ch, 1);
					}
				}
				//防止调整耳机由于降低挡位导致退出充电
				if (!curr_temp[adjust_ch]) {
					//log_puts("wkup ear 2\n");
					boost_set_volt(power_optimize.boost_lvl);
					vout_inserdet_en(adjust_ch, 0);
					vout_pwr_ctrl(adjust_ch, 0);
					delay_n2ms(150);
					vout_inserdet_en(adjust_ch, 1);
					vout_pwr_ctrl(adjust_ch, 1);
				}
			}  
		} else {
			return;
		}
	} else {
		//电流稳定，boost可以继续往下调
		boost_optimize.sub_flt--;
		boost_optimize.add_flt = TCFG_ADJUST_ADD_FLT_TIMES;
		if (!boost_optimize.sub_flt) {
			boost_optimize.sub_flt = TCFG_ADJUST_SUB_FLT_TIMES;
			if (adjust_ear_now_curr > TCFG_STOP_ADJUST_CURR) {
				if (power_optimize.boost_lvl > min_lvl) {
					power_optimize.boost_lvl--;
				} else {
					return;
				}
			}	
		} else {
			return;
		}	
	}

_adjust_boost:
	//更新挡位
	if (power_optimize.boost_lvl > TCFG_BOOST_VOUT_S) {
		power_optimize.boost_lvl = TCFG_BOOST_VOUT_S;
	}
	boost_set_volt(power_optimize.boost_lvl); 
}

#endif

#endif


