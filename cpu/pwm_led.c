#include "includes.h"
#include "app_config.h"
#include "app_var.h"
#include "pwm_led.h"
#include "ui.h"
#include "power_det.h"

#if TCFG_UI_ENABLE && (TCFG_UI_MODE == UI_PWM_LED)

#if (TCFG_PWM_LED_GROUP_SEL == PWM_LED_USED_GROUP_A)
#define LED_GROUP       0
#define LED0_PORT       0
#define LED1_PORT       1
#define LED2_PORT       2
#define LED_PWM_IOS()   PWM_LED_IOS_A()
#else
#define LED_GROUP       1
#define LED0_PORT       3
#define LED1_PORT       4
#define LED2_PORT       5
#define LED_PWM_IOS()   PWM_LED_IOS_B()
#endif
#define LED_ALL_PORT    (BIT(LED0_PORT) | BIT(LED1_PORT) | BIT(LED2_PORT))

//PWMʱ�Ӽ��㹫ʽΪ:CLK(ʱ��Դ)/DIV(��Ƶ��)
//��һ��PWM�жϵ�ʱ��Ϊ: T = Tpwm * D_PWM_PRD
#define PWM_DIV_SEL     PWM_FREQ_DIV256
#define PWM_CLK_SEL     PWM_CLK_SEL_SRC_32M //����Ƶ��(26M)
//�����Ƶİ�����(��������ʱ��) = T * D_PWM_DUTY * D_PWM_PRD / 100
#define D_PWM_PRD       228	
#define D_PWM_DUTY      50  //%�ߵ�ƽռ�ձ�0~100
#define D_PWM_DIV		2   //���ƺ��������/��͵�ʱ��(100/D_PWM_DUTY)0~100��ʱ��=D_PWM_DUTY*D_PWM_PRD*D_PWM_DIV*Tpwm/100							   
#define LED_FLASH_TMR   44  //LED_FLASH_TMR * D_PWM_PRD * T = 100ms

typedef struct {
	u8 div;			//0~100
    u8 duty;        //0~100
    u8 time;    	//����ʱ��/�����Ĵ���
    u8 onoff : 1;   //����״̬
    u8 light : 1;   //����״̬
    u8 breath : 1;  //����״̬
    u8 busy : 1;    //�ƴ�������̬
} LED_INFO;

static idata LED_INFO led_info[3];
static idata u8 led_scan_cnt, ui_status, ui_new_status;

//--------------------------------------Ӳ������------------------------------------------------------//
#define led0_set_pwm(x)     LED0_DUTY = x
#define led1_set_pwm(x)     LED1_DUTY = x
#define led2_set_pwm(x)     LED2_DUTY = x

void pwm_led_off(u8 led)
{
	led_info[led].light = 0; 
	led_info[led].breath = 0; 
	led_info[led].onoff = 0;
}

void pwm_led_set(u8 led, u8 set_type, u8 time)
{
	if (set_type == LED_SET_BREATH) {
		led_info[led].light = 0;
		led_info[led].breath = 1;	
	} else {
		led_info[led].light = 1;
    	led_info[led].breath = 0;
	}
    led_info[led].time = time;
    led_info[led].busy = 1;
}

void pwm_led_scan_fast(u8 led)
{	
    if (led_info[led].onoff) {
        //������
        if (led_info[led].duty >= (D_PWM_DUTY*D_PWM_PRD/100)) {
			if (led_info[led].breath) {
            	led_info[led].onoff = 0;
        	}
        } else {
			if ((led_info[led].div == D_PWM_DIV)) {
				led_info[led].div = 0;
            	led_info[led].duty++;
			} else {
				led_info[led].div++;
			}
        }
    } else {
        //������
        if (led_info[led].duty == 0) {
            led_info[led].busy = 0;
            //����δ���,busy����
            if (led_info[led].breath) {
                if (led_info[led].time) {
                    if (led_info[led].time != 0xff) {
                        led_info[led].time--;
                    }
                    led_info[led].onoff = 1;
                    led_info[led].busy = 1;
                } else {
                    led_info[led].breath = 0;
                }
            }
            //������,busy����
            if (led_info[led].light) {
                led_info[led].busy = 1;
            }
        } else {
			if ((led_info[led].div == D_PWM_DIV)) {
				led_info[led].div = 0;
            	led_info[led].duty--;
			} else {
				led_info[led].div++;
			}
        }
    }
}

void pwm_led_scan_slow(u8 led)
{	
    if (led_info[led].light) {
        led_info[led].busy = 1;
        if (led_info[led].time) {
            led_info[led].onoff = 1;
            if (led_info[led].time != 0xff) {
                led_info[led].time--;
            }
        } else {
            led_info[led].light = 0;
            led_info[led].onoff = 0;
        }
    }
}

