#ifndef __KEY_H__
#define __KEY_H__
#include "stm32f1xx_hal.h"

// 设置相关参数
struct SetInfo
{
	uint8_t page;		// 当前所处的页面
	uint8_t func_num;	// 当前设置项
};

// 所在页
#define DISPLAY	0
#define SETTING	1
#define SOMEONE	2

extern struct SetInfo Set_info;	// 声明一下 方便外部使用

void Key_Init(void);
uint8_t Key_Scan(void);
void Key_Pro(void);

#endif


