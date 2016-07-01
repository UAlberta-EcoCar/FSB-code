//Message defines for can bus messages
//make sure it is the same as defines in other files
#include <stdio.h>
#include "can_message.h"
#include "can_message_def.h"
#include "fsb_can_handler.h"
#include <mcp2515_lib.h>
#include <Arduino.h>
#include <mcp2515_filters.h>
#include <rtc.h>

/** Steering Wheel **/

//LCD Horn
can_msg::MsgEncode lcd_horn_msg( can_msg::BOOL, can_msg::AUX, can_msg::HORN, can_msg::IMPORTANT, 1);
//LCD WIpers
can_msg::MsgEncode lcd_wipers_msg( can_msg::BOOL, can_msg::AUX, can_msg::WIPERS, can_msg::IMPORTANT, 1);
//LCD Signals
can_msg::MsgEncode lcd_signals_msg( can_msg::BOOL, can_msg::AUX, can_msg::SIGNAL, can_msg::IMPORTANT, 3);
//LCD Headlight
can_msg::MsgEncode lcd_headlights_msg( can_msg::BOOL, can_msg::AUX, can_msg::HEADLIGHTS, can_msg::IMPORTANT, 1);


/** Motor **/

//motor message defines
can_msg::MsgEncode throttle_msg( can_msg::UINT16, can_msg::MOTOR, can_msg::THROTTLE, can_msg::CRITICAL, 1 );
//can_msg::MsgEncode brake_msg( can_msg::BOOL, can_msg::MOTOR, can_msg::BRAKE, can_msg::CRITICAL, 1 );
can_msg::MsgEncode merror_msg( can_msg::BOOL, can_msg::MOTOR, can_msg::MERROR, can_msg::IMPORTANT, 2 );
can_msg::MsgEncode mspeed_msg( can_msg::INT16, can_msg::MOTOR, can_msg::MSPEED, can_msg::INFORMATION, 1 );
can_msg::MsgEncode mcurrent_msg( can_msg::INT16, can_msg::MOTOR, can_msg::MCURRENT, can_msg::INFORMATION, 1 );


/** Fuel Cell **/

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
can_msg::MsgEncode fc_temp_msg( can_msg::UINT8, can_msg::FUEL_CELL, can_msg::FC_TEMP, can_msg::INFORMATION, 2);
//FCPRES
can_msg::MsgEncode fc_pres_msg( can_msg::INT32, can_msg::FUEL_CELL, can_msg::FC_PRES, can_msg::INFORMATION, 1);
//CAPVOLT
can_msg::MsgEncode fc_capvolt_msg( can_msg::INT32, can_msg::FUEL_CELL, can_msg::FC_CAPVOLT, can_msg::INFORMATION, 1);
//FC_FAN_SPEED
can_msg::MsgEncode fc_fan_speed_msg( can_msg::INT32, can_msg::FUEL_CELL, can_msg::FC_FAN_SPEED, can_msg::INFORMATION, 1);
//OUTPUTS
can_msg::MsgEncode fc_outputs_msg( can_msg::BOOL, can_msg::FUEL_CELL, can_msg::FC_OUTPUTS, can_msg::INFORMATION, 6);

/** OTHER **/

//Time
can_msg::MsgEncode can_time_msg( can_msg::UINT8, can_msg::OTHER, can_msg::TIME, can_msg::INFORMATION, 6);


//Starts can bus
char Can::begin(void)
{
    return(can_init(0,0,0,0,0,0,0,0));
}


//FUNCTION FOR FILTERING THROUGH ALL THE MESSAGE ID's AND COLLECTING DATA
void Can::read(void)
{
  CanMessage message;
  message = can_get_message();
 
  //filter through message ID's
  if(message.id != 0)
  { 
    if (message.id == fc_error_msg.id())
    {
      fc_error = message.data[0] | (message.data[1] << 8);
    }
    else if (message.id == fc_state_msg.id())
    {
      fc_state = message.data[0];
    }
    else if (message.id == fc_purge_count_msg.id())
    {
      fc_purge_count = message.data[0];
    }
    else if (message.id == fc_time_between_last_purges_msg.id())
    {
      fc_time_between_last_purges = message.data[0] | (message.data[1] << 8) | (message.data[2] << 16) | (message.data[3] << 24);
    }
    else if (message.id == fc_energy_msg.id())
    {
      fc_energy_since_last_purge = message.data[4] | (message.data[5] << 8) | (message.data[6] << 16) | (message.data[7] << 24);
      fc_total_energy = message.data[0] | (message.data[1] << 8) | (message.data[2] << 16) | (message.data[3] << 24);
    }
    else if (message.id == fc_charge_msg.id())
    {
      fc_charge_since_last_purge = message.data[4] | (message.data[5] << 8) | (message.data[6] << 16) | (message.data[7] << 24);
      fc_total_charge = message.data[0] | (message.data[1] << 8) | (message.data[2] << 16) | (message.data[3] << 24);
    }
    else if (message.id == fc_volt_msg.id())
    {
      fc_volt = message.data[0] | (message.data[1] << 8) | (message.data[2] << 16) | (message.data[3] << 24);
    }
    else if (message.id == fc_curr_msg.id())
    {
      fc_curr =  message.data[0] | (message.data[1] << 8) | (message.data[2] << 16) | (message.data[3] << 24);
    }
    else if (message.id == fc_temp_msg.id())
    {
      fc_temp = message.data[0];
      fc_opttemp = message.data[1];
    }
    else if (message.id == fc_pres_msg.id())
    {
      fc_pres = message.data[0] | (message.data[1] << 8) | (message.data[2] << 16) | (message.data[3] << 24);
    }
    else if (message.id == fc_capvolt_msg.id())
    {
      fc_capvolt = message.data[0] | (message.data[1] << 8) | (message.data[2] << 16) | (message.data[3] << 24);
    }
    else if (message.id == fc_fan_speed_msg.id())
    {
      fc_fan_speed = message.data[0] | (message.data[1] << 8) | (message.data[2] << 16) | (message.data[3] << 24);
    }
    else if (message.id == fc_outputs_msg.id())
    {
      fc_start_relay = message.data[0] & (1 << can_msg::FC_START_RELAY);
      fc_res_relay = message.data[0] & (1 << can_msg::FC_RES_RELAY);
      fc_cap_relay = message.data[0] & (1 << can_msg::FC_CAP_RELAY);
      fc_motor_relay = message.data[0] & (1 << can_msg::FC_MOTOR_RELAY);
      fc_purge_valve = message.data[0] & (1 << can_msg::FC_PURGE_VALVE);
      fc_h2_valve = message.data[0] & (1 << can_msg::FC_H2_VALVE);
    }
  }
}
