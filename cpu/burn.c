#include "includes.h"
#include "burn.h"
#include "app_config.h"

idata struct otp_burn otp_info;
code const struct otp_trim otp_trim _at_ (0x2FD0);
#ifndef TCFG_USE_FLASH_DEBUG
code const u16 res0_value _at_ (0x6E);//不要刪掉
code const u16 res1_value _at_ (0x70);//不要刪掉
#endif

void burn_init(void)
{
	s8 temp;

	//MVIO
	MVDDIO_SET(TCFG_MVDDIO_LEVEL);

	//WVIO
	if (!otp_trim.trim_en.u.wvio) {
		temp = (s8)otp_trim.pmu0.u.wvio_lev + TCFG_WVDDIO_LEVEL - WVIO_S_2700_MV;
	} else {
		temp = TCFG_WVDDIO_LEVEL;
	}
	if (temp < WVIO_S_2700_MV) {
		temp = WVIO_S_2700_MV;	
	} else if (temp > WVIO_S_3100_MV) {
		temp = WVIO_S_3100_MV;
	}
	WVDDIO_SET(temp);

	//MVBG
	if (!otp_trim.trim_en.u.mvbg) {
		MBG_SET(otp_trim.pmu0.u.mvbg_lev);
	}

	//PVBG
	if (!otp_trim.trim_en.u.pvbg) {
		PVBG_SET(otp_trim.pmu0.u.pvbg_lev);
	} else {
	   //默认为0必须重新设置
		PVBG_SET(0x07);
	}

	//CURRENT
	if (!otp_trim.trim_en.u.vbat_cur) {
		CHGI_TRIM_SET(otp_trim.pmu1.u.vbat_cur);
	} else {
		CHGI_TRIM_SET(8);
	}
	
#ifndef TCFG_USE_FLASH_DEBUG
	//LVD配置,先写EN再写OE
	if (otp_trim.lvd.u.sen) {
		LVD_CON0 = BIT(6) | BIT(0);
	} else {
		LVD_CON0 = BIT(6) | otp_trim.lvd.u.lev;
	} 
	LVD_CON0 |= BIT(3);//LVD_EN
	LVD_CON0 |= BIT(2);//LVD_OE
#endif

	//初始化HRC
	if (otp_trim.hrc_freq1 != 0xffff) {
		otp_info.rch_clk = (u32)otp_trim.hrc_freq1 * 1000;
	} else if (otp_trim.hrc_freq0 != 0xffff) {
		otp_info.rch_clk = (u32)otp_trim.hrc_freq0 * 1000;
	} else {
		otp_info.rch_clk = HRC_CLK_DEF;	
	}

#if TCFG_AUTO_WAKEUP_ENABLE && TCFG_AUTO_WAKEUP_TIME_SEC
	//初始化LRC(精确定时唤醒需要用到)
	if (otp_trim.lrc_freq1 != 0xffff) {
		otp_info.lrc_clk = (u32)otp_trim.lrc_freq1 * 10;
	} else if (otp_trim.lrc_freq0 != 0xffff) {
		otp_info.lrc_clk = (u32)otp_trim.lrc_freq0 * 10;
	} else {
		otp_info.lrc_clk = LRC_CLK_DEF;	
	}
#endif
}

