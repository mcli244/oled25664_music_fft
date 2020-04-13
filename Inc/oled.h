//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2017-0101
//  ����޸�   : 
//  ��������   : OLED 4�ӿ���ʾ����(STM32F103ZET6ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              SCK   ��PC0��SCL��PA7
//              SDA   ��PC1��SDA��PA6
//              RES  ��PG15				PA5
//              DC   ��PD3				PA4
//              CS1   ��PD6       PA3  
//              FSO   ��PC11
//              CS2   ��PC12

//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//******************************************************************************/
//******************************************************************************/


#ifndef __OLED_H
#define __OLED_H			  	 
#include "stm32f1xx_hal.h"
#include "stdlib.h"	 

//���ݽṹ
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

// ��ʾ������ ��oled.c�ж���
extern uint8_t OLED_GRAM[64][128];


//OLEDģʽ����
//0:4�ߴ���ģʽ
#define	Shift		0x1C
		    						  
/*******************************add mcli******************************************
 * ���ߣ�mcli
 *	ʱ�䣺2019/1/26
 * �������޸�ΪӲ��SPI������OLED�������о�԰�ṩ�������޸ĵ����ģ�
 * ����Ҫע������о�԰�ṩ�����ϣ���ʹ��Ӳ��SPI��ʱ��ֻ�õ���MOSI
 * �൱��ֻ��OLED����д�������û�ж�ȡ������
 */

// ʹ��8080����ͨ��

#define DATE_PORT		GPIOB

#define RD					GPIO_PIN_15		// ��ȡ	�͵�ƽʹ����������Ϊ���״̬���������MCU��ȡ
#define WR					GPIO_PIN_14		// д��	����������������
#define A0					GPIO_PIN_13		// ����/����	�͵�ƽΪ����	�ߵ�ƽΪ����
#define RES					GPIO_PIN_12		// ��λ �͵�ƽ��λ
#define CS					GPIO_PIN_11		// Ƭѡ �͵�ƽʹ��


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


 
// ʹ��Ӳ��SPI ��������û������
#define OLED_SCLK_Clr() HAL_GPIO_WritePin(DATE_PORT, SCK, GPIO_PIN_RESET)//CLK
#define OLED_SCLK_Set() HAL_GPIO_WritePin(DATE_PORT, SCK, GPIO_PIN_SET)

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(DATE_PORT, SDA, GPIO_PIN_RESET)	//DIN
#define OLED_SDIN_Set() HAL_GPIO_WritePin(DATE_PORT, SDA, GPIO_PIN_SET)

//----------------------------------------------------------------------------
#define ZK_CS_Clr()  HAL_GPIO_WritePin(DATE_PORT, ZK_CS, GPIO_PIN_RESET)//�ֿ�Ƭѡ
#define ZK_CS_Set()  HAL_GPIO_WritePin(DATE_PORT, ZK_CS, GPIO_PIN_SET)					
	
#define ZK_OUT_Clr() HAL_GPIO_WritePin(DATE_PORT, ZK_DO, GPIO_PIN_RESET)//�ֿ�spi�������
#define ZK_OUT_Set() HAL_GPIO_WritePin(DATE_PORT, ZK_DO, GPIO_PIN_SET)	
				
#define ZK_OUT()    HAL_GPIO_ReadPin(DATE_PORT, ZK_DO)//��ȡָ���ܽŵ�����

/************************************add end*************************************/


#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����
//OLED�����ú���

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

// �Զ��庯��
void OLED_ON(void);
void OLED_OFF(void);
// ���ﺯ���������Դ���������� ֻ��OLED_Refresh_Gram���ǰ�����д��OLED��
void OLED_DrawPoint(uint16_t x, uint16_t y, uint8_t color);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t color);
void OLED_Clear(void);
void OLED_Refresh_Gram(void);

//Ӳ�ֿ⺯��
void GBZK_ROMOUTSET(void);		//�ֿ�������� 
void GBZK_ROMOUTRESET(void);	 	//�ֿ��������
void GBZK_GPIO_Config(void);	 //�ֿ�оƬ��ʼ��
void ZK_command( unsigned char  data );
unsigned char  get_data_from_ROM(void);
void get_n_bytes_data_from_ROM(unsigned char  AddrHigh,unsigned char  AddrMid,unsigned char  AddrLow,unsigned char  *pBuff,unsigned char  DataLen );
void Display_GB2312_String(unsigned char zk_num,unsigned int x,unsigned int y, unsigned char  text[]);	// ��ʾ�����ַ���
void Display_GB2312( unsigned char zk_num,unsigned int x,unsigned int y );								 //	��ʾ����
void Display_GBasc_String(unsigned char zk_num,unsigned int x,unsigned int y , unsigned char  text[]);	  // ��ʾGB2312 ascii �ַ���
void Display_GBasc( unsigned char zk_num,unsigned int x,unsigned int y  );								  //  ��ʾGB2312 ascii 
void Display_Asc_String(unsigned char zk_num,unsigned int x,unsigned int y , unsigned char  text[]);		  //   ��ʾASCII �ַ���
void Display_Asc( unsigned char zk_num,unsigned int x,unsigned int y);									  //   ��ʾASCII
void CL_Mem(void);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);




#endif  
	 



