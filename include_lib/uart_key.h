#ifndef _UART_KEY_H_
#define _UART_KEY_H_

#include "typedef.h"

enum {
	EDGE_RISE,
	EDGE_FALL,
};

extern void uart_key_init(void);
extern void uart_key_uninit(void);

#endif  /*  _UART_KEY_H_ */


