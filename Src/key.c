/***********************************************
 * 文件名称：key.c
 * 作    者：mcli
 * 日    期：2019/2/28
 * 版    本：v1.0
 * 描    述：本文件为按键扫描和设置相关源码
 ***********************************************/

#include "key.h"
#include "config.h"	// gui
#include "fft.h"
#include "adc.h"
#include "display.h"
#include "rtcpro.h"

// 光标图像 阴码 顺向取模 5x7
static uint8_t pox[]={0x00,0x40,0x60,0x70,0x60,0x40,0x00};
struct SetInfo Set_info;	// 设置相关的结构体变量
extern uint8_t display_num;	// 定义在main.c中

uint8_t display_mode_temp = 0;			// 设置里面的暂时值 不然那在设置中改变了设置模式 还没等退出 就显示另一个模式了

/* 函数名称：Key_Init
 * 功能描述：初始化设置参数
 * 参数：无
 * 返回值：无
 */
void Key_Init(void)
{
	// 初始化 设置参数
	Set_info.func_num = 0;
	Set_info.page = 0;
}


#define FFT_FUNC_NUM_MAX		4	// 功能项最大值
/* 函数名称：Show_Set_FFT
 * 功能描述：显示设置界面
 * 参数：无
 * 返回值：无
 */
void Show_Set_FFT(void)
{
	GUI_ClearSCR();
	GUI_LoadPic(0, Set_info.func_num*8, pox, 5, 7);
	GUI_PutString(8, 0, "Fal_speed");	// 点下落速度
	GUI_PutString(8, 8, "A Or M");
	GUI_PutString(8, 16, "Show_Mode");			// 退出
	GUI_PutString(8, 24, "Exit");			// 退出
	GUI_Exec();
}

void Func_FalSpeed(void)
{
	GUI_ClearSCR();
	GUI_PutString(60, 0, "Fal_speed Setting");	// 点下落速度
	GUI_PutString(0, 16, "Val:");								// 数值
	GUI_ShowNum(32, 16, fal_speed, 3);
	GUI_Exec();
}
void Func_ModeSet(void)
{
	GUI_ClearSCR();
	GUI_PutString(52, 0, "Mode Setting");		// 点下落速度
	GUI_PutString(0, 16, "Val:");						// 数值
	if(display_auto != 0)
		GUI_PutString(32, 16, "AUTO");			
	else
		GUI_PutString(32, 16, "M");		
	GUI_Exec();
}

void Func_ShowMode(void)
{
	GUI_ClearSCR();
	GUI_PutString(52, 0, "Show Mode");			// 显示模式
	GUI_PutString(0, 16, "Val:");						// 数值
	if(display_mode_temp != 0)
		GUI_PutString(32, 16, "Time");			
	else
		GUI_PutString(32, 16, "FFT");		
	GUI_Exec();
}

void Func_Exit(void)
{
	// 清除设置页里面的标识
	Set_info.page = 0;
	Set_info.func_num = 0;
	
	if(display_mode_temp != 0)
	{
		Current_display_mode = DISPLAY_MODE_TIME;	// 显示时间
		FFT_Start();	// 重新开启定时器启动ADC转换	显示时间的时候也要做FFT 用于信号检测
	}
	else
	{
		Current_display_mode = DISPLAY_MODE_FFT;	// 显示p频谱
		FFT_Start();	// 重新开启定时器启动ADC转换
	}
}

/* 函数名称：	Key_Scan
 * 功能描述：	按键扫描函数
 * 参数：			无
 * 返回值：		无按键返回0，有按键返回按键值
 * 备注：板子上的丝印
 */
#define KEY1_PRESS	1
#define KEY2_PRESS	2
#define KEY3_PRESS	3

uint8_t Key_Scan(void)
{
	uint8_t key_val = 0;
	static uint8_t press_flag = 0;			//按下标志
	
	if((GPIOA->IDR&0xe0) != 0xe0) 		// 这里在硬件上加了消抖 就不用软件延时消抖了
	{
			key_val = (GPIOA->IDR&0xe0);	// 取出按键值
			if(press_flag == 0)						// 第一次扫描
			{
				press_flag = 1;								// 标记已经被按下了
				switch(key_val)
				{
					case 0xc0:return KEY3_PRESS;// K3
					case 0xa0:return KEY2_PRESS;// K2
					case 0x60:return KEY1_PRESS;// K1
					default:
						break;
				}
			}
	}
	else	// 没有按键按下了
			press_flag = 0;	// 松手了就清除标志
	return 0;
}

