//Global constants, variables and objects
#define START_MSG "Running Setup()..."
#define LOOP_DELAY 500
int loopCount = 0;

//Initialisation
void setup() 
{
   Serial.begin(9600);
   Serial.println(START_MSG);
}

//Execution loop
void loop() 
{
   loopCount++;
   Serial.println("Loop() has run " + String(loopCount) + " times");
   delay(LOOP_DELAY);
}
