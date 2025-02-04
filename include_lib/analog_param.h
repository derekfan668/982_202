#ifndef __ANALOG_PARAM_H__
#define __ANALOG_PARAM_H__

#include "includes.h"

//boost vout volatage level selete
enum {
	BOOST_VOUT_S_3800_MV,
	BOOST_VOUT_S_3900_MV,
	BOOST_VOUT_S_4000_MV,
	BOOST_VOUT_S_4100_MV,
	BOOST_VOUT_S_4200_MV,
	BOOST_VOUT_S_4300_MV,
	BOOST_VOUT_S_4400_MV,
	BOOST_VOUT_S_4500_MV,
	BOOST_VOUT_S_4600_MV,
	BOOST_VOUT_S_4700_MV,
	BOOST_VOUT_S_4800_MV,
	BOOST_VOUT_S_4900_MV,
	BOOST_VOUT_S_5000_MV,
	BOOST_VOUT_S_5100_MV,
	BOOST_VOUT_S_5200_MV,
	BOOST_VOUT_S_5300_MV,
};

//boost mode hcomp current compensate resistance selete
enum {
	BOOST_VS_S_2800_ohm,
	BOOST_VS_S_2200_ohm,
	BOOST_VS_S_1600_ohm,
	BOOST_VS_S_900_ohm,
};

//constant voltage charge voltage selete
//HV_MODE: 0
#define CHGV_S_4041_MV	0
#define CHGV_S_4061_MV	1
#define CHGV_S_4081_MV	2
#define CHGV_S_4101_MV	3
#define CHGV_S_4119_MV	4
#define CHGV_S_4139_MV	5
#define CHGV_S_4159_MV	6
#define CHGV_S_4179_MV	7
#define CHGV_S_4199_MV	8
#define CHGV_S_4219_MV	9
#define CHGV_S_4238_MV	10
#define CHGV_S_4258_MV	11
#define CHGV_S_4278_MV	12
#define CHGV_S_4298_MV	13
#define CHGV_S_4318_MV	14
#define CHGV_S_4338_MV	15
//HV_MODE: 1
#define CHGV_S_4237_MV	16
#define CHGV_S_4257_MV	17
#define CHGV_S_4275_MV	18
#define CHGV_S_4295_MV	19
#define CHGV_S_4315_MV	20
#define CHGV_S_4335_MV	21
#define CHGV_S_4354_MV	22
#define CHGV_S_4374_MV	23
#define CHGV_S_4394_MV	24
#define CHGV_S_4414_MV	25
#define CHGV_S_4434_MV	26
#define CHGV_S_4454_MV	27
#define CHGV_S_4474_MV	28
#define CHGV_S_4494_MV	29
#define CHGV_S_4514_MV	30
#define CHGV_S_4534_MV	31

//charge current select
#define CHGI_S_50_MA	0
#define CHGI_S_100_MA	1
#define CHGI_S_150_MA	2
#define CHGI_S_200_MA	3
#define CHGI_S_250_MA	4
#define CHGI_S_300_MA	5
#define CHGI_S_350_MA	6
#define CHGI_S_400_MA	7

//charge current trimming selete
enum {
	CHGI_TRIM_S_180MA,
	CHGI_TRIM_S_195MA,
	CHGI_TRIM_S_210MA,
	CHGI_TRIM_S_225MA,
	CHGI_TRIM_S_240MA,
	CHGI_TRIM_S_255MA,
	CHGI_TRIM_S_270MA,
	CHGI_TRIM_S_285MA,
	CHGI_TRIM_S_300MA,
	CHGI_TRIM_S_315MA,
	CHGI_TRIM_S_330MA,
	CHGI_TRIM_S_345MA,
	CHGI_TRIM_S_360MA,
	CHGI_TRIM_S_375MA,
	CHGI_TRIM_S_390MA,
	CHGI_TRIM_S_405MA,
};														        

//iovdd lvd voltage level selete
enum {
	LVD_S_2400_MV,
	LVD_S_2600_MV,
	LVD_S_2800_MV,
	LVD_S_3000_MV,
};

