#ifndef __CHARGEBOX_HW_H__
#define __CHARGEBOX_HW_H__

#include "typedef.h"

#define TCFG_CHARGEBOX_HW_BAUD	9600L

//固定, P10-左 P11-右，不要改
#define EAR_GROUP_L                 1
#define EAR_PORT_L                  0
#define EAR_IO_L                    IO_PORT_1_0

#define EAR_GROUP_R                 1
#define EAR_PORT_R                  1
#define EAR_IO_R                    IO_PORT_1_1

enum {
	EAR_L,
	EAR_R,
	EAR_MAX,
};

enum {
	MODE_RECV,
	MODE_SEND,
};

extern void chargebox_send_sync(u8 ch);
extern void chargebox_open(u8 ch, u8 mode);
extern void chargebox_close(u8 ch);
extern void chargebox_write(u8 idata *buf, u8 len); 

#endif

