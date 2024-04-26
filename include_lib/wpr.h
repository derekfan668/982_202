#ifndef __WPR_H__
#define __WPR_H__

#include "typedef.h"

extern __bit8 data wpr_st;
extern u16 wpr_voltage;
extern idata volatile u8 g_mux_buf[];//全局数据buf

#define wpr_buf g_mux_buf

#define wpr_open_flag       wpr_st.u.b0 //是否打开无线充模块
#define wpr_fast_en         wpr_st.u.b1 //快速反馈误差包使能,使能后128ms一次,否则256ms一次,库使用
#define wpr_fast_flag       wpr_st.u.b2 //标志位

void get_signal_value(void);
void get_identification(void);
void get_configuration(void);
void get_control_error(s16 err_voltage);//wpc_voltage_normal - wpc_voltage
void get_rectified_value(void);
void get_end_value(void);

#endif

