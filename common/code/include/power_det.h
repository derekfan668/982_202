#ifndef __POWER_DET_H__
#define __POWER_DET_H__

#include "typedef.h"

#define VBAT420_LEV_10 		((3680 - POWER_BOOT_LEV) / 10)
#define VBAT420_LEV_20		((3740 - POWER_BOOT_LEV) / 10)
#define VBAT420_LEV_30		((3770 - POWER_BOOT_LEV) / 10)
#define VBAT420_LEV_40		((3790 - POWER_BOOT_LEV) / 10)
#define VBAT420_LEV_50		((3820 - POWER_BOOT_LEV) / 10)
#define VBAT420_LEV_60		((3870 - POWER_BOOT_LEV) / 10)
#define VBAT420_LEV_70		((3920 - POWER_BOOT_LEV) / 10)
#define VBAT420_LEV_80		((3980 - POWER_BOOT_LEV) / 10)
#define VBAT420_LEV_90		((4060 - POWER_BOOT_LEV) / 10)
#define VBAT420_LEV_100		((POWER_TOP_LEV - POWER_BOOT_LEV) / 10)//4100


#define VBAT435_LEV_10 		((3810 - POWER_BOOT_LEV) / 10)
#define VBAT435_LEV_20		((3880 - POWER_BOOT_LEV) / 10)
#define VBAT435_LEV_30		((3910 - POWER_BOOT_LEV) / 10)
#define VBAT435_LEV_40		((3930 - POWER_BOOT_LEV) / 10)
#define VBAT435_LEV_50		((3960 - POWER_BOOT_LEV) / 10)
#define VBAT435_LEV_60		((4010 - POWER_BOOT_LEV) / 10)
#define VBAT435_LEV_70		((4060 - POWER_BOOT_LEV) / 10)
#define VBAT435_LEV_80		((4130 - POWER_BOOT_LEV) / 10)
#define VBAT435_LEV_90		((4200 - POWER_BOOT_LEV) / 10)
#define VBAT435_LEV_100		((POWER_TOP_LEV - POWER_BOOT_LEV) / 10)//4250

typedef struct {
    u8 percent;
    u8 counter;
    u8 filter_time_cnt : 4;	//滤波数次
	u8 same_volt_cnt   : 4; //相同电压数次
} __power_det;

void power_det_run(void);
void power_det_init(void);
u16 power_get_vbat_voltage(void);
u8 power_get_vbat_percent(void);

#endif


