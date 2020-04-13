//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/shop/view_shop.htm?tracelog=twddp&trace_log=qianniu_pc
//�޸�����:2014/3/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//********************************************************************************
#include "oled.h"
#include "oledfont.h"


// ��ʾ������
// ÿһ����Ա����uint8_t���͵ģ���ʾ����������ݡ�����128����Ա����ʾ256���������
// OLED25664ÿ�����������4bit��ɣ�������16���Ҷȼ���
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
//		for(temp=0; temp<0xf; temp++);			// ģ��SPI��ʱ���޸�������ˢ���ٶ�
	}
}

/******************add mcli***************************
* ���ߣ�mcli
*	ʱ�䣺2019/1/26
* ������ʹ��8080����ͨ��
*/

/* �������ƣ�Write_Register
 * ����������д�Ĵ���
 * ��������
 * ����ֵ����
 */
void Write_Register(unsigned char Data)
{
	
	GPIOB->ODR &= (~(0xff));		// �����8λ PB0-PB7
	GPIOB->ODR |= Data;
	

	OLED_A0_Clr();	// д���������
	OLED_CS_Clr();	// ʹ��оƬ
	OLED_WR_Clr();	// д�� ʹ��

	OLED_WR_Set();
	OLED_CS_Set();
	OLED_A0_Set();	
}

/* �������ƣ�Write_Parameter
 * ����������д����
 * ��������
 * ����ֵ����
 */
void Write_Parameter(unsigned char Data)
{
	
	GPIOB->ODR &= (~(0xff));		// �����8λ PB0-PB7
	GPIOB->ODR |= Data;
	
	OLED_A0_Set();	// ����
	OLED_CS_Clr();	// ʹ��оƬ
	OLED_WR_Clr();	// д�� ʹ��

	OLED_WR_Set();
	OLED_CS_Set();	
	OLED_A0_Set();
}

/* �������ƣ�OLED_ON
 * ������������ʾ��
 * ��������
 * ����ֵ����
 */
void OLED_ON(void)
{
	Write_Register(0xae);
}

/* �������ƣ�OLED_OFF
 * ������������ʾ��
 * ��������
 * ����ֵ����
 */
void OLED_OFF(void)
{
	Write_Register(0xaf);
}


/* �������ƣ�OLED_Refresh_Gram
 * ��������������ʾ������������д��OLED����ʾ
 * ��������
 * ����ֵ����
 */
void OLED_Refresh_Gram(void)
{
	uint8_t i,n;		    
	for(i=0;i<64;i++)  
	{  
		
		// ����ҳ��ַ
		Write_Register (0xb0);    	//����ҳ��ַ��0~64��
		Write_Register(i);
		
		// �����е�ַΪ0
		Write_Register (0x00);      //������ʾλ�á��е͵�ַ
		Write_Register (0x10);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)
			Write_Parameter(OLED_GRAM[i][n]); // д������������ݣ�һ����4λ
	}   
}

/* �������ƣ�OLED_Refresh_Gram
 * ��������������ʾ������������д��OLED����ʾ
 * ��������
 * ����ֵ����
 */
void OLED_DrawPoint(uint16_t x, uint16_t y, uint8_t color)
{
	uint8_t temp = 0;
	if(x>256 || y>64) return;
	if(color>15) color = 15;
	temp = x/2;
	if((x%2) == 0)	//���ֽ�
	{
		OLED_GRAM[y][temp] &= 0x0f;	// �����4λ
		OLED_GRAM[y][temp] |= (color<<4);	// ȡ��ɫֵ�ĵ�4λ���Ҷ�ֵ(0-15)
	}
	else
	{
		OLED_GRAM[y][temp] &= 0xf0;	// �����4λ
		OLED_GRAM[y][temp] |= color;	// ȡ��ɫֵ�ĵ�4λ���Ҷ�ֵ(0-15)
	}
		
}
/* �������ƣ�OLED_Refresh_Gram
 * ��������������ʾ������������д��OLED����ʾ
 * ��������
 * ����ֵ����
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
/* �������ƣ�OLED_Clear
 * ������������������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	
 * ��������
 * ����ֵ����
 */
