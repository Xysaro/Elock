#include <Adafruit_DotStar.h>
#include <Servo.h>
#include <Bounce2.h>

#define NUMPIXELS 1 
#define DATAPIN    7
#define CLOCKPIN   8

const int Button = 2;

byte Prevser1Data = 0;
int ButtonState = LOW;

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

Adafruit_DotStar pixel(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
Servo LockS;
Bounce debouncer = Bounce(); 

void setup() {

  //LockS.attach(0);
  pixel.begin(); // Initialize pins for output
  pixel.setBrightness(100);
  pixel.show();
  pinMode(Button,INPUT_PULLUP);
  digitalWrite(Button, HIGH);

  inputString.reserve(200);

  debouncer.attach(Button);
  debouncer.interval(5); // interval in ms

  Serial1.begin(57600);
  
  Serial.begin(57600);
  
  LockS.write(0);  

}

void loop() {

  debouncer.update();

  serialEvent();
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.print("unlock");
    Serial.print("lock");
    Serial.println(inputString);
    
    if(inputString == "Unlock"){ 
      
      lockoff();
    }
    
    if(inputString == "Lock"){ 
    lockon();
    }else{
      lockon();
      Serial1.println("string wasn't correct");
    }
    
    
    
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  }


  if (debouncer.fell())ButtonState = !ButtonState;



  if (debouncer.fell() && ButtonState == 0){

    lockoff();
   
  }

  if (debouncer.fell() && ButtonState == 1){

    lockon();

  }

}

void lockon(){
  
 LockS.attach(0);
  LockS.write(85);
  pixel.setPixelColor(0,0,255,0);
  pixel.show();
  Serial1.println("Locked");
  LockS.detach();
  
}

void lockoff(){
 LockS.attach(0);
  LockS.write(0);
  pixel.setPixelColor(0,255,0,0);
  pixel.show();
  Serial1.println("Unlocked");
  LockS.detach();
  
}

void serialEvent() {
  while (Serial1.available() && stringComplete == false) {
    // get the new byte:
    char inChar = (char)Serial1.read();
    Serial.println(inChar);
    // add it to the inputString:
    
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n'){ 
      stringComplete = true;
    }else{
    inputString += inChar;
    }
    
  }
}
