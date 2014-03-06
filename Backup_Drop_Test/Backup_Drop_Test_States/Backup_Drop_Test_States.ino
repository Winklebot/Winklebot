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
//#include <Winklelib.h>
#include <Timers.h>
#include <Servo.h>

/*---------------- Module Defines ---------------------------*/
#define DUMP_ANGLE    90
#define RESET_RAMP    9
#define DUMP_TIME     2500
#define MOVING_TOWARDS_EXCHANGE 301
#define WAITING_FOR_BOTH_BUMPERS 302
#define WAITING_FOR_LEFT_BUMPER 303
#define WAITING_FOR_RIGHT_BUMPER 304
#define DUMPING 305
Servo myservo;
int state = MOVING_TOWARDS_EXCHANGE;
int dumped = 0;
int LFB = 1;
int RFB = 1;

#define L_MOTOR_DIR           8
#define L_MOTOR_EN            6 //NEW bummper now on ANALOG 5
#define R_MOTOR_DIR          12 
#define R_MOTOR_EN           11
#define SPEED_SCALER         25

#define BUMPERHIT 0
#define BUMPEROPEN 1




/*---------------- Module Function Prototypes ---------------*/
void DropCoins(void);

/*---------------- Arduino Main Functions -------------------*/
void setup() {
  Serial.begin(9600);
  Serial.println("Move Forward and Dump has begun! ");
  
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(10,OUTPUT);
 
  pinMode(L_MOTOR_DIR , OUTPUT);
  pinMode(R_MOTOR_DIR , OUTPUT);
  pinMode(L_MOTOR_EN , OUTPUT);
  pinMode(R_MOTOR_EN , OUTPUT);
  
  myservo.attach(10);
  myservo.write(RESET_RAMP);
  
  DriveForward(5);
}

void loop() {
  CheckBumpers();
  
  switch(state) {
    case(MOVING_TOWARDS_EXCHANGE) :
      if ((LFB == BUMPERHIT) && (RFB == BUMPERHIT) && (dumped == 0)) {     //both rear bumpers depressed, stop motors
        ChangeState(DUMPING);
      }
      else if ((LFB == BUMPERHIT) && (dumped == 0)) {
        ChangeState(WAITING_FOR_RIGHT_BUMPER);
        LeftMtrSpeed(0);
        RightMtrSpeed(5);
      }
      else if ((RFB == BUMPERHIT) && (dumped == 0)) {         //right rear bumper depressed, run only left motor
        ChangeState(WAITING_FOR_LEFT_BUMPER);
        RightMtrSpeed(0);
        LeftMtrSpeed(5);
      } 
      break;
     
     case(WAITING_FOR_LEFT_BUMPER) :
         if ((LFB == BUMPERHIT) && (dumped == 0)) ChangeState(DUMPING);
      break;
     
     case(WAITING_FOR_RIGHT_BUMPER) :
         if ((RFB == BUMPERHIT) && (dumped == 0)) ChangeState(DUMPING);
      break; 
     
     case(DUMPING) :
       DropCoins();
       dumped = 1;
       Serial.println("both bumpers pressed");
       break;
  }
  
  if ((LFB == 0) && (RFB == 0) && (dumped == 0)) {     //both rear bumpers depressed, stop motors
    dumped = 1;
    LeftMtrSpeed(0);
    RightMtrSpeed(0);
    DropCoins();
    Serial.println("both bumpers pressed, dumped is ");
    Serial.println(dumped);
//    myservo.write(DUMP_ANGLE);
//    delayMicroseconds(DUMP_TIME);
//    myservo.write(RESET_RAMP);
  }
  
  else if ((LFB == 0) && (dumped == 0)) {           //left rear bumper depressed, run only right motor
     LeftMtrSpeed(0);
     RightMtrSpeed(5);
  }
  
  else if ((RFB == 0) && (dumped == 0)) {           //right rear bumper depressed, run only left motor
     RightMtrSpeed(0);
     LeftMtrSpeed(5);
  }
}

//waiting for first bumper to hit
//waiting for second bumper to hit
//waiting for both bumpers to hit

/*---------------- Module Functions -------------------------*/

void DropCoins(void) {
   Serial.println("dropping!");
   myservo.write(90);
   delay(2500);
   myservo.write(9);
}

void CheckBumpers(void) {
  LFB = digitalRead(4);
  RFB = digitalRead(5);
}

void ChangeState(int newState){
  Serial.print("Changing state to ");
  Serial.println(newState);  
  state = newState;
  SetMotors(newState);
  SetTimer(newState);
}

void DriveForward(char newSpeed){
	LeftMtrSpeed(newSpeed);
	RightMtrSpeed(newSpeed);
}

void LeftMtrSpeed(char newSpeed) {
  if ((newSpeed < -10) || (newSpeed > 10)) {
//    return ERR_BADSPEED;
  }
  if (newSpeed < 0) {
    digitalWrite(L_MOTOR_DIR,LOW); // set the direction to reverse
  } else {
    digitalWrite(L_MOTOR_DIR,HIGH); // set the direction to forward
  }
  analogWrite(L_MOTOR_EN,SPEED_SCALER*abs(newSpeed));
 //   return OK_SPEED;
}

void RightMtrSpeed(char newSpeed) {
  if ((newSpeed < -10) || (newSpeed > 10)) {
 //   return ERR_BADSPEED;
  }
  if (newSpeed < 0) {
    digitalWrite(R_MOTOR_DIR,LOW); // set the direction to reverse
  } else {
    digitalWrite(R_MOTOR_DIR,HIGH); // set the direction to forward
  }
  analogWrite(R_MOTOR_EN,SPEED_SCALER*abs(newSpeed));
//    return OK_SPEED;
}


void SetMotors(int newState){
   char rightSpeed;
   char leftSpeed;
   switch(newState) {
     // turning front of the bot right, back left to find an exchange 
     case(SEARCHING_FOR_EXCHANGE_LEFT):
       SpinRight(SCANNING_SPEED);
       break;
     case(SEARCHING_FOR_SERVER):
        switch(orientation){
          // orientation is unknown, spinning front right, back left
          case(UNKNOWN_ORIENTATION) 
            SpinRight(SCANNING_SPEED);
            break;
          // orientation has been found, spinning front left, back right to return to server orientation
          case(RIGHT_ORIENTATION) :
          case(LEFT_ORIENTATION) :
            SpinLeft(SCANNING_SPEED);
            break;
        }
       break; 
     case(MOVING_TOWARDS_TAPE) :
       DriveBackward(TRAVELING_SPEED);
       break;
     case(DUMPING) :
       DriveForward(0);
       break;
   }
}
