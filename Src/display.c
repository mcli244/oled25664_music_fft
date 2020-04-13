/***********************************************
 * �ļ����ƣ�display.c
 * ��    �ߣ�mcli
 * ��    �ڣ�2019/2/28
 * ��    ����v1.0
 * ��    �������ļ�Ϊ��ʾ���Դ���װ
 ***********************************************/
#include "display.h"
#include "fft.h"
#include "tu.h"

uint8_t fal_pot[NPT];							// ��¼����������
uint8_t prt = 4;									// ������ʾ�ı���
uint8_t display_num = 1;					// ��ʾ��ʽ������
uint8_t fal_speed = 7;						// �����ٶ�
uint8_t display_auto = 1;					// �Զ���ʾ��־λ

uint8_t Current_display_mode = DISPLAY_MODE_TIME;	// ��ǰ��ʾģʽ

/* �������ƣ�Display_Init
 * ������������ʾ��ʼ��
 * ��������
 * ����ֵ����
 */
void Display_Init(void)
{
	uint16_t i = 0;
	
	LED_Ctr(LED_OFF);	// �ر�USER_LED
	
	// ��ʾ��ʼ�� ʹ��GUI
	GUI_Initialize();
	GUI_SetColor(0x0f, 0);
	GUI_LoadPic(64,0,(uint8_t *)&gImage_bg,128,64);
	GUI_Exec();
	
	//��ʼ������� ������ĵ� ��ʼ��Ϊ��ײ���ʾ
	for(i=0; i<NPT; i++)
		fal_pot[i] = 63;
	HAL_Delay(1000);
	
}


/* �������ƣ�void LED_Ctr(uint8_t sta)
 * ����������user_led���ƺ���
 * ��    ������
 * �� �� ֵ��ON����				OFF����
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
	

/* �������ƣ�void Single_Check(void)
 * ���������������źż�� ���Ƿ����ź�����
 * ��    ������
 * �� �� ֵ��0�����ź�����	1�����ź�����
 * ��    ������Ҫͨ���ж�FFT֮�����Ƶ�ʶ�Ӧ�ķ�ֵ��û�����Եı任 ��������֮��
 * 					���л�Ϊʱ����ʾ�� �����Ա仯����ʾƵ�ס�
 */
uint8_t Single_Check(void)
{
	static uint8_t num_yes = 0;
	static uint8_t num_no = 0;
	static uint8_t cheeck_num = 0;	// ������
	uint8_t i;
	
	for(i=1;i<0xff;i++)	// ��1��ʼ ȥ��0���ֱ������
	{
		if(lBufMagArray[i] > 50)
		{
			// ��⵽���ź�
			break;
		}
	}
	if(i<0xff)
		num_yes ++;	// ��⵽���ź�
	else// û��⵽�ź�
		num_no ++;

	cheeck_num ++;
	if(cheeck_num==10)	// �����10��
	{
		cheeck_num = 0;
		if(num_yes>num_no)	
		{
			num_yes = 0;
			num_no = 0;
			return SINGLE_OK;	// ���ź�
		}
		else	
		{
			num_yes = 0;
			num_no = 0;
			return SINGLE_NOT;	// ���ź�
		}
	}
	else
		return SINGLE_CHECK;	// ���ڼ��
}

/* �������ƣ�display_pro
 * ������������ʾ������
 * ��������
 * ����ֵ����
 */
