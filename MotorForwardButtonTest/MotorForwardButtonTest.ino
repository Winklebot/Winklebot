/**************************************************************
  File:      MotorForwardButtonTest.pde 
  Contents:  This program drives the bot towards the server button
            and drives the bot into the server button, then moves the bot
            back and forth a set number of times to mine desired # of coins
             
  Notes:    Target: Arduino UNO R1 & R2
            Arduino IDE version: 0022
   
  History:
  when      who  what/why
  ----      ---  -------------------------------------------
  02/19/14  ECM  program created for initial motor test
  03/04/14  HMK  used initial motor test code as a basis for button pusher code
  03/05/14  HMK  removed Winklelib.h to debug code more easily. Copy/pasted relevant 
                   parts of library into module functions instead.
**************************************************************/

/*---------------- Includes ---------------------------------*/
//#include <Winklelib.h>
#include <Timers.h>
#include <Servo.h>

/*---------------- Module Defines ---------------------------*/

// DEFINE STATES
#define BACKING_TOWARDS_SERVER   1
#define PRESSING                 2
#define DRIVING_TOWARDS_EXCHANGE 3
#define READY_TO_DUMP            4
#define LEAVING_SERVER           5
#define ADJUSTING                6

// DEFINE TERMS
#define BUMPERHIT                0
#define BUMPEROPEN               1
//#define SERVER_BEACON_MICROS     1176
//#define EXCHANGE_BEACON_MICRO	   333
//#define NO_SIGNAL_MICROS         2000
#define SPEED_SCALER             25 // map 0-255 PWM settings to 0-10 speed settings
//#define BEACON_INTERRUPT_NUMBER  1 //number, not pin
//#define TAPE_INTERRUPT_NUMBER    0 //number, not pin

// DEFINE PINS
#define LEFT_FRONT_BUMPER        4
#define RIGHT_FRONT_BUMPER       5
#define LEFT_BACK_BUMPER         6
#define RIGHT_BACK_BUMPER        7
#define L_MOTOR_DIR              8
#define L_MOTOR_EN               9
#define R_MOTOR_DIR              12   //moved this from 10 to 12 in order to give the pin_Servo 9 or 10	
#define R_MOTOR_EN               11
#define pin_Servo	         10		

 //available pins: 12, all the analog pins (note: pins 0 and 1 should remain unused)

/*---------------- Module Function Prototypes ---------------*/
int state = BACKING_TOWARDS_SERVER;
char botSpeed = 5;
int ExchangeButtonCounter = 0;

int LFB = BUMPEROPEN;  // set initial bumper reading to open
int RFB = BUMPEROPEN;
int LBB = BUMPEROPEN;
int RBB = BUMPEROPEN;

int coinMax = 3; // number of button presses that switch states
//int pause = 10; // defining the microsecond delay between direction changes

/*---------------- Arduino Main Functions -------------------*/
void setup() {
  Serial.begin(9600);
  Serial.println("The MotorForwardButtonTest program has started!");
  pinMode(L_MOTOR_DIR, OUTPUT);
  pinMode(R_MOTOR_DIR, OUTPUT);
  pinMode(L_MOTOR_EN, OUTPUT);
  pinMode(R_MOTOR_EN, OUTPUT);
  pinMode(LEFT_FRONT_BUMPER, INPUT);
  pinMode(RIGHT_FRONT_BUMPER, INPUT);
  pinMode(LEFT_BACK_BUMPER, INPUT);
  pinMode(RIGHT_BACK_BUMPER, INPUT);
  LeftMtrSpeed(-5);
  RightMtrSpeed(-5);
}

