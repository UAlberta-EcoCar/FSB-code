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
#include <SD.h>

//Define CAN interrupt pin
#define CAN_INIT 10

//define SD card CS pin
#define SD_CS 10

//define status leds
#define CAN_STATUS_LED 2
#define SD_STATUS_LED 3

//define pedal input pins
#define BRAKE_INPUT A0
#define PEDAL_INPUT A1


////////////////
//    TIME    //
RTC_Time now; //variable for holding time

#define SD_WRITE_INTERVAL 200 //how often to log data
uint32_t sd_write_timer;

#define TIME_WRITE_INTERVAL 250 //how often to send time over CAN bus
uint32_t can_time_timer;

#define PEDAL_DATA_WRITE_INTERVAL 20 //how often to send pedal data over CAN bus
uint32_t pedal_send_timer;

//Heart beat timing
#define FCC_PULSE_ALARM_THRES
uint32_t last_fcc_pulse;
uint32_t fcc_pulse;

#define AUX_PULSE_ALARM_THRES
uint32_t last_aux_pulse;
uint32_t aux_pulse;

#define LCD_PULSE_ALARM_THRES
uint32_t last_lcd_pulse;
uint32_t lcd_pulse;

#define MOTOR_PULSE_ALARM_THRES
uint32_t last_motor_pulse;
uint32_t motor_pulse;
///////////////

String filename;

void setup() {
  //Start Serial for debugging
  Serial.begin(9600);
  
  //Start CAN bus communications
  fsb_can_init();
  Serial.println("CAN STARTED");
  
  //connect to RTC
  connect_to_rtc();
  Serial.println("RTC CONNECTED");
  
  delay(500);
  
  //Connect to SD card
  while (0){//!SD.begin(SD_CS)){
    delay(1000);
  }
  //Serial.println("SD CARD CONNECTED");
  Serial.println("NO SD CARD");

  //Make new log file with time/date as name
  now = getTime();
  filename = String(now.year) + String(now.month) + String(now.monthDay) + String(now.hour) + String(now.minute) + String(now.second) + ".csv";
  //File dataFile = SD.open(filename,FILE_WRITE);
  //delay(100);
  //dataFile.close();
  //Serial.println("NEW FILE MADE");
  
  //Set up pins
  pinMode(BRAKE_INPUT,INPUT);
  pinMode(PEDAL_INPUT,INPUT);
}

CanMessage message;

void loop() { 
  //send gas and brake pedal readings over can bus
  if(millis() - pedal_send_timer > PEDAL_DATA_WRITE_INTERVAL)
  {
    digitalWrite(CAN_STATUS_LED,HIGH);
    send_throttle(analogRead(PEDAL_INPUT));
    send_brake(analogRead(BRAKE_INPUT));
    digitalWrite(CAN_STATUS_LED,LOW);
    pedal_send_timer = millis();
  }
  
  //send time over can bus
  if(millis() - can_time_timer > TIME_WRITE_INTERVAL)
  {
    now = getTime();
    digitalWrite(CAN_STATUS_LED,HIGH);
    send_can_time(&now);
    digitalWrite(CAN_STATUS_LED,LOW);
    can_time_timer = millis();
  }
  
  //write to SD card
  if(millis() - sd_write_timer > SD_WRITE_INTERVAL)
  {
    now = getTime();
    digitalWrite(SD_STATUS_LED,HIGH);
    delay(10);
    sd_write_timer = millis();
    digitalWrite(SD_STATUS_LED,LOW);
  }

  //read can bus
  if(digitalRead(9) == 0)
  {
    digitalWrite(CAN_STATUS_LED,HIGH);
    read_can_bus();
    digitalWrite(CAN_STATUS_LED,LOW);
  }
}