//weak iovdd level selete
enum {
	WVIO_S_2350_MV,
	WVIO_S_2400_MV,
	WVIO_S_2450_MV,
	WVIO_S_2500_MV,
	WVIO_S_2550_MV,
	WVIO_S_2600_MV,
	WVIO_S_2650_MV,
	WVIO_S_2700_MV,
	WVIO_S_2750_MV,
	WVIO_S_2800_MV,
	WVIO_S_2850_MV,
	WVIO_S_2900_MV,
	WVIO_S_2950_MV,
	WVIO_S_3000_MV,
	WVIO_S_3050_MV,
	WVIO_S_3100_MV,
};

//pre vbg level selete
enum {
	PVBG_S_1093_MV,
	PVBG_S_1109_MV,
	PVBG_S_1124_MV,
	PVBG_S_1139_MV,
	PVBG_S_1154_MV,
	PVBG_S_1170_MV,
	PVBG_S_1185_MV,
	PVBG_S_1200_MV,
	PVBG_S_1215_MV,
	PVBG_S_1234_MV,
	PVBG_S_1246_MV,
	PVBG_S_1261_MV,
	PVBG_S_1276_MV,
	PVBG_S_1291_MV,
	PVBG_S_1307_MV,
	PVBG_S_1322_MV,
};

//mvbg level selete
enum {
	MVBG_S_1093_MV,
	MVBG_S_1109_MV,
	MVBG_S_1124_MV,
	MVBG_S_1139_MV,
	MVBG_S_1154_MV,
	MVBG_S_1170_MV,
	MVBG_S_1185_MV,
	MVBG_S_1200_MV,
	MVBG_S_1215_MV,
	MVBG_S_1234_MV,
	MVBG_S_1246_MV,
	MVBG_S_1261_MV,
	MVBG_S_1276_MV,
	MVBG_S_1291_MV,
	MVBG_S_1307_MV,
	MVBG_S_1322_MV,
};

//main iovdd level selete, default 3.3V
enum {
	MVIO_S_2700_MV,
	MVIO_S_2800_MV,
	MVIO_S_2900_MV,
	MVIO_S_3000_MV,
	MVIO_S_3100_MV,
	MVIO_S_3200_MV,
	MVIO_S_3300_MV,
	MVIO_S_3400_MV,
};

//mvbg prebg test selete
enum {
	MVBG12,
	PREBG12,
	WBG,
	PREBG,
}; 

//vout0 to earphone power output current sense resistance trim
enum {
	//CS_MODE: 0 
	VOUT0_CSRES_S_7K,
	VOUT0_CSRES_S_8K,
	VOUT0_CSRES_S_9K,
	VOUT0_CSRES_S_10K,
	VOUT0_CSRES_S_11K,
	VOUT0_CSRES_S_12K,
	VOUT0_CSRES_S_13K,
	VOUT0_CSRES_S_14K,

	//CS_MODE: 1 
	VOUT0_CSRES_S_70K,
	VOUT0_CSRES_S_80K,
	VOUT0_CSRES_S_90K,
	VOUT0_CSRES_S_100K,
	VOUT0_CSRES_S_110K,
	VOUT0_CSRES_S_120K,
	VOUT0_CSRES_S_130K,
	VOUT0_CSRES_S_140K,
};

//vout0 to earphone power output current sense vds keep voltage
enum {
	VOUT0_CSVDSVOL_S_25_MV,
	VOUT0_CSVDSVOL_S_50_MV,
	VOUT0_CSVDSVOL_S_75_MV,
	VOUT0_CSVDSVOL_S_100_MV,
	VOUT0_CSVDSVOL_S_125_MV,
	VOUT0_CSVDSVOL_S_150_MV,
	VOUT0_CSVDSVOL_S_175_MV,
	VOUT0_CSVDSVOL_S_200_MV,
};

