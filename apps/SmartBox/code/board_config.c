#include "includes.h"
#include "charge.h"
#include "boost.h"
#include "vout_inserdet.h"
#include "app_config.h"
#include "board_config.h"
#include "app_var.h"
#include "chargebox.h"
#include "pulse_cnt.h"
#include "wake_up.h"

void pmu_isr(void) interrupt PMU_INT
{
	u8 i;
	for (i = WKUP_0_VOUT_OV; i < WKUP_MAX; i++) {
		if (pmu_wkupx_level_pending_get(i) || pmu_wkupx_edge_pending_get(i)) {
			pmu_wkupx_clear_pending(i);
			switch (i) {
				case WKUP_0_VOUT_OV:
				case WKUP_1_VOUT_UV:
					//关闭boost
					BOOST_EN(0);
					BOOST_CLK_DIS();
					b_sys_busy_in_boost = 0;
					
					//关闭VOUT0 VOUT1 EN
					VOUT1_OUTPUT_EN(0);
					VOUT0_OUTPUT_EN(0);

					//关闭OV检测
					VOUT_OV_DET_EN(0);
					VOUT_OV_COMP_OE(0);
					WKUPx_AUTO_EN(WKUP_0_VOUT_OV, 0);
					WKUPx_EN(WKUP_0_VOUT_OV, 0);

					//关闭UV检测
					VOUT_UV_DET_EN(0);
					VOUT_UV_COMP_OE(0);
					WKUPx_AUTO_EN(WKUP_1_VOUT_UV, 0);
					WKUPx_EN(WKUP_1_VOUT_UV, 0);

					b_discharge_vout_ovuv = 1;
					sys_info.sys_abnormal = 1;
					//log_puts("\nWKUP_SYS_ERR");
					break;
				case WKUP_2_VOUT_AVMAX_COMP:
					//log_puts("\nWKUP_2_VOUT_AVMAX_COMP");
					break;
				case WKUP_3_VPWR_OV:
					//log_puts("\nWKUP_3_VPWR_OV");
					break;
				case WKUP_4_VPWR_DECT:
					//log_puts("\nWKUP_4_VPWR_DECT");
					break;
				case WKUP_5_VPWR_VBAT_COMP:
					//log_puts("\nWKUP_5_VPWR_VBAT_COMP");
					break;
				case WKUP_6_VOUT0_DECT:
					//log_puts("\nWKUP_6_VOUT0_DECT");
					break;
				case WKUP_7_VOUT1_DECT:
					//log_puts("\nWKUP_7_VOUT1_DECT");
					break;
			}	
		}	
	}
}

void board_power_init(void)
{
	//开机打开boost偏置电压，支持boost/vout_inserdet/vout_ovuv等模块工作
	BOOST_BIAS_EN(1);

	ear_info[EAR_L].power = ear_info[EAR_R].power = 0xff;

	pmu_wakeup_init();

#if TCFG_LONG_PRESS_RESET_ENABLE
	long_press_reset_init();
#endif

	charge_init();

	boost_init();

	vout_inserdet_init();

#if TCFG_AUTO_WAKEUP_ENABLE
	sleep_auto_wkup_init();
#endif
}

