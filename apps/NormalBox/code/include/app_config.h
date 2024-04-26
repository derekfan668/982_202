#ifndef _APP_CONFIG_H
#define _APP_CONFIG_H

/************************************************************************************
Ӳ����Դռ�����:
--------ģ��-------Ӳ����Դ-----OUTPUT_CHL_X------INPUT_CHL_X------IO----------------
1   ���ߴ���ͨ��    UART0       OUTPUT_CHL_1      INPUT_CHL_0      P10+P11
2	���Դ���        UART1       OUTPUT_CHL_3	                   P06
3   HALL������                                    INPUT_CHL_2      P14
4   ��������                                      INPUT_CHL_3      P15
5   NTC���                                                        P16
6   ��������        TIMER1_CAP     				  INPUT_CHL_1      P17 (����,flash���̿���)
7   ���߳��        WPR                                            P17+P13 (����)
8   lighting����    PPM                                            P24
9   ϵͳ��ʱ��      TIMER0                                                
************************************************************************************/

//*********************************************************************************//
//                                 ���ÿ�ʼ                                        //
//*********************************************************************************//
#define ENABLE_THIS_MOUDLE					1
#define DISABLE_THIS_MOUDLE					0

#define ENABLE								1
#define DISABLE								0

#define TCFG_MUX_BUFFER_ADDR				0x40//���ܶ�

//Ĭ�ϲ�������
#define TCFG_TIMER_LOOP                     2   //ϵͳ��ʱ��������ֵms

//���ڵ��Թ���
#define TCFG_DEBUG_ENABLE                   DEBUG_EN
#define TCFG_DEBUG_BAUD                     115200L
#define TCFG_DEBUG_USED_UARTX               1   //0=ʹ��UART0��ӡ, 1=ʹ��UART1��ӡ
#define DEBUG_GROUP                         0
#define DEBUG_PORT                          6

//HALLʹ�ܿ���
#define TCFG_HALL_ENABLE                    ENABLE_THIS_MOUDLE
#define HALL_GROUP                          1
#define HALL_PORT                           4
#define HALL_IO								IO_PORT_1_4

//KEY ʹ�ܿ��� �̶�ʹ��P15
#define TCFG_KEY_ENABLE                     ENABLE_THIS_MOUDLE
#define KEY_GROUP                           1
#define KEY_PORT                            5
#define KEY_IO								IO_PORT_1_5

//WPR ���߳��,�̶�ʹ��P17Ϊͨ�ſ�,ѡ��P13��Ϊ�����߼������
#define TCFG_WPR_ENABLE                     ENABLE_THIS_MOUDLE
#define TCFG_WPR_VOLTAGE_NORMAL				5800//VWP�������ĵ�ƽ
#define WPR_USED_P05        				0
#define WPR_USED_P17        				1
#define WPR_USED_PORT       				WPR_USED_P17
#define WPR_DET_IO_GROUP                    1
#define WPR_DET_IO_PORT                     3
#define WPR_DET_AD_CH                       AD_IO_CH_P13
#define WPR_DET_DIV_COEFF        			4 //��ѹϵ��������Ӳ������

#ifdef TCFG_USE_FLASH_DEBUG
//����FLASH����,P17�̶�Ϊ������,��ע�����߳�縴�ã�flash�ŻῪ����
#define TCFG_UPDATE_ENABLE					ENABLE_THIS_MOUDLE
#else
#define TCFG_UPDATE_ENABLE					DISABLE_THIS_MOUDLE
#endif

//lighting����,�̶�ʹ��P24
#define TCFG_HANDSHAKE_ENABLE               ENABLE_THIS_MOUDLE

//POWER ������� �����������
#define VBAT_TYPE_4200                      0
#define VBAT_TYPE_4350                      1
#define VBAT_TYPE_SEL                       VBAT_TYPE_4200
#if (VBAT_TYPE_SEL == VBAT_TYPE_4200)
#define POWER_TOP_LEV                       (4100)	//4100
#define POWER_BOOT_LEV                      (3300)	//3300
#else
#define POWER_TOP_LEV                       (4250) //4100*1.036
#define POWER_BOOT_LEV                      (3420) //3300*1.036
#endif

//UI ����,���Ʒ���������ܷ�����
#define TCFG_UI_ENABLE                      ENABLE_THIS_MOUDLE
#define UI_PWM_LED                          0
#define UI_SEG_LED		                    1
#define TCFG_UI_MODE                        UI_PWM_LED
#if (TCFG_UI_MODE == UI_PWM_LED)
//A --> P00 P01 P02
//B --> P13 P14 P15
#define PWM_LED_USED_GROUP_A                0
#define PWM_LED_USED_GROUP_B                1
#define TCFG_PWM_LED_GROUP_SEL              PWM_LED_USED_GROUP_A
#elif (TCFG_UI_MODE == UI_SEG_LED)
//����seg_led.h����ʵ��ʹ�õ�io
//MODE0: P00 P01 P02 P03 P04
//MODE1: P00 P01 P02 P03 P04 P05
#define SEG_LED_MODE0						0 //��ʾ���յ���
#define SEG_LED_MODE1						1 //��ʾ���յ���+���Ҷ�����--��ͨ���ղ�֧����ʾ���Ҷ�����
#define TCFG_SEG_LED_MODE					SEG_LED_MODE0
#endif