void loop() { 
//  switch(state) {
//    CheckLFBumperStatus();
//    CheckRFBumperStatus();
//    CheckLBBumperStatus();
//    CheckRBBumperStatus();
//    case(BACKING_TOWARDS_SERVER):
//      if(LBB == BUMPEROPEN && RBB == BUMPEROPEN) {  // both back bumpers are unhit
// //     Serial.println("Moving back to server, no bumps");
//      DriveBackward(botSpeed);
//  //    LeftMtrSpeed(-5);
//  //    RightMtrSpeed(-5);
//      Serial.println("BACKING_TWOARDS_SERVER");
//  //    state = BACKING_TOWARDS_SERVER;
//      }
//      if(LBB == BUMPERHIT && RBB == BUMPERHIT) {  // both rear bumpers hit
// //     if(LBB == BUMPERHIT || RBB == BUMPERHIT) {  // one of rear bumpers hit
// //     Serial.println("Aligned with exchange");
//      Stop();
// //     DriveForward(0);
//      ExchangeButtonCounter = ExchangeButtonCounter + 1;
//      Serial.println("PRESSING")
//      state = PRESSING;
//      }
////      if(LBB == BUMPEROPEN && RBB == BUMPERHIT) {  // right back hit, left back not, need to twist
////      Serial.println("Right corner hit");
////      LeftMtrSpeed(botSpeed);
////      RightMtrSpeed(0);
////      ExchangeButtonCounter = ExchangeButtonCounter + 1;
////      state = ADJUSTING;
////      }
////      if(LBB == BUMPERHIT && RBB == BUMPEROPEN) {  // left back hit, right back not, need to twist
////      Serial.println("Left corner hit");
////      RightMtrSpeed(botSpeed);
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
//      DriveForward(botSpeed);  // drive forward, away from server
//      delay(20);
//      state = LEAVING_SERVER;
//  case(DRIVING_TOWARDS_EXCHANGE) :
//      if(LFB == BUMPERHIT && RFB == BUMPERHIT){  // both front bumpers hit the Exchange
////      if(LFB == 0 || RFB == 0){  // one of front bumpers hit the Exchange
// //     Serial.println("Hit server");
//      DriveForward(0);
//      Serial.println("READY_TO_DUMP");
//      state = READY_TO_DUMP;
//      }
//   case(LEAVING_SERVER):
//      if (ExchangeButtonCounter < coinMax){ // pushed button enough times to get total coins, head to exchange next
//      delay(500);
//      DriveForward(botSpeed);
//      Serial.println("BACKING_TOWARDS_SERVER");
//      state = BACKING_TOWARDS_SERVER;
//     }
//       else // backwards to the server to get more coins
//      Serial.println("DRIVING_TOWARDS_EXCHANGE");
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

void DriveBackward(char newSpeed){
	LeftMtrSpeed(-1 * newSpeed);
	RightMtrSpeed(-1 * newSpeed);
}
void DriveForward(char newSpeed){
	LeftMtrSpeed(newSpeed);
	RightMtrSpeed(newSpeed);
}

void Stop() {
  LeftMtrSpeed(0);
  RightMtrSpeed(0);
}

void LeftMtrSpeed(char newSpeed) {
//  if ((newSpeed < -10) || (newSpeed > 10)) {
//    return ERR_BADSPEED;
//  }
  if (newSpeed < 0) {
    digitalWrite(L_MOTOR_DIR,LOW); // set the direction to reverse
  } else {
    digitalWrite(L_MOTOR_DIR,HIGH); // set the direction to forward
  }
  analogWrite(L_MOTOR_EN,SPEED_SCALER*abs(newSpeed));
//    return OK_SPEED;
}

void RightMtrSpeed(char newSpeed){
//  if ((newSpeed < -10) || (newSpeed > 10)) {
//    return ERR_BADSPEED;
//  }
  if (newSpeed < 0) {
    digitalWrite(R_MOTOR_DIR,LOW); // set the direction to reverse
  } else {
    digitalWrite(R_MOTOR_DIR,HIGH); // set the direction to forward
  }
  analogWrite(R_MOTOR_EN,SPEED_SCALER*abs(newSpeed));
//    return OK_SPEED;
}

// My bumper hits are registered - coded wrong


