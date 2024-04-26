#ifndef _WDT_H_
#define _WDT_H_

#include "typedef.h"

enum {
    WDT_1MS,
    WDT_2MS,
    WDT_4MS,
    WDT_8MS,
    WDT_16MS,
    WDT_32MS,
    WDT_64MS,
    WDT_128MS,
    WDT_256MS,
    WDT_512MS,
    WDT_1S,
    WDT_2S,
    WDT_4S,
    WDT_8S,
    WDT_16S,
    WDT_32S,
};

#define wdt_clear()         WDT_CON |= BIT(6)
#define wdt_init(x)         WDT_CON = (BIT(6) | BIT(4) | x)
#define wdt_close()         WDT_CON = BIT(6)

#endif  /*  _WDT_H_ */