void display_pro(void)
{
	static uint16_t num = 0;
	
	// �����Զ���ʾ
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
	
	// ������ʾ��ʽ
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


/* �������ƣ�display1
 * ����������������ʾ, �м�Գ���ʾ
 * ��������
 * ����ֵ����
 */
void display1(void)
{
	uint16_t i=0, x=0, y=0;
	uint8_t temp = 0;
	
	/*******************��ʾ*******************/
	// �������
	GUI_ClearSCR();
	
	for(i = 1; i < 128; i++)	
	{
		temp = lBufMagArray[i]/prt; 
		if(temp > 63) temp = 63;	// ������ʾ��Χ
		
		x = 128-i;
		y = 63-temp;	
		GUI_Line(x, y, x, 63, 0x0f);
		
		//������ĵ�
		if(fal_pot[i]>y) fal_pot[i] = y;
		else
		{
				if(fal_pot[i]>62) fal_pot[i]=62;	// ��������һ��                                       
				GUI_Line(x, fal_pot[i], x,fal_pot[i]+1, 0xf);
				fal_pot[i] += 1 ;
		}
	}
	for(i = 1; i < 128; i++)	
	{
		temp = lBufMagArray[i]/prt; 
		if(temp > 63) temp = 63;	// ������ʾ��Χ
		
		x = 127+i;
		y = 63-temp;	
		GUI_Line(x, y, x, 63, 0x0f);
		
		//������ĵ�
		if(fal_pot[i]>y) fal_pot[i] = y;
		else
		{
				if(fal_pot[i]>62) fal_pot[i]=62;	// ��������һ��                                       
				GUI_Line(x, fal_pot[i], x,fal_pot[i]+1, 0xf);
				fal_pot[i] += 1 ;
		}
	}
	GUI_Exec();
}

/* �������ƣ�display2
 * ������������״��ʾ 2����λ��
 * ��������
 * ����ֵ����
 */
void display2(void)
{
	uint16_t i=0, x=0, y=0;
	uint8_t temp = 0;
	
	/*******************��ʾ*******************/
	// �������
	GUI_ClearSCR();

	for(i = 0; i < 64; i++)	//�����ȡ32��Ƶ�ʳ�����ʾ
	{
		
		temp = lBufMagArray[i+1]/prt;
		if(temp > 63) temp = 63;	// ������ʾ��Χ
		
		x = (i<<2);	//i*4
		y = 63-temp;	//��1��Ϊ�˶�����һ��ֱ������
		if(y>60) y = 60;
		
		GUI_LineWith(x, y, x, 63, 3, 0x0f);	// ��һ���п�ȵ���
		
		//������ĵ�
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

/* �������ƣ�display3
 * ������������ʱ��������
 * ��������
 * ����ֵ����
 */
void display3(void)
{
	uint16_t i=0, x=0, y=0;
	uint8_t temp = 0;
	uint8_t points[256];
	
	/*******************��ʾ*******************/
	// �������
	GUI_ClearSCR();
	// ʱ��
	for(i = 0; i < 128; i++)
	{
		points[2*i] = 2*i;
		points[2*i+1] = adc_buf[i]/128;
		if(points[2*i+1] > 32)	points[2*i+1] = 32;	// �������ϰ���Ļ
	}
	GUI_PointS(points, 128, 0x0f);

	// ����
	for(i = 0; i < 64; i++)	//�����ȡ32��Ƶ�ʳ�����ʾ
	{
		
		temp = lBufMagArray[i+1]/prt;
		if(temp > 63) temp = 63;	// ������ʾ��Χ
		
		x = (i<<2);	//i*4
		y = 63-temp;	
		if(y<32) y = 32;	// ���Ƶ��°���Ļ
		if(y>60) y = 60;
		GUI_LineWith(x, y, x, 63, 3, 0xf);	// ��һ���п�ȵ���
		
		//������ĵ�
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


///* �������ƣ�display7
// * ��������������
// * ��������
// * ����ֵ����
// */
//void display7(void)
//{
//	uint16_t i=0;
//	
//	/*******************��ʾ*******************/
//	// �������
//	GUI_ClearSCR();
//	for(i=0; i<128; i++)
//	{
//		GUI_Point(2*i, adc_buf[i]/100, 0x0f);
//		GUI_Point(2*i+1, adc_buf[i]/100, 0x0f);
//	}
//	GUI_Exec();
//}






