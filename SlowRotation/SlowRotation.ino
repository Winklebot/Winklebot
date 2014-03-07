
/*---------------- Includes ---------------------------------*/
//#include <Winklelib.h>
#include <Timers.h>
/*---------------- Module Defines ---------------------------*/

//------------------------------------------PINS-----------------------------------------
#define TAPE_INPUT_PIN 2
#define WIDE_RANGE_BEACON_INPUT_PIN 3
#define SHORT_RANGE_BEACON_INPUT_PIN 13
#define LEFT_FRONT_BUMPER        4
#define RIGHT_FRONT_BUMPER       5
#define LEFT_BACK_BUMPER         A5 // CHANGED FROM 6
#define RIGHT_BACK_BUMPER        7
#define L_MOTOR_DIR              8
#define L_MOTOR_EN               6 // CHANGED FROM 9 because servo library takes over 9 and 10
#define R_MOTOR_DIR              12   //moved this from 10 to 12 in order to give the pin_Servo 9 or 10	
#define R_MOTOR_EN               11
#define pin_Servo	         10
#define BEACON_INTERRUPT_NUMBER 1
#define TAPE_INTERRUPT_NUMBER 0  
//------------------------------------------------------------------------------------------
  
//--------------------------------- BEACON SIGNAL TIMES -------------------------------------
#define SERVER_BEACON_MICROS 1176
#define EXCHANGE_BEACON_MICROS 333
#define WIDE_RANGE_NO_SIGNAL_MICROS 2000
#define WIDE_RANGE_NO_SIGNAL_MILLIS 2
#define BEACON_THRESHOLD 50
#define SHORT_RANGE_NO_SIGNAL_MILLIS 4
//--------------------------------------------------------------------------------------------

//-----------------------------------TIMERS -----------------------------------------------
#define WIDE_RANGE_NO_SIGNAL_TIMER 0
#define SEARCHING_FOR_EXCHANGE_TIMER 1
#define SHORT_RANGE_BEACON_SIGNAL_TIMER 3
//------------------------------------------------------------------------------------------

//-----------------------------------MOVING TIMES------------------------------------------
#define SEARCHING_FOR_EXCHANGE_MILLIS 1000
#define SCANNING_BRAKE_MICROS 10000 
#define RIGHT_DISPLACING_ORIENTATION_MILLIS 200
#define LEFT_DISPLACING_ORIENTATION_MILLIS 200
#define SPIN_MILLIS 200
#define PAUSE_MILLIS 1000

//--------------------------------------------------------------------------------------

//---------------------------ORIENTATIONS------------------------------------------
#define RIGHT_ORIENTATION 0
#define LEFT_ORIENTATION 1
#define UNKNOWN_ORIENTATION -1
//--------------------------------------------------------------------------------

//---------------------- STATES -----------------------------------------------
#define SEARCHING_FOR_SERVER_WIDE 0
#define SEARCHING_FOR_EXCHANGE_LEFT 1
#define MOVING_TOWARDS_TAPE 3
#define SEARCHING_FOR_SERVER_SHORT 4
//#define CORRECTING 5
//#define PULSE_DISPLACING 6
#define INITIAL_SEARCHING_FOR_SERVER_WIDE 7
#define SERVER_FOUND  8
#define SCANNING_PAUSE 9
#define SLOW_SEARCH 10

//----------------------------------------------------------------------------

//-----------------------MOTOR SPEEDS -----------------------------------------
#define SCANNING_SPEED 5
#define TRAVELING_SPEED 7
#define SCANNING_RIGHT_BRAKING_SPEED 10
#define SCANNING_LEFT_BRAKING_SPEED -10
#define SPEED_SCALER 25
//-------------------------------------------------------------------------------



/*---------------- Module Function Prototypes ---------------*/

boolean CheckTapePresence();
void TapeRiseDetected();
void WideRangeBeaconRiseDetected();
void UpdateWideRangeSignalPresence();
void ChangeState(int newState);
void SetTimer(int newState);
void SetMotors(int newState);
boolean CheckWideRangeForServer();
boolean CheckWideRangeForExchange();
boolean CheckShortRangeForSignal();
/*---------------- Module Level Variables -------------------*/
volatile unsigned long previousTime = 0;
volatile unsigned long period = WIDE_RANGE_NO_SIGNAL_MICROS; 
volatile boolean risingEdgeFlag = false; 
//boolean beaconFoundFlag = false;
boolean onTape = false;
int orientation = UNKNOWN_ORIENTATION;

