#ifndef _INT_H_
#define _INT_H_

#include "sfr.h"

/******************************************************/
/*
                Interrupt
                优先级1 高于 优先级0
*/
/******************************************************/
#define TIMER0_INT		    0
#define Timer0_IE(x)		ET0 = x
#define Timer0_IP(x)	    IPCON0L = (IPCON0L & ~BIT(0)) | ((x & 0x1)<<0)

#define TIMER1_INT		    1
#define Timer1_IE(x)		ET1 = x
#define Timer1_IP(x)	    IPCON0L = (IPCON0L & ~BIT(1)) | ((x & 0x1)<<1)

#define SARADC_INT		    2
#define SARADC_IE(x)		EADC = x
#define SARADC_IP(x)	    IPCON0L = (IPCON0L & ~BIT(2)) | ((x & 0x1)<<2)

#define LVD_INT		        3
#define LVD_IE(x)		    ELVD = x
#define LVD_IP(x)	        IPCON0L = (IPCON0L & ~BIT(3)) | ((x & 0x1)<<3)

#define WDT_INT             4
#define WDT_IE(x)			EWDT = x
#define WDT_IP(x)		    IPCON0L = (IPCON0L & ~BIT(4)) | ((x & 0x1)<<4)

#define PORT_INT	        5
#define PORT_IE(x)		    EPORT = x
#define PORT_IP(x)		    IPCON0L = (IPCON0L & ~BIT(5)) | ((x & 0x1)<<5)

#define PMU_INT		        6
#define PMU_IE(x)		    EPMU = x
#define PMU_IP(x)           IPCON0L = (IPCON0L & ~BIT(6)) | ((x & 0x1)<<6)

#define WPR_INT 		    8
#define WPR_IE(x)		    IE1 = (IE1 & ~BIT(0)) | ((x & 0x1)<<0)
#define WPR_IP(x)	        IPCON1L = (IPCON1L & ~BIT(0)) | ((x & 0x1)<<0)

#define UART0_INT		    9
#define UART0_IE(x)		    IE1 = (IE1 & ~BIT(1)) | ((x & 0x1)<<1)
#define UART0_IP(x)	        IPCON1L = (IPCON1L & ~BIT(1)) | ((x & 0x1)<<1)

#define UART1_INT		    10
#define UART1_IE(x)		    IE1 = (IE1 & ~BIT(2)) | ((x & 0x1)<<2)
#define UART1_IP(x)	        IPCON1L = (IPCON1L & ~BIT(2)) | ((x & 0x1)<<2)

#define PPM_INT		        11
#define PPM_IE(x)		    IE1 = (IE1 & ~BIT(3)) | ((x & 0x1)<<3)
#define PPM_IP(x)	        IPCON1L = (IPCON1L & ~BIT(3)) | ((x & 0x1)<<3)

#define PWM_LED_INT         12
#define PWM_LED_IE(x)		IE1 = (IE1 & ~BIT(4)) | ((x & 0x1)<<4)
#define PWM_LED_IP(x)		IPCON1L = (IPCON1L & ~BIT(4)) | ((x & 0x1)<<4)

#define PCNT_INT	        13
#define PCNT_IE(x)		    IE1 = (IE1 & ~BIT(5)) | ((x & 0x1)<<5)
#define PCNT_IP(x)		    IPCON1L = (IPCON1L & ~BIT(5)) | ((x & 0x1)<<5)

#endif  /*  _INT_H_ */

