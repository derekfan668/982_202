#ifndef _BURN_H_
#define _BURN_H_

#include "typedef.h"

struct otp_burn {
    u32 rch_clk;        //系统原生时钟频率
	u32 lrc_clk;		//lrc时钟频率
};

union lvd_trim {
	u16 value;
	struct {
		u16 sen:1;
		u16 lev:2;
		u16 reserved:13;
	} u;
};

union pmu0_trim {
	u16 value;
	struct {
		u16 wvio_lev:4;
		u16 mvbg_lev:4;
		u16 pvbg_lev:4;
		u16 boost_lev:4;
	} u;
};

union pmu1_trim {
	u16 value;
	struct {
		u16 vbat_4p2:4;
		u16 vbat_4p35:4;
		u16 vbat_cur:4;
		u16 reserved:4;
	} u;
};
union new_boost_trim {
	u16 value;
	struct {
		u16 new_boost_lev:4;
		u16 boost_position:2;
		u16 reserved:10;
	} u;
};
union cp_trim_en {
	u16 value;
	struct {
	  	u16 wvio:1;
		u16	mvbg:1;
		u16 pvbg:1;
		u16 boost:1;
		u16 vbat_4p2:1;
		u16 vbat_4p35:1;
		u16 vbat_cur:1;
		u16 ntc_10k:1;
		u16 ntc_100k:1;
		u16 vtemp:1;
		u16 vout0_curr_h:1;
		u16 vout1_curr_h:1;
		u16 vout0_curr_l:1;
		u16 vout1_curr_l:1;
		u16 reserved:2;
	} u;
};

union vout_cur_trim {
	u16 value;
	struct {
		u16 csres:3;
		u16 reserved:3;
		u16 vsense_adc:10;
	} u;
};

struct otp_trim {
	u16 reserved0;
	u16 reserved1;
	u16 reserved2;
	union new_boost_trim new_boost;
	u16 new_vtemp;
	union vout_cur_trim vout1_low;
	union vout_cur_trim vout0_low;
	union vout_cur_trim vout1_high;
    union pmu0_trim pmu0;
	union pmu1_trim pmu1;
	u16 vtemp;
	u16 ntc_100k;
	u16 hrc_freq0;
	u16 lrc_freq0;
    u16 user_key0;
	u16 user_key1;
	union lvd_trim lvd;
	union cp_trim_en trim_en;
	union vout_cur_trim vout0_high;
	u16 ntc_10k;
	u16 hrc_freq1;
	u16 lrc_freq1;
	u16 cp_pass0;
	u16 cp_pass1;
};

void burn_init(void);
void burn_dump(void);
extern idata struct otp_burn otp_info;
extern code const struct otp_trim otp_trim;
extern code const u16 res0_value;
extern code const u16 res1_value;

#endif  /*  _BURN_H_ */


