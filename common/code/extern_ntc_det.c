#include "includes.h"
#include "app_config.h"
#include "adc.h"

#if defined(TCFG_EXTERN_NTC_ENABLE) && TCFG_EXTERN_NTC_ENABLE

#include "extern_ntc_det.h"

/*
 * (JEITA��׼)���ģʽ:
 * T1: 0��  T2:10��  T3: 45�� T4: 60��
 * ��T1~T2��Χ,������Ϊ����������һ��
 * ��T2~T3��Χ,����������
 * ��T3~T4��Χ,�����ѹ���õ�4.1V@FULL=4.2V
 * С��T1���ߴ���T4,���ֹͣ
 *
 * (�����׼)���ģʽ:
 * T1: 0��  T2: 45��
 * С��T1���ߴ���T2,���ֹͣ
 * ��T1~T2��Χ�������
 *
 * �ŵ�ģʽ:
 * T1: -10��  T2: 60��
 * С��T1���ߴ���T2,�ŵ�ֹͣ
 * ��T1~T2��Χ�����ŵ�
 *
 * ͷ�ļ��������������� 100K/10K B=3950 ����
 */

#define NTC_FILTER_CNT              (3) //ע�������˲�����������3

#if (TCFG_EXTERN_NTC_TYPE == TCFG_EXTERN_NTC_100K)

#define JEITA_CHARGE_COLD_RES       NTC_100K_RES_POS_00 //���䱣����ֵ
#define JEITA_CHARGE_COLD_REC_RES   NTC_100K_RES_POS_05 //������䱣����ֵ
#define JEITA_CHARGE_COOL_RES       NTC_100K_RES_POS_10 //��������ֵ
#define JEITA_CHARGE_COOL_REC_RES   NTC_100K_RES_POS_15 //�����������ֵ
#define JEITA_CHARGE_WARM_REC_RES   NTC_100K_RES_POS_40 //����±�����ֵ
#define JEITA_CHARGE_WARM_RES       NTC_100K_RES_POS_45 //�±�����ֵ
#define JEITA_CHARGE_HOT_REC_RES    NTC_100K_RES_POS_55 //������ȱ�����ֵ
#define JEITA_CHARGE_HOT_RES        NTC_100K_RES_POS_60 //���ȱ�����ֵ

#define GB_CHARGE_COLD_RES          NTC_100K_RES_POS_00 //���䱣����ֵ
#define GB_CHARGE_COLD_REC_RES      NTC_100K_RES_POS_05 //������䱣����ֵ
#define GB_CHARGE_HOT_REC_RES       NTC_100K_RES_POS_40 //������ȱ�����ֵ
#define GB_CHARGE_HOT_RES           NTC_100K_RES_POS_45 //���ȱ�����ֵ

#define DISCHARGE_COLD_RES          NTC_100K_RES_NEG_10 //�ŵ���䱣����ֵ
#define DISCHARGE_COLD_REC_RES      NTC_100K_RES_NEG_05 //����ŵ���䱣����ֵ
#define DISCHARGE_HOT_REC_RES       NTC_100K_RES_POS_50 //����ŵ���ȱ�����ֵ
#define DISCHARGE_HOT_RES           NTC_100K_RES_POS_60 //�ŵ���ȱ�����ֵ

#else

#define JEITA_CHARGE_COLD_RES       NTC_10K_RES_POS_00 //���䱣����ֵ
#define JEITA_CHARGE_COLD_REC_RES   NTC_10K_RES_POS_05 //������䱣����ֵ
#define JEITA_CHARGE_COOL_RES       NTC_10K_RES_POS_10 //��������ֵ
#define JEITA_CHARGE_COOL_REC_RES   NTC_10K_RES_POS_15 //�����������ֵ
#define JEITA_CHARGE_WARM_REC_RES   NTC_10K_RES_POS_40 //����±�����ֵ
#define JEITA_CHARGE_WARM_RES       NTC_10K_RES_POS_45 //�±�����ֵ
#define JEITA_CHARGE_HOT_REC_RES    NTC_10K_RES_POS_55 //������ȱ�����ֵ
#define JEITA_CHARGE_HOT_RES        NTC_10K_RES_POS_60 //���ȱ�����ֵ

