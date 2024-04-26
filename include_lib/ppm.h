#ifndef __PPM_H__
#define __PPM_H__

#include "typedef.h"

typedef struct {
    u8 start: 1;        //ppm握手标志
    u8 port_sel: 1;     //出口
    u8 div: 4;          //ppm时钟分频
} PPM_INFO;

#define SUCC_CRC        0xff//0xff则不关心CRC,其他值则需要CRC匹配,请勿随意改动
#define PPM_SUPORT_P25	0

extern PPM_INFO data ppm_info;
extern idata volatile u8 g_mux_buf[];//全局数据buf
#define ppm_buf g_mux_buf

//USB上线时,调用handshake_start,并握手一次调用handshake_run
//USB下线时,调用handshake_stop
//USB在线期间,500ms与充电线握手一次调用handshake_run
#define handshake_start() 	ppm_info.start = 1
#define handshake_stop()	ppm_info.start = 0
void handshake_run(void);

void ppm_data_init(void);
u8 ppm_start(void);

#endif

