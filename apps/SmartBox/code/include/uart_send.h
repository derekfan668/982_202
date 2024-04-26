#ifndef _APP_UART_SEND_H
#define _APP_UART_SEND_H

#include "typedef.h"
#define get_hall_status()    (gpio_read(HALL_GROUP, HALL_PORT) ? 1 : 0)
enum cmd{
    UART_CMD_BATTERY,
    UART_CMD_CHARGING,
    UART_CMD_HALL,
    UART_CMD_MAC,
    UART_CMD_BATTERY_F98,
    UART_CMD_BUTTON,
    UART_CMD_TWS_PAIR,
    UART_CMD_PRODUCT_MODE,   //701仓储模式
    UART_CMD_USER_ADD,
    
    UART_CMD_NULL = 0xFF,
};

enum dir{
    OUT = 0,
    IN,
};

enum key{
    NONE = 0,
    CLICK,
    LONG,
};

u8 app_hall_notice(u8 status);
u8 app_battery_notice(void);
u8 app_f98_battery_adc_val_notice(void);
void app_chargebox_api_send_pair(void);

#endif