#define GB_CHARGE_COLD_RES          NTC_10K_RES_POS_00 //���䱣����ֵ
#define GB_CHARGE_COLD_REC_RES      NTC_10K_RES_POS_05 //������䱣����ֵ
#define GB_CHARGE_HOT_REC_RES       NTC_10K_RES_POS_40 //������ȱ�����ֵ
#define GB_CHARGE_HOT_RES           NTC_10K_RES_POS_45 //���ȱ�����ֵ

#define DISCHARGE_COLD_RES          NTC_10K_RES_NEG_10 //�ŵ���䱣����ֵ
#define DISCHARGE_COLD_REC_RES      NTC_10K_RES_NEG_05 //����ŵ���䱣����ֵ
#define DISCHARGE_HOT_REC_RES       NTC_10K_RES_POS_50 //����ŵ���ȱ�����ֵ
#define DISCHARGE_HOT_RES           NTC_10K_RES_POS_60 //�ŵ���ȱ�����ֵ

#endif

__ntc data ntc;
static idata u16 ntc_base_res;
static idata u16 ntc_res;

static void ntc_get_res(void) 
{
	u16 ntc_adc = adc_get_value(NTC_DET_AD_CH, 1);
	//log_put_u16(ntc_adc);
	if (ntc_adc > 0x3e0) {
        //û�н�NTC,����NTC����ֵ
        ntc_res = ntc_base_res;
		return;
    }
	ntc_res = ntc_base_res * (u32)ntc_adc / (1023 - ntc_adc);   
}

static u8 ntc_get_charge_status(void) 
{
#if (NTC_MODE_SEL == NTC_MODE_GB)
    if (ntc.u.charge == NTC_STATUS_CHARGE_GOOD) {
        if ((ntc_res > GB_CHARGE_COLD_RES) || (ntc_res < GB_CHARGE_HOT_RES)) {
            return NTC_STATUS_CHARGE_STOP;
        }
    } else {
        if ((ntc_res > GB_CHARGE_HOT_REC_RES) && (ntc_res < GB_CHARGE_COLD_REC_RES)) {
            return NTC_STATUS_CHARGE_GOOD;
        }
    }
#else
    if (ntc.u.charge == NTC_STATUS_CHARGE_STOP) {
        if ((ntc_res < JEITA_CHARGE_HOT_REC_RES) || (ntc_res > JEITA_CHARGE_COLD_REC_RES)) {
            return NTC_STATUS_CHARGE_STOP;
        } else if (ntc_res < JEITA_CHARGE_WARM_RES) {
            return NTC_STATUS_CHARGE_VOLT;
        } else if (ntc_res > JEITA_CHARGE_COOL_RES) {
            return NTC_STATUS_CHARGE_CURR;
        } else {
            return NTC_STATUS_CHARGE_GOOD;
        }
    } else if (ntc.u.charge == NTC_STATUS_CHARGE_VOLT) {
        if ((ntc_res < JEITA_CHARGE_HOT_RES) || (ntc_res > JEITA_CHARGE_COLD_RES)) {
            return NTC_STATUS_CHARGE_STOP;
        } else if (ntc_res < JEITA_CHARGE_WARM_REC_RES) {
            return NTC_STATUS_CHARGE_VOLT;
        } else if (ntc_res > JEITA_CHARGE_COOL_RES) {
            return NTC_STATUS_CHARGE_CURR;
        } else {
            return NTC_STATUS_CHARGE_GOOD;
        }
    } else if (ntc.u.charge == NTC_STATUS_CHARGE_GOOD) {
        if ((ntc_res < JEITA_CHARGE_HOT_RES) || (ntc_res > JEITA_CHARGE_COLD_RES)) {
            return NTC_STATUS_CHARGE_STOP;
        } else if (ntc_res < JEITA_CHARGE_WARM_RES) {
            return NTC_STATUS_CHARGE_VOLT;
        } else if (ntc_res > JEITA_CHARGE_COOL_RES) {
            return NTC_STATUS_CHARGE_CURR;
        } else {
            return NTC_STATUS_CHARGE_GOOD;
        }
    } else if (ntc.u.charge == NTC_STATUS_CHARGE_CURR) {
        if ((ntc_res < JEITA_CHARGE_HOT_RES) || (ntc_res > JEITA_CHARGE_COLD_RES)) {
            return NTC_STATUS_CHARGE_STOP;
        } else if (ntc_res < JEITA_CHARGE_WARM_RES) {
            return NTC_STATUS_CHARGE_VOLT;
        } else if (ntc_res < JEITA_CHARGE_COOL_REC_RES) {
            return NTC_STATUS_CHARGE_GOOD;
        } else {
            return NTC_STATUS_CHARGE_CURR;
        }
    }
#endif
    return ntc.u.charge;
}

