#ifndef _IO_H_
#define _IO_H_

#include "typedef.h"

enum {
    IO_PORT_0_0,
    IO_PORT_0_1,
    IO_PORT_0_2,
    IO_PORT_0_3,
    IO_PORT_0_4,
    IO_PORT_0_5,
    IO_PORT_0_6,
    IO_PORT_0_7,

    IO_PORT_1_0,
    IO_PORT_1_1,
    IO_PORT_1_2,
    IO_PORT_1_3,
    IO_PORT_1_4,
    IO_PORT_1_5,
    IO_PORT_1_6,
    IO_PORT_1_7,

    IO_PORT_2_0,
    IO_PORT_2_1,
    IO_PORT_2_2,
    IO_PORT_2_3,
    IO_PORT_2_4,
    IO_PORT_2_5,

	NO_CONFIG_PORT = 0xFF,
};

enum {
    OUT_CH3_SEL_UART1_TX,
    OUT_CH3_SEL_SRC_CLK,
    OUT_CH3_SEL_WPR_TX1,
    OUT_CH3_SEL_PMU_OUT3,
};

enum {
    OUT_CH2_SEL_TIME1_PWM,
    OUT_CH2_SEL_SYS_CLK,
    OUT_CH2_SEL_LED2,
    OUT_CH2_SEL_PMU_OUT2,
};

enum {
    OUT_CH1_SEL_UART0_TX,
    OUT_CH1_SEL_WCLK,
    OUT_CH1_SEL_LED1,
    OUT_CH1_SEL_PMU_OUT1,
};

enum {
    OUT_CH0_SEL_TIME0_PWM,
    OUT_CH0_SEL_WPR_TX0,
    OUT_CH0_SEL_LED0,
    OUT_CH0_SEL_PMU_OUT0
};

enum {
    PPM_IOS_A_P24,
    PPM_IOS_B_P25,
    PPM_IOS_C_INVALID,
    PPM_IOS_D_INVALID,
};

enum {
    UART1_IOS_P11_P11,
    UART1_IOS_P14_P13,
    UART1_IOS_RX_INPUT_CH1,
    UART1_IOS_INVALID,
};

enum {
    UART0_IOS_P10_P10,
    UART0_IOS_P01_P00,
    UART0_IOS_RX_INPUT_CH0,
    UART0_IOS_RX_INPUT_CH1,
};

//IOMC0
#define UART1_IO_EN()           IOMC0 |=  BIT(7)
#define UART1_IO_DIS()          IOMC0 &= ~BIT(7)

#define UART0_IO_EN()           IOMC0 |=  BIT(6)
#define UART0_IO_DIS()          IOMC0 &= ~BIT(6)

#define PPM_OUTPUT_EN()         IOMC0 |=  BIT(5)
#define PPM_OUTPUT_DIS()        IOMC0 &= ~BIT(5)

#define WPR_TX1_OUTPUT_EN()     IOMC0 |=  BIT(4)
#define WPR_TX1_OUTPUT_DIS()    IOMC0 &= ~BIT(4)

#define WPR_TX0_OUTPUT_EN()     IOMC0 |=  BIT(3)
#define WPR_TX0_OUTPUT_DIS()    IOMC0 &= ~BIT(3)

#define LED2_OUTPUT_EN()        IOMC0 |=  BIT(2)
#define LED2_OUTPUT_DIS()       IOMC0 &= ~BIT(2)

#define LED1_OUTPUT_EN()        IOMC0 |=  BIT(1)
#define LED1_OUTPUT_DIS()       IOMC0 &= ~BIT(1)

#define LED0_OUTPUT_EN()        IOMC0 |=  BIT(0)
#define LED0_OUTPUT_DIS()       IOMC0 &= ~BIT(0)

#define LED_ALL_OUTPUT_EN()     IOMC0 |=  (BIT(0) | BIT(1) | BIT(2))
#define LED_ALL_OUTPUT_DIS()    IOMC0 &= ~(BIT(0) | BIT(1) | BIT(2))

