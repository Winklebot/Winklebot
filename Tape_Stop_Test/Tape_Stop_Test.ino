
/**************************************************************
  File:      TAPE_STOP_TEST.ino
  Contents:  Detects whether the sensor is above a tape
             
  Notes:    Target: Arduino UNO R1 & R2
            Arduino IDE version: 0022
            
**************************************************************/

/*---------------- Includes ---------------------------------*/
#include <Winklelib.h>
/*---------------- Module Defines ---------------------------*/
#define TAPE_INPUT_PIN 2
#define BEACON_INPUT_PIN 3
#define BEACON_INTERRUPT_NUMBER 1
#define TAPE_INTERRUPT_NUMBER 0  

boolean onTape = false;
boolean tapeFlag = false;

/*---------------- Module Function Prototypes ---------------*/
void rise_detected();

/*---------------- Arduino Main Functions -------------------*/
void setup() {
  Serial.begin(9600);
  Serial.println("The Tape_Stop_Test program has started!");
  pinMode(TAPE_INPUT_PIN, INPUT);
  WinkleInit();  
  attachInterrupt(TAPE_INTERRUPT_NUMBER, rise_detected, RISING);
 // attachInterrupt(1, fall_detected, FALLING);
  DriveForward(5);
}

void loop() {
  if(tapeFlag){
    tapeFlag = false;
    if(onTape == true){
      Serial.println("Moving ONTO tape");
    }  
    if(onTape == false){
      Serial.println("Moving OFF OF tape");
    }
  }
  
  
}

/*---------------- Module Functions -------------------------*/

void rise_detected() {
  onTape = true;
  tapeFlag = true;
  DriveForward(0);

}  
  