//vout1 to earphone power output current sense resistance trim
enum {
	//CS_MODE: 0 
	VOUT1_CSRES_S_7K,
	VOUT1_CSRES_S_8K,
	VOUT1_CSRES_S_9K,
	VOUT1_CSRES_S_10K,
	VOUT1_CSRES_S_11K,
	VOUT1_CSRES_S_12K,
	VOUT1_CSRES_S_13K,
	VOUT1_CSRES_S_14K,

	//CS_MODE: 1 
	VOUT1_CSRES_S_70K,
	VOUT1_CSRES_S_80K,
	VOUT1_CSRES_S_90K,
	VOUT1_CSRES_S_100K,
	VOUT1_CSRES_S_110K,
	VOUT1_CSRES_S_120K,
	VOUT1_CSRES_S_130K,
	VOUT1_CSRES_S_140K,
};

//vout1 to earphone power output current sense vds keep voltage
enum {
	VOUT1_CSVDSVOL_S_25_MV,
	VOUT1_CSVDSVOL_S_50_MV,
	VOUT1_CSVDSVOL_S_75_MV,
	VOUT1_CSVDSVOL_S_100_MV,
	VOUT1_CSVDSVOL_S_125_MV,
	VOUT1_CSVDSVOL_S_150_MV,
	VOUT1_CSVDSVOL_S_175_MV,
	VOUT1_CSVDSVOL_S_200_MV,
};

//vout earphone insert detective current
enum {
	VOUT_INDET_CS_S_0P25_UA,
	VOUT_INDET_CS_S_0P5_UA,
	VOUT_INDET_CS_S_1_UA,
	VOUT_INDET_CS_S_2_UA,
};

//vout over voltage selete
enum {
	VOUT_OV_VOL_S_5P32V_TO_5P15V,
	VOUT_OV_VOL_S_5P44V_TO_5P26V,
	VOUT_OV_VOL_S_5P53V_TO_5P34V,
	VOUT_OV_VOL_S_5P66V_TO_5P46V,
	VOUT_OV_VOL_S_5P75V_TO_5P54V,
	VOUT_OV_VOL_S_5P84V_TO_5P63V,
	VOUT_OV_VOL_S_5P93V_TO_5P72V,
	VOUT_OV_VOL_S_6P03V_TO_5P81V,
};

//vout under voltage selete
enum {
	VOUT_UV_VOL_S_3P00V_TO_3P12V,
	VOUT_UV_VOL_S_3P20V_TO_3P35V,
	VOUT_UV_VOL_S_3P40V_TO_3P56V,
	VOUT_UV_VOL_S_3P60V_TO_3P79V,
	VOUT_UV_VOL_S_3P80V_TO_4P00V,
	VOUT_UV_VOL_S_4P00V_TO_4P24V,
	VOUT_UV_VOL_S_4P20V_TO_4P45V,
	VOUT_UV_VOL_S_4P40V_TO_4P68V,
};

//BOOST
#define BOOST_EN(i)			PMU_CON_SET(PSFR_ANA_CON0, 0x01 << 0, (i) << 0)
#define BOOST_BIAS_EN(i)	PMU_CON_SET(PSFR_ANA_CON0, 0x01 << 1, (i) << 1)
#define BOOST_TMODE_EN(i)	PMU_CON_SET(PSFR_ANA_CON0, 0x01 << 4, (i) << 4)	//just for test
#define BOOST_TMODE_S(i)	PMU_CON_SET(PSFR_ANA_CON0, 0x03 << 5, (i) << 5)	//just for test
#define BOOST_VBATCS_EN(i)	PMU_CON_SET(PSFR_ANA_CON0, 0x01 << 7, (i) << 7) 
#define BOOST_GM_TSEL(i) 	PMU_CON_SET(PSFR_ANA_CON1, 0x01 << 0, (i) << 0) //just for test
#define BOOST_GM_RUSH(i) 	PMU_CON_SET(PSFR_ANA_CON1, 0x01 << 1, (i) << 1) //just for test
#define BOOST_VOUT_SET(i) 	PMU_CON_SET(PSFR_ANA_CON1, 0x0f << 4, (i) << 4)
#define BOOST_VS_EN(i)		PMU_CON_SET(PSFR_ANA_CON2, 0x01 << 0, (i) << 0)
#define BOOST_VS_SET(i)		PMU_CON_SET(PSFR_ANA_CON2, 0x03 << 1, (i) << 1)
#define BOOST_ZCD_EN(i)		PMU_CON_SET(PSFR_ANA_CON2, 0x01 << 3, (i) << 3)
#define BOOST_ZCD_HSEL(i)	PMU_CON_SET(PSFR_ANA_CON2, 0x07 << 4, (i) << 4)
#define BOOST_ZCD_LSEL(i)	PMU_CON_SET(PSFR_ANA_CON2, 0x01 << 7, (i) << 7)

