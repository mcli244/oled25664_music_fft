#ifndef __FFT_H__
#define __FFT_H__
#include "stm32f1xx_hal.h"

#define NPT 256									//样本数量
#define PI2 6.28318530717959
//采样率计算
//分辨率：Fs/NPT 
//#define Fs	10000							//FS FFT计算的频率 这个频率和ADC的采样频率绑定起来
#define Fs	9984								//取9984能出来整数的分辨率 9984/256 = 39Hz

extern uint8_t adc_dma_ok;			//adc的DMA传输完成标志
extern long lBufMagArray[NPT];	//每个频率对用的幅值
extern uint32_t adc_buf[NPT];		//adc原始数据

void FFT_Start(void);
void FFT_Stop(void);
void FFT_Pro(void);
#endif


