#include <Wire.h>
#include <Sodaq_DS3231.h>
#include <GPRSbee.h>

#define APN "internet"
#define APN_USERNAME ""
#define APN_PASSWORD ""

#define TIME_URL "time.sodaq.net"
#define TIME_ZONE 0.0
#define TIME_ZONE_SEC (TIME_ZONE * 3600)

void setup() 
{
  //Start Serial for serial monitor
  Serial.begin(9600);
  
  //Start Serial1 the Bee port
  Serial1.begin(9600);
  
  //Intialise the GPRSbee
  gprsbee.init(Serial1, BEECTS, BEEDTR);
  
  //Uncomment the line below to debug the GPRSbee with the serial monitor
  //gprsbee.setDiag(Serial);
  
  //This is required for the Switched Power method
  gprsbee.setPowerSwitchedOnOff(true);
  
  //Sync time
  syncRTCwithServer();
  
  //Print out new date/time
  Serial.println(getDateTime());
}

void loop() 
{
}

void syncRTCwithServer()
{
  char buffer[20];
  
  if (gprsbee.doHTTPGET(APN, APN_USERNAME, APN_PASSWORD, TIME_URL, buffer, sizeof(buffer)))
  {
    Serial.println("HTTP GET: " + String(buffer)); 
    
    //Convert the time stamp to unsigned long
    char *ptr;
    uint32_t newTs = strtoul(buffer, &ptr, 0);
    
    //Add the timezone difference plus a few seconds 
    //to compensate for transmission and processing delay
    newTs += 3 + TIME_ZONE_SEC;
    
    //If conversion was successful
    if (ptr != buffer) 
    {
      //Get the old time stamp
      uint32_t oldTs = rtc.now().getEpoch();
      int32_t diffTs = abs(newTs - oldTs);

      //If time is more than 30s off, update
      if (diffTs > 30) 
      {
        //Display old and new time stamps
        Serial.print("Updating RTC, old=" + String(oldTs));
        Serial.println(" new=" + String(newTs));

        //Update the rtc
        rtc.setEpoch(newTs);
      }
    }
  }
}

String getDateTime()
{
  String dateTimeStr;
  
  //Create a DateTime object from the current time
  DateTime dt(rtc.makeDateTime(rtc.now().getEpoch()));

  //Convert it to a String
  dt.addToString(dateTimeStr);
  
  return dateTimeStr;  
}