//CHARGER
#define CHARGE_EN(i)		PMU_CON_SET(PSFR_ANA_CON3, 0x03 << 0, (i) << 0)		
#define CHG_HV_MODE(i)		PMU_CON_SET(PSFR_ANA_CON3, 0x01 << 3, (i) << 3)
#define CHGV_SET(i)			PMU_CON_SET(PSFR_ANA_CON3, 0x0f << 4, (i) << 4)
#define CHGI_TRICKLE_EN(i)	PMU_CON_SET(PSFR_ANA_CON4, 0x01 << 0, (i) << 0)
#define CHGI_SET(i)			PMU_CON_SET(PSFR_ANA_CON4, 0x07 << 1, (i) << 1)
#define CHGI_TRIM_SET(i)	PMU_CON_SET(PSFR_ANA_CON4, 0x0f << 4, (i) << 4)
#define CHG_VLLOOP_EN(i)	PMU_CON_SET(PSFR_ANA_CON5, 0x01 << 0, (i) << 0)
#define CHG_VLLOOP_VSEL(i)	PMU_CON_SET(PSFR_ANA_CON5, 0x01 << 1, (i) << 1)
#define CHGI_GET()			((PMU_CON_GET(PSFR_ANA_CON4) & 0x0e) >> 1)

//BG + LDO
#define WVDDIO_SET(i)		PMU_CON_SET(PSFR_ANA_CON5, 0x0f << 4, (i) << 4)
#define PVBG_SET(i)			PMU_CON_SET(PSFR_ANA_CON6, 0x0f << 0, (i) << 0)
#define MBG_EN(i)			PMU_CON_SET(PSFR_ANA_CON7, 0x01 << 0, (i) << 0)
#define MVIO_EN(i)			PMU_CON_SET(PSFR_ANA_CON7, 0x01 << 1, (i) << 1)
#define MVIO_BYPASS_EN(i)	PMU_CON_SET(PSFR_ANA_CON7, 0x01 << 2, (i) << 2) 
#define MVIO_IFULL_EN(i)	PMU_CON_SET(PSFR_ANA_CON7, 0x01 << 3, (i) << 3)
#define MBG_SET(i)			PMU_CON_SET(PSFR_ANA_CON7, 0x0f << 4, (i) << 4)
#define MVDDIO_SET(i)		PMU_CON_SET(PSFR_ANA_CON8, 0x07 << 0, (i) << 0)

//SARADC
#define PMU_TOADC_EN(i)		PMU_CON_SET(PSFR_ANA_CON8, 0x01 << 6, (i) << 6)
#define PMU_TEST_OE(i)		PMU_CON_SET(PSFR_ANA_CON8, 0x01 << 7, (i) << 7)
#define MVBG_BUF_EN(i)		PMU_CON_SET(PSFR_ANA_CON9, 0x01 << 0, (i) << 0)
#define MVBG_TEST_EN(i)		PMU_CON_SET(PSFR_ANA_CON9, 0x01 << 1, (i) << 1)
#define MVBG_TEST_SET(i)	PMU_CON_SET(PSFR_ANA_CON9, 0x03 << 2, (i) << 2)
#define PMU_TEST_SET(i)		PMU_CON_SET(PSFR_ANA_CON9, 0x0f << 4, (i) << 4)

