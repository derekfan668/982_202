#include "includes.h"
#include "app_config.h"

#if defined(TCFG_KEY_ENABLE) && TCFG_KEY_ENABLE

#include "app_var.h"
#include "key.h"

typedef struct {
    u8 key_delay_counter : 6;
    u8 last_key : 1;
	u8 filter_key : 1;

	u8 key_filter_counter : 3;
    u8 click_cnt : 3;
	u8 notify_key : 2;

	u8 key_press_counter;
} __key_param;

static data __key_param key_param;

static u8 key_get_value(void) 
{
    if (gpio_read(KEY_GROUP, KEY_PORT) == 0) {
        return 0;
    }
    return NO_KEY;
}

void key_scan(void) 
{
    u8 cur_key;

    //去抖
    cur_key = key_get_value();
    if (cur_key != key_param.filter_key) {
        key_param.filter_key = cur_key;
        key_param.key_filter_counter = 0;
        return;
    } else if (key_param.key_filter_counter < KEY_FILTER_CNT) {
        key_param.key_filter_counter++;
        return;
    }

    //按下和抬起处理
    if (key_param.last_key != cur_key) {
        if (cur_key == NO_KEY) {
            if (key_param.key_press_counter >= KEY_LONG_CNT) {
                b_evt_key_up = 1;
                goto __notify_msg;
            }
#if KEY_MULTI_CLICK_EN
            key_param.key_delay_counter = 1;
#else
			b_evt_key_click = 1;
			goto __notify_msg;
#endif
        } else {
            key_param.key_press_counter = 1;
            if (cur_key != key_param.notify_key) {
#if KEY_MULTI_CLICK_EN
                key_param.click_cnt = 1;
#endif
                key_param.notify_key = cur_key;
            } else {
#if KEY_MULTI_CLICK_EN
                key_param.click_cnt++;
                if (key_param.click_cnt > 5) {
                    key_param.click_cnt = 5;
                }
#endif
            }
        }
    } else {
        if (cur_key == NO_KEY) {
#if KEY_MULTI_CLICK_EN
            if (key_param.click_cnt > 0) {
                if (key_param.key_delay_counter > KEY_DELAY_CNT) {
                    byte_key_evt |= BIT(KEY_CLICK_OFFSET + key_param.click_cnt - 1);
                    goto __notify_msg;
                } else {
                    key_param.key_delay_counter++;
                }
            }
#endif
        } else {
            key_param.key_press_counter++;
            if (key_param.key_press_counter == KEY_LONG_CNT) {
                b_evt_key_long = 1;
                goto __notify_msg;
            } else if (key_param.key_press_counter == KEY_HOLD_CNT) {
                key_param.key_press_counter = KEY_LONG_CNT;
                b_evt_key_hold = 1;
                goto __notify_msg;
            }
        }
    }
    goto __scan_end;
__notify_msg:
#if KEY_MULTI_CLICK_EN
    key_param.click_cnt = 0;
#endif
    key_param.notify_key = NO_KEY;
__scan_end:
    key_param.last_key = cur_key;
}

void key_init(void)
{
    gpio_set_dir(KEY_GROUP, KEY_PORT, 1);
    gpio_set_die(KEY_GROUP, KEY_PORT, 1);
    gpio_set_pull_down(KEY_GROUP, KEY_PORT, 0);
    gpio_set_pull_up(KEY_GROUP, KEY_PORT, 1);
    key_param.last_key = NO_KEY;
}

#endif

