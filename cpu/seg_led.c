#include "includes.h"
#include "app_config.h"
#include "app_var.h"
#include "seg_led.h"
#include "ui.h"
#include "power_det.h"
#include "chargebox_hw.h"

#if TCFG_UI_ENABLE && (TCFG_UI_MODE == UI_SEG_LED)

#define FLASH_PERIOD	50//闪烁的周期(FLASH_PERIOD*ui_scan的调用周期*LED_X_MAX)ms

static data u8 led_flash[LED_Y_MAX] = {0};
static data u8 led_onoff[LED_Y_MAX] = {0};
static data u16 flash_cnt;

typedef struct {
    u8 led_x_index : 3;
	u8 led_y_index : 4;
	u8 flash_flag : 1;
} __seg_led;

__seg_led seg_led_p;

code const u8 seg_table[10] = {
    (SEGA | SEGB | SEGC | SEGD | SEGE | SEGF),
    (SEGB | SEGC),
    (SEGA | SEGB | SEGD | SEGE | SEGG),
    (SEGA | SEGB | SEGC | SEGD | SEGG),
    (SEGB | SEGC | SEGF | SEGG),
    (SEGA | SEGC | SEGD | SEGF | SEGG),
    (SEGA | SEGC | SEGD | SEGE | SEGF | SEGG),
    (SEGA | SEGB | SEGC),
    (SEGA | SEGB | SEGC | SEGD | SEGE | SEGF | SEGG),
    (SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
};

void set_seg_param(u8 dig_type, u8 status) reentrant
{
    u8 x, y;
	Timer0_IE(0);
    x = dig_type & 0x0f;
    y = (dig_type >> 4) & 0x0f;
	if (y < LED_Y_MAX) {
    	if (status == LED_STATUS_FLASH) {
        	led_flash[y] |=  BIT(x);
    	} else if (status == LED_STATUS_ON) {
        	led_flash[y] &= ~BIT(x);
			led_onoff[y] |= BIT(x);
    	} else {
        	led_onoff[y] &= ~BIT(x);
        	led_flash[y] &= ~BIT(x);
    	}
	}
	Timer0_IE(1);
}

void set_seg_out(u8 seg, u8 out)
{
    switch (seg) {
    case 0:
        DIG_LED0_OUT(out);
        break;
    case 1:
        DIG_LED1_OUT(out);
        break;
    case 2:
        DIG_LED2_OUT(out);
        break;
    case 3:
        DIG_LED3_OUT(out);
        break;
    case 4:
        DIG_LED4_OUT(out);
        break;
	 case 5:
        DIG_LED5_OUT(out);
        break;
    }
}

void set_hundred_seg(u8 seg, u8 status)
{
    if (seg & SEGB) {
        set_seg_param(DIG_1B, status);
    }
    if (seg & SEGC) {
        set_seg_param(DIG_1C, status);
    }
}

void set_ten_seg(u8 seg, u8 status)
{
    if (seg & SEGA) {
        set_seg_param(DIG_2A, status);
    }
    if (seg & SEGB) {
        set_seg_param(DIG_2B, status);
    }
    if (seg & SEGC) {
        set_seg_param(DIG_2C, status);
    }
    if (seg & SEGD) {
        set_seg_param(DIG_2D, status);
    }
    if (seg & SEGE) {
        set_seg_param(DIG_2E, status);
    }
    if (seg & SEGF) {
        set_seg_param(DIG_2F, status);
    }
    if (seg & SEGG) {
        set_seg_param(DIG_2G, status);
    }
}

void set_unit_seg(u8 seg, u8 status)
{
    if (seg & SEGA) {
        set_seg_param(DIG_3A, status);
    }
    if (seg & SEGB) {
        set_seg_param(DIG_3B, status);
    }
    if (seg & SEGC) {
        set_seg_param(DIG_3C, status);
    }
    if (seg & SEGD) {
        set_seg_param(DIG_3D, status);
    }
    if (seg & SEGE) {
        set_seg_param(DIG_3E, status);
    }
    if (seg & SEGF) {
        set_seg_param(DIG_3F, status);
    }
    if (seg & SEGG) {
        set_seg_param(DIG_3G, status);
    }

#if (TCFG_SEG_LED_MODE == SEG_LED_MODE1)
	//点亮"%"
	set_seg_param(DIG_K9, status);
#endif
}

//普通充电仓不支持显示耳机电量
#ifndef TCFG_NORMALBOX_CASE
#if (TCFG_SEG_LED_MODE == SEG_LED_MODE1)
void update_ear_l_power_seg(void) 
{
	if (!ear_info[EAR_L].online_comm || (ear_info[EAR_L].power == 0xff)) {
		set_seg_param(DIG_K1, LED_STATUS_OFF);
		set_seg_param(DIG_K2, LED_STATUS_OFF);
		set_seg_param(DIG_K3, LED_STATUS_OFF);
		set_seg_param(DIG_K4, LED_STATUS_OFF);
		set_seg_param(DIG_K5, LED_STATUS_OFF);
		return;
	}
		
	if (ear_info[EAR_L].power >= 90) {
		set_seg_param(DIG_K1, LED_STATUS_ON);
		set_seg_param(DIG_K2, LED_STATUS_ON);
		set_seg_param(DIG_K3, LED_STATUS_ON);
	} else if (ear_info[EAR_L].power >= 60) {
		set_seg_param(DIG_K1, LED_STATUS_OFF);
		set_seg_param(DIG_K2, LED_STATUS_ON);
		set_seg_param(DIG_K3, LED_STATUS_ON);
	} else if (ear_info[EAR_L].power >= 30) {
		set_seg_param(DIG_K1, LED_STATUS_OFF);
		set_seg_param(DIG_K2, LED_STATUS_OFF);
		set_seg_param(DIG_K3, LED_STATUS_ON);
	} else {
		set_seg_param(DIG_K1, LED_STATUS_OFF);
		set_seg_param(DIG_K2, LED_STATUS_OFF);
		set_seg_param(DIG_K3, LED_STATUS_OFF);
	}
	set_seg_param(DIG_K4, LED_STATUS_ON);
	set_seg_param(DIG_K5, LED_STATUS_ON);	
}

void update_ear_r_power_seg(void) 
{
	if (!ear_info[EAR_R].online_comm || (ear_info[EAR_R].power == 0xff)) {
		set_seg_param(DIG_K13, LED_STATUS_OFF);
		set_seg_param(DIG_K12, LED_STATUS_OFF);
		set_seg_param(DIG_K11, LED_STATUS_OFF);
		set_seg_param(DIG_K10, LED_STATUS_OFF);
		set_seg_param(DIG_K8, LED_STATUS_OFF);
		return;
	}
		
	if (ear_info[EAR_R].power >= 90) {
		set_seg_param(DIG_K13, LED_STATUS_ON);
		set_seg_param(DIG_K12, LED_STATUS_ON);
		set_seg_param(DIG_K11, LED_STATUS_ON);
	} else if (ear_info[EAR_R].power >= 60) {
		set_seg_param(DIG_K13, LED_STATUS_OFF);
		set_seg_param(DIG_K12, LED_STATUS_ON);
		set_seg_param(DIG_K11, LED_STATUS_ON);
	} else if (ear_info[EAR_R].power >= 30) {
		set_seg_param(DIG_K13, LED_STATUS_OFF);
		set_seg_param(DIG_K12, LED_STATUS_OFF);
		set_seg_param(DIG_K11, LED_STATUS_ON);
	} else {
		set_seg_param(DIG_K13, LED_STATUS_OFF);
		set_seg_param(DIG_K12, LED_STATUS_OFF);
		set_seg_param(DIG_K11, LED_STATUS_OFF);
	}
	set_seg_param(DIG_K10, LED_STATUS_ON);
	set_seg_param(DIG_K8, LED_STATUS_ON);			
}
#endif
#endif

void update_vbat_percent_seg(void)
{  
	set_ten_seg(seg_table[8], LED_STATUS_OFF);
	set_unit_seg(seg_table[8], LED_STATUS_OFF);

	set_hundred_seg(seg_table[1], ((power_get_vbat_percent()/100) ? LED_STATUS_ON : LED_STATUS_OFF));
	set_ten_seg(seg_table[power_get_vbat_percent()/10%10], LED_STATUS_ON);
	set_unit_seg(seg_table[power_get_vbat_percent()%10], LED_STATUS_ON);		
}

void ui_scan(void)
{
	DIG_ALL_OFF();
    //亮起控制
	for (seg_led_p.led_y_index = 0; seg_led_p.led_y_index < LED_Y_MAX; seg_led_p.led_y_index++) {
    	if (led_onoff[seg_led_p.led_y_index] & BIT(seg_led_p.led_x_index)) { 
        	set_seg_out(seg_led_p.led_x_index, 0);
			set_seg_out(seg_led_p.led_y_index, 1);
    	}
	}
	
	seg_led_p.led_x_index++;
	if (seg_led_p.led_x_index < LED_X_MAX) {
		return;
	}

    //闪烁控制
    flash_cnt++;
    if (flash_cnt >= FLASH_PERIOD) {
        flash_cnt = 0;
        seg_led_p.flash_flag = !seg_led_p.flash_flag;
        for (seg_led_p.led_y_index = 0; seg_led_p.led_y_index < LED_Y_MAX; seg_led_p.led_y_index++) {
			for (seg_led_p.led_x_index = 0; seg_led_p.led_x_index < LED_X_MAX; seg_led_p.led_x_index++) {
                if (led_flash[seg_led_p.led_y_index] & BIT(seg_led_p.led_x_index)) {
                    if (seg_led_p.flash_flag) {
                        led_onoff[seg_led_p.led_y_index] |= BIT(seg_led_p.led_x_index);
                    } else {
                        led_onoff[seg_led_p.led_y_index] &= ~BIT(seg_led_p.led_x_index);
                    }
                }
            }
        }
    }
	seg_led_p.led_x_index = 0;
}

void ui_run_status(u8 status)
{
	switch (status) {
#ifndef TCFG_NORMALBOX_CASE
#if (TCFG_SEG_LED_MODE == SEG_LED_MODE1)
	case UI_STATUS_UPDATE_EAR_L_POWER:
		update_ear_l_power_seg();
		break;
	case UI_STATUS_UPDATE_EAR_R_POWER:
		update_ear_r_power_seg();
		break;
#endif
#endif
	case UI_STATUS_UPDATE_VBAT_PERCENT:
		update_vbat_percent_seg();
		break;
	}
}

void ui_init(void)
{
	seg_led_p.led_x_index = 0;
    seg_led_p.led_y_index = 0;
	DIG_ALL_INIT();
}

void ui_uninit(void)
{

}

#endif

