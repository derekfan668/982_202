#ifndef _SEG_LED_H_
#define _SEG_LED_H_

#include "typedef.h"
#include "app_config.h"

#if (TCFG_UI_MODE == UI_SEG_LED)
#if (TCFG_SEG_LED_MODE == SEG_LED_MODE0)

#define LED_GROUP       0
#define LED0_PORT       0
#define LED1_PORT       1
#define LED2_PORT       2
#define LED3_PORT       3
#define LED4_PORT       4

#define LED_ALL_PORT    (BIT(LED0_PORT) | BIT(LED1_PORT) | BIT(LED2_PORT) | BIT(LED3_PORT) | BIT(LED4_PORT))

#define SEGA	BIT(0)
#define SEGB	BIT(1)
#define SEGC	BIT(2)
#define SEGD	BIT(3)
#define SEGE	BIT(4)
#define SEGF	BIT(5)
#define SEGG	BIT(6)

#define LED_STATUS_OFF      0
#define LED_STATUS_ON       1
#define LED_STATUS_FLASH    2

#define LED_X_0     0
#define LED_X_1     1
#define LED_X_2     2
#define LED_X_3     3
#define LED_X_MAX   4

#define LED_Y_0     0
#define LED_Y_1     1
#define LED_Y_2     2
#define LED_Y_3     3
#define LED_Y_4     4
#define LED_Y_MAX   5

#define DIG_1B     ((LED_Y_2<<4)|LED_X_3)
#define DIG_1C     ((LED_Y_1<<4)|LED_X_3)

#define DIG_2A     ((LED_Y_1<<4)|LED_X_2)
#define DIG_2B     ((LED_Y_2<<4)|LED_X_1)
#define DIG_2C     ((LED_Y_3<<4)|LED_X_2)
#define DIG_2D     ((LED_Y_3<<4)|LED_X_1)
#define DIG_2E     ((LED_Y_4<<4)|LED_X_1)
#define DIG_2F     ((LED_Y_4<<4)|LED_X_2)
#define DIG_2G     ((LED_Y_4<<4)|LED_X_3)

#define DIG_3A     ((LED_Y_0<<4)|LED_X_1)
#define DIG_3B     ((LED_Y_1<<4)|LED_X_0)
#define DIG_3C     ((LED_Y_0<<4)|LED_X_2)
#define DIG_3D     ((LED_Y_2<<4)|LED_X_0)
#define DIG_3E     ((LED_Y_0<<4)|LED_X_3)
#define DIG_3F     ((LED_Y_3<<4)|LED_X_0)
#define DIG_3G     ((LED_Y_4<<4)|LED_X_0)   

#define DIG_ALL_OFF()       IO_DIR_OR(LED_GROUP, LED_ALL_PORT)
#define DIG_ALL_INIT()      DIG_ALL_OFF(); IO_HD_OR(LED_GROUP, LED_ALL_PORT)
#define DIG_LED0_OUT(x)     gpio_write(LED_GROUP, LED0_PORT, x);gpio_set_dir(LED_GROUP, LED0_PORT, 0)
#define DIG_LED1_OUT(x)     gpio_write(LED_GROUP, LED1_PORT, x);gpio_set_dir(LED_GROUP, LED1_PORT, 0)
#define DIG_LED2_OUT(x)     gpio_write(LED_GROUP, LED2_PORT, x);gpio_set_dir(LED_GROUP, LED2_PORT, 0)
#define DIG_LED3_OUT(x)     gpio_write(LED_GROUP, LED3_PORT, x);gpio_set_dir(LED_GROUP, LED3_PORT, 0)
#define DIG_LED4_OUT(x)     gpio_write(LED_GROUP, LED4_PORT, x);gpio_set_dir(LED_GROUP, LED4_PORT, 0)
#define DIG_LED5_OUT(x)     

#elif (TCFG_SEG_LED_MODE == SEG_LED_MODE1)

#define LED_GROUP       0
#define LED0_PORT       0
#define LED1_PORT       1
#define LED2_PORT       2
#define LED3_PORT       3
#define LED4_PORT       4
#define LED5_PORT       5

#define LED_ALL_PORT    (BIT(LED0_PORT) | BIT(LED1_PORT) | BIT(LED2_PORT) | BIT(LED3_PORT) | BIT(LED4_PORT) | BIT(LED5_PORT))

