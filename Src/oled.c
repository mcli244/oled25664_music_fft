//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/shop/view_shop.htm?tracelog=twddp&trace_log=qianniu_pc
//修改日期:2014/3/16
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//********************************************************************************
#include "oled.h"
#include "oledfont.h"


// 显示缓存区
// 每一个成员都是uint8_t类型的，表示两个点的数据。所以128个成员，表示256个点的数据
// OLED25664每个点的数据由4bit组成，所有有16个灰度级别
//0:[0] [1] [2] [3] ... [128]		
//1:[0] [1] [2] [3] ... [128]	
//2:[0] [1] [2] [3] ... [128]
//...
//64:[0] [1] [2] [3] ... [128]
uint8_t OLED_GRAM[64][128];

void OLED_Delay(unsigned int Data)
{
	unsigned int temp;
	for(; Data>0; Data--)
	{
		for(temp=0; temp<0xfff; temp++);
//		for(temp=0; temp<0xf; temp++);			// 模拟SPI的时候修改以提升刷新速度
	}
}

/******************add mcli***************************
* 作者：mcli
*	时间：2019/1/26
* 描述：使用8080并口通信
*/

/* 函数名称：Write_Register
 * 功能描述：写寄存器
 * 参数：无
 * 返回值：无
 */
void Write_Register(unsigned char Data)
{
	
	GPIOB->ODR &= (~(0xff));		// 清零低8位 PB0-PB7
	GPIOB->ODR |= Data;
	

	OLED_A0_Clr();	// 写入的是命令
	OLED_CS_Clr();	// 使能芯片
	OLED_WR_Clr();	// 写入 使能

	OLED_WR_Set();
	OLED_CS_Set();
	OLED_A0_Set();	
}

/* 函数名称：Write_Parameter
 * 功能描述：写数据
 * 参数：无
 * 返回值：无
 */
void Write_Parameter(unsigned char Data)
{
	
	GPIOB->ODR &= (~(0xff));		// 清零低8位 PB0-PB7
	GPIOB->ODR |= Data;
	
	OLED_A0_Set();	// 数据
	OLED_CS_Clr();	// 使能芯片
	OLED_WR_Clr();	// 写入 使能

	OLED_WR_Set();
	OLED_CS_Set();	
	OLED_A0_Set();
}

/* 函数名称：OLED_ON
 * 功能描述：显示开
 * 参数：无
 * 返回值：无
 */
void OLED_ON(void)
{
	Write_Register(0xae);
}

/* 函数名称：OLED_OFF
 * 功能描述：显示关
 * 参数：无
 * 返回值：无
 */
void OLED_OFF(void)
{
	Write_Register(0xaf);
}


/* 函数名称：OLED_Refresh_Gram
 * 功能描述：把显示缓存区的数据写入OLED中显示
 * 参数：无
 * 返回值：无
 */
void OLED_Refresh_Gram(void)
{
	uint8_t i,n;		    
	for(i=0;i<64;i++)  
	{  
		
		// 设置页地址
		Write_Register (0xb0);    	//设置页地址（0~64）
		Write_Register(i);
		
		// 设置列地址为0
		Write_Register (0x00);      //设置显示位置—列低地址
		Write_Register (0x10);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)
			Write_Parameter(OLED_GRAM[i][n]); // 写入两个点的数据，一个点4位
	}   
}

/* 函数名称：OLED_Refresh_Gram
 * 功能描述：把显示缓存区的数据写入OLED中显示
 * 参数：无
 * 返回值：无
 */
void OLED_DrawPoint(uint16_t x, uint16_t y, uint8_t color)
{
	uint8_t temp = 0;
	if(x>256 || y>64) return;
	if(color>15) color = 15;
	temp = x/2;
	if((x%2) == 0)	//高字节
	{
		OLED_GRAM[y][temp] &= 0x0f;	// 清除高4位
		OLED_GRAM[y][temp] |= (color<<4);	// 取颜色值的低4位即灰度值(0-15)
	}
	else
	{
		OLED_GRAM[y][temp] &= 0xf0;	// 清除高4位
		OLED_GRAM[y][temp] |= color;	// 取颜色值的低4位即灰度值(0-15)
	}
		
}
/* 函数名称：OLED_Refresh_Gram
 * 功能描述：把显示缓存区的数据写入OLED中显示
 * 参数：无
 * 返回值：无
 */
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t color)
{
	uint8_t i,n;		    
	for(i=x1;i<x2-x1;i++)  
	{    
		for(n=y1;n<y2-y1;n++)
			OLED_DrawPoint(i, n, color);
	}   
}
/* 函数名称：OLED_Clear
 * 功能描述：清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	
 * 参数：无
 * 返回值：无
 */
