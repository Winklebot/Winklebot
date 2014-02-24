
/**************************************************************
  File:      _0219_MotorsTest_1.pde 
  Contents:  This program executes a number of test sequences to 
             ensure our motor is working.
             
             press 'f': drive forward
             press 'b': drive backwards
             press 'x': stop
             
             press 'fs': drive forward, but slowly
             press 'bs': drive backward, but slowly


             press 'r': turn 90deg right, then drive straight
             press 'l': turn 90deg left, then drive straight
             
             
             if proximity sensor/bumpers sense walls, turn around
             
             do we want to create libraries we can call, rather than
             writing out all the functions?
             
  Notes:    Target: Arduino UNO R1 & R2
            Arduino IDE version: 0022
            
   
  History:
  when      who  what/why
  ----      ---  -------------------------------------------
  02/19/14  ECM  program created for initial motor test
**************************************************************/

/*---------------- Includes ---------------------------------*/
#include <Roachlib.h>

/*---------------- Module Defines ---------------------------*/
#define left_motor_pin       3
#define right_motor_pin      5

/*---------------- Module Function Prototypes ---------------*/
unsigned char TestForKey(void);
void RespToKey(void);

unsigned char TestTimerExpired(void);

/*---------------- Arduino Main Functions -------------------*/
void setup() {
  Serial.begin(9600);
  Serial.println("The MotorsTest_1 program has started!");
  TMRArd_InitTimer(0, TIME_INTERVAL);

}

void loop() {
  if (TestForKey());
    RespToKey();
}

/*---------------- Module Functions -------------------------*/
unsigned char TestForKey(void) {
  unsigned char KeyEventOccurred;
  
  KeyEventOccurred = Serial.available();
  return KeyEventOccurred;
}


void RespToKey(void) {
  unsigned char theKey;
  
  theKey = Serial.read();
  
  if(theKey == 'f') {
    Serial.print(theKey);
    Serial.println("I'm going forward!");
      LeftMtrSpeed(10);
      RightMtrSpeed(10);
  }
  
  else if (theKey == 'b') {
      Serial.print(theKey);
      Serial.println("I'm going backwards!");
      LeftMtrSpeed(-10);
      RightMtrSpeed(-10);
  }
  
  else if (theKey == 'x') {
      Serial.print(theKey);
      Serial.println("I'm stopping!");
      LeftMtrSpeed(0);
      RightMtrSpeed(0);
  }
  
  else if (theKey == 'fs') {
      Serial.print(theKey);
      Serial.println("I'm going forward slowly!");
      LeftMtrSpeed(5);
      RightMtrSpeed(5);
  }
  
  else if (theKey == 'bs') {
      Serial.print(theKey);
      Serial.println("I'm going backwards slowly!");
      LeftMtrSpeed(-5);
      RightMtrSpeed(-5);
  }
  
  else if (theKey == 'l') {
      Serial.print(theKey);
      Serial.println("I'm turning left!");
      //start timer for some amount of time - we need to test to figure out
      //exactly how long it takes our motors to turn 90deg
      LeftMtrSpeed(-5);
      RightMtrSpeed(5);
      //timer expires
      LeftMtrSpeed(10);
      RightMtrSpeed(10);
  }
  
  else if (theKey == 'r') {
      Serial.print(theKey);
      Serial.println("I'm turning right!");
      //start timer for some amount of time - we need to test to figure out
      //exactly how long it takes our motors to turn 90deg
      LeftMtrSpeed(5);
      RightMtrSpeed(-5);
      //timer expires
      LeftMtrSpeed(10);
      RightMtrSpeed(10);
  }
  
}



