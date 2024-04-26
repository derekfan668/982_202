#include "includes.h"
#include "app_config.h"
#include "ui.h"

#if defined(TCFG_HALL_ENABLE) && TCFG_HALL_ENABLE

#include "app_var.h"
#include "hall_det.h"

static data u8 hall_cnt; //hall开合盖滤波计数器

#define get_hall_status()    (gpio_read(HALL_GROUP, HALL_PORT) ? 1 : 0)

void hall_scan(void) 
{
    if (get_hall_status() != hall_status) {
        hall_cnt++;
        if (hall_cnt > 30) {
            hall_status = !hall_status;
            b_evt_hall = 1;
#if TCFG_UI_ENABLE
			if (hall_status) {
				ui_run_status(UI_STATUS_OPEN_LID);
			} else {
				ui_run_status(UI_STATUS_CLOSE_LID);
			}
#endif
        }
    } else {
        hall_cnt = 0;
    }
}

void hall_init(void) 
{
    gpio_set_dir(HALL_GROUP, HALL_PORT, 1);
    gpio_set_die(HALL_GROUP, HALL_PORT, 1);
    gpio_set_pull_down(HALL_GROUP, HALL_PORT, 0);
    gpio_set_pull_up(HALL_GROUP, HALL_PORT, 0);
    hall_status = get_hall_status();
}

#endif

