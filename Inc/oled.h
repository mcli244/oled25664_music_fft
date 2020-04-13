//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2017-0101
//  最近修改   : 
//  功能描述   : OLED 4接口演示例程(STM32F103ZET6系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              SCK   接PC0（SCL）PA7
//              SDA   接PC1（SDA）PA6
//              RES  接PG15				PA5
//              DC   接PD3				PA4
//              CS1   接PD6       PA3  
//              FSO   接PC11
//              CS2   接PC12

//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//******************************************************************************/
//******************************************************************************/


#ifndef __OLED_H
#define __OLED_H			  	 
#include "stm32f1xx_hal.h"
#include "stdlib.h"	 

//数据结构
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

// 显示缓存区 在oled.c中定义
extern uint8_t OLED_GRAM[64][128];


//OLED模式设置
//0:4线串行模式
#define	Shift		0x1C
		    						  
/*******************************add mcli******************************************
 * 作者：mcli
 *	时间：2019/1/26
 * 描述：修改为硬件SPI来驱动OLED，根据中景园提供的历程修改得来的，
 * 这里要注意根据中景园提供的资料，在使用硬件SPI的时候，只用到了MOSI
 * 相当于只对OLED进行写入操作，没有读取操作。
 */

// 使用8080并口通信

#define DATE_PORT		GPIOB

#define RD					GPIO_PIN_15		// 读取	低电平使能数据总线为输出状态，供给外界MCU读取
#define WR					GPIO_PIN_14		// 写入	数据在上升沿锁存
#define A0					GPIO_PIN_13		// 数据/命令	低电平为命令	高电平为数据
#define RES					GPIO_PIN_12		// 复位 低电平复位
#define CS					GPIO_PIN_11		// 片选 低电平使能


#define ZK_CS				GPIO_PIN_2
#define ZK_DO				GPIO_PIN_1


#define OLED_WR_Clr() HAL_GPIO_WritePin(DATE_PORT, WR, GPIO_PIN_RESET)	//WR
#define OLED_WR_Set() HAL_GPIO_WritePin(DATE_PORT, WR, GPIO_PIN_SET)

#define OLED_A0_Clr() HAL_GPIO_WritePin(DATE_PORT, A0, GPIO_PIN_RESET)	//A0
#define OLED_A0_Set() HAL_GPIO_WritePin(DATE_PORT, A0, GPIO_PIN_SET)

#define OLED_RST_Clr() HAL_GPIO_WritePin(DATE_PORT, RES, GPIO_PIN_RESET)	//RES
#define OLED_RST_Set() HAL_GPIO_WritePin(DATE_PORT, RES, GPIO_PIN_SET)

#define OLED_CS_Clr()  HAL_GPIO_WritePin(DATE_PORT, CS, GPIO_PIN_RESET)	//CS
#define OLED_CS_Set()  HAL_GPIO_WritePin(DATE_PORT, CS, GPIO_PIN_SET)

#define OLED_RD_Clr() HAL_GPIO_WritePin(DATE_PORT, RD, GPIO_PIN_RESET)	//RD
#define OLED_RD_Set() HAL_GPIO_WritePin(DATE_PORT, RD, GPIO_PIN_SET)


 
// 使用硬件SPI 这两个就没有用了
#define OLED_SCLK_Clr() HAL_GPIO_WritePin(DATE_PORT, SCK, GPIO_PIN_RESET)//CLK
#define OLED_SCLK_Set() HAL_GPIO_WritePin(DATE_PORT, SCK, GPIO_PIN_SET)

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(DATE_PORT, SDA, GPIO_PIN_RESET)	//DIN
#define OLED_SDIN_Set() HAL_GPIO_WritePin(DATE_PORT, SDA, GPIO_PIN_SET)

//----------------------------------------------------------------------------
#define ZK_CS_Clr()  HAL_GPIO_WritePin(DATE_PORT, ZK_CS, GPIO_PIN_RESET)//字库片选
#define ZK_CS_Set()  HAL_GPIO_WritePin(DATE_PORT, ZK_CS, GPIO_PIN_SET)					
	
#define ZK_OUT_Clr() HAL_GPIO_WritePin(DATE_PORT, ZK_DO, GPIO_PIN_RESET)//字库spi数据输出
#define ZK_OUT_Set() HAL_GPIO_WritePin(DATE_PORT, ZK_DO, GPIO_PIN_SET)	
				
#define ZK_OUT()    HAL_GPIO_ReadPin(DATE_PORT, ZK_DO)//读取指定管脚的输入

/************************************add end*************************************/


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
//OLED控制用函数

void Delay(unsigned int Data);
void Write_Register(unsigned char Data);
void Write_Parameter(unsigned char Data);
void Column_Address(unsigned char a);
void Row_Address(unsigned char a);
void ClearLED (unsigned char color);
void Set_Colume_Address(unsigned char Addr1,unsigned char Addr2);
void Set_Row_Address(unsigned char Addr1,unsigned char Addr2);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowString(u8 x,u8 y,u8 *chr);
u32 oled_pow(u8 m,u8 n);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowCHinese(u8 x,u8 y,u8 *chn);
void OLED_DrawBMP(u8 *BMP);
void Initial_sh1122(void);
void drawpoint (unsigned char x,unsigned char y,unsigned char color);
void OLED_Set_Pos(unsigned char x, unsigned char y) ;
void bar (void);

// 自定义函数
void OLED_ON(void);
void OLED_OFF(void);
// 这里函数都是在显存区里面操作 只有OLED_Refresh_Gram才是把数据写入OLED中
void OLED_DrawPoint(uint16_t x, uint16_t y, uint8_t color);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t color);
void OLED_Clear(void);
void OLED_Refresh_Gram(void);

//硬字库函数
void GBZK_ROMOUTSET(void);		//字库输出设置 
void GBZK_ROMOUTRESET(void);	 	//字库输出设置
void GBZK_GPIO_Config(void);	 //字库芯片初始化
void ZK_command( unsigned char  data );
unsigned char  get_data_from_ROM(void);
void get_n_bytes_data_from_ROM(unsigned char  AddrHigh,unsigned char  AddrMid,unsigned char  AddrLow,unsigned char  *pBuff,unsigned char  DataLen );
void Display_GB2312_String(unsigned char zk_num,unsigned int x,unsigned int y, unsigned char  text[]);	// 显示汉字字符串
void Display_GB2312( unsigned char zk_num,unsigned int x,unsigned int y );								 //	显示汉字
void Display_GBasc_String(unsigned char zk_num,unsigned int x,unsigned int y , unsigned char  text[]);	  // 显示GB2312 ascii 字符串
void Display_GBasc( unsigned char zk_num,unsigned int x,unsigned int y  );								  //  显示GB2312 ascii 
void Display_Asc_String(unsigned char zk_num,unsigned int x,unsigned int y , unsigned char  text[]);		  //   显示ASCII 字符串
void Display_Asc( unsigned char zk_num,unsigned int x,unsigned int y);									  //   显示ASCII
void CL_Mem(void);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);




#endif  
	 



