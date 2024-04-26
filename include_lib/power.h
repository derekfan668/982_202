#ifndef _POWER_H_
#define _POWER_H_

#include "typedef.h"

//RST_SRC
#define IS_SOFT_RESET()     RST_SRC & BIT(5)
#define IS_WDT_RESET()      RST_SRC & BIT(4)
#define IS_PIN_RESET()      RST_SRC & BIT(3)
#define IS_MCLR_RESET()     RST_SRC & BIT(2)
#define IS_LVD_RESET()      RST_SRC & BIT(1)
#define IS_POWER_ON_RESET() RST_SRC & BIT(0)


//PWR_CON
#define GET_POR_FLAG()      PWR_CON & BIT(7)   
#define CLR_POR_FLAG()      PWR_CON &= ~BIT(7) //复位源为POR/MCLR/PINR时,该位置1，复位后需将该位清零

#define CPU_RESET()         PWR_CON |= BIT(4)

#define SSMODE_DELAY_8MS()  PWR_CON &= ~BIT(3) //DEFAULT
#define SSMODE_NO_DELAY()   PWR_CON |=  BIT(3)

#define SYS_SLEEP()         PWR_CON |=  BIT(2)
#define SYS_STANDBY()       PWR_CON |=  BIT(1)
#define SYS_IDLE()          PWR_CON |=  BIT(0)

//RST_CON
#define MCLR_EN()           RST_CON |=  BIT(4) //DEFAULT
#define MCLR_DIS()          RST_CON &= ~BIT(4)

//PINR conf
#define PINR_IN_SEL_SET(x)  CLK_CON0 &= ~BIT(3); CLK_CON0 |= (x<<3)	
#define PINR_ENABLE() 		RST_CON |=  BIT(0)
#define PINR_DISABLE() 		RST_CON &= ~BIT(0)
#define PINR_LVL_SET_L() 	RST_CON &= ~BIT(1)
#define PINR_LVL_SET_H() 	RST_CON |=  BIT(1)
#define PINR_TSEL(x) 		RST_CON &= ~0x0C; RST_CON |= (x<<2)

//LVD
#define LVD_PND_GET() 		LVD_CON0 & BIT(7)
#define LVD_PND_CLR() 		LVD_CON0 |= (1<<6)
#define LVD_MODE_SET(x) 	LVD_CON0 &= ~BIT(5); LVD_CON0 |= (x<<5)
#define LVD_EXPIN_EN(x) 	LVD_CON0 &= ~BIT(4); LVD_CON0 |= (x<<4)
#define LVD_EN(x) 			LVD_CON0 &= ~BIT(3); LVD_CON0 |= (x<<3)
#define LVD_OE_EN(x) 		LVD_CON0 &= ~BIT(2); LVD_CON0 |= (x<<2)
#define LVD_SELECT(x) 		LVD_CON0 &= ~0x03; LVD_CON0 |= (x<<0)
#define LVD_WKUP_EN(x) 		LVD_CON1 &= ~BIT(2); LVD_CON1 |= (x<<2)
#define LVD_FLT_EN(x) 		LVD_CON1 &= ~BIT(0); LVD_CON1 |= (x<<0)		

//MODE_CON
#define TRIGGER()           MODE_CON |=  BIT(7) //FPGA

#define TEST_OUTPUT_EN()    MODE_CON |=  BIT(3) //ISP_CLK OUTPUT EN
#define TEST_OUTPUT_DIS()   MODE_CON &= ~BIT(3)

#define ISD_MODE_EN()       MODE_CON |=  BIT(2)
#define ISD_MODE_DIS()      MODE_CON &= ~BIT(2)

#define ISDI_MODE_EN()      MODE_CON |=  BIT(1)
#define ISDI_MODE_DIS()     MODE_CON &= ~BIT(1)

#define ISP_MODE_EN()       MODE_CON |=  BIT(0)
#define ISP_MODE_DIS()      MODE_CON &= ~BIT(0)

#define cpu_reset()			CPU_RESET()
#define sys_enter_sleep()	power_set_soft_poweroff()

void power_set_soft_poweroff(void);
void reset_source_dump(void);
void long_press_reset_init(void);
void lvd_init(void);

#endif