int state;
/*---------------- Arduino Main Functions -------------------*/
void setup() {
  Serial.begin(9600);
  Serial.println("The Starting_Orientation_Test program has started!");
  
  Init();  
  TMRArd_InitTimer(WIDE_RANGE_NO_SIGNAL_TIMER, WIDE_RANGE_NO_SIGNAL_MILLIS);//millis
  TMRArd_InitTimer(SHORT_RANGE_BEACON_SIGNAL_TIMER , SHORT_RANGE_NO_SIGNAL_MILLIS);
  attachInterrupt(BEACON_INTERRUPT_NUMBER, WideRangeBeaconRiseDetected, RISING);
  attachInterrupt(TAPE_INTERRUPT_NUMBER, TapeRiseDetected, RISING);
  ChangeState(INITIAL_SEARCHING_FOR_SERVER_WIDE);
}

void loop() {
  UpdateWideRangeSignalPresence();
  UpdateShortRangeSignalPresence();
  unsigned long currentPeriod = period;
  switch(state) {
    // Search for server using wide range beacon from starting position
    case(INITIAL_SEARCHING_FOR_SERVER_WIDE) :
      if(CheckWideRangeForServer()){ //check if looking at a server beacon
        Serial.println("Server found!");
        Serial.println("Determining Bot orientation - searching towards the LEFT");
        ChangeState(SEARCHING_FOR_EXCHANGE_LEFT);
        break;
            
      case(SEARCHING_FOR_EXCHANGE_LEFT) : 
      // found an exchange in this direction - gives you orientation
      if(CheckWideRangeForExchange()){
        Serial.println("Exchange found to the left of server - Bot in LEFT orientation- Returning to Server orientation - turning RIGHT");
        orientation = LEFT_ORIENTATION;
        ChangeState(SEARCHING_FOR_SERVER_WIDE);
      }
      // time ran out and bot didn't find an exchange beacon in this direction - assume the exchange beacon is in the other direction
      else if(TMRArd_IsTimerExpired(SEARCHING_FOR_EXCHANGE_TIMER)){
        TMRArd_StopTimer(SEARCHING_FOR_EXCHANGE_TIMER);
        Serial.println("No exchange found to the left of server- Bot in RIGHT orientation- Returning to Server orientation - turning RIGHT");
        orientation = RIGHT_ORIENTATION;
        ChangeState(SEARCHING_FOR_SERVER_WIDE);
      }
      break; 
            
      case(SEARCHING_FOR_SERVER_WIDE) :
       TMRArd_InitTimer(10, SPIN_MILLIS);
       if(CheckWideRangeForServer() == true) {
        TMRArd_StopTimer(10);
        delay(500);
        Stop();
        ChangeState(SERVER_FOUND);
       }
       if(TMRArd_IsTimerExpired(10)){
        TMRArd_StopTimer(10);
        Stop();
        ChangeState(SCANNING_PAUSE);
       }
       break;
       
       case(SCANNING_PAUSE):
        TMRArd_InitTimer(11, PAUSE_MILLIS);
         if(CheckWideRangeForServer() == true) {
        TMRArd_StopTimer(11);
        ChangeState(SERVER_FOUND);
         }
        else if(TMRArd_IsTimerExpired(11)){
         TMRArd_StopTimer(11); 
         ChangeState(SEARCHING_FOR_SERVER_WIDE);
        break;
        
        case(SERVER_FOUND):
            delay(1000);
            ChangeState(MOVING_TOWARDS_TAPE);
        break;
            }  
            
     case(MOVING_TOWARDS_TAPE) :
       DriveBackwardCorrected(TRAVELING_SPEED);
       break;
        }
      }
}

      
/*---------------- Module Functions -------------------------*/

boolean CheckWideRangeForServer(){
  int currentPeriod = period;
  return currentPeriod < SERVER_BEACON_MICROS + BEACON_THRESHOLD && currentPeriod > SERVER_BEACON_MICROS - BEACON_THRESHOLD;
}
boolean CheckWideRangeForExchange(){
  int currentPeriod = period;
  return currentPeriod < EXCHANGE_BEACON_MICROS + BEACON_THRESHOLD && currentPeriod > EXCHANGE_BEACON_MICROS - BEACON_THRESHOLD;
}
boolean CheckShortRangeForSignal(){
  //Check to see if the previous UpdateSignal Timer has expired if so, the beaconis out of range, otherwise it is in range
  if(TMRArd_IsTimerExpired(SHORT_RANGE_BEACON_SIGNAL_TIMER)){
     return false;
   }
  else{
    return true;
  }
}
boolean CheckTapePresence(){
  return onTape;
}

