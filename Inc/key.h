#ifndef __KEY_H__
#define __KEY_H__
#include "stm32f1xx_hal.h"

// ������ز���
struct SetInfo
{
	uint8_t page;		// ��ǰ������ҳ��
	uint8_t func_num;	// ��ǰ������
};

// ����ҳ
#define DISPLAY	0
#define SETTING	1
#define SOMEONE	2

extern struct SetInfo Set_info;	// ����һ�� �����ⲿʹ��

void Key_Init(void);
uint8_t Key_Scan(void);
void Key_Pro(void);

#endif


