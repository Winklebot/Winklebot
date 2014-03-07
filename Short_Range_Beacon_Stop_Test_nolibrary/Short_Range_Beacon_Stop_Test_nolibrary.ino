
/**************************************************************
  File:      Short_Range_Beacon_Stop_Test.ino
  Contents:  Turns until it detects a beacon
             
  Notes:    Target: Arduino UNO R1 & R2
            Arduino IDE version: 0022
            
**************************************************************/

/*---------------- Includes ---------------------------------*/
//#include <Winklelib.h>
#include <Timers.h>
/*---------------- Module Defines ---------------------------*/
#define TAPE_INPUT_PIN 2
#define SHORT_RANGE_BEACON_INPUT_PIN 13
#define BEACON_INTERRUPT_NUMBER 1
#define TAPE_INTERRUPT_NUMBER 0  

#define L_MOTOR_DIR              8
#define L_MOTOR_EN               6 // CHANGED FROM 9 because servo library takes over 9 and 10
#define R_MOTOR_DIR              12   //moved this from 10 to 12 in order to give the pin_Servo 9 or 10	
#define R_MOTOR_EN               11
#define SPEED_SCALER             25  
  
#define SERVER_BEACON_MICROS 1176
#define EXCHANGE_BEACON_MICROS 333
#define NO_SIGNAL_MICROS 2000
#define BEACON_ERROR 100

#define SHORT_RANGE_BEACON_SIGNAL_TIMER 0
#define SHORT_RANGE_BEACON_SIGNAL_TIME 4

#define TURNING_SPEED 4
#define FORWARD_SPEED 9
/*---------------- Module Function Prototypes ---------------*/
void rise_detected();
void UpdateSignal();
boolean CheckSignalPresence();
/*---------------- Module Level Variables -------------------*/
volatile unsigned long previousTime = 0;
volatile unsigned long period = NO_SIGNAL_MICROS; 
volatile boolean risingEdgeFlag = false; 
//boolean beaconFoundFlag = false;
int pause = 4;

/*---------------- Arduino Main Functions -------------------*/
void setup() {
  Serial.begin(9600);
  Serial.println("The Short_Range_Beacon_Stop_Test program has started!");
  pinMode(SHORT_RANGE_BEACON_INPUT_PIN, INPUT);
//  WinkleInit();  
  TMRArd_InitTimer(SHORT_RANGE_BEACON_SIGNAL_TIMER , SHORT_RANGE_BEACON_SIGNAL_TIME);
  RightMtrSpeed(-1 * TURNING_SPEED);
  LeftMtrSpeed(TURNING_SPEED);
  Serial.println("Searching for beacon");
  
}

void loop() {
   
  UpdateSignal();
  boolean inRange = CheckSignalPresence();
  if(inRange){
    Serial.println("Beacon in range - moving forward ");
    LeftMtrSpeed(-1 * FORWARD_SPEED);
    RightMtrSpeed(-1 * FORWARD_SPEED);
    //beaconFoundFlag = false;
    
  }
  
}

/*---------------- Module Functions -------------------------*/


//Resets the timer if a low is read from the beacon - timer should
//never expire when a beacon is in range
void UpdateSignal(){
  int signal = digitalRead(SHORT_RANGE_BEACON_INPUT_PIN);
  //Serial.println(signal);
  // if we have a high then we must have encountered a rising edge so there is still a signal
  if(signal == 1){ 
   // Serial.println("updating signal");
    TMRArd_InitTimer(SHORT_RANGE_BEACON_SIGNAL_TIMER , SHORT_RANGE_BEACON_SIGNAL_TIME);
  }
}

//Check to see if the previous UpdateSignal Timer has expired if so, the beacon
//is out of range, otherwise it is in range
boolean CheckSignalPresence(){
   
  if(TMRArd_IsTimerExpired(SHORT_RANGE_BEACON_SIGNAL_TIMER)){
    // TMRArd_InitTimer(SHORT_RANGE_BEACON_SIGNAL_TIMER , SHORT_RANGE_BEACON_SIGNAL_TIME);
     // Serial.println("out of range");
     return false;
   }
  else{
   // Serial.println("in range");
    return true;
  }
     
}


/******************************************************************************
  Function:    Drive
  Contents:    Defines the functions for left and right motor, and forward, backwards, stop
  Parameters:  state
  Returns:     Nothing
  Notes:    
******************************************************************************/ 
void DriveBackward(char newSpeed){  // Ideal speed in testing was Right Motor 9, left motor 7
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


