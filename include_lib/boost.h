#ifndef __BOOST_H__
#define __BOOST_H__

#include "typedef.h"

void boost_ctrl(u8 en);//1:enable 0:disable
void vout_pwr_ctrl(u8 ch, u8 en);
u8 get_vout_pwr_ctrl_en(u8 ch);
void boost_init(void);

u8 vout_current_mA_get(u8 ch);//0~150mA
void vout_current_sense_mode_set(u8 ch, u8 mode);//mode: 1:大电流模式 0:小电流模式
u8 vbat_discharge_current_mA_get(void);//用于过流检测

void boost_set_volt(u8 boost_volt);

void vin2vout(u8 en);//1:enable 0:disable

#endif