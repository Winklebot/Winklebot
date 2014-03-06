
/**************************************************************
  File:      Beacon_Orientation_Test.ino
  Contents:  Turns until it detects a server beacon
             
  Notes:    Target: Arduino UNO R1 & R2
            Arduino IDE version: 0022
            
**************************************************************/

/*---------------- Includes ---------------------------------*/
#include <Winklelib.h>
#include <Timers.h>
/*---------------- Module Defines ---------------------------*/
#define TAPE_INPUT_PIN 2
#define BEACON_INPUT_PIN 3
#define BEACON_INTERRUPT_NUMBER 1
#define TAPE_INTERRUPT_NUMBER 0  
  
#define SERVER_BEACON_MICROS 1176
#define EXCHANGE_BEACON_MICROS 333
#define NO_SIGNAL_MICROS 2000
#define BEACON_THRESHOLD 20

#define NO_SIGNAL_TIMER 0
#define SEARCHING_FOR_EXCHANGE_TIMER 1
#define MOVING_TOWARDS_TAPE_TIMER 2

#define SEARCHING_FOR_EXCHANGE_MILLIS 100
#define MOVING_TOWARDS_TAPE_MILLIS 1000
#define SCANNING_BRAKE_MICROS 10000 //micros

#define RIGHT_ORIENTATION 0
#define LEFT_ORIENTATION 1
#define UNKNOWN_ORIENTATION -1

#define SEARCHING_FOR_SERVER 0
#define SEARCHING_FOR_EXCHANGE_LEFT 1
#define MOVING_TOWARDS_TAPE 3

#define SCANNING_SPEED 4
#define TRAVELING_SPEED 4
#define SCANNING_RIGHT_BRAKING_SPEED 10
#define SCANNING_LEFT_BRAKING_SPEED -10



int state = SEARCHING_FOR_SERVER;
/*---------------- Module Function Prototypes ---------------*/
void rise_detected();
/*---------------- Module Level Variables -------------------*/
volatile unsigned long previousTime = 0;
volatile unsigned long period = NO_SIGNAL_MICROS; 
volatile boolean risingEdgeFlag = false; 
boolean beaconFoundFlag = false;
int orientation = UNKNOWN_ORIENTATION;

/*---------------- Arduino Main Functions -------------------*/
void setup() {
  Serial.begin(9600);
  Serial.println("The Startin_Orientation_Test program has started!");
  pinMode(TAPE_INPUT_PIN, INPUT);
  WinkleInit();  
  TMRArd_InitTimer(NO_SIGNAL_TIMER, 2);//millis
  attachInterrupt(BEACON_INTERRUPT_NUMBER, rise_detected, RISING);
  ChangeState(SEARCHING_FOR_SERVER);
 
 // attachInterrupt(1, fall_detected, FALLING);
  
}

void loop() {
  CheckSignalPresence();
  unsigned long currentPeriod = period;
  switch(state) {
    case(SEARCHING_FOR_SERVER) :
      if(currentPeriod < SERVER_BEACON_MICROS + BEACON_THRESHOLD && currentPeriod > SERVER_BEACON_MICROS - BEACON_THRESHOLD ){
        Serial.println("Server found!");
        switch(orientation){
          case(UNKNOWN_ORIENTATION) :
            Serial.println("Determining Bot orientation - searching towards the LEFT");
            ChangeState(SEARCHING_FOR_EXCHANGE_LEFT);
            break;
          case(RIGHT_ORIENTATION) :
            Serial.println("Bot oriented with server - moving towards tape in RIGHT orientation ");
            ChangeState(MOVING_TOWARDS_TAPE);
            break;
          case(LEFT_ORIENTATION) :
            Serial.println("Bot oriented with server - moving towards tape in LEFT orientation ");
            ChangeState(MOVING_TOWARDS_TAPE);
            break;
        }
        
      }
      break;
    case(SEARCHING_FOR_EXCHANGE_LEFT) :
      if(currentPeriod < EXCHANGE_BEACON_MICROS + BEACON_THRESHOLD && currentPeriod > EXCHANGE_BEACON_MICROS - BEACON_THRESHOLD ){
        Serial.println("Exchange found to the left of server - Bot in LEFT orientation- Returning to Server orientation");
        orientation = LEFT_ORIENTATION;
        ChangeState(SEARCHING_FOR_SERVER);
      }
      else if(TMRArd_IsTimerExpired(SEARCHING_FOR_EXCHANGE_TIMER)){
        Serial.println("No exchange found to the left of server- Bot in RIGHT orientation- Returning to Server orientation");
        orientation = RIGHT_ORIENTATION;
        ChangeState(SEARCHING_FOR_SERVER);
      }
      break;  
    case(MOVING_TOWARDS_TAPE) :
      if(TMRArd_IsTimerExpired(MOVING_TOWARDS_TAPE_TIMER)){
        Serial.println("Test_Completed- stopping bot");
        LeftMtrSpeed(0);
        RightMtrSpeed(0); 
      }
      break;     
  }
        
}

/*---------------- Module Functions -------------------------*/

void rise_detected() {
  unsigned long currentTime = micros(); 
  period = currentTime - previousTime;
  previousTime = currentTime; 
  risingEdgeFlag = true;

}  
void CheckSignalPresence(){
   if(TMRArd_IsTimerExpired(NO_SIGNAL_TIMER)){
    if(!risingEdgeFlag){
      period = NO_SIGNAL_MICROS;
    }
    risingEdgeFlag = false;
    TMRArd_InitTimer(NO_SIGNAL_TIMER, 2);

  }
  
}
  
void ChangeState(int newState){
  Serial.print("Changing state to ");
  Serial.println(newState);  
  state = newState;
  SetMotors(newState);
  SetTimer(newState);
}

void SetTimer(int newState){
  switch(newState){
    case(SEARCHING_FOR_EXCHANGE_LEFT):
      TMRArd_InitTimer(SEARCHING_FOR_EXCHANGE_TIMER, SEARCHING_FOR_EXCHANGE_MILLIS);
      break;
    case(MOVING_TOWARDS_TAPE):
      TMRArd_InitTimer(MOVING_TOWARDS_TAPE_TIMER, MOVING_TOWARDS_TAPE_MILLIS);
      break;
  }
}


/******************************************************************************
  Function:    SetMotors
  Contents:    Sets the left and right motors to the correct speeds for the given state
  Parameters:  state
  Returns:     Nothing
  Notes:    
******************************************************************************/
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
          case(UNKNOWN_ORIENTATION) :
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
   }
}