//CLK
#define RC32M_EN(i)			PMU_CON_SET(PSFR_ANA_CON10, 0x01 << 0, (i) << 0)
#define RC32M_NTRIM_SET(i)	PMU_CON_SET(PSFR_ANA_CON10, 0x07 << 1, (i) << 1)
#define RC32M_PTRIM_SET(i)	PMU_CON_SET(PSFR_ANA_CON10, 0x0f << 4, (i) << 4)
#define RC200K_EN(i)		PMU_CON_SET(PSFR_ANA_CON8, 0x01 << 4, (i) << 4)
#define RC200K_KEEP_EN(i)	PMU_CON_SET(PSFR_ANA_CON16, 0x01 << 3, (i) << 3)

//VIN2VOUT
#define VIN2VOUT_EN(i)		PMU_CON_SET(PSFR_ANA_CON5, 0x01 << 2, (i) << 2) 
#define VIN2VOUT_SSEN(i)	PMU_CON_SET(PSFR_ANA_CON5, 0x01 << 3, (i) << 3) 

//VOUT0 STAGE
#define VOUT0_OUTPUT_EN(i)		PMU_CON_SET(PSFR_ANA_CON11, 0x01 << 0, (i) << 0)
#define VOUT0_CS_EN(i)			PMU_CON_SET(PSFR_ANA_CON11, 0x01 << 1, (i) << 1)
#define VOUT0_CSMODE(i)			PMU_CON_SET(PSFR_ANA_CON11, 0x01 << 2, (i) << 2)
#define VOUT0_CSRES_SET(i)		PMU_CON_SET(PSFR_ANA_CON11, 0x07 << 3, (i) << 3)
#define VOUT0_CSVDSKEEP_EN(i)	PMU_CON_SET(PSFR_ANA_CON12, 0x01 << 0, (i) << 0)
#define VOUT0_CSVDSVOL_SET(i)	PMU_CON_SET(PSFR_ANA_CON12, 0x07 << 1, (i) << 1)
#define VOUT0_INDET_EN(i)		PMU_CON_SET(PSFR_ANA_CON12, 0x01 << 4, (i) << 4)
#define VOUT0_INDET_EN_GET()	(PMU_CON_GET(PSFR_ANA_CON12) & BIT(4))
#define VOUT0_INDET_CS_SET(i)	PMU_CON_SET(PSFR_ANA_CON12, 0x03 << 5, (i) << 5)
#define VOUT0_INDET_GET()		(PMU_CON_GET(PSFR_ANA_READ2) & BIT(2))
#define VOUT0_OUTPUT_EN_GET()	(PMU_CON_GET(PSFR_ANA_CON11) & BIT(0))
#define VOUT0_CSMODE_GET()		(PMU_CON_GET(PSFR_ANA_CON11) & BIT(2))

//VOUT1 STAGE
#define VOUT1_OUTPUT_EN(i)		PMU_CON_SET(PSFR_ANA_CON14, 0x01 << 0, (i) << 0)
#define VOUT1_CS_EN(i)			PMU_CON_SET(PSFR_ANA_CON14, 0x01 << 1, (i) << 1)
#define VOUT1_CSMODE(i)			PMU_CON_SET(PSFR_ANA_CON14, 0x01 << 2, (i) << 2)
#define VOUT1_CSRES_SET(i)		PMU_CON_SET(PSFR_ANA_CON14, 0x07 << 3, (i) << 3)
#define VOUT1_CSVDSKEEP_EN(i)	PMU_CON_SET(PSFR_ANA_CON15, 0x01 << 0, (i) << 0)
#define VOUT1_CSVDSVOL_SET(i)	PMU_CON_SET(PSFR_ANA_CON15, 0x07 << 1, (i) << 1)
#define VOUT1_INDET_EN(i)		PMU_CON_SET(PSFR_ANA_CON15, 0x01 << 4, (i) << 4)
#define VOUT1_INDET_EN_GET()	(PMU_CON_GET(PSFR_ANA_CON15) & BIT(4))
#define VOUT1_INDET_CS_SET(i)	PMU_CON_SET(PSFR_ANA_CON15, 0x03 << 5, (i) << 5)
#define VOUT1_INDET_GET()		(PMU_CON_GET(PSFR_ANA_READ2) & BIT(3))
#define VOUT1_OUTPUT_EN_GET()	(PMU_CON_GET(PSFR_ANA_CON14) & BIT(0))
#define VOUT1_CSMODE_GET()		(PMU_CON_GET(PSFR_ANA_CON14) & BIT(2))

