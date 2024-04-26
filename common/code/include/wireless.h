#ifndef __WIRELESS_H__
#define __WIRELESS_H__

#include "typedef.h"
#include "wpr.h"

void wireless_scan(void);
void wireless_api_open(void);
void wireless_api_close(void);
void wireless_api_128ms_run(void);
void wireless_init(void);

#endif