//IOMC1
#define OTP_OUTPUT_EN()         IOMC1 |=  BIT(7)
#define OTP_OUTPUT_DIS()        IOMC1 &= ~BIT(7)

#define PWM1_OUTPUT_EN()        IOMC1 |=  BIT(6) //PWM1 输出到固定IO使能
#define PWM1_OUTPUT_DIS()       IOMC1 &= ~BIT(6)

#define PWM0_OUTPUT_EN()        IOMC1 |=  BIT(5) //PWM0 输出到固定IO使能
#define PWM0_OUTPUT_DIS()       IOMC1 &= ~BIT(5)

//IOMC2
#define OUT_CH3_SEL(x)          IOMC2 &= ~0xc0; IOMC2 |= (x<<6)
#define OUT_CH2_SEL(x)          IOMC2 &= ~0x30; IOMC2 |= (x<<4)
#define OUT_CH1_SEL(x)          IOMC2 &= ~0x0c; IOMC2 |= (x<<2)
#define OUT_CH0_SEL(x)          IOMC2 &= ~0x03; IOMC2 |= (x<<0)

//IOMC3
#define PWM_LED_IOS_A()         IOMC3 &= ~BIT(7)
#define PWM_LED_IOS_B()         IOMC3 |=  BIT(7)

#define PPM_FORCE_OUTPUT_EN()   IOMC3 |=  BIT(6)
#define PPM_FORCE_OUTPUT_DIS()  IOMC3 &= ~BIT(6)
#define PPM_IOS(x)              IOMC3 &= ~0x30; IOMC3 |= (x<<4)

#define UART1_IOS(x)            IOMC3 &= ~0x0c; IOMC3 |= (x<<2)
#define UART0_IOS(x)            IOMC3 &= ~0x03; IOMC3 |= (x<<0)

//IOMC4
#define OUT_CH3_TO_DIR()        IOMC4 |=  BIT(7) //OUTPUT CHANNLE信号控制IO的方向,选这个时跟固定IO推灯是一致的,输出的电平和IO OUT信号相关
#define OUT_CH3_TO_OUT()        IOMC4 &= ~BIT(7) //OUTPUT CHANNEL信号直接输出到对应IO上,和IO OUT的电平无关了。

#define OUT_CH2_TO_DIR()        IOMC4 |=  BIT(6)
#define OUT_CH2_TO_OUT()        IOMC4 &= ~BIT(6)

#define OUT_CH1_TO_DIR()        IOMC4 |=  BIT(5)
#define OUT_CH1_TO_OUT()        IOMC4 &= ~BIT(5)

#define OUT_CH0_TO_DIR()        IOMC4 |=  BIT(4)
#define OUT_CH0_TO_OUT()        IOMC4 &= ~BIT(4)

#define TMR1_CIN_IOS_P01()          IOMC4 &= ~BIT(3)
#define TMR1_CIN_IOS_INPUT_CH1()    IOMC4 |=  BIT(3)

#define TMR0_CIN_IOS_P03()          IOMC4 &= ~BIT(2)
#define TMR0_CIN_IOS_INPUT_CH0()    IOMC4 |=  BIT(2)

#define TMR1_CAP_IOS_P04()          IOMC4 &= ~BIT(1)
#define TMR1_CAP_IOS_INPUT_CH1()    IOMC4 |=  BIT(1)

#define TMR0_CAP_IOS_P07()          IOMC4 &= ~BIT(0)
#define TMR0_CAP_IOS_INPUT_CH0()    IOMC4 |=  BIT(0)

//IOMC1,5,6,7
#define IN_CH0_SEL(x)           IOMC1 &= ~0x1f; IOMC1 |= (x<<0)
#define IN_CH1_SEL(x)           IOMC5 &= ~0x1f; IOMC5 |= (x<<0)
#define IN_CH2_SEL(x)           IOMC6 &= ~0x1f; IOMC6 |= (x<<0)
#define IN_CH3_SEL(x)           IOMC7 &= ~0x1f; IOMC7 |= (x<<0)

