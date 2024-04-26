#ifndef __PPM_H__
#define __PPM_H__

#include "typedef.h"

typedef struct {
    u8 start: 1;        //ppm���ֱ�־
    u8 port_sel: 1;     //����
    u8 div: 4;          //ppmʱ�ӷ�Ƶ
} PPM_INFO;

#define SUCC_CRC        0xff//0xff�򲻹���CRC,����ֵ����ҪCRCƥ��,��������Ķ�
#define PPM_SUPORT_P25	0

extern PPM_INFO data ppm_info;
extern idata volatile u8 g_mux_buf[];//ȫ������buf
#define ppm_buf g_mux_buf

//USB����ʱ,����handshake_start,������һ�ε���handshake_run
//USB����ʱ,����handshake_stop
//USB�����ڼ�,500ms����������һ�ε���handshake_run
#define handshake_start() 	ppm_info.start = 1
#define handshake_stop()	ppm_info.start = 0
void handshake_run(void);

void ppm_data_init(void);
u8 ppm_start(void);

#endif

