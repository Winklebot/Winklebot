/**************************************************************
  File:      Backup_Drop_Test.pde 
  Contents:  This program executes a number of test sequences to 
             ensure .
             
             if both bumpers sense walls, stop
             
  Notes:    Target: Arduino UNO R1 & R2
            Arduino IDE version: 0022
            
   
  History:
  when      who  what/why
  ----      ---  -------------------------------------------
  03/4/14  DKB  program created
**************************************************************/

/*---------------- Includes ---------------------------------*/
#include <Winklelib.h>
#include <Timers.h>
#include <Servo.h>

/*---------------- Module Defines ---------------------------*/
#define DUMP_ANGLE    90
#define RESET_RAMP    9
#define DUMP_TIME     2500
Servo myservo;

/*---------------- Module Function Prototypes ---------------*/

int pause = 10; // defining the microsecond delay between direction changes
unsigned char TestTimerExpired(void);
void DropCoins(void);

/*---------------- Arduino Main Functions -------------------*/
void setup() {
  Serial.begin(9600);
  Serial.println("Move Forward and Dump has begun! ");
  
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  pinMode(7,INPUT);
  
  myservo.attach(10);
  myservo.write(9);
  WinkleInit();
  //TMRArd_InitTimer(0, TIME_INTERVAL);
  LeftMtrSpeed(4);
  RightMtrSpeed(4);
}

void loop() {
  int LBB = digitalRead(6);
  int RBB = digitalRead(7);
  
  if ((LBB == 0) && (RBB == 0)) {     //both rear bumpers depressed, stop motors
    DropCoins();
    Stop();
  }
  
  else if (LBB == 0) {           //left rear bumper depressed, run only right motor
     LeftMtrSpeed(0);
     RightMtrSpeed(4);
  }
  
  else if (RBB == 0) {           //right rear bumper depressed, run only left motor
     RightMtrSpeed(0);
     LeftMtrSpeed(4);
  }
}

/*---------------- Module Functions -------------------------*/

void DropCoins(void) {
  myservo.write(DUMP_ANGLE);
  delayMicroseconds(DUMP_TIME);
  myservo.write(RESET_RAMP);
}
