#ifndef __VOUT_INDET_H__
#define __VOUT_INDET_H__

#include "typedef.h"

void vout_inserdet_en(u8 ch, u8 en);
void vout_inserdet_cs_s(u8 ch, u8 current);
u8 vout_inserdet_state_get(u8 ch); //return 1 when vout0/1 load in
u8 vout_inserdet_en_get(u8 ch); //return 1 when voutx inserdet enable
void vout_inserdet_init(void);

#endif