void OLED_Clear(void)  
{  
	uint16_t i,n;  
	for(i=0;i<64;i++)for(n=0;n<128;n++)OLED_GRAM[i][n]=0X00;  
}
//*******************add end*************************/

//******************************************************************************
//    函数说明：OLED显示列的起始终止地址
//    入口数据：a  列的起始地址
//              b  列的终止地址
//    返回值：  无
//******************************************************************************
void Column_Address(unsigned char x)
{
	//Write_Register(0x15);			        // Set Column Address
	Write_Register(((x&0xf0)>>4)|0x10);				//   Default => 0x00
	Write_Register((x&0x0f)); 			//   Default => 0x77
}

//******************************************************************************
//    函数说明：OLED显示行的起始终止地址
//    入口数据：a  行的起始地址
//              b  行的终止地址
//    返回值：  无
//******************************************************************************
void Row_Address(unsigned char a)
{
	Write_Register(0xb0);			// Set Row Address
	Write_Parameter(a);				//   Default => 0x00
//	Write_Parameter(b);				//   Default => 0x3F
}

//******************************************************************************
//    函数说明：OLED清屏显示
//    入口数据：无
//    返回值：  无
//******************************************************************************
void ClearLED (unsigned char color)
{
	unsigned char x,y; 
//	Write_Register(0x5C);    //开始写RAM命令
	
	for(y=0;y<64;y++)
	{                   
		for(x=0;x<128;x++)
		{    
			Write_Parameter(color | (color << 4));
		}
	}
}

void bar ()
{ 
	unsigned char i,j,k;
	
	for(i=0;i<15;i++)
	{
		for(k=0;k<64;k++)
		{
				OLED_Set_Pos( 8*i,k) ;
				for(j=0;j<16;j++)
				{
						Write_Parameter (i | (i << 4));				
				}
		}
	}
}


void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	Write_Register(0xB0);
	Write_Register(y);
	Write_Register(((x&0xf0)>>4)|0x10);
	Write_Register((x&0x0f)); 
} 
void drawpoint (unsigned char x,unsigned char y,unsigned char color)
{
	OLED_Set_Pos( x,   y) ;
	Write_Parameter(color | (color << 4));
}
void Set_Colume_Address(unsigned char Addr1,unsigned char Addr2)
{
	Write_Register(0x15); 
	Write_Parameter(Addr1+28); 	//送列地址开始
	Write_Parameter(Addr2+28);  	//送列地址结束
}

//=======写行地址==============
void Set_Row_Address(unsigned char Addr1,unsigned char Addr2)
{
	Write_Register(0x75); 
    Write_Parameter(Addr1); 	//送行地址开始
    Write_Parameter(Addr2);  	//送行地址结束
}
//============显示一个8*16的字符==============================
void OLED_ShowChar(u8 x,u8 y,u8 chr)
{unsigned char c=0,i=0;	
/*8x16点阵的上半部分即上8x8*/
	u8 m,x1,y1,y2;
	u8 Data1,Data2,DATA1=0;
			c=chr-' ';//得到偏移后的值
	x1=x;
	y1=y;
		for(m=0;m<4;m++)
	{

			Data1=F8X16[c*16+2*m+0];
			Data2=F8X16[c*16+2*m+1];
			for(i=0;i<8;i++)
			{
				if(Data1&(0x01<<i))	
				{
					DATA1=0xf0;
				}
				if(Data2&(0x01<<i))
				{
					DATA1|=0x0f;
				}	
					OLED_Set_Pos( x1,y1) ;
				y1++;
				
				Write_Parameter (DATA1);
				DATA1=0;
				
			}
			x1++;
			y1=y;
		}
	
	/*****************8x16点阵的下半部分即下8x8****************************************/

			x1=x;
	y2=y+8;
		for(m=0;m<4;m++)
	{

			Data1=F8X16[c*16+2*m+8];
			Data2=F8X16[c*16+2*m+9];
			for(i=0;i<8;i++)
			{
				if(Data1&(0x01<<i))	
				{
					DATA1=0xf0;
				}
				if(Data2&(0x01<<i))
				{
					DATA1|=0x0f;
				}	
					OLED_Set_Pos( x1,y2) ;
				y2++;
				
				Write_Parameter (DATA1);
				DATA1=0;
				
			}
			x1++;
			y2=y+8;
		}
}	

//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j]);
			x+=4;
			j++;
	}
}

//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/4)*t,y,' ');
				continue;
			}else enshow=1; 
		}
	 	OLED_ShowChar(x+4*t,y,temp+'0'); 
	}
}


