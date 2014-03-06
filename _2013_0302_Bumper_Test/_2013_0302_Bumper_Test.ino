// this tests that we are getting a response from a sample switch we found in lab


#include "Arduino.h" 
#include "Winklelib.h"
#include "Timers.h"
#include "Servo.h"

#define LEFT_FRONT_BUMPER        4
#define RIGHT_FRONT_BUMPER       5
#define LEFT_BACK_BUMPER         6
#define RIGHT_BACK_BUMPER        7


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
  int LFB = digitalRead(4);
  if(LFB == 1){
    Serial.println("Left Front Bumper");
    Serial.println(LFB);
  }
  
  int RFB = digitalRead(5);
  if(RFB == 1){
    Serial.println("Right Front Bumper");
    Serial.println(RFB);
  }

  int LBB = digitalRead(6);
  if(LBB == 1){
    Serial.println("Left Back Bumper");
    Serial.println(LBB);
  }

  int RBB = digitalRead(7);
  if(RBB == 1){
    Serial.println("Right Back Bumper");
    Serial.println(RBB);  
  }
}
  

