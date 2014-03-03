// this tests that we are getting a response from a sample switch we found in lab


#include "Arduino.h" 
#include "Winklelib.h"
#include "Timers.h"


#define LEFT_FRONT_BUMPER        4 



void setup() {
  Serial.begin(9600);
  Serial.println("The Bumpers program has started!");
  pinMode(4,INPUT);
  WinkleInit();
}

void loop() {
  int val = digitalRead(4);
  Serial.println(val);
}
  

