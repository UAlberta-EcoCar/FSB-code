//I quit attempting to can the FCC can bus module to work
//instead, this sketch will link the CAN-BUS to the FCC usart module

//to do:
//rumman's new message id system
//implement two way communication 


#include <mcp2515_filter_settings.h>
#include <can_message_defs.h>
#include <mcp2515_lib.h>
#include <mcp2515_settings.h>
#include "FC_USART_CAN_BRIDGE_LIB.h"

#include <SPI.h>
#include <SD.h>

const int CS = 10;
const int CAN_STATUS_LED = 2;
const int SD_STATUS_LED  = 3;


void setup() {
  pinMode(CAN_STATUS_LED,OUTPUT);
  pinMode(SD_STATUS_LED,OUTPUT);
  
  // Start Serial Communication
  Serial.begin(115200);
  while (!Serial);

  Serial.setTimeout(500);
  
  // Initialize CAN Controller
  if (can_init())
  {
    while(1); //hang up program
  }
  digitalWrite(CAN_STATUS_LED,HIGH);
  
  //Initialize SD Communication
  // Retry initializing until successful
  while (!SD.begin(CS)){
    delay(1000);
  }
  digitalWrite(SD_STATUS_LED,HIGH);

  delay(100);

  digitalWrite(SD_STATUS_LED,LOW);

  if(SD.exists("datalog.txt"))
  {
  }
  else
  {
    File dataFile = SD.open("datalog.txt",FILE_WRITE);
    delay(100);
    dataFile.write("\n\n\n\n\n\n\n");
    dataFile.close();
  }
  
  // Short delay and then begin listening
  delay(2000);
}

CanMessage message;      // Create empty message for sending
String dataString;
String subString;

unsigned char index;

long long val;

void loop() 
{
    dataString = "";
    dataString = Serial.readStringUntil('\n'); //note this has to be single quotes
    //it is a character -> double " is a string

    if(dataString == "")
    {
      
    }
    else
    {
    digitalWrite(SD_STATUS_LED,HIGH);
    // Open a File for writing to
    File dataFile = SD.open("datalog.txt", FILE_WRITE);   // Open file in SD card for writing
    delayMicroseconds(10); // Let SD finish creating the file
    // Write strings to SD card  
    if (dataFile) { // if the file is open, perform the write commands
      dataFile.print(dataString);       // Write to opened file then close it
      dataFile.close();
      delayMicroseconds(10);
      digitalWrite(SD_STATUS_LED,LOW);
    }
    else {      // if the file isn't open, pop up an error:
    }

    //run through everything and send over CAN-BUS

    //FC_ERROR
    val = parse_csv(FC_ERROR_CSV,dataString);

    //FC_STATE
    val = parse_csv(FC_STATE_CSV,dataString);

    //PURGE_COUNT
    val = parse_csv(PURGE_COUNT_CSV,dataString);

    //TIME_BETWEEN_LAST_PURGES
    val = parse_csv(TIME_BETWEEN_LAST_PURGES_CSV,dataString);

    //ENERGY_SINCE_LAST_PURGE
    val = parse_csv(ENERGY_SINCE_LAST_PURGE_CSV,dataString);
    
    //TOTAL_ENERGY
    val = parse_csv(TOTAL_ENERGY_CSV,dataString);

    //CHARGE_SINCE_LAST_PURGE
    val = parse_csv(CHARGE_SINCE_LAST_PURGE_CSV,dataString);

    //TOTAL_CHARGE
    val = parse_csv(TOTAL_CHARGE_CSV,dataString);

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
    
    
    }
}
