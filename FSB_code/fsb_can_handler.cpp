//this is going to be a massive file for recieving a ton for data off of can bus
#include <stdio.h>
#include "can_message.h"
#include "can_message_def.h"
#include "fsb_can_handler.h"
#include <mcp2515_lib.h>
#include <Arduino.h>
#include <mcp2515_filters.h>
#include "rtc.h"

//LCD Horn
can_msg::MsgEncode lcd_horn_msg( can_msg::BOOL, can_msg::AUX, can_msg::HORN, can_msg::IMPORTANT, 1);

//LCD WIpers
can_msg::MsgEncode lcd_wipers_msg( can_msg::BOOL, can_msg::AUX, can_msg::WIPERS, can_msg::IMPORTANT, 1);

//LCD Signals
can_msg::MsgEncode lcd_signals_msg( can_msg::BOOL, can_msg::AUX, can_msg::SIGNAL, can_msg::IMPORTANT, 3);

//LCD Headlight
can_msg::MsgEncode lcd_headlights_msg( can_msg::BOOL, can_msg::AUX, can_msg::HEADLIGHTS, can_msg::IMPORTANT, 1);


//motor message defines
can_msg::MsgEncode throttle_msg( can_msg::UINT16, can_msg::MOTOR, can_msg::THROTTLE, can_msg::CRITICAL, 1 );
can_msg::MsgEncode brake_msg( can_msg::UINT16, can_msg::MOTOR, can_msg::BRAKE, can_msg::CRITICAL, 1 );
can_msg::MsgEncode merror_msg( can_msg::BOOL, can_msg::MOTOR, can_msg::MERROR, can_msg::IMPORTANT, 2 );
can_msg::MsgEncode mspeed_msg( can_msg::INT16, can_msg::MOTOR, can_msg::MSPEED, can_msg::INFORMATION, 1 );
can_msg::MsgEncode mcurrent_msg( can_msg::INT16, can_msg::MOTOR, can_msg::MCURRENT, can_msg::INFORMATION, 1 );

//fuel cell message defines
//FC_ERROR
can_msg::MsgEncode fc_error_msg( can_msg::UINT16, can_msg::FUEL_CELL, can_msg::FC_ERROR, can_msg::CRITICAL, 1 );
//FC_STATE
can_msg::MsgEncode fc_state_msg( can_msg::UINT8, can_msg::FUEL_CELL, can_msg::FC_STATE, can_msg::IMPORTANT, 1 );
//FC_PURGE_COUNT
can_msg::MsgEncode fc_purge_count_msg( can_msg::UINT8, can_msg::FUEL_CELL, can_msg::FC_PURGE_COUNT, can_msg::LOGGING, 1 );
//FC_TIME_BETWEEN_LAST_PURGES
can_msg::MsgEncode fc_time_between_last_purges_msg( can_msg::UINT32, can_msg::FUEL_CELL, can_msg::FC_TIME_BETWEEN_LAST_PURGES, can_msg::LOGGING, 1 );
//FC_ENERGY
can_msg::MsgEncode fc_energy_msg( can_msg::UINT32, can_msg::FUEL_CELL, can_msg::FC_ENERGY, can_msg::LOGGING, 2 );
//FC_CHARGE
can_msg::MsgEncode fc_charge_msg( can_msg::UINT32, can_msg::FUEL_CELL, can_msg::FC_CHARGE, can_msg::LOGGING, 2);
//FCVOLT
can_msg::MsgEncode fc_volt_msg( can_msg::INT32, can_msg::FUEL_CELL, can_msg::FC_VOLT, can_msg::INFORMATION, 1);
//FCCURR
can_msg::MsgEncode fc_curr_msg( can_msg::INT32, can_msg::FUEL_CELL, can_msg::FC_CURR, can_msg::INFORMATION, 1);
//FCTEMP
can_msg::MsgEncode fc_temp_msg( can_msg::INT32, can_msg::FUEL_CELL, can_msg::FC_TEMP, can_msg::INFORMATION, 1);
//FCPRES
can_msg::MsgEncode fc_pres_msg( can_msg::INT32, can_msg::FUEL_CELL, can_msg::FC_PRES, can_msg::INFORMATION, 1);
//CAPVOLT
can_msg::MsgEncode fc_capvolt_msg( can_msg::INT32, can_msg::FUEL_CELL, can_msg::FC_CAPVOLT, can_msg::INFORMATION, 1);
//FC_FAN_SPEED
can_msg::MsgEncode fc_fan_speed_msg( can_msg::INT32, can_msg::FUEL_CELL, can_msg::FC_FAN_SPEED, can_msg::INFORMATION, 1);
//OUTPUTS
can_msg::MsgEncode fc_outputs_msg( can_msg::BOOL, can_msg::FUEL_CELL, can_msg::FC_OUTPUTS, can_msg::INFORMATION, 6);

