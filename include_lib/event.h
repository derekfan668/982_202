#ifndef __EVENT_H__
#define __EVENT_H__

#include "typedef.h"

extern __bit8 bdata sys_event0;
extern __bit8 bdata sys_event1;
extern __bit8 bdata sys_event2;
extern __bit8 bdata sys_event3;

#define b_evt_wpr_128ms     sys_event0.u.b0
#define b_evt_256ms         sys_event0.u.b1
#define b_evt_512ms         sys_event0.u.b2
#define b_evt_8s            sys_event0.u.b3

#define b_evt_ear0_com_in   sys_event0.u.b4 //耳机0通信在线事件
#define b_evt_ear0_com_out  sys_event0.u.b5	//耳机0通信离线事件
#define b_evt_ear1_com_in   sys_event0.u.b6	//耳机1通信在线事件
#define b_evt_ear1_com_out  sys_event0.u.b7	//耳机1通信离线事件

#define b_evt_hall          sys_event1.u.b0 //hall开合盖事件
#define b_evt_2ms           sys_event1.u.b1 
#define b_evt_usb_in        sys_event1.u.b2 //USB插入事件
#define b_evt_usb_out       sys_event1.u.b3 //USB拔出事件
#define b_evt_vbat_full     sys_event1.u.b4 //舱充满事件
#define b_evt_vbat_low      sys_event1.u.b5 //舱低电事件
#define b_evt_wireless_on   sys_event1.u.b6	//无线充5V在线事件
#define b_evt_wireless_off  sys_event1.u.b7	//无线充5V离线事件

#define b_evt_key_long      sys_event2.u.b0 //长按
#define b_evt_key_hold      sys_event2.u.b1 //hold住
#define b_evt_key_up        sys_event2.u.b2 //抬起
#define b_evt_key_click     sys_event2.u.b3 //单击
#define b_evt_key_double    sys_event2.u.b4 //双击
#define b_evt_key_triple    sys_event2.u.b5 //3击
#define b_evt_key_fourth    sys_event2.u.b6 //4击
#define b_evt_key_firth     sys_event2.u.b7 //5击
#define byte_key_evt        sys_event2.byte
#define KEY_CLICK_OFFSET    3
	
#define b_evt_ear0_full  	sys_event3.u.b0	//耳机0满电事件
#define b_evt_ear1_full  	sys_event3.u.b1	//耳机1满电事件

#define b_evt_ntc_charge   	  sys_event3.u.b2 //ntc充电状态变化事件
#define b_evt_ntc_discharge   sys_event3.u.b3 //ntc放电状态变化事件

#define b_evt_wireless_ov   sys_event3.u.b4	//无线充过压事件
#define b_evt_pair          sys_event3.u.b5	//无线充过压事件

#define app_event_call(evt, func) \
    if (evt) { \
        evt = 0; \
        func(); \
    }

#define app_event_call_2(evt, func0, func1) \
    if (evt) { \
        evt = 0; \
        func0(); \
		func1(); \
    }

#endif


