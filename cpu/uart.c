#include "includes.h"
#include "app_config.h"
#include "uart_send.h"
#include <string.h>
#if TCFG_DEBUG_ENABLE

#if (TCFG_DEBUG_USED_UARTX == 0)
#define DEBUG_CON0          UART0_CON0
#define DEBUG_CON1          UART0_CON1
#define DEBUG_BAUD          UART0_BAUD
#define DEBUG_BUF           UART0_BUF
#define DEBUG_IO_DIS()      UART0_IO_DIS()
#else
#define DEBUG_CON0          UART1_CON0
#define DEBUG_CON1          UART1_CON1
#define DEBUG_BAUD          UART1_BAUD
#define DEBUG_BUF           UART1_BUF
#define DEBUG_IO_DIS()      UART1_IO_DIS()
#endif

void uart_init(void) 
{
    DEBUG_IO_DIS();
	DEBUG_BAUD = (UART_BAUD_DIVS3_SRC(TCFG_DEBUG_BAUD) & 0xff00) >> 8;
    DEBUG_BAUD = (UART_BAUD_DIVS3_SRC(TCFG_DEBUG_BAUD) & 0x00ff) >> 0;
    DEBUG_CON1 = BIT(4) | BIT(5);
    DEBUG_CON0 = BIT(0) | BIT(4);	//DIV3
#if (TCFG_DEBUG_USED_UARTX == 0)
    gpio_output_channel_1(DEBUG_GROUP, DEBUG_PORT, OUT_CH1_SEL_UART0_TX);
#else
    gpio_output_channel_3(DEBUG_GROUP, DEBUG_PORT, OUT_CH3_SEL_UART1_TX);
#endif
    DEBUG_BUF = 0;
}

void uart_uninit(void) 
{
    DEBUG_CON0 = 0;
    DEBUG_CON1 = BIT(4) | BIT(5);
    gpio_output_channel_close(DEBUG_GROUP, DEBUG_PORT);
}

void putbyte(char a) 
{
    if ((DEBUG_CON0 & BIT(0)) == 0) {
        return;
    }
    while (!(DEBUG_CON1 & 0x80));
    DEBUG_CON1 |= BIT(5);
    DEBUG_BUF = a;
}

void putchar(u8 a) 
{
    if (a == '\n') {
        putbyte(0x0d);
        putbyte(0x0a);
    } else {
        putbyte(a);
    }
}

void puts(char code *p) 
{
    while (*p) {
        putchar(*p++);
    }
}

void get_hex_data(u8 dat)
{
    u8 dat0;
    if (dat < 0x0a) {
        dat0 = dat + '0';
    } else {
        dat0 = dat + 'A' - 10;
    }
    putchar(dat0);
}

void put_u8(u8 dat)
{					
    get_hex_data(dat >> 4);
    get_hex_data(dat & 0xf);
    putchar(' ');
}

void put_u16(u16 dat)
{
    get_hex_data(dat >> 12);
    get_hex_data((dat >> 8) & 0xf);
    get_hex_data((dat >> 4) & 0xf);
    get_hex_data(dat & 0xf);
    putchar(' ');
}

void put_u32(u32 dat)
{
    u8 idata *p = (u8 *)&dat;
    get_hex_data(p[0] >> 4);
    get_hex_data(p[0] & 0xf);
    get_hex_data(p[1] >> 4);
    get_hex_data(p[1] & 0xf);
    get_hex_data(p[2] >> 4);
    get_hex_data(p[2] & 0xf);
    get_hex_data(p[3] >> 4);
    get_hex_data(p[3] & 0xf);
    putchar(' ');
}

void put_buf(u8 idata *buf, u8 len)
{
    u8 i;
    for (i = 0; i < len; i++) {
        if ((i % 16) == 0) {
            putchar('\n');
        }
        put_u8(buf[i]);
    }
}

