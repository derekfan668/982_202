#include "includes.h"
#include "app_config.h"

struct pmu_wkup {
	u8 pending_hw_auto; //产生pending后硬件触发自动关闭boost和vin2vout，仅wkup0和wkup1可用
	u8 filt_sel;		//滤波时间 wkup0-3只有not filter和32us，wkup4-7可选所有
	u8 edge_ie;			//边沿中断使能
	u8 level_ie;		//电平中断使能
	u8 inv;				//取反 0:上升沿，高电平触发pending 1:下降沿，低电平触发pengding(仅wkup 4-7有此功能)
	u8 wkup_en;			//唤醒使能
};

struct pmu_wkup_param {
    struct pmu_wkup *wkup_i[WKUP_MAX];
};

code const struct pmu_wkup wkup0_vout_ov = {
	DISABLE,				
	WKUP_FLT_NOT,		
	ENABLE,				
	ENABLE,			
	0,					
	ENABLE,			
};

code const struct pmu_wkup wkup1_vout_uv = {
	DISABLE,
	WKUP_FLT_NOT,
	ENABLE,
	ENABLE,
	0,
	ENABLE,	
};

code const struct pmu_wkup wkup2_vout_avmax_comp = {
	DISABLE,
	WKUP_FLT_32US,
	ENABLE,
	DISABLE,
	0,
	DISABLE,	
};

code const struct pmu_wkup wkup3_vpwr_ov = {
	DISABLE,
	WKUP_FLT_32US,
	ENABLE,
	DISABLE,
	0,
	ENABLE,	
};

code const struct pmu_wkup wkup4_vpwr_dect = {
	DISABLE,
	WKUP_FLT_32US,
	ENABLE,
	DISABLE,
	0,
	ENABLE,	
};

code const struct pmu_wkup wkup5_vpwr_vbat_comp = {
	DISABLE,
	WKUP_FLT_32US,
	ENABLE,
	DISABLE,
	0,
	DISABLE,	
};

code const struct pmu_wkup wkup6_vout0_dect = {
	DISABLE,
	WKUP_FLT_2MS,
	ENABLE,
	DISABLE,
	0,
	DISABLE,	
};

code const struct pmu_wkup wkup7_vout1_dect = {
	DISABLE,
	WKUP_FLT_2MS,
	ENABLE,
	DISABLE,
	0,
	DISABLE,	
};

code const struct pmu_wkup_param pmuwk_param = {
	&wkup0_vout_ov,
	&wkup1_vout_uv,
	&wkup2_vout_avmax_comp,
	&wkup3_vpwr_ov,
	&wkup4_vpwr_dect,
	&wkup5_vpwr_vbat_comp,
	&wkup6_vout0_dect,
	&wkup7_vout1_dect,
};

void pmu_wakeup_init(void) 
{
	u8 i;
	for (i = WKUP_0_VOUT_OV; i < WKUP_MAX; i++) {
		pmu_wkupx_clear_pending(i);
		WKUPx_FLT_SEL(i, pmuwk_param.wkup_i[i]->filt_sel);
		WKUPx_EIE_EN(i, pmuwk_param.wkup_i[i]->edge_ie);
		WKUPx_LIE_EN(i, pmuwk_param.wkup_i[i]->level_ie);
		if (i > 3) {
			WKUPx_INV_EN(i, pmuwk_param.wkup_i[i]->inv);
		}
		WKUPx_EN(i, pmuwk_param.wkup_i[i]->wkup_en);
	}
	PMU_IE(1);
}
