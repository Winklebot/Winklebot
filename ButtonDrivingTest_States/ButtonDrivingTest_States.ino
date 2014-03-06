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
#define MOVING_TOWARDS_TAPE      3   // from Lucas' "Starting_Orientation_Test" code
#define FINDING_SERVER           101  // using "100" to avoid state overlap with other codes.
#define PRESSING_SEQUENCE        102
#define PREPARING_BUT_PRESS      103
#define ADJUSTING                104
#define COUNTING_BUTTONS         105
#define MOVING_TOWARDS_EXCHANGE  301 // leads to David's servo dumping code

// DEFINE TERMS
#define BUMPERHIT                0
#define BUMPEROPEN               1
#define SPEED_SCALER             25 // map 0-255 PWM settings to 0-10 speed settings
#define TRAVELING_SPEED          4

#define SERVER_BEACON_MICROS     1176
#define EXCHANGE_BEACON_MICRO	 333
#define NO_SIGNAL_MICROS         2000
#define BEACON_INTERRUPT_NUMBER  1 //number, not pin
#define TAPE_INTERRUPT_NUMBER    0 //number, not pin

// DEFINE TIMERS
#define NO_SIGNAL_TIMER              0      //Lucas' timers, to avoid duplicates
#define SEARCHING_FOR_EXCHANGE_TIMER 1
#define MOVING_TOWARDS_TAPE_TIMER    2
#define PREPARING_BUT_PRESS_TIMER     15     
#define PREPARING_BUT_PRESS_MILLIS    500
#define BEACON_INTERRUPT_NUMBER  1
#define TAPE_INTERRUPT_NUMBER    0 

// DEFINE PINS
#define TAPE_INPUT_PIN           2
#define BEACON_INPUT_PIN         3
#define LEFT_FRONT_BUMPER        4
#define RIGHT_FRONT_BUMPER       5
#define LEFT_BACK_BUMPER         A5 // CHANGED FROM 6
#define RIGHT_BACK_BUMPER        7
#define L_MOTOR_DIR              8
#define L_MOTOR_EN               6 // CHANGED FROM 9 because servo library takes over 9 and 10
#define R_MOTOR_DIR              12   //moved this from 10 to 12 in order to give the pin_Servo 9 or 10	
#define R_MOTOR_EN               11
#define pin_Servo	         10		

 //available pins: 12, all the analog pins (note: pins 0 and 1 should remain unused)

int state = PRESSING_SEQUENCE;
int ExchangeButtonCounter = 0;

int LFB = BUMPEROPEN;  // set initial bumper reading to open
int RFB = BUMPEROPEN;
int LBB = BUMPEROPEN;
int RBB = BUMPEROPEN;

int coinMax = 3; // number of button presses that switch states
//int pause = 10; // defining the microsecond delay between direction changes

boolean onTape = false;
boolean tapeFlag = false;

volatile unsigned long previousTime = 0;
volatile unsigned long period = NO_SIGNAL_MICROS; 
volatile boolean risingEdgeFlag = false; 
boolean beaconFoundFlag = false;
int pause = 4;

/*---------------- Module Function Prototypes ---------------*/

void rise_detected();

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
  pinMode(TAPE_INPUT_PIN, INPUT);
  pinMode(BEACON_INPUT_PIN, INPUT);
  attachInterrupt(TAPE_INTERRUPT_NUMBER, rise_detected, RISING);
  attachInterrupt(BEACON_INTERRUPT_NUMBER, rise_detected, RISING);
//  LeftMtrSpeed(-5);
//  RightMtrSpeed(-5);
}

void loop() { 
  switch(state) {
    case(MOVING_TOWARDS_TAPE):
//    INSERT LUCAS CODE
//      if(tapeFlag){
//        tapeFlag = false;
//        if(onTape == true){
//          Stop()
//        }  
//      }
      ChangeState(FINDING_SERVER);
      break;
  case(FINDING_SERVER):
   // INSERT LUCAS CODE
      ChangeState(ADJUSTING);
      break;
  case(ADJUSTING):
     if(LBB != RBB){
      if(LBB == BUMPEROPEN && RBB == BUMPERHIT) {  // right back hit, left back not, need to twist
      LeftMtrSpeed(-1 * TRAVELING_SPEED);
      RightMtrSpeed(0);
      }
      else {  // left back hit, right back not, need to twist
      RightMtrSpeed(-1 * TRAVELING_SPEED);
      LeftMtrSpeed(0);
      }
     }
     else {
      Serial.println("PRESSING_SEQUENCE");
      ChangeState(PRESSING_SEQUENCE);
     }
      break;
  case(PRESSING_SEQUENCE) :
     ButtonPressingSequence();
     ChangeState(MOVING_TOWARDS_EXCHANGE);
      break;
//  case(MOVING_TOWARDS_EXCHANGE) :
//      if(LFB != RFB){
//      ChangeState(ADJUSTING);
//      }
// case(ADJUSTING):
//   if(LFB != RFB){
//     if(LFB == BUMPEROPEN && RFB == BUMPERHIT) {  // right back hit, left back not, need to twist
//      LeftMtrSpeed(TRAVELING_SPEED);
//      RightMtrSpeed(0);
//      }
//      else {  // left back hit, right back not, need to twist
//      Serial.println("Left corner hit");
//      RightMtrSpeed(TRAVELING_SPEED);
//      LeftMtrSpeed(0);
//      }
//      Serial.println("WAITING_TO_DUMP");
//      ChangeState(WAITING_TO_DUMP);
//   }
//      break;
      
  }
}

