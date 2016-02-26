//I quit attempting to can the FCC can bus module to work
//instead, this sketch will link the CAN-BUS to the FCC usart module

//to do:
//rumman's new message id system
//implement two way communication 

#include <MemoryFree.h>
#include <mcp2515_lib.h>
#include <fc_can_messages.h>
#include <mcp2515_filters.h>
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
  while(can_init(EXACT_FILTER_MASK,0,0,EXACT_FILTER_MASK,0,0,0,0)); //will ignore all rx messages
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
  	  send_fc_charge_since_last_purge((uint32_t) val);

      //TOTAL_CHARGE
      val = parse_csv(FC_TOTAL_CHARGE_CSV,dataString);
	    send_fc_total_charge((uint32_t) val);

      delay(1);

      //FCVOLT
      val = parse_csv(FCVOLT_CSV,dataString);
  	  send_fc_volt((int32_t) val);

      //FCCURR
      val = parse_csv(FCCURR_CSV,dataString);
  	  send_fc_curr((int32_t) val);

      delay(1);

      //FCTEMP
      val = parse_csv(FCTEMP_CSV,dataString);
  	  send_fc_temp((int32_t) val);

      //FCPRES
      val = parse_csv(FCPRES_CSV,dataString);
  	  send_fc_pres((int32_t) val);

      delay(1);

      //CAPVOLT
      val = parse_csv(CAPVOLT_CSV,dataString);
  	  send_fc_capvolt((int32_t) val);

      //FC_FAN_SPEED
      val = parse_csv(FC_FAN_SPEED_CSV,dataString);
  	  send_fc_fan_speed((int32_t) val);

      delay(1);

  	  //OUTPUTS
	  
      unsigned char val1 = parse_csv(FC_START_RELAY_CSV,dataString);
      unsigned char val2 = parse_csv(FC_RES_RELAY_CSV,dataString);
      unsigned char val3 = parse_csv(FC_CAP_RELAY_CSV,dataString);
      unsigned char val4 = parse_csv(FC_MOTOR_RELAY_CSV,dataString);
      unsigned char val5 = parse_csv(FC_PURGE_VALVE_CSV,dataString);
      unsigned char val6 = parse_csv(FC_H2_VALVE_CSV,dataString);
      
      if((val1 | val2 | val3 | val4 | val5 | val6) > 1) //if returned value is not a sign bit there is something wrong
      {
        //set all values to 1 to show there is a problem
        val1 = 1;
        val2 = 1;
        val3 = 1;
        val4 = 0; //except motor relay, just in case we use that bit to tell motor it can run.
        val5 = 1;
        val6 = 1;
      }
  	  send_fc_outputs(val1,val2,val3,val4,val5,val6);
      
	  
      Serial.println("");
      Serial.print("freeMemory: ");
      Serial.println(freeMemory());
    }
}