void burn_dump(void)
{
	log_puts("\n-------------------burn dump--------------------\n");
	log_put_u16(otp_trim.vout1_low.value);log_puts(":vout1_low\n");
	log_put_u16(otp_trim.vout0_low.value);log_puts(":vout0_low\n");
	log_put_u16(otp_trim.vout1_high.value);log_puts(":vout1_high\n");
	log_put_u16(otp_trim.pmu0.value);log_puts(":pmu0\n");
	log_put_u16(otp_trim.pmu1.value);log_puts(":pmu1\n");
	log_put_u16(otp_trim.vtemp);log_puts(":vtemp\n");
	log_put_u16(otp_trim.new_vtemp);log_puts(":new_vtemp\n");
	log_put_u16(otp_trim.ntc_100k);log_puts(":ntc_100k\n");
	log_put_u16(otp_trim.hrc_freq0);log_puts(":hrc_freq0\n");
	log_put_u16(otp_trim.lrc_freq0);log_puts(":lrc_freq0\n");
	log_put_u16(otp_trim.lvd.value);log_puts(":lvd_level\n");
	log_put_u16(otp_trim.trim_en.value);log_puts(":cp_trim_en\n");
	log_put_u16(otp_trim.vout0_high.value);log_puts(":vout0_high\n");
	log_put_u16(otp_trim.ntc_10k);log_puts(":ntc_10k\n");
	log_put_u16(otp_trim.hrc_freq1);log_puts(":hrc_freq1\n");
	log_put_u16(otp_trim.lrc_freq1);log_puts(":lrc_freq1\n");
	log_put_u16(otp_trim.cp_pass0);log_puts(":cp_pass0\n");
	log_put_u16(otp_trim.cp_pass1);log_puts(":cp_pass1\n");
	log_puts("-------------------is trim dump-----------------\n");
	log_putchar('1'-otp_trim.trim_en.u.wvio);log_puts(":wvio is trim\n");
	log_putchar('1'-otp_trim.trim_en.u.mvbg);log_puts(":mvbg is trim\n");
	log_putchar('1'-otp_trim.trim_en.u.pvbg);log_puts(":pvbg is trim\n");
	log_putchar('1'-otp_trim.trim_en.u.boost);log_puts(":boost is trim\n");
	log_putchar('1'-otp_trim.trim_en.u.vbat_4p2);log_puts(":vbat_4p2 is trim\n");
	log_putchar('1'-otp_trim.trim_en.u.vbat_4p35);log_puts(":vbat_4p35 is trim\n");
	log_putchar('1'-otp_trim.trim_en.u.vbat_cur);log_puts(":vbat_cur is trim\n");
	log_putchar('1'-otp_trim.trim_en.u.ntc_10k);log_puts(":ntc_10k is trim\n");
	log_putchar('1'-otp_trim.trim_en.u.ntc_100k);log_puts(":ntc_100k is trim\n");
	log_putchar('1'-otp_trim.trim_en.u.vtemp);log_puts(":vtemp_100k is trim\n");
	log_putchar('1'-otp_trim.new_vtemp>>15);log_puts(":new_vtemp_100k is trim\n");
	log_putchar('1'-otp_trim.trim_en.u.vout0_curr_h);log_puts(":vout0_curr_h is trim\n");
	log_putchar('1'-otp_trim.trim_en.u.vout1_curr_h);log_puts(":vout1_curr_h is trim\n");
	log_putchar('1'-otp_trim.trim_en.u.vout0_curr_l);log_puts(":vout0_curr_l is trim\n");
	log_putchar('1'-otp_trim.trim_en.u.vout1_curr_l);log_puts(":vout1_curr_l is trim\n");
	log_puts("-------------------lvd  dump--------------------\n");
	log_put_u8(otp_trim.lvd.u.sen);log_puts(":lvd_sen\n");
	log_put_u8(otp_trim.lvd.u.lev);log_puts(":lvd_lev\n");
	log_puts("-------------------pmu0 dump--------------------\n");
	log_put_u8(otp_trim.pmu0.u.wvio_lev);log_puts(":wvio_lev\n");
	log_put_u8(otp_trim.pmu0.u.mvbg_lev);log_puts(":mvbg_lev\n");
	log_put_u8(otp_trim.pmu0.u.pvbg_lev);log_puts(":pvbg_lev\n");
	log_put_u8(otp_trim.pmu0.u.boost_lev);log_puts(":boost_lev\n");
	log_puts("-------------------pmu1 dump--------------------\n");
	log_put_u8(otp_trim.pmu1.u.vbat_4p2);log_puts(":vbat_4p2\n");
	log_put_u8(otp_trim.pmu1.u.vbat_4p35);log_puts(":vbat_4p35\n");
	log_put_u8(otp_trim.pmu1.u.vbat_cur);log_puts(":vbat_cur\n");
	log_puts("----------------new_boost_trim------------------\n");
	log_put_u8(otp_trim.new_boost.u.new_boost_lev);log_puts(":new_boost_lev\n");
	log_put_u8(otp_trim.new_boost.u.boost_position);log_puts(":boost_position\n");
	log_puts("-------------------vout dump--------------------\n");
	log_put_u8(otp_trim.vout1_low.u.csres);log_puts(":vout1_low csres ");log_put_u16(otp_trim.vout1_low.u.vsense_adc);log_puts(":vout1_low adc\n");
	log_put_u8(otp_trim.vout0_low.u.csres);log_puts(":vout0_low csres ");log_put_u16(otp_trim.vout0_low.u.vsense_adc);log_puts(":vout0_low adc\n");
	log_put_u8(otp_trim.vout1_high.u.csres);log_puts(":vout1_high csres ");log_put_u16(otp_trim.vout1_high.u.vsense_adc);log_puts(":vout1_high adc\n");
	log_put_u8(otp_trim.vout0_high.u.csres);log_puts(":vout0_high csres ");log_put_u16(otp_trim.vout0_high.u.vsense_adc);log_puts(":vout0_high adc\n");
	log_puts("------------------------------------------------\n");
}

