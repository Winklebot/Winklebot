//**************************************************************
//  File:      RampPush.pde 
//  Contents:  When both front bumpers are activated, this program 
//             operates a servo motor in order to lift a ramp.
//
//  Notes:    Target: Arduino UNO R1 & R2
//            Arduino IDE version: 0022
//
//  History:
//  when      who  what/why
//  ----      ---  -------------------------------------------
//  03/03/14  ecm  Program created
//  
//
//**************************************************************/

#include <Winklelib.h>
#include <Servo.h>

#define ONE_SEC            1000


Servo myservo;
int SERVO = 10;

void setup() { 
  Serial.begin(9600);
//  Serial.println("Starting ServoTest...");
//  pinMode(SERVO, OUTPUT);
  myservo.attach(10);
}

//Main Functions
void loop() {  
    if (TestForBump()) RespToBump();
}

//Module Functions
unsigned char TestForBump(void) {
  static unsigned char LastBumper = 0xF0;
  //need to check to make sure all these pins (0xF0) are the correct ones
  unsigned char bumper;
  unsigned char EventOccurred;
  
  bumper = ReadBumpers();
  EventOccurred = ((bumper != 0xF0) && (bumper != LastBumper));
  if (EventOccurred) {
    SET_SHARED_BYTE_TO(bumper);
    LastBumper = bumper;
  }
  return EventOccurred;
}

void RespToBump(void) {
    unsigned char bumper;

    bumper = GET_SHARED_BYTE();

    // display which bumper(s) were hit
    switch (bumper) {
        case (0xD0):  Serial.println("  Front Right..."); break;
        case (0xE0):  Serial.println("  Front Left..."); break;
        case (0x70):  Serial.println("  Back Left..."); break;
        case (0xB0):  Serial.println("  Back Right..."); break;

        case (0xC0):  Serial.println("  Both Front ...");
//both bummpers have to be activated for one second before the ramp tilts
          if {(LFB = 1) && (RFB = 1)
          (ONE_SEC)};    //not sure how to implement this. here or in WinkleLib?
          myservo.write(90);  // <--90 is 'on' - ramp high; 0 is 'off' - ramp low
          break;
          
        case (0x30):  Serial.println("  Both Back..."); break;
        case (0x90):  Serial.println("  Both Right..."); break;
        case (0x60):  Serial.println("  Both Left..."); break;

        default:      Serial.print("  What's this->");
                      Serial.println(bumper,HEX);
    }
}


