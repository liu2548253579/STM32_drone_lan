#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

#define SEND_PORT USART3

extern void USART_SendByte(const int8_t *Data,uint8_t len);
extern void USART_CONFIG(int btl);

extern int fputc(int ch, FILE *f);



extern void USART1_setBaudRate(uint32_t baudRate);

#endif /* __USART1_H */

