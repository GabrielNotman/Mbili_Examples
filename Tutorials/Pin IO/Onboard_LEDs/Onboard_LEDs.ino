//How long to activate each LED
#define DELAY_TIME 1000

void setup() 
{
  //LED1
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW);
  
  //LED2
  pinMode(LED2, OUTPUT);
  digitalWrite(LED2, LOW);
}

void loop() 
{
  //Switch LED1 on then off again after DELAY_TIME (ms)
  digitalWrite(LED1, HIGH);
  delay(DELAY_TIME);
  digitalWrite(LED1, LOW);
  
  //Repeat for LED2
  digitalWrite(LED2, HIGH);
  delay(DELAY_TIME);
  digitalWrite(LED2, LOW);
}
