#ifndef __FFT_H__
#define __FFT_H__
#include "stm32f1xx_hal.h"

#define NPT 256									//��������
#define PI2 6.28318530717959
//�����ʼ���
//�ֱ��ʣ�Fs/NPT 
//#define Fs	10000							//FS FFT�����Ƶ�� ���Ƶ�ʺ�ADC�Ĳ���Ƶ�ʰ�����
#define Fs	9984								//ȡ9984�ܳ��������ķֱ��� 9984/256 = 39Hz

extern uint8_t adc_dma_ok;			//adc��DMA������ɱ�־
extern long lBufMagArray[NPT];	//ÿ��Ƶ�ʶ��õķ�ֵ
extern uint32_t adc_buf[NPT];		//adcԭʼ����

void FFT_Start(void);
void FFT_Stop(void);
void FFT_Pro(void);
#endif


