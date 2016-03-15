#ifndef RTC_H
#define RTC_H


void connect_to_rtc(void);

//structure for time
typedef struct
{
  int second;
  int minute;
  int hour;
  int weekDay;
  int monthDay;
  int month;
  int year;
} RTC_Time; 

RTC_Time getTime(void);

#endif