//IOMC end----------------------------------------------------------

//设置方向,输出或者输入
#define __IO_DIR_OUT(i, x)  P##i##_DIR &= ~BIT(x)
#define __IO_DIR_IN(i, x)   P##i##_DIR |=  BIT(x)
#define __IO_DIR_AND(i, x)  P##i##_DIR &= x
#define __IO_DIR_OR(i, x)   P##i##_DIR |= x
#define IO_DIR_OUT(i, x)    __IO_DIR_OUT(i, x)
#define IO_DIR_IN(i, x)     __IO_DIR_IN(i, x)
#define IO_DIR_AND(i, x)    __IO_DIR_AND(i, x)
#define IO_DIR_OR(i, x)     __IO_DIR_OR(i, x)
#define gpio_set_dir(i, x, v) \
    if (v) { \
        IO_DIR_IN(i, x); \
    } else { \
        IO_DIR_OUT(i, x); \
    }

//设置数字/模拟输入
#define __IO_DIE_EN(i, x)   P##i##_DIE |=  BIT(x)
#define __IO_DIE_DIS(i, x)  P##i##_DIE &= ~BIT(x)
#define __IO_DIE_AND(i, x)  P##i##_DIE &= x
#define __IO_DIE_OR(i, x)   P##i##_DIE |= x
#define IO_DIE_EN(i, x)     __IO_DIE_EN(i, x)
#define IO_DIE_DIS(i, x)    __IO_DIE_DIS(i, x)
#define IO_DIE_AND(i, x)    __IO_DIE_AND(i, x)
#define IO_DIE_OR(i, x)     __IO_DIE_OR(i, x)
#define gpio_set_die(i, x, v) \
    if (v) { \
        IO_DIE_EN(i, x); \
    } else { \
        IO_DIE_DIS(i, x); \
    }

//设置强驱
#define __IO_HD_EN(i, x)    P##i##_HD |=  BIT(x)
#define __IO_HD_DIS(i, x)   P##i##_HD &= ~BIT(x)
#define __IO_HD_AND(i, x)   P##i##_HD &= x
#define __IO_HD_OR(i, x)    P##i##_HD |= x
#define IO_HD_EN(i, x)      __IO_HD_EN(i, x)
#define IO_HD_DIS(i, x)     __IO_HD_DIS(i, x)
#define IO_HD_AND(i, x)     __IO_HD_AND(i, x)
#define IO_HD_OR(i, x)      __IO_HD_OR(i, x)
#define gpio_set_hd(i, x, v) \
    if (v) { \
        IO_HD_EN(i, x); \
    } else { \
        IO_HD_DIS(i, x); \
    }

//设置上拉
#define __IO_PU_EN(i, x)    P##i##_PU |=  BIT(x)
#define __IO_PU_DIS(i, x)   P##i##_PU &= ~BIT(x)
#define __IO_PU_AND(i, x)   P##i##_PU &= x
#define __IO_PU_OR(i, x)    P##i##_PU |= x
#define IO_PU_EN(i, x)      __IO_PU_EN(i, x)
#define IO_PU_DIS(i, x)     __IO_PU_DIS(i, x)
#define IO_PU_AND(i, x)     __IO_PU_AND(i, x)
#define IO_PU_OR(i, x)      __IO_PU_OR(i, x)
#define gpio_set_pull_up(i, x, v) \
    if (v) { \
        IO_PU_EN(i, x); \
    } else { \
        IO_PU_DIS(i, x); \
    }

