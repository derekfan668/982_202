#ifndef _APP_OPEN_H
#define _APP_OPEN_H

#include "typedef.h"
#include "app_config.h"

void app_open(void);

#define KEY_PAIR_CNT   10

extern data volatile u8 goto_pair_cnt;//配对功能按键时间计数
extern data volatile u8 auto_exit_cnt;//退出开盖通信模式计数

#endif


