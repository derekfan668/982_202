#ifndef _APP_VAR_H
#define _APP_VAR_H

#include "typedef.h"
#include "app_config.h"
#include "burn.h"

#define APP_MODE_OPEN       0
#define APP_MODE_CLOSE      1
#define APP_MODE_LOWPOWER   2

//发送shutdown的个数	
#define TCFG_SEND_SHUT_DOWN_MAX  5 // <16
//发送closelid的个数 -- 需要 EAR_OFFLINE_MAX 大
#define TCFG_SEND_CLOSE_LID_MAX  5 // <16
//检测几次不在线后才认为耳机拔出
#define TCFG_EAR_OFFLINE_MAX     4 // <8
//收到几次满电后判定耳机满电
#define TCFG_EAR_FULL_DET_CNT	 2 // <4 
//自动关机计数
#define TCFG_LIFE_CNT_MAX		 8 // <16
//充电舱满电计数
#define TCFG_LOCAL_FULL_CNT_MAX  14 // <16 
//系统异常次数计数
#define TCFG_SYS_ABNORMAL_CNT_MAX  5 // <16 	 

typedef struct {
    u8 mode: 2;             //运行的模式(open close lowpower)
    u8 pair_status: 2;      //配对状态
    u8 pair_succ: 1;        //配对成功
    u8 reserve: 1;          //保留
    u8 bat_low: 1;          //低电标记
    u8 localcharge: 1;      //给舱充电标记

	u8 life_cnt: 6;         	//超时休眠
    u8 sys_abnormal: 1;    		//系统异常(vout ov_uv/voutx oc)
	u8 charge_cc_state: 1;  	//舱是否处于涓流充电状态

	u8 localfull_cnt: 4;		//仓满电计数
	u8 sys_abnormal_cnt: 4;		//系统异常次数
	u8 key_evt_pair_cnt;		    
} SYS_INFO;

typedef struct {
    u8 shut_cnt: 4;         //关机命令计数器
    u8 lid_cnt: 4;          //关盖命令计数器

    u8 online_comm: 3;      //通信在线
    u8 full_cnt: 2;         //充满计数
    u8 online_hw: 1;        //物理在线
    u8 full_flag: 1;        //充满标记--可通过通信或者是电流检测
    u8 charge: 1;           //在充电标记

	u8 reserve: 5;          //保留
	u8 charge_times: 3;		//充电时长/半小时
	u8 charge_cnt;			//充电计数/8s

	u8 power;               //耳机电量
	u8 force_charge;		//强制充电时间计数
} EAR_INFO;

extern __bit8 bdata device_st;
extern __bit8 bdata auto_sleep_en;
extern SYS_INFO data sys_info;
extern EAR_INFO data ear_info[2];
extern __bit8 bdata charge_sw;
extern __bit8 bdata discharge_sw;

//系统休眠判断
#define sys_auto_sleep_check()  auto_sleep_en.byte
#define b_sys_busy_in_charge    auto_sleep_en.u.b0  //系统在充电中,禁止进入sleep状态
#define b_sys_busy_in_boost     auto_sleep_en.u.b1  //系统在升压中,禁止进入sleep状态
#define b_sys_busy_in_led       auto_sleep_en.u.b2  //系统LED忙,禁止进入sleep状态
#define b_sys_busy_in_comm      auto_sleep_en.u.b3  //系统有数据未发送完成,禁止进入sleep状态
#define b_sys_busy_in_wireless  auto_sleep_en.u.b4  //系统无线充电在线，禁止进入sleep状态
#define b_sys_busy_in_usb_on	auto_sleep_en.u.b5  //系统USB在线，禁止进入sleep状态
#define b_sys_busy_in_vin2vout  auto_sleep_en.u.b6  //系统在vin2vout中，禁止进入sleep状态

//外设状态
#define hall_status         device_st.u.b0  //hall的状态
#define wpr_status          device_st.u.b1  //指示无线充电的状态
#define wpr_disable         device_st.u.b2  //指示无线充电是否使能,当有线充时关闭无线充电
#define ui_mutex            device_st.u.b3  //UI控制锁,切换UI时,中断不执行
#define wpr_common			device_st.u.b4  //指示无线充电在通信
#define usb_shake_flag      device_st.u.b5  //指示USB是否有抖动
#define boost_lock_volt     device_st.u.b6  //锁存升压电压标志
#define boost_start_unlock  device_st.u.b7	//判断升压启动过程锁存标志
//CHARGE开关 
#define charge_sw_check()  			charge_sw.byte
#define b_charge_disable	  		charge_sw.u.b0  //USB插入才能充电
#define b_charge_inside_ntc_err	  	charge_sw.u.b1  //内置ntc异常状态
#define b_charge_outside_ntc_err	charge_sw.u.b2  //外置ntc异常状态
#define b_charge_localfull	  		charge_sw.u.b3  //充电舱满电状态

//DISCHARGE开关 
#define discharge_sw_check()  		discharge_sw.byte
#define b_discharge_ear_full	  	discharge_sw.u.b0  //双耳满电/无耳充电
#define b_discharge_vout_ovuv	  	discharge_sw.u.b1  //vout ovuv异常状态
#define b_discharge_current_limit	discharge_sw.u.b2  //放电过流状态
#define b_discharge_inside_ntc_err	discharge_sw.u.b3  //内置ntc异常状态
#define b_discharge_outside_ntc_err	discharge_sw.u.b4  //外置ntc异常状态
#define b_discharge_disable			discharge_sw.u.b5  //合盖状态下才能使能
#define b_discharge_boat_mode		discharge_sw.u.b6  //合盖状态下才能使能

#endif    /*    _APP_VAR_H    */