/* 函数名称：Key_Pro
 * 功能描述：按键处理函数
 * 参数：无
 * 返回值：无
 */



/* 函数名称：void Key_fft(uint8_t key_val)
 * 功能描述：按键处理函数
 * 参数：无
 * 返回值：无
 */
void Key_fft(uint8_t key_val)
{
	switch(key_val)
	{
		case KEY1_PRESS:	// 功能键
				if(Set_info.page == DISPLAY)
				{
					FFT_Stop();								// 停止FFT的采集
					Set_info.page = SETTING;	// 进入设置页面
					Show_Set_FFT();								// 显示设置界面
				}
				else if(Set_info.page == SETTING)
				{
					Set_info.page = SOMEONE;			// 表示进入某项设置项的页面了
					switch(Set_info.func_num)
					{
						case 0:
								Func_FalSpeed();	// 显示下落速度设置值
							break;
						case 1:
								Func_ModeSet();		// 显示模式设置 手动/自动
							break;
						case 2:
								Func_ShowMode();	// 显示时间还是频谱设置
							break;
						case 3:
								Func_Exit();	// 退出
							break;
						default:
							break;
					}
				}
				else if(Set_info.page == SOMEONE)	// 表示上一次按下时候已经进入了功能项 这次是退出这个功能项
				{
					Set_info.page = SETTING;
					Show_Set_FFT();								// 显示设置界面
				}
			break;
		case KEY2_PRESS:	// +
				if(Set_info.page == DISPLAY)	// 正常显示页面
				{
					display_num ++;
					if(display_num > DISPLAY_FUNC_NUM) display_num = 0;
				}
				else if(Set_info.page == SETTING)	// 设置页面
				{
					Set_info.func_num++;
					if(Set_info.func_num > FFT_FUNC_NUM_MAX-1)	Set_info.func_num = 0;
					Show_Set_FFT();								// 显示设置界面
				}
				else if(Set_info.page == SOMEONE)	// 具体某项
				{
					switch(Set_info.func_num)
					{
						case 0:	// Func_FalSpeed
							fal_speed++;
							if(fal_speed>10)	fal_speed = 0;
							Func_FalSpeed();	// 显示下落速度设置值
							break;
						case 1:	// Func_ModeSet
							display_auto = !display_auto;
							Func_ModeSet();	// 刷新显示
							break;
						case 2:	// Func_ShowMode
							display_mode_temp = !display_mode_temp;
							Func_ShowMode();	// 刷新显示
							break;
						default:
							break;
					}
				}
			break;
		case KEY3_PRESS:	// -
				if(Set_info.page == DISPLAY)
				{
					if(display_num == 0)	display_num = DISPLAY_FUNC_NUM+1;
					display_num--;
				}
				else if(Set_info.page == SETTING)
				{
					if(Set_info.func_num == 0)	Set_info.func_num = FFT_FUNC_NUM_MAX;
					Set_info.func_num--;
					Show_Set_FFT();								// 显示设置界面
				}
				else if(Set_info.page == SOMEONE)	// 具体某项
				{
					switch(Set_info.func_num)
					{
						case 0:	// Func_FalSpeed
							if(fal_speed==0)	fal_speed = 11;
							fal_speed--;
							Func_FalSpeed();	// 显示下落速度设置值
							break;
						case 1:	// Func_ModeSet
							display_auto = !display_auto;	
							Func_ModeSet();	// 刷新显示
							break;
						case 2:	// Func_ShowMode
							display_mode_temp = !display_mode_temp;
							Func_ShowMode();	// 刷新显示
							break;
						default:
							break;
					}
				}
			break;
		default:
			break;
	}
}



#define TIME_FUNC_NUM_MAX		8	// 功能项最大值
/* 函数名称：Show_Set_FFT
 * 功能描述：显示设置界面
 * 参数：无
 * 返回值：无
 */
