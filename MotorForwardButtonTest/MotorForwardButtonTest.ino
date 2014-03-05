
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
#define WAITING      1
#define DRIVING_TOWARDS_EXCHANGE 2
#define PRESSING 3
#define DRIVING_TOWARDS_SERVER 4
#define READY_TO_DUMP 5

#define LEFT_FRONT_BUMPER        4
#define RIGHT_FRONT_BUMPER       5
#define LEFT_BACK_BUMPER         6
#define RIGHT_BACK_BUMPER        7

/*---------------- Module Defines ---------------------------*/

/*---------------- Module Function Prototypes ---------------*/
int pause = 10; // defining the microsecond delay between direction changes
unsigned char TestTimerExpired(void);
char forwardspeed = 5;
int ExchangeButtonCounter = 0;
static int state = 1;

/*---------------- Arduino Main Functions -------------------*/
void setup() {
  Serial.begin(9600);
  Serial.println("The MotorForwardButtonTest program has started!");
  WinkleInit();
  //TMRArd_InitTimer(0, TIME_INTERVAL);
}

void loop() {
  switch(state) {
//    case(WAITING) : 
//      if
    case(DRIVING_TOWARDS_EXCHANGE) :
      if(LEFT_FRONT_BUMPER == 1 && RIGHT_FRONT_BUMPER == 1) {  // both front bumpers are unhit
      Serial.println("Moving forward, no bumps");
      DriveForward(forwardspeed);
      }
      if(LEFT_FRONT_BUMPER == 0 && RIGHT_FRONT_BUMPER == 0) {  // both front bumpers hit
      Serial.println("Aligned with exchange");
      DriveForward(0);
      ExchangeButtonCounter = ExchangeButtonCounter + 1;
      state = PRESSING;
      }
//      if(LEFT_FRONT_BUMPER == 1 && RIGHT_FRONT_BUMPER == 0) {  // front right hit, front left not, need to twist
//      Serial.println("Right corner hit")
//      LeftMtrSpeed(forwardspeed);
//      RightMtrSpeed(0);
////      ExchangeButtonCounter = ExchangeButtonCounter + 1;
//      state = ADJUSTING
//      }
//      if(LEFT_FRONT_BUMPER == 0 && RIGHT_FRONT_BUMPER == 1) {  // front left hit, front right not, need to twist
//      Serial.println("Left corner hit")
//      RightMtrSpeed(forwardspeed);
//      LeftMtrSpeed(0);
////      ExchangeButtonCounter = ExchangeButtonCounter + 1;
//      state = ADJUSTING
//      }
      break;
  case(PRESSING) :
    if (ExchangeButtonCounter < 2) {
      DriveForward(-1 * forwardspeed);  // drive in reverse
      delay(20);
      DriveForward(forwardspeed);
      state = DRIVING_TOWARDS_EXCHANGE;
    }
      else DriveForward(-1 * forwardspeed); // backwards to the server
      state = DRIVING_TOWARDS_SERVER;
  case(DRIVING_TOWARDS_SERVER) :
      if(LEFT_BACK_BUMPER == 0 && RIGHT_BACK_BUMPER == 0); {  // both front bumpers hit
      Serial.println("Aligned with server");
      DriveForward(0);
      state = READY_TO_DUMP;
      }
  }
}

/*---------------- Module Functions -------------------------*/


// My bumper hits are registered - coded wrong


