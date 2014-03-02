//**************************************************************
//  File:      ServoTest.pde 
//  Contents:  This program operates a servo motor
//
//  Notes:    Target: Arduino UNO R1 & R2
//            Arduino IDE version: 0022
//
//  History:
//  when      who  what/why
//  ----      ---  -------------------------------------------
//  03/01/14  HMK  Program created
//  
//**************************************************************/

/*---------------- Includes ---------------------------------*/
//#include <SoftwareServo.h>
#include <Servo.h>

/*---------------- Module Defines ---------------------------*/
Servo myservo;
//#define pinServo 3;

/*---------------- Module Function Prototypes ---------------*/

<<<<<<< HEAD
//unsigned char theKey;
//unsigned char direct;
int SERVO = 9;
//int DIR1 = 12;
//int ENABLE2 = 5;
//int DIR2 = 4;
//void RespToKey(void);

=======
>>>>>>> FETCH_HEAD
/*---------------- Module Level Variables ---------------*/

/*---------------- Arduino Main Functions -------------------*/
void setup() { 
  Serial.begin(9600);
//  Serial.println("Starting ServoTest...");
//  pinMode(SERVO, OUTPUT);
  myservo.attach(9);
}

void loop() {  
  myservo.write(90);
}
      
/*---------------- Module Functions -------------------------*/

