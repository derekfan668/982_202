#include "includes.h"
#include "app_config.h"
#include "app_var.h"
#include "chargebox.h"
#include "power_det.h"
#include "boost.h"
#include "power_optimize.h"
#include "uart.h"
#define TCFG_EAR_FULL_DET_LEVEL  100 //满电值

//进入通信时,需要关闭的一些模块
void enter_hook(u8 chl) 
{
	vout_pwr_ctrl(chl, 0);	
    log_putchar('+');
    uart_uninit();
}

//退出通信时,需要恢复的一些模块
void exit_hook(u8 chl) 
{
	if (ear_info[chl].charge && (!discharge_sw_check())) {
		vout_pwr_ctrl(chl, 1);
	}
    #if TCFG_DEBUG_ENABLE
        uart_init();
    #endif
}

//检测通信是否在线
void app_chargebox_api_check_online(u8 ch, u8 ret) 
{
    if (ret == TRUE) {
        if (ear_info[ch].online_comm == 0) {
            if (ch == EAR_L) {
                b_evt_ear0_com_in = 1;
            } else {
                b_evt_ear1_com_in = 1;
            }
        }
        ear_info[ch].online_comm = TCFG_EAR_OFFLINE_MAX;
    } else {
        if (ear_info[ch].online_comm) {
            ear_info[ch].online_comm--;
            if (ear_info[ch].online_comm == 0) {
                ear_info[ch].power = 0xff;
				if(ear_info[ch].online_hw == 0) {	 //物理不在线，耳机才真正out
					if (ch == EAR_L) {
	                	b_evt_ear0_com_out = 1;
	            	} else {
	                	b_evt_ear1_com_out = 1;
	            	}
				}
            }
        }
    }
}

//判断满电
void app_chargebox_api_check_ear_full(u8 ch) 
{
#if TCFG_BOOST_OPTIMIZE_ENABLE	
    if (ear_info[ch].full_flag || boost_optimize.boost_optimize_en) 
#else
	if (ear_info[ch].full_flag)
#endif
	{
        return;
    }

	if ((vout_current_mA_get(ch) > 5) && !ear_info[ch].online_comm) {
		return;
	} else if (!ear_info[ch].online_hw) {	  //物理不在线，通信也不在线
		goto _ear_full_check;
	} else if (ear_info[ch].online_comm) {
		//log_put_u8(ear_info[ch].power & 0x7f);
    	if ((ear_info[ch].power & 0x7f) == TCFG_EAR_FULL_DET_LEVEL) {
			goto _ear_full_check; 
    	} else {
        	ear_info[ch].full_cnt = 0;
    	}
	} 	 
	return;

_ear_full_check:
	ear_info[ch].full_cnt++;
    if (ear_info[ch].full_cnt >= TCFG_EAR_FULL_DET_CNT) {
    	ear_info[ch].full_flag = 1;
        ear_info[ch].full_cnt = 0;
		if (ch) {
			b_evt_ear1_full = 1;
		} else {
			b_evt_ear0_full = 1;
		}               
    }
}

//开盖电量交互
void app_chargebox_api_send_power_open(u8 ch) 
{
	u8 local_power, tws_power, ret;
    if (ear_info[ch].online_hw == 0) {
        return;
    }

    local_power = power_get_vbat_percent();
    tws_power = ear_info[!ch].power;
    if (sys_info.localcharge) {
        local_power |= BIT(7);
    }
    enter_hook(ch);
    ret = chargebox_api_send_power_open(ch, local_power, tws_power);
    exit_hook(ch);
    app_chargebox_api_check_online(ch, ret);
    if (ret == TRUE) {
        ear_info[ch].power = chargebox_buf[DATA_OF_POWER];
    } 
}

//盒盖电量交互
void app_chargebox_api_send_power_close(u8 ch) 
{
	u8 local_power, tws_power, ret;
    if (ear_info[ch].online_hw == 0) {
        return;
    }
    local_power = power_get_vbat_percent();
    tws_power = ear_info[!ch].power;
    if (sys_info.localcharge) {
        local_power |= BIT(7);
    }
    enter_hook(ch);
    ret = chargebox_api_send_power_close(ch, local_power, tws_power);
    exit_hook(ch);
    app_chargebox_api_check_online(ch, ret);
    if (ret == TRUE) {
        ear_info[ch].power = chargebox_buf[DATA_OF_POWER];
    }
	app_chargebox_api_check_ear_full(ch);
}

//关机指令
void app_chargebox_api_send_shutdown(u8 ch)
{
	u8 ret;
    if (ear_info[ch].online_hw == 0) {
        return;
    }
    enter_hook(ch);
    ret = chargebox_api_send_shut_down(ch);
    exit_hook(ch);
    app_chargebox_api_check_online(ch, ret);
}

//盒盖指令
void app_chargebox_api_send_close_lid(u8 ch) 
{
	u8 ear_number, ret;
    if (ear_info[ch].online_hw == 0) {
        return;
    }
    ear_number = 1;
    if (ear_info[!ch].online_hw) {
        ear_number = 2;
    }
    enter_hook(ch);
    ret = chargebox_api_send_close_lid(ch, ear_number);
    exit_hook(ch);
    app_chargebox_api_check_online(ch, ret);
}

//配对指令
void app_chargebox_api_send_pair(void) 
{
	u8 ret;
    if ((ear_info[EAR_L].online_comm == 0) || (ear_info[EAR_R].online_comm == 0)) {
        return;
    }
    enter_hook(EAR_L);
    enter_hook(EAR_R);
    ret = chargebox_api_exchange_addr();
    exit_hook(EAR_L);
    exit_hook(EAR_R);
    if (ret == TRUE) {
        sys_info.pair_succ = 1;
    }
}