void UpdateWideRangeSignalPresence(){
    // if there hasn't been a rising edge for some amount of time, there is no signal 
   if(TMRArd_IsTimerExpired(WIDE_RANGE_NO_SIGNAL_TIMER)){//check the signal every so often to see if any rising edge flag has been set since you last checked 
    if(!risingEdgeFlag){
      period = WIDE_RANGE_NO_SIGNAL_MICROS;
    }
    risingEdgeFlag = false;
    TMRArd_InitTimer(WIDE_RANGE_NO_SIGNAL_TIMER, WIDE_RANGE_NO_SIGNAL_MILLIS);

  }
}

void UpdateShortRangeSignalPresence(){
  //Resets the timer if a high is read from the short range sensor
  //timer should never expire when a beacon is in range
  int signal = digitalRead(SHORT_RANGE_BEACON_INPUT_PIN);
  // if we have a high then we must have encountered a rising edge so there is still a signal
  if(signal == 1){ 
    TMRArd_InitTimer(SHORT_RANGE_BEACON_SIGNAL_TIMER , SHORT_RANGE_NO_SIGNAL_MILLIS);
  }
}

  
void ChangeState(int newState){
  Serial.print("Changing state to ");
  Serial.println(newState);  
  state = newState;
  SetMotors(newState);
  SetTimer(newState);
}


void TapeRiseDetected() {
  onTape = true;
  //tapeFlag = true;
}
void WideRangeBeaconRiseDetected() {
  unsigned long currentTime = micros(); 
  period = currentTime - previousTime;
  previousTime = currentTime; 
  risingEdgeFlag = true;

}  


void SetTimer(int newState){
  switch(newState){
    case(SEARCHING_FOR_EXCHANGE_LEFT):
      TMRArd_InitTimer(SEARCHING_FOR_EXCHANGE_TIMER, SEARCHING_FOR_EXCHANGE_MILLIS);
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
       
     case(INITIAL_SEARCHING_FOR_SERVER_WIDE):
          case(UNKNOWN_ORIENTATION) :
            SpinRight(SCANNING_SPEED);
            break;
      
     case(SEARCHING_FOR_SERVER_WIDE):
        switch(orientation){
          case(RIGHT_ORIENTATION) :
            SpinLeft(SCANNING_SPEED); /// ADDED 12:47pm
//            Serial.println("spin left while searching for server wide");
          case(LEFT_ORIENTATION) :
            SpinRight(SCANNING_SPEED);
        }
            break;
            
     case(SEARCHING_FOR_SERVER_SHORT):
        switch(orientation){
          // orientation has been found, spinning front left, back right to return to server orientation
         // case(UNKNOWN_ORIENTATION) :
          //  SpinRight(SCANNING_SPEED);
          case(RIGHT_ORIENTATION) :
            Serial.println("switching to searching for server state- spinning back towards the left!");
            SpinRight(SCANNING_SPEED); //Turn back towards the left to align with server
            break;
          case(LEFT_ORIENTATION) :
            Serial.println("switching to searching for server state- spinning back towards the right!");
            SpinLeft(SCANNING_SPEED); // Turn back towards the right to align with server
            break;
        }
       break;  
     case(MOVING_TOWARDS_TAPE) :
       DriveBackwardCorrected(TRAVELING_SPEED);
       break;
   }
}
      
      
void Init(){
  pinMode(TAPE_INPUT_PIN, INPUT);
  pinMode(L_MOTOR_DIR, OUTPUT);
  pinMode(R_MOTOR_DIR, OUTPUT);
  pinMode(L_MOTOR_EN, OUTPUT);
  pinMode(R_MOTOR_EN, OUTPUT);
  pinMode(LEFT_FRONT_BUMPER, INPUT);
  pinMode(RIGHT_FRONT_BUMPER, INPUT);
  pinMode(LEFT_BACK_BUMPER, INPUT);
  pinMode(RIGHT_BACK_BUMPER, INPUT);
  pinMode(WIDE_RANGE_BEACON_INPUT_PIN, INPUT);
  pinMode(SHORT_RANGE_BEACON_INPUT_PIN, INPUT);
}

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

void SpinRight(char newSpeed){
	LeftMtrSpeed(-1 * newSpeed);
	RightMtrSpeed(1 * newSpeed);
}

void SpinLeft(char newSpeed){
	LeftMtrSpeed(1 * newSpeed);
	RightMtrSpeed(-1 * newSpeed);
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

