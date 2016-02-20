//I quit attempting to can the FCC can bus module to work
//instead, this sketch will link the CAN-BUS to the FCC usart module

//to do:
//rumman's new message id system
//implement two way communication 

#include <MemoryFree.h>
#include <mcp2515_lib.h>
#include <fc_can_messages.h>
#include "FC_USART_CAN_BRIDGE_LIB.h"


const int CAN_STATUS_LED = 2;
const int MSG_STATUS_LED  = 3;

void setup() {
  pinMode(CAN_STATUS_LED,OUTPUT);
  pinMode(MSG_STATUS_LED,OUTPUT);
  
  // Start Serial Communication
  Serial.begin(115200);
  while (!Serial);
  Serial.setTimeout(500);

  //start CAN-bus
  while(can_init());
  digitalWrite(CAN_STATUS_LED,HIGH);
  
  // Short delay and then begin listening
  delay(2000);
}

unsigned char index;
long val;

String dataString;

void loop() 
{
    digitalWrite(MSG_STATUS_LED,LOW);
    dataString = "";
    dataString = Serial.readStringUntil('\n'); //note this has to be single quotes
    //it is a character -> double " is a string

    if(dataString == "")
    {
      
    }
    else
    {
      digitalWrite(MSG_STATUS_LED,HIGH);
      //run through everything and send over CAN-BUS

      //FC_ERROR
      val = parse_csv(FC_ERROR_CSV,dataString);
      send_fc_error((unsigned int)val);
    
      //FC_STATE
      val = parse_csv(FC_STATE_CSV,dataString);
      send_fc_state((unsigned char)val);

      delay(1);

      //PURGE_COUNT
      val = parse_csv(FC_PURGE_COUNT_CSV,dataString);
      send_fc_purge_count((unsigned char)val);
      
      //TIME_BETWEEN_LAST_PURGES
      val = parse_csv(FC_TIME_BETWEEN_LAST_PURGES_CSV,dataString);
      send_fc_time_between_last_purges((uint32_t) val);

      delay(1);

      //ENERGY_SINCE_LAST_PURGE
      val = parse_csv(FC_ENERGY_SINCE_LAST_PURGE_CSV,dataString);
      send_fc_energy_since_last_purge((uint32_t) val);
    
      //TOTAL_ENERGY
      val = parse_csv(FC_TOTAL_ENERGY_CSV,dataString);
      send_fc_total_energy((uint32_t) val);

      delay(1);

      //CHARGE_SINCE_LAST_PURGE
      val = parse_csv(FC_CHARGE_SINCE_LAST_PURGE_CSV,dataString);

      //TOTAL_CHARGE
      val = parse_csv(FC_TOTAL_CHARGE_CSV,dataString);

      //FCVOLT
      val = parse_csv(FCVOLT_CSV,dataString);

    //FCCURR
    val = parse_csv(FCCURR_CSV,dataString);

    //FCTEMP
    val = parse_csv(FCTEMP_CSV,dataString);

    //FCPRES
    val = parse_csv(FCPRES_CSV,dataString);

    //CAPVOLT
    val = parse_csv(CAPVOLT_CSV,dataString);

    //FC_FAN_SPEED
    val = parse_csv(FC_FAN_SPEED_CSV,dataString);

    //FC_START_RELAY
    val = parse_csv(FC_START_RELAY_CSV,dataString);

    //FC_RES_RELAY
    val = parse_csv(FC_RES_RELAY_CSV,dataString);

    //FC_CAP_RELAY
    val = parse_csv(FC_CAP_RELAY_CSV,dataString);

    //FC_MOTOR_RELAY
    val = parse_csv(FC_MOTOR_RELAY_CSV,dataString);

    //FC_PURGE_VALVE
    val = parse_csv(FC_PURGE_VALVE_CSV,dataString);

    //FC_H2_VALVE
    val = parse_csv(FC_H2_VALVE_CSV,dataString);
    Serial.println("");
    Serial.print("freeMemory: ");
    Serial.println(freeMemory());
    }
}
