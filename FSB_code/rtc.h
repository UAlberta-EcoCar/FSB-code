#ifndef RTC_H
#define RTC_H


void connect_to_rtc(void);

//structure for time
typedef struct
{
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
  uint8_t weekDay;
  uint8_t monthDay;
  uint8_t month;
  uint8_t year;
} RTC_Time; 

RTC_Time getTime(void);

#endif
