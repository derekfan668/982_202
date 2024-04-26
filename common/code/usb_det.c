#include "includes.h"
#include "usb_det.h"
#include "app_var.h"

#define USB_DET_ONLINE_TIME     (16) //USB²åÈë¼ì²âÂË²¨´ÎÊý
#define USB_DET_OFFLINE_TIME    (16) //USB°Î³ö¼ì²âÂË²¨´ÎÊý

typedef struct {
    u8 on_counter;
	u8 off_counter;
} __usb_det;

__usb_det usb_det;
#if TCFG_WPR_ENABLE
static u8 wpr_filter_cnt;
#endif

void usb_scan(void)
{
#if TCFG_WPR_ENABLE
	if (wpr_common) {
		wpr_filter_cnt = 2;
		return;
	} else if (wpr_filter_cnt) {
		wpr_filter_cnt--;
	}
#endif

	if (VIN_INDET_GET() && VIN_VBAT_COMP_GET() && (!VPWR_OV_COMP_GET())) {
		usb_det.off_counter = 0;
		usb_det.on_counter++;
		if (usb_det.on_counter > USB_DET_ONLINE_TIME) {
			usb_det.on_counter = 0;
			if (!sys_info.localcharge) {
				usb_shake_flag = 0;
				sys_info.localcharge = 1;
				b_evt_usb_in = 1;
			}	
		}
	} else {
		usb_det.on_counter = 0;
		usb_det.off_counter++;
		usb_shake_flag = 1;
		if (usb_det.off_counter > USB_DET_OFFLINE_TIME) {
			usb_det.off_counter = 0;
			if (sys_info.localcharge) {
				sys_info.localcharge = 0;
				b_evt_usb_out = 1;
				usb_shake_flag = 0;
			}	
		}
	}		
}
