/******************************************************************
 * �ļ����ƣ�fft.c
 * ��    �ߣ�mcli
 * ��    �ڣ�2019/2/28
 * ��    ����v1.0
 * ��    �������ļ�ΪFFT�㷨��ص�Դ���ADC���ݲɼ����Դ��
 ******************************************************************/

#include "fft.h"
#include "adc.h"
#include "tim.h"
#include "math.h"
#include "stm32_dsp.h"
#include "table_fft.h"

uint32_t adc_buf[NPT]={0};			//���ڴ洢ADCת�����������	

static long lBufInArray[NPT];					//�����FFT������
//long lBufOutArray[NPT/2];				//FFT��� ��Ϊ�������ǶԳƵ� ��������ֻȡ��ǰ���һ��
//long lBufMagArray[NPT/2];				//ÿ��Ƶ�ʶ��õķ�ֵ
static long lBufOutArray[NPT];				//FFT��� 
long lBufMagArray[NPT];				//ÿ��Ƶ�ʶ��õķ�ֵ

uint8_t adc_dma_ok = 0;					//adc��DMA������ɱ�־

/* �������ƣ�void FFT_Start(void)
 * ����������FFT���ݲɼ�������
 * ��������
 * ����ֵ����
 */
void FFT_Start(void)
{
	/*����ADC��DMA���䣬��϶�ʱ������ADCת��*/
	HAL_ADC_Start_DMA(&hadc1, adc_buf, NPT);
	/*������ʱ���������ʱ��������ADC*/
	HAL_TIM_Base_Start(&htim3);
}

/* �������ƣ�void FFT_Stop(void)
 * ����������FFT���ݲɼ���ֹͣ
 * ��������
 * ����ֵ����
 */
void FFT_Stop(void)
{
	/*ֹͣADC��DMA����*/
	HAL_ADC_Stop_DMA(&hadc1);
	/*ֹͣ��ʱ��*/
	HAL_TIM_Base_Stop(&htim3);
}
/* �������ƣ�GetPowerMag
 * ������������ȡFFT���ֱ������
 * ��������
 * ����ֵ����
 */
void GetPowerMag(void)
{
    signed short lX,lY;
    float X,Y,Mag;
    unsigned short i;
	
    for(i=0; i<NPT; i++)
    {
        lX  = (lBufOutArray[i] << 16) >> 16;
        lY  = (lBufOutArray[i] >> 16);
			
				//����32768�ٳ�65536��Ϊ�˷��ϸ������������
        X = NPT * ((float)lX) / 32768;
        Y = NPT * ((float)lY) / 32768;
        Mag = sqrt(X * X + Y * Y)*1.0/ NPT;
        if(i == 0)	
            lBufMagArray[i] = (unsigned long)(Mag * 32768);
        else
            lBufMagArray[i] = (unsigned long)(Mag * 65536);
    }
}

/* �������ƣ�void FFT_Pro(void)
 * ����������FFT������
 * ��������
 * ����ֵ����
 */
void FFT_Pro(void)
{
	uint16_t i = 0;
	//�������
	for(i=0;i<NPT;i++)
	{
		//������Ϊ��Ƭ����ADCֻ�ܲ����ĵ�ѹ ������Ҫǰ����ֱ��ƫִ
		//����ֱ��ƫִ�� 2.5V ��ӦADֵΪ3103
		lBufInArray[i] = ((signed short)(adc_buf[i])-3103) << 16;		
	}
	//256��FFT�任
	cr4_fft_256_stm32(lBufOutArray, lBufInArray, NPT);
	//�������ӦƵ�ʵ�ģ ��ÿ��Ƶ�ʶ�Ӧ�ķ�ֵ
	GetPowerMag();	
}

/* �������ƣ�void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
 * ����������ADC��DMA�ص�����
 * ������hadc --- adc�ṹ��ָ��
 * ����ֵ����
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
//	HAL_ADC_Stop_DMA(&hadc1);	//ֹͣADC��DMA����
	FFT_Stop();
	adc_dma_ok = 1;						//���ADC_DMA�������
}


