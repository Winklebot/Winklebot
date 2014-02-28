
/**************************************************************
  File:      Tape_Presence_Detection_Test_Basic.ino
  Contents:  Detects whether the sensor is above a tape
             
  Notes:    Target: Arduino UNO R1 & R2
            Arduino IDE version: 0022
            
**************************************************************/

/*---------------- Includes ---------------------------------*/

boolean onTape = false;
boolean tapeFlag = false;
/*---------------- Module Defines ---------------------------*/
#define TAPE_INPUT_PIN 2

/*---------------- Module Function Prototypes ---------------*/
void rise_detected();
void fall_detected();
/*---------------- Arduino Main Functions -------------------*/
void setup() {
  Serial.begin(9600);
  Serial.println("The Tape_Presence_Detection_Test program has started!");
  pinMode(TAPE_INPUT_PIN, INPUT);
  attachInterrupt(0, rise_detected, RISING);  
 // attachInterrupt(1, fall_detected, FALLING);
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
  tapeFlag = true;
  onTape = true; 
  detachInterrupt(0);
  attachInterrupt(0, fall_detected, FALLING);  

}  
void fall_detected() {
  tapeFlag = true;
  onTape = false;
  detachInterrupt(0);
  attachInterrupt(0, rise_detected, RISING);  
  
}
  





