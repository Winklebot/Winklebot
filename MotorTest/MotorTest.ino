//**************************************************************
//  File:      MotorTest.pde 
//  Contents:  This program drives two stepper motors thru 
//    an H-bridge
//    (based on Lab3 Part 4 code)
//  Notes:    Target: Arduino UNO R1 & R2
//            Arduino IDE version: 0022
//
//  History:
//  when      who  what/why
//  ----      ---  -------------------------------------------
//  02/22/14  HMK  Program created
//  
//**************************************************************/

/*---------------- Includes ---------------------------------*/
#include <Pulse.h>

/*---------------- Module Defines ---------------------------*/

/*---------------- Module Function Prototypes ---------------*/

int ENABLE1 = 3;
int DIR1 = 2;
int ENABLE2 = 5;
int DIR2 = 4;
//int potent = 0;
//int POTENT_IN = A1;  //Potentiometer input arduino pin
//void analogRead(void);
//void analogWrite(void);
//void digitalWrite(void);

/*---------------- Module Level Variables ---------------*/

/*---------------- Arduino Main Functions -------------------*/
void setup() { 
  Serial.begin(9600);
//  Serial.println("Starting MotorTest...");
  pinMode(ENABLE1, OUTPUT);
  pinMode(DIR1, OUTPUT);
 // pinMode(ENABLE2, OUTPUT);
//  pinMode(DIR2, OUTPUT);

}

void loop() {  
  
    digitalWrite(DIR1, LOW);
//  digitalWrite(DIR2, HIGH);
//  analogWrite(ENABLE1, 255);
//  analogWrite(ENABLE2, 255);
//  pulseIn(ENABLE1, HIGH, 20);
//  pulseIn(ENABLE2, HIGH, 20);
     
}
      
/*---------------- Module Functions -------------------------*/


