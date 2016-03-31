#include "arduino.h"
#include "rtc.h"
#include "Wire.h" //I2C communication for RTC

//RTC I2C address
#define DS1307_ADDRESS 0x68
//Not sure what issue #527 is
byte zero = 0x00; //workaround for issue #527 

void connect_to_rtc(void)
{
  //START I2C for RTC
  Wire.begin();
}

byte decToBcd(byte val){
// Convert normal decimal numbers to binary coded decimal
  return ( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val)  {
// Convert binary coded decimal to normal decimal numbers
  return ( (val/16*10) + (val%16) );
}

RTC_Time getTime(){
  RTC_Time now; //make struct
  // Reset the register pointer
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();
  delay(10);
  Wire.requestFrom(DS1307_ADDRESS, 7);
  delay(10);
  now.second = bcdToDec(Wire.read());
  now.minute = bcdToDec(Wire.read());
  now.hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  now.weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  now.monthDay = bcdToDec(Wire.read());
  now.month = bcdToDec(Wire.read());
  now.year = bcdToDec(Wire.read());

  //check data
  if(now.second > 60)
  {
    now.second = 99;
  }
  if(now.minute > 60)
  {
    now.minute = 99;
  }
  if(now.hour > 24)
  {
    now.hour = 99;
  }
  if(now.monthDay > 31)
  {
    now.monthDay = 99;
  }
  return(now);
}
