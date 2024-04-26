#ifndef _APP_CONFIG_H
#define _APP_CONFIG_H

/************************************************************************************
硬件资源占用情况:
--------模块-------硬件资源-----OUTPUT_CHL_X------INPUT_CHL_X------IO----------------
1   单线串口通信    UART0       OUTPUT_CHL_1      INPUT_CHL_0      P10+P11
2	调试串口        UART1       OUTPUT_CHL_3	                   P06
3   HALL传感器                                    INPUT_CHL_2      P14
4   按键功能                                      INPUT_CHL_3      P15
5   NTC检测                                                        P16
6   带电升级        TIMER1_CAP     				  INPUT_CHL_1      P17 (复用,flash工程开启)
7   无线充电        WPR                                            P17+P13 (复用)
8   lighting握手    PPM                                            P24
9   系统定时器      TIMER0                                                
************************************************************************************/

//*********************************************************************************//
//                                 配置开始                                        //
//*********************************************************************************//
#define ENABLE_THIS_MOUDLE					1
#define DISABLE_THIS_MOUDLE					0

#define ENABLE								1
#define DISABLE								0

#define TCFG_MUX_BUFFER_ADDR				0x40//不能动

//默认参数设置
#define TCFG_TIMER_LOOP                     2   //系统定时器的周期值ms

//串口调试功能
#define TCFG_DEBUG_ENABLE                   DEBUG_EN
#define TCFG_DEBUG_BAUD                     115200L
#define TCFG_DEBUG_USED_UARTX               0   //0=使用UART0打印, 1=使用UART1打印
#define DEBUG_GROUP                         1
#define DEBUG_PORT                          6

//UART1串口

//HALL使能开关
#define TCFG_HALL_ENABLE                    ENABLE_THIS_MOUDLE
#define HALL_GROUP                          0
#define HALL_PORT                           1
#define HALL_IO								IO_PORT_0_1

//KEY 使能开关 固定使用P15
#define TCFG_KEY_ENABLE                     ENABLE_THIS_MOUDLE
#define KEY_GROUP                           1
#define KEY_PORT                            5
#define KEY_IO								IO_PORT_1_5

//WPR 无线充电,固定使用P17为通信口,选择P13作为上下线检测引脚
#define TCFG_WPR_ENABLE                     0//ENABLE_THIS_MOUDLE
#define TCFG_WPR_VOLTAGE_NORMAL				5800//VWP调节中心电平
#define WPR_USED_P05        				0
#define WPR_USED_P17        				1
#define WPR_USED_PORT       				WPR_USED_P17
#define WPR_DET_IO_GROUP                    1
#define WPR_DET_IO_PORT                     3
#define WPR_DET_AD_CH                       AD_IO_CH_P13
#define WPR_DET_DIV_COEFF        			4 //分压系数，根据硬件决定

#ifdef TCFG_USE_FLASH_DEBUG
//升级FLASH功能,P17固定为升级口,和注意无线充电复用（flash才会开启）
#define TCFG_UPDATE_ENABLE					ENABLE_THIS_MOUDLE
#else
#define TCFG_UPDATE_ENABLE					DISABLE_THIS_MOUDLE
#endif

//lighting握手,固定使用P24
#define TCFG_HANDSHAKE_ENABLE               0//ENABLE_THIS_MOUDLE

//POWER 电量检测 电池类型设置
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

//UI 配置,三灯方案，数码管方案等
#define TCFG_UI_ENABLE                      0//ENABLE_THIS_MOUDLE
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
//可在seg_led.h配置实际使用的io
//MODE0: P00 P01 P02 P03 P04
//MODE1: P00 P01 P02 P03 P04 P05
#define SEG_LED_MODE0						0 //显示充电舱电量
#define SEG_LED_MODE1						1 //显示充电舱电量+左右耳电量--普通充电舱不支持显示左右耳电量
#define TCFG_SEG_LED_MODE					SEG_LED_MODE1
#endif

