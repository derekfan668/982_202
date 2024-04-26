#ifndef _TIMER_H_
#define _TIMER_H_

#include "typedef.h"

/*
    TMR PWMӦ��ʱ, PWM_EN=1ʱ��дCNT�ǲ���PWMռ�ձȼĴ���,PWM_EN=0ʱ��дCNT���ǲ�����ʱ����ֵ;
    �ӹ̶�IO���PWM�ź�ʱ,��Ҫ���� PWM0_OUTPUT_EN() �� PWM1_OUTPUT_EN()

    TMR ��Ƶ���ʱ�Ӳ��ܳ���ϵͳʱ��!����TMR������������
*/


#define TMR_PND         BIT(7)
#define TMR_CPND        BIT(6)
#define TMR_DIV0(x)     ((x & 0x03) << 4)
#define TMR_CLK(x)      (x << 2)
#define TMR_MODE(x)     (x << 0)

#define TMR_DIV1(x)     ((x >> 2) << 6) //only timer0
#define TMR_PWM_EN      BIT(0)
#define TMR_PWM_INV     BIT(1)

enum {
    TMR_DIV_1,
    TMR_DIV_4,
    TMR_DIV_16,
    TMR_DIV_64,
    TMR_DIV_256,    //only timer0
    TMR_DIV_1024,   //only timer0
    TMR_DIV_4096,   //only timer0
    TMR_DIV_16384,  //only timer0
};

enum {
    TMR0_SYS_CLK,   //ϵͳʱ��
    TMR0_IO_CLK,    //IOʱ��
    TMR0_WCLK,      //WCLK-31.25K
    TMR0_SRC_CLK,   //ϵͳʱ�ӷ�Ƶǰ��ʱ��
};

enum {
    TMR1_SYS_CLK,   //ϵͳʱ��
    TMR1_IO_CLK,    //IOʱ��
    TMR1_RCH_CLK,   //RCHΪ����оƬ��������ʱ��,����ֵΪ28M
    TMR1_LRC_CLK,   //LRCʱ��,��Լ100K
};

enum {
    TMR_MODE_CLOSE,
    TMR_MODE_CNT,
    TMR_MODE_RISE_CAP,
    TMR_MODE_FALL_CAP,
};

extern data u8 timer_8s_cnt;

void timer0_init(void);

#endif  /*  _TIMER_H_ */



