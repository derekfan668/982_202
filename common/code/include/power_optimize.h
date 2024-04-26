#ifndef _POWER_OPTIMIZE_H
#define _POWER_OPTIMIZE_H

#include "typedef.h"

extern idata volatile u8 g_mux_buf[];//ȫ������buf
#define optimize_buf g_mux_buf

u8 get_charge_optimize_end_flag(void);
void power_optimize_for_charge_current_adjust(void);

void power_optimize_for_boost_voltage_adjust(void);

typedef struct {
    u8 charge_lvl: 4; 
	//lvl : 0 1  2  3  4  5  6  7  8  9   10  11  12  13  14  15  
	//mA :  5 10 15 20 25 30 35 40 50 100 150 200 250 300 350 400
	u8 boost_lvl: 4;
	//lvl : 0 	1  	2  	3  	4  	5  	6  	7  	8  	9  	10  11  12  13  14  15
	//V : 	3.8 3.9 4.0 4.1 4.2 4.3 4.4 4.5 4.6 4.7 4.8 4.9 5.0 5.1 5.2 5.3
} __power_optimize;

extern __power_optimize power_optimize;

typedef struct {
	u8 vout_curr[2]; 			//��¼������һ��boost��λ�ĳ�����
	u8 low_ch: 2;				//��¼˫�����ʱ�ϵ͵�Ķ���(EAR_L/EAR_R)
	u8 optimize_stop_flt: 3;	//�رո�������
	u8 optimize_start_flt: 3;	//������������
	u8 add_flt: 3;				//add boost lvl flt
	u8 sub_flt: 3;				//sub boost lvl flt
	u8 init_state: 1;			//������ʼ����־
	u8 boost_optimize_en: 1;	//�����ʹ��
	u8 low_ch_flt: 4;			//˫�����ʱ���Ƚϵ͵��������
	u8 full_ch_flt: 4;			//˫�����ʱ���ߵ��������������
	u8 adjust_time;				//����׷�����ֵ����
	u8 max_ch_curr;				//��¼�����������ʵʱ����
} __boost_optimize;

extern __boost_optimize boost_optimize;

#endif