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
#define CAN_INT 9



//define status leds
#define CAN_STATUS_LED 3
#define SD_STATUS_LED 2

//define pedal input pins
#define BRAKE_INPUT A2
#define PEDAL_INPUT A3

bool brake_value;


////////////////
//    TIME    //
RTC_Time now; //variable for holding time

#define TIME_WRITE_INTERVAL 250 //how often to send time over CAN bus
uint32_t can_time_timer;

#define PEDAL_DATA_WRITE_INTERVAL 200 //how often to send pedal data over CAN bus
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
<<<<<<< HEAD

  //connect to RTC
  connect_to_rtc();
  Serial.println("RTC CONNECTED");

  digitalWrite(CAN_STATUS_LED,HIGH);


  //Make new log file with time/date as name
  //now = getTime();
  filename = String(now.year) + String(now.month) + String(now.monthDay) + String(now.hour) + String(now.minute) + String(now.second) + ".csv";
}
=======
  
  digitalWrite(CAN_STATUS_LED,HIGH);

  delay(100);
  
  }
>>>>>>> 056bfdb5306366a4e68a599c359a0391982d068c

uint32_t time_var;
uint16_t SD_STATUS_LED_hold;

<<<<<<< HEAD
void loop() {
  if(millis() - time_var > 100)
  {
  time_var = millis();
  }

  //send gas and brake pedal readings over can bus
  if((millis() - pedal_send_timer) > PEDAL_DATA_WRITE_INTERVAL)
  {
    if (analogRead(BRAKE_INPUT) > 50) {
      brake_value = 1;
      Serial.print("Brake On");
    }
    else {
      brake_value = 0;
      Serial.println("Brake Off");
    }
    myCan.send_throttle(analogRead(PEDAL_INPUT));
    myCan.send_brake(brake_value);

    Serial.print("Brake: ");
    Serial.println(analogRead(BRAKE_INPUT));
    Serial.print("Throttle: ");
    Serial.println(analogRead(PEDAL_INPUT));
    pedal_send_timer = millis();
  }

  //send time over can bus
  if(millis() - can_time_timer > TIME_WRITE_INTERVAL)
  {
    //now = getTime();
    myCan.send_time(&now);
    can_time_timer = millis();
  }

//This needs to be fixed - It seems like as soon as it's triggered once it runs forever
  if(digitalRead(CAN_INT) == 0) //If there was a "message received interrupt" (happy mike?)
  {
    digitalWrite(SD_STATUS_LED, !digitalRead(SD_STATUS_LED));
    Serial.print("Read something");
    Serial.println(millis());
=======
void loop() { 
  
  if(digitalRead(9) == 0)
  {
    digitalWrite(CAN_STATUS_LED,HIGH);
    myCan.read();
    myCan.read();
    digitalWrite(CAN_STATUS_LED,LOW);
>>>>>>> 056bfdb5306366a4e68a599c359a0391982d068c
  }
}
