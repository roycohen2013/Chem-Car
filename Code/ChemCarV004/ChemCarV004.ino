

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
#define MAG_MIXER_PIN 7
#define SOLUTION_RELEASE_PIN
#define MOTOR_PIN 3
#define LED_ARRAY_PIN 9
#define PHOTOCELL_PIN A2
#define START_BUTTON_PIN 10
#define MOUSE_DATA_PIN 5
#define MOUSE_CLOCK_PIN 6




//------------------------Constants-----------------------------------
#define MAG_MIXER_SPEED_ADDRESS 0
#define LED_ARRAY_BRIGHTNESS_ADDRESS 1 
#define PHOTOCELL_WHITE_VAL_ADDRESS 2
#define PHOTOCELL_BLACK_VAL_ADDRESS 4





PS2 mouse(MOUSE_CLOCK_PIN, MOUSE_DATA_PIN);  // Pin 5 is the mouse data pin, pin 6 is the clock pin mouse(clock, data)

Servo magneticMixer;  // create servo object to control a servo
Servo releaseValve;  // create servo object to control a servo




// chars are used over bytes because they are signed
char mstat;       // this byte contains statuses of buttons and modes on the optical mouse. for specifics go to figure 5.3(status request) in the PAW3402 datasheet
char mx;        // this is the distance on the X axis the mouse traveled since its last data request.(delta X)  Every time the register for X or Y is read from the mouse it erases
char my;        //this is the distance on the Y axis the mouse traveled since its last data request.(delta Y)  Every time the register for X or Y is read from the mouse it erases
byte mResolution;   //the PAW3402 has three resolutions which are 500/800/1000 DPI. This variable is read straight from the chip so it will give you the corresponding values of 1 , 2, or 3
byte mSamplingRate;  // the default sampling rate of the mouse is 100 cycles a second and does not affect us
double  dpi;      //This is the current DPI (dots per inch) of the mouse and is used to convert from the distance in dots to distance in inches.

long distanceTraveledDotsX = 0;          // the distance the vehicle traveled in DPI before any conversion is done
long distanceTraveledDotsY = 0;          // the distance the vehicle traveled in DPI before any conversion is done
double distanceTraveledInchX = 0;          // the distance the vehicle traveled in inches since it was turned on or reset
double distanceTraveledInchY = 0;          // the distance the vehicle traveled in inches since it was turned on or reset

long lastMouseRequest = 0;              //last time the arduino requested dX from the mouse. Timer counts from the second the processor is turned on

int vehicleSpeedX = 0;     //inches/second       // this is the vehicles speed calculated from dX/dt
int vehicleSpeedY = 0;     //inches/second       // this is the vehicles speed calculated from dy/dt


int sensorValue = 0; 

int whiteVal = 150; 
int blackVal = 600;
int reactionThreshold = 400; 


int buttonVal = 0;
int buttonValCounter = 0;
int startButtonState = 0;
int lastButtonState = 0;
boolean buttonPressedFlag = false;



String inputString = "";         // a string to hold incoming data (from user) 
boolean stringComplete = false;  // whether the string is complete (has a carriage return)






byte highLowBytes[] = {
  0,0}; 
int  startInt;

int *PstartInt     = &startInt;
byte *PHighLowBytes = &highLowBytes[0];




/*
 * initialize the mouse. Reset it, and place it into remote
 * mode, so we can get the encoder data on demand.
 */