//设置下拉
#define __IO_PD_EN(i, x)    P##i##_PD |=  BIT(x)
#define __IO_PD_DIS(i, x)   P##i##_PD &= ~BIT(x)
#define __IO_PD_AND(i, x)   P##i##_PD &= x
#define __IO_PD_OR(i, x)    P##i##_PD |= x
#define IO_PD_EN(i, x)      __IO_PD_EN(i, x)
#define IO_PD_DIS(i, x)     __IO_PD_DIS(i, x)
#define IO_PD_AND(i, x)     __IO_PD_AND(i, x)
#define IO_PD_OR(i, x)      __IO_PD_OR(i, x)
#define gpio_set_pull_down(i, x, v) \
    if (v) { \
        IO_PD_EN(i, x); \
    } else { \
        IO_PD_DIS(i, x); \
    }

//设置IO输出电平
#define __IO_WRITE_H(i, x)  P##i##_OUT |=  BIT(x)
#define __IO_WRITE_L(i, x)  P##i##_OUT &= ~BIT(x)
#define __IO_TRRN(i, x)     P##i##_OUT ^=  BIT(x)
#define __IO_WRITE_AND(i, x) P##i##_OUT &= x
#define __IO_WRITE_OR(i, x)  P##i##_OUT |= x
#define IO_WRITE_H(i, x)    __IO_WRITE_H(i, x)
#define IO_WRITE_L(i, x)    __IO_WRITE_L(i, x)
#define IO_TRUN(i,x)        __IO_TRRN(i, x)
#define IO_WRITE_AND(i, x)  __IO_WRITE_AND(i, x)
#define IO_WRITE_OR(i, x)   __IO_WRITE_OR(i, x)
#define gpio_write(i, x, v) \
    if (v) { \
        IO_WRITE_H(i, x); \
    } else { \
        IO_WRITE_L(i, x); \
    }

//读取IO输入电平
#define __IO_READ(i, x)     (P##i##_IN & BIT(x))
#define IO_READ(i, x)       __IO_READ(i, x)
#define gpio_read(i, x)     IO_READ(i, x)

//设置OUTPUT CHANNLE 使能
#define __IO_OC_EN(i, x)    (P##i##_OCEN |=  BIT(x))
#define __IO_OC_DIS(i, x)   (P##i##_OCEN &= ~BIT(x))
#define __IO_OC_AND(i, x)   P##i##_OCEN &= x
#define __IO_OC_OR(i, x)    P##i##_OCEN |= x
#define IO_OC_EN(i, x)      __IO_OC_EN(i, x)
#define IO_OC_DIS(i, x)     __IO_OC_DIS(i, x)
#define IO_OC_AND(i, x)     __IO_OC_AND(i, x)
#define IO_OC_OR(i, x)      __IO_OC_OR(i, x)
#define gpio_set_oc_en(i, x, v) \
    if (v) { \
        IO_OC_EN(i, x); \
    } else { \
        IO_OC_DIS(i, x); \
    }

//设置输出重映射
#define gpio_output_channel_0(i, x, ch) \
    OUT_CH0_SEL(ch); \
    IO_DIR_OUT(i, x); \
    IO_PD_DIS(i, x); \
    IO_PU_DIS(i, x); \
    IO_OC_EN(i, x)

#define gpio_output_channel_1(i, x, ch) \
    OUT_CH1_SEL(ch); \
    IO_DIR_OUT(i, x); \
    IO_PD_EN(i, x); \
    IO_PU_DIS(i, x); \
    IO_OC_EN(i, x)

#define gpio_output_channel_2(i, x, ch) \
    OUT_CH2_SEL(ch); \
    IO_DIR_OUT(i, x); \
    IO_PD_DIS(i, x); \
    IO_PU_EN(i, x); \
    IO_OC_EN(i, x)

#define gpio_output_channel_3(i, x, ch) \
    OUT_CH3_SEL(ch); \
    IO_DIR_OUT(i, x); \
    IO_PD_EN(i, x); \
    IO_PU_EN(i, x); \
    IO_OC_EN(i, x)

#define gpio_output_channel_close(i, x) \
	IO_DIR_IN(i, x); \
    IO_PD_DIS(i, x); \
    IO_PU_DIS(i, x); \
    IO_OC_DIS(i, x)

#endif  /*  _IO_H_ */


