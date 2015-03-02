#define MOTOR_PIN 4 //Use digital pin 4 for the vibration motor
#define TOUCH_PIN 20  //Use digital pin 20 for the touch sensor

void setup() 
{
  //Set the digital pin modes
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(TOUCH_PIN, INPUT);
}
 
void loop() 
{
  //Read the current state of the touch sensor
  int sensorValue = digitalRead(TOUCH_PIN);
  
  if(sensorValue == HIGH)
  {
    //If the sensor is reading HIGH switch on the vibration motor
    digitalWrite(MOTOR_PIN, HIGH);
  }
  else
  {
    //If not, turn the vibration motor off
    digitalWrite(MOTOR_PIN, LOW);
  }
}
