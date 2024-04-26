#ifndef __WPR_H__
#define __WPR_H__

#include "typedef.h"

extern __bit8 data wpr_st;
extern u16 wpr_voltage;
extern idata volatile u8 g_mux_buf[];//ȫ������buf

#define wpr_buf g_mux_buf

#define wpr_open_flag       wpr_st.u.b0 //�Ƿ�����߳�ģ��
#define wpr_fast_en         wpr_st.u.b1 //���ٷ�������ʹ��,ʹ�ܺ�128msһ��,����256msһ��,��ʹ��
#define wpr_fast_flag       wpr_st.u.b2 //��־λ

void get_signal_value(void);
void get_identification(void);
void get_configuration(void);
void get_control_error(s16 err_voltage);//wpc_voltage_normal - wpc_voltage
void get_rectified_value(void);
void get_end_value(void);

#endif

