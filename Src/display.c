/***********************************************
 * 文件名称：display.c
 * 作    者：mcli
 * 日    期：2019/2/28
 * 版    本：v1.0
 * 描    述：本文件为显示相关源码封装
 ***********************************************/
#include "display.h"
#include "fft.h"
#include "tu.h"

uint8_t fal_pot[NPT];							// 记录下落点的坐标
uint8_t prt = 4;									// 量化显示的比例
uint8_t display_num = 1;					// 显示样式的数量
uint8_t fal_speed = 7;						// 下落速度
uint8_t display_auto = 1;					// 自动显示标志位

uint8_t Current_display_mode = DISPLAY_MODE_TIME;	// 当前显示模式

/* 函数名称：Display_Init
 * 功能描述：显示初始化
 * 参数：无
 * 返回值：无
 */
void Display_Init(void)
{
	uint16_t i = 0;
	
	LED_Ctr(LED_OFF);	// 关闭USER_LED
	
	// 显示初始化 使用GUI
	GUI_Initialize();
	GUI_SetColor(0x0f, 0);
	GUI_LoadPic(64,0,(uint8_t *)&gImage_bg,128,64);
	GUI_Exec();
	
	//初始化下落点 把下落的点 初始化为最底部显示
	for(i=0; i<NPT; i++)
		fal_pot[i] = 63;
	HAL_Delay(1000);
	
}


/* 函数名称：void LED_Ctr(uint8_t sta)
 * 功能描述：user_led控制函数
 * 参    数：无
 * 返 回 值：ON：开				OFF：关
 */
void LED_Ctr(uint8_t sta)
{
	if(sta == LED_ON)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	}
}
	

/* 函数名称：void Single_Check(void)
 * 功能描述：输入信号检测 看是否有信号输入
 * 参    数：无
 * 返 回 值：0：有信号输入	1：无信号输入
 * 描    述：主要通过判断FFT之后各个频率对应的幅值，没有明显的变换 持续几秒之后
 * 					就切换为时间显示， 有明显变化就显示频谱。
 */
uint8_t Single_Check(void)
{
	static uint8_t num_yes = 0;
	static uint8_t num_no = 0;
	static uint8_t cheeck_num = 0;	// 检测次数
	uint8_t i;
	
	for(i=1;i<0xff;i++)	// 从1开始 去电0这个直流分量
	{
		if(lBufMagArray[i] > 50)
		{
			// 检测到了信号
			break;
		}
	}
	if(i<0xff)
		num_yes ++;	// 检测到了信号
	else// 没检测到信号
		num_no ++;

	cheeck_num ++;
	if(cheeck_num==10)	// 检测了10次
	{
		cheeck_num = 0;
		if(num_yes>num_no)	
		{
			num_yes = 0;
			num_no = 0;
			return SINGLE_OK;	// 有信号
		}
		else	
		{
			num_yes = 0;
			num_no = 0;
			return SINGLE_NOT;	// 无信号
		}
	}
	else
		return SINGLE_CHECK;	// 正在检测
}

/* 函数名称：display_pro
 * 功能描述：显示处理函数
 * 参数：无
 * 返回值：无
 */
void display_pro(void)
{
	static uint16_t num = 0;
	
	// 处理自动显示
	if(display_auto)
	{
		num++;
		if(num>=200)
		{
			num = 0;
			display_num++;
			if(display_num > DISPLAY_FUNC_NUM)
			{
				display_num = 0;
			}
		}
	}
	
	// 具体显示样式
	switch(display_num)
	{
		case 1:
			display1();
		break;
		case 2:
			display2();
		break;
		case 3:
			display3();
		break;
		default:
			display3();
		break;
	}
}


/* 函数名称：display1
 * 功能描述：单柱显示, 中间对称显示
 * 参数：无
 * 返回值：无
 */
