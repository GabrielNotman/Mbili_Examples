#define LED_PIN 4 //Use digital pin 4 for the LED
#define SENSOR_PIN A4 //Use analog pin A4 for the sensor
#define THRESHOLD_VALUE 50 //Activation threshold
 
void setup() 
{
  //Set the LED digital pin to OUTPUT mode
  pinMode(LED_PIN, OUTPUT); 
}
 
void loop() 
{
  //Read the analog value from the sensor
  int sensorValue = analogRead(SENSOR_PIN);
 
  //Calculate the resistance from the sensor
  float rSensor=(float)(1023-sensorValue)*10 / sensorValue; 
 
  //Compare the calculated resistance against the threshold
  if (rSensor > THRESHOLD_VALUE)
  {
    //If the result is above the threshold, turn the LED on     
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    //If not, turn the LED off
    digitalWrite(LED_PIN, LOW);   
  }
}
