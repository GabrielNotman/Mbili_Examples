#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

//SODAQ Mbili libraries
#include <RTCTimer.h>
#include <Sodaq_BMP085.h>
#include <Sodaq_SHT2x.h>
#include <Sodaq_DS3231.h>
#include <GPRSbee.h>
#include <Sodaq_PcInt.h>

//The delay in seconds between the sensor readings 
#define READ_DELAY 60

//The sleep length in seconds (MAX 86399)
#define SLEEP_PERIOD 300

//RTC Interrupt pin and period
#define RTC_PIN A7

//Digital pin 11 is the MicroSD slave select pin on the Mbili
#define SD_SS_PIN 11

//The data log file
#define FILE_NAME "DataLog.txt"

//Data header
#define DATA_HEADER "TimeDate, TempSHT21, TempBMP, PressureBMP, HumiditySHT21, Voltage"

//Network constants
#define APN "internet.wind"
#define APN_USERNAME ""
#define APN_PASSWORD ""

//SpeakThings constants
//#define URL "api.thingspeak.com/update"
#define URL "184.106.153.149/update"
#define WRITE_API_KEY "XXXXXXXXXXXXXXXX" //Change to your channel's key

//Seperators
#define FIRST_SEP "?"
#define OTHER_SEP "&"
#define LABEL_DATA_SEP "="

//Data labels, cannot change for ThingSpeak
#define LABEL1 "field1"
#define LABEL2 "field2"
#define LABEL3 "field3"
#define LABEL4 "field4"
#define LABEL5 "field5"

//TPH BMP sensor
Sodaq_BMP085 bmp;

//RTC Timer
RTCTimer timer;

//These constants are used for reading the battery voltage
#define ADC_AREF 3.3
#define BATVOLTPIN A6
#define BATVOLT_R1 4.7
#define BATVOLT_R2 10

void setup() 
{
  //Initialise the serial connection
  Serial.begin(9600);
  Serial.println("Power up...");
  
  //Initialise sensors
  setupSensors();
    
  //Initialise log file
  setupLogFile();
  
  //Setup timer events
  setupTimer();
  
  //Setup GPRSbee
  setupComms();
  
  //Setup sleep mode
  setupSleep();
  
  //Echo the data header to the serial connection
  Serial.println(DATA_HEADER);
  
  //Take first reading immediately
  takeReading(getNow());
}

void loop() 
{
  //Update the timer 
  timer.update();
  
  //Sleep
  systemSleep();  
}

