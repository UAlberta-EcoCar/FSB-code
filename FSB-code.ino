/*
  SD card datalogger

 This example shows how to log data from three analog sensors
 to an SD card using the SD library.

 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

 created  24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#include <SPI.h>
#include <SD.h>
#include <mcp2515_lib.h>
#include <spi_lib.h>

const int chipSelect = 4;
String file_name = "myfile.txt";

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  spi_init();
  mcp2515_init();

  //make new file
  
}

unsigned long time1 = 0;
unsigned long log_interval = 500;
CanMessage message;

//allocate ram for sensor values
unsigned int sensor1;

void loop()
{
  //loop while constantly updating variables
  message = can_get_message();
  if(message.id == 1)
  {
    sensor1 = message.data[0];
  }
  
  //if log interval hit write to sd card
  if(millis() - time1 > log_interval)
  {
    // make a string for assembling the data to log:
    String dataString = "";
    dataString += sensor1;
    dataString += ",";
    
  
  

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(file_name, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}









