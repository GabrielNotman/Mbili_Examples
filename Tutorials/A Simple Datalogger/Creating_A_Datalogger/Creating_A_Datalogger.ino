#include <Wire.h>
#include <SPI.h>
#include <SD.h>

//SODAQ Mbili libraries
#include <Sodaq_BMP085.h>
#include <Sodaq_SHT2x.h>
#include <Sodaq_DS3231.h>

//The delay between the sensor readings
#define READ_DELAY 1000

//Digital pin 11 is the MicroSD slave select pin on the Mbili
#define SD_SS_PIN 11

//The data log file
#define FILE_NAME "DataLog.txt"

//Data header
#define DATA_HEADER "TimeDate, TempSHT21, TempBMP, PressureBMP, HumiditySHT21"

//TPH BMP sensor
Sodaq_BMP085 bmp;

void setup() 
{
  //Initialise the serial connection
  Serial.begin(9600);
  
  //Initialise sensors
  setupSensors();
    
  //Initialise log file
  setupLogFile();
  
  //Echo the data header to the serial connection
  Serial.println(DATA_HEADER);
}

void loop() 
{
  //Create the data record
  String dataRec = createDataRecord();
  
  //Save the data record to the log file
  logData(dataRec);
  
  //Echo the data to the serial connection
  Serial.println(dataRec);
 
  //Wait before taking the next reading
  delay(READ_DELAY);
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
  //TimeDate, TempSHT21, TempBMP, PressureBMP, HumiditySHT21
  String data = getDateTime() + ", ";
  data += String(SHT2x.GetTemperature())  + ", ";
  data += String(bmp.readTemperature()) + ", ";
  data += String(bmp.readPressure() / 100)  + ", ";
  data += String(SHT2x.GetHumidity());
  
  return data;
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
