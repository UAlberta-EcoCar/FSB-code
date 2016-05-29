//Code for the Front Sensors Board
//March 13, 2016
//Functionality:
    //Detecting "Heart beat" of other boards
    //Logging Data to SD card
    //Transmitting the time from RTC over CAN
    //Possibly Reading Gas and Brake Pedals

#include "fsb_can_handler.h"
#include <rtc.h>
#include <mcp2515_lib.h>
#include <SPI.h>

//Define CAN interrupt pin
#define CAN_INIT 10


//define status leds
#define CAN_STATUS_LED 3
#define SD_STATUS_LED 2

//define pedal input pins
#define BRAKE_INPUT A2
#define PEDAL_INPUT A3


////////////////
//    TIME    //
RTC_Time now; //variable for holding time

#define TIME_WRITE_INTERVAL 250 //how often to send time over CAN bus
uint32_t can_time_timer;

#define PEDAL_DATA_WRITE_INTERVAL 1000 //how often to send pedal data over CAN bus
uint32_t pedal_send_timer;

Can myCan;

String filename;

void setup() {
  //Set up pins
  pinMode(BRAKE_INPUT,INPUT);
  pinMode(PEDAL_INPUT,INPUT);
  pinMode(CAN_STATUS_LED,OUTPUT);
  pinMode(SD_STATUS_LED,OUTPUT);
  
  //Start Serial for debugging
  Serial.begin(9600);
  delay(100);
  
  //Start CAN bus communications
  myCan.begin();
  Serial.println("CAN STARTED");
  
  digitalWrite(CAN_STATUS_LED,HIGH);

  delay(100);
  
  }

uint32_t time_var;

void loop() { 
  
  if(digitalRead(9) == 0)
  {
    digitalWrite(CAN_STATUS_LED,HIGH);
    myCan.read();
    myCan.read();
    digitalWrite(CAN_STATUS_LED,LOW);
  }
}