//������λ����
#define TCFG_LONG_PRESS_RESET_ENABLE		ENABLE_THIS_MOUDLE
#define IO_INPUT_CHANNEL0					0 //��֧��-����ͨ��ռ��
#define VPWR_IN_DETECT						1
#define LONG_PRESS_PINR_IN_SEL				VPWR_IN_DETECT //������λ���ź�Դѡ��
#define TIME_1S								0
#define TIME_2S								1
#define TIME_4S								2
#define TIME_8S								3
#define LONG_PRESS_TIME 					TIME_4S
#define LOW_VAIL							0
#define HIGH_VAIL							1
#if (LONG_PRESS_PINR_IN_SEL == IO_INPUT_CHANNEL0)
#define LONG_PRESS_RESET_IO					IO_PORT_1_5
#define LONG_PRESS_PINR_LEVEL				LOW_VAIL
#else
#define LONG_PRESS_RESET_IO					NO_CONFIG_PORT
#define LONG_PRESS_PINR_LEVEL				HIGH_VAIL
#endif

//CHARGER
#define TCFG_CHARGE_OPTIMIZE_ENABLE			ENABLE_THIS_MOUDLE //����������
#if (VBAT_TYPE_SEL == VBAT_TYPE_4200)
#define CHARGE_CONSTANT_VOLTAGE				CHGV_S_4199_MV
#else
#define CHARGE_CONSTANT_VOLTAGE				CHGV_S_4354_MV
#endif
#define CHARGE_RECHARGE_PERCENT				80 //�س��ص����ٷֱ�
#define CHARGE_CURRENT_MA					CHGI_S_300_MA//���߳�����
#define CHARGE_WIRELESS_CURRENT_MA			CHGI_S_200_MA//���߳�����
#define CHARGE_WIRELESS_OV_CURRENT_MA		CHGI_S_100_MA//���߹�ѹ������
#define CHARGE_FULL_CURRENT_MA				30 //��������/10
#define CHARGE_TC_2_CC_VOLT_MV				3000//���ת�����ĵ�ص�ѹ
	
//оƬ����NTC
#define TCFG_INSIDE_NTC_ENABLE              ENABLE_THIS_MOUDLE	//����NTC
//���NTC ��⿪�� �̶�ʹ��P16���ܸĶ�
#define TCFG_EXTERN_NTC_ENABLE              ENABLE_THIS_MOUDLE //����NTC
#define TCFG_EXTERN_NTC_100K				0
#define TCFG_EXTERN_NTC_10K					1
#define TCFG_EXTERN_NTC_TYPE				TCFG_EXTERN_NTC_10K
#define NTC_MODE_JEITA                      0 //JEITA��׼
#define NTC_MODE_GB                         1 //����
#define NTC_MODE_SEL                        NTC_MODE_GB
#define NTC_GROUP                           1
#define NTC_PORT                            6
#define NTC_DET_AD_CH                       AD_IO_CH_P16
#if (NTC_MODE_SEL == NTC_MODE_JEITA) 
#define NTC_CHARGE_CURRENT_MA				CHGI_S_100_MA
#if (VBAT_TYPE_SEL == VBAT_TYPE_4200)
#define NTC_CHARGE_CONSTANT_VOLTAGE			CHGV_S_4101_MV
#else
#define NTC_CHARGE_CONSTANT_VOLTAGE			CHGV_S_4257_MV
#endif
#endif

//BOOST
#define TCFG_BOOST_OPTIMIZE_ENABLE			DISABLE_THIS_MOUDLE //����䣨�������ߺ�����������С��150mA��
#define TCFG_MODE_FOLLOW_MAX				0 //����ϴ�����Ķ������зŵ�
#define TCFG_MODE_FOLLOW_MIN				1 //�����С�����Ķ������зŵ�
#define TCFG_BOOST_OPTIMIZE_MODE			TCFG_MODE_FOLLOW_MIN
#define TCFG_EAR_CHARGE_CURRENT_MA			40//��������������
#define TCFG_EAR_CHARGE_FULL_CURRENT_MA     5//��������������� 
#define TCFG_BOOST_VOUT_S					BOOST_VOUT_S_4700_MV
#define TCFG_DISCHARGE_OVERTIME				4//*30mins��������糬ʱ

//VIN2VOUT
#define TCFG_VIN2VOUT_ENABLE				(!TCFG_WPR_ENABLE) //�����߳䷽��������vin2vout

//CURRENT LIMIT �ŵ�������
#define TCFG_CURRENT_LIMIT_MA				100 //���Ƶ������ŵ����

//VOUT_INDET �������ؼ��
#define VOUT_INDET_CURRENT					VOUT_INDET_CS_S_0P5_UA

//VOUT OV UV
#define VOUT_OV_VOLT						VOUT_OV_VOL_S_5P53V_TO_5P34V
#define VOUT_UV_VOLT						VOUT_UV_VOL_S_3P20V_TO_3P35V

#define TCFG_AUTO_WAKEUP_ENABLE				ENABLE_THIS_MOUDLE
#define WAKEUP_TIME_5_DAYS					0
#define WAKEUP_TIME_10_DAYS					1
#define WAKEUP_TIME_15_DAYS					2
#define TCFG_AUTO_WAKEUP_TIME_DAY           WAKEUP_TIME_10_DAYS //��ʱ����ʱ��/�죬��ѡ�Ѽ���õ�5/10/15��
#define TCFG_AUTO_WAKEUP_TIME_SEC			0//��ʱ����ʱ��/�룬0��Ĭ��ʹ��TCFG_AUTO_WAKEUP_TIME_DAY 

//vddio��ѹ��λ����
#define TCFG_MVDDIO_LEVEL 					MVIO_S_3000_MV
#define TCFG_WVDDIO_LEVEL 					WVIO_S_2700_MV

#endif    /*    _APP_CONFIG_H    */



