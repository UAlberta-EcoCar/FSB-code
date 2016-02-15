//this sketch simulates a node in the CAN network
#include <mcp2515_filter_settings.h>
#include <can_message_defs.h>
#include <mcp2515_lib.h>
#include <mcp2515_settings.h>

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

  if(SD.exists("datalog.txt"))
  {
  }
  else
  {
    File dataFile = SD.open("datalog.txt",FILE_WRITE);
    delay(100);
    dataFile.close();
  }
  
  // Short delay and then begin listening
  delay(2000);
}

CanMessage message;      // Create empty message for sending
String dataString;

void loop() 
{
    dataString = "";

    dataString = Serial.readStringUntil('\n'); //note this has to be single quotes
    //it is a character -> double " is a string
  
    // Open a File for writing to
    File dataFile = SD.open("datalog.txt", FILE_WRITE);   // Open file in SD card for writing
    delayMicroseconds(10); // Let SD finish creating the file
    // Write strings to SD card  
    if (dataFile) { // if the file is open, perform the write commands
      dataFile.print(dataString);       // Write to opened file then close it
      dataFile.close();
    }
    else {      // if the file isn't open, pop up an error:
      digitalWrite(SD_STATUS_LED,LOW);
    }

}