void display1(void)
{
	uint16_t i=0, x=0, y=0;
	uint8_t temp = 0;
	
	/*******************显示*******************/
	// 清除缓存
	GUI_ClearSCR();
	
	for(i = 1; i < 128; i++)	
	{
		temp = lBufMagArray[i]/prt; 
		if(temp > 63) temp = 63;	// 限制显示范围
		
		x = 128-i;
		y = 63-temp;	
		GUI_Line(x, y, x, 63, 0x0f);
		
		//画下落的点
		if(fal_pot[i]>y) fal_pot[i] = y;
		else
		{
				if(fal_pot[i]>62) fal_pot[i]=62;	// 这里限制一下                                       
				GUI_Line(x, fal_pot[i], x,fal_pot[i]+1, 0xf);
				fal_pot[i] += 1 ;
		}
	}
	for(i = 1; i < 128; i++)	
	{
		temp = lBufMagArray[i]/prt; 
		if(temp > 63) temp = 63;	// 限制显示范围
		
		x = 127+i;
		y = 63-temp;	
		GUI_Line(x, y, x, 63, 0x0f);
		
		//画下落的点
		if(fal_pot[i]>y) fal_pot[i] = y;
		else
		{
				if(fal_pot[i]>62) fal_pot[i]=62;	// 这里限制一下                                       
				GUI_Line(x, fal_pot[i], x,fal_pot[i]+1, 0xf);
				fal_pot[i] += 1 ;
		}
	}
	GUI_Exec();
}

/* 函数名称：display2
 * 功能描述：柱状显示 2个单位宽
 * 参数：无
 * 返回值：无
 */
void display2(void)
{
	uint16_t i=0, x=0, y=0;
	uint8_t temp = 0;
	
	/*******************显示*******************/
	// 清除缓存
	GUI_ClearSCR();

	for(i = 0; i < 64; i++)	//间隔的取32个频率出来显示
	{
		
		temp = lBufMagArray[i+1]/prt;
		if(temp > 63) temp = 63;	// 限制显示范围
		
		x = (i<<2);	//i*4
		y = 63-temp;	//加1是为了丢掉第一个直流分量
		if(y>60) y = 60;
		
		GUI_LineWith(x, y, x, 63, 3, 0x0f);	// 划一条有宽度的线
		
		//画下落的点
		if(fal_pot[i]>y) fal_pot[i] = y;
		else
		{
				if(fal_pot[i]>60) fal_pot[i]=60;
				GUI_LineWith(x,fal_pot[i],x,fal_pot[i]+3,3,0xf);
				fal_pot[i] += (10-fal_speed) ;
		}
	}
	GUI_Exec();
}

/* 函数名称：display3
 * 功能描述：上时域下谱域
 * 参数：无
 * 返回值：无
 */
void display3(void)
{
	uint16_t i=0, x=0, y=0;
	uint8_t temp = 0;
	uint8_t points[256];
	
	/*******************显示*******************/
	// 清除缓存
	GUI_ClearSCR();
	// 时域
	for(i = 0; i < 128; i++)
	{
		points[2*i] = 2*i;
		points[2*i+1] = adc_buf[i]/128;
		if(points[2*i+1] > 32)	points[2*i+1] = 32;	// 限制在上半屏幕
	}
	GUI_PointS(points, 128, 0x0f);

	// 谱域
	for(i = 0; i < 64; i++)	//间隔的取32个频率出来显示
	{
		
		temp = lBufMagArray[i+1]/prt;
		if(temp > 63) temp = 63;	// 限制显示范围
		
		x = (i<<2);	//i*4
		y = 63-temp;	
		if(y<32) y = 32;	// 限制到下半屏幕
		if(y>60) y = 60;
		GUI_LineWith(x, y, x, 63, 3, 0xf);	// 划一条有宽度的线
		
		//画下落的点
		if(fal_pot[i]>y) fal_pot[i] = y;
		else
		{
				if(fal_pot[i]>60) fal_pot[i]=60;
				GUI_LineWith(x,fal_pot[i],x,fal_pot[i]+3,3,0xf);
				fal_pot[i] += (10-fal_speed) ;
		}
	}
	GUI_Exec();
}


///* 函数名称：display7
// * 功能描述：测试
// * 参数：无
// * 返回值：无
// */
//void display7(void)
//{
//	uint16_t i=0;
//	
//	/*******************显示*******************/
//	// 清除缓存
//	GUI_ClearSCR();
//	for(i=0; i<128; i++)
//	{
//		GUI_Point(2*i, adc_buf[i]/100, 0x0f);
//		GUI_Point(2*i+1, adc_buf[i]/100, 0x0f);
//	}
//	GUI_Exec();
//}






