#include "includes.h"
#include "app_config.h"
#include "wake_up.h" 
#include "app_var.h"

#if TCFG_HALL_ENABLE || TCFG_KEY_ENABLE	

void io_wakeup_en(void) 
{
	WAKE_SET_DIS(WAKE_CH_1);
	WAKE_SET_DIS(WAKE_CH_2);
	WAKE_SET_DIS(WAKE_CH_3);

	// if (sys_info.sys_abnormal_cnt >= TCFG_SYS_ABNORMAL_CNT_MAX) {
	// 	return;
	// }

	// WAKE_SET_FILTER(WAKE_FILTER_2MS);
	WAKE_SET_FILTER(WAKE_FILTER_64US);

#if TCFG_HALL_ENABLE //input channal2
	if (HALL_GROUP == 1) {
		P1_DIE |= BIT(HALL_PORT);
	} else {
		P0_DIE |= BIT(HALL_PORT);
	}
	IN_CH2_SEL(HALL_IO);
    WAKE_FILTER_EN(WAKE_CH_2);
	WAKE_SET_EDGE(WAKE_CH_2, RISING_EDGE);
    WAKE_SET_EN(WAKE_CH_2);	
#endif

#if TCFG_KEY_ENABLE //input channal3
	if (KEY_GROUP == 1) {
		P1_DIE |= BIT(KEY_PORT);
	} else {
		P0_DIE |= BIT(KEY_PORT);
	}
	IN_CH3_SEL(KEY_IO);
    WAKE_FILTER_EN(WAKE_CH_3);
	WAKE_SET_EDGE(WAKE_CH_3, FAILING_EDGE);
    WAKE_SET_EN(WAKE_CH_3);	
#endif

	//串口1 rx
	P1_DIE |= BIT(3);
	IN_CH1_SEL(IO_PORT_1_3);
    WAKE_FILTER_EN(WAKE_CH_1);
	WAKE_SET_EDGE(WAKE_CH_1, FAILING_EDGE);
    WAKE_SET_EN(WAKE_CH_1);	
	
	WAKE_CLR_ALL_PND(); 
}

#endif
