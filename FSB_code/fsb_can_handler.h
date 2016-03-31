#ifndef FSB_CAN_HANDLER_H
#define FSB_CAN_HANDLER_H

#include "rtc.h"

void fsb_can_init(void);

void read_can_bus(void);

void send_throttle(uint16_t val);
void send_brake(uint16_t val);
void send_can_time(RTC_Time *now);

#endif
