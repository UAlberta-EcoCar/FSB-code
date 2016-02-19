//this sketch logs serial data from our fuel cell controller 
//to an SD card and parses some values and sends them over CAN-BUS

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

long long CAPVOLT;
unsigned int FC_ERROR;

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

    /*

    //find 2nd csv value
    index = 0;
    for(char x = 0; x < 1; x++)
    { 
      index = dataString.indexOf(',',index); //find first comma in csv
    }
    //retrieve value after 1st comma before 2nd
    subString = dataString.substring(index,dataString.indexOf(',',index));
    FC_ERROR = subString.toInt();

    message.id = MESSAGE_FC_ERROR_ID;
    message.length = MESSAGE_FC_ERROR_LENGTH;
    message.MESSAGE_FC_ERROR_DATA = FC_ERROR;

    can_send_message(&message);
    */
    
    //find 10th csv (CAPVOLT)
    index = 0;
    for(char x = 1; x < 12; x++) //find 10th csv value
    { 
      index = dataString.indexOf(',',index+1); //find first comma in csv
      //Serial.println(index);
    }

    //Serial.println(dataString);
    subString = "";
    subString = dataString.substring(index+1,dataString.indexOf(',',index+1));

    //Serial.println(subString);
    CAPVOLT = 0;
    CAPVOLT = subString.toInt();
    Serial.println(subString);
    message.id = 99;
    message.length = 4;
    message.data.s32[0] = CAPVOLT;

    can_send_message(&message);
    }
}
