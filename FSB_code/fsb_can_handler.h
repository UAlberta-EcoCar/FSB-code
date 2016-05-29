#ifndef FSB_CAN_HANDLER_H
#define FSB_CAN_HANDLER_H

#include <rtc.h>
#include <Arduino.h>

class Can {
public:
  Can() {};
  void begin();

  void read();
  void send_throttle(uint16_t val);
  void send_brake(uint16_t val);
  void send_time(RTC_Time *now);

  //motor vars
  uint16_t speed;
  uint16_t throttle;
  uint16_t brake;
  uint16_t mcurrent;

  //fuel cell vars
  int32_t FC_VOLT;
  int32_t CAP_VOLT;
  int32_t FC_TEMP;
  uint16_t FC_ERROR;
  uint8_t FC_PURGE_COUNT;

  //aux/lcd
  bool horn;
  bool wipers;
  bool headlights;

  //"heart beat monitoring"
  bool aux_online;
  bool fc_online;
};


#endif
