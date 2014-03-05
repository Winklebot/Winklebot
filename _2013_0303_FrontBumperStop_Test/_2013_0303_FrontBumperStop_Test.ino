// this tests that we are getting a response from a sample switch we found in lab


#include "Arduino.h" 
#include "Winklelib.h"
#include "Timers.h"
#include "Servo.h"

#define LEFT_FRONT_BUMPER        4
#define RIGHT_FRONT_BUMPER       5
#define LEFT_BACK_BUMPER         6
#define RIGHT_BACK_BUMPER        7

int LFB = digitalRead(4);
int RFB = digitalRead(5);
int LBB = digitalRead(6);
int RBB = digitalRead(7);

void Stop (); //declared in WinkleLib

void setup() {
  Serial.begin(9600);
  Serial.println("The Bumpers program has started!");
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  pinMode(7,INPUT);
  WinkleInit();
}

void loop() {
  if (LFB == 0 && RFB == 0) {    //check the switches to see which on/off state corresponds to 0 and 1
    Stop();
  }
    
}
  