void Show_Set_Time(void)
{
	GUI_ClearSCR();
	GUI_LoadPic(0, Set_info.func_num*8, pox, 5, 7);
	GUI_PutString(8, 0, "Year");			// 年
	GUI_PutString(8, 8, "Month");			// 月
	GUI_PutString(8, 16, "Date");			// 日
	GUI_PutString(8, 24, "Hours");		// 小时
	GUI_PutString(8, 32, "Minutes");	// 分钟
	GUI_PutString(8, 40, "Seconds");	// 秒
	GUI_PutString(8, 48, "Show_Mode");		// 显示模式设置
	GUI_PutString(8, 56, "Exit");			// 退出
	GUI_Exec();
}

void Func_Year(void)
{
	GUI_ClearSCR();
	GUI_PutString(60, 0, "Year Setting");	// 点下落速度
	GUI_PutString(0, 16, "Val:");								// 数值
	GUI_ShowNum(32, 16, date.Year, 4);
	GUI_Exec();
}
void Func_Month(void)
{
	GUI_ClearSCR();
	GUI_PutString(60, 0, "Month Setting");	// 点下落速度
	GUI_PutString(0, 16, "Val:");								// 数值
	GUI_ShowNum(32, 16, date.Month, 2);
	GUI_Exec();
}
void Func_Date(void)
{
	GUI_ClearSCR();
	GUI_PutString(60, 0, "Date Setting");	// 点下落速度
	GUI_PutString(0, 16, "Val:");								// 数值
	GUI_ShowNum(32, 16, date.Date, 2);
	GUI_Exec();
}
void Func_Hours(void)
{
	GUI_ClearSCR();
	GUI_PutString(60, 0, "Hours Setting");	// 点下落速度
	GUI_PutString(0, 16, "Val:");								// 数值
	GUI_ShowNum(32, 16, time.Hours, 2);
	GUI_Exec();
}
void Func_Minutes(void)
{
	GUI_ClearSCR();
	GUI_PutString(60, 0, "Minutes Setting");	// 点下落速度
	GUI_PutString(0, 16, "Val:");								// 数值
	GUI_ShowNum(32, 16, time.Minutes, 2);
	GUI_Exec();
}
void Func_Seconds(void)
{
	GUI_ClearSCR();
	GUI_PutString(60, 0, "Seconds Setting");	// 点下落速度
	GUI_PutString(0, 16, "Val:");								// 数值
	GUI_ShowNum(32, 16, time.Seconds, 2);
	GUI_Exec();
}
void Func_ShowMode_Time(void)
{
	GUI_ClearSCR();
	GUI_PutString(60, 0, "ShowMode Setting");	// 点下落速度
	GUI_PutString(0, 16, "Val:");								// 数值
	if(display_mode_temp != 0)
		GUI_PutString(32, 16, "Time");			
	else
		GUI_PutString(32, 16, "FFT");		
	GUI_Exec();
}
void Func_Exit_Time(void)
{
	// 清除设置页里面的标识
	Set_info.page = 0;
	Set_info.func_num = 0;
	
	if(display_mode_temp != 0)
	{
		Current_display_mode = DISPLAY_MODE_TIME;	// 显示时间
		// 把修改了的时间设置进去
		RTC_Set(date.Year, date.Month, date.Date,
		time.Hours, time.Minutes, time.Seconds);	
		
	}
	else
	{
		Current_display_mode = DISPLAY_MODE_FFT;	// 显示p频谱
		FFT_Start();	// 重新开启定时器启动ADC转换
	}
}
/* 函数名称：void Key_time(uint8_t key_val)
 * 功能描述：在显示时间时候 按下按键的处理函数
 * 参数：key_val   按键值
 * 返回值：无
 */