#define SEGA	BIT(0)
#define SEGB	BIT(1)
#define SEGC	BIT(2)
#define SEGD	BIT(3)
#define SEGE	BIT(4)
#define SEGF	BIT(5)
#define SEGG	BIT(6)

#define LED_STATUS_OFF      0
#define LED_STATUS_ON       1
#define LED_STATUS_FLASH    2

#define LED_X_0     0
#define LED_X_1     1
#define LED_X_2     2
#define LED_X_3     3
#define LED_X_4     4
#define LED_X_5     5
#define LED_X_MAX   6

#define LED_Y_0     0
#define LED_Y_1     1
#define LED_Y_2     2
#define LED_Y_3     3
#define LED_Y_4     4
#define LED_Y_5     5
#define LED_Y_MAX   6

//左耳电量格
#define DIG_K1     ((LED_Y_0<<4)|LED_X_1)
#define DIG_K2     ((LED_Y_0<<4)|LED_X_2)
#define DIG_K3     ((LED_Y_0<<4)|LED_X_3)
#define DIG_K4     ((LED_Y_0<<4)|LED_X_4)
//"L"显示
#define DIG_K5     ((LED_Y_0<<4)|LED_X_5)

//188数显
#define DIG_1B     ((LED_Y_1<<4)|LED_X_0)
#define DIG_1C     ((LED_Y_1<<4)|LED_X_2)

#define DIG_2A     ((LED_Y_1<<4)|LED_X_3)
#define DIG_2B     ((LED_Y_1<<4)|LED_X_4)
#define DIG_2C     ((LED_Y_1<<4)|LED_X_5)
#define DIG_2D     ((LED_Y_2<<4)|LED_X_0)
#define DIG_2E     ((LED_Y_2<<4)|LED_X_1)
#define DIG_2F     ((LED_Y_2<<4)|LED_X_3)
#define DIG_2G     ((LED_Y_2<<4)|LED_X_4)

#define DIG_3A     ((LED_Y_2<<4)|LED_X_5)
#define DIG_3B     ((LED_Y_3<<4)|LED_X_0)
#define DIG_3C     ((LED_Y_3<<4)|LED_X_1)
#define DIG_3D     ((LED_Y_3<<4)|LED_X_2)
#define DIG_3E     ((LED_Y_3<<4)|LED_X_4)
#define DIG_3F     ((LED_Y_3<<4)|LED_X_5)
#define DIG_3G     ((LED_Y_4<<4)|LED_X_0)

//"%"显示
#define DIG_K9     ((LED_Y_4<<4)|LED_X_2)

//右耳电量格
#define DIG_K10    ((LED_Y_4<<4)|LED_X_3)
#define DIG_K11    ((LED_Y_4<<4)|LED_X_5)
#define DIG_K12    ((LED_Y_5<<4)|LED_X_0)
#define DIG_K13    ((LED_Y_5<<4)|LED_X_1)
//"R"显示
#define DIG_K8     ((LED_Y_4<<4)|LED_X_1)

#define DIG_ALL_OFF()       IO_DIR_OR(LED_GROUP, LED_ALL_PORT)
#define DIG_ALL_INIT()      DIG_ALL_OFF(); IO_HD_OR(LED_GROUP, LED_ALL_PORT)
#define DIG_LED0_OUT(x)     gpio_write(LED_GROUP, LED0_PORT, x);gpio_set_dir(LED_GROUP, LED0_PORT, 0)
#define DIG_LED1_OUT(x)     gpio_write(LED_GROUP, LED1_PORT, x);gpio_set_dir(LED_GROUP, LED1_PORT, 0)
#define DIG_LED2_OUT(x)     gpio_write(LED_GROUP, LED2_PORT, x);gpio_set_dir(LED_GROUP, LED2_PORT, 0)
#define DIG_LED3_OUT(x)     gpio_write(LED_GROUP, LED3_PORT, x);gpio_set_dir(LED_GROUP, LED3_PORT, 0)
#define DIG_LED4_OUT(x)     gpio_write(LED_GROUP, LED4_PORT, x);gpio_set_dir(LED_GROUP, LED4_PORT, 0)
#define DIG_LED5_OUT(x)     gpio_write(LED_GROUP, LED5_PORT, x);gpio_set_dir(LED_GROUP, LED5_PORT, 0)

#endif
#endif

#endif  /*  _SEG_LED_H_ */

