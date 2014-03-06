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
#define L_MOTOR_DIR           8
#define L_MOTOR_EN            6 //NEW bummper now on ANALOG 5
#define R_MOTOR_DIR          12 
#define R_MOTOR_EN           11
#define SPEED_SCALER  25

Servo myservo;


/*---------------- Module Function Prototypes ---------------*/

int pause = 10; // defining the microsecond delay between direction changes
int dumped = 0;
unsigned char TestTimerExpired(void);
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
  myservo.write(9);

//  WinkleInit();
  //TMRArd_InitTimer(0, TIME_INTERVAL);
  DriveForward(5);
}

void loop() {
  int LFB = digitalRead(4);
  int RFB = digitalRead(5);
  
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

/*---------------- Module Functions -------------------------*/

void DropCoins(void) {
   Serial.println("dropping!");
   myservo.write(90);
   delay(2500);
   myservo.write(9);
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

