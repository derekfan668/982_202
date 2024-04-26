#ifndef __PMU_H__
#define __PMU_H__

#include "typedef.h"

void PMU_CON_SET(u8 addr, u8 mark, u8 dat);
#define PMU_CON_GET(addr) pmu_sfr_rd(addr)
u8 pmu_sfr_rd(u8 addr);
void pmu_sfr_wr(u8 addr, u8 dat);
void pmu_sfr_and(u8 addr, u8 dat);
void pmu_sfr_or(u8 addr, u8 dat);
void pmu_sfr_xor(u8 addr, u8 dat);


//pmu api
#define pmu_wkupx_level_pending_get(i) 	WKUPx_LEVEL_PENDING_GET(i)
#define pmu_wkupx_edge_pending_get(i) 	WKUPx_EDGE_PENDING_GET(i)
#define pmu_wkupx_clear_pending(i) 		WKUPx_CLEAR_PENDING(i)
#define pmu_wkupx_check_pending() 		WKUPx_INT_GUIDE_GET()	

//wkup_index 0-7
enum {
	WKUP_0_VOUT_OV,
	WKUP_1_VOUT_UV,
	WKUP_2_VOUT_AVMAX_COMP,
	WKUP_3_VPWR_OV,
	WKUP_4_VPWR_DECT,
	WKUP_5_VPWR_VBAT_COMP,
	WKUP_6_VOUT0_DECT,
	WKUP_7_VOUT1_DECT,
	WKUP_MAX,
};
void pmu_wakeup_init(void);

//wkup0-3 only use WKUP_FLT_NOT and WKUP_FLT_32US
//wkup4-7 can use all filter
enum {
	WKUP_FLT_NOT,
	WKUP_FLT_32US,
	WKUP_FLT_64US,
	WKUP_FLT_128US,
	WKUP_FLT_256US,
	WKUP_FLT_512US,
	WKUP_FLT_1MS,
	WKUP_FLT_2MS,
};

//pmu output sel
enum {
	BOOST_GMENO = 1,
	BOOST_HCOMPENO,
	BOOST_HCOMP,
	BOOST_LCOMP,
	BOOST_LDONE,
	BOOST_LSTART,
	BOOST_NON,
	BOOST_PON,
	BOOST_PWM,
	BOOST_SS_DONE,
	BOOST_ZCD,
	BOOST_TIMEOUT,
	VIN_INDET,
	VIN_VBAT_COMP,
	VOUT0_INDET,
	VOUT1_INDET,
	VOUT_AVMAX_COMP,
	VOUT_OV_COMP,
	VOUT_UV_COMP,
	VPWR_OV_COMP,
	LRC_CLK_OUT,
	RCH_CLK_OUT,
};

#define PSFR_ANA_CON0   0x0
#define PSFR_ANA_CON1   0x1
#define PSFR_ANA_CON2   0x2
#define PSFR_ANA_CON3   0x3
#define PSFR_ANA_CON4   0x4
#define PSFR_ANA_CON5   0x5
#define PSFR_ANA_CON6   0x6
#define PSFR_ANA_CON7   0x7

#define PSFR_ANA_CON8   0x8
#define PSFR_ANA_CON9   0x9
#define PSFR_ANA_CON10  0xA
#define PSFR_ANA_CON11  0xB
#define PSFR_ANA_CON12  0xC
#define PSFR_ANA_CON13  0xD
#define PSFR_ANA_CON14  0xE
#define PSFR_ANA_CON15  0xF

#define PSFR_ANA_CON16  0x10
#define PSFR_ANA_CON17  0x11
#define PSFR_ANA_CON18  0x12
#define PSFR_ANA_CON19  0x13

//pmu register
#define PSFR_WKUP_CPND0 0x14
#define PSFR_PMU_OCTL0  0x16
#define PSFR_PMU_OCTL1  0x17
#define PSFR_WKUP_CON0  0x18
#define PSFR_WKUP_CON1  0x19
#define PSFR_WKUP_CON2  0x1A
#define PSFR_WKUP_CON3  0x1B
#define PSFR_WKUP_CON4  0x1C
#define PSFR_WKUP_CON5  0x1D
#define PSFR_WKUP_CON6  0x1E
#define PSFR_WKUP_CON7  0x1F
#define PSFR_WKUP_PND0  0x80   
#define PSFR_WKUP_LVL0  0x84  
#define PSFR_WKUP_GUIDE 0x86 

#define PSFR_ANA_READ0  0x88    //read only
#define PSFR_ANA_READ1  0x89    //read only
#define PSFR_ANA_READ2  0x8A    //read only
#define PSFR_ANA_READ3  0x8B    //read only
#define PSFR_ANA_READ4  0x8C    //read only
#define PSFR_ANA_READ5  0x8D    //read only
#define PSFR_ANA_READ6  0x8E    //read only
#define PSFR_ANA_READ7  0x8F    //read only

#define PMU_RESERVED	PSFR_ANA_CON13
#define LP_KEEP 		PSFR_ANA_CON16

#define WKUPx_AUTO_EN(x, i)	 	PMU_CON_SET(PSFR_WKUP_CON0 + x, 0x01 << 7, (i) << 7)//产生pending后硬件触发自动关闭boost和vin2vout, 仅wkup0和wkup1可用	
#define WKUPx_FLT_SEL(x, i)		PMU_CON_SET(PSFR_WKUP_CON0 + x, 0x07 << 4, (i) << 4)
#define WKUPx_EIE_EN(x, i)		PMU_CON_SET(PSFR_WKUP_CON0 + x, 0x01 << 3, (i) << 3)//edge interrupt enable
#define WKUPx_LIE_EN(x, i) 		PMU_CON_SET(PSFR_WKUP_CON0 + x, 0x01 << 2, (i) << 2)//level interrupt enable
#define WKUPx_INV_EN(x, i) 		PMU_CON_SET(PSFR_WKUP_CON0 + x, 0x01 << 1, (i) << 1)
#define WKUPx_EN(x, i) 			PMU_CON_SET(PSFR_WKUP_CON0 + x, 0x01 << 0, (i) << 0)
	
#define WKUPx_LEVEL_PENDING_GET(i)	(PMU_CON_GET(PSFR_WKUP_LVL0) & BIT(i))
#define WKUPx_EDGE_PENDING_GET(i)	(PMU_CON_GET(PSFR_WKUP_PND0) & BIT(i))
#define WKUPx_INT_GUIDE_GET()		(PMU_CON_GET(PSFR_WKUP_GUIDE) & 0x0F)
#define WKUPx_CLEAR_PENDING(i)		PMU_CON_SET(PSFR_WKUP_CPND0, 0x01 << (i), 1 << (i)) 	

#define PMU_OUTPUT_CONTROL0(i) 		PMU_CON_SET(PSFR_PMU_OCTL0, 0x1f << 0, (i) << 0) 
#define PMU_OUTPUT_CONTROL1(i) 		PMU_CON_SET(PSFR_PMU_OCTL1, 0x1f << 0, (i) << 0)

#endif

