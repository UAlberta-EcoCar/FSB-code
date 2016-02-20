#include <arduino.h>
#include "FC_USART_CAN_BRIDGE_LIB.h"


long parse_csv(char val,String& dataString)
{  
    //String * dataString = dataStringPointer;
    String subString = "";
    char index = 0;
    for(char x = 1; x < val; x++) //find 10th csv value
    { 
      index = dataString.indexOf(',',index+1); //find first comma in csv
      //Serial.println(index);
    }

    //Serial.println(dataString);
    
    subString = dataString.substring(index+1,dataString.indexOf(',',index+1));
    return(subString.toInt());
}