void ui_update_power(void)
{
	u8 power_lev = power_get_vbat_percent();
	if (power_lev < 50) {
		if (b_sys_busy_in_usb_on) {
			pwm_led_set(LED_RED, LED_SET_LIGHT, 40);//����4s
		} else {
			pwm_led_set(LED_RED, LED_SET_BREATH, 8);//����8��
		}
	} else if (power_lev < 60) {
		pwm_led_set(LED_RED, LED_SET_LIGHT, 80);//����8s
	} else {
		pwm_led_set(LED_GREEN, LED_SET_LIGHT, 80);//����8s
	}
}

void ui_update_status(u8 status)
{
	switch (status) {
	case UI_STATUS_USB_IN:
	case UI_STATUS_USB_OUT:
    case UI_STATUS_EAR0_IN:
    case UI_STATUS_EAR0_OUT:
    case UI_STATUS_EAR1_IN:
    case UI_STATUS_EAR1_OUT:
	case UI_STATUS_OPEN_LID:
	case UI_STATUS_CLOSE_LID:
	case UI_STATUS_KEY_CLICK:
	case UI_STATUS_PAIR_STOP:
		ui_update_power();
		break;
	case UI_STATUS_PAIR_PRE:
		pwm_led_set(LED_WHITE, LED_SET_LIGHT, 0xff);//�׵Ƴ���
		break;
	case UI_STATUS_PAIRING:
		pwm_led_set(LED_WHITE, LED_SET_BREATH, 0xff);//�׵ƺ���
		break;
	case UI_STATUS_SYS_ABNORMAL:
		pwm_led_set(LED_RED, LED_SET_BREATH, 1);//��ƺ���
		break;
	}
}

void pwm_led_isr(void) interrupt PWM_LED_INT
{	
    LED_CON0 |= BIT(6);
    pwm_led_scan_fast(0);
    pwm_led_scan_fast(1);
    pwm_led_scan_fast(2);
    if ((led_scan_cnt % LED_FLASH_TMR) == 0) {
        led_scan_cnt = 0;
        pwm_led_scan_slow(0);
        pwm_led_scan_slow(1);
        pwm_led_scan_slow(2);
    }
    led_scan_cnt++;
    led0_set_pwm(led_info[0].duty);
    led1_set_pwm(led_info[1].duty);
    led2_set_pwm(led_info[2].duty);
    if ((!led_info[0].busy) && (!led_info[1].busy) && (!led_info[2].busy)) {
        if (ui_new_status != UI_STATUS_NULL) {
			ui_update_status(ui_new_status);
			ui_new_status = UI_STATUS_NULL;
		} else {
			b_sys_busy_in_led = 0;
			PWM_LED_IE(0);
			ui_status = UI_STATUS_NULL;
			//log_puts("led idle!\n");
		}
    }
}
	
//--------------------------------------���API-------------------------------------------------------//
void ui_run_status(u8 status) 
{	
    if (status == ui_status) {
        return;
    }
	//���״̬ʱ����ʾ�����UI
	if (sys_info.pair_status && (status < UI_STATUS_PAIR_STOP)) {
		return;
	}
    PWM_LED_IE(0);
	//�����
	pwm_led_off(0);
	pwm_led_off(1);
	pwm_led_off(2);
	ui_new_status = status;
	ui_status = status;
	led_scan_cnt = 0;
	if (status != UI_STATUS_SYS_ABNORMAL) {//�쳣UI��δ��ʾ��ɾ�ֱ�ӹػ�
    	b_sys_busy_in_led = 1;
	}
    PWM_LED_IE(1);
}

void ui_init(void) 
{	
    ui_status = UI_STATUS_NULL;
	ui_new_status = UI_STATUS_NULL;
	LED0_DUTY = 0;//Ҫ��ʼ��,��ֵ��ȷ��
	LED1_DUTY = 0;
	LED2_DUTY = 0;
    LED_PWM_IOS();
    LED_PRD = D_PWM_PRD;
    LED_CON1 = 0;
    LED_CON0 = BIT(6) | (PWM_CLK_SEL << 5) | (PWM_DIV_SEL << 1) | BIT(0);
    LED_ALL_OUTPUT_EN();
    IO_WRITE_OR(LED_GROUP, LED_ALL_PORT);
    IO_DIR_AND(LED_GROUP, ~LED_ALL_PORT);
}

void ui_uninit(void)
{
	LED_ALL_OUTPUT_DIS();
    IO_WRITE_AND(LED_GROUP, ~LED_ALL_PORT);
}

#endif

