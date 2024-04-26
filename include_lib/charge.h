#ifndef __CHARGE_H__
#define __CHARGE_H__

#include "typedef.h"


void charge_set_charge_mA(u8 current_lvl);
u16 vbat_charge_current_mA_get(void);
void charge_ctrl(u8 en);//1:enable 0:disable
void charge_cc_check(void);
void charge_init(void);
void charge_set_charge_volt(u8 full_volt);

#endif