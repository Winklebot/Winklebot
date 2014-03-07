/**************************************************************
  File:      Button_Push_Dump_Integration.pde 
  Contents:  Integrates the button pushing sequence and the backup/dump sequence.
            This program moves the bot into the exchange a set number of times,
             then back from the server to the large exchange and drops the chips 
             if both bumpers sense walls.
             
             Initial condition: Back of the bot is facing the server button. Bot is on the
             tape line and reasonably straight. (assumption: this code will pick up after 
             the initial beacon location gets the bot to the tape, and then the focused beacon 
             sensor circuit stops the bot close to straight-on the server)
             
  Notes:    Target: Arduino UNO R1 & R2
            Arduino IDE version: 0022
            
  History:
  when      who  what/why
  ----      ---  -------------------------------------------
  03/6/14  HMK  program created
  
  /*---------------- Includes ---------------------------------*/
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
#define MOVING_TOWARDS_EXCHANGE  301 // servo dumping code
#define DUMPING                  302

// DEFINE TERMS
#define BUMPERHIT                0
#define BUMPEROPEN               1
#define SPEED_SCALER             25 // map 0-255 PWM settings to 0-10 speed settings
//#define TRAVELING_SPEED_LEFT     9
//#define TRAVELING_SPEED_RIGHT    7
#define NOMINAL_TRAVELING_SPEED          8 // Left is speed + 1, right is speed - 1

#define DUMP_ANGLE               90
#define RESET_RAMP               9
#define DUMP_TIME                2500

#define BEACON_INTERRUPT_NUMBER  1 //number, not pin
#define TAPE_INTERRUPT_NUMBER    0 //number, not pin

// DEFINE TIMERS
#define NO_SIGNAL_TIMER              0      //Lucas' timers, to avoid duplicates
#define SEARCHING_FOR_EXCHANGE_TIMER 1
#define MOVING_TOWARDS_TAPE_TIMER    2     
#define BUTTON_PRESS_MILLIS    333
#define BUTTON_PRESS_WAIT_MILLIS 1000  // time of delay that bot is waiting at pushed button before leaving
#define BEACON_INTERRUPT_NUMBER  1
#define TAPE_INTERRUPT_NUMBER    0 
#define DRIVE_DELAY_MILLIS        100
#define STOP_DELAY_MILLIS        100


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

 //available pins: 13, A0-4 (note: pins 9, 0 and 1 should remain unused)

int LFB = BUMPEROPEN;  // set initial bumper reading to open
int RFB = BUMPEROPEN;
int LBB = BUMPEROPEN;
int RBB = BUMPEROPEN;

int coinMax = 3; // total number of button presses needed to get desired coins (2 coins = 3 presses)
                // using "<=" this number for count threshold, so should avoid an off-by-one error

int state = FINDING_SERVER;
int ExchangeButtonCounter = 0;
int dumped = 0;

unsigned char TestTimerExpired(void);
void DropCoins(void);

/*---------------- Module Function Prototypes ---------------*/

void rise_detected();
Servo myservo;

/*---------------- Arduino Main Functions -------------------*/
void setup() {
  Serial.begin(9600);
  Serial.println("The MotorForwardButtonTest program has started!");
  pinMode(L_MOTOR_DIR, OUTPUT);
  pinMode(R_MOTOR_DIR, OUTPUT);
  pinMode(L_MOTOR_EN, OUTPUT);
  pinMode(R_MOTOR_EN, OUTPUT);
  pinMode(pin_Servo, OUTPUT);
  pinMode(LEFT_FRONT_BUMPER, INPUT);
  pinMode(RIGHT_FRONT_BUMPER, INPUT);
  pinMode(LEFT_BACK_BUMPER, INPUT);
  pinMode(RIGHT_BACK_BUMPER, INPUT);
  pinMode(TAPE_INPUT_PIN, INPUT);
  pinMode(BEACON_INPUT_PIN, INPUT);

  myservo.attach(10);
  myservo.write(9);
  
}

