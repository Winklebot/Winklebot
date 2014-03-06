/**************************************************************
  File:      MotorForwardButtonTest.pde 
  Contents:  This program drives the bot towards the server button
            and drives the bot into the server button, back and forth
             
  Notes:    Target: Arduino UNO R1 & R2
            Arduino IDE version: 0022
   
  History:
  when      who  what/why
  ----      ---  -------------------------------------------
  02/19/14  ECM  program created for initial motor test
**************************************************************/

/*---------------- Includes ---------------------------------*/
#include <Winklelib.h>
#include <Timers.h>
#include <Servo.h>

/*---------------- Module Defines ---------------------------*/
#define BACKING_TOWARDS_SERVER   1
#define PRESSING                 2
#define DRIVING_TOWARDS_EXCHANGE 3
#define READY_TO_DUMP            4
#define LEAVING_SERVER           5
#define ADJUSTING                6

#define BUMPERHIT                0
#define BUMPEROPEN               1

#define LEFT_FRONT_BUMPER        4
#define RIGHT_FRONT_BUMPER       5
#define LEFT_BACK_BUMPER         6
#define RIGHT_BACK_BUMPER        7

/*---------------- Module Function Prototypes ---------------*/
int state = BACKING_TOWARDS_SERVER;
unsigned char TestTimerExpired(void);
char botspeed = 5;
char revbotspeed = -5;
int ExchangeButtonCounter = 0;
int LFB = 1;
int RFB = 1;
int LBB = 1;
int RBB = 1;
int coinmax = 3; // number of button presses that switch states
//int pause = 10; // defining the microsecond delay between direction changes

/*---------------- Arduino Main Functions -------------------*/
void setup() {
  Serial.begin(9600);
  Serial.println("The MotorForwardButtonTest program has started!");
  pinMode(LEFT_FRONT_BUMPER, INPUT);
  pinMode(RIGHT_FRONT_BUMPER, INPUT);
  pinMode(LEFT_BACK_BUMPER, INPUT);
  pinMode(RIGHT_BACK_BUMPER, INPUT);
  WinkleInit();
  LeftMtrSpeed(-5);
  RightMtrSpeed(-5);
  //TMRArd_InitTimer(0, TIME_INTERVAL);
}

void loop() {
//  
//  switch(state) {
//    CheckLFBumperStatus();
//    CheckRFBumperStatus();
//    CheckLBBumperStatus();
//    CheckRBBumperStatus();
//    case(BACKING_TOWARDS_SERVER):
//      if(LBB == BUMPEROPEN && RBB == BUMPEROPEN) {  // both back bumpers are unhit
//      Serial.println("Moving back to server, no bumps");
//      LeftMtrSpeed(-5);
//      RightMtrSpeed(-5);
//  //    state = BACKING_TOWARDS_SERVER;
//      }
//      if(LBB == BUMPERHIT && RBB == BUMPERHIT) {  // both rear bumpers hit
// //     if(LBB == BUMPERHIT || RBB == BUMPERHIT) {  // one of rear bumpers hit
//      Serial.println("Aligned with exchange");
//      DriveForward(0);
//      ExchangeButtonCounter = ExchangeButtonCounter + 1;
//      state = PRESSING;
//      }
////      if(LBB == BUMPEROPEN && RBB == BUMPERHIT) {  // right back hit, left back not, need to twist
////      Serial.println("Right corner hit");
////      LeftMtrSpeed(revbotspeed);
////      RightMtrSpeed(0);
////      ExchangeButtonCounter = ExchangeButtonCounter + 1;
////      state = ADJUSTING;
////      }
////      if(LBB == BUMPERHIT && RBB == BUMPEROPEN) {  // left back hit, right back not, need to twist
////      Serial.println("Left corner hit");
////      RightMtrSpeed(revbotspeed);
////      LeftMtrSpeed(0);
////      ExchangeButtonCounter = ExchangeButtonCounter + 1;
////      state = ADJUSTING;
////      }
////      break;
////  case(ADJUSTING) :
////      if(LBB == BUMPERHIT && RBB == BUMPERHIT) {  // both rear bumpers hit
////      Serial.println("Aligned with exchange");
////      Stop();
////      state = PRESSING;
////      }
//  case(PRESSING) :
//      DriveForward(botspeed);  // drive away from server
//      state = LEAVING_SERVER;
//  case(DRIVING_TOWARDS_EXCHANGE) :
//      if(LFB == BUMPERHIT && RFB == BUMPERHIT){  // both front bumpers hit the Exchange
////      if(LFB == 0 || RFB == 0){  // one of front bumpers hit the Exchange
//      Serial.println("Hit server");
//      DriveForward(0);
//      state = READY_TO_DUMP;
//      }
//   case(LEAVING_SERVER):
//      if (ExchangeButtonCounter < coinmax){ // pushed button enough times to get total coins, head to exchange next
//      delay(500);
//      DriveForward(revbotspeed);
//      state = BACKING_TOWARDS_SERVER;
//     }
//       else // backwards to the server to get more coins
//       state = DRIVING_TOWARDS_EXCHANGE;
//  }
}

/*---------------- Module Functions -------------------------*/

void CheckLFBumperStatus(){
  LFB = digitalRead(LEFT_FRONT_BUMPER);
}

void CheckRFBumperStatus(){
  RFB = digitalRead(RIGHT_FRONT_BUMPER);
}

void CheckLBBumperStatus(){
  LBB = digitalRead(LEFT_BACK_BUMPER);
}

void CheckRBBumperStatus(){
  RBB = digitalRead(RIGHT_BACK_BUMPER);
}

// My bumper hits are registered - coded wrong


