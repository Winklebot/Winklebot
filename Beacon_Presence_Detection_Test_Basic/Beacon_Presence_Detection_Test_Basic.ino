
/**************************************************************
  File:      Beacon_Presence_Detection_Test_Basic.ino
  Contents:  Detects whether the sensor is facing a beacon
             
  Notes:    Target: Arduino UNO R1 & R2
            Arduino IDE version: 0022
            
**************************************************************/

/*---------------- Includes ---------------------------------*/

/*---------------- Module Defines ---------------------------*/
#define BEACON_INPUT_PIN 3

/*---------------- Module Function Prototypes ---------------*/
void rise_detected();
void fall_detected();
/*---------------- Arduino Main Functions -------------------*/
void setup() {
  Serial.begin(9600);
  Serial.println("The Beacon_Presence_Detection_Test program has started!");
  pinMode(BEACON_INPUT_PIN, INPUT);
  attachInterrupt(0, rise_detected, RISING);  
  //attachInterrupt(1, fall_detected, FALLING);
}

void loop() {
  
}

/*---------------- Module Functions -------------------------*/

void rise_detected() {
  Serial.println("Beacon in range!");
  //detatch interrupt to avoid calling this function at beacon frequency
  detachInterrupt(0);
  attachInterrupt(1, fall_detected, FALLING);
  
}  
void fall_detected() {
  Serial.println("Beacon out of range!");
  detachInterrupt(1);
  attachInterrupt(0, rise_detected, RISING);  
}
  





