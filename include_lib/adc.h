#ifndef _ADC_H_
#define _ADC_H_

#include "typedef.h"

//io ch to adc
#define AD_IO_CH_P00   0x00
#define AD_IO_CH_P01   0x01
#define AD_IO_CH_P02   0x02
#define AD_IO_CH_P03   0x03
#define AD_IO_CH_P13   0x04
#define AD_IO_CH_P14   0x05
#define AD_IO_CH_P15   0x06
#define AD_IO_CH_P16   0x07

//ana ch to adc
#define	AD_ANA_CH_BG				0x10
#define	AD_ANA_CH_VTEMP 			0x13	//[-40C~125C] -> [1.62V~1.00V], -3.75mV/C
#define	AD_ANA_CH_VBAT_ISENSE		0x14	//Vsense = = 2.5 * Isense(mA)
#define	AD_ANA_CH_VOUT0_ISENSE		0x15	//CS_MODE = 1, Vsense0(mV) = 20 * Isense1(mA); CS_MODE = 0, Vsense0(mV) = 200 * Isense0(mA)
#define	AD_ANA_CH_VOUT1_ISENSE		0x16	//CS_MODE = 1, Vsense1(mV) = 20 * Isense1(mA); CS_MODE = 0, Vsense1(mV) = 200 * Isense1(mA)
#define	AD_ANA_CH_PROGF				0x17	//Vprogf(mV) = 50* Icharge(mA), 测量范围0~40mA
#define	AD_ANA_CH_PROGI				0x18	//Vprogi(mV) = 5 * Icharge(mA), 测量范围0~400mA
#define	AD_ANA_CH_VBAT_DIV_4		0x19
#define	AD_ANA_CH_VBAT_DIV_2		0x1a	//对VDDIO有限制,SDK不用
#define	AD_ANA_CH_VIN_DIV_4			0x1b
#define	AD_ANA_CH_VOUT0_DIV_4		0x1c
#define	AD_ANA_CH_VOUT1_DIV_4		0x1d
#define	AD_ANA_CH_VOUT_DIV_4		0x1e
#define	AD_ANA_CH_VPWR_DIV_8		0x1f


#define ADC_DIV_6	1
#define ADC_DIV_12  2
#define ADC_DIV_24  3
#define ADC_DIV_32  4

#define ADC_CH_SEL(x)			ADC_CON1 &= ~0x07; ADC_CON1 |= (x<<0)

void adc_init(void);
void adc_uninit(void);
u16 adc_get_value(u8 ch, u8 n);
u16 adc_get_voltage(u8 ch, u8 n);
void adc_update_vbg(void);

#endif  /*  _ADC_H_ */


