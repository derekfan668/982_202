#include "includes.h"
#include "app_config.h"
#include "app_var.h"
#include "wpr.h"
#include "wireless.h"
#include "adc.h"
#include "uart_key.h"
#include "charge.h"

#if TCFG_WPR_ENABLE

#if (WPR_USED_PORT == WPR_USED_P17)
#define WPR_GROUP           1
#define WPR_PORT            7
#define WPR_OUTPUT_EN()     WPR_TX1_OUTPUT_EN()
#define WPR_OUTPUT_DIS()    WPR_TX1_OUTPUT_DIS()
#else
#define WPR_GROUP           0
#define WPR_PORT            5
#define WPR_OUTPUT_EN()     WPR_TX0_OUTPUT_EN()
#define WPR_OUTPUT_DIS()    WPR_TX0_OUTPUT_DIS()
#endif

static data u8 wpr_cnt, wpr_128ms_cnt;
static data u8 error_packet_count;

#define WPR_ONLINE()    ((wpr_voltage > 3500) ? 1 : 0)
#define WPR_FILTER_CNT  (20 / TCFG_TIMER_LOOP - 1)
#define TIMER_128MS_CNT (128 / TCFG_TIMER_LOOP - 1)

static void wireless_det_voltage(void)
{
	wpr_voltage = adc_get_voltage(WPR_DET_AD_CH, 1) * WPR_DET_DIV_COEFF;
}

static void wpr_send_data(u8 packet_len) 
{
	gpio_write(WPR_GROUP, WPR_PORT, 0);
    gpio_set_dir(WPR_GROUP, WPR_PORT, 0);
	WPR_DAT_CNT = packet_len;
    WPR_PRE_CNT = 13;
    WPR_DAT_ADR = (u8)wpr_buf;
	WPR_CON0 = 0x71;
    WPR_CON0 |= BIT(1); //kick start
	WPR_OUTPUT_EN();
	//一定要等pending才能退出循环
	while (!(WPR_CON0 & BIT(7))) {
		//握手阶段发送数据时不断检测是否在线
		if ((wpr_open_flag == 0) && wpr_status) {
			wireless_det_voltage();
			if (!WPR_ONLINE()) {
				wpr_status = 0;
				WPR_OUTPUT_DIS();
			}
		}
	}
	WPR_OUTPUT_DIS();
    WPR_CON0 = BIT(6);
}

void wireless_scan(void) 
{
    if (wpr_disable) {
        wpr_open_flag = 0;
        wpr_status = 0;
        wpr_cnt = 0;
        return;
    }

	wireless_det_voltage();

    if (WPR_ONLINE() != wpr_status) {
        wpr_cnt++;
        if (wpr_cnt > WPR_FILTER_CNT) {
			wpr_cnt = 0;
            wpr_status = !wpr_status;
            if (wpr_status) {
                b_evt_wireless_on = 1;
            } else {
                b_evt_wireless_off = 1;
            }
        }
    } else {
        wpr_cnt = 0;
    }

    if (wpr_open_flag) {
        wpr_128ms_cnt++;
        if (wpr_128ms_cnt >= TIMER_128MS_CNT) {
            wpr_128ms_cnt = 0;
			b_evt_wpr_128ms = 1;
			if (wpr_voltage >= (10*1000)) {
				b_evt_wireless_ov = 1;
				charge_set_charge_mA(CHARGE_WIRELESS_OV_CURRENT_MA);
			} else {
				b_evt_wireless_ov = 0;
				charge_set_charge_mA(CHARGE_WIRELESS_CURRENT_MA);
			}
        }
    }
}

void wireless_api_open(void) 
{
    log_puts("wpr in\n");

#if (TCFG_UPDATE_ENABLE && (WPR_USED_PORT == WPR_USED_P17))	
	uart_key_uninit();
#endif
	wpr_common = 1;
    //发送信号强度包
    get_signal_value();
	wpr_send_data(3);
    delay_n2ms(3);
    if (!wpr_status) {
        goto __err_exit;
    }

    //发送身份包
    get_identification();
	wpr_send_data(9);
    delay_n2ms(3);
    if (!wpr_status) {
        goto __err_exit;
    }

    //发送配置包
    get_configuration();
	wpr_send_data(7);
    delay_n2ms(3);
    if (!wpr_status) {
        goto __err_exit;
    }

    b_evt_wpr_128ms = 0;
    error_packet_count = 0;
    wpr_fast_flag = 0;
    wpr_fast_en = 1;
    wpr_128ms_cnt = 0;
    wpr_open_flag = 1;
	b_sys_busy_in_wireless = 1;
	wpr_common = 0;
	return;

__err_exit:
    wireless_api_close();
    return;
}

void wireless_api_close(void) 
{
    log_puts("wpr out\n");
    wpr_open_flag = 0;
	wpr_common = 0;
	b_sys_busy_in_wireless = 0;
#if (TCFG_UPDATE_ENABLE && (WPR_USED_PORT == WPR_USED_P17))
	uart_key_init();					  
#endif
}			    

void wireless_api_128ms_run(void) 
{	
	s16 err_voltage;
    if (wpr_disable || (!wpr_open_flag)) {
        return;
    }

    if (wpr_fast_en == 0) {
        wpr_fast_flag = !wpr_fast_flag;
        if (wpr_fast_flag) {
            return;
        }
    }

    error_packet_count++;
	wpr_common = 1;
	if (error_packet_count <= 6) {
		err_voltage = (s16)TCFG_WPR_VOLTAGE_NORMAL - (s16)wpr_voltage;
        get_control_error(err_voltage);
		wpr_send_data(3);
    } else {
        error_packet_count = 0;
        get_rectified_value();
		wpr_send_data(3);
    }
	wpr_common = 0;
}

void wireless_init(void)
{
    //时钟初始化
    WPR_DIV = (SYS_CLK / 4000L - 1) >> 8;    //13:8
    WPR_DIV = (SYS_CLK / 4000L - 1) & 0xff;  // 7:0
    //gpio_set_dir(WPR_DET_IO_GROUP, WPR_DET_IO_PORT, 1);
    gpio_set_die(WPR_DET_IO_GROUP, WPR_DET_IO_PORT, 0);
    //gpio_set_pull_down(WPR_DET_IO_GROUP, WPR_DET_IO_PORT, 0);
    //gpio_set_pull_up(WPR_DET_IO_GROUP, WPR_DET_IO_PORT, 0);
	wireless_det_voltage();
	wpr_status = WPR_ONLINE();
	if (wpr_status) {
		b_evt_wireless_on = 1;
	}
}

#endif

