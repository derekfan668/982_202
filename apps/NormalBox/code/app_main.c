#include "includes.h"
#include "app_config.h"
#include "app_var.h"
#include "adc.h"
#include "timer.h"
#include "chargebox.h"
#include "app_close.h"
#include "app_common.h"
#include "key.h"
#include "hall_det.h"
#include "wireless.h"
#include "extern_ntc_det.h"
#include "ui.h"
#include "power_det.h"
#include "burn.h"
#include "uart_key.h"
#include "board_config.h"
#include "power_optimize.h"
#include "uart.h"

__bit8 bdata device_st;
__bit8 bdata danger_signals;
idata volatile u8 g_mux_buf[13] _at_ TCFG_MUX_BUFFER_ADDR;//
SYS_INFO data sys_info;
EAR_INFO data ear_info[2];
__bit8 bdata sys_event0;
__bit8 bdata sys_event1;
__bit8 bdata sys_event2;
__bit8 bdata sys_event3;
__bit8 bdata auto_sleep_en;
__bit8 bdata charge_sw;
__bit8 bdata discharge_sw;
#if TCFG_CHARGE_OPTIMIZE_ENABLE || TCFG_BOOST_OPTIMIZE_ENABLE
__power_optimize power_optimize;
#endif

void sys_init(void) 
{
	MCLR_DIS();
	P0_PD = 0;
	b_charge_disable = 1;
	b_discharge_disable = 1;
	burn_init();
#if TCFG_DEBUG_ENABLE
    uart_init();
#endif
    adc_init();
	board_power_init(); 
}

void module_init(void)
{
#if TCFG_KEY_ENABLE
    key_init();
#endif
#if TCFG_HALL_ENABLE
    hall_init();
#endif
#if TCFG_WPR_ENABLE
    wireless_init();
#endif
#if TCFG_UPDATE_ENABLE
#if (TCFG_WPR_ENABLE && (WPR_USED_PORT == WPR_USED_P17))
	if (!wpr_status) {
		uart_key_init();
	}
#else
	uart_key_init();
#endif
#endif
#if TCFG_EXTERN_NTC_ENABLE
    ntc_init();
#endif
#if TCFG_UI_ENABLE
    ui_init();
#endif
	timer0_init();
	power_det_init(); 
    wdt_init(WDT_4S);
}

void main(void)
{	
	local_irq_disable();
	sys_init();
#if TCFG_DEBUG_ENABLE
	reset_source_dump();
#endif
	module_init();
    app_auto_mode();
	local_irq_enable();
	
    while (1) {
        app_close();
    }
}



