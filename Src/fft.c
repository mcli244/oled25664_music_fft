/******************************************************************
 * 文件名称：fft.c
 * 作    者：mcli
 * 日    期：2019/2/28
 * 版    本：v1.0
 * 描    述：本文件为FFT算法相关的源码和ADC数据采集相关源码
 ******************************************************************/

#include "fft.h"
#include "adc.h"
#include "tim.h"
#include "math.h"
#include "stm32_dsp.h"
#include "table_fft.h"

uint32_t adc_buf[NPT]={0};			//用于存储ADC转换结果的数组	

static long lBufInArray[NPT];					//传入给FFT的数组
//long lBufOutArray[NPT/2];				//FFT输出 因为输出结果是对称的 所以我们只取了前面的一半
//long lBufMagArray[NPT/2];				//每个频率对用的幅值
static long lBufOutArray[NPT];				//FFT输出 
long lBufMagArray[NPT];				//每个频率对用的幅值

uint8_t adc_dma_ok = 0;					//adc的DMA传输完成标志

/* 函数名称：void FFT_Start(void)
 * 功能描述：FFT数据采集的启动
 * 参数：无
 * 返回值：无
 */
void FFT_Start(void)
{
	/*启动ADC的DMA传输，配合定时器触发ADC转换*/
	HAL_ADC_Start_DMA(&hadc1, adc_buf, NPT);
	/*开启定时器，用溢出时间来触发ADC*/
	HAL_TIM_Base_Start(&htim3);
}

/* 函数名称：void FFT_Stop(void)
 * 功能描述：FFT数据采集的停止
 * 参数：无
 * 返回值：无
 */
void FFT_Stop(void)
{
	/*停止ADC的DMA传输*/
	HAL_ADC_Stop_DMA(&hadc1);
	/*停止定时器*/
	HAL_TIM_Base_Stop(&htim3);
}
/* 函数名称：GetPowerMag
 * 功能描述：获取FFT后的直流分量
 * 参数：无
 * 返回值：无
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
			
				//除以32768再乘65536是为了符合浮点数计算规律
        X = NPT * ((float)lX) / 32768;
        Y = NPT * ((float)lY) / 32768;
        Mag = sqrt(X * X + Y * Y)*1.0/ NPT;
        if(i == 0)	
            lBufMagArray[i] = (unsigned long)(Mag * 32768);
        else
            lBufMagArray[i] = (unsigned long)(Mag * 65536);
    }
}

/* 函数名称：void FFT_Pro(void)
 * 功能描述：FFT处理函数
 * 参数：无
 * 返回值：无
 */
void FFT_Pro(void)
{
	uint16_t i = 0;
	//填充数组
	for(i=0;i<NPT;i++)
	{
		//这里因为单片机的ADC只能测正的电压 所以需要前级加直流偏执
		//加入直流偏执后 2.5V 对应AD值为3103
		lBufInArray[i] = ((signed short)(adc_buf[i])-3103) << 16;		
	}
	//256点FFT变换
	cr4_fft_256_stm32(lBufOutArray, lBufInArray, NPT);
	//计算出对应频率的模 即每个频率对应的幅值
	GetPowerMag();	
}

/* 函数名称：void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
 * 功能描述：ADC的DMA回调函数
 * 参数：hadc --- adc结构体指针
 * 返回值：无
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
//	HAL_ADC_Stop_DMA(&hadc1);	//停止ADC的DMA传输
	FFT_Stop();
	adc_dma_ok = 1;						//标记ADC_DMA传输完成
}


