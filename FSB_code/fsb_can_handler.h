#ifndef FSB_CAN_HANDLER_H
#define FSB_CAN_HANDLER_H

#include <rtc.h>
#include <Arduino.h>

/** CAN Bus Class **/

class Can {
public:
  Can() {};
  /** Start MCP2515 CHIP **/
  char begin();

  /** Read a message from MCP2515 chip **/
  void read();

  /** Fuel Cell Variables Recieved Over CAN Bus **/
  uint16_t fc_error;
  uint8_t fc_state;
  uint8_t fc_purge_count;
  uint32_t fc_time_between_last_purges;
  uint32_t fc_energy_since_last_purge;
  uint32_t fc_total_energy;
  uint32_t fc_charge_since_last_purge;
  uint32_t fc_total_charge;
  int16_t fc_volt;
  int16_t fc_curr;
  uint8_t fc_temp;
  uint8_t fc_opttemp;
  int16_t fc_pres;
  int16_t fc_capvolt;
  int16_t fc_fan_speed;

  bool fc_start_relay;
  bool fc_res_relay;
  bool fc_cap_relay;
  bool fc_motor_relay;
  bool fc_purge_valve;
  bool fc_h2_valve;


  /** Function for sending rtc time **/
  void send_time(RTC_Time *now);


};


#endif
