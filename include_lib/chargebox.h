#ifndef __CHARGEBOX_H__
#define __CHARGEBOX_H__

#include "typedef.h"
#include "chargebox_hw.h"

//用户可以定私有命令,命令号 >= 0xC0
#define CMD_USER                    0xC0
#define CMD_GET_COMM_MAC            0xAA

#define DATA_USR_START              3 //用户数据起始位置,不可改动
#define DATA_OF_CMD                 (DATA_USR_START + 0) //命令偏移
#define DATA_OF_POWER               (DATA_USR_START + 1) //电量偏移
#define chargebox_buf               g_mux_buf
#define PACKET_SIZE                 (15-5)//(12 - 5)

extern idata volatile u8 g_mux_buf[];    //全局数据buf
extern data volatile u8 uart_write_len;   //发送数据长度变量
extern data volatile u8 uart_read_len;    //接收数据长度变量

//解码或者交换地址使用
extern __bit8 data chargebox_bits;
#define g_is_encryption chargebox_bits.u.b0
#define g_enc_type      chargebox_bits.u.b1
#define g_now_type      chargebox_bits.u.b2
#define g_recv_data     chargebox_bits.u.b3

#define g_first_flag    chargebox_bits.u.b4
#define g_stop_flag     chargebox_bits.u.b5
#define g_read_lr       chargebox_bits.u.b6
#define g_write_lr      chargebox_bits.u.b7

void chargebox_api_recv_data(u8 data_);
u8 chargebox_api_write_read(u8 ch);
u8 chargebox_api_send_power_open(u8 ch, u8 local_power, u8 tws_power);
u8 chargebox_api_send_power_close(u8 ch, u8 local_power, u8 tws_power);
u8 chargebox_api_send_close_lid(u8 ch, u8 ear_number);
u8 chargebox_api_send_ear_L_or_R(u8 ch);
u8 chargebox_api_send_shut_down(u8 ch);
u8 chargebox_api_send_restore_sys(u8 ch);
u8 chargebox_api_send_enter_dut(u8 ch);
u8 chargebox_api_delete_tws_addr(u8 ch);
u8 chargebox_api_delete_phone_addr(u8 ch);
u8 chargebox_delete_all_addr(u8 ch);
u8 chargebox_api_exchange_addr(void);

void app_chargebox_api_check_online(u8 ch, u8 ret);
void app_chargebox_api_check_ear_full(u8 ch);

#endif

