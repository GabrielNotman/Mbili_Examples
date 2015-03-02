#define PING_PIN 4 //Use digital pin 4 for the range finder
#define LED_PIN 20 //Use digital pin 20 for the LED
#define LED_LIGHT_DISTANCE 5 //At this distance (in cm) or less the LED is lit up
#define READ_DELAY 100 //Delay between readings in milliseconds

void setup() 
{
  //Start the serial connection
  Serial.begin(9600);
  
  //Set the digital pin modes
  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
  //Set Ping Pin to OUTPUT mode
  pinMode(PING_PIN, OUTPUT);
  
  //Send a HIGH signal for 5 microseconds
  digitalWrite(PING_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(PING_PIN, LOW);

  //Now read back the distance as a time value (microseconds) using the same pin
  pinMode(PING_PIN, INPUT);
  long duration = pulseIn(PING_PIN, HIGH);

  //Convert the time into a distance
  long cm = microsecondsToCentimeters(duration);
  
  //Output the distance to the serial monitor
  Serial.println("Distance in cm: " + String(cm));
  
   if (cm < LED_LIGHT_DISTANCE)
  {
    //If the distance reading is equal to or less than 
    //LED_LIGHT_DISTANCE switch the LED on
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    //If not, turn the LED off
    digitalWrite(LED_PIN, LOW);
  }
  
  //Delay before the next reading
  delay(READ_DELAY);
}

long microsecondsToCentimeters(long microseconds)
{
  //The speed of sound is 340 m/s or 29 microseconds per centimeter
  //The ping travels twice the distance, there and back and so divide by 58
  return microseconds / 58;
}
