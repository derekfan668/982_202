#ifndef __KEY_H__
#define __KEY_H__

#include "typedef.h"

/*按键门槛值*/
#define KEY_FILTER_CNT          4   //按键按下滤波,设置小于6
#define KEY_LONG_CNT            75  //长按
#define KEY_HOLD_CNT            (KEY_LONG_CNT + 15) //hold
#define KEY_DELAY_CNT           35  //连按等待时间

#define NO_KEY                  0x01

/*多击按键使能*/
#define KEY_MULTI_CLICK_EN		1


void key_scan(void);
void key_init(void);

#endif