void put_u8_dec(u8 dat)
{
	get_hex_data((dat/100));
	get_hex_data((dat%100/10));
	get_hex_data((dat%10));
	putchar(' ');
}

void put_u16_dec(u16 dat)
{
	get_hex_data((dat/10000));
	get_hex_data((dat%10000/1000));
	get_hex_data((dat%1000/100));
	get_hex_data((dat%100/10));
	get_hex_data((dat%10));
	putchar(' ');
}

#endif
#if TCFG_UART1_ENABLE
#define UART1_PACKET_SIZE  11
idata u8 uart1_buffer[UART1_PACKET_SIZE];

void uart1_rx_io_init(void)
{
    gpio_set_dir(1, 3, 1);
    gpio_set_die(1, 3, 1);
    gpio_set_pull_up(1, 3, 1);
    gpio_set_pull_down(1, 3, 0);
    gpio_set_hd(1, 3, 0);
}

void uart1_tx_io_init(void)
{
    gpio_set_dir(1, 4, 0);
    gpio_set_die(1, 4, 1);
    gpio_set_pull_up(1, 4, 0);
    gpio_set_pull_down(1, 4, 0);
    gpio_set_hd(1, 4, 0);
}

void uart1_init(void) 
{
    // log_puts("uart1\n");
    UART1_IO_EN();
    UART1_IOS(UART1_IOS_P14_P13); //B group
	UART1_BAUD = (UART_BAUD_DIVS3_SRC(TCFG_UART1_BAUD) & 0xff00) >> 8;
    UART1_BAUD = (UART_BAUD_DIVS3_SRC(TCFG_UART1_BAUD) & 0x00ff) >> 0;
    UART1_CON1 = BIT(4) | BIT(5);
    UART1_CON0 = BIT(4) | BIT(0) | BIT(3);
    uart1_rx_io_init();
    uart1_tx_io_init();
    UART1_IE(1);
    UART1_IP(1);
    UART1_BUF = 0;
}

void uart1_close()
{
    // log_puts("uart1_close\n");
    UART1_IE(0);
    UART1_CON0 = 0;
    UART1_CON1 = BIT(5) | BIT(4);
}

void uart1_write(u8 idata *buf, u8 len) 
{	
	u8 i;
    // log_puts("uart1_write\n");
    for (i = 0; i < len; i++) {
        // delay_n2ms(1); //���ӻᶪ��һ�ֽ�
        UART1_BUF = buf[i];
        while (!(UART1_CON1 & 0x80)){
        }
        UART1_CON1 |= BIT(5);
    }
}

data u8 uart1_rx_cnt = 0;
u8 uart1_rx_packet_over = 0;
extern u8 notice_701_buff[11];
#define UART1_MARK0  0x55
#define UART1_MARK1  0xAA
static void uart1_recv_data(u8 data_) //isr
{
	if(uart1_rx_packet_over == 0) {
	    if (uart1_rx_cnt == 0) {
	        if ((data_ == UART1_MARK0)) {
	            uart1_buffer[0] = data_;
	            uart1_rx_cnt++;
	        } 
	    } else if (uart1_rx_cnt == 1) {
	        if (data_ == UART1_MARK1) {
	            uart1_buffer[1] = data_;
	            uart1_rx_cnt++;
	        }
	    } else {
	        uart1_buffer[uart1_rx_cnt] = data_;
	        uart1_rx_cnt++;
			if(uart1_rx_cnt == uart1_buffer[4] + 5) {
				uart1_rx_cnt = 0;
				uart1_rx_packet_over = 1;
			} else {

			}
	    }
		return;
	}   
__redo_recv:
    uart1_rx_cnt = 0;
	return;
	 
}

void uart1_isr(void) interrupt UART1_INT
{
	UART1_CON1 |= BIT(4);
	// log_putchar('/');        //中断里不能加打印，调试可以打开，调试完记得关闭
    uart1_recv_data(UART1_BUF);
}

#endif
