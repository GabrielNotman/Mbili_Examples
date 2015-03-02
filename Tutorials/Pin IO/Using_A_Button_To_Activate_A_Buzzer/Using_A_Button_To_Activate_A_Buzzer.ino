#define BUTTON_PIN 20 //Use digital pin 20 for the button
#define BUZZER_PIN 4 //Use digital pin 4 for the buzzer
int buttonState = 0; 

void setup()
{
    //Set the digital pin modes
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);
}
 
void loop()
{
    //Read the current state of the button
    buttonState = digitalRead(BUTTON_PIN);
    if (buttonState == HIGH) 
    {
        //If the button is pressed, turn the buzzer on
        digitalWrite(BUZZER_PIN, HIGH);
    }
    else 
    {
        //If not, turn the buzzer off
        digitalWrite(BUZZER_PIN, LOW);
    }
}