/*---------------- Module Functions -------------------------*/

// CHECK FOR TAPE
void rise_detected() {
  onTape = true;
  tapeFlag = true;
//  DriveForward(0);
}  

unsigned char CheckButtonTimer(void){
  return (unsigned char)TMRArd_IsTimerExpired(PREPARING_BUT_PRESS_TIMER);
  }  

void ChangeState(int newState){
  Serial.print("Changing state to ");
  Serial.println(newState);  
  state = newState;
  SetMotors(newState);
//  SetTimer(newState);
}

//void SetTimer(int newState){
//  switch(newState){
//    case(PRESSING_BUTTON):
//      TMRArd_InitTimer(PREPARING_BUT_PRESS_TIMER, PREPARING_BUT_PRESS_MILLIS);
//      break;
 
/******************************************************************************
  Function:    ButtonPressingSequence
  Contents:    Runs button pressing sequence
  Parameters:  PRESSING_BUTTON
  Returns:     Nothing
  Notes:    
******************************************************************************/
void ButtonPressingSequence(){
   while (ExchangeButtonCounter < coinMax){
     DriveBackward(TRAVELING_SPEED);
     if(LBB == BUMPERHIT || RBB == BUMPERHIT){
     ExchangeButtonCounter = ExchangeButtonCounter + 1;
     TMRArd_InitTimer(PREPARING_BUT_PRESS_TIMER, PREPARING_BUT_PRESS_MILLIS); 
     DriveForward(TRAVELING_SPEED);  // drive forward, away from server
      if (CheckButtonTimer()){
        TMRArd_StopTimer(PREPARING_BUT_PRESS_TIMER);
        TMRArd_ClearTimerExpired(PREPARING_BUT_PRESS_TIMER);
        DriveBackward(TRAVELING_SPEED);
        }
     }
  }
}
  
/******************************************************************************
  Function:    CheckBumpers
  Contents:    Checks all four bumpers, sets a variable with each
  Parameters:  state
  Returns:     Nothing
  Notes:    
******************************************************************************/
void CheckBumpers(){
  LFB = digitalRead(LEFT_FRONT_BUMPER);
  RFB = digitalRead(RIGHT_FRONT_BUMPER);
  LBB = digitalRead(LEFT_BACK_BUMPER);
  RBB = digitalRead(RIGHT_BACK_BUMPER);
}
    
/******************************************************************************
  Function:    SetMotors
  Contents:    Sets the motors to the correct speed/direction for the given state
  Parameters:  state
  Returns:     Nothing
  Notes:    
******************************************************************************/
void SetMotors(int newState){
//   char rightSpeed;
//   char leftSpeed;
   switch(newState) { 
     case(MOVING_TOWARDS_TAPE) :
       DriveBackward(TRAVELING_SPEED);
       break;
    case(FINDING_SERVER):
      DriveBackward(TRAVELING_SPEED);
      break;
    case(PRESSING_SEQUENCE):
      Stop();
      break;
    case(MOVING_TOWARDS_EXCHANGE):
      DriveForward(TRAVELING_SPEED);
      break;
 //   case(ADJUSTING):
   }
}

/******************************************************************************
  Function:    AdjustOrientation
  Contents:    Straightens bot if hits destination with a corner
  Parameters:  state
  Returns:     Nothing
  Notes:    
******************************************************************************/
//void AdjustOrientation(int newState2){
//   switch(newState2) { 
//    case(BACKING_TOWARDS_SERVER):
//      if(LBB != RBB)
//      ChangeState(ADJUSTING);
//      break;
//    case(MOVING_TOWARDS_EXCHANGE):
//      if(LFB != RFB)
//      ChangeState(ADJUSTING);
//      break;
//   }
//}

/******************************************************************************
  Function:    Drive
  Contents:    Defines the functions for left and right motor, and forward, backwards, stop
  Parameters:  state
  Returns:     Nothing
  Notes:    
******************************************************************************/ 
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
  if (newSpeed < 0) {
    digitalWrite(L_MOTOR_DIR,LOW); // set the direction to reverse
  } else {
    digitalWrite(L_MOTOR_DIR,HIGH); // set the direction to forward
  }
  analogWrite(L_MOTOR_EN,SPEED_SCALER*abs(newSpeed));
}

void RightMtrSpeed(char newSpeed){
  if (newSpeed < 0) {
    digitalWrite(R_MOTOR_DIR,LOW); // set the direction to reverse
  } else {
    digitalWrite(R_MOTOR_DIR,HIGH); // set the direction to forward
  }
  analogWrite(R_MOTOR_EN,SPEED_SCALER*abs(newSpeed));
}
