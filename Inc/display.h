#ifndef __DISPLAY_H__
#define __DISPLAY_H__
#include "stm32f1xx_hal.h"
#include "config.h"

// 信号检测的三种返回值
#define SINGLE_OK					0
#define SINGLE_NOT				1
#define SINGLE_CHECK			2
uint8_t Single_Check(void);
	
extern uint8_t display_num;
extern uint8_t fal_speed;
extern uint8_t display_auto;
extern uint8_t Current_display_mode;
// 显示种类
#define DISPLAY_MODE_FFT		0			// 显示模式为频谱
#define DISPLAY_MODE_TIME		1			// 显示模式为时间


// LED控制信号
#define LED_ON		0
#define LED_OFF 	1

void LED_Ctr(uint8_t sta);
void Display_Init(void);
void display_pro(void);

#define DISPLAY_FUNC_NUM	3	// 显示方式数量
void display1(void);
void display2(void);
void display3(void);
#endif




