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


Servo myservo;
int SERVO = 10;

void setup() { 
  Serial.begin(9600);
//  Serial.println("Starting ServoTest...");
//  pinMode(SERVO, OUTPUT);
  myservo.attach(10);
}

void loop() {  
  myservo.write(0);  // <--90 when activated
}
      

