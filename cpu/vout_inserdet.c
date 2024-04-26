#include "includes.h"
#include "vout_inserdet.h"
#include "app_config.h"
#include "chargebox_hw.h"

void vout_inserdet_cs_s(u8 ch, u8 current)
{
	if (ch) {
		VOUT1_INDET_CS_SET(current);
	} else {
		VOUT0_INDET_CS_SET(current);
	}	
}

#ifdef TCFG_NORMALBOX_CASE
void vout_inserdet_en(u8 ch, u8 en)
{
	if (ch) {
		VOUT1_INDET_EN(en);
	} else {
		VOUT0_INDET_EN(en);
	}	
}

u8 vout_inserdet_state_get(u8 ch)
{
	u8 ret = 0;
	if (ch) {
		if (VOUT1_INDET_GET()) {
			ret = 1;
		}
	} else {
		if (VOUT0_INDET_GET()) {
			ret = 1;
		}
	}
	return ret;
}
#else
void vout_inserdet_en(u8 ch, u8 en)
{
	if (ch) {
		VOUT1_INDET_EN(en);
		gpio_set_pull_up(EAR_GROUP_R, EAR_PORT_R, en);
	} else {
		VOUT0_INDET_EN(en);
		gpio_set_pull_up(EAR_GROUP_L, EAR_PORT_L, en);
	}	
}

u8 vout_inserdet_state_get(u8 ch)
{
	u8 ret = 0;
	if (ch) {
		gpio_set_pull_up(EAR_GROUP_R, EAR_PORT_R, 0);
		delay_n2ms(2);
		if (VOUT1_INDET_GET()) {
			ret = 1;
		}
		gpio_set_pull_up(EAR_GROUP_R, EAR_PORT_R, 1);
	} else {
		gpio_set_pull_up(EAR_GROUP_L, EAR_PORT_L, 0);
		delay_n2ms(2);
		if (VOUT0_INDET_GET()) {
			ret = 1;
		}
		gpio_set_pull_up(EAR_GROUP_L, EAR_PORT_L, 1);
	}
	return ret;
}
#endif

u8 vout_inserdet_en_get(u8 ch)
{
	if (ch) {
		if (VOUT1_INDET_EN_GET()) {
			return 1;
		}
	} else {
		if (VOUT0_INDET_EN_GET()) {
			return 1;
		}
	}
	return 0;
}

void vout_inserdet_init(void)
{
	vout_inserdet_cs_s(EAR_L, VOUT_INDET_CURRENT);
	vout_inserdet_cs_s(EAR_R, VOUT_INDET_CURRENT);
}