void Key_time(uint8_t key_val)
{
	switch(key_val)
	{
		case KEY1_PRESS:	// 功能键
				if(Set_info.page == DISPLAY)
				{
					Set_info.page = SETTING;				// 进入设置页面
					RTC_Get();											// 读取时间
					Show_Set_Time();								// 显示设置界面
				}
				else if(Set_info.page == SETTING)
				{
					Set_info.page = SOMEONE;			// 表示进入某项设置项的页面了
					switch(Set_info.func_num)
					{
						case 0:	
								Func_Year();		// 设置年
							break;
						case 1:
								Func_Month();		// 设置月
							break;
						case 2:
								Func_Date();		// 设置日期
							break;
						case 3:
								Func_Hours();		// 设置小时
							break;
						case 4:
								Func_Minutes();		// 设置分钟
							break;
						case 5:
								Func_Seconds();		// 设置秒钟
							break;
						case 6:
								Func_ShowMode_Time();		// 设置模式
							break;
						case 7:
								Func_Exit_Time();		// 退出
							break;
						default:
							break;
					}
				}
				else if(Set_info.page == SOMEONE)	// 表示上一次按下时候已经进入了功能项 这次是退出这个功能项
				{
					Set_info.page = SETTING;
					Show_Set_Time();								// 显示设置界面
				}
			break;
		case KEY2_PRESS:	// +
				if(Set_info.page == DISPLAY)	// 正常显示页面
				{
					
				}
				else if(Set_info.page == SETTING)	// 设置页面
				{
					Set_info.func_num++;
					if(Set_info.func_num > TIME_FUNC_NUM_MAX-1)	Set_info.func_num = 0;
					Show_Set_Time();								// 显示设置界面
				}
				else if(Set_info.page == SOMEONE)	// 具体某项
				{
					switch(Set_info.func_num)
					{
						case 0:	// Func_Year
							date.Year++;
							if(date.Year>2099)	date.Year = 1970;
							Func_Year();	// 显示设置的值
							break;
						case 1:	// Func_Month
							date.Month++;
							if(date.Month>12)	date.Month = 0;
							Func_Month();	// 显示设置的值
							break;
						case 2:	// 
							date.Date++;
							if(date.Date>31)	date.Date = 0;
							Func_Date();	// 显示设置的值
							break;
						case 3:	// 
							time.Hours++;
							if(time.Hours>24)	time.Hours = 0;
							Func_Hours();	// 显示设置的值
							break;
						case 4:	// 
							time.Minutes++;
							if(time.Minutes>59)	time.Minutes = 0;
							Func_Minutes();	// 显示设置的值
							break;
						case 5:	// 
							time.Seconds++;
							if(time.Seconds>59)	time.Seconds = 0;
							Func_Seconds();	// 显示设置的值
							break;
						case 6:	// 
							display_mode_temp = !display_mode_temp;
							Func_ShowMode_Time();	// 显示设置的值
							break;
						default:
							break;
					}
				}
			break;
		case KEY3_PRESS:	// -
				if(Set_info.page == DISPLAY)
				{

				}
				else if(Set_info.page == SETTING)
				{
					if(Set_info.func_num == 0)	Set_info.func_num = TIME_FUNC_NUM_MAX;
					Set_info.func_num--;
					Show_Set_Time();								// 显示设置界面
				}
				else if(Set_info.page == SOMEONE)	// 具体某项
				{
					switch(Set_info.func_num)
					{
						case 0:	// Func_Year
							if(date.Year<1970)	date.Year = 2100;
							date.Year--;
							Func_Year();	// 显示设置的值
							break;
						case 1:	// Func_Month
							if(date.Month == 0)	date.Month = 13;
							date.Month--;
							Func_Month();	// 显示设置的值
							break;
						case 2:	// 
							if(date.Date == 0)	date.Date = 32;
							date.Date--;
							Func_Date();	// 显示设置的值
							break;
						case 3:	// 
							if(time.Hours == 0)	time.Hours = 25;
							time.Hours--;
							Func_Hours();	// 显示设置的值
							break;
						case 4:	// 
							if(time.Minutes == 0)	time.Minutes = 60;
							time.Minutes--;
							Func_Minutes();	// 显示设置的值
							break;
						case 5:	// 
							if(time.Seconds == 0)	time.Seconds = 60;
							time.Seconds--;
							Func_Seconds();	// 显示设置的值
							break;
						case 6:	// 
							display_mode_temp = !display_mode_temp;
							Func_ShowMode_Time();	// 显示设置的值
							break;
						default:
							break;
					}
				}
			break;
		default:
			break;
	}
}



/* 函数名称：Key_Pro
 * 功能描述：按键处理函数
 * 参数：无
 * 返回值：无
 */
void Key_Pro(void)
{
	uint8_t key_val = 0;
	
	// 按键扫描
	key_val = Key_Scan();
	
	// 有按键按下
	if(0 != key_val)
	{
		// 2种情况 
		// 1.在显示频谱时候按下
		if(Current_display_mode == DISPLAY_MODE_FFT)
		{
			Key_fft(key_val);	// 调转至FFT相关设置
		}
		// 2.显示时间的时候被按下
		else if(Current_display_mode == DISPLAY_MODE_TIME)
		{
			Key_time(key_val);	// 调转至Time相关设置
		}
	}
}

