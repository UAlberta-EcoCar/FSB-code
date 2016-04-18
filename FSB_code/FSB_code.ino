//Code for the Front Sensors Board
//March 13, 2016
//Functionality:
    //Detecting "Heart beat" of other boards
    //Logging Data to SD card
    //Transmitting the time from RTC over CAN
    //Possibly Reading Gas and Brake Pedals

#include "fsb_can_handler.h"
#include "rtc.h"
#include <mcp2515_lib.h>
#include <SPI.h>

//Define CAN interrupt pin
#define CAN_INIT 10


//define status leds
#define CAN_STATUS_LED A0
#define SD_STATUS_LED 3

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
  Serial.println("HI");
  //Start CAN bus communications
  myCan.begin();
  Serial.println("CAN STARTED");
  
  //connect to RTC
  connect_to_rtc();
  Serial.println("RTC CONNECTED");

  digitalWrite(CAN_STATUS_LED,HIGH);
  
  
  //Make new log file with time/date as name
  //now = getTime();
  filename = String(now.year) + String(now.month) + String(now.monthDay) + String(now.hour) + String(now.minute) + String(now.second) + ".csv";
}

uint32_t time_var;

void loop() { 
  if(millis() - time_var > 100)
  { 
  time_var = millis();
  Serial.print(myCan.FC_VOLT);
  Serial.print(" ");
  Serial.println(myCan.speed);
  
  }

  
  //send gas and brake pedal readings over can bus
  if((millis() - pedal_send_timer) > PEDAL_DATA_WRITE_INTERVAL)
  {
    myCan.send_throttle(analogRead(PEDAL_INPUT));
    myCan.send_brake(analogRead(BRAKE_INPUT));
    pedal_send_timer = millis();
  }
  
  //send time over can bus
  if(millis() - can_time_timer > TIME_WRITE_INTERVAL)
  {
    //now = getTime();
    myCan.send_time(&now);
    can_time_timer = millis();
  }
  
  if(digitalRead(9) == 0)
  {
    digitalWrite(CAN_STATUS_LED,HIGH);
    myCan.read();
    myCan.read();
    digitalWrite(CAN_STATUS_LED,LOW);
  }
}
