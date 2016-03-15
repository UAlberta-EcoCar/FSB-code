//this is going to be a massive file for recieving a ton for data off of can bus
#include <stdio.h>
#include "can_message.h"
#include "can_message_def.h"
#include "fsb_can_handler.h"
#include <mcp2515_lib.h>
#include <Arduino.h>
#include <mcp2515_filters.h>
#include "rtc.h"

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


//Starts can bus
void fsb_can_init(void)
{
    while(can_init(0,0,0,0,0,0,0,0))
    {
        delay(100);
    }
}

//FUNCTIONS FOR SENDING VALUES OVER CAN BUS
void send_throttle(uint16_t val) {
  // send throttle value
  CanMessage msg;
  msg.id = throttle_msg.id();
  msg.length = 2;
  throttle_msg.buf(msg.data, val);
  while(can_send_message(&msg));
}
void send_brake(uint16_t val) {
  // send brake value
  CanMessage msg;
  msg.id = brake_msg.id();
  msg.length = brake_msg.len();
  brake_msg.buf(msg.data, val);
  while(can_send_message(&msg));
}

void send_can_time(RTC_Time *now)
{
  CanMessage msg;
  msg.id = can_time_msg.id();
  msg.length = can_time_msg.len();
  can_time_msg.buf(msg.data,((now->year << can_msg::YEAR)|(now->month << can_msg::MONTH)|(now->monthDay << can_msg::DAY)|(now->hour << can_msg::HOUR)|(now->minute << can_msg::MINUTE)|(now->second << can_msg::SECOND)));
  while(can_send_message(&msg));
}

//DEFINE VARIABLES TO BE RECEIVED OVER CAN BUS AND FUNCTIONS TO ACCESS THEM



//FUNCTION FOR FILTERING THROUGH ALL THE MESSAGE ID's AND COLLECTING DATA
void read_can_bus(void)
{
  CanMessage message;
  message = can_get_message();
  //filter through message ID's
}