void setupSleep()
{
  pinMode(RTC_PIN, INPUT_PULLUP);
  PcInt::attachInterrupt(RTC_PIN, wakeISR);

  //Setup the RTC in interrupt mode
  //rtc.begin(); //already initialized in setupSensors()
  
  //Set the sleep mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void wakeISR()
{
  //Leave this blank
}

void systemSleep()
{
  Serial.print("Sleeping in low-power mode for ");
  Serial.print(SLEEP_PERIOD/60.0);
  Serial.println(" minutes");
  //This method handles any sensor specific sleep setup
  sensorsSleep();
  
  //Wait until the serial ports have finished transmitting
  Serial.flush();
  Serial1.flush();
  
  //Schedule the next wake up pulse timeStamp + SLEEP_PERIOD
  DateTime wakeTime(getNow() + SLEEP_PERIOD);
  rtc.enableInterrupts(wakeTime.hour(), wakeTime.minute(), wakeTime.second());
  
  //The next timed interrupt will not be sent until this is cleared
  rtc.clearINTStatus();
    
  //Disable ADC
  ADCSRA &= ~_BV(ADEN);
  
  //Sleep time
  noInterrupts();
  sleep_enable();
  interrupts();
  sleep_cpu();
  sleep_disable();
 
  //Enbale ADC
  ADCSRA |= _BV(ADEN);
  
  Serial.println("Waking-up");
  
  //This method handles any sensor specific wake setup
  sensorsWake();
}

void sensorsSleep()
{
  //Add any code which your sensors require before sleep
}

void sensorsWake()
{
  //Add any code which your sensors require after waking
}

void takeReading(uint32_t ts)
{
  //Create the data record
  String dataRec = createDataRecord();
  
  //Save the data record to the log file
  logData(dataRec);
  
  //Echo the data to the serial connection
  Serial.println(dataRec);
  
  //Get the data record as a URL
  String url = createDataURL();
  
  //Send it over the GPRS connection
  sendURLData(url);
}

void setupSensors()
{
  //Initialise the wire protocol for the TPH sensors
  Wire.begin();
  
  //Initialise the TPH BMP sensor
  bmp.begin();

  //Initialise the DS3231 RTC
  rtc.begin();
}

void setupLogFile()
{
  //Initialise the SD card
  if (!SD.begin(SD_SS_PIN))
  {
    Serial.println("Error: SD card failed to initialise or is missing.");
    //Hang
    while (true); 
  }
  
  //Check if the file already exists
  bool oldFile = SD.exists(FILE_NAME);  
  
  //Open the file in write mode
  File logFile = SD.open(FILE_NAME, FILE_WRITE);
  
  //Add header information if the file did not already exist
  if (!oldFile)
  {
    logFile.println(DATA_HEADER);
  }
  
  //Close the file to save it
  logFile.close();  
}

void setupTimer()
{
  //Instruct the RTCTimer how to get the current time reading
  timer.setNowCallback(getNow);

  //Schedule the reading every second
  timer.every(READ_DELAY, takeReading);
}

void setupComms()
{
  //Start Serial1 the Bee port
  Serial1.begin(9600);
  
  //Intialise the GPRSbee
  gprsbee.init(Serial1, BEECTS, BEEDTR);

  //uncomment this line to debug the GPRSbee with the serial monitor
  //gprsbee.setDiag(Serial);
  
  //This is required for the Switched Power method
  gprsbee.setPowerSwitchedOnOff(true); 
}

void logData(String rec)
{
  //Re-open the file
  File logFile = SD.open(FILE_NAME, FILE_WRITE);
  
  //Write the CSV data
  logFile.println(rec);
  
  //Close the file to save it
  logFile.close();  
}

String createDataRecord()
{
  //Create a String type data record in csv format
  //TimeDate, TempSHT21, TempBMP, PressureBMP, HumiditySHT21, Voltage
  String data = getDateTime() + ", ";
  data += String(SHT2x.GetTemperature())  + ", ";
  data += String(bmp.readTemperature()) + ", ";
  data += String(bmp.readPressure() / 100)  + ", ";
  data += String(SHT2x.GetHumidity()) + ", ";
  data += String(getRealBatteryVoltage() * 1000);
  
  return data;
}

String createDataURL()
{
  //Construct data URL
  String url = URL;
 
  //Add key followed by each field
  url += String(FIRST_SEP) + String("key");
  url += String(LABEL_DATA_SEP) + String(WRITE_API_KEY);
  
  url += String(OTHER_SEP) + String(LABEL1);
  url += String(LABEL_DATA_SEP) + String(SHT2x.GetTemperature());
  
  url += String(OTHER_SEP) + String(LABEL2);
  url += String(LABEL_DATA_SEP) + String(bmp.readTemperature());
  
  url += String(OTHER_SEP) + String(LABEL3);
  url += String(LABEL_DATA_SEP) + String(bmp.readPressure() / 100);
  
  url += String(OTHER_SEP) + String(LABEL4);
  url += String(LABEL_DATA_SEP) + String(SHT2x.GetHumidity());
  
  url += String(OTHER_SEP) + String(LABEL5);
  url += String(LABEL_DATA_SEP) + String(getRealBatteryVoltage() * 1000);

  return url;  
}

void sendURLData(String url)
{
  char result[20] = "";
  gprsbee.doHTTPGET(APN, APN_USERNAME, APN_PASSWORD, url.c_str(), result, sizeof(result));
  
  Serial.println("Received: " + String(result));
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

uint32_t getNow()
{
  return rtc.now().getEpoch();
}


float getRealBatteryVoltage()
{
  uint16_t batteryVoltage = analogRead(BATVOLTPIN);
  return (ADC_AREF / 1023.0) * (BATVOLT_R1 + BATVOLT_R2) / BATVOLT_R2 * batteryVoltage;
}
