#ifndef __KEY_H__
#define __KEY_H__

#include "typedef.h"

/*�����ż�ֵ*/
#define KEY_FILTER_CNT          4   //���������˲�,����С��6
#define KEY_LONG_CNT            75  //����
#define KEY_HOLD_CNT            (KEY_LONG_CNT + 15) //hold
#define KEY_DELAY_CNT           35  //�����ȴ�ʱ��

#define NO_KEY                  0x01

/*�������ʹ��*/
#define KEY_MULTI_CLICK_EN		1


void key_scan(void);
void key_init(void);

#endif


