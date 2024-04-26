#ifndef _UI_H_
#define _UI_H_

#include "typedef.h"

//每个UI驱动都必须实现以下4个接口,没有的就写空函数
//1、ui_init
//2、ui_run_status
//3、ui_scan
//4、ui_uninit

enum {
    UI_STATUS_NULL,
	UI_STATUS_ALL_OFF,
    UI_STATUS_USB_IN,
    UI_STATUS_USB_OUT,
    UI_STATUS_BAT_FULL,
    UI_STATUS_EAR0_IN,
    UI_STATUS_EAR0_OUT,
    UI_STATUS_EAR1_IN,
    UI_STATUS_EAR1_OUT,
	UI_STATUS_OPEN_LID,
	UI_STATUS_CLOSE_LID,
	UI_STATUS_KEY_CLICK,
	UI_STATUS_SYS_ABNORMAL,
	UI_STATUS_UPDATE_EAR_L_POWER,
	UI_STATUS_UPDATE_EAR_R_POWER,
	UI_STATUS_UPDATE_VBAT_PERCENT,
	UI_STATTUS_SWITCH_MAC,

	//配对状态UI放后面
	UI_STATUS_PAIR_STOP,
	UI_STATUS_PAIR_PRE,
	UI_STATUS_PAIRING,
};

void ui_init(void);
void ui_uninit(void);
void ui_scan(void);
void ui_run_status(u8 status);

#endif  /*  _UI_H_ */



