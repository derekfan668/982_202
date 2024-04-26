#include "includes.h"
#include "chargebox.h"

//单线串口驱动

void uart0_isr(void) interrupt UART0_INT
{
	UART0_CON1 |= BIT(4);
	//调用智能充电舱数据接收流程
	chargebox_api_recv_data(UART0_BUF);
}

void chargebox_send_sync(u8 ch) 
{
    if (ch == EAR_L) {
		gpio_set_dir(EAR_GROUP_L, EAR_PORT_L, 0);
        gpio_write(EAR_GROUP_L, EAR_PORT_L, 0);
        delay_n2ms(4);
        gpio_write(EAR_GROUP_L, EAR_PORT_L, 1);
    } else {
	    gpio_set_dir(EAR_GROUP_R, EAR_PORT_R, 0);
        gpio_write(EAR_GROUP_R, EAR_PORT_R, 0);
        delay_n2ms(4);
        gpio_write(EAR_GROUP_R, EAR_PORT_R, 1);
    }
}

void chargebox_open(u8 ch, u8 mode) 
{
	//连续写两次,第一次写高8bit,第二次写低8bit
	UART0_IO_DIS();
	UART0_BAUD = (UART_BAUD_SRC(TCFG_CHARGEBOX_HW_BAUD) & 0xff00) >> 8;
    UART0_BAUD = (UART_BAUD_SRC(TCFG_CHARGEBOX_HW_BAUD) & 0x00ff) >> 0;
    UART0_CON1 = BIT(5) | BIT(4);
    UART0_CON0 = BIT(3) | BIT(0);
	if (mode == MODE_SEND) {
		if (ch == EAR_L) {
			gpio_set_dir(EAR_GROUP_L, EAR_PORT_L, 0);
        	gpio_write(EAR_GROUP_L, EAR_PORT_L, 1);
			gpio_output_channel_1(EAR_GROUP_L, EAR_PORT_L, OUT_CH1_SEL_UART0_TX);
		} else {
			gpio_set_dir(EAR_GROUP_R, EAR_PORT_R, 0);
        	gpio_write(EAR_GROUP_R, EAR_PORT_R, 1);
			gpio_output_channel_1(EAR_GROUP_R, EAR_PORT_R, OUT_CH1_SEL_UART0_TX);
		}
	} else {
		if (ch == EAR_L) {
        	IN_CH0_SEL(EAR_IO_L);
			gpio_set_dir(EAR_GROUP_L, EAR_PORT_L, 1);
			gpio_set_die(EAR_GROUP_L, EAR_PORT_L, 1);
		} else {
			IN_CH0_SEL(EAR_IO_R);
			gpio_set_dir(EAR_GROUP_R, EAR_PORT_R, 1);
			gpio_set_die(EAR_GROUP_R, EAR_PORT_R, 1);
		}
		UART0_IOS(UART0_IOS_RX_INPUT_CH0);
    	UART0_IE(1);
	}
}

void chargebox_close(u8 ch)
{
    UART0_IOS(UART0_IOS_P10_P10);
    UART0_IE(0);
    UART0_CON0 = 0;
    UART0_CON1 = BIT(5) | BIT(4);
	IN_CH0_SEL(0);
	if (ch == EAR_L) {
		gpio_output_channel_close(EAR_GROUP_L, EAR_PORT_L);
		gpio_set_die(EAR_GROUP_L, EAR_PORT_L, 0);
		gpio_set_pull_up(EAR_GROUP_L, EAR_PORT_L, 1);
	} else {
		gpio_output_channel_close(EAR_GROUP_R, EAR_PORT_R);
		gpio_set_die(EAR_GROUP_R, EAR_PORT_R, 0);
		gpio_set_pull_up(EAR_GROUP_R, EAR_GROUP_R, 1);
	}
}

void chargebox_write(u8 idata *buf, u8 len) 
{	
	u8 i;
    for (i = 0; i < len; i++) {
        UART0_BUF = buf[i];
        while (!(UART0_CON1 & 0x80));
        UART0_CON1 |= BIT(5);
    }
}


