/***********************************************
 * �ļ����ƣ�key.c
 * ��    �ߣ�mcli
 * ��    �ڣ�2019/2/28
 * ��    ����v1.0
 * ��    �������ļ�Ϊ����ɨ����������Դ��
 ***********************************************/

#include "key.h"
#include "config.h"	// gui
#include "fft.h"
#include "adc.h"
#include "display.h"
#include "rtcpro.h"

// ���ͼ�� ���� ˳��ȡģ 5x7
static uint8_t pox[]={0x00,0x40,0x60,0x70,0x60,0x40,0x00};
struct SetInfo Set_info;	// ������صĽṹ�����
extern uint8_t display_num;	// ������main.c��

uint8_t display_mode_temp = 0;			// �����������ʱֵ ��Ȼ���������иı�������ģʽ ��û���˳� ����ʾ��һ��ģʽ��

/* �������ƣ�Key_Init
 * ������������ʼ�����ò���
 * ��������
 * ����ֵ����
 */
void Key_Init(void)
{
	// ��ʼ�� ���ò���
	Set_info.func_num = 0;
	Set_info.page = 0;
}


#define FFT_FUNC_NUM_MAX		4	// ���������ֵ
/* �������ƣ�Show_Set_FFT
 * ������������ʾ���ý���
 * ��������
 * ����ֵ����
 */
void Show_Set_FFT(void)
{
	GUI_ClearSCR();
	GUI_LoadPic(0, Set_info.func_num*8, pox, 5, 7);
	GUI_PutString(8, 0, "Fal_speed");	// �������ٶ�
	GUI_PutString(8, 8, "A Or M");
	GUI_PutString(8, 16, "Show_Mode");			// �˳�
	GUI_PutString(8, 24, "Exit");			// �˳�
	GUI_Exec();
}

void Func_FalSpeed(void)
{
	GUI_ClearSCR();
	GUI_PutString(60, 0, "Fal_speed Setting");	// �������ٶ�
	GUI_PutString(0, 16, "Val:");								// ��ֵ
	GUI_ShowNum(32, 16, fal_speed, 3);
	GUI_Exec();
}
void Func_ModeSet(void)
{
	GUI_ClearSCR();
	GUI_PutString(52, 0, "Mode Setting");		// �������ٶ�
	GUI_PutString(0, 16, "Val:");						// ��ֵ
	if(display_auto != 0)
		GUI_PutString(32, 16, "AUTO");			
	else
		GUI_PutString(32, 16, "M");		
	GUI_Exec();
}

void Func_ShowMode(void)
{
	GUI_ClearSCR();
	GUI_PutString(52, 0, "Show Mode");			// ��ʾģʽ
	GUI_PutString(0, 16, "Val:");						// ��ֵ
	if(display_mode_temp != 0)
		GUI_PutString(32, 16, "Time");			
	else
		GUI_PutString(32, 16, "FFT");		
	GUI_Exec();
}

void Func_Exit(void)
{
	// �������ҳ����ı�ʶ
	Set_info.page = 0;
	Set_info.func_num = 0;
	
	if(display_mode_temp != 0)
	{
		Current_display_mode = DISPLAY_MODE_TIME;	// ��ʾʱ��
		FFT_Start();	// ���¿�����ʱ������ADCת��	��ʾʱ���ʱ��ҲҪ��FFT �����źż��
	}
	else
	{
		Current_display_mode = DISPLAY_MODE_FFT;	// ��ʾpƵ��
		FFT_Start();	// ���¿�����ʱ������ADCת��
	}
}

/* �������ƣ�	Key_Scan
 * ����������	����ɨ�躯��
 * ������			��
 * ����ֵ��		�ް�������0���а������ذ���ֵ
 * ��ע�������ϵ�˿ӡ
 */
#define KEY1_PRESS	1
#define KEY2_PRESS	2
#define KEY3_PRESS	3

