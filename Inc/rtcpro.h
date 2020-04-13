#ifndef __RTCPRO_H__
#define __RTCPRO_H__
#include "stm32f1xx_hal.h"

typedef struct
{
  uint8_t Hours;         
  uint8_t Minutes; 
  uint8_t Seconds;         
}my_RTC_Time; 

typedef struct
{
  uint8_t WeekDay;
  uint8_t Month; 
  uint8_t Date; 
  uint16_t Year;            
}my_RTC_Date; 

extern my_RTC_Time time; // 时间
extern my_RTC_Date date;	// 日期

void RTC_Display(void);
void RTC_Init(void);

uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day);
uint8_t RTC_Set(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec);
void RTC_Get(void);
#endif