void loop() { 
  switch(state) {
    case(MOVING_TOWARDS_TAPE):
//    INSERT LUCAS CODE
      ChangeState(FINDING_SERVER);
      break;
  case(FINDING_SERVER):
//    INSERT LUCAS CODE
      DriveBackwardCorrected(NOMINAL_TRAVELING_SPEED);
      ChangeState(ADJUSTING);
      break;
  case(ADJUSTING):
      CheckBumpers();
     if(LBB != RBB){ // if bumpers are not the same, then only one side hit wall
      if((LBB == BUMPEROPEN) && (RBB == BUMPERHIT)) {  // right back hit, left back not, need to twist left towards wall
      LeftMtrSpeed(-1 * NOMINAL_TRAVELING_SPEED);
      RightMtrSpeed(0);
      }
      else {  // left back hit, right back not, need to twist right towards wall
      RightMtrSpeed(-1 * NOMINAL_TRAVELING_SPEED);
      LeftMtrSpeed(0);
      }
     }
     if((LBB == BUMPERHIT) && (RBB == BUMPERHIT)) {
      Serial.println("PRESSING_SEQUENCE");
      ChangeState(PRESSING_SEQUENCE);
     }
      break;
  case(PRESSING_SEQUENCE) :
     ButtonPressingSequence();
     ChangeState(MOVING_TOWARDS_EXCHANGE);
      break;
  case(MOVING_TOWARDS_EXCHANGE) :
    DriveForwardCorrected(NOMINAL_TRAVELING_SPEED);
    delay(DRIVE_DELAY_MILLIS);
    Stop();
    delay(STOP_DELAY_MILLIS);
    CheckBumpers();
    if ((LFB == BUMPERHIT) && (RFB == BUMPERHIT) && (dumped == 0)) {     //both rear bumpers depressed, stop motors
      dumped = 1;
      DriveForwardCorrected(0);
      DropCoins();
      Serial.println("both bumpers pressed, dumped is ");
      Serial.println(dumped);
      ChangeState(DUMPING);
    }
    else if ((LFB == BUMPERHIT) && (dumped == 0)) {           //left rear bumper depressed, run only right motor
       LeftMtrSpeed(0);
       RightMtrSpeed(NOMINAL_TRAVELING_SPEED);
    }
    else if ((RFB == BUMPERHIT) && (dumped == 0)) {           //right rear bumper depressed, run only left motor
       RightMtrSpeed(0);
       LeftMtrSpeed(NOMINAL_TRAVELING_SPEED);
    }
    break;
      
  }
}

/*---------------- Module Functions -------------------------*/

void DropCoins(void) {
   Serial.println("dropping!");
   myservo.write(90);
   delay(2500);
   myservo.write(9);
}

void ChangeState(int newState){
  Serial.print("Changing state to ");
  Serial.println(newState);  
  state = newState;
  SetMotors(newState);
}


/******************************************************************************
  Function:    ButtonPressingSequence
  Contents:    Runs button pressing sequence
  Parameters:  PRESSING_BUTTON
  Returns:     Nothing
  Notes:    
******************************************************************************/
void ButtonPressingSequence(){
   Serial.println("Button pressing!");
   ExchangeButtonCounter = 1; //button has already been pushed once
   while (ExchangeButtonCounter <= coinMax){
      Serial.println("Driving forward");
      DriveForwardCorrected(NOMINAL_TRAVELING_SPEED);  // drive forward, away from server
      delay(BUTTON_PRESS_MILLIS);
      Serial.println("Driving backward");
      DriveForwardCorrected(0); 
      DriveBackwardCorrected(NOMINAL_TRAVELING_SPEED);
      delay(BUTTON_PRESS_WAIT_MILLIS); // drive into button for twice the time spend driving away, to ensure you make contact
     if(LBB == BUMPERHIT || RBB == BUMPERHIT){
       ExchangeButtonCounter = ExchangeButtonCounter + 1;   
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
  Serial.println(LBB);
  RBB = digitalRead(RIGHT_BACK_BUMPER);
  Serial.println(RBB);
}
    
/******************************************************************************
  Function:    SetMotors
  Contents:    Sets the motors to the correct speed/direction for the given state
  Parameters:  state
  Returns:     Nothing
  Notes:    
******************************************************************************/
void SetMotors(int newState){
   switch(newState) { 
     case(MOVING_TOWARDS_TAPE) :
       DriveBackwardCorrected(NOMINAL_TRAVELING_SPEED);
       break;
    case(FINDING_SERVER):
      DriveBackwardCorrected(NOMINAL_TRAVELING_SPEED);
      break;
    case(PRESSING_SEQUENCE):
      Stop();
      break;
    case(MOVING_TOWARDS_EXCHANGE):
      DriveForwardCorrected(NOMINAL_TRAVELING_SPEED);
      break;
   }
}

/******************************************************************************
  Function:    Drive
  Contents:    Defines the functions for left and right motor, and forward, backwards, stop
  Parameters:  state
  Returns:     Nothing
  Notes:    
******************************************************************************/ 
void DriveBackwardCorrected(char newSpeed){  // Ideal speed in testing was Right Motor 9, left motor 7
	LeftMtrSpeed(-1 * (newSpeed + 1));
	RightMtrSpeed(-1 * (newSpeed - 1));
}
void DriveForwardCorrected(char newSpeed){
	LeftMtrSpeed((newSpeed + 1));
	RightMtrSpeed((newSpeed - 1));
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

