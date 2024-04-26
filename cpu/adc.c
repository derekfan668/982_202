#include "includes.h"
#include "adc.h"

//ADC CLK 1M
//actual_wait_time = ADC_WAIT_TIME * 8 * Tadc_aclk
#define ADC_WAIT_TIME       2//等待采样时间16us
//fadc_clk=fsys_clk/(x+1); (fadc_clk<=2MHz)
#define ADC_CLK             ADC_DIV_32

#define TCFG_VBG_VOLTAGE	1200L//VBG电压
#define ADC_RES_SEL_TYPE	1//采样结果对齐方式
#define ADC_ISEL_TYPE		0//工作电流挡位 1:大电流 0:小电流

static idata u16 adc_vbg;

void adc_sample(u8 ch)
{	
	if (ch & BIT(4)) {		
		PMU_TEST_SET(ch & 0x0f);
		ADC_CON2 &= ~BIT(0);
		ADC_CON2 |= BIT(1);
		
	} else {
		ADC_CON1 &= ~0x07;
		ADC_CON1 |= ch;
		ADC_CON2 &= ~BIT(1);
		ADC_CON2 |= BIT(0);
	}	
}

u16 adc_get_value(u8 ch, u8 n) 
{
	u8 i;
	u16 adc_res = 0;
	adc_sample(ch);
	for (i = 0; i < n; i++) {
		ADC_CON0 |= BIT(6);
		while (!(ADC_CON0 & BIT(7)));
		adc_res += ((ADC_RESH << 8) | ADC_RESL);
	}
	return (adc_res / n);
}

u16 adc_get_voltage(u8 ch, u8 n) 
{	
    if (adc_vbg == 0) {
        return 0;
    }
    return ((u32)adc_get_value(ch, n) * TCFG_VBG_VOLTAGE / adc_vbg);
}

void adc_update_vbg(void)
{
	adc_vbg = adc_get_value(AD_ANA_CH_BG, 8);
}

void adc_init(void) 
{
#if ADC_ISEL_TYPE
	ADC_CON2 = ADC_ISEL_TYPE << 2;
#endif
	ADC_CON1 = (ADC_WAIT_TIME << 4) | (ADC_RES_SEL_TYPE << 3);
	PMU_TOADC_EN(1);
	PMU_TEST_OE(1);
	ADC_CON0 = BIT(4) | BIT(3) | (ADC_CLK << 0);
	//adc scan vbg
	MVBG_BUF_EN(1);
	MVBG_TEST_EN(1);
	MVBG_TEST_SET(MVBG12);
    adc_update_vbg();
}

void adc_uninit(void)
{
	ADC_CON0 = 0x40;
    ADC_CON1 = 0;
    ADC_CON2 = 0;
	PMU_TOADC_EN(0);
   	PMU_TEST_SET(0);
	PMU_TEST_OE(0);
}