//Time
can_msg::MsgEncode can_time_msg( can_msg::UINT8, can_msg::OTHER, can_msg::TIME, can_msg::INFORMATION, 6);


  //fuel cell vars
  int32_t FC_VOLT;
  int32_t CAP_VOLT;
  uint16_t FC_ERROR;
  int32_t FC_TEMP;
  uint8_t FC_PURGE_COUNT;

  //aux/lcd
  bool horn;
  bool wipers;
  bool headlights;

  //motor
  uint16_t throttle;
  uint16_t brake;
  uint16_t speed;
  uint16_t mcurrent;

  //"heart beat monitoring"
  bool aux_online;
  bool fc_online;


//Starts can bus
void Can::begin(void)
{
    can_init(0,0,0,0,0,0,0,0);
}

//FUNCTIONS FOR SENDING VALUES OVER CAN BUS
void Can::send_throttle(uint16_t val) {
  // send throttle value
  throttle = val;
  CanMessage msg;
  msg.id = throttle_msg.id();
  msg.length = throttle_msg.len();
  throttle_msg.buf(msg.data, val);
  can_send_message(&msg);
}
void Can::send_brake(uint16_t val) {
  // send brake value
  brake = val;
  CanMessage msg;
  msg.id = brake_msg.id();
  msg.length = brake_msg.len();
  brake_msg.buf(msg.data, val);
  can_send_message(&msg);
}

void Can::send_time(RTC_Time *now)
{
  CanMessage msg;
  msg.id = can_time_msg.id();
  msg.length = can_time_msg.len();
  msg.data[can_msg::YEAR] = now->year;
  msg.data[can_msg::MONTH] = now->month;
  msg.data[can_msg::DAY] = now->monthDay;
  msg.data[can_msg::HOUR] = now->hour;
  msg.data[can_msg::MINUTE] = now->minute;
  msg.data[can_msg::SECOND] = now->second;
  can_send_message(&msg);
}



//FUNCTION FOR FILTERING THROUGH ALL THE MESSAGE ID's AND COLLECTING DATA
void Can::read(void)
{
  CanMessage message;
  message = can_get_message();
  //filter through message ID's
  if(message.id == lcd_horn_msg.id())
  {
    if(message.data[0])
    {
      horn = 1;
    }
    else
    {
      horn = 0;
    }
  }
  else if(message.id == lcd_wipers_msg.id())
  {
    if(message.data[0])
    {
      wipers = 1;
    }
    else
    {
      wipers = 0;
    }
  }
  else if(message.id == lcd_headlights_msg.id())
  {
    if(message.data[0])
    {
      headlights = 1;
    }
    else
    {
      headlights = 0;
    }
  }
  else if(message.id == lcd_signals_msg.id())
  {
    
  }
  else if(message.id == brake_msg.id())
  {
    //Serial.println("BRAKE");
  }
  else if(message.id == throttle_msg.id())
  {
    //Serial.println("THROTTLE");
  }
  else if(message.id == mspeed_msg.id())
  {
    speed = message.data[0] | (message.data[1] << 8);
  }
  else if( message.id == mcurrent_msg.id())
  {
    mcurrent = message.data[0] | (message.data[1] << 8);
  }
  
  else if(message.id == fc_volt_msg.id())
  {
    FC_VOLT = message.data[0] | (message.data[1] << 8) | (message.data[2] << 16) | (message.data[3] << 24);
  }
  else if(message.id == fc_capvolt_msg.id())
  {
    CAP_VOLT = message.data[0] | (message.data[1] << 8) | (message.data[2] << 16) | (message.data[3] << 24);
  }
  else if (message.id == fc_temp_msg.id())
  {
    FC_TEMP = message.data[0] | (message.data[1] << 8) | (message.data[2] << 16) | (message.data[3] << 24);
  }
  else if (message.id == fc_purge_count_msg.id())
  {
    FC_PURGE_COUNT = message.data[0];
  }
  else if (message.id != 0)
  {
    //Serial.println(message.id);
  }
}
