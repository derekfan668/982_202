#ifndef _DRIVER_TEST_H_
#define _DRIVER_TEST_H_

#include "typedef.h"

enum {
    DRIVER_TEST_UART0_A,
    DRIVER_TEST_UART0_B,

    DRIVER_TEST_UART1_A,
    DRIVER_TEST_UART1_B,

    DRIVER_TEST_TMR0_PWM,
    DRIVER_TEST_TMR1_PWM,

    DRIVER_TEST_WPR_TX0,
    DRIVER_TEST_WPR_TX1,

    DRIVER_TEST_LED_A,
    DRIVER_TEST_LED_B,

    DRIVER_TEST_PPM,

    DRIVER_TEST_SLEEP,
};

void driver_test_func(u8 driver_number);

#endif  /*  _DRIVER_TEST_H_ */


