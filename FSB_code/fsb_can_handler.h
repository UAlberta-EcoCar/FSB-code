#ifndef FSB_CAN_HANDLER_H
#define FSB_CAN_HANDLER_H

#include <rtc.h>
#include <Arduino.h>

class Can {
public:
  Can() {};
  void begin();

  void read();
//  void send_throttle(uint16_t val);
//  void send_brake(uint16_t val);
//  void send_time(RTC_Time *now);

  uint16_t fc_error;
  uint8_t fc_state;
  uint8_t fc_purge_count;
  uint32_t fc_time_between_last_purges;
  uint32_t fc_energy_since_last_purge;
  uint32_t fc_total_energy;
  uint32_t fc_charge_since_last_purge;
  uint32_t fc_total_charge;
  int32_t fc_volt;
  int32_t fc_curr;
  uint8_t fc_temp;
  uint8_t fc_opttemp;
  int32_t fc_pres;
  int32_t fc_capvolt;
  int32_t fc_fan_speed;

  bool fc_start_relay;
  bool fc_res_relay;
  bool fc_cap_relay;
  bool fc_motor_relay;
  bool fc_purge_valve;
  bool fc_h2_valve;
  
  void send_throttle(uint16_t val);
  void send_brake(bool val);  
  void send_time(RTC_Time *now);

  //motor vars
  uint16_t speed;
  uint16_t throttle;
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
  bool brake;

};


#endif
