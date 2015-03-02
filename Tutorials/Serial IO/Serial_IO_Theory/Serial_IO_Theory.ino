#define INPUT_DELAY 100 //Specifies how often we check for input
int value = 0; //A variable which is modified by commands from the serial connection

void setup()
{
  //Open the port
  Serial.begin(9600);
  Serial.println("Opened serial connection");

  //Print initial message
  Serial.println("Enter the commands <add> or <sub>");
}

void loop()
{
  //Check if any data has been received
  //If so call the method handleInput() to process it
  if (Serial.available() > 0)
  {
    handleInput();
  }
    
  //This delay dictates how often the device will check for input
  delay(INPUT_DELAY);
}

void handleInput()
{
  //Get the input string
  String input = Serial.readString();

  //Remove any whitespace or CR/LF
  input.trim();

  //Echo the input
  Serial.println("Command: " + input + " received");

  //Process the input
  if (input == "add")
  {
    Serial.println("Adding 1 to the value");
    value++;
  }
  else if (input == "sub")
  {
    Serial.println("Subtracting 1 from the value");
    value--;
  }
  else
  {
    Serial.println("Unknown command: " + input);
  }

  //Echo the changes
  Serial.print("The current value is: ");
  Serial.println(value);
}
