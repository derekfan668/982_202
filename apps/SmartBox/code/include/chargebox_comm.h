#ifndef _CHARGEBOX_COMM_H
#define _CHARGEBOX_COMM_H

#include "typedef.h"
#include "chargebox.h"

void app_chargebox_api_send_power_open(u8 ch);
void app_chargebox_api_send_power_close(u8 ch);
void app_chargebox_api_send_shutdown(u8 ch);
void app_chargebox_api_send_close_lid(u8 ch);
void app_chargebox_api_send_pair(void);

void app_chargebox_get_comm_mac(void);
void chargebox_api_send_private_protocol_cmd(void); 
#endif