//长按复位功能
#define TCFG_LONG_PRESS_RESET_ENABLE		ENABLE_THIS_MOUDLE
#define IO_INPUT_CHANNEL0					0 //不支持-串口通信占用
#define VPWR_IN_DETECT						1
#define LONG_PRESS_PINR_IN_SEL				VPWR_IN_DETECT //长按复位的信号源选择
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
#define TCFG_CHARGE_OPTIMIZE_ENABLE			ENABLE_THIS_MOUDLE //充电电流缓变
#if (VBAT_TYPE_SEL == VBAT_TYPE_4200)
#define CHARGE_CONSTANT_VOLTAGE				CHGV_S_4199_MV
#else
#define CHARGE_CONSTANT_VOLTAGE				CHGV_S_4354_MV
#endif
#define CHARGE_RECHARGE_PERCENT				80 //回充电池电量百分比
#define CHARGE_CURRENT_MA					CHGI_S_300_MA//有线充电电流
#define CHARGE_WIRELESS_CURRENT_MA			CHGI_S_200_MA//无线充电电流
#define CHARGE_WIRELESS_OV_CURRENT_MA		CHGI_S_100_MA//无线过压充电电流
#define CHARGE_FULL_CURRENT_MA				30 //恒流电流/10
#define CHARGE_TC_2_CC_VOLT_MV				3000//涓流转恒流的电池电压	

//芯片内置NTC
#define TCFG_INSIDE_NTC_ENABLE              ENABLE_THIS_MOUDLE //内置NTC
//外挂NTC 检测开关 固定使用P16不能改动
#define TCFG_EXTERN_NTC_ENABLE              0//ENABLE_THIS_MOUDLE //外挂NTC
#define TCFG_EXTERN_NTC_100K				0
#define TCFG_EXTERN_NTC_10K					1
#define TCFG_EXTERN_NTC_TYPE				TCFG_EXTERN_NTC_10K
#define NTC_MODE_JEITA                      0 //JEITA标准
#define NTC_MODE_GB                         1 //国标
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
#define TCFG_BOOST_OPTIMIZE_ENABLE			DISABLE_THIS_MOUDLE //跟随充（耳机单边恒流电流必须小于150mA）
#define TCFG_MODE_FOLLOW_MAX				0 //跟随较大电量的耳机进行放电
#define TCFG_MODE_FOLLOW_MIN				1 //跟随较小电量的耳机进行放电
#define TCFG_EAR_CHARGE_CURRENT_MA			40//耳机充电恒流电流 
#define TCFG_BOOST_OPTIMIZE_MODE			TCFG_MODE_FOLLOW_MIN
#define TCFG_BOOST_VOUT_S					BOOST_VOUT_S_4700_MV
#define TCFG_DISCHARGE_OVERTIME				4//*30mins，耳机充电超时

//VIN2VOUT
#define TCFG_VIN2VOUT_ENABLE				(!TCFG_WPR_ENABLE) //带无线充方案不允许开vin2vout

//CURRENT LIMIT 放电过流检测
#define TCFG_CURRENT_LIMIT_MA				100 //限制单耳最大放电电流

//VOUT_INDET 耳机负载检测
#define VOUT_INDET_CURRENT					VOUT_INDET_CS_S_0P5_UA

//VOUT OV UV
#define VOUT_OV_VOLT						VOUT_OV_VOL_S_5P53V_TO_5P34V
#define VOUT_UV_VOLT						VOUT_UV_VOL_S_3P20V_TO_3P35V
//耳机低电的时候强制充电1分钟
#define CHGBOX_FORCE_CHARGE_TIMES           120//1min   240*512ms

#define TCFG_AUTO_WAKEUP_ENABLE				ENABLE_THIS_MOUDLE
#define WAKEUP_TIME_5_DAYS					0
#define WAKEUP_TIME_10_DAYS					1
#define WAKEUP_TIME_15_DAYS					2
#define TCFG_AUTO_WAKEUP_TIME_DAY           WAKEUP_TIME_10_DAYS //定时唤醒时间/天，可选已计算好的5/10/15天
#define TCFG_AUTO_WAKEUP_TIME_SEC			0//定时唤醒时间/秒，0则默认使用TCFG_AUTO_WAKEUP_TIME_DAY 

//vddio电压挡位配置
#define TCFG_MVDDIO_LEVEL 					MVIO_S_3000_MV
#define TCFG_WVDDIO_LEVEL 					WVIO_S_2700_MV

#define     OPEN_CLOSE_HALL_NOTIGY          1       //开关盖唤醒701
#define     TCFG_UART1_ENABLE                   ENABLE_THIS_MOUDLE
#define     TCFG_UART1_BAUD                     115200L
#define     UART1_RX_IO                         IO_PORT_1_3
#define     UART1_TX_IO                         IO_PORT_1_4
#endif    /*    _APP_CONFIG_H    */



