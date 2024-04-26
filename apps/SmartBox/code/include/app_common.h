#ifndef _APP_COMMON_H
#define _APP_COMMON_H

#include "typedef.h"
#include "app_config.h"
#include "power_det.h"

void app_auto_mode(void);
void app_auto_sleep(void);
void app_auto_sleep_reset(void);
void app_common_event_deal(void);
void app_common_evt_512ms(void);


#endif


