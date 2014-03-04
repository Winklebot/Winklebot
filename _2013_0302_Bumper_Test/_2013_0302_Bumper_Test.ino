// this tests that we are getting a response from a sample switch we found in lab


#include "Arduino.h" 
#include "Winklelib.h"
#include "Timers.h"


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
  int val = digitalRead(4);
//  Serial.println("Left Front Bumper");
  Serial.println(val);
  
//  int val = digitalRead(5);
//  Serial.println("Right Front Bumper");
//  Serial.println(val);

//  int val = digitalRead(6);
//  Serial.println("Left Back Bumper");
//  Serial.println(val);

//  int val = digitalRead(7);
//  Serial.println("Right Back Bumper");
//  Serial.println(val);
  
  
}
  