void OLED_ShowCHinese(u8 x,u8 y,u8 *chn)
{
	/*****************16x16点阵的上半部分，即上8x16****************************************/
	u8 j,i,k;
	u8 Data1,Data2,DATA1=0,DATA2=0;
	//x_number=x_number/4;
	
	k=y;
	
	for(j=0;j<16;j++)
	{
		OLED_Set_Pos( x,   k) ;
		Data1=*chn++;
		Data2=*chn++;
		for(i=0;i<4;i++)
		{
			if(Data1&(0x01<<(i*2)))	
			{
				DATA1=0xF0;
			}
			if(Data1&(0x01<<(i*2+1)))
			{
				DATA1|=0x0F;
			}	
			Write_Parameter (DATA1);
			DATA1=0;
		}
		for(i=0;i<4;i++)
		{
			if(Data2&(0x01<<(i*2)))	
			{
				DATA2=0xF0;
			}
			if(Data2&(0x01<<(i*2+1)))
			{
				DATA2|=0x0F;
			}	
			Write_Parameter (DATA2);
			DATA2=0;
		}
		k++;
	}
}	

//BMP为图像数据，Gray_Level=0时全屏只显示一种颜色，Gray_Level=1时显示支持灰度图片；Color为灰度值0～15;
void OLED_DrawBMP(u8 *BMP)
{/*****************16x16点阵的上半部分，即上8x16****************************************/
	u8 j,k;
	for(k=0;k<64;k++)
	{
		OLED_Set_Pos( 0,   k) ;
		for(j=0;j<128;j++)
		{
			Write_Parameter (*(BMP+128*k+j));
		}
	}
}


// add by mcli
void GPIO_Config(void)
{
	/*这是原版
	GPIO_InitTypeDef  GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOG, ENABLE);	 //??PC,D,G????

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_6;	 //PD3,PD6????  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??50MHz
 	GPIO_Init(GPIOD, &GPIO_InitStructure);	  //???GPIOD3,6
 	GPIO_SetBits(GPIOD,GPIO_Pin_3|GPIO_Pin_6);	//PD3,PD6 ???

 #if OLED_MODE==1
 
 	GPIO_InitStructure.GPIO_Pin =0xFF; //PC0~7 OUT????
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOC,0xFF); //PC0~7???

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //PG13,14,15 OUT????
 	GPIO_Init(GPIOG, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOG,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);						 //PG13,14,15 OUT  ???

 #else
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;				 //PC0,1 OUT????
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1);						 //PC0,1 OUT  ???

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //PG15 OUT????	  RST
 	GPIO_Init(GPIOG, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOG,GPIO_Pin_15);						 //PG15 OUT  ???
 #endif	*/
 

}
//******************************************************************************
//    函数说明：OLED屏初始化
//    入口数据：无
//    返回值：  无
//******************************************************************************
//              GND    电源地
//              VCC  	接5V或3.3v电源
//              SCK   接PC0（SCL）		
//              SDA   接PC1（SDA）
//              RES  	接PG15				
//              DC   	接PD3				
//              CS1   接PD6        
//              FSO   接PC11
//              CS2   接PC12
void Initial_sh1122(void)
{	
 	
	GPIO_Config();	// 这里使用的是硬件SPI IO口的初始化在其他地方执行了 所以这里就是一个空函数

	OLED_RST_Clr();	
	OLED_Delay(1000);	
	OLED_RST_Set();
	Write_Register(0xAE); //Set Display Off
	Write_Register(0xB0); //Row address Mode Setting
	Write_Register(0x00);
	Write_Register(0x10); //Set Higher Column Address of display RAM
	Write_Register(0x00); //Set Lower Column Address of display RAM
	Write_Register(0xD5); //Set Display Clock Divide Ratio/Oscillator Frequency
//	Write_Register(0x50); //50 125hz
	Write_Register(0x40); //mcli 2019/1/26
	Write_Register(0xD9); //Set Discharge/Precharge Period
	Write_Register(0x22);
	Write_Register(0x40); //Set Display Start Line
	Write_Register(0x81); //The Contrast Control Mode Set
	Write_Register(0xFF);
	Write_Register(0xA0); //Set Segment Re-map
	Write_Register(0xC0); //Set Common Output Scan Direction
	Write_Register(0xA4); //Set Entire Display OFF/ON
	Write_Register(0xA6); //Set Normal/Reverse Display
	Write_Register(0xA8); //Set Multiplex Ration
	Write_Register(0x3F);
	Write_Register(0xAD); //DC-DC Setting
	Write_Register(0x80); //DC-DC is disable
	Write_Register(0xD3); //Set Display Offset
	Write_Register(0x00);
	Write_Register(0xDB); //Set VCOM Deselect Level
	Write_Register(0x30);
	Write_Register(0xDC); //Set VSEGM Level
	Write_Register(0x30);
	Write_Register(0x33); //Set Discharge VSL Level 1.8V
	Write_Register(0xAF); //Set Display On
	OLED_Set_Pos(0, 0) ;
}

