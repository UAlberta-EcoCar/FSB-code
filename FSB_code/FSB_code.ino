//Code for the Front Sensors Board
//June 29, 2016

//Functionality:
  //Reading Fuel Cell data off of can bus and sending over serial
//To Do:
  //read other values
  //sd card
  //rtc
  //"heart beat monitoring
  //Message filtering

#include "fsb_can_handler.h"
#include <mcp2515_lib.h>
#include <SPI.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>


//Define CAN interrupt pin
#define CAN_INT 9

//define status leds
#define CAN_RXTX_LED 3
#define CAN_STATUS_LED 2

Can myCan;
uint32_t can_reset_timer;

/*
 * Watchdog Timer Interrupt
 */
ISR(WDT_vect)
{
  if(check_overflow())
  {
    digitalWrite(CAN_STATUS_LED,HIGH);
    fix_overflow_error();
  }
  else
  {
    digitalWrite(CAN_STATUS_LED,LOW);
  }
}

void wdtSetup() {
  cli();
  MCUSR |= 0;
  WDTCSR |= B00011000;
  WDTCSR = B01000111;
  sei();
}

void setup() {
  //Set up pins
  pinMode(CAN_RXTX_LED,OUTPUT);
  pinMode(CAN_STATUS_LED,OUTPUT);

  //Start Serial for debugging
  Serial.begin(115200);
  can_reset_timer = millis();
  delay(1000);
  
  //Start CAN bus communications
  myCan.begin();
  //can_reset_timer = millis();
  digitalWrite(CAN_RXTX_LED,HIGH);
  //wdtSetup();
}

uint32_t time_var;

#define comma Serial.print(',')
#define NL Serial.print('\n');

void loop() { 
  
  if(digitalRead(CAN_INT) == 0)
  {
    digitalWrite(CAN_RXTX_LED,HIGH);
    myCan.read();
    can_reset_timer = millis();
    digitalWrite(CAN_RXTX_LED,LOW);
  }
  
  if(millis() - time_var > 1000)
  {
    Serial.print(myCan.fc_error);comma;
    Serial.print(myCan.fc_state);comma;
    Serial.print(myCan.fc_purge_count);comma;
    Serial.print(myCan.fc_time_between_last_purges);comma;
    Serial.print(myCan.fc_energy_since_last_purge);comma;
    Serial.print(myCan.fc_total_energy);comma;
    Serial.print(myCan.fc_charge_since_last_purge);comma;
    Serial.print(myCan.fc_total_charge);comma;
    Serial.print(myCan.fc_volt);comma;
    Serial.print(myCan.fc_curr);comma;
    Serial.print(myCan.fc_capvolt);comma;
    Serial.print(myCan.fc_temp);comma;
    Serial.print(myCan.fc_opttemp);comma;
    Serial.print(myCan.fc_pres);comma;
    Serial.print(myCan.fc_fan_speed);comma;

    Serial.print(myCan.fc_start_relay);comma;
    Serial.print(myCan.fc_res_relay);comma;
    Serial.print(myCan.fc_cap_relay);comma;
    Serial.print(myCan.fc_motor_relay);comma;
    Serial.print(myCan.fc_purge_valve);comma;
    Serial.print(myCan.fc_h2_valve);NL;

    time_var = millis();
  }
}