void OLED_Clear(void)  
{  
	uint16_t i,n;  
	for(i=0;i<64;i++)for(n=0;n<128;n++)OLED_GRAM[i][n]=0X00;  
}
//*******************add end*************************/

//******************************************************************************
//    ����˵����OLED��ʾ�е���ʼ��ֹ��ַ
//    ������ݣ�a  �е���ʼ��ַ
//              b  �е���ֹ��ַ
//    ����ֵ��  ��
//******************************************************************************
void Column_Address(unsigned char x)
{
	//Write_Register(0x15);			        // Set Column Address
	Write_Register(((x&0xf0)>>4)|0x10);				//   Default => 0x00
	Write_Register((x&0x0f)); 			//   Default => 0x77
}

//******************************************************************************
//    ����˵����OLED��ʾ�е���ʼ��ֹ��ַ
//    ������ݣ�a  �е���ʼ��ַ
//              b  �е���ֹ��ַ
//    ����ֵ��  ��
//******************************************************************************
void Row_Address(unsigned char a)
{
	Write_Register(0xb0);			// Set Row Address
	Write_Parameter(a);				//   Default => 0x00
//	Write_Parameter(b);				//   Default => 0x3F
}

//******************************************************************************
//    ����˵����OLED������ʾ
//    ������ݣ���
//    ����ֵ��  ��
//******************************************************************************
void ClearLED (unsigned char color)
{
	unsigned char x,y; 
//	Write_Register(0x5C);    //��ʼдRAM����
	
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
	Write_Parameter(Addr1+28); 	//���е�ַ��ʼ
	Write_Parameter(Addr2+28);  	//���е�ַ����
}

//=======д�е�ַ==============
void Set_Row_Address(unsigned char Addr1,unsigned char Addr2)
{
	Write_Register(0x75); 
    Write_Parameter(Addr1); 	//���е�ַ��ʼ
    Write_Parameter(Addr2);  	//���е�ַ����
}
//============��ʾһ��8*16���ַ�==============================
void OLED_ShowChar(u8 x,u8 y,u8 chr)
{unsigned char c=0,i=0;	
/*8x16������ϰ벿�ּ���8x8*/
	u8 m,x1,y1,y2;
	u8 Data1,Data2,DATA1=0;
			c=chr-' ';//�õ�ƫ�ƺ��ֵ
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
	
	/*****************8x16������°벿�ּ���8x8****************************************/

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

//��ʾһ���ַ��Ŵ�
void OLED_ShowString(u8 x,u8 y,u8 *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j]);
			x+=4;
			j++;
	}
}

//m^n����
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
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
	/*****************16x16������ϰ벿�֣�����8x16****************************************/
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

//BMPΪͼ�����ݣ�Gray_Level=0ʱȫ��ֻ��ʾһ����ɫ��Gray_Level=1ʱ��ʾ֧�ֻҶ�ͼƬ��ColorΪ�Ҷ�ֵ0��15;
void OLED_DrawBMP(u8 *BMP)
{/*****************16x16������ϰ벿�֣�����8x16****************************************/
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
	/*����ԭ��
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
//    ����˵����OLED����ʼ��
//    ������ݣ���
//    ����ֵ��  ��
//******************************************************************************
//              GND    ��Դ��
//              VCC  	��5V��3.3v��Դ
//              SCK   ��PC0��SCL��		
//              SDA   ��PC1��SDA��
//              RES  	��PG15				
//              DC   	��PD3				
//              CS1   ��PD6        
//              FSO   ��PC11
//              CS2   ��PC12
void Initial_sh1122(void)
{	
 	
	GPIO_Config();	// ����ʹ�õ���Ӳ��SPI IO�ڵĳ�ʼ���������ط�ִ���� �����������һ���պ���

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

