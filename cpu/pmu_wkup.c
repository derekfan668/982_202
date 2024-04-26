#include "includes.h"
#include "app_config.h"

struct pmu_wkup {
	u8 pending_hw_auto; //����pending��Ӳ�������Զ��ر�boost��vin2vout����wkup0��wkup1����
	u8 filt_sel;		//�˲�ʱ�� wkup0-3ֻ��not filter��32us��wkup4-7��ѡ����
	u8 edge_ie;			//�����ж�ʹ��
	u8 level_ie;		//��ƽ�ж�ʹ��
	u8 inv;				//ȡ�� 0:�����أ��ߵ�ƽ����pending 1:�½��أ��͵�ƽ����pengding(��wkup 4-7�д˹���)
	u8 wkup_en;			//����ʹ��
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