void setup()
{

  magneticMixer.attach(MAG_MIXER_PIN);
  //myservo.attach(9);


  Serial.begin(9600);

  *PHighLowBytes = *PstartInt;

  //startInt = 0x0304;
  startInt = 700;


  //Serial.print(highLowBytes[0]); 
  //Serial.println(highLowBytes[1]); 


  int byteHigh = 0;
  int byteLow = 0;
  int startingInt= 500;

  Serial.println(startingInt);

  byteHigh = (startingInt >> 8);
  byteLow = (startingInt << 8) >> 8;

  Serial.println(byteHigh);
  Serial.println(byteLow);

  startingInt = byteHigh<<8 | byteLow;

  //startingInt = byteHigh;
  //startingInt = (startingInt << 8) | byteLow; 

  Serial.println(startingInt);






  //delay(10000);


  mouse_init();    // resets the mouse and sets it into remote mode

  requestMouseStatus();  // requests the mouse's resolution and sampling rate in order to let us to know how to convert DPI to inches
  printMouseStatus();

  switch(mResolution){   // this case statment determines the DPI conversion factor by requesting the mouse's resolution for the PAW3402 its three resolutions are 500/800/1000

  case 1:
    dpi = 500;      //mouse resolution value 1
    return;

  case 2:
    dpi = 800;    //mouse resolution value 2
    return;

  case 3:
    dpi = 1000;  //mouse resolution value 3
    return;
  }

  pinMode(START_BUTTON_PIN, INPUT_PULLUP); //enebles the internal 20k pull up resistor for use with the button (HIGH = open) (LOW = pressed) 
  //pinMode(START_BUTTON_PIN, INPUT);

  //fadeInLedArray();  // for cool effect Leds around reaction chamber fade on
  //spinUpMagMixer();  // for cool effect the magnetic stirring motor slowly whirs up making it sound cool


}









void loop()
{
  //analogWrite(LED_ARRAY_PIN, 255);
  //magneticMixer.write(255); //on


  requestMouseData();
  //printMouseData();
 




  buttonVal = digitalRead(START_BUTTON_PIN);

  if(buttonVal == 0){
    buttonValCounter = constrain(buttonValCounter++,0,10);
  } 
  else{
    buttonValCounter = constrain(buttonValCounter--,0,10);
  }

  if (buttonValCounter >= 10){
    startButtonState = 1;
  } 
  else if(buttonValCounter <= 0) { 
    startButtonState = 0;
  }









  if (startButtonState != lastButtonState) {
    Serial.println("state Changed");
    // if the state has changed, increment the counter




    if (startButtonState == 1) {
      // if the current state is HIGH then the button
      // wend from off to on:
      
      if(buttonPressedFlag == true) {
        analogWrite(MOTOR_PIN, 255);
      }

      

      

      
      
      
      Serial.println("on");
    } else {
      magneticMixer.write(255); //on
      analogWrite(LED_ARRAY_PIN, 255);
      buttonPressedFlag = true;
      Serial.println("off");      
    }



  } 



  lastButtonState = startButtonState;
  
    sensorValue = analogRead(PHOTOCELL_PIN);  
    
      if(sensorValue >= reactionThreshold){
        //Serial.print("Sensor Val:  "); 
        //Serial.println(sensorValue); 
      
        magneticMixer.write(100); // OFF
        analogWrite(MOTOR_PIN, 0);  
      }

















  /*
  // compare the buttonState to its previous state
   if (startButtonState != lastButtonState) {
   // if the state has changed, increment the counter
   if (startButtonState == HIGH) {
   // if the current state is HIGH then the button
   // wend from off to on:
   
   Serial.println("on");
   
   
   
   } 
   else {
   // if the current state is LOW then the button
   // wend from on to off:
   Serial.println("off");
   
   }
   }
   // save the current state as the last state, 
   //for next time through the loop
   lastButtonState = startButtonState;
   
   Serial.println("Done");
   
   */













/*

  if (stringComplete) {
    stringComplete = false; // prevent this loop from entering again while we process the command
    inputString.toUpperCase();  // Allows lowercase to also be recognized
    processCommand();
  }
*/




}






//============================================INTERFACE ==================================================

void processCommand(){

  String commandData = inputString.substring(1);    //cuts out everything after the first character and puts it into a new string

  switch (inputString[0])                        // the switch statment runs along the first first character of whatever line is input to it. 
  {
  case '0':

    break;

  case '1':

    break;

  }
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





//==================================================ASTHETICS ==============================================



void fadeInLedArray(){

  for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=5) { 
    // sets the value (range from 0 to 255):
    analogWrite(LED_ARRAY_PIN, fadeValue);         
    // wait for 30 milliseconds to see the dimming effect    
    delay(30);                            
  } 
}

void spinUpMagMixer(){

  for(int fadeValue = 127 ; fadeValue <= 255; fadeValue +=5) { 
    // sets the value (range from 127 to 255):    // it starts from 127 because the servo has the ability to spin in both directions so 127 should be the 0 point between the two directions
    magneticMixer.write(fadeValue);                  // sets the speed of the spinner motor  
    // wait for 30 milliseconds to see the dimming effect    
    delay(30);                            
  }  
}