//VOUT OV/UV
#define VOUT_OV_DET_EN(i)		PMU_CON_SET(PSFR_ANA_CON17, 0x01 << 0, (i) << 0)
#define VOUT_OV_COMP_OE(i)		PMU_CON_SET(PSFR_ANA_CON17, 0x01 << 1, (i) << 1)
#define VOUT_OV_VOL_SET(i)		PMU_CON_SET(PSFR_ANA_CON17, 0x07 << 4, (i) << 4)
#define VOUT_UV_DET_EN(i)		PMU_CON_SET(PSFR_ANA_CON18, 0x01 << 0, (i) << 0)
#define VOUT_UV_COMP_OE(i)		PMU_CON_SET(PSFR_ANA_CON18, 0x01 << 1, (i) << 1)
#define VOUT_UV_VOL_SET(i)		PMU_CON_SET(PSFR_ANA_CON18, 0x07 << 4, (i) << 4)
#define VOUT_OV_COMP_GET()		(PMU_CON_GET(PSFR_ANA_READ2) & BIT(5))
#define VOUT_UV_COMP_GET()		(PMU_CON_GET(PSFR_ANA_READ2) & BIT(6))

//NTC
#define NTC_PU_EN(i)			PMU_CON_SET(PSFR_ANA_CON8, 0x01 << 5, (i) << 5)//100K pull up enable

//BOOST TEST SIGNAL
#define BOOST_GMENO_GET()		(PMU_CON_GET(PSFR_ANA_READ0) & BIT(0))
#define BOOST_HCOMPENO_GET()	(PMU_CON_GET(PSFR_ANA_READ0) & BIT(1))
#define BOOST_HCOMP_GET()		(PMU_CON_GET(PSFR_ANA_READ0) & BIT(2))
#define BOOST_LCOMP_GET()		(PMU_CON_GET(PSFR_ANA_READ0) & BIT(3))
#define BOOST_LDONE_GET()		(PMU_CON_GET(PSFR_ANA_READ0) & BIT(4))
#define BOOST_LSTART_GET()		(PMU_CON_GET(PSFR_ANA_READ0) & BIT(5))
#define BOOST_NON_GET()			(PMU_CON_GET(PSFR_ANA_READ0) & BIT(6))
#define BOOST_PON_GET()			(PMU_CON_GET(PSFR_ANA_READ0) & BIT(7))
#define BOOST_PWM_GET()			(PMU_CON_GET(PSFR_ANA_READ1) & BIT(0))
#define BOOST_SS_DONE_GET()		(PMU_CON_GET(PSFR_ANA_READ1) & BIT(1))
#define BOOST_ZCD_GET()			(PMU_CON_GET(PSFR_ANA_READ1) & BIT(2))
#define BOOST_TIMEOUT_GET()		(PMU_CON_GET(PSFR_ANA_READ1) & BIT(3))

//DETECTVE
#define VIN_INDET_GET()			(PMU_CON_GET(PSFR_ANA_READ2) & BIT(0))//return 1 when vin > 2.0V
#define VIN_VBAT_COMP_GET()		(PMU_CON_GET(PSFR_ANA_READ2) & BIT(1))//return 1 when vin > vbat + 100mV
#define VOUT_AVMAX_COMP_GET()	(PMU_CON_GET(PSFR_ANA_READ2) & BIT(4))//return 1 when vout > avmax
#define VPWR_OV_COMP_GET()		(PMU_CON_GET(PSFR_ANA_READ2) & BIT(7))//return 1 when vpwr over voltage

#endif