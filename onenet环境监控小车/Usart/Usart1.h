#ifndef _USART_H_
#define _USART_H_
#include "sys.h"
// #include "pbdata.h"


typedef struct _doorState
{
    char status;
}Door;

extern Door door;

void USART_Config(u32 baud);


#endif
