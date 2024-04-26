#ifndef __UART_H__
#define __UART_H__

#include "typedef.h"

void uart_init(void);
void uart_uninit(void);
void putbyte(char a);
void putchar(u8 a);
void puts(char code *s);
void put_u8(u8 dat);
void put_u16(u16 dat);
void put_u32(u32 dat);
void put_buf(u8 idata *buf, u8 len);
void put_u8_dec(u8 dat);
void put_u16_dec(u16 dat);

#define DEBUG_EN	1

#if DEBUG_EN
#define log_puts(x)  		puts(x)
#define log_put_u8(x)		put_u8(x)
#define	log_put_u16(x)		put_u16(x)
#define	log_put_u32(x)		put_u32(x)
#define log_putchar(x)		putchar(x)
#define log_put_u8_dec(x)	put_u8_dec(x)
#define	log_put_u16_dec(x)	put_u16_dec(x)
#else
#define log_puts(x)  		
#define log_put_u8(x)		
#define	log_put_u16(x)	
#define	log_put_u32(x)	
#define log_putchar(x)
#define log_put_u8_dec(x)
#define	log_put_u16_dec(x)	
#endif

void uart1_init(void); 
void uart1_close(void);
void uart1_write(u8 idata *buf, u8 len);
#endif

