#ifndef _FLASH_H_
#define _FLASH_H_
#include "stm32f10x.h"

#define FLASH_Page_Size    (0x00000400) //FLASHҳ�Ĵ�С
#define FLASH_Start_Addr   (0x0800FC00) //Ҫ��д����ʼ��ַ
#define FLASH_End_Addr     (0x08010000) //Ҫ��д�Ľ�����ַ
#define DATA_32            (0x12345678) //Ҫ��д������

extern void FLASH_read(uint32_t *data,uint8_t len);
extern void FLASH_write(int16_t *data,uint8_t len);	

#endif