uint8_t Key_Scan(void)
{
	uint8_t key_val = 0;
	static uint8_t press_flag = 0;			//���±�־
	
	if((GPIOA->IDR&0xe0) != 0xe0) 		// ������Ӳ���ϼ������� �Ͳ��������ʱ������
	{
			key_val = (GPIOA->IDR&0xe0);	// ȡ������ֵ
			if(press_flag == 0)						// ��һ��ɨ��
			{
				press_flag = 1;								// ����Ѿ���������
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
	else	// û�а���������
			press_flag = 0;	// �����˾������־
	return 0;
}

/* �������ƣ�Key_Pro
 * ��������������������
 * ��������
 * ����ֵ����
 */



/* �������ƣ�void Key_fft(uint8_t key_val)
 * ��������������������
 * ��������
 * ����ֵ����
 */
void Key_fft(uint8_t key_val)
{
	switch(key_val)
	{
		case KEY1_PRESS:	// ���ܼ�
				if(Set_info.page == DISPLAY)
				{
					FFT_Stop();								// ֹͣFFT�Ĳɼ�
					Set_info.page = SETTING;	// ��������ҳ��
					Show_Set_FFT();								// ��ʾ���ý���
				}
				else if(Set_info.page == SETTING)
				{
					Set_info.page = SOMEONE;			// ��ʾ����ĳ���������ҳ����
					switch(Set_info.func_num)
					{
						case 0:
								Func_FalSpeed();	// ��ʾ�����ٶ�����ֵ
							break;
						case 1:
								Func_ModeSet();		// ��ʾģʽ���� �ֶ�/�Զ�
							break;
						case 2:
								Func_ShowMode();	// ��ʾʱ�仹��Ƶ������
							break;
						case 3:
								Func_Exit();	// �˳�
							break;
						default:
							break;
					}
				}
				else if(Set_info.page == SOMEONE)	// ��ʾ��һ�ΰ���ʱ���Ѿ������˹����� ������˳����������
				{
					Set_info.page = SETTING;
					Show_Set_FFT();								// ��ʾ���ý���
				}
			break;
		case KEY2_PRESS:	// +
				if(Set_info.page == DISPLAY)	// ������ʾҳ��
				{
					display_num ++;
					if(display_num > DISPLAY_FUNC_NUM) display_num = 0;
				}
				else if(Set_info.page == SETTING)	// ����ҳ��
				{
					Set_info.func_num++;
					if(Set_info.func_num > FFT_FUNC_NUM_MAX-1)	Set_info.func_num = 0;
					Show_Set_FFT();								// ��ʾ���ý���
				}
				else if(Set_info.page == SOMEONE)	// ����ĳ��
				{
					switch(Set_info.func_num)
					{
						case 0:	// Func_FalSpeed
							fal_speed++;
							if(fal_speed>10)	fal_speed = 0;
							Func_FalSpeed();	// ��ʾ�����ٶ�����ֵ
							break;
						case 1:	// Func_ModeSet
							display_auto = !display_auto;
							Func_ModeSet();	// ˢ����ʾ
							break;
						case 2:	// Func_ShowMode
							display_mode_temp = !display_mode_temp;
							Func_ShowMode();	// ˢ����ʾ
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
					Show_Set_FFT();								// ��ʾ���ý���
				}
				else if(Set_info.page == SOMEONE)	// ����ĳ��
				{
					switch(Set_info.func_num)
					{
						case 0:	// Func_FalSpeed
							if(fal_speed==0)	fal_speed = 11;
							fal_speed--;
							Func_FalSpeed();	// ��ʾ�����ٶ�����ֵ
							break;
						case 1:	// Func_ModeSet
							display_auto = !display_auto;	
							Func_ModeSet();	// ˢ����ʾ
							break;
						case 2:	// Func_ShowMode
							display_mode_temp = !display_mode_temp;
							Func_ShowMode();	// ˢ����ʾ
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



#define TIME_FUNC_NUM_MAX		8	// ���������ֵ
/* �������ƣ�Show_Set_FFT
 * ������������ʾ���ý���
 * ��������
 * ����ֵ����
 */
void Show_Set_Time(void)
{
	GUI_ClearSCR();
	GUI_LoadPic(0, Set_info.func_num*8, pox, 5, 7);
	GUI_PutString(8, 0, "Year");			// ��
	GUI_PutString(8, 8, "Month");			// ��
	GUI_PutString(8, 16, "Date");			// ��
	GUI_PutString(8, 24, "Hours");		// Сʱ
	GUI_PutString(8, 32, "Minutes");	// ����
	GUI_PutString(8, 40, "Seconds");	// ��
	GUI_PutString(8, 48, "Show_Mode");		// ��ʾģʽ����
	GUI_PutString(8, 56, "Exit");			// �˳�
	GUI_Exec();
}

void Func_Year(void)
{
	GUI_ClearSCR();
	GUI_PutString(60, 0, "Year Setting");	// �������ٶ�
	GUI_PutString(0, 16, "Val:");								// ��ֵ
	GUI_ShowNum(32, 16, date.Year, 4);
	GUI_Exec();
}
void Func_Month(void)
{
	GUI_ClearSCR();
	GUI_PutString(60, 0, "Month Setting");	// �������ٶ�
	GUI_PutString(0, 16, "Val:");								// ��ֵ
	GUI_ShowNum(32, 16, date.Month, 2);
	GUI_Exec();
}
void Func_Date(void)
{
	GUI_ClearSCR();
	GUI_PutString(60, 0, "Date Setting");	// �������ٶ�
	GUI_PutString(0, 16, "Val:");								// ��ֵ
	GUI_ShowNum(32, 16, date.Date, 2);
	GUI_Exec();
}
void Func_Hours(void)
{
	GUI_ClearSCR();
	GUI_PutString(60, 0, "Hours Setting");	// �������ٶ�
	GUI_PutString(0, 16, "Val:");								// ��ֵ
	GUI_ShowNum(32, 16, time.Hours, 2);
	GUI_Exec();
}
void Func_Minutes(void)
{
	GUI_ClearSCR();
	GUI_PutString(60, 0, "Minutes Setting");	// �������ٶ�
	GUI_PutString(0, 16, "Val:");								// ��ֵ
	GUI_ShowNum(32, 16, time.Minutes, 2);
	GUI_Exec();
}
void Func_Seconds(void)
{
	GUI_ClearSCR();
	GUI_PutString(60, 0, "Seconds Setting");	// �������ٶ�
	GUI_PutString(0, 16, "Val:");								// ��ֵ
	GUI_ShowNum(32, 16, time.Seconds, 2);
	GUI_Exec();
}
void Func_ShowMode_Time(void)
{
	GUI_ClearSCR();
	GUI_PutString(60, 0, "ShowMode Setting");	// �������ٶ�
	GUI_PutString(0, 16, "Val:");								// ��ֵ
	if(display_mode_temp != 0)
		GUI_PutString(32, 16, "Time");			
	else
		GUI_PutString(32, 16, "FFT");		
	GUI_Exec();
}
void Func_Exit_Time(void)
{
	// �������ҳ����ı�ʶ
	Set_info.page = 0;
	Set_info.func_num = 0;
	
	if(display_mode_temp != 0)
	{
		Current_display_mode = DISPLAY_MODE_TIME;	// ��ʾʱ��
		// ���޸��˵�ʱ�����ý�ȥ
		RTC_Set(date.Year, date.Month, date.Date,
		time.Hours, time.Minutes, time.Seconds);	
		
	}
	else
	{
		Current_display_mode = DISPLAY_MODE_FFT;	// ��ʾpƵ��
		FFT_Start();	// ���¿�����ʱ������ADCת��
	}
}
/* �������ƣ�void Key_time(uint8_t key_val)
 * ��������������ʾʱ��ʱ�� ���°����Ĵ�����
 * ������key_val   ����ֵ
 * ����ֵ����
 */
void Key_time(uint8_t key_val)
{
	switch(key_val)
	{
		case KEY1_PRESS:	// ���ܼ�
				if(Set_info.page == DISPLAY)
				{
					Set_info.page = SETTING;				// ��������ҳ��
					RTC_Get();											// ��ȡʱ��
					Show_Set_Time();								// ��ʾ���ý���
				}
				else if(Set_info.page == SETTING)
				{
					Set_info.page = SOMEONE;			// ��ʾ����ĳ���������ҳ����
					switch(Set_info.func_num)
					{
						case 0:	
								Func_Year();		// ������
							break;
						case 1:
								Func_Month();		// ������
							break;
						case 2:
								Func_Date();		// ��������
							break;
						case 3:
								Func_Hours();		// ����Сʱ
							break;
						case 4:
								Func_Minutes();		// ���÷���
							break;
						case 5:
								Func_Seconds();		// ��������
							break;
						case 6:
								Func_ShowMode_Time();		// ����ģʽ
							break;
						case 7:
								Func_Exit_Time();		// �˳�
							break;
						default:
							break;
					}
				}
				else if(Set_info.page == SOMEONE)	// ��ʾ��һ�ΰ���ʱ���Ѿ������˹����� ������˳����������
				{
					Set_info.page = SETTING;
					Show_Set_Time();								// ��ʾ���ý���
				}
			break;
		case KEY2_PRESS:	// +
				if(Set_info.page == DISPLAY)	// ������ʾҳ��
				{
					
				}
				else if(Set_info.page == SETTING)	// ����ҳ��
				{
					Set_info.func_num++;
					if(Set_info.func_num > TIME_FUNC_NUM_MAX-1)	Set_info.func_num = 0;
					Show_Set_Time();								// ��ʾ���ý���
				}
				else if(Set_info.page == SOMEONE)	// ����ĳ��
				{
					switch(Set_info.func_num)
					{
						case 0:	// Func_Year
							date.Year++;
							if(date.Year>2099)	date.Year = 1970;
							Func_Year();	// ��ʾ���õ�ֵ
							break;
						case 1:	// Func_Month
							date.Month++;
							if(date.Month>12)	date.Month = 0;
							Func_Month();	// ��ʾ���õ�ֵ
							break;
						case 2:	// 
							date.Date++;
							if(date.Date>31)	date.Date = 0;
							Func_Date();	// ��ʾ���õ�ֵ
							break;
						case 3:	// 
							time.Hours++;
							if(time.Hours>24)	time.Hours = 0;
							Func_Hours();	// ��ʾ���õ�ֵ
							break;
						case 4:	// 
							time.Minutes++;
							if(time.Minutes>59)	time.Minutes = 0;
							Func_Minutes();	// ��ʾ���õ�ֵ
							break;
						case 5:	// 
							time.Seconds++;
							if(time.Seconds>59)	time.Seconds = 0;
							Func_Seconds();	// ��ʾ���õ�ֵ
							break;
						case 6:	// 
							display_mode_temp = !display_mode_temp;
							Func_ShowMode_Time();	// ��ʾ���õ�ֵ
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
					Show_Set_Time();								// ��ʾ���ý���
				}
				else if(Set_info.page == SOMEONE)	// ����ĳ��
				{
					switch(Set_info.func_num)
					{
						case 0:	// Func_Year
							if(date.Year<1970)	date.Year = 2100;
							date.Year--;
							Func_Year();	// ��ʾ���õ�ֵ
							break;
						case 1:	// Func_Month
							if(date.Month == 0)	date.Month = 13;
							date.Month--;
							Func_Month();	// ��ʾ���õ�ֵ
							break;
						case 2:	// 
							if(date.Date == 0)	date.Date = 32;
							date.Date--;
							Func_Date();	// ��ʾ���õ�ֵ
							break;
						case 3:	// 
							if(time.Hours == 0)	time.Hours = 25;
							time.Hours--;
							Func_Hours();	// ��ʾ���õ�ֵ
							break;
						case 4:	// 
							if(time.Minutes == 0)	time.Minutes = 60;
							time.Minutes--;
							Func_Minutes();	// ��ʾ���õ�ֵ
							break;
						case 5:	// 
							if(time.Seconds == 0)	time.Seconds = 60;
							time.Seconds--;
							Func_Seconds();	// ��ʾ���õ�ֵ
							break;
						case 6:	// 
							display_mode_temp = !display_mode_temp;
							Func_ShowMode_Time();	// ��ʾ���õ�ֵ
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



/* �������ƣ�Key_Pro
 * ��������������������
 * ��������
 * ����ֵ����
 */
void Key_Pro(void)
{
	uint8_t key_val = 0;
	
	// ����ɨ��
	key_val = Key_Scan();
	
	// �а�������
	if(0 != key_val)
	{
		// 2����� 
		// 1.����ʾƵ��ʱ����
		if(Current_display_mode == DISPLAY_MODE_FFT)
		{
			Key_fft(key_val);	// ��ת��FFT�������
		}
		// 2.��ʾʱ���ʱ�򱻰���
		else if(Current_display_mode == DISPLAY_MODE_TIME)
		{
			Key_time(key_val);	// ��ת��Time�������
		}
	}
}

