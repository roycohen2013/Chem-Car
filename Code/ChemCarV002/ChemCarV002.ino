

#include <ps2.h>                    // did editing to this h file in order to make compatable
#include <Servo.h> 
#include <EEPROM.h>

/* ========TO DO=======

  create auto threshhold code
  
  add communication protocol and UI
  save to eeprom array of photocell_Vals/time
  save to eeprom array of Vehicle_Speed/time
  
  check what comes out of mouse resolution character

 */




/* =====wireless interface=======
     -start competition sequence vehicle at certain competition mode
     -stop competition sequence vehicle 
     -manualy set mixer speed
     -manualy set led array brightness
     -manualy set motor speed
     -manualy set vehicle speed
     -move vehicle a certain distance
     
     
     -save Led array brightness
     -save Mixer speed
     -save Car speed
     
     -Calibrate photocell White Value
     -Calibrate photocell black Value
     
     
     
     -get optical flow sensor status
     -get instant photocell value
     -get total distance traveled
     -get speed/time eeprom graph
     -get photo/time eeprom graph
     -clear speed/time eeprom graph
     -clear photo/time eeprom graph

 */





//------------------------Pins-----------------------------------
#define MAG_MIXER_PIN
#define SOLUTION_RELEASE_PIN
#define MOTOR_PIN 
#define LED_ARRAY_PIN 
#define PHOTOCELL_PIN 
#define START_BUTTON_PIN 

PS2 mouse(6, 5);  // Pin 5 is the mouse data pin, pin 6 is the clock pin


//------------------------Constants-----------------------------------
#define MAG_MIXER_SPEED_ADDRESS 1
#define LED_ARRAY_BRIGHTNESS_ADDRESS 2 
#define PHOTOCELL_WHITE_VAL_ADDRESS 
#define PHOTOCELL_BLACK_VAL_ADDRESS
 
 
 
 
 
 
 Servo magneticMixer;  // create servo object to control a servo
 Servo releaseValve;  // create servo object to control a servo





byte mstat;
byte mx;
byte my;
byte mResolution;
byte mSamplingRate;

long distanceTraveledDots = 0;
long distanceTraveledInch = 0;
long lastMouseRequest = 0;

int vehicleSpeed = 0;


String inputString = "";         // a string to hold incoming data (from user)
boolean stringComplete = false;  // whether the string is complete (has a carriage return)
byte incomingByte = 0;   // for incoming serial data

/*
 * initialize the mouse. Reset it, and place it into remote
 * mode, so we can get the encoder data on demand.
 */
 




void setup()
{
  Serial.begin(9600);
  mouse_init();
  
  requestMouseStatus();
  printMouseStatus();
}

 
 
void loop()
{
  requestMouseData();
  printMouseData();
 
 
   if (stringComplete) {
    stringComplete = false; // prevent this loop from entering again while we process the command
    inputString.toUpperCase();  // Allows lowercase to also be recognized
    processCommand();
  }
  
  
  
  switch(incomingByte){    // look at incomingByte and decide what methods to run depdending on its value
    
    case 1:
      // do somthing here
      break;
    
    
  }
  
  
  //myservo.write(val);                  // sets the servo position according to the scaled value 

  
  
}

void processCommand(){
  
  
}



void serialEvent()
{
    while (Serial.available()) 
    {
      // get the new byte:
      char inChar = (char)Serial.read(); 
      Serial.print(inChar);  // echo it to the screen
      
      inputString += inChar;  // add it to the inputString:
            
      if (inChar == '\n')
        stringComplete = true;  // if the incoming character is a newline, set a flag
  

    } // while
}


