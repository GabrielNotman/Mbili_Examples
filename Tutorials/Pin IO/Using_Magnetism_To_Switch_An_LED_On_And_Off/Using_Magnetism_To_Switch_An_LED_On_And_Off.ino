#define MAGNETIC_SWITCH 20 //Use digital pin 20 for the magnetic switch
#define LED_PIN 4 //Use digital pin 4 for the LED
 
void setup()
{
  //Set the digital pin modes
  pinMode(MAGNETIC_SWITCH, INPUT);
  pinMode(LED_PIN,OUTPUT);
}
 
void loop()
{
  //Read the current state of the magnetic switch
  int sensorValue = digitalRead(MAGNETIC_SWITCH);
  
  if(sensorValue == HIGH)
  {
    //If the switch is activated turn the LED on
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    //If not, turn the LED off
    digitalWrite(LED_PIN, LOW);
  }
}
