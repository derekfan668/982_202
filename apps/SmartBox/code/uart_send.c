#include "includes.h"
#include "app_config.h"
#include "uart_send.h"
#include "uart.h"
#include "app_var.h"
#include "hall_det.h"
#include "chargebox.h"
#include "chargebox_hw.h"								  
#include <string.h>

u8 notice_701_buff[11] = {0x55, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

extern void enter_hook(u8 chl);
extern void exit_hook(u8 chl);
extern u16 power_get_vbat_voltage(void);
extern void uart1_write(u8 idata *buf, u8 len);
extern u8 power_get_vbat_percent(void);
extern idata u8 uart1_buffer[15];
extern u8 uart1_rx_packet_over;
extern void uart1_tx_io_init(void);
extern void uart1_close(void);
extern void uart1_init(void);
void user_set_wakeup_701(void)
{
    gpio_set_dir(1,4,0);
    gpio_write(1,4,0);
    delay_n2ms(25);
    uart1_tx_io_init();
}

u8 wait_for_ack(u8 cmd_type)
{
    if(uart1_rx_packet_over){
        if((uart1_buffer[2] == cmd_type) && (uart1_buffer[4] == 0)){
            return 0;
        } else {
            return 1;
        }
    } else {
        return 2;
    }
}

bool box_into_boat_mode=0;
/*hall开关盖 同步状态*/
u8 app_hall_notice(u8 status)
{
    u8 ret = 0xff, err_resend_cnt = 0;
    if(box_into_boat_mode){
        return 0;
    }
    memset(&notice_701_buff[2], 0, 9);
    notice_701_buff[2] = UART_CMD_HALL;
    notice_701_buff[3] = OUT;
    notice_701_buff[4] = 0x05;                              //包长度
    notice_701_buff[5] = 0xff;                            //电池电压  默认不同步
    notice_701_buff[8] = (sys_info.localcharge << 2);       //仓本地充电状态
    if(ear_info[0].online_hw){                      
        notice_701_buff[6] = ear_info[0].power;             //获取到的L耳电量
        if(ear_info[0].charge){
            notice_701_buff[8] += (0x01 << 1);              //L耳充电状态
        }
    }else{
        notice_701_buff[6] = 0xff;                          //耳机不在线默认0xff的电量
    }
    if(ear_info[1].online_hw){
        notice_701_buff[7] = ear_info[1].power;             //获取到的L耳电量
        if(ear_info[1].charge){
            notice_701_buff[8] += 0x01;                     //R耳充电状态
        }
    }else{
        notice_701_buff[7] = 0xff;                          //耳机不在线默认0xff的电量
    }
    notice_701_buff[9] = status;                        //霍尔的状态  1开盖 2关盖
    log_puts("\nsend_hall_notice:");
    put_buf(notice_701_buff,sizeof(notice_701_buff));
    uart1_write(notice_701_buff, sizeof(notice_701_buff));
    return 0;
}

/*充电以及电量状态同步*/
u8 app_battery_notice(void)
{
    u8 ret = 0xff, err_resend_cnt = 0;
    if(box_into_boat_mode){
        return 0;
    }
    memset(&notice_701_buff[2], 0, 9);
    notice_701_buff[2] = UART_CMD_CHARGING;         
    notice_701_buff[3] = OUT;
    notice_701_buff[4] = 0x04;
    notice_701_buff[5] = 0xff;                            //仓本身电池电压，默认不同步
    notice_701_buff[8] = (sys_info.localcharge << 2);       //仓是否在充电
    if(ear_info[0].online_hw){                              //L是否在仓
        notice_701_buff[6] = ear_info[0].power;             //L耳电量
        if(ear_info[0].charge){                             //L耳是否充电
            notice_701_buff[8] += (0x01 << 1);    
        }
    }else{
        notice_701_buff[6] = 0xff;
    }
    if(ear_info[1].online_hw){                              //R是否在仓
        notice_701_buff[7] = ear_info[1].power;             //R耳电量
        if(ear_info[1].charge){                             //R耳是否充电
            notice_701_buff[8] += 0x01;    
        }
    }else{
        notice_701_buff[7] = 0xff;
    }
    log_puts("\nsend_battery_notice:");
    put_buf(notice_701_buff,sizeof(notice_701_buff));
    uart1_write(notice_701_buff, sizeof(notice_701_buff));
    return 0;
}

u8 app_f98_battery_adc_val_notice(void)
{
    u16 vbat_v = power_det_voltage();
    if(box_into_boat_mode){
        return 0;
    }
    /* log_puts(" \napp_f98_battery_adc_val_notice: 0x");
    log_put_u16(vbat_v);
    log_puts("\n"); */
    memset(&notice_701_buff[2], 0, 9);
    notice_701_buff[2] = UART_CMD_BATTERY_F98;         
    notice_701_buff[3] = OUT;
    notice_701_buff[4] = 0x03;
    notice_701_buff[5] = (u8)((vbat_v >> 8) & 0xff);             
    notice_701_buff[6] = (u8)(vbat_v & 0xff);      
    notice_701_buff[7] = sys_info.localcharge;      
    // notice_701_buff[2] = UART_CMD_NULL;//发两次无用的事件防止701进入低功耗
    // uart1_write(notice_701_buff, sizeof(notice_701_buff));
    // uart1_write(notice_701_buff, sizeof(notice_701_buff));
    notice_701_buff[2] = UART_CMD_BATTERY_F98;
    log_puts("\nsend_f98_battery_notice:");
    put_buf(notice_701_buff,sizeof(notice_701_buff));
    uart1_write(notice_701_buff, sizeof(notice_701_buff));
    return 0;
}

u8 app_custom_notice(void)
{
    u8 ret = 0xff, err_resend_cnt = 0;
    if(box_into_boat_mode){
        return 0;
    }
    memset(&notice_701_buff[2], 0, 9);
    notice_701_buff[2] = UART_CMD_USER_ADD;         
    notice_701_buff[3] = OUT;
    notice_701_buff[4] = 0x01;
    notice_701_buff[5] = 0x01;
    log_puts("\nsend_custom_notice:");
    put_buf(notice_701_buff,sizeof(notice_701_buff));
    uart1_write(notice_701_buff, sizeof(notice_701_buff));
    return 0;
}

extern void app_chargebox_api_send_pair(void);
extern idata u8 uart1_buffer[15];
extern u8 chargebox_api_send_tws_mac(void);
void packet_deal(void)
{
    switch(uart1_buffer[2])
    {
        case UART_CMD_PRODUCT_MODE:
            log_puts(" ENTER UART_CMD_PRODUCT_MODE");
            box_into_boat_mode = 1;
            b_discharge_boat_mode = 1;
            break;
        case UART_CMD_TWS_PAIR:
            if(uart1_buffer[5] == 0xac){
                b_evt_pair = 1;
    		}
            break;
		default:
			break;
	}
}

void tx_packet_ask(u8 cmd_type)
{
    memset(&notice_701_buff[2], 0, 9);
    notice_701_buff[2] = cmd_type;
    notice_701_buff[3] = OUT;
    notice_701_buff[4] = 0x00;
    uart1_write(notice_701_buff, sizeof(notice_701_buff));
}

int my_strncmp(const char* string1, const char* string2, size_t count)
{
    int res = 0;
    while (count-- != 0)
    {
        res = *string1 - *string2;
        if (res != 0)
            break;
        string1++;
        string2++;
    }
    return res;
}

extern u8 uart1_rx_packet_over;
extern idata u8 uart1_buffer[15];
void uart1_rx_packet_handle(void)
{
    u8 i;
    if(uart1_rx_packet_over){
        /* log_putchar('\n');
        log_puts("rx:");
        for(i = 0; i < 11; i++)
        {
            log_put_u8(uart1_buffer[i]);
        }
        log_putchar('\n'); */
        if(!(my_strncmp(uart1_buffer, notice_701_buff, 2))){
            tx_packet_ask(uart1_buffer[2]);
            uart1_rx_packet_over = 0;
            packet_deal();
        } else {
            /* log_puts("\nrx_err:\n"); */
        }
    } else {

    }
}

void uart1_send_tws_mac(u8 *mac_addr)
{
    memset(&notice_701_buff[2], 0, 9);
    notice_701_buff[2] = UART_CMD_MAC;
    notice_701_buff[3] = OUT;
    notice_701_buff[4] = 0x06;
    memcpy(&notice_701_buff[5], mac_addr, 6);
    uart1_write(notice_701_buff, sizeof(notice_701_buff));
}

u8 tws_mac_addr[6] = {0};
u8 chargebox_api_send_tws_mac(void)
{
    u8 ret,  ear_ch;
    //log_puts("\n hargebox_api_send_tws_mac\n");
    uart_write_len = 4;
    for (ear_ch = EAR_L; ear_ch < EAR_MAX; ear_ch++) {
        chargebox_buf[DATA_USR_START + 0] = 0xAA;
        chargebox_buf[DATA_USR_START + 1] = 0X10;
        chargebox_buf[DATA_USR_START + 2] = 0X00;
        chargebox_buf[DATA_USR_START + 3] = 0XBA;
        enter_hook(ear_ch);
        ret = chargebox_api_write_read(ear_ch);
        exit_hook(ear_ch);
        if(!(ear_ch)){
/*             put_buf(chargebox_buf,strlen(chargebox_buf));
            log_puts("\n hargebox_api_send_tws_mac %d %d\n",ret,chargebox_buf[DATA_OF_CMD]);
	        log_put_u8(ret);
            log_put_u8(chargebox_buf[DATA_OF_CMD]); */
            if(!(ret == 0) && (chargebox_buf[DATA_OF_CMD] == CMD_GET_COMM_MAC)){
                memcpy(tws_mac_addr, &chargebox_buf[5], 6);
            } else {
                // memset(tws_mac_addr, 0, 6);
                log_puts("\nL_get_tws_mac_error\n");
                return -1;
            }
        } else {
            put_buf(&chargebox_buf[5], 6);
            put_buf(tws_mac_addr,strlen(tws_mac_addr));
            if(!(ret == 0) && (chargebox_buf[DATA_OF_CMD] == CMD_GET_COMM_MAC)){
                if(memcmp(tws_mac_addr, &chargebox_buf[5], 6) == 0){
                    log_puts("\nuart1_send_tws_mac\n");
                    uart1_send_tws_mac(tws_mac_addr);
                } else {
                    log_puts("\nL & R TWS MAC NOT SAME\n");
                    return -3;
                }
            } else {
                log_puts("\nR_get_tws_mac_error\n");
                return -2;
            }
        }
    }
    return 0;
}