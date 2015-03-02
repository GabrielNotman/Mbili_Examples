#define ROTARY_ANGLE_SENSOR A0 //Use analog pin A0 for the Rotary Angle Sensor
#define ADC_REF 3.3 //Rreference voltage of ADC is 3.3v
#define FULL_ANGLE 300.0 //Full value of the rotary angle is 300 degrees
 
void setup()
{
  //Start the serial connection
  Serial.begin(9600);
}
 
void loop()
{
  //Read the value of the rotary angle sensor in degrees
  int degrees = getDegrees();
  
  //Output it to the serial monitor
  Serial.print("The angle between the mark and the start position: ");
  Serial.println(degrees);
  
  //The delay between readings
  delay(500);
}
 
int getDegrees()
{
  //Read the raw sensor value
  int sensor_value = analogRead(ROTARY_ANGLE_SENSOR);
  
  //Convert the sensor reading to degrees and return that value
  float voltage = (float)sensor_value * ADC_REF / 1023; 
  float degrees = (voltage * FULL_ANGLE) / ADC_REF; 
  return degrees;
}