static u8 ntc_get_discharge_status(void) 
{
    if (ntc.u.discharge == NTC_STATUS_DISCHARGE_GOOD) {
        if ((ntc_res > DISCHARGE_COLD_RES) || (ntc_res < DISCHARGE_HOT_RES)) {
            return NTC_STATUS_DISCHARGE_STOP;
        }
    } else {
        if ((ntc_res > DISCHARGE_HOT_REC_RES) && (ntc_res < DISCHARGE_COLD_REC_RES)) {
            return NTC_STATUS_DISCHARGE_GOOD;
        }
    }
    return ntc.u.discharge;
}

void ntc_det_run(void) 
{
    ntc_get_res();

    //���NTC״̬���
    if (ntc_get_charge_status() != ntc.u.charge) {
        ntc.u.charge_cnt++;
        if (ntc.u.charge_cnt >= NTC_FILTER_CNT) {
            ntc.u.charge_cnt = 0;
			ntc.u.charge = ntc_get_charge_status();
            b_evt_ntc_charge = 1;
        }
    } else {
        ntc.u.charge_cnt = 0;
    }

    //�ŵ�NTC״̬���
    if (ntc_get_discharge_status() != ntc.u.discharge) {
        ntc.u.discharge_cnt++;
        if (ntc.u.discharge_cnt >= NTC_FILTER_CNT) {
            ntc.u.discharge_cnt = 0;
			ntc.u.discharge = ntc_get_discharge_status();
            b_evt_ntc_discharge = 1;
        }
    } else {
        ntc.u.discharge_cnt = 0;
    }
}

void ntc_init(void)
{
    //gpio_set_dir(NTC_GROUP, NTC_PORT, 1);
    gpio_set_die(NTC_GROUP, NTC_PORT, 0);
    //gpio_set_pull_down(NTC_GROUP, NTC_PORT, 0);
#if (TCFG_EXTERN_NTC_TYPE == TCFG_EXTERN_NTC_100K)
	//gpio_set_pull_up(NTC_GROUP, NTC_PORT, 0);
	NTC_PU_EN(1);//100K
	//У׼�ӷ�: VDDIO(3.069)--RES--15uA--GND
	//RES = (3069 - (AD*3/16)) / 15uA
	if (!otp_trim.trim_en.u.ntc_100k) {
	    ntc_base_res = 2046 - (otp_trim.ntc_100k / 8);
	} else {
		ntc_base_res = NTC_100K_RES_POS_25;
	}
#else
	gpio_set_pull_up(NTC_GROUP, NTC_PORT, 1);
	//У׼�ӷ�: VDDIO(3.069)--RES--150uA--GND
	//RES = (3069 - (AD*3/16)) / 150uA
	if (!otp_trim.trim_en.u.ntc_10k) {
		ntc_base_res = (2046 + 5 - (otp_trim.ntc_10k / 8)) / 10;
	} else {
		ntc_base_res = NTC_10K_RES_POS_25;
	}
#endif
    ntc.u.charge = NTC_STATUS_CHARGE_GOOD;
    ntc.u.discharge = NTC_STATUS_DISCHARGE_GOOD;
    ntc_get_res();

	if (ntc_get_charge_status() != ntc.u.charge) {
    	ntc.u.charge = ntc_get_charge_status();
		b_evt_ntc_charge = 1;
	}

	if (ntc_get_discharge_status() != ntc.u.discharge) {
    	ntc.u.discharge = ntc_get_discharge_status();
		b_evt_ntc_discharge = 1;
	}
}

#endif

